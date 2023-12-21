// Compile: clear && clang++ -std=c++20 19_triangle_grid.cpp -o 19_triangle_grid
#include <iostream>
#include <tuple>
#include <algorithm>
#include "ppmpp.hpp"

void averageTriangleColor(ppm::Image& img, ppm::Point& pt1, ppm::Point& pt2, ppm::Point& pt3, ppm::Pixel& averageColor) {
    double red_sum = 0.0;double green_sum = 0.0;double blue_sum = 0.0;
    int count = 0;

    auto [x1, y1] = pt1;auto [x2, y2] = pt2;auto [x3, y3] = pt3;

    if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
    if (y1 > y3) { std::swap(y1, y3); std::swap(x1, x3); }
    if (y2 > y3) { std::swap(y2, y3); std::swap(x2, x3); }

    int t1x = x1, t2x = x1, y = y1;
    int dx1 = std::abs(x2 - x1), dx2 = std::abs(x3 - x1);
    int dy1 = y2 - y1, dy2 = y3 - y1;
    int signx1 = (x2 - x1 > 0) ? 1 : -1, signx2 = (x3 - x1 > 0) ? 1 : -1;
    int e1 = dx1 / 2, e2 = dx2 / 2;
    bool changed1 = false, changed2 = false;
    if (dy1 > dx1) { std::swap(dy1, dx1); changed1 = true; }
    if (dy2 > dx2) { std::swap(dy2, dx2); changed2 = true; }

    ppm::Pixel px;

    for (; y <= y3; y++) {
        int minx = std::min(t1x, t2x);
        int maxx = std::max(t1x, t2x);

        for (int x = minx; x <= maxx; ++x) {
            if (x >= 0 && x < img.getWidth() && y >= 0 && y < img.getHeight()) {
                px = img.getPixel(x, y);
                red_sum += std::get<0>(px);
                green_sum += std::get<1>(px);
                blue_sum += std::get<2>(px);
                ++count;
            }
        }

        for (int i = 0; i < dx1; i++) {
            e1 += dy1;
            while (e1 >= dx1) {
                e1 -= dx1;
                if (changed1) break;
                else t1x += signx1;
            }
            if (changed1) t1x += signx1;
        }

        for (int i = 0; i < dx2; i++) {
            e2 += dy2;
            while (e2 >= dx2) {
                e2 -= dx2;
                if (changed2) break;
                else t2x += signx2;
            }
            if (changed2) t2x += signx2;
        }
    }

    if (count == 0) {
        averageColor = std::make_tuple(0.0, 0.0, 0.0);
    } else {
        averageColor = std::make_tuple(red_sum / count, green_sum / count, blue_sum / count);
    }
}

int main() {
    ppm::Image img2("19_photo.ppm");
    ppm::Pixel px=img2.getAverageRgbOfImage();
    int width = img2.getWidth();int height = img2.getHeight();
    ppm::Image img(width,height);
    img.setAllPixels(px);
    double h=0.0;double s=0.0;double v=0.0;
    ppm::Pixel px1;
    ppm::Pixel px2;
    ppm::Point pt1;
    ppm::Point pt2;
    ppm::Point pt3;

    int gridSize = 64;
    int squareWidth = width / gridSize;
    int squareHeight = height / gridSize;

    for (int i = 0; i <= gridSize; ++i) {
        for (int j = 0; j <= gridSize; ++j) {
            int pt1_x = i * squareWidth, pt1_y = j * squareHeight;
            int pt2_x = pt1_x + squareWidth, pt2_y = pt1_y;
            int pt3_x = pt2_x, pt3_y = pt2_y + squareHeight;
            int pt4_x = pt1_x, pt4_y = pt1_y + squareHeight;
    
            pt1=std::make_tuple(pt1_x, pt1_y);pt2=std::make_tuple(pt2_x, pt2_y);pt3=std::make_tuple(pt4_x, pt4_y);
            averageTriangleColor(img2,pt1,pt2,pt3,px1);
            img.drawFilledTriangle(pt1,pt2,pt3,px1);

            pt1=std::make_tuple(pt3_x, pt3_y);pt2=std::make_tuple(pt4_x, pt4_y);pt3=std::make_tuple(pt2_x, pt2_y);
            averageTriangleColor(img2,pt1,pt2,pt3,px2);
            img.drawFilledTriangle(pt1,pt2,pt3,px2);
        }
    }

    img.write("19_triangle_grid.ppm");

    return 0;
}