#include <BattleshipLike/bsbattleship.hpp>

const int BATTLESHIP::Battleship::_FADES [27] =
	{ 0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 255, 240, 220, 200, 180, 160, 140, 120, 100, 80, 60, 40, 20, 0 }; 

// ---
BATTLESHIP::Battleship::Battleship (int cId, BATTLESHIP::Spaceship::Behaviour* bhv, 
		const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: BATTLESHIP::Spaceship (cId, bhv, f, d),
	  _transportedElements (), 
	  _shootingsRemaining (0), 
	  _thingsAround () 
{
	assert (counters ());

	counters () -> addCounter 
		(new QGAMES::Counter (_COUNTERTOBRIGHT, 2, 0, true, true));
	counters () -> addCounter
		(new QGAMES::Counter (_COUNTERBRIGHT, sizeof (_FADES) / sizeof (_FADES [0]), 0, true, true));
}

// ---
QGAMES::Entity* BATTLESHIP::Battleship::clone () const
{
	QGAMES::Entity* result = new BATTLESHIP::Battleship 
		(_id, dynamic_cast <BATTLESHIP::Spaceship::Behaviour*> (_behaviour -> clone ()), _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
bool BATTLESHIP::Battleship::canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
{
	QGAMES::Rectangle mV = getMovingZone (); // Includes the max dimensions...
	int fW, fH, fD;
	maxVisualDimensions (fW, fH, fD);
	QGAMES::Position nPos = position () + d;

	// Out of screen in X axis?
	bool lX = (nPos.posX () > (mV.pos2 ().posX () - __BD 10) || 
		(nPos.posX () < (mV.pos1 ().posX () + __BD (fW - visualLength () + 10)))); 
	// Out of screen in Y axis?
	bool lY = (nPos.posY () > (mV.pos2 ().posY () - __BD 10) || 
		(nPos.posY () < (mV.pos1 ().posY () + __BD (fH - visualHeight () + 10)))); 
	
	return (!(lX || lY));
}

// ---
bool BATTLESHIP::Battleship::isEnemy (const BATTLESHIP::SpaceElement* ss) const
{ 
	return (dynamic_cast <const BATTLESHIP::UFO*> (ss) != NULL || 
			dynamic_cast <const BATTLESHIP::Asteroid*> (ss) != NULL); 
}

// ---
bool BATTLESHIP::Battleship::toShoot (int f, const QGAMES::Vector& d, int nP)
{
	bool result = BATTLESHIP::Spaceship::toShoot (f, d, nP);

	// It sounds only when battleship shoots...
	if (result)
		game () -> sound (__BATTLESHIP_SPACESHIPFIRINGSOUNDID__) -> play (__QGAMES_MAINCHARACTERSOUNDCHANNEL__);

	return (result);
}

// ---
void BATTLESHIP::Battleship::setTransportedElements (const BATTLESHIP::Game::TransportedElements& p)
{
	_transportedElements = p;

	// Builds up the list of the types of the elements transported...
	std::string lOfV;
	std::vector <int> lOfT = _transportedElements.listOfElementsType ();
	for (int i = 0; i < (int) lOfT.size (); i++)
		lOfV += ((i == 0) ? std::string (__NULL_STRING__) : std::string (",")) + 
			std::to_string (lOfT [i] != -1 ? lOfT [i] : -1);

	// ...and notify them just to actualize (if any) any score object that could be observing this
	QGAMES::SetOfOpenValues oV;
	oV.addOpenValue (__BATTLESHIP_BATTLESHIPTHINGSCARRIEDVALUEPARAM__, QGAMES::OpenValue (lOfV));
	notify (QGAMES::Event (__BATTLESHIP_BATTLESHIPTHINGSCARRIEDACTUALIZED__, this, oV));

	// Keeps the info the configuration object
	// Possible the info is comming from it, but just in case...
	BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (bG); // Just in case...
	bG -> setTransportedElements (transportedElements ());
}

// ---
bool BATTLESHIP::Battleship::toCatchOrLeaveThing ()
{
	BATTLESHIP::Game::TransportedElement elementToLeave;

	BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (bG); // Just in case...

	// Something to catch in the space eventually?
	if (!_thingsAround.empty ())
	{
		BATTLESHIP::ThingToCatch* th = 	
			dynamic_cast <BATTLESHIP::ThingToCatch*> ((*(_thingsAround.begin ())).second);
		assert (th); // Just in case...it shouldn't but who knows...

		// Catch it and determinate what it left add the same time, if any...
		elementToLeave = _transportedElements.toAddElement
			(BATTLESHIP::Game::TransportedElement (th -> id (), th -> type (), th -> runtimeValues ()));
		th -> setVisible (false); // No longer in the screen...

		// Something happens...
		th -> whenCaughtBy (this);

		// Notify that something has been caught...
		notify (QGAMES::Event (__BATTLESHIP_SPACEELEMENTCAUGHT__, this));

		game () -> sound (__BATTLESHIP_BATTLESHIPGETSOUNDID__) -> play (__QGAMES_MAINCHARACTERSOUNDCHANNEL__);
	}
	else
		elementToLeave = _transportedElements.toAddElement (BATTLESHIP::Game::TransportedElement ());

	// Just to notify the new situation of the pocket...
	setTransportedElements (_transportedElements);

	// ...and leave if there is something!
	// To leave something has to be processed by the scene (which is the one with the list of sprites frees if any)
	if (elementToLeave._id != -1)
		notify (QGAMES::Event (__BATTLESHIP_BATTLESHIPLEFTTHING__, this, elementToLeave.likeSetOfOpenValues ()));

	return (true);
}

// ---
void BATTLESHIP::Battleship::reduceShootingsRemaining (int a)
{ 
	int oS = _shootingsRemaining;

	_shootingsRemaining = _shootingsRemaining - a;
	if (_shootingsRemaining < 0) _shootingsRemaining = 0; // Never negative...
	if (_shootingsRemaining > 999) _shootingsRemaining = 999; // Never biger than 1000...

	// If there has been a change in the shootings reaming,
	// The information is notified to be (hopefully) reflected in the score object...
	if (oS != _shootingsRemaining)
	{
		QGAMES::SetOfOpenValues val;
		val.addOpenValue (__BATTLESHIP_SPACESHIPSHOOTINGSVALUEPARAM__, QGAMES::OpenValue (_shootingsRemaining));
		notify (QGAMES::Event (__BATTLESHIP_SPACESHIPSHOOTINGSACTUALIZED__, this, val));
	}
}

// ---
QGAMES::Rectangle BATTLESHIP::Battleship::collisionZone () const
{
	if (isSealed ())
		return (QGAMES::Artist::collisionZone ()); 
	// The full size when sealed (because it is ocuppied by the halo)
	// NOTE: Notice that it not invoked the direct parent class but two levels above!

	QGAMES::Rectangle rect = BATTLESHIP::Spaceship::collisionZone ();
	QGAMES::Vector dg = __BD 0.25 /** 25% less. */ * (rect.pos2 () - rect.pos1 ());
	return (QGAMES::Rectangle (rect.pos1 () + dg, rect.pos2 () - dg));
}

// ---
void BATTLESHIP::Battleship::initialize ()
{
	BATTLESHIP::Spaceship::initialize ();

	_shootingsRemaining = 0;

	// Staying...
	setStateFor (_spaceElementState = _MOVING, QGAMES::Vector::_cero);

	// The initial position is in the middle of the screen...
	setPosition (game () -> mainScreen () -> visualCenter () - 
		QGAMES::Position (__BD (currentForm () -> frameWidthForFrame (currentAspect ()) >> 1),
						  __BD (currentForm () -> frameHeightForFrame (currentAspect ()) >> 1), __BD 0));

	// Sets the power level to th eone kept into the configuration data
	// ...and also the score...
	BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (bG); // It shouldn't, but just in case...
	setScore (bG -> score ());
	setPowerLevel (bG -> level ());
	reduceShootingsRemaining (shootingsRemaining () - bG -> shootingsLeft ()); // To put into the right number...

	// By default the battleship doesn't lose energy...
	// Unless the specific game state says it!
	// @see begamestate.cpp
	setLosingEnergy (false);

	// If the password has been set, 
	// then the battleship won't be affected neither by enemies nor by similars...
	// Whatever the configuration was!
	if (bG -> isPasswordSet ())
	{
		behaviour () -> _energyLostPerLoop = 0; // and additionally it doesn't lose energy
		behaviour () -> _canBeHurmedByEnemies = behaviour () -> _canBeHurmedBySimilars = false;
	}
}

// ---
void BATTLESHIP::Battleship::updatePositions ()
{
	BATTLESHIP::Spaceship::updatePositions ();

	/** If sealed, the the bright is modified little by little. */
	if (isSealed () && counter (_COUNTERTOBRIGHT) -> isEnd ())
		counter (_COUNTERBRIGHT) -> isEnd ();

	_thingsAround = QGAMES::Entities ();
}

// ---
void BATTLESHIP::Battleship::finalize ()
{
	BATTLESHIP::Spaceship::finalize ();

	// Saves the score and thge power level...
	BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (bG); // It shouldn't, but just in case...
	bG -> setScore (score ()); // Save also the score...
	bG -> setLevel (powerLevel ()); // Save the level into the configuration...
	bG -> setShootingsLeft (shootingsRemaining ()); // Save the number of shootings left...
	// What is never saved is the energy left. This is like a type of award!
}

// ---
void BATTLESHIP::Battleship::drawOn (QGAMES::Screen* scr, const QGAMES::Position& p)
{
	BATTLESHIP::Spaceship::drawOn (scr, p);

	// When selaed, there is a halo around...
	if (isSealed ())
	{
		QGAMES::Position ctr = centerPosition ();
		QGAMES::bdata rdx = ctr.posX () - collisionZone ().pos1 ().posX ();
		QGAMES::Color clr (255, 255, 255, _FADES [counter (_COUNTERBRIGHT) -> value ()]);
		for (int i = 0; i < 3; i++)
			scr -> drawCircle (ctr, QGAMES::Vector::_zNormal, rdx + __BD i, rdx + __BD i, clr);
	}
}

// ---
void BATTLESHIP::Battleship::whenCollisionWith (QGAMES::Entity* e)
{
	if (!isAlive ())
		return; // Nothing to do if the battleship is not moving...

	BATTLESHIP::Spaceship::whenCollisionWith (e);

	// When the thing the battleship has crashed against is a thing to catch
	// either it is "noted" or just simply caught, depending on the game configuration!
	if (dynamic_cast <BATTLESHIP::ThingToCatch*> (e))
	{
		BATTLESHIP::ThingToCatch* sT = dynamic_cast <BATTLESHIP::ThingToCatch*> (e);
		if (sT -> isAlive ())
		{
			BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
			assert (bG);


			if (bG -> dataGame () -> _clickNeededToCatchThings)
				_thingsAround [sT -> id ()] = sT; // Indicates it...
			else
				sT -> whenCaughtBy (this);
		}
	}
}

// ---
void BATTLESHIP::Battleship::processEvent (const QGAMES::Event& evnt)
{
	// In the case of the battleship
	// when it is out of the screen it doesn't dissapear as it is defined in the parent class
	// So, how that event is understood needs to be redefined...
	if (evnt.code () == __QGAMES_MOVEMENTNOTPOSSIBLE__)
		return;
	
	BATTLESHIP::Spaceship::processEvent (evnt);
}

// ---
void BATTLESHIP::Battleship::setStateFor (int st, const QGAMES::Vector& o)
{
	switch (st)
	{
		case _HIDDEN:
			setCurrentState (__BATTLESHIP_BATTLESHIPBLANKSTATE__);
			break;

		case _MOVING:
			{
				if (o.posX () > 0)
					setCurrentState (o.posY () > 0 
						? __BATTLESHIP_BATTLESHIPMOVINGRIGHTBACKWARDSTATE__ // Right - Backward
						: (o.posY () < 0 
							? __BATTLESHIP_BATTLESHIPMOVINGRIGHTFORWARDSTATE__ // Right - Forward
							: __BATTLESHIP_BATTLESHIPMOVINGRIGHTSTATE__)); // Right
				else
				if (o.posX () < 0)
					setCurrentState (o.posY () > 0 
						? __BATTLESHIP_BATTLESHIPMOVINGLEFTBACKWARDSTATE__ // Left - Backward
						: (o.posY () < 0 
							? __BATTLESHIP_BATTLESHIPMOVINGLEFTFORWARDSTATE__ // Left - Forward
							: __BATTLESHIP_BATTLESHIPMOVINGLEFTSTATE__)); // Left
				else
					setCurrentState (o.posY () > 0 
						? __BATTLESHIP_BATTLESHIPMOVINGBACKWARDSTATE__ // Backward
						: (o.posY () < 0 
							? __BATTLESHIP_BATTLESHIPMOVINGFORWARDSTATE__ // Forward
							: __BATTLESHIP_BATTLESHIPSTAYINGSTATE__)); // Stay
			}

			break;

		case _EXPLODING:
			setCurrentState (__BATTLESHIP_BATTLESHIPEXPLODINGSTATE__);
			break;

		case _DESTROYED:
			setCurrentState (__BATTLESHIP_BATTLESHIPBLANKSTATE__);
			break;

		case _ROTATING:
			setCurrentState (__BATTLESHIP_BATTLESHIPROTATINGSTATE__);
			break;

		default:
			assert (0); // No others states have been defined in the default implementation of battleship
	}
}
