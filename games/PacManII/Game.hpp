/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: Game.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 28/01/2022 \n
 *	Description: To manipulate the very basics of the PacManII game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_GAME__
#define __PACMANII_GAME__

#include <PacManLike/pcinclude.hpp>

#include "Builders.hpp"

namespace PacManII
{
	class Game : public PACMAN::Game
	{
		public:
		Game (const PACMAN::DataGames& dt)
			: PACMAN::Game (dt)
							{ }

		protected:
		virtual QGAMES::EntityBuilder* createEntityBuilder () override
							{ return (new EntityBuilder (parameter (__GAME_PROPERTYENTITIESFILE__), 
									formBuilder (), movementBuilder ())); }
		virtual QGAMES::MovementBuilder* createMovementBuilder () override
							{ return (new MovementBuilder (parameter (__GAME_PROPERTYMOVEMENTSFILE__))); }
		virtual QGAMES::WorldBuilder* createWorldBuilder () override
							{ return (new WorldBuilder (parameter (__GAME_PROPERTYWORLDSFILE__), 
								mapBuilder ())); }
	};
}

#endif
  
// End of the file
/*@}*/