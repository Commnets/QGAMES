#include <Platform/ptscenes.hpp>
#include <Platform/ptbuilders.hpp>
#include <Platform/ptartists.hpp>

// ---
QGAMES::PlatformScene::PlatformScene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
	: QGAMES::SceneWithBackground (c, m, cn, p, ePL),
	  _exitDoors (),
	  _numberOfThingsToEatAdded (0), _numberOfThingsToCatchAdded (0), _numberOfBadGuysAdded (0),
	  _characteresAdded (),
	  _badGuys ()
{
	for (QGAMES::Maps::const_iterator i = _maps.begin (); i != _maps.end (); i++)
		assert (dynamic_cast <QGAMES::PlatformMap*> ((*i).second) || dynamic_cast <QGAMES::BackgroundMap*> ((*i).second));
	// To be sure all are playing or background maps...
}

// ---
void QGAMES::PlatformScene::setDoorOpen (int id, bool s)
{
	assert (_exitDoors.find (id) != _exitDoors.end ());

	(*_exitDoors.find (id)).second -> _state = (s == true) ? 1 : 0;
}

// ---
bool QGAMES::PlatformScene::doorOpen (int id) const
{
	assert (_exitDoors.find (id) != _exitDoors.end ());

	return ((*_exitDoors.find (id)).second -> _state != 0);
}

// ---
void QGAMES::PlatformScene::initialize ()
{
	// The map should have been mapped before reaching this point...
	assert (activeMap ());

	// Gets all things to eat, to catch and bad guys definition from the game configuration...
	// This is a temporal copy. Once the playing scene finishes, the current state will be copied back into the 
	// game configuration.
	QGAMES::PlatformGame* g = dynamic_cast <QGAMES::PlatformGame*> (game ());
	assert (g); // It has to be...

	// Add the characters that are something to eat...
	_numberOfThingsToEatAdded = 0;
	QGAMES::PlatformGame::ThingsToEatDefinition thingsToEat = g -> thingsToEatFor (id ()); // For this scene...
	int nE = (int) thingsToEat.size ();
	assert (nE <= __QGAMES_PTNUMBERSOMETHINGTOEAT__); // Just in case...
	for (QGAMES::PlatformGame::ThingsToEatDefinition::const_iterator i = thingsToEat.begin ();
			i != thingsToEat.end (); i++)
	{
		QGAMES::SomethingToEat* sE = dynamic_cast <QGAMES::SomethingToEat*> 
			(g -> character (__QGAMES_PTSOMETHINGTOEATBASEID__ + _numberOfThingsToEatAdded++));
		assert (sE); // Just in case...
		addCharacter (sE);
		sE -> setFromDefinition ((*i).second);
		sE -> setMap (activeMap ()); 

		sE -> setVisible (true);

		_characteresAdded [sE -> id ()] = sE; // To simplify unplug
	}

	// Add the characters that are something to catch...
	_numberOfThingsToCatchAdded = 0;
	QGAMES::PlatformGame::ThingsToCatchDefinition thingsToCatch = g -> thingsToCatchFor (id ());
	int nC = (int) thingsToCatch.size ();
	assert (nC <= __QGAMES_PTNUMBERSOMETHINGTOCATCH__); // Just in case...
	for (QGAMES::PlatformGame::ThingsToCatchDefinition::const_iterator i = thingsToCatch.begin ();
			i != thingsToCatch.end (); i++)
	{
		QGAMES::SomethingToCatch* sC = dynamic_cast <QGAMES::SomethingToCatch*> 
			(g -> character (__QGAMES_PTSOMETHINGTOCATCHBASEID__ + _numberOfThingsToCatchAdded++));
		assert (sC); // Just in case...
		addCharacter (sC);
		sC -> setFromDefinition ((*i).second);
		sC -> setMap (activeMap ());

		sC -> setVisible (true);

		_characteresAdded [sC -> id ()] = sC; // To simplify unplug
	}

	// Add the characters that are a bad guy...
	_numberOfBadGuysAdded = 0;
	_badGuys.clear (); // The scene is not the owner of the bad guys...
	QGAMES::PlatformGame::BadGuysDefinition badGuys = g -> badGuysFor (id ());
	int nB = (int) badGuys.size ();
	assert (nB <= __QGAMES_PTNUMBERBADGUY__); //( Just in case...
	for (QGAMES::PlatformGame::BadGuysDefinition::const_iterator i = badGuys.begin ();
			i != badGuys.end (); i++)
	{
		QGAMES::BadGuy* bG = dynamic_cast <QGAMES::BadGuy*> 
			(g -> character (__QGAMES_PTBADGUYBASEID__ + _numberOfBadGuysAdded++));
		assert (bG); // Just in case...
		addCharacter (bG);
		bG -> setFromDefinition ((*i).second);
		bG -> setMap (activeMap ()); 

		bG -> setVisible (true);

		_badGuys [bG -> id ()] = bG; // Just to be used maybe later...

		_characteresAdded [bG -> id ()] = bG; // To simplify unplug
	}

	// Load the exit doors...
	_exitDoors = g -> exitDoorsFor (id ()); 

	// Execute the default initialization, to init all members...
	QGAMES::SceneWithBackground::initialize ();
}

// ---
void QGAMES::PlatformScene::finalize ()
{
	// First off all finishes everything...
	QGAMES::SceneWithBackground::finalize ();

	// Remove the characteres used...(they were added when initializing)
	for (QGAMES::Entities::iterator i = _characteresAdded.begin (); i != _characteresAdded.end (); i++)
	{
		(*i).second -> setVisible (false); // But first, no longer visible...
		removeCharacter (dynamic_cast <QGAMES::Character*> ((*i).second));
	}

	_characteresAdded.clear (); // Just in case...
}

// ---
QGAMES::PlatformGame::ThingsToEatDefinition QGAMES::PlatformScene::thingsToEatDefinition () const
{
	const QGAMES::PlatformGame* pG = dynamic_cast <const QGAMES::PlatformGame*> (game ());
	const QGAMES::PlatformMap* m = dynamic_cast <const QGAMES::PlatformMap*> (activeMap ());
	const QGAMES::ToKnowTileFrameInformation* a = pG -> tileFrameInformation ();
	assert (m && a);

	QGAMES::PlatformGame::ThingsToEatDefinition result;
	QGAMES::PlatformMap::ObjectLocations locs = m -> positionsForObject (a -> _thingsToEatLocationTileFrames);
	for (int i = 0; i < (int) locs.size (); i++)
	{
		QGAMES::PlatformGame::ThingToEatDefinition* eT = createThingToEatDefinitionFrom (locs [i]); 
		assert (eT); // Just in case...
		result [eT -> _id] = eT;
	}

	return (result);
}

// ---
QGAMES::PlatformGame::ThingsToCatchDefinition QGAMES::PlatformScene::thingsToCatchDefinition () const
{
	const QGAMES::PlatformGame* pG = dynamic_cast <const QGAMES::PlatformGame*> (game ());
	const QGAMES::PlatformMap* m = dynamic_cast <const QGAMES::PlatformMap*> (activeMap ());
	const QGAMES::ToKnowTileFrameInformation* a = pG -> tileFrameInformation ();
	assert (m && a);

	QGAMES::PlatformGame::ThingsToCatchDefinition result;
	QGAMES::PlatformMap::ObjectLocations locs = m -> positionsForObject (a -> _thingsToCatchLocationTileFrames);
	for (int i = 0; i < (int) locs.size (); i++)
	{
		QGAMES::PlatformGame::ThingToCatchDefinition* cT = createThingToCatchDefinitionFrom (locs [i]); 
		assert (cT); // Just in case...
		result [cT -> _id] = cT;
	}

	return (result);
}

// ---
QGAMES::PlatformGame::BadGuysDefinition QGAMES::PlatformScene::badGuysDefinition () const
{
	const QGAMES::PlatformGame* pG = dynamic_cast <const QGAMES::PlatformGame*> (game ());
	const QGAMES::PlatformMap* m = dynamic_cast <const QGAMES::PlatformMap*> (activeMap ());
	const QGAMES::ToKnowTileFrameInformation* a = pG -> tileFrameInformation ();
	assert (m && a);

	QGAMES::PlatformGame::BadGuysDefinition result;
	QGAMES::PlatformMap::ObjectLocations locs = m -> positionsForObject (a -> _badGuysLocationTileFrames);
	for (int i = 0; i < (int) locs.size (); i++)
	{
		QGAMES::PlatformGame::BadGuyDefinition* bG = createBadGuyDefinitionFrom (locs [i]);
		assert (bG); // Just in case...
		bG -> _otherValues.addSetOfOpenValues (0, locs [i]._parameters); // If any...
		result [bG -> _id] = bG;
	}

	return (result);
}

// ---
QGAMES::PlatformGame::ExitDoorsDefinition QGAMES::PlatformScene::exitDoorsDefinition () const 
{
	const QGAMES::PlatformGame* pG = dynamic_cast <const QGAMES::PlatformGame*> (game ());
	const QGAMES::PlatformMap* m = dynamic_cast <const QGAMES::PlatformMap*> (activeMap ());
	const QGAMES::ToKnowTileFrameInformation* a = pG -> tileFrameInformation ();
	assert (m && a);

	QGAMES::PlatformGame::ExitDoorsDefinition result;
	QGAMES::PlatformMap::NotificationBlockLocations blks = 
		m -> positionsForNotificationBlock (a -> _doorsNotificationBlockTileFrames);
	for (int i = 0; i < (int) blks.size (); i++)
	{
		QGAMES::PlatformGame::ExitDoorDefinition* eD = createExitDoorDefinitionFrom (blks [i]);
		assert (eD); // Just in case
		result [eD -> _id]= eD;
	}

	return (result);
}

// ---
QGAMES::PlatformMap::ObjectLocations QGAMES::PlatformScene::positionsForObject (int nO) const
{
	const QGAMES::PlatformMap* m = dynamic_cast <const QGAMES::PlatformMap*> (activeMap ());
	assert (m);

	return (m -> positionsForObject (nO));
}

// ---
QGAMES::PlatformMap::ObjectLocations QGAMES::PlatformScene::positionsForObject (const std::vector <int>& nO) const
{
	const QGAMES::PlatformMap* m = dynamic_cast <const QGAMES::PlatformMap*> (activeMap ());
	assert (m);

	return (m -> positionsForObject (nO));
}

// ---
QGAMES::PlatformMap::NotificationBlockLocations QGAMES::PlatformScene::positionsForNotificationBlock (int nO) const
{
	const QGAMES::PlatformMap* m = dynamic_cast <const QGAMES::PlatformMap*> (activeMap ());
	assert (m);

	return (m -> positionsForNotificationBlock (nO));
}

// ---
QGAMES::PlatformMap::NotificationBlockLocations QGAMES::PlatformScene::positionsForNotificationBlock (const std::vector <int>& nO) const
{
	const QGAMES::PlatformMap* m = dynamic_cast <const QGAMES::PlatformMap*> (activeMap ());
	assert (m);

	return (m -> positionsForNotificationBlock (nO));
}

// ---
QGAMES::PlatformGame::ThingToEatDefinition* QGAMES::PlatformScene::createThingToEatDefinitionFrom 
	(const QGAMES::PlatformMap::ObjectLocation& obj) const
{
	const QGAMES::PlatformGame::Conf* cfg = 
		dynamic_cast <const QGAMES::PlatformGame::Conf*> (game () -> configuration ());
	assert (cfg);

	QGAMES::PlatformGame::ThingToEatDefinition* result = cfg -> createEmptyThingToEatDefinition ();
	result -> _id = game () -> uniqueId ();
	result -> _type = obj._numberObject;
	result -> _worldId = world () -> id ();
	result -> _sceneId = id ();
	QGAMES::Positions pos; pos.push_back (obj._position);
	result -> _positions = pos;
	// Not other values needed...
	// Note that the state is not set, so -1 is by default...

	return (result);
}

// ---
QGAMES::PlatformGame::ThingToCatchDefinition* QGAMES::PlatformScene::createThingToCatchDefinitionFrom 
	(const QGAMES::PlatformMap::ObjectLocation& obj) const
{
	const QGAMES::PlatformGame::Conf* cfg = 
		dynamic_cast <const QGAMES::PlatformGame::Conf*> (game () -> configuration ());
	assert (cfg);

	QGAMES::PlatformGame::ThingToCatchDefinition* result = cfg -> createEmptyThingToCatchDefinition ();
	result -> _id = game () -> uniqueId ();
	result -> _type = obj._numberObject;
	result -> _worldId = world () -> id ();
	result -> _sceneId = id ();
	QGAMES::Positions pos; pos.push_back (obj._position);
	result -> _positions = pos;
	// Not other values needed...
	// Note that the state is not set, so -1 is by default...

	return (result);
}

// ---
QGAMES::PlatformGame::BadGuyDefinition* QGAMES::PlatformScene::createBadGuyDefinitionFrom 
	(const QGAMES::PlatformMap::ObjectLocation& obj) const 
{
	const QGAMES::PlatformGame::Conf* cfg = 
		dynamic_cast <const QGAMES::PlatformGame::Conf*> (game () -> configuration ());
	assert (cfg);

	QGAMES::PlatformGame::BadGuyDefinition* result = cfg -> createEmptyBadGuyDefinition ();
	result -> _id = game () -> uniqueId ();
	result -> _type = obj._numberObject;
	result -> _worldId = world () -> id ();
	result -> _sceneId = id ();
	QGAMES::Positions pos; pos.push_back (obj._position);
	result -> _positions = pos;
	// Not other values needed...
	// Note that the state is not set, so -1 is by default...

	return (result);
}

// ---
QGAMES::PlatformGame::ExitDoorDefinition* QGAMES::PlatformScene::createExitDoorDefinitionFrom 
	(const QGAMES::PlatformMap::NotificationBlockLocation& blk) const
{
	const QGAMES::PlatformGame::Conf* cfg = dynamic_cast <const QGAMES::PlatformGame::Conf*> (game () -> configuration ());
	assert (cfg);

	QGAMES::PlatformGame::ExitDoorDefinition* result = cfg -> createEmptyDoorDefinition ();
	result -> _id = game () -> uniqueId ();
	result -> _type = blk._numberNotification;
	result -> _worldId = world () -> id ();
	result -> _sceneId = id ();
	result -> _positions = blk._positions;
	// Not other values needed...

	return (result);
}
