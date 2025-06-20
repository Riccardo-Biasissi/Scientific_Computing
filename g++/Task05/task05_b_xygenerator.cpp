#include <iostream> // Used for input/output
#include <fstream>  // Used for file operations
#include <vector>   // Used for vectors operations
#include <string>   // Used for strings
#include <random>   // Used for random number generation

// Define a function that generates two vectors x and y of size N
// and saves them to files with the given filename_prefix
void generate_vectors(int N, std::string filename_prefix) {
    // Generate vectors x and y as long doubles of size N with Gaussian distribution (mean=0.0, stddev=1.0)
    std::random_device rd;                                  // Generate a random number using an hardware entropy generator (slow but real random numbers since it uses physical sources)
    std::mt19937 gen(rd());                                 // Initialize a random seed generator with the random number to generate pseudo-random numbers
    std::normal_distribution<long double> Gauss_dist(0.0, 1.0);  // Normal distribution for x using <random> library with mean=0.0 and standard deviation=1.0

    std::vector<long double> x_vec(N), y_vec(N);
    for (int i = 0; i < N; ++i) {
        x_vec[i] = Gauss_dist(gen);                         // Fill vector x with random numbers from the normal distribution
        y_vec[i] = Gauss_dist(gen);                         // Fill vector y with random numbers from the normal distribution
    }
    
    // Create filenames using the filename_prefix and N
    std::string filename_x = filename_prefix + "N" + std::to_string(N) + "_x.dat";
    std::string filename_y = filename_prefix + "N" + std::to_string(N) + "_y.dat";
    
    // Save vectors to files
    std::ofstream file_x(filename_x); // Open file for writing 
    std::ofstream file_y(filename_y); // in them

    // Write each value of the vectors to the files
    for (long double value : x_vec) {
        file_x << value << "\n";
    }
    
    for (long double value : y_vec) {
        file_y << value << "\n";
    }
    
    // Close the files
    file_x.close();
    file_y.close();
}

// Define the main function that takes the number of elements N
// and the filename prefix as arguments
int main(int argc, char* argv[]) {
    // Check if the number of arguments is correct. Remember
    // that the first argument is the name of the program,
    // so we need 3 arguments in total (program, N, filename_prefix)
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <N> <filename_prefix>" << std::endl;
        return 1;
    }

    // Parse the arguments
    int N = std::stoi(argv[1]);             // Convert the first argument to an integer
    std::string filename_prefix = argv[2];  // Get the second argument as a string

    generate_vectors(N, filename_prefix);   // Call the function to generate the vectors

    return 0;
}

//*-------------------- Example of usage --------------------*//

// To use this code you can run the following commands in the terminal
// g++ task03_1.cpp -o task03
// ./task03 10 vector_
//
// This will generate two files vector_N10_x.dat and vector_N10_y.dat
// with the vectors x and y filled with 0.1 and 7.1 respectively