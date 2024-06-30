/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: dwmaps.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 04/04/2019 \n
 *	Description: Defines the base for all maps in dragonwind like games.
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_MAPS__
#define __DRAGONWIND_MAPS__

#include <DragonwindLike/dwdefinitions.hpp>
#include <Platform/ptinclude.hpp>

namespace DRAGONWIND
{
	/** Class to define common funtionality to any map in a DragonWind game. */
	class Map : public QGAMES::PlatformMap
	{
		public:
		Map (int c, const QGAMES::Layers& l, int w, int h, int tW, int tH,
				const QGAMES::MapProperties& p = QGAMES::MapProperties ())
			: QGAMES::PlatformMap (c, l, w, h, tW, tH, p)
							{ }
	};
}

#endif
  
// End of the file
/*@}*/

