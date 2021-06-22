from .packets import *
import serial
import time
import multiprocessing
import redis
import json


class SerialManager(multiprocessing.Process):

	def __init__(self, device, baud=115200, waittime = .3,redishost = 'localhost',redisport = 6379):
		
		super(SerialManager,self).__init__()
		self.device = device
		self.baud = baud
		self.waittime = waittime
		self.boot_messages = ''
		self.prevSendTime = 0
		self.sendDelta = 400e6
		

		self.packetRecordTimeout = 2*60 #default 2 minute timeout
		#dictionary structure
        #{key(packet uid):[client id, time_sent]}
		self.packetRecord = {} 
		self.counter = 0

		self.receivedQueueTimeout = 10*60 #default 10 minute timeout
        #dictionary structure
        #{key(client id):serialized packet data,time_added}
		#self.receivedPackets = {}

		self.exit_event = multiprocessing.Event()
		#connect to redis 
		self.rd = redis.Redis(host = redishost,port = redisport)
		self.__connect__() #connect to ricardo 
		


	def run(self):
		while not self.exit_event.is_set():
			self.__checkSendQueue__()
			self.__readPacket__()
			self.__cleanupPacketRecord__()


	#kill the thread if program exited
	def stop(self):
		self.exit_event.set()
		
	def __connect__(self):
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
		#print('flushing boot messages')
		#get boot messages after reboot
		while (self.ser.in_waiting):
			data = self.ser.read(1)
			try:
				self.boot_messages += data.decode("utf-8")
			except:
				self.boot_messages += str(data)
	
	def __readPacket__(self):
		if self.ser.in_waiting > 0: 
			b = self.ser.read(1)
			if (b == Header.start_byte.to_bytes(1, 'little')):
	
				header_bytes = self.ser.read(Header.header_size - 1) # -1 as we have read the first byte already
				if (len(b+header_bytes) < Header.header_size):
					#full header wasnt received and we porbbaly wont recieve this full header so dump it
					return

				header : Header = Header.from_bytes(b + header_bytes)
				body = self.ser.read(header.packet_len) # Read the rest of the packet -> this is likley the command which will block. Maybe its better to rewrite this to individually read each byte
				
				if (len(body) < header.packet_len):
					#full body wasnt received and we probbaly wont recieve the full packet so dump it
					return
				#add received packet to packet buffer
				#self.packetBuffer.append((b + header_bytes + body))
				self.__processReceivedPacket__((b + header_bytes + body))

	def __processReceivedPacket__(self,data:bytes):
		header = Header.from_bytes(data)#decode header
		uid = header.uid #get unique id

		if uid in self.packetRecord:
			#get client id from packetrecord and remove corresponding entry
			client_id = self.packetRecord.pop(uid)[0]
			#add entry to recieved packets dictionary with client id as key
			#self.receivedPackets[client_id] = [data,time.time()]
			key = "ReceiveQueue:"+str(client_id)	
		else:
			#we dont have this packet as a repsonse so place it in recieve buffer
			print("unkown packet recieved")
			key = "ReceiveQueue:__LOCAL__"
		#add received packets to redis db
		self.rd.lpush(key,data)
		#set timeout for list so list will be deleted if never acsessed
		#ensure on any redis interfaces, that the expiry is reset to esnure we dont loose packets
		self.rd.expire(key , self.receivedQueueTimeout) 


	def __sendPacket__(self,data:bytes,clientid):
		header = Header.from_bytes(data)#decode header
		uid = self.__generateUID__() #get uuid
		header.uid = uid #get uuid
		serialized_header = header.serialize() #re-serialize header
		#data[:len(serialized_header)] = serialized_header #overwrite old header
		# print(data)
		# print(data.hex())
		#modifieddata = serialized_header + data[len(serialized_header)-1 : -1]
		modifieddata = bytearray(data)
		modifieddata[:len(serialized_header)] = serialized_header
		# print(modifieddata)
		# print(modifieddata.hex())
		self.packetRecord[uid] = [clientid,time.time()] #update packetrecord dictionary
		#self.sendBuffer.append(data)#add packet to send buffer

		self.ser.write(modifieddata)#write packet to serial port and hope its free lol

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
			
				
		

	def __generateUID__(self):#replace this with a better uuid method lol this is such a hacky way
		uid = self.counter
		self.counter += 1
		return uid 
			
	def __cleanupPacketRecord__(self):
		expiry_time = time.time() - self.packetRecordTimeout
		
		#use list to force python to copy items
		for key,value in list(self.packetRecord.items()):
			if value[1] < expiry_time:
				self.packetRecord.pop(key) #remove entry
		
        