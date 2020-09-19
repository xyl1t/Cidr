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

class Timer; 

Cidr::RGBA blurShader(const Cidr::Renderer& renderer, int x, int y);
Cidr::RGBA hBlurShader(const Cidr::Renderer& renderer, int x, int y);
Cidr::RGBA vBlurShader(const Cidr::Renderer& renderer, int x, int y);
Cidr::RGBA hsvHueRotationShader(const Cidr::Renderer& renderer, int x, int y);
Cidr::RGBA distortionShader(const Cidr::Renderer& renderer, int x, int y);

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
			// mx-=3;
			// my-=3;
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
		
		auto lambda = [](const Cidr::Renderer& renderer, int x, int y) -> Cidr::RGBA {
			return {
				static_cast<uint8_t>(x - 3), 
				static_cast<uint8_t>(y - 3), 
				static_cast<uint8_t>(255 - (x - 3))};
		};
		cidrRend.FillRectangle(lambda, (Cidr::Point){3, 3}, 255, 255);
		
		cidrRend.DrawLine(0x00ff00ff, 128, 64-10, 400, 370-10, true, true);
		cidrRend.DrawLine(0x00ff00ff, 128, 64,    400, 370);
		cidrRend.DrawLine(0x00ff00ff, 128, 64+10, 400, 370+10, true);
		
		cidrRend.FillRectangle(0xff0000ff, 300-5,300+5,40,40);
		cidrRend.FillRectangle(0x00ff00ff, 320-5,320+5,40,40);
		cidrRend.FillRectangle(0x0000ffff, 340-5,340+5,40,40);
		
		/* CIRCLE */
		cidrRend.DrawCircle(0x23ff10ff, 340, 340, 50);
		
		/* SHADER */
		int shaderSize = 128;
		cidrRend.FillRectangle(&hBlurShader, mx-shaderSize, my-shaderSize, shaderSize, shaderSize);
		cidrRend.FillRectangle(&vBlurShader, mx-shaderSize, my-shaderSize, shaderSize, shaderSize);
		cidrRend.DrawRectangle({0xe0, 0xef, 0xff}, mx-shaderSize, my-shaderSize, shaderSize, shaderSize);
		
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
	int blurSize = 5;
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
	
	int offset = -5;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.0093f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.028002f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.065984f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.121703f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.175713f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.198596f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.175713f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.121703f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.065984f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.028002f;
	finalColor += renderer.GetPixel(x + offset++, y) * 0.0093f;
	
	return finalColor;
}
Cidr::RGBA vBlurShader(const Cidr::Renderer& renderer, int x, int y) {
	Cidr::RGBA finalColor{};
	
	int offset = -5;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.0093f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.028002f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.065984f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.121703f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.175713f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.198596f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.175713f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.121703f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.065984f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.028002f;
	finalColor += renderer.GetPixel(x, y + offset++) * 0.0093f;
	
	return finalColor;
}
Cidr::RGBA distortionShader(const Cidr::Renderer& renderer, int x, int y) {
	Cidr::RGBA color{};
	
	static double v1{0};
	static double v2{0};
	static double v3{0};
	v1 = sin(v3);
	v2 = cos(v3);
	v3 += 0.000001;
	color = renderer.GetPixel(
		x + (cos(x / 8.f + v1 * 2 * M_PI)) * 12,
		y + (sin(y / 6.f + v2 * 2 * M_PI)) * 18
	);

	return color;
}
