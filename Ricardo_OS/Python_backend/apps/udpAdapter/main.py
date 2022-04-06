import argparse
import socket
from serialmanager import SerialManager
from pylibrnp.defaultpackets import SimpleCommandPacket
import pylibrnp.rnppacket as rnppacket
import json
import multiprocessing
import sys
import signal
import time

# Argument Parsing
ap = argparse.ArgumentParser()
ap.add_argument("-d", "--dev", required=True, help="Serial Device", type=str)
ap.add_argument("-b", "--baud", required=False, help="Serial Buad", type=int,default=115200)
ap.add_argument("-c", "--config", required=True, help="Config file", type=str)
ap.add_argument("-v", "--verbose", required=False, help="Config file", action='store_true',default=False)

args = vars(ap.parse_args())


class RxPort():
    def __init__(self,config:dict) -> None:
        self.udpPort = config['port'] #this will throw if no port is given
        self.udpIp = config.get('host','255.255.255.255') #broadcast by default
        self.filter = config.get('filter',None) # no filter 
        self.sock = None
        self.queue = multiprocessing.Queue()
       

    def newPacket(self,packet:bytearray):
        if self.filter is not None: # if no filter is given, send all packets received
            header:rnppacket.RnpHeader = rnppacket.RnpHeader.from_bytes(packet['data'])
            # packet filtering
            matched_keys = self.filter.keys() & vars(header).keys()
            matches = [1 for key in matched_keys if vars(header).get(key) in [self.filter.get(key)]]
            if sum(matches) < len(matched_keys): #if number of matches is less than the length, we know that not all conditions are met
                return
        self.sock.sendto(packet['data'],(self.udpIp,self.udpPort))
        
    def run(self):
        with socket.socket(socket.AF_INET,socket.SOCK_DGRAM) as self.sock:
            if '255' in self.udpIp:
                self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

            while True:
                newdata = self.queue.get(block=True)
                self.newPacket(newdata)


class TxPort():
    def __init__(self,config) -> None:
        self.udpPort = config['port'] #this will throw if no port is given
        self.udpIp = config.get('host','0.0.0.0') #listen to all ip by default
        self.sock = None
        self.queue = multiprocessing.Queue()
    
    def run(self):
        with socket.socket(socket.AF_INET,socket.SOCK_DGRAM) as self.sock:
            self.sock.bind((self.udpIp,self.udpPort))
            while True:
                data, addr = self.sock.recvfrom(8192)
                self.queue.put({'data':data})
            
def receivePacketQueueManager(receiveQueue,PacketFilterQueues):
    while True:
        newpacket = receiveQueue.get(block=True)
        for queue in PacketFilterQueues:
            queue.put(newpacket)

def exitHandler(*args):
    exit=True

if __name__ == "__main__":
    multiprocessing.set_start_method('spawn',force=True)

    exit = False
    signal.signal(signal.SIGINT, exitHandler)
    signal.signal(signal.SIGTERM, exitHandler)

    receivePacketQueue = multiprocessing.Queue()
    proclist = []
    udpRxPortQueueList = []
    #import config
    with open(args['config']) as jsonfile:
        config = json.load(jsonfile)
    
    #setup udp ports
    udpTxPort = TxPort(config['Tx'])
    proclist.append(multiprocessing.Process(target=udpTxPort.run))

    udpRxPort = RxPort(config['Rx'])
    proclist.append(multiprocessing.Process(target=udpRxPort.run))
    udpRxPortQueueList.append(udpRxPort.queue)


    for filterconfig in config['filters']:
        filteredRxPort = RxPort(filterconfig)
        proclist.append(multiprocessing.Process(target=filteredRxPort.run))
        udpRxPortQueueList.append(filteredRxPort.queue)
    
    #spawn receive queue manager
    proclist.append(multiprocessing.Process(target=receivePacketQueueManager,args=(receivePacketQueue,udpRxPortQueueList,)))

    #start serial manager
    sm = SerialManager(device=args['dev'],sendQ=udpTxPort.queue,receiveQ=receivePacketQueue,baud=args['baud'],verbose=args['verbose'])

    proclist.append(multiprocessing.Process(target=sm.run))

    #spawn processes
    [p.start() for p in proclist]

    while not exit:
        prevTime = 0
        if (time.time_ns() - prevTime > 5000000):
            cmd_packet = SimpleCommandPacket(command=8,arg=0) # 8 for telemetry
            cmd_packet.header.source = 1
            cmd_packet.header.destination = 0
            cmd_packet.header.source_service = 2
            cmd_packet.header.destination_service = 2
            udpTxPort.queue.put({"data":bytes(cmd_packet.serialize())})
            prevTime = time.time_ns()

    [p.terminate() for p in proclist]
    [p.join() for p in proclist]
    sys.exit(0)