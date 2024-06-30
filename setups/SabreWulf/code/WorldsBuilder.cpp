#include "WorldsBuilder.hpp"
#include "Worlds.hpp"
#include "Scenes.hpp"
#include "Defs.hpp"

// ---
QGAMES::World* WorldsBuilder::createWorldObject (int no, const QGAMES::Scenes& s, 
	const QGAMES::WorldProperties& p)
{
	QGAMES::World* result = NULL;
	switch (no)
	{
		case __SABREWULFWORLD__:
			result = new SabreWulfWorld (s, p);
			break;

		default:
			exit (1); // No others worls are availables...
			break;
	}

	return (result);
}

// ---
QGAMES::Scene* WorldsBuilder::createSceneObject (int ns, const QGAMES::Maps& m, 
	const QGAMES::Scene::Connections& cn, const QGAMES::SceneProperties& p, 
	const QGAMES::EntitiesPerLayer& ePL)
{
	QGAMES::Scene* result = NULL;
	switch (ns)
	{
		case __SABREWULFSCENE__:
			result = new SabreWulfScene (m, cn, p, ePL);
			break;

		default:
			exit (1); // Any other is not allowed...
			break;
	}

	return (result);
}
