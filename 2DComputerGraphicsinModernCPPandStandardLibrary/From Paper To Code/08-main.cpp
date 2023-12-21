// From Paper to Code
// Sigma - Summation
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: \sum_{i=1}^{2} \sum_{j=4}^{6} (3ij)
// Test LaTeX: https://quicklatex.com/
#include <iostream>

int main() {
	int sum=0;
	for (int i = 1; i <= 2; i++) {
		for (int j = 4; j <= 6; j++) {
			sum += (3 * i * j);
		}
	}

	std::cout << "Sum = " << sum << std::endl;
}