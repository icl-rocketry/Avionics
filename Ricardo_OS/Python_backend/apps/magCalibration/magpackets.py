import struct

from pylibrnp.rnppacket import *

class MagCalCommand(RnpPacket):
	
	struct_str = '<Bfffffffffffffff'
	size = struct.calcsize(struct_str)
	packet_type = 10

	def __init__(self,
				command:int = 0,
				fieldMagnitude:float=0,inclination:float=0,declination:float=0,
				A11:float=0,A12:float=0,A13:float=0,
				A21:float=0,A22:float=0,A23:float=0,
				A31:float=0,A32:float=0,A33:float=0,
				b1:float=0,b2:float=0,b3:float=0):


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

		super().__init__(list(vars(self).keys()),
						 MagCalCommand.struct_str,
						 MagCalCommand.size,
						 MagCalCommand.packet_type)

	def __str__(self):
		header_str = self.header.__str__() + '\ns'
		desc_str = f'MAG CAL COMMAND BODY: havent done this\n'
		return header_str


