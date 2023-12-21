// Compile: clear && clang++ -std=c++20 20_color_quantizer.cpp -o 20_color_quantizer
#include <vector>
#include <cmath>
#include <random>
#include <fstream>
#include <sstream>
#include <limits>
#include <iostream>
#include <algorithm>
#include <filesystem>

namespace quant {
	struct Color
	{
		double r, g, b;

		// Define the '<' operator
        bool operator<(const Color& other) const {
            return std::tie(r, g, b) < std::tie(other.r, other.g, other.b);
        }
        bool operator==(const Color& other) const {
	        return r == other.r && g == other.g && b == other.b;
	    }
	};

	class ColorQuantizer {
	public:
		
		ColorQuantizer(std::string& fname) {
			filename=filenameWithoutExtension(fname);
			readPPMP6();
			kmeansQuantization();
    		floydSteinbergDithering();
    		writePalette();
    		writeIndexedImage();
    		writePPMP6();
		}
		
	private:
		void readPPMP6() {
			std::cout << "Reading: " << filename << ".ppm" << std::endl;
	        std::string magic;
	        int max;
	        uint8_t buffer[3];

	        std::ifstream in(filename+".ppm", std::ifstream::binary);
	        if (!in.is_open()) {
	            std::cout << "Can't open " << filename << std::endl;
	            exit(1);
	        }

	        in >> magic;
	        in.seekg(1, std::ios::cur);
	        char c;
	        in.get(c);
	        if (c == '#') {
	            while (c != '\n') {
	                in.get(c);
	            }
	        } else {
	            in.seekg(-1, std::ios::cur);
	        }

	        in >> width >> height >> max;
	        if (max != 255) {
	            std::cout << "Not 8 bit per RGB color." << std::endl;
	            exit(1);
	        }

	        if (magic == "P6") {
	            in.seekg(1, std::ios::cur);
	            imgsrc.clear();
	            for (int i = 0; i < width * height; ++i) {
	                in.read(reinterpret_cast<char*>(buffer), 3);

	                Color color;
	                color.r = static_cast<double>(buffer[0]) / 255.0;
	                color.g = static_cast<double>(buffer[1]) / 255.0;
	                color.b = static_cast<double>(buffer[2]) / 255.0;

	                imgsrc.push_back(color);
	            }
	        } else {
	            std::cout << filename << " is not a P6 file." << std::endl;
	            exit(1);
	        }
	        
	        in.close();
	    }

	    void writePPMP6() {
	    	std::cout << "Writing: " << filename << "_quantized.ppm" << std::endl;
		    std::ofstream outfile(filename+"_quantized.ppm", std::ofstream::binary);

		    if (!outfile.is_open()) {
		        std::cout << "Can't open " << filename << "_quantized.ppm" << std::endl;
		        exit(1);
		    }

		    // Write the header
		    outfile << "P6\n" << width << " " << height << "\n" << "255\n";

		    // Prepare to write image data
		    uint8_t buffer[3];

		    for (const auto& color : imgdst) {
		        buffer[0] = static_cast<uint8_t>(std::round(color.r * 255.0));
		        buffer[1] = static_cast<uint8_t>(std::round(color.g * 255.0));
		        buffer[2] = static_cast<uint8_t>(std::round(color.b * 255.0));
		        outfile.write(reinterpret_cast<char*>(buffer), 3);
		    }

		    outfile.close();
		}

		void writeIndexedImage() {
		    std::cout << "Writing: " << filename << "_quantized.dat" << std::endl;
		    createIndexedImage();
		    std::ofstream outfile(filename + "_quantized.dat", std::ofstream::binary);
		    if (!outfile.is_open()) {
		        std::cout << "Can't open " << filename << "_quantized.dat" << std::endl;
		        exit(1);
		    }
		    uint8_t index;
		    for (const auto& value : indexedImage) {
		        index = static_cast<uint8_t>(value);
		        outfile.write(reinterpret_cast<char*>(&index), 1);
		    }
		    outfile.close();
		}

	    std::string filenameWithoutExtension(const std::string& path) {
		    std::filesystem::path p(path);
		    return p.stem().string();
		}

		void writePalette() {
			std::cout << "Writing: " << filename << "_quantized.txt" << std::endl;
			uniqueColorPalette();
		    std::ofstream outfile(filename+"_quantized.txt");

		    if (!outfile.is_open()) {
		        std::cout << "Can't open " << filename << "_quantized.txt" << std::endl;
		        exit(1);
		    }
		    outfile << "# meta and palette for: " << filename << "_quantized.dat\n";
		    outfile << width << " " << height << "\n" << palette.size() << "\n";
		    for (const auto& color : palette) {
		        outfile << static_cast<int>(color.r * 255.0) << ' '
		                << static_cast<int>(color.g * 255.0) << ' '
		                << static_cast<int>(color.b * 255.0) << '\n';
		    }
		    outfile.close();
		}

		void uniqueColorPalette() {
			std::sort(palette.begin(), palette.end(), [](const Color& a, const Color& b) {
		        return std::tie(a.r, a.g, a.b) < std::tie(b.r, b.g, b.b);
		    });
		    auto last = std::unique(palette.begin(), palette.end(), [](const Color& a, const Color& b) {
		        return a.r == b.r && a.g == b.g && a.b == b.b;
		    });
		    palette.erase(last, palette.end());
		}

		void createIndexedImage() {
		    for (const auto& color : imgdst) {
		        auto it = std::find_if(palette.begin(), palette.end(), [&color](const Color& paletteColor) {
		            return color == paletteColor;
		        });

		        if (it != palette.end()) {
		            int index = std::distance(palette.begin(), it);
		            indexedImage.push_back(index);
		        }
		    }
		}

	    void kmeansQuantization() {
	   		std::cout << "kmeansQuantizer..." << std::endl;
		    // Initialize palette randomly
		    std::random_device rd;
		    std::mt19937 gen(rd());
		    std::uniform_real_distribution<> dis(0, 1);

		    for (int i = 0; i < 256; ++i) {
		        palette.push_back({ dis(gen), dis(gen), dis(gen) });
		    }

		    // Run K-means clustering
		    const int iterations = 10;
		    for (int iter = 0; iter < iterations; ++iter) {
		        std::vector<Color> newPalette(256, {0, 0, 0});
		        std::vector<int> counts(256, 0);

		        for (const auto &pixel : imgsrc) {
		            int bestIdx = 0;
		            double bestDist = std::numeric_limits<double>::max();
		            
		            for (int i = 0; i < 256; ++i) {
		                double dist = colorDistance(pixel, palette[i]);
		                if (dist < bestDist) {
		                    bestDist = dist;
		                    bestIdx = i;
		                }
		            }

		            newPalette[bestIdx].r += pixel.r;
		            newPalette[bestIdx].g += pixel.g;
		            newPalette[bestIdx].b += pixel.b;
		            counts[bestIdx]++;
		        }

		        // Update palette
		        for (int i = 0; i < 256; ++i) {
		            if (counts[i] > 0) {
		                newPalette[i].r /= counts[i];
		                newPalette[i].g /= counts[i];
		                newPalette[i].b /= counts[i];
		            }
		        }

		        palette = newPalette;
		    }
		}

		double colorDistance(const Color &c1, const Color &c2) {
		    return std::sqrt(std::pow(c1.r - c2.r, 2) + std::pow(c1.g - c2.g, 2) + std::pow(c1.b - c2.b, 2));
		}

		void floydSteinbergDithering() {
			std::cout << "FloydSteinbergDithering" << std::endl;
		    imgdst = imgsrc;
		    for (int y = 0; y < height; ++y) {
		        for (int x = 0; x < width; ++x) {
		            int idx = y * width + x;
		            Color oldPixel = imgdst[idx];

		            // Find nearest color from palette
		            int bestIdx = 0;
		            double bestDist = std::numeric_limits<double>::max();
		            for (int i = 0; i < 256; ++i) {
		                double dist = colorDistance(oldPixel, palette[i]);
		                if (dist < bestDist) {
		                    bestDist = dist;
		                    bestIdx = i;
		                }
		            }

		            Color newPixel = palette[bestIdx];
		            imgdst[idx] = newPixel;

		            Color error{
		                oldPixel.r - newPixel.r,
		                oldPixel.g - newPixel.g,
		                oldPixel.b - newPixel.b
		            };

		            // Floyd-Steinberg dithering
		            std::vector<std::pair<int, int>> dirs = { {1, 0}, {-1, 1}, {0, 1}, {1, 1} };
		            std::vector<double> coeffs = { 7.0 / 16, 3.0 / 16, 5.0 / 16, 1.0 / 16 };

		            for (size_t i = 0; i < dirs.size(); ++i) {
		                int dx = dirs[i].first;
		                int dy = dirs[i].second;
		                if (x + dx >= 0 && x + dx < width && y + dy >= 0 && y + dy < height) {
		                    int dIdx = (y + dy) * width + (x + dx);
		                    imgdst[dIdx].r = std::clamp(imgdst[dIdx].r + error.r * coeffs[i], 0.0, 1.0);
		                    imgdst[dIdx].g = std::clamp(imgdst[dIdx].g + error.g * coeffs[i], 0.0, 1.0);
		                    imgdst[dIdx].b = std::clamp(imgdst[dIdx].b + error.b * coeffs[i], 0.0, 1.0);
		                }
		            }
		        }
		    }
		}

		std::vector<int> indexedImage;
		std::vector<Color> imgsrc;
		std::vector<Color> imgdst;
		std::vector<Color> palette;
	    int width=0; int height=0;
	    std::string filename="";
	};
}

int main() {
	std::string filename = "20_colorquantizer.ppm";
    quant::ColorQuantizer cq(filename);

    return 0;
}