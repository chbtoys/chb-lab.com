// From Paper to Code
// intervals
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: x \in [0, 1]^3
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <cmath>
#include <limits>
#include <vector>

int main() {
	std::vector<double> a = {nextafter(0.0, std::numeric_limits<double>::infinity()), nextafter(1.0, -std::numeric_limits<double>::infinity())}; // open interval
	std::vector<double> b = {nextafter(0.0, std::numeric_limits<double>::infinity()), 1}; // interval closed on the left 
	std::vector<double> c = {0, nextafter(1.0, -std::numeric_limits<double>::infinity())}; // interval closed on the right
	std::vector<double> d = {0, 1}; // closed interval
}