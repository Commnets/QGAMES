/** \ingroup PACMAN */
/*@{*/

/**	
 *	@file	
 *	File: pcfruit.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: Such important element in any pacman game!. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMAN_FRUIT__
#define __PACMAN_FRUIT__

#include <PacManLike/pcdefs.hpp>
#include <PacManLike/pcartists.hpp>

#include <advancedArcade/adinclude.hpp>

namespace PACMAN
{
	/** The fruits in the maze. \n
		@see Thing documentation for more details. */
	class Fruit final : public Thing
	{
		public:
		Fruit (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Thing (cId, f, d)
							{ }

		virtual QGAMES::Entity* clone () const override
							{ return (new Fruit (id (), forms (), data ())); }

		protected:
		virtual void setStateToStandLookingTo (const QGAMES::Vector&) override;
		virtual void setStateToMoveTo (const QGAMES::Vector&) override;
	};
}

#endif
  
// End of the file
/*@}*/