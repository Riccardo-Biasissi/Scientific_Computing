# Compiler
CXX = g++

# Variables for library paths (adjust to yours)
# To look where you have the libraries, you can use the commands:
# "locate libhdf5.so" or "locate libgsl.so"
HDF5_INCLUDE ?= /data0/biasissi/Scientific_Computing/my_libraries/hdf5/include
HDF5_LIB ?= /data0/biasissi/Scientific_Computing/my_libraries/hdf5/lib
GSL_INCLUDE ?= /data0/biasissi/Scientific_Computing/my_libraries/gsl/include
GSL_LIB ?= /data0/biasissi/Scientific_Computing/my_libraries/gsl/lib

# -std=c++17 for C++17 standard, -Wall -Wextra for warnings
# the -I flag is used to include the hdf5 and gsl header files
CXXFLAGS = -Wall -Wextra -std=c++17 -I$(HDF5_INCLUDE) -I$(GSL_INCLUDE)

# the -L flag is used to include the hdf5 and gsl libraries files
# the -lhdf5 -lgsl -lgslcblas flags are used to link the hdf5 library as well as the gsl libraries	
LDFLAGS = -L$(HDF5_LIB) -L$(GSL_LIB) -lhdf5 -lgsl -lgslcblas

# Targets to be compiled
TARGETS = task03_1 task03_2 task03_1_hdf5_gsl task03_2_hdf5_gsl

# Default rule (compile all targets)
all: $(TARGETS)

# Pattern rule to compile each target
# $@: target name (aka the final task file or what is on left side of ":")
# $<: first dependency (aka the input .cpp file or what is on right side of ":")
%: %.cpp
	@echo "Compiling $< ..."
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# Clean rule (delete all compiled targets)
clean:
	@echo "Cleaning up ..."
	rm -f $(TARGETS)
