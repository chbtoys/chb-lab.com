// Compile: clear && clang++ -std=c++20 21_exchange_colors.cpp -o 21_exchange_colors
#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include <iostream>
#include <filesystem>

namespace quant {
	struct Color
	{
		uint8_t r, g, b;
	};

	class ExchangeColors {
	public:
		ExchangeColors(std::string& fname) {
			filename=filenameWithoutExtension(fname);
			readPalette();
			readIndexedImage();
			generateImage();
			writePPMP6();
		}
	private:
		void readPalette() {
			std::cout << "Reading: " << filename << ".txt" << std::endl;
	        std::ifstream infile(filename+".txt", std::ifstream::binary);
	        if (!infile.is_open()) {
	            std::cout << "Can't open " << filename << ".txt" << std::endl;
	            exit(1);
	        }
	        // Skip the first line (comment line)
		    std::string line;
		    std::getline(infile, line);
		    infile >> width >> height;
		    int paletteSize;
		    infile >> paletteSize;
		    palette.clear();
		    palette.reserve(paletteSize);
		    Color color;
		    for (int i = 0; i < paletteSize; ++i) {
		        int r, g, b;
		        infile >> r >> g >> b;
		        color={static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b)};
		        if (isBlue(color)) {
		        	color={static_cast<uint8_t>(r), static_cast<uint8_t>(b), static_cast<uint8_t>(g)};
		        }
		        palette.push_back(color);
		    }
		    infile.close();
	    }

	    void readIndexedImage() {
	    	std::cout << "Reading: " << filename << ".dat" << std::endl;
		    std::ifstream infile(filename+".dat", std::ios::binary);
		    if (!infile) {
		        std::cout << "Can't open " << filename << ".dat" << std::endl;
	            exit(1);
		    }
		    uint8_t byte;
		    while (infile.read(reinterpret_cast<char*>(&byte), sizeof(uint8_t))) {
		        int value = static_cast<int>(byte);
		        indexedImage.push_back(value);
		    }
		    infile.close();
	    }

	    void writePPMP6() {
	    	std::string fname="21_exchangecolors.ppm";
	    	std::cout << "Writing: " << fname << std::endl;
		    std::ofstream outfile(fname, std::ofstream::binary);
		    if (!outfile.is_open()) {
		        std::cout << "Can't open " << fname << std::endl;
		        exit(1);
		    }
		    outfile << "P6\n" << width << " " << height << "\n" << "255\n";
		    uint8_t buffer[3];
		    for (const auto& color : image) {
		        buffer[0] = color.r;
		        buffer[1] = color.g;
		        buffer[2] = color.b;
		        outfile.write(reinterpret_cast<char*>(buffer), 3);
		    }
		    outfile.close();
		}

	    void generateImage() {
		    image.clear();
		    image.reserve(indexedImage.size());
		    for (const auto& index : indexedImage) {
		        if (index >= 0 && index < static_cast<int>(palette.size())) {
		            image.push_back(palette[index]);
		        }
		    }
		}

	    std::string filenameWithoutExtension(const std::string& path) {
		    std::filesystem::path p(path);
		    return p.stem().string();
		}

		bool isBlue(const Color& color) {
		    return color.b > color.r && color.b > color.g;
		}

		std::vector<int> indexedImage;
		std::vector<Color> image;
		std::vector<Color> palette;
	    int width=0; int height=0;
	    std::string filename="";
	};
}

int main() {
	std::string filename = "20_colorquantizer_quantized.txt";
    quant::ExchangeColors ec(filename);

	return 0;
}