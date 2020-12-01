# Python script to test communication between a RICARDO unit and a computer
# Written by --PAVoL MaRGItFalvI--

# CURRENTLY SENDS A 'DETAILED_ALL' COMMAND EVERY 5 SECONDS AND PRINTS THE RESPONSE

import serial
import argparse
import time
from packets import *

# Argument Parsing
ap = argparse.ArgumentParser()
ap.add_argument("-p", "--port", required=True, help="Port hosting ricardo", type=str)
args = vars(ap.parse_args())


ser = serial.Serial(args['port'])  # open serial port
ser.baudrate = 115200
ser.bytesize = serial.EIGHTBITS

# Test send a comand packet
header = Header(2, 0, 2, 0, source=4, destination=0) # source=4 for USB and destination=0 for rocket
while True:
	cmd_packet = Command(header, 50, 0) # 50 for detailed all
	serialized_packet = cmd_packet.serialize()
	ser.write(serialized_packet)
	ser.flush()
	b = ser.read(1)
	if b == Header.start_byte.to_bytes(1, 'big'):
		# We've read the header start byte, deserialize the header
		header_bytes = ser.read(Header.header_size)
		recv_header = Header.from_bytes(header_bytes)
		rcv_packet_type = recv_header.packet_type

		packet_body = ser.read(recv_header.packet_len) # Read the rest of the packet

		if rcv_packet_type == 1:
			# Telemetry
			pass
		elif rcv_packet_type == 2:
			# Command
			pass
		elif rcv_packet_type == 3:
			# Detailed all
			rcv_packet = DetailedAll.from_bytes(header + packet_body) # Constructor expects to receive bytes consisting off header + packe body
			print('RECEIVED PACKET:')
			print(rcv_packet)
	time.sleep(5)

ser.close() 