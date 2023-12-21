// Compile: clear && clang++ -std=c++20 17_colab_peacock.cpp -o 17_colab_peacock
#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

bool startsWithCaseInsensitive(std::string mainStr, std::string toMatch)
{
	// Convert mainStr to lower case
	std::transform(mainStr.begin(), mainStr.end(), mainStr.begin(), ::tolower);
	// Convert toMatch to lower case
	std::transform(toMatch.begin(), toMatch.end(), toMatch.begin(), ::tolower);
	if(mainStr.find(toMatch) == 0)
		return true;
	else
		return false;
}

int get_index(int x, int y, int width)
{
	return x+width*y;
}

int calc_size(int width, int height)
{
	return width*height;
}

void clear_image(std::vector<std::tuple<float, float, float>> &image, int width, int height)
{
	for (int y=0;y<height;++y) {
		for (int x=0;x<width;++x) {
			image[get_index(x,y,width)]=std::make_tuple(0.0f, 0.0f, 0.0f);
		}
	}
}

void generate_noise_image(std::vector<std::tuple<float, float, float>> &image, int width, int height)
{
	std::tuple<float, float, float> color;
	float rndnum=0.0f;

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	for (int y=0;y<height;++y) {
		for (int x=0;x<width;++x) {
			rndnum=dist(mt);
			color=std::make_tuple(rndnum,rndnum,rndnum);
			image[get_index(x,y,width)]=color;
		}
	}
}

void generate_linear_gradient_image(std::vector<std::tuple<float, float, float>> &image, std::tuple<float, float, float> &fcolor, std::tuple<float, float, float> &tcolor, int width, int height)
{
	std::tuple<float, float, float> color;
	double resultRed=0.0f;
	double resultGreen=0.0f;
	double resultBlue=0.0f;
	double fr,fg,fb,tr,tg,tb;
	double percent=0.0f;
	std::tie(fr,fg,fb)=fcolor;
	std::tie(tr,tg,tb)=tcolor;

	for (int y=0;y<height;++y) {
		percent=double(y)/double(height);
		resultRed = fr + percent * (tr - fr);
		resultGreen = fg + percent * (tg - fg);
		resultBlue = fb + percent * (tb - fb);
		color=std::make_tuple(resultRed,resultGreen,resultBlue);
		for (int x=0;x<width;++x) {
			image[get_index(x,y,width)]=color;
		}
	}
}

void blended_two_images(std::vector<std::tuple<float, float, float>> &blend1,std::vector<std::tuple<float, float, float>> &blend2,int width,int height,float alpha)
{
	std::tuple<float, float, float> color1=std::make_tuple(0.0f,0.0f,0.0f);
	std::tuple<float, float, float> color2=std::make_tuple(0.0f,0.0f,0.0f);
	float r=0.0f; float g=0.0f; float b=0.0f;

	for (int y=0;y<height;++y) {
		for (int x=0;x<width;++x) {
			color1=blend1[get_index(x,y,width)];
			color2=blend2[get_index(x,y,width)];
			r = (std::get<0>(color2) * alpha) + (std::get<0>(color1) * (1.0f - alpha));
			g = (std::get<1>(color2) * alpha) + (std::get<1>(color1) * (1.0f - alpha));
			b = (std::get<2>(color2) * alpha) + (std::get<2>(color1) * (1.0f - alpha));
			blend1[get_index(x,y,width)]=std::make_tuple(r, g, b);
		}
	}
}

void draw_line(std::vector<std::tuple<float, float, float>> &image, std::tuple<int, int, int, int> &coords, std::tuple<float,float,float> &color, int width, int height)
{
	int x1=std::get<0>(coords); int y1=std::get<1>(coords); int x2=std::get<2>(coords); int y2=std::get<3>(coords);
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1; dy = y2 - y1;
	if (dx == 0)
	{
		if (y2 < y1) std::swap(y1, y2);
		for (y = y1; y <= y2; y++)
			image[get_index(x1,y,width)]=color;
		return;
	}
	if (dy == 0)
	{
		if (x2 < x1) std::swap(x1, x2);
		for (x = x1; x <= x2; x++)
			image[get_index(x,y1,width)]=color;
		return;
	}
	dx1 = abs(dx); dy1 = abs(dy);
	px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1; y = y1; xe = x2;
		}
		else
		{
			x = x2; y = y2; xe = x1;
		}
		image[get_index(x,y,width)]=color;
		for (i = 0; x<xe; i++)
		{
			x = x + 1;
			if (px<0)
				px = px + 2 * dy1;
			else
			{
				if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			image[get_index(x,y,width)]=color;
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1; y = y1; ye = y2;
		}
		else
		{
			x = x2; y = y2; ye = y1;
		}
		image[get_index(x,y,width)]=color;
		for (i = 0; y<ye; i++)
		{
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else
			{
				if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}
			image[get_index(x,y,width)]=color;
		}
	}
}

int find_region(int x, int y, int width, int height)
{
	int code=0;
	if(y >= height)
	code |= 1; //top
	else if(y < 0)
	code |= 2; //bottom
	if(x >= width)
	code |= 4; //right
	else if (x < 0)
	code |= 8; //left
	return(code);
}

bool clip_line(std::tuple<int, int, int, int> &coords1, std::tuple<int, int, int, int> &coords2, int width, int height)
{
	int x1=std::get<0>(coords1); int y1=std::get<1>(coords1); int x2=std::get<2>(coords1); int y2=std::get<3>(coords1);
	int x3=0; int y3=0; int x4=0; int y4=0;
	int code1=0, code2=0, codeout=0;
	bool accept = 0, done=0;
	code1 = find_region(x1, y1, width, height); //the region outcodes for the endpoints
	code2 = find_region(x2, y2, width, height);
	do //In theory, this can never end up in an infinite loop, it'll always come in one of the trivial cases eventually
	{
		if(!(code1 | code2)) accept = done = 1;  //accept because both endpoints are in screen or on the border, trivial accept
		else if(code1 & code2) done = 1; //the line isn't visible on screen, trivial reject
		else  //if no trivial reject or accept, continue the loop
		{
			int x, y;
			codeout = code1 ? code1 : code2;
			if(codeout & 1) //top
			{
				x = x1 + (x2 - x1) * (height - y1) / (y2 - y1);
				y = height - 1;
			}
			else if(codeout & 2) //bottom
			{
				x = x1 + (x2 - x1) * -y1 / (y2 - y1);
				y = 0;
			}
			else if(codeout & 4) //right
			{
				y = y1 + (y2 - y1) * (width - x1) / (x2 - x1);
				x = width - 1;
			}
			else //left
			{
				y = y1 + (y2 - y1) * -x1 / (x2 - x1);
				x = 0;
			}
			if(codeout == code1) //first endpoint was clipped
			{
				x1 = x; y1 = y;
				code1 = find_region(x1, y1, width, height);
			}
			else //second endpoint was clipped
			{
				x2 = x; y2 = y;
				code2 = find_region(x2, y2, width, height);
			}
		}
	}
	while(done == 0);
	if(accept)
	{
		x3 = x1;
		x4 = x2;
		y3 = y1;
		y4 = y2;
		coords2=std::make_tuple(x3,y3,x4,y4);
		return 1;
	}
	else
	{
		x3 = x4 = y3 = y4 = 0;
		coords2=std::make_tuple(x3,y3,x4,y4);
		return 0;
	}
}

void get_angle_line(std::tuple<int, int, int, int> &coords,int x_cen,int y_cen,double degrees,int length)
{
	double angle = degrees * (M_PI / 180);
	coords=std::make_tuple(x_cen,y_cen,int(double(x_cen) + cos(angle)*double(length)),int(double(y_cen) + sin(angle)*double(length)));
}

void save_image(std::vector<std::tuple<float, float, float>> &image, int width, int height, std::string filename)
{
	std::tuple<float, float, float> color;
	std::ofstream out(filename, std::ios_base::out | std::ios_base::binary);
	out << "P6" << std::endl << width << ' ' << height << std::endl << "255" << std::endl;

	for (int i=0;i<(width*height);++i)
	{
		color=image[i];
		out << char(std::get<0>(color)*255.0f) << char(std::get<1>(color)*255.0f) << char(std::get<2>(color)*255.0f);
	}
	out.close();
}

void draw(std::vector<std::tuple<float, float, float>> &image,std::tuple<int, int, int, int> &coords,std::vector<std::tuple<float,float,float>> &colors,int width,int height,int x_cen,int y_cen,double degrees,int length,int color)
{
	get_angle_line(coords,x_cen,y_cen,degrees,length);
	if (clip_line(coords,coords,width,height)) {
		draw_line(image,coords,colors[color],width,height);
	}
	std::cout << "L " << std::get<0>(coords) << " " << std::get<1>(coords) << " " << std::get<2>(coords) << " " << std::get<3>(coords) << " " << color << std::endl;
}

void parse_draw_line_from_file(const std::string str,std::vector<std::tuple<float, float, float>> &image,std::vector<std::tuple<float,float,float>> &colors,int width,int height)
{
	size_t start;
	size_t end = 0;
	std::vector<std::string> out;

	while ((start = str.find_first_not_of(' ', end)) != std::string::npos)
	{
		end = str.find(' ', start);
		out.push_back(str.substr(start, end - start));
	}

	std::tuple<int, int, int, int> coords=std::make_tuple(stoi(out[1]),stoi(out[2]),stoi(out[3]),stoi(out[4]));
	draw_line(image,coords,colors[stoi(out[5])],width,height);
}

void parse_draw_marker_from_file(const std::string str,std::vector<std::tuple<float, float, float>> &image,std::vector<std::tuple<float,float,float>> &colors,int width,int height)
{
	size_t start;
	size_t end = 0;
	std::vector<std::string> out;

	while ((start = str.find_first_not_of(' ', end)) != std::string::npos)
	{
		end = str.find(' ', start);
		out.push_back(str.substr(start, end - start));
	}

	int x=stoi(out[1]),y=stoi(out[2]),color=stoi(out[3]);

	// Draw Marker
	image[get_index(x-1,y-1,width)]=colors[color];
	image[get_index(x,y-1,width)]=colors[color];
	image[get_index(x+1,y-1,width)]=colors[color];
	image[get_index(x+1,y,width)]=colors[color];
	image[get_index(x+1,y+1,width)]=colors[color];
	image[get_index(x,y+1,width)]=colors[color];
	image[get_index(x-1,y+1,width)]=colors[color];
	image[get_index(x-1,y,width)]=colors[color];
	image[get_index(x,y,width)]=colors[color];
}

void parse_draw_spot_from_file(const std::string str,std::vector<std::tuple<float, float, float>> &image,std::vector<std::tuple<float,float,float>> &colors,int width,int height)
{
	size_t start;
	size_t end = 0;
	int radius=5;
	std::vector<std::string> out;

	while ((start = str.find_first_not_of(' ', end)) != std::string::npos)
	{
		end = str.find(' ', start);
		out.push_back(str.substr(start, end - start));
	}

	int x=stoi(out[1]),y=stoi(out[2]),color=stoi(out[3]);	

	int x0 = 0;
	int y0 = radius;
	int d = 3 - 2 * radius;
	if (!radius) return;

	auto drawline = [&](int sx, int ex, int ny)
	{
		for (int i = sx; i <= ex; i++)
			image[get_index(i, ny, width)]=colors[color];
	};

	while (y0 >= x0)
	{
		drawline(x - x0, x + x0, y - y0);
		drawline(x - y0, x + y0, y - x0);
		drawline(x - x0, x + x0, y + y0);
		drawline(x - y0, x + y0, y + x0);
		if (d < 0) d += 4 * x0++ + 6;
		else d += 4 * (x0++ - y0--) + 10;
	}
}

int main()
{
	std::vector<std::tuple<float, float, float>> image;
	std::vector<std::tuple<float, float, float>> image2;
	std::tuple<int, int, int, int> coords;
	std::vector<std::tuple<float,float,float>> colors;
	colors.push_back(std::make_tuple(0.5f,0.5f,0.5f));// Medium Grey
	colors.push_back(std::make_tuple(0.862745098f,0.862745098f,0.862745098f));// Red
	colors.push_back(std::make_tuple(1.0f,1.0f,1.0f));// Black
	colors.push_back(std::make_tuple(0.7f,0.7f,0.7f));// Blue
	colors.push_back(std::make_tuple(0.6f,0.6f,0.6f));// Purple
	colors.push_back(std::make_tuple(0.2f,0.2f,0.2f));// Light Green
	colors.push_back(std::make_tuple(0.1f,0.1f,0.1f));// Green
	colors.push_back(std::make_tuple(0.4f,0.4f,0.4f));// Cyan
	float deg=0.0;
	int len=0,len2=0;

	int width=1600;int height=900;

	image.resize(calc_size(width, height));
	image2.resize(calc_size(width, height));

	generate_noise_image(image, width, height);

	std::tuple<float, float, float> fcolor=std::make_tuple(0.05882352941f,0.0862745098f,0.1254901961f);
	std::tuple<float, float, float> tcolor=std::make_tuple(0.2156862745f,0.3019607843f,0.3490196078f);
	generate_linear_gradient_image(image2,fcolor,tcolor,width,height);

	blended_two_images(image,image2,width,height,0.9);

	// read 17_colab.txt and draw it
	std::ifstream infile("17_colab_peacock.txt");
	std::string line;
	while (getline(infile, line))
	{
		if (startsWithCaseInsensitive(line,"L")) {
			parse_draw_line_from_file(line,image,colors,width,height);
		}
		if (startsWithCaseInsensitive(line,"M")) {
			parse_draw_marker_from_file(line,image,colors,width,height);
		}
		if (startsWithCaseInsensitive(line,"S")) {
			parse_draw_spot_from_file(line,image,colors,width,height);
		}
	}
	infile.close();

	save_image(image,width,height,"17_colab_peacock.ppm");
	return 0;
}