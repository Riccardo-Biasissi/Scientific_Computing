# Compiler
CXX = /data0/biasissi/Scientific_Computing/my_libraries/openmpi/bin/mpicxx

# Compiler flags for MPI
CXXFLAGS = -O3 -std=c++11

# Linker flags
LDFLAGS = 

# Targets to be compiled
TARGETS = task09 task09_xygenerator task09_serial task09_xygenerator_serial

# Default rule (compile all targets)
all: $(TARGETS)

# Pattern rule to compile MPI targets
# $@: target name (aka the final task file or what is on left side of ":")
# $<: first dependency (aka the input .cpp file or what is on right side of ":")
task09: task09.cpp
	@echo "Compiling $< with MPI support..."
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

task09_xygenerator: task09_xygenerator.cpp
	@echo "Compiling $< with MPI support..."
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# Special rules for serial versions (without MPI)
task09_serial: task09_serial.cpp
	@echo "Compiling $< as SERIAL version (no MPI)..."
	g++ -O3 -std=c++11 -o $@ $< $(LDFLAGS)

task09_xygenerator_serial: task09_xygenerator_serial.cpp
	@echo "Compiling $< as SERIAL version (no MPI)..."
	g++ -O3 -std=c++11 -o $@ $< $(LDFLAGS)

# Clean rule (delete all compiled targets)
clean:
	@echo "Cleaning up ..."
	rm -f $(TARGETS)
