// From Paper to Code
// Common Function
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: \begin{cases}
//     -1& \text{if } x < 0\\
//     0, & \text{if } {x = 0}\\
//     1, & \text{if } x > 0\\
// \end{cases}
// Test LaTeX: https://quicklatex.com/
#include <iostream>

int sgn(int x)
{
	if (x < 0) return -1;
	if (x > 0) return 1;
	return 0;
}

int main() {
	std::cout << "sgn(-5) = " << sgn(-5) << std::endl;
}