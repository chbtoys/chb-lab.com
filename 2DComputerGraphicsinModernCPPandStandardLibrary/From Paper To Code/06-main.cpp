// From Paper to Code
// Sigma - Summation
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: \sum_{i=1}^{100} i
// Test LaTeX: https://quicklatex.com/
#include <iostream>

int main() {
	int sum=0;
	for (int i=1;i<=100;i++)
	{
		sum += i;
	}

	std::cout << "Sum = " << sum << std::endl;
}