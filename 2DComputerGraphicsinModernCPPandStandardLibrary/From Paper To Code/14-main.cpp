// From Paper to Code
// Element of
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: A=\left \{ 3,9,14  \right \}, 3 \in A
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <vector>
#include <algorithm>

bool has(std::vector<int> &v, int e)
{
    if (std::find(v.begin(), v.end(), e) != v.end())
    {
        return true;
    }
    return false;
}

int main() {
    std::vector<int> A = { 3, 9, 14 };
    if (has(A,3))
    {
        std::cout << "Element found" << std::endl;
    } else {
        std::cout << "Element not found" << std::endl;
    }
}