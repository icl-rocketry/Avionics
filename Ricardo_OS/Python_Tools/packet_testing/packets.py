import struct

class Header:
	start_byte = 0xAF 
	#ttl = 10 # Time to live

	def __init__(self, packet_type: int,
				src_interface: int = 0, packet_len: int = 0,
				system_time: int = 0, source: int = 0,
				destination: int = 0, ttl: int = 10):
		
		self.packet_type = packet_type
		self.src_interface = src_interface
		self.packet_len = packet_len
		self.system_time = system_time
		self.source = source
		self.destination = destination
		self.ttl = ttl
	
	def serialize(self) -> bytearray:
		arr = [(self.start_byte, 1), (self.packet_len, 4),
				(self.system_time, 4), (self.packet_type, 1),
				(self.source, 1), (self.destination, 1),
				(self.src_interface, 1), (self.ttl, 1)]
		bytearr = bytearray()

		for item in arr:
			byte_data = item[0].to_bytes(item[1], 'big')
			for i in range(item[1]):
				bytearr.append(byte_data[i])

		return bytearr

	def deserialize(self, data: bytearray):
		# TODO
		pass


class Packet:
	pass


class Telemetry(Packet):
	
	def __init__(self, header: Header,
				x: float, y: float, z: float,
				vx: float, vy: float, vz: float,
				ax: float, ay: float, az: float, lora_rssi: int):

		self.header = header

		self.x = x
		self.y = y
		self.z = z
		
		self.vx = vx
		self.vy = vy
		self.vz = vz

		self.ax = ax
		self.ay = ay
		self.az = az

		self.lora_rssi = lora_rssi
	
	def serialize(self) -> bytearray:
		data_byte_arr = self.header.serialize()

		float_vars = [self.x, self.y, self.z,
					  self.ax, self.ay, self.az,
					  self.vx, self.vy, self.vz]
		
		for var in float_vars:
			byte_data = struct.pack('>f', var)
			for byte in byte_data:
				data_byte_arr.append(byte)
		data_byte_arr.append(self.lora_rssi.to_bytes(1, 'big')[0])

		return data_byte_arr
	
	def deserialize(self, data: bytearray):
		# TODO
		pass


class DetailedAll(Packet):
	pass

class Command(Packet):
	pass