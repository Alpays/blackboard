#pragma once

#define COLOR_GREEN 0, 255, 0,   255
#define COLOR_BLUE  0, 0, 255,   255
#define COLOR_BLACK 0, 0, 0,     255
#define COLOR_WHITE 255,255,255, 255
#define COLOR_LPINK 255,233,243, 255
#define COLOR_GRAY 	128,128,128, 255

#define COLOR_SAND  246,220,189,255

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

typedef struct
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	uint8_t quit;
	int debugMode;

	int width;
	int height;

	TTF_Font* font;
} sWindow;
