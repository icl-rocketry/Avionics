from  .packets import *
import threading
import time
from .serialmanager import *

class PacketHandler():
    def __init__(self,serialmanager : SerialManager):
        self.serialmanager = serialmanager

        self.timeout = 2*60 #default 2 minute timeout
        #dictionary structure
        #{key(packet uid):[client id, time_sent]}
        self.packetRecord = {} 
        #dictionary structure
        #{key(client id):serialized packet data,time_added}
        self.receivedPackets = {}
        self.counter = 0
        


    def run(self):
        #check redis for new messages
        #check timeouts on the expected responses
        pass
    
    def sendPacket(self,data:bytes,clientid):
        header = Header(data)#decode header
        uid = self._generateUID() #get uuid
        header.system_time = uid #get uuid
        serialized_header = header.serialize #re-serialize header
        data[:len(serialized_header)] = serialized_header #overwrite old header

        self.packetRecord[uid] = [clientid,time.time()] #update packetrecord dictionary
        self.serialmanager.sendPacket(data)#forward packet to serial manager
    
    def receivePacket(self,data:bytes):
        header = Header(data)#decode header
        uid = header.system_time #get unique id

        if uid in self.packetRecord:
            #get client id from packetrecord and remove corresponding entry
            client_id = self.packetRecord.pop(uid)[0]
            #add entry to recieved packets dictionary with client id as key
            self.receivedPackets[client_id] = [data,time.time()]
        else:
            #we dont have this packet so dump it but maybe also log it at the same time
            print("unkown packet recieved")


    def _cleanupPacketRecord(self):
        pass

    def _cleanupReceivedPackets(self):
        pass

    def _generateUID(self):#replace this with a better uuid method
        uuid = self.counter
        self.counter += 1
        return uuid 