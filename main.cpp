/********************************
 * Project: Cidr                *
 * File: main.cpp               *
 * Date: 10.8.2020              *
 ********************************/

#include <iostream>
#include <SDL2/SDL.h>
#include "cidr.hpp"
// using namespace Cidr; 

int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_VIDEO);
	srand(time(NULL));
	
	const int WIDTH = 800;
	const int HEIGHT = 600;

	int zoom = 1;

	SDL_Window* window = SDL_CreateWindow("Cidr Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, WIDTH / zoom, HEIGHT / zoom);
	// SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	
	uint32_t* pixels = new uint32_t[WIDTH * HEIGHT];
	memset(pixels, 0, WIDTH * HEIGHT * sizeof(uint32_t));
	
	Cidr::Renderer cidrRend {pixels, WIDTH, HEIGHT};

	float locationX = 128 + 256;
	float locationY = 128 + 256;
	float width = 64;
	float height = 64;
	Cidr::FPoint p1{ locationX, locationY };
	Cidr::FPoint p2{ locationX + width, locationY };
	Cidr::FPoint p3{ locationX + width, locationY + height };
	Cidr::FPoint p4{ locationX, locationY + height };
	
	Cidr::FPoint p11{ locationX, locationY };
	Cidr::FPoint p12{ locationX + width, locationY };
	Cidr::FPoint p13{ locationX + width, locationY + height };
	Cidr::FPoint p14{ locationX, locationY + height };
	float angle = 0.001f;
	
	SDL_Event e;
	bool alive = true;
	int mx, my;
	while(alive) {
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				alive = false;
			}
			SDL_GetMouseState(&mx, &my);
			mx /= zoom;
			my /= zoom;
			mx-=5;
			my-=5;
		}
		
		cidrRend.Clear();
		// cidrRend.DrawPoint(0xff0000ff, 256, 100);
		// cidrRend.DrawPoint(0xff00ffff, mx, my);
		// cidrRend.DrawLine({0, 0xff, 0}, 0, 0, mx, my, true);
		
		// for (int i = 0; i < 255; i++) {
		// 	for (int j = 0; j < 255; j++) {
		// 		Cidr::RGB color {
		// 			static_cast<uint8_t>(i),
		// 			static_cast<uint8_t>(j),
		// 			static_cast<uint8_t>(255 - i)
		// 		};
		// 		cidrRend.DrawPoint(color, i, j);
		// 	}	
		// }
		
		// cidrRend.DrawLine(0x00ff0013, 128, 64-10, mx, my-10, true, true);
		// cidrRend.DrawLine(0x00ff0013, 128, 64, mx, my, false);
		// cidrRend.DrawLine(0x00ff0013, 128, 64+10, mx, my+10, true);;
		
		p1.x = std::cos(angle) * (-width / 2) - std::sin(angle) * (-height / 2) + locationX;
		p1.y = std::sin(angle) * (-width / 2) + std::cos(angle) * (-height / 2) + locationY;
		
		p2.x = std::cos(angle) * (width / 2) - std::sin(angle) * (-height / 2) + locationX;
		p2.y = std::sin(angle) * (width / 2) + std::cos(angle) * (-height / 2) + locationY;
		
		p3.x = std::cos(angle) * (width / 2) - std::sin(angle) * (height / 2) + locationX;
		p3.y = std::sin(angle) * (width / 2) + std::cos(angle) * (height / 2) + locationY;
		
		p4.x = std::cos(angle) * (-width / 2) - std::sin(angle) * (height / 2) + locationX;
		p4.y = std::sin(angle) * (-width / 2) + std::cos(angle) * (height / 2) + locationY ;
		
		
		cidrRend.DrawLine({32, 200, 8}, p1, p2);
		cidrRend.DrawLine({32, 200, 8}, p2, p3);
		cidrRend.DrawLine({32, 200, 8}, p3, p4);
		cidrRend.DrawLine({32, 200, 8}, p4, p1);
		
		cidrRend.DrawLine({32, 200, 8}, (Cidr::FPoint){p1.x + width * 1.5f, p1.y}, (Cidr::FPoint){p2.x + width * 1.5f, p2.y}, true);
		cidrRend.DrawLine({32, 200, 8}, (Cidr::FPoint){p2.x + width * 1.5f, p2.y}, (Cidr::FPoint){p3.x + width * 1.5f, p3.y}, true);
		cidrRend.DrawLine({32, 200, 8}, (Cidr::FPoint){p3.x + width * 1.5f, p3.y}, (Cidr::FPoint){p4.x + width * 1.5f, p4.y}, true);
		cidrRend.DrawLine({32, 200, 8}, (Cidr::FPoint){p4.x + width * 1.5f, p4.y}, (Cidr::FPoint){p1.x + width * 1.5f, p1.y}, true);
		
		cidrRend.DrawLine({32, 200, 8}, (Cidr::FPoint){p1.x + width * 1.5f * 2, p1.y}, (Cidr::FPoint){p2.x + width * 1.5f * 2, p2.y}, true, true);
		cidrRend.DrawLine({32, 200, 8}, (Cidr::FPoint){p2.x + width * 1.5f * 2, p2.y}, (Cidr::FPoint){p3.x + width * 1.5f * 2, p3.y}, true, true);
		cidrRend.DrawLine({32, 200, 8}, (Cidr::FPoint){p3.x + width * 1.5f * 2, p3.y}, (Cidr::FPoint){p4.x + width * 1.5f * 2, p4.y}, true, true);
		cidrRend.DrawLine({32, 200, 8}, (Cidr::FPoint){p4.x + width * 1.5f * 2, p4.y}, (Cidr::FPoint){p1.x + width * 1.5f * 2, p1.y}, true, true);
		
		cidrRend.DrawLine({32, 200, 8}, (Cidr::FPoint){p11.x + width, p11.y - height * 2}, (Cidr::FPoint){p12.x + width, p12.y - height * 2});
		cidrRend.DrawLine({32, 200, 8}, (Cidr::FPoint){p12.x + width, p12.y - height * 2}, (Cidr::FPoint){p13.x + width, p13.y - height * 2});
		cidrRend.DrawLine({32, 200, 8}, (Cidr::FPoint){p13.x + width, p13.y - height * 2}, (Cidr::FPoint){p14.x + width, p14.y - height * 2});
		cidrRend.DrawLine({32, 200, 8}, (Cidr::FPoint){p14.x + width, p14.y - height * 2}, (Cidr::FPoint){p11.x + width, p11.y - height * 2});
		
		
		angle += 0.01;
		
		// for(int i = 0; i < 360; i++) {
		// 	float x = cos(i/360.f * 3.14159*2) * width  * std::sqrt(2) / 2.f + locationX;
		// 	float y = sin(i/360.f * 3.14159*2) * height * std::sqrt(2) / 2.f + locationY;
		// 	cidrRend.DrawPoint(0xff00ffff, x, y);
		// }
		
		
		SDL_UpdateTexture(texture, nullptr, pixels, WIDTH * sizeof(uint32_t));
		SDL_RenderClear(renderer);
		SDL_Rect rect1{0,0, WIDTH/zoom, HEIGHT/zoom};
		SDL_Rect rect2{0,0, WIDTH, HEIGHT};
		SDL_RenderCopy(renderer, texture, 0, 0);
		SDL_RenderPresent(renderer);
	}
	
	SDL_Quit();
	
	return 0;
}