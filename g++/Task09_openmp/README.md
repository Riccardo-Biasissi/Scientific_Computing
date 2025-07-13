# Task09 - OpenMP Parallelization

This folder contains two C++ programs that have been enhanced with OpenMP parallelization for improved CPU performance.

## Programs

### 1. task09_xygenerator.cpp
Generates two vectors x and y of specified size with Gaussian distribution and saves them to files.

**OpenMP Features:**
- Parallelized vector generation loop using `#pragma omp parallel for`
- Thread-safe random number generation with separate generators per thread
- Performance timing to measure parallelization benefits

**Usage:**
```bash
./task09_xygenerator <N> <filename_prefix>
```

### 2. task09.cpp
Computes vector d = a * x + y using chunked processing with parallel computation.

**OpenMP Features:**
- Parallelized main computation loop across chunks
- Parallel reduction for sum computation
- Performance timing and thread count reporting

**Usage:**
```bash
./task09 <config_file> <chunksize>
```

## Compilation

The Makefile has been updated to include OpenMP support:

```bash
make clean && make all
```

The compilation uses:
- `-fopenmp` flag for OpenMP support
- `-O3` optimization
- `-std=c++11` C++ standard

## Configuration File Format

Create a config.txt file with the following format:
```
filename_x=./vector_N<size>_x.dat
filename_y=./vector_N<size>_y.dat
N=<size>
a=<scalar_value>
prefix_output=./vector_
```

## Performance Benefits

With OpenMP parallelization on a 24-core system:
- Vector generation shows significant speedup for large datasets
- Chunk-based computation utilizes multiple cores effectively
- Parallel reduction optimizes sum calculations

## Example Usage

Generate vectors:
```bash
./task09_xygenerator 1000000 ./vector_
```

Process vectors:
```bash
./task09 config.txt 1000
```

The programs will report:
- Number of OpenMP threads available
- Computation timing
- Verification of results

## Thread Control

You can control the number of OpenMP threads using:
```bash
export OMP_NUM_THREADS=8
./task09 config.txt 1000
```

## Performance Comparison

The Task09 folder includes both OpenMP and serial versions for performance comparison:

### Programs Available:
- `task09` and `task09_xygenerator` - OpenMP parallelized versions
- `task09_serial` and `task09_xygenerator_serial` - Serial versions for comparison

### Benchmark Results (24-core system):

**Vector Generation (1M elements):**
- Serial: 122 ms
- OpenMP: 13 ms  
- **Speedup: 9.4x**

**Vector Processing (1M elements):**
- Serial: 10 ms
- OpenMP: 6 ms
- **Speedup: 1.7x**

### Running Benchmarks:
```bash
# Automated benchmark comparing all versions
./benchmark.sh

# Manual comparison
./task09_xygenerator_serial 1000000 ./vector_
./task09_xygenerator 1000000 ./vector_

# Control thread count
OMP_NUM_THREADS=4 ./task09_xygenerator 1000000 ./vector_
```

### Key Findings:
- OpenMP provides significant speedup for large datasets (>100K elements)
- Vector generation shows the best parallelization benefits
- Optimal performance with dataset sizes ≥ 1M elements
- Thread scaling efficiency decreases with very high thread counts due to memory bandwidth
