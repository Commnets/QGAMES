/** \addtogroup 3InARow */
/*@{*/

#ifndef __MOVEMENTS_HPP__
#define __MOVEMENTS_HPP__

#include "Defs.hpp"
#include <Board/boardinclude.hpp>

/** Describe the movement of the entities moving across the board
	leaving thing to be eaten by the players during the game. */
class MovementAnimating : public QGAMES::Movement
{
	public:
	MovementAnimating (std::map <int, double> v)
		: QGAMES::Movement (__MOVEMENTENTITYANIMATING__, v)
							{ }

	virtual Movement* clone () const;
	virtual QGAMES::Vector direction () const;
	virtual QGAMES::Vector acceleration () const
							{ return (QGAMES::Vector::_cero); }

	// Another methods to control the movement when iut is initiated
	// or when it is finished, can be added here...
	virtual void move (const QGAMES::Vector& d, const QGAMES::Vector& a, 
		QGAMES::Entity* e);
};

// Another movements can be added here...

#endif
  
// End of the file
/*@}*/
