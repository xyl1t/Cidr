/********************************
 * Project: Cidr                *
 * File: rgb.hpp                *
 * Date: 12.9.2020              *
 ********************************/

#include "point.hpp"

Cidr::Point::operator Cidr::FPoint() const {
	return {static_cast<float>(x), static_cast<float>(y)};
}
Cidr::FPoint::operator Cidr::Point() const {
	return {static_cast<int>(x), static_cast<int>(y)};
}