#include <iostream>         // Used for input/output
#include <fstream>          // Used for file operations
#include <vector>           // Used for vectors operations
#include <string>           // Used for strings
#include <sstream>          // Used for string stream
#include <unordered_map>    // Used for dictionary operations
#include <hdf5.h>           // Used for HDF5 operations
#include <gsl/gsl_vector.h> // Used for GSL vector operations
#include <gsl/gsl_blas.h>   // Used for GSL BLAS operations

// Function to read a configuration file into a dictionary by splitting each line by '='
std::unordered_map<std::string, std::string> read_config(std::string config_file) {
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

// Function to read a GSL vector from an HDF5 file
gsl_vector* read_gsl_vector_hdf5(const std::string& filename, const std::string& dataset_name, int N) {
    // Open the HDF5 file
    hid_t file_id = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);

    // Open the dataset of the HDF5 file
    hid_t dataset_id = H5Dopen2(file_id, dataset_name.c_str(), H5P_DEFAULT);

    // Get the dataspace and the number of elements in the dataset
    hid_t dataspace_id = H5Dget_space(dataset_id);
    hsize_t dims[1];
    H5Sget_simple_extent_dims(dataspace_id, dims, NULL);

    // Allocate GSL vector
    gsl_vector* vec = gsl_vector_alloc(N);

    // Read the data into the GSL vector
    H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, vec->data);

    // Close the dataset and the file
    H5Sclose(dataspace_id);
    H5Dclose(dataset_id);
    H5Fclose(file_id);

    return vec;
}

// Function to write a GSL vector to an HDF5 file
void write_gsl_vector_hdf5(const std::string& filename, const gsl_vector* vec) {
    // Create a new HDF5 file
    hid_t file_id = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    // Create dataspace for the dataset
    hsize_t dims[1] = { static_cast<hsize_t>(vec->size) };
    hid_t dataspace_id = H5Screate_simple(1, dims, NULL);
    // Create the dataset
    hid_t dataset_id = H5Dcreate2(file_id, "d", H5T_NATIVE_DOUBLE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    // Write the data to the dataset
    H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, vec->data);
    // Close the dataset and the file
    H5Dclose(dataset_id);
    H5Sclose(dataspace_id);
    H5Fclose(file_id);
}

// Function to calculate the vector d = a * x + b * y
void calculate_d(const std::string& config_file) {
    // Read configuration file
    auto config = read_config(config_file);

    // Extract values from configuration
    std::string filename_x = config["filename_x"];
    std::string filename_y = config["filename_y"];
    int N = std::stoi(config["N"]);     // stoi converts a string to an integer
    double a = std::stod(config["a"]);  // stod converts a string to a double
    double b = std::stod(config["b"]);  // stod converts a string to a double
    std::string prefix_output = config["prefix_output"];

    // Read GSL vectors x and y from HDF5 files
    gsl_vector* x = read_gsl_vector_hdf5(filename_x, "x", N);
    gsl_vector* y = read_gsl_vector_hdf5(filename_y, "y", N);

    // Perform the operation d = a * x + b * y using gsl_vector_axpby
    gsl_vector_axpby(a, x, b, y);

    // Create output filename
    std::string filename_d = prefix_output + "N" + std::to_string(N) + "_d.h5";

    // Save vector y (which now contains the result) to HDF5 file
    write_gsl_vector_hdf5(filename_d, y);

    // Free the GSL vectors
    gsl_vector_free(x);
    gsl_vector_free(y);
}

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

//*-------------------- Example of usage --------------------*//

// To use this code you can run the following commands in the terminal
// make
// ./task03_2_hdf5_gsl config_hdf5.txt
//
// Where the config_hdf5.txt file contains the following configuration:
// filename_x=/path/to/vector_x.h5
// filename_y=/path/to/vector_y.h5
// N=10
// a=3.0
// b=1.0
// prefix_output=/path/to/output/vector_
//
// This will generate an HDF5 file vector_N10_d.h5 with the dataset d obtained
// by the daxpby computation d = a * x + b * y.
//
// To print the content of the generated files you can use the following command:
// h5dump -d d vector_N10_d.h5