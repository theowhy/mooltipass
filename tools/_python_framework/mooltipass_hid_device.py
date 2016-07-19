#!/usr/bin/env python2
#
# Copyright (c) 2016 Mathieu Stephan
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
from mooltipass_defines import *
from generic_hid_device import *
from array import array
import struct
import random
import glob
import os

# Custom HID device class
class mooltipass_hid_device:

	# Device constructor
	def __init__(self):
		# HID device constructor
		self.device = generic_hid_device()
		
	def connect(self):
		return self.device.connect(True, USB_VID, USB_PID, USB_READ_TIMEOUT, self.createPingPacket(), self.checkPingAnswerPacket);
		
	# Get text from byte array
	def getTextFromUsbPacket(self, usb_packet):
		return "".join(map(chr, usb_packet[DATA_INDEX:])).split(b"\x00")[0]
		
	# Transform text to byte array
	def textToByteArray(self, text):
		ret_dat = array('B') 
		ret_dat.extend(map(ord,text))
		ret_dat.append(0)
		return ret_dat
		
	# Get a packet to send for a given command and payload
	def getPacketForCommand(self, cmd, len, data):
		# data to send
		arraytosend = array('B')

		# if command copy it otherwise copy the data
		if cmd != 0:
			arraytosend.append(len)
			arraytosend.append(cmd)

		# add the data
		if data is not None:
			arraytosend.extend(data)
			
		return arraytosend

	# Create a ping packet to be sent to the device
	def createPingPacket(self):	
		# prepare ping packet
		ping_packet = array('B')
		byte1 = random.randint(0, 255)
		byte2 = random.randint(0, 255)
		ping_packet.extend([2, CMD_PING, byte1, byte2]);
		return ping_packet

	# Function that the pong matches with the ping sent
	def checkPingAnswerPacket(self, pingSent, pongReceived):
		if pongReceived[CMD_INDEX] == CMD_PING and pongReceived[DATA_INDEX] == pingSent[DATA_INDEX] and pongReceived[DATA_INDEX+1] == pingSent[DATA_INDEX+1]:
			return True
		else:
			return False
			
	# Get the Mooltipass version, returns [Flash Mb, Version, Variant]
	def getMooltipassVersionAndVariant(self):
		self.device.sendHidPacket([0, CMD_VERSION]);
		version_data = self.device.receiveHidPacket()[DATA_INDEX:]
		# extract interesting data
		nbMb = version_data[0]
		version = "".join(map(chr, version_data[1:])).split(b"\x00")[0]
		variant = "standard"
		if "_" in version:
			variant = version.split('_', 1)[1]
			version = version.split('_', 1)[0]
		return [nbMb, version, variant]

	# Query the Mooltipass status
	def getMooltipassStatus(self):
		self.device.sendHidPacket([0, CMD_MOOLTIPASS_STATUS])
		status_data = self.device.receiveHidPacket()
		if status_data[DATA_INDEX] == 0:
			return "No card in Mooltipass"
		elif status_data[DATA_INDEX] == 1:
			return "Mooltipass locked"
		elif status_data[DATA_INDEX] == 3:
			return "Mooltipass locked, unlocking screen"
		elif status_data[DATA_INDEX] == 5:
			return "Mooltipass unlocked"
		elif status_data[DATA_INDEX] == 9:
			return "Unknown smartcard inserted"
			
	# Send custom packet
	def sendCustomPacket(self):
		command = raw_input("CMD ID: ")
		packet = array('B')
		temp_bool = 0
		length = 0

		#fill packet
		packet.append(0)
		packet.append(int(command, 16))

		#loop until packet is filled
		while temp_bool == 0 :
			try :
				intval = int(raw_input("Byte %s: " %length), 16)
				packet.append(intval)
				length = length + 1
			except ValueError :
				temp_bool = 1

		#update packet length
		packet[0] = length

		#ask for how many packets to receiveHidPacket
		packetstoreceive = input("How many packets to be received: ")

		#send packet
		self.device.sendHidPacket(packet)

		#receive packets
		for i in range(0, packetstoreceive):
			received_data = self.device.receiveHidPacket()
			print "Packet #" + str(i) + " in hex: " + ' '.join(hex(x) for x in received_data)

	# Upload bundle
	def	uploadBundle(self, password, filename):	
		password_set = False
		mooltipass_variant = self.getMooltipassVersionAndVariant()[2]
		
		# Check if a file name was passed
		if filename is not None:
			if not os.path.isfile(filename):
				print "Filename passed as arg isn't valid"
				filename = None
			else:
				bundlefile = open(filename, 'rb')
			
		# List available img files
		if filename is None:
			file_list = glob.glob("*.img")
			if len(file_list) == 0:
				print "No img file available!"
				return
			elif len(file_list) == 1:
				print "Using bundle file", file_list[0]
				bundlefile = open(file_list[0], 'rb')
			else:
				for i in range(0, len(file_list)):
					print str(i) + ": " + file_list[i]
				picked_file = raw_input("Choose file: ")
				if int(picked_file) >= len(file_list):
					print "Out of bounds"
					return
				else:
					bundlefile = open(file_list[int(picked_file)], 'rb')
		
		# Ask for Mooltipass password
		if password is None:
			mp_password = raw_input("Enter Mooltipass Password, press enter if None: ")
			if len(mp_password) == DEVICE_PASSWORD_SIZE*2:
				password_set = True
			else:
				print "Empty or erroneous password, using zeros"
		else:
			mp_password = password
		
		# Prepare the password
		print "Starting upload..."
		mooltipass_password = array('B')
		mooltipass_password.append(DEVICE_PASSWORD_SIZE)
		mooltipass_password.append(CMD_IMPORT_MEDIA_START)
		if password_set:
			for i in range(DEVICE_PASSWORD_SIZE):
				mooltipass_password.append(int(mp_password[i*2:i*2+2], 16))
		else:
			for i in range(DEVICE_PASSWORD_SIZE):
				mooltipass_password.append(0)
			
		# Success status boolean
		success_status = False
		
		# Send import media start packet
		self.device.sendHidPacket(mooltipass_password)
		
		# Check that the import command worked
		if self.device.receiveHidPacket()[DATA_INDEX] == 0x01:
			# Open bundle file
			byte = bundlefile.read(1)
			bytecounter = 0
			
			# Prepare first packet to send
			packet_to_send = array('B')
			packet_to_send.append(0)
			packet_to_send.append(CMD_IMPORT_MEDIA)
			
			# While we haven't finished looping through the bytes
			while byte != '':
				# Add byte to current packet
				packet_to_send.append(struct.unpack('B', byte)[0])
				# Increment byte counter
				bytecounter = bytecounter + 1
				# Read new byte
				byte = bundlefile.read(1)
				# If packet full, send it
				if bytecounter == 33:
					# Set correct payload size and send packet
					packet_to_send[LEN_INDEX] = bytecounter
					self.device.sendHidPacket(packet_to_send)
					# Prepare new packet to send
					packet_to_send = array('B')
					packet_to_send.append(0)
					packet_to_send.append(CMD_IMPORT_MEDIA)
					# Reset byte counter
					bytecounter = 0
					# Check ACK
					if self.device.receiveHidPacket()[DATA_INDEX] != 0x01:
						print "Error in upload"
						raw_input("press enter to acknowledge")
						
			# Send the remaining bytes
			packet_to_send[LEN_INDEX] = bytecounter
			self.device.sendHidPacket(packet_to_send)
			# Wait for ACK
			self.device.receiveHidPacket()
			# Inform we sent everything
			self.device.sendHidPacket([0, CMD_IMPORT_MEDIA_END])
			# If mini variant, the device reboots
			if mooltipass_variant != "mini":
				# Check ACK
				if self.device.receiveHidPacket()[DATA_INDEX] == 0x01:
					success_status = True
			else:
				success_status = True
			# Close file
			bundlefile.close()
			print "Done!"
		else:
			success_status = False
			print "fail!!!"
			print "likely causes: mooltipass already setup"

		return success_status
		
	def checkSecuritySettings(self):
		correct_password = raw_input("Enter mooltipass password: ")
		correct_key = raw_input("Enter request key: ")
		
		# Get Mooltipass variant
		mooltipass_variant = self.getMooltipassVersionAndVariant()[2]
		
		# Mooltipass password to be set
		mooltipass_password = array('B')
		request_key_and_uid = array('B')
		
		print "Getting random bytes - 1/2 random password"
		self.device.sendHidPacket([0, CMD_GET_RANDOM_NUMBER])
		data = self.device.receiveHidPacket()
		mooltipass_password.extend(data[DATA_INDEX:DATA_INDEX+32])
		print "Getting random bytes - 2/2 random password"
		self.device.sendHidPacket([0, CMD_GET_RANDOM_NUMBER])
		data2 = self.device.receiveHidPacket()
		mooltipass_password.extend(data2[DATA_INDEX:DATA_INDEX+30])
		print "Getting random bytes - UID & request key"
		self.device.sendHidPacket([0, CMD_GET_RANDOM_NUMBER])
		request_key_and_uid.extend(self.device.receiveHidPacket()[DATA_INDEX:DATA_INDEX+22])	
		print "Getting random bytes - 1/2 random password for jump command"
		self.device.sendHidPacket([0, CMD_GET_RANDOM_NUMBER])
		random_data = self.device.receiveHidPacket()[DATA_INDEX:DATA_INDEX+32]
		print "Getting random bytes - 2/2 random password for jump command"
		self.device.sendHidPacket([0, CMD_GET_RANDOM_NUMBER])
		random_data.extend(self.device.receiveHidPacket()[DATA_INDEX:DATA_INDEX+30])
		print "Done... starting test"
		print ""
				
		self.device.sendHidPacket(self.getPacketForCommand(CMD_SET_UID, 22, request_key_and_uid))
		if self.device.receiveHidPacket()[DATA_INDEX] == 0x01:
			print "FAIL - Other UID set!"
		else:
			print "OK - Couldn't set new UID"
			
		self.device.sendHidPacket(self.getPacketForCommand(CMD_GET_UID, 16, array('B', correct_key.decode("hex"))))
		data = self.device.receiveHidPacket()
		if data[LEN_INDEX] == 0x01:
			print "FAIL - Couln't fetch UID"
		else:
			print "OK - Fetched UID"
			if data[DATA_INDEX:DATA_INDEX+6] == array('B', correct_key.decode("hex"))[16:16+6]:
				print "OK - UID fetched is correct!"
			else:
				print "FAIL - UID fetched is different than the one provided!"
				print data[DATA_INDEX:DATA_INDEX+6], "instead of", array('B', correct_key.decode("hex"))[16:16+6]
		
		self.device.sendHidPacket(self.getPacketForCommand(CMD_GET_UID, 16, random_data[0:16]))
		data = self.device.receiveHidPacket()
		if data[LEN_INDEX] == 0x01:
			print "OK - Couldn't fetch UID with random key"
		else:
			print "FAIL - Fetched UID with random key"
			
		self.device.sendHidPacket(self.getPacketForCommand(CMD_SET_BOOTLOADER_PWD, 62, mooltipass_password))
		if self.device.receiveHidPacket()[DATA_INDEX] == 0x01:
			print "FAIL - New Mooltipass password was set"
		else:
			print "OK - Couldn't set new Mooltipass password"
		
		if mooltipass_variant != "mini":
			self.device.sendHidPacket(self.getPacketForCommand(CMD_JUMP_TO_BOOTLOADER, 62, random_data))
			print "Sending jump to bootloader with random password... did it work?"
			raw_input("Press enter")
			
			self.device.sendHidPacket(self.getPacketForCommand(CMD_JUMP_TO_BOOTLOADER, 62, array('B', correct_password.decode("hex"))))
			print "Sending jump to bootloader with good password... did it work?"
			raw_input("Press enter")