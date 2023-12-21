// Compile: clear && clang++ -std=c++20 27_image_to_patterns.cpp -o 27_image_to_patterns
#include "ppmpp.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>

using namespace ppm; // Use the ppm namespace for convenience

struct GrayscalePattern {
    uint8_t data[8][8];
};

std::map<int, GrayscalePattern> deserializeMetadata() {
    std::map<int, GrayscalePattern> metadata;
    std::ifstream inFile("27_patterns.dat", std::ios::binary);

    int brightness;
    GrayscalePattern pattern;
    while (inFile.read(reinterpret_cast<char*>(&brightness), sizeof(brightness))) {
        inFile.read(reinterpret_cast<char*>(&pattern), sizeof(GrayscalePattern));
        metadata[brightness] = pattern;
    }
    return metadata;
}

void generateOutputPpm(const std::string& inputPpmFilename, const std::string& outputPpmFilename, const std::map<int, GrayscalePattern>& metadata) {
    Image inputImage(inputPpmFilename);

    inputImage.convertToGrayscale();

    int inputWidth = inputImage.getWidth();
    int inputHeight = inputImage.getHeight();


    Image outputImage(inputWidth * 8, inputHeight * 8); // New image with increased size

    for (int y = 0; y < inputHeight; ++y) {
        for (int x = 0; x < inputWidth; ++x) {
            auto inputPixel = inputImage.getPixel(x, y);
            int brightness = static_cast<int>(std::get<0>(inputPixel) * 255.0); // Use red channel as grayscale value

            GrayscalePattern pattern;
            try {
                pattern = metadata.at(brightness);
            } catch (const std::out_of_range& e) {
                std::cerr << "Brightness level " << brightness << " not found in metadata." << std::endl;
                continue; // Skip this pixel or handle the error as required
            }

            for (int py = 0; py < 8; ++py) {
                for (int px = 0; px < 8; ++px) {
                    Pixel outputPixel = std::make_tuple((pattern.data[py][px]/255.0), (pattern.data[py][px]/255.0), (pattern.data[py][px]/255.0));
                    outputImage.setPixel(x * 8 + px, y * 8 + py, outputPixel);
                }
            }
        }
    }

    outputImage.write(outputPpmFilename);
}

int main() {
    std::string inputFilename = "27_input.ppm";
    std::string outputFilename = "27_output.ppm";

    auto metadata = deserializeMetadata();
    generateOutputPpm(inputFilename, outputFilename, metadata);
    return 0;
}