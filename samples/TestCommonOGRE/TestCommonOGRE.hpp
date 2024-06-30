/** \ingroup Example */
/*@{*/

/**	
 *	@file	
 *	File: main.cpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/01/2018 \n
 *	Description: A pure QGAMES game based in OGRE behind. Definition file.
 *	Versions: 1.0 Initial
 */

#ifndef __TESTCOMMONOGREINCLUDE__
#define __TESTCOMMONOGREINCLUDE__

#include "stdafx.h"
#include "Defs.hpp"

namespace TestOGRE
{
	// -------The score-------------------------------------------------------------------------
	/** Defines a basic score object. */
	class ScoreObject : public QGAMES::ScoreObject
	{
		public:
		ScoreObject (int id, 
				const QGAMES::Forms& frms = QGAMES::Forms (), const QGAMES::Entity::Data& dt = QGAMES::Entity::Data ())
			: QGAMES::ScoreObject (id, frms, dt)
								{ }

		/** @see parent. 
			They don't have either movements, nor animations, nor states to copy. */
		virtual QGAMES::Entity* clone () const
								{ return (new ScoreObject (_id, _forms, _data)); }
	};
	// --------------------------------------------------------------------------------

	// -------The entity-------------------------------------------------------------------------
	/** Defines the basic entity used by this test. */
	class Entity : public QGAMES::Entity
	{
		public:
		Entity (int id, 
				const QGAMES::Forms& frms = QGAMES::Forms (), const QGAMES::Entity::Data& dt = QGAMES::Entity::Data ())
			: QGAMES::Entity (id, frms, dt)
								{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;
	};

	/** To create basic entities. 
		Nothing is done. */
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

	// -------The movement-------------------------------------------------------------------------
	/** To create basic movements. 
		Nothing is done. */
	class MovementBuilder : public QGAMES::MovementBuilder
	{
		public:
		MovementBuilder (const std::string& fDef)
			: QGAMES::MovementBuilder (fDef)
								{ }
	
		private:
		virtual QGAMES::Movement* createMovement (const MovementDefinition& def)
								{ return (NULL); }
	};
	// --------------------------------------------------------------------------------

	// -------The input handler-------------------------------------------------------------------------
	/** The handler managing the input elements. */
	class InputHandlerBehaviour : public QGAMES::InputHandler::Behaviour
	{
		public:
		InputHandlerBehaviour ()
			: QGAMES::InputHandler::Behaviour ()
								{ }

		/** @see parent. */
		/** Because it is necessary to detect the key to exit the game. */
		virtual void onKeyboardEvent (QGAMES::KeyBoardEventData* dt);
	};
	// --------------------------------------------------------------------------------

	// -------The world, scenes and maps-------------------------------------------------------------------------
	/** The class defining the head world. */
	class World : public QGAMES::World
	{
		public:
		World (int c, const QGAMES::Scenes& s, 
			   const QGAMES::WorldProperties& p = QGAMES::WorldProperties ())
			: QGAMES::World (c, s, p),
			  _pX (0), _pY (0)
								{ }

		/** @see parent. */
		void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position&p = QGAMES::Position::_noPoint);

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
								{ return (new Counters ()); }

		private:
		// Implementation
		/** To count when to change the position of the screen to draw */
		static const int _COUNTERCHANGEANGLE = 0;
		/** To count the angle the screen is now. */
		static const int _COUNTINGANGLE = 1;

		QGAMES::bdata _pX, _pY;
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
		virtual void updatePositions ();

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		private:
		// Implementation
		/** To count when the scene has to change. */
		static const int _COUNTERCHANGESCENE = 0;
		/** Switch to control whether the scene is an state or in another. */
		static const int _SWITCHSTATEACTIVE = 0;
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

	// -------The game-------------------------------------------------------------------------
	/** The demo game. */
	class Game : public QGAMES::ArcadeGame
	{
		public:
		Game ();

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		/** @see parent. */
		virtual void addScoreObjects ();

		private:
		/** see parent. */
		virtual QGAMES::EntityBuilder* createEntityBuilder ()
								{ return (new EntityBuilder (parameter (__GAME_PROPERTYENTITIESFILE__), 
									formBuilder (), movementBuilder ())); }
		virtual QGAMES::MovementBuilder* createMovementBuilder () 
								{ return (new MovementBuilder (parameter (__GAME_PROPERTYMOVEMENTSFILE__))); }
		virtual QGAMES::InputHandler* createInputHandler ()
								{ return (implementation () -> createInputHandler (new InputHandlerBehaviour ())); }
		virtual QGAMES::Screens createScreens ()
								{ QGAMES::Screens result;
								  result.insert (QGAMES::Screens::value_type (__QGAMES_MAINSCREEN__, 
										implementation () -> createScreen 
											(std::string ("Test"),
											QGAMES::Position::_cero, 800, 600, 100, 100)));
								  return (result); }
		virtual QGAMES::WorldBuilder* createWorldBuilder ()
								{ return (new WorldBuilder (parameter (__GAME_PROPERTYWORLDSFILE__), 
									mapBuilder ())); }
		virtual QGAMES::MapBuilder* createMapBuilder ()
								{ QGAMES::MapBuilder* result = new QGAMES::MapBuilder (parameter (__GAME_MAPSOBJECTSFILE__));
								  result -> addAddsOn (new QGAMES::ObjectMapBuilderAddsOn (objectBuilder ()));
								  return (result); }

		private:
		// A reference to the camera used in the game...
		QGAMES::CameraControlledByInputHandler* _cameraManager;

		// Implementation
		int _count;
		int _up;
	};
}

#endif
  
// End of the file
/*@}*/