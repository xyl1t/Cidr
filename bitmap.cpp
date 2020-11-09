/********************************
 * Project: Cidr				*
 * File: bitmap.cpp				*
 * Date: 10.10.2020				*
 ********************************/

#include "bitmap.hpp"
#include <stb/stb_image.h>



/* RGBABitmap *******************************************************************************/

Cidr::RGBABitmap::RGBABitmap(int width, int height) : 
	data{new uint32_t[width * height]}, width{width}, height{height} { 
	memset(data, 0, width * height * sizeof(uint32_t));
}
Cidr::RGBABitmap::RGBABitmap(uint32_t* source, int sourceWidth, int sourceHeight) :
	data{new uint32_t[sourceWidth * sourceHeight]}, width{sourceWidth}, height{sourceHeight} {
	memcpy(data, source, width * height * sizeof(uint32_t));
}
Cidr::RGBABitmap::RGBABitmap(const std::string& file) {
	int channels;
	uint8_t* imageData = stbi_load(file.c_str(), &this->width, &this->height, &channels, 0);
	if(imageData) {
		data = new uint32_t[width * height];
		for(int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				// NOTE: this handles the cases where the image is monochrome, rgb or rgba (and maybe other cases, haven't tested)
				data[i + j * width] = 
					(imageData[(i + j * width) * channels + 0] << 24) + 
					(imageData[(i + j * width) * channels + 1] << 16) + 
					(imageData[(i + j * width) * channels + 2] <<  8) + 
					((channels == 4) ? (imageData[(i + j * width) * channels + 3] <<  0) : 0xff);
				
			}
		}
		stbi_image_free(imageData);
	}
}

Cidr::RGBABitmap::RGBABitmap(const RGBABitmap& other)  : 
	data{new uint32_t[other.width * other.height]}, width{other.width}, height{other.height} { 
	memcpy(data, other.data, width * height * sizeof(uint32_t));
}
Cidr::RGBABitmap& Cidr::RGBABitmap::operator=(const RGBABitmap& other) {
	delete[] data;
	this->width = other.width;
	this->height = other.height;
	data = new uint32_t[width * height];
	memcpy(data, other.data, width * height * sizeof(uint32_t));
	
	return *this;
}
Cidr::RGBABitmap::RGBABitmap(RGBABitmap&& other) noexcept : 
	data{other.data} , width{other.width}, height{other.height} { 
	other.width = 0;
	other.height = 0;
	other.data = nullptr;
}
Cidr::RGBABitmap& Cidr::RGBABitmap::operator=(RGBABitmap&& other) noexcept {
	if(this == &other) return *this;
	
	delete[] data;
	this->width = other.width;
	this->height = other.height;
	data = other.data;
	other.width = 0;
	other.height = 0;
	other.data = nullptr;
	
	return *this;
}
Cidr::RGBABitmap::~RGBABitmap() {
	delete[] data;
}



/* RGBBitmap *******************************************************************************/

Cidr::RGBBitmap::RGBBitmap(int width, int height) : 
	data{new uint32_t[width * height]}, width{width}, height{height} { 
	memset(data, 0, width * height * sizeof(uint32_t));
}
Cidr::RGBBitmap::RGBBitmap(uint32_t* source, int sourceWidth, int sourceHeight) :
	data{new uint32_t[sourceWidth * sourceHeight]}, width{sourceWidth}, height{sourceHeight} {
	memcpy(data, source, width * height * sizeof(uint32_t));
}
Cidr::RGBBitmap::RGBBitmap(const std::string& file) {
	int channels;
	uint8_t* imageData = stbi_load(file.c_str(), &this->width, &this->height, &channels, 0);
	if(imageData) {
		data = new uint32_t[width * height];
		for(int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				data[i + j * width] = 
					(imageData[(i + j * width) * channels + 0] << 24) + 
					(imageData[(i + j * width) * channels + 1] << 16) + 
					(imageData[(i + j * width) * channels + 2] <<  8) + 
					0xff;
			}
		}
		stbi_image_free(imageData);
	}
}

Cidr::RGBBitmap::RGBBitmap(const RGBBitmap& other)  : 
	data{new uint32_t[other.width * other.height]}, width{other.width}, height{other.height} { 
	memcpy(data, other.data, width * height * sizeof(uint32_t));
}
Cidr::RGBBitmap& Cidr::RGBBitmap::operator=(const RGBBitmap& other) {
	delete[] data;
	this->width = other.width;
	this->height = other.height;
	data = new uint32_t[width * height];
	memcpy(data, other.data, width * height * sizeof(uint32_t));
	
	return *this;
}
Cidr::RGBBitmap::RGBBitmap(RGBBitmap&& other) noexcept : 
	data{other.data} , width{other.width}, height{other.height} { 
	other.width = 0;
	other.height = 0;
	other.data = nullptr;
}
Cidr::RGBBitmap& Cidr::RGBBitmap::operator=(RGBBitmap&& other) noexcept {
	if(this == &other) return *this;
	
	delete[] data;
	this->width = other.width;
	this->height = other.height;
	data = other.data;
	other.width = 0;
	other.height = 0;
	other.data = nullptr;
	
	return *this;
}
Cidr::RGBBitmap::~RGBBitmap() {
	delete[] data;
}



/* MonochromeBitmap *******************************************************************************/

Cidr::MonochromeBitmap::MonochromeBitmap(int width, int height) : 
	data{new uint8_t[width * height]}, width{width}, height{height} { 
	memset(data, 0, width * height * sizeof(uint8_t));
}
Cidr::MonochromeBitmap::MonochromeBitmap(uint8_t* source, int sourceWidth, int sourceHeight) :
	data{new uint8_t[sourceWidth * sourceHeight]}, width{sourceWidth}, height{sourceHeight} {
	memcpy(data, source, width * height * sizeof(uint8_t));
}
Cidr::MonochromeBitmap::MonochromeBitmap(const std::string& file) {
	int channels;
	uint8_t* imageData = stbi_load(file.c_str(), &this->width, &this->height, &channels, 0);
	if(imageData) {
		data = new uint8_t[width * height];
		for(int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {
				// TODO: make monochrome bitmap even if source image is not monochrome.
				// see: https://stackoverflow.com/q/596216/10187214
				if(channels == 1) {
					data[i + j * width] = imageData[i + j * width];
				} else {
					RGB color {
						(imageData[(i + j * width) * channels + 0] << 24),
						(imageData[(i + j * width) * channels + 1] << 16),
						(imageData[(i + j * width) * channels + 2] <<  8),
					};
					uint8_t monochromeColor {
						color.r * 0.2126 + color.b * 0.7152 + color.g * 0.0722,
					};
					data[i + j * width] = monochromeColor;
				}
			}
		}
		stbi_image_free(imageData);
	}
}

Cidr::MonochromeBitmap::MonochromeBitmap(const MonochromeBitmap& other)  : 
	data{new uint8_t[other.width * other.height]}, width{other.width}, height{other.height} { 
	memcpy(data, other.data, width * height * sizeof(uint8_t));
}
Cidr::MonochromeBitmap& Cidr::MonochromeBitmap::operator=(const MonochromeBitmap& other) {
	delete[] data;
	this->width = other.width;
	this->height = other.height;
	data = new uint8_t[width * height];
	memcpy(data, other.data, width * height * sizeof(uint8_t));
	
	return *this;
}
Cidr::MonochromeBitmap::MonochromeBitmap(MonochromeBitmap&& other) noexcept : 
	data{other.data} , width{other.width}, height{other.height} { 
	other.width = 0;
	other.height = 0;
	other.data = nullptr;
}
Cidr::MonochromeBitmap& Cidr::MonochromeBitmap::operator=(MonochromeBitmap&& other) noexcept {
	if(this == &other) return *this;
	
	delete[] data;
	this->width = other.width;
	this->height = other.height;
	data = other.data;
	other.width = 0;
	other.height = 0;
	other.data = nullptr;
	
	return *this;
}
Cidr::MonochromeBitmap::~MonochromeBitmap() {
	delete[] data;
}
