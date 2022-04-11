from packets.rnppacket import RnpPacket

import struct

class testpacket(RnpPacket):

    struct_str = '<ifi'
    packet_size = struct.calcsize('<ifi')
    
    def __init__(self):
        self.a:int = 1
        self.b:float = 2.02
        self.c:int  = 300
        # super().__init__(list(vars(self).values()),testpacket.struct_str,testpacket.packet_size)
        super().__init__(list(vars(self).keys()),testpacket.struct_str,testpacket.packet_size)
        self.d = 2


t = testpacket()
print('original')
print(hex(id(t.a)))
print(id(t.b))
print(id(t.c))
print(id(t.d))
t.d = 0
t.a = 2000
print('modified')
print(id(t.a))
print(id(t.b))
print(id(t.c))
print(id(t.d))
a = t.serialize()
print(a.hex())
t_new = testpacket.from_bytes(a)
print(t_new)
print(t_new.serialize().hex())
print('received')
print(id(t_new.a))
print(id(t_new.b))
print(id(t_new.c))
print(id(t_new.d))
print(id(t_new.header))
