/********************************
 * Project: Cidr                *
 * File: point.cpp              *
 * Date: 12.9.2020              *
 ********************************/

#include "point.hpp"

Cidr::Point::Point() : tem::ivec2() {
}
Cidr::Point::Point(int x, int y) : tem::ivec2(x, y) {
}
Cidr::Point::operator Cidr::FPoint() const {
	return {static_cast<float>(x), static_cast<float>(y)};
}


Cidr::FPoint::FPoint() : tem::vec2() {
}
Cidr::FPoint::FPoint(float x, float y) : tem::vec2(x, y) {
}
Cidr::FPoint::operator Cidr::Point() const {
	return {static_cast<int>(x), static_cast<int>(y)};
}