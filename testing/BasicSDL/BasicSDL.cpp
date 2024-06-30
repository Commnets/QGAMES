#include "stdafx.h"

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <iostream>
#include <string>

SDL_Window* _window;
SDL_Renderer* _renderer;

#define _WIDTHWINDOW	600
#define _HEIGHTWINDOW	480

SDL_Texture* _texture1;
SDL_Texture* _texture2;
SDL_Texture* _texture3;
SDL_Texture* _texture4;
int _txtWidth, _txtHeight;

int _add;

bool initSDL ()
{
	if (SDL_Init (SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError () << std::endl;
		return (false);
	}

	_window = SDL_CreateWindow ("Hello World!", 100, 100, _WIDTHWINDOW, _HEIGHTWINDOW, SDL_WINDOW_SHOWN);
	if (_window == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError () << std::endl;
		return (false);
	}

	_renderer = SDL_CreateRenderer (_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (_renderer == nullptr)
	{
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError () << std::endl;
		return (false);
	}

	return (true);
}

bool loadElements ()
{
	SDL_Surface * tmpsurface;

	tmpsurface = SDL_LoadBMP ("image_1.bmp");
	if (tmpsurface == NULL) 
	{
		SDL_Log ("Could not load image_1.bmp");
		return (false);
	} 
	else 
	{
		_texture1 = SDL_CreateTextureFromSurface (_renderer, tmpsurface);
		_txtWidth = tmpsurface -> w; _txtHeight = tmpsurface -> h;
		SDL_SetTextureBlendMode (_texture1, SDL_BLENDMODE_BLEND);
		SDL_FreeSurface (tmpsurface);
	}

	tmpsurface = SDL_LoadBMP ("image_2.bmp");
	if (tmpsurface == NULL) 
	{
		SDL_Log ("Could not load image_2.bmp");
		return (false);
	} 
	else 
	{
		_texture2 = SDL_CreateTextureFromSurface (_renderer, tmpsurface);
		SDL_SetTextureBlendMode (_texture2, SDL_BLENDMODE_BLEND);
		SDL_FreeSurface (tmpsurface);
	}

	_add = 50;

	return (true);
}

void drawExample ()
{
	if ((_add += 5) >= _WIDTHWINDOW)
		_add = _WIDTHWINDOW;

	// Creates the mask for the state A
	_texture3 = 
		SDL_CreateTexture (_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _txtWidth, _txtHeight);
	SDL_SetTextureBlendMode (_texture3, SDL_BLENDMODE_MOD); 
	// Blending mode to MOD 
	// When writting this texture over a destination. This is the source (dstRGB = srcRGB * dstRGB, dstA = dstA)
	SDL_SetRenderTarget (_renderer, _texture3);
	// At that point the rending system that will be used to draw both the box and the circle is BLEND
	boxRGBA (_renderer, 0, 0, _WIDTHWINDOW, _HEIGHTWINDOW, 255, 255, 255, 255);
	filledEllipseRGBA (_renderer, _WIDTHWINDOW >> 1, _HEIGHTWINDOW >> 1, _add, _add, 0, 0, 0, 255);

	// Creates the visual part of the scene A
	SDL_Texture* partA = 
		SDL_CreateTexture (_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _txtWidth, _txtHeight);
	// In the zones where the ellipse is going to be drawn: (255, 255) -> (0, 255) that is a dark circle totally opaque!
	SDL_SetRenderTarget (_renderer, partA);
	SDL_RenderCopy (_renderer, _texture1, NULL, NULL);
	SDL_RenderCopy (_renderer, _texture3, NULL, NULL);
	// When render texture3 (mask: A circle to hide things there)
	// Where the circle is everything it is deleted
	// (txt1 color, 255)-> (0, 255) in the circle (not visible and opaque) and (color, 255) out of it (then visible and opaque)

	// Creates the mask for the state B
	_texture4 = 
		SDL_CreateTexture (_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _txtWidth, _txtHeight);
	SDL_SetTextureBlendMode (_texture4, SDL_BLENDMODE_MOD); 
	// Blending mode to MOD 
	// When writting this texture over a destination. This is the source (dstRGB = srcRGB * dstRGB, dstA = dstA)
	SDL_SetRenderTarget (_renderer, _texture4);
	// At that point the rending system that will be used to draw both the box and the circle is BLEND
	boxRGBA (_renderer, 0, 0, _WIDTHWINDOW, _HEIGHTWINDOW, 0, 0, 0, 255);
	filledEllipseRGBA (_renderer, _WIDTHWINDOW >> 1, _HEIGHTWINDOW >> 1, _add, _add, 255, 255, 255, 255); 
	
	// Creates the visual part for the state B
	SDL_Texture* partB = 
		SDL_CreateTexture (_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _txtWidth, _txtHeight);
	// In the zones where the ellipse is going to be drawn: (0, 0) -> (255, 255) that is a white circle totally opaque!
	SDL_SetRenderTarget (_renderer, partB);
	SDL_RenderCopy (_renderer, _texture2, NULL, NULL);
	SDL_RenderCopy (_renderer, _texture4, NULL, NULL);
	// When render texture4 (mask: A circle to show things there)
	// Where the circle is not everything is deleted
	// (txt1 color, 255)-> (txt color, 255) in the circle (visible and opaque) and (0, 255) out of it (not visible and opaque)

	SDL_SetRenderTarget (_renderer, NULL);
	// Now the rendering surface is the normal buffer...

	SDL_RenderCopy (_renderer, partB, NULL, NULL);
	SDL_SetTextureBlendMode (partA, SDL_BLENDMODE_ADD);
	// Blending system ADD.
	// That is when PartA is the original one is conserved!

	SDL_RenderCopy (_renderer, partA, NULL, NULL);

	SDL_DestroyTexture (_texture3);
	SDL_DestroyTexture (_texture4);
	SDL_DestroyTexture (partA);
	SDL_DestroyTexture (partB);
}

void unLoadElements ()
{
	SDL_DestroyTexture (_texture1);
	SDL_DestroyTexture (_texture2);
}

void finishSDL ()
{
	SDL_DestroyRenderer (_renderer);
	SDL_DestroyWindow (_window);
	SDL_Quit ();
}

void loopUntilQuit ()
{
	bool done = false;
	while (!done) 
	{
		SDL_Event e;
		while (SDL_PollEvent(&e)) 
		{
			if (e.type == SDL_QUIT)
				done = 1;
			else if (e.type == SDL_KEYUP) 
			{
				Uint32 sym = e.key.keysym.sym;
				if (sym == SDLK_ESCAPE)
					done = 1;
				else if (sym == SDLK_f) 
				{
					if (SDL_GetWindowFlags (_window) & SDL_WINDOW_FULLSCREEN)
						SDL_SetWindowFullscreen (_window, SDL_FALSE);
					else
						SDL_SetWindowFullscreen (_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				}
			} 
		}

		SDL_SetRenderDrawColor (_renderer, 0, 0, 0, 0); 
		SDL_RenderClear (_renderer);
		drawExample ();
		SDL_RenderPresent (_renderer);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (!initSDL ())
	{
		finishSDL ();
		return (1);
	}

	if (!loadElements ())
	{
		finishSDL ();
		return (1);
	}

	loopUntilQuit ();
	finishSDL ();

	unLoadElements ();

	return (0);
}
