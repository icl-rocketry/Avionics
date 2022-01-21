import multiprocessing
import queue
import socketio
import sys
import signal
import argparse
import json


# import magcalibration
from . import magcalibration
from . import cmdui
# import cmdui

ap = argparse.ArgumentParser()
ap.add_argument("-f","--filename",required=False,help='Filename of csv mag data formatted as mx,my,mz',type=str,default='')
ap.add_argument("-s",'--server',required=False,help='address of backend',type=str,default='localhost')
ap.add_argument("-p","--port",required=False,help='port of backend',type=int,default=1337)
args = vars(ap.parse_args())



def startNetworkedMagCalApp(msgQ,
                            server='localhost',
                            port=1337):

    magcal = magcalibration.MagCalibration(msgQ,server,port)

    sio = socketio.Client()

    sio.connect('http://'+server+':'+str(port),namespaces=['/telemetry'])

    @sio.on('telemetry',namespace='/telemetry')
    def on_telemetry(data):  
        magcal.updateData(json.loads(data))

    magcal.run()


def startFileMagCalApp(msgQ,
                       server:str=None,
                       port:int=None,
                       filename:str=''):

    magcal = magcalibration.MagCalibration(msgQ,server,port,filename)
    magcal.run()


if __name__ == '__main__':
    multiprocessing.set_start_method('spawn',force=True) # must use spawn instead of fork on macos due to fork security in objc runtime

    messageQueue = multiprocessing.Queue(10)

    if args["filename"] != '':
        p = multiprocessing.Process(target=startFileMagCalApp,args=(messageQueue,args["server"],args["port"],args["filename"],))
    else:
        p = multiprocessing.Process(target=startNetworkedMagCalApp,args=(messageQueue,args["server"],args["port"],))

    p.start()

    cmdui = cmdui.MagCalCmdUI(messageQueue)
    
    
    cmdui.cmdloop()

    p.terminate()
    p.join()
    sys.exit(0)
    

    

    
