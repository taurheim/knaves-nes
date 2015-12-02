#include "SDL.h"

enum PixelType {BACKGROUND_TILE, FOREGROUND_SPRITE, BACKGROUND_SPRITE};
typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} palette_entry;

class renderer {
public:
	void init();
	void update();
	void put_pixel(const enum PixelType &type, int x, int y, const palette_entry &color);

private:
	SDL_Window *window;
	SDL_Surface *screen, *bg, *sprBg, *sprFg;
};