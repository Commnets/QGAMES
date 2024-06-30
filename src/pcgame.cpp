#include <PacManLike/pcgame.hpp>
#include <PacManLike/pcworlds.hpp>
#include <PacManLike/pcscoreobjects.hpp>

// ---
PACMAN::Game::Conf::Conf (int nP, int mL, int dL)
	: QGAMES::AdvancedArcadeGame::Conf (nP),
	  _maxLives (mL),
	  _defaultLevel (dL),
	  _playerName (),
	  _score (), 
	  _playerSeconds (),
	  _level (),
	  _levelCompleted (),
	  _triesOnLevel (),
	  _fruitsEaten ()
{ 
	assert (_numberPlayers > 0);
	assert (_maxLives >= 1);
	assert (_defaultLevel > 0);

	adjustToPlayers (_numberPlayers);
}

// ---
void PACMAN::Game::Conf::actualizeHallOfFame ()
{
	int iS = (int) _hallOfFame.size ();
	for (int i = 0; i < _numberPlayers; i++)
	{
		QGAMES::OpenValues vals; 
		vals.insert (QGAMES::OpenValues::value_type (0, QGAMES::OpenValue (_score [i])));
		vals.insert (QGAMES::OpenValues::value_type (1, QGAMES::OpenValue (_playerName [i])));
		_hallOfFame.insert (std::multimap <std::string, QGAMES::SetOfOpenValues>::value_type 
			(std::to_string (i + iS), QGAMES::SetOfOpenValues (vals)));
	}
}

// ---
void PACMAN::Game::Conf::adjustToPlayers (int nP)
{
	QGAMES::AdvancedArcadeGame::Conf::adjustToPlayers (nP);

	// Readjust the number of lives
	_numberLives = std::vector <int> (nP, _maxLives);

	_playerName = std::vector <std::string> (nP, std::string (__NULL_STRING__));
	_score = std::vector <int> (nP, 0);
	_playerSeconds = std::vector <int> (nP, 0);
	_level = std::vector <int> (nP, _defaultLevel); 
	_levelCompleted = std::vector <std::map <int, bool>> (nP, std::map <int, bool> ());
	_triesOnLevel = std::vector <std::map <int, int>> (nP, std::map <int, int> ());
	_fruitsEaten = std::vector <std::map <int, bool>> (nP, std::map <int, bool> ());

	// When the players are adjusted, the information kept in the worlds have to be deleted to...
	// Remember that this information is kept by player, and...
	for (QGAMES::Worlds::const_iterator i = (__AGM game ()) -> worlds ().begin (); 
			i != (__AGM game ()) -> worlds ().end (); i++)
	{
		((PACMAN::World*) (*i).second) -> initializeRuntimeValuesFrom (QGAMES::SetOfOpenValues ());
		if (existConfValueForElement ((*i).second -> id ())) // Just if the element exists
			setConfValueForElement ((*i).second -> id (), QGAMES::SetOfOpenValues ());
	}
}

// ---
void PACMAN::Game::Conf::cfgToStream (std::ostringstream& oS)
{
	QGAMES::AdvancedArcadeGame::Conf::cfgToStream (oS);

	for (int i = 0; i < _numberPlayers; i++)
		oS << _playerName [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _score [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _playerSeconds [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _level [i] << std::endl;
	
	for (int i = 0; i < _numberPlayers; i++)
	{
		oS << _levelCompleted [i].size () << std::endl;
		for (std::map <int, bool>::const_iterator j = _levelCompleted [i].begin (); 
				j != _levelCompleted [i].end (); j++)
			oS << (*j).first << std::endl << (*j).second << std::endl;
	}
	
	for (int i = 0; i < _numberPlayers; i++)
	{
		oS << _triesOnLevel [i].size () << std::endl;
		for (std::map <int, int>::const_iterator j = _triesOnLevel [i].begin (); 
				j != _triesOnLevel [i].end (); j++)
			oS << (*j).first << std::endl << (*j).second << std::endl;
	}
	
	for (int i = 0; i < _numberPlayers; i++)
	{
		oS << _fruitsEaten [i].size () << std::endl;
		for (std::map <int, bool>::const_iterator j = _fruitsEaten [i].begin (); 
				j != _fruitsEaten [i].end (); j++)
			oS << (*j).first << std::endl << (*j).second << std::endl;
	}
}

// ---
void PACMAN::Game::Conf::cfgFromStream (std::istringstream& iS)
{
	QGAMES::AdvancedArcadeGame::Conf::cfgFromStream (iS);

	_playerName.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _playerName [i];
	_score.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _score [i];
	_playerSeconds.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _playerSeconds [i];
	_level.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _level [i];
	
	_levelCompleted.resize (_numberPlayers);
	int nE, nSC; bool sC;
	for (int i = 0; i < _numberPlayers; i++)
	{
		iS >> nE;
		for (int j = 0; j < nE; j++)
		{
			iS >> nSC; iS >> sC;
			_levelCompleted [i][nSC] = sC;
		}
	}
	
	_triesOnLevel.resize (_numberPlayers);
	int nT;
	for (int i = 0; i < _numberPlayers; i++)
	{
		iS >> nE;
		for (int j = 0; j < nE; j++)
		{
			iS >> nSC; iS >> nT;
			_triesOnLevel [i][nSC] = nT;
		}
	}
	
	_fruitsEaten.resize (_numberPlayers);
	bool fE;
	for (int i = 0; i < _numberPlayers; i++)
	{
		iS >> nE;
		for (int j = 0; j < nE; j++)
		{
			iS >> nSC; iS >> fE;
			_fruitsEaten [i][nSC] = fE;
		}
	}
}

// ---
PACMAN::Game::~Game ()
{ 
	removeScoreObjects ();

	for (auto i : _theDataGames)
		delete (i);
	_theDataGames = { };
}

// ---
const PACMAN::DataGame& PACMAN::Game::dataGame () const
{ 
	PACMAN::Game::Conf* cfg = dynamic_cast <PACMAN::Game::Conf*> (game () -> configuration ());
	assert (cfg != nullptr); // It should, but just in case...
	return ((cfg -> difficulty () > (int) _theDataGames.size ()) 
		? *_theDataGames [(int) _theDataGames.size () - 1] : *_theDataGames [cfg -> difficulty ()]);
}

// ---
void PACMAN::Game::setLevel (int l, int nP)
{
	((PACMAN::Game::Conf*) configuration ()) -> setLevel ((nP == -1) ? currentPlayer () : nP, l);

	setWorld (dataGame ().levelDefinition (level (nP)).worldTypeId ());
	activeWorld () -> setScene (dataGame ().levelDefinition (level (nP)).sceneTypeId ());
	activeWorld () -> activeScene () -> setMap (dataGame ().levelDefinition (level (nP)).mapTypeId ());
	activeWorld () -> initialize ();

	setCurrentWorld (activeWorld () -> id (), nP);
}

// ---
int PACMAN::Game::intermissionScreenAfterCurrentLevel () const
{
	return (dataGame ().levelDefinition (level ()).intermissionAfterEnd ());
}

// ---
bool PACMAN::Game::isCurrentLevelLast () const
{
	return (level () == dataGame ().numberLevelsDefined ());
}

// ---
void PACMAN::Game::setShowFPS (bool s)
{
	_showFPS = s;

	QGAMES::AdvancedGameStateControl* gCS = 
		dynamic_cast <QGAMES::AdvancedGameStateControl*> 
			(gameStateBuilder () -> gameState (__PACMAN_STANDARDCONTROLGAMESTATE__));
	assert (gCS != nullptr);

	gCS -> setOverlay 
		(_showFPS ? game () -> state (__QGAMES_GAMESTATEFPSNAME__) : nullptr); 
}

// ---
void PACMAN::Game::addScoreObjects ()
{
	if (!_lives) 
	{
		addScoreObject (_lives = new PACMAN::Lives ()); 
		_lives -> setLives (lives ());
		_lives -> setPosition (
			QGAMES::Position (__BD 10, 
							  __BD (mainScreen () -> visualHeight () - 10 - _lives -> visualHeight ()), 
							  __BD 0));

		_lives -> observe (pacman ());
	}

	if (!_timeCounter)
	{
		addScoreObject (_timeCounter = new PACMAN::TimeCounter ());
		_timeCounter -> setSeconds (0); // Starting...
		_timeCounter -> setPosition (QGAMES::Position (__BD 10, __BD 10, __BD 0));
		// Any time the time changes, the time counter is actualized...
	}

	if (!_scoreCounter) 
	{
		addScoreObject (_scoreCounter = new PACMAN::ScoreCounter ());
		_scoreCounter -> setNumber (score ());
		_scoreCounter -> setPosition (
			QGAMES::Position (__BD (mainScreen () -> visualWidth () - _scoreCounter -> visualLength () - 20), 
							  __BD 10, 
							  __BD 0));

		_scoreCounter -> observe (pacman ()); // Only for the one playing
	}

	if (!_playerName)
	{
		addScoreObject (_playerName = new PACMAN::FixText (0, playerName ())); // First fixed text
		_playerName -> setPosition (QGAMES::Position (
			_lives -> position ().posX (),
			_lives -> position ().posY () - __BD (_playerName -> visualHeight () + 10),
			__BD 0));
		_playerName -> setAlphaLevel (100); // A little bit faded, because it is on the playing zone!
	}

	if (!_fruitsEaten)
	{
		addScoreObject (_fruitsEaten = new PACMAN::FruitsEaten ());
		_fruitsEaten -> setFruitsEaten (std::vector <int> ()); // Nothing
		_fruitsEaten -> setPosition (_lives -> position () + QGAMES::Vector (__BD 250, __BD 0, __BD 0));

		_fruitsEaten -> observe (pacman ()); // Only for the one playing
	}
}

// ---
void PACMAN::Game::removeScoreObjects ()
{
	PACMAN::PacMan* pc = pacman ();
	assert (pc != nullptr); // It has to exist at this point...

	if (_lives != nullptr)
	{
		assert (_lives -> isObserving (pc));
		_lives -> unObserve (pc); 
		delete (_lives); _lives = nullptr; 
	}
	
	delete (_timeCounter); _timeCounter = nullptr;
	
	if (_scoreCounter != nullptr)
	{
		assert (_scoreCounter -> isObserving (pc));
		_scoreCounter -> unObserve (pc); 
		delete (_scoreCounter); _scoreCounter = nullptr; 
	}

	delete (_playerName); _playerName = nullptr;

	if (_fruitsEaten != nullptr)
	{
		assert (_fruitsEaten -> isObserving (pc));
		_fruitsEaten -> unObserve (pc); 
		delete (_fruitsEaten); _fruitsEaten = nullptr; 
	}

	QGAMES::AdvancedArcadeGame::removeScoreObjects ();
}

// ---
void PACMAN::Game::pauseGame ()
{
	QGAMES::AdvancedArcadeGame::pauseGame ();

	// Stop the seconds...
	loopCounter (_COUNTERSECONDS) -> stop ();
}

// ---
void PACMAN::Game::continueGame ()
{
	QGAMES::AdvancedArcadeGame::continueGame ();

	// The seconds continue...
	loopCounter (_COUNTERSECONDS) -> run ();
}

// ---
void PACMAN::Game::processEvent (const QGAMES::Event& evnt)
{
	// To switch on / off the frames per second...
	if (evnt.code () == __QGAMES_KEYBOARDEVENT__)
	{
		QGAMES::KeyBoardEventData* dt = (QGAMES::KeyBoardEventData*) evnt.data ();
		if (dt -> _internalCode == QGAMES::KeyCode::QGAMES_F1 && !dt -> _on)
			setShowFPS (!_showFPS); 
	}
	else
	// When a significant number of points have been reached, a new live is added...
	if (evnt.code () == __PACMAN_PACMANPOINTSHIGHLIGHTREACHED__)
	{
		setLives (lives () + 1);

		_lives -> setLives (lives ());

		sound (__PACMAN_SOUNDEXTRAPAC__) -> play (-1);
	}

	QGAMES::AdvancedArcadeGame::processEvent (evnt);
}

// ---
void PACMAN::Game::detectCollisions ()
{
	if (_detectCollisions)
		QGAMES::AdvancedArcadeGame::detectCollisions ();
}

// ---
void PACMAN::Game::initializeEntitiesAndWorlds ()
{
	// Add the worlds and scenes used in the game...
	std::vector <int> wLst = worldBuilder () -> resourcesId ();
	for (int i = 0; i < (int) wLst.size (); addWorld (wLst [i++]));
	// Add the artists used in the game 
	std::vector <int> eLst = entityBuilder () -> resourcesId ();
	for (int i = 0; i < (int) eLst.size (); i++)
	{
		if (dynamic_cast <QGAMES::Artist*> (entityBuilder () -> entity (eLst [i])))
			addArtist (eLst [i]); // Only artists...and all (none visible at that point, remember!)
	}

	// All the worlds and scenes (and their action blocks too) in the game 
	// are considered as elements which information have to be saved.
	// The entities are not considered in the list of information to be kept
	// Because their status is already part of the general configuration.
	// IMPORTANT: Worlds, Scenes and Action Blocks have to have different ids from each other
	// to guaranttee that they are kept right.
	// Only worlds are added, because when they runtimeValues are got everything nested inside is also got.
	PACMAN::Game::Conf* cfg = dynamic_cast <PACMAN::Game::Conf*> (configuration ());
	assert (cfg != nullptr);
	QGAMES::Elements elmts;
	for (int i = 0; i < (int) wLst.size (); i++)
		elmts.push_back (world (wLst [i]));

	cfg -> setElementsToKeep (elmts); // They will be also kept (or reload) 
									  // when configuration is kept (or reload)!

	// The previous instruction will have set a not valid initial value for the worlds
	// It could cause an error when the game starts
	// So, it is better to set back those values to nothing!
	for (QGAMES::Worlds::const_iterator i = worlds ().begin (); i != worlds ().end (); i++)
		cfg -> setConfValueForElement ((*i).second -> id (), QGAMES::SetOfOpenValues ());
}

// ---
QGAMES::GameStateBuilder* PACMAN::Game::createGameStateBuilder ()
{
	QGAMES::GameStateBuilder* result = QGAMES::AdvancedArcadeGame::createGameStateBuilder ();
	result -> addAddsOn (new PACMAN::ForInitializePACMANGameStateAddOn (result));
	result -> addAddsOn (new PACMAN::ForShowingMonsterMovingGameStateAddOn (result));
	result -> addAddsOn (new PACMAN::ForShowingPlayerNameGameStateAddOn (result));
	result -> addAddsOn (new PACMAN::ForShowingLevelGameStateAddOn (result));
	result -> addAddsOn (new PACMAN::ForPlayingControlGameStateAddOn (result));
	result -> addAddsOn (new PACMAN::ForPlayingGameStateAddOn (result));
	result -> addAddsOn (new PACMAN::ForStopPacmanElemntsMovingGameStateAddOn (result));
	result -> addAddsOn (new PACMAN::ForBlinkMazePlayingGameStateAddOn (result));
	result -> addAddsOn (new PACMAN::ForOptionsWhenPlayingGameStateAddOn (result));
	result -> addAddsOn (new PACMAN::ForShowingHallOfFameGameStateAddOn (result));
	result -> addAddsOn (new PACMAN::ForGameStateControlAddon (result));

	return (result);
}

// ---
QGAMES::MapBuilder* PACMAN::Game::createMapBuilder ()
{
	QGAMES::MapBuilder* result = new QGAMES::MapBuilder (parameter (__GAME_MAPSOBJECTSFILE__));
	result -> addAddsOn (_tmxAddsOnMapBuilder = new PACMAN::TMXMapBuilder ((QGAMES::Sprite2DBuilder*) formBuilder ()));

	return (result);
}

// ---
QGAMES::Game::Configuration* PACMAN::Game::createConfiguration ()
{ 
	return (new Conf 
		(1 /** Default players */, 
		 std::atoi (parameter (__PACMAN_MAXNUMBEROFLIVESPARAMETER__).c_str ()),
		 std::atoi (parameter (__PACMAN_LEVELPARAMETER__).c_str ()))); 
}

// ---
std::string PACMAN::Game::defaultParameter (const std::string& p) const
{
	std::string result (__NULL_STRING__);
	if (p == std::string (__PACMAN_MAXNUMBEROFLIVESPARAMETER__))
		result = std::string (__PACMAN_DEFAULTMAXNUMBEROFLIVESPARAMETER__);
	else if (p == std::string (__PACMAN_LEVELPARAMETER__))
		result = std::string (__PACMAN_DEFAULTLEVELPARAMETER__);
	else if (p == std::string (__PACMAN_SHOWPFSPARAMETER__))
		result = std::string (__PACMAN_DEFAULTSHOWFPSPARAMETER__);
	else if (p == std::string (__PACMAN_DRAWARTISTTARGETPARAMETER__))
		result = std::string (__PACMAN_DEFAULTDRAWARTISTTARGETPARAMETER__);
	else if (p == std::string (__PACMAN_PASSWORDPARAMETER__))
		result = std::string (__PACMAN_DEFAULTPASSWORDPARAMETER__);
	else result = QGAMES::AdvancedArcadeGame::defaultParameter (p);
	return (result);
}

// ---
void PACMAN::Game::initialize ()
{
	if (isInitialized ())
		return; // Only once...

	QGAMES::AdvancedArcadeGame::initialize ();

	if (!isInitialized ())
		return;

	// It is not needed to relocate th main screen, because it is alocated at the centr of the creen by default!

	// Add all game states from the definition file...
	addMainStatesFromBuilder (); 
	// ...and start to play with the main one...
	QGAMES::AdvancedGameStateControl* gCS = 
		dynamic_cast <QGAMES::AdvancedGameStateControl*> 
			(gameStateBuilder () -> gameState (__PACMAN_STANDARDCONTROLGAMESTATE__));
	assert (gCS != nullptr);

#ifndef NDEBUG
	setShowFPS (true);
	_drawArtistTarget = true;
#else
	std::string sfps = parameter (std::string (__PACMAN_SHOWPFSPARAMETER__));
	setShowFPS (QGAMES::toUpper (QGAMES::trim (sfps)) == std::string (__YES_STRING__));
	_drawArtistTarget = parameter (std::string (__PACMAN_DRAWARTISTTARGETPARAMETER__)) == std::string (__YES_STRING__);
#endif
	_passwordWellIntroduced = parameter (std::string (__PACMAN_PASSWORDPARAMETER__)) == std::string ("PACMANPWD");

	setState (gCS); // To start...

	observe (inputHandler ());
}

// ---
void PACMAN::Game::finalize ()
{
	unObserve (inputHandler ());

	QGAMES::AdvancedArcadeGame::finalize ();
}
