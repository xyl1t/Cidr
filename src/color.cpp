/********************************
 * Project: Cidr                *
 * File: color.cpp              *
 * Date: 10.9.2020              *
 ********************************/

#include "color.hpp"
#include <algorithm>

Cidr::RGB::RGB() : r{0}, g{0}, b{0} {
}
Cidr::RGB::RGB(uint8_t r, uint8_t g, uint8_t b) : r{r}, g{g}, b{b} {
}
Cidr::RGB::RGB(uint32_t color)
	: r{static_cast<uint8_t>((color >> 24) & 0xff)}, 
	g{static_cast<uint8_t>((color >> 16) & 0xff)}, 
	b{static_cast<uint8_t>((color >>  8) & 0xff)} {
}

Cidr::RGB& Cidr::RGB::operator+=(const RGB& that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r + that.r, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g + that.g, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b + that.b, 0, 255));
	return *this;
}
Cidr::RGB& Cidr::RGB::operator-=(const RGB& that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r - that.r, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g - that.g, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b - that.b, 0, 255));
	return *this;
}
Cidr::RGB& Cidr::RGB::operator*=(const int that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r * that, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g * that, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b * that, 0, 255));
	return *this;
}
Cidr::RGB& Cidr::RGB::operator/=(const int that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r / that, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g / that, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b / that, 0, 255));
	return *this;
}
Cidr::RGB& Cidr::RGB::operator*=(const float that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r * that, 0.f, 255.f));
	this->g = static_cast<uint8_t>(std::clamp(this->g * that, 0.f, 255.f));
	this->b = static_cast<uint8_t>(std::clamp(this->b * that, 0.f, 255.f));
	return *this;
}
Cidr::RGB& Cidr::RGB::operator/=(const float that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r / that, 0.f, 255.f));
	this->g = static_cast<uint8_t>(std::clamp(this->g / that, 0.f, 255.f));
	this->b = static_cast<uint8_t>(std::clamp(this->b / that, 0.f, 255.f));
	return *this;
}
bool Cidr::RGB::operator==(const RGB& that) const {
	return this->r == that.r && this->g == that.g && this->b == that.b;
}
bool Cidr::RGB::operator!=(const RGB& that) const {
	return !(*this == that);
}
Cidr::RGB Cidr::RGB::operator+(const RGB& that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r + that.r, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g + that.g, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b + that.b, 0, 255)),
	};
}
Cidr::RGB Cidr::RGB::operator-(const RGB& that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r - that.r, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g - that.g, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b - that.b, 0, 255)),
	};
}
Cidr::RGB Cidr::RGB::operator*(const int that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r * that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g * that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b * that, 0, 255)),
	};
}
Cidr::RGB Cidr::RGB::operator/(const int that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r / that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g / that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b / that, 0, 255)),
	};
}
Cidr::RGB Cidr::RGB::operator*(const float that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->g * that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->b * that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->r * that, 0.f, 255.f)),
	};
}
Cidr::RGB Cidr::RGB::operator/(const float that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r / that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->g / that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->b / that, 0.f, 255.f)),
	};
}




Cidr::RGBA::RGBA() : Cidr::RGB(), a(0xff) {
}
Cidr::RGBA::RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : Cidr::RGB(r, g, b), a(a) {
}
Cidr::RGBA::RGBA(uint32_t color) : RGB(color), a{static_cast<uint8_t>(color & 0xff)} {
}
Cidr::RGBA::RGBA(const Cidr::RGB& rgb, int a) : Cidr::RGB(rgb), a(a) {
	
}

std::ostream& operator<<(std::ostream& out, const Cidr::RGB& rgb) {
	out << "r: " << (int)rgb.r << " g: " << (int)rgb.g << " b: " << (int)rgb.b;
	return out;
}
std::ostream& operator<<(std::ostream& out, const Cidr::RGBA& rgba) {
	out << "r: " << (int)rgba.r << " g: " << (int)rgba.g << " b: " << (int)rgba.b << " a: " << (int)rgba.a;
	return out;
}

Cidr::RGBA& Cidr::RGBA::operator+=(const RGBA& that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r + that.r, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g + that.g, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b + that.b, 0, 255));
	return *this;
}
Cidr::RGBA& Cidr::RGBA::operator-=(const RGBA& that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r - that.r, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g - that.g, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b - that.b, 0, 255));
	return *this;
}
Cidr::RGBA& Cidr::RGBA::operator*=(const int that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r * that, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g * that, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b * that, 0, 255));
	return *this;
}
Cidr::RGBA& Cidr::RGBA::operator/=(const int that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r / that, 0, 255));
	this->g = static_cast<uint8_t>(std::clamp(this->g / that, 0, 255));
	this->b = static_cast<uint8_t>(std::clamp(this->b / that, 0, 255));
	return *this;
}

Cidr::RGBA& Cidr::RGBA::operator*=(const float that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r * that, 0.f, 255.f));
	this->g = static_cast<uint8_t>(std::clamp(this->g * that, 0.f, 255.f));
	this->b = static_cast<uint8_t>(std::clamp(this->b * that, 0.f, 255.f));
	return *this;
}
Cidr::RGBA& Cidr::RGBA::operator/=(const float that) {
	this->r = static_cast<uint8_t>(std::clamp(this->r / that, 0.f, 255.f));
	this->g = static_cast<uint8_t>(std::clamp(this->g / that, 0.f, 255.f));
	this->b = static_cast<uint8_t>(std::clamp(this->b / that, 0.f, 255.f));
	return *this;
}
bool Cidr::RGBA::operator==(const RGBA& that) const {
	return this->r == that.r && this->g == that.g && this->b == that.b && this->a == that.a;
}
bool Cidr::RGBA::operator!=(const RGBA& that) const {
	return !(*this == that);
}
Cidr::RGBA Cidr::RGBA::operator+(const RGBA& that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r + that.r, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g + that.g, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b + that.b, 0, 255)),
		this->a,
	};
}
Cidr::RGBA Cidr::RGBA::operator-(const RGBA& that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r - that.r, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g - that.g, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b - that.b, 0, 255)),
		this->a,
	};
}
Cidr::RGBA Cidr::RGBA::operator*(const int that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r * that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g * that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b * that, 0, 255)),
		this->a,
	};
}
Cidr::RGBA Cidr::RGBA::operator/(const int that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r / that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->g / that, 0, 255)),
		static_cast<uint8_t>(std::clamp(this->b / that, 0, 255)),
		this->a,
	};
}
Cidr::RGBA Cidr::RGBA::operator*(const float that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r * that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->g * that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->b * that, 0.f, 255.f)),
		this->a,
	};
}
Cidr::RGBA Cidr::RGBA::operator/(const float that) const {
	return {
		static_cast<uint8_t>(std::clamp(this->r / that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->g / that, 0.f, 255.f)),
		static_cast<uint8_t>(std::clamp(this->b / that, 0.f, 255.f)),
		this->a,
	};
}




uint32_t Cidr::ToColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return (r << 24) + (g << 16) + (b << 8) + (a);
}

const Cidr::RGBA Cidr::RGBA::Transparent { 0, 0, 0, 0 };

const Cidr::RGB Cidr::RGB::Black 	{  0,   0,   0};
const Cidr::RGB Cidr::RGB::Red 		{255,   0,   0};
const Cidr::RGB Cidr::RGB::Green 	{  0, 255,   0};
const Cidr::RGB Cidr::RGB::Blue 	{  0,   0, 255};
const Cidr::RGB Cidr::RGB::Cyan 	{  0, 255, 255};
const Cidr::RGB Cidr::RGB::Magenta 	{255,   0, 255};
const Cidr::RGB Cidr::RGB::Yellow 	{255, 255,   0};
const Cidr::RGB Cidr::RGB::White 	{255, 255, 255};
const Cidr::RGB Cidr::RGB::Gray 	{128, 128, 128};
const Cidr::RGB Cidr::RGB::Grey 	{192, 192, 192};
const Cidr::RGB Cidr::RGB::Maroon 	{128,   0,   0};
const Cidr::RGB Cidr::RGB::Darkgreen{  0, 128,   0};
const Cidr::RGB Cidr::RGB::Navy 	{  0,   0, 128};
const Cidr::RGB Cidr::RGB::Teal 	{  0, 128, 128};
const Cidr::RGB Cidr::RGB::Purple 	{128,   0, 128};
const Cidr::RGB Cidr::RGB::Olive 	{128, 128,   0};