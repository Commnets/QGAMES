#include "Game.hpp"
#include "EntitiesBuilder.hpp"
#include "MovementsBuilder.hpp"
#include "MapsBuilder.hpp"
#include "WorldsBuilder.hpp"
#include "Orchild.hpp"
#include "Guardian.hpp"
#include "Movements.hpp"
#include "GameStates.hpp"
#include "InputHandler.hpp"
#include "Defs.hpp"
#include <SDL/sdlgame.hpp>

// ---
SabreWulfGame::SabreWulfGame ()
	 : QGAMES::ArcadeGame (new SDLGame (/** No FMOD. */), QGAMES::Worlds ()),
	  _levels (),
	  _level (0), // The level by default...	  
	  _scoreFrame (NULL), _energy (NULL), _lives (NULL),
	  _timeCounter (NULL), _scoreCounter (NULL),
	  _energyPercentage (100),
	  _numberOfLives (__INITIALNUMBEROFLIVES__),
	  _numberOfSeconds (0),
	  _scoreNumber (0),
	  _countEnergy (0),
	  _counterPerSecond (0),
	  _n (0)
{ 
#ifdef NDEBUG
	// If the data parameter exists, then changes the default temporal file...
	// It makes sen se only when running release version of the app!
	if (dynamic_cast <QGAMES::InZipResourceReader*> (QGAMES::ResourceReader::resourceReader ()) != NULL)
		((QGAMES::InZipResourceReader*) QGAMES::ResourceReader::resourceReader ()) -> 
			setTempLocation (parameter (std::string (__GAME_DATADIRPROPERTYNAME__)));
#endif

	// Creates the levels...
	_levels.resize (3);
	
	Level l1;
	l1._level = 1;
	l1._maxTimeToAppearNasties = 2;
	l1._maxNumberOfAppearances = 1;
	l1._maxNumberOfAppearingMonsters = 3;
	l1._percentUpPerFood = 5;
	l1._pointsUpPerFood = 140;
	l1._percentDownPerMonster = 1;
	l1._pointsUpWhenKillMonster = 200;
	l1._secondsInNotNormalState = 5;
	l1._secondsBetweenBlinks = 0.25f;
	l1._numberOfBlinks = 4;
	l1._secondsToGrowOrchild = 6;
	l1._secondsOrchildBig = 2;
	_levels [0] = l1;
	
	Level l2;
	l2._level = 2;
	l2._maxTimeToAppearNasties = 1;
	l2._maxNumberOfAppearances = 1;
	l2._maxNumberOfAppearingMonsters = 3;
	l2._percentUpPerFood = 3;
	l2._pointsUpPerFood = 160;
	l2._percentDownPerMonster = 1;
	l2._pointsUpWhenKillMonster = 300;
	l2._secondsInNotNormalState = 4;
	l2._secondsBetweenBlinks = 0.25f;
	l2._numberOfBlinks = 3;
	l2._secondsToGrowOrchild = 5;
	l2._secondsOrchildBig = 2;
	_levels [1] = l2;
	
	Level l3;
	l3._level = 3;
	l3._maxTimeToAppearNasties = 1;
	l3._maxNumberOfAppearances = 1;
	l3._maxNumberOfAppearingMonsters = 3;
	l3._percentUpPerFood = 1;
	l3._pointsUpPerFood = 200;
	l3._percentDownPerMonster = 2;
	l3._pointsUpWhenKillMonster = 400;
	l3._secondsInNotNormalState = 4;
	l3._secondsBetweenBlinks = 0.15f;
	l3._numberOfBlinks = 2;
	l3._secondsToGrowOrchild = 4;
	l3._secondsOrchildBig = 2;
	_levels [2] = l3;
}

// ---
void SabreWulfGame::resetGame ()
{
	assert (_scoreFrame);
	assert (_energy);
	assert (_timeCounter);
	assert (_scoreCounter);
	assert (_activeWorld);

	// The internal status variables are set to their initial values...
	_numberOfLives = __INITIALNUMBEROFLIVES__;
	_lives -> setLives (_numberOfLives);

	// The level doesn't change...
	
	// The score objects are also initialized...
	// Init the energy...
	_energyPercentage = 100;
	_energy -> setPercentage (_energyPercentage);
	// Initialize the time...
	_numberOfSeconds = 0; // At the beggning no time has been consumed... 
	_timeCounter -> setSeconds (_numberOfSeconds);
	// Initialize the score..
	_scoreNumber = 0; // At the beggining there is no puntuation...
	_scoreCounter -> setNumber (_numberOfSeconds);

	// Initialize the things (set tke keys...), and the room number...
	((SabreWulfWorld*) _activeWorld) -> initializeThings ();
	((SabreWulfWorld*) _activeWorld) -> setMazePlaceNumber (__FIRSTMAZEPLACE__);
	// ...the main character is not carrying anything...
	((Sabreman*) artist (__SABREWULFSABREMAN__)) -> leaveAmulets ();
	// Init the room number...
	((SabreWulfWorld*) _activeWorld) -> setMazePlaceNumber (__FIRSTMAZEPLACE__);
	// The player has to be at the middle of the screen, it that first screen...
	Sabreman* sM = (Sabreman*) artist (__SABREWULFSABREMAN__);
	sM -> setPosition (QGAMES::Position (
		__BD ((__SCREENWIDTH__ - sM -> currentForm () -> frameWidth ()) >> 1),
		__BD ((__SCREENHEIGHT__ - sM -> currentForm () -> frameHeight ()) >> 1), __BD 0));
}

// ---
void SabreWulfGame::setLevel (int l)
{
	assert (l < 3);
	_level = l;
}

// ---
void SabreWulfGame::setJoystick (bool j)
{
	assert (_inputHandler);
	((InputHandler*) _inputHandler -> behaviour ()) -> activateJoystick (j);
}

// ---
int SabreWulfGame::mazePlaceNumber () const
{
	return ((_activeWorld == NULL) // No way to know the current maze place...
		? 0 : ((SabreWulfWorld*) _activeWorld) -> mazePlaceNumber ());
}

// ---
void SabreWulfGame::setMazePlaceNumber (int nR)
{
	assert (_activeWorld);
	((SabreWulfWorld*) _activeWorld) -> setMazePlaceNumber (nR);
}

// ---
void SabreWulfGame::setEnergyAvailable (int e)
{
	assert (_energy);

	_energyPercentage = e;
	if (_energyPercentage < 0)
		_energyPercentage = 0;
	if (_energyPercentage > 100)
		_energyPercentage = 100;

	_energy -> setPercentage (_energyPercentage);
}

// ---
void SabreWulfGame::setNumberOfLives (int nL)
{
	assert (_lives);

	_numberOfLives = nL;
	if (_numberOfLives < 0) 
		_numberOfLives = 0;
	if (_numberOfLives > __MAXNUMBEROFLIVES__) 
		_numberOfLives = __MAXNUMBEROFLIVES__;

	_lives -> setLives (_numberOfLives - 1); // Always one less (the other one is being playing...
}

// ---
void SabreWulfGame::setNumberOfSeconds (int nS)
{
	assert (_timeCounter);

	_numberOfSeconds = nS;
	_timeCounter -> setSeconds (nS);
}

// ---
void SabreWulfGame::setNumberOfPoints (int nP)
{
	assert (_scoreCounter);

	_scoreNumber = nP;
	_scoreCounter -> setNumber (nP);
}

// ---
int SabreWulfGame::percentagePlayed () const
{
	assert (_activeWorld);
	return (((SabreWulfWorld*) _activeWorld) -> percentagePlayed ());
}

// ---
void SabreWulfGame::fight ()
{
	((Sabreman*) artist (__SABREWULFSABREMAN__)) -> setFighting (true);
}

// ---
void SabreWulfGame::noFight ()
{
	((Sabreman*) artist (__SABREWULFSABREMAN__)) -> setFighting (false);
}

// ---
bool SabreWulfGame::isFighting ()
{
	return (((Sabreman*) artist (__SABREWULFSABREMAN__)) -> isFighting ());
}

// ---
void SabreWulfGame::updatePositions ()
{
	assert (_currentGameState);

	// If the state is not playing ...makes no sense to actualize the states..
	if (_currentGameState -> type () != __SABREWULFPLAYINGSTATE__ &&
		_currentGameState -> type () != __SABREWULFDIESTATE__ &&
		_currentGameState -> type () != __SABREWULFFLYDIESTATE__ &&
		_currentGameState -> type () != __SABREWULFDIEFOREVERSTATE__)
		return; // This is the reason nothing moves unless you're playing!

	// The things are moved...
	QGAMES::ArcadeGame::updatePositions ();
	// Then the collisions are detected!
	detectCollisions ();

	// Counts down the energy...represented in the chicken...
	// a 1 percent is lot per second...
	// This is something constant and can not be change using the configuration level...
	// The energy only goes down when sabreman is visible!
	Sabreman* s = (Sabreman*) artist (__SABREWULFSABREMAN__); 
	if (s -> isVisible ())
	{
		if (_countEnergy++ > (__SPEEDLOSSINGLIVE * framesPerSecond ()))
		{
			_countEnergy = 0;
			setEnergyAvailable (energyAvailable () - 
				(s -> isFighting () ? 2 : 1)); // Fighting cosumes double energy...
			if (energyAvailable () <= 0) // It is going to die...
				playerDies ();
		}
	}

	// Counts per second...
	// Even when sabreman is not visible!
	if (_counterPerSecond++ > (1 * framesPerSecond ()))
	{
		_counterPerSecond = 0;
		setNumberOfSeconds (numberOfSeconds () + 1);
		if (numberOfSeconds () >= 3600) /** 1 hourt maximum... the game finishes */
			setState (std::string (__SABREWULFDIESTATENAME__));
	}
}

// ---
void SabreWulfGame::addScoreObjects ()
{
	if (!_scoreObjects.empty ())
		return; // The object has been already created...

	// Add the frame around all the score elements...
	_scoreFrame = new SabreWulfScoreFrame;
	_scoreFrame -> setPosition (QGAMES::Position (__BD 0, __BD 0, __BD 0));
	_scoreFrame -> initialize ();

	// Adds the chicken of energy...
	_energyPercentage = 100;
	_energy = new SabreWulfEnergy;
	_energy -> setPosition (QGAMES::Position 
		(__BD __MAPORIGINX__ + 76, __BD (__MAPORIGINY__ + (__ORIGINALSPECTRUMHEIGHT__ << 1) - 10), __BD 0));
	_energy -> initialize ();
	setEnergyAvailable (100); // Always full when adding...

	// Adds the score object to indicate the lives...
	_numberOfLives = __INITIALNUMBEROFLIVES__;
	_lives = new SabreWulfLives;
	_lives -> setPosition (QGAMES::Position 
		(__BD (__MAPORIGINX__ + __ORIGINALSPECTRUMWIDTH__), __BD __MAPORIGINY__, __BD 0));
	_lives -> initialize ();
	setNumberOfLives (_numberOfLives); // Converves the last number of lives declared...

	// Adds the score object to count time...
	_numberOfSeconds = 0;
	_timeCounter = new SabreWulfTimeCounter;
	_timeCounter -> setPosition (QGAMES::Position 
		(__BD (__MAPORIGINX__ + ((__ORIGINALSPECTRUMWIDTH__ << 1) - 132)), 
			__BD __MAPORIGINY__, __BD 0));
	_timeCounter -> initialize ();
	_timeCounter -> setSpace (0);
	_timeCounter -> setMaxTextLength (5);
	_timeCounter -> setSeconds (_numberOfSeconds);

	// Adds the score to count points...
	_scoreNumber = 0;
	_scoreCounter = new SabreWulfScoreCounter;
	_scoreCounter -> setPosition (QGAMES::Position 
		(__BD __MAPORIGINX__, __BD __MAPORIGINY__, __BD 0));
	_scoreCounter -> initialize ();
	_scoreCounter -> setSpace (0);
	_scoreCounter -> setNumberLength (5);
	_scoreCounter -> setNumber (_scoreNumber);

	// Adds all objects to the list...
	_scoreObjects.insert (QGAMES::Entities::value_type (_scoreFrame -> id (), _scoreFrame));
	_scoreObjects.insert (QGAMES::Entities::value_type (_energy -> id (), _energy));
	_scoreObjects.insert (QGAMES::Entities::value_type (_lives -> id (), _lives));
	_scoreObjects.insert (QGAMES::Entities::value_type (_timeCounter -> id (), _timeCounter));
	_scoreObjects.insert (QGAMES::Entities::value_type (_scoreCounter -> id (), _scoreCounter));
}

// ---
void SabreWulfGame::removeScoreObjects ()
{
	QGAMES::ArcadeGame::removeScoreObjects ();

	delete (_scoreFrame);
	_scoreFrame = NULL;
	delete (_energy);
	_energy = NULL;
	delete (_lives);
	_lives = NULL;
	delete (_timeCounter);
	_timeCounter = NULL;
	delete (_scoreCounter);
	_scoreCounter = NULL;
	// The objects are set to NULL, because this method can be
	// invoked twice in a programming mistake...
}

// ---
#ifndef NDEBUG
void SabreWulfGame::onOffDrawLimits ()
{
	assert (_activeWorld);
	((SabreWulfWorld*) _activeWorld) -> onOffDrawLimits (); // No draw or draw the limits of the scene...
	((Sabreman*) entity (__SABREWULFSABREMAN__)) -> onOffDrawLimits (); // Nor draw or draw the limits of the sabreman
}
#endif
// ---

// ---
void SabreWulfGame::drawOn (QGAMES::Screen* s)
{
	if (_currentGameState != NULL && 
		(_currentGameState -> type () == __SABREWULFAMULETSTATE__ || 
		 _currentGameState -> type () == __SABREWULFENDSTATE__ ||
		 _currentGameState -> type () == __SABREWULFWINSTATE__))
		QGAMES::Game::drawOn (s); // It doesn't draw the world, just the score objects!
	else
		QGAMES::ArcadeGame::drawOn (s); // Draws everything...
}

// ---
QGAMES::EntityBuilder* SabreWulfGame::createEntityBuilder ()
{ 
	return (new EntitiesBuilder (parameter (__GAME_PROPERTYENTITIESFILE__), 
		formBuilder (), movementBuilder ())); 
}

// ---
QGAMES::MovementBuilder* SabreWulfGame::createMovementBuilder () 
{ 
	return (new MovementsBuilder (parameter (__GAME_PROPERTYMOVEMENTSFILE__))); 
}

// ---
QGAMES::InputHandler* SabreWulfGame::createInputHandler ()
{
	return (implementation () -> createInputHandler (new InputHandler ()));
}

// ---
QGAMES::Screens SabreWulfGame::createScreens ()
{ 
	QGAMES::Screens r;
	QGAMES::Screen* scr = implementation () -> createScreen (std::string (__GAMESNAME__),
		QGAMES::Position (0,0), __SCREENWIDTH__, __SCREENHEIGHT__, __SCREENXPOS__, __SCREENYPOS__);
	assert (scr); // Just in case. It should happen anything but...!
	scr -> windowAtCenter (); // To center the screen...
	r.insert (QGAMES::Screens::value_type (__QGAMES_MAINSCREEN__, scr));
	return (r); 
}

// ---
QGAMES::WorldBuilder* SabreWulfGame::createWorldBuilder ()
{
	return (new WorldsBuilder (parameter (__GAME_PROPERTYWORLDSFILE__), mapBuilder ()));
}

// ---
QGAMES::MapBuilder* SabreWulfGame::createMapBuilder ()
{
	QGAMES::MapBuilder* result = new QGAMES::MapBuilder (parameter (__GAME_MAPSOBJECTSFILE__));
	result -> addAddsOn (new MapsBuilderAddsOn (objectBuilder ()));
	return (result);
}

// ---
void SabreWulfGame::initialize ()
{
	QGAMES::ArcadeGame::initialize ();

	// Sets an icon to the screen...
	mainScreen () -> setIcon (formBuilder () -> form (__SABREWULFICO__));

	// There is only one world being part of sabrewulf game...
	_worlds.insert (QGAMES::Worlds::value_type (__SABREWULFWORLD__,
		_worldBuilder -> world (__SABREWULFWORLD__)));

	// Adds the entities...
	// The main character
	addArtist (__SABREWULFSABREMAN__);
	// The orchild
	addArtist (__SABREWULFORCHILD__);
	// The objects. There could be a maximum of three per maze place
	// 2 for the objects to eat, and more more in some cases
	// with a piece of the amulet to be able to exit!
	for (int i = 0; i < __SABREWULFOBJECTSPERPLACE__; i++)
		addArtist (__SABREWULFOBJECTSARTIST__ + i);
	// The nasties...
	for (int i = 0; i < __SABREWULFMAXNUMBERNASTIES__; i++)
		addArtist (__SABREWULFNASTIESBASE__ + i);
	// The fire...
	addArtist (__SABREWULFFIRE__);
	// The inmortals...
	for (int i = 0; i < __SABREWULFNUMBERINMORTALS__; i++)
		addArtist (__SABREWULFINMORTALSBASE__ + i);
	// The guardian
	addArtist (__SABREWULFGUARDIAN__);

	// Adds the score objects...
	addScoreObjects ();

	// Inserts the states of the game...
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__SABREWULFINTROSTATENAME__), new SabreWulfIntroState (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__SABREWULFSELECTSTATENAME__), new SabreWulfSelectionState (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__SABREWULFDEMOSTATENAME__), new SabreWulfDemoState (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__SABREWULFSTARTSTATENAME__), new SabreWulfStartState (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__SABREWULFPLAYINGSTATENAME__), new SabreWulfPlayingState (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__SABREWULFDIESTATENAME__), new SabreWulfDieState (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__SABREWULFENDSTATENAME__), new SabreWulfEndState (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__SABREWULFWINSTATENAME__), new SabreWulfWinState (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__SABREWULFAMULETSTATENAME__), new SabreWulfAmuletState (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__SABREWULFFLYDIESTATENAME__), new SabreWulfFlyingDie (this)));
	_gameStates.insert (QGAMES::GameStates::value_type 
		(std::string (__SABREWULFDIEFOREVERSTATENAME__), new SabreWulfDieForEverState (this)));

	// States the world...and initialize it...
	setWorld (__SABREWULFWORLD__);
	_activeWorld -> initialize ();

	// The initial state...
	setState (std::string (__SABREWULFINTROSTATENAME__));
}

// ---
void SabreWulfGame::finalize ()
{
	removeScoreObjects ();
	// The only thing to do just in case...
}

// ---
void SabreWulfGame::detectCollisions ()
{
	Sabreman* player = (Sabreman*) artist (__SABREWULFSABREMAN__);
	if (!player -> isVisible ())
		return; // There are no collisions possible when sabreman is not visible!

	// First thing is to know whether the player is interacting or
	// Not with an orchild. In that case, the consecuences depends on
	// the orchild...
	Orchild* o = (Orchild*) artist (__SABREWULFORCHILD__);
	if ((o -> isBig () && player -> hasCollisionWith (o)) && 
			player -> internalState () != Sabreman::_STOPPED) // Only when it is big, affects the player!, and only when he's moving...
		playerCollisionWithOrchild ();

	// Second thing is to know the collisions among the player and the different objects...
	for (int i = 0; i < __SABREWULFOBJECTSPERPLACE__; i++)
	{
		SabreWulfObject* obj = (SabreWulfObject*) artist (__SABREWULFOBJECTSARTIST__ + i);
		if (obj -> isVisible () && player -> hasCollisionWith (obj))
			playerCollisionWithObject (obj);
	}

	// Then, it is time to analize the collisions with the nastie objects...
	for (int i = 0; i < __SABREWULFMAXNUMBERNASTIES__; i++)
	{
		SabreWulfNastie* mt = (SabreWulfNastie*) artist (__SABREWULFNASTIESBASE__ + i);
		if (mt -> isVisible () && mt -> state () == SabreWulfMonster::State::_MOVING &&
				player -> hasCollisionWith (mt))
			playerCollisionWithNastie (mt);
	}

	// Then detects the collision with the fire...
	SabreWulfNastie* fr = (SabreWulfNastie*) artist (__SABREWULFFIRE__);
	if (fr -> isVisible () && fr -> state () == SabreWulfMonster::State::_MOVING &&
			player -> hasCollisionWith (fr))
		playerCollisionWithFire ();

	// Now it is time to test the collisions with inmortals
	// What happen when the collisions occurrs could be different depending on
	// the inmortal sabreman collisions with and the state of sabreman
	for (int i = 0; i < __SABREWULFNUMBERINMORTALS__; i++)
	{
		SabreWulfInmortal* iM = (SabreWulfInmortal*) artist (__SABREWULFINMORTALSBASE__ + i);
		if (iM -> isVisible () && player -> hasCollisionWith (iM))
			playerCollisionWithInmortal (iM);
	}

	// ...Third detects the collision with the guardian!
	SabreWulfGuardian* g = (SabreWulfGuardian*) artist (__SABREWULFGUARDIAN__);
	if (g -> isVisible () && player -> hasCollisionWith (g))
		playerCollisionWithGuardian ();
}

// ---
void SabreWulfGame::playerCollisionWithOrchild ()
{
	#ifndef NDEBUG
	std::cout << _n++ << " Collision with orchild" << std::endl;
	#endif

	// Depending on the type of the orchild,
	// The effect will be one or other over the player...
	// The same is with the orchild!
	Sabreman* s = (Sabreman*) artist (__SABREWULFSABREMAN__);
	Orchild* o = (Orchild*) artist (__SABREWULFORCHILD__);
	s -> whenCollisionWith (o); o -> whenCollisionWith (s);
}

// ---
void SabreWulfGame::playerCollisionWithObject (SabreWulfObject* obj)
{
	assert (obj);

	#ifndef NDEBUG
	std::cout << _n++ << " Collision with object: " << obj -> type () << std::endl;
	#endif

	// Depending on what the player is collisioning with
	// The effects will be different...
	std::string nStatus (__NULL_STRING__); // Do not change the game status by default
	Sabreman* o = (Sabreman*) artist (__SABREWULFSABREMAN__);
	// Incremenmts the energy available depending on the level
	setEnergyAvailable (energyAvailable () + levelInfo ()._percentUpPerFood);
	// Adds new points, depending on the level too
	setNumberOfPoints (numberOfPoints () + levelInfo ()._pointsUpPerFood);
	// Anyway... what happen in the object when collisions? (it dissapears)
	obj -> whenCollisionWith (o);
	// ...but nothing else happens to sabreman..or not?
	o -> whenCollisionWith (obj);

	// Special actions, depending on the object type, sabreman is collisioning with
	switch (obj -> type ()) 
	{
		case SabreWulfObject::Type::_AMULET:
			// Piece got!
			o -> setAmultePiece (obj -> amuletPieceNumber ());
			// The temporal status...
			nStatus = std::string (__SABREWULFAMULETSTATENAME__);
			break;

		case SabreWulfObject::Type::_BALL:
		case SabreWulfObject::Type::_CHEST:
		case SabreWulfObject::Type::_DIAMOND:
		case SabreWulfObject::Type::_DOLLARS:
		case SabreWulfObject::Type::_HAT:
		case SabreWulfObject::Type::_RING:
		case SabreWulfObject::Type::_SYRINGE:
			sound (__SABREMANGETSOBJECT__) -> play (__SABREWULF_BACKGROUNDCHANNEL__);
			break;

		case SabreWulfObject::Type::_LIFE:
			// Double points and energy... 
			setEnergyAvailable (energyAvailable () + levelInfo ()._percentUpPerFood);
			setNumberOfPoints (numberOfPoints () + levelInfo ()._pointsUpPerFood);
			// Additionally the lives moves up one...
			setNumberOfLives (numberOfLives () + 1); 
			break;

		default:
			exit (1); // This situation can't happen...
	}

	// Change the state of the game if needed...
	if (nStatus != std::string (__NULL_STRING__))
		setState (nStatus);
}

// ---
void SabreWulfGame::playerCollisionWithNastie (SabreWulfNastie* nt)
{
	assert (nt);

	#ifndef NDEBUG
	std::cout << _n++ << " Collision with nastie " << nt -> id () << std::endl;
	#endif

	// Even fighting, the result of the combat will
	// depend whether sabreman is fighting in the direction the nastie is comming to sabreman
	std::string nStatus (__NULL_STRING__); // Do not change the game status
	Sabreman* o = (Sabreman*) artist (__SABREWULFSABREMAN__);
	// When the nastie collisions with sabreman, it dissapear...just if it is possible!
	nt -> whenCollisionWith (o);
	// ...and nothing special more happens to sabreman...
	o -> whenCollisionWith (nt);

	// Special effects depending what sabreman is doing!
	if (o -> isFighting ())
	{
		if (o -> couldKill (nt)) // Depending on the position of the two elements, he can kill or not!
		{
			setNumberOfPoints (numberOfPoints () + 
				levelInfo ()._pointsUpWhenKillMonster); // Adds points...
		}
		// If the collision has happen and due to the relative
		// position between monster and sabreman, it could not be killed
		// then sabreman dies...
		else
		{
			#ifndef NDEBUG
			std::cout << "Sabreman dies" << std::endl;
			#endif

			nStatus = std::string (__SABREWULFFLYDIESTATENAME__);
		}
	}
	// When the collision happens and sabreman is not fighting, he dies!
	else
	if (o -> canDie ())
	{
		#ifndef NDEBUG
		std::cout << "Sabreman dies" << std::endl;
		#endif

		nStatus = std::string (__SABREWULFFLYDIESTATENAME__);
	}

	// Change the state of the game if needed...
	if (nStatus != std::string (__NULL_STRING__))
		setState (nStatus);
}

// ---
void SabreWulfGame::playerCollisionWithFire ()
{
	#ifndef NDEBUG
	std::cout << _n++ << " Collision with fire" << std::endl;
	#endif

	playerCollisionWithNastie ((SabreWulfNastie*) artist (__SABREWULFFIRE__));
}

// ---
void SabreWulfGame::playerCollisionWithInmortal (SabreWulfInmortal* iM)
{
	assert (iM);

	#ifndef NDEBUG
	std::cout << _n++ << " Collision with inmortal " << iM -> id () << std::endl;
	#endif

	// Even fighting, the result of the combat will
	// depend whether sabreman is fighting in the direction the nastie is comming to sabreman
	std::string nStatus (__NULL_STRING__); // Do not change the game status
	Sabreman* o = (Sabreman*) artist (__SABREWULFSABREMAN__);
	// When the inmortal collisions with sabreman, what happens depends on the inmortal!
	iM -> whenCollisionWith (o);
	// ...and nothing special more happens to sabreman...or not!
	o -> whenCollisionWith (iM);

	// Special things depending the relative situation of the inmortal
	// and sabreman!
	if (o -> isVisible () && o -> canDie () && 
		(!o -> isFighting () || !o -> couldDamage (iM)))
	{
		#ifndef NDEBUG
		std::cout << "Sabreman dies" << std::endl;
		#endif

		nStatus = std::string (__SABREWULFFLYDIESTATENAME__);
	}

	// Change the state of the game if needed...
	if (nStatus != std::string (__NULL_STRING__))
		setState (nStatus);
}

// ---
void SabreWulfGame::playerCollisionWithGuardian ()
{
	#ifndef NDEBUG
	std::cout << _n++ << " Collision with guardian" << std::endl;
	#endif

	// ...the player can't touch guardian...in that case he dies!
	setState (__SABREWULFFLYDIESTATENAME__);
}

// ---
void SabreWulfGame::playerDies ()
{
	#ifndef NDEBUG
	std::cout << "Player dies" << std::endl;
	#endif

	// The state of the game is dieing...
	setState (std::string (__SABREWULFDIESTATENAME__));
}
