/********************************
 * Project: Cidr				*
 * File: rgb.hpp				*
 * Date: 10.8.2020				*
 ********************************/

#ifndef CIDR_RGB_HPP
#define CIDR_RGB_HPP

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

}


std::ostream& operator<<(std::ostream& out, const Cidr::RGB& rgb);
std::ostream& operator<<(std::ostream& out, const Cidr::RGBA& rgba);

#endif
