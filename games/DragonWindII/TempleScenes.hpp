/** \ingroup DWTheRevege */
/*@{*/

/**	
 *	@file	
 *	File: Scenes.hpp \n
 *	Game: Dragonwind II (The Revenge) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 19/04/2020 \n
 *	Description: Defines the Temple Scenes of Dragonwind II \n
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWINDTHEREVENGE_TEMPLESCENES__
#define __DRAGONWINDTHEREVENGE_TEMPLESCENES__

#include <DragonwindLike/dwinclude.hpp>
#include "Defs.hpp"

namespace DragonTheRevenge
{
	/** Base for any Temple scene. */
	class TempleScene : public DRAGONWIND::PlayingScene
	{
		public:
		TempleScene (int c, const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: DRAGONWIND::PlayingScene (c, m, cn, p, ePL),
			  _mainBadGuy (NULL),
			  _badGuyEnergyLevel (NULL)
							{ }

		/** @see parent. */
		virtual void initialize ();
		virtual void finalize ();

		protected:
		DRAGONWIND::BadGuy* _mainBadGuy;
		DRAGONWIND::EntityEnergy* _badGuyEnergyLevel;
	};

	/** Temple Scene 0. */
	class TempleScene0 : public TempleScene
	{
		public:
		TempleScene0 (const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: TempleScene (__DRAGONWINDTHEREVENGE_TEMPLEWORLDSCENE0ID__, m, cn, p, ePL)
							{ }

		/** @see parent. */
		virtual void initialize ();
	};

	/** Temple Scene 1. */
	class TempleScene1 : public TempleScene
	{
		public:
		TempleScene1 (const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: TempleScene (__DRAGONWINDTHEREVENGE_TEMPLEWORLDSCENE1ID__, m, cn, p, ePL)
							{ }

		/** @see parent. */
		virtual void initialize ();
	};

	/** Temple Scene 2. \n
		A pair of monkeys ar looking after the bombs to be used against jack. \n
		The monkeys jump, and everytime they hit back the ground a small earthquake happens! */
	class TempleScene2 : public TempleScene
	{
		public:
		TempleScene2 (const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: TempleScene (__DRAGONWINDTHEREVENGE_TEMPLEWORLDSCENE2ID__, m, cn, p, ePL),
			  _monkey1 (NULL), _monkey2 (NULL),
			  _doorActionBlocks ()
							{ }

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		private:
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }

		private:
		DRAGONWIND::BadGuy* _monkey1;
		DRAGONWIND::BadGuy* _monkey2;

		// Implementation
		std::vector <DRAGONWIND::MoveLinearASetOfLayersActionBlock*> _doorActionBlocks;

		static const int _NUMBERDOORS = 3;
		static const int _SWITCHDOOROPEN [_NUMBERDOORS];
		static const int _SWITCHEARTHQUAKE = 3;

		static const int _COUNTEREARTHQUAKE = 0;
	};
}

#endif

// End of the file
/*@}*/
