#include "EntitiesBuilder.hpp"
#include "Entities.hpp"
#include "Defs.hpp"

// ---
QGAMES::Entity* EntitiesBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	if (def._id == __ENTITYANIMATING3INAROW__)
		return (new EntityAnimating3InARow ());
	else
		exit (1);
}
