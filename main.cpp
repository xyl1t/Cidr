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
	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	
	uint32_t* pixels = new uint32_t[WIDTH * HEIGHT];
	memset(pixels, 0, WIDTH * HEIGHT * sizeof(uint32_t));
	
	Cidr::Renderer cidrRend {pixels, WIDTH, HEIGHT};
	
	cidrRend.DrawLine({0, 0xff, 0}, 8, 4, 64, 32);
	
	SDL_Event e;
	bool alive = true;
	while(alive) {
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				alive = false;
			}
		}
		
		SDL_UpdateTexture(texture, nullptr, pixels, WIDTH * sizeof(uint32_t));
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}
	
	SDL_Quit();
	
	return 0;
}