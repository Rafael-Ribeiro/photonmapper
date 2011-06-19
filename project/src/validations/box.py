#!/usr/bin/python

from numpy import *
import pylab as p
import mpl_toolkits.mplot3d.axes3d as p3


fig=p.figure()
ax = p3.Axes3D(fig)

f = open('box', 'r')
data = eval(f.read())

x = map(lambda a: a[0], data);
y = map(lambda a: a[1], data);
z = map(lambda a: a[2], data);
 
ax.scatter3D(x, y, z)
p.show()
