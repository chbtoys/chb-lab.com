// From Paper to Code
// Floor & Ceiling
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: \lfloor x \rfloor
// LaTeX: round(x) =  \lfloor x \rceil
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <cmath>

int main() {
	// Example of round()
	std::cout << "Example of round():" << '\n';
	std::cout << "round(+2.3) = " << std::round(2.3) << '\n'
		<< "round(+2.5) = " << std::round(2.5) << '\n'
		<< "round(+2.7) = " << std::round(2.7) << '\n' << '\n'
		<< "round(-2.3) = " << std::round(-2.3) << '\n'
		<< "round(-2.5) = " << std::round(-2.5) << '\n'
		<< "round(-2.7) = " << std::round(-2.7) << '\n' << '\n';

	std::cout << "round(-0.0) = " << std::round(-0.0)  << '\n'
		<< "round(-Inf) = " << std::round(-INFINITY) << '\n';
}