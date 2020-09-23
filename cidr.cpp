/********************************
 * Project: Cidr                *
 * File: cidr.cpp               *
 * Date: 10.9.2020              *
 ********************************/

#include "cidr.hpp"
#include <cstring>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <vector>

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

void Cidr::Renderer::DrawRectangle(const RGBA& color, const Point& location, int width, int height) {
		// exit if the rectangle is outside of the screen
	if(location.x >= this->width) return;
	if(location.y >= this->height) return;
	
	// clamp locations
	Point clampedLocation {location.x, location.y};
	if(location.x < 0) {
		width -= std::abs(clampedLocation.x);
		clampedLocation.x = 0;
		// exit function if rectangle is outside of screen
		if(width < 0) 
			return;
	}
	if(location.y < 0) {
		height -= std::abs(clampedLocation.y);
		clampedLocation.y = 0;
		// exit function if rectangle is outside of screen
		if(height < 0) 
			return;
	}
	int clampedWidth {std::min(this->width - clampedLocation.x, width)};
	int clampedHeight {std::min(this->height - clampedLocation.y, height)};
	
	// loop till the itrating value hits the rectangle end or if it goes outside of the screen
	for(int i = clampedLocation.x; i < clampedLocation.x + clampedWidth; i++) {
		if(location.y > 0)
			DrawPoint(color, i, clampedLocation.y);
		if(clampedLocation.y + height < this->height)
			DrawPoint(color, i, clampedLocation.y + height);
	}
	// loop till the itrating value hits the rectangle end or if it goes outside of the screen
	for(int i = clampedLocation.y; i < clampedLocation.y + clampedHeight; i++) {
		if(location.x > 0)
			DrawPoint(color, clampedLocation.x, i);
		if(clampedLocation.x + width < this->width)
			DrawPoint(color, clampedLocation.x + width, i);
	}
}
void Cidr::Renderer::FillRectangle(const RGBA& color, const Point& location, int width, int height) {
	// exit if the rectangle is outside of the screen
	if(location.x >= this->width) return;
	if(location.y >= this->height) return;
	
	// clamp locations
	Point clampedLocation {location.x, location.y};
	if(location.x < 0) {
		width -= std::abs(clampedLocation.x);
		clampedLocation.x = 0;
		// exit function if rectangle is outside of screen
		if(width < 0) 
			return;
	}
	if(location.y < 0) {
		height -= std::abs(clampedLocation.y);
		clampedLocation.y = 0;
		// exit function if rectangle is outside of screen
		if(height < 0) 
			return;
	}
	int clampedWidth {std::min(this->width - clampedLocation.x, width)};
	int clampedHeight {std::min(this->height - clampedLocation.y, height)};
	for(int i = 0; i < clampedHeight; i++) {
		std::fill_n(pixels + getIndex(clampedLocation.x, clampedLocation.y + i), clampedWidth, RGBtoUINT(color));
	}
}
void Cidr::Renderer::FillRectangle(RGBA (*shader)(const Renderer& renderer, int x, int y), const Point& location, int width, int height) {
	// exit if the rectangle is outside of the screen
	if(location.x >= this->width) return;
	if(location.y >= this->height) return;
	
	// clamp locations
	Point clampedLocation {location.x, location.y};
	if(location.x < 0) {
		width -= std::abs(clampedLocation.x);
		clampedLocation.x = 0;
		// exit function if rectangle is outside of screen
		if(width < 0) 
			return;
	}
	if(location.y < 0) {
		height -= std::abs(clampedLocation.y);
		clampedLocation.y = 0;
		// exit function if rectangle is outside of screen
		if(height < 0) 
			return;
	}
	int clampedWidth {std::min(this->width - clampedLocation.x, width)};
	int clampedHeight {std::min(this->height - clampedLocation.y, height)};
	
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

// TODO: implement AA if possible
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
			else if(AA && i == (int)-x + centreLocation.x) {
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

void Cidr::Renderer::DrawTriangle(const RGBA& color, const Point& p1, const Point& p2, const Point& p3, bool AA, bool GC) {
	DrawLine(color, p1, p2, AA, GC);
	DrawLine(color, p2, p3, AA, GC);
	DrawLine(color, p3, p1, AA, GC);
}

void Cidr::Renderer::FillTriangle(const RGBA& color, Point p1, Point p2, Point p3, bool AA) {
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

	// TEST 
	DrawPoint(0xff0000ff, p1);
	DrawPoint(0x00ff00ff, p2);
	DrawPoint(0x0000ffff, p3);
	uint32_t invertedColor = ((~RGBtoUINT(color) >> 8) << 8) + 0xff;
	
	// Start at top point
	float x1 = p1.x;
	float x2 = p1.x; 
	// Slopes
	float step1 = (p3.x - p1.x) / (float)(p3.y - p1.y);
	float step2 = (p2.x - p1.x) / (float)(p2.y - p1.y);
	/* FIRST TRIANGLE */
	for	(int i = p1.y; i < p2.y; i++) {
		DrawLine(color, x1, i, x2, i);
		DrawPoint(invertedColor, x1, i);
		DrawPoint(invertedColor, x2, i);
		x1 += step1;
		x2 += step2;
	}
	
	// Set point to second part/half of the triangle
	x2 = p2.x;
	// Recalculate step
	step2 = (p3.x - p2.x) / (float)(p3.y - p2.y);
	/* SECOND TRIANGLE */
	for	(int i = p2.y; i < p3.y; i++) {
		DrawLine(color, x1, i, x2, i);
		DrawPoint(invertedColor, x1, i);
		DrawPoint(invertedColor, x2, i);
		x1 += step1;
		x2 += step2;
	}
}
