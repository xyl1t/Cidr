/********************************
 * Project: Cidr				*
 * File: bitmap.hpp				*
 * Date: 10.10.2020				*
 ********************************/

#ifndef CIDR_BITMAP_HPP
#define CIDR_BITMAP_HPP

#include <cstdint>
#include "color.hpp"
#include <type_traits>

namespace cdr {

class BaseBitmap {
protected:
	/* Individual pixels of the bitmap */
	uint32_t* data{nullptr};
	/* Width of the bitmap */
	int width{0};
	/* Height of the bitmap */
	int height{0};
	/* Num of components*/
	int components;
	
public:
	enum class Formats {
		PNG,
		BMP,
		TGA,
		JPG,
	};
	
	BaseBitmap(int width, int height, int numComponents = 4);
	BaseBitmap(uint32_t* source, int sourceWidth, int sourceHeight, int sourceComponents);
	BaseBitmap(std::string_view file, int reqComponents = 0);
	virtual ~BaseBitmap();

	BaseBitmap(const BaseBitmap& other);
	BaseBitmap& operator=(const BaseBitmap& other);
	BaseBitmap(BaseBitmap&& other) noexcept;
	BaseBitmap& operator=(BaseBitmap&& other) noexcept;

	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }
	inline uint32_t* GetData() const { return data; }
	inline uint32_t GetRawPixel(int x, int y) const { return data[x + y * width]; }
	inline void SetRawPixel(uint32_t value, int x, int y) { data[x + y * width] = value; }
	inline void SetRawPixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a, int x, int y) { data[x + y * width] = (r << 24) + (g << 16) + (b << 8) + a; }
	
	void SaveAs(const std::string& fileName, Formats format, int quality = 100);
};

class RGBABitmap : public BaseBitmap {
	static constexpr int components{4};
	
public:
	RGBABitmap(int width, int height);
	RGBABitmap(uint32_t* source, int sourceWidth, int sourceHeight);
	RGBABitmap(std::string_view file);
	~RGBABitmap();

	// copy constructor/assignment
	RGBABitmap(const RGBABitmap& other);
	RGBABitmap& operator=(const RGBABitmap& other);
	RGBABitmap(RGBABitmap&& other) noexcept;
	RGBABitmap& operator=(RGBABitmap&& other) noexcept;
	
	inline RGBA GetPixel(int x, int y) const {
		return RGBA{data[x + y * width]};
	}
	inline void SetPixel(const RGB& value, int x, int y) {
		data[x + y * width] = RGBtoUINT(value);
	}
};

class RGBBitmap : public BaseBitmap {
	static constexpr int components{3};
	
public:
	RGBBitmap(int width, int height);
	RGBBitmap(uint32_t* source, int sourceWidth, int sourceHeight);
	RGBBitmap(std::string_view file);
	~RGBBitmap();

	// copy constructor/assignment
	RGBBitmap(const RGBBitmap& other);
	RGBBitmap& operator=(const RGBBitmap& other);
	RGBBitmap(RGBBitmap&& other) noexcept;
	RGBBitmap& operator=(RGBBitmap&& other) noexcept;
	
	inline RGB GetPixel(int x, int y) const {
		return RGB{data[x + y * width]};
	}
	inline void SetPixel(const RGB& value, int x, int y) {
		data[x + y * width] = RGBtoUINT(value);
	}
};

// class MonochromeBitmap {
// private:
// 	/* Individual pixels of the bitmap */
// 	uint8_t* data{nullptr};
// 	/* Width of the bitmap */
// 	int width{0};
// 	/* Height of the bitmap */
// 	int height{0};
// public:
// 	MonochromeBitmap(int width, int height);
// 	MonochromeBitmap(uint8_t* source, int sourceWidth, int sourceHeight);
// 	MonochromeBitmap(std::string_view file);
// 	~MonochromeBitmap();

// 	// copy constructor/assignment
// 	MonochromeBitmap(const MonochromeBitmap& other);
// 	MonochromeBitmap& operator=(const MonochromeBitmap& other);
// 	MonochromeBitmap(MonochromeBitmap&& other) noexcept;
// 	MonochromeBitmap& operator=(MonochromeBitmap&& other) noexcept;

// 	inline int GetWidth() const  {
// 		return width;
// 	}
// 	inline int GetHeight() const {
// 		return height;
// 	}
// 	inline uint8_t* GetData() const {
// 		return data;
// 	}
	
// 	inline uint8_t GetPixel(int x, int y) {
// 		return data[x + y * width];
// 	}
// 	inline void SetPixel(const uint8_t& value, int x, int y) {
// 		data[x + y * width] = value;
// 	}
// };

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