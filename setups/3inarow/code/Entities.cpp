#include "Entities.hpp"

// ---
QGAMES::Entity* EntityAnimating3InARow::clone () const
{ 
	return (new EntityAnimating3InARow (_forms, _data)); 
}
