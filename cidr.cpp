/********************************
 * Project: Cidr				*
 * File: cidr.cpp				*
 * Date: 10.8.2020				*
 ********************************/

#include "cidr.hpp"
#include <cstring>
#include <algorithm>
#include <iterator>
#include <cmath>

static inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

Cidr::Renderer::Renderer(uint32_t* pixels, size_t width, size_t height) 
	: pixels{pixels}, 
	width{width}, 
	height{height} {
	
}

void Cidr::Renderer::Clear() {
	std::fill(pixels, pixels + width * height, 0x000000ff);
}
void Cidr::Renderer::Clear(RGBA color) {
	std::fill(pixels, pixels + width * height, color.getColor());
}
void Cidr::Renderer::Clear(uint32_t color) {
	std::fill(pixels, pixels + width * height, color);
}

void Cidr::Renderer::DrawPoint(Cidr::RGBA color, int x, int y) {
	pixels[getIndex(x, y)] = color.getColor();
}
void Cidr::Renderer::DrawPoint(uint32_t color, int x, int y) {
	pixels[getIndex(x, y)] = color;
}
	

void Cidr::Renderer::DrawLine(Cidr::RGBA color, int x1, int y1, int x2, int y2, bool AA) {
	// calculate delta lengths
	int dx {x2 - x1}; 
	int dy {y2 - y1};
	
	// Anti aliasing disabled
	if(!AA) {
		int biggest {std::max(std::abs(dx), std::abs(dy))};
		float x;
		float y;
		for(int i {0}; i < biggest; i++) {
			float t {i / static_cast<float>(biggest)};
			x = std::round(lerp(x1, x2, t));
			y = std::round(lerp(y1, y2, t));
			DrawPoint(color, x, y);
		}
	}
	// Anti aliasing enabled
	else {
		bool isSteep = std::abs(y2 - y1) > std::abs(x2 - x1);
		// steep
		if(isSteep) {
			if(y1 > y2) {
				std::swap(x1, x2);
				std::swap(y1, y2);
			}
			float m = dy == 0 ? 1 : static_cast<float>(dx) / dy;
			float x = x1;
			for (int y = y1; y < y2; y++, x += m) {
				RGBA c1 { color.getRGB(), static_cast<uint8_t>(color.a * (x - static_cast<int>(x)))};
				RGBA c2 { color.getRGB(), static_cast<uint8_t>(color.a * (1.f - (x - static_cast<int>(x))))};
				DrawPoint(c2, x, y);
				DrawPoint(c1, x+1, y);
			}
		}
		// shallow
		else {
			if(x1 > x2) {
				std::swap(x1, x2);
				std::swap(y1, y2);
			}
			float m = dx == 0 ? 1 : static_cast<float>(dy) / dx;
			float y = y1;
			for (int x = x1; x < x2; x++, y += m) {
				RGBA c1 { color.getRGB(), static_cast<uint8_t>(color.a * (y - static_cast<int>(y)))};
				RGBA c2 { color.getRGB(), static_cast<uint8_t>(color.a * (1.f - (y - static_cast<int>(y))))};
				DrawPoint(c2, x, y);
				DrawPoint(c1, x, y + 1);
			}
		}
	}
}


/*
if(x2 < x1) {
			std::swap(x1, x2);
			std::swap(y1, y2);
		}
		
		float dx = x2 - x1;
		float dy = y2 - y1;
		float gradient = dy / dx;
		
		// first point
		float xend = std::round(x1);
		float yend = y1 + gradient * (xend - x1);
		float xgap = 1 - ((x1 + 0.5) - static_cast<int>(x1 + 0.5));
		float xpxl1 = xend;
		float ypxl1 = static_cast<int>(yend);
		DrawPoint({color.getRGB(), static_cast<int>(255*(1 - (yend - static_cast<int>(yend))) * xgap)}, xpxl1, ypxl1);
		DrawPoint({color.getRGB(), static_cast<int>(255*(yend - static_cast<int>(yend)) * xgap)}, xpxl1, ypxl1 + 1);
		float intery = yend + gradient; // first y for loop
		
		xend = std::round(x2);
		yend = y2 + gradient * (xend - x2);
		xgap = (x1 + 0.5) - static_cast<int>(x1 + 0.5);
		float xpxl2 = xend;
		float ypxl2 = static_cast<int>(yend);
		DrawPoint({color.getRGB(), static_cast<int>(255*(1 - (yend - static_cast<int>(yend))) * xgap)}, xpxl2, ypxl2);
		DrawPoint({color.getRGB(), static_cast<int>(255*(yend - static_cast<int>(yend)) * xgap)}, xpxl2, ypxl2 + 1);
		
		for (int x = xpxl1 + 1; x < xpxl2 - 1; x++) {
			DrawPoint({color.getRGB(), static_cast<int>(255*(1 - (intery - (int)intery)))}, x, (int)intery);
			DrawPoint({color.getRGB(), static_cast<int>(255*(intery - (int)intery))}, x, (int)intery + 1);
			intery += gradient;
		}
*/