// Compile: clear && clang++ -std=c++20 07_clouds_marble_wood_texture.cpp -o 07_clouds_marble_wood_texture
#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <random>
#include <cmath>

int get_index(int x, int y, int width)
{
	return x+width*y;
}

int calc_size(int width, int height)
{
	return width*height;
}

void clear_image(std::vector<std::tuple<float, float, float>> &image, std::tuple<float, float, float> &color, int width, int height)
{
	for (int y=0;y<height;++y) {
		for (int x=0;x<width;++x) {
			image[get_index(x,y,width)]=color;
		}
	}
}

void save_image(std::vector<std::tuple<float, float, float>> &image, int width, int height, std::string filename)
{
	std::tuple<float, float, float> color;
	std::ofstream out(filename, std::ofstream::out);
	out << "P3\n" << width << " " << height << "\n255\n";
	for (int i=0;i<(width*height);++i)
	{
		color=image[i];
		out << int(std::get<0>(color)*255.0f) << " " << int(std::get<1>(color)*255.0f) << " " << int(std::get<2>(color)*255.0f) << '\n';
	}
	out.close();
}

void generate_noise(std::vector<double> &noise, int width, int height)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<> dist(0.0, 1.0);

	for (int y=0;y<height;++y) {
		for (int x=0;x<width;++x) {
			noise[get_index(x,y,width)]=dist(mt);
		}
	}
}

double smooth_noise(double x, double y, int width, int height, std::vector<double> &noise)
{
	// Get fractional part of X and Y.
	double fractionX = x - int(x);
	double fractionY = y - int(y);

	// Wrap around.
	int x1 = (int(x) + width) % width;
	int y1 = (int(y) + height) % height;

	// Neighbor values.
	int x2 = (x1 + width - 1) % width;
	int y2 = (y1 + height - 1) % height;

	// Smoth the noise with bilinear interpolation.
	double value=0.0;
	value += fractionX * fractionY * noise[get_index(x1,y1,width)];
	value += (1 - fractionX) * fractionY * noise[get_index(x2,y1,width)];
	value += fractionX * (1 - fractionY) * noise[get_index(x1,y2,width)];
	value += (1 - fractionX) * (1 - fractionY) * noise[get_index(x2,y2,width)];

	return value;
}

double turbulence(double x, double y, int width, int height, double size, std::vector<double> &noise)
{
	double value = 0.0;
	double initialSize = size;

	while (size >= 1)
	{
		value += smooth_noise(x/size,y/size,width,height,noise) * size;
		size /= 2.0;
	}

	return (128.0 * value / initialSize);
}

void generate_cloud(std::vector<std::tuple<float, float, float>> &image, int width, int height, std::vector<double> &noise)
{
	std::tuple<float, float, float> color;
	int c=0;

	for (int y=0;y<height;++y)
	{
		for (int x=0;x<width;++x) {
			c=int(turbulence(double(x),double(y),width,height,64,noise));
			color=std::make_tuple(float(c/255.0f), float(c/255.0f), float(c/255.0f));
			image[get_index(x, y, width)]=color;
		}
	}
}

void generate_marble(std::vector<std::tuple<float, float, float>> &image, int width, int height, std::vector<double> &noise)
{
	std::tuple<float, float, float> color;

	// xPeriod and yPeriod together define the angle of the lines.
	// xPeriod and yPeriod both 0 ==> it becomes a normal clouds or turbulence pattern.
	double xPeriod = 5.0; // Defines repetition of marble lines in x direction.
	double yPeriod = 10.0; // Defines repetition of marble lines in y direction.
	// turbulencePower = 0 ==> it becomes a normal sine pattern.
	double turbulencePower = 5.0; // Makes twists.
	double turbulenceSize = 32.0; // Initial size of the turbulence.

	double xyValue = 0.0;
	double sineValue = 0.0;

	for (int y=0;y<height;++y)
	{
		for (int x=0;x<width;++x) {
			xyValue = x * xPeriod / width + y * yPeriod / height + turbulencePower * turbulence(double(x),double(y),width,height,turbulenceSize,noise) / 255.0;
			sineValue = 256 * fabs(sin(xyValue * M_PI));
			color=std::make_tuple(float(sineValue/255.0f), float(sineValue/255.0f), float(sineValue/255.0f));
			image[get_index(x, y, width)]=color;
		}
	}
}

void generate_wood(std::vector<std::tuple<float, float, float>> &image, int width, int height, std::vector<double> &noise)
{
	std::tuple<float, float, float> color;
	
	double xyPeriod = 25.0; // Number of rings.
	double turbulencePower = 0.1; // Makes twists.
	double turbulenceSize = 32.0; // Initial size of the turbulence.

	double sineValue = 0.0;
	double distValue = 0.0;
	double xValue = 0.0;
	double yValue = 0.0;

	for (int y=0;y<height;++y)
	{
		for (int x=0;x<width;++x) {
			xValue = (x - width / 2) / double(width);
			yValue = (y - height / 2) / double(height);
			distValue = sqrt(xValue * xValue + yValue * yValue) + turbulencePower * turbulence(double(x),double(y),width,height,turbulenceSize,noise) / 255.0;
			sineValue = 128.0 * fabs(sin(2 * xyPeriod * distValue * M_PI));
			color=std::make_tuple(float((80 + sineValue)/255.0f), float((30 + sineValue)/255.0f), float(30.0f/255.0f));
			image[get_index(x, y, width)]=color;
		}
	}
}

int main()
{
	std::vector<double> noise;
	std::vector<std::tuple<float, float, float>> image;
	std::tuple<float, float, float> color=std::make_tuple(0.0f,0.0f,0.06666666667f);
	int width=256;
	int height=256;

	image.resize(calc_size(width, height));
	std::cout << "Created a vector with the size: " << calc_size(width, height) << std::endl;
	clear_image(image,color,width,height);
	std::cout << "Cleared image with dark blue color." << std::endl;
	// Generate noise
	noise.resize(calc_size(width, height));
	generate_noise(noise,width,height);
	std::cout << "Generated noise." << std::endl;
	// Cloud Texture
	generate_cloud(image,width,height,noise);
	std::cout << "Generated Cloud." << std::endl;
	save_image(image,width,height,"07_cloud_texture.ppm");
	std::cout << "Save Cloud." << std::endl;
	// Marble Texture
	generate_marble(image,width,height,noise);
	std::cout << "Generated Marble." << std::endl;
	save_image(image,width,height,"07_marble_texture.ppm");
	std::cout << "Save Marble." << std::endl;
	// Wood Texture
	generate_wood(image,width,height,noise);
	std::cout << "Generated Wood." << std::endl;
	save_image(image,width,height,"07_wood_texture.ppm");
	std::cout << "Save Wood." << std::endl;

	return 0;
}