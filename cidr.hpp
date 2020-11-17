/********************************
 * Project: Cidr				*
 * File: rgb.hpp				*
 * Date: 10.9.2020				*
 ********************************/

#ifndef CIDR_CIDR_HPP
#define CIDR_CIDR_HPP

#include <cmath>
#include <string_view>
#include "color.hpp"
#include "point.hpp"
#include "bitmap.hpp"
#include "rectangle.hpp"

namespace Cidr {
	
class Renderer {
private:
	uint32_t* pixels {nullptr};
	int width {0};
	int height {0};
	
public:
	/* CONSTRUCTOR - DESTRUCTOR */
	Renderer(uint32_t* pixels, int width, int height);
	
	/* CLEAR FUNCTIONS */ 
	void Clear();
	void Clear(const RGBA& color);
	void Clear(uint32_t color); 
	
	/* CORE DRAWING FUNCTIONS */
	void DrawPoint(const RGBA& color, const Point& p);
	void DrawLine(const RGBA& color, const Point& start, const Point& end, bool AA = false, bool GC = false);
	void DrawRectangle(const RGBA& color, Rectangle rectangle);
	void FillRectangle(const RGBA& color, Rectangle rectangle);
	void FillRectangle(RGBA (*shader)(const Renderer& renderer, int x, int y), Rectangle rectangle);
	void DrawCircle(const RGBA& color, const Point& centreLocation, int radius, bool AA = false);
	void FillCircle(const RGBA& color, const Point& centreLocation, int radius, bool AA = false);
	void FillCircle(RGBA (*shader)(const Renderer& renderer, int x, int y), const Point& centreLocation, int radius, bool AA = false);
	void DrawTriangle(const RGBA& color, const Point& p1, const Point& p2, const Point& p3, bool AA = false, bool GC = false);
	void FillTriangle(const RGBA& color, Point p1, Point p2, Point p3);
	void FillTriangle(RGBA color1, RGBA color2, RGBA color3, Point p1, Point p2, Point p3);
	void FillTriangle(RGBA (*shader)(const Renderer& renderer, int x, int y), Point p1, Point p2, Point p3);
	void DrawBitmap(const Bitmap& bitmap, float destX, float destY, int destWidth, int destHeight, float srcX, float srcY, int srcWidth, int srcHeight);
	void DrawText(const std::string_view text, int x, int y, const RGBA& fColor = RGB::White, const RGBA& bColor = RGBA::Transparent, const RGBA& shadowColor = RGBA::Transparent, int shadowOffsetX = 1, int shadowOffsetY = 1);

	void DrawTriangle(const Bitmap& texture, FPoint tp1, FPoint tp2, FPoint tp3, Point p1, Point p2, Point p3);
	
	/* DRAWING FUNCTION OVERLOADS */
	inline void DrawPoint(const RGBA& color, int x, int y) { DrawPoint(color, Point(x, y)); }
	inline void DrawLine(const RGBA& color, int x1, int y1, int x2, int y2, bool AA = false, bool GC = false) { DrawLine(color, (Point){x1, y1}, (Point){x2, y2}, AA, GC); }
	inline void DrawRectangle(const RGBA& color, int x, int y, int width, int height) { DrawRectangle(color, (Rectangle){x, y, width, height}); }
	inline void FillRectangle(const RGBA& color, int x, int y, int width, int height) { FillRectangle(color, (Rectangle){x,y, width, height}); }
	inline void FillRectangle(RGBA (*shader)(const Renderer& renderer, int x, int y), int x, int y, int width, int height) { FillRectangle(shader, (Rectangle){x, y, width, height}); }
	inline void DrawCircle(const RGBA& color, int centreX, int centreY, int radius, bool AA = false) { DrawCircle(color, (Point){centreX,centreY}, radius, AA); }
	inline void FillCircle(const RGBA& color, int centreX, int centreY, int radius, bool AA = false) { FillCircle(color, (Point){centreX,centreY}, radius, AA); }
	inline void FillCircle(RGBA (*shader)(const Renderer& renderer, int x, int y), int centreX, int centreY, int radius, bool AA = false) { FillCircle(shader, (Point){centreX,centreY}, radius, AA); }
	inline void DrawTriangle(const RGBA& color, int x1, int y1, int x2, int y2, int x3, int y3, bool AA = false, bool GC = false) { DrawTriangle(color, (Point){x1, y1}, (Point){x2, y2}, (Point){x3, y3}, AA, GC ); }
	inline void DrawTriangle(const Bitmap& texture, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, int x1, int y1, int x2, int y2, int x3, int y3) { DrawTriangle(texture, (FPoint){tx1, ty1}, (FPoint){tx2, ty2}, (FPoint){tx3, ty3}, (Point){x1, y1}, (Point){x2, y2}, (Point){x3, y3}); }
	inline void FillTriangle(const RGBA& color, int x1, int y1, int x2, int y2, int x3, int y3) { FillTriangle(color, (Point){x1, y1}, (Point){x2, y2}, (Point){x3, y3} ); }
	inline void FillTriangle(RGBA color1, RGBA color2, RGBA color3, int x1, int y1, int x2, int y2, int x3, int y3) { FillTriangle(color1, color2, color3, (Point){x1, y1}, (Point){x2, y2}, (Point){x3, y3}); }
	inline void FillTriangle(RGBA (*shader)(const Renderer& renderer, int x, int y), int x1, int y1, int x2, int y2, int x3, int y3) { FillTriangle(shader, (Point){x1, y1}, (Point){x2, y2}, (Point){x3, y3} ); }
	inline void DrawBitmap(const Bitmap& bitmap, FPoint destLocation, int destWidth, int destHeight, FPoint srcLocation, int srcWidth, int srcHeight) { DrawBitmap(bitmap, destLocation.x, destLocation.y, destWidth, destHeight, srcLocation.x, srcLocation.y, srcWidth, srcHeight); }
	
	inline void DrawPoint(uint32_t color, const Point& p) { DrawPoint((RGBA){color}, p); }
	inline void DrawLine(uint32_t color, const Point& start, const Point& end, bool AA = false, bool GC = false) { DrawLine((RGBA){color}, start, end, AA, GC); }
	inline void DrawRectangle(uint32_t color, Rectangle rectangle) { DrawRectangle((RGBA){color}, rectangle); }	
	inline void FillRectangle(uint32_t color, Rectangle rectangle) { FillRectangle((RGBA){color}, rectangle); }
	inline void DrawCircle(uint32_t color, const Point& centreLocation, int radius, bool AA = false) { DrawCircle((RGBA){color}, centreLocation, radius, AA); }
	inline void FillCircle(uint32_t color, const Point& centreLocation, int radius, bool AA = false) { FillCircle((RGBA){color}, centreLocation, radius, AA); }
	inline void DrawTriangle(uint32_t color, const Point& p1, const Point& p2, const Point& p3, bool AA = false, bool GC = false) { DrawTriangle((RGBA){color}, p1, p2, p3, AA, GC ); }
	inline void FillTriangle(uint32_t color, const Point& p1, Point p2, Point p3) { FillTriangle((RGBA){color}, p1, p2, p3 ); }
	inline void FillTriangle(uint32_t color1, uint32_t color2, uint32_t color3, Point p1, Point p2, Point p3) { FillTriangle((RGBA){color1}, (RGBA){color2}, (RGBA){color3}, p1, p2, p3); }
	inline void DrawBitmap(const Bitmap& bitmap, FRectangle destRect, FRectangle srcRect) { DrawBitmap(bitmap, destRect.x, destRect.y, destRect.width, destRect.height, srcRect.x, srcRect.y, srcRect.width, srcRect.height); }
	
	inline void DrawPoint(uint32_t color, int x, int y) { DrawPoint((RGBA){color}, (Point){x, y}); }
	inline void DrawLine(uint32_t color, int x1, int y1, int x2, int y2, bool AA = false, bool GC = false) { DrawLine((RGBA){color}, (Point){x1, y1}, (Point){x2, y2}, AA, GC); }
	inline void DrawRectangle(uint32_t color, int x, int y, int width, int height) { DrawRectangle((RGBA){color}, (Rectangle){x, y, width, height}); }
	inline void FillRectangle(uint32_t color, int x, int y, int width, int height) { FillRectangle((RGBA){color}, (Rectangle){x, y, width, height}); }
	inline void DrawCircle(uint32_t color, int centreX, int centreY, int radius, bool AA = false) { DrawCircle((RGBA){color}, (Point){centreX,centreY}, radius, AA); }
	inline void FillCircle(uint32_t color, int centreX, int centreY, int radius, bool AA = false) { FillCircle((RGBA){color}, (Point){centreX,centreY}, radius, AA); }
	inline void DrawTriangle(uint32_t color, int x1, int y1, int x2, int y2, int x3, int y3, bool AA = false, bool GC = false) { DrawTriangle((RGBA){color}, (Point){x1, y1}, (Point){x2, y2}, (Point){x3, y3}, AA, GC ); }
	inline void FillTriangle(uint32_t color, int x1, int y1, int x2, int y2, int x3, int y3) { FillTriangle((RGBA){color}, (Point){x1, y1}, (Point){x2, y2}, (Point){x3, y3} ); }
	inline void FillTriangle(uint32_t color1, uint32_t color2, uint32_t color3, int x1, int y1, int x2, int y2, int x3, int y3) { FillTriangle((RGBA){color1}, (RGBA){color2}, (RGBA){color3}, (Point){x1, y1}, (Point){x2, y2}, (Point){x3, y3}); }
	
	/* GETTERS */
	inline uint32_t* GetData() const {
		return pixels;
	}
	inline int GetWidth() const {
		return width;
	}
	inline int GetHeight() const {
		return height;
	}
	inline Cidr::RGBA GetPixel(const Point& p) const {
		if(p.x < 0 || p.y < 0 || p.x >= GetWidth() || p.y >= GetHeight()) return Cidr::RGBA{};
		return Cidr::RGBA{pixels[getIndex(p)]};
	}
	inline Cidr::RGBA GetPixel(int x, int y) const {
		if(x < 0 || y < 0 || x >= GetWidth() || y >= GetHeight()) return Cidr::RGBA{};
		return Cidr::RGBA{pixels[getIndex(x, y)]};
	}
	
	enum class ScaleType {
		Nearest,
		Linear,
	} ScaleType = ScaleType::Nearest;
	
	enum class OutOfBoundsType {
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder,
	} OutOfBoundsType = OutOfBoundsType::ClampToBorder;
	RGBA ClampToBorderColor {};
	
	/* UTILITY FUNCTIONS */
private:
	inline int getIndex(const Point& p) const {
		return p.x + p.y * width;
	}
	inline int getIndex(int x, int y) const {
		return x + y * width;
	}
	void drawScanLine(uint32_t color, int startX, int endX, int y);
	void drawScanLine(const RGBA& color1, const RGBA& color2, int startX, int endX, int y);
	
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