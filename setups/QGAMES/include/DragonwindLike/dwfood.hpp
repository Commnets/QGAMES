/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: dwfood.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 28/06/2019 \n
 *	Description: Define the behaviour of the food that appear in the game
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_FOOD__
#define __DRAGONWIND_FOOD__

#include <DragonwindLike/dwdefinitions.hpp>
#include <DragonwindLike/dwartists.hpp>

namespace DRAGONWIND
{
	/** Something to eat in this game. */
	class Food : public DragonArtist
	{
		public:
		Food (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::PlatformArtist::Data& d = QGAMES::PlatformArtist::Data ())
			: DragonArtist (cId, f, d),
			  _energyLevel (0), // The minimum possible...
			  _inTheAir (false) // Falls
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent. */
		virtual void setPlatformState (int id);
		virtual bool isPlatformStatePossible (int pS) const;

		/** To know the level of energy that the food has
			It can be 0, 1, 2,... depending of the tile used in the location layer to define it. */
		int energyLevel () const
							{ return (_energyLevel); }
		/** To know whether the food floats in the air or can fall. */
		bool inTheAir () const
							{ return (_inTheAir); }

		/** @see parent. */
		virtual void setFromDefinition (QGAMES::PlatformGame::ElementDefinition* eD);

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint);

		/** @see parent. */
		virtual void whenCollisionWith (QGAMES::Entity* e);

		protected:
		/** @see parent. */
		virtual void whenGoingOutOfTheMap ()
							{ removeFromTheGame (); }

		// Methods invoked from setDescriptionFrom that can be adjusted later...
		/** To know the energy level associated to a specific food type.
			The default implementation: 0, 1 level 1; 2, 3 level 2,...and so on so forth. */
		virtual int energyAssociatedToFoodType (int t);
		/** To know whether a food type float in the air or falls. 
			The default implementation: Even types fall, odd types float in the air. */
		virtual int foodTypeToBeInTheAir (int t)
							{ return ((t % 2) == 1); }
		/** To know whether the food contains really high energy. */
		virtual bool isHighEnergy () const;

		private:
		// Implementation
		/** To remove the food from the game. */
		void removeFromTheGame ();

		protected:
		// Implementation
		int _energyLevel;
		bool _inTheAir;
	};
}

#endif
  
// End of the file
/*@}*/