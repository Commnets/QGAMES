/** \ingroup PACMAN */
/*@{*/

/**	
 *	@file	
 *	File: pcstandardscenes.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: To manipulate the standard scenes of the game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMAN_STANDARDSCENES__
#define __PACMAN_STANDARDSCENES__

#include <PacManLike/pcdefs.hpp>
#include <PacManLike/pcworlds.hpp>
#include <PacManLike/pcscenes.hpp>
#include <PacManLike/pcartists.hpp>
#include <PacManLike/pcmaze.hpp>

#include <advancedArcade/adinclude.hpp>

namespace PACMAN
{
	/** 
	  * The standard scene only considers the single pacman. \n
	  * The rest of elments would have to be defined as external actionblocks in the XML file or
	  * specifically inheriting from this one, that can be overloaded.
	  * That scene control when pacman is in chasing modee taking into account the situation of the monsters. \n
	  */
	class StandardScene : public Scene
	{
		public:
		StandardScene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: Scene (c, m, cn, p, ePL),
			  _pacman (nullptr),
			  _numberMonstersEaten (0)
							{ }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		private:
		PacMan* _pacman;

		// Implementation
		int _numberMonstersEaten;
	};

	/**
	  * The very basic Scene, is a standard one but defining the monsters and the fruit scene action blocks
	  * in the constrctor of the class, avoiding so their definition in the scene xml file. \n
	  * It can also be extended, to add (e.g.) specific action blocks!
	  */
	class BasicScene : public StandardScene
	{
		public:
		BasicScene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ());

		virtual void initialize () override;
	};
}

#endif
  
// End of the file
/*@}*/