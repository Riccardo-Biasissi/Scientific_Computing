# CMakeLists.txt for Task07

# Ensure CMake is at least version 3.10
cmake_minimum_required(VERSION 3.10)
# Set the project name and specify the languages used
project(Task07 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Create an executable for the xygenerator test
add_executable(test_task07_xygenerator catch_amalgamated.cpp test_task07_xygenerator.cpp task07_xygenerator.cpp)
# Create an executable for the daxpy test  
add_executable(test_task07 catch_amalgamated.cpp test_task07.cpp task07.cpp)