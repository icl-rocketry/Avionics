from packets import *
import serial
import argparse
import time
from database import Database
from plotter import Plotter


class NetworkManager:

	def __init__(self, port, baud=115200):
		self.ser = serial.Serial(port=port, baudrate=baud)  # open serial port

		self.ser.stopbits = serial.STOPBITS_ONE
		self.ser.parity = serial.PARITY_NONE
		self.ser.bytesize = serial.EIGHTBITS

		print('call reset on esp32')
		self.ser.setDTR(False)
		time.sleep(1)
		self.ser.flushInput()
		self.ser.setDTR(True)
		print('esp32 reset')
		self.ser.flushInput()
		time.sleep(1)
		print('flushing boot messages')
		self.ser.read(self.ser.in_waiting)

		self.db = Database()
		self.plotter = Plotter()

		self.run = True # Run flag

	def loop(self):
		while self.run:
			header = Header(2, 0, 2, 0, source=4, destination=0) # source=4 for USB and destination=0 for rocket
			cmd_packet = Command(header, 50, 0) # 50 for detailed all

			self._send_packet(cmd_packet)
			packet = self._read_next_packet()
			
			self.plotter.update(packet)
			#self.db.add(packet)
	
	def _read_next_packet(self):
		
		b = self.ser.read(1)
		
		while not (b == Header.start_byte.to_bytes(1, 'little')):
			b = self.ser.read(1)
		
		header_bytes = self.ser.read(Header.header_size - 1)
		header = Header.from_bytes(b + header_bytes)
		packet_type = header.packet_type

		body = self.ser.read(header.packet_len) # Read the rest of the packet
		if packet_type == 1:
			# Telemetry TODO
			print('telem')
			pass
		elif packet_type == 2:
			# Command TODO
			print('command')
			pass
		elif packet_type == 3:
			# Detailed all
			print('detailed')
			rcv_packet = DetailedAll.from_bytes(b + header_bytes + body) # Constructor expects to receive bytes consisting off header + packet body
			
			print('RECEIVED PACKET:')
			print(rcv_packet)
			
			return rcv_packet
	
	def _send_packet(self, packet):
		self.ser.write(packet.serialize())


	def _call_callbacks(self):
		pass

if __name__ == '__main__':
	# Argument Parsing
	ap = argparse.ArgumentParser()
	ap.add_argument("-p", "--port", required=True, help="Port hosting ricardo", type=str)
	args = vars(ap.parse_args())

	nm = NetworkManager(args['port'])
	nm.loop()
