/********************************
 * Project: Cidr				*
 * File: bitmap.cpp				*
 * Date: 10.10.2020				*
 ********************************/

#include "bitmap.hpp"
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <stdexcept>


/* RGBABitmap *******************************************************************************/

Cidr::BaseBitmap::BaseBitmap(int width, int height, int numComponents) : 
	data{new uint32_t[width * height]}, width{width}, height{height}, components{numComponents} { 
	memset(data, 0, width * height * sizeof(uint32_t));
}
Cidr::BaseBitmap::BaseBitmap(uint32_t* source, int sourceWidth, int sourceHeight, int sourceComponents) :
	data{new uint32_t[sourceWidth * sourceHeight]}, width{sourceWidth}, height{sourceHeight}, components{sourceComponents} {
	memcpy(data, source, width * height * sizeof(uint32_t));
}
Cidr::BaseBitmap::BaseBitmap(const std::string& file, int reqComponents) {
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

Cidr::BaseBitmap::BaseBitmap(const BaseBitmap& other) : 
	data{new uint32_t[other.width * other.height]}, width{other.width}, height{other.height}, components{other.components} { 
	memcpy(data, other.data, width * height * sizeof(uint32_t));
}
Cidr::BaseBitmap& Cidr::BaseBitmap::operator=(const BaseBitmap& other) {
	delete[] data;
	this->width = other.width;
	this->height = other.height;
	this->components = other.components;
	data = new uint32_t[width * height];
	memcpy(data, other.data, width * height * sizeof(uint32_t));
	
	return *this;
}
Cidr::BaseBitmap::BaseBitmap(BaseBitmap&& other) noexcept : 
	data{other.data} , width{other.width}, height{other.height}, components{other.components} { 
	other.width = 0;
	other.height = 0;
	other.data = nullptr;
}
Cidr::BaseBitmap& Cidr::BaseBitmap::operator=(BaseBitmap&& other) noexcept {
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

Cidr::BaseBitmap::~BaseBitmap() {
	delete[] data;
}

void Cidr::BaseBitmap::SaveAs(const std::string& fileName, Formats format, int quality) {
	// NOTE: Cidr uses rgba, stbi uses abgr
	uint32_t* abgrData = new uint32_t[this->width * this->height];
	for (int i = 0; i < this->width * this->height; i++) {
		abgrData[i] = UINT_RGBAtoUINT_ABGR(data[i]);
	}
	
	switch(format) {
		case Formats::PNG:
			stbi_write_png(fileName.c_str(), GetWidth(), GetHeight(), this->components, abgrData, this->components * GetWidth());
			break;
		case Formats::BMP:
			stbi_write_bmp(fileName.c_str(), GetWidth(), GetHeight(), this->components, abgrData);
			break;
		case Formats::TGA:
			stbi_write_tga(fileName.c_str(), GetWidth(), GetHeight(), this->components, abgrData);
			break;
		case Formats::JPG:
			stbi_write_jpg(fileName.c_str(), GetWidth(), GetHeight(), this->components, abgrData, quality);
			break;
	}
	
	delete[] abgrData;
}


/* RGBABitmap *******************************************************************************/

Cidr::RGBABitmap::RGBABitmap(int width, int height) : BaseBitmap(width, height, 4) {}
Cidr::RGBABitmap::RGBABitmap(uint32_t* source, int sourceWidth, int sourceHeight) : BaseBitmap(source, sourceWidth, sourceHeight, 4) {}
Cidr::RGBABitmap::RGBABitmap(const std::string& file) : BaseBitmap(file, 4) {}

Cidr::RGBABitmap::RGBABitmap(const RGBABitmap& other) : BaseBitmap(other) {}
Cidr::RGBABitmap& Cidr::RGBABitmap::operator=(const RGBABitmap& other) {
	BaseBitmap::operator=(other);
	return *this;
}
Cidr::RGBABitmap::RGBABitmap(RGBABitmap&& other) noexcept : BaseBitmap(other) {}
Cidr::RGBABitmap& Cidr::RGBABitmap::operator=(RGBABitmap&& other) noexcept {
	BaseBitmap::operator=(other);
	return *this;
}
Cidr::RGBABitmap::~RGBABitmap() {}


/* RGBBitmap *******************************************************************************/

Cidr::RGBBitmap::RGBBitmap(int width, int height) : BaseBitmap(width, height, 3) {}
Cidr::RGBBitmap::RGBBitmap(uint32_t* source, int sourceWidth, int sourceHeight) : BaseBitmap(source, sourceWidth, sourceHeight, 4) {}
Cidr::RGBBitmap::RGBBitmap(const std::string& file) : BaseBitmap(file, 3) {}

Cidr::RGBBitmap::RGBBitmap(const RGBBitmap& other) : BaseBitmap(other) {}
Cidr::RGBBitmap& Cidr::RGBBitmap::operator=(const RGBBitmap& other) {
	BaseBitmap::operator=(other);
	return *this;
}
Cidr::RGBBitmap::RGBBitmap(RGBBitmap&& other) noexcept : BaseBitmap(other) {}
Cidr::RGBBitmap& Cidr::RGBBitmap::operator=(RGBBitmap&& other) noexcept {
	BaseBitmap::operator=(other);
	return *this;
}
Cidr::RGBBitmap::~RGBBitmap() {}


/* MonochromeBitmap *******************************************************************************/

// Cidr::MonochromeBitmap::MonochromeBitmap(int width, int height) : 
// 	data{new uint8_t[width * height]}, width{width}, height{height} { 
// 	memset(data, 0, width * height * sizeof(uint8_t));
// }
// Cidr::MonochromeBitmap::MonochromeBitmap(uint8_t* source, int sourceWidth, int sourceHeight) :
// 	data{new uint8_t[sourceWidth * sourceHeight]}, width{sourceWidth}, height{sourceHeight} {
// 	memcpy(data, source, width * height * sizeof(uint8_t));
// }
// Cidr::MonochromeBitmap::MonochromeBitmap(const std::string& file) {
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

// Cidr::MonochromeBitmap::MonochromeBitmap(const MonochromeBitmap& other)  : 
// 	data{new uint8_t[other.width * other.height]}, width{other.width}, height{other.height} { 
// 	memcpy(data, other.data, width * height * sizeof(uint8_t));
// }
// Cidr::MonochromeBitmap& Cidr::MonochromeBitmap::operator=(const MonochromeBitmap& other) {
// 	delete[] data;
// 	this->width = other.width;
// 	this->height = other.height;
// 	data = new uint8_t[width * height];
// 	memcpy(data, other.data, width * height * sizeof(uint8_t));
	
// 	return *this;
// }
// Cidr::MonochromeBitmap::MonochromeBitmap(MonochromeBitmap&& other) noexcept : 
// 	data{other.data} , width{other.width}, height{other.height} { 
// 	other.width = 0;
// 	other.height = 0;
// 	other.data = nullptr;
// }
// Cidr::MonochromeBitmap& Cidr::MonochromeBitmap::operator=(MonochromeBitmap&& other) noexcept {
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
// Cidr::MonochromeBitmap::~MonochromeBitmap() {
// 	delete[] data;
// }
