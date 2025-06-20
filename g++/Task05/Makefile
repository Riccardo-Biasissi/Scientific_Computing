# Compiler
CXX = g++

# Targets to be compiled
TARGETS = task05_a task05_b_xygenerator task05_b

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