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
    r = requests.post("http://localhost:1337/command",json = data)
    print(r.status_code, r.reason)

while True:
    new_command(100,0)

    time.sleep(0.5)
    new_command(105,0)
    
    time.sleep(0.5)