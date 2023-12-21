// Compile: clear && clang++ -std=c++20 25_lens.cpp -o 25_lens
#include "ppmpp.hpp"
#include <iostream>

int main() {
	ppm::Image img("28_input.ppm");

	img.applyLens(8);
	img.write("25_lens.ppm");
	return 0;
}