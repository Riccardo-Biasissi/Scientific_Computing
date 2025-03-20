# Compiler
CXX = g++								# g++ for C++ programs
CXXFLAGS = -Wall -Wextra -O2 -std=c++17 # -O2 for optimization, -std=c++17 for C++17 standard, -Wall -Wextra for warnings

# Targets to be compiled
TARGETS = task03_1 task03_2

# Default rule (compile all targets)
all: $(TARGETS)

# Pattern rule to compile each target
# $@: target name (what is on left side of ":")
# $<: first dependency (what is on right side of ":")
%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# Clean rule (delete all compiled targets)
clean:
	rm -f $(TARGETS)
