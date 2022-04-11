from serial.serialutil import PARITY_NONE,SerialException
# from .packets import *
from pylibrnp.rnppacket import DeserializationError, RnpHeader
import serial
import time
from cobs import cobs
import signal
import sys
import multiprocessing
import queue
import signal
import os

class SerialManager():

	def __init__(self, device, sendQ,receiveQ, baud=115200, waittime = .3,verbose=False):

		self.device = device
		self.baud = baud
		self.waittime = waittime
		self.prevSendTime = 0
		self.sendDelta = 5e6
		self.verbose = verbose

		self.ser = None
		

		# self.packetRecordTimeout = 2*60 #default 2 minute timeout
		self.receiveBuffer = []
		self.counter = 0


		self.sendQueue = sendQ
		self.receiveQueue = receiveQ

		
		
	def run(self):
		signal.signal(signal.SIGINT,self.exitHandler)
		signal.signal(signal.SIGTERM,self.exitHandler)
		print("PID:" + str(os.getpid()) + " - Starting SerialManager")
		self.__connect__() #connect to ricardo 
		while True:
			self.__checkSendQueue__()
			self.__readPacket__()

	def exitHandler(self,sig=None,frame=None):
		print("Serial Manager Exited")
		print("PID:" + str(os.getpid()) + " killed")
		try:
			self.ser.close() #close serial port
		except:
			pass
		sys.exit(0)

		
	def __connect__(self):
		boot_messages = ''

		try:
			self.ser = serial.Serial(port=self.device, baudrate=self.baud, timeout = self.waittime)  # open serial port
		except SerialException as e:
			print(e)
			self.exitHandler()

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
		#get boot messages after reboot
		while (self.ser.in_waiting):
			data = self.ser.read(1)
			try:
				boot_messages += data.decode("utf-8")
			except:
				boot_messages += str(data)
		if self.verbose:
			print(boot_messages)

	def __readPacket__(self):
		#cobs decode
		while self.ser.in_waiting > 0:
			incomming = self.ser.read(1)
			if (incomming == (0x00).to_bytes(1,'little')):
				if (len(self.receiveBuffer) == 0):
					#empty frame receved, discard this
					return
				try:
					decodedData = cobs.decode(bytearray(self.receiveBuffer))
					self.__processReceivedPacket__(decodedData)
					if self.verbose:
						print(bytearray(decodedData).hex())
				except cobs.DecodeError as e:
					print("Decode Error, the following data could not be decoded...")
					print(e)
					print(self.receiveBuffer)
				#empty receive buffer
				self.receiveBuffer = []
			else:
				#place new byte at end of buffer
				self.receiveBuffer += incomming
 
	def __processReceivedPacket__(self,data:bytes):
		try:
			header = RnpHeader.from_bytes(data)#decode header
		except DeserializationError:
			print("Deserialization Error")
			print(data)
			return
		#check header len
		if (len(data) != (RnpHeader.size + header.packet_len)):
			print("Length Mismatch")
			return
		self.receiveQueue.put({"data":data})

	def __sendPacket__(self,data:bytes):
		header = RnpHeader.from_bytes(data)#decode header
		uid = self.__generateUID__() #get uuid
		header.uid = uid #get uuid
		serialized_header = header.serialize() #re-serialize header
		modifieddata = bytearray(data)
		modifieddata[:len(serialized_header)] = serialized_header
		# cobs encode
		encoded = bytearray(cobs.encode(modifieddata))
		encoded += (0x00).to_bytes(1,'little') #add end packet marker
		self.ser.write(encoded)#write packet to serial port and hope its free lol
		

	def __checkSendQueue__(self):
		#check if there are items present in send queue
		# if (time.time_ns() - self.prevSendTime) > self.sendDelta :
		try:
			packet = self.sendQueue.get_nowait()
			self.__sendPacket__(bytes(packet['data']))
		except queue.Empty:
			pass
			# self.prevSendTime = time.time_ns()
				

	def __generateUID__(self):#replace this with a better uuid method lol this is such a hacky way
		uid = self.counter
		self.counter += 1
		return uid 

	def __printToMon__(self,data):
		try:
			print(data.decode("utf-8"))
		except:
			print(str(data))