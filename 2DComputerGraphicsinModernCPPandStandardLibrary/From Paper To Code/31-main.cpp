// From Paper to Code
// Intervals - union
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: [3, 5) \cup [4, 6] = [3, 6]
// Test LaTeX: https://quicklatex.com/
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
 
int main()
{
    {
        std::vector<int> v1 = {1, 2, 3, 4, 5}; 
        std::vector<int> v2 = {      3, 4, 5, 6, 7}; 
        std::vector<int> dest1;

        std::set_union(v1.begin(), v1.end(),
                       v2.begin(), v2.end(),                  
                        std::back_inserter(dest1));
 
        for (const auto &i : dest1) {
            std::cout << i << ' ';
        }   
        std::cout << '\n';
    }
    {
        std::vector<int> v1 = {1, 2, 3, 4, 5, 5, 5}; 
        std::vector<int> v2 = {      3, 4, 5, 6, 7}; 
        std::vector<int> dest1;
 
        std::set_union(v1.begin(), v1.end(),
                       v2.begin(), v2.end(),                  
                        std::back_inserter(dest1));
 
        for (const auto &i : dest1) {
            std::cout << i << ' ';
        }   
        std::cout << '\n';
    }
}