import requests

while True:
    source = input("source node : ")
    destination = input("destination node : ")
    command_num = input("command id : ")
    arg = input("arg : ")
    data = {
        "source":int(source),
        "destination":int(destination),
        "command":int(command_num),
        "arg":int(arg),
        "clientid":"LOCAL:COMMANDTESTER"
    }
    r = requests.post("http://localhost:1337/command",json = data)
    print(r.status_code, r.reason)
