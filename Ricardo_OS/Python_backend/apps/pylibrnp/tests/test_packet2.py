
from packets.rnppacket import RnpPacket
from packets.defaultpackets import *

c = TelemetryPacket(10,3,23,4,5,566,5,4,3,2,3,4,5,6,7,8,7,6,5,5,6,7,8)
c.header.source_service = 2
c.header.destination_service = 4
c.header.source = 3
c.header.destination = 10

print(c)
b = c.serialize()

print(c)
print(b.hex())


dec = TelemetryPacket.from_bytes(b)
print(dec)

print(b == dec.serialize())
