#!/usr/bin/python

from numpy import *

data_file = '/tmp/fft.dump.all'
histogram_file = '/tmp/histogram.dat'

x_min, x_max, y_min, y_max = 2400.0, 2500.0, -90.0, 0.0

# Number of 2D regions in which the plot is divided.
x_resolution, y_resolution = 180, 180

def read_angles(line):
    tokens = line.split(' ')
    x = float(tokens[0])
    y = float(tokens[1])
    return [x, y]

points = [read_angles(line) for line in open(data_file)]
points = [p for p in points if p[1]>y_min and p[1]<y_max and p[0]>=x_min and p[1]<=x_max]
count = len(points)
histogram = zeros([x_resolution, y_resolution])
x_interval_length = (x_max - x_min) / x_resolution
y_interval_length = (y_max - y_min) / y_resolution
interval_surface = x_interval_length * y_interval_length
increment = 1000.0 / count / interval_surface

for i in points:
    x = int((i[0] - x_min) / x_interval_length)
    y = int((i[1] - y_min) / y_interval_length)
    histogram[x,y] += increment

x_intervals = arange(x_min, x_max, (x_max - x_min) / x_resolution)
y_intervals = arange(y_min, y_max, (y_max - y_min) / y_resolution)

o = open(histogram_file, 'w')
for i, x in enumerate(x_intervals):
    for j, y in enumerate(y_intervals):
        o.write('%f %f %f \n' % (x, y, histogram[i,j]))
    o.write('\n')
