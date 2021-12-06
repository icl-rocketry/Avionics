import socketio

# standard Python
sio = socketio.Client(logger=True, engineio_logger=True)
sio.connect('http://192.168.1.224:1337',namespaces=['/','/telemetry'])

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
    print(data)    
