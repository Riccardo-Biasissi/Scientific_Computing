import numpy as np

x,y,d = np.genfromtxt("g++/Task05/vector_N100000_x.dat"), np.genfromtxt("g++/Task05/vector_N100000_y.dat"), np.genfromtxt("g++/Task05/vector_N100000_d.dat")

print(np.mean(x), np.std(x))
print(np.mean(y), np.std(y))
print(np.mean(d), np.std(d))