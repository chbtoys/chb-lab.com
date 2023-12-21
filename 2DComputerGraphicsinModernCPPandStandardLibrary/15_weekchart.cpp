// Compile: clear && clang++ -std=c++20 15_weekchart.cpp -o 15_weekchart
#define _USE_MATH_DEFINES

#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

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
			image[get_index(x,y,width)]=std::make_tuple(0.1254901961, 0.1294117647, 0.1411764706);
		}
	}
}

void draw_filled_circle(int x, int y, int radius, std::vector<std::tuple<float, float, float>> &image, std::tuple<float, float, float> &color, int width, int height)
{
	int x0 = 0;
	int y0 = radius;
	int d = 3 - 2 * radius;
	if (!radius) return;

	auto drawline = [&](int sx, int ex, int ny)
	{
		for (int i = sx; i <= ex; i++)
			image[get_index(i, ny, width)]=color;
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

void get_all_ys(std::vector<int> &ys,std::vector<std::tuple<int,int>> &coords)
{
	for (auto& c : coords) {
		ys.push_back(std::get<1>(c));
	}

	sort(ys.begin(), ys.end());
	ys.erase( unique( ys.begin(), ys.end() ), ys.end() );
}

void draw_line_coords(int x1, int y1, int x2, int y2,std::vector<std::tuple<int,int>> &coords)
{
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1; dy = y2 - y1;
	if (dx == 0)
	{
		if (y2 < y1) std::swap(y1, y2);
		for (y = y1; y <= y2; y++)
			coords.push_back(std::make_tuple(x1,y));
		return;
	}
	if (dy == 0)
	{
		if (x2 < x1) std::swap(x1, x2);
		for (x = x1; x <= x2; x++)
			coords.push_back(std::make_tuple(x,y1));
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
		coords.push_back(std::make_tuple(x,y));
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
			coords.push_back(std::make_tuple(x,y));
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
		coords.push_back(std::make_tuple(x,y));
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
			coords.push_back(std::make_tuple(x,y));
		}
	}
}

void draw_wedge(int x_cen,int y_cen,int rad,int start_ang,int end_ang,std::vector<std::tuple<float, float, float>> &image,std::tuple<float, float, float> &color, int width, int height)
{
	std::vector<std::tuple<int,int>> coords;

	float ang=(((start_ang<=end_ang)?start_ang:end_ang)*(M_PI/180));
	float range=(((end_ang>start_ang)?end_ang:start_ang)*(M_PI/180));
	float x=(rad*cos(ang));
	float y=(rad*sin(ang));
	do
	{
		coords.push_back(std::make_tuple((int)(x_cen+x+0.5),(int)(y_cen-y+0.5)));
		ang+=0.001;
		x=(rad*cos(ang));
		y=(rad*sin(ang));
	}
	while(ang<=range);

	std::tuple<int,int> co1=coords.front();
	std::tuple<int,int> co2=coords.back();

	draw_line_coords(x_cen,y_cen,std::get<0>(co1),std::get<1>(co1),coords);
	draw_line_coords(x_cen,y_cen,std::get<0>(co2),std::get<1>(co2),coords);

	for (auto & e : coords) {
		image[get_index(std::get<0>(e),std::get<1>(e),width)]=color;
	}
}

void draw_filled_wedge(int x_cen,int y_cen,int rad,int start_ang,int end_ang,std::vector<std::tuple<float, float, float>> &image,std::tuple<float, float, float> &color, int width, int height)
{
	std::vector<std::tuple<int,int>> coords;

	float ang=(((start_ang<=end_ang)?start_ang:end_ang)*(M_PI/180));
	float range=(((end_ang>start_ang)?end_ang:start_ang)*(M_PI/180));
	float x=(rad*cos(ang));
	float y=(rad*sin(ang));
	do
	{
		coords.push_back(std::make_tuple((int)(x_cen+x+0.5),(int)(y_cen-y+0.5)));
		ang+=0.001;
		x=(rad*cos(ang));
		y=(rad*sin(ang));
	}
	while(ang<=range);

	std::tuple<int,int> co1=coords.front();
	std::tuple<int,int> co2=coords.back();

	draw_line_coords(x_cen,y_cen,std::get<0>(co1),std::get<1>(co1),coords);
	draw_line_coords(x_cen,y_cen,std::get<0>(co2),std::get<1>(co2),coords);

	std::vector<int> ys;
	std::vector<int> xs;
	get_all_ys(ys,coords);
	std::vector<std::tuple<int,int,int,int>> lines;

	for (int search=0;search<=ys.size();++search)
	{
		for (auto& c : coords) {
			if (std::get<1>(c) == ys[search]) {
				xs.push_back(std::get<0>(c));
			}
		}
		sort(xs.begin(), xs.end());
		lines.push_back(std::make_tuple(xs.front(),ys[search],xs.back(),ys[search]));
		xs.clear();
	}

	auto drawline = [&](int sx, int ex, int ny)
	{
		for (int i = sx; i <= ex; i++)
			image[get_index(i, ny, width)]=color;
	};

	for (auto& l : lines) {
		drawline(std::get<0>(l),std::get<2>(l),std::get<1>(l));
	}
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

int main()
{
	int width=1920;
	int height=1080;

	float a1[] = {0.89,1.00,0.86,0.84,0.80,0.87,0.86,0.92,0.82,0.91,0.85,0.80,1.00,0.87,0.85,0.93,0.97,0.94,1.00,0.91,0.85,0.92,0.96,0.82,0.94,0.83,0.93,0.95,1.00,0.98,0.96,0.97,0.80,1.00,0.93,0.86,0.90,0.93,0.92,0.90,0.92,0.98,0.84,1.00,0.92,0.92,0.83,0.89,0.90,0.99,0.97,0.92};
	float a2[] = {0.70,0.51,0.69,0.52,0.56,0.59,0.50,0.68,0.73,0.46,0.40,0.56,0.41,0.54,0.64,0.45,0.56,0.45,0.41,0.60,0.68,0.45,0.65,0.60,0.65,0.66,0.57,0.47,0.47,0.58,0.57,0.48,0.41,0.62,0.50,0.46,0.75,0.50,0.71,0.56,0.57,0.53,0.75,0.45,0.51,0.47,0.71,0.74,0.51,0.72,0.56,0.71};
	float a3[] = {0.21,0.11,0.13,0.21,0.04,0.21,0.24,0.26,0.32,0.28,0.09,0.15,0.19,0.28,0.22,0.25,0.03,0.22,0.18,0.15,0.31,0.08,0.27,0.07,0.08,0.10,0.08,0.23,0.17,0.11,0.34,0.29,0.10,0.09,0.06,0.31,0.32,0.22,0.26,0.21,0.14,0.13,0.08,0.15,0.27,0.04,0.06,0.17,0.10,0.02,0.12,0.17};

	std::vector<std::tuple<float, float, float>> image;
	std::tuple<float,float,float> bg=std::make_tuple(0.3058823529,0.2509803922,0.2274509804);
	std::tuple<float,float,float> color1=std::make_tuple(0.5254901961,0.4588235294,0.3960784314);
	std::tuple<float,float,float> color2=std::make_tuple(0.6352941176,0.6196078431,0.6);
	std::tuple<float,float,float> color3=std::make_tuple(0.8117647059,0.7607843137,0.7058823529);

	image.resize(calc_size(width, height));
	clear_image(image,width,height);

	int x_cen,y_cen,rad;
	double start_ang,end_ang,length,rad_size,color_size,color_num=0.0;
	x_cen=width/2;
	y_cen=height/2;
	rad=y_cen*0.8;
	start_ang=0;
	end_ang=6.9230769231;
	image[get_index(x_cen,y_cen, width)]=color1;
	rad_size=rad/52;
	length=rad;
	color_size=1.0/52;

	draw_filled_circle(x_cen,y_cen,y_cen*0.9,image,bg,width,height);

	for (int i=0;i<52;++i) {
		draw_filled_wedge(x_cen,y_cen,length*a1[i],start_ang,end_ang,image,color1,width,height);
		draw_filled_wedge(x_cen,y_cen,length*a2[i],start_ang,end_ang,image,color2,width,height);
		draw_filled_wedge(x_cen,y_cen,length*a3[i],start_ang,end_ang,image,color3,width,height);
		start_ang=end_ang;end_ang+=6.9230769231;
	}

	save_image(image,width,height,"15_weekchart.ppm");

	return 0;
}