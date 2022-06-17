from serial.serialutil import PARITY_NONE
# from .packets import *
from pylibrnp.rnppacket import DeserializationError, RnpHeader
import serial
import time
import redis
import json
from cobs import cobs
import signal
import sys
import socket



class SerialManager():

	def __init__(self, device, baud=115200, waittime = .3,redishost = 'localhost',redisport = 6379,verbose=False,UDPMonitor=False,UDPIp='127.0.0.1',UDPPort=7000):
		signal.signal(signal.SIGINT,self.exitHandler)
		signal.signal(signal.SIGTERM,self.exitHandler)
		self.device = device
		self.baud = baud
		self.waittime = waittime
		self.prevSendTime = 0
		self.sendDelta = 1e4
		self.verbose = verbose
		

		self.packetRecordTimeout = 2*60 #default 2 minute timeout
		self.messageQueueSize = 5000
		self.receiveBuffer = []

		self.packetRecord = {} 
		self.counter = 0

		self.receivedQueueTimeout = 10*60 #default 10 minute timeout

		self.redishost = redishost
		self.redisport= redisport

		#connect to redis 
		self.rd = redis.Redis(host = self.redishost,port = self.redisport)
		#clear SendQueue
		self.rd.delete("SendQueue")

		self.UDPMonitor = UDPMonitor
		#setup udp monitor
		if (UDPMonitor):
			self.UDPIp = UDPIp
			self.UDPPort = UDPPort
			self.sock = None

		
	def run(self):
		with socket.socket(socket.AF_INET,socket.SOCK_DGRAM) as self.sock:
			self.__connect__() #connect to ricardo 
			
			while True:
				self.__checkSendQueue__()
				self.__readPacket__()
				self.__cleanupPacketRecord__()
	
	def exitHandler(self,sig,frame):
		print("Serial Manager Exited")
		self.ser.close() #close serial port
		sys.exit(0)

		
	def __connect__(self):
		boot_messages = ''
		self.ser = serial.Serial(port=self.device, baudrate=self.baud, timeout = self.waittime)  # open serial port

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
					self.__sendToUDP__(decodedData)
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
			print("Length Mismatch expected:" + str((RnpHeader.size + header.packet_len)) + " Received: " + str(len(data)))
			print(data.hex())
			return

		uid = header.uid #get unique id
	
		if uid in self.packetRecord:
			#get client id from packetrecord and remove corresponding entry
			client_id = self.packetRecord.pop(uid)[0]
			#add entry to recieved packets dictionary with client id as key
			key = "ReceiveQueue:"+str(client_id)	
		else:
			#handle packets addressed to the local packet handler on the backend
			if (uid == 0) and (header.destination_service == 0):
				self.__localPacketHandler__(data)

				return

			#unkown packet received
			print("[ERROR] unkown packet recieved")
			key = "ReceiveQueue:__UNKOWN_PACKET__"

			
		#add received packets to redis db
		self.rd.lpush(key,data)
		#set timeout for list so list will be deleted if never acsessed
		#ensure on any redis interfaces, that the expiry is reset to esnure we dont loose packets
		self.rd.expire(key , self.receivedQueueTimeout) 


	def __sendPacket__(self,data:bytes,clientid):
		header = RnpHeader.from_bytes(data)#decode header
		uid = self.__generateUID__() #get uuid
		header.uid = uid #get uuid
		serialized_header = header.serialize() #re-serialize header
		modifieddata = bytearray(data)
		modifieddata[:len(serialized_header)] = serialized_header
		self.packetRecord[uid] = [clientid,time.time()] #update packetrecord dictionary
		#self.sendBuffer.append(data)#add packet to send buffer
		self.__sendToUDP__(modifieddata)  #send data to udp monitor
		# cobs encode
		encoded = bytearray(cobs.encode(modifieddata))
		encoded += (0x00).to_bytes(1,'little') #add end packet marker
		self.ser.write(encoded)#write packet to serial port and hope its free lol
		

	def __checkSendQueue__(self):
		#check if there are items present in send queue
		if (time.time_ns() - self.prevSendTime) > self.sendDelta :
			if self.rd.llen("SendQueue") > 0:
				item = json.loads(self.rd.rpop("SendQueue"))
				#item is a json object with structure 
				#{data:bytes as hex string,
				# clientid:""}
				self.__sendPacket__(bytes.fromhex(item["data"]),item["clientid"])
				self.prevSendTime = time.time_ns()
			
				
		

	def __generateUID__(self):
		#UID is a unsigend 16bit integer. UID 0 is reserved for forwarding to local so we want 
		#strictly increasing integers in the range [1 65535]
		self.counter = (self.counter%65535) + 1
		return self.counter 
			
	def __cleanupPacketRecord__(self):
		expiry_time = time.time() - self.packetRecordTimeout
		
		#use list to force python to copy items
		for key,value in list(self.packetRecord.items()):
			if value[1] < expiry_time:
				self.packetRecord.pop(key) #remove entry

	def __sendToUDP__(self,data:bytearray):
		if (self.UDPMonitor):
			self.sock.sendto(data,(self.UDPIp,self.UDPPort))

	def __localPacketHandler__(self,data:bytes):
		#decode header
		header = RnpHeader.from_bytes(data)
		if (header.packet_type == 100): #message packet
			packet_body = data[RnpHeader.size:]
			try:
				message:str = packet_body.decode('UTF-8') 
			except:
				message:str = str(packet_body)
			if self.verbose:
				print("Message: " + message)
			json_message = {"header" : vars(header),
							"message": message}
			#check length of message queue before pushing 
			if (self.rd.llen("MessageQueue") > self.messageQueueSize):
				self.rd.delete("MessageQueue")
			self.rd.lpush("MessageQueue",json.dumps(json_message))
			self.rd.expire("MessageQueue" , self.receivedQueueTimeout) 
			return
		
		