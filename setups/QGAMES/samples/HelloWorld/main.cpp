/**	
 *	@file	
 *	File: main.cpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 16/04/2018 \n
 *	Description: Main programm for testing simple things: Ninja running!
 *	Versions: 1.0 Initial
 */

#include "stdafx.h"
#include <SDL/sdlgame.hpp>
#include <SDL/sdlscreen.hpp>
#include <SDL/sdlform.hpp>
#include <SDL_image.h>
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

	/** To represent a ninja. */
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
		QGAMES::Form* _ninjaForm;
		QGAMES::Character* _ninja;
	};
}

// ---
void HelloWorld::Ninja::initialize ()
{ 
	reStartAllCounters ();

	setCurrentForm (0); 
	setCurrentAspect (0); 
}

// ---
void HelloWorld::Ninja::updatePositions ()
{
	QGAMES::Character::updatePositions ();

	if (counter (0) ->isEnd ())
	{
		counter (1) -> isEnd ();

		setCurrentAspect (counter (1) -> value ());
	}
}

// ---
__IMPLEMENTCOUNTERS__ (HelloWorld::Ninja::Counters)
{
	addCounter (new QGAMES::Counter (0, 5, 0, true, true)); // Brake
	addCounter (new QGAMES::Counter (1, 6, 0, true, true)); // Different running aspects
}

// ---
HelloWorld::Game::Game ()
	: QGAMES::Game (/** The specific implementation **/ new SDLGame ()),
	  _ninjaForm (nullptr), 
	  _ninja (nullptr)
{ 
	// Noithing else to execute
}

// ---
HelloWorld::Game::~Game ()
{ 
	delete (_ninja); 
	delete (_ninjaForm); 
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
	SDLScreen* scr = dynamic_cast <SDLScreen*> (mainScreen ());

	/** Creates the ninja sprite. */
	delete (_ninjaForm); // Just in case...
	QGAMES::Sprite2DFormData* ninjaFormData = 
		new QGAMES::Sprite2DFormData (10, 0 /* Ortogonal */, 
			0, 0, 640, 64, 64, 64, 255, 0, new QGAMES::Sprite2DFrameDataFactory);
	SDL_Surface* sfc = IMG_Load (std::string ("./ninjaright.png").c_str ());
	_ninjaForm = new SDLForm (0, std::string ("ninja"), 
			10, std::string ("./ninjaright.png"), 640, 64, ninjaFormData, 
			SDL_CreateTextureFromSurface (scr -> render (), sfc));
	SDL_FreeSurface (sfc);
	_ninjaForm -> preLoad ();

	/** Creates the ninja entity. */
	delete (_ninja); // Just in case...
	QGAMES::Forms ninjaForms; 
	ninjaForms [_ninjaForm -> id ()] = _ninjaForm;
	_ninja = new Ninja (0, ninjaForms);
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

	_ninja -> drawOn (s, QGAMES::Position (__BD 100, __BD 100, __BD 0));
}

// ---
void HelloWorld::Game::finalize ()
{
	QGAMES::Game::finalize ();

	_ninja -> finalize ();

	delete (_ninja);
	_ninja = nullptr;
	delete (_ninjaForm);
	_ninjaForm = nullptr;
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
