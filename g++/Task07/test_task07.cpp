// test_task07.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp> // Catch2 header for unit testing
using Catch::Approx;            // For floating-point comparison
#include <fstream>              // Used for file operations
#include <cstdio>               // Used for file operations (remove)
#include <vector>               // Used for vectors operations
#include <string>               // Used for strings
#include <unordered_map>        // Used for key-value pairs in config
#include <sstream>              // Used for string stream operations

// Forward declarations (functions used later on)
std::unordered_map<std::string, std::string> read_config(const std::string&);
std::vector<long double> read_vector(const std::string&);
void write_vector(const std::string&, const std::vector<long double>&);
void calculate_d(const std::string&);

// Helpers (small utility functions to simplify testing)
void write_file(const std::string& filename, const std::vector<long double>& data) {
    std::ofstream f(filename);
    for (auto v : data) f << v << "\n";
}
void write_text_file(const std::string& filename, const std::string& content) {
    std::ofstream f(filename);
    f << content;
}
void remove_file(const std::string& filename) {
    std::remove(filename.c_str());
}
bool file_exists(const std::string& filename) {
    std::ifstream f(filename);
    return f.good();
}

// Testing unit

// Catch2 organizes test cases in sections using TEST_CASE
// Each TEST_CASE has a ("description", "[tags]") format

// Test reading config.txt file
TEST_CASE("read_config parses config file correctly", "[read_config]") {
    std::string fname = "test_config.txt";  // Temporary file for testing
    // Create a sample configuration file content
    std::string content = "filename_x=x.dat\nfilename_y=y.dat\nN=5\na=2.5\nprefix_output=out_";
    write_text_file(fname, content);

    // Read the configuration file
    auto config = read_config(fname);
    // Check that each key is correctly parsed
    REQUIRE(config["filename_x"] == "x.dat");
    REQUIRE(config["filename_y"] == "y.dat");
    REQUIRE(config["N"] == "5");
    REQUIRE(config["a"] == "2.5");
    REQUIRE(config["prefix_output"] == "out_");

    remove_file(fname);
}

// Test reading and writing a vector from a file
TEST_CASE("read_vector and write_vector work as inverse", "[vector_io]") {
    std::string fname = "test_vec.dat";
    std::vector<long double> data = {1.1, 2.2, 3.3, 4.4};
    write_vector(fname, data);

    auto read = read_vector(fname);
    REQUIRE(read == data);

    remove_file(fname);
}

// Test regarding the calculation of vector d
TEST_CASE("calculate_d computes d = a*x + y and writes output", "[calculate_d]") {
    // Prepare input files
    std::vector<long double> x = {1, 2, 3};
    std::vector<long double> y = {10, 20, 30};
    std::string xfile = "test_x.dat";
    std::string yfile = "test_y.dat";
    write_file(xfile, x);
    write_file(yfile, y);

    // Prepare config
    int N = 3;
    long double a = 2.0;
    std::string prefix = "testout_";
    std::string configfile = "test_config2.txt";
    std::ostringstream oss;
    oss << "filename_x=" << xfile << "\n";
    oss << "filename_y=" << yfile << "\n";
    oss << "N=" << N << "\n";
    oss << "a=" << a << "\n";
    oss << "prefix_output=" << prefix << "\n";
    write_text_file(configfile, oss.str());

    // Run
    calculate_d(configfile);

    // Check output
    std::string dfile = prefix + "N" + std::to_string(N) + "_d.dat";
    REQUIRE(file_exists(dfile));
    auto d = read_vector(dfile);
    REQUIRE(d.size() == 3);
    // Check the values of d using Approx for floating-point comparison
    REQUIRE(d[0] == Approx(a * x[0] + y[0]));
    REQUIRE(d[1] == Approx(a * x[1] + y[1]));
    REQUIRE(d[2] == Approx(a * x[2] + y[2]));

    // Cleanup
    remove_file(xfile);
    remove_file(yfile);
    remove_file(configfile);
    remove_file(dfile);
}