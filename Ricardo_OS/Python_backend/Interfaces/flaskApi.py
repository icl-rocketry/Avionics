from flask import Flask,jsonify,request,Response
from flask_socketio import SocketIO, emit
import RicardoBackend
#import config
import time
#import RicardoBackend
#from flask_pymongo import PyMongo

app = Flask(__name__)
app.config["SECRET_KEY"] = "secret!"
app.config['DEBUG'] = True

socketio = SocketIO(app)

@app.route('/')
def index():
    #maybe here have a webpage with current status
    return str(RicardoBackend.t.get()),200

@app.route('/command', methods=['POST'])
def send_command():
    command_data = request.json
    return 'OK',200

# @app.route("/telemetry",['GET'])
# def getTelemetry(): 
#     telemetryData = jsonify(
#         data='telemetry'
#     )
#     return telemetryData,200

@socketio.on('connect')
def connect():
    emit('Connect',{'data':'Connected'})

@socketio.on('disconnect')
def disconnect():
    print('client disconnected')
    
@socketio.on('get_telemetry')
def getTelemetryStream(json):#retrieve data from db or return error if no rocket connected
    telemetryData = jsonify(
        data='telemetry'
    )
    return telemetryData,200

