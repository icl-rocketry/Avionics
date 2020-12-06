from packets import *
import matplotlib.pyplot as plt
import numpy as np


class RingBuffer():
	"A 1D ring buffer using numpy arrays"
	def __init__(self, length):
		self.data = np.zeros(length, dtype='f')  # Initialise ring array 'data' as length-array of floats
		self.index = 0  # Initialise InPointer as 0 (where new data begins to be written)

	def extend(self, x):
		"adds array x to ring buffer"
		x_indices = (self.index + np.arange(x.size)) % self.data.size  # Find indices that x will occupy in 'data' array
		self.data[x_indices] = x  # Input the new array into ring buffer ('data')
		self.index = x_indices[-1] + 1  # Find new index for next new data

	def get(self):
		"Returns the first-in-first-out data in the ring buffer (returns data in order of introduction)"
		idx = (self.index + np.arange(self.data.size)) % self.data.size
		return self.data[idx]

class Plotter:

	def __init__(self):
		plt.ion()
		self.rb_length = 100
		self.t = 0
		self.lines = None

		self.time_series = {'t': RingBuffer(self.rb_length),
							'ax': RingBuffer(self.rb_length),  'ay': RingBuffer(self.rb_length), 'az': RingBuffer(self.rb_length),
							'mx': RingBuffer(self.rb_length),  'my': RingBuffer(self.rb_length), 'mz': RingBuffer(self.rb_length),
							'gx': RingBuffer(self.rb_length),  'gy': RingBuffer(self.rb_length), 'gz': RingBuffer(self.rb_length),} # time series for all the different accels

		self.fig = plt.figure()
		self.ax = self.fig.add_subplot(111)

		#line1, = ax.plot(x, y, 'r-') # Returns a tuple of line objects, thus the comma

	def _time_series_append_packet(self, packet: DetailedAll, t: float):
		self.time_series['ax'].extend(np.array(packet.ax))
		self.time_series['ay'].extend(np.array(packet.ay))
		self.time_series['az'].extend(np.array(packet.az))

		self.time_series['mx'].extend(np.array(packet.mx))
		self.time_series['my'].extend(np.array(packet.my))
		self.time_series['mz'].extend(np.array(packet.mz))

		self.time_series['gx'].extend(np.array(packet.gx))
		self.time_series['gy'].extend(np.array(packet.gy))
		self.time_series['gz'].extend(np.array(packet.gz))
	
	def _plot_lines(self):
		plot_vars = ['ax', 'ay', 'az', 'mx', 'my', 'mz', 'gx', 'gy', 'gz']
		if self.lines is None:
			self.lines = []
		
			for var in plot_vars:
				line = self.ax.plot(self.time_series['t'], self.time_series[var])
				self.lines.append(line)
		else:
			for idx, var in enumerate(plot_vars):
				self.lines[idx].set_xdata(self.time_series['t'])
				self.lines[idx].set_ydata(self.time_series[var])

	
	def update(self, packet: DetailedAll, dt: float):
		self.t += dt

		self._time_series_append_packet(packet, self.t)
		self._plot_lines()

		self.fig.canvas.draw()
		self.fig.canvas.flush_events()