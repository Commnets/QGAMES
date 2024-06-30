#include "Game.hpp"

// ---
BattleshipII::DataGame::DataGame ()
	: BATTLESHIP::DataGame ()
{ 
	_numberOfShootingTypes								= 9;
	_numberOfUFOTypes									= 8; // To be aligned, with the size of the list...(_ufoTypesList.size ())

	const int LEVELDATADEFAULTVALUES [11][5] = 
		
	{ 
	  // These are the ones defined in the basic type!
	  { 0, 2  , 200, 1, 40 },
	  { 1, 5  , 200, 2, 40 }, 
	  { 1, 10 , 200, 2, 40 }, 
	  { 2, 15 , 200, 3, 40 }, 
	  { 2, 20 , 200, 3, 40 }, 
	  { 3, 30 , 400, 3, 40 }, 
	  { 4, 50 , 400, 4, 40 }, 
	  { 5, 60 , 400, 5, 40 }, 
	  { 6, 60 , 600, 5, 40 }, 
	  { 7, 100, 600, 5, 40 }, 
	  // This one has been added for battleshipII...
	  { 8, 100, 600, 2, 50 } 
	};

	for (int i = 0; i < _numberOfPowerLevels; delete [] _LEVELDATA [i++]); delete _LEVELDATA;
	_numberOfPowerLevels = 11; // One more than before
	_LEVELDATA = new int* [_numberOfPowerLevels];
	for (int i = 0; i < _numberOfPowerLevels; i++)
	{
		_LEVELDATA [i] = new int [5];
		for (int j = 0; j < 5; j++)
			_LEVELDATA [i][j] = LEVELDATADEFAULTVALUES [(i >= 11) ? 10 : i][j];
	}
}

// ---
std::vector <int> BattleshipII::DataGame::defaultStatesForUFOType (int uT) const
{
	std::vector <int> result;

	switch (uT)
	{
		case __BATTLESHIPII_ENEMTYTYPE1000__:
			result = std::vector <int> 
				{ 6, __BATTLESHIP_ENEMYBLANKSTATE__, 7, __BATTLESHIP_ENEMYEXPLODINGSTATE__, __BATTLESHIP_ENEMYBLANKSTATE__ } ;
			break;

		case __BATTLESHIPII_ENEMTYTYPE1001__:
			result = std::vector <int> 
				{ 8, __BATTLESHIP_ENEMYBLANKSTATE__, 9, __BATTLESHIP_ENEMYEXPLODINGSTATE__, __BATTLESHIP_ENEMYBLANKSTATE__ } ;
			break;

		case __BATTLESHIPII_ENEMTYTYPE1010__:
			result = std::vector <int> 
				{ 10, __BATTLESHIP_ENEMYBLANKSTATE__, 11, 
					/** Special state for exploding, still rotating. */ 18, __BATTLESHIP_ENEMYBLANKSTATE__ } ;
			break;

		case __BATTLESHIPII_ENEMTYTYPE1011__:
			result = std::vector <int> 
				{ 12, __BATTLESHIP_ENEMYBLANKSTATE__, 13, 
					/** Special state for exploding, still rotating. */ 19, __BATTLESHIP_ENEMYBLANKSTATE__ } ;
			break;

		case __BATTLESHIPII_ENEMTYTYPE1012__:
			result = std::vector <int> 
				{ 14, __BATTLESHIP_ENEMYBLANKSTATE__, 15, 
					/** Special state for exploding, still rotating. */ 20, __BATTLESHIP_ENEMYBLANKSTATE__ } ;
			break;

		default:
			result = BATTLESHIP::DataGame::defaultStatesForUFOType (uT);
	}

	return (result);
}

// ---
const std::vector <int>& BattleshipII::DataGame::ufoTypesList () const
{
	if (_ufoTypesList.empty ())
	{
		_ufoTypesList = BATTLESHIP::DataGame::ufoTypesList ();

		// Add two additional types of UFOs...
		_ufoTypesList.push_back (__BATTLESHIPII_ENEMTYTYPE1000__);
		_ufoTypesList.push_back (__BATTLESHIPII_ENEMTYTYPE1001__);
		_ufoTypesList.push_back (__BATTLESHIPII_ENEMTYTYPE1010__);
		_ufoTypesList.push_back (__BATTLESHIPII_ENEMTYTYPE1011__);
		_ufoTypesList.push_back (__BATTLESHIPII_ENEMTYTYPE1012__);
	}

	return (_ufoTypesList);
}

// ---
QGAMES::Position BattleshipII::DataGame::DataGame::defaultInitialPointForMovementType (int nC, 
	const QGAMES::Rectangle& vZ, const QGAMES::Rectangle& mZ)
{
	QGAMES::Position result = QGAMES::Position::_cero;

	switch (nC)
	{
		case __BATTLESHIPII_SINUSOIDE1UFOSMOVEMENT__:
		case __BATTLESHIPII_SINUSOIDE2UFOSMOVEMENT__:
			result =
				QGAMES::Position (__BD (((rand () % 80) + 10) / __BD 100), // Between 10% and 90%
								  __BD (((rand () % 80) + 10) / __BD 100), __BD 0);
			break;

		default:
			result = BATTLESHIP::DataGame::defaultInitialPointForMovementType (nC, vZ, mZ);
	}

	return (result);
}

// ---
std::map <int, double> BattleshipII::DataGame::defaultParametersForMovementType (int nC, 
	QGAMES::bdata sF, const QGAMES::Rectangle& vZ, const QGAMES::Rectangle& mZ, const QGAMES::Position& iP,
	QGAMES::bdata& tR)
{
	std::map <int, double> result { };
	tR = __BD 0; // Nothing...

	switch (nC)
	{
		// Range 70
		// Amplitud 55
		// From 0 to 4PI
		case __BATTLESHIPII_SINUSOIDE1UFOSMOVEMENT__ :
		case __BATTLESHIPII_SINUSOIDE3UFOSMOVEMENT__ :
			{
				result = std::map <int, double> 
					{ { 0, 70.0f },
					  { 1, 55.0f },
					  { 2, (double) sF },
					  { 3, 0.0 }, { 4, 0.0 }, { 5, 1.0 }, // XY Plane...
					  { 6, 0.0 },
					  { 7, (double) ((__BD 2 * __PI) + (__BD 2 * __PI)) }
					};

				if (nC == __BATTLESHIPII_SINUSOIDE3UFOSMOVEMENT__) tR = -__PI;
			}

			break;

		// Range 70
		// Amplitud 55
		// From 0 to 4PI
		case __BATTLESHIPII_SINUSOIDE2UFOSMOVEMENT__ :
		case __BATTLESHIPII_SINUSOIDE4UFOSMOVEMENT__ :
			{
				result = std::map <int, double> 
					{ { 0, 70.0f },
					  { 1, 55.0f },
					  { 2, (double) sF },
					  { 3, 0.0 }, { 4, 0.0 }, { 5, 1.0 }, // XY Plane...
					  { 6, (double) (__PI / __BD 2) },
					  { 7, (double) ((__BD 5 * __PI / __BD 2) + (__BD 2 * __PI)) }
					};

				if (nC == __BATTLESHIPII_SINUSOIDE4UFOSMOVEMENT__) tR = -__PI;
			}

			break;

		default:
			result = BATTLESHIP::DataGame::defaultParametersForMovementType (nC, sF, vZ, mZ, iP, tR);
	}

	return (result);
}

// ---
const std::map <int, int>& BattleshipII::DataGame::curveforMovementTypeMap () const
{
	if (_curveForMovementTypeMap.empty ())
	{
		_curveForMovementTypeMap = BATTLESHIP::DataGame::curveforMovementTypeMap ();
	
		// Add 4 additional movements and their associated curve!
		_curveForMovementTypeMap [__BATTLESHIPII_SINUSOIDE1UFOSMOVEMENT__] = __BATTLESHIPII_SINUSOIDEGOANDBACK__;
		_curveForMovementTypeMap [__BATTLESHIPII_SINUSOIDE2UFOSMOVEMENT__] = __BATTLESHIPII_SINUSOIDEGOANDBACK__;
		_curveForMovementTypeMap [__BATTLESHIPII_SINUSOIDE3UFOSMOVEMENT__] = __BATTLESHIPII_SINUSOIDEGOANDBACK__;
		_curveForMovementTypeMap [__BATTLESHIPII_SINUSOIDE4UFOSMOVEMENT__] = __BATTLESHIPII_SINUSOIDEGOANDBACK__;
	}

	return (_curveForMovementTypeMap);
}


// ---
BattleshipII::Game::Game ()
	: BATTLESHIP::Game (),
	  _showFPS (false) // Not to show by default...
{ 
	delete (_theDataGame);

	_theDataGame = new BattleshipII::DataGame;
}

// ---
void BattleshipII::Game::Conf::adjustToPlayers (int nP)
{
	BATTLESHIP::Game::Conf::adjustToPlayers (nP);
	for (int i = 1; i <= numberPlayers (); setNumberLives (i++, _maxLives)); // Set the number of lives per player...
}

// ---
void BattleshipII::Game::setShowFPS (bool s)
{
	_showFPS = s;

	QGAMES::AdvancedGameStateControl* gCS = 
		dynamic_cast <QGAMES::AdvancedGameStateControl*> 
			(gameStateBuilder () -> gameState (__BATTLESHIP_STANDARDCONTROLGAMESTATE__));
	assert (gCS);

	gCS -> setOverlay 
		(_showFPS ? game () -> state (__QGAMES_GAMESTATEFPSNAME__) : NULL); 
}

// ---
void BattleshipII::Game::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __QGAMES_KEYBOARDEVENT__)
	{
		QGAMES::KeyBoardEventData* dt = (QGAMES::KeyBoardEventData*) evnt.data ();
		if (dt -> _internalCode == QGAMES::KeyCode::QGAMES_F1 && !dt -> _on)
			setShowFPS (!_showFPS); 
	}

	BATTLESHIP::Game::processEvent (evnt);
}

// ---
QGAMES::Game::Configuration* BattleshipII::Game::createConfiguration ()
{ 
	return (new Conf 
		(1 /** Default players */, std::atoi (parameter (__BATTLESHIPII_MAXNUMBEROFLIVESPARAMETER__).c_str ()))); 
}

// ---
std::string BattleshipII::Game::defaultParameter (const std::string& p) const
{
	std::string result (__NULL_STRING__);
	if (p == std::string (__BATTLESHIPII_MAXNUMBEROFLIVESPARAMETER__))
		result = std::string (__BATTLESHIPII_DEFAULTMAXNUMBEROFLIVESPARAMETER__);
	else
	if (p == std::string (__BATTLESHIPII_SHOWFPSPARAMETER__))
		result = std::string (__BATTLESHIPII_DEFAULTSHOWFPSPARAMETER__);
	else
		result = BATTLESHIP::Game::defaultParameter (p);
	return (result);
}

// ---
void BattleshipII::Game::initialize ()
{
	if (isInitialized ())
		return; // Only once...

	BATTLESHIP::Game::initialize ();

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
			(gameStateBuilder () -> gameState (__BATTLESHIP_STANDARDCONTROLGAMESTATE__));
	assert (gCS);

#ifndef NDEBUG
	setShowFPS (true);
#else
	std::string sfps = parameter (std::string (__BATTLESHIPII_SHOWFPSPARAMETER__));
	setShowFPS (QGAMES::toUpper (QGAMES::trim (sfps)) == std::string (__YES_STRING__));
#endif

	setState (gCS); // To start...

	observe (inputHandler ());
}

// ---
void BattleshipII::Game::finalize ()
{
	unObserve (inputHandler ());

	BATTLESHIP::Game::finalize ();
}
