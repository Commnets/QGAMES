#include "Movements.hpp"

// ---
QGAMES::Movement* MovementAnimating::clone () const
{
	QGAMES::Movement* result = new MovementAnimating (_variables); 
	result -> initialize (); 
	return (result); 
}

// ---
QGAMES::Vector MovementAnimating::direction () const
{ 
	return (QGAMES::Vector::_cero); 
}

// ---
void MovementAnimating::move (const QGAMES::Vector& d, const QGAMES::Vector& a, 
		QGAMES::Entity* e)
{
}
