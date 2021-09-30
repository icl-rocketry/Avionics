import requests

import time
import random

def new_command(command,arg):
    source = 2
    destination = 0
    command_num = command
    arg = arg
    data = {
        "source":int(source),
        "destination":int(destination),
        "command":int(command_num),
        "arg":int(arg),
        "clientid":"LOCAL:COMMANDTESTER"
    }
    r = requests.post("http://192.168.2.71:1337/command",json = data)
    print(r.status_code, r.reason)

while True:
    new_command(100,0)
    print("debug")
    time.sleep(1.2)
    new_command(105,0)
    print("exit")
    time.sleep(1.2)
    
