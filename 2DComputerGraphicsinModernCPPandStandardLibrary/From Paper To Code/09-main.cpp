// From Paper to Code
// Capital Pi - Product
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: \prod_{i=1}^{6} i
// Test LaTeX: https://quicklatex.com/
#include <iostream>

int main() {
	int prod = 1;
	for (int i = 1; i <= 6; i++) {
		prod *= i;
	}

	std::cout << "Product = " << prod << std::endl;
}