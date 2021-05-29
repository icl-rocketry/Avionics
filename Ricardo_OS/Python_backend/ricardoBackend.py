from Interfaces import flaskApi
import argparse
from RicardoHandler import network_manager

# Argument Parsing
ap = argparse.ArgumentParser()
ap.add_argument("-d", "--device", required=True, help="Ricardo Serial Port", type=str)
ap.add_argument("-b", "--baud", required=False, help="Serial Port Baud Rate", type=int,default=115200)
ap.add_argument("-p", "--port", required=False, help="Network Port", type=int,default = 1337)
ap.add_argument("-t", "--tui", required=False, help="Launch Text-User-Interface", type=bool,default = False)
ap.add_argument("-v", "--verbose", required=False, help="Enable Verbose Mode", type=bool,default = False)
args = vars(ap.parse_args())




if __name__ == '__main__':
    flaskApi.socketio.run(flaskApi.app,port=5000)

