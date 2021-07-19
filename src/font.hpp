/********************************
 * Project: Cidr				*
 * File: font.hpp				*
 * Date: 12.2.2021				*
 ********************************/

#ifndef CIDR_FONT_HPP
#define CIDR_FONT_HPP

#include "bitmap.hpp"

namespace cdr {
	
class Font {
private:
	Bitmap fontSheet;
	int fontWidth;
	int fontHeight;
	
public: 
	Font(const uint8_t data[], int fontSheetWidth, int fontSheetHeight, int fontWidth, int fontHeight);
	
	inline const RGBA GetPixel(int x, int y) const {
		return fontSheet.GetPixel(x, y);
	}	
	inline int GetFontWidth() const { return fontWidth; }
	inline int GetFontHeight() const { return fontHeight; }
	inline int GetFontSheetWidth() const { return fontSheet.GetWidth(); }
	inline int GetFontSheetHeight() const { return fontSheet.GetHeight(); }
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

}

#endif