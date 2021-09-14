import socketio
import redis
import json

r = redis.Redis("localhost","6000")

# standard Python
sio = socketio.Client(logger=False, engineio_logger=False)
sio.connect('http://localhost:1337',namespaces=['/','/telemetry'])

@sio.event
def connect():
    print("I'm connected!")

@sio.event
def connect_error(data):
    print("The connection failed!")

@sio.event
def disconnect():
    print("I'm disconnected!")

@sio.on('response')
def on_message(data):
    print(data)

@sio.on('telemetry',namespace='/telemetry')
def on_message(data):
    r.set("telemetry",json.dumps(data))
