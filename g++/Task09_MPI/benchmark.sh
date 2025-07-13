#!/bin/bash

# Benchmark script to compare MPI vs Serial performance
# Usage: ./benchmark.sh

echo "=================================================="
echo "MPI vs Serial Performance Comparison"
echo "=================================================="
echo ""

# Test different vector sizes and process counts
SIZES=(10000 100000 1000000)
PROCESSES=(8)
CHUNKSIZE=1000

echo "System Information:"
echo "CPU cores: $(nproc)"
echo "MPI implementation: $(/data0/biasissi/Scientific_Computing/my_libraries/openmpi/bin/mpirun --version 2>/dev/null | head -1 || echo "MPI not found")"
echo ""

for SIZE in "${SIZES[@]}"; do
    echo "=================================================="
    echo "Testing with vector size: $SIZE"
    echo "=================================================="
    
    # Update config file for current size
    cat > config_benchmark.txt << EOF
filename_x=./vector_N${SIZE}_x.dat
filename_y=./vector_N${SIZE}_y.dat
N=$SIZE
a=1.0
prefix_output=./vector_
EOF
    
    echo ""
    echo "1. Generating vectors with SERIAL version..."
    echo "----------------------------------------"
    ./task09_xygenerator_serial $SIZE ./vector_
    
    echo ""
    echo "2. Processing vectors with SERIAL version..."
    echo "----------------------------------------"
    ./task09_serial config_benchmark.txt $CHUNKSIZE
    
    for NP in "${PROCESSES[@]}"; do
        echo ""
        echo "3. Generating vectors with MPI ($NP processes)..."
        echo "----------------------------------------"
        /data0/biasissi/Scientific_Computing/my_libraries/openmpi/bin/mpirun --map-by :OVERSUBSCRIBE -np $NP ./task09_xygenerator $SIZE ./vector_
        
        echo ""
        echo "4. Processing vectors with MPI ($NP processes)..."
        echo "----------------------------------------"
        /data0/biasissi/Scientific_Computing/my_libraries/openmpi/bin/mpirun --map-by :OVERSUBSCRIBE -np $NP ./task09 config_benchmark.txt $CHUNKSIZE
    done
    
    echo ""
    echo "=================================================="
    echo ""
done
