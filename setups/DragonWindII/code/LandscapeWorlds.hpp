/** \ingroup DWTheRevege */
/*@{*/

/**	
 *	@file	
 *	File: Worlds.hpp \n
 *	Game: DragonWind II (The Revenge) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 24/03/2020 \n
 *	Description: Defines the base for all worlds in DragonWind II  \n
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWINDTHEREVENGE_LANDSCAPEWORLDS__
#define __DRAGONWINDTHEREVENGE_LANDSCAPEWORLDS__

#include <DragonwindLike/dwinclude.hpp>
#include "Defs.hpp"

namespace DragonTheRevenge
{
	/** Class to define common funtionality to any Landscape World in a DragonWind II game. */
	class LandscapeWorld : public DRAGONWIND::World
	{
		public:
		LandscapeWorld (const QGAMES::Scenes& scns, 
				const QGAMES::WorldProperties& prps = QGAMES::WorldProperties ())
			: DRAGONWIND::World (__DRAGONWINDTHEREVENGE_LANDSCAPEWORLDID__, scns, prps)
							{ }
	};
}

#endif

// End of the file
/*@}*/
