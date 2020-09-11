/********************************
 * Project: Cidr				*
 * File: rgb.cpp				*
 * Date: 10.8.2020				*
 ********************************/

#include "rgb.hpp"

Cidr::RGB::RGB(uint8_t r, uint8_t g, uint8_t b) : r{r}, g{g}, b{b} {
}
Cidr::RGB::RGB(const Cidr::RGBA& rgba) : r{rgba.r}, g{rgba.g}, b{rgba.b} {
}

uint32_t Cidr::RGB::getColor() {
	return (r << 24) + (g << 16) + (b << 8) + 0xff;
}

void Cidr::RGB::setColor(uint32_t color) {
	r = (color >> 24) & 0xff;
	g = (color >> 16) & 0xff;
	b = (color >>  8) & 0xff;
}


Cidr::RGBA::RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : Cidr::RGB(r, g, b), a(a) {
}
Cidr::RGBA::RGBA(const Cidr::RGB& rgb, int a) : Cidr::RGB(rgb), a(a) {
	
}

uint32_t Cidr::RGBA::getColor() {
	return (r << 24) + (g << 16) + (b << 8) + (a);
}
void Cidr::RGBA::setColor(uint32_t color) {
	r = (color >> 24) & 0xff;
	g = (color >> 16) & 0xff;
	b = (color >>  8) & 0xff;
	a = (color)       & 0xff;
}

std::ostream& operator<<(std::ostream& out, const Cidr::RGB& rgb) {
	out << "r: " << (int)rgb.r << " g: " << (int)rgb.g << " b: " << (int)rgb.b;
	return out;
}
std::ostream& operator<<(std::ostream& out, const Cidr::RGBA& rgba) {
	out << "r: " << (int)rgba.r << " g: " << (int)rgba.g << " b: " << (int)rgba.b << " a: " << (int)rgba.a;
	return out;
}


uint32_t Cidr::ToColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return (r << 24) + (g << 16) + (b << 8) + (a);
}
