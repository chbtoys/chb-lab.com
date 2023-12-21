// From Paper to Code
// Logical Negation
// Run with: https://www.jdoodle.com/online-compiler-c++17/
// LaTeX: x \neq y \Leftrightarrow \lnot(x = y)
// Test LaTeX: https://quicklatex.com/
#include <iostream>
#include <cassert>

int main() {
	int x=5,y=7;

	if (x != y)
	{
		assert(!(x == y));
	}
}