import multiprocessing
import argparse
import signal
import sys
import redis

from Interfaces import commandlineinterface
from Interfaces import flaskinterface

from RicardoHandler import serialmanager
from RicardoHandler import telemetryhandler
from RicardoHandler import telemetrylogger



# Argument Parsing
ap = argparse.ArgumentParser()
ap.add_argument("-d", "--device", required=True, help="Ricardo Serial Port", type=str)
ap.add_argument("-b", "--baud", required=False, help="Serial Port Baud Rate", type=int,default=115200)
ap.add_argument("--flask-host", required=False, help="flask host", type=str,default="0.0.0.0")
ap.add_argument("--flask-port", required=False, help="flask Port", type=int,default = 1337)
ap.add_argument("-v", "--verbose", required=False, help="Enable Verbose Mode", action='store_true')
ap.add_argument("--redis-host", required=False, help="redis host", type=str,default = "localhost")
ap.add_argument("--redis-port", required=False, help="redis port", type=int,default = 6379)
ap.add_argument('-l','--logger', required=False, help="Enable Telemetry logging",action='store_true',default=False)

argsin = vars(ap.parse_args())


def exitBackend(proclist):
    for key in proclist:
        print("Killing: " + key + " Pid: " + str(proclist[key].pid))
        proclist[key].terminate()
        proclist[key].join()

    proclist = {}

    sys.exit(0)


def checkRedis():
    try:
        server = redis.Redis(host=argsin["redis_host"],port=argsin["redis_port"])
        server.ping()
    except redis.exceptions.ConnectionError:
        errormsg = "[ERROR] -> Redis server not found at host:'" + argsin["redis_host"] + "' port:" + str(argsin["redis_port"]) + "\nPlease check redis is running"
        sys.exit(errormsg)

def startSerialManager(args):
    serman = serialmanager.SerialManager(device = args["device"],
                                     baud = args["baud"],
                                     redishost = args["redis_host"],
                                     redisport=args["redis_port"])
    serman.run()

def startTelemetryHandler(args,taskid):
    telemetrytask = telemetryhandler.TelemetryHandler(redishost = args["redis_host"],
                                                      redisport=args["redis_port"],
                                                      clientid=taskid)
    telemetrytask.run()

def startTelemetryLogger(args):
    logger = telemetrylogger.TelemetryLogger(redishost=args['redis_host'],
                                                 redisport=args['redis_port'],
                                                 filename="telemetry_log")
    logger.run()

if __name__ == '__main__':
    proclist = {}
    #check redis server is running
    checkRedis()

    proclist['serialmanager'] = multiprocessing.Process(target=startSerialManager,args=(argsin,))
    proclist['serialmanager'].start()


    #start telemetry handler process
    telemetrytask_id = "LOCAL:TELEMETRYTASK"
    proclist['telemetryhandler'] = multiprocessing.Process(target=startTelemetryHandler,args=(argsin,telemetrytask_id,))
    proclist['telemetryhandler'].start()

    
    #start flask interface process
    proclist['flaskinterface'] = multiprocessing.Process(target=flaskinterface.startFlaskInterface,args=(argsin['flask_host'],
                                                                                            argsin['flask_port'],
                                                                                            argsin['redis_host'],
                                                                                            argsin['redis_port'],))
    proclist['flaskinterface'].start()


    if (argsin['logger']):
        proclist['telemetrylogger'] = multiprocessing.Process(target=startTelemetryLogger,args=(argsin,))
        proclist['telemetrylogger'].start()
    

    c = commandlineinterface.CommandLineInterface(redishost=argsin['redis_host'],
                                                    redisport=argsin['redis_port'],
                                                    )
    c.cmdloop()
    exitBackend(proclist)





 