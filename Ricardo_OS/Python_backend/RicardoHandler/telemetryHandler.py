import redis
import multiprocessing
from RicardoHandler import packets
import time
import json


class TelemetryHandler(multiprocessing.Process):
    
    def __init__(self,updateTimePeriod = 200e6,redishost = 'localhost',redisport = 6379,clientid = "LOCAL:TELEMETRYTASK"):#default time period corresponds to 5 hz update
        
        super(TelemetryHandler,self).__init__()
        self.prev_time = 0

        self.clientid : str = clientid 
        self.updateTimePeriod = updateTimePeriod

        self.exit_event = multiprocessing.Event()
        self.r = redis.Redis(host=redishost,port=redisport)


    def run(self):
        while not self.exit_event.is_set():
            if (time.time_ns() - self.prev_time > self.updateTimePeriod):
                self.__sendTelemetryPacket__()
                self.__checkRecieveQueue__()
                self.prev_time = time.time_ns()
            time.sleep(.01)    
            
        
    def stop(self):
        self.exit_event.set()

    def __sendTelemetryPacket__(self):
        #construct command packet for telemetry
        header = packets.Header(2, 0, 2, 0, source=4, destination=0) # source=4 for USB and destination=0 for rocket
        cmd_packet = packets.Command(header, 50, 0) # 50 for detailed all
        send_data = {
            "data":cmd_packet.serialize().hex(),
            "clientid":self.clientid
        }
        self.r.lpush("SendQueue",json.dumps(send_data))
    
    def __checkRecieveQueue__(self):
        if self.r.llen("ReceiveQueue:"+str(self.clientid)) > 0:
            #we have packets to process
            #this should return a bytes array
            received_packet : bytes = self.r.rpop("ReceiveQueue:"+str(self.clientid))
            header : packets.Header  = packets.Header.from_bytes(received_packet)
            #check the correct packet type was received
            #!!!! change packet type to telemetry packet once everything els ehas been changed properly
            if header.packet_type == 3:
                decoded_packet = packets.DetailedAll.from_bytes(received_packet)
                packet_data = {
                    "packet_type" : decoded_packet.header.packet_type,
                    "src_interface" : decoded_packet.header.src_interface,
                    "packet_len" : decoded_packet.header.packet_len,
                    "system_time" : decoded_packet.header.system_time,
                    "source" : decoded_packet.header.source,
                    "destination" : decoded_packet.header.destination,
                    "ttl" : decoded_packet.header.ttl,
                    "ax" : decoded_packet.ax,
                    "ay" : decoded_packet.ay,
                    "az" : decoded_packet.az,
                    "gx" : decoded_packet.gx,
                    "gy" : decoded_packet.gy,
                    "gz" : decoded_packet.gz,
                    "mx" : decoded_packet.mx,
                    "my" : decoded_packet.my,
                    "mz" : decoded_packet.mz,
                    "gps_lat" : decoded_packet.gps_lat,
                    "gps_long" : decoded_packet.gps_long,
                    "gps_speed" : decoded_packet.gps_speed,
                    "gps_alt" : decoded_packet.gps_alt,
                    "baro_alt" : decoded_packet.baro_alt,
                    "baro_temp" : decoded_packet.baro_temp,
                    "baro_press" : decoded_packet.baro_press,
                    "batt_volt" : decoded_packet.batt_volt,
                    "batt_percent" : decoded_packet.batt_percent,
                }
                self.r.set("telemetry",json.dumps(packet_data))
            else:
                return





    