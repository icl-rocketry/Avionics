
import serial
import argparse
import time
import signal
import sys

ap = argparse.ArgumentParser()
ap.add_argument("-p", "--port", required=True, help="Port hosting ricardo", type=str)
ap.add_argument("-f", "--filename", required=True, help="filename", type=str)
args = vars(ap.parse_args())

ser = serial.Serial(port = args['port'],baudrate = 115200)  # open serial port
#ser.rtscts = True
#ser.dsrdtr = True
ser.stopbits = serial.STOPBITS_ONE
ser.parity = serial.PARITY_NONE
ser.bytesize = serial.EIGHTBITS

ser.setDTR(False)
time.sleep(1)
ser.flushInput()
ser.setDTR(True)

data = ""

def exitBackend(signalNumber, frame):
    file = open(args["filename"]+'.txt', 'w')
    file.write(data)
    file.close()

    sys.exit(0)

signal.signal(signal.SIGINT, exitBackend)
signal.signal(signal.SIGTERM, exitBackend)


while True:
    b = ser.read()
    try:
        bt = b.decode("utf-8")
    except:
        bt = str(b)
    print(bt)
    data += bt
