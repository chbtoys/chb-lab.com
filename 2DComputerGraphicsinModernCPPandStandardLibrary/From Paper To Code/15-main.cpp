// From Paper to Code
// Function
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: f\left (x \right) = x^{2}
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <cmath>

int square(int x)
{
	return pow(x,2);
}

int main() {
	std::cout << "10^2 = " << square(10) << std::endl;
}