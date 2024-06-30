/** \ingroup DoomCastle */
/*@{*/

/**	
 *	@file	
 *	File: Game.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 02/04/2021 \n
 *	Description: Central class for DoomCastle. \n
 *	Versions: 1.0 Initial
 */

#ifndef __DOOMCASTLE_MAZE__
#define __DOOMCASTLE_MAZE__

#include <Platform/ptinclude.hpp>

namespace DOOMCASTLE
{
	class MazeModel final : public QGAMES::MazeModel
	{
		public:
		MazeModel ();

		MazeModel (const MazeModel& mM) = default;

		MazeModel& operator = (const MazeModel& mM) = default;
	};
}

#endif
  
// End of the file
/*@}*/
