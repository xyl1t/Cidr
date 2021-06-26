/********************************
 * Project: Cidr                *
 * File: point.cpp              *
 * Date: 12.9.2020              *
 ********************************/

#include "point.hpp"

cdr::Point::Point() : tem::ivec2() {
}
cdr::Point::Point(int x, int y) : tem::ivec2(x, y) {
}
cdr::Point::operator cdr::FPoint() const {
	return {static_cast<float>(x), static_cast<float>(y)};
}


cdr::FPoint::FPoint() : tem::vec2() {
}
cdr::FPoint::FPoint(float x, float y) : tem::vec2(x, y) {
}
cdr::FPoint::operator cdr::Point() const {
	return {static_cast<int>(x), static_cast<int>(y)};
}