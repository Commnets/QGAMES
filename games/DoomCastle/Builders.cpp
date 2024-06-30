#include "Builders.hpp"

int DOOMCASTLE::ToKnowTileFrameInformation::_defaultTileFramesRepresentingSomethingToCatch [] =
	{ 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 50 };

// ---
QGAMES::Entity* DOOMCASTLE::EntityBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	QGAMES::Entity* result = nullptr;

	QGAMES::Entity::Data dt;
	dt._capacities = def._capacities;
	dt._physics = def._physics;
	dt._parameters = def._parameters;

	result = QGAMES::PlatformEntityBuilder::createEntity (def);

	return (result);
}

// ---
QGAMES::CompositeEntity* DOOMCASTLE::EntityBuilder::createCompositeEntity 
	(const QGAMES::EntityBuilder::EntityDefinition& def)
{
	QGAMES::CompositeEntity* result = nullptr;

	QGAMES::Entities eties = entities (def._entities);
	QGAMES::Entities etiesClone;
	for (QGAMES::Entities::const_iterator i = eties.begin (); i != eties.end (); i++)
		etiesClone [(*i).first] = (*i).second -> clone ();

	QGAMES::Entity::Data dt;
	dt._capacities = def._capacities;
	dt._physics = def._physics;
	dt._parameters = def._parameters;

	result = QGAMES::PlatformEntityBuilder::createCompositeEntity (def);

	return (result);
}

// ---
QGAMES::SomethingToEat* DOOMCASTLE::EntityBuilder::createSomethingToEat (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	// TODO

	return (nullptr);
}

// ---
QGAMES::SomethingToCatch* DOOMCASTLE::EntityBuilder::createSomethingToCatch 
	(const QGAMES::EntityBuilder::EntityDefinition& def)
{
	// TODO

	return (nullptr);
}

// ---
QGAMES::BadGuy* DOOMCASTLE::EntityBuilder::createBadGuy (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	// TODO

	return (nullptr);
}

// ---
QGAMES::Movement* DOOMCASTLE::MovementBuilder::createMovement 
	(const QGAMES::MovementBuilder::MovementDefinition& def)
{
	QGAMES::Movement* result = nullptr;

	result = QGAMES::AdvancedMovementBuilder::createMovement (def);

	return (result);
}

// ---
DOOMCASTLE::ToKnowTileFrameInformation::ToKnowTileFrameInformation ()
	: QGAMES::ToKnowTileFrameInformation ()
{ 
	// Rewrite what is done in the parent class
	_thingsToCatchLocationTileFrames =
		std::vector <int> (_defaultTileFramesRepresentingSomethingToCatch, 
			_defaultTileFramesRepresentingSomethingToCatch + 
				(sizeof (_defaultTileFramesRepresentingSomethingToCatch) / sizeof (int))); 
}

// ---
QGAMES::World* DOOMCASTLE::WorldBuilder::createWorldObject (int no, const QGAMES::Scenes& s, 
			const QGAMES::WorldProperties& p)
{
	// TODO

	__WLOGPLUS (std::string ("DOOMCASTLE World: ") + 
		std::to_string (no) + std::string (" not defined"));
	__WLOGPLUS ("Going for the default builder...");

	return (QGAMES::PlatformWorldBuilder::createWorldObject (no, s, p));
}

// ---
QGAMES::Scene* DOOMCASTLE::WorldBuilder::createSceneObject (int ns, const QGAMES::Maps& m, 
			const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
{
	// TODO

	__WLOGPLUS (std::string ("DOOMCASTLE Scene: ") + 
		std::to_string (ns) + std::string (" not defined"));
	__WLOGPLUS ("Going for the deafult builder...");

	return (QGAMES::PlatformWorldBuilder::createSceneObject (ns, m, cn, p, ePL));
}

// ---
QGAMES::SceneActionBlock* DOOMCASTLE::WorldBuilder::createSceneActionBlockObject (int nAB, 
			const QGAMES::SceneActionBlockProperties& prps)
{
	QGAMES::SceneActionBlock* result = nullptr;

	// TODO

	__WLOGPLUS (std::string ("DOOMCASTLE Action Block: ") + 
		std::to_string (nAB) + std::string (" not defined"));
	__WLOGPLUS ("Going for the default builder...");

	return (QGAMES::PlatformWorldBuilder::createSceneActionBlockObject (nAB, prps));
}
