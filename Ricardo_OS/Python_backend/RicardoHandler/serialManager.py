from .packets import *
import serial
import time
from collections import deque
import threading



class SerialManager(threading.Thread):

	def __init__(self, port, baud=115200, waittime = .3):
		

		self.port = port
		self.baud = baud
		self.waittime = waittime
		self.boot_messages = ''
		self._connect() #connect to ricardo

		#threadsafe packetBuffer to store incomming packets awaiting processing
		self.packetBuffer = deque()
		self.sendBuffer = deque()


		

		self.exit_event = threading.Event()
		super(SerialManager,self).__init__(self)


	def run(self):
		read_thread = threading.Thread(target= self._read_next_packet)
		read_thread.daemon = True
		read_thread.start()

		while not self.exit_event.is_set():
			self._send_packet()
			#self._read_next_packet()
			
	#this method expects already serialized data
	def sendPacket(self,data):
		self.sendBuffer.append(data)
	#kill the thread if program exited
	def stop(self):
		self.exit_event.set()
		
	def _connect(self):
		self.ser = serial.Serial(port=self.port, baudrate=self.baud, timeout = self.waittime)  # open serial port

		self.ser.stopbits = serial.STOPBITS_ONE
		self.ser.parity = serial.PARITY_NONE
		self.ser.bytesize = serial.EIGHTBITS

		#print('call reset on esp32')
		self.ser.setDTR(False)
		time.sleep(1)
		self.ser.flushInput()
		self.ser.setDTR(True)
		#print('esp32 reset')
		self.ser.flushInput()
		time.sleep(1)
		#print('flushing boot messages')
		#get boot messages after reboot
		while (self.ser.in_waiting):
			data = self.ser.read(1)
			try:
				self.boot_messages += data.decode("utf-8")
			except:
				self.boot_messages += str(data)
	



	def _read_next_packet(self):
		while True:
			b = self.ser.read(1)
			while not (b == Header.start_byte.to_bytes(1, 'little')):
				b = self.ser.read(1)
			header_bytes = self.ser.read(Header.header_size - 1)
			header = Header.from_bytes(b + header_bytes)
			body = self.ser.read(header.packet_len) # Read the rest of the packet
			#add received packet to packet buffer
			self.packetBuffer.append((b + header_bytes + body))

	
	def _send_packet(self):
		if len(self.sendBuffer) > 0:
			self.ser.write(self.sendBuffer.popleft()) 

