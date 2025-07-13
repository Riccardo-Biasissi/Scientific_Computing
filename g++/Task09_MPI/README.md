# Task09 MPI - Vector Operations with MPI Parallelization

This directory contains MPI implementations of the vector operations originally developed for OpenMP in Task09_openmp.

## Files

- `task09.cpp` - Main program that calculates d = a*x + y using MPI parallelization
- `task09_xygenerator.cpp` - Program to generate random vectors x and y using MPI parallelization
- `task09_serial.cpp` - Serial version for comparison (no MPI)
- `task09_xygenerator_serial.cpp` - Serial vector generator for comparison
- `Makefile` - Build system using mpic++ for MPI compilation
- `config.txt` - Configuration file with input parameters

## Key Differences from OpenMP Version

### Parallelization Strategy
- **OpenMP**: Shared memory parallelism using `#pragma omp parallel for`
- **MPI**: Distributed memory parallelism using process-based communication

### Data Distribution
- **OpenMP**: All threads share the same memory space
- **MPI**: Data is distributed across processes using MPI_Bcast, MPI_Gatherv, and MPI_Reduce

### Communication Patterns
- **MPI_Bcast**: Broadcast configuration and input vectors to all processes
- **MPI_Gatherv**: Gather partial results from all processes to rank 0
- **MPI_Reduce**: Reduce partial sums across all processes

### Chunk Distribution
- Chunks are distributed among MPI processes instead of OpenMP threads
- Each process handles a subset of chunks and performs local calculations
- Results are gathered back to rank 0 for final output

## Building

```bash
make all          # Build all targets
make task09       # Build MPI main program
make task09_xygenerator  # Build MPI vector generator
make clean        # Clean compiled files
```

## Running

### Generate vectors (MPI version):
```bash
mpirun -np 4 ./task09_xygenerator 1000000 ./vector_
```

### Run main calculation (MPI version):
```bash
mpirun -np 4 ./task09 config.txt 16
```

### Compare with serial versions:
```bash
./task09_xygenerator_serial 1000000 ./vector_
./task09_serial config.txt 16
```

## MPI Implementation Details

1. **Initialization**: All processes call MPI_Init()
2. **Configuration**: Rank 0 reads config, broadcasts to all processes
3. **Data Distribution**: Input vectors are broadcast to all processes
4. **Computation**: Each process handles assigned chunks
5. **Result Gathering**: Partial results collected using MPI_Gatherv
6. **Sum Reduction**: Partial sums reduced using MPI_Reduce
7. **Output**: Only rank 0 writes final results to file
8. **Cleanup**: All processes call MPI_Finalize()

## Performance Considerations

- **Communication Overhead**: MPI has higher communication costs than OpenMP
- **Memory Usage**: Each process stores full input vectors (could be optimized)
- **Load Balancing**: Chunks distributed evenly across processes
- **Scalability**: Can run on multiple nodes (unlike OpenMP)

## Expected Behavior

The MPI version should produce identical numerical results to the OpenMP and serial versions, but with different performance characteristics depending on the number of processes and system architecture.
