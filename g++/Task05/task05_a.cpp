#include <iostream> // Used for input/output
#include <iomanip>  // Used for input/output manipulators like std::setprecision
#include <numeric>  // Needed for std::accumulate
#include <vector>   // Used for vector operations

// Function to compute the sum with the for loop
double sum_for_loop(std::vector<long double> vec) {
    long double sum = 0.0;   // Initialize the sum
    int n = vec.size(); // Get the size of the vector

    // Iterate through the vector and compute the sum
    for (int i = 0; i < n; i++) {
        sum += vec[i];
    }
    return sum;
}

// Function to compute the sum using external library (e.g. std::accumulate)
double sum_accumulate(std::vector<long double>& vec) {
    long double initial_value = 0.0; // Initial value for the accumulation
    return std::accumulate(vec.begin(), vec.end(), initial_value);
}

// Function to compute the sum using Kahan summation algorithm
double sum_kahan(std::vector<long double>& vec) {
    long double sum = 0.0;   // Initialize the sum
    long double c = 0.0;     // Initialize the compensation for lost low-order bits
    int n = vec.size(); // Get the size of the vector
    for (int i = 0; i < n; i++) {
        long double y = vec[i] - c;      // Compensate for lost low-order bits
        long double t = sum + y;         // Sum with compensation
        c = (t - sum) - y;          // (t - sum) recovers the high-order bits
        sum = t;                    // Update the sum
    }
    return sum;
}

int main() {
    // std::vector<double> vec = {0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1}; // Input vector with large and small values
    std::vector<long double> vec = {1.0, 1.0e16, -1.0e16, -0.5}; // Exercise input vector
    
    double sum_for = sum_for_loop(vec);                     // Compute sum using for loop
    double sum_acc = sum_accumulate(vec);                   // Compute sum using std::accumulate
    double sum_kahan_result = sum_kahan(vec);               // Compute sum using Kahan summation

    std::cout << std::fixed << std::setprecision(15);
    std::cout << "Sum using for loop: " << sum_for << "\n";
    std::cout << "Sum using std::accumulate: " << sum_acc << "\n";
    std::cout << "Sum using Kahan summation: " << sum_kahan_result << "\n";

    return 0;
}