// Compile: clear && clang++ -std=c++20 24_bloom.cpp -o 24_bloom
#include "ppmpp.hpp"
#include <iostream>

int main() {
	ppm::Image img("23_anti_aliasing.ppm");

	img.applyBloom(0.1,25.0);
	img.write("24_bloom.ppm");
	return 0;
}