/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: dwmovements.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Forni?s (Community Networks) \n
 *	Creation Date: 18/04/2018 \n
 *	Description: Defines the different movements of a Dragonwind like game.
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_MOVEMENTS__
#define __DRAGONWIND_MOVEMENTS__

#include <Dragonwindlike/dwdefinitions.hpp>
#include <AdvancedArcade/admovements.hpp>

namespace DRAGONWIND
{
	/** A movement simulating when the fish swmin in a liquid defined by its density (related with water). */
	class SwimingPhysicsIncrementalMovement : public QGAMES::PhysicsIncremetalMovement
	{
		public:
		SwimingPhysicsIncrementalMovement (int id, std::map <int, double> v)
			: QGAMES::PhysicsIncremetalMovement (id, v),
			  _liquidDensity (__BD 2), // Value always bigger than 0.
									   // Density of the water is 1 so, bigger means oil e.g.
			  _liquidForce (QGAMES::Vector::_cero)
							{ }

		/** @see parent. */
		virtual Movement* clone () const;

		/** @see parent. \n
			The information added is the liquid density (openValue). */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** To control the density of the liquid, respect the water.
			It means that 2.00 is a liquid twice dense than the water, and 0.5 the opposite.
			This value is used to fix the friction coefficient in the parent class when started. */
		QGAMES::bdata liquidDensity () const
							{ return (_liquidDensity); }
		void setLiquidDensity (QGAMES::bdata lD); 

		/** @see parent. */
		virtual void initialize ();
		virtual void initializeFrom (const QGAMES::Movement* m);

		protected:
		/** @see parent. */
		virtual void prepareMovement (Data* d, const QGAMES::Vector& dr, const QGAMES::Vector& a, QGAMES::Entity* e);
		virtual void calculateNextMovement (Data* d, const QGAMES::Vector& dr, const QGAMES::Vector& a, QGAMES::Entity* e);
		virtual void stopDuringMovement (const QGAMES::Vector& dr, const QGAMES::Vector& a, QGAMES::Entity* e);

		protected:
		QGAMES::bdata _liquidDensity;

		// Implementation
		QGAMES::Vector _liquidForce;
	};
}

#endif
  
// End of the file
/*@}*/