/********************************
 * Project: Cidr				*
 * File: font.cpp				*
 * Date: 12.2.2021				*
 ********************************/

#include "font.hpp"

cdr::Font::Font(const uint8_t* data, int fontSheetWidth, int fontSheetHeight, int fontWidth, int fontHeight) : 
	fontSheet{fontSheetWidth, fontSheetHeight}, fontWidth{fontWidth}, fontHeight{fontHeight} {
		
	auto getCoordX = [](int val, int width) {
		return val % width;
	};
	auto getCoordY = [](int val, int width) {
		return val / width;
	};
	
	int SIZE = fontSheetWidth*fontSheetHeight;
	
	for(int i = 0; i < SIZE/8; i++) {
		uint8_t color8 = data[i];
		for(int j = 0; j < 8; j++) {
			uint8_t c = ((color8 >> j) & 1) * 255;
			
			fontSheet.SetPixel(cdr::RGBA{c, c, c, 255}, getCoordX(i*8 + j, fontSheetWidth), getCoordY(i*8 + j, fontSheetWidth));
		}
	}
}