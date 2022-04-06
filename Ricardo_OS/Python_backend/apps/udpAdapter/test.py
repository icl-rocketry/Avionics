import multiprocessing
import time

class printer():
    def __init__(self) -> None:
        self.queue = multiprocessing.Queue()

    def run(self):
        while True:
            print(self.queue.get(block=True))
    
if __name__ == "__main__":
    multiprocessing.set_start_method('spawn',force=True)
    printer_obj = printer()
    p = multiprocessing.Process(target=printer_obj.run)
    p.start()

    for i in range(10):
        printer_obj.queue.put(i)
    
    p.join()