#from Interfaces import flaskApi
# import gevent
# from gevent import monkey
# monkey.patch_all()
import eventlet

#eventlet.monkey_patch()


import argparse

import signal
import sys

#from Interfaces import flaskinterface
from Interfaces import flaskinterface
import testclass

from multiprocessing import Process

# Argument Parsing
ap = argparse.ArgumentParser()
# ap.add_argument("-d", "--device", required=True, help="Ricardo Serial Port", type=str)
# ap.add_argument("-b", "--baud", required=False, help="Serial Port Baud Rate", type=int,default=115200)
ap.add_argument("-p", "--port", required=False, help="Network Port", type=int,default = 1337)
# ap.add_argument("-t", "--tui", required=False, help="Launch Text-User-Interface", type=bool,default = False)
# ap.add_argument("-v", "--verbose", required=False, help="Enable Verbose Mode", type=bool,default = False)
args = vars(ap.parse_args())

def exitBackend(signalNumber, frame):
    # f.stop()
    #flask_thread.join()
    #flaskinterface.bg_exit_event.set()
    flaskinterface.flask_thread.join()
    #flaskinterface.bg_thread.join()
    flaskinterface.telemetry_broadcast_thread.join()

    sys.exit(0)

#t = None

if __name__ == '__main__':
    signal.signal(signal.SIGINT, exitBackend)
    signal.signal(signal.SIGTERM, exitBackend)

   # sm = serialManager.SerialkManager(port = args['device'],baud = ['baud'])
   # sm.start()
    #flaskinterface.t.start()

    # f = flaskinterface.FlaskInterface()
    # p = Process(target=f.start())
    # p.start()
    #f.start()
    flaskinterface.startFlaskInterface(args['port'])
    # p = Process(target=flaskinterface.startFlaskInterface,args=(args['port'],))
    # p.start()

    #flaskApi.socketio.run(flaskApi.app,port=args['port'])
    #print("socketio run")
    #socketio.run(app,port=5001,use_reloader = False)


