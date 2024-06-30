#include "Movements.hpp"

// ---
QGAMES::Movement* SabreWulfArtistMovement::clone () const
{
	QGAMES::Movement* result = new SabreWulfArtistMovement (_id, _variables); 
	result -> initialize (); 
	return (result); 
}

// ---
void SabreWulfArtistMovement::initialize ()
{
	QGAMES::MovementOneByOne::initialize ();

	// Faster than none at the begging...
	// Other speeds are i.e double when the yellow orchild affects him...
	_speed = 4 * _baseSpeed;
}

// ---
void SabreWulfArtistMovement::moveOne (const QGAMES::Vector& d, const QGAMES::Vector& a, 
			QGAMES::Entity* e)
{
	e -> setPosition (e -> position () + d);
	e -> setOrientation (d);
}
