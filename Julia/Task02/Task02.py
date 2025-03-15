import numpy as np
import time

start = time.time()

# Define the scalar and vectors
a = 3.0
N = 1e6  # You can change N to any desired dimension
N = int(N)
x = np.zeros(N) + 0.1
y = np.zeros(N) + 7.1

# Compute the vector sum
d = a * x + y

# Define a tolerance for floating-point comparison
tolerance = 1e-3

# Verify that every element of d is equal to 7.4 within the tolerance
is_correct = all(abs(d - 7.4) < tolerance)

# Print the result
# println("Vector d: ", d)
print("Verification result: ", is_correct)

end = time.time()

print("Execution time: ", end - start, "s")