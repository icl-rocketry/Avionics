#from Interfaces import flaskApi
# import gevent
# from gevent import monkey
# monkey.patch_all()
import eventlet

#eventlet.monkey_patch()


import argparse
from RicardoHandler import serialManager
import signal
import sys

from flask import Flask

from flask import jsonify
from flask import request
from flask import Response
from flask_socketio import SocketIO, emit
import testclass


# Argument Parsing
ap = argparse.ArgumentParser()
# ap.add_argument("-d", "--device", required=True, help="Ricardo Serial Port", type=str)
# ap.add_argument("-b", "--baud", required=False, help="Serial Port Baud Rate", type=int,default=115200)
# ap.add_argument("-p", "--port", required=False, help="Network Port", type=int,default = 1337)
# ap.add_argument("-t", "--tui", required=False, help="Launch Text-User-Interface", type=bool,default = False)
# ap.add_argument("-v", "--verbose", required=False, help="Enable Verbose Mode", type=bool,default = False)
args = vars(ap.parse_args())

def exitBackend(signalNumber, frame):
   # sm.stop()
    t.stop()
    sys.exit(0)

app = Flask(__name__)
app.config["SECRET_KEY"] = "secret!"
app.config['DEBUG'] = True

socketio = SocketIO(app)

@app.route('/')
def index():
    #maybe here have a webpage with current status
    return str(t.get()),200

@app.route('/command', methods=['POST'])
def send_command():
    command_data = request.json
    return 'OK',200

# @app.route("/telemetry",['GET'])
# def getTelemetry(): 
#     telemetryData = jsonify(
#         data='telemetry'
#     )
#     return telemetryData,200

@app.route('/telemetry', methods=['GET'])
def get_telemetry():
    telemetryData= jsonify(
        data = 'telemetry'
    )
    return telemetryData,200

#regeister new client to telemetry channel
@socketio.on('connect')
def connect():
    emit('Connect',{'data':'Connected'})

 

@socketio.on('disconnect')
def disconnect():
    print('client disconnected')
    




if __name__ == '__main__':
    signal.signal(signal.SIGINT, exitBackend)
    signal.signal(signal.SIGTERM, exitBackend)

   # sm = serialManager.SerialkManager(port = args['device'],baud = ['baud'])
   # sm.start()

    t = testclass.testclass(socketio)
    t.start()

    #flaskApi.socketio.run(flaskApi.app,port=args['port'])
    print("socketio run")
    socketio.run(app,port=5001,use_reloader = False)


