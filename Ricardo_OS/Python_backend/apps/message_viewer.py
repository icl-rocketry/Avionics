
import socketio
import argparse
import json

sio = socketio.Client(logger=False, engineio_logger=False)

@sio.on('message',namespace='/messages')
def on_message_handler(data_str):
    data = json.loads(data_str)
    head = data["header"]
    prefix:str = "[" + str(head["source"]) + ":" + str(head["source_service"]) + " -> " + str(head["destination"]) + ":" + str(head["destination_service"]) +"] "
    print(prefix + str(data["message"]))


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", required=False, help="backend host", type=str,default = "localhost")
    ap.add_argument("--port", required=False, help="backend port", type=int,default = 1337)
    args = vars(ap.parse_args())

    sio.connect('http://' + args["host"] + ':' + str(args['port']) + '/',namespaces=['/','/messages'])
