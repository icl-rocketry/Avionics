import multiprocessing
import time
import sys
import signal


class TestWorker():
    def __init__(self):
        self.a = 0

    def run(self):
        signal.signal(signal.SIGINT,self.exitHandler)
        signal.signal(signal.SIGTERM,self.exitHandler)
        while True:
            self.a += 1
            print(self.a)
            time.sleep(1)
    
    def exitHandler(self,sig,frame):
        print("terminated")
        sys.exit(0)

def runWorker():
    t = TestWorker()
    t.run()

if __name__ == "__main__":
    p = multiprocessing.Process(target=runWorker)
    p.start()

    input()
    p.terminate()
    p.join()
