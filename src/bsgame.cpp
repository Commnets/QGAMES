#include <BattleshipLike/bsgame.hpp>
#include <BattleshipLike/bsartists.hpp>
#include <BattleshipLike/bsworlds.hpp>

// ---
QGAMES::SetOfOpenValues BATTLESHIP::Game::TransportedElement::likeSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result;

	result.addOpenValue (__BATTLESHIP_ELMTSTRANSATTRIBUTEID__, QGAMES::OpenValue (_id));
	result.addOpenValue (__BATTLESHIP_ELMTSTTRANSATTRIBUTETYPE__, QGAMES::OpenValue (_type));
	result.addSetOfOpenValues (0, _values);// Additional parameters...

	return (result);
}

// ---
std::ostream& BATTLESHIP::operator << (std::ostream& oS, const BATTLESHIP::Game::TransportedElement& tE)
{
	oS << tE._id << std::endl
	   << tE._type << std::endl
	   << tE._values;

	return (oS);
}

// ---
std::istream& BATTLESHIP::operator >> (std::istream& iS, BATTLESHIP::Game::TransportedElement& tE)
{
	iS >> tE._id >> tE._type >> tE._values;

	return (iS);
}

// ---
BATTLESHIP::Game::TransportedElement BATTLESHIP::Game::TransportedElements::toAddElement 
	(const BATTLESHIP::Game::TransportedElement& pE)
{
	BATTLESHIP::Game::TransportedElement lT = _elements [_elements.size () - 1];
	for (int i = (_elements.size () - 2); i >= 0; i--)
		_elements [i + 1] = _elements [i];
	_elements [0] = pE;
	return (lT);
}

// ---
std::vector <int> BATTLESHIP::Game::TransportedElements::listOfElementsType () const
{
	std::vector <int> result (_elements.size (), -1);
	for (int i = 0; i < (int) _elements.size (); i++)
		result [i] = _elements [i]._type;
	return (result);
}

// ---
bool BATTLESHIP::Game::TransportedElements::isEmpty () const
{
	bool result = true;
	for (int i = 0; i < (int) _elements.size () && !result; result = (_elements [i++]._type != -1));
	return (result);
}

// ---
std::ostream& BATTLESHIP::operator << (std::ostream& oS, const BATTLESHIP::Game::TransportedElements& p)
{
	oS << (int) p._elements.size ();
	for (int i = 0; i < (int) p._elements.size (); i++)
		oS << std::endl << p._elements [i];

	return (oS);
}

// ---
std::istream& BATTLESHIP::operator >> (std::istream& iS, BATTLESHIP::Game::TransportedElements& p)
{
	int nE; iS >> nE;
	p._elements.resize (nE);
	for (int i = 0; i < nE; i++)
		iS >> p._elements [i];

	return (iS);	
}

// ---
BATTLESHIP::Game::Conf::Conf (int nP, int dW, int dS)
	: QGAMES::AdvancedArcadeGame::Conf (nP),
	  _transportedElements (),
	  _score (), _level (), _shootingsLeft (), _playerName (), _artistType (), _playerSeconds (),
	  _sceneCompleted (),
	  _defaultWorld (dW), _defaultScene (dS)
{ 
	assert (nP > 0);

	// Add two keys to the default one managed in the parent class...
	_keys.resize (10);
	_keys [__QGAMES_KEYFIRE__]	= QGAMES::KeyCode::QGAMES_M; // Fire (instead SPACE that will now use to jump)
	_keys [__BATTLESHIP_KEYITERATE__] = QGAMES::KeyCode::QGAMES_Q; // Iterate
	_keys [__BATTLESHIP_KEYCATCHLEAVE__] = QGAMES::KeyCode::QGAMES_W; // Catch / Leave
	_keys [__BATTLESHIP_KEYJUMP__] = QGAMES::KeyCode::QGAMES_SPACE; // Jump (or doing something at fly)

	adjustToPlayers (nP); 
}

// ---
void BATTLESHIP::Game::Conf::actualizeHallOfFame ()
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
void BATTLESHIP::Game::Conf::adjustToPlayers (int nP)
{
	QGAMES::AdvancedArcadeGame::Conf::adjustToPlayers (nP);

	_transportedElements = std::vector <BATTLESHIP::Game::TransportedElements> 
		(nP, BATTLESHIP::Game::TransportedElements (__BATTLESHIP_NUMBERTHINGSBEINGCARRIED__)); // The standard size...

	_score = std::vector <int> (nP, 0);
	_level = std::vector <int> (nP, std::atoi (game () -> parameter (__BATTLESHIP_GAMEINITIALPOWERPARAMETER__).c_str ())); 
	_shootingsLeft = std::vector <int> 
		(nP, std::atoi (game () -> parameter (__BATTLESHIP_GAMEINITIALSHOOTINGSLEFTPARAMETER__).c_str ())); 
	_playerName = std::vector <std::string> (nP, std::string (__NULL_STRING__));
	_artistType = std::vector <int> (nP, 0);
	_playerSeconds = std::vector <int> (nP, 0);
	_sceneCompleted = std::vector <std::map <int, bool>> (nP, std::map <int, bool> ());

	// In Battleship like games, these inernal variables need to be readjusted...
	_currentWorld = std::vector <int> (nP, _defaultWorld); 
	_currentScene = std::vector <int> (nP, _defaultScene);

	// When the players are adjusted, the information kept in the worlds have to be deleted to...
	// Remember that this information is kept by player, and...
	for (QGAMES::Worlds::const_iterator i = (__AGM game ()) -> worlds ().begin (); 
			i != (__AGM game ()) -> worlds ().end (); i++)
	{
		((BATTLESHIP::World*) (*i).second) -> initializeRuntimeValuesFrom (QGAMES::SetOfOpenValues ());
		if (existConfValueForElement ((*i).second -> id ())) // Just if the element exists
			setConfValueForElement ((*i).second -> id (), QGAMES::SetOfOpenValues ());
	}
}

// ---
void BATTLESHIP::Game::Conf::cfgToStream (std::ostringstream& oS)
{
	QGAMES::AdvancedArcadeGame::Conf::cfgToStream (oS);

	for (int i = 0; i < _numberPlayers; i++)
		oS << _transportedElements [i] << std::endl;

	for (int i = 0; i < _numberPlayers; i++)
		oS << _score [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _level [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _shootingsLeft [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _playerName [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _artistType [i] << std::endl;
	for (int i = 0; i < _numberPlayers; i++)
		oS << _playerSeconds [i] << std::endl;
	
	for (int i = 0; i < _numberPlayers; i++)
	{
		oS << _sceneCompleted [i].size () << std::endl;
		for (std::map <int, bool>::const_iterator j = _sceneCompleted [i].begin (); 
				j != _sceneCompleted [i].end (); j++)
			oS << (*j).first << std::endl << (*j).second << std::endl;
	}
}

// ---
void BATTLESHIP::Game::Conf::cfgFromStream (std::istringstream& iS)
{
	QGAMES::AdvancedArcadeGame::Conf::cfgFromStream (iS);

	_transportedElements.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _transportedElements [i];

	_score.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _score [i];
	_level.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _level [i];
	_shootingsLeft.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _shootingsLeft [i];
	_playerName.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _playerName [i];
	_artistType.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _artistType [i];
	_playerSeconds.resize (_numberPlayers);
	for (int i = 0; i < _numberPlayers; i++)
		iS >> _playerSeconds [i];
	
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
BATTLESHIP::Game::Game (QGAMES::GameImplementation* impl)
	: QGAMES::AdvancedArcadeGame (impl),
	  _theDataGame (NULL), // With additional data to control the game...
	  _energy (NULL),
	  _energyMainBadGuy (NULL),
	  _lives (NULL), 
	  _things (NULL), 
	  _shootsLeft (NULL), 
	  _timeCounter (NULL),
	  _scoreCounter (NULL),
	  _playerName (NULL),
	  _detectCollisions (false)
{ 
	_theDataGame = new DataGame; 
}

// ---
bool BATTLESHIP::Game::isNotPlaying () const
{
	if (!dynamic_cast <BATTLESHIP::GameStateControl*> (_currentGameState))
		return (true); // It is a game control state not valid...

	if (!dynamic_cast <BATTLESHIP::PlayingControlGameState*> (_currentGameState -> nestedState ()))
		return (true); // The player is not playing...

	if (!dynamic_cast <BATTLESHIP::PlayingGameState*> (_currentGameState -> nestedState () -> nestedState ()))
		return (true); // ...and not really really playing at all!

	return (false);
}

// ---
BATTLESHIP::Battleship* BATTLESHIP::Game::battleship (int nP)
{ 
	return (dynamic_cast <Battleship*> 
		(artist (__BATTLESHIP_MAINARTISTBASEID__ + artistType (nP)))); 
}

// ---
void BATTLESHIP::Game::addScoreObjects ()
{
	// It is done one by one, 
	// because depending on the final game implementation different score objects could be avoided

	if (!_energy)
	{
		addScoreObject (_energy = new BATTLESHIP::EntityEnergy ());
		_energy -> setPosition 
			(QGAMES::Position (__BD 10, 
							   __BD (mainScreen () -> visualHeight () - 10 - _energy -> visualHeight ()), 
							   __BD 0));
		_energy -> setPercentage (100);
		_energy -> observe (battleship ()); // Only for the one playing...
	}

	if (!_lives) 
	{
		addScoreObject (_lives = new BATTLESHIP::BattleshipLives (artistType ())); // Depeding on the type...
		_lives -> setPosition (QGAMES::Position (__BD 10, __BD 10, __BD 0));
		_lives -> setTypeArtist (artistType ());
		_lives -> setLives (lives ());
		_lives -> observe (battleship ());
	}

	if (!_things) 
	{
		addScoreObject (_things = new BATTLESHIP::BattleshipThingsBeingCarried ());
		_things -> setPosition (_lives -> position () + 
			QGAMES::Vector (__BD 0, __BD (_lives -> visualHeight () + 10), __BD 0));
		_things -> observe (battleship ()); // Only for the one playing...
	}

	if (!_shootsLeft) 
	{
		addScoreObject (_shootsLeft = new BATTLESHIP::ShootsLeftCounter ());
		_shootsLeft -> setPosition 
			(QGAMES::Position (__BD (mainScreen () -> visualWidth () - (_shootsLeft -> visualLength () + 20)),
							   __BD 10,
							   __BD 0));
		_shootsLeft -> observe (battleship ()); // Only for the one playing...
	}

	if (!_timeCounter)
	{
		addScoreObject (_timeCounter = new BATTLESHIP::TimeCounter ());
		_timeCounter -> setPosition (_energy -> position () - 
			QGAMES::Vector (__BD 0, __BD (_timeCounter -> visualHeight () + 10), __BD 0));
		_timeCounter -> setSeconds (seconds ());
		// Any time the time changes, the time counter is actualized...
	}

	if (!_scoreCounter) 
	{
		addScoreObject (_scoreCounter = new BATTLESHIP::ScoreCounter ());
		_scoreCounter -> setPosition (QGAMES::Position (
			_energy -> position ().posX () + __BD (_energy -> visualLength () - _scoreCounter -> visualLength ()),
			_energy -> position ().posY () - __BD (10 + _scoreCounter -> visualHeight ()),
			__BD 0));
		_scoreCounter -> observe (battleship ()); // Only for the one playing
	}

	if (!_playerName)
	{
		addScoreObject (_playerName = new BATTLESHIP::FixText (0, playerName ())); // First fixed text
		_playerName -> setPosition (QGAMES::Position (
			__BD (_energy -> position ().posX () + (_energy -> visualLength () >> 1) - (_playerName -> visualLength () >> 1)),
			__BD (_energy -> position ().posY () + (_energy -> visualHeight () >> 1) - (_playerName -> visualHeight () >> 1)),
			__BD 0));
	}

	if (!_energyMainBadGuy)
	{
		addScoreObject (_energyMainBadGuy = new BATTLESHIP::EntityEnergy (1, __QGAMES_REDENERGYBARFORM__));
		_energyMainBadGuy -> setPosition 
			(QGAMES::Position (__BD 10, 
							   __BD (mainScreen () -> visualHeight () - 30 - 
									 _energy -> visualHeight () - 
								     _energyMainBadGuy -> visualHeight () - _scoreCounter -> visualHeight ()), 
							   __BD 0));
		_energyMainBadGuy -> setPercentage (100);
		_energyMainBadGuy -> setVisible (false); // Unless other thing is said...
	}
}

// ---
void BATTLESHIP::Game::removeScoreObjects ()
{
	BATTLESHIP::Battleship* bS = battleship ();
	assert (bS); // It has to exist at this point...

	// It is done one by one, 
	// because depending on the final game different score objects could be avoided

	if (_energy)
	{ 
		assert (_energy -> isObserving (bS)); // Just in case...if the logic of the game is right...
		_energy -> unObserve (bS); 
		delete (_energy); _energy = NULL; 
	}

	if (_lives)
	{ 
		assert (_lives -> isObserving (bS)); // Just in case...if the logic of the game is right...
		_lives -> unObserve (bS); 
		delete (_lives); _lives = NULL; 
	}
	
	if (_things)
	{
		assert (_things -> isObserving (bS));
		_things -> unObserve (bS); 
		delete (_things); _things = NULL; 
	}

	if (_shootsLeft)
	{
		assert (_shootsLeft -> isObserving (bS));
		_shootsLeft -> unObserve (bS); 
		delete (_shootsLeft); _shootsLeft = NULL; 
	}
	
	delete (_timeCounter); _timeCounter = NULL;
	
	if (_scoreCounter) 
	{
		assert (_scoreCounter -> isObserving (bS));
		_scoreCounter -> unObserve (bS); 
		delete (_scoreCounter); _scoreCounter = NULL; 
	}

	delete (_playerName); _playerName = NULL;

	delete (_energyMainBadGuy); _energyMainBadGuy = NULL;

	QGAMES::AdvancedArcadeGame::removeScoreObjects ();
}

// ---
bool BATTLESHIP::Game::isPasswordSet () const
{
	return (parameter (__BATTLESHIP_GAMEPASSWORDPARAMETER__) == std::string ("ICFNO"));
}

// ---
QGAMES::GameStateBuilder* BATTLESHIP::Game::createGameStateBuilder ()
{
	QGAMES::GameStateBuilder* result = QGAMES::AdvancedArcadeGame::createGameStateBuilder ();
	result -> addAddsOn (new BATTLESHIP::ForInitializeBattleshipGameStateAddOn (result));
	result -> addAddsOn (new BATTLESHIP::ForShowingBattleshipMovingGameStateAddOn (result));
	result -> addAddsOn (new BATTLESHIP::ForShowingPlayerNameGameStateAddOn (result));
	result -> addAddsOn (new BATTLESHIP::ForShowingWorldNameGameStateAddOn (result));
	result -> addAddsOn (new BATTLESHIP::ForShowingSceneNameGameStateAddOn (result));
	result -> addAddsOn (new BATTLESHIP::ForCreditTitlesGameStateAddOn (result));
	result -> addAddsOn (new BATTLESHIP::ForMenuAddsOn (result));
	result -> addAddsOn (new BATTLESHIP::ForPlayingControlGameStateAddOn (result));
	result -> addAddsOn (new BATTLESHIP::ForPlayingGameStateAddOn (result));
	result -> addAddsOn (new BATTLESHIP::ForOptionsWhenPlayingGameStateAddOn (result));
	result -> addAddsOn (new BATTLESHIP::ForShowingHallOfFameGameStateAddOn (result));
	result -> addAddsOn (new BATTLESHIP::ForGameStateControlAddon (result));
	return (result);
}

// ---
QGAMES::MapBuilder* BATTLESHIP::Game::createMapBuilder ()
{
	QGAMES::MapBuilder* result = new QGAMES::MapBuilder (parameter (__GAME_MAPSOBJECTSFILE__));
	result -> addAddsOn (new QGAMES::ObjectMapBuilderAddsOn (objectBuilder ()));
	result -> addAddsOn (new QGAMES::BackgroundObjectMapBuilder (objectBuilder ()));

	return (result);
}

// ---
std::string BATTLESHIP::Game::defaultParameter (const std::string& p) const
{
	if (p == std::string (__BATTLESHIP_GAMEINITWORLDPARAMETER__))
		return (__BATTLESHIP_GAMEINITWORLDDEFPARAMETER__);
	else if (p == std::string (__BATTLESHIP_GAMEINITSCENEPARAMETER__))
		return (__BATTLESHIP_GAMEINITSCENEDEFPARAMETER__);
	else if (p == std::string (__BATTLESHIP_GAMEPASSWORDPARAMETER__))
		return (__BATTLESHIP_GAMEPASSWORDDEFPARAMETER__);
	else if (p == std::string (__BATTLESHIP_GAMEINITIALPOWERPARAMETER__))
		return (__BATTLESHIP_GAMEINITIALPOWERDEFPARAMETER__);
	else if (p == std::string (__BATTLESHIP_GAMEINITIALSHOOTINGSLEFTPARAMETER__))
		return (__BATTLESHIP_GAMEINITIALSHOOTINGSLEFTDEFPARAMETER__);
	
	return (QGAMES::AdvancedArcadeGame::defaultParameter (p));
}

// ---
void BATTLESHIP::Game::initialize ()
{
	__WLOGPLUS ("Initializing BATTLESHIP game");

	QGAMES::AdvancedArcadeGame::initialize ();

	BATTLESHIP::Game::Conf* cfg = dynamic_cast <BATTLESHIP::Game::Conf*> (configuration ());
	assert (cfg);
	cfg -> setWorldAndDefaultScene (
		std::atoi (parameter (__BATTLESHIP_GAMEINITWORLDPARAMETER__).c_str ()), 
		std::atoi (parameter (__BATTLESHIP_GAMEINITSCENEPARAMETER__).c_str ()));

	// It is neccessary to rebuild the copy of the configuration 
	// just to have also the default world and scene set ok.
	delete (_copyOfConfiguration); 
	_copyOfConfiguration = (QGAMES::AdvancedArcadeGame::Conf*) configuration () -> clone ();
}

// ---
void BATTLESHIP::Game::inEveryLoop ()
{
	QGAMES::AdvancedArcadeGame::inEveryLoop ();

	if (_timeCounter)
		_timeCounter -> setSeconds (playerSeconds ());
}

// ---
void BATTLESHIP::Game::additionalTasks ()
{
	if (_detectCollisions)
		QGAMES::AdvancedArcadeGame::detectCollisions ();
}

// ---
void BATTLESHIP::Game::detectCollisions ()
{
	QGAMES::ArcadeGame::SetOfPairOfEntities eties;
	pairOfVisibleEntitiesInTheScene (eties);

	for (QGAMES::ArcadeGame::SetOfPairOfEntities::const_iterator i = eties.begin (); i != eties.end (); i++)
	{
		// No sense when energy elements hit thing to catch or viceversa...
		bool tOrf1 = dynamic_cast <BATTLESHIP::EnergyElement*> ((*i)._a) || 
			dynamic_cast <BATTLESHIP::ThingToCatch*> ((*i)._a);
		bool tOrf2 = dynamic_cast <BATTLESHIP::ThingToCatch*> ((*i)._b) || 
			dynamic_cast <BATTLESHIP::EnergyElement*> ((*i)._b);
		// No sense either when 2 shootings hit each other...
		bool tOrf3 = dynamic_cast <BATTLESHIP::SpaceshipShooting*> ((*i)._a) &&
			dynamic_cast <BATTLESHIP::SpaceshipShooting*> ((*i)._b);

		if ((!tOrf1 || !tOrf2) && !tOrf3)
		{
			if ((*i)._a -> hasCollisionWith ((*i)._b))
			{
				(*i)._a -> whenCollisionWith ((*i)._b); 
				(*i)._b -> whenCollisionWith ((*i)._a);
			}
		}
	}
}

// ---
void BATTLESHIP::Game::initializeEntitiesAndWorlds ()
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
	// @see BATTLESHIP::ScsneActionBlock,, BATTLESHIP::Scene and DRADONWIND::World for more details.
	BATTLESHIP::Game::Conf* cfg = dynamic_cast <BATTLESHIP::Game::Conf*> (configuration ());
	assert (cfg);
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
