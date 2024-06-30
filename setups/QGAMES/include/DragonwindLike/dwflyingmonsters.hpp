/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: dwflyingmonsters.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 28/06/2019 \n
 *	Description: Define the behaviour of the flying monsters that appear in the game
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_FLYINGMONSTERS__
#define __DRAGONWIND_FLYINGMONSTERS__

#include <DragonwindLike/dwdefinitions.hpp>
#include <DragonwindLike/dwartists.hpp>

namespace DRAGONWIND
{
	/** To define any flying monster in the scene. 
		The main properties of any flying monster should be:
		Two states call __DRAGONWIND_MONSTERFLYINGRIGHTSTATEID__ (0) an __DRAGONWIND_MONSTERFLYINGLEFTSTATEID__ (1). 
		0 when looking to the right side, and 1 to the left. 
		A unique movement type QGAMES::IncrementalLinearMovement is used. */
	class FlyingMonster : public QGAMES::Character
	{
		public:
		struct Properties
		{
			Properties ()
				: _shootingEnergy (1), // Very simple by default...
				  _dieWhenHit (true)
							{ }

			Properties (int sE, bool dH)
				: _shootingEnergy (sE),
				  _dieWhenHit (dH)
							{ }

			int _shootingEnergy;
			bool _dieWhenHit;
		};

		FlyingMonster (int cId, const Properties& prps = Properties (), 
				const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::PlatformArtist::Data& d = QGAMES::PlatformArtist::Data ())
			: QGAMES::Character (cId, f, d),
			  _properties (prps),
			  _destinationPoint (QGAMES::Position::_cero)
							{ }

		/** To manage the properties. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps)
							{ _properties = prps; }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** Sets the point to fly to. */
		void moveToDestinationPoint (const QGAMES::Position& p, const QGAMES::Vector& iS, 
			QGAMES::bdata mS, QGAMES::bdata a);

		/** @see parent. 
			The collision zone is a little bit less. */
		virtual QGAMES::Rectangle collisionZone () const
							{ return (QGAMES::Character::collisionZone ().scale (__BD 0.25)); }

		/** @see parent. */
		virtual void initialize ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		/** @see parent. */
		virtual void whenCollisionWith (QGAMES::Entity* e);

		protected:
		Properties _properties; 

		// Implementation
		QGAMES::Position _destinationPoint;
	};

	/** A set of flying monsters flying as a flock of birds, bees,... */
	class FlockOfMonsters : public QGAMES::CompositeEntity
	{
		public:
		struct FlyingZone
		{
			FlyingZone ()
				: _frontZone (QGAMES::Game::game () -> mainScreen () -> position () + // In the place where the camera is pointing...
							  QGAMES::Game::game () -> mainScreen () -> visualZone ()),
				  _depth (__BD 0) // 2D by default...
							{ }
			FlyingZone (const QGAMES::Rectangle& fZ, QGAMES::bdata d)
				: _frontZone (fZ),
				  _depth (d)
							{ }

			void setFrontZoneToCurrentScreenSize ()
							{ _frontZone = QGAMES::Game::game () -> mainScreen () -> position () + 
										   QGAMES::Game::game () -> mainScreen () -> visualZone (); }

			QGAMES::Position randomPosition () const;

			QGAMES::Rectangle _frontZone;
			QGAMES::bdata _depth;
		};

		struct Properties
		{
			Properties ()
				: _numberOfMonsters (1),
				  _radix (__BD 50), // In pixels
				  _timeToMove (__BD 1), // In seconds
				  _timeMoving (__BD 3), // In seconds
				  _shootingEnergy (1), // Shooting energy (up to 100)
				  _dieWhenHit (true), // Do the mosters die when hit the enemy?
				  _flyingZone ()
							{ }

			Properties (int nM, QGAMES::bdata r, QGAMES::bdata ttM, QGAMES::bdata tM, 
						int sE, bool dWH, FlyingZone& fZ)
				: _numberOfMonsters (nM),
				  _radix (r),
				  _timeToMove (ttM),
				  _timeMoving (tM),
				  _shootingEnergy (sE),
				  _dieWhenHit (dWH),
				  _flyingZone (fZ)
							{ assert (_numberOfMonsters > 0 && _radix > 0 && 
									  _timeToMove > __BD 0 && _timeMoving > __BD 0 &&
									  _shootingEnergy > 0 && _shootingEnergy < 100); }

			/** 
				A constructor based on parameters.
				It is used in the FlockOfMonsters constructor.
				The flying zone can't be defined in a parameter.
				The attributes to define when the composite entity is defined are...
				...
				<Attributes>
					<Attribute id = "NUMBEROFMONSTERS" value = "0..."/>
					<Attribute id = "RADIX" value = "1..."/>
					<Attribute id = "TIMETOMOVE" value = "0..."/> 
					<Attribute id = "TIMEMOVING" value = "0..."/>
					<Attribute id = "SHOOTINGENERGY" value = "1..100"/>
					<Attribute id = "DIEWHENHIT" value"YES|NO"/>
				</Attributes>
				...
			*/
			Properties (const std::map <std::string, std::string>& prps);

			/** The number of monsters flying. */
			int _numberOfMonsters;
			/** The space in which the different mosters are spreaded. */
			QGAMES::bdata _radix;
			/** Time (in secodns) to wait before the monsters start to move. */
			QGAMES::bdata _timeToMove;
			/** Time (in seconds) the monsters move before changing the direction. */
			QGAMES::bdata _timeMoving;
			/** The energy applied to the enemies when shooting them */
			int _shootingEnergy;
			/** Do the monsters die after hitting the enemy (and reducing his / her energy too)? .*/
			bool _dieWhenHit;
			/** Which is the area (a cube) in the space in which the monsters fly. */
			FlyingZone _flyingZone;
		};
		
		/** The physical variables 0 y 1 have to be defined in the _data internal attribute.
			This is checked at construction time.
			They mean:
			0: Max speed when a flying monster moves.
			1: Acceleration to get the max speed. Both in pixels / s */
		FlockOfMonsters (int cId, const QGAMES::Entities& eties, 
			const Properties& prps = Properties (), 
			const QGAMES::Entity::Data& dt = QGAMES::Entity::Data ());

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** To manage the properties. */
		const Properties& properties () const
							{ return (_properties); }
		void setProperties (const Properties& prps) 
							{ _properties = prps; initialize (); /** Restart. */ }

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint);
		virtual void finalize ();

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
		/** The properties of the flock. */
		Properties _properties;

		// Implementation
		FlyingMonster* _leader; // The first one...
		bool _leadReachedGoal; // True when moving and the lead reaches the goal...

		static const int _COUNTERTOMOVE = 0;
		static const int _COUNTERMOVING = 1;
		static const int _SWITCHMOVING = 0;
	};

	/** Specific flock of bees */
	class FlockOfBees : public FlockOfMonsters
	{
		public:
		FlockOfBees (int cId, const QGAMES::Entities& eties, const QGAMES::Entity::Data& dt = QGAMES::Entity::Data ())
			: FlockOfMonsters (cId, eties, Properties (dt._parameters), dt)
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent 
			The initialization of the sound is specific. */
		virtual void initialize ();
		virtual void finalize ();
	};

	/** Specific flock of birds. */
	class FlockOfBirds : public FlockOfMonsters
	{
		public:
		FlockOfBirds (int cId, const QGAMES::Entities& eties, const QGAMES::Entity::Data& dt = QGAMES::Entity::Data ())
			: FlockOfMonsters (cId, eties, Properties (dt._parameters), dt)
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent 
			The initialization of the sound is specific. */
		virtual void initialize ();
		virtual void finalize ();
	};
}

#endif
  
// End of the file
/*@}*/
