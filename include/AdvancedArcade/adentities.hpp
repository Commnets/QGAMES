/** \ingroup AdArcadeGame */
/** \ingroup Structure */
/*@{*/

/**
*	@file
*	File: adentities.hpp \n
*	Framework: Commty Game Library (CGL) \n
*	Author: Ignacio Cea Fornies (Community Networks) \n
*	Creation Date: 12/05/2018 \n
*	Description: Defining special entities used in the game. \n
*	Versions: 1.0 Initial
*/

#ifndef __QGAMES_ADENTITIES__
#define __QGAMES_ADENTITIES__

#include <AdvancedArcade/addefinitions.hpp>
#include <Arcade/arcadeinclude.hpp>

namespace QGAMES
{
	class PauseLogoEntity : public Character
	{
		PauseLogoEntity (int cId, const Forms& f = Forms (), const Data& d = Data ())
			: Character (cId, f, d)
							{ }

		/** @see parent. */
		virtual Entity* clone ()
							{ return (new PauseLogoEntity (_id, _forms, _data)); }
	};
}

#endif

// End of the file
/*@}*/