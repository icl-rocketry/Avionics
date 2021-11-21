# Module (with interactive interface)


# MODULES
# System Modules
import time
import socketio as sio
from datetime import datetime
# User Modules
import parser
import processor


# DUTY FUNCTION
def emitter(data_array):
    for item in data_array:
        package = processor.format_package(item["data"])

        print(package)
        # or socketio script
        
        time.sleep(item["interval"]/1000)
        

# MAIN
def main():
    print("Please enter your log file name (.csv only): ", end = "")
    filename = input()

    try:
        data_array = parser.parse_log(filename)
        print("Starting emission in...3")
        time.sleep(1)
        print("...2")
        time.sleep(1)
        print("...1")
        time.sleep(1)
        emitter(data_array)
    except FileNotFoundError:
        print("Specified file not found.")
        exit(1)
    except parser.FileTypeError:
        print("Invalid file type.")
        exit(1)


if __name__ == "__main__":
    main()
    