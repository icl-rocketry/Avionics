from matplotlib.colors import get_named_colors_mapping
import redis
import argparse
import sys
import json
import pandas as pd
import matplotlib.pyplot as plt
import time
import matplotlib as mpl




class Grapher():
    def __init__(self,args):

        self.telemetry_timeseries = {}
        self.prev_telemetry_data = None
        self.updatedData = True
        #self.plot_data = ['ax','ay','az','gx','gy','gz','mx','my','mz','roll','pitch','yaw']
        #self.plot_data = ['roll','pitch','yaw']
        #self.plot_colors = ['red','green','blue']
        self.plot_data = args["vars"]
        if self.plot_data is []:
            self.plot_data = ['roll','pitch','yaw']

        self.prevTime = 0
        self.updateDelta = .01
        self.history = 50
    
        #setup figures

        
        
        plt.style.use('dark_background')
        # Set the default color cycle
        mpl.rcParams['axes.prop_cycle'] = mpl.cycler(color=['r','g','b','c','m','y']) 
        plt.ion()
        self.fig = plt.figure(figsize=[6,5])
        
        self.ax = self.fig.add_subplot(111)
        self.ax.grid(True,which='both',color='grey')
        plt.show()

        self.lines = []

        for idx,var in enumerate(self.plot_data):
            line, = self.ax.plot([],[],linewidth = 1,label = var)
            #line, = self.ax.plot([],[],linewidth = 1,label = var)
            self.lines.append(line) 


        self.ax.legend(loc='upper right')
        self.ax.set_xlabel("Time (ms)")
        

        self.r = redis.Redis(args["host"],args["port"])
    
        

    def run(self):
        while True:
            if (time.time() - self.prevTime > self.updateDelta):
                self.prevTime = time.time()
                self.__getTelemetry__() 

    def __getTelemetry__(self):
        data = self.r.get("telemetry")

        if data is None:
            return
        telemetry_data = json.loads(data)
        if not telemetry_data["connectionstatus"] :
            return
        if telemetry_data == self.prev_telemetry_data:
            return#no data has changed so we ignore

        if self.prev_telemetry_data is None:
            length = 0
        else:
            length = len(self.telemetry_timeseries["system_time"])

        for key in telemetry_data.keys():
            #update timeseries with new data
            if key not in self.telemetry_timeseries:
                self.telemetry_timeseries[key] = [telemetry_data[key]]
            else:
                self.telemetry_timeseries[key].append(telemetry_data[key]) 
            if length == self.history:
                self.telemetry_timeseries[key].pop(0) #remove first element from list

        # updatedData = True
        self.prev_telemetry_data = telemetry_data
        self.__updatePlot__()





    def __updatePlot__(self):
        for idx,var in enumerate(self.plot_data):
            
            self.lines[idx].set_xdata(self.telemetry_timeseries["system_time"])
            self.lines[idx].set_ydata(self.telemetry_timeseries[var])
        plt.pause(.001)
        self.ax.relim()
        #self.ax.set_xlim(left=self.telemetry_timeseries["system_time"][1], right=self.telemetry_timeseries["system_time"][-1])
        self.ax.autoscale_view()
        #self.ax.set_ylim(bottom=-3.2, top=3.2)
        self.fig.canvas.draw_idle()
        self.fig.canvas.flush_events()


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", required=False, help="redis host", type=str,default = "localhost")
    ap.add_argument("--port", required=False, help="redis port", type=int,default = 6379)
    ap.add_argument("--vars",nargs='+', required=False, help="plotting variables",type = str,default = ['roll','pitch','yaw'])
    args = vars(ap.parse_args())
    
    g = Grapher(args)
    g.run()