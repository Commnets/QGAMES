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

#ifndef __BATTLESHIPII_STAGE5SCENES__
#define __BATTLESHIPII_STAGE5SCENES__

#include "Defs.hpp"
#include <BattleshipLike/bsinclude.hpp>

namespace BattleshipII
{
	/** Base for Stage 5 Scenes */
	class Stage5Scene : public BATTLESHIP::TimeLimitScene
	{
		public:
		Stage5Scene (int c, const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: BATTLESHIP::TimeLimitScene (c, m, cn, p, ePL)
							{ }

		/** @see parent. */
		virtual void initialize () override;
	};

	/** Stage 5 Scene 1 */
	class Stage5Scene1 : public Stage5Scene
	{
		public:
		Stage5Scene1 (const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: Stage5Scene (__BATTLESHIPII_STAGE5SCENE1__, m, cn, p, ePL)
							{ _externalEventSceneCompletedFunction = _NOSTDUFOSINSCENEEXTERNALEVNT; }
	};

	/** Stage 5 Scene 2 */
	class Stage5Scene2 : public Stage5Scene
	{
		public:
		Stage5Scene2 (const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: Stage5Scene (__BATTLESHIPII_STAGE5SCENE2__, m, cn, p, ePL)
							{ _externalEventSceneCompletedFunction = _NOSTDUFOSINSCENEEXTERNALEVNT; }
	};

	/** Stage 5 Scene 3
		This belongs to a different type. */
	class Stage5Scene3 : public BATTLESHIP::FocusingInOneElementScene
	{
		public:
		Stage5Scene3 (const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: BATTLESHIP::FocusingInOneElementScene (__BATTLESHIPII_STAGE5SCENE3__, m, 
					std::string ("The Real McCoy"), cn, p, ePL), // When the element is killed...
			  _blockWithElementFocused (nullptr),
			  _fliesWithShield (), _numberBlocksShieldAdded (0),
			  _flyWithLife (nullptr),
			  _blocksWithUFOs (), _blockWithMothership (nullptr)
							{ }

		/** @see parent. */
		virtual void initialize () override final;
		virtual void finalize () override final;

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt) override;

		private:
		/** Thee block with the element focused. */
		BATTLESHIP::StdUFOSceneActionBlock* _blockWithElementFocused;
		/** The flies selected to provide a shield */
		using UFOSWithShield = std::vector <BATTLESHIP::UFO*>;
		UFOSWithShield _fliesWithShield;
		int _numberBlocksShieldAdded;
		/** The flies selected to provide a live */
		BATTLESHIP::UFO* _flyWithLife;

		// Implementation
		std::vector <BATTLESHIP::StdUFOSceneActionBlock*> _blocksWithUFOs;
		BATTLESHIP::StdUFOSceneActionBlock* _blockWithMothership;

		/** The number of the base internal action block for the shild object to move. */
		static const int _SHIEDBASEBLOCKNUMBER = 100000;
		/** The number of the internal action block for the life object to move. */
		static const int _LIFEBLOCKNUMBER = 100100;
	};
}

#endif

// End of the file
/*@}*/
