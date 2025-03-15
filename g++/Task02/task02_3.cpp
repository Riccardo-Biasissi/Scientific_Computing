#include <iostream>
#include <vector>
#include <cmath>

const int N = 2000;

int main() {
    // Initialize matrices A, B, and C
    std::vector<std::vector<double>> A(N, std::vector<double>(N, 3.0));
    std::vector<std::vector<double>> B(N, std::vector<double>(N, 7.1));
    std::vector<std::vector<double>> C(N, std::vector<double>(N, 0.0));

    // Compute the matrix C = A*B
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < N; ++k) {
            for (int j = 0; j < N; ++j) {
                C[i][j] += A[i][k] * B[k][j] / N;
            }
        }
        // Show progress
        if (i % 10 == 0) {
            std::cout << "Progress: " << i << " / " << N << std::endl;
        }
    }

    // Check that every element of C is equal to 21.3 within a tolerance
    double expected_value = 21.3;
    double tolerance = 1e-3;
    bool correct = true;
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

    return 0;
}