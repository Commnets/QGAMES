/** \ingroup BSII */
/*@{*/

/**	
 *	@file	
 *	File: Artists.hpp \n
 *	Game: Battleship II \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 08/12/2020 \n
 *	Description: The specific artists (Ufos, battleships, and so on) this game needs. \n
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIPII_ARTISTS__
#define __BATTLESHIPII_ARTISTS__

#include "Defs.hpp"
#include <BattleshipLike/bsinclude.hpp>

namespace BattleshipII
{
	/** This big UFO ahoots from many different places at the same time. \n
		and can only be killed when all the power forces are also killed. \n
		The power positions are defined taking into account the "basic" form of the mothership, 
		they have to be recalculated as the motherhips moves. */
	class MothershipUFO : public BATTLESHIP::BigUFO
	{
		public:
		MothershipUFO (int cId, 
				BATTLESHIP::UFO::Behaviour* bhv = 
					new BATTLESHIP::UFO::Behaviour (true, false, __BD 0, __BD 50, 100, __BD 3, 0, __BD 8.0), // A shield of 8.0 
				const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

		virtual ~MothershipUFO ()
							{ deleteExplosionStatus (); }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const override final;

		/** @see parent. */
		virtual bool toShoot (int f = 1, 
			const QGAMES::Vector& d = QGAMES::Vector (__BD 0, __BD -1, __BD 0), int nP = 0) override final;

		/** @see parent. */
		virtual QGAMES::Position shootingPosition (int nS, int tS, int nP = 0) const override final;

		/** @see parent.
			When exploding, many explosions happen across the UFO. */
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* scr, const QGAMES::Position& pos = QGAMES::Position::_noPoint) override final;

		protected:
		/** @see parent. */
		virtual void setStateFor (int st, const QGAMES::Vector& o = QGAMES::Vector::_cero) override final;

		/** @see parent. */
		virtual QGAMES::Positions calculateCentralShootingPositionsForFormAndAspect (int f, int a) const;
		virtual QGAMES::Rectangles calculateWeakAreasForFormAndAspect (int f, int a) const;

		// Implementation
		void deleteExplosionStatus ()
							{ for (auto i : _explosionStatus) delete (i); 
							  _explosionStatus = std::vector <ExplosionStatus*> (); }

		protected:
		/** A counter for the time it is exploding. */
		static const int _COUNTEREXPLODING = 1; // One more that the one defined at Battleship level
		/** A counter to change the status. */
		static const int _COUNTERTOCHANGEANIMATION = 2;

		// Implementation
		struct ExplosionStatus
		{
			ExplosionStatus ()
				: _beingUsed (false), _numberTimesUsed (0), _position (QGAMES::Position::_cero), _numberAspect (0)
							{ }

			ExplosionStatus (bool u, int nt, const QGAMES::Position& p, int nA)
				: _beingUsed (u), _numberTimesUsed (nt), _position (p), _numberAspect (nA)
							{ }

			bool _beingUsed;
			int _numberTimesUsed;
			QGAMES::Position _position; // Is relative
			int _numberAspect;
		};

		/** The id of the animation used for the different explosions. */
		int _explosionFormId, _explosionInitialFrame, _explosionFinalFrame;
		/** The status of the different explosions. */
		std::vector <ExplosionStatus*> _explosionStatus;
	};
}

#endif
  
// End of the file
/*@}*/
