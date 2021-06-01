import socketio

# standard Python
sio = socketio.Client(logger=True, engineio_logger=True)
sio.connect('http://localhost:5001')

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