/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Scenes.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 29/01/2021 \n
 *	Description: Define the scenes of this version of the game PacManII
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_SCENES__
#define __PACMANII_SCENES__

#include <PacManLike/pcinclude.hpp>

namespace PacManII
{
	/** The extended scene, adds a specific block for monsters to appear. */
	class BasicSceneWithAppearingMonsters final : public PACMAN::BasicScene
	{
		public:
		BasicSceneWithAppearingMonsters 
			(int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			 const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			 const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: PACMAN::BasicScene (c, m, cn, p, ePL),
			  _additionalMonstersActionBlocks ()
							{ }

		/** To set up and remove the blocks controlling additional monsters. */
		virtual void initialize () override;
		virtual void finalize () override;

		private:
		// Implementation
		std::vector <QGAMES::SceneActionBlock*> _additionalMonstersActionBlocks;
	};
}

#endif
  
// End of the file
/*@}*/