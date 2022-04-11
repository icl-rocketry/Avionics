import struct
from pylibrnp.rnppacket import RnpPacket

class TelemetryPacket(RnpPacket):
    '''Telemetry Packer, Also shows how vars can be used to obtain the structure of the packet. 
    Constructor parameters also do not matter to serialization/deserialzation'''
	
    struct_str = '<ffffffffffffffffffffflBffffffffffffHHfflLQhf'
    size = struct.calcsize(struct_str)
    packet_type = 0
   
    def __init__(self):

        self.pn:float = 0
        self.pe:float = 0
        self.pd:float = 0
        self.vn:float = 0
        self.ve:float = 0
        self.vd:float = 0
        self.an:float = 0
        self.ae:float = 0
        self.ad:float = 0
        self.roll:float = 0
        self.pitch:float = 0
        self.yaw:float = 0
        self.q0:float = 0
        self.q1:float = 0
        self.q2:float = 0
        self.q3:float = 0
        self.lat:float = 0
        self.lng:float = 0
        self.alt:int = 0
        self.sat:int = 0
        self.ax:float = 0
        self.ay:float = 0
        self.az:float = 0
        self.h_ax:float = 0
        self.h_ay:float = 0
        self.h_az:float = 0
        self.gx:float = 0
        self.gy:float = 0
        self.gz:float = 0
        self.mx:float = 0
        self.my:float = 0
        self.mz:float = 0
        self.baro_temp:float = 0
        self.baro_press:float = 0
        self.baro_alt:float = 0
        self.batt_voltage:int = 0
        self.batt_percent:int = 0
        self.launch_lat:float = 0
        self.launch_lng:float = 0
        self.launch_alt:int = 0
        self.system_status:int = 0
        self.system_time:int = 0
        self.rssi:int = 0
        self.snr:float = 0

        super().__init__(list(vars(self).keys()),
                         TelemetryPacket.struct_str,
                         TelemetryPacket.size,
                         TelemetryPacket.packet_type)

    def __str__(self):
        header_str = self.header.__str__() + '\ns'
        desc_str = f'TELEMETRY PACKET BODY: Havent done this yet oops\n'
        return header_str

class SimpleCommandPacket(RnpPacket):
    '''Simple Command Packet, shows how to manually add keys to the member variables in the packet'''
    struct_str = '<BI'
    size = struct.calcsize(struct_str)
    packet_type = 0

    def __init__(self, command: int = 0, arg: int = 0):

        self.command = command
        self.arg = arg

        super().__init__(['command','arg'],
                         SimpleCommandPacket.struct_str,
                         SimpleCommandPacket.size,
                         SimpleCommandPacket.packet_type)

    def __str__(self):
        header_str = self.header.__str__() + "\n"
        param_str = f'SIMPLE COMMAND PACKET BODY: \tcommand = {self.command}\n \t\t\targument = {self.arg}\n'
        return header_str + param_str