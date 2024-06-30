#include <Arcade/arcaderesourcesgrouploader.hpp>
#include <Arcade/mapbuilder.hpp>
#include <Arcade/worldbuilder.hpp>
#include <Arcade/arcadegame.hpp>

// ---
QGAMES::ArcadeLibResourceGroupLoader::TypeMapLoader::TypeMapLoader (const std::string& type, QGAMES::MapBuilder* mBld)
	: QGAMES::ResourcesGroupLoader::TypeLoader (type),
	  _mapBuilder (mBld)
{
	assert (_mapBuilder);
	// Just check. It can't be NULL at all.
}

// ---
void QGAMES::ArcadeLibResourceGroupLoader::TypeMapLoader::load (const std::vector <int>& rscs)
{
	for (int i = 0; i < (int) rscs.size (); i++)
		_mapBuilder -> map (rscs [i]); // Just invoke the method to load it...
}

// ---
QGAMES::ArcadeLibResourceGroupLoader::StdTypeMapLoader::StdTypeMapLoader ()
	: QGAMES::ArcadeLibResourceGroupLoader::TypeMapLoader 
		(__QGAMES_STDTYPEMAPSLOADER__, (__AGM QGAMES::Game::game ()) -> mapBuilder ())
{
	// Nothing else to do...
}

// ---
QGAMES::ArcadeLibResourceGroupLoader::TypeWorldLoader::TypeWorldLoader (const std::string& type, QGAMES::WorldBuilder* wBld)
	: QGAMES::ResourcesGroupLoader::TypeLoader (type),
	  _worldBuilder (wBld)
{
	assert (_worldBuilder);
	// Just check. It can't be NULL at all.
}

// ---
void QGAMES::ArcadeLibResourceGroupLoader::TypeWorldLoader::load (const std::vector <int>& rscs)
{
	for (int i = 0; i < (int) rscs.size (); i++)
		_worldBuilder -> world (rscs [i]); // Just invoke the method to load it...
}

// ---
QGAMES::ArcadeLibResourceGroupLoader::StdTypeWorldLoader::StdTypeWorldLoader ()
	: QGAMES::ArcadeLibResourceGroupLoader::TypeWorldLoader 
		(__QGAMES_STDTYPEWORLDSLOADER__, (__AGM QGAMES::Game::game ()) -> worldBuilder ())
{
	// Nothing else to do...
}

// ---
QGAMES::ArcadeLibResourceGroupLoader::ArcadeLibResourceGroupLoader (const std::string& fDef)
	: QGAMES::CommonLibResourceGroupLoader (fDef)
{
	addLoader (new QGAMES::ArcadeLibResourceGroupLoader::StdTypeMapLoader ());
	addLoader (new QGAMES::ArcadeLibResourceGroupLoader::StdTypeWorldLoader ());
}
