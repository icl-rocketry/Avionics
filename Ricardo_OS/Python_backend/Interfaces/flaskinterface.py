import eventlet
eventlet.monkey_patch()
from flask import Flask,jsonify,request,Response
from flask_socketio import SocketIO, emit
import time




app = Flask(__name__)
app.config["SECRET_KEY"] = "secret!"
app.config['DEBUG'] = True

socketio = SocketIO(app)

flask_thread = None
bg_thread = None
telemetry_broadcast_thread = None




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
    

def _FlaskTask(port): 
    print("starting socketio server")  
    socketio.run(app,port=port,debug=True,use_reloader=False)


def _BGTask():
    global prev_time
    # while not bg_exit_event.is_set():
    while True:
        if (time.time_ns() - prev_time > updateTimePeriod):
                
                print("emitt")
                socketio.emit('response', prev_time, broadcast=True,namespace='/')
                prev_time = time.time_ns()
        eventlet.sleep(.02)

def _TelemetryBroadcastTask():
    #global _currentTelemetry
    # get latest telemetry from redis queue and emit
    prev_time = 0
    while True:
        if (time.time_ns() - prev_time > updateTimePeriod):
                
                print("telemetry")
                test_message = 'hi'
                socketio.emit('telemetry', test_message, broadcast=True,namespace='/telemetry')
                prev_time = time.time_ns()
        eventlet.sleep(.02)


def startBackgroundTask():
    global bg_thread
    if bg_thread is None:
        bg_thread = socketio.start_background_task(_BGTask)
        
def startTelemetryBroadcastTask():
    global telemetry_broadcast_thread
    if telemetry_broadcast_thread is None:
        telemetry_broadcast_thread = socketio.start_background_task(_TelemetryBroadcastTask)

    
def startFlaskInterface(port):
    global flask_thread
    if flask_thread is None:
        flask_thread = socketio.start_background_task(_FlaskTask,port)
        #startBackgroundTask()
        startTelemetryBroadcastTask()
    

