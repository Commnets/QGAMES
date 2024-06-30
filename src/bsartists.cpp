#include <BattleshipLike/bsartists.hpp>
#include <BattleshipLike/bsscenes.hpp>

// ---
BATTLESHIP::SpaceElement::Behaviour::Behaviour (bool cE, bool cS, QGAMES::bdata eL, QGAMES::bdata eD, int p, int aM)
	: _canBeHurmedByEnemies (cE),
		_canBeHurmedBySimilars (cS),
		_energyLostPerLoop (eL),
		_energyDetractedWhenHit (eD),
		_pointsGivenWhenDestroyed (p),
		_automaticMovement (aM) 
{
	// It is only energy lost...
	assert (_energyLostPerLoop >= __BD 0 && _energyLostPerLoop <= __BD 100);
	// Values can be from -100 (gain energy) to 100 (lose energy)
	assert (_energyDetractedWhenHit >= __BD -100 && _energyDetractedWhenHit <= __BD 100); 
}

// ---
BATTLESHIP::SpaceElement::Behaviour::Behaviour (const QGAMES::SetOfOpenValues& oV)
	: _canBeHurmedByEnemies (true), _canBeHurmedBySimilars (false),
	  _energyLostPerLoop (__BD 1 / __BD QGAMES::Game::game () -> framesPerSecond ()), 
	  _pointsGivenWhenDestroyed (10),
	  _automaticMovement (-1)
	  // Default values...
{
	std::string tS; // Temporal...
	if (oV.existOpenValue (__BATTLESHIP_BHVOPENVALUEHURMEDEMISPARAM__))
		_canBeHurmedByEnemies = QGAMES::toUpper (tS = std::string
			(oV.openValue (__BATTLESHIP_BHVOPENVALUEHURMEDEMISPARAM__).strValue ())) == std::string (__YES_STRING__);
	if (oV.existOpenValue (__BATTLESHIP_BHVOPENVALUEHURMEDSIMSPARAM__))
		_canBeHurmedBySimilars = QGAMES::toUpper (tS = std::string 
			(oV.openValue (__BATTLESHIP_BHVOPENVALUEHURMEDSIMSPARAM__).strValue ())) == std::string (__YES_STRING__);
	if (oV.existOpenValue (__BATTLESHIP_BHVOPENVALUEENERGYLOSTPARAM__))
		_energyLostPerLoop = __BD std::stof (oV.openValue (__BATTLESHIP_BHVOPENVALUEENERGYLOSTPARAM__).strValue ());
	if (oV.existOpenValue (__BATTLESHIP_BHVOPENVALUEENERGYDETRACTEDPARAM__))
		_energyDetractedWhenHit = __BD std::stof (oV.openValue (__BATTLESHIP_BHVOPENVALUEENERGYDETRACTEDPARAM__).strValue ());
	if (oV.existOpenValue (__BATTLESHIP_BHVOPENVALUEPTSWHENDIEPARAM__))
		_pointsGivenWhenDestroyed = std::stoi (oV.openValue (__BATTLESHIP_BHVOPENVALUEPTSWHENDIEPARAM__).strValue ());
	if (oV.existOpenValue (__BATTLESHIP_BHVOPENVALUEAUTOMOVPARAM__))
		_automaticMovement = std::stoi (oV.openValue (__BATTLESHIP_BHVOPENVALUEAUTOMOVPARAM__).strValue ());

	// TO verify that the elements are right in debug mode...
	BATTLESHIP::SpaceElement::Behaviour (_canBeHurmedByEnemies, _canBeHurmedBySimilars, 
		_energyLostPerLoop, _energyDetractedWhenHit, _pointsGivenWhenDestroyed, _automaticMovement);
}

// ---
BATTLESHIP::SpaceElement::SpaceElement (int cId, BATTLESHIP::SpaceElement::Behaviour* bhv, 
		const QGAMES::Forms& f,	const QGAMES::Entity::Data& d)
	: QGAMES::Artist (cId, f, d),
	  _behaviour (bhv),
	  _spaceElementState (_HIDDEN), // By default it is hidden
	  _energy (__BD 100), // Full by default
	  _losingEnergy (true), // By default all of them lose energy per cycle
	  _energyToAdd (__BD 0),
	  _hitsBy (), // None
	  _lastPosition (QGAMES::Position::_cero),
	  _trendDirection (QGAMES::Vector::_cero),
	  _drawLimits (false)
{ 
	assert (_behaviour);

	addBuoy (__BATTLESHIP_CHANGESPACEELEMENTSTATEBUOY__, 
		new BATTLESHIP::SpaceElement::ToChangeSpaceElementStateBuoy (__BATTLESHIP_CHANGESPACEELEMENTSTATEBUOY__));

#ifndef NDEBUG
	_drawLimits = true; // Limits are drawn by default under debug mode...
#endif
}

// ---
void BATTLESHIP::SpaceElement::setBehaviour (BATTLESHIP::SpaceElement::Behaviour* bhv)
{
	assert (bhv); // It can not be null...

	delete (_behaviour);

	_behaviour = bhv;
}

// ---
bool BATTLESHIP::SpaceElement::canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
{
	QGAMES::Rectangle mV = getMovingZone (); // Includes the max dimensions...
	QGAMES::Position nPos = position () + d;

	// Out of screen in X axis?
	bool lX = (nPos.posX () > mV.pos2 ().posX ()) || (nPos.posX () < mV.pos1 ().posX ()); 
	// Out of screen in Y axis?
	bool lY = (nPos.posY () > mV.pos2 ().posY ()) || (nPos.posY () < mV.pos1 ().posY ()); 
	
	return (!(lX || lY));
}

// ---
int BATTLESHIP::SpaceElement::referenceSpeed () const
{
	if (!map ()) // If no map defined, basic reference speed...
		return (1);

	const BATTLESHIP::PlayingScene* pS = dynamic_cast <const BATTLESHIP::PlayingScene*> (map () -> scene ());
	if (!pS) // If the scene is not of the right type, then basic speed...
		return (1);

	return (pS -> speed ());
}

// ---
void BATTLESHIP::SpaceElement::toForgetDeferreStates ()
{
	ToChangeSpaceElementStateBuoy* b = 
		dynamic_cast <ToChangeSpaceElementStateBuoy*> (buoy (__BATTLESHIP_CHANGESPACEELEMENTSTATEBUOY__));
	assert (b); // It shouldn't, but...

	b -> active (false); // Forgotten
}

// ---
void BATTLESHIP::SpaceElement::toHideDeferred ()
{
	BATTLESHIP::SpaceElement::ToChangeSpaceElementStateBuoy* b = 
		dynamic_cast <BATTLESHIP::SpaceElement::ToChangeSpaceElementStateBuoy*> (buoy (__BATTLESHIP_CHANGESPACEELEMENTSTATEBUOY__));
	assert (b); // It shouldn't, but...

	b -> setValue (__BD BATTLESHIP::SpaceElement::_HIDDEN);
	// No parameters needed...

	b -> active (true);
}

// ---
void BATTLESHIP::SpaceElement::toMove (int tM, const QGAMES::Vector& v)
{
	// The moving state is only possible when is alive or it is hidden...
	if (!isAlive () && !isHidden ())
		return;

	// To move is only possible if it is not moving before or moving towards another direction
	if (!isMoving () || (isMoving () && !isMovingTowards (v)))
		setStateFor (_spaceElementState = _MOVING, v); 
}

// ---
void BATTLESHIP::SpaceElement::toMoveDeferred (int tM, const QGAMES::Vector& v)
{
	BATTLESHIP::SpaceElement::ToChangeSpaceElementStateBuoy* b = 
		dynamic_cast <BATTLESHIP::SpaceElement::ToChangeSpaceElementStateBuoy*> (buoy (__BATTLESHIP_CHANGESPACEELEMENTSTATEBUOY__));
	assert (b); // It shouldn't, but...

	b -> setValue (__BD BATTLESHIP::SpaceElement::_MOVING);
	
	QGAMES::SetOfOpenValues val;
	val.addOpenValue (0, QGAMES::OpenValue (tM));
	std::stringstream sS; sS << v;
	val.addOpenValue (1, QGAMES::OpenValue (sS.str ()));
	b -> setParameters (val);

	b -> active (true);
}

// ---
void BATTLESHIP::SpaceElement::toExplode ()
{
	// To explode is only possible when moving before...
	if (!isAlive () || isExploding ())
		return;

	QGAMES::Position iPos = centerPosition ();
	setStateFor (_spaceElementState = _EXPLODING, orientation ());
	setPosition (position () + (iPos - centerPosition ())); // To center the fire...
}

// ---
void BATTLESHIP::SpaceElement::toExplodeDeferred ()
{
	BATTLESHIP::SpaceElement::ToChangeSpaceElementStateBuoy* b = 
		dynamic_cast <BATTLESHIP::SpaceElement::ToChangeSpaceElementStateBuoy*> (buoy (__BATTLESHIP_CHANGESPACEELEMENTSTATEBUOY__));
	assert (b); // It shouldn't, but...

	b -> setValue (__BD BATTLESHIP::SpaceElement::_EXPLODING);
	// No parameters needed...

	b -> active (true);
}

// ---
void BATTLESHIP::SpaceElement::toDestroyDeferred ()
{
	ToChangeSpaceElementStateBuoy* b = 
		dynamic_cast <ToChangeSpaceElementStateBuoy*> (buoy (__BATTLESHIP_CHANGESPACEELEMENTSTATEBUOY__));
	assert (b); // It shouldn't, but...

	b -> setValue (__BD BATTLESHIP::SpaceElement::_DESTROYED);
	// No parameters needed...

	b -> active (true);
}

// ---
void BATTLESHIP::SpaceElement::setEnergy (QGAMES::bdata e)
{
	QGAMES::bdata lE = _energy;

	_energy = e;

	if (_energy > __BD 100) _energy = __BD 100;
	if (_energy < __BD 0) _energy = __BD 0;

	if (_energy != lE)
	{
		QGAMES::SetOfOpenValues oV;
		oV.addOpenValue (__BATTLESHIP_SPACEELMNTENERGYVALUEPARAMETER__, QGAMES::OpenValue (_energy));
		notify (QGAMES::Event (__BATTLESHIP_SPACEELMNTENERGYACTUALIZED__, this, oV)); // Notify the new level of energy...
	}
}

// ---
QGAMES::Rectangle BATTLESHIP::SpaceElement::collisionZone () const
{
	assert (currentForm ());

	QGAMES::Vector offSet (
		__BD currentForm () -> frameXOffsetForFrame (currentAspect ()), 
		__BD currentForm () -> frameYOffsetForFrame (currentAspect ()),
		__BD 0);

	QGAMES::Vector size (
		__BD currentForm () -> frameWidthForFrame (currentAspect ()),
		__BD currentForm () -> frameHeightForFrame (currentAspect ()), 
		__BD 0);

	QGAMES::Vector lSize = __BD 0.25 * size;
	
	return (QGAMES::Rectangle (_data._position + offSet + lSize, _data._position + offSet + size - lSize));
}

// ---
void BATTLESHIP::SpaceElement::initialize ()
{
	QGAMES::Artist::initialize ();

	setStateFor (_spaceElementState = _HIDDEN);

	_energy = __BD 100;
	_energyToAdd = __BD 0;

	_hitsBy = std::map <BATTLESHIP::SpaceElement*, int> (); // Hits clear...

	// If there is monitor defined...adds it!!
	if (behaviour () -> _automaticMovement != -1)
		addControlStepsMonitor (game () -> characterMonitorBuilder () -> 
			monitorFor (behaviour () -> _automaticMovement, this));

	_lastPosition = QGAMES::Position::_cero;
	_trendDirection = QGAMES::Vector::_cero;
}

// ---
void BATTLESHIP::SpaceElement::inEveryLoop ()
{
	QGAMES::Artist::inEveryLoop ();

	if (_losingEnergy)
		setEnergy (energy () - _behaviour -> _energyLostPerLoop);
	setEnergy (energy () + _energyToAdd);
	_energyToAdd = 0; // Once it's been used becomes useless...

	if (energy () == __BD 0)
	{
		notify (QGAMES::Event (__BATTLESHIP_SPACEELMNTENERGYIS0__, this)); // The energy is 0 eventually...
		
		// ...and it has to die only if it possible...and it is not still alive!
		if (isAlive () && 
			(_behaviour -> _canBeHurmedByEnemies || _behaviour -> _canBeHurmedBySimilars))
		{
			toExplode ();

			BATTLESHIP::Spaceship* sS = dynamic_cast <BATTLESHIP::Spaceship*> (whoHitTheMost ());
			if (sS) // If it was hit by another spaceship, the points are given to it...
				sS -> setScore (sS -> score () + behaviour () -> _pointsGivenWhenDestroyed);

			notify (QGAMES::Event (__BATTLESHIP_SPACEELEMENTDESTROYED__, this));
			// For the scene block to be notified also, if any!

			game () -> sound (__BATTLESHIP_SPACESHIPEXPLODESOUNDID__) -> play (__QGAMES_ARTISTSOUNDCHANNEL__);
		}
	}
}

// ---
void BATTLESHIP::SpaceElement::updatePositions ()
{
	if (position () != _lastPosition)
	{
		_trendDirection = position () - _lastPosition;
		_lastPosition = position ();
	}

	if (!isVisible ())
		return;

	QGAMES::Artist::updatePositions ();

	if (hasExploded ())
	{
		toDestroy ();

		// To notify thatthe spaceship is not longer valid...
		notify (QGAMES::Event (__BATTLESHIP_SPACEELMNTHASDESTROYED__, this));
	}
}

// ---
void BATTLESHIP::SpaceElement::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	// Draws the rest...normally the entity itself!
	QGAMES::Artist::drawOn (s, p);

	if (_drawLimits)
		drawOnLimits (s, p);
}

// ---
void BATTLESHIP::SpaceElement::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __QGAMES_MOVEMENTNOTPOSSIBLE__) // Usually when it exits the screen...
	{
		setVisible (false); // No longer visible...

		toHideDeferred (); // Better in a different loop...

		notify (QGAMES::Event (__BATTLESHIP_SPACEELEMENTOUTOFSCENE__, this)); 
		// In many ocassions this event is used to inform the action block
		// @see BasicSpaceElementSceneActionBlock class
	}

	QGAMES::Artist::processEvent (evnt);
}

// ---
BATTLESHIP::SpaceElement* BATTLESHIP::SpaceElement::whoHitTheMost () const
{
	BATTLESHIP::SpaceElement* result = NULL;

	int nH = 0;
	for (std::map <BATTLESHIP::SpaceElement*, int>::const_iterator i = _hitsBy.begin (); 
			i != _hitsBy.end (); i++)
		if ((*i).second > nH)
			result = (*i).first;

	return (result);
}

// ---
QGAMES::Rectangle BATTLESHIP::SpaceElement::getMovingZone ()
{
	// Initally (by default) a 2D world is considered!
	QGAMES::bdata x1 = __BD 0;
	QGAMES::bdata y1 = __BD 0;
	QGAMES::bdata z1 = __BD 0;
	QGAMES::bdata x2 = __BD (map () ? map () ->  width ()  : (game () -> mainScreen ()) -> visualWidth ());
	QGAMES::bdata y2 = __BD (map () ? map () ->  height () : (game () -> mainScreen ()) -> visualHeight ());
	QGAMES::bdata z2 = __BD 0;

	int fW, fH, fD;
	maxVisualDimensions (fW, fH, fD);
	x1 -= __BD fW; y1 -= __BD fH; 

	return (QGAMES::Rectangle (QGAMES::Position (x1, y1, z1), QGAMES::Position (x2, y2, z2)));
}

// ---
void BATTLESHIP::SpaceElement::hitBy (BATTLESHIP::SpaceElement* sE)
{
	if (_hitsBy.find (sE) == _hitsBy.end ()) 
		_hitsBy [sE] = 0;
	_hitsBy [sE]++; 
}

// ---
void BATTLESHIP::SpaceElement::drawOnLimits (QGAMES::Screen* s, const QGAMES::Position& p)
{
	// Drawing rectangles over and under the entity...
	// Over...
	QGAMES::Rectangle recA1 = coverZone ();
	s -> drawRectangle (recA1, __QGAMES_YELLOWCOLOR__);
	// Under...
	QGAMES::Rectangle recA2 = baseZone ();
	s -> drawRectangle (recA2, __QGAMES_YELLOWCOLOR__);

	// Draws a rectangle at front and at the back of the artist...
	// Front...
	QGAMES::Positions posA3;
	posA3.push_back (recA1.pos4 ()); posA3.push_back (recA1.pos2 ());
	posA3.push_back (recA2.pos2 ()); posA3.push_back (recA2.pos4 ());
	s -> drawPolygon (QGAMES::Polygon (posA3), __QGAMES_YELLOWCOLOR__);
	// Back...
	QGAMES::Positions posA4;
	posA4.push_back (recA1.pos1 ()); posA4.push_back (recA1.pos3 ());
	posA4.push_back (recA2.pos3 ()); posA4.push_back (recA2.pos1 ());
	s -> drawPolygon (QGAMES::Polygon (posA4), __QGAMES_YELLOWCOLOR__);

	// Draws a little rectangle where the base is...
	// This routine is valid initially whatever the proyection type is!
	QGAMES::Position p1 = basePosition () - QGAMES::Vector (__BD 5, __BD 5, __BD 0);
	QGAMES::Position p2 = p1 + QGAMES::Vector (__BD 10, __BD 10, __BD 0);
	QGAMES::Rectangle rec (p1, p2, QGAMES::Vector::_zNormal);
	s -> drawRectangle (rec, __QGAMES_BLUECOLOR__, true);

	// Draws a little rectangle to represent the collision zone...
	// In the same, this routine should be valid whatever the type of projection is!
	s -> drawRectangle (collisionZone (), __QGAMES_REDCOLOR__);

	// Draws a little cross to represent the center
	// Following the same, this part of the routine should be valid, whatever the projection is...
	QGAMES::Position cP = centerPosition ();
	s -> drawLine (cP - QGAMES::Vector (__BD 5, __BD 0, __BD 0), 
				   cP + QGAMES::Vector (__BD 5, __BD 0, __BD 0), __QGAMES_BLACKCOLOR__);
	s -> drawLine (cP - QGAMES::Vector (__BD 0, __BD 0, __BD 5), 
				   cP + QGAMES::Vector (__BD 0, __BD 0, __BD 5), __QGAMES_BLACKCOLOR__);
}

// ---
void* BATTLESHIP::SpaceElement::ToChangeSpaceElementStateBuoy::treatFor (QGAMES::Element* e)
{
	BATTLESHIP::SpaceElement* sE = dynamic_cast <BATTLESHIP::SpaceElement*> (e);
	assert (sE); // It shouldn't, but who knows!!

	switch ((int) value ())
	{
		case BATTLESHIP::SpaceElement::_FROZEN:
			sE -> toFreeze ();
			break;

		case BATTLESHIP::SpaceElement::_HIDDEN:
			sE -> toHide ();
			break;

		case BATTLESHIP::SpaceElement::_MOVING:
			sE -> toMove (_parameters.openValue (0).intValue (),
				QGAMES::Vector (_parameters.openValue (1).strValue ()));
			break;

		case BATTLESHIP::SpaceElement::_EXPLODING:
			sE -> toExplode ();
			break;

		case BATTLESHIP::SpaceElement::_DESTROYED:
			sE -> toDestroy ();
			break;

		default:
			assert (0); // It is not a supported value...
	}

	return (this);
}

// ---
BATTLESHIP::SpaceshipShooting::SpaceshipShooting (int cId, const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: QGAMES::Artist (cId, f, d),
	  _owner (NULL),
	  _energy (__BD 0),
	  _type (0),
	  _elementHitExplode (NULL),
	  _elementHitInitialPos (QGAMES::Position::_cero)
{ 
	addBuoy (__BATTLESHIP_TOEXPLODESHOOTINGBUOY__, 
		new BATTLESHIP::SpaceshipShooting::ToExplodeDeferredBuoy (__BATTLESHIP_TOEXPLODESHOOTINGBUOY__));
}

// ---
QGAMES::Entity* BATTLESHIP::SpaceshipShooting::clone () const
{
	QGAMES::Entity* result = new BATTLESHIP::SpaceshipShooting (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
bool BATTLESHIP::SpaceshipShooting::canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
{
	bool result = collisionZone ().intersectsWith (QGAMES::Rectangle 
		(QGAMES::Position::_cero, QGAMES::Position (__BD map () -> width (), __BD map () -> height (), __BD 0)));
	return (result);
}

// ---
void BATTLESHIP::SpaceshipShooting::setType (int t)
{ 
	assert (t >= 0 && t < __BATTLESHIP_NUMBERSHOOTINGTYPES__);

	_type = t; 
}

// ---
void BATTLESHIP::SpaceshipShooting::toLaunchFrom (const QGAMES::Position& cpos, int sF, const QGAMES::Vector& d)
{
	assert (!isVisible ()); // It hasn't to be visible (so far)

	setVisible (true);

	// There could be many different movements of the shooting regarding the type...
	// So an intermediate method is invoked to determinate that!
	setCurrentState (movementStateForType (_type));

	// The initial position is set a little bit up of the position received as parameter
	// Probably it is something to correct later, becasue it suposse that who is shooting is at
	// the bottom of the visible zone pointing up!
	setPosition (cpos - QGAMES::Vector (__BD (visualLength () >> 1), __BD 0, __BD 0));

	// Change the impulse and the direction according with the data received
	setMove (d);
	QGAMES::SimpleLinearMovement* mov = dynamic_cast <QGAMES::SimpleLinearMovement*> (currentMovement ());
	assert (mov);
	mov -> setSpeed (mov -> speed () * sF);

	notify (QGAMES::Event (__BATTLESHIP_SHOOTINGLAUNCHED__, this));
}

// ---
bool BATTLESHIP::SpaceshipShooting::isFlying () const
{ 
	return (currentState () && 
			(currentState () -> id () >= 0 && currentState () -> id () < (__BATTLESHIPDATAGAME__ -> _numberOfShootingTypes))); 
}

// ---
void BATTLESHIP::SpaceshipShooting::toExplode ()
{
	if (isExploding ())
		return;

	QGAMES::Position ctr = centerPosition ();

	setCurrentState (__BATTLESHIP_SPACESHIPSHOOTINGEXPLODINGSTATE__);

	setPosition (position () + (ctr - centerPosition ()));
}

// ---
void BATTLESHIP::SpaceshipShooting::toExplodeDeferred ()
{
	BATTLESHIP::SpaceshipShooting::ToExplodeDeferredBuoy* b = 
		dynamic_cast <BATTLESHIP::SpaceshipShooting::ToExplodeDeferredBuoy*> (buoy (__BATTLESHIP_TOEXPLODESHOOTINGBUOY__));
	assert (b); // It shouldn't, but...

	b -> active (true);
}

// ---
bool BATTLESHIP::SpaceshipShooting::isExploding () const
{
	return (currentState () && currentState () -> id () == __BATTLESHIP_SPACESHIPSHOOTINGEXPLODINGSTATE__);
}

// ---
void BATTLESHIP::SpaceshipShooting::toEnd ()
{
	setVisible (false);

	setCurrentState (__BATTLESHIP_SPACESHIPSHOOTINGBLANKSTATE__);
}

// ---
void BATTLESHIP::SpaceshipShooting::initialize ()
{
	QGAMES::Artist::initialize ();

	_elementHitExplode = NULL;
	_elementHitInitialPos = QGAMES::Position::_cero;

	setCurrentForm ((*_forms.begin ()).first);
	setCurrentAspect (0); // The basic frame...
	// But the state is not change...
	// This is because the aspect could be used later to calculate where to show it!
	// even being invosible or not having any other aspect!...
}

// ---
void BATTLESHIP::SpaceshipShooting::updatePositions ()
{
	if (!isVisible ())
		return;

	QGAMES::Artist::updatePositions ();

	if (isExploding ())
	{
		if (_elementHitExplode != NULL && _elementHitExplode -> isVisible () && 
			_elementHitInitialPos != _elementHitExplode -> centerPosition ())
		{	
			setPosition (position () + 
				(_elementHitExplode -> centerPosition () - _elementHitInitialPos)); // Moves with the target if it exists...
																					// Tajes the center of the target as reference!

			_elementHitInitialPos = _elementHitExplode -> centerPosition ();
		}

		if (hasExploded ())
		{
			_elementHitExplode = NULL;
			_elementHitInitialPos = QGAMES::Position::_cero;

			notify (QGAMES::Event (__BATTLESHIP_SHOOTINGTOEND__, this));
		}
	}
}

// ---
void BATTLESHIP::SpaceshipShooting::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	QGAMES::Artist::drawOn (s, p);
}

// ---
void BATTLESHIP::SpaceshipShooting::whenCollisionWith (QGAMES::Entity* e)
{
	QGAMES::Artist::whenCollisionWith (e);

	if (!isFlying ())
		return; // No sense when exploding already

	// If the element is not collisioning against any spaceElement...
	// So it has no effect at all! 
	BATTLESHIP::SpaceElement* sE;
	if (!(sE = dynamic_cast <BATTLESHIP::SpaceElement*> (e)))
		return;

	// The shooting might have effects when it collisions with something alive or just exploding...
	// This is because we never know whether which check is executed first: 
	// This one or the one between the spaceship and the shooting. 
	// In that one the spaceship is destroyed (started)...
	if (sE -> isAlive ())
	{
		bool tN = false;
		tN |= (dynamic_cast <BATTLESHIP::Asteroid*> (sE) != NULL) |
			  (dynamic_cast <BATTLESHIP::ThingToCatch*> (sE) != NULL);	
		tN |= (dynamic_cast <BATTLESHIP::Spaceship*> (sE) != NULL && owner () != sE) & 
			  ((owner () -> isEnemy ((BATTLESHIP::Spaceship*) sE) && sE -> behaviour () -> _canBeHurmedByEnemies) |
			   (!owner () -> isEnemy ((BATTLESHIP::Spaceship*) sE) && sE -> behaviour () -> _canBeHurmedBySimilars));
		if (tN)
		{
			_elementHitExplode = sE;
			_elementHitInitialPos = sE -> centerPosition ();

			toExplodeDeferred ();
		}
	}
}

// ---
void BATTLESHIP::SpaceshipShooting::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __QGAMES_MOVEMENTNOTPOSSIBLE__) // The shooting can't move...
		notify (QGAMES::Event (__BATTLESHIP_SHOOTINGTOEND__, this));

	QGAMES::Artist::processEvent (evnt);
}

// ---
int BATTLESHIP::SpaceshipShooting::movementStateForType (int t)
{
	assert (t >= 0 && t < __BATTLESHIP_NUMBERSHOOTINGTYPES__);

	return (t);
}

// ---
void* BATTLESHIP::SpaceshipShooting::ToExplodeDeferredBuoy::treatFor (QGAMES::Element* e)
{
	BATTLESHIP::SpaceshipShooting* sS = dynamic_cast <BATTLESHIP::SpaceshipShooting*> (e);
	assert (sS);
	sS -> toExplode ();

	return (this);
}

// ---
BATTLESHIP::Spaceship::Behaviour::Behaviour (bool cE, bool cS, QGAMES::bdata eL, QGAMES::bdata eD, 
		int p, QGAMES::bdata s, int tS, QGAMES::bdata sP, int aM)
	: BATTLESHIP::SpaceElement::Behaviour (cE, cS, eL, eD, p, aM),
	  _shootingEnergy (s),
	  _typeOfShooting (tS),
	  _shieldProtection (sP)
{
	assert (_shootingEnergy >= __BD 0 && _shootingEnergy <= __BD 100);
	assert (_typeOfShooting >= 0 && _typeOfShooting < __BATTLESHIP_NUMBERSHOOTINGTYPES__);
	assert (_shieldProtection > 0);
}

// ---
BATTLESHIP::Spaceship::Behaviour::Behaviour (const QGAMES::SetOfOpenValues& oV)
	: BATTLESHIP::SpaceElement::Behaviour (oV),
	  _shootingEnergy (__BD 5), 
	  _typeOfShooting (0)  
{
	if (oV.existOpenValue (__BATTLESHIP_BHVOPENVALUESHTENERGYPARAM__))
		_shootingEnergy = __BD std::stof (oV.openValue (__BATTLESHIP_BHVOPENVALUESHTENERGYPARAM__).strValue ());
	if (oV.existOpenValue (__BATTLESHIP_BHVOPENVALUESHTTYPEPARAM__))
		_typeOfShooting = std::stoi (oV.openValue (__BATTLESHIP_BHVOPENVALUESHTTYPEPARAM__).strValue ());
	if (oV.existOpenValue (__BATTLESHIP_BHVOPENVALUESHIELDFACTORPARAM__))
		_shieldProtection = __BD std::stof (oV.openValue (__BATTLESHIP_BHVOPENVALUESHIELDFACTORPARAM__).strValue ());

	// To verify that the values are ok in debug mode...
	BATTLESHIP::Spaceship::Behaviour (_canBeHurmedByEnemies, _canBeHurmedBySimilars, 
		_energyLostPerLoop, _energyDetractedWhenHit, _pointsGivenWhenDestroyed, 
		_shootingEnergy, _typeOfShooting, _shieldProtection, _automaticMovement);
}

// ---
BATTLESHIP::Spaceship::Spaceship (int cId, BATTLESHIP::Spaceship::Behaviour* bhv, const QGAMES::Forms& f, 
		const QGAMES::Entity::Data& d)
	: SpaceElement (cId, bhv, f, d),
	  _powerLevel (0), // Meaning the very basic one...
	  _score (0), // What is the score of the battleship. Usually it makes only sense in battleship
	  _shootings ()
{ 
	// To end a shooting
	addBuoy (__BATTLESHIP_ENDSHOOTINGBUOY__, 
		new BATTLESHIP::Spaceship::ToEndShootingBuoy (__BATTLESHIP_ENDSHOOTINGBUOY__));
}

// ---
void BATTLESHIP::Spaceship::setBehaviour (BATTLESHIP::SpaceElement::Behaviour* bhv)
{
	// To guaranttee the right type...
	assert (dynamic_cast <BATTLESHIP::Spaceship::Behaviour*> (bhv));

	BATTLESHIP::SpaceElement::setBehaviour (bhv);
}

// ---
bool BATTLESHIP::Spaceship::toShoot (int f, const QGAMES::Vector& d, int nP)
{
	// If the wntity is not alive, it is not possible to shoot
	if (!isAlive ())
		return (false);

	// If it is not possible to shoot, it doesn't continue...
	if (!isPossibleToShoot ())
		return (false);

	// Do we have enough shootings left to continue?
	if (shootingsRemaining () != -1 && // Meaning infinite...
		(shootingsRemaining () < instancesPerShooting ()))
		return (false); // To shoot is not possible because there are no more guns in the spaceship...

	// Two action at the same time:
	// To count how many shooting there are now in the space, 
	// and to identify the ones still free to be later used, if any!
	int nSF = 0;
	QGAMES::Entities tS;
	for (QGAMES::Entities::const_iterator i = _shootings.begin (); i != _shootings.end (); i++)
	{
		if (!(*i).second -> isVisible ())
		{
			if ((int) tS.size () < instancesPerShooting ())
				tS [(*i).first] = (*i).second;
		}
		else
		if (((BATTLESHIP::SpaceshipShooting*) (*i).second) -> isFlying ())
			nSF++; // Another one flying...
	}

	// Can there be more shootings flying in the space?
	if (maxNumberOfShootingsInSpaceSimultaneouly () != -1 && // Meaning no limit...
		(nSF >= maxNumberOfShootingsInSpaceSimultaneouly ()))
		return (false); // ...no, So Sorry!!

	// It could happen that most of the shootings are in the air
	// So there wouldn't be enought shootings to show...
	// So it is time to left...
	if (tS.size () != instancesPerShooting ())
		return (false); 

	// Finally it is time to shoot!!!
	int j = 0;
	int iPS = instancesPerShooting ();
	for (QGAMES::Entities::const_iterator i = tS.begin (); i != tS.end (); i++, j++)
	{
		BATTLESHIP::SpaceshipShooting* sS = dynamic_cast <BATTLESHIP::SpaceshipShooting*> ((*i).second);
		assert (sS); // Just in case...It shouldn't ever be possible because it has been checked when loaded...

		sS -> setOwner (this);
		sS -> setEnergy (((BATTLESHIP::Spaceship::Behaviour*) behaviour ()) -> _shootingEnergy);
		sS -> setType (((BATTLESHIP::Spaceship::Behaviour*) behaviour ()) -> _typeOfShooting);
		sS -> setMap (map ()); // The same map than the spaceship...
		sS -> initialize ();
		observe (sS); // To know the events...
		// Start to move...
		// The location of the launch is though from the perspective of a 2D game (typical in arcade)
		sS -> toLaunchFrom (shootingPosition (j, iPS, nP), f, d); 
	}

	reduceShootingsRemaining (instancesPerShooting ()); // ...we have less

	return (true); // It was possible to shoot...
}

// ---
void BATTLESHIP::Spaceship::toSeal (QGAMES::bdata s)
{
	if (s == __BD 0 && isSealed ())
		onOffSwitch (_SWITCHSEALED) -> set (false); // End of sealed...
	else
	{
		onOffSwitch (_SWITCHSEALED) -> set (true); // Sealed!
		counter (_COUNTERTIMESEALED) -> initialize 
			((s == __BD -1) ? __MAXINT__ : (int) (s * __BD game () -> framesPerSecond ()), 0, true, true); // Time sealed!
	}
}

// ---
void BATTLESHIP::Spaceship::setPowerLevel (int pL)
{
	assert (isPossibleToShoot ()); // This state is possible only when shooting is possible...

	if ((_powerLevel = pL) >= __BATTLESHIP_NUMBEROFPOWERLEVELS__)
		_powerLevel = (__BATTLESHIP_NUMBEROFPOWERLEVELS__ - 1);

	// Right type of shooting?
	assert (__BATTLESHIPDATAGAME__ -> _LEVELDATA [_powerLevel][0] >= 0 && 
			__BATTLESHIPDATAGAME__ -> _LEVELDATA [_powerLevel][0] < __BATTLESHIP_NUMBERSHOOTINGTYPES__); 
	((BATTLESHIP::Spaceship::Behaviour*) behaviour ()) -> 
		_typeOfShooting = __BATTLESHIPDATAGAME__ -> _LEVELDATA [_powerLevel][0];
	((BATTLESHIP::Spaceship::Behaviour*) behaviour ()) -> 
		_shootingEnergy = __BD __BATTLESHIPDATAGAME__ -> _LEVELDATA [_powerLevel][4];

	// In this case, the number of shootings available is incremented (notice the negative sign)
	reduceShootingsRemaining ((maxNumberOfShootingsAvailable () == -1) ? 0 : -maxNumberOfShootingsAvailable ()); 
}

// ---
void BATTLESHIP::Spaceship::setScore (int s)
{
	if (_score == s)
		return; // nothing to do if nothing changes...

	_score = s;
	if (_score <= 0)
		_score = 0; // Always greater than 0...
	if (_score > 99999)
		_score = 99999; // Never bigger than 100.000...

	// A notification is sent, with the new value...
	QGAMES::SetOfOpenValues val;
	val.addOpenValue (__BATTLESHIP_SPACESHIPSCOREVALUEPARAMETER__, QGAMES::OpenValue (_score));
	notify (QGAMES::Event (__BATTLESHIP_SPACESHIPSCOREACTUALIZED__, this, val));
}

// ---
void BATTLESHIP::Spaceship::loadShootings (const QGAMES::Entities& sth)
{
	// The shootings will replace what there were in the previous list...
	for (QGAMES::Entities::const_iterator i = sth.begin (); i != sth.end (); i++)
		if (dynamic_cast <BATTLESHIP::SpaceshipShooting*> ((*i).second))
			_shootings [(*i).first] = (*i).second; // Only those that are shootings are included in the list...
}

// ---
BATTLESHIP::SpaceshipShooting* BATTLESHIP::Spaceship::firstAvailableShooting ()
{
	BATTLESHIP::SpaceshipShooting* result = NULL;
	for (QGAMES::Entities::const_iterator i = _shootings.begin (); i != _shootings.end () && !result; i++)
		if ((*i).second -> isVisible ())
			result = (BATTLESHIP::SpaceshipShooting*) (*i).second;

	return (result);
}

// ---
QGAMES::Position BATTLESHIP::Spaceship::shootingPosition (int nS, int tS, int nP) const
{
	assert (tS > nS && nS >= 0);

	QGAMES::Vector sPos ((((__BD (tS - 1)) / __BD 2) - __BD nS) * 
		__BD ((tS == 1) ? 0 : (((visualLength () - 20) >> 1) / (tS >> 1))), __BD 0, __BD 0);
	return (position () + sPos + QGAMES::Vector
		(__BD (currentForm () -> frameWidthForFrame (currentAspect ()) >> 1), __BD 0, __BD 0));
}

// ---
void BATTLESHIP::Spaceship::initialize ()
{
	BATTLESHIP::SpaceElement::initialize ();

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	_score = 0;

	// Restart the shootings...
	for (QGAMES::Entities::const_iterator i = _shootings.begin (); 
		i != _shootings.end (); (*i++).second -> setVisible (false));
}

// ---
void BATTLESHIP::Spaceship::updatePositions ()
{
	// It is has died...
	// Before dissapearing, then it is needed no gove a set of point to the one who hist it the most...
	if (hasExploded ())
	{
		BATTLESHIP::Spaceship* wH = dynamic_cast <BATTLESHIP::Spaceship*> (whoHitTheMost ());
		if (wH != NULL) // If the one who hits this spaceship the most was another spaceship, 
						// then my points are given to that!
			wH -> setScore (wH -> score () + behaviour () -> _pointsGivenWhenDestroyed);
	}

	// Is this invocation the element can be destroyed if it has previously exploded!
	BATTLESHIP::SpaceElement::updatePositions ();

	BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (bG);
	if (bG -> isGamePaused () || bG -> isNotPlaying ())
		return; // Just in case, the time doesn't progress...

	if (isSealed () && counter (_COUNTERTIMESEALED) -> isEnd ())
		toSeal (0); // Never more...
	// Notice that it is not defined what to do while sealed
	// this is somethig that must be defined at other level
}

// ---
void BATTLESHIP::Spaceship::whenCollisionWith (QGAMES::Entity* e)
{
	if (!isAlive ())
		return; // Nothing to do when it is not moving...

	BATTLESHIP::SpaceElement::whenCollisionWith (e);

	// If the spaceship is sealed nothing affect it
	if (isSealed ())
		return;

	// A collision with a space ship has happen
	// Obviously it is considered a "similar"
	// So if it can be hurmed by it, the energy of this one is reduced take into 
	// account the power of the element hitting this one.
	if (dynamic_cast <BATTLESHIP::Spaceship*> (e))
	{
		BATTLESHIP::Spaceship* sS = dynamic_cast <BATTLESHIP::Spaceship*> (e);
		if (sS -> isAlive () &&
			((!sS -> isEnemy (this) && behaviour () -> _canBeHurmedBySimilars) ||
			 (sS -> isEnemy (this) && behaviour () -> _canBeHurmedByEnemies)))
		{
			hitBy (sS);

			setEnergyToAdd (-sS -> behaviour () -> _energyDetractedWhenHit /
				dynamic_cast <BATTLESHIP::Spaceship::Behaviour*> (behaviour ()) -> _shieldProtection); 

			game () -> sound (__BATTLESHIP_SPACESHIPHITSANOTHERELMNTSOUNDID__) 
				-> play (__QGAMES_ARTISTSOUNDCHANNEL__);
		}
	}

	else
	// A collision with an asteriod has happen
	// In this case, an asteroid is always considered as an enemy of the spaceship
	if (dynamic_cast <BATTLESHIP::Asteroid*> (e))
	{
		BATTLESHIP::Asteroid* a = dynamic_cast <BATTLESHIP::Asteroid*> (e);
		if (a -> isAlive () && 
			a -> isEnemy (this) && behaviour () -> _canBeHurmedByEnemies)
		{
			hitBy (a);

			setEnergyToAdd (-a -> behaviour () -> _energyDetractedWhenHit);
			// By default the asteroid energy detracted when hit is 100...
			// So this instruction will be equivalent to destroy the spaceship

			game () -> sound (__BATTLESHIP_SPACESHIPHITSANOTHERELMNTSOUNDID__) 
				-> play (__QGAMES_ARTISTSOUNDCHANNEL__);
		}
	}

	else
	// The collision has happen against a shooting...(and it is not exploding)
	// So, if the shooting doesn't belong to this spaceship, the energy is reduced and the score incremented
	// both regarding the power of the shooting itself...
	if (dynamic_cast <BATTLESHIP::SpaceshipShooting*> (e) && 
		((BATTLESHIP::SpaceshipShooting*) (e)) -> isFlying ())
	{
		BATTLESHIP::SpaceshipShooting* sS = dynamic_cast <BATTLESHIP::SpaceshipShooting*> (e);
		if (sS -> isVisible () && sS -> owner () != this && 
			(sS -> owner () -> isEnemy (this) && behaviour () -> _canBeHurmedByEnemies ||
			 sS -> owner () -> isEnemy (this) && behaviour () -> _canBeHurmedBySimilars))
		{
			hitBy (sS -> owner ());

			setEnergyToAdd (-sS -> energy () /
				dynamic_cast <BATTLESHIP::Spaceship::Behaviour*> (behaviour ()) -> _shieldProtection);

			game () -> sound (__BATTLESHIP_SPACESHIPHITSANOTHERELMNTSOUNDID__) 
				-> play (__QGAMES_ARTISTSOUNDCHANNEL__);
		}
	}

	// Other things are not generic to any type of space ship...
}

// ---
void BATTLESHIP::Spaceship::processEvent (const QGAMES::Event& evnt)
{
	switch (evnt.code ())
	{
		// When the shooting is launched, the event is transmited to the scene
		// To be added there. Remember that the shootings are only added when they are shooted, 
		// and removed from the scene when they finishes...
		case __BATTLESHIP_SHOOTINGLAUNCHED__:
			notify (evnt); // The event is transmited to the scene...
			break;

		// If the shooting can't move (e.g it is out of the screen), then it has to "disappear"
		// and the be ready for the use of another spaceship...
		// But the event is also transmited to the scene to be removed from the scene...
		// Because the shooting is going to be removed from any place
		// The loop processing the events can become unestable, so the removal has to be done in other cycle
		case __BATTLESHIP_SHOOTINGTOEND__:
			{
				BATTLESHIP::Spaceship::ToEndShootingBuoy* b = 
					dynamic_cast <BATTLESHIP::Spaceship::ToEndShootingBuoy*> (buoy (__BATTLESHIP_ENDSHOOTINGBUOY__));
				assert (b); // Just in case...
				
				b -> addShooting ((BATTLESHIP::SpaceshipShooting*) evnt.data ());
				b -> active (true);
				notify (evnt);
			}

			break;

		default:
			break;
	}

	BATTLESHIP::SpaceElement::processEvent (evnt);
}

// ---
__IMPLEMENTCOUNTERS__ (BATTLESHIP::Spaceship::Counters)
{
	addCounter (new QGAMES::Counter (BATTLESHIP::Spaceship::_COUNTERTIMESEALED, 
		0 /** It will be set later too. */, 0, true, true)); 
}

// ---
__IMPLEMENTONOFFSWITCHES__ (BATTLESHIP::Spaceship::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(BATTLESHIP::Spaceship::_SWITCHSEALED, false)); // Not sealed by default...
}

// ---
void BATTLESHIP::Spaceship::ToEndShootingBuoy::addShooting (BATTLESHIP::SpaceshipShooting* sht)
{ 
	assert (sht); 
	
	if (std::find_if (_shootings.begin (), _shootings.end (),
			[=](BATTLESHIP::SpaceshipShooting* i) -> bool { return (i == sht); }) == _shootings.end ())
		_shootings.push_back (sht); 
}

// ---
void* BATTLESHIP::Spaceship::ToEndShootingBuoy::treatFor (QGAMES::Element* e)
{
	BATTLESHIP::Spaceship* sS = dynamic_cast <BATTLESHIP::Spaceship*> (e);
	assert (sS); // Just incase, but it would be different never...

	// If the same soothing were twice in the list, it would get erased just once...
	std::for_each (_shootings.begin (), _shootings.end (), 
		[=](BATTLESHIP::SpaceshipShooting* i) { i -> toEnd (); sS -> unObserve (i); }); // End and left ubobserved...
	_shootings.clear (); // Done

	return (this);
}

// ---
QGAMES::Entity* BATTLESHIP::Asteroid::clone () const
{
	QGAMES::Entity* result = new BATTLESHIP::Asteroid (_id, _behaviour -> clone (), _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
bool BATTLESHIP::Asteroid::isEnemy (const BATTLESHIP::SpaceElement* ss) const
{
	return (dynamic_cast <const BATTLESHIP::Battleship*> (ss) != NULL);
}

// ---
void BATTLESHIP::Asteroid::setType (int t)
{
	assert (t == -1 || (t >= 0 && t < __BATTLESHIP_NUMBEROFASTEROIDTYPES__));

	_type = (t == -1) ? (rand () % __BATTLESHIP_NUMBEROFASTEROIDTYPES__) : t;
	_statesId = BATTLESHIP::Asteroid::StatesId 
		(_type << 1, __BATTLESHIP_ASTEROIDBLANKSTATE__, 1 + (_type << 1), 
			__BATTLESHIP_ASTEROIDEXPLODINGSTATE__, __BATTLESHIP_ASTEROIDBLANKSTATE__ );

	setStateFor (spaceElementState (), orientation ());
}

// ---
void BATTLESHIP::Asteroid::initialize ()
{
	setType (0);

	BATTLESHIP::SpaceElement::initialize ();
}

// ---
QGAMES::Rectangle BATTLESHIP::Asteroid::collisionZone () const
{
	assert (currentForm ());

	QGAMES::Vector offSet (
		__BD currentForm () -> frameXOffsetForFrame (currentAspect ()), 
		__BD currentForm () -> frameYOffsetForFrame (currentAspect ()),
		__BD 0);

	QGAMES::Vector size (
		__BD currentForm () -> frameWidthForFrame (currentAspect ()),
		__BD currentForm () -> frameHeightForFrame (currentAspect ()), 
		__BD 0);

	QGAMES::Vector lSize = __BD 0.10 * size;
	
	return (QGAMES::Rectangle (_data._position + offSet + lSize, _data._position + offSet + size - lSize));
}

// ---
void BATTLESHIP::Asteroid::whenCollisionWith (QGAMES::Entity* e)
{
	if (!isAlive ())
		return; // Nothing else to do when it is not moving anymore...

	BATTLESHIP::SpaceElement::whenCollisionWith (e);
	
	// With any generic spaceship...
	if (dynamic_cast <BATTLESHIP::Spaceship*> (e))
	{
		BATTLESHIP::Spaceship* sS = dynamic_cast <BATTLESHIP::Spaceship*> (e);
		if (sS -> isAlive () && 
			sS -> isEnemy (this) && behaviour () -> _canBeHurmedByEnemies)
		{
			toExplodeDeferred (); // Just hitting the space ship (if moving), the asterior explode
								  // Depeding on its power the space ship (if it possible) would be destroyed too!
								  // The default value says that it is destroyed...

			notify (QGAMES::Event (__BATTLESHIP_SPACEELEMENTDESTROYED__, this));

			game () -> sound (__BATTLESHIP_SPACESHIPEXPLODESOUNDID__) 
				-> play (__QGAMES_ARTISTSOUNDCHANNEL__);
		}
	}

	else
	// When it is hit by a shooting, its energy is reduced from the asteroid...
	// Only when hit by an enemy!
	if (dynamic_cast <BATTLESHIP::SpaceshipShooting*> (e) &&
		((BATTLESHIP::SpaceshipShooting*) (e)) -> isFlying ())
	{
		BATTLESHIP::SpaceshipShooting* sS = dynamic_cast <BATTLESHIP::SpaceshipShooting*> (e);
		if ((sS -> owner () -> isEnemy (this) && behaviour () -> _canBeHurmedByEnemies) ||
			(!sS -> owner () -> isEnemy (this) && behaviour () -> _canBeHurmedBySimilars))
		{
			hitBy (sS -> owner ());

			setEnergyToAdd (-sS -> energy ());
		}
	}

	// Any other type of collision doesn't affect the asteriod...
}

// ---
void BATTLESHIP::Asteroid::setStateFor (int st, const QGAMES::Vector& o)
{
	switch (st)
	{
		case _FROZEN:
			setCurrentState (_statesId._frozenState);
			break;

		case _HIDDEN:
			setCurrentState (_statesId._hiddenState);
			break;

		case _MOVING:
			{
				setCurrentState (_statesId._movingState);

				QGAMES::SimpleLinearMovement* lM = 
					dynamic_cast <QGAMES::SimpleLinearMovement*> (currentMovement ());
				assert (lM); // Just in case...
				lM -> setSpeed (lM -> speed () * (__BD 1 + (__BD referenceSpeed () / __BD 10))); 
				// Increments the speed of the asteroid based on the speed of the scene...
			}

			break;

		case _EXPLODING:
			setCurrentState (_statesId._explodingState);
			break;

		case _DESTROYED:
			setCurrentState (_statesId._destroyedState);
			break;

		default:
			assert (0); // No others states have been defined in the default implementation of asteroid
	}
}

// ---
void BATTLESHIP::ThingToCatch::setType (int t)
{
	assert (t == -1 || (t >= 0 && t < __BATTLESHIP_NUMBEROFTHINGTOCATCHTYPES__));

	_type = (t == -1) ? (rand () % __BATTLESHIP_NUMBEROFTHINGTOCATCHTYPES__) : t;
	_statesId = BATTLESHIP::ThingToCatch::StatesId 
		(_type << 1, __BATTLESHIP_THINGTOCATCHBLANKSTATE__, 1 + (_type << 1), 
			__BATTLESHIP_THINGTOCATCHEXPLODINGSTATE__, __BATTLESHIP_THINGTOCATCHBLANKSTATE__ );

	setStateFor (spaceElementState (), orientation ());
}

// ---
void BATTLESHIP::ThingToCatch::initialize ()
{
	setType (0);

	BATTLESHIP::SpaceElement::initialize ();
}

// ---
void BATTLESHIP::ThingToCatch::whenCollisionWith (QGAMES::Entity* e)
{
	if (!isAlive ())
		return; // Nothing else to do when it is not moving anymore...

	BATTLESHIP::SpaceElement::whenCollisionWith (e);

	// Destroyed when hit by a shooting (whoever the owner is)...
	if (dynamic_cast <BATTLESHIP::SpaceshipShooting*> (e) != NULL &&
		((BATTLESHIP::SpaceshipShooting*) (e)) -> isFlying ())
	{
		// This method is usually invoked before events are processed, so 
		// There is no problem in changing the state...
		// Anyway it is invoked deferred because the shooting
		// could check the state to make something either, 
		// and it is never knwon whether this method is invoked first that the other one or the other way around!		
		toExplodeDeferred ();

		// Instead of reducing the energy of the energy element
		// and let it to "explode" during the inEveryLoop method execution
		// the explosion is automatic.
		// Probably this is a bigger punish than the other behaviour!

		notify (QGAMES::Event (__BATTLESHIP_SPACEELEMENTDESTROYED__, this));

		// It sounds...
		game () -> sound (__BATTLESHIP_SPACESHIPEXPLODESOUNDID__) -> play (__QGAMES_ARTISTSOUNDCHANNEL__);
	}

	// Caught when hit by the battleship...directly!
	else
	if (dynamic_cast <BATTLESHIP::Battleship*> (e) != NULL)
	{
		BATTLESHIP::Battleship* bS = dynamic_cast <BATTLESHIP::Battleship*> (e);

		if (bS -> isAlive ())
		{
			BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
			assert (bG);

			if (!bG -> dataGame () -> _clickNeededToCatchThings && bS -> isAlive ())
			{
				toHide ();

				notify (QGAMES::Event (__BATTLESHIP_SPACEELEMENTCAUGHT__, this));
				// As pill is usually part of an action block, this...
				// ...event is to let it know that it has dissapeared

				setVisible (false);
			}
		}
	}

}

// ---
void BATTLESHIP::ThingToCatch::setStateFor (int st, const QGAMES::Vector& o)
{
	switch (st)
	{
		case _FROZEN:
			setCurrentState (_statesId._frozenState);
			break;

		case _HIDDEN:
			setCurrentState (_statesId._hiddenState);
			break;

		case _MOVING:
			{
				setCurrentState (_statesId._movingState);
				QGAMES::SimpleLinearMovement* lM = 
					dynamic_cast <QGAMES::SimpleLinearMovement*> (currentMovement ());
				assert (lM); // Just in case...
				lM -> setSpeed (lM -> speed () * (__BD 1 + (__BD referenceSpeed () / __BD 10))); 
				// Increments the speed of the pill to catch based on the speed of the scene...
			}

			break;

		case _DESTROYED:
			setCurrentState (_statesId._destroyedState);
			break;

		case _EXPLODING:
			setCurrentState (_statesId._explodingState);
			break;

		default:
			assert (0); // No others states have been defined in the default implementation of the "thing to catch"
	}
}
