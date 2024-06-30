/** \ingroup Example */
/*@{*/

/**	
 *	@file	
 *	File: TestRoomGame.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Forniés (Community Networks) \n
 *	Creation Date: 03/02/2021 \n
 *	Description: Testing how the room game library works. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_TESTROOMGAME__
#define __QGAMES_TESTROOMGAME__

#include "Defs.hpp"
#include <Room/rminclude.hpp>

namespace TestRoomGame
{
	// -------ScoreObjects -----------------------------------------------------------------------
	/** Framerate */
	class FramerateScoreObject final : public QGAMES::ScoreObjectNumber
	{
		public:
		FramerateScoreObject ();

		/** @see parent. 
			It hasn't neither movements, nor animations, not states to copy. */
		virtual QGAMES::Entity* clone () const override
							{ return (new FramerateScoreObject ()); }

		/** @see parent. */
		virtual void updatePositions () override;

		protected:
		/** @see parent */
		virtual int formToDrawDigits () override
							{ return (10000); }
	};

	// -------The entities and their builder -------------------------------------------------------------------------
	/** The brick all walls are made up of. */
	class Brick final : public QGAMES::RoomElement
	{
		public:
		static const int FID = 80000;
		static const int AID = 0;

		Brick (int eId)
			: QGAMES::RoomElement (eId, FID, AID, QGAMES::Position::_cero, 
				QGAMES::Forms { { FID, QGAMES::Game::game () -> form (FID) } })
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const override;

		/** @see parent. */
		virtual void initialize () override;
	};

	/** The picture in the wall of the room. */
	class Picture final : public QGAMES::RoomElement
	{
		public:
		static const int FID = 80000;
		static const int AID = 0;

		Picture (int eId)
			: QGAMES::RoomElement (eId, FID, AID, QGAMES::Position::_cero,
				QGAMES::Forms { { FID, QGAMES::Game::game () -> form (FID) } })
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const override;

		/** @see parent. */
		virtual void initialize () override;
	};

	/** The knight. */
	class Knight final : public QGAMES::Entity
	{
		public:	
		static const int FID = 90000;
		static const int AID = 7;

		Knight (int eId)
			: QGAMES::Entity (eId, QGAMES::Forms { { FID, QGAMES::Game::game () -> form (FID) } })
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const override;

		/** @see parent. */
		virtual void initialize () override;
	};

	/** The cube */
	class Cube final : public QGAMES::Entity
	{
		public:
		static const int FID = 80003;
		static const int AID = 0;

		Cube (int eId)
			: QGAMES::Entity (eId, QGAMES::Forms { { FID, QGAMES::Game::game () -> form (FID) } })
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const override;

		/** @see parent. */
		virtual void initialize () override;
	};

	/** To create the entities used in the example: bricks, knights, etc. */
	class EntityBuilder final : public QGAMES::AdvancedEntityBuilder
	{
		public:
		EntityBuilder (const std::string& fDef, QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
			: QGAMES::AdvancedEntityBuilder (fDef, fB, mB)
							{ }
	
		private:
		/** @see parent. */
		virtual QGAMES::Entity* createEntity (const EntityDefinition& def) override;
	};
	// --------------------------------------------------------------------------------

	// -------The movements-------------------------------------------------------------------------
	/** To create basic movements. 
		Nothing is done for this test. */
	class MovementBuilder final : public QGAMES::MovementBuilder
	{
		public:
		MovementBuilder (const std::string& fDef)
			: QGAMES::MovementBuilder (fDef)
							{ }
	
		private:
		/** @see parent. */
		virtual QGAMES::Movement* createMovement (const MovementDefinition& def) override
							{ return (nullptr); }
	};
	// --------------------------------------------------------------------------------

	// -------The input handler-------------------------------------------------------------------------
	/** The handler managing the input elements. */
	class InputHandlerBehaviour final : public QGAMES::InputHandler::Behaviour
	{
		public:
		InputHandlerBehaviour ()
			: QGAMES::InputHandler::Behaviour ()
							{ }

		/** @see parent. */
		/** Because it is necessary to detect the key to exit the game. */
		virtual void onKeyboardEvent (QGAMES::KeyBoardEventData* dt) override;
	};
	// --------------------------------------------------------------------------------

	// -------The world, scenes and maps-------------------------------------------------------------------------
	/** The very special map, because it moves!. */
	class Map final : public QGAMES::TiledMap
	{
		public:
		Map (int c, const QGAMES::Layers& l, int w, int h, int d, int tW, int tH, int tD,
			const QGAMES::MapProperties& p = QGAMES::MapProperties ())
			: QGAMES::TiledMap (c, l, w, h, d, tW, tH, tD, p)
							{ }
	};

	/** To create the map. */
	class MapBuilderAddsOn final : public QGAMES::TMXMapBuilderAddsOn
	{
		public:
		MapBuilderAddsOn (QGAMES::Sprite2DBuilder* sB)
			: QGAMES::TMXMapBuilderAddsOn (sB)
							{ }

		private:
		virtual QGAMES::Tile* createTile (int id, QGAMES::Form* form, int nf, 
				const QGAMES::TileProperties& p = QGAMES::TileProperties ()) override
							{ return (new QGAMES::Tile (id, form, nf, 0, p)); }
		virtual QGAMES::TileLayer* createTileLayer (int id, const std::string& n, 
				const QGAMES::Tiles& t, QGAMES::Map* m, QGAMES::TileLayer::VisualOrientation o,
				const QGAMES::LayerProperties& p = QGAMES::LayerProperties ()) override
							{ return (new QGAMES::TileLayer (id, n, t, m, o, p)); }
		virtual QGAMES::ObjectLayer* createObjectLayer (int id, const std::string& n, const QGAMES::Objects&, 
				QGAMES::Map* m, const QGAMES::LayerProperties& oP = QGAMES::LayerProperties ()) override
							{ return (nullptr); } // None for the test
		virtual QGAMES::ImageLayer* createImageLayer (int id, const std::string& n, QGAMES::Image* i, 
				QGAMES::Map* m, const QGAMES::LayerProperties& oP = QGAMES::LayerProperties ()) override
							{ return (nullptr); } // None for the test
		virtual QGAMES::Map* createMapObject (int id, const QGAMES::Layers& l, int w, int h, int d, int tW, int tH, int tD,
				const QGAMES::MapProperties& p = QGAMES::MapProperties ()) override
							{ return ((id == __TRMTM_MAPEXAMPLE__) ? 
								new Map (id, l, w, h, d, tW, tH, tD, p) : nullptr); } 
	};

	/** The maze model. */
	class MazeModel final : public QGAMES::MazeModel
	{
		public:
		MazeModel ();
	};

	/** The Room defining the walls and decoration of the example. */
	class Room final : public QGAMES::Room
	{
		public:
		Room ();
	};

	/** The room builder defined for this game. */
	class RoomBuilder final : public QGAMES::RoomBuilder
	{
		public:
		RoomBuilder ()
			: QGAMES::RoomBuilder (std::string ("rooms.xml"), QGAMES::Game::game () -> entityBuilder ())
							{ }
	};

	/** The Scene including the previous Room. */
	class Scene final : public QGAMES::BasicRoomScene
	{
		public:
		Scene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
					const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
					const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ());

		/** @see parent. */
		virtual void initialize () override;

		private:
		Knight* _knight;
	};

	/** The class defining the head world. */
	class World final : public QGAMES::WorldBasedOnRooms
	{
		public:
		World (int c, const QGAMES::Scenes& s, 
				const QGAMES::WorldProperties& p = QGAMES::WorldProperties ())
			: QGAMES::WorldBasedOnRooms (c, s, new MazeModel, p)
							{ }

		/** @see parent. */
		virtual void initialize () override;
	};

	/** To crete the world and scenes used in the test. */
	class WorldBuilder final : public QGAMES::WorldBuilder
	{
		public: 
		WorldBuilder (const std::string& eDef, QGAMES::MapBuilder* mB)
			: QGAMES::WorldBuilder (eDef, mB)
							{ }

		private:
		virtual QGAMES::World* createWorldObject (int no, const QGAMES::Scenes& s, 
			const QGAMES::WorldProperties& p) override;
		virtual QGAMES::Scene* createSceneObject (int ns, const QGAMES::Maps& m, 
			const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL) override;
	};
	// --------------------------------------------------------------------------------

	// -------The Game-------------------------------------------------------------------------
	class Game final : public QGAMES::ArcadeGame
	{
		public:
		Game ();

		/** @see parent. */
		virtual void initialize () override;
		virtual void finalize () override;

		/** @see parent. */
		virtual void addScoreObjects () override;

		/** To get the room builder. */
		RoomBuilder* roomBuilder () 
							{ return (_roomBuilder == nullptr ? (_roomBuilder = new RoomBuilder ()) : _roomBuilder); }

		private:
		/** see parent. */
		virtual QGAMES::EntityBuilder* createEntityBuilder () override
							{ return (new EntityBuilder (parameter (__GAME_PROPERTYENTITIESFILE__), 
								formBuilder (), movementBuilder ())); }
		virtual QGAMES::MovementBuilder* createMovementBuilder () override
							{ return (new MovementBuilder (parameter (std::string (__GAME_PROPERTYMOVEMENTSFILE__)))); }
		virtual QGAMES::InputHandler* createInputHandler () override
							{ return (implementation () -> createInputHandler (new InputHandlerBehaviour ())); }
		virtual QGAMES::Screens createScreens () override
							{ QGAMES::Screens result;
								result.insert (QGAMES::Screens::value_type (__QGAMES_MAINSCREEN__, 
									implementation () -> createScreen 
										(std::string ("Room Test"),
										QGAMES::Position::_cero, 800, 600, 100, 100)));
								return (result); }
		virtual QGAMES::WorldBuilder* createWorldBuilder () override
							{ return (new WorldBuilder (parameter (__GAME_PROPERTYWORLDSFILE__), 
								mapBuilder ())); }
		virtual QGAMES::MapBuilder* createMapBuilder () override;

		private:
		RoomBuilder* _roomBuilder;
	};
	// --------------------------------------------------------------------------------
}

#endif
  
// End of the file
/*@}*/
