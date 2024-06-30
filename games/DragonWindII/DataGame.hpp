/** \ingroup DWTheRevege */
/*@{*/

/**	
 *	@file	
 *	File: DataGame.hpp \n
 *	Framework: Dragonwind II The Revenge \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 22/04/2020 \n
 *	Description: Some modifications to the information define in the library \n
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWINDTHEREVENGE_DATAGAME__
#define __DRAGONWINDTHEREVENGE_DATAGAME__

#include <DragonwindLike/dwinclude.hpp>

namespace DragonTheRevenge
{
	struct DataGame : DRAGONWIND::DataGame
	{
		public:
		DataGame ()
			: DRAGONWIND::DataGame ()
					{ _thingsToCarryFormId = 80005; }
	};
}

#endif
  
// End of the file
/*@}*/
