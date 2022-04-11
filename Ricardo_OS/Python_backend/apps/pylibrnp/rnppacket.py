import struct
import copy

class DeserializationError(Exception):
    pass

class RnpHeader:
	
    struct_str='<BHHBBBBBB'
    size = struct.calcsize(struct_str)
    _start_byte = 0xAF
	

    def __init__(self,packet_len:int = 0,uid:int = 0,
				source_service:int = 0,destination_service:int = 0,
				packet_type:int = 0,
				source:int = 0, destination:int = 0, 
				hops:int = 0):

        self.start_byte = RnpHeader._start_byte
        self.packet_len = packet_len
        self.uid = uid
        self.source_service = source_service
        self.destination_service = destination_service
        self.packet_type = packet_type
        self.source = source
        self.destination = destination
        self.hops = hops

    @classmethod
    def from_bytes(cls,data:bytes):

        if len(data) < cls.size:
            raise DeserializationError("Not enough Bytes to deserialize, received: " + str(len(data)) + " expected: " + str(cls.size))
        
        header_data = data[:cls.size]
        variable_list = struct.unpack(cls.struct_str,header_data)

        if (variable_list[0] != RnpHeader._start_byte):
            raise DeserializationError("Header Start Byte Mismatch, received: " + hex(variable_list[0]))

        variable_list = variable_list[1:] #remove start byte

        return cls(*variable_list)
  

    def serialize(self) -> bytes:

        member_variables = copy.deepcopy(vars(self))

        packet_bytes = struct.pack(RnpHeader.struct_str,*(member_variables.values()))

        return bytes(packet_bytes)

    def __str__(self):
        return f'HEADER:\n\tstart byte = {self.start_byte}\n\tpacket len = {self.packet_len}\n\tuid = {self.uid}\n\tsource_service = {self.source_service}\n\tdestination_service = {self.destination_service}\n\tpacket type = {self.packet_type}\n\tsource = {self.source}\n\tdestination = {self.destination}\n\thops = {self.hops}'



class RnpPacket():

    def __init__(self,packetvars:list=[],struct_str:str = '<',packet_size = 0,packet_type:int=0):

        #list of keys of member variables used in the payload
        self.packetvars = packetvars
        #description of c-struct 
        self.struct_str = struct_str

        self.packet_size = packet_size
        self.packet_type = packet_type

        
        self.header = RnpHeader(packet_len=self.packet_size,packet_type=self.packet_type)
    
    @classmethod
    def from_bytes(cls,data:bytes):
        '''Default deserialization function using struct str to deserialize a simple c struct. 
            Override this in derived class if a more specialized deserialization is required'''
        if cls is RnpPacket:
            raise DeserializationError("from_bytes method called on base class!")

        packet = cls()
        packet.header = RnpHeader.from_bytes(data) #decode header
        packetData = data[RnpHeader.size:] #extract packet data from bytes


        if len(packetData) is not packet.packet_size:
            raise DeserializationError("Received Packet Length Mismatch, received:" + str(len(packetData)) + " expected:" + str(cls().packet_size))
        
        packetvars = struct.unpack(packet.struct_str,packetData)
        packetvars_iter = iter(packetvars)

        membervars = vars(packet)

        #copy received values into member variables
        for key in packet.packetvars:
            if type(key) is not str:
                raise DeserializationError("Key is not a string")
            try:
                membervars[key] = copy.deepcopy(next(packetvars_iter))
            except StopIteration:
                raise DeserializationError("Not enough data")
        
        return packet
    
    def serialize(self) -> bytes:
        data_arr:bytearray = bytearray(self.header.serialize())

        # get list of values of member varaibles for payload and deep copy them
        payload:list = copy.deepcopy([vars(self)[key] for key in self.packetvars])
        payload_bytes:bytearray = struct.pack(self.struct_str,*payload)
        data_arr += payload_bytes
        return bytes(data_arr)
            


