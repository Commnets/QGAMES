#include <DragonwindLike/dwworlds.hpp>
#include <DragonwindLike/dwscenes.hpp>
#include <DragonwindLike/dwgame.hpp>

// ---
DRAGONWIND::World::World (int c, const QGAMES::Scenes& scns, const QGAMES::WorldProperties& prps)
			: QGAMES::PlatformWorld (c, scns, prps),
			  _fullConfPerPlayer ()
{
	for (QGAMES::Scenes::const_iterator i = _scenes.begin (); i != _scenes.end (); i++)
		assert (dynamic_cast <DRAGONWIND::Scene*> ((*i).second));
	// To be sure all are DRAGONWIND scenes...
}

// ---		
QGAMES::SetOfOpenValues DRAGONWIND::World::runtimeValues () const
{ 
	const DRAGONWIND::Game* dG = dynamic_cast <const DRAGONWIND::Game*> (game ());
	assert (dG); // Just in case...

	// Remember that world's runtime values are kept per player
	// Remember also that the runtime info of the characters and entities of a scene are not saved
	// But blocks do, and dragonwind blocks save also in many ocassions entities' info
	// So, e.g, if playing in the scene 2 when the runtime values of the blocks of the scene 1 are got
	// The info of the characters will be the one of the scene 2. This is not consistent
	QGAMES::SetOfOpenValues pOV = currentPlayerRuntimeValues ();

	// If there is no active scene then the info is simply returned
	if (activeScene () == NULL)
	{
		_fullConfPerPlayer.addSetOfOpenValues (dG -> currentPlayer (), pOV);
		return (_fullConfPerPlayer);
	}

	// Get the scenes info of the current runtime values...
	// Only the one for the active scene will be usefull...
	int lNE1 = pOV.lastNestedOpenValueId (); // It is supossed it is the scenes info...
	QGAMES::SetOfOpenValues pyScnOv1 = pOV.setOfOpenValues (lNE1);
	assert (pyScnOv1.name () == std::string (__QGAMES_RUNTIMEVALUESSCENESTYPE__)); // ...but double check just in case...
	
	// If it is the first time the player enters the world, there is nothing else to do...
	// Except removing the info not related with this specific scene!
	if (!_fullConfPerPlayer.existSetOfOpenValues (dG -> currentPlayer ()))
	{
		for (QGAMES::Scenes::const_iterator i = scenes ().begin (); i != scenes ().end (); i++)
		{
			if ((*i).first != activeScene () -> id () &&
				pyScnOv1.existSetOfOpenValues ((*i).first))
				pyScnOv1.removeSetOfOpenValues ((*i).first);
		}
	}

	// Otherwise the info of the previous visits have to be recovered!
	// From the info already saved in _fullConfPlayer!
	else
	{
		QGAMES::SetOfOpenValues pyWOv = _fullConfPerPlayer.setOfOpenValues (dG -> currentPlayer ());
		int lNE2 = pyWOv.lastNestedOpenValueId (); // It is supossed it is the scenes info...
		QGAMES::SetOfOpenValues pyScnOv2 = pyWOv.setOfOpenValues (lNE2);
		assert (pyScnOv2.name () == std::string (__QGAMES_RUNTIMEVALUESSCENESTYPE__)); // ...but double check just in case...

		// The older scenes' info is recovered...
		// ..and the information of the current scene is maintained!
		for (QGAMES::Scenes::const_iterator i = scenes ().begin (); i != scenes ().end (); i++)
		{
			if ((*i).first != activeScene () -> id ())
			{
				if (pyScnOv2.existSetOfOpenValues ((*i).first)) 
					pyScnOv1.addSetOfOpenValues ((*i).first, pyScnOv2.setOfOpenValues ((*i).first)); // Replace...
				else
				if (pyScnOv1.existSetOfOpenValues ((*i).first))
					pyScnOv1.removeSetOfOpenValues ((*i).first); // It is not necessary...
			}
		}
	}

	pOV.addSetOfOpenValues (lNE1, pyScnOv1);

	// Finally creates the info per player, and returns it...
	_fullConfPerPlayer.addSetOfOpenValues (dG -> currentPlayer (), pOV); // Replace the previous one...
	return (_fullConfPerPlayer);
}

// ---
std::string DRAGONWIND::World::name () const
{
	return (existsProperty (__DRAGONWIND_WORLDATTRNAME__) 
		? property (__DRAGONWIND_WORLDATTRNAME__) : __DRAGONWIND_WORLDDEFATTRNAME__);
}

// ---
void DRAGONWIND::World::initialize ()
{
	QGAMES::PlatformWorld::initialize ();

	DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
	assert (dG);
	DRAGONWIND::Game::Conf* cfg = dynamic_cast <DRAGONWIND::Game::Conf*> (dG -> configuration ());
	assert (cfg); // Just in case...

	// Gets back the information kept for that world, if it exists
	// Otherwise an empty element is set up...
	// The world keeps different information per player (id)
	_fullConfPerPlayer = QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__));
	if (cfg -> existConfValueForElement (id ()))
		_fullConfPerPlayer = cfg -> confValueForElement (id ());
	
	// Gets back the specific information of the world for the current player, if it exists
	// That information won't exist if the player hasn't ever played in this world!
	// So, in that situation there won't be anything to configure beyond the work
	// already done at initialization time...
	if (_fullConfPerPlayer.existSetOfOpenValues (dG -> currentPlayer ()))
	{
		// Remember that the characters and entities used in the scene are reused in other and with other players
		// On the other hand, the world is initialized any time a scene starts back
		// So the information kept must be used to initialize just the current scene forgetting the rest...
		QGAMES::SetOfOpenValues pyWOv = _fullConfPerPlayer.setOfOpenValues (dG -> currentPlayer ());
		int lNE = pyWOv.lastNestedOpenValueId (); // It is supossed it is the scenes info...
		QGAMES::SetOfOpenValues pyScnOv = pyWOv.setOfOpenValues (lNE);
		assert (pyScnOv.name () == std::string (__QGAMES_RUNTIMEVALUESSCENESTYPE__)); // ...but double check just in case...

		for (QGAMES::Scenes::const_iterator i = scenes ().begin (); i != scenes ().end (); i++)
			if ((*i).first != activeScene () -> id () && pyScnOv.existSetOfOpenValues ((*i).first))
				pyScnOv.removeSetOfOpenValues ((*i).first); 

		pyWOv.addSetOfOpenValues (lNE, pyScnOv);
		
		// The initialization then takes place...
		QGAMES::PlatformWorld::initializeRuntimeValuesFrom (pyWOv); 
	}
}

// ---
void DRAGONWIND::World::finalize ()
{
	// The current status of the game is stored...
	runtimeValues ();

	// ...and put it back into the configuration object...
	DRAGONWIND::Game::Conf* cfg = dynamic_cast <DRAGONWIND::Game::Conf*> (game () -> configuration ());
	assert (cfg); // Just in case...
	cfg -> setConfValueForElement (id (), _fullConfPerPlayer);

	QGAMES::PlatformWorld::finalize ();
}
