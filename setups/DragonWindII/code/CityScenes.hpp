/** \ingroup DWTheRevege */
/*@{*/

/**	
 *	@file	
 *	File: Scenes.hpp \n
 *	Game: Dragonwind II (The Revenge) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 24/01/2020 \n
 *	Description: Defines City Scenes in Dragonwind II \n
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWINDTHEREVENGE_CITYSCENES__
#define __DRAGONWINDTHEREVENGE_CITYSCENES__

#include <DragonwindLike/dwinclude.hpp>
#include "Defs.hpp"

namespace DragonTheRevenge
{
	/** Base for any City scene. */
	class CityScene : public DRAGONWIND::PlayingScene
	{
		public:
		CityScene (int c, const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: DRAGONWIND::PlayingScene (c, m, cn, p, ePL)
							{ }

		/** @see parent. */
		virtual void initialize ();
	};

	/** CityWorld Scene 0. */
	class CityScene0 : public CityScene
	{
		public:
		CityScene0 (const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: CityScene (__DRAGONWINDTHEREVENGE_CITYWORLDSCENE0ID__, m, cn, p, ePL)
							{ }

		/** @see parent. */
		virtual void initialize ();
	};

	/** CityWorld Scene 1. \n
		When a couple of bad guys are killed, the base needed to get the end of the scene appears. */
	class CityScene1 : public CityScene
	{
		public:
		CityScene1 (const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: CityScene (__DRAGONWINDTHEREVENGE_CITYWORLDSCENE1ID__, m, cn, p, ePL),
			  _theBadGuysToKill (),
			  _blockMoatActionBlock (NULL)
							{ }

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		private:
		// Implementation
		/** The bad guys to kill for the base to appear. */
		QGAMES::Entities _theBadGuysToKill;
		/** A reference to the block controlling the moat (appear / dessapear). */
		DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock* _blockMoatActionBlock;
	};

	/** CityWorld Scene 2. */
	class CityScene2 : public CityScene
	{
		public:
		CityScene2 (const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: CityScene (__DRAGONWINDTHEREVENGE_CITYWORLDSCENE2ID__, m, cn, p, ePL)
							{ }

		/** @see parent. */
		virtual void initialize ();
	};

	/** CityWorld Scene 3. \n
		According with the difficulty of the game, 
		the layers at the end of the scene will be ones or others. */
	class CityScene3 : public CityScene
	{
		public:
		CityScene3 (const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: CityScene (__DRAGONWINDTHEREVENGE_CITYWORLDSCENE3ID__, m, cn, p, ePL),
			  _moveableLayersPerDifficulty ()
							{ }

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		private:
		// Implementation
		QGAMES::AdvancedTileLayers _moveableLayersPerDifficulty;
	};

	/** CityWorld Scene 4. \n
		Jack goes for you! */
	class CityScene4 : public CityScene
	{
		public:
		CityScene4 (const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: CityScene (__DRAGONWINDTHEREVENGE_CITYWORLDSCENE4ID__, m, cn, p, ePL),
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
