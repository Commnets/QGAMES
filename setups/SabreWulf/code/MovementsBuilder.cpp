#include "MovementsBuilder.hpp"
#include "Movements.hpp"
#include "Defs.hpp"

// ---
QGAMES::Movement* MovementsBuilder::createMovement 
	(const QGAMES::MovementBuilder::MovementDefinition& def)
{ 
	QGAMES::Movement* result = NULL;
	if (def._id == __SABREWULFSABREMANMOVS__)
		result = new SabreWulfArtistMovement (def._id, def._variables);
	else if (def._id >= __SABREWULFMONSTERBASEMOV__ && def._id < (__SABREWULFMONSTERBASEMOV__ + __SABREWULFMAXNUMBERNASTIES__))
		result = new SabreWulfArtistMovement (def._id, def._variables);
	else if (def._id == __SABREWULFFIREMOVS__)
		result = new SabreWulfArtistMovement (def._id, def._variables);
	else if (def._id == __SABREWULFINMORTALMOV__)
		result = new SabreWulfArtistMovement (def._id, def._variables);
	else
			exit (1); // If the code is here...wrong!!
	return (result);
}
