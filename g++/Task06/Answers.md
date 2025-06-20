# Answer to the questions

## c2c FFT Error Statistics:

Root Mean Square Absolute Error (Complex-to-Complex):   ~1e-16

Root Mean Square Relative Error (Complex-to-Complex):   ~1e-13

Root Median Square Absolute Error (Complex-to-Complex): ~1e-16

Root Median Square Relative Error (Complex-to-Complex): ~1e-16

## r2c FFT Error Statistics:

Root Mean Square Absolute Error (Real-to-Complex):      ~1e-16

Root Mean Square Relative Error (Real-to-Complex):      ~1e-13

Root Median Square Absolute Error (Real-to-Complex):    ~1e-16

Root Median Square Relative Error (Real-to-Complex):    ~1e-16

Despite there errors being of the same order, those obtained via the c2c FFT are slightly lower. In other words:
$$
\text{Error}_\text{c2c} \lesssim \text{Error}_\text{r2c}
$$

## Machine precision

Machine precision (~1e-16) is reached in all errors except for the root mean square relative error (RMSRE) in both c2c and r2c FFT. I guess that this is due to how the RMSRE is computed. Given elements $A_\text{original}$ from the original matrix and $A_\text{reconstruct}$ from the reconstructed matrix, the relative error (RE) is computed like this:
$$
\text{RE} = \frac{A_\text{original} - A_\text{reconstruct}}{A_\text{reconstruct}} = \frac{\text{AE}}{A_\text{reconstruct}}
$$
where AE is the absolute error. Since we have that, on average, the AE is of the order 1e-16 and some reconstructed values $A_\text{reconstruct}$ could be close to zero (then close to machine precision), we expect that the RE might get quite big. 

## Values of $C[0,0]$ and $R[0,0]$ matrices

I observed that the real part of $C[0,0]$ and $R[0,0]$ matrices correspond to the sum of all values of original matrix $A$. In the case of $N = 1000$, we have that $C[0,0] = R[0,0] = 1 \times 10^6$.