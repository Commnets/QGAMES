#include "Game.hpp"

// ---
void DragonTheRevenge::Game::Conf::adjustToPlayers (int nP)
{
	DRAGONWIND::Game::Conf::adjustToPlayers (nP);
	for (int i = 1; i <= numberPlayers (); setNumberLives (i++, _maxLives)); // Set the number of lives per player...
}

// ---
void DragonTheRevenge::Game::setShowFPS (bool s)
{
	_showFPS = s;

	QGAMES::AdvancedGameStateControl* gCS = 
		dynamic_cast <QGAMES::AdvancedGameStateControl*> 
			(gameStateBuilder () -> gameState (__DRAGONWIND_STANDARDCONTROLGAMESTATE__));
	assert (gCS);

	gCS -> setOverlay 
		(_showFPS ? game () -> state (__QGAMES_GAMESTATEFPSNAME__) : NULL); 
}

// ---
void DragonTheRevenge::Game::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __QGAMES_KEYBOARDEVENT__)
	{
		QGAMES::KeyBoardEventData* dt = (QGAMES::KeyBoardEventData*) evnt.data ();
		if (dt -> _internalCode == QGAMES::KeyCode::QGAMES_F1 && !dt -> _on)
			setShowFPS (!_showFPS); 
	}

	DRAGONWIND::Game::processEvent (evnt);
}

// ---
QGAMES::MapBuilder* DragonTheRevenge::Game::createMapBuilder ()
{
	QGAMES::MapBuilder* result = QGAMES::PlatformGame::createMapBuilder ();
	DragonTheRevenge::TMXMapBuilder* aOn = 
		new DragonTheRevenge::TMXMapBuilder ((QGAMES::Sprite2DBuilder*) formBuilder ());
	result -> addAddsOn (aOn);

	return (result);
}

// ---
QGAMES::Game::Configuration* DragonTheRevenge::Game::createConfiguration ()
{ 
	return (new Conf 
		(1 /** Default players */, std::atoi (parameter (__DRAGONWINDTHEREVENGE_MAXNUMBEROFLIVESPARAMETER__).c_str ()))); 
}

// ---
std::string DragonTheRevenge::Game::defaultParameter (const std::string& p) const
{
	std::string result (__NULL_STRING__);
	if (p == std::string (__DRAGONWINDTHEREVENGE_MAXNUMBEROFLIVESPARAMETER__))
		result = std::string (__DRAGONWINDTHEREVENGE_DEFAULTMAXNUMBEROFLIVESPARAMETER__);
	else
	if (p == std::string (__DRAGONWINDTHEREVENGE_SHOWFPSPARAMETER__))
		result = std::string (__DRAGONWINDTHEREVENGE_DEFAULTSHOWFPSPARAMETER__);
	else
		result = DRAGONWIND::Game::defaultParameter (p);
	return (result);
}

// ---
void DragonTheRevenge::Game::initialize ()
{
	if (isInitialized ())
		return; // Only once...

	DRAGONWIND::Game::initialize ();

	if (!isInitialized ())
		return;

	// The standard builders do everything related with the screen...
	// In terms of position, size, name and even icon to represent it...

	// The entities and world has to be initialized using the method
	// "initializeWorldsAndEntities" or using the gameState "InitializeDragonWindGameState"
	// In the example the second option has been choosen

	// Add all game states from the definition file...
	addMainStatesFromBuilder (); 
	// ...and start to play with the main one...
	QGAMES::AdvancedGameStateControl* gCS = 
		dynamic_cast <QGAMES::AdvancedGameStateControl*> 
			(gameStateBuilder () -> gameState (__DRAGONWIND_STANDARDCONTROLGAMESTATE__));
	assert (gCS);

#ifndef NDEBUG
	setShowFPS (true);
#else
	std::string sfps = parameter (std::string (__DRAGONWINDTHEREVENGE_SHOWFPSPARAMETER__));
	setShowFPS (QGAMES::toUpper (QGAMES::trim (sfps)) == std::string (__YES_STRING__));
#endif

	setState (gCS); // To start...

	observe (inputHandler ());
}

// ---
void DragonTheRevenge::Game::finalize ()
{
	unObserve (inputHandler ());

	DRAGONWIND::Game::finalize ();
}
