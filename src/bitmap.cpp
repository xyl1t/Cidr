/********************************
 * Project: Cidr				*
 * File: bitmap.cpp				*
 * Date: 10.10.2020				*
 ********************************/

#include "bitmap.hpp"
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <stdexcept>
#include <cstring>


/* RGBABitmap *******************************************************************************/

cdr::BaseBitmap::BaseBitmap(int width, int height, int numComponents) : 
	data{new uint32_t[width * height]}, width{width}, height{height}, components{numComponents} { 
	memset(data, 0, width * height * sizeof(uint32_t));
}
cdr::BaseBitmap::BaseBitmap(uint32_t* source, int sourceWidth, int sourceHeight, int sourceComponents) :
	data{new uint32_t[sourceWidth * sourceHeight]}, width{sourceWidth}, height{sourceHeight}, components{sourceComponents} {
	memcpy(data, source, width * height * sizeof(uint32_t));
}
cdr::BaseBitmap::BaseBitmap(const std::string& file, int reqComponents) {
	uint8_t* imageData = stbi_load(file.c_str(), &this->width, &this->height, &this->components, reqComponents);
	this->components = reqComponents;
	if(imageData) {
		data = new uint32_t[width * height];
		for(int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				// NOTE: this handles the cases where the image is monochrome, rgb or rgba (and maybe other cases, haven't tested)
				data[i + j * width] = 
					(imageData[(i + j * width) * components + 0] << 24) + 
					(imageData[(i + j * width) * components + 1] << 16) + 
					(imageData[(i + j * width) * components + 2] <<  8) + 
					((components == 4) ? (imageData[(i + j * width) * components + 3] <<  0) : 0xff);
			}
		}
		stbi_image_free(imageData);
	} else {
		throw std::runtime_error("Cidr: Bitmap not found (" + file + ")");
	}
}

cdr::BaseBitmap::BaseBitmap(const BaseBitmap& other) : 
	data{new uint32_t[other.width * other.height]}, width{other.width}, height{other.height}, components{other.components} { 
	memcpy(data, other.data, width * height * sizeof(uint32_t));
}
cdr::BaseBitmap& cdr::BaseBitmap::operator=(const BaseBitmap& other) {
	delete[] data;
	this->width = other.width;
	this->height = other.height;
	this->components = other.components;
	data = new uint32_t[width * height];
	memcpy(data, other.data, width * height * sizeof(uint32_t));
	
	return *this;
}
cdr::BaseBitmap::BaseBitmap(BaseBitmap&& other) noexcept : 
	data{other.data} , width{other.width}, height{other.height}, components{other.components} { 
	other.width = 0;
	other.height = 0;
	other.data = nullptr;
}
cdr::BaseBitmap& cdr::BaseBitmap::operator=(BaseBitmap&& other) noexcept {
	if(this == &other) return *this;
	
	delete[] data;
	this->width = other.width;
	this->height = other.height;
	this->components = other.components;
	data = other.data;
	other.width = 0;
	other.height = 0;
	other.components = 0;
	other.data = nullptr;
	
	return *this;
}

cdr::BaseBitmap::~BaseBitmap() {
	delete[] data;
}

// provie filename without extension!
void cdr::BaseBitmap::SaveAs(const std::string& fileName, Formats format, int quality) {
	// NOTE: Cidr uses rgba, stbi uses abgr
	uint32_t* abgrData = new uint32_t[this->width * this->height];
	for (int i = 0; i < this->width * this->height; i++) {
		abgrData[i] = UINT_RGBAtoUINT_ABGR(data[i]);
	}
	
	// NOTE: Extension added depending on format argument 
	switch(format) {
		case Formats::PNG:
			stbi_write_png((fileName + ".png").c_str(), GetWidth(), GetHeight(), this->components, abgrData, this->components * GetWidth());
			break;
		case Formats::BMP:
			stbi_write_bmp((fileName + ".bmp").c_str(), GetWidth(), GetHeight(), this->components, abgrData);
			break;
		case Formats::TGA:
			stbi_write_tga((fileName + ".tga").c_str(), GetWidth(), GetHeight(), this->components, abgrData);
			break;
		case Formats::JPG:
			stbi_write_jpg((fileName + ".jpg").c_str(), GetWidth(), GetHeight(), this->components, abgrData, quality);
			break;
	}
	
	delete[] abgrData;
}


/* RGBABitmap *******************************************************************************/

cdr::RGBABitmap::RGBABitmap(int width, int height) : BaseBitmap(width, height, 4) {}
cdr::RGBABitmap::RGBABitmap(uint32_t* source, int sourceWidth, int sourceHeight) : BaseBitmap(source, sourceWidth, sourceHeight, 4) {}
cdr::RGBABitmap::RGBABitmap(const std::string& file) : BaseBitmap(file, 4) {}

cdr::RGBABitmap::RGBABitmap(const RGBABitmap& other) : BaseBitmap(other) {}
cdr::RGBABitmap& cdr::RGBABitmap::operator=(const RGBABitmap& other) {
	BaseBitmap::operator=(other);
	return *this;
}
cdr::RGBABitmap::RGBABitmap(RGBABitmap&& other) noexcept : BaseBitmap(other) {}
cdr::RGBABitmap& cdr::RGBABitmap::operator=(RGBABitmap&& other) noexcept {
	BaseBitmap::operator=(other);
	return *this;
}
cdr::RGBABitmap::~RGBABitmap() {}


/* RGBBitmap *******************************************************************************/

cdr::RGBBitmap::RGBBitmap(int width, int height) : BaseBitmap(width, height, 3) {}
cdr::RGBBitmap::RGBBitmap(uint32_t* source, int sourceWidth, int sourceHeight) : BaseBitmap(source, sourceWidth, sourceHeight, 4) {}
cdr::RGBBitmap::RGBBitmap(const std::string& file) : BaseBitmap(file, 3) {}

cdr::RGBBitmap::RGBBitmap(const RGBBitmap& other) : BaseBitmap(other) {}
cdr::RGBBitmap& cdr::RGBBitmap::operator=(const RGBBitmap& other) {
	BaseBitmap::operator=(other);
	return *this;
}
cdr::RGBBitmap::RGBBitmap(RGBBitmap&& other) noexcept : BaseBitmap(other) {}
cdr::RGBBitmap& cdr::RGBBitmap::operator=(RGBBitmap&& other) noexcept {
	BaseBitmap::operator=(other);
	return *this;
}
cdr::RGBBitmap::~RGBBitmap() {}


/* MonochromeBitmap *******************************************************************************/

// cdr::MonochromeBitmap::MonochromeBitmap(int width, int height) : 
// 	data{new uint8_t[width * height]}, width{width}, height{height} { 
// 	memset(data, 0, width * height * sizeof(uint8_t));
// }
// cdr::MonochromeBitmap::MonochromeBitmap(uint8_t* source, int sourceWidth, int sourceHeight) :
// 	data{new uint8_t[sourceWidth * sourceHeight]}, width{sourceWidth}, height{sourceHeight} {
// 	memcpy(data, source, width * height * sizeof(uint8_t));
// }
// cdr::MonochromeBitmap::MonochromeBitmap(const std::string& file) {
// 	int channels;
// 	uint8_t* imageData = stbi_load(file.c_str(), &this->width, &this->height, &channels, 0);
// 	if(imageData) {
// 		data = new uint8_t[width * height];
// 		for(int i = 0; i < width; i++) {
// 			for (int j = 0; j < height; j++) {
// 				// TODO: make monochrome bitmap even if source image is not monochrome.
// 				// see: https://stackoverflow.com/q/596216/10187214
// 				if(channels == 1) {
// 					data[i + j * width] = imageData[i + j * width];
// 				} else {
// 					RGB color {
// 						(uint8_t)((imageData[(i + j * width) * channels + 0] << 24)),
// 						(uint8_t)((imageData[(i + j * width) * channels + 1] << 16)),
// 						(uint8_t)((imageData[(i + j * width) * channels + 2] <<  8)),
// 					};
// 					uint8_t monochromeColor {
// 						(uint8_t)(color.r * 0.2126 + color.b * 0.7152 + color.g * 0.0722),
// 					};
// 					data[i + j * width] = (uint8_t)monochromeColor;
// 				}
// 			}
// 		}
// 		stbi_image_free(imageData);
// 	}
// }

// cdr::MonochromeBitmap::MonochromeBitmap(const MonochromeBitmap& other)  : 
// 	data{new uint8_t[other.width * other.height]}, width{other.width}, height{other.height} { 
// 	memcpy(data, other.data, width * height * sizeof(uint8_t));
// }
// cdr::MonochromeBitmap& cdr::MonochromeBitmap::operator=(const MonochromeBitmap& other) {
// 	delete[] data;
// 	this->width = other.width;
// 	this->height = other.height;
// 	data = new uint8_t[width * height];
// 	memcpy(data, other.data, width * height * sizeof(uint8_t));
	
// 	return *this;
// }
// cdr::MonochromeBitmap::MonochromeBitmap(MonochromeBitmap&& other) noexcept : 
// 	data{other.data} , width{other.width}, height{other.height} { 
// 	other.width = 0;
// 	other.height = 0;
// 	other.data = nullptr;
// }
// cdr::MonochromeBitmap& cdr::MonochromeBitmap::operator=(MonochromeBitmap&& other) noexcept {
// 	if(this == &other) return *this;
	
// 	delete[] data;
// 	this->width = other.width;
// 	this->height = other.height;
// 	data = other.data;
// 	other.width = 0;
// 	other.height = 0;
// 	other.data = nullptr;
	
// 	return *this;
// }
// cdr::MonochromeBitmap::~MonochromeBitmap() {
// 	delete[] data;
// }
