// From Paper to Code
// Summation
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: X = \{X_{1},X_{2},...,X_{n} \};
// LaTeX: avg(X) = \frac{1}{|x|} \sum_{i\epsilon\{1,2..n\}} x_{i}
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <vector>

double avg(std::vector<double> &X)
{
    double temp=0.0;
    for (int i=0;i<X.size();++i)
    {
        temp = temp + X[i];
    }
    temp = temp*1.0/X.size();
    return temp;
}

int main()
{
    std::vector<double> grades = {3.0, 1.0, 4.0};
    std::cout << "avg(grades) = " << avg(grades) << std::endl;
    std::cout << "Which is the same as: (3.0+1.0+4.0)/3, which is: " << (3.0+1.0+4.0)/3 << std::endl;
}