/********************************
 * Project: Cidr				*
 * File: color.hpp				*
 * Date: 10.8.2020				*
 ********************************/

#ifndef CIDR_COLOR_HPP
#define CIDR_COLOR_HPP

#include <cstdint>
#include <iostream>

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
	
	virtual uint32_t getColor() const;
	virtual void setColor(uint32_t color);
	
    RGB& operator+=(const RGB&  that);
    RGB& operator-=(const RGB&  that);
    virtual RGB& operator*=(const uint8_t that);
    virtual RGB& operator/=(const uint8_t that);
    RGB  operator+ (const RGB&  that);
    RGB  operator- (const RGB&  that);
    RGB  operator* (const uint8_t that);
    RGB  operator/ (const uint8_t that);
};

struct RGBA : RGB {
	uint8_t a{0xff};

	RGBA();
	RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff);
	RGBA(uint32_t color);
	RGBA(const RGB& rgb, int a = 0xff);
	
	virtual uint32_t getColor() const override ;
	virtual void setColor(uint32_t color) override;
	
	inline RGB getRGB() {
		return static_cast<RGB>(*this);
	}
	
	
	RGBA& operator+=(const RGBA&  that);
    RGBA& operator-=(const RGBA&  that);
    virtual RGBA& operator*=(const uint8_t that) override;
    virtual RGBA& operator/=(const uint8_t that) override;
    RGBA  operator+ (const RGBA&  that);
    RGBA  operator- (const RGBA&  that);
    RGBA  operator* (const uint8_t that);
    RGBA  operator/ (const uint8_t that);
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

// inline HSV RGBtoHSV(const RGB& colorRGB) {
	
// }
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
	
	hh = static_cast<int>(colorHSV.h) % 360;
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
// inline HSL RGBtoHSL(const RGB& colorRGB) {
	
// }
// inline RGB HSLtoRGB(const HSL& colorHSL) {
	
// }

} // NAMESPACE CIDR END 



std::ostream& operator<<(std::ostream& out, const Cidr::RGB& rgb);
std::ostream& operator<<(std::ostream& out, const Cidr::RGBA& rgba);

#endif
