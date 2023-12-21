// From Paper to Code
// Summation
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: X = \{X_{1},X_{2},...,X_{n} \};
// LaTeX: var(X) = \frac{1}{|x|^2} \sum_{x \epsilon X} \sum_{x \epsilon X} \frac{1}{2} (i - j)^2
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <vector>
#include <cmath>

double var(std::vector<double> &X)
{
    double temp=0.0;
    for (auto i : X)
    {
        for (auto j : X)
        {
            temp=temp+(1.0/2.0)*pow((i-j),2);
        }
    }
    temp=temp*1.0/pow(X.size(),2);
    return temp;
}

int main()
{
    std::vector<double> grades = {3.0, 1.0, 4.0};
    std::cout << "var(grades) = " << var(grades) << std::endl;
}