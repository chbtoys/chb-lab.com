// From Paper to Code
// Hat
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: \hat{a}
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <tuple>
#include <cmath>

std::tuple<double,double,double> normalize(std::tuple<double,double,double> &vec)
{
    double x=0.0,y=0.0,z=0.0;
    std::tie (x, y, z) = vec; // Unpack elements.

    double squaredLength = x * x + y * y + z * z;

    if (squaredLength > 0.0)
    {
        double length = sqrt(squaredLength);
        return std::make_tuple( (x / length), (y / length), (z / length));
    }
    return vec;
}

int main() {
    std::tuple<double,double,double> a( 0.0, 4.0, -3.0 );
    std::tuple<double,double,double> result=normalize(a);
    std::cout << "Normalized 3D vector = [ " << std::get<0>(result) << ", " << std::get<1>(result) << ", " << std::get<2>(result) << " ]" << std::endl;
}