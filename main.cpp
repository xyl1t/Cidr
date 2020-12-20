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
#include "cidr.hpp"
#include <string>
#include "timer.hpp"

Cidr::RGBA testShader(const Cidr::Renderer& renderer, int x, int y);
Cidr::RGBA blurShader(const Cidr::Renderer& renderer, int x, int y);
Cidr::RGBA hBlurShader(const Cidr::Renderer& renderer, int x, int y);
Cidr::RGBA vBlurShader(const Cidr::Renderer& renderer, int x, int y);
Cidr::RGBA hsvHueRotationShader(const Cidr::Renderer& renderer, int x, int y);
Cidr::RGBA distortionShader(const Cidr::Renderer& renderer, int x, int y);
Cidr::RGBA grayScaleShader(const Cidr::Renderer& renderer, int x, int y);
#include <cinttypes>
#include <utility>
#include <vector>
#include <cstdio>

int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_VIDEO);
	srand(time(NULL));
	
	int zoom {1};

	if(argc != 1 && std::atoi(argv[1]) >= 1 ) {
		zoom = std::atoi(argv[1]);
	}
	
	const int CANVAS_WIDTH = 900;
	const int CANVAS_HEIGHT = 600;
	const int WINDOW_WIDTH = CANVAS_WIDTH * zoom;
	const int WINDOW_HEIGHT = CANVAS_HEIGHT * zoom;
	
	SDL_Window* window = SDL_CreateWindow("Cidr Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, CANVAS_WIDTH, CANVAS_HEIGHT);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	
	uint32_t* pixels = new uint32_t[CANVAS_WIDTH * CANVAS_HEIGHT];
	memset(pixels, 0, CANVAS_WIDTH * CANVAS_HEIGHT * sizeof(uint32_t));
	
	Cidr::Renderer cidrRend {pixels, CANVAS_WIDTH, CANVAS_HEIGHT};
	
	SDL_Event event;
	bool alive = true;
	std::map<int, bool> keyboard;
	int mx, my;
	uint32_t current = SDL_GetTicks();
	uint32_t old = 0;
	uint32_t timer = 0;
	
	Cidr::RGBA (*currentShader)(const Cidr::Renderer& renderer, int x, int y) {nullptr};
	Cidr::RGBABitmap bitmap{"./res/pureTest.png"};
	Cidr::Bitmap triangleTexture{"./res/boxTexture.jpg"};
	
	
	float val1{};
	float val3{};
	float val2{};
	float val4{};
	Timer t {};
	while(alive) {
		/* TIMER */
		old = current;
		current = t.elapsed();
		if(timer > 1000) {
			std::cout << "ms: " << (current - old) / 1000000.f << '\n';
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
			cidrRend.OutOfBoundsType = Cidr::Renderer::OutOfBoundsType::Repeat;
			currentShader = nullptr;
		}
		if(keyboard[SDLK_6]) {
			cidrRend.OutOfBoundsType = Cidr::Renderer::OutOfBoundsType::MirroredRepeat;
			currentShader = nullptr;
		}
		if(keyboard[SDLK_7]) {
			cidrRend.OutOfBoundsType = Cidr::Renderer::OutOfBoundsType::ClampToBorder;
			currentShader = nullptr;
		}
		if(keyboard[SDLK_8]) {
			cidrRend.OutOfBoundsType = Cidr::Renderer::OutOfBoundsType::ClampToEdge;
			currentShader = nullptr;
		}
		if(keyboard[SDLK_9]) {
			cidrRend.ScaleType = Cidr::Renderer::ScaleType::Nearest;
			currentShader = nullptr;
		}
		if(keyboard[SDLK_0]) {
			cidrRend.ScaleType = Cidr::Renderer::ScaleType::Linear;
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
		
		/*** DRAWING ***/
		
		/* CLEARING THE SCREEN*/
		cidrRend.Clear(0x120F1FFF);
		// cidrRend.Clear(Cidr::RGB::Navy);
		// cidrRend.Clear(Cidr::RGB::Black);
		
		
		/* DRAWING A LINE TO THE CURSOR */
		// cidrRend.DrawLine({0, 0xff, 0}, 0, 0, mx, my, true);
		
		
		/* DRAWING A COLORFUL RECTANGLE USING A SHADER */
		auto lambda = [](const Cidr::Renderer& renderer, int x, int y) -> Cidr::RGBA {
			return {
				static_cast<uint8_t>(x - 3), 
				static_cast<uint8_t>(y - 3), 
				static_cast<uint8_t>(255 - (x - 3))};
		};
		cidrRend.FillRectangle(lambda, (Cidr::Rectangle){3, 3, 256, 256});
		
		
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
			Cidr::RGB color{0xff, 0xff, 0xff};
			if(i % 2) {
				color = {};
			}
			if(i == circlesCount - 1) {
				color = {0xff0000ff};
			}
			cidrRend.FillCircle(color, 64, 340, (circlesCount - i) / (float)circlesCount * 50, true);
		}
		cidrRend.FillCircle(0x30ee0Aff, 350, 50, 30, true);
		cidrRend.FillCircle(0x30ee0Aff, 350 + 30*2 + 15, 50, 30, false);


		/* TRIANGLES */
		// cidrRend.FillTriangle(
		// 	Cidr::RGB::Red, 
		// 	Cidr::RGB::Green, 
		// 	Cidr::RGB::Blue, 
		// 	mx-5,  my-5,
		// 	64 + 260, 64 + 128,
		// 	32 + 260, 32 + 128);
		// cidrRend.DrawTriangle(Cidr::RGB::White, 
		// 	mx-5,  my-5,
		// 	64 + 260, 64 + 128,
		// 	32 + 260, 32 + 128);
		cidrRend.FillTriangle(
			Cidr::RGB::Red, 
			Cidr::RGB::Green, 
			Cidr::RGB::Blue, 
			 0 + 300,  0 + 128,
			64 + 300, 32 + 128,
			32 + 300, 64 + 128);
		cidrRend.DrawTriangle(Cidr::RGB::White, 
			 0 + 300, 0 + 128,
			64 + 300,32 + 128,
			32 + 300,64 + 128, true, true);
		
		cidrRend.FillTriangle(
			Cidr::RGB::Red,
			Cidr::RGB::Green,
			Cidr::RGB::Blue,
			390, 		128,
			390, 		128 + 64,
			390 + 32, 	128 + 32);
		cidrRend.FillTriangle(
			Cidr::RGB::Green,
			Cidr::RGB::Blue,
			Cidr::RGB::Red,
			390 + 64, 	128,
			390 + 32, 	128 + 32,
			390 + 64, 	128 + 64);
		cidrRend.FillTriangle(
			Cidr::RGB::Green,
			Cidr::RGB::Blue,
			Cidr::RGB::Red,
			390, 		128 + 64,
			390 + 32, 	128 + 32,
			390 + 64, 	128 + 64);
		cidrRend.FillTriangle(
			Cidr::RGB::Red,
			Cidr::RGB::Green,
			Cidr::RGB::Blue,
			390, 		128,
			390 + 64, 	128,
			390 + 32, 	128 + 32);

		// cidrRend.DrawTriangle(triangleTexture, 
		// 	0, 1,
		// 	0.5, 0,
		// 	1, 1,
		// 	50, 300,
		// 	mx, my,
		// 	300, 350);
		
		
		/* IMAGES */
		cidrRend.ClampToBorderColor = Cidr::RGB::Gray;
		//  cidrRend.OutOfBoundsType = Cidr::Renderer::OutOfBoundsType::Repeat;
		int destWidth = bitmap.GetWidth() * 6;
		int destHeight = bitmap.GetWidth() * 6;
		cidrRend.DrawBitmap(bitmap, 
			(float)360-destWidth/2,(float)340-destHeight/2, destWidth, destHeight, 
			val2/4.f, -val1/4.f, bitmap.GetWidth(), bitmap.GetHeight());

		
		// float left   = 1.0;
		// float right  = 0.0;
		// float top    = 1.0;
		// float bottom = 0.0;
		
		// for(int i = 0; i < mx - 500; i++) {
		// 	for(int j = 0; j < my - 250; j++) {
		// 		float t = i / (float)(mx - 500);
		// 		float d = j / (float)(my - 250);
				
		// 		float val = (1 - t) * left + t * right;
		// 		val *= (bitmap.GetWidth() - 1);
		// 		val = ceil(val);
				
		// 		float val2 = (1 - d) * top + d * bottom;
		// 		val2 *= (bitmap.GetHeight() - 1);
		// 		val2 = ceil(val2);
				
		// 		cidrRend.DrawPoint(bitmap.GetPixel(val, val2), i + 500, j + 250);
		// 	}
		// }

		/* TEXTURED TRIANGLE */
		Cidr::Point p1{mx, my};
		Cidr::Point p2{mx, 350};
		Cidr::Point p3{450, 350};
		Cidr::Point p4{450, my};
		Cidr::Point pInBetween {(p1.x + p2.x + p3.x + p4.x) / 4, (p1.y + p2.y + p3.y + p4.y) / 4};
		
		cidrRend.DrawTriangle(bitmap, 
			0, 0,
			0, 1,
			1, 1,
			p1.x, p1.y,
			p2.x, p2.y,
			p3.x, p3.y);
		cidrRend.DrawTriangle(bitmap, 
			0, 0,
			1, 1,
			1, 0,
			p1.x, p1.y,
			p3.x, p3.y,
			p4.x, p4.y);
		
		
		/* TEXT */
		// cidrRend.DrawText("<two textured triangles>", pInBetween.x - (8 * 24 / 2), pInBetween.y - 10, Cidr::RGB::White, Cidr::RGBA::Transparent, Cidr::RGB::Black, 1, 1);
		
				
		/* APPLY SHADER */
		int shaderSize = 128;
		if(currentShader != nullptr) {
			if(currentShader == &blurShader) {
				cidrRend.FillRectangle(&hBlurShader, mx-shaderSize, my-shaderSize, shaderSize, shaderSize);
				cidrRend.FillRectangle(&vBlurShader, mx-shaderSize, my-shaderSize, shaderSize, shaderSize);
			}
			else {
				cidrRend.FillRectangle(currentShader, mx-shaderSize, my-shaderSize, shaderSize, shaderSize);				
			}
			cidrRend.DrawRectangle({0xe0, 0xef, 0xff}, mx-shaderSize, my-shaderSize, shaderSize, shaderSize);
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

Cidr::RGBA testShader(const Cidr::Renderer& renderer, int x, int y) { 
	Cidr::RGBA finalColor{};
	const Cidr::RGBA& currentPixel = renderer.GetPixel(x,y);

	finalColor.r = 255 - currentPixel.r;
	finalColor.g = 255 - currentPixel.g;
	finalColor.b = 255 - currentPixel.b;
	
	return finalColor;
}

Cidr::RGBA grayScaleShader(const Cidr::Renderer& renderer, int x, int y) { 
	Cidr::RGBA finalColor{};
	const Cidr::RGBA& currentPixel = renderer.GetPixel(x,y);

	finalColor.r = finalColor.g = finalColor.b = 
		0.2126 * currentPixel.r + 0.7152 * currentPixel.g + 0.0722 * currentPixel.b;
	
	return finalColor;
}

Cidr::RGBA hsvHueRotationShader(const Cidr::Renderer& renderer, int x, int y) {
	Cidr::RGBA finalColor{};
	const Cidr::RGBA& currentPixel = renderer.GetPixel(x,y);

	static double v {};
	v += 0.00005;
	if(v > 360) v = 0;

	Cidr::HSV temp = Cidr::RGBtoHSV(currentPixel); 
	temp.setH(temp.getH() + v);
	finalColor = Cidr::HSVtoRGB(temp);
	
	return finalColor;
}
Cidr::RGBA blurShader(const Cidr::Renderer& renderer, int x, int y) {
	Cidr::RGBA finalColor{};
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
Cidr::RGBA hBlurShader(const Cidr::Renderer& renderer, int x, int y) {
	Cidr::RGBA finalColor{};
	
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
Cidr::RGBA vBlurShader(const Cidr::Renderer& renderer, int x, int y) {
	Cidr::RGBA finalColor{};
	
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
Cidr::RGBA distortionShader(const Cidr::Renderer& renderer, int x, int y) {
	Cidr::RGBA color{};
	
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





/*

dawud




*/