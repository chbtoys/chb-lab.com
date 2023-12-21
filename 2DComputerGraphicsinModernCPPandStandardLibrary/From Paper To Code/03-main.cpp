// From Paper to Code
// Dot and Cross - vector multiplication
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: 3k \circ j
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <vector>

void multiply(std::vector<int> &a, std::vector<int> &b, std::vector<int> &c)
{
    if (a.size() == b.size())
    {
        for (int i=0;i<a.size();++i) {
            c.push_back(a[i] * b[i]);   
        }
    }
}

void multiplyScalar(std::vector<int> &a, int s, std::vector<int> &c)
{
    for (int i=0;i<a.size();++i) {
        c.push_back(a[i] * s);   
    }
}

int main() {
    int s = 3;
    std::vector<int> k = {2 , 7 , 4 , 5};
    std::vector<int> j = {2 , 7 , 4 , 5};
    std::vector<int> tmp;
    std::vector<int> result;

    multiply(k, j, tmp);
    multiplyScalar(tmp, s, result);

    std::cout << "Scalar: " << s << '\n' << "k: ";
    for (auto const& e : std::as_const(k)) {
        std::cout << e << " ";
    }
    std::cout << '\n' << "j: ";
    for (auto const& e : std::as_const(j)) {
        std::cout << e << " ";
    }
    std::cout << '\n' << "Multiply(tmp): ";
    for (auto const& e : std::as_const(tmp)) {
        std::cout << e << " ";
    }
    std::cout << '\n' << "Scaler Multiplication(result): ";
    for (auto const& e : std::as_const(result)) {
        std::cout << e << " ";
    }
    std::cout << '\n';
}