/** \ingroup SDL */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: sdlformbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a sdl implememtation of a form builder. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_SDLFORMBUILDER__
#define __QGAMES_SDLFORMBUILDER__

#include <Common/formbuilder.hpp>
#include <SDL/sdlscreen.hpp>

/** To create an SDLForm. */
class SDL2DSpriteBuilder : public QGAMES::Sprite2DBuilder
{
	public:
	SDL2DSpriteBuilder (const std::string& fDef, SDLScreen* s);
	SDL2DSpriteBuilder (const std::string& fDef, SDL_Window* w, SDL_Renderer* r);

	/** @see parent. */
	virtual QGAMES::Form* createForm (QGAMES::FormBuilder::FormDefinition* def);

	private:
	SDL_Window* _window;
	SDL_Renderer* _render;
};

#endif
  
// End of the file
/*@}*/
