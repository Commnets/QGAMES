/** \ingroup SDL */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: sdlform.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a sdl implememtation of a form. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_SDLFORM__
#define __QGAMES_SDLFORM__

#include <SDL.h>
#include <Common/form.hpp>

/** SDL is only useful in 2D worlds.
	SDLForm represents an Sprite using SDL libraries. */
class SDLForm : public QGAMES::Sprite2D
{
	public:
	SDLForm (int id, const std::string& n, int nf, const std::string& s, int w, int h, 
		QGAMES::Sprite2DFormData* fD, SDL_Texture* t);
	virtual ~SDLForm ();

	SDL_Texture* texture ()
							{ return (_texture); }

	private:
	SDL_Texture* _texture;
};

#endif
  
// End of the file
/*@}*/
