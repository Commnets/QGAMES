/** \ingroup SDL */
/** \ingroup Screen */
/*@{*/

/**	
 *	@file	
 *	File: sdlscreen.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a sdl implememtation of an screen. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_SDLSCREEN__
#define __QGAMES_SDLSCREEN__

#include <Common/screen.hpp>
#include <SDL.h>
#include <SDL_ttf.h>
#include <map>

/** Class to represent a zommer in SDL. 
	This is a software zoomer. It declares methods to activate and desactivate the zoom using 
	the main directives of the SDL framework. */
class SDLZoommerOnScreen : public QGAMES::ZoommerOnScreen
{
	public:
	SDLZoommerOnScreen (QGAMES::bdata v, int mW, int mH, SDL_Renderer* r,
			QGAMES::ZoommerOnScreen::Effect e = QGAMES::ZoommerOnScreen::Effect::_TOUPLEFT);

	/** @see parent. */
	virtual void beforeDrawing ();
	virtual void afterDrawing ();

	private:
	SDL_Renderer* _render;
};

/** Class to represent an screen in SDL framework. */
class SDLScreen : public QGAMES::Screen
{
	public:
	class SDLBuffer : public QGAMES::Screen::Buffer
	{
		public:
		SDLBuffer (QGAMES::Screen* s, SDL_BlendMode bM, int w = -1, int h = -1);
		virtual ~SDLBuffer ();

		/** @see parent. */
		virtual void open ();
		virtual void close ();
		virtual void moveToScreen (int x = 0, int y = 0);
		virtual void moveToBuffer (QGAMES::Screen::Buffer* b, int x = 0, int y = 0);

		private:
		// Implementation
		SDL_Texture* _textureBuffer;
	};

	/** The scale is only used in this version of SDL to determinate where to center the screen,
		just because the SDL members returning the resolution of the screen don't take into account 
		the windows DPI adjust. The rest of SDL functions work properly. */
	SDLScreen (const std::string& t, const QGAMES::Position& p, int w, int h, 
		int wPX, int wPY, float sX = 1.0f, float sY = 1.0f);
	virtual ~SDLScreen ();

	virtual void setIcon (QGAMES::Form* f);
	virtual void windowAtPosition (int x, int y);
	virtual void windowAtCenter ();

	SDL_Window* window () 
							{ return (_window); }
	SDL_Renderer* render ()
							{ return (_render); }

	protected:
	class SDLTransaction : public QGAMES::Screen::Transaction
	{
		public:
		SDLTransaction (QGAMES::Screen* s)
			: QGAMES::Screen::Transaction (s)
							{ }
		
		protected:
		/** @see parent. */
		virtual void openFirst ();
		virtual void closeLast ();
		virtual void openOne () { } // Nothing special opening one transaction
		virtual void closeOne () { }
	};

	friend SDLTransaction;
	friend SDLBuffer;

	protected:
	virtual void drawForm (int x, int y, QGAMES::Form* f, int px, int py, int w, int h, int a);
	virtual void drawPoint (int x, int y, const QGAMES::Color& c);
	virtual void drawLine (int x1, int y1, int x2, int y2, const QGAMES::Color& c, int t);
	virtual void drawRectangle (int x1, int y1, int x2, int y2, const QGAMES::Color& c, bool f);
	/** This function won't work with a projection diferent than orthogonal... */
	virtual void drawRoundRectangle (int x1, int y1, int x2, int y2, int rdx, const QGAMES::Color& c, bool f);
	virtual void drawPolygon (int* x, int* y, int np, const QGAMES::Color& c, bool f);
	virtual void drawTriangle (int x1, int y1, int x2, int y2, int x3, int y3, const QGAMES::Color& c, bool f);
	virtual void drawImage (int x, int y, const std::string& i, int a);
	virtual void drawCircle (int x, int y, int a1, int a2, const QGAMES::Color& c, bool f);
	virtual void drawText (int x, int y, const std::string& s,
		const std::string& f, int w, int h, const QGAMES::Color& c, int lt, int ot);
	virtual QGAMES::Screen::Transaction* createTransaction () 
							{ return (new SDLTransaction (this)); }
	virtual QGAMES::Screen::Buffer* createBuffer (int tp, int w, int h);
								  

	private:
	void convertCoordinatesIntToSInt16 (int x, int y, Sint16& X, Sint16& Y);

	private:
	SDL_Window* _window;
	SDL_Renderer* _render;
	int _windowPositionX, _windowPositionY;
	float _scaleX, _scaleY;

	// Implementation
	std::map <std::string, TTF_Font*> _fontsLoaded;
};

#endif
  
// End of the file
/*@}*/
