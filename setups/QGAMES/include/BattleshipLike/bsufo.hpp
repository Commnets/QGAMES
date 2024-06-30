/** \ingroup Battleship */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: bsufo.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 12/12/2019 \n
 *	Description: Define the base of the ufos managed in the game.
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_UFO__
#define __BATTLESHIP_UFO__

#include <BattleshipLike/bsartists.hpp>

namespace BATTLESHIP
{
	/** This class represents an enemy. 
		The UFOs palying in the game, are maybe one of the most configurable parts of the game. 
		The configurable files can be redefined to admit new types of UFOS.
		This is why the class StatesId has been defined inside. 
		The method setType sets the StatesId of the ufo rtaking into account the type received as parameter.
		The possible States are defined in the DataGame structure set by the class Game when created.
		@see DataGame class. */
	class UFO : public Spaceship
	{
		public:
		/** Structure to define together all states related with a type of ufo. */
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
			energy lost per second: 0%
			energy detracted from other when hit: 50% (meaning that the other will be destroyed with two contacts),
			100 points given to the enemies when it is destroyed
			shooting energy (reduced from the one hit) 3%. 
			basic type of shooting (0). */
		UFO (int cId, 
				Behaviour* bhv = new Behaviour (true, false, __BD 0, __BD 50, 100, __BD 3, 0, __BD 1.0), 
				const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: Spaceship (cId, bhv, f, d),
			  _type (-1), // Not defined... 
			  _statesId ()
							{ }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent. 
			Inthe case of UFOs only Battleships are enemies. */
		virtual inline bool isEnemy (const SpaceElement* ss) const;

		/** To change the type of the ufo.
			Take care doing this acton when moving. */
		int type () const
							{ return (_type); }
		virtual void setType (int t); // -1 means random

		/** @see parent. */
		virtual QGAMES::Position shootingPosition (int nS, int tS, int nP = 0) const;

		/** @see parent. */
		virtual void initialize ();

		/** @see parent. 
			Just the core. */
		virtual QGAMES::Rectangle collisionZone () const;

		protected:
		/** @see parent. */
		virtual void setStateFor (int st, const QGAMES::Vector& o = QGAMES::Vector ());

		protected:
		/** The type of UFO */
		int _type;

		// Implementation
		StatesId _statesId;
	};

	/** A big UFO. \n
		In a big UFO there is no a single weak point as usual in the rest. \n
		Those weak points should be located in many different and distributes parts.
		And these zones and parts will depend on the frame and aspect. */
	class BigUFO : public UFO
	{
		public:
		BigUFO (int cId, 
				BATTLESHIP::UFO::Behaviour* bhv = 
					new BATTLESHIP::UFO::Behaviour (true, false, __BD 0, __BD 50, 100, __BD 3, 0, __BD 3.0), 
				const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: UFO (cId, bhv, f, d),
			  _centralShootingPositions (), _weakAreas (),
			  _lastForm (__MININT__), _lastAspect (__MININT__) // Meaning not defined...
							{ }

		/** @see parent. */
		virtual bool hasCollisionWith (QGAMES::Entity* ety) const;

		/** To know the central shooting positions for a specific form and aspect. */
		const QGAMES::Positions& centralShootingPositionsForFormAndAspect (int f, int a) const;
		/** Same for the weak areas. */
		const QGAMES::Rectangles& weakAreasForFormAndAspect (int f, int a) const;

		protected:
		/** @see parent. */
		virtual void drawOnLimits (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint);

		/** To calculate the central positions for a specific form and aspect. */
		virtual QGAMES::Positions calculateCentralShootingPositionsForFormAndAspect (int f, int a) const
							{ return (QGAMES::Positions { (*_forms.find (f)).second -> centerPosition (a) }); }
		/** Same for the weak zones used when detecting collisions e.g. */
		virtual QGAMES::Rectangles calculateWeakAreasForFormAndAspect (int f, int a) const
							{ return (QGAMES::Rectangles { (*_forms.find (f)).second -> roundingCentralZone (a) }); }

		protected:
		/** To determine the list of the different shooting positions. \n
			A normal UFO has only one. In a big UFO there can be several. \n
			As they are actually associated with parts of the space. */
		mutable std::map <int, std::map <int, QGAMES::Positions>> _centralShootingPositions;
		/** The same the the parts declared as weak. */
		mutable std::map <int, std::map <int, QGAMES::Rectangles>> _weakAreas;

		// Implementation
		int _lastForm, _lastAspect;
	};
}

#endif
  
// End of the file
/*@}*/