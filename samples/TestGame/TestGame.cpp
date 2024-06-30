#include "stdafx.h"

#include <Common/game.hpp>
#include <Common/gameimplementation.hpp>
#include <Common/movementbuilder.hpp>
#include <Common/formbuilder.hpp>
#include <Common/objectbuilder.hpp>
#include <Common/entitybuilder.hpp>
#include <Common/soundbuilder.hpp>
#include <Common/inputhandler.hpp>
#include <iostream>
#include <time.h>

namespace TestGame
{
	class FormBuilderTest : public QGAMES::Sprite2DBuilder
	{
		public:
		FormBuilderTest (const std::string& fDef)
			: QGAMES::Sprite2DBuilder (fDef)
								{ }
	
		private:
		virtual QGAMES::Form* createForm (FormDefinition* def)
								{ return (NULL); }
	};

	class MovementBuilderTest : public QGAMES::MovementBuilder
	{
		public:
		MovementBuilderTest (const std::string& fDef)
			: QGAMES::MovementBuilder (fDef)
								{ }
	
		private:
		virtual QGAMES::Movement* createMovement (const MovementDefinition& def)
								{ return (NULL); }
	};

	class SoundBuilderTest : public QGAMES::SoundBuilder
	{
		public:
		SoundBuilderTest (const std::string& fDef)
			: QGAMES::SoundBuilder (fDef)
								{ }
	
		private:
		virtual QGAMES::Sound* createSound (const SoundDefinition& def)
								{ return (NULL); }
		virtual void createSoundSystem ()
								{ }
	};

	class EntityBuilderTest : public QGAMES::EntityBuilder
	{
		public:
		EntityBuilderTest (const std::string& fDef, QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
			: QGAMES::EntityBuilder (fDef, fB, mB)
								{ }
	
		private:
		virtual QGAMES::Entity* createEntity (const EntityDefinition& def)
								{ return (NULL); }
	};

	class InputHandlerTest : public QGAMES::InputHandler
	{
		public:
		InputHandlerTest (QGAMES::Game* g, QGAMES::InputHandler::Behaviour* bhv)
			: QGAMES::InputHandler (g, bhv)
								{ }

		virtual QGAMES::Events readEvents () 
								{ return (QGAMES::Events ()); }
		virtual void clearPendingEvents ()
								{ }

		virtual QGAMES::MouseStateData mouseState () 
								{ return (QGAMES::MouseStateData ()); }
		virtual QGAMES::JoystickStateData joystickState (unsigned int nJ)
								{ return (QGAMES::JoystickStateData ()); }
	};

	class SimpleScreen : public QGAMES::Screen
	{
		public:
		class Buffer : public QGAMES::Screen::Buffer
		{
			public:
			Buffer (QGAMES::Screen* s, int w = -1, int h = -1)
				: QGAMES::Screen::Buffer (s, w, h)
								{ }

			virtual void open ()
								{ }
			virtual void close ()
								{ }
			virtual void moveToScreen (int x = 0, int y = 0)
								{ }
			virtual void moveToBuffer (QGAMES::Screen::Buffer* b,int x = 0, int y = 0)
								{ }
		};

		SimpleScreen (const std::string& t, int w, int h)
			: QGAMES::Screen (t, w, h)
								{ }

		protected:
		class Transaction : public QGAMES::Screen::Transaction
		{
			public:
			Transaction (QGAMES::Screen* s)
				: QGAMES::Screen::Transaction (s)
								{ }
		
			protected:
			virtual void openFirst ()
								{ }
			virtual void closeLast ()
								{ }
			virtual void openOne () 
								{ }
			virtual void closeOne () 
			{ }
		};

		protected:
		virtual void drawForm (int x, int y, QGAMES::Form* f, int px, int py, int w, int h, int a)
								{ }
		virtual void drawPoint (int x, int y, const QGAMES::Color& c)
								{ }
		virtual void drawLine (int x1, int y1, int x2, int y2, const QGAMES::Color& c, int t)
								{ }
		virtual void drawRectangle (int x1, int y1, int x2, int y2, const QGAMES::Color& c, bool f)
								{ }
		virtual void drawPolygon (int* x, int* y, int np, const QGAMES::Color& c, bool f)
								{ }
		virtual void drawTriangle (int x1, int y1, int x2, int y2, int x3, int y3, const QGAMES::Color& c, bool f)
								{ }
		virtual void drawImage (int x, int y, const std::string& i, int a)
								{ }
		virtual void drawCircle (int x, int y, int a1, int a2, const QGAMES::Color& c, bool f)
								{ }
		virtual void drawText (int x, int y, const std::string& s,
			const std::string& f, int w, int h, const QGAMES::Color& c, int lt, int ot)
								{}
		virtual Transaction* createTransaction ()
								{ return (new Transaction (this)); }
		virtual Buffer* createBuffer (int tp, int w, int h)
								{ return (new Buffer (this, w, h)); }
	};

	class GameTestImplementation : public QGAMES::GameImplementation
	{
		public:
		GameTestImplementation ()
			: QGAMES::GameImplementation ()
							{ }

		virtual QGAMES::FormBuilder* createFormBuilder ()
							{ return (new FormBuilderTest (game () -> parameter (__GAME_PROPERTYFORMSFILE__))); }
		virtual	QGAMES::SoundBuilder* createSoundBuilder ()
							{ return (new SoundBuilderTest (game () -> parameter (__GAME_PROPERTYSOUNDSFILE__))); }
		virtual QGAMES::InputHandler* createInputHandler (QGAMES::InputHandler::Behaviour* bhv)
							{ return (new InputHandlerTest (_game, bhv)); }
		virtual QGAMES::Screen* createScreen (const std::string& t, const QGAMES::Position& pos, int w, int h, int pX, int pY)
							{ return (new SimpleScreen (t, w, h)); }
	};

	class GameTest : public QGAMES::Game
	{
		public:
		GameTest ()
			: QGAMES::Game (new GameTestImplementation ()),
			  _pos (10, 0, 0)
								{ }

		virtual void initialize () override
		{
			QGAMES::Game::initialize ();

			__WLOGPLUS ("|red|hola");
			__WLOGPLUS ("|yellow|hola");
			__WLOGPLUS ("|green|hola");
			__WLOGPLUS ("|cyan|hola");
			__WLOGPLUS ("|blue|hola");
			__WLOGPLUS ("|magenta|hola");
			__WLOGPLUS ("|grey|hola");
			__WLOGPLUS ("|dark_red|hola");
			__WLOGPLUS ("|dark_yellow|hola");
			__WLOGPLUS ("|dark_green|hola");
			__WLOGPLUS ("|dark_cyan|hola");
			__WLOGPLUS ("|dark_blue|hola");
			__WLOGPLUS ("|dark_magenta|hola");
			__WLOGPLUS ("|dark_grey|hola");
			__WLOGPLUS ("|back_red|hola");
			__WLOGPLUS ("|back_yellow|hola");
			__WLOGPLUS ("|back_green|hola");
			__WLOGPLUS ("|back_cyan|hola");
			__WLOGPLUS ("|back_blue|hola");
			__WLOGPLUS ("|back_magenta|hola");
			__WLOGPLUS ("|back_grey|hola");
		}

		private:
		virtual QGAMES::EntityBuilder* createEntityBuilder ()
								{ return (new EntityBuilderTest (game () -> parameter (__GAME_PROPERTYENTITIESFILE__),
									formBuilder (), movementBuilder ())); }
		virtual QGAMES::MovementBuilder* createMovementBuilder () 
								{ return (new MovementBuilderTest (parameter (__GAME_PROPERTYMOVEMENTSFILE__))); }
		virtual QGAMES::InputHandler* createInputHandler ()
								{ return (implementation () -> createInputHandler (new QGAMES::InputHandler::Behaviour ())); }
		virtual QGAMES::Screens createScreens ()
								{ QGAMES::Screens result;
								  result.insert (QGAMES::Screens::value_type (__QGAMES_MAINSCREEN__,
									  implementation () -> createScreen (std::string (__NULL_STRING__), 
										QGAMES::Position::_cero, 100, 100, 0, 0)));
								  return (result); }

		virtual void drawOn (QGAMES::Screen* s)
								{ _pos = _pos.rotate (QGAMES::Position (0,0,0), 
								  QGAMES::Position (0,0,1), 
										(QGAMES::bdata) ((QGAMES::bdata) 30 * __PI / (QGAMES::bdata)180));
								  std::cout << _pos << "  ";
								  std::cout << QGAMES::Timer::timer () -> loopsPerSecond () << std::endl; }

		private:
		QGAMES::Position _pos;
	};
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestGame::GameTest game;

	game.exec ();
	return (0);
}

