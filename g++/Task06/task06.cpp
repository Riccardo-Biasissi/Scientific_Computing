#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fftw3.h>
#include <algorithm>
#include <complex>
#include <iomanip>

// Function to generate a squared matrix filled with random values of rank 2
std::vector<std::vector<double>> generateRandomMatrix(int N) {
    std::vector<std::vector<double>> matrix(N, std::vector<double>(N));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<long double> dis(1.0, 1.0);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = dis(gen);
        }
    }
    return matrix;
}

// Function to perform the FFT complex-to-complex on a 2D matrix using FFTW
std::vector<std::vector<std::complex<double>>> perform_c2c_FFT2D(const std::vector<std::vector<double>>& A) {
    int N = A.size();  // Dimension of the square matrix
    std::vector<std::vector<std::complex<double>>> C(N, std::vector<std::complex<double>>(N));

    // FFTW requires to allocate memory for two arrays of complex numbers
    fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * N);
    fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * N);

    // Transform the input matrix A into the FFTW complex format
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            in[i * N + j][0] = A[i][j];   // Real part
            in[i * N + j][1] = 0.0;       // Imaginary part
        }
    }

    // FFTW requires a plan to execute the FFT (FORWARD)
    fftw_plan plan = fftw_plan_dft_2d(N, N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    // Execute the forward FFT
    fftw_execute(plan);

    // Fill the output matrix with complex values
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = std::complex<double>(out[i * N + j][0], out[i * N + j][1]);
        }
    }

    // Clean up to free the allocated memory
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return C;
}

// Function to perform the IFFT complex-to-complex on a 2D matrix using FFTW
std::vector<std::vector<std::complex<double>>> perform_c2c_IFFT2D(const std::vector<std::vector<std::complex<double>>>& C) {
    int N = C.size();  // Dimension of the square matrix
    std::vector<std::vector<std::complex<double>>> reconstructed(N, std::vector<std::complex<double>>(N));

    // Create FFTW plans for inverse FFT
    fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * N);
    fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * N);

    // Fill the input array from the complex output matrix C
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            in[i * N + j][0] = C[i][j].real();   // Real part
            in[i * N + j][1] = C[i][j].imag();   // Imaginary part
        }
    }

    // Create a plan for the inverse (BACKWARD) FFT
    fftw_plan plan = fftw_plan_dft_2d(N, N, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);

    // Execute the inverse FFT
    fftw_execute(plan);

    // Fill the reconstructed matrix with complex values (normalize by N*N)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            reconstructed[i][j] = std::complex<double>(out[i * N + j][0] / (N * N), out[i * N + j][1] / (N * N));
        }
    }

    // Clean up to free the allocated memory
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return reconstructed;
}

// Structure to hold the error statistics, similar to dictionary in Python but with fixed length
struct ErrorStats {
    double root_mean_sq_abs_error;
    double root_mean_sq_rel_error;
    double root_median_sq_abs_error;
    double root_median_sq_rel_error;
};

// Function to compute error statistics between the original and reconstructed matrices
ErrorStats computeErrorStats(
    const std::vector<std::vector<double>>& A,                                      // Original matrix
    const std::vector<std::vector<std::complex<double>>>& A_reconstructed           // Reconstructed matrix after IFFT
) {
    double mean_sq_abs_error = 0.0;                                                 // Mean square absolute error
    double mean_sq_rel_error = 0.0;                                                 // Mean square relative error
    std::vector<double> sq_abs_errors;                                              // Vector to store square of absolute errors
    std::vector<double> sq_rel_errors;                                              // Vector to store square of relative errors
    size_t N = A.size();                                                            // Dimension of the square matrix

    // Calculate absolute and relative errors, and their squares
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            double abs_error = std::abs(A[i][j] - A_reconstructed[i][j].real());    // Absolute error: x0 - x0'
            double rel_error = abs_error / std::abs(A_reconstructed[i][j].real());  // Relative error: (x0 - x0') / |x0'|
            double sq_abs_error = abs_error * abs_error;                            // Square of absolute error
            double sq_rel_error = rel_error * rel_error;                            // Square of relative error
            sq_abs_errors.push_back(sq_abs_error);                                  // Store square of absolute error
            sq_rel_errors.push_back(sq_rel_error);                                  // Store square of relative error
            mean_sq_abs_error += sq_abs_error;                                      // Accumulate mean square absolute error
            mean_sq_rel_error += sq_rel_error;                                      // Accumulate mean square relative error
        }
    }

    // Normalize the mean square errors by the total number of elements in the matrix (N*N)
    mean_sq_abs_error /= (N * N);
    mean_sq_rel_error /= (N * N);

    // Sort the square errors to compute median values
    std::sort(sq_abs_errors.begin(), sq_abs_errors.end());
    std::sort(sq_rel_errors.begin(), sq_rel_errors.end());
    double median_sq_abs_error, median_sq_rel_error;
    size_t n = sq_abs_errors.size();
    if (n % 2 == 0) {       // If the number of elements is even, take the average of the two middle elements
        median_sq_abs_error = (sq_abs_errors[n / 2 - 1] + sq_abs_errors[n / 2]) / 2.0;
        median_sq_rel_error = (sq_rel_errors[n / 2 - 1] + sq_rel_errors[n / 2]) / 2.0;
    } else {                // If the number of elements is odd, take the middle element
        median_sq_abs_error = sq_abs_errors[n / 2];
        median_sq_rel_error = sq_rel_errors[n / 2];
    }

    // Create an ErrorStats object to hold the results
    ErrorStats stats;
    stats.root_mean_sq_abs_error = std::sqrt(mean_sq_abs_error);                    // Square root of mean square absolute error
    stats.root_mean_sq_rel_error = std::sqrt(mean_sq_rel_error);                    // Square root of mean square relative error
    stats.root_median_sq_abs_error = std::sqrt(median_sq_abs_error);                // Square root of median square absolute error
    stats.root_median_sq_rel_error = std::sqrt(median_sq_rel_error);                // Square root of median square relative error
    return stats;
}

// Overload for real-valued reconstructed matrix
ErrorStats computeErrorStats(
    const std::vector<std::vector<double>>& A,                                      // Original matrix
    const std::vector<std::vector<double>>& A_reconstructed                         // Reconstructed matrix after IFFT (real)
) {
    double mean_sq_abs_error = 0.0;                                                 // Mean square absolute error
    double mean_sq_rel_error = 0.0;                                                 // Mean square relative error
    std::vector<double> sq_abs_errors;                                              // Vector to store square of absolute errors
    std::vector<double> sq_rel_errors;                                              // Vector to store square of relative errors
    size_t N = A.size();                                                            // Dimension of the square matrix

    // Calculate absolute and relative errors, and their squares
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            // In the following, we assume A_reconstructed is a real-valued matrix
            // therefore there is no need to take its real() part as above
            double abs_error = std::abs(A[i][j] - A_reconstructed[i][j]);           // Absolute error: x0 - x0'
            double rel_error = abs_error / std::abs(A_reconstructed[i][j]);         // Relative error: (x0 - x0') / |x0'|
            double sq_abs_error = abs_error * abs_error;                            // Square of absolute error
            double sq_rel_error = rel_error * rel_error;                            // Square of relative error
            sq_abs_errors.push_back(sq_abs_error);                                  // Store square of absolute error
            sq_rel_errors.push_back(sq_rel_error);                                  // Store square of relative error
            mean_sq_abs_error += sq_abs_error;                                      // Accumulate mean square absolute error
            mean_sq_rel_error += sq_rel_error;                                      // Accumulate mean square relative error
        }
    }

    // Normalize the mean square errors by the total number of elements in the matrix (N*N)
    mean_sq_abs_error /= (N * N);
    mean_sq_rel_error /= (N * N);

    // Sort the square errors to compute median values
    std::sort(sq_abs_errors.begin(), sq_abs_errors.end());
    std::sort(sq_rel_errors.begin(), sq_rel_errors.end());
    double median_sq_abs_error, median_sq_rel_error;
    size_t n = sq_abs_errors.size();
    if (n % 2 == 0) {       // If the number of elements is even, take the average of the two middle elements
        median_sq_abs_error = (sq_abs_errors[n / 2 - 1] + sq_abs_errors[n / 2]) / 2.0;
        median_sq_rel_error = (sq_rel_errors[n / 2 - 1] + sq_rel_errors[n / 2]) / 2.0;
    } else {                // If the number of elements is odd, take the middle element
        median_sq_abs_error = sq_abs_errors[n / 2];
        median_sq_rel_error = sq_rel_errors[n / 2];
    }

    // Create an ErrorStats object to hold the results
    ErrorStats stats;
    stats.root_mean_sq_abs_error = std::sqrt(mean_sq_abs_error);                        // Square root of mean square absolute error
    stats.root_mean_sq_rel_error = std::sqrt(mean_sq_rel_error);                        // Square root of mean square relative error
    stats.root_median_sq_abs_error = std::sqrt(median_sq_abs_error);                    // Square root of median square absolute error
    stats.root_median_sq_rel_error = std::sqrt(median_sq_rel_error);                    // Square root of median square relative error
    return stats;
}

// Function to perform the FFT real-to-complex on a 2D matrix using FFTW
std::vector<std::vector<std::complex<double>>> perform_r2c_FFT2D(const std::vector<std::vector<double>>& A) {
    int N = A.size();  // Dimension of the square matrix

    // Create a 2D vector to hold the complex output
    // Note: For even N, the second dimension is (N/2 + 1), for odd N it is ((N + 1) / 2)
    // This is because the FFT of a real input produces a complex output with half the frequency bins
    // Therefore if N=8, the real-to-complex FFT will produce an output of size 8 x 5
    std::vector<std::vector<std::complex<double>>> R(N, std::vector<std::complex<double>>((N % 2 == 0) ? (N / 2 + 1) : ((N + 1) / 2)));

    // Allocate input as double* for real data, output as fftw_complex*
    double* in = (double*)fftw_malloc(sizeof(double) * N * N);
    fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * (N / 2 + 1));

    // Fill the input array with the real values from A
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            in[i * N + j] = A[i][j];
        }
    }

    // FFTW requires a plan to execute the FFT (FORWARD)
    fftw_plan plan = fftw_plan_dft_r2c_2d(N, N, in, out, FFTW_ESTIMATE);

    // Execute the forward FFT
    fftw_execute(plan);

    // Fill the output matrix with complex values
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < (N / 2 + 1); ++j) {
            R[i][j] = std::complex<double>(out[i * (N / 2 + 1) + j][0], out[i * (N / 2 + 1) + j][1]);
        }
    }

    // Clean up to free the allocated memory
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return R;
}

// Function to perform the IFFT complex-to-real on a 2D matrix using FFTW (in numpy it is np.fft.irfft2)
std::vector<std::vector<double>> perform_c2r_IFFT2D(const std::vector<std::vector<std::complex<double>>>& C) {
    int N = C.size();  // Dimension of the square matrix
    std::vector<std::vector<double>> reconstructed(N, std::vector<double>(N));

    // Allocate input as fftw_complex* for complex data, output as double*
    fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N * (N / 2 + 1));
    double* out = (double*)fftw_malloc(sizeof(double) * N * N);

    // Fill the input array from the complex output matrix C
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < (N / 2 + 1); ++j) {
            in[i * (N / 2 + 1) + j][0] = C[i][j].real();   // Real part
            in[i * (N / 2 + 1) + j][1] = C[i][j].imag();   // Imaginary part
        }
    }

    // Create a plan for the inverse (BACKWARD) FFT
    fftw_plan plan = fftw_plan_dft_c2r_2d(N, N, in, out, FFTW_ESTIMATE);

    // Execute the inverse FFT
    fftw_execute(plan);

    // Fill the reconstructed matrix with real values (normalize by N*N)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            reconstructed[i][j] = out[i * N + j] / (N * N);
        }
    }

    // Clean up to free the allocated memory
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return reconstructed;
}

int main() {
    // Define the size of the matrix (NxN)
    int N = 1000;
    // Generate a random NxN matrix
    std::vector<std::vector<double>> A = generateRandomMatrix(N);

    // Set precision for output
    // std::cout << std::fixed << std::setprecision(10);

    // // Print the generated matrix
    // std::cout << "Generated Matrix A:" << std::endl;
    // for (const auto& row : A) {
    //     for (const auto& val : row) {
    //         std::cout << val << " ";
    //     }
    //     std::cout << std::endl;
    // }

    std::cout << std::endl;

    // Perform FFT on the matrix
    std::vector<std::vector<std::complex<double>>> C;
    C = perform_c2c_FFT2D(A);

    // // Print the resulting matrix after FFT
    // std::cout << "Resulting Matrix C after FFT:" << std::endl;
    // for (const auto& row : C) {
    //     for (const auto& val : row) {
    //         std::cout << val << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // Perform inverse FFT to reconstruct the original matrix
    std::vector<std::vector<std::complex<double>>> A_reconstructed = perform_c2c_IFFT2D(C);

    // // Print the reconstructed matrix
    // std::cout << std::endl << "Reconstructed Matrix from C:" << std::endl;
    // for (const auto& row : A_reconstructed) {
    //     for (const auto& val : row) {
    //         std::cout << val << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;
    
    // Compute error statistics between the original and reconstructed matrices
    ErrorStats stats = computeErrorStats(A, A_reconstructed);
    double mean_sq_abs_error = stats.root_mean_sq_abs_error * stats.root_mean_sq_abs_error;
    double mean_sq_rel_error = stats.root_mean_sq_rel_error * stats.root_mean_sq_rel_error;
    double median_sq_abs_error = stats.root_median_sq_abs_error * stats.root_median_sq_abs_error;
    double median_sq_rel_error = stats.root_median_sq_rel_error * stats.root_median_sq_rel_error;
    std::cout << "c2c FFT Error Statistics:" << std::endl;
    std::cout << "Root Mean Square Absolute Error (Complex-to-Complex):\t" << std::sqrt(mean_sq_abs_error) << std::endl;
    std::cout << "Root Mean Square Relative Error (Complex-to-Complex):\t" << std::sqrt(mean_sq_rel_error) << std::endl;
    std::cout << "Root Median Square Absolute Error (Complex-to-Complex):\t" << std::sqrt(median_sq_abs_error) << std::endl;
    std::cout << "Root Median Square Relative Error (Complex-to-Complex):\t" << std::sqrt(median_sq_rel_error) << std::endl;

    std::cout << std::endl;
    

    // Perform real-to-complex FFT
    std::vector<std::vector<std::complex<double>>> R = perform_r2c_FFT2D(A);

    // // Print the resulting matrix after real-to-complex FFT
    // std::cout << std::endl << "Resulting Matrix R after Real-to-Complex FFT:" << std::endl;
    // for (const auto& row : R) {
    //     for (const auto& val : row) {
    //         std::cout << val << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;

    // Perform complex-to-real IFFT
    std::vector<std::vector<double>> A_reconstructed_r2c = perform_c2r_IFFT2D(R);

    // // Print the reconstructed matrix from real-to-complex IFFT
    // std::cout << "Reconstructed Matrix from R (Real-to-Complex IFFT):" << std::endl;
    // for (const auto& row : A_reconstructed_r2c) {
    //     for (const auto& val : row) {
    //         std::cout << val << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;

    // Compute error statistics between the original and reconstructed matrices from real-to-complex IFFT
    ErrorStats stats_r2c = computeErrorStats(A, A_reconstructed_r2c);
    double mean_sq_abs_error_r2c = stats_r2c.root_mean_sq_abs_error * stats_r2c.root_mean_sq_abs_error;
    double mean_sq_rel_error_r2c = stats_r2c.root_mean_sq_rel_error * stats_r2c.root_mean_sq_rel_error;
    double median_sq_abs_error_r2c = stats_r2c.root_median_sq_abs_error * stats_r2c.root_median_sq_abs_error;
    double median_sq_rel_error_r2c = stats_r2c.root_median_sq_rel_error * stats_r2c.root_median_sq_rel_error;
    std::cout << "r2c FFT Error Statistics:" << std::endl;
    std::cout << "Root Mean Square Absolute Error (Real-to-Complex):\t" << std::sqrt(mean_sq_abs_error_r2c) << std::endl;
    std::cout << "Root Mean Square Relative Error (Real-to-Complex):\t" << std::sqrt(mean_sq_rel_error_r2c) << std::endl;
    std::cout << "Root Median Square Absolute Error (Real-to-Complex):\t" << std::sqrt(median_sq_abs_error_r2c) << std::endl;
    std::cout << "Root Median Square Relative Error (Real-to-Complex):\t" << std::sqrt(median_sq_rel_error_r2c) << std::endl;

    std::cout << std::endl;

    // Compute the sum of all values of the original matrix A
    double sum_A = 0.0;
    for (const auto& row : A) {
        for (const auto& val : row) {
            sum_A += val;
        }
    }
    std::cout << "Sum of all values in the original matrix A:\t" << sum_A << std::endl;

    // Print the C[0,0] and R[0,0] values
    std::cout << "C[0][0].real (Complex-to-Complex FFT):\t\t" << C[0][0].real() << std::endl;
    std::cout << "R[0][0].real (Real-to-Complex FFT):\t\t" << R[0][0].real() << std::endl;

    return 0;
}