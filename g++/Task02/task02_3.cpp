#include <iostream>
#include <vector>
#include <cmath>
#include <Eigen/Dense>

const int N = 10000;

int main() {
    // Initialize matrices A and B
    Eigen::MatrixXd A = Eigen::MatrixXd::Random(N, N);
    Eigen::MatrixXd B = Eigen::MatrixXd::Random(N, N);
    Eigen::MatrixXd C = A * B; // Uses highly optimized Eigen multiplication

    // Print "End of multiplication" to the console
    std::cout << "End of multiplication" << std::endl;

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