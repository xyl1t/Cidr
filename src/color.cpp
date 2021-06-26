/********************************
 * Project: Cidr                *
 * File: color.cpp              *
 * Date: 10.9.2020              *
 ********************************/

#include "color.hpp"
#include <algorithm>

cdr::RGB::RGB() : r{0}, g{0}, b{0} {
}
cdr::RGB::RGB(uint8_t r, uint8_t g, uint8_t b) : r{r}, g{g}, b{b} {
}
cdr::RGB::RGB(uint32_t color)
	: r{static_cast<uint8_t>((color >> 24) & 0xff)}, 
	g{static_cast<uint8_t>((color >> 16) & 0xff)}, 
	b{static_cast<uint8_t>((color >>  8) & 0xff)} {
}

cdr::RGB& cdr::RGB::operator+=(const RGB& that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r + that.r, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g + that.g, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b + that.b, 0, 255));
	return *this;
}
cdr::RGB& cdr::RGB::operator-=(const RGB& that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r - that.r, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g - that.g, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b - that.b, 0, 255));
	return *this;
}
cdr::RGB& cdr::RGB::operator*=(const int that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r * that, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g * that, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b * that, 0, 255));
	return *this;
}
cdr::RGB& cdr::RGB::operator/=(const int that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r / that, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g / that, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b / that, 0, 255));
	return *this;
}
cdr::RGB& cdr::RGB::operator*=(const float that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r * that, 0.f, 255.f));
	this->g = static_cast<uint8_t>(std::clamp(this->g * that, 0.f, 255.f));
	this->b = static_cast<uint8_t>(std::clamp(this->b * that, 0.f, 255.f));
	return *this;
}
cdr::RGB& cdr::RGB::operator/=(const float that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r / that, 0.f, 255.f));
	this->g = static_cast<uint8_t>(std::clamp(this->g / that, 0.f, 255.f));
	this->b = static_cast<uint8_t>(std::clamp(this->b / that, 0.f, 255.f));
	return *this;
}
bool cdr::RGB::operator==(const RGB& that) const {
	return this->r == that.r && this->g == that.g && this->b == that.b;
}
bool cdr::RGB::operator!=(const RGB& that) const {
	return !(*this == that);
}
cdr::RGB cdr::RGB::operator+(const RGB& that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r + that.r, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g + that.g, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b + that.b, 0, 255)),
	};
}
cdr::RGB cdr::RGB::operator-(const RGB& that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r - that.r, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g - that.g, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b - that.b, 0, 255)),
	};
}
cdr::RGB cdr::RGB::operator*(const int that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r * that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g * that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b * that, 0, 255)),
	};
}
cdr::RGB cdr::RGB::operator/(const int that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r / that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g / that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b / that, 0, 255)),
	};
}
cdr::RGB cdr::RGB::operator*(const float that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->g * that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->b * that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->r * that, 0.f, 255.f)),
	};
}
cdr::RGB cdr::RGB::operator/(const float that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r / that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->g / that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->b / that, 0.f, 255.f)),
	};
}




cdr::RGBA::RGBA() : cdr::RGB(), a(0xff) {
}
cdr::RGBA::RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : cdr::RGB(r, g, b), a(a) {
}
cdr::RGBA::RGBA(uint32_t color) : RGB(color), a{static_cast<uint8_t>(color & 0xff)} {
}
cdr::RGBA::RGBA(const cdr::RGB& rgb, int a) : cdr::RGB(rgb), a(a) {
	
}

std::ostream& operator<<(std::ostream& out, const cdr::RGB& rgb) {
	out << "r: " << (int)rgb.r << " g: " << (int)rgb.g << " b: " << (int)rgb.b;
	return out;
}
std::ostream& operator<<(std::ostream& out, const cdr::RGBA& rgba) {
	out << "r: " << (int)rgba.r << " g: " << (int)rgba.g << " b: " << (int)rgba.b << " a: " << (int)rgba.a;
	return out;
}

cdr::RGBA& cdr::RGBA::operator+=(const RGBA& that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r + that.r, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g + that.g, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b + that.b, 0, 255));
	return *this;
}
cdr::RGBA& cdr::RGBA::operator-=(const RGBA& that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r - that.r, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g - that.g, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b - that.b, 0, 255));
	return *this;
}
cdr::RGBA& cdr::RGBA::operator*=(const int that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r * that, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g * that, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b * that, 0, 255));
	return *this;
}
cdr::RGBA& cdr::RGBA::operator/=(const int that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r / that, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g / that, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b / that, 0, 255));
	return *this;
}

cdr::RGBA& cdr::RGBA::operator*=(const float that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r * that, 0.f, 255.f));
	this->g = static_cast<uint8_t>(std::clamp(this->g * that, 0.f, 255.f));
	this->b = static_cast<uint8_t>(std::clamp(this->b * that, 0.f, 255.f));
	return *this;
}
cdr::RGBA& cdr::RGBA::operator/=(const float that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r / that, 0.f, 255.f));
	this->g = static_cast<uint8_t>(std::clamp(this->g / that, 0.f, 255.f));
	this->b = static_cast<uint8_t>(std::clamp(this->b / that, 0.f, 255.f));
	return *this;
}
bool cdr::RGBA::operator==(const RGBA& that) const {
	return this->r == that.r && this->g == that.g && this->b == that.b && this->a == that.a;
}
bool cdr::RGBA::operator!=(const RGBA& that) const {
	return !(*this == that);
}
cdr::RGBA cdr::RGBA::operator+(const RGBA& that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r + that.r, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g + that.g, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b + that.b, 0, 255)),
		this->a,
	};
}
cdr::RGBA cdr::RGBA::operator-(const RGBA& that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r - that.r, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g - that.g, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b - that.b, 0, 255)),
		this->a,
	};
}
cdr::RGBA cdr::RGBA::operator*(const int that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r * that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g * that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b * that, 0, 255)),
		this->a,
	};
}
cdr::RGBA cdr::RGBA::operator/(const int that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r / that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g / that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b / that, 0, 255)),
		this->a,
	};
}
cdr::RGBA cdr::RGBA::operator*(const float that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r * that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->g * that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->b * that, 0.f, 255.f)),
		this->a,
	};
}
cdr::RGBA cdr::RGBA::operator/(const float that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r / that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->g / that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->b / that, 0.f, 255.f)),
		this->a,
	};
}




uint32_t cdr::ToColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return (r << 24) + (g << 16) + (b << 8) + (a);
}

const cdr::RGBA cdr::RGBA::Transparent { 0, 0, 0, 0 };

const cdr::RGB cdr::RGB::Black 	{  0,   0,   0};
const cdr::RGB cdr::RGB::Red 		{255,   0,   0};
const cdr::RGB cdr::RGB::Green 	{  0, 255,   0};
const cdr::RGB cdr::RGB::Blue 	{  0,   0, 255};
const cdr::RGB cdr::RGB::Cyan 	{  0, 255, 255};
const cdr::RGB cdr::RGB::Magenta 	{255,   0, 255};
const cdr::RGB cdr::RGB::Yellow 	{255, 255,   0};
const cdr::RGB cdr::RGB::White 	{255, 255, 255};
const cdr::RGB cdr::RGB::Gray 	{128, 128, 128};
const cdr::RGB cdr::RGB::Grey 	{192, 192, 192};
const cdr::RGB cdr::RGB::Maroon 	{128,   0,   0};
const cdr::RGB cdr::RGB::Darkgreen{  0, 128,   0};
const cdr::RGB cdr::RGB::Navy 	{  0,   0, 128};
const cdr::RGB cdr::RGB::Teal 	{  0, 128, 128};
const cdr::RGB cdr::RGB::Purple 	{128,   0, 128};
const cdr::RGB cdr::RGB::Olive 	{128, 128,   0};