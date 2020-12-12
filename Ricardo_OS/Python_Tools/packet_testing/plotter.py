from packets import *
import matplotlib.pyplot as plt
import numpy as np
import time

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
		plt.style.use('bmh')
		plt.ion()
		self.rb_length = 50
		self.t = 0
		self.lines = None
		self.iter = 0

		self.time_series = {'t': RingBuffer(self.rb_length),
							'ax': RingBuffer(self.rb_length),  'ay': RingBuffer(self.rb_length), 'az': RingBuffer(self.rb_length),
							'mx': RingBuffer(self.rb_length),  'my': RingBuffer(self.rb_length), 'mz': RingBuffer(self.rb_length),
							'gx': RingBuffer(self.rb_length),  'gy': RingBuffer(self.rb_length), 'gz': RingBuffer(self.rb_length),} # time series for all the different accels

		self.fig = plt.figure(figsize=(12.8, 9.6))
		self.ax = self.fig.add_subplot(111)
		plt.show()
		#a = input('')

		#line1, = ax.plot(x, y, 'r-') # Returns a tuple of line objects, thus the comma

	def _time_series_append_packet(self, packet: DetailedAll, t: float):

		self.time_series['t'].extend(np.array(t))

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
				line, = self.ax.plot(self.time_series['t'].get(), self.time_series[var].get(), linewidth = 1, label=var)
				self.ax.legend(loc='upper right')
				self.ax.set_xlabel("Time (s)")
				self.ax.grid()
				self.lines.append(line)
		else:
			for idx, var in enumerate(plot_vars):
				self.lines[idx].set_xdata(self.time_series['t'].get())
				self.lines[idx].set_ydata(self.time_series[var].get())


	def update(self, packet: DetailedAll, dt: float):
		self.t += dt*1E-9

		self._time_series_append_packet(packet, self.t)


		if self.iter > self.rb_length:
			self._plot_lines()
			plt.pause(.01)
			self.ax.relim()
			self.ax.set_xlim(left=self.time_series["t"].get()[1], right=self.time_series["t"].get()[-1])
			self.ax.autoscale_view()
			#self.ax.grid()
			self.fig.canvas.draw()
			self.fig.canvas.flush_events()
		else:
			time.sleep(.001)
		self.iter += 1