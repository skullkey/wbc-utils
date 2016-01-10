#!/usr/bin/python

from numpy import *

data_file = '/tmp/fft.dump.all'
histogram_file = '/tmp/histogram.dat'

x_min, x_max, y_min, y_max = 2300.0, 2500.0, -100.0, 0.0

# Number of 2D regions in which the plot is divided.
x_resolution, y_resolution = 200, 100

def read_angles(line):
    tokens = line.split(' ')
    x = float(tokens[0])
    y = float(tokens[1])
    return [x, y]


histogram = zeros([x_resolution, y_resolution])
loaded_histogram = zeros([x_resolution, y_resolution])
new_histogram = True
x_intervals = arange(x_min, x_max, (x_max - x_min) / x_resolution)
y_intervals = arange(y_min, y_max, (y_max - y_min) / y_resolution)

x_interval_length = (x_max - x_min) / x_resolution
y_interval_length = (y_max - y_min) / y_resolution
interval_surface = x_interval_length * y_interval_length


try:
    for line in open(histogram_file):
        tokens = line.split(' ')
        if(len(tokens)==4):
            a = float(tokens[0])
            b = float(tokens[1])
            c = float(tokens[2])
            x = int((a - x_min) / x_interval_length)
            y = int((b - y_min) / y_interval_length)
            loaded_histogram[x,y] = c
    new_histogram = False
except Exception as e:
    print "failed to load: %s" % str(e)
    loaded_histogram = zeros([x_resolution, y_resolution])


points = [read_angles(line) for line in open(data_file)]
points = [p for p in points if p[1]>y_min and p[1]<y_max and p[0]>=x_min and p[1]<=x_max]
count = len(points)
increment = 1000.0 / count / interval_surface

for i in points:
    x = int((i[0] - x_min) / x_interval_length)
    y = int((i[1] - y_min) / y_interval_length)
    histogram[x,y] += increment


o = open(histogram_file, 'w')
for i, x in enumerate(x_intervals):
    for j, y in enumerate(y_intervals):
        if new_histogram:
            value = histogram[i,j]
        else:
            value = histogram[i,j]*0.3 + 0.7 * loaded_histogram[i,j]
            #if value != 0.0:
            #    print "%.2f %.2f %.2f"% (histogram[i,j], loaded_histogram[i,j],value)
            #if loaded_histogram[i,j] != 0.0:
            #    print "%.2f %.2f %.2f"% (histogram[i,j], loaded_histogram[i,j],value)

        o.write('%f %f %f \n' % (x, y, value))
    o.write('\n')
