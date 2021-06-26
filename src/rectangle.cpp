/********************************
 * Project: Cidr				*
 * File: rectangle.cpp			*
 * Date: 11.10.2020				*
 ********************************/

#include "rectangle.hpp"

cdr::Rectangle::operator FRectangle() const {
	return FRectangle{(float)x, (float)y, (float)width, (float)height};
}

cdr::FRectangle::operator Rectangle() const {
	return Rectangle{(int)x, (int)y, (int)width, (int)height};
}
