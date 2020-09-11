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
		float x {static_cast<float>(x1)};
		float y {static_cast<float>(y1)};
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
		// check if line is steep or shallow
		bool isSteep = std::abs(y2 - y1) > std::abs(x2 - x1);
		std::cout << "dx: " << dx << '\n';
		std::cout << "dy: " << dy << '\n';
		float m; // slope 
		// starting positions
		float x {static_cast<float>(x1)}; 
		float y {static_cast<float>(y1)};
		
		// steep line
		if(isSteep) {
			std::cout << "steep\n";
			m = static_cast<float>(dx) / dy; 
			// iterate through the line
			while(y < y2) {
				if(static_cast<int>(y) - y >= 0.5) {
					float alphaValue1 = (x - static_cast<int>(x));
					float alphaValue2 = 1 - alphaValue1;
					DrawPoint({color.getRGB(), static_cast<int>(color.a * alphaValue1)}, x, y);
					DrawPoint({color.getRGB(), static_cast<int>(color.a * alphaValue2)}, x + 1, y);
				}
				else {
					float alphaValue1 = (x - static_cast<int>(x));
					float alphaValue2 = 1 - alphaValue1;
					DrawPoint({color.getRGB(), static_cast<int>(color.a * alphaValue1)}, x, y);
					DrawPoint({color.getRGB(), static_cast<int>(color.a * alphaValue2)}, x - 1, y);
				}
				x += m;
				y += 1;
			}
		}
		// shallow line
		else {
			std::cout << "shallow\n";
			m = static_cast<float>(dy) / dx; 
			// iterate through the line
			while(x < x2) {
				if(static_cast<int>(y) - y >= 0.5) {
					float alphaValue1 = (y - static_cast<int>(y));
					float alphaValue2 = 1 - alphaValue1;
					DrawPoint({color.getRGB(), static_cast<int>(color.a * alphaValue1)}, x, y);
					DrawPoint({color.getRGB(), static_cast<int>(color.a * alphaValue2)}, x, y + 1);
				}
				else {
					float alphaValue1 = (y - static_cast<int>(y));
					float alphaValue2 = 1 - alphaValue1;
					DrawPoint({color.getRGB(), static_cast<int>(color.a * alphaValue1)}, x, y);
					DrawPoint({color.getRGB(), static_cast<int>(color.a * alphaValue2)}, x, y - 1);
				}
				x += 1;
				y += m;
			}
		}



*/