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
#include <array>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#endif
#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#endif
#include "timer.hpp"

static inline double lerp(double a, double b, double t) {
	return a + t * (b - a);
}

cdr::Renderer::Renderer(uint32_t* pixels, int width, int height) 
	: pixels{pixels}, 
	width{width}, 
	height{height},
	globalX(0), globalY(0) {
}

void cdr::Renderer::Clear() {
	memset(pixels, 0, width * height * sizeof(uint32_t));
	globalX = globalY = 0;
}
void cdr::Renderer::Clear(const RGBA& color) {
	std::fill(pixels, pixels + width * height, RGBtoUINT(color));
	globalX = globalY = 0;
}
void cdr::Renderer::Clear(uint32_t color) {
	std::fill(pixels, pixels + width * height, color);
	globalX = globalY = 0;
}

void cdr::Renderer::DrawPixel(const cdr::RGBA& color, const Point& p) {
	if(color.a != 0)
		pixels[getIndex(p.x, p.y)] = RGBtoUINT(color);
}

// TODO: Add clipping
void cdr::Renderer::DrawLine(const cdr::RGBA& color, const Point& start, const Point& end, bool AA, bool GC) {
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
			DrawPixel(color, std::round(p.x), std::round(p.y));
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
					DrawPixel(alphaBlendColor(GetPixel(p.x + 1, p.y), result1, result1.a), p.x + 1, p.y);
					DrawPixel(alphaBlendColor(GetPixel(p.x, p.y),     result2, result2.a), p.x,     p.y);
				}
				else {
					DrawPixel(alphaBlendColor(GetPixel(p.x + 1, p.y), color, AAValue1 * color.a), p.x + 1, p.y);
					DrawPixel(alphaBlendColor(GetPixel(p.x,     p.y), color, AAValue2 * color.a), p.x,     p.y); 
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
					DrawPixel(alphaBlendColor(GetPixel(p.x, p.y + 1), result1, result1.a), p.x, p.y + 1);
					DrawPixel(alphaBlendColor(GetPixel(p.x, p.y),     result2, result2.a), p.x, p.y);
				}
				else {
					DrawPixel(alphaBlendColor(GetPixel(p.x, p.y + 1), color, AAValue1 * color.a), p.x, p.y + 1);
					DrawPixel(alphaBlendColor(GetPixel(p.x, p.y),     color, AAValue2 * color.a), p.x, p.y);
				}
			}
			
			// step further in line 
			p += step;
		}
	}
}

void cdr::Renderer::DrawRectangle(const RGBA& color, Rectangle rectangle) {
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
			DrawPixel(color, i, clampedLocation.y);
		// bottom side
		if(clampedLocation.y + rectangle.height - 1 < this->height)
			DrawPixel(color, i, clampedLocation.y + rectangle.height - 1);
	}
	// loop till the itrating value hits the rectangle end or if it goes outside of the screen
	for(int i = clampedLocation.y; i < clampedLocation.y + clampedHeight; i++) {
		// left side
		if(rectangle.x >= 0)
			DrawPixel(color, clampedLocation.x, i);
		// right side
		if(clampedLocation.x + rectangle.width - 1< this->width)
			DrawPixel(color, clampedLocation.x + rectangle.width - 1, i);
	}
}
void cdr::Renderer::FillRectangle(const RGBA& color, Rectangle rectangle) {
	// exit if the rectangle is outside of the screen
	if(rectangle.x >= this->width) return;
	if(rectangle.y >= this->height) return;
	
	if(rectangle.width == 1 && rectangle.height == 1) { 
		DrawPixel(color, rectangle.x, rectangle.y);
		return;
	}
	
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
void cdr::Renderer::FillRectangle(RGBA (*shader)(const Renderer& renderer, int x, int y), Rectangle rectangle) {
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
		shadedPixels.push_back(std::vector<uint32_t>{});
		for (int x = clampedLocation.x; x < clampedLocation.x + clampedWidth; x++) {
			shadedPixels[y - clampedLocation.y].push_back(RGBtoUINT(shader(*this, x, y)));
		}
	}
	
	for (int y = clampedLocation.y; y <	 clampedLocation.y + clampedHeight; y++) {
		memcpy(pixels + getIndex(clampedLocation.x, y), shadedPixels[y - clampedLocation.y].data(), clampedWidth * sizeof(uint32_t));
	}
}

void cdr::Renderer::DrawCircle(const RGBA& color, const Point& centreLocation, int radius, bool AA) {
	if(radius < 1) return;
	if(radius == 1) DrawPixel(color, centreLocation);
	
	float x{static_cast<float>(radius)};
	float y{};
	
	while((int)x > (int)y) {
		x = sqrt(x * x - 2 * y - 1);
		
		if(!AA) {
			DrawPixel(color, (int) x + centreLocation.x, (int) y + centreLocation.y);	
			DrawPixel(color, (int)-x + centreLocation.x, (int) y + centreLocation.y);
			DrawPixel(color, (int) x + centreLocation.x, (int)-y + centreLocation.y);
			DrawPixel(color, (int)-x + centreLocation.x, (int)-y + centreLocation.y);
			DrawPixel(color, (int) y + centreLocation.x, (int) x + centreLocation.y);
			DrawPixel(color, (int)-y + centreLocation.x, (int) x + centreLocation.y);
			DrawPixel(color, (int) y + centreLocation.x, (int)-x + centreLocation.y);
			DrawPixel(color, (int)-y + centreLocation.x, (int)-x + centreLocation.y);
		}
		else {
			float AAValue1 = 255 * (x - static_cast<int>(x));
			float AAValue2 = 255 * (1 - (x - static_cast<int>(x)));
			
			DrawPixel(alphaBlendColor(GetPixel((int) x + centreLocation.x, (int) y + centreLocation.y), color, AAValue1), (int)x + centreLocation.x, (int)y + centreLocation.y);
			DrawPixel(alphaBlendColor(GetPixel((int) x + centreLocation.x - 1, (int) y + centreLocation.y), color, AAValue2), (int) x + centreLocation.x - 1, (int) y + centreLocation.y);
			
			DrawPixel(alphaBlendColor(GetPixel((int)-x + centreLocation.x, (int) y + centreLocation.y), color, AAValue1), (int)-x + centreLocation.x, (int) y + centreLocation.y);
			DrawPixel(alphaBlendColor(GetPixel((int)-x + centreLocation.x + 1, (int) y + centreLocation.y), color, AAValue2), (int)-x + centreLocation.x + 1, (int) y + centreLocation.y);
			
			DrawPixel(alphaBlendColor(GetPixel((int) x + centreLocation.x, (int)-y + centreLocation.y), color, AAValue1), (int) x + centreLocation.x, (int)-y + centreLocation.y);
			DrawPixel(alphaBlendColor(GetPixel((int) x + centreLocation.x - 1, (int)-y + centreLocation.y), color, AAValue2), (int) x + centreLocation.x - 1, (int)-y + centreLocation.y);
			
			DrawPixel(alphaBlendColor(GetPixel((int)-x + centreLocation.x, (int)-y + centreLocation.y), color, AAValue1), (int)-x + centreLocation.x, (int)-y + centreLocation.y);
			DrawPixel(alphaBlendColor(GetPixel((int)-x + centreLocation.x + 1, (int)-y + centreLocation.y), color, AAValue2), (int)-x + centreLocation.x + 1, (int)-y + centreLocation.y);
			
			DrawPixel(alphaBlendColor(GetPixel((int) y + centreLocation.x, (int) x + centreLocation.y), color, AAValue1), (int) y + centreLocation.x, (int) x + centreLocation.y);
			DrawPixel(alphaBlendColor(GetPixel((int) y + centreLocation.x, (int) x + centreLocation.y - 1), color, AAValue2), (int) y + centreLocation.x, (int) x + centreLocation.y - 1);
			
			DrawPixel(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int) x + centreLocation.y), color, AAValue1), (int)-y + centreLocation.x, (int) x + centreLocation.y);
			DrawPixel(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int) x + centreLocation.y - 1), color, AAValue2), (int)-y + centreLocation.x, (int) x + centreLocation.y - 1);
			
			DrawPixel(alphaBlendColor(GetPixel((int) y + centreLocation.x, (int)-x + centreLocation.y), color, AAValue1), (int) y + centreLocation.x, (int)-x + centreLocation.y);
			DrawPixel(alphaBlendColor(GetPixel((int) y + centreLocation.x, (int)-x + centreLocation.y + 1), color, AAValue2), (int) y + centreLocation.x, (int)-x + centreLocation.y + 1);
			
			DrawPixel(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)-x + centreLocation.y), color, AAValue1), (int)-y + centreLocation.x, (int)-x + centreLocation.y);
			DrawPixel(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)-x + centreLocation.y + 1), color, AAValue2), (int)-y + centreLocation.x, (int)-x + centreLocation.y + 1);
		}
		y++;
	}
}
void cdr::Renderer::FillCircle(const RGBA& color, const Point& centreLocation, int radius, bool AA) {
	if(radius < 1) return;
	if(radius == 1) DrawPixel(color, centreLocation);
	
	float x{static_cast<float>(radius)};
	float y{};
	
	while((int) x > 0) {
		x = sqrt(x * x - 2 * y - 1);
		
		for(int i = (int)-x + centreLocation.x; i < (int)x + centreLocation.x; i++) {
			if(!AA || (i != (int)-x + centreLocation.x)){
				DrawPixel(color, i, (int)y + centreLocation.y);
				DrawPixel(color, i, (int)-y + centreLocation.y);
			}
			else {
				float AAValue1 = 255 * (x - static_cast<int>(x));
				float AAValue2 = 255 * (1 - (x - static_cast<int>(x)));
				
				DrawPixel(alphaBlendColor(GetPixel((int)-x + centreLocation.x, (int)y + centreLocation.y), color, AAValue1), (int)-x + centreLocation.x, (int)y + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)-x + centreLocation.x + 1, (int)y + centreLocation.y), color, AAValue2), (int)-x + centreLocation.x + 1, (int)y + centreLocation.y);
				
				DrawPixel(alphaBlendColor(GetPixel((int)x + centreLocation.x, (int)y + centreLocation.y), color, AAValue1), (int)x + centreLocation.x, (int)y + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)x + centreLocation.x - 1, (int)y + centreLocation.y), color, AAValue2), (int)x + centreLocation.x - 1, (int)y + centreLocation.y);
				
				DrawPixel(alphaBlendColor(GetPixel((int)x + centreLocation.x, (int)-y + centreLocation.y), color, AAValue1), (int)x + centreLocation.x, (int)-y + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)x + centreLocation.x - 1, (int)-y + centreLocation.y), color, AAValue2), (int)x + centreLocation.x - 1, (int)-y + centreLocation.y);

				DrawPixel(alphaBlendColor(GetPixel((int)-x + centreLocation.x, (int)-y + centreLocation.y), color, AAValue1), (int)-x + centreLocation.x, (int)-y + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)-x + centreLocation.x + 1, (int)-y + centreLocation.y), color, AAValue2), (int)-x + centreLocation.x + 1, (int)-y + centreLocation.y);
				
				DrawPixel(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)x + centreLocation.y), color, AAValue1), (int)-y + centreLocation.x, (int)x + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)x + centreLocation.y - 1), color, AAValue2), (int)-y + centreLocation.x, (int)x + centreLocation.y - 1);
				
				DrawPixel(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)x + centreLocation.y), color, AAValue1), (int)y + centreLocation.x, (int)x + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)x + centreLocation.y - 1), color, AAValue2), (int)y + centreLocation.x, (int)x + centreLocation.y - 1);

				DrawPixel(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)-x + centreLocation.y), color, AAValue1), (int)-y + centreLocation.x, (int)-x + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)-x + centreLocation.y + 1), color, AAValue2), (int)-y + centreLocation.x, (int)-x + centreLocation.y + 1);
								
				DrawPixel(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)-x + centreLocation.y), color, AAValue1), (int)y + centreLocation.x, (int)-x + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)-x + centreLocation.y + 1), color, AAValue2), (int)y + centreLocation.x, (int)-x + centreLocation.y + 1);
			}
		}
		
		y++;
	}
}
void cdr::Renderer::FillCircle(RGBA (*shader)(const Renderer& renderer, int x, int y), const Point& centreLocation, int radius, bool AA) {
	if(radius < 1) return;
	if(radius == 1) DrawPixel(shader(*this, centreLocation.x, centreLocation.y), centreLocation);
	
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
				DrawPixel(shadedPixels[ni][y + radius], i, (int)y + centreLocation.y);
				DrawPixel(shadedPixels[ni][-y + radius], i, (int)-y + centreLocation.y);
			}
			else  {
				float AAValue1 = 255 * (x - static_cast<int>(x));
				float AAValue2 = 255 * (1 - (x - static_cast<int>(x)));
				
				DrawPixel(alphaBlendColor(GetPixel((int)-x + centreLocation.x, (int)y + centreLocation.y), shadedPixels[(int)-x + radius][y + radius], AAValue1), (int)-x + centreLocation.x, (int)y + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)-x + centreLocation.x + 1, (int)y + centreLocation.y), shadedPixels[(int)-x + 1 + radius][y + radius], AAValue2), (int)-x + centreLocation.x + 1, (int)y + centreLocation.y);
				
				DrawPixel(alphaBlendColor(GetPixel((int)x + centreLocation.x, (int)y + centreLocation.y), shadedPixels[(int)x + radius][y + radius], AAValue1), (int)x + centreLocation.x, (int)y + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)x + centreLocation.x - 1, (int)y + centreLocation.y), shadedPixels[(int)x - 1 + radius][y + radius], AAValue2), (int)x + centreLocation.x - 1, (int)y + centreLocation.y);
				
				DrawPixel(alphaBlendColor(GetPixel((int)x + centreLocation.x, (int)-y + centreLocation.y), shadedPixels[(int)x + radius][-y + radius], AAValue1), (int)x + centreLocation.x, (int)-y + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)x + centreLocation.x - 1, (int)-y + centreLocation.y), shadedPixels[(int)x - 1 + radius][-y + radius], AAValue2), (int)x + centreLocation.x - 1, (int)-y + centreLocation.y);

				DrawPixel(alphaBlendColor(GetPixel((int)-x + centreLocation.x, (int)-y + centreLocation.y), shadedPixels[(int)-x + radius][-y + radius], AAValue1), (int)-x + centreLocation.x, (int)-y + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)-x + centreLocation.x + 1, (int)-y + centreLocation.y), shadedPixels[(int)-x + 1 + radius][-y + radius], AAValue2), (int)-x + centreLocation.x + 1, (int)-y + centreLocation.y);
				
				DrawPixel(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)x + centreLocation.y), shadedPixels[-y + radius][(int)x + radius], AAValue1), (int)-y + centreLocation.x, (int)x + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)x + centreLocation.y - 1), shadedPixels[-y + radius][(int)x + radius - 1], AAValue2), (int)-y + centreLocation.x, (int)x + centreLocation.y - 1);
				
				DrawPixel(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)x + centreLocation.y), shadedPixels[y + radius][(int)x + radius], AAValue1), (int)y + centreLocation.x, (int)x + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)x + centreLocation.y - 1), shadedPixels[y + radius][(int)x + radius - 1], AAValue2), (int)y + centreLocation.x, (int)x + centreLocation.y - 1);

				DrawPixel(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)-x + centreLocation.y), shadedPixels[-y + radius][(int)-x + radius], AAValue1), (int)-y + centreLocation.x, (int)-x + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)-y + centreLocation.x, (int)-x + centreLocation.y + 1), shadedPixels[-y + radius][(int)-x + radius + 1], AAValue2), (int)-y + centreLocation.x, (int)-x + centreLocation.y + 1);
								
				DrawPixel(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)-x + centreLocation.y), shadedPixels[y + radius][(int)-x + radius], AAValue1), (int)y + centreLocation.x, (int)-x + centreLocation.y);
				DrawPixel(alphaBlendColor(GetPixel((int)y + centreLocation.x, (int)-x + centreLocation.y + 1), shadedPixels[y + radius][(int)-x + radius + 1], AAValue2), (int)y + centreLocation.x, (int)-x + centreLocation.y + 1);
			}
		}
		
		y++;
	}
}

void cdr::Renderer::DrawTriangle(const RGBA& color, const Point& p1, const Point& p2, const Point& p3, bool AA, bool GC) {
	DrawLine(color, p1, p2, AA, GC);
	DrawLine(color, p2, p3, AA, GC);
	DrawLine(color, p3, p1, AA, GC);
}
struct DPoint {
	double x;
	double y;
} lerpCoordV1, lerpCoordV2;
void cdr::Renderer::DrawTriangle(const Bitmap& texture, FPoint tp1, FPoint tp2, FPoint tp3, FPoint p1, FPoint p2, FPoint p3) {
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
	
	Timer t{};	
	
	tp1.x *= texture.GetWidth();
	tp2.x *= texture.GetWidth();
	tp3.x *= texture.GetWidth();
	tp1.y *= texture.GetHeight();
	tp2.y *= texture.GetHeight();
	tp3.y *= texture.GetHeight();


// #define cdr_barycentric
#ifndef cdr_barycentric
	
	for (int y = std::ceil(p1.y); y < std::ceil(p2.y); y++) {
		double t1 = (y - p1.y) / (double)(p3.y - p1.y);
		double t2 = (y - p1.y) / (double)(p2.y - p1.y);
		
		double min = lerp(p1.x, p3.x, t1);
		double max = lerp(p1.x, p2.x, t2);
		int startX = std::ceil(min);
		int endX = std::ceil(max);
		
		lerpCoordV1.x = lerp(tp1.x, tp3.x, t1);
		lerpCoordV1.y = lerp(tp1.y, tp3.y, t1);
		
		lerpCoordV2.x = lerp(tp1.x, tp2.x, t2);
		lerpCoordV2.y = lerp(tp1.y, tp2.y, t2);
		
		if (min > max) {
			std::swap(startX, endX);
			std::swap(min, max);
			std::swap(lerpCoordV1, lerpCoordV2);
		}
		
		for (int x = startX; x < endX; x++) {
			DrawPixel(sampleTexture(texture, 
				lerp(lerpCoordV1.x, lerpCoordV2.x, (x - min) / (max - min)), 
				lerp(lerpCoordV1.y, lerpCoordV2.y, (x - min) / (max - min))), 
				x, y);
		}
	}
	
	for (int y = std::ceil(p2.y); y < std::ceil(p3.y); y++) {
		double t1 = (y - p1.y) / (double)(p3.y - p1.y);
		double t2 = (y - p2.y) / (double)(p3.y - p2.y);
		
		double min = lerp(p1.x, p3.x, t1);
		double max = lerp(p2.x, p3.x, t2);
		int startX = std::ceil(min);
		int endX = std::ceil(max);

		lerpCoordV1.x = lerp(tp1.x, tp3.x, t1);
		lerpCoordV1.y = lerp(tp1.y, tp3.y, t1);
		
		lerpCoordV2.x = lerp(tp2.x, tp3.x, t2);
		lerpCoordV2.y = lerp(tp2.y, tp3.y, t2);
		
		if (min > max) {
			std::swap(startX, endX);
			std::swap(min, max);
			std::swap(lerpCoordV1, lerpCoordV2);
		}
		
		for (int x = startX; x < endX; x++) {
			DrawPixel(sampleTexture(texture, 
				lerp(lerpCoordV1.x, lerpCoordV2.x, (x - min) / (max - min)), 
				lerp(lerpCoordV1.y, lerpCoordV2.y, (x - min) / (max - min))), 
				x, y);
		}
	}
#else
	float leftStep{};
	if (p3.y - p1.y != 0) {
		leftStep = (p3.x - p1.x) / (p3.y - p1.y);
	}
	float rightStep{};
	if (p2.y - p1.y != 0) {
		rightStep = (p2.x - p1.x) / (p2.y - p1.y);
	}
	
	float left = p1.x + (std::ceil(p1.y) - p1.y) * leftStep;
	float right = p1.x + (std::ceil(p1.y) - p1.y) * rightStep;
	
	// float denom = edgeFunc(p1, p2, p3);// (p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y);
	float denom = (p3.x - p1.x)*(p2.y - p1.y) - (p3.y - p1.y)*(p2.x - p1.x);// (p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y);
	float w1{};
	float w2{};
	float w3{};

	for (int y = std::ceil(p1.y); y < std::ceil(p2.y); y++) {
		int min = std::ceil(left);
		int max = std::ceil(right);
		
		if (min > max) {
			std::swap(min, max);
		}
		
		for (int x = min; x < max; x++) {
			
			// DrawPixel(RGB::Red, x, y);
			
			w1 = ((x - p2.x)*(p3.y - p2.y) - (y - p2.y)*(p3.x - p2.x)) / denom;
			w2 = ((x - p3.x)*(p1.y - p3.y) - (y - p3.y)*(p1.x - p3.x)) / denom;
			w3 = ((x - p1.x)*(p2.y - p1.y) - (y - p1.y)*(p2.x - p1.x)) / denom;
			
			// w1 = ((p2.y - p3.y)*(x - p3.x) + (p3.x - p2.x)*(y - p3.y)) / denom;
			// w2 = ((p3.y - p1.y)*(x - p3.x) + (p1.x - p3.x)*(y - p3.y)) / denom;
			// w3 = 1.f - w1 - w2;
			
			float t = (w1 * tp1.x + w2 * tp2.x + w3 * tp3.x);
			float s = (w1 * tp1.y + w2 * tp2.y + w3 * tp3.y);
			
			DrawPixel(sampleTexture(texture, 
				t, s),
				x, y);
		}
		
		left += leftStep;
		right += rightStep;
	}
	
	if (p3.y - p2.y != 0) {
		rightStep = (p3.x - p2.x) / (p3.y - p2.y);
	}
	// right = p1.x + (std::ceil(p1.y) - p1.y) * rightStep
	right = p2.x + (std::ceil(p2.y) - p2.y) * rightStep;
	
	for (int y = std::ceil(p2.y); y < std::ceil(p3.y); y++) {
		int min = std::ceil(left);
		int max = std::ceil(right);
		
		if (min > max) {
			std::swap(min, max);
		}
		
		for (int x = min; x < max; x++) {
			
			// DrawPixel(RGB::Green, x, y);
			
			// FPoint p {float(x), float(y)};
			w1 = ((x - p2.x)*(p3.y - p2.y) - (y - p2.y)*(p3.x - p2.x)) / denom;
			w2 = ((x - p3.x)*(p1.y - p3.y) - (y - p3.y)*(p1.x - p3.x)) / denom;
			w3 = ((x - p1.x)*(p2.y - p1.y) - (y - p1.y)*(p2.x - p1.x)) / denom;
			
			// w1 = ((p2.y - p3.y)*(x - p3.x) + (p3.x - p2.x)*(y - p3.y)) / denom;
			// w2 = ((p3.y - p1.y)*(x - p3.x) + (p1.x - p3.x)*(y - p3.y)) / denom;
			// w3 = 1.f - w1 - w2;
			
			float t = (w1 * tp1.x + w2 * tp2.x + w3 * tp3.x);
			float s = (w1 * tp1.y + w2 * tp2.y + w3 * tp3.y);
			
			DrawPixel(sampleTexture(texture, 
				t, 
				s),
				x, y);
		}
		
		left += leftStep;
		right += rightStep;
	}
#endif
	
	static int counter = 0;
	static double accumulation = 0;
	counter++;
	accumulation += t.elapsed()*1000;
	
	if (counter > 128) {
		std::cout << "speed: " << accumulation/counter << std::endl;
		counter = 0;
		accumulation = 0;
	}
}
void cdr::Renderer::FillTriangle(const RGBA& color, Point p1, Point p2, Point p3) {
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
void cdr::Renderer::FillTriangle(RGBA color1, RGBA color2, RGBA color3, Point p1, Point p2, Point p3) {
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

	// std::vector<int> scanBuffer((p3.y - p1.y) * 2);
	
	// auto scanConvertLine = [&](FPoint min, FPoint max, int side) {
	// 	float yDist = max.y - min.y;
	// 	float xDist = max.x - min.x;
	// 	if (yDist <= 0) return;
	// 	float xStep = xDist / yDist;
	// 	float curX = min.x;
	// 	for (int i = min.y; i < max.y; i++) {
	// 		scanBuffer[(i - p1.y) * 2 + side] = curX;
	// 		curX += xStep;
	// 	}
	// };
	
	// scanConvertLine(p1, p3, 0);
	// scanConvertLine(p1, p2, 1);
	// scanConvertLine(p2, p3, 1);
	
	// for (int i = 0; i < scanBuffer.size() / 2 - 1; i++) {
	// 	int xMin = scanBuffer[i * 2];
	// 	int xMax = scanBuffer[i * 2 + 1];
	// 	for (int j = xMin; j < xMax; j++) {
	// 		DrawPixel(color1, j, p1.y + i);
	// 	}
	// }

	// return;


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
void cdr::Renderer::FillTriangle(RGBA (*shader)(const Renderer& renderer, int x, int y), Point p1, Point p2, Point p3) {
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

void cdr::Renderer::drawScanLine(uint32_t color, int startX, int endX, int y) {
	// std::fill_n(pixels + getIndex(startX, y), endX - startX, color);
	for(int i = startX; i <= endX; i++) {
		// if(GetPixel(i,y).r)
		DrawPixel(color, i, y);
	}
}
void cdr::Renderer::drawScanLine(const RGBA& color1, const RGBA& color2, int startX, int endX, int y) {
	float rStep{(color2.r - color1.r) / (float)(endX - startX)};
	float gStep{(color2.g - color1.g) / (float)(endX - startX)};
	float bStep{(color2.b - color1.b) / (float)(endX - startX)};
	float aStep{(color2.a - color1.a) / (float)(endX - startX)};
	
	float rLerp{static_cast<float>(color1.r)};
	float gLerp{static_cast<float>(color1.g)};
	float bLerp{static_cast<float>(color1.b)};
	float aLerp{static_cast<float>(color1.a)};
	
	for (int i = startX; i < endX; i++) {
		DrawPixel({(uint8_t)rLerp, (uint8_t)gLerp, (uint8_t)bLerp, (uint8_t)aLerp}, i, y);
		
		rLerp += rStep;
		gLerp += gStep;
		bLerp += bStep;
		aLerp += aStep;
	}
}
// TODO: fix this mess
void cdr::Renderer::DrawBitmap(const Bitmap& bitmap, float destX, float destY, int destWidth, int destHeight, float srcX, float srcY, int srcWidth, int srcHeight) {
	// Exit if image is out of bounds of the canvas
	if(destX >= width) return;	
	if(destY >= height) return;
	if(destX + destWidth < 0) return;
	if(destY + destHeight < 0) return;
		
	// optimzation if image has no scale
	if(destWidth == srcWidth && destHeight == srcHeight && srcX == 0 && srcY == 0 && srcWidth == bitmap.GetWidth() && srcHeight == bitmap.GetHeight()) {
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
				
				DrawPixel(sampleTexture(bitmap, iSrc, jSrc), iDest, jDest);
				continue;

				int fooX = 0;
				int fooY = 0;
				
				float x {iSrc};
				float y {jSrc};
				
				bool isInBounds = clampCoords(x, y, bitmap.GetWidth(), bitmap.GetHeight());
				if(!isInBounds) {
					if(OutOfBoundsType == OutOfBoundsType::MirroredRepeat) {
						fooX = (int)((iSrc + 0.001) / bitmap.GetWidth()) % 2 + (iSrc < 0 ? 1 : 0);
						fooY = (int)((jSrc + 0.001) / bitmap.GetHeight()) % 2 + (jSrc < 0 ? 1 : 0);
					}
					else if(OutOfBoundsType == OutOfBoundsType::ClampToBorder) {
						DrawPixel(ClampToBorderColor, iDest, jDest);
						continue;
					}
				}
				// static uint64_t timer = 0;				
				// if(timer > 100000.f && iDest == destX && jDest == destY) {
				// 	std::cout << "iSrc: " << iSrc << "; jSrc: " << jSrc << std::endl;
				// 	std::cout << "x: " << x << "; y: " << y << std::endl;
				// 	std::cout << "fooX: " << fooX << "; fooY: " << fooY << std::endl;
				// 	std::cout << "cx: " << cx << "; cy: " << cy << std::endl;
				// 	std::cout << std::endl;
				// 	// std::cout << (((int)iSrc / bitmap.GetWidth()) % 2) << std::endl;
				// 	timer -= 100000.f;
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
					DrawPixel(bitmap.GetPixel(iSrc, jSrc), iDest, jDest);
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
					
					if (iDest == destX && jDest == destY)
						std::cout << "iSrc: " << iSrc << "; jSrc: " << jSrc << std::endl;
					
					const RGBA& colorTL = bitmap.GetPixel(iSrc, jSrc);
					const RGBA& colorBL = bitmap.GetPixel(iSrc, (jSrc+1 >= bitmap.GetHeight() ? bitmap.GetHeight()-1 : jSrc + 1));
					const RGBA& colorTR = bitmap.GetPixel((iSrc+1 >= bitmap.GetWidth() ? bitmap.GetWidth()-1 : iSrc + 1), jSrc);
					const RGBA& colorBR = bitmap.GetPixel((iSrc+1 >= bitmap.GetWidth() ? bitmap.GetWidth()-1 : iSrc + 1), (jSrc+1 >= bitmap.GetHeight() ? bitmap.GetHeight()-1 : jSrc + 1));
					
					RGBA cT { colorTL * (1 - iSrcFraction) + colorTR * iSrcFraction };
					RGBA cB { colorBL * (1 - iSrcFraction) + colorBR * iSrcFraction };
					RGBA c {
						cT * (1 - jSrcFraction) + 
						cB * jSrcFraction 
					};
					
					DrawPixel(c, iDest, jDest);
				}
			}
		}
	}
}
cdr::RGBA cdr::Renderer::sampleTexture(const cdr::Bitmap& bitmap, float xSrc, float ySrc) {
	int fooX = 0;
	int fooY = 0;
	
	float x {xSrc};
	float y {ySrc};
	bool isInBounds = clampCoords(x, y, bitmap.GetWidth(), bitmap.GetHeight());
	if(!isInBounds) {
		if(OutOfBoundsType == OutOfBoundsType::MirroredRepeat) {
			fooX = (int)((xSrc + 0.000001) / bitmap.GetWidth()) % 2 + (xSrc < 0 ? 1 : 0);
			fooY = (int)((ySrc + 0.000001) / bitmap.GetHeight()) % 2 + (ySrc < 0 ? 1 : 0);
		}
		else if(OutOfBoundsType == OutOfBoundsType::ClampToBorder) {
			return ClampToBorderColor;
		}
	} 
	else if (this->ScaleType == ScaleType::Nearest) {
		return bitmap.GetPixel(xSrc, ySrc);
	}
	
	// TODO: maybe simplyify this later 
	if(this->ScaleType == ScaleType::Nearest) {
		if(fooX) 
			x = ceil(x);
		if(fooY) 
			y = ceil(y);
		return bitmap.GetPixel(x, y);
	} else { 
		// TODO: cheßck if downscaling works properly
		
		// NOTE: subtract 0.5 in order to put the point in the centre of the pixel
		if(fooX) x += 0.5;
		else 	 x -= 0.5;
		if(fooY) y += 0.5;
		else 	 y -= 0.5;
		
		if(x < 0) x = 0;
		if(x >= bitmap.GetWidth()) x = bitmap.GetWidth() - 1;
		if(y < 0) y = 0;
		if(y >= bitmap.GetHeight()) y = bitmap.GetHeight() - 1;

		float iSrcFraction = (x) - (int)x;
		float jSrcFraction = (y) - (int)y;
		
		const RGBA& colorTL = bitmap.GetPixel(x, y);
		const RGBA& colorBL = bitmap.GetPixel(x, (y+1 >= bitmap.GetHeight() ? y : y + 1));
		const RGBA& colorTR = bitmap.GetPixel((x+1 >= bitmap.GetWidth() ? x : x + 1), y);
		const RGBA& colorBR = bitmap.GetPixel((x+1 >= bitmap.GetWidth() ? x : x + 1), (y+1 >= bitmap.GetHeight() ? y : y + 1));
		
		RGBA cT { colorTL * (1 - iSrcFraction) + colorTR * iSrcFraction };
		RGBA cB { colorBL * (1 - iSrcFraction) + colorBR * iSrcFraction };
		RGBA c {
			cT * (1 - jSrcFraction) + 
			cB * jSrcFraction 
		};
		
		return c;
	}
}

void cdr::Renderer::DrawText(const std::string_view text, int x, int y, cdr::TextAlignment ta, const Font& font, float size, const RGBA& fColor, const RGBA& bColor, const RGBA& shadowColor, int shadowOffsetX, int shadowOffsetY) {
	int startX = 0;
	if (x < 0 || y < 0) {
		startX = globalX;
	}
	
	int fontSizeWidth = font.GetFontWidth();
	int fontSizeHeight = font.GetFontHeight();
	int charsRows = font.GetFontSheetWidth() / fontSizeWidth;
	int charsCols = font.GetFontSheetHeight() / fontSizeHeight;
		
	int textBoundingBoxWidth{};
	int textBoundingBoxHeight{1};
	int localWidth{};
	for (const auto& letter : text) {
		if (letter == '\n') {
			textBoundingBoxHeight++;
			localWidth = 0;
		} else	if (letter == '\t') {
			// NOTE: the 4 is the tab size
			localWidth += 4 - (localWidth) % 4;
		} else {
			localWidth++;
		}
		if(localWidth > textBoundingBoxWidth) {
			textBoundingBoxWidth = localWidth;
		}
	}
	
	int newLineCount{};
	int caretCol{};
	for (int letterCount = 0; letterCount < text.size(); letterCount++) {
		const unsigned char& letter = text[letterCount];
		if(letter < 0 || letter > 255) continue;
		
		int letterX = letter % charsCols;
		int letterY = letter / charsRows;
		if (letter == '\n') {
			newLineCount++;
			caretCol = 0;
			if(x == -1 || y == -1) {
				globalY++;
				globalX = 0;
			}
		} else	if (letter == '\t') {
			// NOTE: the 4 is the tab size
			caretCol += 4 - (caretCol) % 4;
			if(x == -1 || y == -1) {
				globalX += 4 - (globalX) % 4;
			}
		} else {
			caretCol++;
			if(x == -1 || y == -1) {
				globalX++;
			}
			for (int i = 0; i < fontSizeWidth * size; i++) {
				for (int j = 0; j < fontSizeHeight * size; j++) {
					const RGB& letterPixel = font.GetPixel(letterX * fontSizeWidth + i / size, letterY * fontSizeHeight + j / size);
					
					int subX{};
					int subY{};
					
					if(x >= 0 && y >= 0) {
						subX = x + (caretCol-1) * size * (fontSizeWidth) + i;
						subY = y + newLineCount * fontSizeHeight + j;
					} else {
						subX = startX * fontSizeWidth + (caretCol-1) * size * (fontSizeWidth) + i;
						if(subX >= GetWidth()) {
							globalY++;
							globalX = 0;
							startX = 0;
							caretCol = 1;
							subX = startX * fontSizeWidth + (caretCol-1) * (fontSizeWidth) + i;
						}
						subY = j + globalY*size * fontSizeHeight;
					}
					
					if (x >= 0 && y >= 0) {
						int fw = font.GetFontWidth();
						int fh = font.GetFontHeight();
						switch (ta) {
							case TextAlignment::TL: break; // NOTE: this is Default
							case TextAlignment::TC: subX -= fw * textBoundingBoxWidth/2.f*size; break;
							case TextAlignment::TR: subX -= fw * textBoundingBoxWidth*size; break;
							case TextAlignment::CL: subY -= fh * textBoundingBoxHeight/2.f*size; break;
							case TextAlignment::CC: subX -= fw * textBoundingBoxWidth/2.f*size; 
													subY -= fh * textBoundingBoxHeight/2.f*size; break;
							case TextAlignment::CR: subX -= fw * textBoundingBoxWidth*size; 
													subY -= fh * textBoundingBoxHeight/2.f*size;break;
							case TextAlignment::BL: subY -= fh * textBoundingBoxHeight*size; break;
							case TextAlignment::BC: subX -= fw * textBoundingBoxWidth/2.f*size; 
													subY -= fh * textBoundingBoxHeight*size; break;
							case TextAlignment::BR: subX -= fw * textBoundingBoxWidth*size; 
													subY -= fh * textBoundingBoxHeight*size; break;
						}
					}
					
					if(subX >= GetWidth() || subY >= GetHeight() || 
					subX < 0 || subY < 0)
					continue;
					
					if(letterPixel == RGB::Black && bColor != RGBA::Transparent && GetPixel(subX, subY) != shadowColor) {
						DrawPixel(bColor, subX, subY);
					} else if(letterPixel == RGB::White){
						DrawPixel(fColor, subX, subY);
						if(font.GetPixel(letterX * fontSizeWidth + i/size + shadowOffsetX, letterY * fontSizeHeight + j/size + shadowOffsetY) == RGB::Black) {
							DrawPixel(shadowColor, subX + shadowOffsetX*size, subY + shadowOffsetY*size);
						}
					}
				}
			}
		}
	}
}

/* UTILILTY FUNCTIONS */
bool cdr::Renderer::clampCoords(float& x, float& y, int width, int height) {
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
			if(this->ScaleType == ScaleType::Nearest) {
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

#if 0
void cdr::Renderer::DrawTriangle(const Bitmap& texture, FPoint tp1, FPoint tp2, FPoint tp3, FPoint p1, FPoint p2, FPoint p3) {
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
	
	// std::cout << "p1: " << p1.x << "; " << p1.y << std::endl;
	// std::cout << "p2: " << p2.x << "; " << p2.y << std::endl;
	// std::cout << "p3: " << p3.x << "; " << p3.y << std::endl << std::endl;

	Timer t{};	
	
	tp1.x *= texture.GetWidth();
	tp2.x *= texture.GetWidth();
	tp3.x *= texture.GetWidth();
	tp1.y *= texture.GetHeight();
	tp2.y *= texture.GetHeight();
	tp3.y *= texture.GetHeight();


	struct DPoint {
		double x;
		double y;
	} lerpCoordV1, lerpCoordV2;
	

	// #define barycentric
	// #define accumulation
	// #define lerp
	// #define lerp_for_triangle_sides
	
	#ifdef barycentric

	float leftStep{};
	if (p3.y - p1.y != 0) {
		leftStep = (p3.x - p1.x) / (p3.y - p1.y);
	}
	float rightStep{};
	if (p2.y - p1.y != 0) {
		rightStep = (p2.x - p1.x) / (p2.y - p1.y);
	}
	
	float left = p1.x + (std::ceil(p1.y) - p1.y) * leftStep;
	float right = p1.x + (std::ceil(p1.y) - p1.y) * rightStep;
	
	// float denom = edgeFunc(p1, p2, p3);// (p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y);
	float denom = (p3.x - p1.x)*(p2.y - p1.y) - (p3.y - p1.y)*(p2.x - p1.x);// (p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y);
	float w1{};
	float w2{};
	float w3{};

	for (int y = std::ceil(p1.y); y < std::ceil(p2.y); y++) {
		int min = std::ceil(left);
		int max = std::ceil(right);
		
		if (min > max) {
			std::swap(min, max);
		}
		
		for (int x = min; x < max; x++) {
			
			// DrawPixel(RGB::Red, x, y);
			
			w1 = ((x - p2.x)*(p3.y - p2.y) - (y - p2.y)*(p3.x - p2.x)) / denom;
			w2 = ((x - p3.x)*(p1.y - p3.y) - (y - p3.y)*(p1.x - p3.x)) / denom;
			w3 = ((x - p1.x)*(p2.y - p1.y) - (y - p1.y)*(p2.x - p1.x)) / denom;
			
			// w1 = ((p2.y - p3.y)*(x - p3.x) + (p3.x - p2.x)*(y - p3.y)) / denom;
			// w2 = ((p3.y - p1.y)*(x - p3.x) + (p1.x - p3.x)*(y - p3.y)) / denom;
			// w3 = 1.f - w1 - w2;
			
			float t = (w1 * tp1.x + w2 * tp2.x + w3 * tp3.x);
			float s = (w1 * tp1.y + w2 * tp2.y + w3 * tp3.y);
			
			DrawPixel(sampleTexture(texture, 
				t, s),
				x, y);
		}
		
		left += leftStep;
		right += rightStep;
	}
	
	if (p3.y - p2.y != 0) {
		rightStep = (p3.x - p2.x) / (p3.y - p2.y);
	}
	// right = p1.x + (std::ceil(p1.y) - p1.y) * rightStep
	right = p2.x + (std::ceil(p2.y) - p2.y) * rightStep;
	
	for (int y = std::ceil(p2.y); y < std::ceil(p3.y); y++) {
		int min = std::ceil(left);
		int max = std::ceil(right);
		
		if (min > max) {
			std::swap(min, max);
		}
		
		for (int x = min; x < max; x++) {
			
			// DrawPixel(RGB::Green, x, y);
			
			// FPoint p {float(x), float(y)};
			w1 = ((x - p2.x)*(p3.y - p2.y) - (y - p2.y)*(p3.x - p2.x)) / denom;
			w2 = ((x - p3.x)*(p1.y - p3.y) - (y - p3.y)*(p1.x - p3.x)) / denom;
			w3 = ((x - p1.x)*(p2.y - p1.y) - (y - p1.y)*(p2.x - p1.x)) / denom;
			
			// w1 = ((p2.y - p3.y)*(x - p3.x) + (p3.x - p2.x)*(y - p3.y)) / denom;
			// w2 = ((p3.y - p1.y)*(x - p3.x) + (p1.x - p3.x)*(y - p3.y)) / denom;
			// w3 = 1.f - w1 - w2;
			
			float t = (w1 * tp1.x + w2 * tp2.x + w3 * tp3.x);
			float s = (w1 * tp1.y + w2 * tp2.y + w3 * tp3.y);
			
			DrawPixel(sampleTexture(texture, 
				t, 
				s),
				x, y);
		}
		
		left += leftStep;
		right += rightStep;
	}
	
	
	
	#elif defined(lerp_for_triangle_sides)
	
		struct DPoint {
		double x;
		double y;
	} lerpCoordV1, lerpCoordV2;
	
	for (int y = std::ceil(p1.y); y < std::ceil(p2.y); y++) {
		
		double t1 = (y - p1.y) / (double)(p3.y - p1.y);
		double t2 = (y - p1.y) / (double)(p2.y - p1.y);
			
		double min = lerp(p1.x, p3.x, t1);
		double max = lerp(p1.x, p2.x, t2);
		
		int startX = std::ceil(min);
		int endX = std::ceil(max);
		
		lerpCoordV1.x = lerp(tp1.x, tp3.x, t1);
		lerpCoordV1.y = lerp(tp1.y, tp3.y, t1);
		
		lerpCoordV2.x = lerp(tp1.x, tp2.x, t2);
		lerpCoordV2.y = lerp(tp1.y, tp2.y, t2);
		
		if (min > max) {
			std::swap(startX, endX);
			std::swap(min, max);
			std::swap(lerpCoordV1, lerpCoordV2);
		}
		
		for (int x = startX; x < endX; x++) {
			double xLerp = lerp(lerpCoordV1.x, lerpCoordV2.x, (x - min) / (double)(max - min));
			double yLerp = lerp(lerpCoordV1.y, lerpCoordV2.y, (x - min) / (double)(max - min));
			DrawPixel(sampleTexture(texture, xLerp, yLerp), x, y);
		}
	}
	
	for (int y = std::ceil(p2.y); y < std::ceil(p3.y); y++) {
		double t1 = (y - p1.y) / (double)(p3.y - p1.y);
		double t2 = (y - p2.y) / (double)(p3.y - p2.y);
			
		double min = lerp(p1.x, p3.x, t1);
		double max = lerp(p2.x, p3.x, t2);
		
		int startX = std::ceil(min);
		int endX = std::ceil(max);

		lerpCoordV1.x = lerp(tp1.x, tp3.x, t1);
		lerpCoordV1.y = lerp(tp1.y, tp3.y, t1);
		
		lerpCoordV2.x = lerp(tp2.x, tp3.x, t2);
		lerpCoordV2.y = lerp(tp2.y, tp3.y, t2);
		
		if (min > max) {
			std::swap(startX, endX);
			std::swap(min, max);
			std::swap(lerpCoordV1, lerpCoordV2);
		}
		
		for (int x = startX; x < endX; x++) {
			double xLerp = lerp(lerpCoordV1.x, lerpCoordV2.x, (x - min) / (double)(max - min));
			double yLerp = lerp(lerpCoordV1.y, lerpCoordV2.y, (x - min) / (double)(max - min));
			DrawPixel(sampleTexture(texture, xLerp, yLerp), x, y);
		}
	}
	
	
	
	#elif defined(accumulation) 

	double leftStep{};
	if (p3.y - p1.y != 0) {
		leftStep = (p3.x - p1.x) / (p3.y - p1.y);
	}
	double rightStep{};
	if (p2.y - p1.y != 0) {
		rightStep = (p2.x - p1.x) / (p2.y - p1.y);
	}
	
	double left = p1.x + (std::ceil(p1.y) - p1.y) * leftStep;
	double right = p1.x + (std::ceil(p1.y) - p1.y) * rightStep;

	for (int y = std::ceil(p1.y); y < std::ceil(p2.y); y++) {
		
		double t1 = (y - p1.y) / (double)(p3.y - p1.y);
		double t2 = (y - p1.y) / (double)(p2.y - p1.y);
			
		int startX = std::ceil(left);
		int endX = std::ceil(right);
		double min = left;
		double max = right;
		
		lerpCoordV1.x = lerp(tp1.x, tp3.x, t1);
		lerpCoordV1.y = lerp(tp1.y, tp3.y, t1);
		
		lerpCoordV2.x = lerp(tp1.x, tp2.x, t2);
		lerpCoordV2.y = lerp(tp1.y, tp2.y, t2);
		
		if (min > max) {
			std::swap(startX, endX);
			std::swap(min, max);
			std::swap(lerpCoordV1, lerpCoordV2);
		}
		
		double xStep{};
		double yStep{};
		
		if(max - min != 0) {
			xStep = (lerpCoordV2.x - lerpCoordV1.x) / (endX - startX);
			yStep = (lerpCoordV2.y - lerpCoordV1.y) / (endX - startX);
		}
		
		double xLerp{lerpCoordV1.x};
		double yLerp{lerpCoordV1.y};
		
		for (int x = startX; x < endX; x++) {
			
			// DrawPixel(RGB::Red, x, y);
			
			DrawPixel(sampleTexture(texture, xLerp, yLerp), x, y);
			
			// xLerp += xStep;
			// yLerp += yStep;
			xLerp = lerpCoordV1.x + (xStep * (x - min));
			yLerp = lerpCoordV1.y + (yStep * (x - min));
		}
		
		left += leftStep;
		right += rightStep;
	}
	
	if (p3.y - p2.y != 0) {
		rightStep = (p3.x - p2.x) / (p3.y - p2.y);
	}
	// right = p1.x + (std::ceil(p1.y) - p1.y) * rightStep
	right = p2.x + (std::ceil(p2.y) - p2.y) * rightStep;
	
	for (int y = std::ceil(p2.y); y < std::ceil(p3.y); y++) {
		double t1 = (y - p1.y) / (double)(p3.y - p1.y);
		double t2 = (y - p2.y) / (double)(p3.y - p2.y);
			
		int startX = std::ceil(left);
		int endX = std::ceil(right);
		double min = left;
		double max = right;

		lerpCoordV1.x = lerp(tp1.x, tp3.x, t1);
		lerpCoordV1.y = lerp(tp1.y, tp3.y, t1);
		
		lerpCoordV2.x = lerp(tp2.x, tp3.x, t2);
		lerpCoordV2.y = lerp(tp2.y, tp3.y, t2);
		
		if (min > max) {
			std::swap(startX, endX);
			std::swap(min, max);
			std::swap(lerpCoordV1, lerpCoordV2);
		}
		
		double xStep{};
		double yStep{};
		
		if(max - min != 0) {
			xStep = (lerpCoordV2.x - lerpCoordV1.x) / (endX - startX);
			yStep = (lerpCoordV2.y - lerpCoordV1.y) / (endX - startX);
		}
		
		double xLerp{lerpCoordV1.x};
		double yLerp{lerpCoordV1.y};
		
		for (int x = startX; x < endX; x++) {
			
			// DrawPixel(RGB::Red, x, y);
			
			DrawPixel(sampleTexture(texture, xLerp, yLerp), x, y);
			
			// xLerp += xStep;
			// yLerp += yStep;
			xLerp = lerpCoordV1.x + (xStep * (x - min));
			yLerp = lerpCoordV1.y + (yStep * (x - min));
		}
		
		left += leftStep;
		right += rightStep;
	}
	
	
	
	#elif defined(lerp) 

	double leftStep{};
	if (p3.y - p1.y != 0) {
		leftStep = (p3.x - p1.x) / (p3.y - p1.y);
	}
	double rightStep{};
	if (p2.y - p1.y != 0) {
		rightStep = (p2.x - p1.x) / (p2.y - p1.y);
	}
	
	double left = p1.x + (std::ceil(p1.y) - p1.y) * leftStep;
	double right = p1.x + (std::ceil(p1.y) - p1.y) * rightStep;

	for (int y = std::ceil(p1.y); y < std::ceil(p2.y); y++) {
		
		double t1 = (y - p1.y) / (double)(p3.y - p1.y);
		double t2 = (y - p1.y) / (double)(p2.y - p1.y);
			
		int startX = std::ceil(left);
		int endX = std::ceil(right);
		double min = left;
		double max = right;
		
		lerpCoordV1.x = lerp(tp1.x, tp3.x, t1);
		lerpCoordV1.y = lerp(tp1.y, tp3.y, t1);
		
		lerpCoordV2.x = lerp(tp1.x, tp2.x, t2);
		lerpCoordV2.y = lerp(tp1.y, tp2.y, t2);
		
		if (min > max) {
			std::swap(startX, endX);
			std::swap(min, max);
			std::swap(lerpCoordV1, lerpCoordV2);
		}
		for (int x = startX; x < endX; x++) {
			double xLerp = lerp(lerpCoordV1.x, lerpCoordV2.x, (x - min) / (double)(max - min));
			double yLerp = lerp(lerpCoordV1.y, lerpCoordV2.y, (x - min) / (double)(max - min));
			DrawPixel(sampleTexture(texture, xLerp, yLerp), x, y);
		}
		
		left += leftStep;
		right += rightStep;
	}
	
	if (p3.y - p2.y != 0) {
		rightStep = (p3.x - p2.x) / (p3.y - p2.y);
	}
	// right = p1.x + (std::ceil(p1.y) - p1.y) * rightStep
	right = p2.x + (std::ceil(p2.y) - p2.y) * rightStep;
	
	for (int y = std::ceil(p2.y); y < std::ceil(p3.y); y++) {
		double t1 = (y - p1.y) / (double)(p3.y - p1.y);
		double t2 = (y - p2.y) / (double)(p3.y - p2.y);
			
		int startX = std::ceil(left);
		int endX = std::ceil(right);
		double min = left;
		double max = right;

		lerpCoordV1.x = lerp(tp1.x, tp3.x, t1);
		lerpCoordV1.y = lerp(tp1.y, tp3.y, t1);
		
		lerpCoordV2.x = lerp(tp2.x, tp3.x, t2);
		lerpCoordV2.y = lerp(tp2.y, tp3.y, t2);
		
		if (min > max) {
			std::swap(startX, endX);
			std::swap(min, max);
			std::swap(lerpCoordV1, lerpCoordV2);
		}
		for (int x = startX; x < endX; x++) {
			double xLerp = lerp(lerpCoordV1.x, lerpCoordV2.x, (x - min) / (double)(max - min));
			double yLerp = lerp(lerpCoordV1.y, lerpCoordV2.y, (x - min) / (double)(max - min));
			DrawPixel(sampleTexture(texture, xLerp, yLerp), x, y);
		}
		
		left += leftStep;
		right += rightStep;
	}
	
	
	
	#else // original algorithm
	
	// std::cout << "tp1: " << tp1.x << "; " << tp1.y << std::endl;
	// std::cout << "tp2: " << tp2.x << "; " << tp2.y << std::endl;
	// std::cout << "tp3: " << tp3.x << "; " << tp3.y << std::endl << std::endl;
	
	if(p3.y - p1.y > 0) {
		double x1 = 0;
		double x2 = 0;
		// FPoint lerpCoordV1{}; // vertical lerp coord from side 1
		// FPoint lerpCoordV2{}; // vertical lerp coord from side 2
		
		for (int i = p1.y; i < p2.y; i++) {
			double t1 = (i - p1.y) / (double)(p3.y - p1.y);
			double t2 = (i - p1.y) / (double)(p2.y - p1.y);
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
			
			double xStep{};
			double yStep{};
			
			if(endX - startX != 0) {
				xStep = (lerpCoordV2.x - lerpCoordV1.x) / (x2 - x1);
				yStep = (lerpCoordV2.y - lerpCoordV1.y) / (x2 - x1);
			}
			
			double xLerp{lerpCoordV1.x};
			double yLerp{lerpCoordV1.y};
			
			for (int j = startX; j < endX; j++) {
				double clampedXLerp = xLerp;
				double clampedYLerp = yLerp;
				
				// clampedXLerp = lerp((lerpCoordV1.x), (lerpCoordV2.x), (j - startX) / (double)(endX - startX));
				// clampedYLerp = lerp((lerpCoordV1.y), (lerpCoordV2.y), (j - startX) / (double)(endX - startX));
				// DrawPixel(sampleTexture(texture, clampedXLerp, clampedYLerp), j, i);
				DrawPixel(sampleTexture(texture, xLerp, yLerp), j, i);
				
				xLerp += xStep;
				yLerp += yStep;
			}
		}
		
		for (int i = p2.y; i < p3.y; i++)	{
			double t1 = (i - p1.y) / (double)(p3.y - p1.y);
			double t2 = (i - p2.y) / (double)(p3.y - p2.y);
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
			
			double xStep{};
			double yStep{};
			
			if(endX - startX != 0) {
				xStep = (lerpCoordV2.x - lerpCoordV1.x) / (x2 - x1);
				yStep = (lerpCoordV2.y - lerpCoordV1.y) / (x2 - x1);
			}
			
			double xLerp{lerpCoordV1.x};
			double yLerp{lerpCoordV1.y};
			
			for (int j = startX; j < endX; j++) {
				double clampedXLerp = xLerp;
				double clampedYLerp = yLerp;
				
				// clampedXLerp = lerp((lerpCoordV1.x), (lerpCoordV2.x), (j - startX) / (double)(endX - startX));
				// clampedYLerp = lerp((lerpCoordV1.y), (lerpCoordV2.y), (j - startX) / (double)(endX - startX));
				// DrawPixel(sampleTexture(texture, clampedXLerp, clampedYLerp), j, i);
				DrawPixel(sampleTexture(texture, xLerp, yLerp), j, i);
				
				xLerp += xStep;
				yLerp += yStep;
			}
		}
	}
	#endif
	
	static int counter = 0;
	static float accumulation = 0;
	counter++;
	accumulation += t.elapsed()/1000;
	
	if (counter > 64) {
		std::cout << "speed: " << accumulation/counter << std::endl;
		counter = 0;
		accumulation = 0;
	}
}
#endif