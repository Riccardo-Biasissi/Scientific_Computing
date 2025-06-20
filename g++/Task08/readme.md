# How to run this code

To run this code, simply enter these commands in terminal:
```bash
make
./task08_xygenerator 100 vector_
./task08 config.txt 8
```

The file `task08_xygenerator` will produce the x and y arrays with Gaussian distribution. The file `task08` will perform `daxpy` computation (`a=1.0`) using the desired `chunksize` of 8. The output of `task08` code will give:
- Total number of chunks
- Partial sums of each chunk
- Check that the partial chunk sum and complete sum are equal (within a given tolerance of 1e-10)