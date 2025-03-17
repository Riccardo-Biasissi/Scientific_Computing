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

    // Compute the matrix C = A .* B (element-wise multiplication)
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