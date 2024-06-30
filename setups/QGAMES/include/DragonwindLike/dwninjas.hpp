/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: dwninjas.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 28/06/2019 \n
 *	Description: Define the behaviour of the main characters (ninjas) that appear in the game
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_NINJAS__
#define __DRAGONWIND_NINJAS__

#include <DragonwindLike/dwdefinitions.hpp>
#include <DragonwindLike/dwartists.hpp>

namespace DRAGONWIND
{
	/** The main artist being controlled by the player. */
	class Ninja : public Character
	{
		public:
		/** The physical variable number 0 has to be defined in the _data internal parameter.
			This is checked at construction time (in debug mode).
			It means Max speed when the ninja falls, runs or etc...
			before a notification was sent usually to unfold the parachute. */
		Ninja (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::PlatformArtist::Data& d = QGAMES::PlatformArtist::Data ());

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent. */
		virtual void setEnergyToAdd (QGAMES::bdata e);

		/** @see parent. */
		virtual bool isHappy () const
							{ return (onOffSwitch (_SWITCHTOREACHGOALWHENPOSSIBLE) -> isOn () ||
								DragonArtist::isHappy ()); } // Happy already or estimated to be early...

		/** @see parent. */
		virtual void setPlatformState (int id);
		virtual bool isPlatformStatePossible (int pS) const;

		/** @see parent. 
			Anything different than a Ninja is considered an enemy. */
		virtual bool isEnemy (const DRAGONWIND::DragonArtist* art)
							{ return (!dynamic_cast <const Ninja*> (art)); }

		/** To manage the pocket. */
		const QGAMES::PlatformGame::Pocket& pocket () const
							{ return (_pocket); }
		void setPocket (const QGAMES::PlatformGame::Pocket& p);
		QGAMES::PlatformGame::PocketElement& firstPockeElement ()
							{ return (_pocket.firstElement ()); }
		const QGAMES::PlatformGame::PocketElement& firstPocketElement () const
							{ return (_pocket.firstElement ()); }
		QGAMES::PlatformGame::PocketElement& lastPocketElement ()
							{ return (_pocket.lastElement ()); }
		const QGAMES::PlatformGame::PocketElement& lastPocketElement () const
							{ return (_pocket.lastElement ()); }
		bool carriesTypeInPocket (int t) const;
		int numberOfTypesInPocket (int t) const;

		/** @see parent. 
			Has to keep the score in the configuration data. */
		virtual void setScore (int s); 

		/** @see parent. */
		/** When die, cries and if was climbing, falls as well. */
		virtual bool toDie ();
		virtual bool toJump (int i, bool dP);
		/** If after shooting there is no more shootings available in the gun, the gun is left. */
		virtual bool toShoot (int i, bool uP, int tS = -1);
		
		/** Only ninjas can swim if possible. 
			The parameters received are the impulse (the most fundamental one), 
			and the direction that the ninja is trying to move to 
			(only right or left in the default version is taken into account). */
		virtual bool toSwim (int i, const QGAMES::Vector& dr);

		// To manage the actions when possible.
		/** To float, only when the previous state allows it (fall). 
			Returns true when it was possible and false if it wasn't. */
		virtual bool toFloat ();
		virtual bool toFloatDeferred (); // When it is needed in the middle of processEvent...

		/** To be happy, usually when something important happens. E.g. reaching the main goal. */
		virtual bool toBeHappy ();

		/** To catch something. 
			If nothing is around, then the last thing in the pocket will be left.
			If something to catch is around then it will be first taken, 
			and the last thing in the pocket would be left, if there would be something to leave!. 
			Returns true when it was possible and false in any other case. */
		virtual bool toCatchOrLeaveSomething ();
		/** Just iterate what it is in the pocket. */
		virtual void toIteratePocket ()
							{ _pocket.toIterateElements (); setPocket (_pocket); }

		// What todo when the maingoal of the scene is reached
		/** Just to take note that the ninja is about to reach a goal. 
			The parameter is the number of the goal. */
		void toReachGoal (int nG) // Notice that it can not be overloaded...
							{ _goalReached = nG; 
							  onOffSwitch (_SWITCHTOREACHGOALWHENPOSSIBLE) -> set (true); }
		/** Execute what to do when the goal is reached. \n
			By default toBeHappy is executed if the goal reached (_goalReached) is the main one and 
			it is the first time the scene has been visited. It can be overloaded. */
		virtual void whenGoalReached (int nG);

		/** @see parent. 
			The condition affects the ninja in different ways. 
			By default, ligtning, rain an wind can affect ninja if nothing protects him. */
		virtual void whenWheatherCondition (QGAMES::EnvironmentalCondition* eC);

		/** @see parent. 
			The collision zone is a little bit less. 
			This is simply to make things a little bit easy for the ninja!. */
		virtual QGAMES::Rectangle collisionZone () const
							{ return (DRAGONWIND::DragonArtist::collisionZone ().scale (__BD 0.75)); }

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize (); 
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

		/** @see parent. */
		virtual void whenCollisionWith (QGAMES::Entity* e);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }

		/** @see parent. 
			The "happy" situation has to be taken into account. */
		virtual void afterAllLayersTestedInWhenOnTile ();

		/** @see parent. */
		virtual void whenGoingOutOfTheMap ()
							{ notify (QGAMES::Event (__DRAGONWIND_CHARACTERHASDIED__, this));
							  setVisible (false); }

		/** @see parent. */
		virtual int animationIdForState (DragonState st, int lD);

		/** @see parent. 
			it will be possible to shoot only when last element in pockect is a gun
			and the type of shooting wil depend also on it. */
		virtual bool isPossibleToShoot () const;
		virtual int typeOfShootingAvailable () const;
		virtual int numberShootingsAvailable () const;
		virtual void reduceShootingsAvailable (int nS);

		/** To be executed when an element is taken. 
			By default, when a heart is caught, the number of lives are incresed by the ones stored there in. 
			But it can be modified later to add additional behaviours. */
		virtual void whenThingIsAddedToPocked (const QGAMES::PlatformGame::PocketElement& pElmnt);

		protected:
		/** Ninja's pocket. */
		QGAMES::PlatformGame::Pocket _pocket;

		private:
		// Implementation
		/** A list of things that can be potentially caught by the main character. 
			The list is set to "empty" in every execution of updatePositions method. */
		QGAMES::Entities _thingsAround;
		/** The number of the goal reached, when it has been. */
		int _goalReached;
		/** The different intensities of the shield. */
		static const int _SHIELDINTENSITY [];


		static const int _COUNTERSHIELDTIMER = 0;
		static const int _COUNTERSHIELDASPECT = 1;
		static const int _SWITCHTOREACHGOALWHENPOSSIBLE = 1;
		static const int _SWITCHSOUNDENERGY = 2;
	};
}

#endif
  
// End of the file
/*@}*/
