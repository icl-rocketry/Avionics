import requests
from pylibrnp.defaultpackets import *

while True:
    source = input("source node : ")
    destination = input("destination node : ")
    command_num = input("command id : ")
    arg = input("arg : ")
    cmd_packet : SimpleCommandPacket = SimpleCommandPacket(command = int(command_num), arg = int(arg))
    cmd_packet.header.destination_service = 2 #Note on old fw this will be 1
    cmd_packet.header.source = int(source)
    cmd_packet.header.destination = int(destination)
    
    data = {
        "data":cmd_packet.serialize().hex(),
        "clientid":"LOCAL:COMMANDTESTER"
    }
    r = requests.post("http://localhost:1337/packet",json = data)
    print(r.status_code, r.reason)
    
