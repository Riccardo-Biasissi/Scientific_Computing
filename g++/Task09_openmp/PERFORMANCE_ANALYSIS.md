# Performance Analysis: OpenMP vs Serial

## Benchmark Results Summary

### Vector Generation Performance

| Dataset Size | Serial Time (ms) | OpenMP Time (ms) | Speedup |
|--------------|------------------|------------------|---------|
| 10,000       | 1                | 2                | 0.5x    |
| 100,000      | 14               | 3                | 4.7x    |
| 1,000,000    | 122              | 13               | 9.4x    |

### Total Operation Time (Generation + File I/O)

| Dataset Size | Serial Time (ms) | OpenMP Time (ms) | Speedup |
|--------------|------------------|------------------|---------|
| 10,000       | 8                | 10               | 0.8x    |
| 100,000      | 56               | 51               | 1.1x    |
| 1,000,000    | 798              | 448              | 1.8x    |

### Vector Processing Performance

| Dataset Size | Serial Time (ms) | OpenMP Time (ms) | Speedup |
|--------------|------------------|------------------|---------|
| 10,000       | 0                | 1                | -       |
| 100,000      | 2                | 1                | 2.0x    |
| 1,000,000    | 10               | 6                | 1.7x    |

## Analysis

### Key Observations:

1. **Scaling Effect**: OpenMP shows better performance benefits with larger datasets
   - Small datasets (10K): Overhead dominates, serial can be faster
   - Medium datasets (100K): Modest speedup (2-5x)
   - Large datasets (1M): Significant speedup (1.7-9.4x)

2. **Vector Generation**: Best parallelization candidate
   - Achieves up to 9.4x speedup on large datasets
   - CPU-intensive random number generation benefits from parallel execution

3. **Vector Processing**: Moderate improvements
   - 1.7x speedup on large datasets
   - Memory bandwidth and cache effects may limit scaling

4. **I/O Bottleneck**: File operations limit total speedup
   - Sequential file writing reduces overall performance gains
   - Could be improved with parallel I/O techniques
