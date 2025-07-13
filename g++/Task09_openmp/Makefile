# Compiler
CXX = g++

# Compiler flags for OpenMP
# -fopenmp enables OpenMP support
# -O3 enables optimizations
# -std=c++11 specifies the C++ standard to use
CXXFLAGS = -fopenmp -O3 -std=c++11

# Targets to be compiled
TARGETS = task09 task09_xygenerator task09_serial task09_xygenerator_serial

# Default rule (compile all targets)
all: $(TARGETS)

# Pattern rule to compile each target
# $@: target name (aka the final task file or what is on left side of ":")
# $<: first dependency (aka the input .cpp file or what is on right side of ":")
%: %.cpp
	@echo "Compiling $< with OpenMP support..."
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# Special rules for serial versions (without OpenMP)
task09_serial: task09_serial.cpp
	@echo "Compiling $< as SERIAL version (no OpenMP)..."
	$(CXX) -O3 -std=c++11 -o $@ $< $(LDFLAGS)

task09_xygenerator_serial: task09_xygenerator_serial.cpp
	@echo "Compiling $< as SERIAL version (no OpenMP)..."
	$(CXX) -O3 -std=c++11 -o $@ $< $(LDFLAGS)

# Clean rule (delete all compiled targets)
clean:
	@echo "Cleaning up ..."
	rm -f $(TARGETS)