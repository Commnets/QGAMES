/** \ingroup BSII */
/*@{*/

/**	
 *	@file	
 *	File: Scenes.hpp \n
 *	Game: Battleship II \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 28/11/2020 \n
 *	Description: Defines the base for all scenes in Battleship II game \n
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIPII_STAGE4SCENES__
#define __BATTLESHIPII_STAGE4SCENES__

#include "Defs.hpp"
#include <BattleshipLike/bsinclude.hpp>

namespace BattleshipII
{
	/** Base for Stage 4 Scenes */
	class Stage4Scene : public BATTLESHIP::TimeLimitScene
	{
		public:
		Stage4Scene (int c, const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: BATTLESHIP::TimeLimitScene (c, m, cn, p, ePL)
							{ }

		/** @see parent. */
		virtual void initialize () override final;
	};

	/** Stage 4 Scene 1 */
	class Stage4Scene1 : public Stage4Scene
	{
		public:
		Stage4Scene1 (const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: Stage4Scene (__BATTLESHIPII_STAGE4SCENE1__, m, cn, p, ePL)
							{ }
	};

	/** Stage 4 Scene 2 */
	class Stage4Scene2 : public Stage4Scene
	{
		public:
		Stage4Scene2 (const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: Stage4Scene (__BATTLESHIPII_STAGE4SCENE2__, m, cn, p, ePL)
							{ _externalEventSceneCompletedFunction = _NOSTDUFOSINSCENEEXTERNALEVNT; }
	};

	/** Stage 4 Scene 3 */
	class Stage4Scene3 : public BATTLESHIP::FocusingInOneElementScene
	{
		public:
		Stage4Scene3 (const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: BATTLESHIP::FocusingInOneElementScene (__BATTLESHIPII_STAGE4SCENE3__, m, 
					std::string ("Big Mothership UFO killed"), cn, p, ePL)
							{ }

		/** @see parent. */
		virtual void initialize () override final;

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt) override;
	};
}

#endif

// End of the file
/*@}*/
