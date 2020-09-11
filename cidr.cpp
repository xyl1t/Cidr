/********************************
 * Project: Cidr				*
 * File: cidr.cpp				*
 * Date: 10.8.2020				*
 ********************************/

#include "cidr.hpp"

Cidr::Renderer::Renderer(uint32_t* pixels, size_t width, size_t height) 
	: pixels{pixels}, 
	width{width}, 
	height{height} {
	
}


void Cidr::Renderer::DrawPoint(Cidr::RGBA color, int x, int y) {
	pixels[getIndex(x, y)] = color.getColor();
}
void Cidr::Renderer::DrawPoint(uint32_t color, int x, int y) {
	pixels[getIndex(x, y)] = color;
}
	

void Cidr::Renderer::DrawLine(Cidr::RGBA color, int x1, int y1, int x2, int y2, bool AA) {
	if(x1 > x2 || y1 > y2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}
	int dx {x2 - x1}; 
	int dy {y2 - y1};
	float m { static_cast<float>(dy) / dx }; // slope
	

	// Anti aliasing disabled
	if(!AA) {
		float x {static_cast<float>(x1)}, y {static_cast<float>(y1)};
		const int& greater = dx > dy ? dx : dy;
		float stepX = dx / static_cast<float>(greater);
		float stepY = dy / static_cast<float>(greater);
		
		do {
			DrawPoint(color, x, y);
			x += stepX;
			y += stepY;
		} while(x < x2 && y < y2);
	}
	// Anti aliasing enabled
	else {
		
	}
}
