// From Paper to Code
// Arrows - conjunction & disjunction
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: k > 2 \land k <  4 \Leftrightarrow k = 3
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <cassert>

int main() {
	int k=3;

	if (k == 3)
	{
		assert(k > 2 && k < 4);
	}
}