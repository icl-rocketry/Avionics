
import redis
import argparse
import json

import time
import pandas as pd

import csv



class TelemetryLogger():
    def __init__(self,redishost,redisport,filename,vars = None):

        self.prev_telemetry_data = None

        self.updatedData = True
        self.vars = vars
        self.prevTime = 0
        self.updateDelta = .01
        self.filename = filename
        self._isFirst = True

        

        self.r = redis.Redis(redishost,redisport)
        self._file = open(self.filename+'.txt', 'w')
        self.csv_writer = csv.writer(self._file)
        

    def __exit__(self, *args):
        self._file.close()
        

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

        if (self._isFirst):
            self.csv_writer.writerow(telemetry_data)
            self._isFirst = False
        

        self.csv_writer.writerow(telemetry_data.values())
        



        # updatedData = True
        self.prev_telemetry_data = telemetry_data




if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", required=False, help="redis host", type=str,default = "localhost")
    ap.add_argument("--port", required=False, help="redis port", type=int,default = 6379)
    ap.add_argument("--filename",'-f', required=True, help="log filename",type = str)
    ap.add_argument("--vars",nargs='+', required=False, help="plotting variables",type = str,default = None)
    args = vars(ap.parse_args())
    
    g = TelemetryLogger(args["host"],args['port'],args['filename'],args['vars'])
    g.run()