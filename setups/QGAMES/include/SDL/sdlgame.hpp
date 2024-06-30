/** \ingroup SDL */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: sdlgame.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a sdl implememtation of a game implemenmtation. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_SDLGAME__
#define __QGAMES_SDLGAME__

#include <Common/gameimplementation.hpp>
#include <Common/formbuilder.hpp>
#include <Common/soundbuilder.hpp>
#include <Common/inputhandler.hpp>
#include <SDL.h>

/** SDLGame makes three main things in a different way. \n
	Initialize the SDL system, needed to manage SDL library. \n
	Read and manage the events in a different way than the standard library does.
	and close the SDL system (destroying basic elements created at the beginning) when the game finishes. 
	SDL uses by default it's own sound system, but it can initiated to run with FMOD. */
class SDLGame : public QGAMES::GameImplementation
{
	public:
	SDLGame (bool fmod = false)
		: QGAMES::GameImplementation (),
		  _usingFMOD (fmod)
							{ }

	/** To know whether the implementation is or not using FMOD as sound system. */
	bool usingFMOD () const
							{ return (_usingFMOD); }

	/** @see parent. */
	virtual bool initialize ();
	virtual QGAMES::FormBuilder* createFormBuilder ();
	virtual	QGAMES::SoundBuilder* createSoundBuilder ();
	virtual QGAMES::InputHandler* createInputHandler (QGAMES::InputHandler::Behaviour* bhv);
	virtual QGAMES::Screen* createScreen (const std::string& t, const QGAMES::Position& pos, 
		int w, int h, int pX, int pY);
	virtual void handleEvents ();
	virtual void treatEvent (const QGAMES::Event& evnt);
	virtual void finalize ();

	virtual void treatEvent (SDL_Event& evnt);

	private: // NOT protected
	bool _usingFMOD;
};

#endif
  
// End of the file
/*@}*/
