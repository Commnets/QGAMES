/**	
 *	@file	
 *	File: main.cpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 16/04/2018 \n
 *	Description: Main programm for testing simple things: Ninja Running but without any implementation dependency!
 *	Versions: 1.0 Initial
 */

#include "stdafx.h"
#include <SDL/sdlgame.hpp>
#include "Defs.hpp"

namespace HelloWorld
{
	/** Empty: Needed but not used in this example. */
	class MovementBuilder final : public QGAMES::MovementBuilder
	{
		public:
		MovementBuilder (const std::string& fDef)
			: QGAMES::MovementBuilder (fDef)
								{ }
	
		private:
		virtual QGAMES::Movement* createMovement (const MovementDefinition& def) override
								{ return (nullptr); }
	};

	/** To define the ninja of the game. */
	class Ninja final : public QGAMES::Character
	{
		public:
		Ninja (int cId, const QGAMES::Forms& f = QGAMES::Forms ())
			: QGAMES::Character (cId, f)
							{ }

		virtual Entity* clone () const override
							{ return (new Ninja (id (), forms ())); }

		virtual void initialize () override;
		virtual void updatePositions () override;

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }

		private:
		static const int _TOCHANGEASPECTCOUNTERID	= 0;
		static const int _NUMBERACTIVEASPECTCOUNTERID = 1;
	};

	/** Empty: Needed but not used in this example. */
	class EntityBuilder final : public QGAMES::EntityBuilder
	{
		public:
		EntityBuilder (const std::string& fDef, QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
			: QGAMES::EntityBuilder (fDef, fB, mB)
								{ }
	
		private:
		virtual QGAMES::Entity* createEntity (const EntityDefinition& def) override
								{ return (nullptr); }
	};

	/** Empty: Needed but not used in this example. */
	class InputHandlerBehaviour final : public QGAMES::InputHandler::StandardBehavior
	{
		public:
		virtual void manageKeyOnState (QGAMES::GameState* st, int kP) override { }
		virtual void manageJoystickMovementOnState (QGAMES::GameState* st, int nJ, const QGAMES::Vector& dr) { }
		virtual void manageJoystickButtonOnState (QGAMES::GameState* st, int nJ, bool p, int nB, int bF) { }
	};

	/** The core of the hello world example. */
	class Game final : public QGAMES::Game
	{
		public:
		Game ();
		~Game ();

		protected:
		/** Needed but not used in this example. */
		virtual QGAMES::EntityBuilder* createEntityBuilder () override
							{ return (new EntityBuilder (game () -> parameter (std::string (__GAME_PROPERTYENTITIESFILE__)),
								formBuilder (), movementBuilder ())); }
		virtual QGAMES::MovementBuilder* createMovementBuilder () override
							{ return (new MovementBuilder (game () -> parameter (std::string (__GAME_PROPERTYMOVEMENTSFILE__)))); }
		virtual QGAMES::InputHandler* createInputHandler () override
							{ return (implementation () -> createInputHandler (new InputHandlerBehaviour ())); }

		virtual QGAMES::Screens createScreens ();

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void drawOn (QGAMES::Screen* s) override;
		virtual void finalize () override;

		private:
		QGAMES::Character* _ninja;
	};
}

// ---
void HelloWorld::Ninja::initialize ()
{ 
	reStartAllCounters ();
	counter (_TOCHANGEASPECTCOUNTERID) -> initialize 
		((int) ((float) game () -> framesPerSecond () * 0.1f /** 0.1 seconds before changing the aspect. */), 0, true, true);

	setCurrentForm (__HELLOWORLD_NINJARIGHTFORMID__); 
	setCurrentAspect (__HELLOWORLD_NINJARUNNINGRIGHTINITIALASPECT__); 
}

// ---
void HelloWorld::Ninja::updatePositions ()
{
	QGAMES::Character::updatePositions ();

	if (counter (_TOCHANGEASPECTCOUNTERID) -> isEnd ())
	{
		counter (_NUMBERACTIVEASPECTCOUNTERID) -> isEnd ();

		setCurrentAspect (counter (_NUMBERACTIVEASPECTCOUNTERID) -> value ());
	}
}

// ---
__IMPLEMENTCOUNTERS__ (HelloWorld::Ninja::Counters)
{
	// To redefine _TOCHANGEASPECTCOUNTERID when initilized ninja, so initial values are not now taken into account
	addCounter (new QGAMES::Counter (_TOCHANGEASPECTCOUNTERID, 0, 0, true, true)); 
	addCounter (new QGAMES::Counter (_NUMBERACTIVEASPECTCOUNTERID, __HELLOWORLD_NINJARUNNINGASPECTS__, 0, true, true)); 
}

// ---
HelloWorld::Game::Game ()
	: QGAMES::Game (/** The specific implementation **/ new SDLGame ()),
	  _ninja (nullptr)
{ 
}

// ---
HelloWorld::Game::~Game ()
{ 
	delete (_ninja); 
}

// ---
QGAMES::Screens HelloWorld::Game::createScreens ()
{ 
	QGAMES::Screens result;
	result.insert (QGAMES::Screens::value_type (__QGAMES_MAINSCREEN__,
		implementation () -> createScreen (std::string ("Hello World"), QGAMES::Position::_cero, 400, 400, 0, 0)));
	return (result); 
}

// ---
void HelloWorld::Game::initialize ()
{
	QGAMES::Game::initialize ();

	if (!isInitialized ())
		return;

	mainScreen () -> windowAtCenter ();

	/** Creates the ninja entity, with its different forms! */
	delete (_ninja); // Just in case...
	_ninja = new Ninja (__HELLOWORLD_NINJAENTITYID__, 
		{ { __HELLOWORLD_NINJARIGHTFORMID__,	form (__HELLOWORLD_NINJARIGHTFORMID__) }, 
		  { __HELLOWORLD_NINJALEFTFORMID__,		form (__HELLOWORLD_NINJALEFTFORMID__) } });
	_ninja -> initialize ();
}

// ---
void HelloWorld::Game::updatePositions ()
{
	QGAMES::Game::updatePositions ();

	_ninja -> updatePositions ();
}

// ---
void HelloWorld::Game::drawOn (QGAMES::Screen* s)
{
	QGAMES::Game::drawOn (s);

	form (__HELLOWORLD_BACKGROUND2FORMID__) -> drawOn (s, 0, QGAMES::Position::_cero);

	_ninja -> drawOn (s, QGAMES::Position (__BD 50, __BD 300, __BD 0));
}

// ---
void HelloWorld::Game::finalize ()
{
	QGAMES::Game::finalize ();

	_ninja -> finalize ();

	delete (_ninja);
	_ninja = nullptr;
}

#ifndef _CONSOLE
#include <SDL.h>
#ifdef __cplusplus
#define C_LINKAGE "C"
#else
#define C_LINKAGE
#endif /* __cplusplus */
#if _MSC_VER >= 1900
extern C_LINKAGE FILE __iob_func[3] = { *stdin,*stdout,*stderr };
#endif
extern C_LINKAGE int main (int argc, char *argv[])
#else
int _tmain (int argc, char *argv [])
#endif /* _CONSOLE */
{
	HelloWorld::Game game;
	game.setLinkDrawToFrameRate (true);
	game.exec ();

	return (0);
}
