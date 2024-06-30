/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: gameimplementation.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines an specific implementation for a game (SDL?). \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_GAMEIMPLEMENTATION__
#define __QGAMES_GAMEIMPLEMENTATION__

#include <Common/event.hpp>
#include <Common/inputhandler.hpp>

namespace QGAMES
{
	class Game;
	class FormBuilder;
	class SoundBuilder;
	class Screen;
	class _ExEngine;

	/** 
	  * The class GameImplementation represents the way many different thing
	  *	are really implemented, and in many ocassions they will depend on the libraries
	  *	used behind this framework. e.g. It is not the same to create an input handler 
	  *	under SDL library than in OGRE. \n
	  * Those different thigns are usually the way forms are loaded and represented,
	  * the way sounds are implementd, and the way graphics (through the screen object) are managed too. \
	  * So these are the methods to define for a specific implementation. 
	  */
	class GameImplementation
	{
		public:
		GameImplementation ()
			: _game (NULL)
							{ }
		virtual ~GameImplementation ()
							{ }

		/** This method is used when the game class is built. \n
			The constructor receives always a reference to the basic implementation,
			and then the reference to the game class owning that implementation is set. */
		void setGame (Game* g) 
							{ _game = g; }
		Game* game ()
							{ return (_game); }

		/** To create the external engine if the implementation needs it. 
			By default it is not needed. */
		virtual _ExEngine* createExternalEngine ()
							{ return (NULL); }

		// These all methods are invoked from the exec method of the game class...
		/** Sometimes the implementation libraries have to be initialized. \n
			By default nothing is done, and then true is always returned. */
		virtual bool initialize ()
							{ return (true); }

		/** How to create the form builder and the sound builder is something usually really linked
			to the libraries finally used behind. So these methods are invoked from the game main loop at its initialization,
			and they have to be redefined per library. */
		virtual FormBuilder* createFormBuilder () = 0;
		virtual	SoundBuilder* createSoundBuilder () = 0;

		/** A similar situation happens with the input handler and any screen.
			Those methods are also invoked from the game mainloop just once the builders
			have been created. The input handler creation process needs a reference to the behaviour. */
		virtual InputHandler* createInputHandler (InputHandler::Behaviour* bhv) = 0;
		virtual Screen* createScreen (const std::string& t, const Position& pos, int w, int h, int pX, int pY) = 0;
		virtual void initializeScreens ()
							{ } // Some times, screens created have to be initialized...
								// This method is called after creating screens in the game main loop!
		virtual void finalizeScreens ()
							{ } // Some times, screens created (or around) have to be finalized
							    // This method is invoked when the main loop finishes...

		/** 
		  *	Once the main loop starts to run, 
		  *	how to manage getting events from the system, handle and treat them can be different. \n
		  *	A basic implementation is added, but it can be changed. \n
		  *	The basic implementation consists on getting the events from the input handler
		  *	decalared at game class level, invoke the treatment methods defined there also,
		  *	and delete the event once it has been treated (@see SDL implementation to see how it changes). 
		  */
		virtual void inEveryLoop ()
							{ }
		virtual void handleEvents ();
		virtual void treatEvent (const Event& evnt);

		/** When the main loop is about to finish this method will be invoked.
			Some ocassions the libraries used have to be finalized.
			By default nothing is done. */
		virtual void finalize ()
							{ }

		protected:
		Game* _game;
	};
}

#endif
  
// End of the file
/*@}*/
