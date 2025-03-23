#include <iostream> // Used for input/output
#include <vector>   // Used for vectors operations
#include <string>   // Used for strings
#include <hdf5.h>   // Used for HDF5 operations

// Define a function that generates two vectors x and y of size N
// and saves them to separate HDF5 files with the given filename_prefix
void generate_vectors(int N, std::string filename_prefix) {
    // Generate vectors x and y as doubles of size N
    std::vector<double> x_data(N, 0.1);
    std::vector<double> y_data(N, 7.1);
    
    // Create filenames using the filename_prefix and N
    std::string filename_x = filename_prefix + "N" + std::to_string(N) + "_x.h5";
    std::string filename_y = filename_prefix + "N" + std::to_string(N) + "_y.h5";
    
    // Create a new HDF5 file for x
    hid_t file_x_id = H5Fcreate(filename_x.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    // Create dataspace for the dataset
    hsize_t dims[1] = { static_cast<hsize_t>(N) };
    hid_t dataspace_id = H5Screate_simple(1, dims, NULL);
    // Create the dataset for x
    hid_t dataset_x_id = H5Dcreate2(file_x_id, "x", H5T_NATIVE_DOUBLE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    // Write the data to the dataset
    H5Dwrite(dataset_x_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, x_data.data());
    // Close the dataset and the file
    H5Dclose(dataset_x_id);
    H5Sclose(dataspace_id);
    H5Fclose(file_x_id);
    
    // Create a new HDF5 file for y
    hid_t file_y_id = H5Fcreate(filename_y.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    // Create dataspace for the dataset
    dataspace_id = H5Screate_simple(1, dims, NULL);
    // Create the dataset for y
    hid_t dataset_y_id = H5Dcreate2(file_y_id, "y", H5T_NATIVE_DOUBLE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    // Write the data to the dataset
    H5Dwrite(dataset_y_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, y_data.data());
    // Close the dataset and the file
    H5Dclose(dataset_y_id);
    H5Sclose(dataspace_id);
    H5Fclose(file_y_id);
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
// make
// ./task03_1_hdf5_gsl 10 vector_
//
// This will generate two HDF5 files vector_N10_x.h5 and vector_N10_y.h5
// with the datasets x and y filled with 0.1 and 7.1 respectively
//
// To print the content of the generated files you can use the following commands:
// h5dump -d x vector_N10_x.h5
// h5dump -d y vector_N10_y.h5