/********************************
 * Project: Cidr				*
 * File: rectangle.cpp			*
 * Date: 11.10.2020				*
 ********************************/

#include "rectangle.hpp"

Cidr::Rectangle::operator FRectangle() const {
	return FRectangle{(float)x, (float)y, (float)width, (float)height};
}

Cidr::FRectangle::operator Rectangle() const {
	return Rectangle{(int)x, (int)y, (int)width, (int)height};
}
