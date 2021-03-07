/********************************
 * Project: Cidr                *
 * File: renderer.cpp           *
 * Date: 10.9.2020              *
 ********************************/

#include "renderer.hpp"
#include <cstring>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <vector>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#endif

static inline float lerp(float a, float b, float t) {
	return a + (b - a) * t;
}

Cidr::Renderer::Renderer(uint32_t* pixels, int width, int height) 
	: pixels{pixels}, 
	width{width}, 
	height{height} {
}

void Cidr::Renderer::Clear() {
	memset(pixels, 0, width * height * sizeof(uint32_t));
}
void Cidr::Renderer::Clear(const RGBA& color) {
	std::fill(pixels, pixels + width * height, RGBtoUINT(color));
}
void Cidr::Renderer::Clear(uint32_t color) {
	std::fill(pixels, pixels + width * height, color);
}

void Cidr::Renderer::DrawPoint(const Cidr::RGBA& color, const Point& p) {
	if(color.a != 0)
		pixels[getIndex(p.x, p.y)] = RGBtoUINT(color);
}

// TODO: Add clipping
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

void Cidr::Renderer::DrawRectangle(const RGBA& color, Rectangle rectangle) {
	// exit if the rectangle is outside of the screen
	if(rectangle.x >= this->width) return;
	if(rectangle.y >= this->height) return;
	if(rectangle.x + rectangle.width - 1 < 0) return;
	if(rectangle.y + rectangle.height - 1 < 0) return;
	
	// clamp locations
	Point clampedLocation {rectangle.x, rectangle.y};
	if(rectangle.x < 0) {
		rectangle.width -= std::abs(clampedLocation.x);
		clampedLocation.x = 0;
		// exit function if rectangle is outside of screen
		if(rectangle.width < 0) 
			return;
	}
	if(rectangle.y < 0) {
		rectangle.height -= std::abs(clampedLocation.y);
		clampedLocation.y = 0;
		// exit function if rectangle is outside of screen
		if(rectangle.height < 0) 
			return;
	}
	int clampedWidth {std::min(this->width - clampedLocation.x, rectangle.width)};
	int clampedHeight {std::min(this->height - clampedLocation.y, rectangle.height)};
	
	// loop till the itrating value hits the rectangle end or if it goes outside of the screen
	for(int i = clampedLocation.x + 1; i < clampedLocation.x + clampedWidth; i++) {
		// top side
		if(rectangle.y >= 0)
			DrawPoint(color, i, clampedLocation.y);
		// bottom side
		if(clampedLocation.y + rectangle.height - 1 < this->height)
			DrawPoint(color, i, clampedLocation.y + rectangle.height - 1);
	}
	// loop till the itrating value hits the rectangle end or if it goes outside of the screen
	for(int i = clampedLocation.y; i < clampedLocation.y + clampedHeight; i++) {
		// left side
		if(rectangle.x >= 0)
			DrawPoint(color, clampedLocation.x, i);
		// right side
		if(clampedLocation.x + rectangle.width - 1< this->width)
			DrawPoint(color, clampedLocation.x + rectangle.width - 1, i);
	}
}
void Cidr::Renderer::FillRectangle(const RGBA& color, Rectangle rectangle) {
	// exit if the rectangle is outside of the screen
	if(rectangle.x >= this->width) return;
	if(rectangle.y >= this->height) return;
	
	// clamp locations
	Point clampedLocation {rectangle.x, rectangle.y};
	if(rectangle.x < 0) {
		rectangle.width -= std::abs(clampedLocation.x);
		clampedLocation.x = 0;
		// exit function if rectangle is outside of screen
		if(rectangle.width < 0) 
			return;
	}
	if(rectangle.y < 0) {
		rectangle.height -= std::abs(clampedLocation.y);
		clampedLocation.y = 0;
		// exit function if rectangle is outside of screen
		if(rectangle.height < 0) 
			return;
	}
	int clampedWidth {std::min(this->width - clampedLocation.x, rectangle.width)};
	int clampedHeight {std::min(this->height - clampedLocation.y, rectangle.height)};
	for(int i = 0; i < clampedHeight; i++) {
		std::fill_n(pixels + getIndex(clampedLocation.x, clampedLocation.y + i), clampedWidth, RGBtoUINT(color));
	}
}
void Cidr::Renderer::FillRectangle(RGBA (*shader)(const Renderer& renderer, int x, int y), Rectangle rectangle) {
	// exit if the rectangle is outside of the screen
	if(rectangle.x >= this->width) return;
	if(rectangle.y >= this->height) return;
	
	// clamp locations
	Point clampedLocation {rectangle.x, rectangle.y};
	if(rectangle.x < 0) {
		rectangle.width -= std::abs(clampedLocation.x);
		clampedLocation.x = 0;
		// exit function if rectangle is outside of screen
		if(rectangle.width < 0) 
			return;
	}
	if(rectangle.y < 0) {
		rectangle.height -= std::abs(clampedLocation.y);
		clampedLocation.y = 0;
		// exit function if rectangle is outside of screen
		if(rectangle.height < 0) 
			return;
	}
	int clampedWidth {std::min(this->width - clampedLocation.x, rectangle.width)};
	int clampedHeight {std::min(this->height - clampedLocation.y, rectangle.height)};
	
	std::vector<std::vector<uint32_t>> shadedPixels{};
	for (int y = clampedLocation.y; y < clampedLocation.y + clampedHeight; y++) {
		shadedPixels.push_back((std::vector<uint32_t>){});
		for (int x = clampedLocation.x; x < clampedLocation.x + clampedWidth; x++) {
			shadedPixels[y - clampedLocation.y].push_back(RGBtoUINT(shader(*this, x, y)));
		}
	}
	
	for (int y = clampedLocation.y; y <	 clampedLocation.y + clampedHeight; y++) {
		memcpy(pixels + getIndex(clampedLocation.x, y), shadedPixels[y - clampedLocation.y].data(), clampedWidth * sizeof(uint32_t));
	}
}

void Cidr::Renderer::DrawCircle(const RGBA& color, const Point& centreLocation, int radius, bool AA) {
	if(radius < 1) return;
	if(radius == 1) DrawPoint(color, centreLocation);
	
	float x{static_cast<float>(radius)};
	float y{};
	
	while((int)x > (int)y) {
		x = sqrt(x * x - 2 * y - 1);
		
		if(!AA) {
			DrawPoint(color, (int) x + centreLocation.x, (int) y + centreLocation.y);	
			DrawPoint(color, (int)-x + centreLocation.x, (int) y + centreLocation.y);
			DrawPoint(color, (int) x + centreLocation.x, (int)-y + centreLocation.y);
			DrawPoint(color, (int)-x + centreLocation.x, (int)-y + centreLocation.y);
			DrawPoint(color, (int) y + centreLocation.x, (int) x + centreLocation.y);
			DrawPoint(color, (int)-y + centreLocation.x, (int) x + centreLocation.y);
			DrawPoint(color, (int) y + centreLocation.x, (int)-x + centreLocation.y);
			DrawPoint(color, (int)-y + centreLocation.x, (int)-x + centreLocation.y);
		}
		else {
			float AAValue1 = 255 * (x - static_cast<int>(x));
			float AAValue2 = 255 * (1 - (x - static_cast<int>(x)));
			
			DrawPoint(alphaBlendColor(GetPixel((int) x + centreLocation.x, (int) y + centreLocation.y), color, AAValue1), (int)x + centreLocation.x, (int)y + centreLocation.y);
			DrawPoint(alphaBlendColor(GetPixel((int) x + centreLocation.x - 1, (int) y + centreLocation.y), color, AAValue2), (int) x + centreLocation.x - 1, (int) y + centreLocation.y);
			
			DrawPoint(alphaBlendColor(GetPixel((int)-x + centreLocation.x, (int) y + centreLocation.y), color, AAValue1), (int)-x + centreLocation.x, (int) y + centreLocation.y);
			DrawPoint(alphaBlendColor(GetPixel((int)-x + centreLocation.x + 1, (int) y + centreLocation.y), color, AAValue2), (int)-x + centreLocation.x + 1, (int) y + centreLocation.y);
			
			DrawPoint(alphaBlendColor(GetPixel((int) x + centreLocation.x, (int)-y + centreLocation.y), color, AAValue1), (int) x + centreLocation.x, (int)-y + centreLocation.y);
			DrawPoint(alphaBlendColor(GetPixel((int) x + centreLocation.x - 1, (int)-y + centreLocation.y), color, AAValue2), (int) x + centreLocation.x - 1, (int)-y + centreLocation.y);
			
			DrawPoint(alphaBlendColor(GetPixel((int)-x + centreLocation.x, (int)-y + centreLocation.y), color, AAValue1), (int)-x + centreLocation.x, (int)-y + centreLocation.y);
			DrawPoint(alphaBlendColor(GetPixel((int)-x + centreLocation.x + 1, (int)-y + centreLocation.y), color, AAValue2), (int)-x + centreLocation.x + 1, (int)-y + centreLocation.y);
			
			DrawPoint(alphaBlendColor(GetPixel((int) y + centreLocation.x, (int) x + centreLocation.y), color, AAValue1), (int) y + centreLocation.x, (int) x + centreLocation.y);
			DrawPoint(alphaBlendColor(GetPixel((int) y + centreLocation.x, (int) x + centreLocation.y - 1), color, AAValue2), (int) y + centreLocation.x, (int) x + centreLocation.y - 1);
			
			DrawPoint(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int) x + centreLocation.y), color, AAValue1), (int)-y + centreLocation.x, (int) x + centreLocation.y);
			DrawPoint(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int) x + centreLocation.y - 1), color, AAValue2), (int)-y + centreLocation.x, (int) x + centreLocation.y - 1);
			
			DrawPoint(alphaBlendColor(GetPixel((int) y + centreLocation.x, (int)-x + centreLocation.y), color, AAValue1), (int) y + centreLocation.x, (int)-x + centreLocation.y);
			DrawPoint(alphaBlendColor(GetPixel((int) y + centreLocation.x, (int)-x + centreLocation.y + 1), color, AAValue2), (int) y + centreLocation.x, (int)-x + centreLocation.y + 1);
			
			DrawPoint(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)-x + centreLocation.y), color, AAValue1), (int)-y + centreLocation.x, (int)-x + centreLocation.y);
			DrawPoint(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)-x + centreLocation.y + 1), color, AAValue2), (int)-y + centreLocation.x, (int)-x + centreLocation.y + 1);
		}
		y++;
	}
}
void Cidr::Renderer::FillCircle(const RGBA& color, const Point& centreLocation, int radius, bool AA) {
	if(radius < 1) return;
	if(radius == 1) DrawPoint(color, centreLocation);
	
	float x{static_cast<float>(radius)};
	float y{};
	
	while((int) x > 0) {
		x = sqrt(x * x - 2 * y - 1);
		
		for(int i = (int)-x + centreLocation.x; i < (int)x + centreLocation.x; i++) {
			if(!AA || (i != (int)-x + centreLocation.x)){
				DrawPoint(color, i, (int)y + centreLocation.y);
				DrawPoint(color, i, (int)-y + centreLocation.y);
			}
			else {
				float AAValue1 = 255 * (x - static_cast<int>(x));
				float AAValue2 = 255 * (1 - (x - static_cast<int>(x)));
				
				DrawPoint(alphaBlendColor(GetPixel((int)-x + centreLocation.x, (int)y + centreLocation.y), color, AAValue1), (int)-x + centreLocation.x, (int)y + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)-x + centreLocation.x + 1, (int)y + centreLocation.y), color, AAValue2), (int)-x + centreLocation.x + 1, (int)y + centreLocation.y);
				
				DrawPoint(alphaBlendColor(GetPixel((int)x + centreLocation.x, (int)y + centreLocation.y), color, AAValue1), (int)x + centreLocation.x, (int)y + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)x + centreLocation.x - 1, (int)y + centreLocation.y), color, AAValue2), (int)x + centreLocation.x - 1, (int)y + centreLocation.y);
				
				DrawPoint(alphaBlendColor(GetPixel((int)x + centreLocation.x, (int)-y + centreLocation.y), color, AAValue1), (int)x + centreLocation.x, (int)-y + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)x + centreLocation.x - 1, (int)-y + centreLocation.y), color, AAValue2), (int)x + centreLocation.x - 1, (int)-y + centreLocation.y);

				DrawPoint(alphaBlendColor(GetPixel((int)-x + centreLocation.x, (int)-y + centreLocation.y), color, AAValue1), (int)-x + centreLocation.x, (int)-y + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)-x + centreLocation.x + 1, (int)-y + centreLocation.y), color, AAValue2), (int)-x + centreLocation.x + 1, (int)-y + centreLocation.y);
				
				DrawPoint(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)x + centreLocation.y), color, AAValue1), (int)-y + centreLocation.x, (int)x + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)x + centreLocation.y - 1), color, AAValue2), (int)-y + centreLocation.x, (int)x + centreLocation.y - 1);
				
				DrawPoint(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)x + centreLocation.y), color, AAValue1), (int)y + centreLocation.x, (int)x + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)x + centreLocation.y - 1), color, AAValue2), (int)y + centreLocation.x, (int)x + centreLocation.y - 1);

				DrawPoint(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)-x + centreLocation.y), color, AAValue1), (int)-y + centreLocation.x, (int)-x + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)-x + centreLocation.y + 1), color, AAValue2), (int)-y + centreLocation.x, (int)-x + centreLocation.y + 1);
								
				DrawPoint(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)-x + centreLocation.y), color, AAValue1), (int)y + centreLocation.x, (int)-x + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)-x + centreLocation.y + 1), color, AAValue2), (int)y + centreLocation.x, (int)-x + centreLocation.y + 1);
			}
		}
		
		y++;
	}
}
void Cidr::Renderer::FillCircle(RGBA (*shader)(const Renderer& renderer, int x, int y), const Point& centreLocation, int radius, bool AA) {
	if(radius < 1) return;
	if(radius == 1) DrawPoint(shader(*this, centreLocation.x, centreLocation.y), centreLocation);
	
	float x{static_cast<float>(radius)};
	float y{};
	
	std::vector<std::vector<uint32_t>> shadedPixels{};
	shadedPixels.resize(radius * 2);
	for (int i = 0; i < radius * 2; i++) {
		shadedPixels[i].resize(radius * 2);
	}
	for (int i = 0; i < radius * 2; i++) {
		for (int j = 0; j < radius * 2; j++) {
			if((i-radius) * (i-radius) + (j-radius) * (j-radius) <= radius*radius)
			shadedPixels[i][j] = RGBtoUINT(shader(*this, i - radius + centreLocation.x, j- radius + centreLocation.y));
		}
	}		
	
	while((int) x > 0) {
		x = sqrt(x * x - 2 * y - 1);
		shadedPixels.push_back({});	
		
		for(int i = (int)-x + centreLocation.x; i < (int)x + centreLocation.x; i++) {
			if(!AA || (i != (int)-x + centreLocation.x)) {
				int ni = i - centreLocation.x + radius;
				DrawPoint(shadedPixels[ni][y + radius], i, (int)y + centreLocation.y);
				DrawPoint(shadedPixels[ni][-y + radius], i, (int)-y + centreLocation.y);
			}
			else  {
				float AAValue1 = 255 * (x - static_cast<int>(x));
				float AAValue2 = 255 * (1 - (x - static_cast<int>(x)));
				
				DrawPoint(alphaBlendColor(GetPixel((int)-x + centreLocation.x, (int)y + centreLocation.y), shadedPixels[(int)-x + radius][y + radius], AAValue1), (int)-x + centreLocation.x, (int)y + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)-x + centreLocation.x + 1, (int)y + centreLocation.y), shadedPixels[(int)-x + 1 + radius][y + radius], AAValue2), (int)-x + centreLocation.x + 1, (int)y + centreLocation.y);
				
				DrawPoint(alphaBlendColor(GetPixel((int)x + centreLocation.x, (int)y + centreLocation.y), shadedPixels[(int)x + radius][y + radius], AAValue1), (int)x + centreLocation.x, (int)y + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)x + centreLocation.x - 1, (int)y + centreLocation.y), shadedPixels[(int)x - 1 + radius][y + radius], AAValue2), (int)x + centreLocation.x - 1, (int)y + centreLocation.y);
				
				DrawPoint(alphaBlendColor(GetPixel((int)x + centreLocation.x, (int)-y + centreLocation.y), shadedPixels[(int)x + radius][-y + radius], AAValue1), (int)x + centreLocation.x, (int)-y + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)x + centreLocation.x - 1, (int)-y + centreLocation.y), shadedPixels[(int)x - 1 + radius][-y + radius], AAValue2), (int)x + centreLocation.x - 1, (int)-y + centreLocation.y);

				DrawPoint(alphaBlendColor(GetPixel((int)-x + centreLocation.x, (int)-y + centreLocation.y), shadedPixels[(int)-x + radius][-y + radius], AAValue1), (int)-x + centreLocation.x, (int)-y + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)-x + centreLocation.x + 1, (int)-y + centreLocation.y), shadedPixels[(int)-x + 1 + radius][-y + radius], AAValue2), (int)-x + centreLocation.x + 1, (int)-y + centreLocation.y);
				
				DrawPoint(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)x + centreLocation.y), shadedPixels[-y + radius][(int)x + radius], AAValue1), (int)-y + centreLocation.x, (int)x + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)x + centreLocation.y - 1), shadedPixels[-y + radius][(int)x + radius - 1], AAValue2), (int)-y + centreLocation.x, (int)x + centreLocation.y - 1);
				
				DrawPoint(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)x + centreLocation.y), shadedPixels[y + radius][(int)x + radius], AAValue1), (int)y + centreLocation.x, (int)x + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)x + centreLocation.y - 1), shadedPixels[y + radius][(int)x + radius - 1], AAValue2), (int)y + centreLocation.x, (int)x + centreLocation.y - 1);

				DrawPoint(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)-x + centreLocation.y), shadedPixels[-y + radius][(int)-x + radius], AAValue1), (int)-y + centreLocation.x, (int)-x + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)-x + centreLocation.y + 1), shadedPixels[-y + radius][(int)-x + radius + 1], AAValue2), (int)-y + centreLocation.x, (int)-x + centreLocation.y + 1);
								
				DrawPoint(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)-x + centreLocation.y), shadedPixels[y + radius][(int)-x + radius], AAValue1), (int)y + centreLocation.x, (int)-x + centreLocation.y);
				DrawPoint(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)-x + centreLocation.y + 1), shadedPixels[y + radius][(int)-x + radius + 1], AAValue2), (int)y + centreLocation.x, (int)-x + centreLocation.y + 1);
			}
		}
		
		y++;
	}
}

void Cidr::Renderer::DrawTriangle(const RGBA& color, const Point& p1, const Point& p2, const Point& p3, bool AA, bool GC) {
	DrawLine(color, p1, p2, AA, GC);
	DrawLine(color, p2, p3, AA, GC);
	DrawLine(color, p3, p1, AA, GC);
}
void Cidr::Renderer::DrawTriangle(const Bitmap& texture, FPoint tp1, FPoint tp2, FPoint tp3, Point p1, Point p2, Point p3) {
	// sort top most point
	if(p1.y > p2.y) {
		std::swap(p1, p2);
		std::swap(tp1, tp2);
	}
	if(p2.y > p3.y) {
		std::swap(p2, p3);
		std::swap(tp2, tp3);
	}
	if(p1.y > p2.y) {
		std::swap(p1, p2);
		std::swap(tp1, tp2);
	}
	
	if(p3.y - p1.y != 0) {
		float x1 = 0;
		float x2 = 0;
		FPoint lerpCoordV1{}; // vertical lerp coord from side 1
		FPoint lerpCoordV2{}; // vertical lerp coord from side 2
		
		for (int i = p1.y; i < p2.y; i++)	{
			float t1 = (i - p1.y) / (float)(p3.y - p1.y);
			float t2 = (i - p1.y) / (float)(p2.y - p1.y);
			x1 = lerp(p1.x, p3.x, t1);
			x2 = lerp(p1.x, p2.x, t2);
			
			lerpCoordV1.x = lerp(tp1.x, tp3.x, t1);
			lerpCoordV1.y = lerp(tp1.y, tp3.y, t1);
			
			lerpCoordV2.x = lerp(tp1.x, tp2.x, t2);
			lerpCoordV2.y = lerp(tp1.y, tp2.y, t2);
			
			if(x1 > x2) {
				std::swap(x1, x2);
				std::swap(lerpCoordV1, lerpCoordV2);
			}
			
			int startX = x1;
			int endX = x2;
			
			float xStep{};
			float yStep{};
			
			if(endX - startX != 0) {
				xStep = (lerpCoordV2.x - lerpCoordV1.x) / (float)(endX - startX);
				yStep = (lerpCoordV2.y - lerpCoordV1.y) / (float)(endX - startX);
			}
				
			float xLerp{lerpCoordV1.x};
			float yLerp{lerpCoordV1.y};
			
			
			for (int j = startX; j < endX; j++) {
				float clampedXLerp {};
				float clampedYLerp {};
				
				// if(lerpCoordV1.x > lerpCoordV2.x) {
				// 	clampedXLerp = ceil(xLerp * (texture.GetWidth() - 1));
				// 	clampedYLerp = ceil(yLerp * (texture.GetHeight() - 1));
				// } else {
					clampedXLerp = xLerp * texture.GetWidth();
					clampedYLerp = yLerp * texture.GetHeight();
				// }
				
				if(!clampCoords(clampedXLerp, clampedYLerp, texture.GetWidth(), texture.GetHeight()) && OutOfBoundsType == OutOfBoundsType::ClampToBorder) {
					DrawPoint(ClampToBorderColor, j, i);
				} else {
					if(ScaleType == ScaleType::Nearest) {
						DrawPoint(texture.GetPixel(clampedXLerp, clampedYLerp), j, i);
					} else {
						clampedXLerp -= 0.5;
						clampedYLerp -= 0.5;
						
						auto clamp = [](float x, int low, int high) -> float {
							if(x < low) return low;
							else if(x > high) return high;
							return x;
						};
						
						clampedXLerp = clamp(clampedXLerp, 0, texture.GetWidth()-1);
						clampedYLerp = clamp(clampedYLerp, 0, texture.GetHeight()-1);
						
						const RGBA& texel_tl = texture.GetPixel(clamp(clampedXLerp,   0, texture.GetWidth()-1), clamp(clampedYLerp,   0, texture.GetHeight()-1));
						const RGBA& texel_tr = texture.GetPixel(clamp(clampedXLerp+1, 0, texture.GetWidth()-1), clamp(clampedYLerp,   0, texture.GetHeight()-1));
						const RGBA& texel_bl = texture.GetPixel(clamp(clampedXLerp,   0, texture.GetWidth()-1), clamp(clampedYLerp+1, 0, texture.GetHeight()-1));
						const RGBA& texel_br = texture.GetPixel(clamp(clampedXLerp+1, 0, texture.GetWidth()-1), clamp(clampedYLerp+1, 0, texture.GetHeight()-1));
						
						float diffx = clampedXLerp - (int)clampedXLerp;
						float diffy = clampedYLerp - (int)clampedYLerp;
						
						RGBA cT { texel_tl * (1 - diffx) + texel_tr * diffx };
						RGBA cB { texel_bl * (1 - diffx) + texel_br * diffx };
						RGBA texel {
							cT * (1 - diffy)  + 
							cB * diffy
						};
						
						DrawPoint(texel, j, i);
					}
				}
				
				xLerp += xStep;
				yLerp += yStep;
			}
		}
		
		for (int i = p2.y; i < p3.y; i++)	{
			float t1 = (i - p1.y) / (float)(p3.y - p1.y);
			float t2 = (i - p2.y) / (float)(p3.y - p2.y);
			x1 = lerp(p1.x, p3.x, t1);
			x2 = lerp(p2.x, p3.x, t2);
			
			lerpCoordV1.x = lerp(tp1.x, tp3.x, t1);
			lerpCoordV1.y = lerp(tp1.y, tp3.y, t1);
			
			lerpCoordV2.x = lerp(tp2.x, tp3.x, t2);
			lerpCoordV2.y = lerp(tp2.y, tp3.y, t2);
			
			if(x1 > x2) {
				std::swap(x1, x2);
				std::swap(lerpCoordV1, lerpCoordV2);
			}
			
			int startX = x1;
			int endX = x2;
			
			float xStep{};
			float yStep{};
			if(endX - startX) {
				xStep = (lerpCoordV2.x - lerpCoordV1.x) / (float)(endX - startX);
				yStep = (lerpCoordV2.y - lerpCoordV1.y) / (float)(endX - startX);
			}
				
			float xLerp{lerpCoordV1.x};
			float yLerp{lerpCoordV1.y};
			
			for (int j = startX; j < endX; j++) {
				float clampedXLerp {};
				float clampedYLerp {};
				
				// if(lerpCoordV1.x > lerpCoordV2.x) {
				// 	clampedXLerp = ceil(xLerp * (texture.GetWidth() - 1));
				// 	clampedYLerp = ceil(yLerp * (texture.GetHeight() - 1));
				// } else {
					clampedXLerp = xLerp * texture.GetWidth();
					clampedYLerp = yLerp * texture.GetHeight();
				// }
				
				if(!clampCoords(clampedXLerp, clampedYLerp, texture.GetWidth(), texture.GetHeight()) && OutOfBoundsType == OutOfBoundsType::ClampToBorder) {
					DrawPoint(ClampToBorderColor, j, i);
				} else {
					if(ScaleType == ScaleType::Nearest) {
						DrawPoint(texture.GetPixel(clampedXLerp, clampedYLerp), j, i);
					} else {
						clampedXLerp -= 0.5;
						clampedYLerp -= 0.5;
						
						auto clamp = [](float x, int low, int high) -> float {
							if(x < low) return low;
							else if(x > high) return high;
							return x;
						};
						
						clampedXLerp = clamp(clampedXLerp, 0, texture.GetWidth() - 1);
						clampedYLerp = clamp(clampedYLerp, 0, texture.GetHeight() - 1);
						
						const RGBA& texel_tl = texture.GetPixel(clamp(clampedXLerp,   0, texture.GetWidth()-1), clamp(clampedYLerp,   0, texture.GetHeight()-1));
						const RGBA& texel_tr = texture.GetPixel(clamp(clampedXLerp+1, 0, texture.GetWidth()-1), clamp(clampedYLerp,   0, texture.GetHeight()-1));
						const RGBA& texel_bl = texture.GetPixel(clamp(clampedXLerp,   0, texture.GetWidth()-1), clamp(clampedYLerp+1, 0, texture.GetHeight()-1));
						const RGBA& texel_br = texture.GetPixel(clamp(clampedXLerp+1, 0, texture.GetWidth()-1), clamp(clampedYLerp+1, 0, texture.GetHeight()-1));
						
						float diffx = clampedXLerp - (int)clampedXLerp;
						float diffy = clampedYLerp - (int)clampedYLerp;
						
						RGBA cT { texel_tl * (1 - diffx) + texel_tr * diffx };
						RGBA cB { texel_bl * (1 - diffx) + texel_br * diffx };
						RGBA texel {
							cT * (1 - diffy)  + 
							cB * diffy
						};
						
						DrawPoint(texel, j, i);
					}
				}


				xLerp += xStep;
				yLerp += yStep;
			}
		}
	}
}
void Cidr::Renderer::FillTriangle(const RGBA& color, Point p1, Point p2, Point p3) {
	// sort top most point
	if(p1.y > p2.y) {
		std::swap(p1, p2);
	}
	if(p2.y > p3.y) {
		std::swap(p2, p3);
	}
	if(p1.y > p2.y) {
		std::swap(p1, p2);
	}

	if(p3.y - p1.y != 0) {
		float x1 = 0;
		float x2 = 0;
		for (int i = p2.y; i >= p1.y; i--)	{
			x1 = lerp(p1.x, p3.x, (i - p1.y) / (float)(p3.y - p1.y));
			x2 = lerp(p1.x, p2.x, (i - p1.y) / (float)(p2.y - p1.y));
			if(x1 > x2) {
				std::swap(x1, x2);
			}
			drawScanLine(RGBtoUINT(color), std::round(x1), std::round(x2), i);
		}
		
		for (int i = p2.y; i < p3.y; i++)	{
			x1 = lerp(p1.x, p3.x, (i - p1.y) / (float)(p3.y - p1.y));
			x2 = lerp(p2.x, p3.x, (i - p2.y) / (float)(p3.y - p2.y));
			if(x1 > x2) {
				std::swap(x1, x2);
			}
			drawScanLine(RGBtoUINT(color), std::round(x1), std::round(x2), i);
		}
	}
}
void Cidr::Renderer::FillTriangle(RGBA color1, RGBA color2, RGBA color3, Point p1, Point p2, Point p3) {
	// sort top most point
	if(p1.y > p2.y) {
		std::swap(p1, p2);
		std::swap(color1, color2);
	}
	if(p2.y > p3.y) {
		std::swap(p2, p3);
		std::swap(color2, color3);
	}
	if(p1.y > p2.y) {
		std::swap(p1, p2);
		std::swap(color1, color2);
	}

	if(p3.y - p1.y != 0) {
		float x1 = 0;
		float x2 = 0;
		RGBA lerpColorV1{}; // vertical lerp color from side 1
		RGBA lerpColorV2{}; // vertical lerp color from side 2
		
		for (int i = p2.y; i >= p1.y; i--)	{
			float t1 = (i - p1.y) / (float)(p3.y - p1.y);
			float t2 = (i - p1.y) / (float)(p2.y - p1.y);
			x1 = lerp(p1.x, p3.x, t1);
			x2 = lerp(p1.x, p2.x, t2);
			
			lerpColorV1.r = static_cast<uint8_t>(lerp(color1.r, color3.r, t1));
			lerpColorV1.g = static_cast<uint8_t>(lerp(color1.g, color3.g, t1));
			lerpColorV1.b = static_cast<uint8_t>(lerp(color1.b, color3.b, t1));
			lerpColorV1.a = static_cast<uint8_t>(lerp(color1.a, color3.a, t1));
			lerpColorV2.r = static_cast<uint8_t>(lerp(color1.r, color2.r, t2));
			lerpColorV2.g = static_cast<uint8_t>(lerp(color1.g, color2.g, t2));
			lerpColorV2.b = static_cast<uint8_t>(lerp(color1.b, color2.b, t2));
			lerpColorV2.a = static_cast<uint8_t>(lerp(color1.a, color2.a, t2));
			
			if(x1 > x2) {
				std::swap(x1, x2);
				std::swap(lerpColorV1, lerpColorV2);
			}
			drawScanLine(lerpColorV1, lerpColorV2, std::round(x1), std::round(x2), i);
		}
		
		for (int i = p2.y; i < p3.y; i++)	{
			float t1 = (i - p1.y) / (float)(p3.y - p1.y);
			float t2 = (i - p2.y) / (float)(p3.y - p2.y);
			x1 = lerp(p1.x, p3.x, t1);
			x2 = lerp(p2.x, p3.x, t2);
			
			lerpColorV1.r = static_cast<uint8_t>(lerp(color1.r, color3.r, t1));
			lerpColorV1.g = static_cast<uint8_t>(lerp(color1.g, color3.g, t1));
			lerpColorV1.b = static_cast<uint8_t>(lerp(color1.b, color3.b, t1));
			lerpColorV1.a = static_cast<uint8_t>(lerp(color1.a, color3.a, t1));
			lerpColorV2.r = static_cast<uint8_t>(lerp(color2.r, color3.r, t2));
			lerpColorV2.g = static_cast<uint8_t>(lerp(color2.g, color3.g, t2));
			lerpColorV2.b = static_cast<uint8_t>(lerp(color2.b, color3.b, t2));
			lerpColorV2.a = static_cast<uint8_t>(lerp(color2.a, color3.a, t2));
			
			if(x1 > x2) {
				std::swap(x1, x2);
				std::swap(lerpColorV1, lerpColorV2);
			}
			drawScanLine(lerpColorV1, lerpColorV2, std::round(x1), std::round(x2), i);
		}
	}
}
void Cidr::Renderer::FillTriangle(RGBA (*shader)(const Renderer& renderer, int x, int y), Point p1, Point p2, Point p3) {
	// sort top most point
	if(p1.y > p2.y) {
		std::swap(p1, p2);
	}
	if(p2.y > p3.y) {
		std::swap(p2, p3);
	}
	if(p1.y > p2.y) {
		std::swap(p1, p2);
	}

	// Shaded pixels for the top triangle
	std::vector<std::vector<uint32_t>> shadedPixelsTopTri{};
	// Shaded pixels for the bottom triangle
	std::vector<std::vector<uint32_t>> shadedPixelsBottomTri{};
	
	if(p3.y - p1.y != 0) {
		float x1 = 0;
		float x2 = 0;

		// Get the shaded pixels of the top triangle
		shadedPixelsTopTri.resize(p2.y - p1.y + 1);
		for (int i = p2.y; i >= p1.y; i--) {
			x1 = lerp(p1.x, p3.x, (i - p1.y) / (float)(p3.y - p1.y));
			x2 = lerp(p1.x, p2.x, (i - p1.y) / (float)(p2.y - p1.y));
			if(x1 > x2) {
				std::swap(x1, x2);
			}
			for (int j = std::round(x1); j < std::round(x2); j++) {				
				shadedPixelsTopTri[i - p1.y].push_back(RGBtoUINT(shader(*this, j, i)));
			}
		}
		
		// Get the shaded pixels of the bottom triangle
		shadedPixelsBottomTri.resize(p3.y - p2.y + 1);
		for (int i = p2.y; i < p3.y; i++) {
			x1 = lerp(p1.x, p3.x, (i - p1.y) / (float)(p3.y - p1.y));
			x2 = lerp(p2.x, p3.x, (i - p2.y) / (float)(p3.y - p2.y));
			if(x1 > x2) {
				std::swap(x1, x2);
			}
			for (int j = std::round(x1); j < std::round(x2); j++) {
				shadedPixelsBottomTri[i - p2.y].push_back(RGBtoUINT(shader(*this, j, i)));
			}
		}
		
		// Copy shaded pixels to render pixels
		for (int i = p2.y; i >= p1.y; i--)	{
			x1 = lerp(p1.x, p3.x, (i - p1.y) / (float)(p3.y - p1.y));
			x2 = lerp(p1.x, p2.x, (i - p1.y) / (float)(p2.y - p1.y));
			if(x1 > x2) {
				std::swap(x1, x2);
			}
			memcpy(pixels + getIndex(std::round(x1), i), 
				shadedPixelsTopTri[i - p1.y].data(), 
				(std::round(x2) - std::round(x1)) * sizeof(uint32_t));
		}
		
		// Get the shaded pixels of the bottom triangle
		for (int i = p2.y; i < p3.y; i++)	{
			x1 = lerp(p1.x, p3.x, (i - p1.y) / (float)(p3.y - p1.y));
			x2 = lerp(p2.x, p3.x, (i - p2.y) / (float)(p3.y - p2.y));
			if(x1 > x2) {
				std::swap(x1, x2);
			}
			memcpy(pixels + getIndex(std::round(x1), i), 
				shadedPixelsBottomTri[i - p2.y].data(), 
				(std::round(x2) - std::round(x1)) * sizeof(uint32_t));
		}
	}
}

void Cidr::Renderer::drawScanLine(uint32_t color, int startX, int endX, int y) {
	// std::fill_n(pixels + getIndex(startX, y), endX - startX, color);
	for(int i = startX; i <= endX; i++) {
		// if(GetPixel(i,y).r)
		DrawPoint(color, i, y);
	}
}
void Cidr::Renderer::drawScanLine(const RGBA& color1, const RGBA& color2, int startX, int endX, int y) {
	float rStep{(color2.r - color1.r) / (float)(endX - startX)};
	float gStep{(color2.g - color1.g) / (float)(endX - startX)};
	float bStep{(color2.b - color1.b) / (float)(endX - startX)};
	float aStep{(color2.a - color1.a) / (float)(endX - startX)};
	
	float rLerp{static_cast<float>(color1.r)};
	float gLerp{static_cast<float>(color1.g)};
	float bLerp{static_cast<float>(color1.b)};
	float aLerp{static_cast<float>(color1.a)};
	
	for (int i = startX; i < endX; i++) {
		DrawPoint({(uint8_t)rLerp, (uint8_t)gLerp, (uint8_t)bLerp, (uint8_t)aLerp}, i, y);
		
		rLerp += rStep;
		gLerp += gStep;
		bLerp += bStep;
		aLerp += aStep;
	}
}
// TODO: fix this mess
void Cidr::Renderer::DrawBitmap(const Bitmap& bitmap, float destX, float destY, int destWidth, int destHeight, float srcX, float srcY, int srcWidth, int srcHeight) {
	// Exit if image is out of bounds of the canvas
	if(destX >= width) return;	
	if(destY >= height) return;
	if(destX + destWidth < 0) return;
	if(destY + destHeight < 0) return;
		
	// optimzation if image has no scale
	if(destWidth == srcWidth && destHeight == srcHeight) {
		/* srcRectangle == destRectangle, I'm only going to use srcRectangle */
		
		if(destX < 0) {
			srcX += std::abs(destX);
			srcWidth -= std::abs(destX);
			destWidth = srcWidth;
			destX = 0;
		}
		if(destX + destWidth >= width) {
			srcWidth += width - (destX + destWidth);;
			destWidth = srcWidth;
		}
		if(destY < 0) {
			srcY += std::abs(destY);
			srcHeight -= std::abs(destY);
			destHeight = srcHeight;
			destY = 0;
		}
		if(destY + destHeight >= height) {
			srcHeight += height - (destY + destHeight);;
			destHeight += srcHeight;
		}
		
		for(int i = srcY; i < srcY + bitmap.GetHeight() - (bitmap.GetHeight() - destHeight); i++) {			
			memcpy(pixels + getIndex(destX, destY + (i - srcY)), 
				bitmap.GetData() + i * bitmap.GetWidth() + (int)srcX, 
				(bitmap.GetWidth() - (bitmap.GetWidth() - srcWidth)) * sizeof(uint32_t)); 
		}
	} else {
		float cx = destWidth / (float)srcWidth;
		float cy = destHeight / (float)srcHeight;
		
		for (int iDest = destX; iDest < destX + destWidth; iDest++) {
			for (int jDest = destY; jDest < destY + destHeight; jDest++) {
				if(iDest < 0 || jDest < 0 || iDest >= GetWidth() || jDest >= GetHeight()) 
					continue;
				
				float iSrc = (iDest - destX) / (float)cx + srcX;
				float jSrc = (jDest - destY) / (float)cy + srcY;

				int fooX = 0;
				int fooY = 0;
				
				float x {iSrc};
				float y {jSrc};
				
				bool isInBounds = clampCoords(x, y, bitmap.GetWidth(), bitmap.GetHeight());
				if(!isInBounds) {
					if(OutOfBoundsType == OutOfBoundsType::MirroredRepeat) {
						fooX = (int)((iSrc + (iSrc < 0 ? 1/(cx*cx) : 0)) / bitmap.GetWidth()) % 2 + (iSrc < 0 ? 1 : 0);
						fooY = (int)((jSrc + (jSrc < 0 ? 1/(cy*cy) : 0)) / bitmap.GetHeight()) % 2 + (jSrc < 0 ? 1 : 0);
					}
					else if(OutOfBoundsType == OutOfBoundsType::ClampToBorder) {
						DrawPoint(ClampToBorderColor, iDest, jDest);
						continue;
					}
				}
				// static uint64_t timer = 0;				
				// if(timer / 100000.f > 1 && iDest == destX && jDest == destY) {
				// 	std::cout << "iSrc: " << iSrc << "; jSrc: " << jSrc << std::endl;
				// 	std::cout << "x: " << x << "; y: " << y << std::endl;
				// 	std::cout << (((int)iSrc / bitmap.GetWidth()) % 2) << std::endl;
				// 	timer = 0;
				// }
				// timer++;
				iSrc = x;
				jSrc = y;

				
				// TODO: maybe simplyify this later 
				if(this->ScaleType == ScaleType::Nearest) {
					if(fooX) 
						iSrc = ceil(iSrc);
					if(fooY) 
						jSrc = ceil(jSrc);
					DrawPoint(bitmap.GetPixel(iSrc, jSrc), iDest, jDest);
				} else { 
					// TODO: cheßck if downscaling works properly
					// NOTE: subtract 0.5 in order to put the point in the centre of the pixel
					
					if(fooX) iSrc += 0.5;
					else 	 iSrc -= 0.5;
					if(fooY) jSrc += 0.5;
					else 	 jSrc -= 0.5;
					
					if(iSrc < 0) iSrc = 0;
					if(iSrc >= bitmap.GetWidth()) iSrc = bitmap.GetWidth() - 1;
					if(jSrc < 0) jSrc = 0;
					if(jSrc >= bitmap.GetHeight()) jSrc = bitmap.GetHeight() - 1;

					float iSrcFraction = (iSrc) - (int)iSrc;
					float jSrcFraction = (jSrc) - (int)jSrc;
					
					const RGBA& colorTL = bitmap.GetPixel(iSrc, jSrc);
					const RGBA& colorBL = bitmap.GetPixel(iSrc, (jSrc+1 >= bitmap.GetHeight() ? jSrc : jSrc + 1));
					const RGBA& colorTR = bitmap.GetPixel((iSrc+1 >= bitmap.GetWidth() ? iSrc : iSrc + 1), jSrc);
					const RGBA& colorBR = bitmap.GetPixel((iSrc+1 >= bitmap.GetWidth() ? iSrc : iSrc + 1), (jSrc+1 >= bitmap.GetHeight() ? jSrc : jSrc + 1));
					
					RGBA cT { colorTL * (1 - iSrcFraction) + colorTR * iSrcFraction };
					RGBA cB { colorBL * (1 - iSrcFraction) + colorBR * iSrcFraction };
					RGBA c {
						cT * (1 - jSrcFraction) + 
						cB * jSrcFraction 
					};
					
					DrawPoint(c, iDest, jDest);
				}
			}
		}
	}
}

void Cidr::Renderer::DrawText(const std::string_view text, int x, int y, const Font& font, const RGBA& fColor, const RGBA& bColor, const RGBA& shadowColor, int shadowOffsetX, int shadowOffsetY) {
	int fontSizeWidth = font.GetFontWidth();
	int fontSizeHeight = font.GetFontHeight();
	int charsRows = font.GetFontSheetWidth() / fontSizeWidth;
	int charsCols = font.GetFontSheetHeight() / fontSizeHeight;
	
	for (int letterCount = 0; letterCount < text.size(); letterCount++) {
		const unsigned char& letter = text[letterCount];
		if(letter < 0 || letter > 255) continue;
		
		int letterX = letter % charsCols;
		int letterY = letter / charsRows;
		
		for (int i = 0; i < fontSizeWidth; i++) {
			for (int j = 0; j < fontSizeHeight; j++) {
				const RGB& letterPixel = font.GetPixel(letterX * fontSizeWidth + i, letterY * fontSizeHeight + j);
				
				int subX = x + letterCount * (fontSizeWidth) + i;
				int subY = y + j;
				
				if(subX >= GetWidth() || subY >= GetHeight() || 
				   subX < 0 || subY < 0)
				   continue;
				
				if(letterPixel == RGB::Black && bColor != RGBA::Transparent && GetPixel(subX, subY) != shadowColor) {
					DrawPoint(bColor, subX, subY);
				} else if(letterPixel == RGB::White){
					DrawPoint(fColor, subX, subY);
					if(font.GetPixel(letterX * fontSizeWidth + i + shadowOffsetX, letterY * fontSizeHeight + j + shadowOffsetY) == RGB::Black) {
						DrawPoint(shadowColor, subX + shadowOffsetX, subY + shadowOffsetY);
					}
				}
			}
		}
	}
}

/* UTILILTY FUNCTIONS */
bool Cidr::Renderer::clampCoords(float& x, float& y, int width, int height) {
	// NOTE: return true if coordinates are in bound of bitmap size
	if(x >= 0 && y >= 0 && x < width && y < height) return true;

	switch(OutOfBoundsType) {
		case OutOfBoundsType::Repeat: {
			x = std::fmod((width * ((int)std::abs(x) / width + 1) + x), width);
			y = std::fmod((height * ((int)std::abs(y) / height + 1) + y), height);
		} break;
		case OutOfBoundsType::MirroredRepeat: {
			const auto mod = [](const float a, const float n) noexcept
			{
				return std::fmod((std::fmod(a, n) + n), n);
			};
			const auto mirror = [](const float a) noexcept
			{
				return a >= 0.f ? a : -(1 + a);
			};
			const auto mirrored_repeat_x = [&](const float x) noexcept {
				return (width - 1) - mirror(mod(x, 2 * width) - width);
			};
			const auto mirrored_repeat_y = [&](const float y) noexcept {
				return (height - 1) - mirror(mod(y, 2 * height) - height);
			};
			
			x = mirrored_repeat_x(x);
			y = mirrored_repeat_y(y);
			
			// HACK: this + 0.001f will be problematic prob later! 
		} break;
		case OutOfBoundsType::ClampToEdge: {
			if(ScaleType == ScaleType::Nearest) {
				x = std::fmax(0, std::fmin(width-1, x));
				y = std::fmax(0, std::fmin(height-1, y));
			} else {
				x = std::fmax(0, std::fmin(width, x));
				y = std::fmax(0, std::fmin(height, y));
			}
		} break;
		
		// NOTE: basically OutOfBoundsType::ClampToBorder case
		default:;
	}
	// NOTE: return false if coordinates are out of bounds of the bitmap	
	return false;
}