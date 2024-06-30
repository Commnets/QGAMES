/** \ingroup SDL */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: sdlinputhandler.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a sdl implememtation of an input handler. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_SDLINPUTHANDLER__
#define __QGAMES_SDLINPUTHANDLER__

#include <Common/inputhandler.hpp>
#include <SDL.h>
#include <vector>
#include <map>

typedef std::vector <SDL_Joystick*> SDLJoysticks;

/** SDLInputHandler is just to initialize the Input System using SDL libraries.
	The events are read in the class SDLGame instead of this, just because 
	the way the are treated in SDL are different that the QGAMES' standard proposal. */
class SDLInputHandler : public QGAMES::InputHandler
{
	public:
	SDLInputHandler (QGAMES::Game* g, QGAMES::InputHandler::Behaviour* bhv)
		: QGAMES::InputHandler (g, bhv),
		  _joysticks ()
							{ }

	/** @see parent. */
	virtual void initialize ();
	virtual void finalize ();
	virtual QGAMES::Events readEvents (); 
	virtual void clearPendingEvents ();

	/** @see parent. */
	virtual QGAMES::MouseMovementEventData whereIsMouse ();
	virtual int maxPosJoystick (int nA)
							{ return (32768); }
	virtual int minPosJoystick (int nA)
							{ return (-32768); }

	/** @see parent. */
	virtual QGAMES::MouseStateData mouseState ();
	virtual QGAMES::JoystickStateData joystickState (unsigned int nJ);
	virtual QGAMES::KeyCode translateKeyCode (int kC);
	virtual QGAMES::MouseButtonCode translateMouseButtonCode (int bC);

	/** @see parent. */
	/** In the case of an SDL Input Handler iit is possible to read the status of the keyboard...
		...and treat it. This method converts the internal values read and traslate them into QGAMES possibilities
		callling later to onKeyboardStatus method... */
	virtual void treatKeyboardStatus ();
	/** Read the full status of the joystick and treat it. */
	virtual void treatJoystickStatus ();

	protected:
	void initializeJoysticks ();
	void finalizeJoysticks ();

	protected:
	SDLJoysticks _joysticks;

	private:
	/** This variable is a map to convert SDL key codes into QGAMES ones.
		This is initialized at construction time.*/
	static std::map <int, QGAMES::KeyCode> _keyCodes;
	static std::map <int, QGAMES::KeyCode> initKeyCodes ();
	/** This variable is a map to convert SDL mouse button codes into QGAMES ones. */
	static std::map <int, QGAMES::MouseButtonCode> _mouseButtonCodes;
	static std::map <int, QGAMES::MouseButtonCode> initMouseButtonCodes ();
};

#endif
  
// End of the file
/*@}*/
