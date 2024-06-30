#include <PacManLike/pcworlds.hpp>
#include <PacManLike/pcscenes.hpp>
#include <PacManLike/pcmaps.hpp>
#include <PacManLike/pcgame.hpp>
#include <PacManLike/pcsceneactionblocks.hpp>

// ---
PACMAN::World::World (int c, const QGAMES::Scenes& s, const QGAMES::WorldProperties& p)
	: QGAMES::World (c, s, p),
	  _fullConfPerPlayer (QGAMES::SetOfOpenValues (__QGAMES_RUNTIMEVALUESELMNTTYPE__))
{
#ifndef NDEBUG
	for (auto i : scenes ())
		assert (dynamic_cast <PACMAN::Scene*> (i.second) != nullptr);
#endif
}

// ---		
QGAMES::SetOfOpenValues PACMAN::World::runtimeValues () const
{ 
	const PACMAN::Game* dG = dynamic_cast <const PACMAN::Game*> (game ());
	assert (dG != nullptr); // Just in case...

	// The actual info for the current player
	// By default if dosn't exist...
	QGAMES::SetOfOpenValues iPlayer (std::string ("PLAYERDATA"));

	// If there is no active scene, 
	// it would mean there is no level active either
	// This happens when the game has just started and worlds are not initialized yet!
	// So no info for any player exists either
	if (activeScene () == nullptr)
	{
		_fullConfPerPlayer.addSetOfOpenValues (dG -> currentPlayer (), iPlayer);

		return (_fullConfPerPlayer);
	}

	// Gets the info already kept of the player
	// If there was any previously it is removed, 
	// because a new one will be inserted later when the info of the level is added...
	if (_fullConfPerPlayer.existSetOfOpenValues (dG -> currentPlayer ()))
	{
		QGAMES::SetOfOpenValues iPlayer = _fullConfPerPlayer.setOfOpenValues (dG -> currentPlayer ());
		assert (iPlayer.name () == std::string ("PLAYERDATA")); // Just in case...

		_fullConfPerPlayer.removeSetOfOpenValues (dG -> currentPlayer ());
	}

	// In pacman very world,scene,map is linked to a level, 
	// so getting the info of the world what we are really getting is the info of the level.
	// Removes also the previous info also of the level if it existed, because the new new one will be used!
	QGAMES::SetOfOpenValues iLevel = currentPlayerRuntimeValues ();
	if (iPlayer.existSetOfOpenValues (dG -> level ()))
		iPlayer.removeSetOfOpenValues (dG -> level ());

	// Adds the info of the level into the information of the player...
	iPlayer.addSetOfOpenValues (dG -> level (), iLevel);
	// ...and also adds the information of the player into the información of the full game...
	_fullConfPerPlayer.addSetOfOpenValues (dG -> currentPlayer (), iPlayer);

	// ...what is finally returned
	return (_fullConfPerPlayer);
}

// ---
void PACMAN::World::setClapperBoard (bool o)
{
	// Only over the active scene if any...
	PACMAN::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PACMAN::Scene*> (activeScene ())) != nullptr)
		scn -> setClapperBoard (o);
}

// ---
bool PACMAN::World::clapperBoard () const
{
	const PACMAN::Scene* scn = dynamic_cast <const PACMAN::Scene*> (activeScene ());
	return ((scn != nullptr) ? scn -> clapperBoard () : false);
}

// ---
void PACMAN::World::startBlinking (QGAMES::bdata bT, int nB)
{
	// Only over the active scene if any...
	PACMAN::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PACMAN::Scene*> (activeScene ())) != nullptr)
		scn -> startBlinking (bT, nB);
}

// ---
bool PACMAN::World::isBlinking () const
{
	// Only over the active scene if any...
	const PACMAN::Scene* scn = dynamic_cast <const PACMAN::Scene*> (activeScene ());
	return ((scn != nullptr) ? scn -> isBlinking () : false);
}

// ---
void PACMAN::World::stopBlinking ()
{
	// Only over the active scene if any...
	PACMAN::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PACMAN::Scene*> (activeScene ())) != nullptr)
		scn -> stopBlinking ();
}

// ---
int PACMAN::World::maxNumberBallsToEat () const
{
	// Only over the active scene if any...
	const PACMAN::Scene* scn = dynamic_cast <const PACMAN::Scene*> (activeScene ());
	return ((scn != nullptr) ? scn -> maxNumberBallsToEat () : 0);
}

// ---
int PACMAN::World::numberBallsEaten () const
{
	// Only over the active scene if any...
	const PACMAN::Scene* scn = dynamic_cast <const PACMAN::Scene*> (activeScene ());
	return ((scn != nullptr) ? scn -> numberBallsEaten () : 0);
}

// ---
std::string PACMAN::World::ballsEatenStatus () const
{
	// Only over the active scene if any...
	const PACMAN::Scene* scn = dynamic_cast <const PACMAN::Scene*> (activeScene ());
	return ((scn != nullptr) ? scn -> ballsEatenStatus () : std::string (__NULL_STRING__));
}

// ---
void PACMAN::World::setBallsEatenStatus (const std::string& st)
{
	// Only over the active scene if any...
	PACMAN::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PACMAN::Scene*> (activeScene ())) != nullptr)
		scn -> setBallsEatenStatus (st);
}

// ---
void PACMAN::World::playSiren (bool f)
{
	// Only over the active scene if any...
	PACMAN::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PACMAN::Scene*> (activeScene ())) != nullptr)
		scn -> playSiren (f);
}

// ---
void PACMAN::World::stopBackgroundSounds ()
{
	// Only over the active scene if any...
	PACMAN::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PACMAN::Scene*> (activeScene ())) != nullptr)
		scn -> stopBackgroundSounds ();
}

// ---
void PACMAN::World::stopAllElements ()
{
	// Only over the active scene if any...
	PACMAN::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PACMAN::Scene*> (activeScene ())) != nullptr)
		scn -> stopAllElements ();
}

// ---
void PACMAN::World::hideAllMonsters ()
{
	// Only over the active scene if any...
	PACMAN::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PACMAN::Scene*> (activeScene ())) != nullptr)
		scn -> hideAllMonsters ();
}

// ---
void PACMAN::World::hideAllThings ()
{
	// Only over the active scene if any...
	PACMAN::Scene* scn = nullptr;
	if ((scn = dynamic_cast <PACMAN::Scene*> (activeScene ())) != nullptr)
		scn -> hideAllThings ();
}

// ---
void PACMAN::World::initialize ()
{
	QGAMES::World::initialize ();

	PACMAN::Game* dG = dynamic_cast <PACMAN::Game*> (game ());
	assert (dG != nullptr);
	PACMAN::Game::Conf* cfg = dynamic_cast <PACMAN::Game::Conf*> (dG -> configuration ());
	assert (cfg != nullptr); // Just in case...

	// Initially no ball has been eaten
	setBallsEatenStatus (std::string (__NULL_STRING__));

	// Get the information kept for that world if it exists in the configuration structure...
	_fullConfPerPlayer = QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__));
	if (cfg -> existConfValueForElement (id ()))
		_fullConfPerPlayer = cfg -> confValueForElement (id ());
	
	// Now gets back the info of the current player and level if it exists, 
	// and initialize the world with it...
	if (_fullConfPerPlayer.existSetOfOpenValues (dG -> currentPlayer ()))
	{
		QGAMES::SetOfOpenValues pInfo = _fullConfPerPlayer.setOfOpenValues (dG -> currentPlayer ());
		assert (pInfo.name () == std::string ("PLAYERDATA"));
		if (pInfo.existSetOfOpenValues (dG -> level ()))
			QGAMES::World::initializeRuntimeValuesFrom (pInfo.setOfOpenValues (dG -> level ()));
	}
}

// ---
void PACMAN::World::finalize ()
{
	QGAMES::World::finalize ();

	// The current status of the game is stored in the variable _fullConfPerPlayer
	runtimeValues ();

	// ...and put it back into the configuration object...
	// using the id of that world as reference (take that into account)
	PACMAN::Game::Conf* cfg = dynamic_cast <PACMAN::Game::Conf*> (game () -> configuration ());
	assert (cfg != nullptr); // Just in case...
	cfg -> setConfValueForElement (id (), _fullConfPerPlayer);
}
