// From Paper to Code
// Sigma - Summation
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: \sum_{i=1}^{100} (2i + 1)
// Test LaTeX: https://quicklatex.com/
#include <iostream>

int main() {
	int sum=0;
	for (int i=1;i<=100;i++)
	{
		sum += (2 * i + 1);
	}

	std::cout << "Sum = " << sum << std::endl;
}