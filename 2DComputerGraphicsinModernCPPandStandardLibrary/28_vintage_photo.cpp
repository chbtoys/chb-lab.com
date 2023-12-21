// Compile: clear && clang++ -std=c++20 28_vintage_photo.cpp -o 28_vintage_photo
#include <vector>
#include <tuple>
#include <cmath>
#include <fstream>
#include <random>
#include <algorithm>
#include "ppmpp.hpp"

class Vintage
{
	public:
		Vintage() {
			image.read("28_input.ppm");
			image.write("28_0_output_input_original.ppm");
			width = image.getWidth();
			height = image.getHeight();
		}

		void convertToVintagePhoto() {
			img = image.getImage();
			deserializeAlphaValuesImpl();
			px = ppm::createfPixelWithColor(1.0,1.0,1.0);
			addSchmutzImpl();
			image.setImage(img,width,height);
			image.write("28_1_output_addSchmutz.ppm");
			applyGammaCorrection(1.8);
			image.setImage(img,width,height);
			image.write("28_2_output_applyGamma.ppm");
			addNoiseToImage(0.1);
			image.setImage(img,width,height);
			image.write("28_3_output_addNoise.ppm");
			applyCircularVignetteWithOpacity(0.9);
			image.setImage(img,width,height);
			image.write("28_4_output_applyCircularVignette.ppm");
			applyGammaCorrection(0.85);
			image.setImage(img,width,height);
			image.write("28_5_output_applyGamma.ppm");
			convertToGrayscaleSepiaImpl();
			image.setImage(img,width,height);
			image.write("28_6_output_applySepia.ppm");
		}
	private:
		void deserializeAlphaValuesImpl() {
		    std::ifstream inFile("28_alphavalues.dat", std::ios::binary);

		    double alpha;
		    while (inFile.read(reinterpret_cast<char*>(&alpha), sizeof(double))) {
		        alphavalues.push_back(alpha);
		    }
		}

		void convertToGrayscaleSepiaImpl() {
		    for (auto& pixel : img) {
		        auto& [r, g, b] = pixel;

		        // Convert to grayscale
		        double gray = 0.299 * r + 0.587 * g + 0.114 * b;

		        // Convert grayscale to sepia
		        r = std::clamp(gray * 0.393 + gray * 0.769 + gray * 0.189, 0.0, 1.0);
		        g = std::clamp(gray * 0.349 + gray * 0.686 + gray * 0.168, 0.0, 1.0);
		        b = std::clamp(gray * 0.272 + gray * 0.534 + gray * 0.131, 0.0, 1.0);
		    }
		}

		void applyGammaCorrection(double gamma) {
		    for (auto& pixel : img) {
		        auto& [r, g, b] = pixel;

		        r = std::pow(r, gamma);
		        g = std::pow(g, gamma);
		        b = std::pow(b, gamma);

		        // Ensure the pixel values are still in the range 0.0 to 1.0
		        r = std::clamp(r, 0.0, 1.0);
		        g = std::clamp(g, 0.0, 1.0);
		        b = std::clamp(b, 0.0, 1.0);
		    }
		}

		void addSchmutzImpl() {
		    size_t start_index = 0;
		    for (auto it = img.begin(); it != img.end(); ++it, ++start_index) {
		        double alpha = alphavalues[start_index];
		        auto& foregroundColor = *it;

		        // Invert both layers' color values
		        double invR1 = 1.0 - std::get<0>(px);
		        double invG1 = 1.0 - std::get<1>(px);
		        double invB1 = 1.0 - std::get<2>(px);

		        double invR2 = 1.0 - std::get<0>(foregroundColor);
		        double invG2 = 1.0 - std::get<1>(foregroundColor);
		        double invB2 = 1.0 - std::get<2>(foregroundColor);

		        // Multiply the inverted values and invert the result
		        double r = 1.0 - (invR1 * invR2);
		        double g = 1.0 - (invG1 * invG2);
		        double b = 1.0 - (invB1 * invB2);

		        // Blend with the original image based on alpha
		        r = (r * alpha) + (std::get<0>(foregroundColor) * (1.0 - alpha));
		        g = (g * alpha) + (std::get<1>(foregroundColor) * (1.0 - alpha));
		        b = (b * alpha) + (std::get<2>(foregroundColor) * (1.0 - alpha));

		        *it = {r, g, b};
		    }
		}

		void applyCircularVignetteWithOpacity(double overallOpacity) {
		    double centerX = width / 2.0;
		    double centerY = height / 2.0;
		    double maxDistance = std::sqrt(centerX * centerX + centerY * centerY);

		    for (size_t start_index = 0; start_index < img.size(); ++start_index) {
		        size_t x = start_index % width;
		        size_t y = start_index / width;

		        // Calculate distance from center
		        double distance = std::sqrt(std::pow(x - centerX, 2) + std::pow(y - centerY, 2));
		        double normalizedDistance = (distance*0.7) / maxDistance;

		        // Circular vignette effect: 1.0 at edges, 0.0 at center
		        double vignetteAlpha = normalizedDistance;

		        // Apply overall opacity
		        vignetteAlpha *= overallOpacity;

		        auto& [r, g, b] = img[start_index];
		        // Darken the pixel based on vignetteAlpha
		        r *= (1.0 - vignetteAlpha);
		        g *= (1.0 - vignetteAlpha);
		        b *= (1.0 - vignetteAlpha);
		    }
		}

		void addNoiseToImage(double noiseLevel) {
		    std::random_device rd;
		    std::mt19937 gen(rd());
		    std::uniform_real_distribution<> dis(-noiseLevel, noiseLevel);

		    for (auto& pixel : img) {
		        auto& [r, g, b] = pixel;

		        // Add random noise
		        r = std::clamp(r + dis(gen), 0.0, 1.0);
		        g = std::clamp(g + dis(gen), 0.0, 1.0);
		        b = std::clamp(b + dis(gen), 0.0, 1.0);
		    }
		}

		std::vector<ppm::Pixel> img;
		ppm::Image image;
		std::vector<double> alphavalues;
		ppm::Pixel px;
		int width = 0;
        int height = 0;
};

int main() {
    Vintage vintage;
    vintage.convertToVintagePhoto();
    return 0;
}