// From Paper to Code
// Dot and Cross - Cross Product
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: k \times j
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <tuple>

std::tuple<double,double,double> cross(std::tuple<double,double,double> &a, std::tuple<double,double,double> &b)
{
    double ax=0.0,ay=0.0,az=0.0;
    double bx=0.0,by=0.0,bz=0.0;
    double rx=0.0,ry=0.0,rz=0.0;

    std::tie (ax, ay, az) = a; // unpack elements
    std::tie (bx, by, bz) = b; // unpack elements

    rx = ay * bz - az * by;
    ry = az * bx - ax * bz;
    rz = ax * by - ay * bx;

    return std::make_tuple(rx, ry, rz);
}

int main() {
    std::tuple<double,double,double> k(0.0 , 1.0 , 0.0);
    std::tuple<double,double,double> j(1.0 , 0.0 , 0.0);

    std::tuple<double,double,double> result = cross(k, j);

    std::cout << "Result = [ " << std::get<0>(result) << ", " << std::get<1>(result) << ", " << std::get<2>(result) << " ]" << std::endl;
}