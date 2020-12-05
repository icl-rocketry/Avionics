import pandas as pd


class Database:
	
	def __init__(self):
		"""self.db = pd.DataFrame(columns=['ax', 'ay', 'az', 'mx', 'my', 'mz', 'gx', 'gy', 'gz', 
										'gps_lat', 'gps_long', 'gps_alt', 'gps_speed', 'baro_alt',
										'baro_press', 'baro_temp', 'batt_volt', 'batt_percentage'])"""
		self._db = [] # TODO: Make a proper database, for now just an array of detailed all packets
	
	def add(self, packet):
		self._db.append(packet)
	
	def remove(self, index):
		del self._db[index]
	
	def get(self, index):
		return self._db[index]