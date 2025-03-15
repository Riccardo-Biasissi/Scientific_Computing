#include <iostream>     // Standard input/output stream
#include <vector>       // Vector container
#include <cassert>      // Assert function
#include <chrono>       // Time measurement
#include <cmath>        // Math functions

int main() {
    const int N = 1e6;              // Dimension of the vectors
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
    for (int i = 0; i < N; ++i) {
        assert(std::fabs(d[i] - expected_value) < tolerance);
    }

    // Stop measuring time
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Verification result: true" << std::endl;
    std::cout << "Execution time: " << elapsed.count() << " seconds." << std::endl;

    return 0;
}