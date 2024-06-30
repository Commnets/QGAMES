/** \ingroup Battleship */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: bsthingstocatch.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 02/11/2019 \n
 *	Description: Define the default things to e caught along the game.
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_THINGSTOCATCH__
#define __BATTLESHIP_THINGSTOCATCH__

#include <BattleshipLike/bsartists.hpp>

namespace BATTLESHIP
{
	/** The more typical thing to catch is a "pill" of power. \n
		The pill of power affects e.g to the type of shooting. \n
		By default there are three different types of pill (__PILLTYPE?).
		This type is taken into account when the state is set (_MOVING). \n  
		_EXPLODING is not an allowed state either*/
	class PillPowerToCatch : public ThingToCatch
	{
		public:
		// The deafult types of pill...
		static const int _PILLTYPE1 = 0;
		static const int _PILLTYPE2 = 1;
		static const int _PILLTYPE3 = 2;
		static const int _PILLTYPE4 = 3;

		/** The default behaviour is:
			Not affected by enemies, 
			Not affected by similars, 
			energy lost per loop 0% (it doesn't ever dissappear)
			energy detracted from other when hit 0% (it is to transmit power of fire),
			0 points given to the enemies when it is absorved. */
		PillPowerToCatch (int cId, Behaviour* bhv = new Behaviour (false, false, __BD 0, __BD 0, 0), 
				const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: ThingToCatch (cId, bhv, f, d),
			  _power (0)
							{ _type = _PILLTYPE1; }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** @see parent. */
		virtual void setType (int t)
							{ ThingToCatch::setType ((t == -1) // to strength the number of possibilities...
								? (rand () % __BATTLESHIP_NUMBEROSHOOTINGPILLTYPES__) : t); }

		/** To know thepower of the pill.
			The power can't be breater than the maximum allowed. */
		int power () const
							{ return (_power); }
		virtual void setPower (int p);

		/** @see parent. */
		virtual void whenCaughtBy (Spaceship* spc);

		protected:
		int _power;
	};

	/** The energy is something that load of energy the battleship when caught. */
	class EnergyElement : public ThingToCatch
	{
		public:
		static const int _ENERGYTOCATCH = 4; // After the pills of energy...

		EnergyElement (int cId, Behaviour* bhv = new Behaviour (false, false, __BD 0, __BD -10, 10), 
				const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: ThingToCatch (cId, bhv, f, d)
							{ _type = _ENERGYTOCATCH; }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** Only one type is valid. */
		virtual void setType (int t)
							{ ThingToCatch::setType (_ENERGYTOCATCH); /** Always. */ }

		/** To set the level of energy of the element. 
			The method can be changed in classes inheriting this one. \n
			The default implementation changes the variable _energyDetractedWhenHit of the behaviour,
			using the parameter passed in a negative way. \n
			The default implementation also restart the state when the level is changed (using the same orientation). */
		QGAMES::bdata eneryLevel () const;
		virtual void setEnergyLevel (QGAMES::bdata eL);

		/** @see parent. */
		virtual void whenCaughtBy (Spaceship* spc);
	};
	
	/** The life is another special thing to be cath. \n
		It behaves the way the energy pill does. It is, when collision happens the element is automatically caught. */
	class LiveToCatch : public ThingToCatch
	{
		public:
		static const int _LIVETOCATCH = 5; // After the pills of energy...

		LiveToCatch (int cId, Behaviour* bhv = new Behaviour (false, false, __BD 0, __BD 0, 0), 
				const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: ThingToCatch (cId, bhv, f, d)
							{ _type = _LIVETOCATCH; }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** Only one type is valid. */
		virtual void setType (int t)
							{ ThingToCatch::setType (_LIVETOCATCH); /** Always. */ }

		/** @see parent. */
		virtual void whenCaughtBy (Spaceship* spc);
	};
	
	/** The shield is another special thing to be cath. \n
		It protectes yoy from the impact for a while (defined in the properties. */
	class ShieldToCatch : public ThingToCatch
	{
		public:
		static const int _SHIELDTOCATCH = 6; // After the live to catch...

		ShieldToCatch (int cId, Behaviour* bhv = new Behaviour (false, false, __BD 0, __BD 0, 0), 
				const QGAMES::Forms& f = QGAMES::Forms (), 
				const QGAMES::Entity::Data& d = QGAMES::Entity::Data ())
			: ThingToCatch (cId, bhv, f, d),
			  _secondsShield (__BD 2) // By default...
							{ _type = _SHIELDTOCATCH; }

		/** @see parent. */
		virtual QGAMES::Entity* clone () const;

		/** To know and set the seconds the shield works. */
		QGAMES::bdata secondsShield () const
							{ return (_secondsShield); }
		void setSecondsShield (QGAMES::bdata s) 
							{ assert (s >= 0); _secondsShield = s; }

		/** Only one type is valid. */
		virtual void setType (int t)
							{ ThingToCatch::setType (_SHIELDTOCATCH); /** Always. */ }

		/** @see parent. */
		virtual void whenCaughtBy (Spaceship* spc);

		protected:
		QGAMES::bdata _secondsShield;
	};
}

#endif
  
// End of the file
/*@}*/