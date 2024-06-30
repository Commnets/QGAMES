#include <AdvancedArcade/adgame.hpp>
#include <AdvancedArcade/adgamestate.hpp>
#include <AdvancedArcade/adbuilders.hpp>
#include <SDL/sdlgame.hpp>

// ---
QGAMES::AdvancedArcadeGame::Conf::Conf (int nP)
	: QGAMES::Game::StdConfiguration (),
	  _numberPlayers (nP),
	  _joystick (false),
	  _difficulty (0),
	  _music (true),
	  _keys (),
	  _hallOfFame (),
	  _currentWorld (),
	  _currentScene (),
	  _currentInitPoint (),
	  _lastPlayerPosition (),
	  _numberLives (),
	  _sceneVisited ()
{
	assert (_numberPlayers > 0);

	// The default keys...
	_keys.resize (7);
	_keys [__QGAMES_KEYLEFT__]	= QGAMES::KeyCode::QGAMES_A; // Left
	_keys [__QGAMES_KEYRIGHT__] = QGAMES::KeyCode::QGAMES_S; // Right
	_keys [__QGAMES_KEYUP__]	= QGAMES::KeyCode::QGAMES_P; // Up
	_keys [__QGAMES_KEYDOWN__]	= QGAMES::KeyCode::QGAMES_L; // Down
	_keys [__QGAMES_KEYFIRE__]	= QGAMES::KeyCode::QGAMES_SPACE; // Fire
	_keys [__QGAMES_KEYPAUSE__] = QGAMES::KeyCode::QGAMES_Z; // Pause
	_keys [__QGAMES_KEYSAVE__]	= QGAMES::KeyCode::QGAMES_X; // Save

	adjustToPlayers (_numberPlayers);
}

// ---
std::vector <std::string> QGAMES::AdvancedArcadeGame::Conf::confFileNames () const
{
	std::vector <std::string> result;
	std::vector <std::string> tmp = QGAMES::listOfFiles (game () -> parameter (__GAME_DATADIRPROPERTYNAME__));
	for (int i = 0; i < (int) tmp.size (); i++)
		if (QGAMES::fileExtension (tmp [i]) == std::string ("cfg"))
			result.push_back (tmp [i].substr (0, tmp [i].length () - 4));
	return (result);
}

// ---
void QGAMES::AdvancedArcadeGame::Conf::adjustToPlayers (int nP)
{
	// Hall of fame hasn't to be reset
	// This method is invokd any time the main menu is invoked or the number of players changed
	// resetting the hall of fame would mean to lose the list of winners!
	// _hallOfFame = std::multimap <std::string, QGAMES::SetOfOpenValues> (); // The previous valus are not valid...
	
	_currentWorld = std::vector <int> (nP, 0); // It should be adjusted later for the specific game...
	_currentScene = std::vector <int> (nP, 0);
	_currentInitPoint = std::vector <std::map <int, int>> (nP, std::map <int, int> ());
	_lastPlayerPosition = std::vector <QGAMES::Position> (nP, QGAMES::Position::_cero);
	_numberLives = std::vector <int> (nP, 0);
	_sceneVisited = std::vector <std::map <int, bool>> (nP, std::map <int, bool> ());
}

// ---
void QGAMES::AdvancedArcadeGame::Conf::cfgToStream (std::ostringstream& oS)
{
	QGAMES::Game::StdConfiguration::cfgToStream (oS);

	oS << _numberPlayers << std::endl;
	oS << _joystick << std::endl;
	oS << _difficulty << std::endl;
	oS << _music << std::endl;
	oS << (int) _keys.size () << std::endl;
	for (int i = 0; i < (int) _keys.size (); i++)
		oS << _keys [i] << std::endl;

	oS << (int) _hallOfFame.size () << std::endl; 
	for (std::map <std::string, QGAMES::SetOfOpenValues>::const_iterator i = _hallOfFame.begin (); 
		i != _hallOfFame.end (); i++)
		oS << (*i).first << std::endl << (*i).second << std::endl;

	for (int i = 0; i < _numberPlayers; i++)
		oS << _currentWorld [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _currentScene [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
	{
		oS << _currentInitPoint [i].size () << std::endl;
		for (std::map <int, int>::const_iterator j = _currentInitPoint [i].begin ();
				j != _currentInitPoint [i].end (); j++)
			oS << (*j).first << std::endl << (*j).second << std::endl;
	}

	for (int i = 0; i < _numberPlayers; i++)
		oS << _lastPlayerPosition [i] << std::endl; // Every value of the point separated by commas...
	for (int i = 0; i < _numberPlayers; i++)
		oS << _numberLives [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
	{
		oS << _sceneVisited [i].size () << std::endl;
		for (std::map <int, bool>::const_iterator j = _sceneVisited [i].begin (); 
				j != _sceneVisited [i].end (); j++)
			oS << (*j).first << std::endl << (*j).second << std::endl;
	}
}

// ---
void QGAMES::AdvancedArcadeGame::Conf::cfgFromStream (std::istringstream& iS)
{
	QGAMES::Game::StdConfiguration::cfgFromStream (iS);

	iS >> _numberPlayers;
	iS >> _joystick;
	iS >> _difficulty;
	iS >> _music;
	int nK; iS >> nK; _keys.resize (nK);
	for (int i = 0; i < (int) _keys.size (); i++)
		iS >> _keys [i];

	_hallOfFame = { };
	int nH; iS >> nH; 
	for (int i = 0; i < nH; i++)
	{
		std::string hK; iS >> hK;
		QGAMES::SetOfOpenValues hV; iS >> hV;
		_hallOfFame.insert (std::multimap <std::string, QGAMES::SetOfOpenValues>::value_type (hK, hV));
	} 

	_currentWorld.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _currentWorld [i];
	_currentScene.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _currentScene [i];

	_currentInitPoint.resize (_numberPlayers);
	int nS, sId, nIP;
	for (int i = 0; i < _numberPlayers; i++)
	{
		iS >> nS;
		for (int j = 0; j < nS; j++)
		{
			iS >> sId; iS >> nIP;
			_currentInitPoint [i][sId] = nIP;
		}
	}

	_lastPlayerPosition.resize (_numberPlayers);
	std::string lPP;
	for (int i = 0; i < _numberPlayers; i++)
	{
		iS >> lPP;
		std::vector <QGAMES::bdata> lPPB = QGAMES::getElementsFromAsBDATA (lPP, ',', 3);
		_lastPlayerPosition [i] = QGAMES::Vector (lPPB [0], lPPB [1], lPPB [2]);
	}

	_numberLives.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _numberLives [i];
	
	_sceneVisited.resize (_numberPlayers);
	int nE, nSV; bool sV;
	for (int i = 0; i < _numberPlayers; i++)
	{
		iS >> nE;
		for (int j = 0; j < nE; j++)
		{
			iS >> nSV; iS >> sV;
			_sceneVisited [i][nSV] = sV;
		}
	}
}

// ---
QGAMES::AdvancedArcadeGame::AdvancedArcadeGame (QGAMES::GameImplementation* impl)
		: QGAMES::ArcadeGame (impl != NULL ? impl : new SDLGame (false /** With no FMOD */), QGAMES::Worlds ()),
		  _currentPlayer (1), // First player first...
		  _numberOfSeconds (0),
		  _copyOfConfiguration (NULL)
{ 
	createLoopCounter (_COUNTERSECONDS, QGAMES::Game::game () -> framesPerSecond ()); 
}

// ---
void QGAMES::AdvancedArcadeGame::setConfiguration (QGAMES::Game::Configuration* cfg)
{
	QGAMES::ArcadeGame::setConfiguration (cfg);

	assert (configuration ());

	QGAMES::AdvancedArcadeGame::Conf* aCfg = dynamic_cast <QGAMES::AdvancedArcadeGame::Conf*> (cfg);
	if (!aCfg)
		return;

	QGAMES::InputHandler::StandardBehavior* sB = 
		dynamic_cast <QGAMES::InputHandler::StandardBehavior*> (inputHandler () -> behaviour ());
	if (sB && !aCfg -> controlKeys ().empty ())
		sB -> setKeys (aCfg -> controlKeys ());
	sB -> activateJoystick (aCfg -> useJoystick ());
}

// ---
int QGAMES::AdvancedArcadeGame::loadConfiguration (const std::string& cN)
{
	assert (configuration ());

	// Builds up the name of the configuration file
	std::string fN = parameter (__GAME_DATADIRPROPERTYNAME__) + std::string (__PATH_SEPARATOR__) + cN;

	// When loading a new configuration object, a copy of this is also kept
	// This copy is maintained until the configuration is saved.
	// Only at that point the modifications done aver the "official" 
	// configuration object are consolidated
	int error = configuration () -> load (fN);
	if (error == 0)
	{ 
		delete (_copyOfConfiguration); _copyOfConfiguration = NULL;
		_copyOfConfiguration = 
			(QGAMES::AdvancedArcadeGame::Conf*) game () -> configuration () -> clone ();

		// Plays a sound to indicate, everything has been done...
		sound (__QGAMES_SAVEWAVSOUND__) -> play (__QGAMES_GAMESOUNDCHANNEL__);
	}

	return (error);
}

// ---
int QGAMES::AdvancedArcadeGame::saveConfiguration (const std::string& cN)
{
	assert (configuration ());

	// Builds up the name of the configuration file
	std::string fN = parameter (__GAME_DATADIRPROPERTYNAME__) + std::string (__PATH_SEPARATOR__) + cN;

	// When a configuration object is saved the modifications done over it
	// are then consolidated. So at that point no turn back is possible!
	int error = configuration () -> save (fN);
	if (error == 0)
	{
		delete (_copyOfConfiguration); _copyOfConfiguration = NULL;
		_copyOfConfiguration = 
			(QGAMES::AdvancedArcadeGame::Conf*) game () -> configuration () -> clone ();

		// Plays a sound to indicate, everything has been done...
		sound (__QGAMES_SAVEWAVSOUND__) -> play (__QGAMES_GAMESOUNDCHANNEL__);
	}

	return (error);
}

// ---
void QGAMES::AdvancedArcadeGame::setCurrentPlayer (int nP)
{
	QGAMES::AdvancedArcadeGame::Conf* cfg =
		dynamic_cast <QGAMES::AdvancedArcadeGame::Conf*> (configuration ());
	if (!cfg)
		return; // It is not of the type needed here...

	_currentPlayer = nP;
	if (_currentPlayer < 1) _currentPlayer = 1;
	if (_currentPlayer > cfg -> numberPlayers ()) 
		_currentPlayer = cfg -> numberPlayers ();
}

// ---
void QGAMES::AdvancedArcadeGame::setNextPlayer ()
{
	QGAMES::AdvancedArcadeGame::Conf* cfg =
		dynamic_cast <QGAMES::AdvancedArcadeGame::Conf*> (configuration ());
	if (!cfg)
		return; // It is not of the type needed here...

	setCurrentPlayer ((_currentPlayer < cfg -> numberPlayers ()) ? _currentPlayer + 1 : 1);
}

// ---
void QGAMES::AdvancedArcadeGame::pauseGame ()
{
	// If the game is controlled by a control game state the action is one,
	// and other totally different when it is not.
	if (dynamic_cast <QGAMES::AdvancedGameStateControl*> (activeState ()))
		((QGAMES::AdvancedGameStateControl*) activeState ()) -> stop ();
	else
		activeState () -> setNestedState (state (std::string (__QGAMES_GAMESTATESHOWENTITYNAME__)));

	QGAMES::ArcadeGame::pauseGame ();
	QGAMES::SoundSystem::system () -> pause (); 
}

// ---
void QGAMES::AdvancedArcadeGame::continueGame ()
{
	// If the game is controlled by a control game state the action is one,
	// and other totally different when it is not.
	if (dynamic_cast <QGAMES::AdvancedGameStateControl*> (activeState ()))
		((QGAMES::AdvancedGameStateControl*) activeState ()) -> resume ();
	else
		activeState () -> setNestedState (NULL);

	QGAMES::SoundSystem::system () -> resume (); 
	QGAMES::ArcadeGame::continueGame ();
}

// ---
bool QGAMES::AdvancedArcadeGame::drawGameBeforeState () const
{
	bool result = false;
	if (dynamic_cast <const QGAMES::AdvancedGameStateControl*> (activeState ()))
		result = ((QGAMES::AdvancedGameStateControl*) activeState ()) -> isStopped ();
	return (result);
}

// ---
void QGAMES::AdvancedArcadeGame::processEvent (const QGAMES::Event& e)
{
	if (e.code () == __QGAMES_LOOPCOUNTEREVENT__)
		setSeconds (seconds () + 1);
	else
		QGAMES::ArcadeGame::processEvent (e);
}

// ---
QGAMES::GameStateBuilder* QGAMES::AdvancedArcadeGame::createGameStateBuilder ()
{
	QGAMES::GameStateBuilder* result = QGAMES::ArcadeGame::createGameStateBuilder ();
	result -> addAddsOn (new QGAMES::ForAdvancedGameStatesAddon (result));
	result -> addAddsOn (new QGAMES::ForCreditTitlesGameStateAddOn (result));
	result -> addAddsOn (new QGAMES::ForShowingInstructionsGameStateAddOn (result));
	result -> addAddsOn (new QGAMES::ForRollingStatesGameStateAddOn (result));
	result -> addAddsOn (new QGAMES::ForAdvancedGameStateControlAddon (result));
	return (result);
}

// ---
QGAMES::TextBuilder* QGAMES::AdvancedArcadeGame::createTextBuilder ()
{ 
	return (new AdvancedTextBuilder ());
}

// ---
QGAMES::EntityBuilder* QGAMES::AdvancedArcadeGame::createEntityBuilder ()
{ 
	return (new QGAMES::AdvancedEntityBuilder (parameter (__GAME_PROPERTYENTITIESFILE__), 
		formBuilder (), movementBuilder ())); 
}

// ---
QGAMES::GUISystemBuilder* QGAMES::AdvancedArcadeGame::createGUISystemBuilder ()
{
	return (new QGAMES::AdvancedGUISystemBuilder (parameter (__GAME_PROPERTYGUISYSTEMFILE__), formBuilder ())); 
}

// ---
QGAMES::MovementBuilder* QGAMES::AdvancedArcadeGame::createMovementBuilder () 
{ 
	return (new QGAMES::AdvancedMovementBuilder (parameter (__GAME_PROPERTYMOVEMENTSFILE__))); 
}

// ---
QGAMES::Screens QGAMES::AdvancedArcadeGame::createScreens ()
{ 
	QGAMES::Screens r;
	QGAMES::Screen* scr = implementation () -> createScreen (
		parameter (__QGAMES_GAMENAMEPARAMETER__),
		QGAMES::Position (0,0), 
		std::stoi (parameter (__QGAMES_GAMESCREENWIDTHPARAMETER__)), 
		std::stoi (parameter (__QGAMES_GAMESCREENHEIGHTPARAMETER__)), 
		std::stoi (parameter (__QGAMES_GAMESCREENXPOSPARAMETER__)), 
		std::stoi (parameter (__QGAMES_GAMESCREENYPOSPARAMETER__))); 
	assert (scr); 
	scr -> windowAtCenter (); 
	r.insert (QGAMES::Screens::value_type (__QGAMES_MAINSCREEN__, scr));
	return (r);
}

// ---
std::string QGAMES::AdvancedArcadeGame::defaultParameter (const std::string& p) const
{
	std::string result (__NULL_STRING__);

	if (p == std::string (__QGAMES_GAMENAMEPARAMETER__)) 
		result = std::string (__QGAMES_DEFAULTGAMENAME__);
	else if (p == std::string (__QGAMES_GAMEICONPARAMETER__)) 
		result = std::to_string (__QGAMES_DEFAULTGAMEICON__);
	else if (p == std::string (__QGAMES_GAMESCREENXPOSPARAMETER__))
		result = std::string (std::to_string (__QGAMES_DEFAULTSCREENXPOSITION__));
	else if (p == std::string (__QGAMES_GAMESCREENYPOSPARAMETER__))
		result = std::string (std::to_string (__QGAMES_DEFAULTSCREENYPOSITION__));
	else if (p == std::string (__QGAMES_GAMESCREENWIDTHPARAMETER__))
		result = std::string (std::to_string (__QGAMES_DEFAULTSCREENWIDTH__));
	else if (p == std::string (__QGAMES_GAMESCREENHEIGHTPARAMETER__))
		result = std::string (std::to_string (__QGAMES_DEFAULTSCREENHEIGHT__));

	return ((result == std::string (__NULL_STRING__)) 
		? QGAMES::ArcadeGame::defaultParameter (p) : result);
}

// ---
void QGAMES::AdvancedArcadeGame::initialize ()
{
	if (isInitialized ())
		return;

	QGAMES::ArcadeGame::initialize ();

	if (!isInitialized ())
		return;

	mainScreen () -> setIcon (formBuilder () -> 
		form (std::atoi (parameter (__QGAMES_GAMEICONPARAMETER__).c_str ())));

	addState (new QGAMES::ExitGameGameState, std::string (__QGAMES_EXITGAMEGAMESTATENAME__));
	addState (new QGAMES::ShowingFrameRateGameState, std::string (__QGAMES_GAMESTATEFPSNAME__));
	addState (new QGAMES::PlayingASoundGameState, std::string (__QGAMES_GAMESTATEPLAYSOUNDNAME__));
	addState (new QGAMES::ShowingFixFormGameState, std::string (__QGAMES_GAMESTATESHOWFIXFORMNAME__));
	addState (new QGAMES::ShowingAppearingFormGameState, std::string (__QGAMES_GAMESTATESHOWFORMNAME__));
	addState (new QGAMES::ShowingEntityGameState, std::string (__QGAMES_GAMESTATESHOWENTITYNAME__));
	addState (new QGAMES::LoadingGameState, std::string (__QGAMES_GAMESTATELOADINGNAME__));
	addState (new QGAMES::CountingDownGameState, std::string (__QGAMES_GAMESTATECOUNTINGDOWNNAME__));
	addState (new QGAMES::ShowingHallOfFameGameState, std::string (__QGAMES_GAMESTATEHALLOFFAMENAME__));
	addState (new QGAMES::RollingFormsGameState, std::string (__QGAMES_GAMESTATEROLLINGFRMSNAME__));
	addState (new QGAMES::CreditTitlesGameState, std::string (__QGAMES_GAMESTATECREDITTITLESNAME__));
	addState (new QGAMES::StandardInitialSelectionOptionsGameState, std::string (__QGAMES_GAMESTATESELECTOPTIONSNAME__));

	delete (_copyOfConfiguration); // Not needed, just in case!
	_copyOfConfiguration = (QGAMES::AdvancedArcadeGame::Conf*) configuration () -> clone ();

	_numberOfSeconds = 0;

	// Don't forget to add the worlds, scenes, etc...to the elements to keep in the configuration
}

// ---
void QGAMES::AdvancedArcadeGame::inEveryLoop ()
{
	if (configuration () != NULL) 
		((QGAMES::AdvancedArcadeGame::Conf*) configuration ()) -> inEveryLoop ();
	QGAMES::ArcadeGame::inEveryLoop ();
}
