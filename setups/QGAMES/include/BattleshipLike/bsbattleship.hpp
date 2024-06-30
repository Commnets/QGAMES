/** \ingroup Battleship */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: bsbattleship.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 02/11/2019 \n
 *	Description: Define the base of the battleship flying along the game.
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_BATTLESHIP__
#define __BATTLESHIP_BATTLESHIP__

#include <BattleshipLike/bsartists.hpp>

namespace BATTLESHIP
{
	/** The space battleship managed by the player. */
	class Battleship : public Spaceship
	{
		public:
		/** A new state is introduced for the battleship. */
		static const int _ROTATING = 5;

		/** The default behaviour is:
			Affected by enemies,
			Affected by similars,
			energy lost per second: 1%
			energy detracted from other when hit: 50% (meaning that the other will be destroyed with two contacts),
			0 points given to the enemies when it is destroyed
			shooting energy (reduced from the one hit) 50%. It means 2 shootings to destroy somethig at 100% of energy
			basic type of shooting (0). \n
			Two new counters are defined also inside:
			One to count every how much to change the bright of the halo, 
			and other to indicate the bright of the halo. */
		Battleship (int cId, 
				Behaviour* bhv = new Behaviour (true, true, 
					__BD (__BD 1 / __BD QGAMES::Game::game () -> framesPerSecond ()), __BD 50, 0, __BD 50, 0, __BD 1.0), 
				const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent. */
		virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a);

		/** @see parent. 
			The new state is now also possible. */
		virtual bool isStatePossible (int st)
							{ return (st == _HIDDEN || 
									  st == _MOVING ||
									  st == _EXPLODING ||
									  st == _DESTROYED || 
									  st == _ROTATING); } // Except frozen, the rest are possible...

		/** @see parent. 
			The enemies are ufos and asteroids in the case of a battleship. */
		virtual inline bool isEnemy (const SpaceElement* ss) const;

		/** @see parent.
			Just "sounds" when it was possible to shoot finally. */
		virtual bool toShoot (int f = 1, const QGAMES::Vector& d = QGAMES::Vector (__BD 0, __BD -1, __BD 0), int nP = 0);

		/** toRotate: It should be used in special circunstances: e.g. to celebrate reaching a goal... */
		virtual void toRotate ()
							{ if (isMoving () && !isRotating ()) setStateFor (_spaceElementState = _ROTATING, orientation ()); }
		bool isRotating () const
						{ return (_spaceElementState == _ROTATING); }
		// If rotating, the battleship is not alive either,
		// so it is not needed to modify any parent's method regarding this...

		/** To manage the pocket. */
		const Game::TransportedElements& transportedElements () const
							{ return (_transportedElements); }
		void setTransportedElements (const Game::TransportedElements& p);

		// To do the special things of the battleship
		/** To catch & leave things. */
		/** To catch something. 
			If nothing is around then the last thing in the pocket will be left.
			If something to catch is around then it will be first taken, 
			and the last thing in the pocket left, if there would be something to leave!. 
			Returns true when it was possible and false in any other case. */
		virtual bool toCatchOrLeaveThing ();
		/** Just iterate what it is in the pocket.
			In the default implementation, the pocket has only one element, so the iteration has no effect. */
		virtual void toIteratePocket ()
							{ _transportedElements.toIterateElements (); 
							  setTransportedElements (_transportedElements); }

		/** @see parent. */
		virtual int maxNumberOfShootingsInSpaceSimultaneouly () const
							{ return (__BATTLESHIPDATAGAME__ -> _LEVELDATA [_powerLevel][1]); }
		virtual int maxNumberOfShootingsAvailable () const
							{ return (__BATTLESHIPDATAGAME__ -> _LEVELDATA [_powerLevel][2]); }
		virtual int shootingsRemaining () const
							{ return (_shootingsRemaining); }
		virtual void reduceShootingsRemaining (int a);

		/** @see parent. 
			Just the core, except when sealed. */
		virtual QGAMES::Rectangle collisionZone () const;

		/** @see parent. 
			When sealed, something is drawn around. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();
		virtual void drawOn (QGAMES::Screen* scr, const QGAMES::Position& = QGAMES::Position::_noPoint);

		/** @see parent. 
			In this implementation, take int account that for pills they are taken automatically. 
			For another type of things to catch (in general) they are kept in the list of potential 
			things to be caught. */
		virtual void whenCollisionWith (QGAMES::Entity* e);

		/** @see parent. 
			The battleship is like any other spacechip except it doesn't dissapear when exit the visible zone
			as it is defined in the parent class, so that event is here reinterpreted. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		/** @see parent. */
		virtual void setStateFor (int st, const QGAMES::Vector& o = QGAMES::Vector ());

		protected:
		/** A reference to the battleship's pocket. */
		Game::TransportedElements _transportedElements;

		/** The counters needed here. 
			The counter number 0 has been defined at parent level. */
		static const int _COUNTERTOBRIGHT = 1;
		static const int _COUNTERBRIGHT = 2;
		static const int _FADES [];

		// Implementation
		int _shootingsRemaining;
		/** A list of things that can be potentially caught by the main character. 
			The list is set to "empty" in every execution of updatePositions method. */
		QGAMES::Entities _thingsAround;
	};
}

#endif
  
// End of the file
/*@}*/