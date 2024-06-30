#include <Arcade/arcadegame.hpp>
#include <Arcade/artist.hpp>
#include <iostream>

// ---
void QGAMES::ArcadeGame::addWorld (int nW)
{
	assert (_worldBuilder);
	assert (_worlds.find (nW) == _worlds.end ()); // Doesn't added before...

	_worlds.insert (QGAMES::Worlds::value_type (nW, _worldBuilder -> world (nW)));
}

// ---
bool QGAMES::ArcadeGame::existWorld (int nW) const
{
	return (_worlds.find (nW) != _worlds.end ());
}

// ---
QGAMES::World* QGAMES::ArcadeGame::world (int nW)
{
	QGAMES::Worlds::const_iterator i = _worlds.find (nW);
	assert (i != _worlds.end ());

	return ((*i).second);
}

// ---
bool QGAMES::ArcadeGame::existWorldForScene (int scnId)
{
	bool result = false;
	for (QGAMES::Worlds::const_iterator i = _worlds.begin (); i != _worlds.end () && !result; i++)
		if ((*i).second -> existScene (scnId))
			result = true;
	return (result);
}

// ---
QGAMES::World* QGAMES::ArcadeGame::worldForScene (int scnId)
{
	QGAMES::World* result = NULL;
	for (QGAMES::Worlds::const_iterator i = _worlds.begin (); i != _worlds.end () && !result; i++)
		if ((*i).second -> existScene (scnId))
			result = (*i).second;

	// The answer has to exist, otherwise it will be a mistake!.
	assert (result);
	return (result);
}

// ---
void QGAMES::ArcadeGame::nextWorldAndScene (int oWrldId, int oScnId, int cp, int& tWrldId, int& tScnId, int& tCp)
{
	if (isConncetionInfoFor (oScnId, cp) && 
		existWorldForScene (oScnId) && worldForScene (oScnId) -> id () == oWrldId)
		connectionInfoFor (oScnId, cp, tWrldId, tScnId, tCp);
	else
	{
		tWrldId = tScnId = -1;
		tCp = 0; // It doesn't really matter...
	}
}

// ---
void QGAMES::ArcadeGame::setWorld (int nW)
{
	// __MININT__ means none

	if (nW == __MININT__)
	{
		if (_activeWorld != NULL)
		{
			unObserve (_activeWorld);
			_activeWorld = NULL;
		}
	}
	else
	{
		QGAMES::Worlds::const_iterator i = _worlds.find (nW);
		assert (i != _worlds.end ());
		if (_activeWorld != NULL)
			unObserve (_activeWorld);
		_activeWorld = (*i).second;
		observe (_activeWorld);
	}
}

// ---
bool QGAMES::ArcadeGame::isConncetionInfoFor (int oScnId, int cp)
{
	bool result = false;
	QGAMES::World* oWrld = worldForScene (oScnId);
	if (oWrld)
	{
		QGAMES::Scene* oScn = oWrld -> scene (oScnId);
		if (oScn)
			result = oScn -> existsConnection (cp);
	}

	return (result);
}

// ---
void QGAMES::ArcadeGame::connectionInfoFor (int oScnId, int cp, int& wrlId, int& tScnId, int& tCp)
{
	QGAMES::World* oWrld = worldForScene (oScnId);
	QGAMES::Scene* oScn = oWrld -> scene (oScnId);
	QGAMES::Scene::Connection cnt = oScn -> connection (cp);
	tScnId = cnt._toScene -> id ();
	wrlId = worldForScene (tScnId) -> id ();
	tCp = cnt._toPoint;
}

// ---
QGAMES::Entity* QGAMES::ArcadeGame::artist (int nA)
{
	QGAMES::Entities::const_iterator i = _artists.find (nA);
	assert (i != _artists.end ()); // If it doesn't exist...exit the game...

	return ((*i).second);
}

// ---
void QGAMES::ArcadeGame::addArtist (int nE)
{
	// Check whether the element is or not already in the list...
	QGAMES::Entities::const_iterator i = _artists.find (nE);
	// If not, then it is added, only if it is an artist...
	// This method is usually invoke many times...
	if (i == _artists.end ())
	{
		QGAMES::Entity* e = entity (nE);
		assert ((dynamic_cast <QGAMES::Artist*> (e)) != NULL); 
		_artists.insert (QGAMES::Entities::value_type (nE, e));
	}
}

// ---
QGAMES::ArcadeGame::~ArcadeGame ()
{
	delete (_mapBuilder);
	_mapBuilder = NULL;
	delete (_worldBuilder);
	_worldBuilder = NULL;
}

// ---
void QGAMES::ArcadeGame::drawOn (QGAMES::Screen* s)
{
	assert (s);

	if (_activeWorld && _activeWorld -> isVisible ())
		_activeWorld -> drawOn (s); // The parts are drawn where they are defined...

	QGAMES::Game::drawOn (s);
}

// ---
std::string QGAMES::ArcadeGame::defaultParameter (const std::string& p) const
{
	std::string result (__NULL_STRING__);

	if (p == std::string (__GAME_PROPERTYWORLDSFILE__))
		result = std::string (__GAME_WORLDSDEFINITIONSTDFILE__);
	else if (p == std::string (__GAME_MAPSOBJECTSFILE__))
		result = std::string (__GAME_MAPSDEFINITIONSTDFILE__);
	else 
		result = QGAMES::Game::defaultParameter (p); // Looks at the parent class...

	return (result);

}

// ---
void QGAMES::ArcadeGame::initialize ()
{
	if (isInitialized ())
		return; // Just once...

	QGAMES::Game::initialize ();

	if (!isInitialized ())
		return; // Nothing else to initialized if the basic initialization was wrong!

	(_preLoad) ? mapBuilder () -> preLoad () : mapBuilder ();
	(_preLoad) ? worldBuilder () -> preLoad () : worldBuilder ();
}

// ---
void QGAMES::ArcadeGame::inEveryLoop ()
{
	QGAMES::Game::inEveryLoop ();

	if (_activeWorld)
		_activeWorld -> inEveryLoop ();
}

// ---
void QGAMES::ArcadeGame::updatePositions ()
{
	QGAMES::Game::updatePositions ();

	if (_activeWorld)
		_activeWorld -> updatePositions ();
}

// ---
void QGAMES::ArcadeGame::detectCollisions ()
{
	QGAMES::ArcadeGame::SetOfPairOfEntities eties;
	pairOfVisibleArtists (eties);

	for (QGAMES::ArcadeGame::SetOfPairOfEntities::const_iterator i = eties.begin (); i != eties.end (); i++)
	{
		if ((*i)._a -> hasCollisionWith ((*i)._b))
		{
			(*i)._a -> whenCollisionWith ((*i)._b);
			(*i)._b -> whenCollisionWith ((*i)._a);
		}
	}
}

// ---
void QGAMES::ArcadeGame::pairOfVisibleArtists (QGAMES::ArcadeGame::SetOfPairOfEntities& p)
{
	QGAMES::Entity *a, *b;
	for (QGAMES::Entities::const_iterator i = _artists.begin (); i != _artists.end (); i++)
		for (QGAMES::Entities::const_iterator j = std::next (i, 1); j != _artists.end (); j++)
			if ((a = (*i).second) -> isVisible () && (b = (*j).second) -> isVisible ())
				p.push_back (QGAMES::ArcadeGame::PairOfEntities (a, b));
}

// ---
void QGAMES::ArcadeGame::pairOfVisibleEntitiesInTheScene (QGAMES::ArcadeGame::SetOfPairOfEntities& p)
{
	QGAMES::World* wrld = activeWorld ();
	if (!wrld) return;
	QGAMES::Scene* scn = wrld -> activeScene ();
	if (!scn) return;

	QGAMES::Entity *a, *b;
	QGAMES::Entities eties = scn -> entities ();
	eties.insert (scn -> characters ().begin (), scn -> characters ().end ());
	for (QGAMES::Entities::const_iterator i = eties.begin (); i != eties.end (); i++)
		for (QGAMES::Entities::const_iterator j = std::next (i, 1); j != eties.end (); j++)
			if ((a = (*i).second) -> isVisible () && (b = (*j).second) -> isVisible ())
				p.push_back (QGAMES::ArcadeGame::PairOfEntities (a, b));
}
