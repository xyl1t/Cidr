/********************************
 * Project: Cidr				* 
 * File: main.cpp				* 
 * Date: 10.8.2020				* 
 ********************************/

#include <iostream>
#include <SDL2/SDL.h>
#include "rgb.hpp"
#include "cidr.hpp"
// using namespace Cidr; 

int main() {
	SDL_Init(SDL_INIT_VIDEO);

	const int WIDTH = 800;
	const int HEIGHT = 600;

	SDL_Window* window = SDL_CreateWindow("Cidr example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	
	uint32_t* pixels = new uint32_t[WIDTH * HEIGHT];
	memset(pixels, 0, WIDTH * HEIGHT * sizeof(uint32_t));
	
	Cidr::Renderer cidrRend {pixels, WIDTH, HEIGHT};
	
	
	SDL_Event e;
	bool alive = true;
	int mx, my;
	while(alive) {
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				alive = false;
			}
			SDL_GetMouseState(&mx, &my);
			mx-=5;
			my-=5;
		}
		
		cidrRend.Clear();
		// cidrRend.DrawPoint(0xff0000ff, 256, 100);
		// cidrRend.DrawPoint(0xff00ffff, mx, my);
		// cidrRend.DrawLine({0, 0xff, 0}, 0, 0, mx, my, true);
		
		cidrRend.DrawLine({0, 0xff, 0}, 128, 128, mx, my, true);
		
		SDL_UpdateTexture(texture, nullptr, pixels, WIDTH * sizeof(uint32_t));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}
	
	SDL_Quit();
	
	return 0;
}