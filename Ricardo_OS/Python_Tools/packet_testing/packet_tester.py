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
telemetry_deserialized = Telemetry.from_bytes(telemetry_packet.serialize())

print(telemetry_packet.__dict__)
print('\n\n\n')
print(telemetry_deserialized.__dict__)


detailed_pckt = DetailedAll(header, 1.2, -68, 3.25, 7.48, 15.32, -19.4, -24.9,
							12.4, -18.88, 94.32, 182.32, 1800.2, 60000, 50000,
							247, 128.3, 14, 89)
detailed_deserialized = DetailedAll.from_bytes(detailed_pckt.serialize())
print(detailed_pckt.__dict__)
print('\n\n\n')
print(detailed_deserialized.__dict__)

cmd_pckt = Command(header, 8, 14)
cmd_deserialized = Command.from_bytes(cmd_pckt.serialize())
print(cmd_pckt.__dict__)
print('\n\n\n')
print(cmd_deserialized.__dict__)

