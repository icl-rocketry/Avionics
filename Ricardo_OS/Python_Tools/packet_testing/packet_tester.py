"""
Script for testing the sending & parsing the different packets between a desktop PC and a Ricardo board
Written by Pavol Margitfalvi (pm519@ic.ac.uk)
"""

import serial
import argparse
import binascii
from packets import *

header = Header(4, 4, 32)
print(header.serialize())
print('\n\n')
telemetry_packet = Telemetry(header, 0.4, 1.5, -3.7, 42.1, 17.5, 18.4, 872.64, -19.0, 12.32, 10)
print(binascii.hexlify(telemetry_packet.serialize()))

