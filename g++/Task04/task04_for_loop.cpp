#include <iostream> // Used for input/output
#include <fstream>  // Used for file operations
#include <vector>   // Used for vector operations
#include <string>   // Used for strings
#include <cmath>    // Used for mathematical functions
#include <iomanip>  // Used for input/output manipulators like std::setprecision
#include <sstream>  // Used for parsing the config file
#include <gsl/gsl_integration.h> // GSL library for numerical integration

// Function to generate linearly spaced points between x_inf and x_sup
std::vector<double> generate_linspace(double x_inf, double x_sup, int N) {
    std::vector<double> points(N);
    double step = (x_sup - x_inf) / (N - 1);
    for (int i = 0; i < N; ++i) {
        points[i] = x_inf + i * step;
    }
    return points;
}

// Function to generate linearly spaced points between x_inf and x_sup
std::vector<int> generate_linspace_int(double x_inf, double x_sup, int N) {
    std::vector<int> points(N);
    double step = (x_sup - x_inf) / (N - 1);
    for (int i = 0; i < N; ++i) {
        points[i] = x_inf + i * step;
    }
    return points;
}

// Function to compute f(x) = e^x * cos(x) for a vector of x values
std::vector<double> compute_function(const std::vector<double>& x_values) {
    std::vector<double> f_values(x_values.size());
    for (size_t i = 0; i < x_values.size(); ++i) {
        f_values[i] = std::exp(x_values[i]) * std::cos(x_values[i]);
    }
    return f_values;
}

// Function to save a vector of x values and their corresponding f(x) values to a file
void save_to_file(const std::vector<double>& x_values, const std::vector<double>& f_values) {
    std::string filename = "data.dat";
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }

    file << std::fixed << std::setprecision(6);
    for (size_t i = 0; i < x_values.size(); ++i) {
        file << x_values[i] << "\t" << f_values[i] << "\n";
    }

    file.close();
}

// Function to compute the integral of f(x) = e^x * cos(x) using the trapezoidal rule
double compute_integral_trapezoidal(double x_inf, double x_sup, int N) {
    // Generate linearly spaced points
    std::vector<double> x_values = generate_linspace(x_inf, x_sup, N);
    std::vector<double> f_values = compute_function(x_values);

    // Apply the trapezoidal rule
    double integral = 0.0;
    for (int i = 0; i < N - 1; ++i) {
        double dx = x_values[i + 1] - x_values[i];
        integral += 0.5 * dx * (f_values[i] + f_values[i + 1]);
    }

    return integral;
}

// Function to compute the integral of f(x) = e^x * cos(x) using Simpson's rule
double compute_integral_simpson(double x_inf, double x_sup, int N) {
    if (N % 2 != 0) {
        std::cerr << "Error: N must be even for Simpson's rule." << std::endl;
        return 0.0;
    }

    double step = (x_sup - x_inf) / N;
    double integral = 0.0;

    for (int i = 0; i <= N; ++i) {
        double x = x_inf + i * step;
        double weight = (i == 0 || i == N) ? 1 : (i % 2 == 0 ? 2 : 4);
        integral += weight * std::exp(x) * std::cos(x);
    }

    integral *= step / 3.0;
    return integral;
}

// Function to compute the integral of f(x) = e^x * cos(x) using GSL
double compute_integral_gaussian(double x_inf, double x_sup) {
    gsl_integration_workspace* workspace = gsl_integration_workspace_alloc(1000);

    double result, error;

    gsl_function F;
    F.function = [](double x, void* params) -> double {
        return std::exp(x) * std::cos(x);
    };
    F.params = nullptr;

    gsl_integration_qags(&F, x_inf, x_sup, 0, 1e-7, 1000, workspace, &result, &error);

    gsl_integration_workspace_free(workspace);

    return result;
}

// Function to read configuration from a file
bool read_config(const std::string& filename, int& N, double& x_inf, double& x_sup) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open config file " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string value;
            if (std::getline(iss, value)) {
                if (key == "N") {
                    N = std::stoi(value);
                } else if (key == "x_inf") {
                    x_inf = std::stod(value);
                } else if (key == "x_sup") {
                    x_sup = std::stod(value);
                }
            }
        }
    }

    file.close();
    return true;
}

// Function to read configuration from a file
bool read_config(const std::string& filename, double& x_inf, double& x_sup) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open config file " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string value;
            if (std::getline(iss, value)) {
                if (key == "x_inf") {
                    x_inf = std::stod(value);
                } else if (key == "x_sup") {
                    x_sup = std::stod(value);
                }
            }
        }
    }

    file.close();
    return true;
}

int main() {
    double x_inf, x_sup;

    // Read configuration from config.txt
    if (!read_config("config.txt", x_inf, x_sup)) {
        return 1;
    }

    // Open the output file
    std::ofstream output_file("integral_value.txt");
    if (!output_file) {
        std::cerr << "Error: Could not open file integral_value.txt for writing." << std::endl;
        return 1;
    }

    // Write the header to the file
    output_file << "N\tTrapezoidal\tSimpson\n";

    // Set precision for output
    output_file << std::fixed << std::setprecision(30);

    // List of N values
    std::vector<int> N_values = generate_linspace_int(10,10000010,1001);

    for (int N : N_values) {
        if (N < 2) {
            std::cerr << "Error: N must be at least 2 to generate linearly spaced points." << std::endl;
            continue;
        }

        // Compute the integral using the trapezoidal rule
        double integral_trapezoidal = compute_integral_trapezoidal(x_inf, x_sup, N);

        // Compute the integral using Simpson's rule
        double integral_simpson = 0.0;
        if (N % 2 == 0) { // Ensure N is even for Simpson's rule
            integral_simpson = compute_integral_simpson(x_inf, x_sup, N);
        } else {
            std::cerr << "Warning: Skipping Simpson's rule for N = " << N << " (N must be even)." << std::endl;
        }

        // Write the results to the file
        output_file << N << "\t" << integral_trapezoidal << "\t" << integral_simpson << "\n";
    }

    // Close the output file
    output_file.close();

    std::cout << "Results saved to integral_value.txt" << std::endl;

    return 0;
}