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

Cidr::RGBA Cidr::alphaBlendColor(const Cidr::RGBA& color1, const Cidr::RGBA& color2) {
	return (Cidr::RGBA){ 
		static_cast<uint8_t>(color2.r * (color2.a / 255.f) + color1.r * (1 - color2.a / 255.f)),
		static_cast<uint8_t>(color2.g * (color2.a / 255.f) + color1.g * (1 - color2.a / 255.f)),
		static_cast<uint8_t>(color2.b * (color2.a / 255.f) + color1.b * (1 - color2.a / 255.f)),
		0xff
	};
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
				uint8_t alpha1 = color.a * (x - static_cast<int>(x));		
				uint8_t alpha2 = color.a * (1 - (x - static_cast<int>(x)));
				
				RGBA c1 { color.getRGB(), alpha1 };
				RGBA c2 { color.getRGB(), alpha2 };
				DrawPoint(alphaBlendColor(GetPixel(x + 1, y), c1), x + 1, y);
				DrawPoint(alphaBlendColor(GetPixel(x, y), c2), x, y);
			}
			// line is shallow
			else {
				uint8_t alpha1 = color.a * (y - static_cast<int>(y));
				uint8_t alpha2 = color.a * (1 - (y - static_cast<int>(y)));
				
				RGBA c1 { color.getRGB(), alpha1 };
				RGBA c2 { color.getRGB(), alpha2 };
				DrawPoint(alphaBlendColor(GetPixel(x, y + 1), c1), x, y + 1);
				DrawPoint(alphaBlendColor(GetPixel(x, y), c2), x, y);
			}
			
			// step further in line 
			x += stepX;
			y += stepY;
		}
	}
}
