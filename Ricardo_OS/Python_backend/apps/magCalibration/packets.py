import struct
import copy

class DeserializeError(Exception):
	pass
class Header:
	
	struct_str='<BHHBBBBBB'
	size = struct.calcsize(struct_str)
	_start_byte = 0xAF
	

	def __init__(self,packet_len:int = 0,uid:int = 0,
				source_service:int = 0,destination_service:int = 0,
				packet_type:int = 0,
				source:int = 0, destination:int = 0, 
				hops:int = 0):

		self.start_byte = Header._start_byte
		self.packet_len = packet_len
		self.uid = uid
		self.source_service = source_service
		self.destination_service = destination_service
		self.packet_type = packet_type
		self.source = source
		self.destination = destination
		self.hops = hops

	@staticmethod
	def from_bytes(data: bytes): # Deserialize from a bytearray

		header_data = data[:Header.size]
		variable_list = struct.unpack(Header.struct_str,header_data)
		if (variable_list[0] != Header._start_byte):
			raise DeserializeError("Header Start Byte Mismatch!!!")
		variable_list = variable_list[1:] #remove start byte
		
		obj = Header(*variable_list)
		
		return obj
	
	def serialize(self) -> bytes:

		member_variables = copy.deepcopy(vars(self))
	
		packet_bytes = struct.pack(Header.struct_str,*(member_variables.values()))

		return bytes(packet_bytes)
	
	def __str__(self):
		return f'HEADER:\n\tstart byte = {self.start_byte}\n\tpacket len = {self.packet_len}\n\tuid = {self.uid}\n\tsource_service = {self.source_service}n\tdestination_service = {self.destination_service}\n\tpacket type = {self.packet_type}\n\tsource = {self.source}\n\tdestination = {self.destination}\n\thops = {self.hops}'


class Packet:
	pass

class MagCalCommand(Packet):
	
	struct_str = '<Bfffffffffffffff'
	size = struct.calcsize(struct_str)
	service = 2
	packet_type = 10

	def __init__(self, header: Header = Header(packet_len=size,destination_service=service,packet_type=packet_type),
				command:int = 0,
				fieldMagnitude:float=0,inclination:float=0,declination:float=0,
				A11:float=0,A12:float=0,A13:float=0,
				A21:float=0,A22:float=0,A23:float=0,
				A31:float=0,A32:float=0,A33:float=0,
				b1:float=0,b2:float=0,b3:float=0):

		self.header = header

		self.command = command

		self.fieldMagnitude = fieldMagnitude
		self.inclination = inclination
		self.declination = declination
		self.A11 = A11
		self.A12 = A12
		self.A13 = A13
		self.A21 = A21
		self.A22 = A22
		self.A23 = A23
		self.A31 = A31
		self.A32 = A32
		self.A33 = A33
		self.b1 = b1
		self.b2 = b2
		self.b3 = b3


	@staticmethod
	def from_bytes(data: bytes): # Deserialize from a bytearray
		header = Header.from_bytes(data)
		telemetry_data = data[Header.size:] # Drop the first n bytes beloning to header
		variable_list = struct.unpack(MagCalCommand.struct_str,telemetry_data)
		obj = MagCalCommand(header,*variable_list)
		
		return obj
	
	def serialize(self) -> bytes:
		data_byte_arr = bytearray(self.header.serialize())

		member_variables = copy.deepcopy(vars(self))
		#remove header variable
		member_variables.pop("header")
		
		packet_bytes = struct.pack(MagCalCommand.struct_str,*(member_variables.values()))

		data_byte_arr += packet_bytes

		return bytes(data_byte_arr)

	def __str__(self):
		header_str = self.header.__str__() + '\ns'
		desc_str = f'MAG CAL COMMAND BODY: havent done this\n'
		return header_str



class SimpleCommand(Packet):
	
	struct_str = '<BI'
	size = struct.calcsize(struct_str)
	service = 2
	packet_type = 0

	def __init__(self, header: Header= Header(packet_len=size,destination_service=service,packet_type=packet_type), command: int = 0, arg: int = 0):

		self.header = header

		self.command = command
		self.arg = arg



	@staticmethod
	def from_bytes(data: bytes): # Deserialize from a bytearray
		header = Header.from_bytes(data)
		telemetry_data = data[Header.size:] # Drop the first n bytes beloning to header
		variable_list = struct.unpack(SimpleCommand.struct_str,telemetry_data)
		obj = SimpleCommand(header,*variable_list)
		
		return obj
	
	def serialize(self) -> bytes:
		data_byte_arr = bytearray(self.header.serialize())

		member_variables = copy.deepcopy(vars(self))
		#remove header variable
		member_variables.pop("header")
		
		packet_bytes = struct.pack(SimpleCommand.struct_str,*(member_variables.values()))

		data_byte_arr += packet_bytes

		return bytes(data_byte_arr)

	def __str__(self):
		header_str = self.header.__str__() + "\n"
		param_str = f'SIMPLE COMMAND PACKET BODY: \tcommand = {self.command}\n \t\t\targument = {self.arg}\n'
		return header_str + param_str



