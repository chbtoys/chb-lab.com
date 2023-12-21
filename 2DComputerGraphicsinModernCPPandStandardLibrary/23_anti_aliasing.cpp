// Compile: clear && clang++ -std=c++20 23_anti_aliasing.cpp -o 23_anti_aliasing
#include "ppmpp.hpp"
#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <sstream>
#include <algorithm>

using BezierControlPoints = std::tuple<ppm::Point, ppm::Point, ppm::Point, ppm::Point>;

int main() {
    std::vector<BezierControlPoints> bezierCurves;
    int xOffset = 247;
    int yOffset = 298;

    std::vector<ppm::Coord> lineData = {
        ppm::Coord{247,298,727,298},
        ppm::Coord{1191,298,1672,298},
        ppm::Coord{920,318,932,368},
        ppm::Coord{998,318,986,368}
    };

    std::vector<std::string> pathData = {"711 66 731 66 739 70 739 70",
										 "751 20 751 20 760 58 764 93",
										 "764 93 768 128 829 105 881 94",
										 "881 94 933 83 944 0 944 0",
										 "1425 0 1425 0 1134 87 1207 281",
										 "1207 281 1207 281 779 215 712 484",
										 "712 484 645 216 217 281 217 281",
										 "217 281 290 88 0 0 0 0",
										 "480 0 480 0 491 83 543 94",
										 "543 94 595 105 656 128 660 93",
										 "660 93 664 58 673 20 673 20",
										 "685 70 685 70 693 66 711 66"};
    for (const auto& data : pathData) {
        std::istringstream stream(data);
        int x0, y0, x1, y1, x2, y2, x3, y3;
        stream >> x0 >> y0 >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
        bezierCurves.emplace_back(ppm::Point{x0 + xOffset, y0 + yOffset},
                                  ppm::Point{x1 + xOffset, y1 + yOffset},
                                  ppm::Point{x2 + xOffset, y2 + yOffset},
                                  ppm::Point{x3 + xOffset, y3 + yOffset});
    }

    ppm::Pixel bezierColor{0.9, 0.9, 0.9};
    ppm::Image img(1920,1080);
    img.setAllPixels(ppm::Pixel{0.07,0.07,0.07});

    for (const auto& controlPoints : bezierCurves) {
        auto [pt0, pt1, pt2, pt3] = controlPoints;
        img.drawBezierCubic(pt0, pt1, pt2, pt3, 150, bezierColor);
    }

    for (auto& lineCoords : lineData) {
        img.drawLine(lineCoords, bezierColor);
    }

    img.applyAntiAliasing();

    img.write("23_anti_aliasing.ppm");

    return 0;
}
