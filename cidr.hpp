/********************************
 * Project: Cidr				*
 * File: rgb.hpp				*
 * Date: 10.8.2020				*
 ********************************/

#ifndef CIDR_CIDR_HPP
#define CIDR_CIDR_HPP

#include "rgb.hpp"

namespace Cidr {
	
class Renderer {
private:
	uint32_t* pixels {nullptr};
	size_t width {0};
	size_t height {0};
	
	inline size_t getIndex(int x, int y) {
		return x + y * width;
	}
	
public:
	Renderer(uint32_t* pixels, size_t width, size_t height);
	
	void Clear();
	void Clear(RGBA color);
	void Clear(uint32_t color);
	
	void DrawPoint(RGBA color, int x, int y); // WIP
	void DrawPoint(uint32_t color, int x, int y); // WIP
	void DrawLine(RGBA color, int x1, int y1, int x2, int y2, bool AA = false); // WIP
	void DrawLine(uint32_t color, int x1, int y1, int x2, int y2, bool AA = false);
	void DrawRectangle(RGBA color, int x, int y, int wdith, int height);
	void DrawRectangle(uint32_t color, int x, int y, int wdith, int height);
	void FillRectangle(RGBA color, int x, int y, int wdith, int height);
	void FillRectangle(uint32_t color, int x, int y, int wdith, int height);
	void DrawCircle(RGBA color, int centreX, int centreY, int radius, bool AA = false);
	void DrawCircle(uint32_t color, int centreX, int centreY, int radius, bool AA = false);
	void FillCircle(RGBA color, int centreX, int centreY, int radius, bool AA = false);
	void FillCircle(uint32_t color, int centreX, int centreY, int radius, bool AA = false);
	// void DrawSprite();
	
};

}

#endif