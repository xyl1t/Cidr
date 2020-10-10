/********************************
 * Project: Cidr				*
 * File: bitmap.hpp				*
 * Date: 10.10.2020				*
 ********************************/

#ifndef CIDR_BITMAP_HPP
#define CIDR_BITMAP_HPP

#include <cstdint>
#include "color.hpp"

namespace Cidr {

class RGBABitmap {
private:
	/* Individual pixels of the bitmap */
	uint32_t* data{nullptr};
	/* Width of the bitmap */
	int width{0};
	/* Height of the bitmap */
	int height{0};
	
public:
	RGBABitmap(int width, int height);
	RGBABitmap(uint32_t* source, int sourceWidth, int sourceHeight);
	RGBABitmap(const std::string& file);
	~RGBABitmap();

	// copy constructor/assignment
	RGBABitmap(const RGBABitmap& other);
	RGBABitmap& operator=(const RGBABitmap& other);
	RGBABitmap(RGBABitmap&& other) noexcept;
	RGBABitmap& operator=(RGBABitmap&& other) noexcept;

	
	// RGBABitmap(uint32_t* data, int width, int height);
	inline int GetWidth() const  {
		return width;
	}
	inline int GetHeight() const {
		return height;
	}
	inline uint32_t* GetData() const {
		return data;
	}
	inline const RGBA& GetPixel(int x, int y) const {
		return data[x + y * width];
	}
	inline void SetPixel(const RGB& value, int x, int y) {
		data[x + y * width] = RGBtoUINT(value);
	}
};

class RGBBitmap {
	/* Individual pixels of the bitmap */
	uint32_t* data{nullptr};
	/* Width of the bitmap */
	int width{0};
	/* Height of the bitmap */
	int height{0};
	
	inline RGB GetPixel(int x, int y) {
		return data[x + y * width];
	}
	inline void SetPixel(const RGB& value, int x, int y) {
		data[x + y * width] = RGBtoUINT(value);
	}
};

class MonochromeBitmap {
	/* Individual pixels of the bitmap */
	uint8_t* data{nullptr};
	/* Width of the bitmap */
	int width{0};
	/* Height of the bitmap */
	int height{0};
	
	inline uint8_t GetPixel(int x, int y) {
		return data[x + y * width];
	}
	inline void SetPixel(const uint8_t& value, int x, int y) {
		data[x + y * width] = value;
	}
};

using Bitmap = RGBABitmap;

}

#endif

/*
template<typename T> 
struct Bitmap {
	T* data{nullptr};
	int width{0};
	int height{0};
	
	inline const T& GetPixel(int x, int y) {
		return data[x + y * width];
	}
	inline void SetPixel(const T& value, int x, int y) {
		data[x + y * width] = value;
	}
};

template<>
struct Bitmap<RGBA> {
	uint32_t* data{nullptr};
	int width{0};
	int height{0};
	
	inline RGBA GetPixel(int x, int y) {
		return data[x + y * width];
	}
	inline void SetPixel(const RGB& value, int x, int y) {
		data[x + y * width] = RGBtoUINT(value);
	}
};

template<>
struct Bitmap<RGB> {
	uint32_t* data{nullptr};
	int width{0};
	int height{0};
	
	inline RGB GetPixel(int x, int y) {
		return data[x + y * width];
	}
	inline void SetPixel(const RGB& value, int x, int y) {
		data[x + y * width] = RGBtoUINT(value);
	}
};

template<>
struct Bitmap<uint8_t> {
	uint8_t* data{nullptr};
	int width{0};
	int height{0};
	
	inline uint8_t GetPixel(int x, int y) {
		return data[x + y * width];
	}
	inline void SetPixel(const uint8_t& value, int x, int y) {
		data[x + y * width] = value;
	}
};

using BitmapRGBA = Bitmap<RGBA>;
using BitmapRGB = Bitmap<RGBA>;
using BitmapMonochrome = Bitmap<RGBA>;
*/