#include <iostream>         // Used for input/output
#include <fstream>          // Used for file operations
#include <vector>           // Used for vectors operations
#include <string>           // Used for strings
#include <sstream>          // Used for string stream
#include <unordered_map>    // Used for dictionary operations
#include <hdf5.h>           // Used for HDF5 operations
#include <gsl/gsl_vector.h> // Used for GSL vector operations
#include <gsl/gsl_blas.h>   // Used for GSL BLAS operations

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

// Function to read a vector from an HDF5 file
std::vector<double> read_vector_hdf5(const std::string& filename, const std::string& dataset_name) {
    // Open the HDF5 file
    hid_t file_id = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file_id < 0) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return {};
    }

    // Open the dataset
    hid_t dataset_id = H5Dopen2(file_id, dataset_name.c_str(), H5P_DEFAULT);
    if (dataset_id < 0) {
        std::cerr << "Error: Unable to open dataset '" << dataset_name << "' in file " << filename << std::endl;
        H5Fclose(file_id);
        return {};
    }

    // Get the dataspace and the number of elements in the dataset
    hid_t dataspace_id = H5Dget_space(dataset_id);
    hsize_t dims[1];
    H5Sget_simple_extent_dims(dataspace_id, dims, NULL);
    size_t num_elements = dims[0];

    // Read the data into a vector
    std::vector<double> vec(num_elements);
    H5Dread(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, vec.data());

    // Close the dataset and the file
    H5Sclose(dataspace_id);
    H5Dclose(dataset_id);
    H5Fclose(file_id);

    return vec;
}

// Function to write a vector to an HDF5 file
void write_vector_hdf5(const std::string& filename, const std::vector<double>& vec) {
    // Create a new HDF5 file
    hid_t file_id = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    // Create dataspace for the dataset
    hsize_t dims[1] = { static_cast<hsize_t>(vec.size()) };
    hid_t dataspace_id = H5Screate_simple(1, dims, NULL);
    // Create the dataset
    hid_t dataset_id = H5Dcreate2(file_id, "d", H5T_NATIVE_DOUBLE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    // Write the data to the dataset
    H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, vec.data());
    // Close the dataset and the file
    H5Dclose(dataset_id);
    H5Sclose(dataspace_id);
    H5Fclose(file_id);
}

// Function to calculate the vector d = a * x + y
void calculate_d(const std::string& config_file) {
    // Read configuration file
    auto config = read_config(config_file);

    // Extract values from configuration
    std::string filename_x = config["filename_x"];
    std::string filename_y = config["filename_y"];
    int N = std::stoi(config["N"]);     // stoi converts a string to an integer
    double a = std::stod(config["a"]);  // stod converts a string to a double
    std::string prefix_output = config["prefix_output"];

    // Read vectors x and y from HDF5 files
    std::vector<double> x_data = read_vector_hdf5(filename_x, "x");
    std::vector<double> y_data = read_vector_hdf5(filename_y, "y");

    // Check if the sizes of the vectors match the expected size N
    if (x_data.size() != static_cast<std::vector<double>::size_type>(N) || y_data.size() != static_cast<std::vector<double>::size_type>(N)) {
        std::cerr << "Error: Vector sizes do not match the expected size N." << std::endl;
        return;
    }

    // Create GSL vectors from the data
    gsl_vector* x = gsl_vector_alloc(N);
    gsl_vector* y = gsl_vector_alloc(N);
    gsl_vector* d = gsl_vector_alloc(N);
    for (int i = 0; i < N; ++i) {
        gsl_vector_set(x, i, x_data[i]);
        gsl_vector_set(y, i, y_data[i]);
    }

    // Perform the operation d = a * x + y
    gsl_vector_memcpy(d, y);
    gsl_blas_daxpy(a, x, d);

    // Convert GSL vector d to std::vector
    std::vector<double> d_data(N);
    for (int i = 0; i < N; ++i) {
        d_data[i] = gsl_vector_get(d, i);
    }

    // Create output filename
    std::string filename_d = prefix_output + "N" + std::to_string(N) + "_d.h5";

    // Save vector d to HDF5 file
    write_vector_hdf5(filename_d, d_data);

    // Free the GSL vectors
    gsl_vector_free(x);
    gsl_vector_free(y);
    gsl_vector_free(d);
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
// h5dump -d x vector_N10_d.h5