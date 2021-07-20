/********************************
 * Project: Cidr                *
 * File: main.cpp               *
 * Date: 10.9.2020              *
 ********************************/

#if __has_include("SDL2/SDL.h")
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include <iostream>
#include <map>
#include <string>
#include "src/timer.hpp"

#include <cinttypes>
#include <utility>
#include <vector>
#include <cstdio>
#include <fstream>

#define CIDR_IMPLEMENTATION
#ifndef NDEBUG
#include "src/renderer.hpp"
#else
#include "cidr.hpp"
#endif

cdr::RGBA testShader(const cdr::Renderer& renderer, int x, int y);
cdr::RGBA blurShader(const cdr::Renderer& renderer, int x, int y);
cdr::RGBA hBlurShader(const cdr::Renderer& renderer, int x, int y);
cdr::RGBA vBlurShader(const cdr::Renderer& renderer, int x, int y);
cdr::RGBA hsvHueRotationShader(const cdr::Renderer& renderer, int x, int y);
cdr::RGBA distortionShader(const cdr::Renderer& renderer, int x, int y);
cdr::RGBA grayScaleShader(const cdr::Renderer& renderer, int x, int y);

int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_VIDEO);
	srand(time(NULL));
	
	float zoom {1};

	if(argc != 1 && std::atoi(argv[1]) >= 1 ) {
		zoom = std::atof(argv[1]);
	}
	
	const int CANVAS_WIDTH = 500;
	const int CANVAS_HEIGHT = 500;
	const int WINDOW_WIDTH = CANVAS_WIDTH * zoom;
	const int WINDOW_HEIGHT = CANVAS_HEIGHT * zoom;
	
	SDL_Window* window = SDL_CreateWindow("Cidr Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, CANVAS_WIDTH, CANVAS_HEIGHT);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	
	uint32_t* pixels = new uint32_t[CANVAS_WIDTH * CANVAS_HEIGHT];
	memset(pixels, 0, CANVAS_WIDTH * CANVAS_HEIGHT * sizeof(uint32_t));
	
	cdr::Renderer cidrRend {pixels, CANVAS_WIDTH, CANVAS_HEIGHT};
	cidrRend.EnableAlphaBlending();
	
	SDL_Event event;
	bool alive = true;
	std::map<int, bool> keyboard;
	int mx, my;
	bool showText = true;
	constexpr int shaderRectSize = 128;
	uint32_t current = SDL_GetTicks();
	uint32_t old = 0;
	uint32_t timer = 0;
	
	cdr::Bitmap savingBitmap{256, 256};
	cdr::Renderer savingBitmapRenderer{savingBitmap.GetData(), savingBitmap.GetWidth(), savingBitmap.GetHeight()};
	for (uint32_t x = 0; x < 256; x++) {
		for (uint32_t y = 0; y < 256; y++) {
			cdr::RGBA c1((x^y) | ~x, (x) & ~y, (y) | x);
			cdr::RGBA c2(((y+x)*2) >> (~x&y), ((y+x)*4) >> (x&y), ((y+x)*8) >> (x&~y));
			savingBitmap.SetRawPixel(
				(y & (x^~y)) >> (~x&~y) | ((y & (x^~y))),
				(x & (x^y) ) >> (~x& y) | ((x & (x^y) )),
				(y & (x^y) ) >> ( x&~y) | ((y & (x^y) )),
				0xff, x, y);
		}
	}
	
	savingBitmapRenderer.DrawText("Cidr", 132, 128, {cdr::Fonts::Raster8x16, cdr::TextAlignment::CC, 4, cdr::RGB::White, cdr::RGBA::Transparent, cdr::RGB::Black});
	
	savingBitmap.SaveAs("savingBitmap.png", cdr::Bitmap::Formats::PNG);
	
	cdr::RGBA (*currentShader)(const cdr::Renderer& renderer, int x, int y) {nullptr};
	cdr::Bitmap bitmap{"../res/pureTest.png"};
	cdr::Bitmap triangleTexture{"../res/treeImg.png"};
	
	
	float val1{};
	float val2{};
	// float val3{};
	// float val4{};
	Timer t {};
	while(alive) {
		/* TIMER */
		old = current;
		current = t.elapsed();
		if(timer > 1000) {
			// std::cout << "ms: " << (current - old) / 1000000.f << '\n';
			timer = 0; 
		}
		timer += (current - old) / 1000000.f;
		
		
		/* EVENTS */
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				alive = false;
			}
			switch(event.type) {
			case SDL_KEYDOWN:
				keyboard[event.key.keysym.sym] = true;
				break;
			case SDL_KEYUP:
				keyboard[event.key.keysym.sym] = false;
				break;
			}
			SDL_GetMouseState(&mx, &my);
			mx /= zoom;
			my /= zoom;
			// mx-=3;
			// my-=3;
		}

		/* SELECT SHADER */
		if(keyboard[SDLK_1]) {
			currentShader = &distortionShader;
		}
		if(keyboard[SDLK_2]) {
			currentShader = &hsvHueRotationShader;
		}
		if(keyboard[SDLK_3]) {
			currentShader = &blurShader;
		}
		if(keyboard[SDLK_4]) {
			currentShader = &testShader;
		}
		if(keyboard[SDLK_5]) {
			cidrRend.OutOfBoundsType = cdr::Renderer::OutOfBoundsType::Repeat;
			currentShader = nullptr;
		}
		if(keyboard[SDLK_6]) {
			cidrRend.OutOfBoundsType = cdr::Renderer::OutOfBoundsType::MirroredRepeat;
			currentShader = nullptr;
		}
		if(keyboard[SDLK_7]) {
			cidrRend.OutOfBoundsType = cdr::Renderer::OutOfBoundsType::ClampToBorder;
			currentShader = nullptr;
		}
		if(keyboard[SDLK_8]) {
			cidrRend.OutOfBoundsType = cdr::Renderer::OutOfBoundsType::ClampToEdge;
			currentShader = nullptr;
		}
		if(keyboard[SDLK_9]) {
			cidrRend.ScaleType = cdr::Renderer::ScaleType::Nearest;
			currentShader = nullptr;
		}
		if(keyboard[SDLK_0]) {
			cidrRend.ScaleType = cdr::Renderer::ScaleType::Linear;
			currentShader = nullptr;
		}
		if(keyboard[SDLK_UP]) {
			val1 += 1;
		}
		if(keyboard[SDLK_DOWN]) {
			val1 -= 1;
		}
		if(keyboard[SDLK_LEFT]) {
			val2 -= 1;
		}
		if(keyboard[SDLK_RIGHT]) {
			val2 += 1;
		}
		if(keyboard[SDLK_t]) {
			showText = false;
		}
		if(keyboard[SDLK_t] && keyboard[SDLK_LSHIFT]) {
			showText = true;
		}
		
		/*** DRAWING ***/
		
		/* CLEARING THE SCREEN*/
		cidrRend.Clear(0x120F1FFF);
		
		
		/* DRAWING A LINE TO THE CURSOR */
		// cidrRend.DrawLine({0, 0xff, 0}, 0, 0, mx, my, true);
		
		
		/* DRAWING A COLORFUL RECTANGLE USING A SHADER */
		auto lambda = [](const cdr::Renderer& renderer, int x, int y) -> cdr::RGBA {
			return {
				static_cast<uint8_t>(x - 3), 
				static_cast<uint8_t>(y - 3), 
				static_cast<uint8_t>(255 - (x - 3))};
		};
		cidrRend.FillRectangle(lambda, cdr::Rectangle{3, 3, 256, 256});
		
		
		/* DRAWING THREE TYPES OF LINES */
		cidrRend.DrawLine(0x00ff00ff, 280, 230-10, 460, 270-10, true, true);
		cidrRend.DrawLine(0x00ff00ff, 280, 230,    460, 270);
		cidrRend.DrawLine(0x00ff00ff, 280, 230+10, 460, 270+10, true);
		
		
		/* DRAWING FILLED RGB SQUARES */
		cidrRend.FillRectangle(0xff0000ff, 152,300+5,40,40);
		cidrRend.FillRectangle(0x00ff00ff, 172,320+5,40,40);
		cidrRend.FillRectangle(0x0000ffff, 192,340+5,40,40);
		
		
		/* CIRCLES */
		cidrRend.DrawCircle(0x23ff10ff, 192, 340, 50);
		int circlesCount = 7;
		for(int i = 0; i < circlesCount; i++) {
			cdr::RGB color{0xff, 0xff, 0xff};
			if(i % 2) {
				color = {};
			}
			if(i == circlesCount - 1) {
				color = {0xff0000ff};
			}
			cidrRend.FillCircle(color, 64, 340, (circlesCount - i) / (float)circlesCount * 50, true);
		}
		cidrRend.FillCircle(0x30ee0Aff, 345, 64, 30, true);
		cidrRend.FillCircle(0x30ee0Aff, 345 + 30*2 + 15, 64, 30, false);


		/* TRIANGLES */
		cidrRend.FillTriangle(
			cdr::RGB::Red, 
			cdr::RGB::Green, 
			cdr::RGB::Blue, 
			 0 + 310,  0 + 128,
			64 + 310, 32 + 128,
			32 + 310, 64 + 128);
		cidrRend.DrawTriangle(cdr::RGB::White, 
			 0 + 310, 0 + 128,
			64 + 310,32 + 128,
			32 + 310,64 + 128, true, true);
		
		cidrRend.FillTriangle(
			cdr::RGB::Red,
			cdr::RGB::Green,
			cdr::RGB::Blue,
			400, 		128,
			400, 		128 + 64,
			400 + 32, 	128 + 32);
		cidrRend.FillTriangle(
			cdr::RGB::Green,
			cdr::RGB::Blue,
			cdr::RGB::Red,
			400 + 64, 	128,
			400 + 32, 	128 + 32,
			400 + 64, 	128 + 64);
		cidrRend.FillTriangle(
			cdr::RGB::Green,
			cdr::RGB::Blue,
			cdr::RGB::Red,
			400, 		128 + 64,
			400 + 32, 	128 + 32,
			400 + 64, 	128 + 64);
		cidrRend.FillTriangle(
			cdr::RGB::Red,
			cdr::RGB::Green,
			cdr::RGB::Blue,
			400, 		128,
			400 + 64, 	128,
			400 + 32, 	128 + 32);
		
		/* IMAGES */
		cidrRend.ClampToBorderColor = cdr::RGB::Gray;
		int destWidth = bitmap.GetWidth() * 6;
		int destHeight = bitmap.GetWidth() * 6;
		cidrRend.DrawBitmap(bitmap, 
			(float)360-destWidth/2.f,(float)340-destHeight/2.f, destWidth, destHeight, 
			val2/4.f, -val1/4.f, bitmap.GetWidth(), bitmap.GetHeight());


		/* TEXTURED TRIANGLE */
		cdr::Point p1{32, 48};
		cdr::Point p2{32, 256};
		cdr::Point p3{256-32, 256};
		cdr::Point p4{256-32, 48};
		cdr::Point pInBetween {(p1.x + p2.x + p3.x + p4.x) / 4, (p1.y + p2.y + p3.y + p4.y) / 4};
		
		cidrRend.DrawTriangle(triangleTexture, 
			0 + val2/100.f, 0 - val1/100.f,
			0 + val2/100.f, 1 - val1/100.f,
			1 + val2/100.f, 1 - val1/100.f,
			p1.x, p1.y,
			p2.x, p2.y,
			p3.x, p3.y);
		cidrRend.DrawTriangle(triangleTexture, 
			0 + val2/100.f, 0 - val1/100.f,
			1 + val2/100.f, 1 - val1/100.f,
			1 + val2/100.f, 0 - val1/100.f,
			p1.x, p1.y,
			p3.x, p3.y,
			p4.x, p4.y);
		
		
		/* TEXT */
		cidrRend.SetTextFont(cdr::Fonts::Raster8x16);
		if(showText) {
			cidrRend.DrawText("textured triangles", pInBetween.x - (8 * 18 / 2), pInBetween.y - 32);
			cidrRend.DrawText("shaded rectangle", 64, 16);
			cidrRend.DrawText("balls", 365, 16);
			cidrRend.DrawText("triangles", 350, 110);
			cidrRend.DrawText("lines", 365, 220);
			cidrRend.DrawText("combinations", 80, 270);
			cidrRend.DrawText("images", 335, 270);
			if(currentShader != nullptr) {
				std::string shaderText = "shader";
				cidrRend.DrawText(shaderText, mx-shaderRectSize - (cdr::Fonts::Raster8x16.GetFontWidth() * shaderText.length()) / 2 + shaderRectSize/2, my-shaderRectSize-cdr::Fonts::Raster8x16.GetFontHeight());
			}
		}
		
		int seed = 6123434;
		auto getRandomColor = [&seed](int min = 0, int max = 255) -> cdr::RGBA {
			seed += 3489;
			return cdr::RGBA{(uint8_t)std::clamp(seed*2378%255, min, max), (uint8_t)std::clamp(seed*899%255, min, max), (uint8_t)std::clamp(seed*328%255,min,max)};
		};
		
		//#0CD848 nicegreen 
		
		int wordCount = 0;
		for(const std::string& word : {	"Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur", "adipiscing", "elit,"	}) {
			cidrRend.DrawText(word, 16 + wordCount * cdr::Fonts::Raster8x16.GetFontWidth(), 420, 
				{cdr::Fonts::Raster8x16, cdr::TextAlignment::TL, 1, getRandomColor(128, 255), getRandomColor(0, 64), cdr::RGBA::Black, 1, 1});
			wordCount += word.length();
			cidrRend.DrawText(" ", 16 + wordCount * cdr::Fonts::Raster8x16.GetFontWidth(), 420);
			wordCount++;
		}
		wordCount = 0;
		for(const std::string& word : { "sed", "do", "eiusmod", "tempor", "incididunt", "labore", "et", "dolore", "aliqua." }) {
			cidrRend.DrawText(word, 16 + wordCount * cdr::Fonts::Raster8x16.GetFontWidth(), 420 + cdr::Fonts::Raster8x16.GetFontHeight(), 
				{cdr::Fonts::Raster8x16, cdr::TextAlignment::TL, 1, getRandomColor(128, 255), getRandomColor(0, 64), cdr::RGBA::Black, 1, 1});
			wordCount += word.length();
			cidrRend.DrawText(" ", 16 + wordCount * cdr::Fonts::Raster8x16.GetFontWidth(), 420 + cdr::Fonts::Raster8x16.GetFontHeight());
			wordCount++;
		}
		
		/* APPLY SHADER */
		if(currentShader != nullptr) {
			if(currentShader == &blurShader) {
				cidrRend.FillRectangle(&hBlurShader, mx-shaderRectSize, my-shaderRectSize, shaderRectSize, shaderRectSize);
				cidrRend.FillRectangle(&vBlurShader, mx-shaderRectSize, my-shaderRectSize, shaderRectSize, shaderRectSize);
			}
			else {
				cidrRend.FillRectangle(currentShader, mx-shaderRectSize, my-shaderRectSize, shaderRectSize, shaderRectSize);				
			}
			cidrRend.DrawRectangle({0xe0, 0xef, 0xff}, mx-shaderRectSize, my-shaderRectSize, shaderRectSize, shaderRectSize);
		}
		
		/* UPDATE SCREEN */
		SDL_UpdateTexture(texture, nullptr, pixels, CANVAS_WIDTH * sizeof(uint32_t));
		SDL_RenderClear(renderer);
		SDL_Rect rect1{0,0, CANVAS_WIDTH, CANVAS_HEIGHT};
		SDL_Rect rect2{0,0, WINDOW_WIDTH, WINDOW_HEIGHT};
		SDL_RenderCopy(renderer, texture, &rect1, &rect2);
		SDL_RenderPresent(renderer);
	}
	
	SDL_Quit();
	
	return 0;
}

cdr::RGBA testShader(const cdr::Renderer& renderer, int x, int y) { 
	cdr::RGBA finalColor{};
	const cdr::RGBA& currentPixel = renderer.GetPixel(x,y);

	finalColor.r = 255 - currentPixel.r;
	finalColor.g = 255 - currentPixel.g;
	finalColor.b = 255 - currentPixel.b;
	
	return finalColor;
}

cdr::RGBA grayScaleShader(const cdr::Renderer& renderer, int x, int y) { 
	cdr::RGBA finalColor{};
	const cdr::RGBA& currentPixel = renderer.GetPixel(x,y);

	finalColor.r = finalColor.g = finalColor.b = 
		0.2126 * currentPixel.r + 0.7152 * currentPixel.g + 0.0722 * currentPixel.b;
	
	return finalColor;
}

cdr::RGBA hsvHueRotationShader(const cdr::Renderer& renderer, int x, int y) {
	cdr::RGBA finalColor{};
	const cdr::RGBA& currentPixel = renderer.GetPixel(x,y);

	static double v {};
	v += 0.00005;
	if(v > 360) v = 0;

	cdr::HSV temp = cdr::RGBtoHSV(currentPixel); 
	temp.setH(temp.getH() + v);
	finalColor = cdr::HSVtoRGB(temp);
	
	return finalColor;
}
cdr::RGBA blurShader(const cdr::Renderer& renderer, int x, int y) {
	cdr::RGBA finalColor{};
	int blurSize = 3;
	int rTotal{0}, gTotal{0}, bTotal{0};
	float total = 0;
	
	for(int bx = -blurSize; bx <= blurSize; bx++) {	
		for(int by = -blurSize; by <= blurSize; by++) {
			int fx = bx + x;
			int fy = by + y;
			if(fx < 0) fx += renderer.GetWidth();
			if(fy < 0) fy += renderer.GetWidth();
			if(fx >= 0) fx %= renderer.GetWidth();
			if(fy >= 0) fy %= renderer.GetWidth();
			rTotal += renderer.GetPixel(fx, fy).r;
			gTotal += renderer.GetPixel(fx, fy).g;
			bTotal += renderer.GetPixel(fx, fy).b;
			total++;
		}
	}
	finalColor.r = rTotal / total;	
	finalColor.g = gTotal / total;	
	finalColor.b = bTotal / total;	
	
	return finalColor;
}
cdr::RGBA hBlurShader(const cdr::Renderer& renderer, int x, int y) {
	cdr::RGBA finalColor{};
	
	int offset = -8;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.000078f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.000489f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.002403f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.009245f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.027835f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.065592f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.12098f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.17467f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.197417f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.17467f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.12098f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.065592f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.027835f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.009245f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.002403f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.000489f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.000078f;
	
	return finalColor;
}
cdr::RGBA vBlurShader(const cdr::Renderer& renderer, int x, int y) {
	cdr::RGBA finalColor{};
	
	int offset = -8;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.000078f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.000489f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.002403f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.009245f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.027835f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.065592f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.12098f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.17467f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.197417f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.17467f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.12098f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.065592f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.027835f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.009245f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.002403f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.000489f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.000078f;
	
	return finalColor;
}
cdr::RGBA distortionShader(const cdr::Renderer& renderer, int x, int y) {
	cdr::RGBA color{};
	
	static double v1{0};
	static double v2{0};
	static double v3{0};
	v1 = sin(v3);
	v2 = cos(v3);
	v3 += 0.0000015;
	color = renderer.GetPixel(
		x + (cos(x / 8.f + v1 * 2 * M_PI)) * 12,
		y + (sin(y / 6.f + v2 * 2 * M_PI)) * 18
	);

	return color;
}