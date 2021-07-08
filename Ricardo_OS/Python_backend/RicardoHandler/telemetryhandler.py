import redis
import multiprocessing
from RicardoHandler import packets
import time
import json


class TelemetryHandler(multiprocessing.Process):
    
    def __init__(self,updateTimePeriod = 800e6,redishost = 'localhost',redisport = 6379,clientid = "LOCAL:TELEMETRYTASK"):
        
        super(TelemetryHandler,self).__init__()
        self.prev_time = 0

        self.lastPacketTime = 0
        self.packetTimeout = 1e10
        self.lastTelemetry = {}


        self.state = {
            "run":True,
            "source":2,
            "destination":0,
            "dt":updateTimePeriod,
        }

        self.clientid : str = clientid 
        self.updateTimePeriod = updateTimePeriod

        self.exit_event = multiprocessing.Event()
        self.r = redis.Redis(host=redishost,port=redisport)
        self.r.set(clientid + ":STATE",json.dumps(self.state))


    def run(self):
        while not self.exit_event.is_set():
            if (time.time_ns() - self.prev_time > self.state["dt"]):
                if self.state["run"]:
                    self.__sendTelemetryPacket__()
                self.prev_time = time.time_ns()
            self.__checkRecieveQueue__()
            self.__checkState__()
            time.sleep(.001)    
            
    def get_id(self):
        return self.clientid

    def stop(self):
        self.exit_event.set()

    def __sendTelemetryPacket__(self):
        #construct command packet for telemetry
        header = packets.Header(2, 0, 2, 0, source=self.state["source"], destination=self.state["destination"]) # source=4 for USB and destination=0 for rocket
        cmd_packet = packets.Command(header, 8, 0) # 8 for telemetry
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
            if header.packet_type == 1:
                self.lastPacketTime = time.time_ns()
                decoded_packet = packets.Telemetry.from_bytes(received_packet)
                packet_data = vars(decoded_packet)
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

    
