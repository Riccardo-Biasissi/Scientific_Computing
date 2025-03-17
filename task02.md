# Task02 assignment

Markdown file regarding the task02. The codes I selected to delve in during this course are Julia and C++.

## Point a

I performed tasks 1/2 and 3 in two separate scripts.

### Julia

Point 1 and 2

```
# Measure the execution time for comparison with compiled language (C++)
@time begin
    # Define the dimensions
    N = 1e8
    N = Int(N)
    
    # Define the scalar and vectors
    a = 3.0
    x = fill(0.1, N)
    y = fill(7.1, N)

    # Compute the vector sum
    d = a .* x .+ y

    # Define a tolerance for floating-point comparison
    expected_value = 7.4
    tolerance = 1e-3

    # Verify that every element of d is equal to 7.4 within the tolerance
    is_correct = all(abs.(d .- expected_value) .< tolerance)

    # println("Vector d: ", d)
    println("Verification result: ", is_correct)
end
```

Point 3

```
# Measure the execution time for comparison with compiled language (C++)
@time begin
    # Define the dimensions
    N = 10000
    N = Int(N)

    # Create matrices A and B
    A = fill(3.0, N, N)
    B = fill(7.1, N, N)

    # Perform the multiplication
    C = A .* B

    # Define the expected value and tolerance
    expected_value = 21.3
    tolerance = 1e-3

    # Check that every element of C is equal to the expected value within the tolerance
    is_correct = all(abs.(C .- expected_value) .< tolerance)

    # Print the result
    println("Verification result: ", is_correct)
end
```

### C++

Point 1 and 2

```
#include <iostream>     // Standard input/output stream
#include <vector>       // Vector container
#include <cassert>      // Assert function
#include <chrono>       // Time measurement
#include <cmath>        // Math functions

int main() {
    const int N = 1e8;              // Dimension of the vectors
    double a = 3.0;                 // Scalar value a
    std::vector<double> x(N, 0.1);  // Vector x with all values 0.1
    std::vector<double> y(N, 7.1);  // Vector y with all values 7.1
    std::vector<double> d(N);       // Resultant vector d

    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    // Compute d = a * x + y
    for (int i = 0; i < N; ++i) {
        d[i] = a * x[i] + y[i];
    }

    // Verify that every element of d is equal to 7.4 within the tolerance
    double expected_value = 7.4;    // Expected value of the elements in d
    double tolerance = 1e-3;        // Tolerance for the verification
    bool correct = true;            // Flag to indicate correctness
    for (int i = 0; i < N; ++i) {
        if (std::fabs(d[i] - expected_value) > tolerance) {
            correct = false;
            break;
        }
    }

    if (correct) {
        std::cout << "All elements of vector d are equal to 21.3 within the tolerance." << std::endl;
    } else {
        std::cout << "Some elements of vector d are not equal to 21.3 within the tolerance." << std::endl;
    }

    // Stop measuring time
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Execution time: " << elapsed.count() << " seconds." << std::endl;

    return 0;
}
```

Point 3

```
#include <iostream>     // Standard input/output stream
#include <vector>       // Vector container
#include <cassert>      // Assert function
#include <chrono>       // Time measurement
#include <cmath>        // Math functions

int N = 10000;

int main() {
    // Initialize matrices A, B, and C
    std::vector<std::vector<double>> A(N, std::vector<double>(N, 3.0));
    std::vector<std::vector<double>> B(N, std::vector<double>(N, 7.1));
    std::vector<std::vector<double>> C(N, std::vector<double>(N, 0.0));

    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    // Compute the matrix C = A * B (element-wise multiplication)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = A[i][j] * B[i][j];
        }
    }

    // Check that every element of C is equal to 21.3 within a tolerance
    double expected_value = 21.3;   // Expected value of the elements in C
    double tolerance = 1e-3;        // Tolerance for the verification
    bool correct = true;            // Flag to indicate correctness
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (std::fabs(C[i][j] - expected_value) > tolerance) {
                correct = false;
                break;
            }
        }
        if (!correct) break;
    }

    if (correct) {
        std::cout << "All elements of matrix C are equal to 21.3 within the tolerance." << std::endl;
    } else {
        std::cout << "Some elements of matrix C are not equal to 21.3 within the tolerance." << std::endl;
    }

    // Stop measuring time
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Execution time: " << elapsed.count() << " seconds." << std::endl;

    return 0;
}
```

## Point b

- I did not encounter problems in the tasks assigned. However, the expression of point 3 puzzled me a bit: we were asked to compute the multiplication of two matrices $A$ (filled with $3.0$) and $B$ (filled with $7.1$) in order to recover matrix $C$ such that:

$$c_{ij} = \sum_{k}{a_{ik} \cdot b_{kj}}$$

but this computation results in differents values from those we were asked to check (21.3). For example, for dimension N=2, the resulting C matrix will be filled with 42.6. The only way to correct the results would have been to perform this computation instead:

$$c_{ij} = \frac{1}{N} \sum_{k}{a_{ik} \cdot b_{kj}}$$

In the end, I believed that the task revolved in computing just the element-wise multiplication (in other words, $A_{i,j} \cdot B_{i,j}$) since this does not require the 1/N factor.

- I was able to correctly check the results. The resulting vector d and matrix C are affected by floating point limit. The way I used to surprass this constraint, is to verify the computed $R_c$ and theoretical $R_t$ results within an arbitrary tolerance $T$ (in my code, 1e-3 proved to be small enough). The condition I used in the verification is:
$$|R_c - R_t| < T$$

## Computation times

I compared the execution times for the scripts in Julia and C++. For the tasks 1 and 2 (N=1e8), Julia required around 0.60-1.00s while C++ took consistently 0.52-0.54s. Regarding the task 3 (N=1e4) the computation times were 0.60-0.75s for Julia and 0.88-0.90s for C++ (clearly there is space for large improvements in my code regarding task 3!).