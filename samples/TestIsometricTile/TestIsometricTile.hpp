/** \ingroup Example */
/*@{*/

/**	
 *	@file	
 *	File: main.cpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/01/2018 \n
 *	Description: A pure QGAMES game based in SDL. Definition file.
 *	Versions: 1.0 Initial
 */

#ifndef __TESTISOMETRICTILEINCLUDE__
#define __TESTISOMETRICTILEINCLUDE__

#include "stdafx.h"
#include "Defs.hpp"

#define __PROJECTIONSWORLD__ 100
#define __PROJECTIONSSCENE__ 100

namespace TestIsometric
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

	// -------The entities-------------------------------------------------------------------------
	/** Any entity of this demo. */
	class Entity : public QGAMES::Entity
	{
		public:
		Entity (int eId, const QGAMES::Forms& frms = QGAMES::Forms ())
			: QGAMES::Entity (eId, frms)
							{ }

		/** @see parent. */
		virtual void drawOn (QGAMES::Screen* scr, const QGAMES::Position& pos = QGAMES::Position::_noPoint) override;
	};

	/** The cube */
	class Cube final : public Entity
	{
		public:
		static const int FID = 80003;
		static const int AID = 0;

		Cube (int eId)
			: Entity (eId, QGAMES::Forms { { FID, QGAMES::Game::game () -> form (FID) } })
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const override;

		/** @see parent. */
		virtual void initialize () override;
		virtual void drawOn (QGAMES::Screen* scr, const QGAMES::Position& pos = QGAMES::Position::_noPoint) override;
	};

	/** The knight. */
	class Knight final : public Entity
	{
		public:	
		static const int FID = 90000;
		static const int AID = 7;

		Knight (int eId)
			: Entity (eId, QGAMES::Forms { { FID, QGAMES::Game::game () -> form (FID) } })
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const override;

		/** @see parent. */
		virtual void initialize () override;
	};

	/** To create basic entities. 
		Nothing is done. */
	class EntityBuilder final : public QGAMES::EntityBuilder
	{
		public:
		EntityBuilder (const std::string& fDef, QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
			: QGAMES::EntityBuilder (fDef, fB, mB)
							{ }

		private:
		virtual QGAMES::Entity* createEntity (const EntityDefinition& def) override
							{ return ((def._id >= __TITM_CUBEBASEID__ && def._id < (__TITM_CUBEBASEID__ + __TITM_NUMBERCUBES__)) 
								? (QGAMES::Entity*) new Cube (def._id) 
								: ((def._id >= __TITM_KNIGHTBASEID__ && def._id < (__TITM_KNIGHTBASEID__ + __TITM_NUMBERKNIGHTS__)) 
									? (QGAMES::Entity*) new Knight (def._id) : NULL)); }
	};
	// --------------------------------------------------------------------------------

	// -------The movements-------------------------------------------------------------------------
	/** To create basic movements. 
		Nothing is done for this test */
	class MovementBuilder final : public QGAMES::MovementBuilder
	{
		public:
		MovementBuilder (const std::string& fDef)
			: QGAMES::MovementBuilder (fDef)
							{ }
	
		private:
		/** @see parent. */
		virtual QGAMES::Movement* createMovement (const MovementDefinition& def) override
							{ return (NULL); }
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

	// -------The wordls and the map-------------------------------------------------------------------------
	/** The very special map, because it moves!. */
	class Map final : public QGAMES::TiledMap
	{
		public:
		Map (int c, const QGAMES::Layers& l, int w, int h, int d, int tW, int tH, int tD,
			const QGAMES::MapProperties& p = QGAMES::MapProperties ())
			: QGAMES::TiledMap (c, l, w, h, d, tW, tH, tD, p),
			  _movXDir (__BD 1)
							{ }

		/** @see parent. */
		virtual void initialize () override;
		virtual void updatePositions () override;

		private:
		// Implementation
		QGAMES::bdata _movXDir;
	};

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
							{ return (NULL); } // None for the test
		virtual QGAMES::ImageLayer* createImageLayer (int id, const std::string& n, QGAMES::Image* i, 
				QGAMES::Map* m, const QGAMES::LayerProperties& oP = QGAMES::LayerProperties ()) override
							{ return (NULL); } // None for the test
		virtual QGAMES::Map* createMapObject (int id, const QGAMES::Layers& l, int w, int h, int d, int tW, int tH, int tD,
				const QGAMES::MapProperties& p = QGAMES::MapProperties ()) override
							{ return ((id == __TITM_EXAMPLEMAPID__) ? 
								new Map (id, l, w, h, d, tW, tH, tD, p) : NULL); } 
	};

	/** Just to add the right scene. */
	class World final : public QGAMES::World
	{
		public:
		World (int c, const QGAMES::Scenes& s, const QGAMES::WorldProperties& p = QGAMES::WorldProperties ())
			: QGAMES::World (c, s, p)
							{ }

		/** @see parent. */
		virtual void initialize () override
							{ setScene (__TITM_EXAMPLESCENEID__); 
							  QGAMES::World::initialize (); }
	};

	/** Just to add the right map and artists, and moving them across the tiles
		to test how the important methods really work (drawing, locating, ordering, etc.). */
	class Scene final : public QGAMES::Scene
	{
		public:
		Scene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: QGAMES::Scene (c, m , cn, p, ePL),
			  _cube (NULL), _knight (NULL),
			  _movZDir (__BD -1)
							{ }

		/** @see parent. */
		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint) override;
		virtual void finalize () override;

		private:
		Cube* _cube;
		Knight* _knight;

		// Implementation
		QGAMES::bdata _movZDir;
	};

	/** To build the scene and the world. */
	class WorldBuilder final : public QGAMES::WorldBuilder
	{
		public: 
		WorldBuilder (const std::string& eDef, QGAMES::MapBuilder* mB)
			: QGAMES::WorldBuilder (eDef, mB)
							{ }

		private:
		virtual QGAMES::World* createWorldObject (int no, const QGAMES::Scenes& s, 
			const QGAMES::WorldProperties& p) override
							{ return ((no == __TITM_EXAMPLEWORLDID__) 
								? new World (no, s, p) : NULL); }
		virtual QGAMES::Scene* createSceneObject (int ns, const QGAMES::Maps& m, 
			const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL) override
							{ return ((ns == __TITM_EXAMPLESCENEID__) 
								? new Scene (ns, m, cn, p, ePL) : NULL); }
	};
	// --------------------------------------------------------------------------------

	/** The demo game. */
	class Game final : public QGAMES::ArcadeGame
	{
		public:
		Game ();

		/** @see parent. */
		virtual void initialize ();
		virtual void finalize ();

		/** @see parent. */
		virtual void addScoreObjects ();

		private:
		/** see parent. */
		virtual QGAMES::EntityBuilder* createEntityBuilder () override
							{ return (new EntityBuilder (parameter (std::string (__GAME_PROPERTYENTITIESFILE__)), 
								formBuilder (), movementBuilder ())); }
		virtual QGAMES::MovementBuilder* createMovementBuilder () override
							{ return (new MovementBuilder (parameter (std::string (__GAME_PROPERTYMOVEMENTSFILE__)))); }
		virtual QGAMES::InputHandler* createInputHandler () override
							{ return (implementation () -> createInputHandler (new InputHandlerBehaviour ())); }
		virtual QGAMES::Screens createScreens () override
							{ QGAMES::Screens result;
								result.insert (QGAMES::Screens::value_type (__QGAMES_MAINSCREEN__, 
									implementation () -> createScreen 
										(std::string ("Test Isometric"),
										QGAMES::Position::_cero, 800, 600, 100, 100)));
								return (result); }
		virtual QGAMES::WorldBuilder* createWorldBuilder () override
							{ return (new WorldBuilder (parameter (std::string (__GAME_PROPERTYWORLDSFILE__)), mapBuilder ())); }
		virtual QGAMES::MapBuilder* createMapBuilder () override;
	};
}

#endif
  
// End of the file
/*@}*/