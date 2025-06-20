# Answer to the questions

a)
    The results are always 0.5 (correct), however initially I used only `double` type numbers for both vector and parameters while in the uploaded version I used `long double` for improved precision. This initial choice resulted in all three methods giving -0.5 (losing precision in first step when summin 1 + 1E16). Kahan algorithm gives the correct result if we consider a sum vector constituted by 10 values equal to 0.1, while the other two methods fails

b)
    To check that `d = a*x+y` (with a=1.0) is correct, I would compute the mean and standard deviation of the array `d`. It can be done rapidly using the `checker.py` file (requires `numpy`). The result during my run is that x_vector and y_vector have respectively a standard deviation of 0.998 and 1.005 (coherent with what was required), the d_vector instead has a standard deviation of 1.418 which is the sum in quadrature of the other two (coherent with the sum of two Gaussian distribution)
