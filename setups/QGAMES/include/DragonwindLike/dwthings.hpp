/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: dwthing.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 28/06/2019 \n
 *	Description: Define the behaviour of the things that appear in the game
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_THINGS__
#define __DRAGONWIND_THINGS__

#include <DragonwindLike/dwdefinitions.hpp>
#include <DragonwindLike/dwartists.hpp>

namespace DRAGONWIND
{
	/** Something to be caught along the game. */
	class Thing : public DragonArtist
	{
		public:
		Thing (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::PlatformArtist::Data& d = QGAMES::PlatformArtist::Data ())
			: DragonArtist (cId, f, d)
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent. */
		virtual void setPlatformState (int id);
		virtual bool isPlatformStatePossible (int pS) const;

		/** @see parent. */
		virtual void setFromDefinition (QGAMES::PlatformGame::ElementDefinition* eD);

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint);

		// To convert from the type to the visual aspect and the other way around...
		/** A method to know which is the aspect associated to a specific type of thing. 
			The default implememtation returns the type less than 10.
			It can be overloaded later if new things are created. */
		virtual int aspectAssociatedToThingType (int t)
							{ return (t - 10); }
		/** The opposite. */
		virtual int thingTypeAssociatedToAspect (int a)
							{ return (a + 10); }

		protected:
		/** @see parent. */
		virtual void whenGoingOutOfTheMap ()
							{ removeFromTheGame (); }

		private:
		// Implementation
		/** To remove the food from the game. */
		void removeFromTheGame ();
	};
}

#endif
  
// End of the file
/*@}*/