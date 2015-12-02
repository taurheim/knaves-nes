#include "renderer.h"
#include <iostream>
#include <array>

palette_entry current_screen[240][256];

void renderer::init() {
	SDL_Init(SDL_INIT_VIDEO);

	int width = 256;
	int height = 240;

	window = SDL_CreateWindow("Knaves NES Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);

	screen = SDL_GetWindowSurface(window);

	bg = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	sprBg = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	sprFg = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

}

void renderer::put_pixel(const enum PixelType &type, int x, int y, const palette_entry &color) {
	if (y < 3) return;
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
	Uint32 *pixel_location = (Uint32*) dest->pixels + y*256 + x;

	SDL_LockSurface(bg);
	*(Uint32*)pixel_location = col;
	current_screen[y][x] = color;
	SDL_UnlockSurface(bg);
}

void renderer::update() {
	SDL_BlitSurface(bg, NULL, screen, NULL);
	//SDL_BlitSurface(sprBg, NULL, screen, NULL);
	//SDL_BlitSurface(sprFg, NULL, screen, NULL);

	std::cout << "Updating" << std::endl;
	SDL_UpdateWindowSurface(window);
}