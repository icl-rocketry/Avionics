import packets

header = packets.Header(4,uid=10)
print(vars(header))
telem = packets.Telemetry(header,1,2,3,4,5,6,7,8,9,11)
print(vars(telem))
t_bytes = telem.serialize()
print(t_bytes)
t1 = packets.Telemetry.from_bytes(t_bytes)
print(vars(t1.header))
print(vars(t1))