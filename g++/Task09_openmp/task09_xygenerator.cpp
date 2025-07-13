#include <iostream> // Used for input/output
#include <fstream>  // Used for file operations
#include <vector>   // Used for vectors operations
#include <string>   // Used for strings
#include <random>   // Used for random number generation
#include <omp.h>    // Used for OpenMP parallelization
#include <chrono>   // Used for timing measurements

// Define a function that generates two vectors x and y of size N
// and saves them to files with the given filename_prefix
void generate_vectors(int N, std::string filename_prefix) {
    std::cout << "Generating vectors of size " << N << " using " << omp_get_max_threads() << " OpenMP threads" << std::endl;
    
    // Start timing the vector generation
    auto start_time = std::chrono::high_resolution_clock::now();
    // Generate vectors x and y as long doubles of size N with Gaussian distribution (mean=0.0, stddev=1.0)
    std::random_device rd;                                  // Generate a random number using an hardware entropy generator (slow but real random numbers since it uses physical sources)
    std::mt19937 gen(rd());                                 // Initialize a random seed generator with the random number to generate pseudo-random numbers
    std::normal_distribution<long double> Gauss_dist(0.0, 1.0);  // Normal distribution for x using <random> library with mean=0.0 and standard deviation=1.0

    std::vector<long double> x_vec(N), y_vec(N);
    
    // Use OpenMP parallelization to fill the vectors x and y
    #pragma omp parallel
    {
        // Each thread gets its own random generator seeded with thread ID
        std::mt19937 local_gen(rd() + omp_get_thread_num());
        std::normal_distribution<long double> local_dist(0.0, 1.0);
        
        #pragma omp for
        for (int i = 0; i < N; ++i) {
            x_vec[i] = local_dist(local_gen);               // Fill vector x with random numbers from the normal distribution
            y_vec[i] = local_dist(local_gen);               // Fill vector y with random numbers from the normal distribution
        }
    }
    
    // End timing for vector generation
    auto end_gen_time = std::chrono::high_resolution_clock::now();
    auto gen_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_gen_time - start_time);
    std::cout << "Vector generation time: " << gen_duration.count() << " ms" << std::endl;
    
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
    
    // End timing for entire operation
    auto end_time = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Total operation time: " << total_duration.count() << " ms" << std::endl;
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