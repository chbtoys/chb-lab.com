// From Paper to Code
// Complex Numbers
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: 7 + 3i
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <complex>

int main() {
    // defines the complex number: (7.0 + 3.0i)
    std::complex<double> mycomplex (7.0, 3.0);

    // prints the real part using the real function
    std::cout << "Real part: " << real(mycomplex) << std::endl;
    std::cout << "Imaginary part: " << imag(mycomplex) << std::endl;

    // prints the absolute value of the complex number
    std::cout << "The absolute value of " << mycomplex << " is: ";
    std::cout << abs(mycomplex) << std::endl;

    // prints the argument of the complex number
    std::cout << "The argument of " << mycomplex << " is: ";
    std::cout << arg(mycomplex) << std::endl;
}