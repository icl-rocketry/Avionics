# Module (with interactive interface)

# SYSTEM VARIABLES
external: bool = True

'''PARSER'''
# MODULES
import csv
import copy
import os


# UTILS
# Custom Errors
class FileTypeError(Exception):
    pass

# Handlers
def filename_handler(filename):
    name_array = filename.split('.')
    if (len(name_array) == 1):
        return filename + '.csv'
    else:
        if (name_array[1] != 'csv'):
            raise FileTypeError('Given file is not a .csv file')
        else:
            return filename

def row_counter(reader):
    ans = 0
    for row in reader:
        ans += 1
    return ans

def interval_setter(data_array):
    for index in range(len(data_array) - 1):
        data_array[index]["interval"] = int(data_array[index+1]["data"]["system_time"]) - \
            int(data_array[index]["data"]["system_time"])


# DUTY FUNCTION
def parse_log(filename):
    # opening file
    try:
        filename = filename_handler(filename)
        filepath = os.path.dirname(__file__) + "/data/" + filename
        target = open(filepath, "r")
        # print(filepath)
    except FileTypeError:
        print("FileTypeError: Specified file is not a .csv file.")
        exit(1)
    except FileNotFoundError:
        print("FileNotFoundError: File with name " + filename + " not found.")
        exit(1)

    # reading csv
    reader = csv.reader(target)
    
    column_name = []
    parsed_data = []

    row_num = 0
    row_object = { "index": 0, "interval": 0, "data": {} }
    for row_content in reader:
        if (row_num == 0):
            column_name = row_content

        else:
            row_object["index"] = row_num - 1

            column_num = 0
            for column in column_name:
                row_object["data"][column] = row_content[column_num]
                column_num += 1
            
            parsed_data.append(copy.deepcopy(row_object))

        row_num += 1

    # calculate interval from log data
    interval_setter(parsed_data)
    
    return parsed_data


# MODULES
# System Modules
import time
import eventlet
import importlib
# User Modules
# from processor import processor
# processor = importlib.import_module("processor", package="processor.py")
# from parser_csv import parser_csv
# parser_csv = importlib.import_module("parser_csv", package="parser_csv.py")
try:
    from __main__ import socketio
except ImportError:
    external = False
    print("InportError: Failed to import socketio from __main__")
    

'''PROCESSOR'''
# MODULES
from datetime import datetime
import json


# UTILS
# convert current time into javascript time
def system_time():
    return int((datetime.now() - datetime(1970, 1, 1)).total_seconds())

def decode_double_string(string):
    return float(string)

def decode_boolean_string(string):
    if (string == "TRUE"):
        return 1
    else:
        return 0

def decode_system_status(system_status):
    binary_string = format(int(system_status), "029b")[::-1]
    return binary_string


# DUTY FUNCTION
def format_package(data_dict):
    for key in data_dict:
        if (key == "system_status"):
            data_dict[key] = decode_system_status(data_dict[key])
        elif (key == "system_time"):
            data_dict[key] = system_time()
        elif (key == "connection_status"):
            data_dict[key] = decode_boolean_string(data_dict[key])
        else:
            data_dict[key] = decode_double_string(data_dict[key])
    return json.dumps(data_dict, indent = 2)


'''ACTUAL'''
# DUTY FUNCTION
def __emitter(data_array):
    for item in data_array:
        package = format_package(item["data"])

        # emit socketio signal
        socketio.emit("package", package, namespace="/telemetry",broadcast=True)
        # print package when emitted
        print(package)

        eventlet.sleep(item["interval"]/1000)

def emitter(filename):
    data_array = parse_log(filename)
    __emitter(data_array)
        

# MAIN
def main():
    print("Please enter your log file name (.csv only): ", end = "")
    filename = input()

    try:
        data_array = parse_log(filename)
        print("Starting emission in...3")
        time.sleep(1)
        print("...2")
        time.sleep(1)
        print("...1")
        time.sleep(1)
        __emitter(data_array)
    except FileNotFoundError:
        print("Specified file not found.")
        exit(1)
    except FileTypeError:
        print("Invalid file type.")
        exit(1)


if __name__ == "__main__":
    main()
    