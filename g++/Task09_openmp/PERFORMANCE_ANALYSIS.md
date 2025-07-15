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
