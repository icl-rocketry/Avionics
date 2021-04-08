from flask import Flask,jsonify,request,Response
from flask_socketio import SocketIO, emit
#from flask_pymongo import PyMongo

app = Flask(__name__)
app.config["SECRET_KEY"] = "secret!"
app.config['DEBUG'] = True

socketio = SocketIO(app)

@app.route('/')
def index():
    return "RICARDO OS PYTHON BACKEND",200

@app.route('/command', methods=['POST'])
def send_command():
    command_data = request.json
    return 'OK',200

@socketio.on('connect')
def connect();
    emit('Connect',{'data':'Connected'})

@socketio.on('disconnect')
def disconnect():
    print('client disconnected')
    
@socketio.on('get_telemetry')
def getTelemetry(json):#retrieve data from db or return error if no rocket connected
    telemetryData = jsonify(
        data='telemetry'
    )
    return telemetryData,200




if __name == '__main__':
    socketio.run(app,port=5000)