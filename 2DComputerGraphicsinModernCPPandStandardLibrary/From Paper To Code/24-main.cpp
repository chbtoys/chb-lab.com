// From Paper to Code
// Arrows - order of magnitude
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: k \gg j
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <cmath>

double orderOfMagnitude(double n)
{
	return trunc(log10(n));
}

int main() {
	double k=1000.0;
	double j=0.001;

	if (orderOfMagnitude(k) > orderOfMagnitude(j))
	{
		std::cout << "k is in order of magnitude larger then j." << std::endl;
	}
}