#include "stdafx.h"

#include <Arcade/arcadegame.hpp>
#include <Arcade/worldbuilder.hpp>
#include <Arcade/mapbuilder.hpp>
#include <Common/movementbuilder.hpp>
#include <Common/formbuilder.hpp>
#include <Common/entitybuilder.hpp>
#include <Common/soundbuilder.hpp>
#include <iostream>
#include <time.h>
#include <tinyxml.h>

namespace TestArcade
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

	class MapBuilderTestAddsOn : public QGAMES::TMXMapBuilderAddsOn
	{
		public:
		MapBuilderTestAddsOn (QGAMES::Sprite2DBuilder* sB)
			: QGAMES::TMXMapBuilderAddsOn (sB)
								{ }

		private:
		virtual QGAMES::Tile* createTile (int id, QGAMES::Form* form, int nf, 
				const QGAMES::TileProperties& p = QGAMES::TileProperties ())
								{ return (NULL); }
		virtual QGAMES::TileLayer* createTileLayer (int id, const std::string& n, 
				const QGAMES::Tiles& t, QGAMES::Map* m, QGAMES::TileLayer::VisualOrientation o,
				const QGAMES::LayerProperties& p = QGAMES::LayerProperties ())
								{ return (NULL); }
		virtual QGAMES::ObjectLayer* createObjectLayer (int id, const std::string& n, const QGAMES::Objects&, 
				QGAMES::Map* m, const QGAMES::LayerProperties& oP = QGAMES::LayerProperties ())
								{ return (NULL); }
		virtual QGAMES::ImageLayer* createImageLayer (int id, const std::string& n, QGAMES::Image* i, 
				QGAMES::Map* m, const QGAMES::LayerProperties& oP = QGAMES::LayerProperties ())
								{ return (NULL); }
		virtual QGAMES::Map* createMapObject (int id, const QGAMES::Layers& l, int w, int h, int d, int tW, int tH, int tD,
				const QGAMES::MapProperties& p = QGAMES::MapProperties ())
								{ return (NULL); }
	};

	class WorldBuilderTest : public QGAMES::WorldBuilder
	{
		public: 
		WorldBuilderTest (const std::string& eDef, QGAMES::MapBuilder* mB)
			: QGAMES::WorldBuilder (eDef, mB)
								{ }

		private:
		virtual QGAMES::World* createWorldObject (int no, const QGAMES::Scenes& s, 
			const QGAMES::WorldProperties& p)
								{ return (NULL); }
		virtual QGAMES::Scene* createSceneObject (int ns, const QGAMES::Maps& m, 
			const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
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
								{ }
		virtual Transaction* createTransaction ()
								{ return (new Transaction (this)); }
		virtual Buffer* createBuffer (int tp, int w, int h)
								{ return (new Buffer (this, w, h)); }
	};

	class ArcadeTestImplementation : public QGAMES::GameImplementation
	{
		public:
		ArcadeTestImplementation ()
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

	class ArcadeTest : public QGAMES::ArcadeGame
	{
		public:
		ArcadeTest ()
			: QGAMES::ArcadeGame (new ArcadeTestImplementation (), QGAMES::Worlds ()),
			  _pos (10, 0, 0),
			  _loopCount (0)
								{ }

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

		virtual QGAMES::WorldBuilder* createWorldBuilder ()
								{ return (new WorldBuilderTest (parameter (__GAME_PROPERTYWORLDSFILE__), 
									mapBuilder ())); }
		virtual QGAMES::MapBuilder* createMapBuilder ()
								{ QGAMES::MapBuilder* result = new QGAMES::MapBuilder (parameter (__GAME_MAPSOBJECTSFILE__));
								  result -> addAddsOn (new MapBuilderTestAddsOn 
									((QGAMES::Sprite2DBuilder*) formBuilder ()));
								  return (result); }

		virtual void drawOn (QGAMES::Screen* s)
								{ _pos = _pos.rotate (QGAMES::Position (0,0,0), 
								  QGAMES::Position (0,0,1), 
										(QGAMES::bdata) ((QGAMES::bdata) 30 * __PI / (QGAMES::bdata)180));
								  std::cout << _pos << "  ";
								  std::cout << QGAMES::Timer::timer () -> loopsPerSecond () << std::endl; }

		private:
		QGAMES::Position _pos;
		int _loopCount;
	};
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestArcade::ArcadeTest game;
	game.exec ();
	return (0);
}

