import eventlet
eventlet.monkey_patch()
from flask import Flask,jsonify,request,Response
from flask_socketio import SocketIO, emit
import time
import redis
import threading
import json
from RicardoHandler import packets



app = Flask(__name__)
app.config["SECRET_KEY"] = "secret!"
app.config['DEBUG'] = False

socketio = SocketIO(app)

flask_thread = None

telemetry_broadcast_thread = None
telemetry_broadcast_exit_event = threading.Event()

r : redis.Redis = None

prev_time = 0
updateTimePeriod = 200e6



@app.route('/')
def index():

    #maybe here have a webpage with current status
    return "Ricardo Backend",200

@app.route('/command', methods=['POST'])
def send_command():
    command_data = request.json
    if command_data == None:
        return 'Bad Request',400
    try:
        if "source" in command_data:
            source = command_data["source"]
        else:
            source = 2
        header = packets.Header(2,0,2,0,source = source,destination = command_data["destination"])
        cmd_packet = packets.Command(header,command_data["command"],command_data["arg"])
        send_data = {
            "data":cmd_packet.serialize().hex(),
            "clientid":command_data["clientid"]
        }
        r.lpush("SendQueue",json.dumps(send_data))
        return 'OK',200
    except KeyError:
        return 'Bad Request',400

@app.route('/packet', methods=['POST'])
def send_packet():
    packet_data = request.json
    if packet_data == None:
        return 'Bad Request',400
    if all (keys in packet_data for keys in ("data","clientid")):
        r.lpush("SendQueue",json.dumps(packet_data))
        return 'OK',200
    else:
        return 'Bad Request',400

@app.route('/response', methods=['POST'])
def get_response():
    response_data = request.json
    if response_data == None:
        return 'Bad Request \nNo Data Posted',400
    if "clientid" in response_data:
        key = "ReceiveQueue:" + str(response_data["clientid"])
        if r.llen(key) > 0 :
            received_response = json.loads(r.rpop(key))
            return received_response,200
        else:
            return "NODATA",200
    else:
        return 'Bad Request \nJSON INVALID',400


@app.route('/telemetry', methods=['GET'])
def get_telemetry():
    #get telemetry data from redis db
                #the telemetry key will be a json object
    telemetry_data = r.get("telemetry")
    if telemetry_data is not None:
        return json.loads(telemetry_data),200
    else:
        return "NODATA",200


@socketio.on('connect')
def connect():
    emit('Connect',{'data':'Connected'})

@socketio.on('disconnect')
def disconnect():
    print('client disconnected')
    

def __FlaskTask__(host,port): 
    print("starting socketio server")  
    socketio.run(app,host=host,port=port,debug=False,use_reloader=False)



def __TelemetryBroadcastTask__():
    #global _currentTelemetry
    # get latest telemetry from redis queue and emit
    prev_time = 0
    while not telemetry_broadcast_exit_event.is_set():
        if (time.time_ns() - prev_time > updateTimePeriod):
                
                #get telemetry data from redis db
                #the telemetry key will be a json object
                telemetry_data = r.get("telemetry")
                if telemetry_data is not None:
                    socketio.emit('telemetry', json.loads(telemetry_data), broadcast=True,namespace='/telemetry')
                prev_time = time.time_ns()
        eventlet.sleep(.02)

def __stopTelemetryBroadcastTask__():
    global telemetry_broadcast_exit_event
    telemetry_broadcast_exit_event.set()



        
def __startTelemetryBroadcastTask__():
    global telemetry_broadcast_thread
    if telemetry_broadcast_thread is None:
        telemetry_broadcast_thread = socketio.start_background_task(__TelemetryBroadcastTask__)

    
def startFlaskInterface(flaskhost="0.0.0.0",flaskport=5000,redishost = 'localhost',redisport = 6379):
    global flask_thread,r
    if r is None:
        r = redis.Redis(host=redishost,port=redisport)
    if flask_thread is None:
        flask_thread = socketio.start_background_task(__FlaskTask__,host = flaskhost,port = flaskport)
        __startTelemetryBroadcastTask__()
    

def stopFlaskInterface():
    __stopTelemetryBroadcastTask__()