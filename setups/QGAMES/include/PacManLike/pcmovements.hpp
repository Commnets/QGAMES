/** \ingroup PACMAN */
/*@{*/

/**	
 *	@file	
 *	File: pcmovements.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 30/10/2021 \n
 *	Description: The maze movement defines how any elements moves in the maze. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMAN_MOVEMENTS__
#define __PACMAN_MOVEMENTS__

#include <PacManLike/pcdefs.hpp>

#include <AdvancedArcade/adinclude.hpp>

namespace PACMAN
{
	/** Movements in the maze are similar for both pacman and monsters. */
	class MazeMovement final : public QGAMES::Movement
	{
		public:
		static const QGAMES::bdata _CONSTANT;

		MazeMovement () = delete;

		MazeMovement (int id, ::std::map <int, double> v)
			: QGAMES::Movement (id, v),
			  _speed (__BD 0),
			  _lastDirection (QGAMES::Vector::_cero),
			  _qLeft (__BD 0)
							{ }

		MazeMovement (const MazeMovement&) = delete;

		const MazeMovement& operator = (const MazeMovement&) = delete;

		virtual Movement* clone () const
							{ return (new MazeMovement (_id, _variables)); }

		virtual QGAMES::Vector direction () const override
							{ return (_lastDirection); }
		virtual QGAMES::Vector acceleration () const override
							{ return (QGAMES::Vector::_cero); } // No accelration in this simply movement...

		/** To set the speed. 
			This parameter comes from the configuration of the game. */
		QGAMES::bdata speed () const
							{ return (_speed); }
		void setSpeed (QGAMES::bdata s)
							{ _speed = s; }

		virtual void initialize () override;
		virtual void initializeFrom (const QGAMES::Movement* m) override;
		virtual void move (const QGAMES::Vector& d, const QGAMES::Vector& a, QGAMES::Entity* e) override;

		protected:
		/** The speed in the movement. */
		QGAMES::bdata _speed;

		// Implementation
		QGAMES::Vector _lastDirection;
		/** This variable is to define how much is left to move from th previous movement. 
			It is actualized inside the updatePositions method. */
		QGAMES::bdata _qLeft;
	};
}

#endif
  
// End of the file
/*@}*/
