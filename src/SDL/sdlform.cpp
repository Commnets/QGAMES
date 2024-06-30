#include <SDL/sdlform.hpp>

// ---
SDLForm::SDLForm (int id, const std::string& n, int nf, const std::string& s, int w, int h, 
		QGAMES::Sprite2DFormData* fD, SDL_Texture* t)
	: QGAMES::Sprite2D (id, n, nf, s, w, h, fD),
	  _texture (t)
{
	// Nothing additional to do
}

// ---
SDLForm::~SDLForm ()
{
	SDL_DestroyTexture (_texture);
	_texture = NULL;
}

