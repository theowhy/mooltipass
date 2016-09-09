#!/usr/bin/env python
#
# Copyright (c) 2014 Darran Hunt (darran [at] hunt dot net dot nz)
# All rights reserved.
#
# CDDL HEADER START
#
# The contents of this file are subject to the terms of the
# Common Development and Distribution License (the "License").
# You may not use this file except in compliance with the License.
#
# You can obtain a copy of the license at src/license_cddl-1.0.txt
# or http://www.opensolaris.org/os/licensing.
# See the License for the specific language governing permissions
# and limitations under the License.
#
# When distributing Covered Code, include this CDDL HEADER in each
# file and include the License file at src/license_cddl-1.0.txt
# If applicable, add the following below this CDDL HEADER, with the
# fields enclosed by brackets "[]" replaced with your own identifying
# information: Portions Copyright [yyyy] [name of copyright owner]
#
# CDDL HEADER END

import os
import sys
import png, math
import numpy as np
from optparse import OptionParser
from struct import *

parser = OptionParser(usage = 'usage: %prog [options]')
parser.add_option('-n', '--name', help='name for bitmap', dest='name', default=None)
parser.add_option('-o', '--output', help='name of output file (.img produces binary blob, .h produces optimized header)', dest='output', default=None)
parser.add_option('-i', '--input', help='input header file', dest='input', default=None)
parser.add_option('-c', '--compress', help='compress output', action='store_true', dest='compress', default=False)
parser.add_option('-b', '--bitdepth', help='number of bits per pixel (default: 4)', type='int', dest='bitDepth', default=4)
(options, args) = parser.parse_args()

FLAGS_RLE_COMPRESSED = 1        # Run Length Encoded

if options.name == None or options.input == None:
    parser.error('name and input options are required')

def compressImage(image, bitDepth):
    ''' Takes unpacked indexed data and produces bitDepth packed data
    '''
    wordSize = 16       # 16 bits per word
    data = image['data']
    width = image['width']
    height = image['height']

    # arange in lines
    data = np.array(data).reshape(height,width)

    depth = data.max()+1
    if (depth > 2**bitDepth):
        print 'Warning: Image has {} colours, more than a bit depth of {} can accurately encode ({})'.format(depth, bitDepth, 2**bitDepth)
    scale = 16 / float(depth)
    scale = 2**bitDepth / float(depth)

    pixels = 0
    bitCount = wordSize
    output = []
    for line in data:
        line = line * scale
        for pix in line:
            if bitCount < bitDepth:
                lastPixels = pixels
                pixels |= int(pix) >> (bitDepth - bitCount)
                output.append(pixels)
                bitCount = wordSize - (bitDepth - bitCount)
                if int(pix) > 15:
                    print >> sys.stderr, 'ERROR: pix = {}'.format(pix)
                pixels = (int(pix) << bitCount) & 0xFFFF
            else:
                bitCount -= bitDepth
                pixels |= int(pix) << bitCount
                if bitCount == 0:
                    bitCount = wordSize
                    output.append(pixels)
                    pixels = 0
    if bitCount != 16:
        pixels = pixels << bitCount
        output.append(pixels)

    image['data'] = pack('<{}H'.format(len(output)), *output) # little endian, byte packed uint16_t
    image['flags'] = 0      # uncompressed
    image['depth'] = bitDepth

    return image

def compressImageRLE(image):
    ''' Takes unpacked indexed data and produces 4-bit RLE compressed data
    '''
    count = 0
    pixels = 0
    runCount = 0
    runPixel = 0
    data = image['data']
    width = image['width']
    height = image['height']

    # arange in lines
    data = np.array(data).reshape(height,width)

    depth = data.max()+1
    if (depth > 16):
        print "Warning: Image has more colors than bit depth can accurately encode"
    scale = 16 / float(depth)

    output = []
    for line in data:
        #line = line * scale
        ind = 0
        for pix in line:
            if runCount == 0:
                runPixel = pix
                runCount = 1
                continue
            if pix != runPixel:
                output.append((runCount-1) << 4 | int(runPixel))
                runPixel = pix
                runCount = 1
                count += 1
            else:
                runCount += 1
                if runCount > 16:
                    output.append(0xF0 | int(runPixel))
                    runCount = 1
                    count += 1
    if runCount != 0:
        output.append((runCount-1) << 4 | int(runPixel))
        runCount = 0
        count += 1

    if len(output) & 0x01 != 0:
        output.append(0)

    image['data'] = pack('{}B'.format(len(output)), *output) # bytes
    image['flags'] = FLAGS_RLE_COMPRESSED
    image['depth'] = 4

    return image

def parseGimpHeader(filename):
    ''' Parse GIMP H file image into array
    '''
    if (filename == "-"):
        fd = sys.stdin
    else:
        fd = open(filename, 'r');

    # locate width and height
    # then import all pixel data
    init = True
    init2 = True
    data = []
    mapping = []
    for line in fd:
        if init:
            if 'width' in line:
                width = int(line.split('=')[-1].strip()[:-1])
            elif 'height' in line:
                height = int(line.split('=')[-1].strip()[:-1])
            elif 'header_data_cmap[256][3]' in line:
                print "found cmap header"
                init = False
        elif init2:
            if 'header_data[]' in line:
                init2 = False
            elif '};' in line:
                continue
            else:
                mapping.append(int(line.replace("{", "").replace("}", "").strip().split(',')[0]))
        else:
            if '};' in line:
                break
            data.extend(line.split(','))

    data = [int(round(float(mapping[int(x.strip())])/255*15)) for x in data if x.strip().isdigit()]
    #print data
    if len(data) == 0:
        print 'Failed to extract pixel data from {}'.format(imageFile)
        sys.exit(1)

    dataSizeBytes = len(data)

    image = {
        'data': data,
        'dataSizeBytes': dataSizeBytes,
        'width': width,
        'height': height,
        }

    return image


def writeImage(filename, image):
    flags = image['flags']
    data = image['data']

    dataSize = len(data)
    if image['flags'] != FLAGS_RLE_COMPRESSED:
        dataSize /= 2

    if (filename == "-"):
        fd = sys.stdout
    else:
        fd = open(filename, 'wb');

    # Write header
    fd.write(pack('=HBBBH', image['width'], image['height'], image['depth'], flags, dataSize))
    # Write data
    fd.write(data)
    fd.close()


def writeMooltipassHeader(filename, imageName, image):
    width = image['width']
    height = image['height']
    data = unpack('<{}H'.format(len(image['data'])/2), image['data'])
    flags = image['flags']
    imageNameUpper = imageName.upper()
    dataArraySize = len(data)
    dataSize = dataArraySize

    if image['flags'] != FLAGS_RLE_COMPRESSED:
        dataSize /= 2

    if (filename == "-"):
        fd = sys.stdout
    else:
        fd = open(filename, 'w');

    print >> fd, '/*'
    print >> fd, ' * bitmap {}'.format(imageName)
    print >> fd, ' */'
    print >> fd
    print >> fd, '#define {}_WIDTH {}'.format(imageNameUpper, width)
    print >> fd, '#define {}_HEIGHT {}'.format(imageNameUpper, height)
    print >> fd, ''
    print >> fd, 'const struct {'
    print >> fd, '    uint16_t width;'
    print >> fd, '    uint8_t height;'
    print >> fd, '    uint8_t depth;'
    print >> fd, '    uint8_t flags;'
    print >> fd, '    uint16_t dataSize;'
    print >> fd, '    uint16_t data[{}];'.format(dataArraySize)
    print >> fd, '}} image_{} __attribute__((__progmem__)) = {{'.format(imageName)
    print >> fd, '    {0}_WIDTH, {0}_HEIGHT, {1}, {2}, {3},'.format(imageNameUpper, image['depth'], flags, dataSize)
    print >> fd, '    {',
    for ind in xrange(0,len(data)):
        if ind % 8 == 0:
            print >> fd, ''
            print >> fd, '    ',
        print >> fd, '0x{:04x}, '.format(data[ind]),
    print >> fd, '    }'
    print >> fd, '};'


def main():
    if (options.input == options.output):
        raise Exception("input and output must not be the same")

    image = parseGimpHeader(options.input)
    origSize = image['dataSizeBytes']
    print "Parsed header: {}x{}".format(image['width'], image['height'])

    if options.compress:
        image = compressImageRLE(image)
        print "Compressed image: {} -RLE-> {} bytes".format(origSize, len(image['data']))
    else:
        image = compressImage(image, options.bitDepth)
        print "Compressed image: {} -{}bit-> {} bytes".format(origSize, options.bitDepth, len(image['data']))

    unused, outputExtension = os.path.splitext(options.output)
    if outputExtension == ".img":
        writeImage(options.output, image)
    elif outputExtension == ".h":
        writeMooltipassHeader(options.output, options.name, image)
    print "Wrote {}".format(options.output)


if __name__ == "__main__":
    main()
