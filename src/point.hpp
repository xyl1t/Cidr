/********************************
 * Project: Cidr                *
 * File: point.hpp              *
 * Date: 12.9.2020              *
 ********************************/

#ifndef CIDR_POINT_HPP
#define CIDR_POINT_HPP

#include "tensorMath.hpp"

// TODO: ADD CONSTURCTORS!

// In order to keep the code "consice" I just derive the point classes from vectors
// But Since points are not really vectors they shouldn't have length functions so I hide them
namespace Cidr {

struct FPoint;

struct Point : public tem::ivec2 {
public:
	Point();
	Point(int x, int y);
	operator Cidr::FPoint() const;
	
private:
	using tem::ivec2::length;
    using tem::ivec2::setLength;
    using tem::ivec2::addLength;
    using tem::ivec2::subLength;
};

struct FPoint : public tem::vec2 {
public:
	FPoint();
	FPoint(float x, float y);
	operator Cidr::Point() const;
private:
	using tem::vec2::length;
    using tem::vec2::setLength;
    using tem::vec2::addLength;
    using tem::vec2::subLength;
};
}

#endif