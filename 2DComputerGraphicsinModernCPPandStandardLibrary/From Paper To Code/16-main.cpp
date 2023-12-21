// From Paper to Code
// Function
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: f(x,y) = \sqrt{x^2 + y^2}
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <cmath>

int length(int x, int y)
{
	return sqrt(x * x + y * y);
}

int main() {
	std::cout << "length(4, 5) = " << length(4, 5) << std::endl;
}