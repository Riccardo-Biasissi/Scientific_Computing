# Measure the execution time for comparison with compiled language (C++)
@time begin
    # Define the dimensions
    N = 10
    N = Int(N)

    # Create matrices A and B
    A = fill(3.0, N, N)
    B = fill(7.1, N, N)

    # Perform the multiplication
    C = A*B/N

    println("Matrix C: ", C)

    # Define the expected value and tolerance
    expected_value = 21.3
    tolerance = 1e-3

    # Check that every element of C is equal to the expected value within the tolerance
    is_correct = all(abs.(C .- expected_value) .< tolerance)

    # Print the result
    println("Verification result: ", is_correct)
end