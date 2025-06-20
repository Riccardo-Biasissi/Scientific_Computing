#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp> // Catch2 header for unit testing
#include <fstream>              // Used for file operations
#include <cstdio>               // Used for file operations (remove)
#include <string>               // Used for strings
#include <vector>               // Used for vectors operations
#include <sstream>              // Used for string stream operations

// Forward declarations (functions used later on)
void generate_vectors(int N, std::string filename_prefix);

// Helpers (small utility functions to simplify testing)
std::vector<std::string> read_lines(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

// Helper function to check if a file exists
bool file_exists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

// Helper function to remove a file
void remove_file(const std::string& filename) {
    std::remove(filename.c_str());
}

// Testing unit

// Catch2 organizes test cases in sections using TEST_CASE
// Each TEST_CASE has a ("description", "[tags]") format

// Test the generate_vectors function
TEST_CASE("generate_vectors creates files with correct number of lines and valid numbers", "[generate_vectors]") {
    int N = 10;
    std::string prefix = "testvec_";
    std::string filename_x = prefix + "N" + std::to_string(N) + "_x.dat";
    std::string filename_y = prefix + "N" + std::to_string(N) + "_y.dat";

    generate_vectors(N, prefix);

    REQUIRE(file_exists(filename_x));
    REQUIRE(file_exists(filename_y));

    auto x_lines = read_lines(filename_x);
    auto y_lines = read_lines(filename_y);

    REQUIRE(x_lines.size() == N);
    REQUIRE(y_lines.size() == N);

    for (const auto& line : x_lines) {
        std::istringstream iss(line);
        long double val;
        REQUIRE(iss >> val);
    }
    for (const auto& line : y_lines) {
        std::istringstream iss(line);
        long double val;
        REQUIRE(iss >> val);
    }

    remove_file(filename_x);
    remove_file(filename_y);
}

// Test the generate_vectors function with a different prefix
TEST_CASE("generate_vectors works with N=1", "[generate_vectors]") {
    int N = 1;
    std::string prefix = "testvec1_";
    std::string filename_x = prefix + "N" + std::to_string(N) + "_x.dat";
    std::string filename_y = prefix + "N" + std::to_string(N) + "_y.dat";

    generate_vectors(N, prefix);

    REQUIRE(file_exists(filename_x));
    REQUIRE(file_exists(filename_y));

    auto x_lines = read_lines(filename_x);
    auto y_lines = read_lines(filename_y);

    REQUIRE(x_lines.size() == 1);
    REQUIRE(y_lines.size() == 1);

    remove_file(filename_x);
    remove_file(filename_y);
}

// Test the generate_vectors function with N=0, should create empty files
TEST_CASE("generate_vectors with N=0 creates empty files", "[generate_vectors]") {
    int N = 0;
    std::string prefix = "testvec0_";
    std::string filename_x = prefix + "N" + std::to_string(N) + "_x.dat";
    std::string filename_y = prefix + "N" + std::to_string(N) + "_y.dat";

    generate_vectors(N, prefix);

    REQUIRE(file_exists(filename_x));
    REQUIRE(file_exists(filename_y));

    auto x_lines = read_lines(filename_x);
    auto y_lines = read_lines(filename_y);

    REQUIRE(x_lines.empty());
    REQUIRE(y_lines.empty());

    remove_file(filename_x);
    remove_file(filename_y);
}