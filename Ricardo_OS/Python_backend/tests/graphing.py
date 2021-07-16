import socketio
import argparse
import sys
import json
import pandas as pd
import matplotlib.pyplot as plt
import time


# standard Python
sio = socketio.Client(logger=False, engineio_logger=True)

@sio.event
def connect():
    print("I'm connected!")

@sio.event
def connect_error(data):
    print("The connection failed!")
    sys.exit(1)

@sio.event
def disconnect():
    print("I'm disconnected!")

@sio.on('telemetry',namespace='/telemetry')
def on_message(data):
    global prev_telemetry_data
    global telemetry_timeseries
    global updatedData
    if data is None:
        print("no data")
        return
    
    telemetry_data = data
    if not telemetry_data["connectionstatus"] :
        print("no new data")
        return
    if telemetry_data == prev_telemetry_data:
        print("same")
        return#no data has changed so we ignore

    if prev_telemetry_data is None:
        length = 0
    else:
        length = len(telemetry_timeseries["system_time"])
 
    for key in telemetry_data.keys():
        #update timeseries with new data
        if key not in telemetry_timeseries:
            telemetry_timeseries[key] = [telemetry_data[key]]
        else:
            telemetry_timeseries[key].append(telemetry_data[key]) 
        if length == 100:
                telemetry_timeseries[key].pop() #remove first element from list
    
    # updatedData = True
    _updatePlot_()
    print("plot")
    
    prev_telemetry_data = telemetry_data

 

def _updatePlot_():
    global lines,ax,fig,plot_data

    
    for idx,var in enumerate(plot_data):
        lines[idx].set_xdata(telemetry_timeseries["system_time"])
        lines[idx].set_ydata(telemetry_timeseries[var])
    plt.pause(.01)
    ax.relim()
    ax.set_xlim(left=telemetry_timeseries["system_time"][0], right=telemetry_timeseries["system_time"][-1])
    ax.autoscale_view()
    ax.grid()
    fig.canvas.draw()
    fig.canvas.flush_events()

if __name__ == '__main__':
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", required=False, help="socket io host", type=str,default = "localhost")
    ap.add_argument("--port", required=False, help="socket io port", type=int,default = 1337)
    args = vars(ap.parse_args())

    telemetry_timeseries = {}
    prev_telemetry_data = None
    updatedData = True
    plot_data = ['ax','ay','az','gx','gy','gz','mx','my','mz','roll','pitch','yaw']

   
    #setup figures
    lines = []

    plt.style.use('bmh')
    plt.ion()
    fig = plt.figure(figsize=[12,10])

    ax = fig.add_subplot(111)
    plt.show()
    plt.pause(.01)
    time.sleep(.01)

    for var in plot_data:
       line, = ax.plot([],[],linewidth = 1,label = var)
       lines.append(line) 


    ax.legend(loc='upper right')
    ax.set_xlabel("Time (s)")
    ax.grid()

    sio.connect('http://' + args["host"] + ":" + str(args["port"]),namespaces=['/','/telemetry'])

    # while True:
    #     if updatedData:
    #         for idx,var in enumerate(plot_data):
    #             lines[idx].set_xdata(telemetry_timeseries["system_time"])
    #             lines[idx].set_ydata(telemetry_timeseries[var])
    #         ax.relim()
    #         ax.set_xlim(left=telemetry_timeseries["system_time"][0], right=telemetry_timeseries["system_time"][-1])
    #         ax.autoscale_view()
    #         ax.grid()
    #         fig.canvas.draw()
    #         fig.canvas.flush_events()
    #         updatedData = False
