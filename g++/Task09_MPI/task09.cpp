#include <iostream>         // Used for input/output
#include <fstream>          // Used for file operations
#include <vector>           // Used for vectors operations
#include <string>           // Used for strings
#include <sstream>          // Used for string stream
#include <unordered_map>    // Used for dictionary operations
#include <mpi.h>            // Used for MPI parallelization
#include <chrono>           // Used for timing measurements
#include <cmath>            // Used for abs function

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

// Function to calculate the vector d = a * x + y using MPI
void calculate_d(const std::string& config_file, int chunksize = 16) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get process rank
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Get total number of processes

    // Read configuration file (only on rank 0)
    std::unordered_map<std::string, std::string> config;
    std::string filename_x, filename_y, prefix_output;
    int N;
    long double a;
    
    if (rank == 0) {
        config = read_config(config_file);
        filename_x = config["filename_x"];
        filename_y = config["filename_y"];
        N = std::stoi(config["N"]);
        a = std::stod(config["a"]);
        prefix_output = config["prefix_output"];
    }

    // Broadcast configuration parameters to all processes
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&a, 1, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);

    // Read vectors x and y (only on rank 0)
    std::vector<long double> x, y;
    if (rank == 0) {
        x = read_vector(filename_x);
        y = read_vector(filename_y);
    } else {
        x.resize(N);
        y.resize(N);
    }

    // Broadcast vectors to all processes
    MPI_Bcast(x.data(), N, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(y.data(), N, MPI_LONG_DOUBLE, 0, MPI_COMM_WORLD);

    int chunk_number = compute_chunk_number(N, chunksize);
    
    if (rank == 0) {
        std::cout << "Number of chunks: " << chunk_number << std::endl;
        std::cout << "MPI processes: " << size << std::endl;
    }

    // Distribute chunks among MPI processes
    int chunks_per_process = chunk_number / size;
    int remaining_chunks = chunk_number % size;
    
    // Calculate which chunks this process will handle
    int start_chunk = rank * chunks_per_process + std::min(rank, remaining_chunks);
    int end_chunk = start_chunk + chunks_per_process + (rank < remaining_chunks ? 1 : 0);
    
    // Initialize local variables
    std::vector<long double> d_local;
    long double local_partial_sum = 0.0;

    // Start timing the main computation (synchronized across all processes)
    MPI_Barrier(MPI_COMM_WORLD);
    auto start_time = std::chrono::high_resolution_clock::now();

    // Process assigned chunks
    for (int chunk = start_chunk; chunk < end_chunk; ++chunk) {
        int start = chunk * chunksize;
        int end = std::min(start + chunksize, N);
        
        for (int i = start; i < end; ++i) {
            long double d_value = a * x[i] + y[i];
            d_local.push_back(d_value);
            local_partial_sum += d_value;
        }
    }

    // Gather all partial results to rank 0
    std::vector<long double> d(N);
    std::vector<int> recvcounts(size), displs(size);
    
    // Calculate receive counts and displacements
    for (int i = 0; i < size; ++i) {
        int proc_start_chunk = i * chunks_per_process + std::min(i, remaining_chunks);
        int proc_end_chunk = proc_start_chunk + chunks_per_process + (i < remaining_chunks ? 1 : 0);
        
        recvcounts[i] = 0;
        for (int chunk = proc_start_chunk; chunk < proc_end_chunk; ++chunk) {
            int chunk_start = chunk * chunksize;
            int chunk_end = std::min(chunk_start + chunksize, N);
            recvcounts[i] += chunk_end - chunk_start;
        }
        
        if (i == 0) {
            displs[i] = 0;
        } else {
            displs[i] = displs[i-1] + recvcounts[i-1];
        }
    }

    // Gather partial results
    MPI_Gatherv(d_local.data(), d_local.size(), MPI_LONG_DOUBLE,
                d.data(), recvcounts.data(), displs.data(), MPI_LONG_DOUBLE,
                0, MPI_COMM_WORLD);

    // Sum up partial chunk sums across all processes
    long double d_partial_chunk_sum = 0.0;
    MPI_Reduce(&local_partial_sum, &d_partial_chunk_sum, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Compute complete sum using MPI reduction
    long double local_complete_sum = 0.0;
    for (const auto& value : d_local) {
        local_complete_sum += value;
    }
    
    long double d_complete_sum = 0.0;
    MPI_Reduce(&local_complete_sum, &d_complete_sum, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // End timing and report performance (only on rank 0)
    MPI_Barrier(MPI_COMM_WORLD);
    auto end_time = std::chrono::high_resolution_clock::now();
    
    if (rank == 0) {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
        std::cout << "Computation time: " << duration.count() << " ms" << std::endl;

        // Check that d_partial_chunk_sum and d_complete_sum are equal within floating point tolerance
        const long double epsilon = 1e-10;
        if (std::abs(d_partial_chunk_sum - d_complete_sum) < epsilon) {
            std::cout << "The partial chunk sum and the complete sum are equal (within floating point tolerance)." << std::endl;
        } else {
            std::cout << "The partial chunk sum and the complete sum are NOT equal!" << std::endl;
            std::cout << "Partial chunk sum: " << d_partial_chunk_sum << std::endl;
            std::cout << "Complete sum: " << d_complete_sum << std::endl;
        }

        // Create output filename and save vector d to file
        std::string filename_d = prefix_output + "N" + std::to_string(N) + "_d.dat";
        write_vector(filename_d, d);
    }
}

int main(int argc, char* argv[]) {
    // Initialize MPI
    MPI_Init(&argc, &argv);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Check if the number of arguments is correct.
    // We need 2 or 3 arguments: program, config_file, [chunksize]
    if (argc != 3) {
        if (rank == 0) {
            std::cerr << "Usage: " << argv[0] << " <config_file> [chunksize]" << std::endl;
        }
        MPI_Finalize();
        return 1;
    }

    std::string config_file = argv[1];
    int chunksize = 16; // Default value

    if (argc == 3) {
        chunksize = std::stoi(argv[2]);
        if (chunksize <= 0) {
            if (rank == 0) {
                std::cerr << "Chunksize must be a positive integer." << std::endl;
            }
            MPI_Finalize();
            return 1;
        }
    }

    calculate_d(config_file, chunksize);

    // Finalize MPI
    MPI_Finalize();
    return 0;
}
