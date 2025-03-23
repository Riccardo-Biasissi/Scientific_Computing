# Compiler
CXX = g++

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