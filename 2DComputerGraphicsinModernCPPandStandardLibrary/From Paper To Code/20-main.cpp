// From Paper to Code
// Floor & Ceiling
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: \lfloor x \rfloor
// LaTeX: \lceil x \rceil 
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <cmath>

int main() {
    // Example of floor()
    std::cout << "Example of floor():" << '\n' << std::fixed
        << "floor(+2.7) = " << std::floor(+2.7) << '\n'
        << "floor(-2.7) = " << std::floor(-2.7) << '\n'
        << "floor(-0.0) = " << std::floor(-0.0) << '\n'
        << "floor(-Inf) = " << std::floor(-INFINITY) << '\n';

    // Example of ceil()
    std::cout << '\n' << "Example of ceil():" << '\n' << std::fixed
        << "ceil(+2.4) = " << std::ceil(+2.4) << '\n'
        << "ceil(-2.4) = " << std::ceil(-2.4) << '\n'
        << "ceil(-0.0) = " << std::ceil(-0.0) << '\n'
        << "ceil(-Inf) = " << std::ceil(-INFINITY) << '\n';
}