// Import the required libraries, iostream for input/output, vector for the vectors and fstream for file handling
#include <iostream>
#include <vector>
#include <fstream>

// Define the main function
int main() {
    // Define the parameter a
    int a = 3;

    // Create x vector (N=20) of 1, integers
    std::vector<int> x(20, 1);

    // Create y vector (N=20) of 4, integers
    std::vector<int> y(20, 4);

    // Create z vector (N=20) of 0, integers. This will be used for the computation
    std::vector<int> z(20);

    // Compute the operation z = a * x + y
    for (size_t i = 0; i < x.size(); ++i) {
        z[i] = a * x[i] + y[i];
    }

    // Check the vectors just created printing them
    std::cout << "x: ";
    for (const auto& value : x) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    std::cout << "y: ";
    for (const auto& value : y) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    std::cout << "z: ";
    for (const auto& value : z) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    // Create a file output.txt and write the z vector in it
    std::ofstream outputFile("output.txt");
    for (const auto& value : z) {
        outputFile << value << std::endl;
    }
    // Close the file
    outputFile.close();

    // Return 0 to indicate the program has ended successfully
    return 0;
}