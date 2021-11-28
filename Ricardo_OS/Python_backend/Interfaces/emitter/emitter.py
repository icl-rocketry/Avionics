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
    