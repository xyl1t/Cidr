/********************************
 * Project: Cidr				*
 * File: rgb.hpp				*
 * Date: 10.8.2020				*
 ********************************/

#ifndef CIDR_CIDR_HPP
#define CIDR_CIDR_HPP

#include "rgb.hpp"
#include <cmath>

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
	
	void DrawPoint(RGBA color, int x, int y);
	void DrawPoint(uint32_t color, int x, int y);
	void DrawLine(RGBA color, int x1, int y1, int x2, int y2, bool AA = false, bool GC = false); // WIP
	void DrawLine(uint32_t color, int x1, int y1, int x2, int y2, bool AA = false, bool GC = false); // NOT IMPLEMENTED
	void DrawRectangle(RGBA color, int x, int y, int wdith, int height); // NOT IMPLEMENTED
	void DrawRectangle(uint32_t color, int x, int y, int wdith, int height); // NOT IMPLEMENTED
	void FillRectangle(RGBA color, int x, int y, int wdith, int height); // NOT IMPLEMENTED
	void FillRectangle(uint32_t color, int x, int y, int wdith, int height); // NOT IMPLEMENTED
	void DrawCircle(RGBA color, int centreX, int centreY, int radius, bool AA = false); // NOT IMPLEMENTED
	void DrawCircle(uint32_t color, int centreX, int centreY, int radius, bool AA = false); // NOT IMPLEMENTED
	void FillCircle(RGBA color, int centreX, int centreY, int radius, bool AA = false); // NOT IMPLEMENTED
	void FillCircle(uint32_t color, int centreX, int centreY, int radius, bool AA = false); // NOT IMPLEMENTED
	// void DrawSprite();
	
	inline Cidr::RGBA GetPixel(int x, int y) {
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