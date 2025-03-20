# Define the parameter a and the number of points N
a = 3.0
N = 20

# Create x vector (N=20) of 1
x = range(1,1,N)

# Create y vector (N=20) of 4
y = range(4,4,N)

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

# # Suppose that now I want to plot the z in function of a list like [1,2,3,4,5,...,20]
# using Plots         # This package is used to plot the data
# using LaTeXStrings  # This package is used to write LaTeX strings as L"..."

# # I want to create a list of x values from 0 to 20 with N=200 points
# x = range(0, stop=100, length=1000)

# z = a .* sin.(x)./x 
# w = (x .- 1)./(x .+ a)
# # Increase the image resolution
# size = 20
# default(size=(1000,800), legendfontsize=size, guidefontsize=size, tickfontsize=size, titlefontsize=size)
# # Plot the z in function of the x
# plot(x, z, label=L"z = a \cdot \frac{sin(x)}{x}", xlabel="x", ylabel="z", title="Plot of z in function of the x", lw=3, color="royalblue")
# plot!(x, w, label=L"w = \frac{x - 1}{x + a}", lw=3, color="deeppink")
# savefig("Julia/Task01_Followup/plot.png")