/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: dwscenes.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 04/04/2019 \n
 *	Description: Defines the base for all scenes in dragonwind like games.
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_SCENES__
#define __DRAGONWIND_SCENES__

#include <DragonwindLike/dwdefinitions.hpp>
#include <DragonwindLike/dwartists.hpp>
#include <Platform/ptinclude.hpp>

namespace DRAGONWIND
{
	class BadGuysActionBlock;

	/** Class to define common funtionality to any scene in a DragonWind game. */
	class Scene : public QGAMES::PlatformScene
	{
		public:
		Scene (int c, const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: QGAMES::PlatformScene (c, m, cn, p, ePL)
							{ }

		/** To know whether the scene is a main one or not.
			In a specific scene could be connections to other scenes where to look for something e.g.
			Those other scenes should be declared as fundamental.
			It is defined in the configuration data file. 
			If nothing has been defined, then NO is returned.
			<Attributes>
				<Attribute id="FUNDAMENTAL" value="YES|NO"/>
				...
			</Attributes>
		 */
		bool fundamental () const;

		/** To know the name of the scene. 
			It is defined in the configuration data file. 
			If nothing has been defined, then a default one is returned. @see dwdefinitions.hpp.
			<Attributes>
				<Attribute id="NAME" value="???"/>
				...
			</Attributes>
		 */
		std::string name () const;

		/** To know which axis the scene moves towards.
			It has to be defined in the scene configuration data file.
			If nothing has been defined a default one is returned. @see dwdefinitions.hpp.
			<Attributes>
				<Attribute id="MOVEMENT" value="X,Y,Z"/>
				...
			</Attributes>
		 */
		QGAMES::Vector movement () const;

		/** To know which the fluidity of the liquids in that scene.
			It has to be defined in the scene configuration data file.
			If nothing has been defined a default one is returned. @see dwdefinitions.hpp.
			<Attributes>
				<Attribute id="LIQUIDSFLUIDITY" value="0..1"/>
				...
			</Attributes>
		 */
		QGAMES::bdata liquidsFluidity () const;
	};

	/** The scene used to play. */
	class PlayingScene : public Scene
	{
		public:
		PlayingScene (int c, const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: Scene (c, m, cn, p, ePL),
			  _ninja (NULL),
			  _backgroundSound (-1)
							{ }

		/** To verify whether there is certain type of element in the scene or not. */
		virtual bool isThingVisible (int t);

		/** @see parent. */
		virtual void initialize ();
		virtual void finalize ();

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		/** To get the first bad guy of a specifi type in the scene. */
		const BadGuy* firstBadGuyType (int t) const;
		BadGuy* firstBadGuyType (int t)
							{ return ((BadGuy*) (((const PlayingScene*) this) -> firstBadGuyType (t))); }
		QGAMES::Entities badGuysType (int t) const;

		/** TO get the action block controlling a specific badguy. */
		const BadGuysActionBlock* actionBlockControlling (BadGuy* bG) const;
		BadGuysActionBlock* actionBlockControlling (BadGuy* bG)
							{ return ((BadGuysActionBlock*) (((const PlayingScene*) this) -> 
								actionBlockControlling (bG))); }

		/** @see parent. 
			When it is a gun, the max number of shoots has to be added. */
		virtual QGAMES::PlatformGame::ThingToCatchDefinition* createThingToCatchDefinitionFrom 
				(const QGAMES::PlatformMap::ObjectLocation& obj) const;

		/** When a explosion happens around a specific position.
			In the default implementation the entities around are affected and die 
			if they can be affected by enemies.
			So the explosion is considered as an enemy regarless who threw it. */
		virtual void explosionAround (Shooting* sth, QGAMES::bdata rdx = __BD 50);

		/** What to do when an entity collisions with a notification tile.
			The method returs true when the collision has been treated and false in any other circunstance. 
			The method receives both a reference to the character who collisions and the number of the frame of 
			the tile that has been collisioned with (and the tile itself).
			It can be overloaded later. */
		virtual bool treatCollisionWithNotificationTile (DRAGONWIND::Character* a, 
			int nF, QGAMES::NotificationTile* t);

		/** To adapt the the scene to the difficulty level. \n
			The difficulty level is a parameter that doesn't depend on the current player . It is common for the game. \n
			This method is invoked when a scene is "initialized". \n
			This method has to adapt everything: Scene itself, entities, etc. 
			By default it adjust some bad guys' behavioural parameters according to the difficulty level,
			and also invokes an equivalent method in the action blocks inside (only if they are DRAGONWIND type). */
		virtual void adaptSceneToDifficultyLevel ();

		protected:
		/** Set when the game is initialized. */
		Ninja* _ninja;

		// Implementation
		/** Only used when playing a background sound, 
			always in background standard channel. */
		int _backgroundSound; 
	};

	/** Just an example */
	class SceneExample : public PlayingScene
	{
		public:
		SceneExample (int c, const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: PlayingScene (c, m, cn, p, ePL),
			  _backgroundMapId (-1), // First time, it hasn't been defined yet...
			  _wall (NULL)
							{ }

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		protected:
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }

		protected:
		/** The number of background map. */
		int _backgroundMapId;

		// Implementation
		QGAMES::AdvancedTileLayer* _wall;

		static const int _SWITCHMESSAGES = 0;
		static const int _COUNTERTOHIDEWALL = 0;
	};

	/** The next scene... */
	class SceneExampleNext : public PlayingScene
	{
		public:
		SceneExampleNext (int c, const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: PlayingScene (c, m, cn, p, ePL),
			  _backgroundMapId (-1) // First time, it hasn't been defined yet...
							{ }

		/** @see parent. */
		virtual void initialize ();

		protected:
		/** The number of background map. */
		int _backgroundMapId;
	};
}

#endif
  
// End of the file
/*@}*/
