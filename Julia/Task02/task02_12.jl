# Measure the execution time for comparison with compiled language (C++)
@time begin
    # Define the dimensions
    N = 1e6
    N = Int(N)
    
    # Define the scalar and vectors
    a = 3.0
    x = fill(0.1, N)
    y = fill(7.1, N)

    # Compute the vector sum
    d = a .* x .+ y

    # Define a tolerance for floating-point comparison
    expected_value = 7.4
    tolerance = 1e-3

    # Verify that every element of d is equal to 7.4 within the tolerance
    is_correct = all(abs.(d .- expected_value) .< tolerance)

    # println("Vector d: ", d)
    println("Verification result: ", is_correct)
end