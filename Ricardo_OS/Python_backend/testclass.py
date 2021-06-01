import threading
import time
import eventlet
eventlet.monkey_patch()

class testclass(threading.Thread):
    def __init__(self,socketio):
        self.prev_time = 0
        self.updateTimePeriod = 200e6
        self.instance = socketio
        self.exit_event = threading.Event()


        #super(testclass,self).__init__(self)
        super().__init__()

    def run(self):
        while not self.exit_event.is_set():
            if (time.time_ns() - self.prev_time > self.updateTimePeriod):
                
                print("emitt")
                self.instance.emit('response', self.prev_time, broadcast=True,namespace='/')
                self.prev_time = time.time_ns()
            
            eventlet.sleep(.01)
            

    def stop(self):
        print("stopping Thread")
        self.exit_event.set()
    
    def get(self):
        return self.prev_time