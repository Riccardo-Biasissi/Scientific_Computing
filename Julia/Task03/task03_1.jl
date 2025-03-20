# This package is used to write and read files in a more efficient way
using DelimitedFiles

function generate_vectors(N, filename_prefix)
    # Generate vectors x and y with the usual fill function
    x = fill(0.1, N)
    y = fill(7.1, N)
    
    # Create filenames using the filename_prefix and N
    # Remember that you input an f-string (Python-like) with $(param)
    # instead of {param} like you would do in Python
    filename_x = "$(filename_prefix)N$(N)_x.dat"
    filename_y = "$(filename_prefix)N$(N)_y.dat"
    
    # Save vectors to files using the DelimitedFiles package
    # I discovered that writelm function is faster than using a 
    # for loop to write each element of the vector in the ouput file
    writedlm(filename_x, x)
    writedlm(filename_y, y)
end

# Read N and filename_prefix from command line and in case they are not
# provided, print the usage of the script and exit
if length(ARGS) != 2
    println("Usage: julia task03.jl <N> <filename_prefix>")
    exit(1)
end

# Parse the input arguments (we want N to be an integer, not a string)
N = parse(Int, ARGS[1])
filename_prefix = ARGS[2]

# Call the function to generate the vectors
generate_vectors(N, filename_prefix)

#*-------------------- Example of usage --------------------*#

# To use this code you can run the following command in the terminal
# julia task03_1.jl 10 ./vector_
#
# This will generate two files vector_N10_x.dat and vector_N10_y.dat
# with the vectors x and y filled with 0.1 and 7.1 respectively