# Module (without interactive interface)


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