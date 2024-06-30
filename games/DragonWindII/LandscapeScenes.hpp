/** \ingroup DWTheRevege */
/*@{*/

/**	
 *	@file	
 *	File: Scenes.hpp \n
 *	Game: Dragonwind II (The Revenge) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 30/03/2020 \n
 *	Description: Defines Landscape Scenes in Dragonwind II \n
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWINDTHEREVENGE_LANDSCAPESCENES__
#define __DRAGONWINDTHEREVENGE_LANDSCAPESCENES__

#include <DragonwindLike/dwinclude.hpp>
#include "Entities.hpp"
#include "Defs.hpp"

namespace DragonTheRevenge
{
	/** Base for any Landscape scene. */
	class LandscapeScene : public DRAGONWIND::PlayingScene
	{
		public:
		LandscapeScene (int c, const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: DRAGONWIND::PlayingScene (c, m, cn, p, ePL)
							{ }

		/** @see parent. */
		virtual void initialize ();
	};

	/** LandscapeWorld Scene 0. */
	class LandscapeScene0 : public LandscapeScene
	{
		public:
		LandscapeScene0 (const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: LandscapeScene (__DRAGONWINDTHEREVENGE_LANDSCAPEWORLDSCENE0ID__, m, cn, p, ePL)
							{ }

		/** @see parent. */
		virtual void initialize ();
	};

	/** LandscapeWorld Scene 1. \n
		The most complex scene of the game with no doubt. \n
		Going to the underground!. \n
		There is a gorilla watching the main pass from one side to other. \n
		The gorilla moves towards a coin, leaving the pass free! Take the coin in the room aside. \n
		There is two rooms aside hidden by a block. The block can be broken with an explosion. */
	class LandscapeScene1 : public LandscapeScene
	{
		public:
		LandscapeScene1 (const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: LandscapeScene (__DRAGONWINDTHEREVENGE_LANDSCAPEWORLDSCENE1ID__, m, cn, p, ePL),
			  _blockRemoveableActionBlocks (),
			  _gorilla (NULL), 
			  _gorillaActionBlock (NULL)
							{ }

		/** @see parent. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		private:
		/** @see parent. 
			When the explosion happens the walls (right / left) could be destroyed too, 
			cleaning up the way to interesting lateral caves. */
		virtual void explosionAround (DRAGONWIND::Shooting* sth, QGAMES::bdata rdx = __BD 50);

		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }

		private:
		// Implementation
		/** The blocks managing how the lefta nd right wall appears & disappears. */
		std::vector <DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock*> _blockRemoveableActionBlocks;
		/** A reference to the gorilla and the action block controlling it. */
		DRAGONWIND::BadGuysActionBlock* _gorillaActionBlock;
		DRAGONWIND::BadGuy* _gorilla;

		static const int _NUMBERWALLS = 2;
		static const int _SWITCHTOSHOWWALLS [_NUMBERWALLS];
		static const int _SWITCHCOINSHOWN = _NUMBERWALLS; // Always the next...
	};

	/** LandscapeWorld Scene 2. */
	class LandscapeScene2 : public LandscapeScene
	{
		public:
		LandscapeScene2 (const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: LandscapeScene (__DRAGONWINDTHEREVENGE_LANDSCAPEWORLDSCENE2ID__, m, cn, p, ePL)
							{ }

		/** @see parent. */
		virtual void initialize ();
	};

	/** LandscapeWorld Scene 3. */
	class LandscapeScene3 : public LandscapeScene
	{
		public:
		LandscapeScene3 (const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: LandscapeScene (__DRAGONWINDTHEREVENGE_LANDSCAPEWORLDSCENE3ID__, m, cn, p, ePL)
							{ }

		/** @see parent. */
		virtual void initialize ();
	};

	/** LandscapeWorld Scene 4. \n
		Jack goes for you. */
	class LandscapeScene4 : public LandscapeScene
	{
		public:
		LandscapeScene4 (const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: LandscapeScene (__DRAGONWINDTHEREVENGE_LANDSCAPEWORLDSCENE4ID__, m, cn, p, ePL),
			  _mainBadGuy (NULL),
			  _badGuyEnergyLevel (NULL)
							{ }

		/** @see parent. */
		virtual void initialize ();
		virtual void finalize ();

		private:
		// Implementation
		DRAGONWIND::BadGuy* _mainBadGuy;
		DRAGONWIND::EntityEnergy* _badGuyEnergyLevel;
	};
}

#endif

// End of the file
/*@}*/
