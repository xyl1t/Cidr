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
	uint8_t r{0};
	uint8_t g{0};
	uint8_t b{0};

	RGB(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);
	RGB(const RGBA& rgba);
	
	virtual uint32_t getColor();
	virtual void setColor(uint32_t color);
};

struct RGBA : RGB {
	uint8_t a{0xff};

	RGBA(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0xff);
	RGBA(const RGB& rgb);
	
	virtual uint32_t getColor() override;
	virtual void setColor(uint32_t color) override;
};

uint32_t ToColor(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0xff);

}


std::ostream& operator<<(std::ostream& out, const Cidr::RGB& rgb);
std::ostream& operator<<(std::ostream& out, const Cidr::RGBA& rgba);

#endif
