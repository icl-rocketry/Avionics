import eventlet
eventlet.monkey_patch()
from flask import Flask,jsonify,request,Response
from flask_socketio import SocketIO, emit
import time

import testclass
import threading


app = Flask(__name__)
app.config["SECRET_KEY"] = "secret!"
app.config['DEBUG'] = True

socketio = SocketIO(app)


prev_t = 0

@app.route('/')
def index():

    #maybe here have a webpage with current status
    return str(prev_t),200

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
    



class FlaskInterface():
    def __init__(self):
        self.thread = None
        self.bg_thread = None
        self.bg_exit_event = threading.Event()

        
        self.prev_time = 0
        self.updateTimePeriod = 200e6

    def start_server(self):
        print("starting socketio server")
        #socketio.run(app, port=RicardoBackend.args['port'], debug=True, use_reloader=False)
        socketio.run(app, port=5001, debug=True, use_reloader=False)
        

    def start(self):
        self.thread = socketio.start_background_task(self.start_server)
        self.bg_thread = socketio.start_background_task(self._bg_time_emit)
        

    def stop(self):
        self.thread.join()
        self.bg_exit_event.set()

    def _bg_time_emit(self):
        while not self.bg_exit_event.is_set():
            if (time.time_ns() - self.prev_time > self.updateTimePeriod):
                    
                    print("emitt")
                    socketio.emit('response', self.prev_time, broadcast=True,namespace='/')
                    self.prev_time = time.time_ns()
            eventlet.sleep(.02)
