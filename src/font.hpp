/********************************
 * Project: Cidr				*
 * File: font.hpp				*
 * Date: 12.2.2021				*
 ********************************/

#ifndef CIDR_FONT_HPP
#define CIDR_FONT_HPP

#include "bitmap.hpp"
#include <utility>
#include <vector>
#include <string_view>

namespace cdr {
	
class Font {
private:
	Bitmap fontSheet;
	int fontWidth;
	int fontHeight;
	std::vector<std::pair<int, int>> kernX;
	
public: 
	Font(const uint8_t data[], int fontSheetWidth, int fontSheetHeight, int fontWidth, int fontHeight);
	Font(std::string_view fontPath, int fontWidth, int fontHeight);
	Font(const Bitmap& fontSheet, int fontWidth, int fontHeight);
	
	inline const RGBA GetPixel(int x, int y) const {
		return fontSheet.GetPixel(x, y);
	}	
	inline int GetFontWidth() const { return fontWidth; }
	inline int GetFontHeight() const { return fontHeight; }
	inline int GetFontSheetWidth() const { return fontSheet.GetWidth(); }
	inline int GetFontSheetHeight() const { return fontSheet.GetHeight(); }
	inline int GetLeftKernel(int characterX, int characterY) const { return kernX[characterX + characterY * fontSheet.GetWidth() / fontWidth].first; }
	inline int GetRightKernel(int characterX, int characterY) const { return kernX[characterX + characterY * fontSheet.GetWidth() / fontWidth].second; }
};

// NOTE: 
// [T]op, [B]ottom [C]enter
// [R]ight, [L]eft
enum class TextAlignment {
	TL,	TC,	TR,
	CL,	CC,	CR,
	BL,	BC,	BR,
};
namespace Fonts {
extern const cdr::Font Raster8x16;
extern const cdr::Font Raster8x8;
extern const cdr::Font Raster10x10;
extern const cdr::Font Raster10x12;
extern const cdr::Font Raster8x12;
}

struct TextStyle {
	const Font* font;
	bool useKerning;
	TextAlignment ta;
	float size;
	RGBA fColor;
	RGBA bColor;
	RGBA shadowColor;
	int shadowOffsetX;
	int shadowOffsetY;
	
	TextStyle(const Font& font = cdr::Fonts::Raster8x12, bool useKerning = false, TextAlignment ta = TextAlignment::TL, float size = 1, const RGBA& fColor = RGB::White, const RGBA& bColor = RGBA::Transparent, const RGBA& shadowColor = RGBA::Transparent, int shadowOffsetX = 1, int shadowOffsetY = 1);
	TextStyle(const TextStyle& other);
	TextStyle& operator=(const TextStyle& other);
};

extern const TextStyle DefaultTextStyle;

}

#endif