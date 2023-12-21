// From Paper to Code
// Pipes - Determinant
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX:  |A|
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <vector>

double determinant(std::vector<std::vector<double>> &a)
{
    double c, r=1;
    int n=2;
    for(int i = 0; i < n; i++) {
        for(int k = i+1; k < n; k++) {
            c = a[k][i] / a[i][i];
            for(int j = i; j < n; j++)
                a[k][j]=a[k][j] - c*a[i][j];
        }
    }
    for (int i = 0; i < n; i++)
    {
        r*=a[i][i];
    }
    return r;
}

int main() {
    std::vector<std::vector<double>> matrix = { {1.0 , 0.0} , {0.0 , 1.0} };
    double det = determinant(matrix);
    std::cout << "Determinant(Matrix) = " << det << std::endl;
}