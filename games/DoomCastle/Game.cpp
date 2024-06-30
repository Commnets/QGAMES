#include "Game.hpp"

// ---
DOOMCASTLE::Game::Conf::Conf (int nP, int mL, int iW, int iS)
	: QGAMES::PlatformGame::Conf (nP),
	  _maxLives (mL),
	  _initialWorld (iW), _initialScene (iS),
	  _theMazeModel ()
{ 
	adjustToPlayers (nP); 
}

// ---
void DOOMCASTLE::Game::Conf::adjustToPlayers (int nP)
{
	QGAMES::PlatformGame::Conf::adjustToPlayers (nP);

	for (int i = 1; i <= numberPlayers (); setNumberLives (i++, _maxLives)); // Set the number of lives per player...

	// TODO
	// To set up the things in the maze...
}

// ---
QGAMES::Game::Configuration* DOOMCASTLE::Game::Conf::clone () const
{ 
	Conf* result = new Conf (*this); 
	
	result -> cloneImplementation (this); 

	return (result); 
}

// ---
void DOOMCASTLE::Game::Conf::cfgToStream (std::ostringstream& oS)
{
	QGAMES::PlatformGame::Conf::cfgToStream (oS);

	oS << _theMazeModel.asSetOfOpenValues ();
}

// ---
void DOOMCASTLE::Game::Conf::cfgFromStream (std::istringstream& iS)
{
	QGAMES::PlatformGame::Conf::cfgFromStream (iS);

	QGAMES::SetOfOpenValues oV; 
	iS >> oV;
	_theMazeModel.fromSetOfOpenValues (oV);
}

// ---
void DOOMCASTLE::Game::setShowFPS (bool s)
{
	_showFPS = s;

	QGAMES::AdvancedGameStateControl* gCS = 
		dynamic_cast <QGAMES::AdvancedGameStateControl*> 
			(gameStateBuilder () -> gameState (__DOOMCASTLE_STANDARDCONTROLGAMESTATE__));
	assert (gCS);

	gCS -> setOverlay 
		(_showFPS ? game () -> state (__QGAMES_GAMESTATEFPSNAME__) : NULL); 
}

// ---
void DOOMCASTLE::Game::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __QGAMES_KEYBOARDEVENT__)
	{
		QGAMES::KeyBoardEventData* dt = (QGAMES::KeyBoardEventData*) evnt.data ();
		if (dt -> _internalCode == QGAMES::KeyCode::QGAMES_F1 && !dt -> _on)
			setShowFPS (!_showFPS); 
	}

	QGAMES::PlatformGame::processEvent (evnt);
}

// ---
QGAMES::MapBuilder* DOOMCASTLE::Game::createMapBuilder ()
{
	QGAMES::MapBuilder* result = QGAMES::PlatformGame::createMapBuilder ();
	DOOMCASTLE::TMXMapBuilderAddsOn* aOn = 
		new DOOMCASTLE::TMXMapBuilderAddsOn ((QGAMES::Sprite2DBuilder*) formBuilder ());
	result -> addAddsOn (aOn);

	return (result);
}

// ---
QGAMES::Game::Configuration* DOOMCASTLE::Game::createConfiguration ()
{ 
	return (new Conf 
		(1 /** Default players */, 
		 std::atoi (parameter (__DOOMCASTLE_MAXNUMBEROFLIVESPARAMETER__).c_str ()), /** What defined in external file. */
		 std::atoi (parameter (__DOOMCASTLE_INITIALWORLDPARAMETER__).c_str ()),
		 std::atoi (parameter (__DOOMCASTLE_INITIALSCENEPARAMETER__).c_str ()))); 
}

// ---
std::string DOOMCASTLE::Game::defaultParameter (const std::string& p) const
{
	std::string result (__NULL_STRING__);
	if (p == std::string (__DOOMCASTLE_INITIALWORLDPARAMETER__))
		result = std::to_string (__DOOMCASTLE_INITIALWORLDDEFAULTPARAMETER__);
	else
	if (p == std::string (__DOOMCASTLE_INITIALSCENEPARAMETER__))
		result = std::to_string (__DOOMCASTLE_INITIALSCENEDEFAULTPARAMETER__);
	else
	if (p == std::string (__DOOMCASTLE_SHOWFPSPARAMETER__))
		result = std::string (__DOOMCASTLE_SHOWFPSDEFAULTPARAMETER__);
	else
		result = QGAMES::PlatformGame::defaultParameter (p);
	return (result);
}

// ---
void DOOMCASTLE::Game::initialize ()
{
	if (isInitialized ())
		return; // Only once...

	QGAMES::PlatformGame::initialize ();

	if (!isInitialized ())
		return;

	// TODO

	// Add all game states from the definition file...
	addMainStatesFromBuilder (); 
	// ...and start to play with the main one...
	QGAMES::AdvancedGameStateControl* gCS = 
		dynamic_cast <QGAMES::AdvancedGameStateControl*> 
			(gameStateBuilder () -> gameState (__DOOMCASTLE_STANDARDCONTROLGAMESTATE__));
	assert (gCS);

#ifndef NDEBUG
	setShowFPS (true);
#else
	std::string prm = parameter (std::string (__DOOMCASTLE_SHOWFPSPARAMETER__));
	setShowFPS (QGAMES::toUpper (QGAMES::trim (prm)) == std::string (__YES_STRING__));
#endif

	setState (gCS); // To start...

	observe (inputHandler ());
}

// ---
void DOOMCASTLE::Game::finalize ()
{
	unObserve (inputHandler ());

	QGAMES::PlatformGame::finalize ();
}
