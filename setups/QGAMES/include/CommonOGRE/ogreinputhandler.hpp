/** \ingroup OGRE */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: ogreinputhandler.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 27/08/2017 \n
 *	Description: Defines a class to control the events in an ogre game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_OGREINPUTHANDLER__
#define __QGAMES_OGREINPUTHANDLER__

#include <SDL/sdlinputhandler.hpp>
#include <OgreHeaderPrefix.h>
#include <OgreFrameListener.h>
#include <OgreHeaderSuffix.h>

/** To represent a input handler in OGRE.
	Most of the behaviour isrelated with SDL one. */
class OGREInputHandler : public SDLInputHandler
{
	public:
	/** In OGRE events have to be read when the frame system ends or finishes. 
		So it is needed a frame listener first of all. */
	class Handler : public Ogre::FrameListener
	{
		public:
		Handler (OGREInputHandler* iH, Ogre::Root* r, Ogre::RenderWindow* w);
		~Handler ();

		// To get the main attributes of the handler...
		InputHandler* inpuHandler ()
							{ return (_inputHandler); }
		Ogre::RenderWindow* window ()
							{ return (_window); }

		// To return the last events treated...
		Ogre::FrameEvent* lastFrameEvent ()
							{ return (_lastFrameEvent); }

		/** @see parent. */
		virtual bool frameRenderingQueued (const Ogre::FrameEvent& evt);
		virtual bool frameEnded (const Ogre::FrameEvent& evt); 

		private:
		/** Reference to the input handler owing this handler. */
		OGREInputHandler* _inputHandler;
		/** A reference to the window this handler is acting over. */
		Ogre::RenderWindow* _window;
		/** A reference to the root element. */
		Ogre::Root* _root;

		// Impementation
		/** This variable to keep the last frame event. */
		Ogre::FrameEvent* _lastFrameEvent;
	};

	OGREInputHandler (Ogre::Root* r, QGAMES::Game* g, QGAMES::InputHandler::Behaviour* bhv);
	// The destructor is only needed to destroy the handler itself...
	// All other internal objects are destroyed when the main window
	// is closed. Take care of destroying screens before the input handler...
	// By the way this is the default behaviour!
	~OGREInputHandler ();

	/** To get a reference to the internal handler. */
	Handler* handler ()
							{ return (_handler); }

	/** To treat the sdl events. */
	void OGREInputHandler::treatEvent (SDL_Event& evnt);

	private:
	// Implementation
	/** A reference to the root of the system. */
	Ogre::Root* _root;
	/** The handler. */
	Handler* _handler;
};

#endif

// End of the file
/*@}*/