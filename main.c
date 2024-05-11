// gcc -L lib -o main drawCircle.h app_window.h main.c -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf

/*

Blackboard

Very simple drawing app made with C and SDL.

Controls

F4 - Clears the drawings.
F5 - Undo latest drawing.

*/

int prevBoard[1280][720] = { 0 };
int board[1280][720] = { 0 };

#define SDL_MAIN_HANDLED

#include "app_window.h"
#include "drawCircle.h"

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

sWindow app;

void initApp(void);
void initFontLibrary(void);
void initImgLibrary(void);
void render(void);
void debug(const char* msg);

void draw(void);
void clearBoard(void);
void undoLatest(void);
void copyBoardToBuffer(void);

SDL_Texture* textTexture;
SDL_Rect textRect;

bool drawing = false;

int main(void)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	initApp();
	while(!app.quit)
	{
		const Uint8* keyStates = SDL_GetKeyboardState(0);

		int a, b = 0;
		const Uint8 mState = SDL_GetMouseState(&a,&b);

		if(keyStates[SDL_SCANCODE_F4])
		{
			clearBoard();
		}
		if(keyStates[SDL_SCANCODE_F5])
		{
			undoLatest();
		}

		while (SDL_PollEvent(&app.event)) {
			switch (app.event.type) {
	
			case SDL_QUIT:
			{
				app.quit = 1;
				break;
			}
			
			case SDL_MOUSEBUTTONUP:
			{
				drawing = false;
				continue;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				copyBoardToBuffer();
				drawing = true;
				continue;
			}
			}
		}
		draw();
		render();
	}

	SDL_DestroyWindow(app.window);
	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyTexture(textTexture);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}

void initApp(void)
{
	initFontLibrary();
	initImgLibrary();

	app.width = 1280;
	app.height = 720;
	app.window = SDL_CreateWindow("blackboard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, app.width, app.height, SDL_WINDOW_SHOWN);
	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
	app.quit = 0;
	app.debugMode = 1;

	app.font = TTF_OpenFont("fonts/pdown.otf", 40);
	if(app.font == NULL) { debug("Failed to open font!"); debug(TTF_GetError()); }
	SDL_Color textColor = { COLOR_BLUE };
	SDL_Surface* textSurface = TTF_RenderText_Solid(app.font, "blackboard", textColor);
	textTexture = SDL_CreateTextureFromSurface(app.renderer, textSurface);
	textRect.x = textRect.y = 0;

	SDL_QueryTexture(textTexture, 0,0, &textRect.w, &textRect.h);

	SDL_FreeSurface(textSurface);
	textSurface = NULL;
}

void render(void)
{
	SDL_RenderSetScale(app.renderer, 1.0f, 1.0f);

	SDL_SetRenderDrawColor(app.renderer, COLOR_BLACK);
	SDL_RenderClear(app.renderer);

	SDL_SetRenderDrawColor(app.renderer, COLOR_WHITE);

	for(int i = 0; i < app.width; ++i)
	{
		for(int j = 0; j < app.height; ++j)
		{
			if(board[i][j]) SDL_RenderDrawPoint(app.renderer, i, j);
		}
	}
	int x, y;
	SDL_GetMouseState(&x, &y);

	SDL_RenderDrawCircle(app.renderer, x, y, 50);

	SDL_RenderCopy(app.renderer, textTexture, 0, &textRect);

	SDL_RenderPresent(app.renderer);
}

void debug(const char* msg)
{
	if(app.debugMode)
	{
		printf(msg);
		printf("\n");
	}
}

void initFontLibrary(void)
{
	debug("Initialising the font library...");
	if(TTF_Init() < 0)
	{
		debug("ERROR!");
		debug(TTF_GetError());
	}
	else debug("Successfully Initialised!");
}

void initImgLibrary(void)
{
	debug("Initialising the image library...");
	if(!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_JPG | IMG_INIT_JPG))
	{
		debug("Error initialising the image library.");
		debug(IMG_GetError());
	}
	else debug("Successfully Initialised!");
}

void draw(void)
{				
	if(drawing)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		board[x][y] = 1;
	}
}

void clearBoard()
{
	copyBoardToBuffer();
	SDL_Delay(250);

	for(int i = 0; i < app.width; ++i)
	{
		for(int j = 0; j < app.height; ++j)
		{
			board[i][j] = 0;
		}
	}
}

void undoLatest(void)
{
	for(int i = 0; i < app.width; ++i)
	{
		for(int j = 0; j < app.height; ++j)
		{
			board[i][j] = prevBoard[i][j];
		}
	}	
}

void copyBoardToBuffer(void)
{
	for(int i = 0; i < app.width; ++i)
	{
		for(int j = 0; j < app.height; ++j)
		{
			prevBoard[i][j] = board[i][j];
		}
	}		
}