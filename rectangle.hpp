/********************************
 * Project: Cidr				*
 * File: rectangle.hpp			*
 * Date: 11.10.2020				*
 ********************************/

namespace Cidr {

struct FRectangle;

struct Rectangle {
	int x{0};
	int y{0};
	int width{0};
	int height{0};
	
	operator FRectangle() const;
};

struct FRectangle {
	float x{0};
	float y{0};
	float width{0};
	float height{0};
	
    operator Rectangle() const;
};

}