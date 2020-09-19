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
    virtual RGB& operator*=(const int that);
    virtual RGB& operator/=(const int that);
    virtual RGB& operator*=(const float that);
    virtual RGB& operator/=(const float that);
    RGB  operator+ (const RGB&  that) const;
    RGB  operator- (const RGB&  that) const;
    RGB  operator* (const int that) const;
    RGB  operator/ (const int that) const;
    RGB  operator* (const float that) const;
    RGB  operator/ (const float that) const;
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
    virtual RGBA& operator*=(const int that);
    virtual RGBA& operator/=(const int that);
    virtual RGBA& operator*=(const float that);
    virtual RGBA& operator/=(const float that);
    RGBA  operator+ (const RGBA&  that) const;
    RGBA  operator- (const RGBA&  that) const;
    RGBA  operator* (const int that) const;
    RGBA  operator/ (const int that) const;
    RGBA  operator* (const float that) const;
    RGBA  operator/ (const float that) const;
};

uint32_t ToColor(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0xff);

class HSV {
float h;
float s;
float v;
public:
	inline void setH(float valueInDegrees) {
		if(valueInDegrees < 0) {
			valueInDegrees += 360 * std::ceil(std::abs(valueInDegrees / 360.f));
		}
		else {
			valueInDegrees = std::fmod(valueInDegrees, 360);
		}
		this->h = valueInDegrees;
	}
	inline void setS(float value) {
		if(value < 0) s = 0;
		else if(value > 1) s = 1;
		else s = value;
	}
	inline void setV(float value) {
		if(value < 0) v = 0;
		else if(value > 1) v = 1;
		else v = value;
	}
	
	inline float getH() const {
		return h;
	}
	inline float getS() const  {
		return s;
	}
	inline float getV() const  {
		return v;
	}
};
class HSL {
	float h;
	float s;
	float l;
public:
	inline void setH(float valueInDegrees) {
		if(valueInDegrees < 0) {
			valueInDegrees += 360 * std::ceil(std::abs(valueInDegrees / 360.f));
		}
		else {
			valueInDegrees = std::fmod(valueInDegrees, 360.f);
		}
		this->h = valueInDegrees;
	}
	inline void setS(float value) {
		if(value < 0) s = 0;
		else if(value > 1) s = 1;
		else s = value;
	}
	inline void setL(float value) {
		if(value < 0) l = 0;
		else if(value > 1) l = 1;
		else l = value;
	}
	
	inline float getH() const {
		return h;
	}
	inline float getS() const  {
		return s;
	}
	inline float getL() const  {
		return l;
	}
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
	
	color.setV(max / 255.f);
	delta = max - min;
	if(delta < 0.00001f) {
		color.setS(0);
		color.setH(0);
		return color;
	}
	if(max > 0.f) {
		color.setS(delta / max);
	}
	else {
		color.setS(0.f);
		color.setH(0.f);
		return color;
	}
	if(colorRGB.r >= max) {
		color.setH((colorRGB.g - colorRGB.b) / delta);
	}
	else if(colorRGB.g >= max) {
		color.setH(2.0 + (colorRGB.b - colorRGB.r) / delta);
	}
	else {
		color.setH(4.0 + (colorRGB.r - colorRGB.g) / delta);
	}
	
	color.setH(color.getH() * 60);
	
	if(color.getH() < 0.f) {
		color.setH(color.getH() + 360.f);
	}
	
	return color;
}
inline RGB HSVtoRGB(const HSV& colorHSV) {
	RGB color;
	float hh, ff, p, q, t;
	int i;
	
	if(colorHSV.getS() <= 0) {
		color.r = colorHSV.getV();
		color.g = colorHSV.getV();
		color.b = colorHSV.getV();
		return color;
	}
	
	hh = colorHSV.getH();
    if(hh >= 360.0) hh = 0.0;
	// hh = std::fmod(hh, 360);
	hh /= 60.f;
	i = static_cast<int>(hh);
	ff = hh - i;
	p = colorHSV.getV() * (1.f - colorHSV.getS());
	q = colorHSV.getV() * (1.f - (colorHSV.getS() * ff));
	t = colorHSV.getV() * (1.f - (colorHSV.getS() * (1.f - ff)));
	
    switch(i) {
    case 0:
        color.r = colorHSV.getV() * 255;
        color.g = t * 255;
        color.b = p * 255;
        break;
    case 1:
        color.r = q * 255;
        color.g = colorHSV.getV() * 255;
        color.b = p * 255;
        break;
    case 2:
        color.r = p * 255;
        color.g = colorHSV.getV() * 255;
        color.b = t * 255;
        break;

    case 3:
        color.r = p * 255;
        color.g = q * 255;
        color.b = colorHSV.getV() * 255;
        break;
    case 4:
        color.r = t * 255;
        color.g = p * 255;
        color.b = colorHSV.getV() * 255;
        break;
    case 5:
    default:
        color.r = colorHSV.getV() * 255;
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
	
	color.setL((cmax + cmin) / 2.f);
	
	if(delta == 0){
		color.setH(0);
		color.setS(0);
		return color;
	}
	else if(cmax == r) {
		color.setH(60.f * (std::fmodf((g - b) / delta, 6)));
	}
	else if(cmax == g) {
		color.setH(60.f * ((b - r) / delta + 2)); 
	}
	else if(cmax == b) {
		color.setH(60.f * ((r - g) / delta + 4));
	}
	
	if(color.getH() < 0) color.setH(color.getH() + 360);
	if(color.getH() > 360) color.setH(color.getH() - 360);
	
	color.setS(delta / (1 - std::abs(color.getL() * 2 - 1)));
	
	return color;
}
inline RGB HSLtoRGB(const HSL& colorHSL) {
	float c = (1 - std::abs(2 * colorHSL.getL() - 1)) * colorHSL.getS();
	float x = c * (1 - std::abs(std::fmodf((colorHSL.getH()) / 60.f, 2) - 1.f));
	float m = colorHSL.getL() - c / 2.f;
	
	float r, g, b;
	if(colorHSL.getH() >= 0 && colorHSL.getH() < 60) {
		r = c;
		g = x;
		b = 0;
	}
	else if(colorHSL.getH() >= 60 && colorHSL.getH() < 120){
		r = x;
		g = c;
		b = 0;
	}
	else if(colorHSL.getH() >= 120 && colorHSL.getH() < 180){
		r = 0;
		g = c;
		b = x;
	}
	else if(colorHSL.getH() >= 180 && colorHSL.getH() < 240){
		r = 0;
		g = x;
		b = c;
	}
	else if(colorHSL.getH() >= 240 && colorHSL.getH() < 300){
		r = x;
		g = 0;
		b = c;
	}
	else if(colorHSL.getH() >= 300 && colorHSL.getH() < 360){
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