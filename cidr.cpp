/********************************
 * Project: Cidr                *
 * File: cidr.cpp               *
 * Date: 10.8.2020              *
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

void Cidr::Renderer::DrawLine(Cidr::RGBA color, int x1, int y1, int x2, int y2, bool AA, bool GC) {
	// calculate delta lengths
	int dx {x2 - x1}; 
	int dy {y2 - y1};

	// get largest component (x or y) for stepping 
	int biggest {std::max(std::abs(dx), std::abs(dy))};
	// loop variables
	float x { static_cast<float>(x1) };
	float y { static_cast<float>(y1) };
	// calculate how far each component should move every step in the loop
	float stepX = dx / static_cast<float>(biggest);
	float stepY = dy / static_cast<float>(biggest);
	
	// Anti aliasing disabled
	if(!AA) {
		
		for(int i {0}; i < biggest; i++) {
			// Plot point in current location 
			DrawPoint(color, std::round(x), std::round(y));
			// step further in line 
			x += stepX;
			y += stepY;
		}
	}
	// Anti aliasing enabled
	else {
		for(int i {0}; i < biggest; i++) {
			// line is steep
			if(biggest == std::abs(dy)) {
				// calculate alpha values for points that are close to the line 
				uint8_t alpha1 = color.a * (x - static_cast<int>(x));		
				uint8_t alpha2 = color.a * (1 - (x - static_cast<int>(x)));
				// use gamma correction
				if(GC) {
					DrawPoint(alphaBlendColorGammaCorrected(GetPixel(x + 1, y), color, alpha1), x + 1, y);
					DrawPoint(alphaBlendColorGammaCorrected(GetPixel(x, y), color, alpha2), x, y);
				}
				else {
					DrawPoint(alphaBlendColor(GetPixel(x + 1, y), color, alpha1), x + 1, y);
					DrawPoint(alphaBlendColor(GetPixel(x, y), color, alpha2), x, y);
				}
			}
			// line is shallow
			else {
				// calculate alpha values for points that are close to the line 
				uint8_t alpha1 = color.a * (y - static_cast<int>(y));
				uint8_t alpha2 = color.a * (1 - (y - static_cast<int>(y)));
				if(GC) {
					DrawPoint(alphaBlendColorGammaCorrected(GetPixel(x, y + 1), color, alpha1), x, y + 1);
					DrawPoint(alphaBlendColorGammaCorrected(GetPixel(x, y), color, alpha2), x, y);
				}
				else {
					DrawPoint(alphaBlendColor(GetPixel(x, y + 1), color, alpha1), x, y + 1);
					DrawPoint(alphaBlendColor(GetPixel(x, y), color, alpha2), x, y);
				}
			}
			
			// step further in line 
			x += stepX;
			y += stepY;
		}
	}
}
