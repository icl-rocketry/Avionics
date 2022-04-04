import argparse
import socket
import pylibrnp.rnppacket as rnppacket
import json
import multiprocessing
# Argument Parsing
ap = argparse.ArgumentParser()
ap.add_argument("-d", "--dev", required=True, help="Serial Device", type=str)
ap.add_argument("-c", "--config", required=True, help="Config file", type=str)
ap.add_argument("-v", "--verbose", required=False, help="Config file", action='store_true',default=False)

args = vars(ap.parse_args())


class PacketFilter():
    def __init__(self,config:dict) -> None:
        self.udpPort = config['port'] #this will throw if no port is given
        self.udpIp = config.get('host','255.255.255.255') #broadcast by default
        self.filter = config.get('filter',None) # no filter 
        self.sock = None
       

    def newPacket(self,packet:bytearray):
        if self.filter is not None: # if no filter is given, send all packets received
            header:rnppacket.RnpHeader = rnppacket.RnpHeader.from_bytes(packet)
            # packet filtering
            matched_keys = self.filter.keys() & vars(header).keys()
            matches = [1 for key in matched_keys if vars(header).get(key) in self.filter.get(key)]
            if sum(matches) < len(matched_keys): #if number of matches is less than the length, we know that not all conditions are met
                return
        self.sock.sendto(packet,(self.udpIp,self.udpPort))
        
    def run(self):
        with socket.socket(socket.AF_INET,socket.SOCK_DGRAM) as self.sock:
            while True:
                pass


class TxPort():
    def __init__(self,config) -> None:
        self.udpPort = config['port'] #this will throw if no port is given
        self.udpIp = config.get('host','0.0.0.0') #listen to all ip by default
        self.sock = None
    
    def run(self):
        with socket.socket(socket.AF_INET,socket.SOCK_DGRAM) as self.sock:
            self.sock.bind((self.udpIp,self.udpPort))
            while True:
                data, addr = self.sock.recvfrom(8192)
            


if __name__ == "__main__":
    multiprocessing.set_start_method('spawn',force=True)
    #process config
    #create filter objects
    #start serial manager
    with open(args['config']) as jsonfile:
        configFile = json.load(jsonfile)
    
