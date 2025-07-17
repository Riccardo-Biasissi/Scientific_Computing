#!/bin/bash

# Benchmark script to compare OpenMP vs Serial performance
# Usage: ./benchmark.sh

echo "=================================================="
echo "OpenMP vs Serial Performance Comparison"
echo "=================================================="
echo ""

# Test different vector sizes
SIZES=(10000 100000 1000000)
CHUNKSIZE=10000

echo "System Information:"
echo "CPU cores: $(nproc)"
echo "OpenMP threads available: $(echo | cpp -fopenmp -dM | grep -i omp_thread | wc -l)"
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
    echo "2. Generating vectors with OpenMP version..."
    echo "----------------------------------------"
    ./task09_xygenerator $SIZE ./vector_
    
    echo ""
    echo "3. Processing vectors with SERIAL version..."
    echo "----------------------------------------"
    ./task09_serial config_benchmark.txt $CHUNKSIZE
    
    echo ""
    echo "4. Processing vectors with OpenMP version..."
    echo "----------------------------------------"
    ./task09 config_benchmark.txt $CHUNKSIZE
    
    echo ""
    echo "=================================================="
    echo ""
done
