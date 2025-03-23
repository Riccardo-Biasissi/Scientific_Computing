# Compiler
CXX = g++

# for optimization, -std=c++17 for C++17 standard, -Wall -Wextra for warnings
# the -I flag is used to include the hdf5 header files (hdf5/include)
CXXFLAGS = -Wall -Wextra -std=c++17 -I/data0/biasissi/Scientific_Computing/my_libraries/hdf5/include -I/data0/biasissi/Scientific_Computing/my_libraries/gsl/include

# the -L flag is used to include the hdf5 library files (hdf5/lib)
# the -lhdf5 flag is used to link the hdf5 library as well as the gsl libraries	
LDFLAGS = -L/data0/biasissi/Scientific_Computing/my_libraries/hdf5/lib -L/data0/biasissi/Scientific_Computing/my_libraries/gsl/lib -lhdf5 -lgsl -lgslcblas

# Targets to be compiled
TARGETS = task03_1 task03_2 task03_1_hdf5_gsl task03_2_hdf5_gsl

# Default rule (compile all targets)
all: $(TARGETS)

# Pattern rule to compile each target
# $@: target name (aka the final task file or what is on left side of ":")
# $<: first dependency (aka the input .cpp file or what is on right side of ":")
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# Clean rule (delete all compiled targets)
clean:
	rm -f $(TARGETS)
