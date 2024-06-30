/** \ingroup DWTheRevege */
/*@{*/

/**	
 *	@file	
 *	File: Worlds.hpp \n
 *	Game: DragonWind II (The Revenge) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 19/04/2020 \n
 *	Description: Defines the base for all worlds in DragonWind II  \n
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWINDTHEREVENGE_TEMPLEWORLDS__
#define __DRAGONWINDTHEREVENGE_TEMPLEWORLDS__

#include <DragonwindLike/dwinclude.hpp>
#include "Defs.hpp"

namespace DragonTheRevenge
{
	/** Class to define common funtionality to any Temple World in a DragonWind II game. */
	class TempleWorld : public DRAGONWIND::World
	{
		public:
		TempleWorld (const QGAMES::Scenes& scns, 
				const QGAMES::WorldProperties& prps = QGAMES::WorldProperties ())
			: DRAGONWIND::World (__DRAGONWINDTHEREVENGE_TEMPLEWORLDID__, scns, prps)
							{ }
	};
}

#endif

// End of the file
/*@}*/
