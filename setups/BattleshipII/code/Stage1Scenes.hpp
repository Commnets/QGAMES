/** \ingroup BSII */
/*@{*/

/**	
 *	@file	
 *	File: Scenes.hpp \n
 *	Game: Battleship II \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 24/03/2020 \n
 *	Description: Defines the base for all scenes in Battleship II game \n
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIPII_STAGE1SCENES__
#define __BATTLESHIPII_STAGE1SCENES__

#include "Defs.hpp"
#include <BattleshipLike/bsinclude.hpp>

namespace BattleshipII
{
	/** Base for Stage 1 Scene. */
	class Stage1Scene : public BATTLESHIP::TimeLimitScene
	{
		public:
		Stage1Scene (int c, const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: BATTLESHIP::TimeLimitScene (c, m, cn, p, ePL)
							{ }

		/** @see parent. */
		virtual void initialize () override final;
	};

	/** Stage 1 Scene 1. \n
		Normal scene with asteroids. */
	class Stage1Scene1 : public Stage1Scene
	{
		public:
		Stage1Scene1 (const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: Stage1Scene (__BATTLESHIPII_STAGE1SCENE1__, m, cn, p, ePL)
							{ }
	};

	/** Stage 1 Scene 2. \n
		Other scene with asteroids moving quicker. */
	class Stage1Scene2 : public Stage1Scene
	{
		public:
		Stage1Scene2 (const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: Stage1Scene (__BATTLESHIPII_STAGE1SCENE2__, m, cn, p, ePL)
							{ }
	};

	/** Stage 1 Scene 3. \n
		Many asteroids but with a little difference.
		In this scene, since the middle of the duration of the stage, a life can appear.
		Once it's been got, it never appears back. */
	class Stage1Scene3 : public Stage1Scene
	{
		public:
		Stage1Scene3 (const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ());

		/** @see parent. */
		virtual void updatePositions () override final;
		virtual void finalize () override final;

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt) override final;

		private:
		static const int _SWITCHLIFEONTHESCREEN = 0;
		static const int _SWITCHLIFEALREADYGOT = 1;

		/** The number of the inernal action block for the life object to move... */
		static const int _LIFEBLOCKNUMBER = 100000;
	};
}

#endif

// End of the file
/*@}*/
