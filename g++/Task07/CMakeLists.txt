# CMakeLists.txt for Task07

# Ensure CMake is at least version 3.5
cmake_minimum_required(VERSION 3.5)
# Set the project name and specify the languages used
project(Task07 LANGUAGES CXX)

# Find the required package
# set(CMAKE_PREFIX_PATH "/data0/biasissi/Scientific_Computing/my_libraries/catch")
find_package(Catch2)

# Create an executable for the xygenerator test
add_executable(test_task07_xygenerator test_task07_xygenerator.cpp task07_xygenerator.cpp)
# Create an executable for the daxpy test
add_executable(test_task07 test_task07.cpp task07.cpp)

# Link the Catch2 library to the executables
target_link_libraries(test_task07_xygenerator Catch2::Catch2WithMain)
target_link_libraries(test_task07 Catch2::Catch2WithMain)