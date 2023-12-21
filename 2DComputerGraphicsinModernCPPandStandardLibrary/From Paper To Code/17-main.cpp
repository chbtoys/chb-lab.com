// From Paper to Code
// Piecewise Function
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: f(x)= 
// \begin{cases}
//     \frac{x^2-x}{x},& \text{if } x\geq 1\\
//     0, & \text{otherwise}
// \end{cases} 
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <cmath>

int f(int x)
{
	if (x >= 1) {
		return (pow(x, 2) - x) / x;
	}

	return 0;
}

int main() {
	std::cout << "f(5) = " << f(5) << std::endl;
}