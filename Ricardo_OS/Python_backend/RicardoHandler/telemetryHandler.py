import threading
import RicardoHandler.packets as packets
import time
import RicardoHandler.serialManager as serialManager

class TelemetryHandler(threading.Thread):
    
    def __init__(self,sm : serialManager.SerialManager,updateTimePeriod = 200e6):#default time period corresponds to 5 hz update

        self.prev_time = 0

        self.sm = sm # class instance of serial manager
        self.updateTimePeriod = updateTimePeriod

        self.exit_event = threading.Event()
        super().__init__()

    def run(self):
        while not self.exit_event.is_set():
            if (time.time_ns() - self.prev_time > self.updateTimePeriod):
                self._sendTelemetryPacket()
                self.prev_time = time.time_ns()
            
        
    def stop(self):
        self.exit_event.set()

    def _sendTelemetryPacket(self):
        #construct command packet for telemetry
        header = packets.Header(2, 0, 2, 0, source=2, destination=0) # source=4 for USB and destination=0 for rocket
        cmd_packet = packets.Command(header, 50, 0) # 50 for detailed all
        self.nm.sendPacket(cmd_packet.serialize())#send packet



    