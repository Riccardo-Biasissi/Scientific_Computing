#include <iostream> // Used for input/output
#include <fstream>  // Used for file operations
#include <vector>   // Used for vectors operations
#include <string>   // Used for strings
#include <random>   // Used for random number generation
#include <mpi.h>    // Used for MPI parallelization
#include <chrono>   // Used for timing measurements

// Define a function that generates two vectors x and y of size N
// and saves them to files with the given filename_prefix using MPI
void generate_vectors(int N, std::string filename_prefix) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // Get process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // Get total number of processes
    
    if (rank == 0) {
        std::cout << "Generating vectors of size " << N << " using " << size << " MPI processes" << std::endl;
    }
    
    // Synchronize all processes before starting timing
    MPI_Barrier(MPI_COMM_WORLD);            // Ensure all processes start timing at the same time
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Calculate how many elements each process will generate
    int elements_per_process = N / size;
    int remaining_elements = N % size;
    
    // Calculate start and end indices for this process
    int start_idx = rank * elements_per_process + std::min(rank, remaining_elements);
    int end_idx = start_idx + elements_per_process + (rank < remaining_elements ? 1 : 0);
    int local_count = end_idx - start_idx;
    
    // Generate local portion of vectors x and y with Gaussian distribution
    std::random_device rd;
    std::mt19937 gen(rd() + rank); // Seed with rank to ensure different sequences per process
    std::normal_distribution<long double> Gauss_dist(0.0, 1.0);
    
    std::vector<long double> x_local(local_count), y_local(local_count);
    
    for (int i = 0; i < local_count; ++i) {
        x_local[i] = Gauss_dist(gen);
        y_local[i] = Gauss_dist(gen);
    }
    
    // Gather all local vectors to rank 0
    std::vector<long double> x_vec, y_vec;
    std::vector<int> recvcounts(size), displs(size);
    
    // Calculate receive counts and displacements for MPI_Gatherv
    for (int i = 0; i < size; ++i) {
        int proc_start = i * elements_per_process + std::min(i, remaining_elements);
        int proc_end = proc_start + elements_per_process + (i < remaining_elements ? 1 : 0);
        recvcounts[i] = proc_end - proc_start;
        displs[i] = (i == 0) ? 0 : displs[i-1] + recvcounts[i-1];
    }
    
    // Resize vectors on rank 0 to hold all elements
    if (rank == 0) {
        x_vec.resize(N);
        y_vec.resize(N);
    }
    
    // Gather x and y vectors to rank 0 that menages the reconstruction of the full vectors
    MPI_Gatherv(x_local.data(), local_count, MPI_LONG_DOUBLE,
                x_vec.data(), recvcounts.data(), displs.data(), MPI_LONG_DOUBLE,
                0, MPI_COMM_WORLD);
    
    MPI_Gatherv(y_local.data(), local_count, MPI_LONG_DOUBLE,
                y_vec.data(), recvcounts.data(), displs.data(), MPI_LONG_DOUBLE,
                0, MPI_COMM_WORLD);
    
    // End timing for vector generation
    MPI_Barrier(MPI_COMM_WORLD);            // Ensure all processes finish before measuring time
    auto end_gen_time = std::chrono::high_resolution_clock::now();
    
    if (rank == 0) {
        auto gen_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_gen_time - start_time);
        std::cout << "Vector generation time: " << gen_duration.count() << " ms" << std::endl;
        
        // Create filenames using the filename_prefix and N
        std::string filename_x = filename_prefix + "N" + std::to_string(N) + "_x.dat";
        std::string filename_y = filename_prefix + "N" + std::to_string(N) + "_y.dat";
        
        // Save vectors to files
        std::ofstream file_x(filename_x);
        std::ofstream file_y(filename_y);

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
}

// Define the main function that takes the number of elements N
// and the filename prefix as arguments
int main(int argc, char* argv[]) {
    // Initialize MPI
    MPI_Init(&argc, &argv);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Check if the number of arguments is correct. Remember
    // that the first argument is the name of the program,
    // so we need 3 arguments in total (program, N, filename_prefix)
    if (argc != 3) {
        if (rank == 0) {
            std::cerr << "Usage: " << argv[0] << " <N> <filename_prefix>" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    // Parse the arguments
    int N = std::stoi(argv[1]);             // Convert the first argument to an integer
    std::string filename_prefix = argv[2];  // Get the second argument as a string

    generate_vectors(N, filename_prefix);   // Call the function to generate the vectors

    // Finalize MPI
    MPI_Finalize();
    return 0;
}
