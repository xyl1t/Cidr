/********************************
 * Project: Cidr                *
 * File: main.cpp               *
 * Date: 10.8.2020              *
 ********************************/

#include <iostream>
#if __has_include("SDL2/SDL.h")
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include "cidr.hpp"
#include <string>
#include <chrono> // for std::chrono functions
// using namespace Cidr;
 
class Timer
{
private:
	// Type aliases to make accessing nested type easier
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;
	
	std::chrono::time_point<clock_t> m_beg;
 
public:
	Timer() : m_beg(clock_t::now())
	{
	}
	
	void reset()
	{
		m_beg = clock_t::now();
	}
	
	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

Cidr::RGBA myShader(const Cidr::Renderer& renderer, int x, int y) {
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

int main(int argc, char** argv) {
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
	
	SDL_Event e;
	bool alive = true;
	int mx, my;
	uint32_t current = SDL_GetTicks();
	uint32_t old = 0;
	uint32_t timer = 0;
	
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

		old = current;
		current = SDL_GetTicks();
		if(timer > 1000) {
			std::cout << "ms: " << current - old << '\n';
			timer = 0;
		}
		timer += current - old;
		
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

		// 		cidrRend.DrawPoint(color, i + 3, j + 3);
		// 	}
		// }
		
		auto lambda = [](const Cidr::Renderer& renderer, int x, int y) -> Cidr::RGBA {
			return {
				static_cast<uint8_t>(x), 
				static_cast<uint8_t>(y), 
				static_cast<uint8_t>(255 - x)};
		};
		cidrRend.FillRectangle(lambda, (Cidr::Point){mx, my}, 256, 256);
		
		cidrRend.DrawLine(0x00ff00ff, 128, 64-10, 400, 400-10, true, true);
		cidrRend.DrawLine(0x00ff00ff, 128, 64,    400, 400);
		cidrRend.DrawLine(0x00ff00ff, 128, 64+10, 400, 400+10, true);
		
		cidrRend.FillRectangle(0xff0000ff, 300,300,40,40);
		cidrRend.FillRectangle(0x00ff00ff, 320,320,40,40);
		cidrRend.FillRectangle(0x0000ffff, 340,340,40,40);
		
		// cidrRend.DrawRectangle((Cidr::RGB){0x20ee05}, mx, my, 64, 64);
		cidrRend.FillRectangle(&myShader, (Cidr::Point){mx, my}, 256, 256);
		cidrRend.DrawRectangle({0xe0, 0xef, 0xff}, {mx, my}, 256, 256);
		
		
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