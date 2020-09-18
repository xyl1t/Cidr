/********************************
 * Project: Cidr				*
 * File: rgb.hpp				*
 * Date: 10.8.2020				*
 ********************************/

#ifndef CIDR_CIDR_HPP
#define CIDR_CIDR_HPP

#include "color.hpp"
#include <cmath>
#include "point.hpp"

namespace Cidr {
	
class Renderer {
private:
	uint32_t* pixels {nullptr};
	int width {0};
	int height {0};
	
	inline int getIndex(const Point& p) const {
		return p.x + p.y * width;
	}
	inline int getIndex(int x, int y) const {
		return x + y * width;
	}
	
public:
	Renderer(uint32_t* pixels, int width, int height);
	
	void Clear();
	void Clear(const RGBA& color);
	void Clear(uint32_t color); 
	
	void DrawPoint(const RGBA& color, const Point& p);
	void DrawLine(const RGBA& color, const Point& start, const Point& end, bool AA = false, bool GC = false);
	void DrawRectangle(const RGBA& color, const Point& location, int width, int height);
	void FillRectangle(const RGBA& color, const Point& location, int width, int height);
	void FillRectangle(RGBA (*shader)(const Renderer& renderer, int x, int y), const Point& location, int width, int height); // WIP
	void DrawCircle(const RGBA& color, const Point& centreLocation, int radius, bool AA = false); // NOT IMPLEMENTED
	void FillCircle(const RGBA& color, const Point& centreLocation, int radius, bool AA = false); // NOT IMPLEMENTED
	
	// overloads 
	inline void DrawPoint(const RGBA& color, int x, int y) { DrawPoint(color, (Point){x, y}); }
	inline void DrawLine(const RGBA& color, int x1, int y1, int x2, int y2, bool AA = false, bool GC = false) { DrawLine(color, (Point){x1, y1}, (Point){x2, y2}, AA, GC); }
	inline void DrawRectangle(const RGBA& color, int x, int y, int width, int height) { DrawRectangle(color, (Point){x,y}, width, height); }
	inline void FillRectangle(const RGBA& color, int x, int y, int width, int height) { FillRectangle(color, (Point){x,y}, width, height); }
	inline void DrawCircle(const RGBA& color, int centreX, int centreY, int radius, bool AA = false) { DrawCircle(color, (Point){centreX,centreY}, radius, AA); }
	inline void FillCircle(const RGBA& color, int centreX, int centreY, int radius, bool AA = false) { FillCircle(color, (Point){centreX,centreY}, radius, AA); }

	inline void DrawPoint(uint32_t color, const Point& p) { DrawPoint((RGBA){color}, p); }
	inline void DrawLine(uint32_t color, const Point& start, const Point& end, bool AA = false, bool GC = false) { DrawLine((RGBA){color}, start, end, AA, GC); }
	inline void DrawRectangle(uint32_t color, const Point& location, int width, int height) { DrawRectangle((RGBA){color}, location, width, height); }
	inline void FillRectangle(uint32_t color, const Point& location, int width, int height) { FillRectangle((RGBA){color}, location, width, height); }
	inline void DrawCircle(uint32_t color, const Point& centreLocation, int radius, bool AA = false) { DrawCircle((RGBA){color}, centreLocation, radius, AA); }
	inline void FillCircle(uint32_t color, const Point& centreLocation, int radius, bool AA = false) { FillCircle((RGBA){color}, centreLocation, radius, AA); }
	
	inline void DrawPoint(uint32_t color, int x, int y) { DrawPoint((RGBA){color}, (Point){x, y}); }
	inline void DrawLine(uint32_t color, int x1, int y1, int x2, int y2, bool AA = false, bool GC = false) { DrawLine((RGBA){color}, (Point){x1, y1}, (Point){x2, y2}, AA, GC); }
	inline void DrawRectangle(uint32_t color, int x, int y, int width, int height) { DrawRectangle((RGBA){color}, (Point){x,y}, width, height); }
	inline void FillRectangle(uint32_t color, int x, int y, int width, int height) { FillRectangle((RGBA){color}, (Point){x,y}, width, height); }
	inline void DrawCircle(uint32_t color, int centreX, int centreY, int radius, bool AA = false) { DrawCircle((RGBA){color}, (Point){centreX,centreY}, radius, AA); }
	inline void FillCircle(uint32_t color, int centreX, int centreY, int radius, bool AA = false) { FillCircle((RGBA){color}, (Point){centreX,centreY}, radius, AA); }
	
	// TODO: check for valid coordinates
	inline Cidr::RGBA GetPixel(const Point& p) const {
		return Cidr::RGBA{pixels[getIndex(p)]};
	}
	inline Cidr::RGBA GetPixel(int x, int y) const {
		return Cidr::RGBA{pixels[getIndex(x, y)]};
	}
};

inline RGB alphaBlendColor(const Cidr::RGB& color1, const Cidr::RGB& color2, float alpha) {
	return (Cidr::RGB) { 
		static_cast<uint8_t>(color2.r * (alpha / 255.f) + color1.r * (1 - alpha / 255.f)),
		static_cast<uint8_t>(color2.g * (alpha / 255.f) + color1.g * (1 - alpha / 255.f)),
		static_cast<uint8_t>(color2.b * (alpha / 255.f) + color1.b * (1 - alpha / 255.f))
	};
}
inline RGB alphaBlendColorGammaCorrected(const Cidr::RGB& color1, const Cidr::RGB& color2, float alpha, float gamma = 2.2) {
	return (Cidr::RGB) { 
		static_cast<uint8_t>(std::pow(std::pow(color2.r, gamma) * (alpha / 255.f) + std::pow(color1.r, gamma) * (1 - alpha / 255.f), 1.f / gamma)),
		static_cast<uint8_t>(std::pow(std::pow(color2.g, gamma) * (alpha / 255.f) + std::pow(color1.g, gamma) * (1 - alpha / 255.f), 1.f / gamma)),
		static_cast<uint8_t>(std::pow(std::pow(color2.b, gamma) * (alpha / 255.f) + std::pow(color1.b, gamma) * (1 - alpha / 255.f), 1.f / gamma))
	};
}
inline RGBA alphaBlendColor(const Cidr::RGBA& color1, const Cidr::RGBA& color2){
	 // uses color2's alpha value
	return (Cidr::RGBA) {
		static_cast<uint8_t>(color2.r * (color2.a / 255.f) + color1.r * (1 - color2.a / 255.f)),
		static_cast<uint8_t>(color2.g * (color2.a / 255.f) + color1.g * (1 - color2.a / 255.f)),
		static_cast<uint8_t>(color2.b * (color2.a / 255.f) + color1.b * (1 - color2.a / 255.f)),
		0xff
	};
}



}

#endif