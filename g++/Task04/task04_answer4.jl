function main()
    data = readdlm("/data0/biasissi/Scientific_Computing/g++/Task04/data.dat", '\t', Float64)
    x = data[:, 1]
    y = data[:, 2]

    x_diff = x[2:end] - x[1:end-1]
    y_average = (y[2:end] + y[1:end-1]) ./ 2

    I = 0.0
    for i in eachindex(x_diff)
        I += x_diff[i] * y_average[i]
    end

    println("I = $I")
    println(1.905238690482675-I)
    println("All ok")
end

main()