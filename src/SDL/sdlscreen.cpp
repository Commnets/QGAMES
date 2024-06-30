#include <SDL/sdlscreen.hpp>
#include <SDL/sdlform.hpp>
#include <SDL2_gfxPrimitives.h>
#include <Common/rectangle.hpp>
#include <Common/color.hpp>
#include <Common/resourcesreader.hpp>
#include <SDL_image.h>
#include <iostream>

// ---
SDLZoommerOnScreen::SDLZoommerOnScreen (QGAMES::bdata v, int mW, int mH, 
	SDL_Renderer* r, QGAMES::ZoommerOnScreen::Effect e)
		: QGAMES::ZoommerOnScreen (v, mW, mH, e, true),
		  _render (r)
{ 
	assert (_render); // It can't be null...
}

// ---
void SDLZoommerOnScreen::beforeDrawing ()
{
	if (useSoftware () && value () != 1.0f)
		SDL_RenderSetScale (_render, (float) _status._value, (float) _status._value);
}

// ---
void SDLZoommerOnScreen::afterDrawing ()
{
	if (useSoftware () && value () != 1.0f)
		SDL_RenderSetScale (_render, 1.0f, 1.0f);
}

// ---
SDLScreen::SDLBuffer::SDLBuffer (QGAMES::Screen* s, SDL_BlendMode bM, int w, int h)
	: QGAMES::Screen::Buffer (s, w, h),
	  _textureBuffer (NULL)
{
	_textureBuffer = 
		SDL_CreateTexture (((SDLScreen*) _screen) -> _render, 
			SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _width, _height);
	SDL_SetTextureBlendMode (_textureBuffer, bM);
}

// ---
SDLScreen::SDLBuffer::~SDLBuffer ()
{ 
	// makes sure that before becoming destroyed, the render target is the default one and not this...
	assert (SDL_GetRenderTarget (((SDLScreen*) _screen) -> _render) != _textureBuffer);

	SDL_DestroyTexture (_textureBuffer);
	_textureBuffer = NULL;
}

// ---
void SDLScreen::SDLBuffer::open ()
{
	SDL_Renderer* r = ((SDLScreen*) _screen) -> _render;

	SDL_SetRenderTarget (r, _textureBuffer); // Everything drawn to the target...
	SDL_RenderClear (r); // Get it clear first!
}

// ---
void SDLScreen::SDLBuffer::close ()
{
	SDL_SetRenderTarget (((SDLScreen*) _screen) -> _render, NULL); // Te render targets the default screen...
}

// ---
void SDLScreen::SDLBuffer::moveToScreen (int x, int y)
{
	SDL_Renderer* r = ((SDLScreen*) _screen) -> _render;

	SDL_Rect dst; dst.x = x; dst.y = y; dst.w = _width; dst.h = _height;
	SDL_Rect org; org.x = org.y = 0; org.w = _width; org.h = _height;

	SDL_Texture* cR = SDL_GetRenderTarget (r);
	if (cR != NULL)
		SDL_SetRenderTarget (r, NULL); // Everything drawn back to the screen...
	SDL_RenderCopy (r, _textureBuffer, &org, &dst);
	SDL_SetRenderTarget (r, cR); // The target goes back what is was previously...
}

// ---
void SDLScreen::SDLBuffer::moveToBuffer (QGAMES::Screen::Buffer* b, int x, int y)
{
	assert (dynamic_cast <SDLScreen::SDLBuffer*> (b));

	SDL_Renderer* r = ((SDLScreen*) _screen) -> _render;

	SDL_Rect dst; dst.x = x; dst.y = y; dst.w = _width; dst.h = _height;
	SDL_Rect org; org.x = org.y = 0; org.w = _width; org.h = _height;

	SDL_Texture* tR =  (dynamic_cast <SDLScreen::SDLBuffer*> (b)) -> _textureBuffer;
	SDL_Texture* cR = SDL_GetRenderTarget (r);
	SDL_SetRenderTarget (r, tR); // Everything to the buffer...
	SDL_RenderCopy (r, _textureBuffer, &org, &dst);
	SDL_SetRenderTarget (r, cR); // The target goes back what is was previously...
}

//---
SDLScreen::SDLScreen (const std::string& t, const QGAMES::Position& p, int w, int h, int wPX, int wPY, float sX, float sY)
	: QGAMES::Screen (t, w, h), // No camera passed as parameter...
	  _window (NULL),
	  _render (NULL),
	  _windowPositionX (wPX), _windowPositionY (wPY),
	  _scaleX (sX), _scaleY (sY),
	  _fontsLoaded ()
{
	// Sets the window...
	_window = SDL_CreateWindow (_title.c_str (), 
		/*SDL_WINDOWPOS_CENTERED*/ _windowPositionX, /*SDL_WINDOWPOS_CENTERED*/ _windowPositionY, 
		_visualWidth, _visualHeight, SDL_WINDOW_SHOWN);
	assert (_window); // The window has to be created...
	_render = SDL_CreateRenderer (_window, -1, 
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	assert (_render); // The render has to be created...

	// Sets the camera, because none has been created before by default...
	QGAMES::Camera* cm = new QGAMES::Camera (
			std::string (__QGAMES_MAINCAMERAID__), // Main camera...
			QGAMES::Camera::ProjectionType::_ORTHOGONAL, 
			new SDLZoommerOnScreen (__BD 1 /** no zoom. */, w, h, _render, QGAMES::ZoommerOnScreen::_TOUPLEFT), 
			w, h, p);
	addCamera (cm);
	setCurrentCamera (cm); // By default the camera is the current one...
}

//---
SDLScreen::~SDLScreen ()
{
	SDL_DestroyRenderer (_render);
	_render = NULL;
	SDL_DestroyWindow (_window);
	_window = NULL;

	// Destroy the loaded fonts...
	// Some times, the game finished first and TTF library finishes too
	// So make no sense here to liberate the fonts!
	if (TTF_WasInit ())
		for (std::map <std::string, TTF_Font*>::const_iterator i = _fontsLoaded.begin ();
				i != _fontsLoaded.end (); i++)
			TTF_CloseFont ((*i).second);
	_fontsLoaded.clear ();
}

// ---
void SDLScreen::setIcon (QGAMES::Form* f)
{
	QGAMES::Sprite2D* sp = static_cast <QGAMES::Sprite2D*> (f);
	if (!sp)
		return; // It has to be something in 2D

	// The form used has to be an sprite 2d...
	// Otherwise the icon will not be drawn!
	std::string s = sp -> source (); 
	SDL_Surface* sfc = IMG_Load ((__RL (s)).c_str ());
	SDL_SetWindowIcon (_window, sfc);
	SDL_FreeSurface (sfc);
}

// ---
void SDLScreen::windowAtPosition (int x, int y)
{
	 _windowPositionX = x; _windowPositionY = y;
	SDL_SetWindowPosition (_window, x, y);
}

// ---
void SDLScreen::windowAtCenter ()
{
	SDL_DisplayMode d;
	SDL_GetCurrentDisplayMode (0, &d);
	windowAtPosition (((int) ((float) d.w / _scaleX) - _visualWidth) >> 1, 
		(int) ((int) ((float) d.h / _scaleY) - _visualHeight) >> 1);
}

//---
void SDLScreen::SDLTransaction::openFirst ()
{
	SDL_SetRenderDrawColor (((SDLScreen*) _screen) -> _render, 0, 0, 0, 255); 
	SDL_RenderClear (((SDLScreen*) _screen) -> _render);
}

//---
void SDLScreen::SDLTransaction::closeLast ()
{
	SDL_RenderPresent (((SDLScreen*) _screen) -> _render);
}

//---
void SDLScreen::drawForm (int x, int y, QGAMES::Form* f, int px, int py, int w, int h, int a)
{
	SDL_Rect srcRect, dstRect;
	srcRect.x = px; srcRect.y = py;
	srcRect.w = dstRect.w = w;
	srcRect.h = dstRect.h = h;
	dstRect.x = x; dstRect.y = y;
	SDL_Texture* t =  (static_cast <SDLForm*> (f)) -> texture ();
	SDL_SetTextureBlendMode (t, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod (t, a); // To fade out/in the image...
	SDL_RenderCopy (_render, t, &srcRect, &dstRect);
}

// ---
void SDLScreen::drawPoint (int x, int y, const QGAMES::Color& c)
{
	Sint16 X, Y; 
	convertCoordinatesIntToSInt16 (x, y, X, Y);

	pixelRGBA (_render, X, Y, c.red (), c.green (), c.blue (), c.alpha ());
}

//---
void SDLScreen::drawLine (int x1, int y1, int x2, int y2, const QGAMES::Color& c, int t)
{
	if (t <= 1)
	{
		SDL_SetRenderDrawColor (_render, c.red (), c.green (), c.blue (), c.alpha ());
		SDL_RenderDrawLine (_render, x1, y1, x2, y2);
	}
	else 
	{
		Sint16 X1, Y1, X2, Y2;
		convertCoordinatesIntToSInt16 (x1, y1, X1, Y1);
		convertCoordinatesIntToSInt16 (x2, y2, X2, Y2);

		thickLineRGBA (_render, X1, Y1, X2, Y2, t, c.red (), c.green (), c.blue (), c.alpha ());
	}
}

//---
void SDLScreen::drawRectangle (int x1, int y1, int x2, int y2, const QGAMES::Color& c, bool f)
{
	Sint16 X1, Y1, X2, Y2;
	convertCoordinatesIntToSInt16 (x1, y1, X1, Y1);
	convertCoordinatesIntToSInt16 (x2, y2, X2, Y2);

	(f)
		? boxRGBA (_render, X1, Y1, X2, Y2, c.red (), c.green (), c.blue (), c.alpha ())
		: rectangleRGBA (_render, X1, Y1, X2, Y2, c.red (), c.green (), c.blue (), c.alpha ());
}

// ---
void SDLScreen::drawRoundRectangle (int x1, int y1, int x2, int y2, int rdx, const QGAMES::Color& c, bool f)
{
	Sint16 X1, Y1, X2, Y2;
	convertCoordinatesIntToSInt16 (x1, y1, X1, Y1);
	convertCoordinatesIntToSInt16 (x2, y2, X2, Y2);

	(f)
		? roundedBoxRGBA (_render, X1, Y1, X2, Y2, rdx, c.red (), c.green (), c.blue (), c.alpha ())
		: roundedRectangleRGBA (_render, X1, Y1, X2, Y2, rdx, c.red (), c.green (), c.blue (), c.alpha ());
}

// ---
void SDLScreen::drawPolygon (int* x, int* y, int np, const QGAMES::Color& c, bool f)
{
	assert (x);
	assert (y);
	assert (np != 0);

	Sint16* X = new Sint16 [np];
	Sint16* Y = new Sint16 [np];
	
	// Coverting (downtyping) types could generate mistakes...
	// when the values are bigger than expected...
	// so take care...
	for (int i = 0; i < np; i++) 
		convertCoordinatesIntToSInt16 (x [i], y [i], X [i], Y [i]);

	(f) 
		? filledPolygonRGBA (_render, X, Y, np, c.red (), c.green (), c.blue (), c.alpha ())
		: polygonRGBA (_render, X, Y, np, c.red (), c.green (), c.blue (), c.alpha ());

	delete [] X;
	delete [] Y;
}

//---
void SDLScreen::drawTriangle (int x1, int y1, int x2, int y2, int x3, int y3, const QGAMES::Color& c, bool f)
{
	Sint16 X1, Y1, X2, Y2, X3, Y3;
	convertCoordinatesIntToSInt16 (x1, y1, X1, Y1);
	convertCoordinatesIntToSInt16 (x2, y2, X2, Y2);
	convertCoordinatesIntToSInt16 (x3, y3, X3, Y3);

	(f) 
		? filledTrigonRGBA (_render, X1, Y1, X2, Y2, X3, Y3, c.red (), c.green (), c.blue (), c.alpha ())
		: trigonRGBA (_render, X1, Y1, X2, Y2, X3, Y3, c.red (), c.green (), c.blue (), c.alpha ());
}

// ---
void SDLScreen::drawImage (int x, int y, const std::string& i, int a)
{
	SDL_Surface* surface = IMG_Load (i.c_str ());
	#ifndef NDEBUG
	if (!surface)
		std::cout << IMG_GetError () << std::endl;
	#endif

	if (!surface)
		return;
		
	SDL_Texture* texture = SDL_CreateTextureFromSurface (_render, surface);
	SDL_Rect srcRect, dstRect;
	srcRect.x = 0; srcRect.y = 0;
	srcRect.w = dstRect.w = surface -> w;
	srcRect.h = dstRect.h = surface -> h;
	dstRect.x = x; dstRect.y = y;

	SDL_SetTextureBlendMode (texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod (texture, a); // To fade out/in the image...
	SDL_RenderCopy (_render, texture, &srcRect, &dstRect);
	SDL_FreeSurface (surface);
	SDL_DestroyTexture (texture);
}

// ---
void SDLScreen::drawCircle (int x, int y, int a1, int a2, const QGAMES::Color& c, bool f)
{
	Sint16 X, Y, A1, A2;
	convertCoordinatesIntToSInt16 (x, y, X, Y);
	convertCoordinatesIntToSInt16 (a1, a2, A1, A2);

	(f) 
		? filledEllipseRGBA (_render, X, Y, A1, A2, c.red (), c.green (), c.blue (), c.alpha ())
		: ellipseRGBA (_render, X, Y, A1, A2, c.red (), c.green (), c.blue (), c.alpha ());
}

// ---
void SDLScreen::drawText (int x, int y, const std::string& s,
	const std::string& f, int w, int h, const QGAMES::Color& c, int lt, int ot)
{
	SDL_Color cl = { (Uint8) c.red (), (Uint8) c.green (), (Uint8) c.blue (), (Uint8) c.alpha () };

	// Gets the font if it has been already loaded...
	// Or retrieve it!
	TTF_Font* font = NULL;
	std::map <std::string, TTF_Font*>::const_iterator i;
	if ((i = _fontsLoaded.find (f)) != _fontsLoaded.end ())
		font = (*i).second;
	else
	{
		font = TTF_OpenFont (f.c_str (), w);
		assert (font);
		_fontsLoaded [f] = font;
	}

	// It there is outline...
	if (ot != 0)
		TTF_SetFontOutline (font, ot);

	// Sets the type of letter...
	int lS = 0;
	if (lt & QGAMES::Screen::_FONTBOLD) lS |= TTF_STYLE_BOLD;
	if (lt & QGAMES::Screen::_FONTITALIC) lS |= TTF_STYLE_ITALIC;
	if (lt & QGAMES::Screen::_FONTNORMAL) lS |= TTF_STYLE_NORMAL;
	if (lt & QGAMES::Screen::_FONTSTRIKETHROUGH) lS |= TTF_STYLE_STRIKETHROUGH;
	if (lt & QGAMES::Screen::_FONTUNDERLINE) lS |= TTF_STYLE_UNDERLINE;
	TTF_SetFontStyle (font, lS);

	// Now draw the text...
	SDL_Surface* fontS = TTF_RenderText_Blended (font, s.c_str (), cl);
	SDL_Texture* fontT = SDL_CreateTextureFromSurface (_render, fontS);
	SDL_Rect srcRect, dstRect;
	srcRect.x = 0; srcRect.y = 0;
	srcRect.w = dstRect.w = fontS -> w;
	srcRect.h = dstRect.h = fontS -> h;
	dstRect.x = x; dstRect.y = y;
	SDL_RenderCopy (_render, fontT, &srcRect, &dstRect);

	SDL_FreeSurface (fontS);
	SDL_DestroyTexture (fontT);
}

// ---
QGAMES::Screen::Buffer* SDLScreen::createBuffer (int tp, int w, int h)
{
	SDL_BlendMode		bM = SDL_BlendMode::SDL_BLENDMODE_NONE;
	if (tp == 1)		bM = SDL_BlendMode::SDL_BLENDMODE_BLEND;
	else if (tp == 2)	bM = SDL_BlendMode::SDL_BLENDMODE_ADD;
	else if (tp == 3)	bM = SDL_BlendMode::SDL_BLENDMODE_MOD;
	// ANy other type is none...

	return (new SDLBuffer (this, bM, w, h));
}

// ---
void SDLScreen::convertCoordinatesIntToSInt16 (int x, int y, Sint16& X, Sint16& Y)
{
	bool xOut = x <= (int) std::numeric_limits <Sint16> ().min () || x >= (int) std::numeric_limits <Sint16> ().max (); 
	bool yOut = y <= (int) std::numeric_limits <Sint16> ().min () || y >= (int) std::numeric_limits <Sint16> ().max ();
	if (!xOut && !yOut)
	{
		X = (Sint16) x; Y = (Sint16) y;
	}
	else
	{
		bool xGreater = std::abs (x) >= std::abs (y);
		if (xGreater)
		{
			X = (x > 0) ? std::numeric_limits <Sint16> ().max () : std::numeric_limits <Sint16> ().min ();
			Y = (Sint16) ((float) X * ((float) y / (float) x));
		}
		else
		{
			Y = (y > 0) ? std::numeric_limits <Sint16> ().max () : std::numeric_limits <Sint16> ().min ();
			X = (Sint16) ((float) Y * ((float) x / (float) y));
		}
	}
}
