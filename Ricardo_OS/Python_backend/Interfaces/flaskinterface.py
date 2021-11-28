import eventlet #Dont Monkey Patch 
#this module is started as a multiprocessing process in the RicardoBackend.py module. If we monkey patch, we monkey patch everything else imported.
# We can get away with this here as we make sure to not use anything requiring monkey patch i.e using socketio.start_background_task() instead of starting
# the thread with python threading. Eventlet monkey patch changes the memory location of threading.current_thread resulting 
# in (threading.current_tread() is threading.main_thread() returning false which breaks cmd2...
from flask import Flask,jsonify,request,Response,render_template
from flask_socketio import SocketIO, emit
import time
import redis
import json
import signal
import sys

# packages used to send static files from frontend
from flask import send_from_directory
import os


app = Flask(__name__, static_folder='../frontend/build')
app.config["SECRET_KEY"] = "secret!"
app.config['DEBUG'] = False

socketio = SocketIO(app,cors_allowed_origins="*",async_mode='eventlet')

flask_thread = None

telemetry_broadcast_running:bool = False

dummy_signal_running:bool = False
r : redis.Redis = None
rhost = ''
rport = ''


prev_time = 0
updateTimePeriod = 10e6


# return react site
@app.route('/', defaults={'path': ''})
@app.route('/<path:path>')
def serve(path):
    if path != "" and os.path.exists(app.static_folder + '/' + path):
        return send_from_directory(app.static_folder, path)
    else:
        return send_from_directory(app.static_folder, 'index.html')

# @app.route('/')
# def index():
#     print("GET / 200")
#     #maybe here have a webpage with current status
#     return "Hello world", 200


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
            received_response :bytes = r.rpop(key)
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

@app.route("/graph",methods=['get'])
def get_graph():
    return render_template('graph.html',x_window = 100)

@app.route("/map",methods=['get'])
def get_map():
    return render_template('map.html',x_window = 100)

@socketio.on('connect',namespace='/telemetry')
def connect_telemetry():
    global telemetry_broadcast_running
    #start broadcasting telemetry on socketio on connection of the first client to the telemetry namespace
    if not telemetry_broadcast_running :
        telemetry_broadcast_running = True
        socketio.start_background_task(__TelemetryBroadcastTask__,rhost,rport)

@socketio.on('connect', namespace='/')
def connect():
    global dummy_signal_running
    if not dummy_signal_running:
        dummy_signal_running = True
        socketio.start_background_task(__DummySignalBroadcastTask__)


# @socketio.on('send_packet',namespace='/sendData')
# def socketio_send_data(data):
#     try:
#         packetData = json.loads(data)
#     except:
#         print("Json deserialization error")
#         return
    
#     if all (keys in packetData for keys in ("data","clientid")):
#         packetData["clientid"] = packetData["clientid"] + ":" + 

#     pass

@socketio.on('disconnect')
def disconnect():
    pass

def __TelemetryBroadcastTask__(redishost,redisport):
    redis_connection = redis.Redis(host=redishost,port=redisport)
    prev_telemetry = {}
    prev_time = 0
    
    while telemetry_broadcast_running:
        if (time.time_ns() - prev_time > updateTimePeriod):
                
            telemetry_data = redis_connection.get("telemetry")
            if telemetry_data is not None:
                if prev_telemetry.get("connectionstatus",None) is not None and True: #check if theres prev_telemetry and that we are connected
                    
                    if (prev_telemetry.get("system_time",0) == (json.loads(telemetry_data)).get("system_time",0)):#dont broadcast a duplicate packet
                        continue

                socketio.emit('telemetry', json.loads(telemetry_data), broadcast=True,namespace='/telemetry')
        
            prev_time = time.time_ns()
            prev_telemetry = json.loads(telemetry_data)

        eventlet.sleep(.02)

# dummy signal
from emitter import emitter
def __DummySignalBroadcastTask__():
    while dummy_signal_running:
        emitter.external_emitter('telemetry-log')

def cleanup(sig=None,frame=None): #ensure the telemetry broadcast thread has been killed
    global telemetry_broadcast_running, dummy_signal_running
    
    # original
    # print("Flask Interface Exited")
    # for testing directly with flaskinterface.py
    print("\nFlask Interface Exited")

    telemetry_broadcast_running = False
    dummy_signal_running = False
    sys.exit(0)



    
def startFlaskInterface(flaskhost="0.0.0.0", flaskport=5000, 
                        redishost='localhost', redisport=6379, real_data=True):
    # original signal handler
    if (real_data):
        print("Server starting on port " + str(flaskport) + " ...")
        global r, rhost, rport
        rhost = redishost
        rport = redisport

        # signal.signal(signal.SIGINT,cleanup)
        # signal.signal(signal.SIGTERM,cleanup)

        if r is None:
            r = redis.Redis(host=redishost,port=redisport)

        socketio.run(app, host=flaskhost, port=flaskport, debug=False, use_reloader=False)
        cleanup()

    # fake signal handler
    else:
        # server logs
        fake_signal_filename = 'telemetry_log.txt'
        print("Reading fake signal from " + fake_signal_filename)
        print("Starting server on port " + str(flaskport) + "...")

        socketio.run(app, host=flaskhost, port=flaskport, debug=False, use_reloader=False)
        cleanup()
        

if __name__ == "__main__":
    startFlaskInterface(flaskport=3001, real_data=False)