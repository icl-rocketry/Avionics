import requests
import packets

import socketio
import argparse



sio = socketio.Client(logger=False, engineio_logger=False)


@sio.event
def connect():
    print("I'm connected!")

@sio.event
def connect_error(data):
    print("The connection failed!")

@sio.event
def disconnect():
    print("I'm disconnected!")

@sio.on('Response',namespace='/command')
def on_response_handler(data):
    print(data)

@sio.on('Error',namespace='/command')
def on_error_handler(data):
    print(data)


    
if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", required=False, help="backend host", type=str,default = "localhost")
    ap.add_argument("--port", required=False, help="backend port", type=int,default = 1337)
    args = vars(ap.parse_args())

    sio.connect('http://' + args["host"] + ':' + str(args['port']) + '/',namespaces=['/','/telemetry','/command'])

    while True:
        source = input("source node : ")
        while not source:
            source = input("source node : ")
        destination = input("destination node : ")
        command_num = input("command id : ")
        arg = input("arg : ")
        cmd_packet :packets.SimpleCommand = packets.SimpleCommand(command = int(command_num), arg = int(arg))
        cmd_packet.header.source = int(source)
        cmd_packet.header.destination = int(destination)
        serializedPacket:str = cmd_packet.serialize().hex()
        sio.emit('send_data',{'data':serializedPacket},namespace='/command')
    