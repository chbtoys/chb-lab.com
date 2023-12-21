// From Paper to Code
// Dot and Cross - Dot Product
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: k\cdot j
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <vector>

double dot(std::vector<double> &a, std::vector<double> &b)
{
    double tmp=0.0;

    if (a.size() == b.size())
    {
        for (int i=0;i<a.size();++i) {
            tmp = tmp + (a[i] * b[i]);   
        }
    }
    
    return tmp;
}

int main() {
    std::vector<double> k = {0.0 , 1.0 , 0.0};
    std::vector<double> j = {1.0 , 0.0 , 0.0};

    double d = dot(k, j);

    if (d == 0.0)
    {
        std::cout << "Vectors are perpendicular to each other." << std::endl;
    }
}