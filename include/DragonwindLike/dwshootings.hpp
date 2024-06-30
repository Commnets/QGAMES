/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: dwshootings.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 28/06/2019 \n
 *	Description: Define the behaviour of the shootings that appear in the game
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_SHOOTINGS__
#define __DRAGONWIND_SHOOTINGS__

#include <DragonwindLike/dwdefinitions.hpp>
#include <DragonwindLike/dwartists.hpp>

namespace DRAGONWIND
{
	/** To refer to a shooting. 
		There can be many different types of shootings. 
		Each of them can hurt the characters (defined later) in different ways. */
	class Shooting : public DragonArtist
	{
		public:
		Shooting (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
			const QGAMES::PlatformArtist::Data& d = QGAMES::PlatformArtist::Data ());

		~Shooting () // In this case, the shooting owns the definition (is is creted per shooting)
							{ delete (_definition); }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent. */
		virtual void setPlatformState (int id);
		virtual bool isPlatformStatePossible (int pS) const;

		/** To manage the owner. */
		const DRAGONWIND::Character* owner () const
							{ return (_owner); }
		DRAGONWIND::Character* owner ()
							{ return (_owner); }

		/** @see parent.
			In this case THROW means flying. */
		virtual bool isOnAPlatformState () const
							{ return (isStanding () || isRunning () || isSlidding () || isRolling () || 
									  isDieing () || isDied ()); }

		/** To launch means to start the movement. 
			It is necessary to pass the basic data of the movement: orientation and force! 
			Apart of a reference to who is launching the shooting.
			The initial position will part of the definition. */
		virtual void toLaunchFrom (DRAGONWIND::Character* chr, int o, int f = 1);

		// Managing the movements of the entity...
		/** To be thrown. 
			The parameter indicates whether it bounces or not. By default it doiesn't. */
		virtual void toBeThrown (bool b = false);
		/** Starts to roll over a platform, only when it is on a platform. */
		virtual void toRoll ();
		/** Explode, wherever the shooting is. */
		virtual void toExplode ();

		/** @see parent. 
			The collision zone is really small. */
		virtual QGAMES::Rectangle collisionZone () const
							{ return (DRAGONWIND::DragonArtist::collisionZone ().scale (__BD 0.25)); }

		/** @see parent. */
		virtual void updatePositions ();

		/** @see parent. */
		virtual void whenCollisionWith (QGAMES::Entity* e);

		/** @see parent. 
			The element definition in this case is a shooting definition.
			The definition is own by the shooting. */ 
		virtual void setFromDefinition (QGAMES::PlatformGame::ElementDefinition* eD);

		/** @see parent. 
			To control the movement of the shooting. */
		virtual void processEvent (const QGAMES::Event& evnt);

		/** @see parent. */
		virtual void afterAllLayersTestedInWhenOnTile ();

		protected:
		/** Defines how the shooting behaves. 
			It is defined when the definition is set. A default behaviour is also set at construction time. */
		class Behaviour
		{
			public:
			Behaviour (Shooting* sth)
				: _shooting (sth)
							{ assert (_shooting); } // It can't be NULL...
			virtual ~Behaviour ()
							{ }

			/** To execute a task. 
				The method can be extended, but a couple of default behaviours are defined (@see next). */
			virtual void executeTask (int tsk, 
				const QGAMES::SetOfOpenValues& oV = QGAMES::SetOfOpenValues ());

			/** What to do when started to fly. 
				Determinates the initial state. Nothing is declared by default. */
			virtual void whenStartedToFly (QGAMES::bdata f) = 0;
			/** What to do when the shooting is first time on a platform. 
				By default it is the same than when stop. */
			virtual void whenOnPlatform ()
							{ whenStopped (); }
			/** What to do when the shooting hits a base. 
				By default it is the same that when stopped. */
			virtual void whenHitBase ()
							{ whenStopped (); }
			/** What to do when the shooting stops. 
				By default it is the same behaviour than finishing the last state. */
			virtual void whenStopped ()
							{ whenFinishingLastState (); }
			/** When stopped the shooting could e.g. explode.
				That last state should be the one not looping.
				After finishing that last state, what to do?. 
				By default send the event SHOOTINGTOEND. */
			virtual void whenFinishingLastState ();

			protected:
			Shooting* _shooting;
		};

		// To simplify the standard reatments later...
		// They can be extended also in other uses of the library
		#define __DRAGONWIND_STHBHVTSKSTARTTOFLY__						0
		#define __DRAGONWIND_STHBHVTSKSTARTTOFLYPOWERPRM__				0
		#define __DRAGONWIND_STHBHVTSKONPLATFORM__						1
		#define __DRAGONWIND_STHBHVTSKHITBASE__							2
		#define __DRAGONWIND_STHBHVTSKSTOPPED__							3
		#define __DRAGONWIND_STHBHVTSKLASTSTATE__						4

		/** To change the the behaviour. */
		virtual void executeActionOverBehaviour (int tsk, const 
			QGAMES::SetOfOpenValues& oV = QGAMES::SetOfOpenValues ());

		/** A buoy to execute a task over the shooting behaviour in a deferred way. */
		class ToExecuteTaskOverBehaviourBuoy : public QGAMES::Buoy
		{
			public:
			ToExecuteTaskOverBehaviourBuoy ()
				: QGAMES::Buoy (__DRAGONWIND_STHTASKEXECDEFEREDBUOYID__, (QGAMES::bdata) 0), 
				  _task (-1),
				  _taskData (QGAMES::SetOfOpenValues ())
								{ /** Nothing else to do. */ }

			void setTask (int tsk)
								{ _task = tsk; }
			void setTaskData (const QGAMES::SetOfOpenValues& sV)
								{ _taskData = sV; }

			virtual void* treatFor (QGAMES::Element* e)
								{ ((Shooting*) e) -> executeActionOverBehaviour (_task, _taskData); 
								  return (this); }

			private:
			int _task;
			QGAMES::SetOfOpenValues _taskData;
		};

		/** To execute an action over the behaviour in a deferred way. */
		virtual void executeActionOverBehaviourDeferred (int tsk, const 
			QGAMES::SetOfOpenValues& oV = QGAMES::SetOfOpenValues ());

		/** Just fly and finishes when hits any thing. 
			FLying in the shooting element is represented with the state JUMP. */
		class JustFly : public Behaviour
		{
			public:
			JustFly (Shooting* sth)
				: Behaviour (sth)
							{ }

			/** @see parent.
				The normal parabolic movement, expresed with the state INTHEAIR. 
				Notice that the method receives the intensity as a parameter. */
			virtual void whenStartedToFly (QGAMES::bdata f);
		};

		/** Just fly */
		class FlyAndBounce : public Behaviour
		{
			public:
			FlyAndBounce (Shooting* sth)
				: Behaviour (sth)
							{ }
				
			/** @see parent. */
			/** This state is expressed with the state THROW. */
			virtual void whenStartedToFly (QGAMES::bdata f);
			/** When it is on a base, nothing is done because it is supossed it bounces. */
			virtual void whenOnPlatform ()
							{ }
		};

		/** Just fly and explode when collision with something. */
		class FlyAndExplode : public JustFly
		{
			public:
			FlyAndExplode (Shooting* sth)
				: JustFly (sth)
							{ }

			/** @see parent. */
			virtual void whenStopped ()
							{ _shooting -> toExplode (); }
		};

		/** Fly, bounce and explode when stopped. */
		class FlyAndBounceAndExplode : public FlyAndBounce
		{
			public:
			FlyAndBounceAndExplode (Shooting* sth)
				: FlyAndBounce (sth)
							{ }

			/** @see parent. */
			virtual void whenStopped ()
							{ _shooting -> toExplode (); }
		};

		/** Fly and roll */
		class FlyAndRoll : public JustFly
		{
			public:
			FlyAndRoll (Shooting* sth)
				: JustFly (sth)
							{ }

			/** @see parent.
				Expressed with the state ROLL. */
			virtual void whenOnPlatform ()
							{ _shooting -> toRoll (); }
		};

		/** Fly, rool and explode when finishing. */
		class FlyAndRollAndExplode : public FlyAndRoll
		{
			public:
			FlyAndRollAndExplode (Shooting* sth)
				: FlyAndRoll (sth)
							{ }

			/** @see parent. */
			virtual void whenStopped ()
							{ _shooting -> toExplode (); }
		};

		/** Structure to define together all states related with a type of shooting. */
		struct StatesId
		{
			StatesId ()
				: _standingRight (-1), _standingLeft (-1),
				  _flyingRight (-1), _flyingLeft (-1),
				  _bouncingRight (-1), _bouncingLeft (-1),
				  _rollingRight (-1), _rollingLeft (-1),
				  _fallingRight (-1), _fallingLeft (-1),
				  _exploding (-1),
				  _weightCoef (__BD 1),
				  _hurm (true)
							{ }

			StatesId (int sR, int sL, int fR, int fL, int bR, int bL, 
					  int rR, int rL, int faR, int faL, int e, QGAMES::bdata wC = __BD 1, bool h = true)
				: _standingRight (sR), _standingLeft (sL),
				  _flyingRight (fR), _flyingLeft (fL),
				  _bouncingRight (bR), _bouncingLeft (bL),
				  _rollingRight (rR), _rollingLeft (rL),
				  _fallingRight (faR), _fallingLeft (faL),
				  _exploding (e),
				  _weightCoef (wC),
				  _hurm (h)
							{ }

			int _standingRight, _standingLeft;
			int _flyingRight, _flyingLeft;
			int _bouncingRight, _bouncingLeft;
			int _rollingRight, _rollingLeft;
			int _fallingRight, _fallingLeft;
			int _exploding;
			QGAMES::bdata _weightCoef;
			bool _hurm;
		};

		/** To calculate the factor to multiply the speed according with the impulse.
			It can be overloaded later. */
		virtual QGAMES::bdata factorForImpulse (QGAMES::bdata i);

		/** To get the stateid object for a specific type of shooting guy. */
		virtual StatesId statesIdForType (int t);
		/** Same for the behaviour. */
		virtual Behaviour* behaviourForType (int t);

		// Just a couple of defines to simplify the definition of the shootings...
		// For a better understanding, see dwentities.xml file (at shootings states definition)
		#define __DRAGONWIND_ARROWSHOOTINGSTATESID__ \
			DRAGONWIND::Shooting::StatesId (0, 1, 2, 3, 4, 5, -1, -1, -1, -1, 50, __BD 1.3, true)
		#define __DRAGONWIND_STARSHOOTINGSTATESID__ \
			DRAGONWIND::Shooting::StatesId (10, 11, 12, 13, 14, 15, -1, -1, -1, -1, 50, __BD 1.3, true)
		#define __DRAGONWIND_BOMBSHOOTINGSTATESID__ \
			DRAGONWIND::Shooting::StatesId (20, 21, 22, 23, 24, 25, -1, -1, -1, -1, 50, __BD 1.5, false) // Heavy...
		#define __DRAGONWIND_POLLBALLSHOOTINGSTATESID__ \
			DRAGONWIND::Shooting::StatesId (30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 50, __BD 1.8, true) // Really heavy...

		protected:
		// Implementation
		int _impulse;
		Behaviour* _behaviour;
		StatesId _statesId;
		DRAGONWIND::Character* _owner;
	};
}

#endif
  
// End of the file
/*@}*/
