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


app = Flask(__name__)
app.config["SECRET_KEY"] = "secret!"
app.config['DEBUG'] = False

socketio = SocketIO(app,cors_allowed_origins="*",async_mode='eventlet')

flask_thread = None

telemetry_broadcast_running:bool = False

r : redis.Redis = None
rhost = ''
rport = ''


prev_time = 0
updateTimePeriod = 10e6


@app.route('/')
def index():

    #maybe here have a webpage with current status
    return "Ricardo Backend",200

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

@socketio.on('connect')
def connect():  
    global telemetry_broadcast_running
    #start broadcasting telemetry on socketio on connection of the first client
    if not telemetry_broadcast_running :
        telemetry_broadcast_running = True
        socketio.start_background_task(__TelemetryBroadcastTask__,rhost,rport)
    

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


def cleanup(): #ensure the telemetry broadcast thread has been killed
    global telemetry_broadcast_running
    telemetry_broadcast_running = False

    
def startFlaskInterface(flaskhost="0.0.0.0",flaskport=5000,redishost = 'localhost',redisport = 6379):
    global r,rhost,rport
    rhost = redishost
    rport = redisport

    if r is None:
        r = redis.Redis(host=redishost,port=redisport)
    
    print("starting socketio server")  
    socketio.run(app,host=flaskhost,port=flaskport,debug=False,use_reloader=False)
    cleanup()
 

if __name__ == "__main__":
    startFlaskInterface(flaskport=2000)