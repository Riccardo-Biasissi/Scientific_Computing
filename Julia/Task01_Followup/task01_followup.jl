# Define the parameter a
a = 3.0

# Create x vector (N=20) of 1
x = zeros(20)
for i in eachindex(x)
    x[i] = x[i] + 1
end

# Create y vector (N=20) of 4
y = zeros(20)
for i in eachindex(y)
    y[i] = y[i] .+ 4
end

# Compute the operation z = a * x + y
z = a .* x .+ y

# Check the vectors just created printing them
println("x: ", x)
println("y: ", y)
println("z: ", z)

# Create a file output.txt and write the z vector in it
open("Julia/Task01_Followup/output.txt", "w") do io
    for i in eachindex(z)
        println(io, z[i])
    end
end