/** \ingroup Example */
/*@{*/

/**	
 *	@file	
 *	File: TestSDL.hpp \n
 *	Framework: Test SDL \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 15/01/2018 \n
 *	Description: The definitions needed just for an example to test SDL libraries
 *	Versions: 1.0 Initial
 */

#ifndef __GAMETESTSDLINCLUDE__
#define __GAMETESTSDLINCLUDE__

#include "Defs.hpp"

namespace TestSDL
{
	// -------The GUI System-------------------------------------------------------------------------
	/** Defines a decorator to show the element selected. */
	class SelectionDecorator : public QGAMES::Widget::Decorator
	{
		public:
		SelectionDecorator (QGAMES::Form* frm)
			: QGAMES::Widget::Decorator (),
			  _form (frm)
							{ assert (frm); }


		/** @see parent. */
		void decorate (QGAMES::Screen* scr, const QGAMES::Position& p, QGAMES::Widget* wdt);
		QGAMES::Widget::Decorator* clone () const
							{ return (new SelectionDecorator (_form)); }
		bool isBefore () { return (false); }

		private:
		QGAMES::Form* _form;
	};

	/** Defines a widget coantaining all and managing the user iteration and selection. */
	class MainWidget : public QGAMES::CompositeWidget
	{
		public:
		MainWidget (int id, QGAMES::Widget::Decorator* d = NULL, 
			const QGAMES::ListOfWidgets& wdts = QGAMES::ListOfWidgets (), int wF = -1)
				: QGAMES::CompositeWidget (id, d, wdts, wF),
				  _inputDeviceType (0), _startSelected (false)
							{ }

		bool startSelected () const
							{ return (_startSelected); }
		int inputDeviceType () const
							{ return (_inputDeviceType); }
		const std::string& playerName ()
							{ return (((QGAMES::IntroWidget*) widget (__GAMETEST_NAMEWIDGETID__)) -> text ()); }
		
		/** @see parent. */
		virtual void initialize ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& e);

		private:
		int _inputDeviceType; // 0 keyboard, 1 joystick
		bool _startSelected;
	};

	/** The standard GUI System builder.
		It is used by most of the implementations. */
	class GUISystemBuilder : public QGAMES::StandardGUISystemBuilder
	{
		public:
		GUISystemBuilder (const std::string& fDef, QGAMES::FormBuilder* fB)
			:  QGAMES::StandardGUISystemBuilder (fDef, fB)
							{ }

		protected:
		/** @see parent. */
		virtual QGAMES::Widget* createWidget (const QGAMES::GUISystemBuilder::WidgetDefinition& def);
	};
	// --------------------------------------------------------------------------------

	// -------The input handler-------------------------------------------------------------------------
	class InputHandlerBehaviour : public QGAMES::InputHandler::StandardBehavior
	{
		public:
		InputHandlerBehaviour ()
			: QGAMES::InputHandler::StandardBehavior ()
							{ }

		/** @see parent. */
		virtual void onKeyboardStatus (const std::vector <bool>& kS);
		virtual void manageKeyOnState (QGAMES::GameState* st, int kP);
		virtual void manageJoystickMovementOnState (QGAMES::GameState* st, int nJ, const QGAMES::Vector& dr);
		virtual void manageJoystickButtonOnState (QGAMES::GameState* st, int nJ, bool p, int nB, int bF);
	};
	// --------------------------------------------------------------------------------

	// -------The Score-------------------------------------------------------------------------
	/** Defines a basic score object. 
		Just an example shown at the bottom of the screen. */
	class ScoreObject : public QGAMES::ScoreObject
	{
		public:
		ScoreObject (int id, 
				const QGAMES::Forms& frms = QGAMES::Forms (), const QGAMES::Entity::Data& dt = QGAMES::Entity::Data ())
			: QGAMES::ScoreObject (id, frms, dt)
								{ }

		/** @see parent. 
			They don't have either movements, nor animations, nor states to copy. */
		virtual Entity* clone () const
								{ return (new ScoreObject (_id, _forms, _data)); }
	};
	// --------------------------------------------------------------------------------

	// -------The characters-------------------------------------------------------------------------
	/** Defines the basic entity managed by the user and used in this test. */
	class MainCharacter : public QGAMES::ArtistInATiledMap
	{
		public:
		MainCharacter (int id, 
				const QGAMES::Forms& frms = QGAMES::Forms (), const QGAMES::Entity::Data& dt = QGAMES::Entity::Data ());

		/** @see parent. */
		virtual Entity* clone () const;
		virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a);

		// Controlling the movement...
		bool lookingToRight ();
		void toTheRight ();
		bool goingRight ();
		void toTheLeft ();
		bool goingLeft ();
		void toFall ();
		bool falling ();
		void toJump ();
		bool jumping ();
		void toStay ();
		bool staying ();

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);
	
		/** @see parent. */
		virtual void whenCollisionWith (const QGAMES::Tiles& t, 
			QGAMES::ArtistInATiledMap::TilePosition tP, QGAMES::TileLayer* l = NULL);
		virtual void whenOnTile (const QGAMES::Tiles& t, QGAMES::TileLayer* l = NULL);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& e);

		private:
		virtual void adjustRelevants (QGAMES::bdata& m, QGAMES::Vector& pv)
								{ m = (QGAMES::bdata) 0.90; pv = QGAMES::Vector::_cero; }

		private:
		/** When the main charater can't move a notification cames to processEvent method
			If the movement is changed there the system will crash, because a notifier disappears 
			(the previous movemenet) in the middle of the threatment loop. 
			So a buoy is set insted and treated in everyLoop method later. */
		class ToChangeStateBuoy : public QGAMES::Buoy
		{
			public:
			ToChangeStateBuoy ()
				: QGAMES::Buoy (__GAMETEST_CHANGESTATEBUOYID__, __BD -1) 
								{ }

			/** @see parent. */
			virtual void* treatFor (QGAMES::Element* e);
		};

		QGAMES::Layer* _layerUnder;
		QGAMES::Layers _layersToTest;
	};

	/** To create basic entities. */
	class EntityBuilder : public QGAMES::EntityBuilder
	{
		public:
		EntityBuilder (const std::string& fDef, QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
			: QGAMES::EntityBuilder (fDef, fB, mB)
								{ }
	
		private:
		virtual QGAMES::Entity* createEntity (const EntityDefinition& def);
	};
	// --------------------------------------------------------------------------------

	// -------The movements-------------------------------------------------------------------------
	/** To create basic movements. 
		Nothing is done. */
	class MovementBuilder : public QGAMES::MovementBuilder
	{
		public:
		MovementBuilder (const std::string& fDef)
			: QGAMES::MovementBuilder (fDef)
								{ }
	
		private:
		virtual QGAMES::Movement* createMovement (const MovementDefinition& def);
	};
	// --------------------------------------------------------------------------------

	// -------The world, scenes and maps-------------------------------------------------------------------------
	/** The class defining the head world. */
	class World : public QGAMES::World
	{
		public:
		World (int c, const QGAMES::Scenes& s, 
				const QGAMES::WorldProperties& p = QGAMES::WorldProperties ())
			: QGAMES::World (c, s, p)
								{ }

		/** @see parent. */
		void initialize ();
	};

	/** The class defining the scene inside the world above used in the test. */
	class Scene : public QGAMES::Scene
	{
		public:
		Scene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
					const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
					const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
				: QGAMES::Scene (c, m, cn, p, ePL)
								{ }

		/** @see parent. */
		virtual void initialize ();
		virtual void finalize ();
	};

	/** The class repreenting the map where the entiies are playing */
	class Map : public QGAMES::TiledMap
	{
		public:
		Map (int c, const QGAMES::Layers& l, int w, int h, int tW, int tH,
			const QGAMES::MapProperties& p = QGAMES::MapProperties ())
				: QGAMES::TiledMap (c, l, w, h, 0, tW, tH, 0, p),
				  _relevantLayers ()
								{ }
	
		/** @see parent. */
		virtual QGAMES::Layers relevantLayers ();

		private:
		// Implementation
		QGAMES::Layers _relevantLayers;
	};

	/** The layer representing a moving platform. */
	class PlatformLayer : public QGAMES::TileLayer
	{
		public:
		PlatformLayer (int c, const std::string& n, const QGAMES::Tiles& t, 
			QGAMES::Map* m, QGAMES::TileLayer::VisualOrientation o,
			const QGAMES::LayerProperties p = QGAMES::LayerProperties ())
				: QGAMES::TileLayer (c, n, t, m, o, p),
				  _internalMovement (NULL)
							{ }

		/** @see parent. */
		virtual void initialize ();

		private:
		// Implementation
		QGAMES::Layer::MovementBehaviour* _internalMovement;
	};

	/** To build up the map,layers and tiles. */
	class MapsBuilderAddsOn : public QGAMES::TMXMapBuilderAddsOn
	{
		public:
		MapsBuilderAddsOn (QGAMES::Sprite2DBuilder* sB)
			: QGAMES::TMXMapBuilderAddsOn (sB)
								{ }

		private:
		virtual QGAMES::Tile* createTile (int id, QGAMES::Form* form, int nf, 
			const QGAMES::TileProperties& p = QGAMES::TileProperties ());
		virtual QGAMES::TileLayer* createTileLayer (int id, const std::string& n, 
			const QGAMES::Tiles& t, QGAMES::Map* m, QGAMES::TileLayer::VisualOrientation o, 
			const QGAMES::LayerProperties& p = QGAMES::LayerProperties ());
		virtual QGAMES::ObjectLayer* createObjectLayer (int id, const std::string& n, const QGAMES::Objects&, 
			QGAMES::Map* m, const QGAMES::LayerProperties& oP = QGAMES::LayerProperties())
								{ return (NULL); }
		virtual QGAMES::ImageLayer* createImageLayer (int id, const std::string& n, QGAMES::Image* i, 
			QGAMES::Map* m, const QGAMES::LayerProperties& oP = QGAMES::LayerProperties ())
								{ return (NULL); }
		virtual QGAMES::Map* createMapObject (int id, const QGAMES::Layers& l, int w, int h, int d, 
			int tW, int tH, int tD, const QGAMES::MapProperties& p = QGAMES::MapProperties ());
	};

	/** The extension to create maps. */
	class WorldBuilder : public QGAMES::WorldBuilder
	{
		public: 
		WorldBuilder (const std::string& eDef, QGAMES::MapBuilder* mB)
			: QGAMES::WorldBuilder (eDef, mB)
								{ }

		private:
		virtual QGAMES::World* createWorldObject (int no, const QGAMES::Scenes& s, 
			const QGAMES::WorldProperties& p);
		virtual QGAMES::Scene* createSceneObject (int ns, const QGAMES::Maps& m, 
			const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL);
	};
	// --------------------------------------------------------------------------------

	// -------The states of the game-------------------------------------------------------------------------
	/** The game states.
		First state: The presentation of the game. */
	class StatePresentation : public QGAMES::GameState 
	{
		public:
		StatePresentation (QGAMES::Game* g)
			: QGAMES::GameState (__GAMETEST_STATEPRESENTATION__, g, NULL),
			  _options (NULL)
								{ }
		~StatePresentation ()
								{ delete (_options); }

		/** @see parent. */
		void onEnter ();
		void updatePositions ();
		void drawOn (QGAMES::Screen* s);
		void onExit ();

		private:
		QGAMES::GUISystem* _options;
	};

	/** When playing. */
	class StatePlay : public QGAMES::GameState 
	{
		public:
		StatePlay (QGAMES::Game* g)
			: QGAMES::GameState (__GAMETEST_STATEPLAY__, g)
								{ }

		/** @see parent. */
		void onEnter ();
		void drawOn (QGAMES::Screen* s);
		void onExit ();
	};
 
	/** Afetr reaching the flag. */
	class StateEnd : public QGAMES::GameState 
	{
		public:
		static std::string _name;

		StateEnd (QGAMES::Game* g)
			: QGAMES::GameState (__GAMETEST_STATEEND__, g)
								{ }

		/** @see parent. */
		void onEnter ();
		void updatePositions ();
		void drawOn (QGAMES::Screen* s);

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		private:
		// Implementation
		/** To count when the zoom has to act. */
		static const int _COUNTERCHANGEZOOM = 0;
		/** Switch to control whether the background is zooming out or in. */
		static const int _SWITCHZOOMOUT = 0;

	};
	// --------------------------------------------------------------------------------

	// -------The game itself-------------------------------------------------------------------------
	/** The demo game. */
	class Game : public QGAMES::ArcadeGame
	{
		public:
		class ConfData : public QGAMES::Game::Configuration
		{
			public:
			enum InputDeviceType
			{
				_KEYBOARD = 0,
				_JOYSTICK = 1
			};

			ConfData ()
				: _inputDeviceType (_KEYBOARD),
				  _playerName (std::string ("ICF")),
				  _playerScore (0)
							{ }

			/** @see parent. */
			virtual Configuration* clone () const
							{ return (new ConfData (*this)); }

			InputDeviceType _inputDeviceType;
			std::string _playerName;
			int _playerScore;

			protected:
			/** @see parent. */
			virtual int confToData (char*& str);
			virtual void dataToConf (char* str, int nB);
		};

		Game ();

		/** @see parent. */
		virtual void addScoreObjects ();

		/** @see parent. */
		void processEvent (const QGAMES::Event& e);

		private:
		/** see parent. */
		virtual QGAMES::EntityBuilder* createEntityBuilder ()
								{ return (new EntityBuilder (parameter (__GAME_PROPERTYENTITIESFILE__), 
									formBuilder (), movementBuilder ())); }
		virtual QGAMES::GUISystemBuilder* createGUISystemBuilder ()
								{ return (new GUISystemBuilder (game () -> parameter (__GAME_PROPERTYGUISYSTEMFILE__), 
									game () -> formBuilder ())); }
		virtual QGAMES::MovementBuilder* createMovementBuilder () 
								{ return (new MovementBuilder (parameter (__GAME_PROPERTYMOVEMENTSFILE__))); }
		virtual QGAMES::InputHandler* createInputHandler ()
								{ return (implementation () -> createInputHandler (new InputHandlerBehaviour ())); }
		virtual QGAMES::Screens createScreens ()
								{ QGAMES::Screens result;
								  result.insert (QGAMES::Screens::value_type (__QGAMES_MAINSCREEN__, 
										implementation () -> createScreen 
											(std::string ("SDLTest"),
											QGAMES::Position::_cero, __GAMETEST_SCREENWIDTH__, __GAMETEST_SCREENHEIGHT__, 100, 100)));
								  return (result); }
		virtual QGAMES::WorldBuilder* createWorldBuilder ()
								{ return (new WorldBuilder (parameter (__GAME_PROPERTYWORLDSFILE__), 
									mapBuilder ())); }
		virtual QGAMES::MapBuilder* createMapBuilder ()
								{ QGAMES::MapBuilder* result = new QGAMES::MapBuilder (parameter (__GAME_MAPSOBJECTSFILE__));
								  result -> addAddsOn (new QGAMES::ObjectMapBuilderAddsOn (objectBuilder ()));
								  result -> addAddsOn (new MapsBuilderAddsOn ((QGAMES::Sprite2DBuilder*) formBuilder ())); // For maps...
								  return (result); }

		/** @see parent. */
		virtual Configuration* createConfiguration ()
							{ return (new ConfData ()); } 
		virtual void initialize ();
		virtual void finalize ();
	};
}

#endif
  
// End of the file
/*@}*/
