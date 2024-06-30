/** \addtogroup SabreWulf */
/*@{*/

#ifndef __MOVEMENTS_HPP__
#define __MOVEMENTS_HPP__

#include "Defs.hpp"
#include <Arcade/arcadeinclude.hpp>

/** The movement of sabreman and any other entity in the maze. */
class SabreWulfArtistMovement : public QGAMES::MovementOneByOne
{
	public:
	SabreWulfArtistMovement (int id, std::map <int, double> v)
		: QGAMES::MovementOneByOne (id, v)
							{ }

	virtual Movement* clone () const;

	virtual void initialize ();

	virtual QGAMES::Vector direction () const
							{ return (QGAMES::Vector::_cero); }
	virtual QGAMES::Vector acceleration () const
							{ return (QGAMES::Vector::_cero); }

	private:
	virtual void moveOne (const QGAMES::Vector& d, const QGAMES::Vector& a, 
			QGAMES::Entity* e);
};

#endif
  
// End of the file
/*@}*/
