# Performance Analysis: MPI vs Serial

## Benchmark Results Summary

### Vector Generation Performance

| Dataset Size | Serial Time (ms) | MPI-1P (ms) | MPI-2P (ms) | MPI-4P (ms) | MPI-8P (ms) | Best Speedup |
|--------------|------------------|-------------|-------------|-------------|-------------|--------------|
| 10,000       | 1                | 1           | 0           | 0           | 0           | ∞ (limited)  |
| 100,000      | 14               | 15          | 9           | 6           | 4           | 3.5x         |
| 1,000,000    | 123              | 136         | 65          | 36          | 24          | 5.1x         |

### Total Operation Time (Generation + File I/O)

| Dataset Size | Serial Time (ms) | MPI-1P (ms) | MPI-2P (ms) | MPI-4P (ms) | MPI-8P (ms) | Best Speedup |
|--------------|------------------|-------------|-------------|-------------|-------------|--------------|
| 10,000       | 9                | 8           | 8           | 7           | 7           | 1.3x         |
| 100,000      | 83               | 85          | 78          | 75          | 74          | 1.1x         |
| 1,000,000    | 817              | 826         | 497         | 462         | 494         | 1.8x         |

### Vector Processing Performance (d = a*x + y)

| Dataset Size | Serial Time (ms) | MPI-1P (ms) | MPI-2P (ms) | MPI-4P (ms) | MPI-8P (ms) | Best Speedup |
|--------------|------------------|-------------|-------------|-------------|-------------|--------------|
| 10,000       | 0                | 0           | 0           | 0           | 0           | -            |
| 100,000      | 2                | 4           | 3           | 2           | 2           | 1.0x         |
| 1,000,000    | 20               | 32          | 18          | 15          | 14          | 1.4x         |
