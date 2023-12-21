// From Paper to Code
// Factorial
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: e = \sum_{n=0}^{\infty} \frac{1}{n!}
// Test LaTeX: https://quicklatex.com/
#include <iostream>

int factorial(int n)
{
    int temp=1;
    for (int i=1;i<=n;++i) {
        temp=temp*i;
    }
    return temp;
}

double e()
{
    double temp=0.0;
    int temp2=0;
    for (int n=0;n<100;++n) // Instead of infinity we try with 100 iterations.
    {
        temp2=factorial(n);
        if (temp2 != 0) // Can't divide with 0.
        {
            temp = temp + 1.0 / temp2;
        }
    }
    return temp;
}

int main()
{
    std::cout << "e(): " << e() << std::endl;
    std::cout << "Instead of running: 1.0/(factorial(0))+1.0/(factorial(1))+1.0/(factorial(2))+1.0/(factorial(3))..." << std::endl;
}