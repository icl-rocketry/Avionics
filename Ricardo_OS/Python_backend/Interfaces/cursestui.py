import redis
import multiprocessing
import time
import json
import curses

class CursesTUI(multiprocessing.Process):
    def __init__(self,redishost = 'localhost',redisport = 6379):
        super(CursesTUI,self).__init__()
        #setup curses screen
        self.stdscr = curses.initscr()
        curses.noecho()
        curses.cbreak()
        self.stdscr.keypad(True)

        
        self.r = redis.Redis(host=redishost,port=redisport)
        self.exit_event = multiprocessing.Event()

    def run(self):
        while not self.exit_event.is_set():
            #do some stuff
            pass
        #call exit handler when exiting
        self.__onExit__()
    
    def stop(self):
        self.exit_event.set()
        

    def __onExit__(self):
        curses.nocbreak()
        self.stdscr.keypad(False)
        curses.echo()
        curses.endwin()

    def __getTelemetry__(self):
        return self.r.get("telemetry")
    
    def __decodeSystemStatus__(self,SystemStatusVariable):
        pass
