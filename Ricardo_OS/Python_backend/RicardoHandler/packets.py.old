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

class Telemetry(Packet):
	
	struct_str = '<fffffffffffffffffflBffffffffffffHHfflLQhf'
	size = struct.calcsize(struct_str)
	service = 3
	packet_type = 0

	def __init__(self, header: Header = Header(packet_len=size,destination_service=service,packet_type=packet_type),
				pn: float = 0, pe: float = 0, pd: float = 0,
				vn: float = 0, ve: float = 0, vd: float = 0,
				an: float = 0, ae: float = 0, ad: float = 0,
				roll: float = 0, pitch: float = 0, yaw: float = 0,
				q0: float = 0, q1: float = 0, q2: float = 0, q3:float = 0,
				lat: float = 0, lng: float = 0, alt: int = 0, sat: int = 0,
				ax: float = 0, ay: float = 0, az: float = 0,
				gx: float = 0, gy: float = 0, gz: float = 0,
				mx: float = 0, my: float = 0, mz: float = 0,
				baro_temp: float = 0, baro_press: float = 0,baro_alt: float = 0,
				batt_voltage: int = 0, batt_percent: int = 0,
				launch_lat: float = 0, launch_lng: float = 0, launch_alt: int = 0,
				system_status: int = 0,
				system_time: int = 0,
				rssi: int = 0, snr: float = 0):

		self.header = header

		self.pn = pn
		self.pe = pe
		self.pd = pd
		self.vn = vn
		self.ve = ve
		self.vd = vd
		self.an = an
		self.ae = ae
		self.ad = ad
		self.roll = roll
		self.pitch = pitch
		self.yaw = yaw
		self.q0 = q0
		self.q1 = q1
		self.q2 = q2
		self.q3 = q3
		self.lat = lat
		self.lng = lng
		self.alt = alt
		self.sat = sat
		self.ax = ax
		self.ay = ay
		self.az = az
		self.gx = gx
		self.gy = gy
		self.gz = gz
		self.mx = mx
		self.my = my
		self.mz = mz
		self.baro_temp = baro_temp
		self.baro_press = baro_press
		self.baro_alt = baro_alt
		self.batt_voltage = batt_voltage
		self.batt_percent = batt_percent
		self.launch_lat = launch_lat
		self.launch_lng = launch_lng
		self.launch_alt = launch_alt
		self.system_status = system_status
		self.system_time = system_time
		self.rssi = rssi
		self.snr = snr



	@staticmethod
	def from_bytes(data: bytes): # Deserialize from a bytearray
		header = Header.from_bytes(data)
		telemetry_data = data[Header.size:] # Drop the first n bytes beloning to header
		variable_list = struct.unpack(Telemetry.struct_str,telemetry_data)
		obj = Telemetry(header,*variable_list)
		
		return obj
	
	def serialize(self) -> bytes:
		data_byte_arr = bytearray(self.header.serialize())

		member_variables = copy.deepcopy(vars(self))
		#remove header variable
		member_variables.pop("header")
		
		packet_bytes = struct.pack(Telemetry.struct_str,*(member_variables.values()))

		data_byte_arr += packet_bytes

		return bytes(data_byte_arr)

	def __str__(self):
		header_str = self.header.__str__() + '\ns'
		desc_str = f'TELEMETRY PACKET BODY: Havent done this yet oops\n'
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
		command_data = data[Header.size:] # Drop the first n bytes beloning to header
		variable_list = struct.unpack(SimpleCommand.struct_str,command_data)
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



