// Compile: clear && clang++ -std=c++20 18_concentric_hexagons.cpp -o 18_concentric_hexagons
#include <cmath>
#include <vector>
#include <tuple>
#include <algorithm>
#include "ppmpp.hpp"

bool isInsideTriangle(const ppm::Point& p, const ppm::Point& a, const ppm::Point& b, const ppm::Point& c) {
    int x = std::get<0>(p), y = std::get<1>(p);
    int x1 = std::get<0>(a), y1 = std::get<1>(a);
    int x2 = std::get<0>(b), y2 = std::get<1>(b);
    int x3 = std::get<0>(c), y3 = std::get<1>(c);

    double denominator = (y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3);
    if (denominator == 0) return false;

    double aNumerator = (y2 - y3) * (x - x3) + (x3 - x2) * (y - y3);
    double bNumerator = (y3 - y1) * (x - x3) + (x1 - x3) * (y - y3);
    double aCoord = aNumerator / denominator;
    double bCoord = bNumerator / denominator;
    double cCoord = 1 - aCoord - bCoord;

    return 0 <= aCoord && aCoord <= 1 && 0 <= bCoord && bCoord <= 1 && 0 <= cCoord && cCoord <= 1;
}

void drawFilledHexagonWithTexture(ppm::Image& targetImage, ppm::Image& textureImage, const ppm::Point& center, int radius, int offset) {
    std::vector<ppm::Point> points;
    ppm::Pixel textureColor;
    int min_x = std::numeric_limits<int>::max(), max_x = std::numeric_limits<int>::min();
    int min_y = std::numeric_limits<int>::max(), max_y = std::numeric_limits<int>::min();

    // Compute the corners of the hexagon
    for (int i = 0; i < 6; ++i) {
        double angle = M_PI / 3.0 * i;
        int x = std::get<0>(center) + radius * std::cos(angle);
        int y = std::get<1>(center) + radius * std::sin(angle);
        points.emplace_back(x, y);

        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);
    }

    // Loop through all the pixels in the bounding box
    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            ppm::Point p(x, y);
            // Check if the point lies inside any of the four triangles that make up the hexagon
            if (isInsideTriangle(p, center, points[0], points[1]) ||
                isInsideTriangle(p, center, points[1], points[2]) ||
                isInsideTriangle(p, center, points[2], points[3]) ||
                isInsideTriangle(p, center, points[3], points[4]) ||
                isInsideTriangle(p, center, points[4], points[5]) ||
                isInsideTriangle(p, center, points[5], points[0])) {
                
                textureColor = textureImage.getPixel(x - offset, y - offset);
                targetImage.setPixel(x, y, textureColor);
            }
        }
    }
}

// Function to generate and draw a filled hexagon
void drawFilledHexagon(ppm::Image& image, const ppm::Point& center, int radius, const ppm::Pixel& color) {
    std::vector<ppm::Point> points;
    int min_x = std::numeric_limits<int>::max(), max_x = std::numeric_limits<int>::min();
    int min_y = std::numeric_limits<int>::max(), max_y = std::numeric_limits<int>::min();

    for (int i = 0; i < 6; ++i) {
        double angle = M_PI / 3.0 * i;
        int x = std::get<0>(center) + radius * std::cos(angle);
        int y = std::get<1>(center) + radius * std::sin(angle);
        points.emplace_back(x, y);

        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
        min_y = std::min(min_y, y);
        max_y = std::max(max_y, y);
    }

    // Go through all the pixels in the bounding box
    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            ppm::Point p(x, y);
            // Check if the point lies inside any of the four triangles that make up the hexagon
            if (isInsideTriangle(p, center, points[0], points[1]) ||
                isInsideTriangle(p, center, points[1], points[2]) ||
                isInsideTriangle(p, center, points[2], points[3]) ||
                isInsideTriangle(p, center, points[3], points[4]) ||
                isInsideTriangle(p, center, points[4], points[5]) ||
                isInsideTriangle(p, center, points[5], points[0])) {
                image.setPixel(x, y, color);
            }
        }
    }
}

// Function to generate and draw a single hexagon
void drawHexagon(ppm::Image& image, const ppm::Point& center, int radius, const ppm::Pixel& color) {
    std::vector<ppm::Point> points;
    ppm::Point pt1; ppm::Point pt2;

    for (int i = 0; i < 6; ++i) {
        double angle = M_PI / 3.0 * i;
        int x = std::get<0>(center) + radius * std::cos(angle);
        int y = std::get<1>(center) + radius * std::sin(angle);
        points.emplace_back(x, y);
    }

    for (int i = 0; i < 6; ++i) {
        pt1=points[i];
        pt2=points[(i + 1) % 6];
        ppm::Coord lineCoords = std::make_tuple(std::get<0>(pt1), std::get<1>(pt1), std::get<0>(pt2), std::get<1>(pt2));
        image.drawLine(lineCoords, color);
    }
}

int main() {
    ppm::Image img(1920, 1080);
    ppm::Image img2(1920, 1080);
    ppm::Pixel white = std::make_tuple(1.0, 1.0, 1.0);
    ppm::Pixel black = std::make_tuple(0.0, 0.0, 0.0);
    std::vector<ppm::Pixel> colors={{38.0/255.0,39.0/255.0,42.0/255.0},{72.0/255.0,69.0/255.0,68.0/255.0},
    {120.0/255.0,118.0/255.0,120.0/255.0},{157.0/255.0,134.0/255.0,113.0/255.0},{192.0/255.0,176.0/255.0,157.0/255.0}};
    std::vector<ppm::Pixel> colors2={{5.0/255.0,5.0/255.0,5.0/255.0},{17.0/255.0,17.0/255.0,17.0/255.0},{34.0/255.0,34.0/255.0,34.0/255.0},
    {51.0/255.0,51.0/255.0,51.0/255.0},{68.0/255.0,68.0/255.0,68.0/255.0},{85.0/255.0,85.0/255.0,85.0/255.0},
    {102.0/255.0,102.0/255.0,102.0/255.0},{119.0/255.0,119.0/255.0,119.0/255.0},{136.0/255.0,136.0/255.0,136.0/255.0},
    {153.0/255.0,153.0/255.0,153.0/255.0}};
    img.drawGradients(colors,45.0);
    img2=img;

    // Center of the image
    ppm::Point center = std::make_tuple(1920 / 2, 1080 / 2);

    // Draw concentric hexagons
    for (int radius = 50; radius <= 500; radius += 50) {
        drawHexagon(img, center, radius, white);
    }

    img.write("18_concentric_hexagons.ppm");

    // Draw filled concentric hexagons
    img.setAllPixels(black);
    int i=0;
    for (int radius = 500; radius >= 0; radius -= 50) {
        drawFilledHexagon(img, center, radius, colors2[i]);
        i++;
    }

    img.write("18_concentric_filled_hexagons.ppm");
    img=img2;
    std::reverse(colors.begin(), colors.end());
    img2.drawGradients(colors,45.0);

    // Draw textured concentric hexagons
    int offset=40;
    for (int radius = 500; radius >= 0; radius -= 50) {
        drawFilledHexagonWithTexture(img, img2, center, radius, offset);
        offset+=40;
    }

    img.write("18_concentric_textured_hexagons.ppm");

    return 0;
}