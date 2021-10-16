import requests
import packets

while True:
    source = input("source node : ")
    destination = input("destination node : ")
    command_num = input("command id : ")
    arg = input("arg : ")
    cmd_packet :packets.SimpleCommand = packets.SimpleCommand(command = int(command_num), arg = int(arg))
    cmd_packet.header.source = int(source)
    cmd_packet.header.destination = int(destination)
    
    data = {
        "data":cmd_packet.serialize().hex(),
        "clientid":"LOCAL:COMMANDTESTER"
    }
    r = requests.post("http://localhost:1337/packet",json = data)
    print(r.status_code, r.reason)
    
