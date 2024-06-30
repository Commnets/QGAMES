/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/


/**	
 *	@file	
 *	File: dwlayers.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 19/03/2020 \n
 *	Description: Some type of layers are even more special in a Dragonwind like game
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_LAYERS__
#define __DRAGONWIND_LAYERS__

#include <Platform/ptlayers.hpp>

namespace DRAGONWIND
{
	/** Represents a liquid layer. 
		In a Dragonwind like game, the liquid can be poison killing entities. */
	class LiquidTileLayer : public QGAMES::LiquidTileLayer
	{
		public:
		LiquidTileLayer (int c, const std::string& n, const QGAMES::Tiles& t, 
			QGAMES::Map* m, QGAMES::TileLayer::VisualOrientation o,
			const QGAMES::LayerProperties p = QGAMES::LayerProperties (), bool oT = false)
				: QGAMES::LiquidTileLayer (c, n, t, m, o, p, oT)
							{ }

		/** To know whether the liquid is or not poison. */
		bool isPoison () const;
	};
}

#endif
  
// End of the file
/*@}*/