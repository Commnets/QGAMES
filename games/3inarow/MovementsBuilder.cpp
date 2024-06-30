#include "MovementsBuilder.hpp"
#include "Movements.hpp"
#include "Defs.hpp"

// ---
QGAMES::Movement* MovementsBuilder::createMovement 
	(const QGAMES::MovementBuilder::MovementDefinition& def)
{ 
	QGAMES::Movement* result = NULL;
	if (def._id == __MOVEMENTENTITYANIMATING__)
		result = new MovementAnimating (def._variables);
	else
		exit (1); // Big error...
	return (result);
}
