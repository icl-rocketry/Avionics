import struct

class Header:
	start_byte = 0xAF
	header_size = 14 # WARNING: This has to match with the size in packets.h!!!!
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

	@classmethod
	def from_bytes(cls, data: bytes): # Deserialize Header from a bytearray
		obj = cls.__new__(cls)  # Does not call __init__
		super(Header, obj).__init__()
		obj.deserialize(data)
		return obj

	def serialize(self) -> bytes:
		arr = [(self.start_byte, 1), (self.packet_len, 4), # Array with the integer variable and their number of bytes
				(self.system_time, 4), (self.packet_type, 1),
				(self.source, 1), (self.destination, 1),
				(self.src_interface, 1), (self.ttl, 1)]
		bytearr = bytearray()

		for item in arr:
			byte_data = item[0].to_bytes(item[1], 'big')
			for i in range(item[1]):
				bytearr.append(byte_data[i])

		return bytes(bytearr)

	def deserialize(self, data: bytes):
		i = 0
		for byte in data:
			if i == 0 and byte == self.start_byte:
				# We found the start byte, increment i
				i += 1
			elif i == 1:
				self.packet_len = int.from_bytes(data[i:i+4], 'big', signed=False)
				i += 4 # increment by number of bytes read
			elif i == 5:
				self.system_time = int.from_bytes(data[i:i+4], 'big', signed=False)
				i += 4 # increment by number of bytes read
			elif i == 9:
				self.packet_type = data[i]
				i += 1
			elif i == 10:
				self.source = data[i]
				i += 1
			elif i == 11:
				self.destination = data[i]
				i += 1
			elif i == 12:
				self.src_interface = data[i]
				i += 1
			elif i == 13:
				self.ttl = data[i]
				i += 1
			elif i == 14:
				break
			else:
				print('Loop variable not caught by any statements, check Header.deserialize method')

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

	@classmethod
	def from_bytes(cls, data: bytes): # Deserialize from a bytearray
		obj = cls.__new__(cls)  # Does not call __init__
		super(Telemetry, obj).__init__()
		obj.deserialize(data)
		return obj
	
	def serialize(self) -> bytes:
		data_byte_arr = bytearray(self.header.serialize())

		float_vars = [self.x, self.y, self.z,
					  self.ax, self.ay, self.az,
					  self.vx, self.vy, self.vz]
		
		for var in float_vars:
			byte_data = struct.pack('>f', var)
			for byte in byte_data:
				data_byte_arr.append(byte)
		data_byte_arr.append(self.lora_rssi.to_bytes(1, 'big')[0])

		return bytes(data_byte_arr)
	
	def deserialize(self, data: bytes):
		self.header = Header.from_bytes(data)
		telemetry_data = data[Header.header_size:] # Drop the first n bytes beloning to header
		
		self.x, self.y, self.z = struct.unpack('>fff', telemetry_data[:4*3]) # Unpack first 12 bytes
		self.ax, self.ay, self.az = struct.unpack('>fff', telemetry_data[4*3:4*3*2])
		self.vx, self.vy, self.vz = struct.unpack('>fff', telemetry_data[4*3*2:4*3*3])
		
		self.lora_rssi = telemetry_data[4*3*3]

class DetailedAll(Packet):

	def __init__(self, header: Header,
				ax: float, ay: float, az: float,
				gx: float, gy: float, gz: float,
				mx: float, my: float, mz: float,
				gps_lat: float, gps_long: float,
				gps_speed: float, gps_alt: float,
				baro_alt: float, baro_temp: float,
				baro_press: float, batt_volt: int,
				batt_percent: int):

		self.header = header

		self.ax = ax
		self.ay = ay
		self.az = az

		self.gx = gx
		self.gy = gy
		self.gz = gz
		
		self.mx = mx
		self.my = my
		self.mz = mz

		self.gps_lat = gps_lat
		self.gps_long = gps_long
		self.gps_speed = gps_speed
		self.gps_alt = gps_alt

		self.baro_alt = baro_alt
		self.baro_temp = baro_temp
		self.baro_press = baro_press
		
		self.batt_volt = batt_volt
		self.batt_percent = batt_percent

	@classmethod
	def from_bytes(cls, data: bytes): # Deserialize from bytes
		obj = cls.__new__(cls)  # Does not call __init__
		super(DetailedAll, obj).__init__()
		obj.deserialize(data)
		return obj

	def serialize(self) -> bytes:
		data_byte_arr = bytearray(self.header.serialize())

		float_vars = [self.ax, self.ay, self.az,
					  self.gx, self.gy, self.gz,
					  self.mx, self.my, self.mz,
					  self.gps_lat, self.gps_long,
					  self.gps_speed, self.gps_alt,
					  self.baro_alt, self.baro_temp,
					  self.baro_press]
		for var in float_vars:
			byte_data = struct.pack('>f', var)
			for byte in byte_data:
				data_byte_arr.append(byte)
		
		int_vars = [self.batt_volt, self.batt_percent] # 8-bit integer variables
		for item in int_vars:
			byte_data = item.to_bytes(1, 'big')
			data_byte_arr.append(byte_data[0])
		
		return bytes(data_byte_arr)
	
	def deserialize(self, data: bytes):
		self.header = Header.from_bytes(data)
		detailed_data = data[Header.header_size:] # Drop the first n bytes beloning to header
		
		self.ax, self.ay, self.az = struct.unpack('>fff', detailed_data[:4*3]) # Unpack first 12 bytes
		self.gx, self.gy, self.gz = struct.unpack('>fff', detailed_data[4*3:4*3*2])
		self.mx, self.my, self.mz = struct.unpack('>fff', detailed_data[4*3*2:4*3*3])
		self.gps_lat, self.gps_long, self.gps_speed, self.gps_alt = struct.unpack('>ffff', detailed_data[4*3*3:4*3*3 + 4*4])
		self.baro_alt, self.baro_temp, self.baro_press = struct.unpack('>fff', detailed_data[4*3*3 + 4*4:4*4*4])

		self.batt_volt = detailed_data[4*4*4]
		self.batt_percent = detailed_data[4*4*4 + 1]
		


class Command(Packet):
	
	def __init__(self, header: Header, command: int, arg: int):
		self.header = header

		self.command = command
		self.arg = arg

	@classmethod
	def from_bytes(cls, data: bytes): # Deserialize from bytes
		obj = cls.__new__(cls)  # Does not call __init__
		super(Command, obj).__init__()
		obj.deserialize(data)
		return obj

	def serialize(self) -> bytes:
		data_byte_arr = bytearray(self.header.serialize())

		int_vars = [self.command, self.arg] # 8-bit integer variables
		for item in int_vars:
			byte_data = item.to_bytes(1, 'big')
			data_byte_arr.append(byte_data[0])
		
		return bytes(data_byte_arr)
		

	def deserialize(self, data: bytes):
		self.header = Header.from_bytes(data)
		command_data = data[Header.header_size:]

		self.command = command_data[0]
		self.arg = command_data[1]
