/** \ingroup Platform */
/** \ingroup Strcuture */
/*@{*/

/**	
 *	@file	
 *	File: ptworlds.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 06/01/2019 \n
 *	Description: The typical worlds used in the platform games.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_PTWORLDS__
#define __QGAMES_PTWORLDS__

#include <Platform/ptdefinitions.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace QGAMES
{
	/** In a platform game all scene must be playing scenes. */
	class PlatformWorld : public QGAMES::World
	{
		public:
		PlatformWorld (int c, const Scenes& s, const WorldProperties& p = WorldProperties ());
	};
}

#endif

// End of the file
/*@}*/