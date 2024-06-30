#include "Sabreman.hpp"
#include "Orchild.hpp"
#include "Movements.hpp"
#include "Maps.hpp"
#include "Game.hpp"

// ---
Sabreman::Sabreman (const QGAMES::Forms& f, const QGAMES::Entity::Data& d) 
	: QGAMES::Artist (__SABREWULFSABREMAN__, f, d),
	  _fighting (false),
	  _internalState (Sabreman::InternalState::_NORMAL),
	  _amuletPieces (__NUMBEROFAMULETPIECES__),
	  _counter (0), _walkingSound (false),
	  _blinking (false), _blinkingState (false),
	  _counterBeforeBlinking (0),
	  _counterBlinkings (0), _counterBlinks (0),
	  _counterStopped (0),
	  _counterAfterStopped (0),
	  _yellowOrchildsAffects (true),
	  _lastMov (QGAMES::Vector (__BD 1, __BD 0, __BD 0)),
	  _initialFrameRight (0), _finalFrameRight (0),
	  _initialFrameLeft (0), _finalFrameLeft (0),
	  _initialFrameUp (0), _finalFrameDown (0),
	  _cInitialFrame (0), _cFinalFrame (0),
	  _lastOrientation (QGAMES::Vector::_cero),
	  _lastMonsterOrientation (QGAMES::Vector::_cero)
	  // ---
	  #ifndef NDEBUG
	  ,_onOffDrawLimits (true)
	  #endif
	  // ---
{
	// Visible by default...
	setVisible (true);

	for (int i = 0; i < __NUMBEROFAMULETPIECES__; i++)
		_amuletPieces [i] = false;
}

// ---
bool Sabreman::canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
{
	assert (_currentMovement);
	assert (_currentForm);
	assert (_map);

	// If the interal State is stopped...
	// Then sabreman can not move!
	if (_internalState == Sabreman::InternalState::_STOPPED)
		return (false);

	// Calculates the next position and the position to test according
	// with the direction sabrewulf has...
	// Just the reference position has been calculated...
	// It is needed to verify whether it is or not free
	return (((SabreWulfMazePlace*) _map) -> isPositionFree (referenceZone (d) + d));
}

// ---
QGAMES::Rectangle Sabreman::collisionZone () const
{
	// A form has to exist...
	assert (_currentForm);

	// The collision zone is the center of the entity moving...
	QGAMES::Vector v (__BD (_currentForm -> frameWidth () >> 2), 
		__BD (_currentForm -> frameHeight () >> 2), __BD 0);
	return (QGAMES::Rectangle (_data._position + v, _data._position + (3 * v)));
}

// ---
QGAMES::Position Sabreman::referenceZone (const QGAMES::Vector& d) const
{
	QGAMES::Rectangle collZone = collisionZone ();
	QGAMES::Position refPos = collZone.pos1 ();
	// Going to the right...
	if (d.posX () == __BD 1) 
	{
		// ..and nothing else
		if (d.posY () == __BD 0) refPos = collZone.pos3 ();
		else
		// ...and also going down...
		if (d.posY () == __BD 1) refPos = collZone.pos2 ();
		else
		// ...and also going up...
		if (d.posY () == __BD -1) refPos = collZone.pos3 ();
	}
	else
	// Going to the left, or just not moving int he x axis...
	if (d.posX () == __BD -1 || d.posX () == __BD 0)
		// ...and also going down, the pos1 can't be...
		if (d.posY () == __BD 1) refPos = collZone.pos4 ();

	// Returns the zone...
	return (refPos);
}

// ---
void Sabreman::setFighting (bool f)
{
	// Nothing to change, if the new status is the same than old...
	if (_fighting == f)
		return; 

	// Fighting is only possible when sabreman is in normal state...
	if (f && _internalState != Sabreman::InternalState::_NORMAL)
		return;

	_fighting = f; 
	determinateFormAndFrames ();
	_cInitialFrame = (_lastMov.posX () == 1) ? _initialFrameRight : _initialFrameLeft; 
	_cFinalFrame = (_lastMov.posX () == 1) ? _finalFrameRight : _finalFrameLeft;
	setCurrentAspect (_cInitialFrame);
	// The method determinateFormAndFrames doesn't set the aspect
	// This is because that method when sabreman blinks, has to manintain
	// exactly the aspect and change only the form!
}

// ---
void Sabreman::setInternalState (Sabreman::InternalState st)
{ 
	// If nothing significant is going to change then returns...
	if (st == _internalState)
		return;

	Sabreman::InternalState oState = _internalState;
	_internalState = st; 

	_blinking = false;
	_blinkingState = false;
	_counterBeforeBlinking = 0;
	_counterBlinkings = 0; _counterBlinks = 0;

	_counterStopped = 0;
	_counterAfterStopped = 0;
	_yellowOrchildsAffects = true;

	// When the state changes, the fighting stops...
	_fighting = false;

	determinateFormAndFrames ();
	// The method determinateFormAndFrames doesn't set the aspect
	// This is because that method when sabreman blinks, has to maintain
	// exactly the aspect and change only the form!
	// If the change is "complex" the new aspect has to be set!
	if (((oState == Sabreman::InternalState::_NORMAL ||
		  oState == Sabreman::InternalState::_FAST ||
		  oState == Sabreman::InternalState::_INMORTAL) && 
		 (_internalState == Sabreman::InternalState::_NORMAL ||
		  _internalState == Sabreman::InternalState::_GOINGBACK ||
		  _internalState == Sabreman::InternalState::_STOPPED)) ||
		((_internalState == Sabreman::InternalState::_NORMAL ||
		  _internalState == Sabreman::InternalState::_FAST ||
		  _internalState == Sabreman::InternalState::_INMORTAL) && 
		 (oState == Sabreman::InternalState::_NORMAL ||
		  oState == Sabreman::InternalState::_GOINGBACK ||
		  oState == Sabreman::InternalState::_STOPPED)))
	{
		_cInitialFrame = _initialFrameRight; _cFinalFrame = _finalFrameRight;
		
		if (_lastMov.posX () != 0)
		{
			_cInitialFrame = (_lastMov.posX () == 1) ? _initialFrameRight : _initialFrameLeft; 
			_cFinalFrame = (_lastMov.posX () == 1) ? _finalFrameRight : _finalFrameLeft;
		}
		else
		{
			_cInitialFrame = (_lastMov.posY () == 1) ? _initialFrameDown : _initialFrameUp; 
			_cFinalFrame = (_lastMov.posY () == 1) ? _finalFrameDown : _finalFrameUp;
		}

		setCurrentAspect (_cInitialFrame);

		// Sounds something...
		game () -> sound (__SABREMANFLOWER__) -> play (__SABREWULF_BACKGROUNDCHANNEL__);
	}

	// Fix the speed...depending on the state...
	assert (_currentMovement); // It has to exist. It is not normal the opposite
	int bS = ((SabreWulfArtistMovement*) _currentMovement) -> baseSpeed ();
	((SabreWulfArtistMovement*) _currentMovement) -> setSpeed 
		((_internalState == Sabreman::InternalState::_FAST) ? (bS * 8) : (bS * 4));
}

// ---
void Sabreman::setAmultePiece (int nP)
{
	assert (nP < __NUMBEROFAMULETPIECES__);
	_amuletPieces [nP] = true;
}

// ---
bool Sabreman::isCarryingPiece (int nP) const
{
	assert (nP < __NUMBEROFAMULETPIECES__);
	return (_amuletPieces [nP]);
}

// ---
bool Sabreman::isCarryingAmulet () const
{
	bool result = true;
	for (int i = 0; i < __NUMBEROFAMULETPIECES__ && result; i++)
		result &= isCarryingPiece (i);
	return (result);
}

// ---
void Sabreman::leaveAmulets ()
{
	for (int i = 0; i < __NUMBEROFAMULETPIECES__; i++)
		_amuletPieces [i] = false;
}

// ---
bool Sabreman::couldKill (SabreWulfMonster* m)
{
	assert (m);

	if (!m -> isNastie ())
		return (false); // If the monster is not a nastie, it can be never killed!
	if (!isFighting ())
		return (false); // If he is not fighting it wouldn't be possible to kill anything!

	// if it is not a nastie and sabreman is fighting...
	// the result depends on the relative postion between both...
	return (!isMonsterCommingBack (m));
}

// ---
bool Sabreman::couldDamage (SabreWulfInmortal* iM)
{
	assert (iM);
	return (iM -> couldBeDamagedBySabreman () && !isMonsterCommingBack (iM));
}

// ---
void Sabreman::setVisible (bool v)
{ 
	QGAMES::Artist::setVisible (v);
	if (_walkingSound)
	{
		_walkingSound = false;
		game () -> sound (__SABREMANWALKINGSOUND__) -> stop ();
	}
}

// ---
int Sabreman::mazePlaceNumber () const
{
	assert (_map);
	return (((SabreWulfMazePlace*) _map) -> mazePlaceNumber ());
}

// ---
void Sabreman::initialize ()
{
	QGAMES::Artist::initialize ();

	setVisible (true);

	setFighting (false); // By default it is not fighting...
	setInternalState (Sabreman::InternalState::_NORMAL); // Normal aspect by default...
	for (int i = 0; i < __NUMBEROFAMULETPIECES__; i++)
		_amuletPieces [i] = false; // Not carrying anthing...

	_counter = 0; _walkingSound = false;

	_blinking = false; _blinkingState = false;
	_counterBeforeBlinking = 0;
	_counterBlinkings = 0; _counterBlinks = 0;

	_counterStopped = 0;
	_counterAfterStopped = 0;
	_yellowOrchildsAffects = true;

	_lastMov = QGAMES::Vector (__BD 1, __BD 0, __BD 0);

	// The form and the aspect...
	determinateFormAndFrames (); // The form is set here
	_cInitialFrame = _initialFrameRight; _cFinalFrame = _finalFrameRight;
	setCurrentAspect (_cInitialFrame);

	_lastOrientation = QGAMES::Vector::_cero;
}

// ---
void Sabreman::updatePositions ()
{
	// If Sabreman is not visible, he has not to be actualized!
	if (!isVisible ())
		return;

	// Change the aspect of the character when it is moving...
	// ..ans make a sound when it is neccessary!
	if (_data._orientation != QGAMES::Vector::_cero)
	{
		if (_counter++ == 4)
		{
			_counter = 0;
			if (_currentAspect++ >= _cFinalFrame)
				_currentAspect = _cInitialFrame;
		}

		if (!_walkingSound)
		{
			_walkingSound = true;
			game () -> sound (__SABREMANWALKINGSOUND__) -> play (__SABREWULF_PLAYERCHANNEL__);
			// The sound has been defined in the sound file as ethernal!
		}
	}

	// The aspect can change depending on the direction of the movement...
	if (_data._orientation != _lastOrientation)
	{
		// Keeps track of the not null last movement to the rigth or left...
		if (_data._orientation.posX () != __BD 0)
			_lastMov = _data._orientation; 
		// ...and the sound stops, when sabreman stands up!
		else
		{
			_walkingSound = false;
			game () -> sound (__SABREMANWALKINGSOUND__) -> stop ();
			// Take care, that the walking sound has been defined as ethernal in the definition file!
		}

		// Depends on the movement, the frames change!
		if (_data._orientation.posX () == 1)
		{
			_cInitialFrame = _initialFrameRight; 
			_cFinalFrame = _finalFrameRight;
		}
		else
		if (_data._orientation.posX () == -1)
		{
			_cInitialFrame = _initialFrameLeft; 
			_cFinalFrame = _finalFrameLeft;
		}
		else
		if (_data._orientation.posY () == 1)
		{
			_cInitialFrame = _initialFrameDown; 
			_cFinalFrame = _finalFrameDown;
			if (_fighting && _lastMov.posX () == -1)
			// When fighting the aspect moving down will depends on
			// the aspect when moving horizontally...
			// This is really important to control later
			// Whether the monsters affect or not sabreman!
			{
				_cInitialFrame = _initialFrameUp; 
				_cFinalFrame = _finalFrameUp;
			}
		}
		else
		if (_data._orientation.posY () == -1)
		{
			_cInitialFrame = _initialFrameUp; 
			_cFinalFrame = _finalFrameUp;
			if (_fighting && _lastMov.posX () == 1)
			// When fighting the aspect moving down will depends on
			// the aspect when moving horizontally...
			// This is really important to control later
			// Whether the monsters affect or not sabreman!
			{
				_cInitialFrame = _initialFrameDown; 
				_cFinalFrame = _finalFrameDown;
			}
		}

		_lastOrientation = _data._orientation;
		setCurrentAspect (_cInitialFrame);
	}

	// If sabreman is stopped...
	// it is only possible for a couple of seconds...
	if (_internalState == Sabreman::InternalState::_STOPPED)
	{
		if (_counterStopped++ ==
				((((SabreWulfGame*) game ()) -> levelInfo ()._secondsInNotNormalState >> 1) * 
					game () -> framesPerSecond ()))
		{
			setInternalState (Sabreman::InternalState::_NORMAL); // Backs to normal...
			_counterAfterStopped = 0; // ...for a while it can not be affected by yellow orchilds again
			_yellowOrchildsAffects = false; // and a counter is set to control.
		}
	}

	// counting 1/2 second after being affected by an yellow orchild...
	if (!_yellowOrchildsAffects && 
			_counterAfterStopped++ >= (game () -> framesPerSecond () >> 1))
		_yellowOrchildsAffects = true; //...and then they affect it again...

	// If Sabreman is not neither in the normal aspect nor stopped,
	// The it is needed to count the time, and then to start o blink
	// before going back to the normal state...
	// This part of the code controls the blinking state of sabreman...
	if (_internalState != Sabreman::InternalState::_NORMAL &&
		_internalState != Sabreman::InternalState::_STOPPED)
	{
		if (!_blinking)
		{
			if (_counterBeforeBlinking++ == 
				(((SabreWulfGame*) game ()) -> levelInfo ()._secondsInNotNormalState  * 
					game () -> framesPerSecond ()))
			{
				int cA = currentAspect ();
				_blinking = true; // Starts to blink...
				_blinkingState = false; // Initially doesn't blink!
				determinateFormAndFrames (); // Can change the aspect?...but only in the form!
				setCurrentAspect (cA); // Maintains the aspect...
				// When blinking, the current aspect doesn't change...it is important!!

				// The effect finishes...
				game () -> sound (__SABREMANENDFLOWER__) -> play (__SABREWULF_BACKGROUNDCHANNEL__);
				// This sound has not been defined as ethernal
				// so the time blinking has to be coordinated with the
				// time the sound plays!
			}
		}
		else
		{
			if (_counterBlinks++ == (int)
				(((SabreWulfGame*) game ()) -> levelInfo ()._secondsBetweenBlinks * 
					game () -> framesPerSecond ()))
			{
				_counterBlinks = 0;
				if (_counterBlinkings++ == 
					((SabreWulfGame*) game ()) -> levelInfo ()._numberOfBlinks)
					setInternalState (Sabreman::InternalState::_NORMAL);
					// After blinking X times, then goes back to the normal state...
				else
				{
					int cA = currentAspect ();
					_blinkingState = !_blinkingState;
					determinateFormAndFrames ();
					setCurrentAspect (cA); // Maintains the aspect...
					// When blinking, the current aspect doesn't change...it is important!!
				}
			}
		}
	}

	// Verify whether the character is out of the limits...
	// Taking into account the movement of the sabreman!
	// If it is a notification is thrown
	assert (_map);
	if (((SabreWulfMazePlace*) _map) -> isOutOfLimits (referenceZone (_data._orientation)))
		notify (QGAMES::Event (__SABREMANOUTOFLIMITS__, NULL));
	// Verifies whether the sabreman is or not in the exit zone, and carrying everything...
	// Obviouslly it could be true just in the case sabreman is in the exit maze place!
	if (((SabreWulfMazePlace*) _map) -> isInExitZone (referenceZone (_data._orientation)) &&
			isCarryingAmulet ())
		notify (QGAMES::Event (__SABREMANOUTEXITZONE__, NULL));

	// Normal treatement...
	QGAMES::Artist::updatePositions ();
}

// ---
void Sabreman::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Artist::drawOn (s, p);

	// While debug mode
	// A rectangle is drawn rounding the sabreman...
	#ifndef NDEBUG
	if (_onOffDrawLimits)
	{
		s -> drawRectangle (QGAMES::Rectangle (_data._position, _data._position + 
			QGAMES::Vector (__BD _currentForm -> frameWidth (), 
				__BD _currentForm -> frameHeight (), __BD 0)), __GRAYCOLOR__);
		s -> drawRectangle (collisionZone (), __GRAYCOLOR__);
	}
	#endif
}

// ---
#ifndef NDEBUG
void Sabreman::onOffDrawLimits ()
{
	_onOffDrawLimits = !_onOffDrawLimits;
}
#endif
// ---

// ---
void Sabreman::whenCollisionWith (QGAMES::Entity* e)
{
	assert (e);

	// If the collision is with an orchild...
	if (dynamic_cast <Orchild*> (e) != NULL)
	{
		Orchild* o = (Orchild*) e;
		switch (o -> type ())
		{
			case Orchild::Type::_RED:
				setInternalState (Sabreman::_INMORTAL);
				break;
			case Orchild::Type::_PINK:
				setInternalState (Sabreman::_GOINGBACK);
				break;
			case Orchild::Type::_BLUE:
				setInternalState (Sabreman::_FAST);
				break;
			case Orchild::Type::_YELLOW:
				if (_yellowOrchildsAffects) // Do yellow orchilds affect sabreman?
					setInternalState (Sabreman::_STOPPED);
				break;
			case Orchild::Type::_WHITE:
				setInternalState (Sabreman::_NORMAL);
				break;

			default:
				exit (1); // This status can't exit...

		}
	}
	else
	// If the collision is with a monster...
	// The direction the mosnter had is kept!
	// This direction is used later, when sabreman dies flying, 
	// To determinate the flying direction...
	if (dynamic_cast <SabreWulfMonster*> (e) != NULL)
		_lastMonsterOrientation = QGAMES::Vector (e -> orientation ().posX (), __BD 0, __BD 0);
}

// ---
void Sabreman::determinateFormAndFrames ()
{
	if (_fighting)
	{
		_initialFrameLeft	= __SABREMANFIGHTINITLEFTASPECT__; 
		_finalFrameLeft		= __SABREMANFIGHTENDLEFTASPECT__;
		_initialFrameRight	= __SABREMANFIGHTINITRIGHTASPECT__; 
		_finalFrameRight	= __SABREMANFIGHTENDRIGHTASPECT__;
		_initialFrameUp		= __SABREMANFIGHTINITLEFTASPECT__; 
		_finalFrameUp		= __SABREMANFIGHTENDLEFTASPECT__;
		_initialFrameDown	= __SABREMANFIGHTINITRIGHTASPECT__; 
		_finalFrameDown		= __SABREMANFIGHTENDRIGHTASPECT__;
		// When fighting up ans down frams are used in a different way...
		// @see update method...

		setCurrentForm (__SABREMANFIGHTINGFORM__);
	}
	else
	{
		// The frames used in sabreman are independent of the form...
		// when he is sat down!
		if (_internalState == Sabreman::InternalState::_STOPPED)
		{
			_initialFrameLeft	= 
			_finalFrameLeft		= 
			_initialFrameRight	= 
			_finalFrameRight	= 
			_initialFrameUp		= 
			_finalFrameUp		= 
			_initialFrameDown	= 
			_finalFrameDown		= __SABREMANSITASPECT__;
			setCurrentForm ((_lastOrientation.posX () == __BD -1) // Depending on the last orientation...
				? __SABREMANSTOPPEDLEFTFORM__ : __SABREMANSTOPPEDRIGHTFORM__);
		}
		else
		{
			_initialFrameLeft	= __SABREMANNINITLEFTASPECT__; 
			_finalFrameLeft		= __SABREMANNENDLEFTASPECT__;
			_initialFrameRight	= __SABREMANNINITRIGHTASPECT__; 
			_finalFrameRight	= __SABREMANNENDRIGHTASPECT__;
			_initialFrameUp		= __SABREMANNINITUPASPECT__; 
			_finalFrameUp		= __SABREMANNENDUPASPECT__;
			_initialFrameDown	= __SABREMANNINITDOWNASPECT__; 
			_finalFrameDown		= __SABREMANNENDDOWNASPECT__;

			switch (_internalState)
			{
				case Sabreman::InternalState::_NORMAL:
					setCurrentForm (__SABREMANNORMALFORM__);
					break;
				case Sabreman::InternalState::_FAST:
					setCurrentForm (__SABREMANSPEEDFORM__);
					break;
				case Sabreman::InternalState::_INMORTAL:
					setCurrentForm (__SABREMANINMORTALFORM__);
					break;
				case Sabreman::InternalState::_GOINGBACK:
					setCurrentForm (__SABREMANGOINGBACKFORM__);
					break;
				case Sabreman::InternalState::_STOPPED:
					break; // Nothing else to do...
				default:
					exit (1); // This status can't be possible...
			}

			// If sabreman is blinking, then the form is corrected depending
			// on the blinking state...
			if (_blinking && _blinkingState == false)
				setCurrentForm (__SABREMANNORMALFORM__);
		}
	}

	// Normally the initial and final frame of the movement must be set here...
	// but in this case, it is not needed. The reason
	// is that this method is also used when sabreman blinks, and when he blinks
	// only change the form, but not the aspect...

	// Reset the current aspect...
}

// ---
bool Sabreman::isMonsterCommingBack (SabreWulfMonster* mt)
{
	bool result = true;
	if (_lastMov.posX () == 1) // Sabreman going to the right...
	{
		if (mt -> position ().posX () >= position ().posX ()) // ...when the monster is in the right side...
			result = false; // then it is not comming back!
	}
	else // Going to the left...
	{
		if (position ().posX () >= mt -> position ().posX ()) // ...when the monster is in the left side...
			result = false; // same!
	}

	return (result);
}
