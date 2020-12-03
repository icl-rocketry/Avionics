
import serial
import argparse
import time

ap = argparse.ArgumentParser()
ap.add_argument("-p", "--port", required=True, help="Port hosting ricardo", type=str)
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

while True:
    b = ser.read()
    in_hex = hex(int.from_bytes(b,byteorder='big'))
    print(b)
    print(in_hex)
    print ('%x' % ord(b))
