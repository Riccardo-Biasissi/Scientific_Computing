#include <iostream> // Used for input/output
#include <vector>   // Used for vectors operations
#include <string>   // Used for strings
#include <hdf5.h>   // Used for HDF5 operations
#include <gsl/gsl_vector.h> // Used for GSL vector operations
#include <gsl/gsl_blas.h>   // Used for GSL BLAS operations

// Define a function that generates two vectors x and y of size N
// and saves them to HDF5 files with the given filename_prefix
void generate_vectors(int N, std::string filename_prefix) {
    // Generate vectors x and y as doubles of size N
    std::vector<double> x_data(N, 0.1);
    std::vector<double> y_data(N, 7.1);
    
    // Create GSL vectors from the data
    gsl_vector* x = gsl_vector_alloc(N);
    gsl_vector* y = gsl_vector_alloc(N);
    for (int i = 0; i < N; ++i) {
        gsl_vector_set(x, i, x_data[i]);
        gsl_vector_set(y, i, y_data[i]);
    }
    
    // Perform the operation y = a * x + b * y
    double a = 1.0;
    double b = 1.0;
    gsl_vector_axpby(a, x, b, y);
    
    // Create filenames using the filename_prefix and N
    std::string filename = filename_prefix + "N" + std::to_string(N) + ".h5";
    
    // Create a new HDF5 file
    hid_t file_id = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    
    // Create dataspace for the datasets
    hsize_t dims[1] = { static_cast<hsize_t>(N) };
    hid_t dataspace_id = H5Screate_simple(1, dims, NULL);
    
    // Create the datasets
    hid_t dataset_x_id = H5Dcreate2(file_id, "x", H5T_NATIVE_DOUBLE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    hid_t dataset_y_id = H5Dcreate2(file_id, "y", H5T_NATIVE_DOUBLE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    
    // Write the data to the datasets
    H5Dwrite(dataset_x_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, x_data.data());
    H5Dwrite(dataset_y_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, y_data.data());
    
    // Close the datasets and the file
    H5Dclose(dataset_x_id);
    H5Dclose(dataset_y_id);
    H5Sclose(dataspace_id);
    H5Fclose(file_id);
    
    // Free the GSL vectors
    gsl_vector_free(x);
    gsl_vector_free(y);
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
// g++ task03_1.cpp -o task03 -lhdf5 -lgsl -lgslcblas
// ./task03 10 vector_
//
// This will generate an HDF5 file vector_N10.h5
// with the datasets x and y filled with 0.1 and 7.1 respectively