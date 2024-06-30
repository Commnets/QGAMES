#include "Worlds.hpp"
#include "Scenes.hpp"
#include "Sabreman.hpp"
#include "Orchild.hpp"
#include "Guardian.hpp"
#include "Sabrewulf.hpp"
#include "Game.hpp"

// ---
SabreWulfWorld::SabreWulfWorld (const QGAMES::Scenes& s, const QGAMES::WorldProperties& p)
	: QGAMES::World (__SABREWULFWORLD__, s, p),
	  _mazePlaceNumber (__FIRSTMAZEPLACE__),
	  _mainCharacter (NULL),
	  _orchilds (),
	  _orchild (NULL),
	  _objects (),
	  _guardian (NULL),
	  _visitedMazePlaces (),
	  _counterToFire (0)
{ 
	QGAMES::Buoys lB;
	lB.insert (QGAMES::Buoys::value_type 
		(__SABREWULFTOCHANGEROOMBUOYID__, new SabreWulfWorld::ToChangeMazePlaceBuoy));
	lB.insert (QGAMES::Buoys::value_type 
		(__SABREWULFTOEXITMAZEBUOYID__, new SabreWulfWorld::ToExitMazeBuoy));
	lB.insert (QGAMES::Buoys::value_type 
		(__INMORTALTOCHANGEROOMBUOYID__, new SabreWulfWorld::InmortalToChangeMazePlaceBuoy));
	setBuoys (lB);
}

// ---
void SabreWulfWorld::setMazePlaceNumber (int r)
{
	assert (_activeScene);
	assert (_orchild);
	assert (r < __NUMBEROFMAZEPLACES__);

	_mazePlaceNumber = r;

	#ifndef NDEBUG
	std::cout << "Maze Place Number: " << _mazePlaceNumber << std::endl;
	#endif

	// Sets the situation in the scene...
	// With the real situation...
	// The position of the tombs is passed also as parameter
	((SabreWulfScene*) _activeScene) -> setMazePlaceNumber (r);
	// The orchild is initialized with the general status controlled here...
	_orchild -> actualizeFrom (_orchilds [_mazePlaceNumber]);
	// Actualize the forms of the objects...
	for (int i = 0; i < __SABREWULFOBJECTSPERPLACE__; i++)
		((SabreWulfObject*) (__AGM game ()) -> artist (__SABREWULFOBJECTSARTIST__ + i)) -> 
			actualizeFrom (_objects [_mazePlaceNumber][i]); // First could be null...
	
	// If sabreman enter the exit maze place, the guardian becomes visible...
	// ...and if he has all pieces of the amulet, the guardian moves either!
/*	_guardian -> setPosition (QGAMES::Position (
		__BD ((__SCREENWIDTH__ - _guardian -> currentForm () -> frameWidth ()) >> 1),
		__BD ((__SCREENHEIGHT__ - _guardian -> currentForm () -> frameHeight ()) >> 1), __BD 0));
	_guardian -> setVisible (_mazePlaceNumber == __LASTMAZEPLACE__);
	_guardian -> setMoves 
		(_mazePlaceNumber == __LASTMAZEPLACE__ && _mainCharacter -> isCarryingAmulet ());*/
}

// ---
int SabreWulfWorld::percentagePlayed () const
{
	return (std::count (_visitedMazePlaces.begin (), 
		_visitedMazePlaces.end (), true) * 100 / __NUMBEROFMAZEPLACES__);
}

// ---
void SabreWulfWorld::moveCharacter (const QGAMES::Vector& d)
{
	assert (_mainCharacter);
	_mainCharacter -> setMove (d * 
		((_mainCharacter -> internalState () == Sabreman::InternalState::_GOINGBACK) ? __BD -1 : __BD 1));
}

// ---
void SabreWulfWorld::killNasties ()
{
	// Kill nasties...
	for (int i = 0; i < __SABREWULFMAXNUMBERNASTIES__; i++)
		((SabreWulfNastie*) (__AGM game ()) -> artist (__SABREWULFNASTIESBASE__ + i)) -> kill ();
	// Kill the fire...
	((SabreWulfNastie*) (__AGM game ()) -> artist (__SABREWULFFIRE__)) -> kill ();
	_counterToFire = 0; // Start to count again for fire...
}

// ---
void SabreWulfWorld::initialize ()
{
	// The initial scene...
	setScene (__SABREWULFSCENE__);

	// Initialize the maze place
	_mazePlaceNumber = __FIRSTMAZEPLACE__;

	// Set up the main character...
	// It will be manage during the game
	_mainCharacter = 
		(Sabreman*) (__AGM game ()) -> artist (__SABREWULFSABREMAN__);
	_mainCharacter -> setCurrentMovement (__SABREWULFSABREMANMOVS__);
	_activeScene -> addEntity (_mainCharacter);

	// Set up the nasties...
	for (int i = 0; i < __SABREWULFMAXNUMBERNASTIES__; i++)
	{
		SabreWulfNastie* ntie = (SabreWulfNastie*)
			(__AGM game ()) -> artist (__SABREWULFNASTIESBASE__ + i);
		ntie -> setCurrentMovement (__MININT__);
		_activeScene -> addEntity (ntie);
	}

	// Setup the fire...
	SabreWulfNastie* fr = (SabreWulfNastie*)
		(__AGM game ()) -> artist (__SABREWULFFIRE__);
	fr -> setCurrentMovement (__MININT__);
	_activeScene -> addEntity (fr);

	// ...and then the other inmortals
	for (int i = 0;i < __SABREWULFNUMBERINMORTALS__; i++)
	{
		SabreWulfInmortal* mt = (SabreWulfInmortal*)
			(__AGM game ()) -> artist (__SABREWULFINMORTALSBASE__ + i);
		mt -> setCurrentMovement (__MININT__);
		_activeScene -> addEntity (mt);
	}

	// Sets up the guardian
	_guardian = (SabreWulfGuardian*) (__AGM game ()) -> artist (__SABREWULFGUARDIAN__);
	_guardian -> setCurrentMovement (__MININT__);
	_guardian -> setVisible (_mazePlaceNumber == __LASTMAZEPLACE__);
	_guardian -> setMoves 
		(_mazePlaceNumber == __LASTMAZEPLACE__ && _mainCharacter -> isCarryingAmulet ());
	_activeScene -> addEntity (_guardian);

	// Initialize all things in the game...
	// Orchilds, objects...
	initializeThings ();

	// Set up the orchild entity...
	_orchild = (Orchild*) (__AGM game ()) -> artist (__SABREWULFORCHILD__);
	_orchild -> setCurrentMovement (__MININT__); // Orchild doesn't move!
	_orchild -> actualizeFrom (_orchilds [_mazePlaceNumber]);
	_activeScene -> addEntity (_orchild);

	// Set up the elements, up to __SABREWULFOBJECTSPERPLACE__ (maximum) remember...
	for (int i = 0; i < __SABREWULFOBJECTSPERPLACE__; i++)
	{
		SabreWulfObject* obj = (SabreWulfObject*) 
			(__AGM game ()) -> artist (__SABREWULFOBJECTSARTIST__ + i);
		obj -> setCurrentMovement (__MININT__);
		obj -> actualizeFrom (_objects [_mazePlaceNumber][i]); // First object could be null...
		_activeScene -> addEntity (obj);
	}

	// Init the world...
	QGAMES::World::initialize ();

	// Sets the map of the main character...
	_mainCharacter -> setMap (_activeScene -> activeMap ());
	// Sets up the map of th orchild element...
	_orchild -> setMap (_activeScene -> activeMap ());
	// Adds the map to the objects...
	for (int i = 0; i < __SABREWULFOBJECTSPERPLACE__; i++)
		((SabreWulfObject*) (__AGM game ()) -> artist (__SABREWULFOBJECTSARTIST__ + i)) -> 
			setMap (_activeScene -> activeMap ());
	// Adds the map to the nasties...
	for (int i = 0; i < __SABREWULFMAXNUMBERNASTIES__; i++)
		((SabreWulfNastie*) (__AGM game ()) -> artist (__SABREWULFNASTIESBASE__  + i)) -> 
			setMap (_activeScene -> activeMap ());
	// Add the map to the fire...
	((SabreWulfNastie*) (__AGM game ()) -> artist (__SABREWULFFIRE__)) -> 
		setMap (_activeScene -> activeMap ());
	// ...and to the rest of inmortals...
	for (int i = 0; i < __SABREWULFNUMBERINMORTALS__; i++)
		((SabreWulfInmortal*) (__AGM game ()) -> artist (__SABREWULFINMORTALSBASE__  + i)) -> 
			setMap (_activeScene -> activeMap ());
	// Adds the map to the guardian...
	_guardian -> setMap (_activeScene -> activeMap ());

	// Set the position of the main elements...
	// The player has to be at the middle of the screen, inthe first screen...
	// That it is exactly the screen when the game starts...
	_mainCharacter -> setPosition (QGAMES::Position (
		__BD ((__SCREENWIDTH__ - _mainCharacter -> currentForm () -> frameWidth ()) >> 1),
		__BD ((__SCREENHEIGHT__ - _mainCharacter -> currentForm () -> frameHeight ()) >> 1), __BD 0));
	// ...and the guardina the same...
	_guardian -> setPosition (QGAMES::Position (
		__BD ((__SCREENWIDTH__ - _guardian -> currentForm () -> frameWidth ()) >> 1),
		__BD ((__SCREENHEIGHT__ - _guardian -> currentForm () -> frameHeight ()) >> 1), __BD 0));
	// The guardian is initially invisible
	_guardian -> setVisible (false);

	// After everything the initial maze place is set to the first one...
	setMazePlaceNumber (_mazePlaceNumber);
}

// ---
void SabreWulfWorld::initializeThings ()
{
	clearThings ();

	// ---
	// Initialize the orchilds...
	_orchilds.clear (); _orchilds.resize (__NUMBEROFMAZEPLACES__);
	for (int i = 0; i < __NUMBEROFMAZEPLACES__; i++)
	{
		SabreWulfWorld::OrchildCounter* ct = new SabreWulfWorld::OrchildCounter;

		ct -> _mazePlace = i;
		ct -> _color = SabreWulfMaze::_data._orchildTypes [rand () % __NUMBERPOSSIBILITIESORCHILD__];
		ct -> _position = QGAMES::Position 
			(__BD SabreWulfMaze::_data._orchildCoordinates [SabreWulfMaze::_data._mazePlaces [i]]._x,
			 __BD SabreWulfMaze::_data._orchildCoordinates [SabreWulfMaze::_data._mazePlaces [i]]._y, 
			 __BD 0);
		ct -> _secondsToGrow = (rand () % ((SabreWulfGame*) game ()) -> levelInfo ()._secondsToGrowOrchild) + 1;
		ct -> _secondsBig = (rand () % ((SabreWulfGame*) game ()) -> levelInfo ()._secondsOrchildBig) + 1;
		// The rest of the variables are initalized by default!

		_orchilds [i] = ct;
	}
	// ---

	// ---
	// Decide witch possibility for the amulets is the right one...
	// There are sereval ones defined in the standard game!!
	int amuletPos = rand () % __NUMBEROFAMULTERRANDPOS__;
	// Initialize the objects...
	_objects.clear (); _objects.resize (__NUMBEROFMAZEPLACES__);
	for (int i = 0; i < __NUMBEROFMAZEPLACES__; i++)
	{
		_objects [i] = SabreWulfWorld::ObjectCounters (__NUMBEROBJECTSPERMAZEPLACE__ + 1);
		for (int j = 0; j < (__NUMBEROBJECTSPERMAZEPLACE__ + 1); j++)
			_objects [i][j] = NULL;
	}

	// This counter is going to point which piece of the amulet is being set...
	int amuletPiece = 0; 
	// Sets the objects maze place per maze place...
	for (int i = 0; i < __NUMBEROFMAZEPLACES__; i++)
	{
		int adjustPos = -1;
		SabreWulfWorld::ObjectCounter* ct = NULL;
		// Is the current maze place a place where a piece must be also set?
		// The potential positions of the pieces in the maze have been sorted before
		// this method, when SabreWulfMaze class was built!
		if (amuletPiece < __NUMBEROFAMULETPIECES__ &&
			i == SabreWulfMaze::_data._amuletMazePlaces [amuletPos][amuletPiece])
		{
			ct = new SabreWulfWorld::ObjectCounter;
			ct -> _mazePlace = i;
			ct -> _type = __NUMBERPOSSIBILITIESOBJECT__ + amuletPiece; // The piece of the amulet code
			ct -> _position = QGAMES::Position (
				__BD SabreWulfMaze::_data._objectCoordinates 
					[SabreWulfMaze::_data._mazePlaces [i]][0]._x,
				__BD SabreWulfMaze::_data._objectCoordinates 
					[SabreWulfMaze::_data._mazePlaces [i]][0]._y,
				__BD 0);
			ct -> _caught = false; // Initially everything can be caught...
			amuletPiece++;
			adjustPos = 0;
		}

		_objects [i][0] = ct; // The first object could be null or a part of the amulet...

		// Sets the standard pieces...
		// There will be a random number of objects per maze place
		// These number willbe between 0 and __NUMBEROBJECTSPERMAZEPLACE__ (default 2)
		int iPos = rand () % __NUMBERPOSSIBLITIESFOROBJS__; // in which position must the amulet be set?
		if (iPos == 3) iPos = iPos - 1; // If the last one is choosen, then select the previous one...
		if (iPos == 0 && adjustPos == 0) iPos = iPos + 1; // If the selected position is the first and there was found an amulet, 
		for (int j = 1; j <= (rand () % (__NUMBEROBJECTSPERMAZEPLACE__ + 1)); j++)
		{
			SabreWulfWorld::ObjectCounter* ct = new SabreWulfWorld::ObjectCounter;
			ct -> _mazePlace = i;
			ct -> _type = SabreWulfMaze::_data._objectTypes [rand () % __NUMBERPOSSIBILITIESOBJECT__];
			ct -> _position = QGAMES::Position (
				__BD SabreWulfMaze::_data._objectCoordinates 
					[SabreWulfMaze::_data._mazePlaces [i]][iPos + (j - 1)]._x,
				__BD SabreWulfMaze::_data._objectCoordinates 
					[SabreWulfMaze::_data._mazePlaces [i]][iPos + (j - 1)]._y,
				__BD 0);
			ct -> _caught = false; // Initially everything can be caught...

			_objects [i][j + adjustPos] = ct;
		}
	}
	// ---

	// ---
	// Decide where the inmortals have to be at the beggining...
	for (int i = 0; i < __SABREWULFNUMBERINMORTALS__; i++)
	{
		SabreWulfInmortal* mt = (SabreWulfInmortal*)
			(__AGM game ()) -> artist (__SABREWULFINMORTALSBASE__ + i);
		mt -> initializeAs (SabreWulfMaze::_data._inmortalTypes [i]._type,
			SabreWulfMaze::_data._inmortalTypes [i]._location, 
			QGAMES::Position (__BD SabreWulfMaze::_data._inmortalTypes [i]._position._x, 
				__BD SabreWulfMaze::_data._inmortalTypes [i]._position._y, __BD 0), 
			_mainCharacter);
	}

	// Init the status of the visited maze places
	_visitedMazePlaces.clear ();
	_visitedMazePlaces.resize (__NUMBEROFMAZEPLACES__);
	for (int i = 0; i < __NUMBEROFMAZEPLACES__; i++)
		_visitedMazePlaces [i] = false;
}

// ---
void SabreWulfWorld::updatePositions ()
{
	assert (_activeScene);

	// Update the positions for all the orchilds...
	// This situation is done at world level.
	// The situation of every orchild is controlled here
	// As the orchild entity has a reference to the orchild counter
	// determinating its behaviour, then simply actualizing
	// it here, will modify the orchild status (@see updatePositions method in the orchild)
	for (int i = 0; i < __NUMBEROFMAZEPLACES__; i++)
	{
		// The orchild is already growing...
		if (_orchilds [i] -> _growing)
		{
			// Grows (or grows down) a little bit every four steps...
			if (_orchilds [i] -> _counter++ >= 4)
			{
				_orchilds [i] -> _counter = 0;
				_orchilds [i] -> _percentaje += (10 * _orchilds [i] -> _growingDirection);
				// When it is at its maximum devolopment, don't grow more!!
				// The flower will be in this situation for a while...
				if (_orchilds [i] -> _percentaje >= 100)
				{
					_orchilds [i] -> _percentaje = 100;
					_orchilds [i] -> _growing = false;
				}
				else
				// When it is really small, then stop growing down!!
				// The flower will be at this situation for a while...
				if (_orchilds [i] -> _percentaje <= 0)
				{
					_orchilds [i] -> _percentaje = 0;
					_orchilds [i] -> _growing = false;
				}
			}
		}
		// The orchild is not now growing...
		// It can be because it is big already, or because is really small...
		else
		{
			// If it is big
			// controls the time being big. After that time, the orchild starts
			// To grow down!!
			if (_orchilds [i] -> _percentaje == 100)
			{
				if (_orchilds [i] -> _counter++ >= 
					(_orchilds [i] -> _secondsBig * game () -> framesPerSecond ()))
				{
					_orchilds [i] -> _counter = 0;
					_orchilds [i] -> _growing = true;
					_orchilds [i] -> _growingDirection = -1; // Down...
				}
			}
			// If it is not big, controls the
			// time to start to grow
			// After that time starts to grow!!
			else
			{
				if (_orchilds [i] -> _counter++ >= 
					(_orchilds [i] -> _secondsToGrow * game () -> framesPerSecond ()))
				{
					_orchilds [i] -> _counter = 0;
					_orchilds [i] -> _growing = true;
					_orchilds [i] -> _growingDirection = 1;
				}
			}
		}
	}

	// Nasties are moved just in playing mode...
	// If there is a place for the maximum number of nasties in this maze place number...
	// a new has to grow...
	if (game () -> activeState () -> type () == __SABREWULFPLAYINGSTATE__)
	{
		// Moves the nasties...
		if (((SabreWulfScene*) _activeScene) -> numberOfNasties () != __NUMBERNASTIESPERPLACE__)
		{
			int nN = ((SabreWulfScene*) _activeScene) -> firstNastieFree ();
			if (nN != -1) // It could happen there was no free nastie place if sabreman moves quick!!
			{
				SabreWulfNastie* nT = 
					(SabreWulfNastie*) (__AGM game ()) -> artist (__SABREWULFNASTIESBASE__  + nN);
				SabreWulfMonster::Aspect a = (SabreWulfMonster::Aspect) (rand () % __NUMBERPOSIBILITIESNATIE__);
				nT -> initializeAs (a, _mazePlaceNumber, 
						((SabreWulfScene*) _activeScene) -> randomFreePosition (_mazePlaceNumber), _mainCharacter);
			}
		}

		// Moves the fire if needed...
		// When sabreman stays a lot in the same room (3 seconds)...
		if (_counterToFire++ > (3 * game () -> framesPerSecond ()))
		{
			_counterToFire = 0;
			SabreWulfNastie* fr = (SabreWulfNastie*) (__AGM game ()) -> artist (__SABREWULFFIRE__);
			if (!fr -> isVisible ())
				fr -> initializeAs (SabreWulfMonster::Aspect::_FIRE, _mazePlaceNumber, 
					_mainCharacter -> position (), _mainCharacter);
		}
	}

	// Then update the rest of the world!!
	QGAMES::World::updatePositions ();
}

// ---
void SabreWulfWorld::processEvent (const QGAMES::Event& e)
{
	assert (_mainCharacter);

	// When the main character is in the limits of the maze place...
	if (e.code () == __SABREMANOUTOFLIMITS__)
	{
		// Calculate where to go, and the new position...
		int toAdd = 0; 
		QGAMES::Vector toMove = QGAMES::Vector::_cero;
		QGAMES::Position refPos = _mainCharacter -> referenceZone (_mainCharacter -> orientation ());
		inLimitNewPlaceFor (_mainCharacter, refPos, toMove, toAdd);

		// Activate the buoy, to tretat the change...
		_visitedMazePlaces [mazePlaceNumber ()] = true; // The place has been visited...
		SabreWulfChangeMazePlaceEventData* eData = new SabreWulfChangeMazePlaceEventData;
		eData -> _mazePlace = mazePlaceNumber () + toAdd;
		eData -> _position = _mainCharacter -> position () + toMove;
		SabreWulfWorld::ToChangeMazePlaceBuoy* b = 
			(SabreWulfWorld::ToChangeMazePlaceBuoy*) buoy (__SABREWULFTOCHANGEROOMBUOYID__);
		b -> setData (eData); // The buoy destructor will destroy the data also!
		b -> active (true);
		
		// Kill the fire...if any...
		_counterToFire = 0;
		((SabreWulfNastie*) (__AGM game ()) -> artist (__SABREWULFFIRE__)) -> kill ();
	}
	// When sabreman is finishing the game...
	else if (e.code () == __SABREMANOUTEXITZONE__)
	{
		SabreWulfWorld::ToExitMazeBuoy* b = 
			(SabreWulfWorld::ToExitMazeBuoy*) buoy (__SABREWULFTOEXITMAZEBUOYID__);
		b -> active (true); 
	}
}

// ---
void SabreWulfWorld::processEvent (const QGAMES::Event& e, QGAMES::Notifier* n)
{
	assert (n); // The sender has to be someone! 
	if (e.code () == __INMORTALOUTOFLIMITS__)
	{
		SabreWulfInmortal* im = dynamic_cast <SabreWulfInmortal*> (n);
		assert (im); // It shouldn't be any other thing...
		// Calculate where to go, and the new position...
		int toAdd = 0; 
		QGAMES::Vector toMove = QGAMES::Vector::_cero;
		QGAMES::Position refPos = im -> referenceZone (im -> orientation ());
		inLimitNewPlaceFor (im, refPos, toMove, toAdd);

		// Activate the buoy, to tretat the change...
		InmortalChangeMazePlaceEventData* eData = new InmortalChangeMazePlaceEventData;
		eData -> _mazePlace = im -> mazePlaceNumber () + toAdd;
		eData -> _position = im -> position () + toMove;
		eData -> _inmortal = im;
		SabreWulfWorld::InmortalToChangeMazePlaceBuoy* b = 
			(SabreWulfWorld::InmortalToChangeMazePlaceBuoy*) buoy (__INMORTALTOCHANGEROOMBUOYID__);
		b -> setData (eData); // The buoy destructor will destroy the data also!
		b -> active (true); 
	}
}

// ---
#ifndef NDEBUG
void SabreWulfWorld::onOffDrawLimits ()
{
	assert (_activeScene);
	((SabreWulfScene*) _activeScene) -> onOffDrawLimits ();
}
#endif
// ---

// ---
void SabreWulfWorld::clearThings ()
{
	for (int i = 0; i < (int) _orchilds.size (); i++)
		delete (_orchilds [i]);
	for (int i = 0; i < (int) _objects.size (); i++)
		for (int j = 0; j < (int) _objects[i].size (); j++)
			delete (_objects [i][j]);
}

// ---
void SabreWulfWorld::inLimitNewPlaceFor (QGAMES::Entity* e, const QGAMES::Position& rZ,
	QGAMES::Vector& tM, int& tA)
{
	assert (e);

	tA = 0;
	if (rZ.posX () >= __BD __MAXVISIBLEPOSX__) 
		{ tA = 1; 
		  tM = -QGAMES::Vector (__BD (__ORIGINALMAPLENGTH__ << 1), __BD 0, __BD 0);
		  tM += QGAMES::Vector (__BD (e -> currentForm () -> frameWidth () >> 1), __BD 0, __BD 0); }
	if (rZ.posX () <= __BD __MINVISIBLEPOSX__) 
		{ tA = -1; 
		  tM = QGAMES::Vector (__BD (__ORIGINALMAPLENGTH__ << 1), __BD 0, __BD 0);
		  tM -= QGAMES::Vector (__BD (e -> currentForm () -> frameWidth () >> 1), __BD 0, __BD 0); }
	if (rZ.posY () >= __BD __MAXVISIBLEPOSY__) 
		{ tA = __NUMBEROFMAZEPLACESX__; 
		  tM = -QGAMES::Vector (__BD 0, __BD (__ORIGINALMAPHEIGHT__ << 1), __BD 0);
		  tM += QGAMES::Vector (__BD 0, __BD (e -> currentForm () -> frameHeight () >> 1), __BD 0); }
	if (rZ.posY () <= __BD __MINVISIBLEPOSY__) 
		{ tA = -__NUMBEROFMAZEPLACESX__; 
		  tM = QGAMES::Vector (__BD 0, __BD (__ORIGINALMAPHEIGHT__ << 1), __BD 0); 
		  tM -= QGAMES::Vector (__BD 0, __BD (e -> currentForm () -> frameHeight () >> 1), __BD 0); }
}

// ---
void* SabreWulfWorld::ToChangeMazePlaceBuoy::treatFor (QGAMES::Element* e)
{
	assert (e);

	SabreWulfWorld* w = (SabreWulfWorld*) e;
	w -> setMazePlaceNumber (_data -> _mazePlace);
	w -> _mainCharacter -> setPosition (_data -> _position);
	delete (_data);
	_data = NULL; // The buoy has been treated...

	return (this);
}

// ---
void* SabreWulfWorld::ToExitMazeBuoy::treatFor (QGAMES::Element* e)
{
	assert (e);

	SabreWulfWorld* w = (SabreWulfWorld*) e;
	w -> game () -> setState (std::string (__SABREWULFWINSTATENAME__));
	return (this);	
}

// ---
void* SabreWulfWorld::InmortalToChangeMazePlaceBuoy::treatFor (QGAMES::Element* e)
{
	assert (e);

	SabreWulfWorld* w = (SabreWulfWorld*) e;
	_data -> _inmortal -> setMazePlaceNumber (_data -> _mazePlace);
	_data -> _inmortal -> setPosition (_data -> _position);
	delete (_data);
	_data = NULL; // The buoy has been treated...

	return (this);
}
