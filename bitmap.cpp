/********************************
 * Project: Cidr				*
 * File: bitmap.cpp				*
 * Date: 10.10.2020				*
 ********************************/

#include "bitmap.hpp"
#include <stb/stb_image.h>

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
	data = new uint32_t[width * height];
	for(int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			/* this handles the cases where the image is monochrome, rgb or rgba (and maybe other cases, haven't tested) */
			int offset = 0;
			data[i + j * width] = 
				(imageData[(i + j * width) * channels + offset++] << 24) + 
				(imageData[(i + j * width) * channels + offset++] << 16) + 
				(imageData[(i + j * width) * channels + offset++] <<  8) + 
				((channels == 4) ? (imageData[(i + j * width) * channels + offset++] <<  0) : 0xff);
		}
	}
	stbi_image_free(imageData);
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