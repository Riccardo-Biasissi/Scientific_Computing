// test_task07_xygenerator.cpp
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include <sstream>

// Forward declaration of the function to test
void generate_vectors(int N, std::string filename_prefix);

// Helper function to read all lines from a file into a vector
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