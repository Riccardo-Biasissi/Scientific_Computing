import numpy as np
import matplotlib.pyplot as plt

df = np.loadtxt('g++/Task04/integral_value.txt', delimiter='\t', skiprows=1)

exact = (np.exp(np.pi/2)-1)/2

n = df[:, 0]
trapezoidal = df[:, 1]
simpson = df[:, 2]

# mask = n<100
# trapezoidal = trapezoidal[mask]
# simpson = simpson[mask]
# n = n[mask]

trapezoidal = trapezoidal/exact - 1
simpson = simpson/exact - 1
delta = np.abs(trapezoidal - simpson)
print(len(n), len(trapezoidal), len(simpson))

plt.figure(figsize=(10,6))

plt.yscale('symlog')

plt.plot(n, trapezoidal, color='royalblue', label='Trapezoidal Rule')
plt.plot(n, simpson, color='deeppink', label='Simpson Rule')
# plt.plot(n, delta, color='forestgreen', label='Difference')
plt.grid()
plt.legend()
plt.savefig('g++/Task04/benchmark.png', dpi=200, bbox_inches='tight')
plt.close()