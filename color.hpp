/********************************
 * Project: Cidr				*
 * File: color.hpp				*
 * Date: 10.8.2020				*
 ********************************/

#ifndef CIDR_COLOR_HPP
#define CIDR_COLOR_HPP

#include <cstdint>
#include <iostream>
#include <cmath>

namespace Cidr {

struct RGBA;
	
struct RGB {
	uint8_t r;
	uint8_t g;
	uint8_t b;

	RGB();
	RGB(uint8_t r, uint8_t g, uint8_t b);
	RGB(uint32_t color);
	RGB(const RGBA& rgba);
	
    RGB& operator+=(const RGB&  that);
    RGB& operator-=(const RGB&  that);
    virtual RGB& operator*=(const uint8_t that);
    virtual RGB& operator/=(const uint8_t that);
    RGB  operator+ (const RGB&  that) const;
    RGB  operator- (const RGB&  that) const;
    RGB  operator* (const uint8_t that) const;
    RGB  operator/ (const uint8_t that) const;
};

struct RGBA : RGB {
	uint8_t a{0xff};

	RGBA();
	RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff);
	RGBA(uint32_t color);
	RGBA(const RGB& rgb, int a = 0xff);
	
	inline RGB getRGB() {
		return static_cast<RGB>(*this);
	}
	
	
	RGBA& operator+=(const RGBA&  that);
    RGBA& operator-=(const RGBA&  that);
    virtual RGBA& operator*=(const uint8_t that) override;
    virtual RGBA& operator/=(const uint8_t that) override;
    RGBA  operator+ (const RGBA&  that) const;
    RGBA  operator- (const RGBA&  that) const;
    RGBA  operator* (const uint8_t that) const;
    RGBA  operator/ (const uint8_t that) const;
};

uint32_t ToColor(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0xff);

struct HSV {
	float h;
	float s;
	float v;
};
struct HSL {
	float h;
	float s;
	float l;
};

inline uint32_t RGBtoUINT(const RGB& colorRGB) {
	return (colorRGB.r << 24) + (colorRGB.g << 16) + (colorRGB.b << 8) + 0xff;
}
inline uint32_t RGBtoUINT(const RGBA& colorRGB) {
	return (colorRGB.r << 24) + (colorRGB.g << 16) + (colorRGB.b << 8) + colorRGB.a;
}
inline RGBA UINTtoRGB(const uint32_t& colorUINT) {
	return {
		static_cast<uint8_t>((colorUINT >> 24) & 0xff),
		static_cast<uint8_t>((colorUINT >> 16) & 0xff),
		static_cast<uint8_t>((colorUINT >>  8) & 0xff),
		static_cast<uint8_t>((colorUINT >>  0) & 0xff)
	};
}
inline HSV RGBtoHSV(const RGB& colorRGB) {
	HSV color;
	float min, max, delta;
	
	min = std::min(std::min(colorRGB.r, colorRGB.g), colorRGB.b);
	max = std::max(std::max(colorRGB.r, colorRGB.g), colorRGB.b);
	
	color.v = max / 255.f;
	delta = max - min;
	if(delta < 0.00001f) {
		color.s = 0;
		color.h = 0;
		return color;
	}
	if(max > 0.f) {
		color.s = delta / max;
	}
	else {
		color.s = 0.f;
		color.h = 0.f;
		return color;
	}
	if(colorRGB.r >= max) {
		color.h = (colorRGB.g - colorRGB.b) / delta;
	}
	else if(colorRGB.g >= max) {
		color.h = 2.0 + (colorRGB.b - colorRGB.r) / delta;
	}
	else {
		color.h = 4.0 + (colorRGB.r - colorRGB.g) / delta;
	}
	
	color.h *= 60;
	
	if(color.h < 0.f) {
		color.h += 360.f;
	}
	
	return color;
}
inline RGB HSVtoRGB(const HSV& colorHSV) {
	RGB color;
	float hh, ff, p, q, t;
	int i;
	
	if(colorHSV.s <= 0) {
		color.r = colorHSV.v;
		color.g = colorHSV.v;
		color.b = colorHSV.v;
		return color;
	}
	
	hh = colorHSV.h;
    if(hh >= 360.0) hh = 0.0;
	// hh = std::fmod(hh, 360);
	hh /= 60.f;
	i = static_cast<int>(hh);
	ff = hh - i;
	p = colorHSV.v * (1.f - colorHSV.s);
	q = colorHSV.v * (1.f - (colorHSV.s * ff));
	t = colorHSV.v * (1.f - (colorHSV.s * (1.f - ff)));
	
    switch(i) {
    case 0:
        color.r = colorHSV.v * 255;
        color.g = t * 255;
        color.b = p * 255;
        break;
    case 1:
        color.r = q * 255;
        color.g = colorHSV.v * 255;
        color.b = p * 255;
        break;
    case 2:
        color.r = p * 255;
        color.g = colorHSV.v * 255;
        color.b = t * 255;
        break;

    case 3:
        color.r = p * 255;
        color.g = q * 255;
        color.b = colorHSV.v * 255;
        break;
    case 4:
        color.r = t * 255;
        color.g = p * 255;
        color.b = colorHSV.v * 255;
        break;
    case 5:
    default:
        color.r = colorHSV.v * 255;
        color.g = p * 255;
        color.b = q * 255;
        break;
    }
	
	return color;
}
inline HSL RGBtoHSL(const RGB& colorRGB) {
	HSL color;
	float r {colorRGB.r / 255.f};
	float g {colorRGB.g / 255.f};
	float b {colorRGB.b / 255.f};
	float cmax {static_cast<float>(std::fmax(std::fmax(r, g), b))};
	float cmin {static_cast<float>(std::fmin(std::fmin(r, g), b))};
	float delta {cmax - cmin};
	
	color.l = (cmax + cmin) / 2.f;
	
	if(delta == 0){
		color.h = 0;
		color.s = 0;
		return color;
	}
	else if(cmax == r) {
		color.h = 60.f * (std::fmodf((g - b) / delta, 6));
	}
	else if(cmax == g) {
		color.h = 60.f * ((b - r) / delta + 2); 
	}
	else if(cmax == b) {
		color.h = 60.f * ((r - g) / delta + 4);
	}
	
	if(color.h < 0) color.h += 360;
	if(color.h > 360) color.h -= 360;
	
	color.s = delta / (1 - std::abs(color.l * 2 - 1));
	
	return color;
}
inline RGB HSLtoRGB(const HSL& colorHSL) {
	float c = (1 - std::abs(2 * colorHSL.l - 1)) * colorHSL.s;
	float x = c * (1 - std::abs(std::fmodf((colorHSL.h) / 60.f, 2) - 1.f));
	float m = colorHSL.l - c / 2.f;
	
	float r, g, b;
	if(colorHSL.h >= 0 && colorHSL.h < 60) {
		r = c;
		g = x;
		b = 0;
	}
	else if(colorHSL.h >= 60 && colorHSL.h < 120){
		r = x;
		g = c;
		b = 0;
	}
	else if(colorHSL.h >= 120 && colorHSL.h < 180){
		r = 0;
		g = c;
		b = x;
	}
	else if(colorHSL.h >= 180 && colorHSL.h < 240){
		r = 0;
		g = x;
		b = c;
	}
	else if(colorHSL.h >= 240 && colorHSL.h < 300){
		r = x;
		g = 0;
		b = c;
	}
	else if(colorHSL.h >= 300 && colorHSL.h < 360){
		r = c;
		g = 0;
		b = x;
	}
	return (RGB){
		static_cast<uint8_t>((r + m) * 255),
		static_cast<uint8_t>((g + m) * 255),
		static_cast<uint8_t>((b + m) * 255)
		};
}

} // NAMESPACE CIDR END 

std::ostream& operator<<(std::ostream& out, const Cidr::RGB& rgb);
std::ostream& operator<<(std::ostream& out, const Cidr::RGBA& rgba);

#endif

/*
From stackoverflow hsl -> rgb
RGB color;
if(colorHSL.s == 0) {
	color.r = colorHSL.l * 255;
	color.g = colorHSL.l * 255;
	color.b = colorHSL.l * 255;
}
else {
	float h = colorHSL.h / 360.f;
	auto hueToRgb {[](float p, float q, float t) -> float{
		if (t < 0)
			t += 1;

		if (t > 1)
			t -= 1;

		if ((6 * t) < 1)
			return (p + (q - p) * 6 * t);

		if ((2 * t) < 1)
			return q;

		if ((3 * t) < 2)
			return (p + (q - p) * ((2.0f / 3) - t) * 6);

		return p;
	}
	};
	

	float q = colorHSL.l < 0.5f ? colorHSL.l * (1 + colorHSL.s) : colorHSL.l + colorHSL.s - colorHSL.l * colorHSL.s;
	float p = 2 * colorHSL.l - q;
	color.r = std::min(255.f, hueToRgb(p, q, h + 1/3.f) * 255.f);
	color.g = std::min(255.f, hueToRgb(p, q, h        ) * 255.f);
	color.b = std::min(255.f, hueToRgb(p, q, h - 1/3.f) * 255.f);
}

return color;*/