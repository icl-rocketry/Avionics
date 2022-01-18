# Module (with interactive interface)


# MODULES
# System Modules
import time
import eventlet
# User Modules
try:
    from emitter.util import parser_csv as parser_csv
    from emitter.util import processor as processor
except ModuleNotFoundError:
    from util import parser_csv as parser_csv
    from util import processor as processor
# System Variables
external:bool = True
try:
    from __main__ import socketio
except ImportError:
    external = False


#Class implementation to respect the kill signal from flaskinterface
class EmitterClass():
    def __init__(self,filename:str,loop:bool = False):
        self.data_array = parser_csv.parse_log(filename)
        self.index = 0
        self.loop = loop

    def emit(self):
        if self.index < len(self.data_array):
            item = self.data_array[self.index]
            package = processor.format_package(item["data"])
            if external:
                # emit socketio signal
                print("Emitting package " + str(self.index))
                socketio.emit("telemetry", package, namespace="/telemetry",broadcast=True)
            else:
                # print package when emitted
                print(package)

            self.index += 1
            if self.loop and self.index == len(self.data_array):
                self.index = 0

            eventlet.sleep(item["interval"]/1000)
        return

        
              
        

# DUTY FUNCTION
def emitter(data_array):
    index = 0
    for item in data_array:
        package = processor.format_package(item["data"])

        if external:
            # emit socketio signal
            print("Emitting package " + str(index))
            socketio.emit("package", package, namespace="/telemetry",broadcast=True)
        else:
            # print package when emitted
            print(package)

        index += 1
        eventlet.sleep(item["interval"]/1000)

def external_emitter(filename):
    data_array = parser_csv.parse_log(filename)
    emitter(data_array)


# MAIN
def main():
    print("Please enter your log file name (.csv only): ", end = "")
    filename = input()

    try:
        data_array = parser_csv.parse_log(filename)
        print("Starting emission in...3")
        time.sleep(1)
        print("Starting emission in...2")
        time.sleep(1)
        print("Starting emission in...1")
        time.sleep(1)
        emitter(data_array)
    except FileNotFoundError:
        print("Specified file not found.")
        exit(1)
    except parser_csv.FileTypeError:
        print("Invalid file type.")
        exit(1)


if __name__ == "__main__":
    main()
    