print(__name__)
print(__package__)
print(__file__)

from re import A
from pylibrnp.defaultpackets import *
from pylibrnp.rnppacket import *
import random
# import ..pylibrnp.defaultpackets
# import ..pylibrnp.rnppacket

class PickleRickRawSensors(RnpPacket):
    '''Telemetry Packer, Also shows how vars can be used to obtain the structure of the packet. 
    Constructor parameters also do not matter to serialization/deserialzation'''
	
    struct_str = '<fffffffffffffffffllllBHBBBBBBBB'
    size = struct.calcsize(struct_str)
    packet_type = 1
    
    def __init__(self):

        self.ax:float = 0 
        self.ay:float = 0
        self.az:float = 0
        self.gx:float = 0
        self.gy:float = 0
        self.gz:float = 0
        self.h_ax:float = 0
        self.h_ay:float = 0
        self.h_az:float = 0
        self.mx:float = 0
        self.my:float = 0
        self.mz:float = 0
        self.baro_alt:float = 0
        self.baro_temp:float = 0
        self.baro_press:float = 0
        self.gps_lat:float = 0
        self.gps_lng:float = 0
        self.gps_alt:int = 0
        self.gps_v_n:int = 0
        self.gps_v_e:int = 0
        self.gps_v_d:int = 0
        self.gps_sat:int = 0                    
        self.gps_pdop:int = 0              
        self.gps_fix:int = 0  
        self.gps_updated:int = 0 
        self.gps_valid:int = 0
        self.imu_error:int = 0
        self.haccel_error:int = 0
        self.mag_error:int = 0
        self.baro_error:int = 0
        self.gps_error:int = 0


        super().__init__(list(vars(self).keys()),
                         PickleRickRawSensors.struct_str,
                         PickleRickRawSensors.size,
                         PickleRickRawSensors.packet_type)

    def __str__(self):
        header_str = self.header.__str__() + '\ns'
        desc_str = f'TELEMETRY PACKET BODY: Havent done this yet oops\n'
        return header_str




import socketio
import argparse



sio = socketio.Client(logger=False, engineio_logger=False)


@sio.event
def connect():
    print("I'm connected!")

@sio.event
def connect_error(data):
    print("The connection failed!")

@sio.event
def disconnect():
    print("I'm disconnected!")

@sio.on('Response',namespace='/command')
def on_response_handler(data):
    print(data)
    try:
        header = RnpHeader.from_bytes(bytes.fromhex(data['Data']))
        print(header)
    except:
        print("Failed to decode header")

@sio.on('Error',namespace='/command')
def on_error_handler(data):
    print(data)


    
if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", required=False, help="backend host", type=str,default = "localhost")
    ap.add_argument("--port", required=False, help="backend port", type=int,default = 1337)
    args = vars(ap.parse_args())

    sio.connect('http://' + args["host"] + ':' + str(args['port']) + '/',namespaces=['/','/telemetry','/command'])
    ay = 0
    while True:
        

        source = input("press enter to send data")

        packet = PickleRickRawSensors()
        packet.header.destination_service = 3
        packet.header.source_service = 1
        packet.header.destination = 0
        packet.header.source = 1





        serializedPacket:str = packet.serialize().hex()
        sio.emit('send_data',{'data':serializedPacket},namespace='/command')
    