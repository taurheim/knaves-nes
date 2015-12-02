#include "renderer.h"
#include <iostream>

void renderer::init() {
	SDL_Init(SDL_INIT_VIDEO);

	int width = 256;
	int height = 240;

	window = SDL_CreateWindow("Knaves NES Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

	screen = SDL_GetWindowSurface(window);

	bg = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	sprBg = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	sprFg = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

	SDL_LockSurface(bg);

}

void renderer::put_pixel(const enum PixelType &type, int x, int y, const palette_entry &color) {
	if (y > 2) {
		return;
	}
	SDL_Surface * dest;

	//Figure out where we're blitting
	switch (type) {
	case PixelType::BACKGROUND_TILE:
		dest = bg;
		//std::cout << std::dec << (int)color.r << "," << (int)color.g << "," << (int) color.b << std::endl;
		break;
	case PixelType::BACKGROUND_SPRITE:
		dest = sprBg;
		break;
	case PixelType::FOREGROUND_SPRITE:
		dest = sprFg;
		break;
	default:
		return;
	}

	Uint32 col = SDL_MapRGBA(dest->format, color.r, color.g, color.b, color.a);
	x = 50;
	y = 50;
	Uint32 *pixel_location = (Uint32*) dest->pixels + y * dest->pitch + x*dest->format->BytesPerPixel;

	*(Uint32*)pixel_location = col;
	x++;
	pixel_location = (Uint32*)dest->pixels + y * dest->pitch + x*dest->format->BytesPerPixel;

	*(Uint32*)pixel_location = col;
}

void renderer::update() {
	SDL_UnlockSurface(bg);
	SDL_BlitSurface(bg, NULL, screen, NULL);
	//SDL_BlitSurface(sprBg, NULL, screen, NULL);
	//SDL_BlitSurface(sprFg, NULL, screen, NULL);

	SDL_UpdateWindowSurface(window);
}