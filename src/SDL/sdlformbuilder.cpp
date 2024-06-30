#include <SDL/sdlformbuilder.hpp>
#include <SDL/sdlform.hpp>
#include <SDL_image.h>
#include <Common/resourcesreader.hpp>
#include <Common/game.hpp>
#include <iostream>

// ---
SDL2DSpriteBuilder::SDL2DSpriteBuilder (const std::string& fDef, SDLScreen* s)
	: QGAMES::Sprite2DBuilder (fDef),
	  _render (NULL)
{
	assert (s); // Valid?

	_window = s -> window (); 
	_render = s -> render ();
	assert (_window && _render); // Can't be null, just in case.
}

// ---
SDL2DSpriteBuilder::SDL2DSpriteBuilder (const std::string& fDef, SDL_Window* w, SDL_Renderer* r)
	: QGAMES::Sprite2DBuilder (fDef),
	  _window (w),
	  _render (r)
{
	assert (_window && _render); // Valid?
}

// ---
QGAMES::Form* SDL2DSpriteBuilder::createForm (QGAMES::FormBuilder::FormDefinition* def)
{
	SDL_Surface* surface = IMG_Load (__RL (def -> _fileDefinition).c_str ());
	#ifndef NDEBUG
	if (!surface)
		std::cout << IMG_GetError () << std::endl;
	#endif

	assert (surface);

	SDL_Texture* texture = SDL_CreateTextureFromSurface (_render, surface);
	SDL_FreeSurface (surface);

	QGAMES::Sprite2DBuilder::SpriteDefinition* defS =
		dynamic_cast <QGAMES::Sprite2DBuilder::SpriteDefinition*> (def);
	assert (defS); // Just in case...

	QGAMES::Sprite2DFormData* data = createSprite2DFormDataFromSpriteDefinition (defS);

	return (new SDLForm (def -> _id, def -> _name, def -> _numberFrames, def -> _fileDefinition, 
		defS -> _width, defS -> _height, data, texture));
}