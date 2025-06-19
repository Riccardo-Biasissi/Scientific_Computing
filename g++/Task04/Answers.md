# Answer to the questions

1) 
    Using N=100, the relative error of the trapezoidal rule with respect to the reference integration result (GSL's adaptive quadrature method) is of -6.4E-5

2) 
    To reduce the relative error, we can use a more complex integration method e.g. the Simpson rule. In this way, without reducing the number of samples (N=100) we recover a relative error just of -1.4E-9

3) 
    The minimum achievable absolute errors (E) using the trapezoidal (T) or Simpson (S) rules are:
    $$
    |E_T| \le \frac{(b-a)^3}{12N^2} \cdot \max_{x \in [a,b]}{|f^{(2)}(x)|}
    \\
    |E_S| \le \frac{(b-a)^5}{180N^4} \cdot \max_{x \in [a,b]}{|f^{(4)}(x)|}
    $$

    therefore, by substituting $f(x) = e^x \cdot \cos{(x)}$ we recover:
    $$
    \begin{align*}
    & |E_T| \le \frac{\pi^3 \exp{(\pi/2)}}{48N^2}
    \\
    & |E_S| \le \frac{\sqrt{2} \cdot \pi^5 \exp{(\pi/4)}}{2880N^4}
    \end{align*}
    $$

    By using for example N=100000 we recover relative errors (wrt GSL) of -6.3E-11 (trapezoidal) and 1.6E-13 (Simpson). The limit is the sampling frequency

4) 
    I computed the integral using the trapezoidal rule, combining the `task04_answer4.jl` and `data.dat` output file (N=100000). The absolute error wrt GSL computation is 9.2e-10