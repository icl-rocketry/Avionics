import argparse
import time
import json
import requests
from Python_backend.pylibrnp.rnppacket import RnpPacket
from pylibrnp.rnppacket import RnpHeader

ap = argparse.ArgumentParser()
ap.add_argument("-i", "--id", required=True, help="Client ID", type=str)
ap.add_argument("-a",'--address', required=False,help="ip address",type=str,default='localhost')
ap.add_argument("-p",'--port',required=False,help='port',type=str,default=1337)

args = vars(ap.parse_args())

url = "http://" + args["address"] +":" + str(args["port"]) + "/response"

prevtime = 0
checkDelta = .5

payload = {
        "clientid":args["id"]
    }
    


if __name__ == '__main__':
    while True:
        if (time.time() - prevtime > checkDelta):
            prevtime=time.time()
            r = requests.post(url,json=payload)
            if (r.status_code is 200 and r.text != "NODATA"):
                data = r.content
                print(data)

                head = RnpPacket.from_bytes(data)
                print(head)

                
