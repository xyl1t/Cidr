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
void Cidr::Renderer::Clear(const RGBA& color) {
	std::fill(pixels, pixels + width * height, color.getColor());
}

void Cidr::Renderer::DrawPoint(const Cidr::RGBA& color, const Point& p) {
	pixels[getIndex(p.x, p.y)] = color.getColor();
}

void Cidr::Renderer::DrawLine(const Cidr::RGBA& color, const Point& start, const Point& end, bool AA, bool GC) {
	// calculate delta lengths
	int dx {end.x - start.x}; 
	int dy {end.y - start.y};

	// get largest component (x or y) for stepping 
	int biggest {std::max(std::abs(dx), std::abs(dy))};
	// p is the point that runs along the line 
	FPoint p { static_cast<float>(start.x), static_cast<float>(start.y) };
	// calculate how far each component should move every step in the loop
	FPoint step {dx / static_cast<float>(biggest), dy / static_cast<float>(biggest)};
	
	// Anti aliasing disabled
	if(!AA) {
		
		for(int i {0}; i < biggest; i++) {
			// Plot point in current location 
			DrawPoint(color, std::round(p.x), std::round(p.y));
			// step further in line 
			p += step;
		}
	}
	// Anti aliasing enabled
	else {
		for(int i {0}; i < biggest; i++) {
			// line is steep
			if(biggest == std::abs(dy)) {
				// calculate alpha values for points that are close to the line 
				uint8_t alpha1 = color.a * (p.x - static_cast<int>(p.x));		
				uint8_t alpha2 = color.a * (1 - (p.x - static_cast<int>(p.x)));
				// use gamma correction
				if(GC) {
					DrawPoint(alphaBlendColorGammaCorrected(GetPixel(p.x + 1, p.y), color, alpha1), p.x + 1, p.y);
					DrawPoint(alphaBlendColorGammaCorrected(GetPixel(p.x,     p.y), color, alpha2), p.x,     p.y);
				}
				else {
					DrawPoint(alphaBlendColor(GetPixel(p.x + 1, p.y), color, alpha1), p.x + 1, p.y);
					DrawPoint(alphaBlendColor(GetPixel(p.x,     p.y), color, alpha2), p.x,     p.y); // 14
				}
			}
			// line is shallow
			else {
				// calculate alpha values for points that are close to the line 
				uint8_t alpha1 = color.a * (p.y - static_cast<int>(p.y));
				uint8_t alpha2 = color.a * (1 - (p.y - static_cast<int>(p.y)));
				if(GC) {
					DrawPoint(alphaBlendColorGammaCorrected(GetPixel(p.x, p.y + 1), color, alpha1), p.x, p.y + 1);
					DrawPoint(alphaBlendColorGammaCorrected(GetPixel(p.x, p.y),     color, alpha2), p.x, p.y);
				}
				else {
					DrawPoint(alphaBlendColor(GetPixel(p.x, p.y + 1), color, alpha1), p.x, p.y + 1);
					DrawPoint(alphaBlendColor(GetPixel(p.x, p.y),     color, alpha2), p.x, p.y);
				}
			}
			
			// step further in line 
			p += step;
		}
	}
}