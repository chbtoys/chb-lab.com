// From Paper to Code
// Factorial
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: \prod_{i=1}^5 i
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

int main()
{
    std::cout << "Factorial of 5 is: " << factorial(5) << std::endl;
    std::cout << "This is the same as: 1*2*3*4*5, which is: " << 1*2*3*4*5 << std::endl;
}