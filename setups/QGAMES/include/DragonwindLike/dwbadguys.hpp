/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: dwbadguys.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 28/06/2019 \n
 *	Description: Define the behaviour of the bad guys that appear in the game
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_BADGUYS__
#define __DRAGONWIND_BADGUYS__

#include <DragonwindLike/dwdefinitions.hpp>
#include <DragonwindLike/dwartists.hpp>

namespace DRAGONWIND
{
	/** The bad guys of the game. */
	class BadGuy : public Character
	{
		public:
		/** Structure to define together all states related with a type of bad guy. */
		struct StatesId
		{
			StatesId ()
				: _standingRight (-1), _standingLeft (-1),
				  _runningRight (-1), _runningLeft (-1),
				  _jumpingRight (-1), _jumpingLeft (-1),
				  _fallingRight (-1), _fallingLeft (-1),
				  _slidingRight (-1), _slidingLeft (-1),
				  _dieingRight (-1), _dieingLeft (-1),
				  _diedRight (-1), _diedLeft (-1)
							{ }

			StatesId (int sR, int sL, int rR, int rL, int jR, int jL, 
					  int fR, int fL, int sdR, int sdL, int dR, int dL, int aDR, int aDL)
				: _standingRight (sR), _standingLeft (sL),
				  _runningRight (rR), _runningLeft (rL),
				  _jumpingRight (jR), _jumpingLeft (jL),
				  _fallingRight (fR), _fallingLeft (fL),
				  _slidingRight (sdR), _slidingLeft (sdL),
				  _dieingRight (dR), _dieingLeft (dL),
				  _diedRight (aDR), _diedLeft (aDL)
							{ }

			int _standingRight, _standingLeft;
			int _runningRight, _runningLeft;
			int _jumpingRight, _jumpingLeft;
			int _fallingRight, _fallingLeft;
			int _slidingRight, _slidingLeft;
			int _dieingRight, _dieingLeft;
			int _diedRight, _diedLeft;
		};

		// Just a couple of defines to simplify the definition of the very basic bad guys...
		// For a better understanding, see dwentities.xml file (at bad guys states definition)
		#define __DRAGONWIND_CATSTATESID__ \
			DRAGONWIND::BadGuy::StatesId (0, 10, 1, 11, 2, 12, 3, 13, 4, 14, 5, 15, 6, 16)
		#define __DRAGONWIND_DOGSTATESID__ \
			DRAGONWIND::BadGuy::StatesId (20, 30, 21, 31, 22, 32, 23, 33, 24, 34, 25, 35, 26, 36)
		#define __DRAGONWIND_ROBOTSTATESID__ \
			DRAGONWIND::BadGuy::StatesId (40, 50, 41, 51, 42, 52, 43, 53, 44, 54, 45, 55, 46, 56)
		#define __DRAGONWIND_JACKSTATESID__ \
			DRAGONWIND::BadGuy::StatesId (60, 70, 61, 71, 62, 72, 63, 73, 64, 74, 65, 75, 66, 76)

		BadGuy (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::PlatformArtist::Data& d = QGAMES::PlatformArtist::Data ())
			: Character (cId, new Behaviour (), f, d),
			  _statesId ()
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent. */
		virtual void setPlatformState (int id);
		virtual bool isPlatformStatePossible (int pS) const;

		/** @see parent. 
			Anything different than a BadGuy is an enemy. */
		virtual bool isEnemy (const DRAGONWIND::DragonArtist* art)
							{ return (!dynamic_cast <const BadGuy*> (art)); }

		/** @see parent. 
			The element definition in this case is a shooting definition. */ 
		virtual void setFromDefinition (QGAMES::PlatformGame::ElementDefinition* eD);

		/** @see parent. */
		virtual bool toDie (); // The monitors (if any) has to stop...
		virtual bool toShoot (int i, bool uP, int tS = -1);

		/** @see parent. 
			The collision zone is a little bit less. 
			This is simply to make things a little bit easy for the ninja!. */
		virtual QGAMES::Rectangle collisionZone () const
							{ return (DRAGONWIND::DragonArtist::collisionZone ().scale (__BD 0.75)); }

		/** @see parent. */
		virtual void finalize (); 
	
		/** @see parent. */
		virtual void whenCollisionWith (QGAMES::Entity* e);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		/** @see parent. */
		virtual void adjustRelevants (QGAMES::bdata& m, QGAMES::Vector& pv);

		protected:
		/** To get the stateid object for a specific type of bad guy. */
		virtual StatesId statesIdForType (int t);

		/** @see parent. */
		virtual int animationIdForState (DragonState st, int lD);

		protected:
		// Implementation
		StatesId _statesId;
	};

	// Defines to declare types of bad guys...
	#define __DRAGONWIND_CATTYPEID__			0
	#define __DRAGONWIND_DOGTYPEID__			1
	#define __DRAGONWIND_ROBOTTYPEID__			2
	#define __DRAGONWIND_JACKTYPEID__			3
}

#endif
  
// End of the file
/*@}*/