// From Paper to Code
// Pipes - Euclidean norm
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: ||v||
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <tuple>
#include <cmath>

double length(std::tuple<double,double,double> &vec)
{
	double x=0.0,y=0.0,z=0.0;
	std::tie (x, y, z) = vec;
	return sqrt(x * x + y * y + z * z);
}

int main() {
	std::tuple<double,double,double> v(0.0, 4.0, -3.0);
	std::cout << "The length of vector = " << length(v) << std::endl;
}