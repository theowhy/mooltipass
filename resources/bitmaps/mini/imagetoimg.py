#!/usr/bin/env python
#
# Copyright (c) 2016 Mathieu Stpehan
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
import argparse
from array import array
from PIL import Image
from struct import *
import random
import time
import sys
import os
import logging
logging.basicConfig(level=logging.INFO)

logger = logging.getLogger(__name__)

USB_VID					= 0x16D0
USB_PID					= 0x09A0

LEN_INDEX				= 0x00
CMD_INDEX				= 0x01
DATA_INDEX				= 0x02
PREV_ADDRESS_INDEX		= 0x02
NEXT_ADDRESS_INDEX		= 0x04
NEXT_CHILD_INDEX		= 0x06
SERVICE_INDEX			= 0x08
DESC_INDEX				= 6
LOGIN_INDEX				= 37
NODE_SIZE				= 132

CMD_PING				= 0xA1
CMD_MINI_FRAME_BUF_DATA = 0x9E


parser = argparse.ArgumentParser(description="Convert png images to binary format suitable for mooltipass")
parser.add_argument("input", help="Input files")
parser.add_argument('-o', '--output', help='converted image', dest='output', default=None)
parser.add_argument('-r', '--reverse', help='set to inverse pixel data', action="store_true", default=False)
parser.add_argument("-q", "--quiet", action="store_true", help="Only ouptut errors")
args = parser.parse_args()

if args.quiet:
	logger.setLevel(logging.ERROR)

def main():

	# Open image and convert it to monochrome
	image = Image.open(args.input)
	image = image.convert(mode="RGB", colors=1)

	# Get image specs
	img_format = image.format
	img_size = image.size
	img_mode = image.mode
	logger.info("Format: {}, size: {}, mode {}".format(img_format, img_size, img_mode))

	# Check size
	if img_size[0] > 128 or img_size[1] > 32:
		logger.error("Wrong dimensions or format!")
		sys.exit(0)

	# Compute size
	dataSize = img_size[0] * int(((img_size[1]+7) / 8))
	logger.info("Total data size: {} bytes".format(dataSize))

	# Turn image left 90 degrees
	image_rot = image.transpose(Image.ROTATE_270)
	#image_rot.save("lapin.jpg", "JPEG")
	#print image_rot.size[0]
	#print image_rot.size[1]

	# Process the pixels
	pixel_data = 0
	bitCount = 7
	bitstream = []
	for y in range(0, image_rot.size[1]):
		for page in range(0, int((image_rot.size[0]+7)/8)):
			bitCount = 7
			pixel_data = 0
			for x in range(page*8, page*8 + 8):
				# Get RGB values
				if(x >= image_rot.size[0]):
					rgb = [0,0,0]
				else:
					rgb = image_rot.getpixel((x, y))
				# Update pixel data
				if args.reverse:
					if rgb[0] < 128:
						pixel_data |= 1 << bitCount
				else:
					if rgb[0] > 128:
						pixel_data |= 1 << bitCount
				bitCount-=1
				if bitCount < 0:
					bitstream.append(pixel_data)
					pixel_data = 0
					bitCount = 7
			if bitCount != 7:
				bitstream.append(pixel_data)
	logger.debug("bitstream: {}".format(bitstream))

	# Open file to write
	filename = None
	if args.output:
		filename = args.output
	else:
		filename, file_extension = os.path.splitext(args.input)
		filename += ".img"

	fd = open(filename, 'wb');

	# Write header
	fd.write(pack('=HBBBH', img_size[0], img_size[1], 1, 0, dataSize))
	# Write data
	fd.write(pack('<{}B'.format(len(bitstream)), *bitstream))
	fd.close()

	logger.info("Data written to {}".format(filename))

if __name__ == "__main__":
	try:
		main()
	except Exception as e:
		logger.exception("{}: {}".format(e, e))
