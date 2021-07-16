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


ser = serial.Serial(port = args['port'],baudrate = 115200,timeout = .3)  # open serial port
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
time.sleep(2)
print('flushing boot messages')



boot_messages = ""
while ser.in_waiting > 0:
	data = ser.read(1)
	try:
		boot_messages += data.decode("utf-8")
	except:
		boot_messages += str(data)

#boot_messages = ser.read(ser.in_waiting)

print(boot_messages)

# Test send a comand packet
header = Header(2, 0, 2, 0, source=2, destination=0) # source=4 for USB and destination=0 for rocket
cmd_packet = Command(header, 50, 0) # 50 for detailed all
serialized_packet = cmd_packet.serialize()
print(serialized_packet.hex())
ser.write(serialized_packet)
t = time.time_ns()
print('we wrote shit')


while True:
	
	
	

	b = ser.read(1)
	#print(str(b,"utf-8"),end="",flush=True)
	try:
		print(b.decode("utf-8"),end='',flush=True)
	except:
		print(b,end='',flush=True)
	#print(ser.in_waiting)
	
	if b == Header.start_byte.to_bytes(1, 'little'):
		# We've read the header start byte, deserialize the header
		header_bytes = ser.read(Header.header_size-1)

		recv_header = Header.from_bytes(b+header_bytes)
		rcv_packet_type = recv_header.packet_type

		packet_body = ser.read(recv_header.packet_len) # Read the rest of the packet
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
			rcv_packet = DetailedAll.from_bytes(b + header_bytes + packet_body) # Constructor expects to receive bytes consisting off header + packet body
			print('RECEIVED PACKET:')
			print(rcv_packet)
			dt = (time.time_ns() - t) * (10**-9)
			print("time: " + str(dt) + " frequency: " + str(1/dt))
	

ser.close() 
