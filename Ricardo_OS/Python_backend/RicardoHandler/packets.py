import struct



class Header:
	start_byte = 0xAF
	header_size = 15 # WARNING: This has to match with the size in packets.h!!!!
	

	def __init__(self, packet_type: int,
				src_interface: int = 0, packet_len: int = 0,
				uid: int = 0, source: int = 0,
				destination: int = 0, ttl: int = 10):

		
		self.packet_len = packet_len
		self.uid = uid
		self.packet_type = packet_type
		self.source = source
		self.destination = destination
		self.src_interface = src_interface
		self.ttl = ttl

	@classmethod
	def from_bytes(cls, data: bytes): # Deserialize Header from a bytearray
		obj = cls.__new__(cls)  # Does not call __init__
		super(Header, obj).__init__()
		obj.deserialize(data)
		return obj

	def serialize(self) -> bytes:
		arr = [(self.start_byte, 1), (self.header_size,1),  (self.packet_len, 4), # Array with the integer variable and their number of bytes
				(self.uid, 4), (self.packet_type, 1),
				(self.source, 1), (self.destination, 1),
				(self.src_interface, 1), (self.ttl, 1)]
		bytearr = bytearray()

		for item in arr:
			byte_data = item[0].to_bytes(item[1], 'little')
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
				self.header_size = data[i]
				i += 1
			elif i == 2:
				self.packet_len = int.from_bytes(data[i:i+4], 'little', signed=False)
				i += 4 # increment by number of bytes read
			elif i == 6:
				self.uid = int.from_bytes(data[i:i+4], 'little', signed=False)
				i += 4 # increment by number of bytes read
			elif i == 10:
				self.packet_type = data[i]
				i += 1
			elif i == 11:
				self.source = data[i]
				i += 1
			elif i == 12:
				self.destination = data[i]
				i += 1
			elif i == 13:
				self.src_interface = data[i]
				i += 1
			elif i == 14:
				self.ttl = data[i]
				i += 1
			elif i == 15:
				break
			else:
				print('Loop variable not caught by any statements, check Header.deserialize method')

	
	def __str__(self):
		return f'HEADER:\n\tpacket type = {self.packet_type}\n\tpacket_len = {self.packet_len}\n\tunique_id = {self.uid}\n\tsource = {self.source}\n\tdestination = {self.destination}\n\tttl = {self.ttl}\n'


class Packet:
	pass

class Telemetry(Packet):
	
	def __init__(self, header: Header,
				pn: float = 0, pe: float = 0, pd: float = 0,
				vn: float = 0, ve: float = 0, vd: float = 0,
				an: float = 0, ae: float = 0, ad: float = 0,
				roll: float = 0, pitch: float = 0, yaw: float = 0,
				q0: float = 0, q1: float = 0, q2: float = 0, q3:float = 0,
				lat: int = 0, lng: int = 0, alt: int = 0, sat: int = 0,
				ax: float = 0, ay: float = 0, az: float = 0,
				gx: float = 0, gy: float = 0, gz: float = 0,
				mx: float = 0, my: float = 0, mz: float = 0,
				temp: float = 0, press: float = 0,
				batt_voltage: int = 0, batt_percent: int = 0,
				launch_lat: int = 0, launch_lng: int = 0, launch_alt: int = 0,
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
		self.temp = temp
		self.press = press
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
		telemetry_data = data[Header.header_size:] # Drop the first n bytes beloning to header
		variable_list = struct.unpack('<fffffffffffffffflllBfffffffffffHHlllIQhf',telemetry_data)
		obj = Telemetry(header,*variable_list)
		
		return obj
	
	def serialize(self) -> bytes:
		data_byte_arr = bytearray(self.header.serialize())

		member_variables = vars(self)
		#remove header variable
		member_variables.pop("header")
		
		packet_bytes = struct.pack('<fffffffffffffffflllBfffffffffffHHlllIQhf',*(member_variables.values()))

		data_byte_arr += packet_bytes

		return bytes(data_byte_arr)
	
	# def deserialize(self, data: bytes):
	# 	self.header = Header.from_bytes(data)
	# 	telemetry_data = data[Header.header_size:] # Drop the first n bytes beloning to header		
	# 	variable_list = struct.unpack('<ffffffffffffqqqBfffffffffffHHqqqIQhf',telemetry_data)
		

	def __str__(self):
		header_str = self.header.__str__() + '\ns'
		desc_str = f'TELEMETRY PACKET BODY: \t(x, y, z)=({self.x:.3g}, {self.y:.3g}, {self.z:.3g})\n\t\t(ax, ay, az)=({self.ax:.3g}, {self.ay:.3g}, {self.az:.3g})\n\t\t(x, y, z)=({self.vx:.3g}, {self.vy:.3g}, {self.vz:.3g})\n\t\tlora rssi = {self.lora_rssi}\n'
		return header_str + desc_str

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
			byte_data = struct.pack('<f', var)
			for byte in byte_data:
				data_byte_arr.append(byte)
		
		int_vars = [self.batt_volt, self.batt_percent] # 8-bit integer variables
		for item in int_vars:
			byte_data = item.to_bytes(1, 'little')
			data_byte_arr.append(byte_data[0])
		
		return bytes(data_byte_arr)
	
	def deserialize(self, data: bytes):
		self.header = Header.from_bytes(data)
		detailed_data = data[Header.header_size:] # Drop the first n bytes beloning to header
		
		self.ax, self.ay, self.az = struct.unpack('<fff', detailed_data[:4*3]) # Unpack first 12 bytes
		self.gx, self.gy, self.gz = struct.unpack('<fff', detailed_data[4*3:4*3*2])
		self.mx, self.my, self.mz = struct.unpack('<fff', detailed_data[4*3*2:4*3*3])
		self.gps_lat, self.gps_long, self.gps_speed, self.gps_alt = struct.unpack('<ffff', detailed_data[4*3*3:4*3*3 + 4*4])
		self.baro_alt, self.baro_temp, self.baro_press = struct.unpack('<fff', detailed_data[4*3*3 + 4*4:4*4*4])

		self.batt_volt = detailed_data[4*4*4]
		self.batt_percent = detailed_data[4*4*4 + 1]
	
	def __str__(self):
		header_str = self.header.__str__() + '\n'
		position_str = f'DETAILED PACKET BODY: \t(ax, ay, az)=\t({self.ax:.3g}, {self.ay:.3g}, {self.az:.3g})\n\t\t\t(gx, gy, gz)=\t({self.gx:.3g}, {self.gy:.3g}, {self.gz:.3g})\n\t\t\t(mx, my, mz)=\t({self.mx:.3g}, {self.my:.3g}, {self.mz:.3g})\n'
		gps_str = f'\t\t\t(gps_lat, gps_long)=\t({self.gps_lat:.5g}, {self.gps_long:.5g})\n\t\t\t(gps_speed, gps_alt)=\t({self.gps_speed:.5g}, {self.gps_alt:.5g})\n'
		baro_str = f'\t\t\t(baro_alt, baro_temp, baro_press)=\t({self.baro_alt:.5g}, {self.baro_temp:.4g}, {self.baro_press:.8g})\n'
		battery_str = f'\t\t\t(bat_voltage, batt_percent)=\t({self.batt_volt:.5g}, {self.batt_percent:.3g})'
		return header_str + position_str + gps_str + baro_str + battery_str
		


class Command(Packet):
	
	def __init__(self, header: Header, command: int, arg: int = 0):
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
			byte_data = item.to_bytes(1, 'little')
			data_byte_arr.append(byte_data[0])
		
		return bytes(data_byte_arr)
	
	def __str__(self):
		header_str = self.header.__str__() + "\n"
		param_str = f'COMMAND PACKET BODY: \tcommand = {self.command}\n \t\t\targument = {self.arg}\n'
		return header_str + param_str

	def deserialize(self, data: bytes):
		self.header = Header.from_bytes(data)
		command_data = data[Header.header_size:]

		self.command = command_data[0]
		self.arg = command_data[1]
