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


ser = serial.Serial(port = args['port'],baudrate = 115200)  # open serial port
#ser.rtscts = True
#ser.dsrdtr = True
ser.stopbits = serial.STOPBITS_ONE
ser.parity = serial.PARITY_NONE
ser.bytesize = serial.EIGHTBITS

print('call reset on esp32')
ser.setDTR(False)
time.sleep(1)
ser.flushInput()
ser.setDTR(True)
print('esp32 reset')
ser.flushInput()
time.sleep(1)
print('flushing boot messages')
ser.read(ser.in_waiting)



# Test send a comand packet
header = Header(2, 0, 2, 0, source=4, destination=0) # source=4 for USB and destination=0 for rocket
cmd_packet = Command(header, 50, 0) # 50 for detailed all
serialized_packet = cmd_packet.serialize()
print(serialized_packet.hex())
ser.write(serialized_packet)
print('we wrote shit')

while True:
	#print('num bytes: ' , ser.in_waiting)
	b = ser.read(1)
	print('0x',b.hex())
	#print(b.hex(),end=' ',flush=True)
	if b == Header.start_byte.to_bytes(1, 'big'):
		# We've read the header start byte, deserialize the header
		header_bytes = ser.read(Header.header_size)
		recv_header = Header.from_bytes(b+header_bytes)
		rcv_packet_type = recv_header.packet_type
		print('packet_len',recv_header.packet_len)
		print('packet_type',rcv_packet_type)

		packet_body = ser.read(recv_header.packet_len) # Read the rest of the packet
		print('got packet data')
		if rcv_packet_type == 1:
			# Telemetry
			print('telem')
			pass
		elif rcv_packet_type == 2:
			# Command
			print('command')
			pass
		elif rcv_packet_type == 3:
			# Detailed all
			print('detailed')
			rcv_packet = DetailedAll.from_bytes(header + packet_body) # Constructor expects to receive bytes consisting off header + packe body
			print('RECEIVED PACKET:')
			print(rcv_packet)
	time.sleep(.01)

ser.close() 
