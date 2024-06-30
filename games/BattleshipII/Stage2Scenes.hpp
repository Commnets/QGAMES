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

#ifndef __BATTLESHIPII_STAGE2SCENES__
#define __BATTLESHIPII_STAGE2SCENES__

#include "Defs.hpp"
#include <BattleshipLike/bsinclude.hpp>

namespace BattleshipII
{
	/** Base for Stage 2 Scenes */
	class Stage2Scene : public BATTLESHIP::SequentialActionBlocksScene
	{
		public:
		Stage2Scene (int c, const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: BATTLESHIP::SequentialActionBlocksScene (c, m, cn, p, ePL)
							{ }

		/** @see parent. */
		virtual void initialize () override;
	};

	/** Stage 2 Scene 1. \n
		UFOS moving in a block and then attacking like in GALAGA. */
	class Stage2Scene1 : public Stage2Scene
	{
		public:
		Stage2Scene1 (const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: Stage2Scene (__BATTLESHIPII_STAGE2SCENE1__, m, cn, p, ePL)
							{ }
	};

	/** Stage 2 Scene 2. \n
		Same than in the previous but with more intensity. */
	class Stage2Scene2 : public Stage2Scene
	{
		public:
		Stage2Scene2 (const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: Stage2Scene (__BATTLESHIPII_STAGE2SCENE2__, m, cn, p, ePL)
							{ }
	};

	/** Stage 2 Scene 3. \n
		Even more, but when the critical fly is killed,	a life is provided, 
		but just one that does not regenerate. Take it quick!.
		It appears always once in this stage, even if it has been got and you died!. */
	class Stage2Scene3 : public Stage2Scene
	{
		public:
		Stage2Scene3 (const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: Stage2Scene (__BATTLESHIPII_STAGE2SCENE3__, m, cn, p, ePL),
			  _flyWithLife (NULL)
							{ }

		/** @see parent. */
		virtual void initialize () override final;
		virtual void finalize () override final;

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt) override final;

		private:
		/** The fly selected to provide the life. */
		BATTLESHIP::UFO* _flyWithLife;

		/** The number of the inernal action block for the life object to move... */
		static const int _LIFEBLOCKNUMBER = 100000;
	};
}

#endif

// End of the file
/*@}*/
