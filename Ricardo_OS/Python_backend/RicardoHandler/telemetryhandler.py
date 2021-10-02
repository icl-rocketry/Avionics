import redis
from RicardoHandler import packets
import time
import json
import copy
import signal
import sys

class TelemetryHandler():
    
    def __init__(self,updateTimePeriod = 500e6,redishost = 'localhost',redisport = 6379,clientid = "LOCAL:TELEMETRYTASK"):
        signal.signal(signal.SIGTERM,self.exitHandler)
        signal.signal(signal.SIGINT,self.exitHandler)

        self.prev_time = 0

        self.lastPacketTime = 0
        self.packetTimeout = 1e10
        self.lastTelemetry = {}


        self.state = {
            "run":False,
            "source":4,
            "destination":2,
            "dt":updateTimePeriod,
        }

        self.clientid : str = clientid 
        self.updateTimePeriod = updateTimePeriod


        self.r = redis.Redis(host=redishost,port=redisport)
        self.r.set(clientid + ":STATE",json.dumps(self.state))


    def run(self):

        while True:
            if (time.time_ns() - self.prev_time > self.state["dt"]):
                if self.state["run"]:
                    self.__sendTelemetryPacket__()
                self.prev_time = time.time_ns()
            self.__checkRecieveQueue__()
            self.__checkState__()
            time.sleep(.001)    

    def exitHandler(self,sig,frame):
        print("Telemetry Handler Exited")
        sys.exit(0)

    def __sendTelemetryPacket__(self):
        #construct command packet for telemetry
        # header = packets.Header(packet_len = packets, 0, 2, 0, source=self.state["source"], destination=self.state["destination"]) # source=4 for USB and destination=0 for rocket
        cmd_packet = packets.SimpleCommand(command=8,arg=0) # 8 for telemetry
        cmd_packet.header.source = self.state["source"]
        cmd_packet.header.destination = self.state["destination"]
        send_data = {
            "data":cmd_packet.serialize().hex(),
            "clientid":self.clientid
        }
        self.r.lpush("SendQueue",json.dumps(send_data))
        
    
    def __checkRecieveQueue__(self):
        self.r.persist("ReceiveQueue:"+str(self.clientid)) #remove key expiry as we are acsessing it
        if self.r.llen("ReceiveQueue:"+str(self.clientid)) > 0:
            #we have packets to process
            #this should return a bytes array
            received_packet : bytes = self.r.rpop("ReceiveQueue:"+str(self.clientid))
            header : packets.Header  = packets.Header.from_bytes(received_packet)
            #check the correct packet type was received
            #!!!! change packet type to telemetry packet once everything els ehas been changed properly
            if header.service == 2:
                self.lastPacketTime = time.time_ns()
                decoded_packet = packets.Telemetry.from_bytes(received_packet)
                packet_data = copy.deepcopy(vars(decoded_packet))
                #remove header from data
                packet_data.pop("header")
                packet_data["connectionstatus"] = True
                self.lastTelemetry = packet_data
                self.r.set("telemetry",json.dumps(packet_data))
            else:
                return
        elif (time.time_ns() - self.lastPacketTime) > self.packetTimeout:
            data = self.lastTelemetry
            data["connectionstatus"] = False
            self.r.set("telemetry",json.dumps(data))


    def __checkState__(self):
        state = self.r.get(self.clientid + ":STATE")
        try:
            self.state = json.loads(state)
        except:
            #if we have some error deserializing the key, place the current state back onto redis
            self.r.set(self.clientid + ":STATE", json.dumps(self.state))

    
