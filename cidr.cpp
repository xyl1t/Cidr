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
	memset(pixels, 0, width * height * sizeof(uint32_t));
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
				// calculate anti aliasing values for points that are close to the line 
				float AAValue1 = (p.x - static_cast<int>(p.x));
				float AAValue2 = (1 - (p.x - static_cast<int>(p.x)));
				// use gamma correction
				if(GC) {
					RGBA result1 {alphaBlendColorGammaCorrected(GetPixel(p.x + 1, p.y), color, AAValue1 * 255.f)};
					RGBA result2 {alphaBlendColorGammaCorrected(GetPixel(p.x, p.y),     color, AAValue2 * 255.f)};
					result1.a = result1.a * color.a / 255.f;
					result2.a = result2.a * color.a / 255.f;
					DrawPoint(alphaBlendColor(GetPixel(p.x + 1, p.y), result1, result1.a), p.x + 1, p.y);
					DrawPoint(alphaBlendColor(GetPixel(p.x, p.y),     result2, result2.a), p.x,     p.y);
				}
				else {
					DrawPoint(alphaBlendColor(GetPixel(p.x + 1, p.y), color, AAValue1 * color.a), p.x + 1, p.y);
					DrawPoint(alphaBlendColor(GetPixel(p.x,     p.y), color, AAValue2 * color.a), p.x,     p.y); 
				}
			}
			// line is shallow
			else {
				// calculate anti aliasing values for points that are close to the line 
				float AAValue1 = (p.y - static_cast<int>(p.y));
				float AAValue2 = (1 - (p.y - static_cast<int>(p.y)));
				// use gamma correction
				if(GC) {
					RGBA result1 {alphaBlendColorGammaCorrected(GetPixel(p.x, p.y + 1), color, AAValue1 * 255.f)};
					RGBA result2 {alphaBlendColorGammaCorrected(GetPixel(p.x, p.y),     color, AAValue2 * 255.f)};
					result1.a = result1.a * color.a / 255.f;
					result2.a = result2.a * color.a / 255.f;
					DrawPoint(alphaBlendColor(GetPixel(p.x, p.y + 1), result1, result1.a), p.x, p.y + 1);
					DrawPoint(alphaBlendColor(GetPixel(p.x, p.y),     result2, result2.a), p.x, p.y);
				}
				else {
					DrawPoint(alphaBlendColor(GetPixel(p.x, p.y + 1), color, AAValue1 * color.a), p.x, p.y + 1);
					DrawPoint(alphaBlendColor(GetPixel(p.x, p.y),     color, AAValue2 * color.a), p.x, p.y);
				}
			}
			
			// step further in line 
			p += step;
		}
	}
}