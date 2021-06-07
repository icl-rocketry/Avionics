import eventlet
eventlet.monkey_patch()
from flask import Flask,jsonify,request,Response
from flask_socketio import SocketIO, emit
import time
import redis
import threading
import json



app = Flask(__name__)
app.config["SECRET_KEY"] = "secret!"
app.config['DEBUG'] = True

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
    return str(prev_time),200

@app.route('/command', methods=['POST'])
def send_command():
    command_data = request.json
    return 'OK',200

@app.route('/telemetry', methods=['GET'])
def get_telemetry():
    #get telemetry data from redis db
                #the telemetry key will be a json object
    telemetry_data = r.get("telemetry")
    if telemetry_data is not None:
        return json.loads(telemetry_data),200
    else:
        return "NODATA",200
#regeister new client to telemetry channel

@socketio.on('connect')
def connect():
    emit('Connect',{'data':'Connected'})

@socketio.on('disconnect')
def disconnect():
    print('client disconnected')
    

def __FlaskTask__(port): 
    print("starting socketio server")  
    socketio.run(app,port=port,debug=True,use_reloader=False)



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

    
def startFlaskInterface(port,redishost = 'localhost',redisport = 6379):
    global flask_thread,r
    if r is None:
        r = redis.Redis(host=redishost,port=redisport)
    if flask_thread is None:
        flask_thread = socketio.start_background_task(__FlaskTask__,port)
        __startTelemetryBroadcastTask__()
    

def stopFlaskInterface():
    __stopTelemetryBroadcastTask__()