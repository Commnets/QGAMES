#include <DragonwindLike/dwgame.hpp>
#include <DragonwindLike/dwworlds.hpp>

// ---
DRAGONWIND::Game::Conf::Conf (int nP, int dW, int dS)
	: QGAMES::PlatformGame::Conf (nP),
	  _score (), _level (), _playerName (), _artistType (), _playerSeconds (), _playerEnergy (),
	  _sceneCompleted ()

{ 
	assert (nP > 0);

	// Add two keys to the default one managed in the parent class...
	_keys.resize (10);
	_keys [__QGAMES_KEYFIRE__]	= QGAMES::KeyCode::QGAMES_M; // Fire (instead SPACE that will now use to jump)
	_keys [__DRAGONWIND_KEYITERATE__] = QGAMES::KeyCode::QGAMES_Q; // Iterate
	_keys [__DRAGONWIND_KEYCATCHLEAVE__] = QGAMES::KeyCode::QGAMES_W; // Catch / Leave
	_keys [__DRAGONWIND_KEYJUMP__] = QGAMES::KeyCode::QGAMES_SPACE; // To jump

	adjustToPlayers (nP); 
}

// ---
void DRAGONWIND::Game::Conf::actualizeHallOfFame ()
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
void DRAGONWIND::Game::Conf::adjustToPlayers (int nP)
{
	QGAMES::PlatformGame::Conf::adjustToPlayers (nP);

	_score = std::vector <int> (nP, 0);
	_level = std::vector <int> (nP, 0); // The first level is 1
	_playerName = std::vector <std::string> (nP, std::string (__NULL_STRING__));
	_artistType = std::vector <int> (nP, 0);
	_playerSeconds = std::vector <int> (nP, 0);
	_playerEnergy = std::vector <QGAMES::bdata> (nP, __BD 100); // Full, initially!
	_sceneCompleted = std::vector <std::map <int, bool>> (nP, std::map <int, bool> ());

	// In Dragonwind like games, these inernal variables need to be readjusted...
	_pocket = std::vector <QGAMES::PlatformGame::Pocket> 
		(nP, QGAMES::PlatformGame::Pocket (__DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__)); // The standard size...
	_currentWorld = std::vector <int> (nP, _defaultWorld); 
	_currentScene = std::vector <int> (nP, _defaultScene);

	// When the players are adjusted, the information kept in the worlds have to be deleted to...
	// Remember that this information is kept by player, and...
	for (QGAMES::Worlds::const_iterator i = (__AGM game ()) -> worlds ().begin (); 
			i != (__AGM game ()) -> worlds ().end (); i++)
	{
		((DRAGONWIND::World*) (*i).second) -> initializeRuntimeValuesFrom (QGAMES::SetOfOpenValues ());
		if (existConfValueForElement ((*i).second -> id ())) // Just if the element exists
			setConfValueForElement ((*i).second -> id (), QGAMES::SetOfOpenValues ());
	}
}

// ---
void DRAGONWIND::Game::Conf::cfgToStream (std::ostringstream& oS)
{
	QGAMES::PlatformGame::Conf::cfgToStream (oS);

	for (int i = 0; i < _numberPlayers; i++)
		oS << _score [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _level [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _playerName [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _artistType [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _playerSeconds [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _playerEnergy [i] << std::endl;
	
	for (int i = 0; i < _numberPlayers; i++)
	{
		oS << _sceneCompleted [i].size () << std::endl;
		for (std::map <int, bool>::const_iterator j = _sceneCompleted [i].begin (); 
				j != _sceneCompleted [i].end (); j++)
			oS << (*j).first << std::endl << (*j).second << std::endl;
	}
}

// ---
void DRAGONWIND::Game::Conf::cfgFromStream (std::istringstream& iS)
{
	QGAMES::PlatformGame::Conf::cfgFromStream (iS);

	_score.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _score [i];
	_level.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _level [i];
	_playerName.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _playerName [i];
	_artistType.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _artistType [i];
	_playerSeconds.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _playerSeconds [i];
	_playerEnergy.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _playerEnergy [i];
	
	_sceneCompleted.resize (_numberPlayers);
	int nE, nSC; bool sC;
	for (int i = 0; i < _numberPlayers; i++)
	{
		iS >> nE;
		for (int j = 0; j < nE; j++)
		{
			iS >> nSC; iS >> sC;
			_sceneCompleted [i][nSC] = sC;
		}
	}
}

// ---
std::string DRAGONWIND::Game::passwordString (int i) const
{
	return (parameter (std::string (__DRAGONWIND_GAMEPASSWORDPARAMETER__) + 
		(i == -1 ? std::string (__NULL_STRING__) : std::to_string (i))));
}

// ---
void DRAGONWIND::Game::addScoreObjects ()
{
	// It is done one by one, 
	// because depending on the final game implementation different score objects could be avoided

	if (!_energy)
	{
		addScoreObject (_energy = new DRAGONWIND::EntityEnergy ());
		_energy -> setPosition 
			(QGAMES::Position (__BD 10, 
							   __BD (mainScreen () -> visualHeight () - 10 - _energy -> visualHeight ()), 
							   __BD 0));
		_energy -> setPercentage (100);
		_energy -> observe (mainArtist ()); // Only for the one playing...
	}

	if (!_lives) 
	{
		addScoreObject (_lives = new DRAGONWIND::NinjaLives (artistType ())); // Depeding on the type...
		_lives -> setPosition (QGAMES::Position (__BD 10, __BD 10, __BD 0));
		_lives -> setTypeArtist (artistType ());
		_lives -> setLives (lives ());
		_lives -> observe (mainArtist ());
	}

	if (!_things) 
	{
		addScoreObject (_things = new DRAGONWIND::NinjaThingsBeingCarried ());
		_things -> setPosition (_lives -> position () + 
			QGAMES::Vector (__BD 0, __BD (_lives -> visualHeight () + 10), __BD 0));
		_things -> observe (mainArtist ()); // Only for the one playing...
	}

	if (!_shootsLeft) 
	{
		addScoreObject (_shootsLeft = new DRAGONWIND::ShootsLeftCounter ());
		_shootsLeft -> setPosition 
			(QGAMES::Position (__BD (mainScreen () -> visualWidth () - (_shootsLeft -> visualLength () + 20)),
							   __BD 10,
							   __BD 0));
		_shootsLeft -> observe (mainArtist ()); // Only for the one playing...
	}

	if (!_timeCounter)
	{
		addScoreObject (_timeCounter = new DRAGONWIND::TimeCounter ());
		_timeCounter -> setPosition (_energy -> position () - 
			QGAMES::Vector (__BD 0, __BD (_timeCounter -> visualHeight () + 10), __BD 0));
		_timeCounter -> setSeconds (seconds ());
		// Any time the time changes, the time counter is actualized...
	}

	if (!_scoreCounter) 
	{
		addScoreObject (_scoreCounter = new DRAGONWIND::ScoreCounter ());
		_scoreCounter -> setPosition (QGAMES::Position (
			_energy -> position ().posX () + __BD (_energy -> visualLength () - _scoreCounter -> visualLength ()),
			_energy -> position ().posY () - __BD (10 + _scoreCounter -> visualHeight ()),
			__BD 0));
		_scoreCounter -> observe (mainArtist ()); // Only for the one playing
	}

	if (!_playerName)
	{
		addScoreObject (_playerName = new DRAGONWIND::FixText (0, playerName ())); // First fixed text
		_playerName -> setPosition (QGAMES::Position (
			__BD (_energy -> position ().posX () + (_energy -> visualLength () >> 1) - (_playerName -> visualLength () >> 1)),
			__BD (_energy -> position ().posY () + (_energy -> visualHeight () >> 1) - (_playerName -> visualHeight () >> 1)),
			__BD 0));
	}

	if (!_energyMainBadGuy)
	{
		addScoreObject (_energyMainBadGuy = new DRAGONWIND::EntityEnergy (1, __QGAMES_REDENERGYBARFORM__));
		_energyMainBadGuy -> setPosition 
			(QGAMES::Position (__BD 10, 
							   __BD (mainScreen () -> visualHeight () - 30 - 
									 _energy -> visualHeight () - 
								     _energyMainBadGuy -> visualHeight () - _scoreCounter -> visualHeight ()), 
							   __BD 0));
		_energyMainBadGuy -> setPercentage (100);
		_energyMainBadGuy -> setVisible (false);
	}
}

// ---
void DRAGONWIND::Game::removeScoreObjects ()
{
	DRAGONWIND::Character* mA = mainArtist ();
	assert (mA); // It has to exist at this point...

	// It is done one by one, 
	// because depending on the final game different score objects could be avoided

	if (_energy)
	{ 
		assert (_energy -> isObserving (mA)); // Just in case...if the logic of the game is right...
		_energy -> unObserve (mA); 
		delete (_energy); _energy = NULL; 
	}

	if (_lives)
	{
		assert (_lives -> isObserving (mA));
		_lives -> unObserve (mA); 
		delete (_lives); _lives = NULL;
	}
	
	if (_things)
	{
		assert (_things -> isObserving (mA));
		_things -> unObserve (mA); 
		delete (_things); _things = NULL; 
	}

	if (_shootsLeft)
	{
		assert (_shootsLeft -> isObserving (mA));
		_shootsLeft -> unObserve (mA); 
		delete (_shootsLeft); _shootsLeft = NULL; 
	}
	
	delete (_timeCounter); _timeCounter = NULL;
	
	if (_scoreCounter) 
	{
		assert (_scoreCounter -> isObserving (mA));
		_scoreCounter -> unObserve (mA); 
		delete (_scoreCounter); _scoreCounter = NULL; 
	}

	delete (_playerName); _playerName = NULL;

	if (_energyMainBadGuy)
	{
		assert (_energyMainBadGuy -> observers ().empty ());
		delete (_energyMainBadGuy); _energyMainBadGuy = NULL;
	}

	QGAMES::PlatformGame::removeScoreObjects ();
}

// ---
QGAMES::GameStateBuilder* DRAGONWIND::Game::createGameStateBuilder ()
{
	QGAMES::GameStateBuilder* result = QGAMES::PlatformGame::createGameStateBuilder ();
	result -> addAddsOn (new DRAGONWIND::ForInitializeDragonWindGameStateAddOn (result));
	result -> addAddsOn (new DRAGONWIND::ForShowingPlayerNameGameStateAddOn (result));
	result -> addAddsOn (new DRAGONWIND::ForShowingWorldNameGameStateAddOn (result));
	result -> addAddsOn (new DRAGONWIND::ForShowingSceneNameGameStateAddOn (result));
	result -> addAddsOn (new DRAGONWIND::ForCreditTitlesGameStateAddOn (result));
	result -> addAddsOn (new DRAGONWIND::ForMenuAddsOn (result));
	result -> addAddsOn (new DRAGONWIND::ForPlayingControlGameStateAddOn (result));
	result -> addAddsOn (new DRAGONWIND::ForPlayingGameStateAddOn (result));
	result -> addAddsOn (new DRAGONWIND::ForOptionsWhenPlayingGameStateAddOn (result));
	result -> addAddsOn (new DRAGONWIND::ForShowingHallOfFameGameStateAddOn (result));
	result -> addAddsOn (new DRAGONWIND::ForGameStateControlAddon (result));
	return (result);
}

// ---
QGAMES::MapBuilder* DRAGONWIND::Game::createMapBuilder ()
{
	QGAMES::MapBuilder* result = QGAMES::PlatformGame::createMapBuilder ();
	// Just an additional add on to create the specific and tipical map in DRAGONWIND games (is a generic one)
	DRAGONWIND::TMXMapBuilder* aOn = new DRAGONWIND::TMXMapBuilder ((QGAMES::Sprite2DBuilder*) formBuilder ());
	result -> addAddsOn (aOn);

	return (result);
}

// ---
std::string DRAGONWIND::Game::defaultParameter (const std::string& p) const
{
	if (p == std::string (__DRAGONWIND_GAMEINITWORLDPARAMETER__))
		return (std::string (__DRAGONWIND_GAMEINITWORLDDEFPARAMETER__));
	else if (p == std::string (__DRAGONWIND_GAMEINITSCENEPARAMETER__))
		return (std::string (__DRAGONWIND_GAMEINITSCENEDEFPARAMETER__));
	else if (p.find (std::string (__DRAGONWIND_GAMEPASSWORDPARAMETER__)) != std::string::npos)
		return (std::string (__DRAGONWIND_GAMEPASSWORDDEFPARAMETER__));
	
	return (QGAMES::PlatformGame::defaultParameter (p));
}

// ---
void DRAGONWIND::Game::initialize ()
{
	__WLOGPLUS ("Initializing DRAGONWIND game");

	QGAMES::PlatformGame::initialize ();

	DRAGONWIND::Game::Conf* cfg = dynamic_cast <DRAGONWIND::Game::Conf*> (configuration ());
	assert (cfg);
	cfg -> setWorldAndDefaultScene (
		std::atoi (parameter (__DRAGONWIND_GAMEINITWORLDPARAMETER__).c_str ()), 
		std::atoi (parameter (__DRAGONWIND_GAMEINITSCENEPARAMETER__).c_str ()));

	// It is neccessary to rebuild the copy of the configuration 
	// just to have also the default world and scene set ok.
	delete (_copyOfConfiguration); 
	_copyOfConfiguration = (QGAMES::AdvancedArcadeGame::Conf*) configuration () -> clone ();
}

// ---
void DRAGONWIND::Game::inEveryLoop ()
{
	QGAMES::PlatformGame::inEveryLoop ();

	if (_timeCounter)
		_timeCounter -> setSeconds (playerSeconds ());
}

// ---
void DRAGONWIND::Game::additionalTasks ()
{
	if (_detectCollisions)
		QGAMES::AdvancedArcadeGame::detectCollisions ();
}

// ---
void DRAGONWIND::Game::detectCollisions ()
{
	QGAMES::ArcadeGame::SetOfPairOfEntities eties;
	pairOfVisibleEntitiesInTheScene (eties);

	for (QGAMES::ArcadeGame::SetOfPairOfEntities::const_iterator i = eties.begin (); i != eties.end (); i++)
	{
		// No sense when food hit food or things hit thigs and combinations
		bool tOrf1 = dynamic_cast <DRAGONWIND::Food*> ((*i)._a) || dynamic_cast <DRAGONWIND::Thing*> ((*i)._a);
		bool tOrf2 = dynamic_cast <DRAGONWIND::Food*> ((*i)._b) || dynamic_cast <DRAGONWIND::Thing*> ((*i)._b);
		if (!tOrf1 || !tOrf2)
		{
			if ((*i)._a -> hasCollisionWith ((*i)._b))
			{
				(*i)._a -> whenCollisionWith ((*i)._b); // It could be a composity entity...take care
				(*i)._b -> whenCollisionWith ((*i)._a);
			}
		}
	}
}

// ---
void DRAGONWIND::Game::initializeEntitiesAndWorlds ()
{
	__WLOGPLUS ("Initializing entities and worlds");

	// Add the worlds and scenes used in the game...
	std::vector <int> wLst = worldBuilder () -> resourcesId ();
	for (int i = 0; i < (int) wLst.size (); addWorld (wLst [i++]));
	// Add the artists used in the game 
	std::vector <int> eLst = entityBuilder () -> resourcesId ();
	for (int i = 0; i < (int) eLst.size (); i++)
	{
		if (dynamic_cast <QGAMES::Artist*> (entityBuilder () -> entity (eLst [i])))
			addArtist (eLst [i]); // Only artists...and all (none visible at that point, remember!)
		if (dynamic_cast <DRAGONWIND::FlockOfMonsters*> (entityBuilder () -> entity (eLst [i])))
			_flockOfMonsters [eLst [i]] = entityBuilder () -> entity (eLst [i]); // List of flocks...
		if (dynamic_cast <DRAGONWIND::Character*> (entityBuilder () -> entity (eLst [i])))
			_playingCharacters [eLst [i]] = entityBuilder () -> entity (eLst [i]); // List of playing characters...
	}

	// All the worlds and scenes (and their action blocks too) in the game 
	// are considered as elements which information have to be saved.
	// The entities are not considered in the list of information to be kept
	// Because their status is already part of the general configuration.
	// IMPORTANT: Worlds, Scenes and Action Blocks have to have different ids from each other
	// to guaranttee that they are kept right.
	// Only worlds are added, because when they runtimeValues are got everything nested inside is also got.
	// @see DRAGONWIND::ScsneActionBlock,, DRAGONWIND::Scene and DRADONWIND::World for more details.
	DRAGONWIND::Game::Conf* cfg = dynamic_cast <DRAGONWIND::Game::Conf*> (configuration ());
	assert (cfg);
	QGAMES::Elements elmts;
	for (int i = 0; i < (int) wLst.size (); i++)
		elmts.push_back (world (wLst [i]));

	// At this point the internat structure of the game has already been created
	// but nothng has initialized yet.
	// The methoid will refresh the internal status and invoke the process to generate the runtime structure
	// So the methods invoked must be designed for this situation
	cfg -> setElementsToKeep (elmts); // They will be also kept (or reload) 
									  // when configuration is kept (or reload)!

	// The previous instruction will have set a not valid initial value for the worlds
	// It could cause an error when the game starts
	// So, it is better to set back those values to nothing!
	for (QGAMES::Worlds::const_iterator i = worlds ().begin (); i != worlds ().end (); i++)
		cfg -> setConfValueForElement ((*i).second -> id (), QGAMES::SetOfOpenValues ());
}
