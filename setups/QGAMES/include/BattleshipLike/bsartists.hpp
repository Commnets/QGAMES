/** \ingroup Battleship */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: bsartist.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 02/11/2019 \n
 *	Description: Define the base of the most common type of artists used in a Battleship like game. \n
 *				 The default implementation has been done thinking in a 2D Arcade Game.
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_ARTIST__
#define __BATTLESHIP_ARTIST__

#include <BattleshipLike/bsdefinitions.hpp>
#include <BattleshipLike/bsgame.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace BATTLESHIP
{
	/** All elements in the game must inherit from this one. \n
		Any space element can move accross the space. \n
		It has energy. An energy that loses or gain as it interats with others. \n
		Any space element has also a behaviour. 
		In the behaviour how that iteration with the rest behaves is defined. */
	class SpaceElement : public QGAMES::Artist
	{
		public:
		/** A set of parameters to define the behaviour of the spaceship. 
			It can be extended later if needed. */
		struct Behaviour
		{
			Behaviour ()
				: _canBeHurmedByEnemies (true), // Can be damaged by enemies? (if it can, it can also die)
				  _canBeHurmedBySimilars (false), // Can be damaged by similars? (if it can, it can also die)
				  _energyLostPerLoop (__BD 1 / __BD QGAMES::Game::game () -> framesPerSecond ()), // 1% per second (100 seconds live)
				  _energyDetractedWhenHit (10), // Level of energy reduced from an enemy when hit her / him / it
				  _pointsGivenWhenDestroyed (10), // When destroyed, the number of points given to the enemy who killed him/her
				  _automaticMovement (-1) // Means none by default. The id of the steps control monitor if any
							{ }

			Behaviour (bool cE, bool cS, QGAMES::bdata eL, QGAMES::bdata eD, int p, int aM = -1);

			/** 
			  * The behaviour can be set from a set of values.
			  * The "name" of the values is always a number (the id), 
			  * and the value is a string so they must interpreted.
			  * The meaning of those are:
			  * 0 (__BATTLESHIP_BHVOPENVALUEHURMEDEMISPARAM__)		: Can be hurmed by enemies? (YES | NO)
			  * 1 (__BATTLESHIP_BHVOPENVALUEHURMEDSIMSPARAM__)		: Can be hurmed by similars? (YES | NO)
			  * 2 (__BATTLESHIP_BHVOPENVALUEENERGYLOSTPARAM__)		: The energy lost (in percentage so between 0 and 100) per loop
			  * 3 (__BATTLESHIP_BHVOPENVALUEENERGYDETRACTEDPARAM__)	: The energy reduced from an enemy when hit her / him / it (also in percentage)
			  * 4 (__BATTLESHIP_BHVOPENVALUEPTSWHENDIEPARAM__)		: When it is destroyed, how many points are given to the hunter?
			  * 5 (__BATTLESHIP_BHVOPENVALUEAUTOMOVPARAM__)			: The steps monitor id associated to the element. -1 means none
			  */
			Behaviour (const QGAMES::SetOfOpenValues& oV);

			virtual Behaviour* clone ()
							{ return (new Behaviour (*this)); }

			bool _canBeHurmedByEnemies, _canBeHurmedBySimilars;
			QGAMES::bdata _energyLostPerLoop;
			QGAMES::bdata _energyDetractedWhenHit;
			int _pointsGivenWhenDestroyed;
			int _automaticMovement; // -1 means none...
		};

		/** Basic internal state for any space element. 
			It could describe, high level, the life cycle.
			0: _FROZEN		The spaceelement in in the scene but not moving.
			1: _HIDDEN		The spaceelement is still pending to appear, so it is not visible.
			2: _MOVING		The spaceelement is moving accross the space.
			3: _EXPLODING	The spaceelement has been hit and it is exploding. 
			4: _DESTROYED	The spaceelement has been destroyed eventually. */
		static const int _FROZEN = 0;
		static const int _HIDDEN = 1;
		static const int _MOVING = 2;
		static const int _EXPLODING = 3;
		static const int _DESTROYED = 4;

		SpaceElement (int cId, Behaviour* bhv, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

		/** Managing the behaviour of the spaceship. */
		const Behaviour* behaviour () const
							{ return (_behaviour); }
		Behaviour* behaviour () 
							{ return (_behaviour); }
		virtual void setBehaviour (Behaviour* bhv);

		/** @see parent. 
			The default implementation considers that the movement is in the XY plane and in 2D.
			In the moment the entity is out of a visible "zone", the movement is not possible. 
			@see the method getMovingZone to know how the zone is calculated. */
		virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a);

		/** To active or desactive to draw limits. */
		void setDrawLimits (bool dL)
							{ _drawLimits = dL; }

		/** To know the speed of the scene where the element is moving. 
			There is no method to change the speed.
			Only main character defines one (@see Battleship class). */
		int referenceSpeed () const;

		/** To verify whether a space element state is or not possible. \n
			it can be overloaded / completed in other futher classes. \n
			A default implementation to recognize the default space elements states is provided. */
		virtual bool isStatePossible (int st)
							{ return (st == _FROZEN || 
									  st == _HIDDEN || 
									  st == _EXPLODING || 
									  st == _MOVING ||
									  st == _DESTROYED); }

		/** To know whether another space element is or not an enemy of this one.
			By default all of them are. */
		virtual bool isEnemy (const SpaceElement* ss) const
							{ return (true); }

		/** To get the internal status of the space element.
			To change this state, a specific method should be implemented and used (e.g toHide. @see below) */
		int spaceElementState () const
							{ return (_spaceElementState); }

		/** Managing the basic states of the space elements.
			All of them can be modified in the future, but a default implementation is always provided. 
			A version to execute the state in a deferred loop is also provided. 
			This version can't be overloaded. */
		void toForgetDeferreStates ();
		virtual void toFreeze ()
							{ if (!isFrozen ()) setStateFor (_spaceElementState = _FROZEN); }
		bool isFrozen () const
							{ return (_spaceElementState == _FROZEN); }
		virtual void toHide ()
							{ if (!isHidden ()) setStateFor (_spaceElementState = _HIDDEN, orientation ()); }
		void toHideDeferred ();
		bool isHidden () const
							{ return (_spaceElementState == _HIDDEN); }
		virtual void toMove (int tM, const QGAMES::Vector& v);
		void toMoveDeferred (int tM, const QGAMES::Vector& v);
		bool isMoving () const 
							{ return (_spaceElementState == _MOVING); }
		bool isMovingTowards (const QGAMES::Vector& o)
							{ return (_spaceElementState == _MOVING && orientation () == o); }
		virtual void toExplode ();
		void toExplodeDeferred ();
		bool isExploding () const
							{ return (_spaceElementState == _EXPLODING); }
		bool startedToExplode () const
							{ return (isExploding () && currentAnimation () -> beginning ()); }
		bool hasExploded () const
							{ return (isExploding () && currentAnimation () -> end ()); }
		virtual void toDestroy ()
							{ if (hasExploded ()) setStateFor (_spaceElementState = _DESTROYED, orientation ()); }
		void toDestroyDeferred ();
		bool isDestroyed () const
							{ return (_spaceElementState == _DESTROYED); }
		// Initially is the same than not moving, but the framework could be extended to add new states
		// so this method should be extended too in that case!
		virtual bool isAlive () const
							{ return (isMoving () || isFrozen ()); }

		/** Manages the energy of the space element. 
			Always between 0 an 100. When 0, Changes will be notified in the inEveryLoop method. */
		QGAMES::bdata energy () const
							{ return (_energy); }
		void setEnergy (QGAMES::bdata e);
		/** The energy is actually only modified in the method inEveryLoop, so this other one just "take note"
			of the amount of energy to add (or substract) to/from the total in the inEveryLoop execution. 
			This method can be call from many different parts in a single loop.
			e.g. Just moving could reduce the energy (@see Behaviour), 
			but it could also be reduced when the space element is hit by other
			Every change in the energy is notified with the event: __BATTLESHIP_SPACEELMNTENERGYACTUALIZED__ */
		void setEnergyToAdd (QGAMES::bdata e)
							{ _energyToAdd += e; }
		/** Restore level of energy. */
		void restoreLevelOfEnergy ()
							{ _energyToAdd = 100 - _energy; }
		/** Stops / Continue losing energy.
			In some situations, the characters stops loosing energy.
			e.g. It could be shields. */
		bool losingEnergy () const
							{ return (_losingEnergy); }
		void setLosingEnergy (bool sL)
							{ _losingEnergy = sL; }

		/** To know the trend of the movement: The real direction, lets say. */
		const QGAMES::Vector& trendDirection () const
							{ return (_trendDirection); }
		void restartTrendDirection ()
							{ _lastPosition = position (); _trendDirection = QGAMES::Position::_cero; }

		/** @see parent. 
			The standard zone is reduced a little bit just to make more playable the game. */
		virtual QGAMES::Rectangle collisionZone () const;

		/** @see parent. */
		virtual void initialize ();
		virtual void inEveryLoop ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		/** To know the entity who has hit this other one the most. 
			If there were many, then the firt one would be returned. */
		SpaceElement* whoHitTheMost () const;
		
		// Related with the moving zone
		/** To know the zone where the space element can or not move. \n
			The zone is widen by the left and by the up side with the maximum size of the element. \n
			That zone will be the map if any, or the screen active in other case.
			These formulas are onlt valid for 2D games. */
		virtual QGAMES::Rectangle getMovingZone ();
		QGAMES::Position relativePositionInMovinZone (const QGAMES::Position& p)
							{ QGAMES::Rectangle mZ = getMovingZone (); 
							  return (QGAMES::Position ((p.posX () - mZ.pos1 ().posX ()) / mZ.width (), 
														(p.posY () - mZ.pos1 ().posY ()) / mZ.height (), __BD 0)); }
		QGAMES::Position relativeCenterPositionInMovingZone ()
							{ return (relativePositionInMovinZone (centerPosition ())); }

		protected:
		/** To change the state depending on the type of space element and its orientation. \n
			It has to be overloaded depending on the type of spaceship. */
		virtual void setStateFor (int st, const QGAMES::Vector& o = QGAMES::Vector::_cero) = 0;
		
		/** To add a new hit by a character. 
			This variable can be checked in any time to determinate decisions. */
		void hitBy (SpaceElement* sE);

		/** Very internal method to draw things around the entity. 
			The method can be activated or desactivated with an internal variable. */ 
		virtual void drawOnLimits (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint);

		protected:
		/** To change the state of the element using a buoy. 
			Only one state can be active at the same time. */
		class ToChangeSpaceElementStateBuoy : public QGAMES::Buoy
		{
			public:
			ToChangeSpaceElementStateBuoy (int id)
				: QGAMES::Buoy (id, 0), // The value will be the state to change...
				  _parameters ()
							{ }

			/** To set parameters. Maybe needed in a couple of states. */
			const QGAMES::SetOfOpenValues& parameters () const
							{ return (_parameters); }
			void setParameters (const QGAMES::SetOfOpenValues& oV)
							{ _parameters = oV; }

			/** @see parent. */
			virtual void* treatFor (QGAMES::Element* e);

			protected:
			QGAMES::SetOfOpenValues _parameters;
		};

		/** How the space elements behave. */
		Behaviour* _behaviour;
		/** Life cycle of any space element. 
			The uses of this internal status variable can vary regarding what the element is actually (implementation).
			@see above for the meaning. 
			Usually -1 means not defined. */
		int _spaceElementState;
		/** The energy the space element has. It goes from 100 (when initialized) to 0. */
		QGAMES::bdata _energy;
		/** To manage whether the space element reduced energy or not (is stopped. @see setLoosingEnergy). */
		bool _losingEnergy;

		// Implementation
		/** The level of energy to add or subtract in the next execution of inEveryMethod. */
		QGAMES::bdata _energyToAdd;
		/** A map with references to who and how many times has be hit the space element. */
		std::map <SpaceElement*, int> _hitsBy;
		/** Used to calculate the trend. */
		QGAMES::Position _lastPosition;
		QGAMES::Vector _trendDirection;

		private:
		/** To determinate whether the limits has or not to be drawn. 
			By default when the compilation mode is a debug mode, the limits will be shown. */
		bool _drawLimits;
	};

	// To simplify the definition of a set of elements...
	typedef std::vector <SpaceElement*> SpaceElements;

	/** The spaceship shooting. 
		The shooting can came either from the enemies or from our own space battlehip. 
		Every shooting has to have always a reference to his / her owner. 
		The shooting is "loaded" with energy (in percetage) that is reduced from the enemies it hits. */
	class Spaceship;
	class SpaceshipShooting : public QGAMES::Artist
	{
		public:
		SpaceshipShooting (int cId, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent. 
			When the element is out of the visual screen, then the movement is not possible any more. */
		virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a);

		/** To know and change the owner. It can be null, when no owner is still defined. */
		const Spaceship* owner () const
							{ return (_owner); }
		Spaceship* owner () 
							{ return (_owner); }
		void setOwner (Spaceship* sS)
							{_owner = sS; }

		/** To know and change the energy of the shooting. */
		QGAMES::bdata energy () const
							{ return (_energy); }
		void setEnergy (QGAMES::bdata e)
							{ _energy = e; }

		/** To know the type of shooting.
			The type can determine e.g. the animation used, and the form and aspect used (when launched).
			It can be overloaded for this reason. 
			In the default implementation, the type is just taken into account when launched, so this method has no effect 
			until toLaunchFrom is finally invoked. */
		int type () const
							{ return (_type); }
		virtual void setType (int t);

		/** Makes the shooting visible and starts the right animation. \n
			The method movementStateForType is invoked inside in the default implementation (@see below). 
			The method receives the central position of the launching. 
			In the default implementation that position is taken as the base of the shooting.
			The method receives also a factor to multiply if any the speed, and the direction of the shooting. 
			Both optional. No optional speed factor and going up by default. */ 
		virtual void toLaunchFrom (const QGAMES::Position& cPos, int sF = 1,  
			const QGAMES::Vector& d = QGAMES::Vector (__BD 0, __BD -1, __BD 0));
		inline bool isFlying () const;
		/** When the shooting collisions agains something, it has to explode. */
		virtual void toExplode ();
		void toExplodeDeferred ();
		inline bool isExploding () const;
		bool hasExploded () const
							{ return (isExploding () && currentAnimation () -> end ()); }
		/** The very basic animation is set, and it is also set as invisible. 
			The invisible set is what the toShoot method recognize to determinate whether a shooting is available. */
		virtual void toEnd ();

		/** @see parent. 
			Just to set the default form. It is needed. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint);

		/** @see parent. */
		virtual void whenCollisionWith (QGAMES::Entity* e);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		/** To know what is the movement state for a type of shooting. 
			The animation is of course part of the state. 
			-1 would mean none is right. Any case the method should control if the the parameter is or not right. 
			By default the method returns as a valid state the same value that the type (type = state). */
		virtual int movementStateForType (int t);

		protected:
		/** To explode the shooting deferred. */
		class ToExplodeDeferredBuoy : public QGAMES::Buoy
		{
			public:
			ToExplodeDeferredBuoy (int id)
				: QGAMES::Buoy (id, 0) // No parameteres needed actually
							{ }

			/** @see parent. */
			virtual void* treatFor (QGAMES::Element* e);
		};

		/** A referente to the one owning this element. 
			It is important not to forget to set this variable when shooting. */
		Spaceship* _owner;
		/** The energy of the shooting in percentage. */
		QGAMES::bdata _energy;
		/** The type of shooting. */
		int _type;

		// Implementation
		SpaceElement* _elementHitExplode;
		QGAMES::Position _elementHitInitialPos;
	};

	/** All spaceships appearing in the game must inherit from this one. 
		The spaceships are always space elements with the capacity to shoot. 
		Additionally any sapaceship has a score assigned. */
	class Spaceship : public SpaceElement
	{
		public:
		/** A set of parameters to define the behaviour of the spaceship. 
			This class extends the behaviour of the space element. */
		struct Behaviour : public SpaceElement::Behaviour
		{
			Behaviour ()
				: SpaceElement::Behaviour (),
				  _shootingEnergy (__BD 50), // Shooting energy.
											 // It will be substracted from the enemy's energy when hit it
				  _typeOfShooting (0),		 // Meaning the very basic one.
				  _shieldProtection (__BD 1) // By default, no protection...
							{ }

			Behaviour (bool cE, bool cS, QGAMES::bdata eL, QGAMES::bdata eD, int p, QGAMES::bdata s, 
					   int tS = 0, QGAMES::bdata sP = __BD 1.0, int aM = -1);

			/** 
			  * The behaviour can be set from a set of values.
			  * @see SpaceElement::Behaviour
			  * The name of the values are the ones in the parent class plus:
			  * 6 (__BATTLESHIP_BHVOPENVALUESHTENERGYPARAM__)	: The energy (between 0 and 100) the shootings of the entity has?
			  * 7 (__BATTLESHIP_BHVOPENVALUESHTTYPEPARAM__)		: The type of shooting used if any (could be -1 means none)
			  *	8 (__BATTLESHIP_BHVOPENVALUESHIELDFACTORPARAM__): Shield factor. How much the space is covered against shootings.
			  *													  It must be greater than 0 and the greater it is, much more 
			  *													  the spaceship is protected. between 0 (never) and 1 means no protection, 
			  *													  greater than 1 means some protection.
			  */
			Behaviour (const QGAMES::SetOfOpenValues& oV);

			/** @see parent. */
			virtual SpaceElement::Behaviour* clone () const
							{ return (new Behaviour (*this)); }

			QGAMES::bdata _shootingEnergy;
			int _typeOfShooting;
			QGAMES::bdata _shieldProtection;
		};

		Spaceship (int cId, Behaviour* bhv, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

		/** @see parent. 
			Overloaded just to guaranttee that the behaviour has the proper type (Spaceship). */
		virtual void setBehaviour (SpaceElement::Behaviour* bhv);

		// The specify things of this type of elements...
		/** To know whether is or not possible to shoot. 
			By default it is always possible. */
		virtual bool isPossibleToShoot () const
							{ return (true); }
		/** To shoot. \n
			The parameters indicated the "impulse" of the fire if any (by default 1),
			and the direction of the shooting. \n
			The default implementation neither takes into account the force nor the directon of the shoot,
			but in other implementations of the library it could be needed. 
			Returns true when it has been possible to shoot. \n
			The method receives also the number of position where the shooting is taking place from. 
			In some ocassiones, the battleship could shoot 
			through different positions (when it is big e.g). By default it is 0 and it is not used. */
		virtual bool toShoot (int f = 1, 
			const QGAMES::Vector& d = QGAMES::Vector (__BD 0, __BD -1, __BD 0), int nP = 0);
		/** To jump. \n
			This method could be used to make special movements in the space. \n
			The parameters are the "impulse" of the jump and the direction of it. \n
			The default implementation doesn't do anything. */
		virtual void toJump (int f = 1, const QGAMES::Vector& d = QGAMES::Vector::_cero)
							{ }

		/** To protect the battleship from external shootings. \n
			The parameter is the time in seconds the battleship is shield. \n
			When the number is -1 (or negative in general) means forever. 
			When the number is 0 means never, and if the spaceship was already sealed, it won't be any more. */
		virtual void toSeal (QGAMES::bdata s = __BD 1);
		bool isSealed () const
						{ return (onOffSwitch (_SWITCHSEALED) -> isOn ()); }

		/** To change the power.
			The default implementation affects both to the type of shooting and also to the characteristics 
			of the shooting itself (number of simultaneous shootings, instances per shooting, and so on. \n
			It is guaranttee that the power level is between 0 and __BATTLESHIP_NUMBEROFPOWERLEVELS__ that 
			is a varible defined in DataGame also. */
		int powerLevel () const
							{ return (_powerLevel); }
		virtual void setPowerLevel (int pL);

		/** Manages the score. */
		int score () const
							{ return (_score); }
		/** Any change in the score should be notified with the event: __BATTLESHIP_SPACESHIPSCOREACTUALIZED__
			It is what the default implementation does. */
		virtual void setScore (int s);

		/** Any spacesship has a set of shootings that can be shoot
			represented by a list of entities. Those ones have to be preloaded into the class. \n
			The shootings have to be loaded before intiializing the spaceship. \n
			They are added in BATTLESHIP::Scene's initialize method. */
		void loadShootings (const QGAMES::Entities& sth); 
		/** The first "available" shooting will be that one not still in use. 
			This one should be the first one marked as "not visible". \n
			Take care, the result can be null because none is available. */
		SpaceshipShooting* firstAvailableShooting (); // Null when none...
		void clearShootings ()
							{ _shootings = QGAMES::Entities (); }

		// Managing the behaviour of the shooting
		// The methods can be overloaded later to adapt them to the specific implementation of the spaceship
		/** How many shooting can be simultaneously "flying" in the space. 
			-1 means no limit. */
		virtual int maxNumberOfShootingsInSpaceSimultaneouly () const
							{ return (-1); }
		/** How many shootings are available? -1 means no limit. */
		virtual int maxNumberOfShootingsAvailable () const
							{ return (-1); } 
		/** How many shooting per fire?.
			By default just the ones defined by the power level. */
		virtual int instancesPerShooting () const
							{ return (__BATTLESHIPDATAGAME__ -> _LEVELDATA [_powerLevel][3]); }
		/** How many shootings are still available? -1 means "all" */
		virtual int shootingsRemaining () const
							{ return (-1); }
		/** Reduce the number of shooting available. 
			As the default implementation doesn't consider any limit, this method does nothing. */
		virtual void reduceShootingsRemaining (int a)
							{ }
		/** To calculate the position for a specific number of shooting.
			The method receives the number of shooting and the total to shootings. 
			The method can be overloaded later. 
			The default implementation adjust the shootings within the width of the spaceship.
			First one will be in the middle, second a little bit to the right, 
			the third one symmetric in the left side, and so on and so forth... \n
			The last parameter indicatesd the number of position where the shooting is taking place. 
			By deafult it is 0 and it is not considered. */
		virtual QGAMES::Position shootingPosition (int nS, int tS, int nP = 0) const;

		/** @see parent. */
		virtual void initialize ();
		/** To treat the sealed state when needed. */
		virtual void updatePositions ();

		/** @see parent. */
		virtual void whenCollisionWith (QGAMES::Entity* e);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }

		protected:
		/** The power level of the Spaceship, affecting (e.g) the type of shooting. */
		int _powerLevel;
		/** The number of points the spaceship has. */
		int _score;
		/** A reference to the shootings available for the spaceship.
			The method to add them verifies they are all real "shootings". */
		QGAMES::Entities _shootings;

		/** To end a shooting in a different loop than the one processing the events. 
			When a shooting ends (it exit the visible screen, it hits other...), it is needed 
			to change it state (@see toEnd). \n
			But it can't be done in the same loop than the one notifying the event because 
			the system could become unestable. Ths is the reason to do so using a buoy. \n
			Several shooting can be ended in the same loop. */
		class ToEndShootingBuoy : public QGAMES::Buoy
		{
			public:
			ToEndShootingBuoy (int id)
			: QGAMES::Buoy (id, 0), // Value is not used...
			  _shootings ()
							{ }

			int numberShootings () const
							{ return ((int) _shootings.size ()); }

			const SpaceshipShooting* shooting (int nS) const
							{ return ((nS >= 0 && nS < (int) _shootings.size ()) ? _shootings [nS] : NULL); }
			SpaceshipShooting* shooting (int nS)
							{ return ((nS >= 0 && nS < (int) _shootings.size ()) ? _shootings [nS] : NULL); }
			void addShooting (SpaceshipShooting* sht);

			/** @see parent. */
			virtual void* treatFor (QGAMES::Element* e);
			
			private:
			std::vector <SpaceshipShooting*> _shootings;
		};

		/** The counters and the switches... */
		static const int _COUNTERTIMESEALED = 0;
		static const int _SWITCHSEALED = 0;
	};

	/** The asteroids are something to collision with. */
	class Asteroid : public SpaceElement
	{
		public:
		/** Structure to define together all states related with a type of asteroid. */
		struct StatesId
		{
			StatesId ()
				: _frozenState (-1), _hiddenState (-1), _movingState (-1), 
				  _explodingState (-1), _destroyedState (-1)
							{ }

			StatesId (int fS, int hS, int mS, int eS, int dS)
				: _frozenState (fS), _hiddenState (hS), _movingState (mS), 
				  _explodingState (eS), _destroyedState (dS)
							{ }

			StatesId (const std::vector <int>& sts)
				: _frozenState (-1), _hiddenState (-1), _movingState (-1), 
				  _explodingState (-1), _destroyedState (-1)
							{ assert (sts.size () == 5);
							  _frozenState = sts [0]; _hiddenState = sts [1]; _movingState = sts [2];
							  _explodingState = sts [3]; _destroyedState = sts [4]; }

			int _frozenState;
			int _hiddenState;
			int _movingState;
			int _explodingState;
			int _destroyedState;
		};

		/** The default behaviour is:
			Affected by enemies, 
			Not affected by similars, 
			energy lost per second 0% (it doesn't ever get tired)
			energy detracted from other when hit 100% (meaning that the otehr will be destroyed ever),
			10 points given to the enemies when it is destroyed. */
		Asteroid (int cId, Behaviour* bhv = new Behaviour (true, false, __BD 0, __BD 100, 10), 
				const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: SpaceElement (cId, bhv, f, d),
			  _type (-1), // Not defined...
			  _statesId ()
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent. */
		virtual bool isEnemy (const SpaceElement* ss) const;

		/** To change the type of asteroid. 
			When the type is changed then the state could be also actualized. By default it is. */
		int type () const
							{ return (_type); }
		virtual void setType (int t); // -1 means random 

		/** To change the points that the asteroid has inside.
			Those points will be given to the one who hit it the most when it is destroyed. */
		void setPoints (int p)
							{ behaviour () -> _pointsGivenWhenDestroyed = p; }

		/** @see parent. */
		virtual void initialize ();

		/** @see parent. 
			In the case of the asteroid the collision zone is almost the full visible zone. 
			It is supossed that is more dangerous, and this is the reason. */
		virtual QGAMES::Rectangle collisionZone () const;

		/** @see parent. */
		virtual void whenCollisionWith (QGAMES::Entity* e);

		protected:
		/** @see parent. 
			In the default implementation the state is linked to the type. */
		virtual void setStateFor (int st, const QGAMES::Vector& o = QGAMES::Vector ());

		protected:
		/** The type of the asteroid. */
		int _type;

		// Implementation
		StatesId _statesId;
	};

	/** ...and also things to be caught. 
		Any ThingToCatch has to have an internal type.
		The interpretation of the type depends on the implementation of the specific class. */
	class ThingToCatch : public SpaceElement
	{
		public:
		/** Structure to define together all states related with a type of thing to catch. */
		struct StatesId
		{
			StatesId ()
				: _frozenState (-1), _hiddenState (-1), _movingState (-1), 
				  _explodingState (-1), _destroyedState (-1)
							{ }

			StatesId (int fS, int hS, int mS, int eS, int dS)
				: _frozenState (fS), _hiddenState (hS), _movingState (mS), 
				  _explodingState (eS), _destroyedState (dS)
							{ }

			StatesId (const std::vector <int>& sts)
				: _frozenState (-1), _hiddenState (-1), _movingState (-1), 
				  _explodingState (-1), _destroyedState (-1)
							{ assert (sts.size () == 5);
							  _frozenState = sts [0]; _hiddenState = sts [1]; _movingState = sts [2];
							  _explodingState = sts [3]; _destroyedState = sts [4]; }

			int _frozenState;
			int _hiddenState;
			int _movingState;
			int _explodingState;
			int _destroyedState;
		};

		ThingToCatch (int cId, Behaviour* bhv, const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: SpaceElement (cId, bhv, f, d),
			  _type (-1) // Not defined
							{ }

		/** To know and change the type of the thing to catch. \n
			The default implementation of the method sets the new type and invokes the method setStateFor
			where not only the state but also the type should be taken into account depending the implementation. \n 
			setStateFor receives the current space element state and aldo the current orientation. */
		int type () const
							{ return (_type); }
		virtual void setType (int t); // -1 means random

		/** When the element is caught something happens. 
			That... It has to be defined in later futher classes!. */
		virtual void whenCaughtBy (Spaceship* spc) = 0;

		/** @see parent. */
		virtual void initialize ();

		/** @see parent. */
		virtual void whenCollisionWith (QGAMES::Entity* e);

		protected:
		/** @see parent.
			This default implmeentation takes into account the type when the state is _MOVING. */
		virtual void setStateFor (int st, const QGAMES::Vector& o = QGAMES::Vector ());

		protected:
		/** The type. */
		int _type;

		// Implementation
		StatesId _statesId;
	};
}

// Includes specific entities inheriting from the previous ones...
#include <BattleshipLike/bsufo.hpp>
#include <BattleshipLike/bsbattleship.hpp>
#include <BattleshipLike/bsthingstocatch.hpp>

#endif
  
// End of the file
/*@}*/