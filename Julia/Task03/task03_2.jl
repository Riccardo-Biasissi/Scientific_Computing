# This package is used to write and read files in a more efficient way
using DelimitedFiles

# Define the function to calculate the vector d
function calculate_d(config_file)
    # Read configuration file as a dictionary (therefore
    # we will have key-value pairs)
    config = Dict{String, String}() # Initialize an empty dictionary
    open(config_file, "r") do file # Open the configuration file in read mode
        for line in eachline(file)
            key, value = split(line, "=")
            config[strip(key)] = strip(value)
        end
    end

    # Extract values from configuration
    filename_x = config["filename_x"]
    filename_y = config["filename_y"]
    N = parse(Int, config["N"])
    a = parse(Float64, config["a"])
    prefix_output = config["prefix_output"]

    # Read vectors x and y from files
    x = readdlm(filename_x)
    y = readdlm(filename_y)

    # Calculate vector d
    d = a .* x .+ y

    # Create output filename
    filename_d = "$(prefix_output)N$(N)_d.dat"

    # Save vector d to file
    writedlm(filename_d, d)
end

# Read config file from command line
if length(ARGS) != 1
    println("Usage: julia task03_calculate.jl <config_file>")
    exit(1)
end

# Parse the input arguments (in this case we have only
# one argument: a string with the configuration file)
config_file = ARGS[1]

# Call the function to calculate the vector d
calculate_d(config_file)


#*-------------------- Example of usage --------------------*#

# To use this code you can run the following command in the terminal
# julia task03_2.jl config.txt
#
# This will read the configuration from the file config.txt and 
# calculate the vector d using the vectors x and y read from the
# files specified in the configuration file. The output will be
# saved in a file with the name specified in the configuration file
# with the prefix_output key
#
# The configuration file should look like this:
# filename_x = /path/to/vector_x.dat
# filename_y = /path/to/vector_y.dat
# N = 10
# a = 3.0
# prefix_output = /path/to/output/vector_