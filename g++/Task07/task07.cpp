#include <iostream>         // Used for input/output
#include <fstream>          // Used for file operations
#include <vector>           // Used for vectors operations
#include <string>           // Used for strings
#include <sstream>          // Used for string stream
#include <unordered_map>    // Used for dictionary operations

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

// Function to calculate the vector d = a * x + y
void calculate_d(const std::string& config_file) {
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

    // Calculate vector d
    std::vector<long double> d(N);
    for (int i = 0; i < N; ++i) {
        d[i] = a * x[i] + y[i];
    }

    // Create output filename
    std::string filename_d = prefix_output + "N" + std::to_string(N) + "_d.dat";

    // Save vector d to file
    write_vector(filename_d, d);
}

// COMMENTED OUT FOR UNIT TESTING - main is provided by Catch2
/*
int main(int argc, char* argv[]) {
    // Check if the number of arguments is correct. Remember
    // that the first argument is the name of the program,
    // so we need 2 arguments in total (program, config_file)
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file>" << std::endl;
        return 1;
    }

    // Parse the argument
    std::string config_file = argv[1];
    calculate_d(config_file);

    return 0;
}
*/