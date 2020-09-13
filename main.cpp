/********************************
 * Project: Cidr                *
 * File: main.cpp               *
 * Date: 10.8.2020              *
 ********************************/

#include <iostream>
#include <SDL2/SDL.h>
#include "cidr.hpp"
// using namespace Cidr; 

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	srand(time(NULL));
	
	const int WIDTH = 800;
	const int HEIGHT = 600;

	int zoom = 1;

	SDL_Window* window = SDL_CreateWindow("Cidr Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, WIDTH / zoom, HEIGHT / zoom);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	
	uint32_t* pixels = new uint32_t[WIDTH * HEIGHT];
	memset(pixels, 0, WIDTH * HEIGHT * sizeof(uint32_t));
	
	Cidr::Renderer cidrRend {pixels, WIDTH, HEIGHT};
	
	int x1 {rand()%WIDTH/zoom};
	int y1 {rand()%HEIGHT/zoom};
	int x2 {rand()%WIDTH/zoom};
	int y2 {rand()%HEIGHT/zoom};
	
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
		
		for (int i = 0; i < 255; i++) {
			for (int j = 0; j < 255; j++) {
				Cidr::RGB color {
					static_cast<uint8_t>(i),
					static_cast<uint8_t>(j),
					static_cast<uint8_t>(255 - i)
				};
				cidrRend.DrawPoint(color, i, j);
			}	
		}
		for (int i = 35; i < 85; i++) {
			for (int j = 35; j < 85; j++) {
				// Cidr::RGBA color = Cidr::alphaBlendColor(cidrRend.GetPixel(i, j), {0x00ff0077});
				// cidrRend.DrawPoint(color, i,j);
			}	
		}
		
		// cidrRend.DrawLine({0xff, 0, 0}, x1, y1, x2, y2, true)
		
		cidrRend.DrawLine({0, 0xff, 0}, 128, 64+10, mx, my+10, true);;
		cidrRend.DrawLine({0, 0xff, 0}, 128, 64, mx, my, false);
		cidrRend.DrawLine({0, 0xff, 0}, 128, 64-10, mx, my-10, true, true);
		
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