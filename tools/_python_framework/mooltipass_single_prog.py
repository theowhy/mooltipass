#!/usr/bin/env python2
#
# Copyright (c) 2014 Mathieu Stephan
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
# TODO: investigate the avrdude outputs when more than 6 buttons are pressed at the same time: not sure all of them are programmed.
from mooltipass_hid_device import *
from generate_prog_file import *
import firmwareBundlePackAndSign
from datetime import datetime
from array import array
try:
	import seccure
except ImportError:
	pass
import threading
import commands
import pickle
import time
import sys
import os

avrdude_command_verbose = True
# TLV Field indexes
LEN_INDEX               = 0x00
CMD_INDEX               = 0x01
DATA_INDEX              = 0x02
# General defines
AES_KEY_LENGTH 			= 256/8
UID_REQUEST_KEY_LENGTH 	= 16
UID_KEY_LENGTH 			= 6
# Global variables for user input
main_program_running 	= True
serial_to_program		= 0
serial_entered			= False
ic_programmed 			= False


def user_input_loop():
	global main_program_running
	global serial_to_program
	global serial_entered
	global ic_programmed
	
	# Main loop
	while main_program_running:
		# Ask user for serial number to input
		serial_to_program = raw_input("Enter Serial Number or Press Enter to Exit: ")
		ic_programmed = False
		
		# Check for program exit
		if serial_to_program == "":
			main_program_running = False
			print "Good Bye!"
		else:
			serial_to_program = int(serial_to_program)
			serial_entered = True
			# Wait for programming thread to finish
			while ic_programmed == False:
				time.sleep(.1)	

def pickle_write(data, outfile):
	f = open(outfile, "w+b")
	pickle.dump(data, f)
	f.close()
		
def pickle_read(filename):
	f = open(filename)
	data = pickle.load(f)
	f.close()
	return data
	
def start_programming(mooltipass_id, flashFile, EepromFile, encryptedKeysFile):
	print "Programming with flash file", flashFile, "and eeprom file", EepromFile
	
	# Read fuses using avrdude
	socket_id = 0
	avrdude_command = "avrdude -c avrisp2 -p m32u4 -B 10 -U lfuse:r:/tmp/low_fuse_val_"+str(socket_id)+".hex:r -U hfuse:r:/tmp/high_fuse_val_"+str(socket_id)+".hex:r -U efuse:r:/tmp/extended_fuse_val_"+str(socket_id)+".hex:r -U lock:r:/tmp/lock_fuse_val_"+str(socket_id)+".hex:r"
	output_avrdude = commands.getstatusoutput(avrdude_command)
	
	# Check for generated files
	if not os.path.isfile("/tmp/low_fuse_val_"+str(socket_id)+".hex"):
		print "Couldn't read fuse files"
		print "Avrdude command", avrdude_command
		print "Output:", output_avrdude
		return [False, mooltipass_id, flashFile, EepromFile, "avrdude error!", encryptedKeysFile]
		
	# Read generated files
	file = open("/tmp/low_fuse_val_"+str(socket_id)+".hex", 'rb')
	low_fuse = struct.unpack('B', file.read(1))[0]
	file.close()
	file = open("/tmp/high_fuse_val_"+str(socket_id)+".hex", 'rb')
	high_fuse = struct.unpack('B', file.read(1))[0]
	file.close()
	file = open("/tmp/extended_fuse_val_"+str(socket_id)+".hex", 'rb')
	extended_fuse = struct.unpack('B', file.read(1))[0]
	file.close()
	file = open("/tmp/lock_fuse_val_"+str(socket_id)+".hex", 'rb')
	lock_fuse = struct.unpack('B', file.read(1))[0]
	file.close()
	
	# Print values and delete temporary values
	#print "Low fuse:", hex(low_fuse)
	#print "High fuse:", hex(high_fuse)
	#print "Extended fuse:", hex(extended_fuse)
	#print "Lock fuse:", hex(lock_fuse)
	os.remove("/tmp/low_fuse_val_"+str(socket_id)+".hex")
	os.remove("/tmp/high_fuse_val_"+str(socket_id)+".hex")
	os.remove("/tmp/extended_fuse_val_"+str(socket_id)+".hex")
	os.remove("/tmp/lock_fuse_val_"+str(socket_id)+".hex")
	
	# Check if it wasn't already programmed
	#if low_fuse == 0xFF and high_fuse == 0xD8 and extended_fuse == 0xC8 and lock_fuse == 0x3C and True:
	#	return [False, mooltipass_id, flashFile, EepromFile, "already programmed!", encryptedKeysFile]
		
	# Erase device
	commands.getstatusoutput("avrdude -c avrisp2 -p m32u4 -B 10 -e")
	
	# Program all fuses except lock fuse
	avrdude_command = "avrdude -c avrisp2 -p m32u4 -B 10 -U lfuse:w:0xFF:m -U hfuse:w:0xD8:m -U efuse:w:0xC8:m"
	output_avrdude_prog_fuse = commands.getstatusoutput(avrdude_command)
	if avrdude_command_verbose:
		print output_avrdude_prog_fuse
	
	# Program flash & eeprom
	avrdude_command = "avrdude -c avrisp2 -p m32u4 -B 1 -U flash:w:"+flashFile+":i -U eeprom:w:"+EepromFile+":i"
	output_avrdude_flash = commands.getstatusoutput(avrdude_command)
	if avrdude_command_verbose:
		print output_avrdude_flash
	
	# Program lock fuse
	avrdude_command = "avrdude -c avrisp2 -p m32u4 -B 1 -U lock:w:0x3C:m"
	output_avrdude_prog_lock = commands.getstatusoutput(avrdude_command)	
	if avrdude_command_verbose:
		print output_avrdude_prog_lock
	
	# Return success state
	if "failed" in output_avrdude_prog_fuse[1] or "failed" in output_avrdude_flash[1] or "failed" in output_avrdude_prog_lock[1]:
		return [False, mooltipass_id, flashFile, EepromFile, "couldn't program!", encryptedKeysFile]
	else:
		return [True, mooltipass_id, flashFile, EepromFile, "programmed", encryptedKeysFile]

def main():
	print "Mooltipass Programming Tool"
	rng_buf_save_armed = True
	global main_program_running
	global serial_to_program
	global serial_entered
	global ic_programmed
	
	# Delete temp .hex file that might have been left there
	commands.getstatusoutput("rm /tmp/*.hex")
	
	# Check for public key
	if not os.path.isfile("publickey.bin"):
		print "Couldn't find public key!"
		return
	
	# Check for firmware file presence
	if not os.path.isfile("Mooltipass.hex"):
		print "Couldn't find Mooltipass.hex"
		sys.exit(0)
	
	# Check for bootloader file presence
	if not os.path.isfile("bootloader_mini.hex"):
		print "Couldn't find bootloader_mini.hex"
		sys.exit(0)
		
	# Random bytes buffer
	random_bytes_buffer = []
	
	# HID device constructor
	mooltipass_device = mooltipass_hid_device()	
	
	# Connect to device
	if mooltipass_device.connect(True) == False:
		print "No Mooltipass Connected!"
		sys.exit(0)
	
	# Generate a blank firmware to see if we actually can generate it and then print the hash
	return_gen = generateFlashAndEepromHex("Mooltipass.hex", "bootloader_mini.hex", 12345, [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0], [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0], [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0], [0,0,0,0,0,0], "/tmp/test_flash.hex", "/tmp/test_eeprom.hex", True)
	os.remove("/tmp/test_flash.hex")
	os.remove("/tmp/test_eeprom.hex")
	
	# Check the success status
	if return_gen[0] == False:
		print "Couldn't generate a template flash hex!"
		sys.exit(0)
	
	# Check for random numbers file presence
	if os.path.isfile("rng.bin"):
		try:
			random_bytes_buffer = pickle_read("rng.bin")
		except EOFError:
			# This happens when the file is corrupted
			random_bytes_buffer = []
			os.remove("rng.bin")
		
	# Set to true to export the random bytes
	if False:		
		f = open('randombytes.bin', 'wb')
		random_bytes_buffer.tofile(f)
		f.flush()
		f.close()
				
	# Read public key
	public_key = pickle_read("publickey.bin")
	
	# Start user input thread
	user_input_thread = threading.Thread(target=user_input_loop, args=())
	user_input_thread.start()
	
	# Main loop
	last_second = int(time.time())
	while main_program_running:
		time.sleep(.1)
	
		# Our generator generates 8 bytes per second
		ts = int(time.time())
		if (ts - last_second) > 4:
			# Fetch random bytes
			mooltipass_device.getInternalDevice().sendHidPacket(mooltipass_device.getPacketForCommand(CMD_GET_RANDOM_NUMBER, 0, None))
			random_bytes = mooltipass_device.getInternalDevice().receiveHidPacketWithTimeout()[DATA_INDEX:DATA_INDEX+32]
			random_bytes_buffer.extend(random_bytes)
			last_second = ts
			
		# Store buffer every minute
		if datetime.now().second == 0:
			if rng_buf_save_armed:
				#print "Random bytes buffer saved:", len(random_bytes_buffer), "bytes available"
				pickle_write(random_bytes_buffer, "rng.bin")
				rng_buf_save_armed = False
		else:
			rng_buf_save_armed = True
		
		# If a serial number was entered
		if serial_entered == True and len(random_bytes_buffer) >= AES_KEY_LENGTH+AES_KEY_LENGTH+UID_REQUEST_KEY_LENGTH+UID_KEY_LENGTH:
			# Store serial number to program
			mooltipass_id = serial_to_program
			
			# Generate keys from the random bytes buffer
			aes_key1 = random_bytes_buffer[0:AES_KEY_LENGTH]
			aes_key2 = random_bytes_buffer[AES_KEY_LENGTH:AES_KEY_LENGTH+AES_KEY_LENGTH]
			uid_key = random_bytes_buffer[AES_KEY_LENGTH+AES_KEY_LENGTH:AES_KEY_LENGTH+AES_KEY_LENGTH+UID_REQUEST_KEY_LENGTH]
			uid = random_bytes_buffer[AES_KEY_LENGTH+AES_KEY_LENGTH+UID_REQUEST_KEY_LENGTH:AES_KEY_LENGTH+AES_KEY_LENGTH+UID_REQUEST_KEY_LENGTH+UID_KEY_LENGTH]
			del(random_bytes_buffer[0:AES_KEY_LENGTH+AES_KEY_LENGTH+UID_REQUEST_KEY_LENGTH+UID_KEY_LENGTH])
			
			# Write in file: Mooltipass ID | aes key 1 | aes key 2 | request ID key | UID, flush write					
			aes_key1_text =  "".join(format(x, "02x") for x in aes_key1)
			aes_key2_text =  "".join(format(x, "02x") for x in aes_key2)
			uid_key_text = "".join(format(x, "02x") for x in uid_key)
			uid_text = "".join(format(x, "02x") for x in uid)					
			string_export = str(mooltipass_id)+"|"+ aes_key1_text +"|"+ aes_key2_text +"|"+ uid_key_text +"|"+ uid_text+"\r\n"
			#print string_export
			pickle_file_name = time.strftime("export/%Y-%m-%d-%H-%M-%S-Mooltipass-")+str(mooltipass_id)+".txt"
			pickle_write(seccure.encrypt(string_export, public_key, curve='secp521r1/nistp521'), pickle_file_name)	
			
			# Generate programming file					
			generateFlashAndEepromHex("Mooltipass.hex", "bootloader_mini.hex", mooltipass_id, aes_key1, aes_key2, uid_key, uid, "/tmp/flash_"+str(mooltipass_id)+".hex", "/tmp/eeprom_"+str(mooltipass_id)+".hex", False)
			
			# Start programming 
			return_data = start_programming(mooltipass_id, "/tmp/flash_"+str(mooltipass_id)+".hex", "/tmp/eeprom_"+str(mooltipass_id)+".hex", pickle_file_name)
			
			# Check success state
			if return_data[0] :
				print "Programming succeeded (mooltipass id:", str(return_data[1]) + ")"
			else:
				print "Programming failed (mooltipass id:", str(return_data[1]) + ")"				
				# Delete encrypted keys file
				os.remove(return_data[5])
			
			# Free waiting thread
			serial_entered = False
			ic_programmed = True

if __name__ == "__main__":
	main()

	
	
	
	
	
	
	
	
	
	
	
	