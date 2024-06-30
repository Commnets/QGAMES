#include <Board/boardresourcesgrouploader.hpp>
#include <Board/boardbuilder.hpp>
#include <Board/boardgameapp.hpp>

// ---
QGAMES::BoardLibResourceGroupLoader::TypeBoardLoader::TypeBoardLoader (const std::string& type, QGAMES::BoardBuilder* bBld)
	: QGAMES::ResourcesGroupLoader::TypeLoader (type),
	  _boardBuilder (bBld)
{
	assert (_boardBuilder);
	// Just check. It can't be NULL at all.
}

// ---
void QGAMES::BoardLibResourceGroupLoader::TypeBoardLoader::load (const std::vector <int>& rscs)
{
	for (int i = 0; i < (int) rscs.size (); i++)
		_boardBuilder -> board (rscs [i]); // Just invoke the method to load it...
}

// ---
QGAMES::BoardLibResourceGroupLoader::StdTypeBoardLoader::StdTypeBoardLoader ()
	: QGAMES::BoardLibResourceGroupLoader::TypeBoardLoader 
		(__QGAMES_STDTYPEBOARDSLOADER__, (__BGM QGAMES::Game::game ()) -> boardBuilder ())
{
	// Nothing else to do...
}

// ---
QGAMES::BoardLibResourceGroupLoader::BoardLibResourceGroupLoader (const std::string& fDef)
	: QGAMES::CommonLibResourceGroupLoader (fDef)
{
	addLoader (new QGAMES::BoardLibResourceGroupLoader::StdTypeBoardLoader ());
}
