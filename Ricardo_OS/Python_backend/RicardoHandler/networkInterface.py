from  .packets import *
import threading

class NetworkInterface(threading.Thread):
    def __init__(self):
        self.packetRecord = ''



    def run(self):
        pass
    

    def sendCommand(self,command,responseExpected = True,returnID = None):
        if responseExpected:
            pass
        pass

    def sendPacket(self,header : Header,body : bytes,resposneExpected = True,returnID = None):
        if resposneExpected:
            pass
        pass