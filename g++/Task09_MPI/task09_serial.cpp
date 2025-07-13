#include <iostream>         // Used for input/output
#include <fstream>          // Used for file operations
#include <vector>           // Used for vectors operations
#include <string>           // Used for strings
#include <sstream>          // Used for string stream
#include <unordered_map>    // Used for dictionary operations
#include <chrono>           // Used for timing measurements

// Function to read a configuration file into a dictionary
std::unordered_map<std::string, std::string> read_config(const std::string& config_file) {
    std::unordered_map<std::string, std::string> config;    // Create an empty dictionary
    std::ifstream file(config_file);                        // Open the file for reading
    std::string line;                                       // Create a string to store each line of the file
    // Read each line of the file
    while (std::getline(file, line)) {
        std::istringstream iss(line);                       // Create an input string stream from the line to read key and value
        std::string key, value;                             // Create strings to store the key and value
        // Read the key and value separated by '=', note that the config.txt file is not supposed to have spaces!!!!
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            config[key] = value;
        }
    }
    return config;
}

// Function to read a vector from a file
std::vector<long double> read_vector(const std::string& filename) {
    std::vector<long double> vec;        // Create an empty vector
    std::ifstream file(filename);   // Open the file for reading
    long double value;                   // Create a double to store each value
    // Read each value from the file, push_back adds the value to the end of the vector "vec"
    while (file >> value) {
        vec.push_back(value);
    }
    return vec;
}

// Function to write a vector to a file
void write_vector(const std::string& filename, const std::vector<long double>& vec) {
    std::ofstream file(filename);       // Open the file for writing
    for (const long double& value : vec) {
        file << value << "\n";
    }
}

// Function to compute the chunk number given a chunksize
int compute_chunk_number(int N, int chunksize) {
    return (N + chunksize - 1) / chunksize; // This calculates the number of chunks needed to cover N elements
}

// Function to calculate the vector d = a * x + y (SERIAL VERSION)
void calculate_d(const std::string& config_file, int chunksize = 16) {
    // Read configuration file
    auto config = read_config(config_file);

    // Extract values from configuration
    std::string filename_x = config["filename_x"];
    std::string filename_y = config["filename_y"];
    int N = std::stoi(config["N"]);     // stoi converts a string to an integer
    long double a = std::stod(config["a"]);  // stod converts a string to a double
    std::string prefix_output = config["prefix_output"];

    // Read vectors x and y from files
    std::vector<long double> x = read_vector(filename_x);
    std::vector<long double> y = read_vector(filename_y);

    int chunk_number = compute_chunk_number(N, chunksize); // Calculate the number of chunks
    // Print the number of chunks (SERIAL VERSION - 1 thread)
    std::cout << "Number of chunks: " << chunk_number << std::endl;
    std::cout << "Running SERIAL version (1 thread)" << std::endl;

    // Define a partial_chunk_sum array to hold the sum of the last chunk
    std::vector<long double> partial_chunk_sum(chunk_number, 0.0);

    // Start timing the main computation
    auto start_time = std::chrono::high_resolution_clock::now();

    // Calculate vector d progressing in chunks (SERIAL VERSION)
    std::vector<long double> d(N);
    for (int chunk = 0; chunk < chunk_number; ++chunk) {
        // Calculate the start and end indices for the current chunk
        int start = chunk * chunksize;
        int end = std::min(start + chunksize, N); // Ensure we don't go out of bounds

        // Process each element in the current chunk
        for (int i = start; i < end; ++i) {
            d[i] = a * x[i] + y[i]; // Calculate d[i]
            // Add the value to the partial sum for this chunk
            partial_chunk_sum[chunk] += d[i];
        }
    }

    // Sum up the partial sums to get the total sum
    long double d_partial_chunk_sum = 0.0;
    for (int i = 0; i < chunk_number; ++i) {
        d_partial_chunk_sum += partial_chunk_sum[i];
    }

    // Compute the total sum of vector d (SERIAL VERSION)
    long double d_complete_sum = 0.0;
    for (int i = 0; i < N; ++i) {
        d_complete_sum += d[i];
    }

    // End timing and report performance
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "SERIAL computation time: " << duration.count() << " ms" << std::endl;

    // Check that d_partial_chunk_sum and d_complete_sum are equal within floating point tolerance
    const long double epsilon = 1e-10;
    if (std::abs(d_partial_chunk_sum - d_complete_sum) < epsilon) {
        std::cout << "The partial chunk sum and the complete sum are equal (within floating point tolerance)." << std::endl;
    } else {
        std::cout << "The partial chunk sum and the complete sum are NOT equal!" << std::endl;
        std::cout << "Partial chunk sum: " << d_partial_chunk_sum << std::endl;
        std::cout << "Complete sum: " << d_complete_sum << std::endl;
    }

    // Create output filename
    std::string filename_d = prefix_output + "N" + std::to_string(N) + "_d_serial.dat";

    // Save vector d to file
    write_vector(filename_d, d);
}

int main(int argc, char* argv[]) {
    // Check if the number of arguments is correct.
    // We need 2 or 3 arguments: program, config_file, [chunksize]
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <config_file> [chunksize]" << std::endl;
        return 1;
    }

    std::string config_file = argv[1];
    int chunksize = 16; // Default value

    if (argc == 3) {
        chunksize = std::stoi(argv[2]);
        if (chunksize <= 0) {
            std::cerr << "Chunksize must be a positive integer." << std::endl;
            return 1;
        }
    }

    calculate_d(config_file, chunksize);

    return 0;
}
