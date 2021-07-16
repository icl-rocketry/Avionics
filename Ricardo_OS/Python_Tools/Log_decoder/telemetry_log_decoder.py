f = open("telemetry_log","rb")
data = []
byte = f.read(1)
while byte:
    # Do stuff with byte.
    data.append(byte)
    byte = f.read(1)
print(data)