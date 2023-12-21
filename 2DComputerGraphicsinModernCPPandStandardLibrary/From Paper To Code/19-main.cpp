// From Paper to Code
// Prime
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: f'(x) = 2x
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <cmath>

int f(int x)
{
	return pow(x, 2);
}

int fPrime(int x)
{
	return 2 * x;
}

int main() {
	std::cout << "f(5) = " << f(5) << " | fPrime(5) = " << fPrime(5) << std::endl;
}