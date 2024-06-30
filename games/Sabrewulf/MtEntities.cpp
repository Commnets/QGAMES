#include "MtEntities.hpp"
#include "Movements.hpp"
#include "Maps.hpp"
#include "Game.hpp"
#include "Maps.hpp"
#include "Scenes.hpp"

// ---
SabreWulfMonster::State SabreWulfMonster::Behaviour::newState ()
{
	assert (_monster);
	// By default it returns the same state than the mosnter has now...
	return (_monster -> state ());
}

// ---
SabreWulfMonster::SabreWulfMonster (int id, const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: QGAMES::Artist (id, f, d),
	  _mazePlaceNumber (0),
	  _aspect (SabreWulfMonster::Aspect::_APE),
	  _state (SabreWulfMonster::State::__NOTALIVE),
	  _behaviour (NULL),
	  _sabreman (NULL),
	  _initialFrameRight (0), 
	  _finalFrameRight (0),
	  _initialFrameLeft (0), 
	  _finalFrameLeft (0),
	  _cInitialFrame (0), 
	  _cFinalFrame (0),
	  _counter (0), 
	  _decideTarget (false),
	  _numberSteps (0)
{
	// Nothing else to do so far...
}

// ---
bool SabreWulfMonster::canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
{
	assert (_map);
	assert (_behaviour);

	// Does the behaviour allow to move the monster?...
	if (! _behaviour -> canMove (d, a))
	{
		_decideTarget = true;
		return (false);
	}

	// The movement is only possible when the next position is free...
	// ...always in the maze place where the monster is based on!
	bool result = ((SabreWulfMazePlace*) _map) -> 
		isPositionFree (referenceZone (d) + d, _mazePlaceNumber);
	if (!result)
		_decideTarget = true;
	return (result);
}

// ---
QGAMES::Rectangle SabreWulfMonster::collisionZone () const
{
	// A form has to exist...
	assert (_currentForm);

	// The collision zone is the center of the entity moving...
	QGAMES::Vector v (__BD (_currentForm -> frameWidth () >> 2), 
		__BD (_currentForm -> frameHeight () >> 2), __BD 0);
	return (QGAMES::Rectangle (_data._position + v, _data._position + (3 * v)));
}

// ---
QGAMES::Position SabreWulfMonster::referenceZone (const QGAMES::Vector& d) const
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
void SabreWulfMonster::setMazePlaceNumber (int mP)
{
	assert (mP >=0 && mP < __NUMBEROFMAZEPLACES__);
	_mazePlaceNumber = mP;
}

// ---
void SabreWulfMonster::setState (SabreWulfMonster::State s)
{
	_state = s;
	setAspectAndForm ();
}

// ---
void SabreWulfMonster::setAspectAndState (SabreWulfMonster::Aspect a, SabreWulfMonster::State s)
{
	_aspect = a;
	_state = s;
	setAspectAndForm ();
}

// ---
bool SabreWulfMonster::couldBeDamagedBySabreman ()
{
	assert (_behaviour);
	return (_behaviour -> couldBeDamagedBySabreman ());
}

// ---
void SabreWulfMonster::initialize ()
{
	// The standard initialization supposses the 
	// monster is a nastie in the mazePlace 0 by default...
	// It's true this method is not going to be the
	// one to be usually executed!

	_mazePlaceNumber = 0;
	_sabreman = NULL;

	// The asepct depends on the aspect an the state...usually
	setAspectAndState (SabreWulfMonster::Aspect::_APE, SabreWulfMonster::State::__NOTALIVE);
	// Initially it has not a position defined in the maze
	setPosition (QGAMES::Position::_cero);

	_behaviour = NULL;

	_counter = 0;
	_decideTarget = false;
	_numberSteps = 0;
}

// ---
void SabreWulfMonster::initializeAs (SabreWulfMonster::Aspect a, int mP, 
	const QGAMES::Position& pos, Sabreman* s)
{
	assert (mP >= 0 && mP < __NUMBEROFMAZEPLACES__);
	assert (s);

	// Where the monster is based initially (this place changes for inmortals)
	_mazePlaceNumber = mP; 
	// To persuit sabreman, just in case!
	_sabreman = s; 

	// Not alive initially, and with the aspect received as parameter!
	// The specific initialization of nastie and inmortals
	// will decide the specific state at the beginning
	setAspectAndState (a, SabreWulfMonster::State::__NOTALIVE);

	// The position selected by the user...
	setPosition (pos); 
	
	// The behaviour will depend on the
	// type of monster: Two different behaviours, one for nasties, and another one for inmortals!
	delete (_behaviour);
	_behaviour = NULL;
	// The behaviour has to be set by the methods inheriting this one

	_counter = 0; // This counter has many uses...
	_decideTarget = false;
	_numberSteps = 0;
}

// ---
void SabreWulfMonster::inEveryLoop ()
{
	// Sets whether the monster is or not visible...
	// It is something to be done in every iteration because the monster (all types)
	// moves across differents maze places...
	setVisible (((SabreWulfMazePlace*) map ()) -> mazePlaceNumber () == _mazePlaceNumber &&
		_state != SabreWulfMonster::State::__NOTALIVE);
	QGAMES::Artist::inEveryLoop ();
}

// ---
void SabreWulfMonster::updatePositions ()
{
	// The monster updates even when it is not visible...
	// Because it can grow or similar!

	assert (_behaviour);

	SabreWulfMonster::State nS = _state;
	switch (_state)
	{
		// In this state we can have both nasties and inmortals!
		case SabreWulfMonster::State::_WAITING:
			nS = whenWaiting ();
			break;

		// This state is just for nasties only...
		case SabreWulfMonster::State::_GROWING:
			nS = whenGrowing ();
			break;

		// For nasties and inmortals...
		case SabreWulfMonster::State::_MOVING:
			nS = whenMoving ();
			break;

		// Just for nasties only...
		case SabreWulfMonster::State::_DIEING:
			nS = whenDieing ();
			break;

		// Just for nasties only...
		case SabreWulfMonster::State::__NOTALIVE:
			nS = whenNotLiving ();
			break;

		// Nothing to do by default...
		// In this state nobody cn be, but it is included just in case!
		default:
			break;
	};

	// If the state changes, 
	// the form and the aspect has to be adapted...
	if (nS != _state) 
	{
		setState (nS);
		_decideTarget = true;
	}
	// If it does change,
	// the state could change because the specific method of the behaviour says it!
	// It is like the last opportunity!
	else
	{
		nS = _behaviour -> newState ();
		if (nS != _state)
		{
			setState (nS);
			_decideTarget = true;
		}
	}

	QGAMES::Artist::updatePositions ();
}

// ---
void SabreWulfMonster::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Artist::drawOn (s, p);
	if (_behaviour)
		_behaviour -> drawOn (s, p); // Draws something special relative to the behaviour...
}

// ---
void SabreWulfMonster::whenCollisionWith (QGAMES::Entity* e)
{
	assert (e);
	assert (_behaviour);

	// Does the monster collisioning with sabreman?
	if (dynamic_cast <Sabreman*> (e) != NULL) 
	{
		Sabreman* s = (Sabreman*) e;
		// Only happens something when sabreman is not inmortal
		// ..and he is fighting either!
		// The specific reation depends on the type of monster...
		if (!s -> isInmortal () && s -> isFighting ())
		{
			_behaviour -> whatToDoWhenSabremanHitsIt ();
			game () -> sound (__SABREWULFHITANIMALSOUND__) -> play (__SABREWULF_INMORTALCHANNEL__);
			_decideTarget = true; // Hi, it is probable to change the direction!
		}
		// In other circunstances, the monster could die,
		// But it controlled in the game class
	}
}

// ---
SabreWulfMonster::State SabreWulfMonster::whenMoving ()
{
	assert (_behaviour);

	if (_decideTarget)
	{
		_decideTarget = false;

		// Sets a new direction for movement...
		QGAMES::Position oldDir = orientation ();
		QGAMES::Position nDir = _behaviour -> changeMovementDirection ();
		QGAMES::bdata sF = _behaviour -> changeMovementSpeedFactor ();
		setMove (nDir); 
		// When the monster movesit means it has a movement behaviour...
		((SabreWulfArtistMovement*) currentMovement ()) -> 
			setSpeed ((int) (sF * ((SabreWulfArtistMovement*) currentMovement ()) -> baseSpeed ()));

		// The number of steps to advanced before asking again
		// whether the position is or not right, will depend
		// on the behaviour. That's it the monster is a nastie or an inmortal!
		_numberSteps = (_behaviour -> numberStepsBeforeAsking ());

		// When the old and new direction doesn't match
		// the aspects have to be corrected!
		if (oldDir.posX () != nDir.posX ())
		{
			_cInitialFrame = _initialFrameRight; _cFinalFrame = _finalFrameRight;
			_currentAspect = _cInitialFrame;
			if (nDir.posX () == -1)
			{
				_cInitialFrame = _initialFrameLeft; _cFinalFrame = _finalFrameLeft;
				_currentAspect = _cInitialFrame;
			}
		}
	}
	else
	{
		setMove (_behaviour -> movementDirection ());
		if (_numberSteps-- <= 0)
		{
			_numberSteps = 0;
			_decideTarget = true; 
			// Next time a new target has to be defined...
		}
	}

	// Evolves the aspect, when it's the time!
	if (_counter++ >= 3)
	{
		_counter = 0;
		if (_currentAspect++ >= _cFinalFrame)
			_currentAspect = _cInitialFrame;
	}

	// ..always moving!...
	return (SabreWulfMonster::State::_MOVING);
}

// ---
SabreWulfMonster::State SabreWulfMonster::whenNotLiving ()
{
	return (SabreWulfMonster::State::__NOTALIVE);
}

// This variable describes the number of sounds when appear a nastie
// ...it is static because whatever the number of nasties ar
// the sound has to be unique
int SabreWulfNastie::_numberSoundsAppearing = 0;

// ---
SabreWulfNastie::SabreWulfNastie (int id, const QGAMES::Forms& f, 
		const QGAMES::Entity::Data& d)
	: SabreWulfMonster (id, f, d),
	  _counterAppearances (0), 
	  _counterNotVisible (0),
	  _timeToAppear (0), 
	  _numberOfAppears (0)
{
	// Nothing else to do so far...
}

// ---
void SabreWulfNastie::kill ()
{
	if (_state == SabreWulfMonster::State::_GROWING)
		soundAppearing (false); // Stops the sound when growing...just in case...
	setState (SabreWulfMonster::State::__NOTALIVE);
}

// ---
void SabreWulfNastie::initialize ()
{
	SabreWulfMonster::initialize ();

	_counterAppearances = 0;
	_counterNotVisible = 0;
	_timeToAppear = 0;
	_numberOfAppears = 0;
}

// ---
void SabreWulfNastie::initializeAs (SabreWulfMonster::Aspect a, int mP, 
		const QGAMES::Position& pos, Sabreman* s)
{
	assert (_aspect == _APE || 
			_aspect == _BIRD1 || 
			_aspect == _BIRD2 ||
			_aspect == _BIRD3 || 
			_aspect == _BUG || 
			_aspect == _LOBSTER || 
			_aspect == _MOTH || 
			_aspect == _MOUSE || 
			_aspect == _SCORPION ||
			_aspect == _SKUNK || 
			_aspect == _SNAKE1 || 
			_aspect ==_SNAKE2 ||
			_aspect == _SNAKE3 || 
			_aspect == _SPIDER1 || 
			_aspect == _SPIDER2 ||
			_aspect == _SPIDER3 || 
			_aspect == _THING ||
			_aspect == _FROG1 ||
			_aspect == _FROG2 ||
			_aspect == _LIZARD ||
			_aspect == _OWL1 ||
			_aspect == _OWL2 ||
			_aspect == _FIRE); // It has to be a nastie!

	SabreWulfMonster::initializeAs (a, mP, pos, s);

	setState (SabreWulfMonster::State::_WAITING);
	// In the case of a nastie, waiting forms and not alive forms are the same!

	if (_behaviour) delete (_behaviour); // Deletes a previous behaviour...
	_behaviour = (a == _FIRE) 
		? new FireBehaviour (this, _sabreman) : new NastieBehaviour (this, _sabreman);

	_counterAppearances = 0;
	_counterNotVisible = 0;
	// The time to appear the monster, is a random number of seconds
	// depending on the level of the game!
	// If it is the fire...the time is 0!
	_timeToAppear = (a == _FIRE) 
		? 0 : (rand () % (((SabreWulfGame*) game ()) -> levelInfo ()._maxTimeToAppearNasties + 1)) *
			game () -> framesPerSecond ();
	_numberOfAppears = 0;
}

// ---
void SabreWulfNastie::updatePositions ()
{
	// If the nastie is not visible
	// then counts the time to kill it, if it is moving
	// When sabreman changes the place, the nasties in the previous maze place
	// will disappear after a couple of seconds
	// This is just to avoid sabre man goes back and forward to "avoid"
	// the nasties of a concrete maze place!
	if (!isVisible ())
	{
		// If the state is growing...
		// switch off the sound, and prepare it to move next time!
		if (_state == SabreWulfMonster::State::_GROWING)
		{
			soundAppearing (false); 

			_counterAppearances = 0;
			setMove (QGAMES::Vector (__BD 0, __BD 0, __BD 0)); 
			_decideTarget = true; // First thing to decide where sabreman is...or
			setState (SabreWulfMonster::State::_MOVING); 
		}
		// If the state is dieing...
		// and the nastie is not already visible, then is the same that not alive!
		else
		if (_state == SabreWulfMonster::State::_DIEING)
		{
			_decideTarget = false; 
			setState (SabreWulfMonster::State::__NOTALIVE); 
		}
		// In any other state (usually when moving),
		// After a couple of seconds, the monster disappears...
		else
		if (_counterNotVisible++ >= 2 * game () -> framesPerSecond ())
		{
			_counterNotVisible = 0;
			setState (SabreWulfMonster::State::__NOTALIVE);
			// After two seconds being not visible...
		}
	}
	// The normal behaviour if the nastie is visible!
	else
		SabreWulfMonster::updatePositions ();
}

// ---
void SabreWulfNastie::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	// If the nastie's state is waiting, then it is not visible either!
	if (_state == SabreWulfMonster::State::_WAITING) return;
	SabreWulfMonster::drawOn (s, p);
}

// ---
void SabreWulfNastie::soundAppearing (bool s)
{
	if (s)
	{
		if (_numberSoundsAppearing++ == 0)
			game () -> sound (__SABREWULFNASTIEAPPEARS__) -> play (__SABREWULF_NASTIECHANNELBASE__);
	}
	else
	{
		if (--_numberSoundsAppearing == 0)
			game () -> sound (__SABREWULFNASTIEAPPEARS__) -> stop ();
	}
}

// ---
SabreWulfMonster::State SabreWulfNastie::whenWaiting ()
{
	SabreWulfMonster::State result = SabreWulfMonster::State::_WAITING;
	if (_counter++ >= _timeToAppear)
	{
		_counter = 0;

		// Depending on the form assigned
		// the original position defined is corrected to be at the center of the nastie
		setPosition (position () - 
			QGAMES::Vector (__BD (_currentForm -> frameWidth () >> 1), 
				__BD (_currentForm -> frameHeight () >> 1), __BD 0));
		// The number of times the process of appearing is repeted...
		_numberOfAppears = 1 + 
			(rand () % (((SabreWulfGame*) game ()) -> levelInfo ()._maxNumberOfAppearances));
		_counterAppearances = 0;
		// Starts to sound...
		soundAppearing (true); 

		// After all of this, the new status is to grow...
		result = SabreWulfMonster::State::_GROWING;
	}

	return (result);
}

// ---
SabreWulfMonster::State SabreWulfNastie::whenGrowing ()
{
	SabreWulfMonster::State result = SabreWulfMonster::State::_GROWING;
	if (_counter++ >= 3)
	{
		_counter = 0;
		if (_currentAspect++ >= _cFinalFrame)
		{
			_currentAspect = _cInitialFrame;
			if (_counterAppearances++ >= _numberOfAppears)
			{
				_counterAppearances = 0;

				// First time in the new state, the direction has to be decide
				setMove (QGAMES::Vector (__BD 0, __BD 0, __BD 0)); 
				_decideTarget = true; 
				// No more sound...if there is none more growing!
				soundAppearing (false);

				// The new status is to start to move...
				result = SabreWulfMonster::State::_MOVING;
			}
		}
	}

	return (result);
}

// ---
SabreWulfMonster::State SabreWulfNastie::whenDieing ()
{
	SabreWulfMonster::State result = SabreWulfMonster::State::_DIEING;
	if (_counter++ > 3)
	{
		_counter = 0;
		if (_currentAspect++ >= _cFinalFrame)
		{
			_currentAspect = _cInitialFrame;
			if (_counterAppearances++ >= _numberOfAppears)
			{
				_counterAppearances = 0;
				_decideTarget = false; // Just in case, the monster was killed before decide!

				result = SabreWulfMonster::State::__NOTALIVE; 
			}
		}
	}

	return (result);
}

// ---
void SabreWulfNastie::setAspectAndForm ()
{
	switch (_state)
	{
		case SabreWulfMonster::State::_GROWING:
			// The aspects don't depend on nastie's form!
			// Fire has an special form and the number of aspects are also different...
			_initialFrameRight = 0;  
			_finalFrameRight = (_aspect == SabreWulfMonster::Aspect::_FIRE) 
				? (__SABREWULFFIREBORNASPECTS__ - 1) : (__SABREWULFNASTIEBORNASPECTS__ - 1);
			_initialFrameLeft = 0;
			_finalFrameLeft = (_aspect == SabreWulfMonster::Aspect::_FIRE) 
				? (__SABREWULFFIREBORNASPECTS__ - 1) : (__SABREWULFNASTIEBORNASPECTS__ - 1);
			_cInitialFrame = _initialFrameLeft;
			_cFinalFrame = _finalFrameLeft;
			setCurrentForm ((_aspect == SabreWulfMonster::Aspect::_FIRE) ?
				__SABREWULFFIREBORNFORM__ : (__SABREWULFBASENASTIEBORNFORM__ + (int) _aspect)); // Sets the Form...
			setCurrentAspect (_cInitialFrame); // ...and aspect
			setCurrentMovement (__MININT__);
			break;

		case SabreWulfMonster::State::_MOVING:
			// The aspects don't depend on nastie's form!
			// Fire has an special form and the number of aspects are also different...
			_initialFrameRight = (_aspect == SabreWulfMonster::Aspect::_FIRE)
				? __SABREWULFFIRERIGHTINIT__ : __SABREWULFNASTIERIGHTINIT__;  
			_finalFrameRight = (_aspect == SabreWulfMonster::Aspect::_FIRE)
				? __SABREWULFFIRERIGHTEND__ : __SABREWULFNASTIERIGHTEND__;
			_initialFrameLeft = (_aspect == SabreWulfMonster::Aspect::_FIRE)
				? __SABREWULFFIRELEFTINIT__ : __SABREWULFNASTIELEFTINIT__;
			_finalFrameLeft = (_aspect == SabreWulfMonster::Aspect::_FIRE)
				? __SABREWULFFIRELEFTEND__ : __SABREWULFNASTIELEFTEND__;
			_cInitialFrame = _initialFrameLeft;
			_cFinalFrame = _finalFrameLeft;
			setCurrentForm ((_aspect == SabreWulfMonster::Aspect::_FIRE) 
				? __SABREWULFFIREFORM__ : __SABREWULFBASENASTIEFORM__ + (int) _aspect); // Sets the Form...
			setCurrentAspect (_cInitialFrame); // ...and aspect
			setCurrentMovement ((_aspect == SabreWulfMonster::Aspect::_FIRE) 
				? __SABREWULFFIREMOVS__ : (__SABREWULFMONSTERBASEMOV__ + (_id - __SABREWULFNASTIESBASE__)));
			break;

		case SabreWulfMonster::State::_DIEING:
			// The aspects don't depend on nastie's form!
			_initialFrameRight = 0;  
			_finalFrameRight = (_aspect == SabreWulfMonster::Aspect::_FIRE)
				? (__SABREWULFFIREDIEASPECTS__ - 1) : (__SABREWULFNASTIEDIEASPECTS__ - 1);
			_initialFrameLeft = 0;
			_finalFrameLeft = (_aspect == SabreWulfMonster::Aspect::_FIRE)
				? (__SABREWULFFIREDIEASPECTS__ - 1) : (__SABREWULFNASTIEDIEASPECTS__ - 1);
			_cInitialFrame =_initialFrameLeft;
			_cFinalFrame = _finalFrameLeft;
			setCurrentForm ((_aspect == SabreWulfMonster::Aspect::_FIRE)
				? __SABREWULFFIREDIEFORM__ : (__SABREWULFBASENASTIEDIEFORM__ + (int) _aspect)); // Sets the Form...
			setCurrentAspect (_cInitialFrame); // ...and aspect
			setCurrentMovement (__MININT__);
			break;

		case SabreWulfMonster::State::_WAITING:
		case SabreWulfMonster::State::__NOTALIVE:
		default:
			// No aspect...it is not visible...
			_initialFrameRight =	0;  
			_finalFrameRight =		0;
			_initialFrameLeft =		0;
			_finalFrameLeft =		0;
			_cInitialFrame =		0;
			_cFinalFrame =			0;
			setCurrentForm ((*_forms.begin ()).first); // The first form...just for defining one! 
			setCurrentAspect (0); 
			setCurrentMovement (__MININT__);
			break;
	}
}

// ---
QGAMES::Vector NastieBehaviour::changeMovementDirection ()
{
	assert (_sabreman);
	assert (_monster);

	int dx = 0; int dy = 0;
	// 75% possibilities to follow sabreman...
	if (__3IN4PROBABLE__) 
	{
		QGAMES::Position sPos = _sabreman -> position ();
		QGAMES::Position mPos = _monster -> position ();
		dx = (sPos.posX () > mPos.posX ()) 
			? 1 : ((sPos.posX () < mPos.posX ()) ? -1 : 0);
		dy = (sPos.posY () > mPos.posY ()) 
			? 1 : ((sPos.posY () < mPos.posY ()) ? -1 : 0);
	}
	else 
	{
		// 50% moving within in this 25% remaining...
		// ...so the most improbable thing is to stay stopped!
		if (__1IN2PROBABLE__) 
		{
			// So when move...33% left, 33% right, and 33% stopped...
			// For both axis of movement!
			dx = (__1IN3PROBABLE__) ? 1 : ((__1IN2PROBABLE__) ? -1 : 0);
			dy = (__1IN3PROBABLE__) ? 1 : ((__1IN2PROBABLE__) ? -1 : 0);
		}
	}

	// If the movement is not possible it is modified...
	// until the movement is possible or it is cero!
	QGAMES::Vector result = QGAMES::Vector (__BD dx, __BD dy, __BD 0);
	while (result != QGAMES::Vector::_cero && !_monster -> canMove (result, QGAMES::Vector::_cero))
	{
		if (dy == 0)
		{
			// It the movement is already horizontally, and it is not possible
			// the movementy is not more possible!
			if (dx != 0)
				dx = 0;
		}
		// If the monster can move vertically, 
		// and the movement is not possible, then tries a pure
		// Horizontal movement...
		else 
		{
			dy = 0;
		}

		result = QGAMES::Vector (__BD dx, __BD dy, __BD 0);
	}

	_lastMovement = result;
	return (result);
}

// ---
void NastieBehaviour::whatToDoWhenSabremanHitsIt ()
{
	// Just if the nastie can die and it is moving
	// the nastie dies...then after it, it will start back again!
	// Nasties can't die when they are borning
	// The monster always die, even if sabreman dies as well!
	if (_monster -> state () == SabreWulfMonster::State::_MOVING || 
		_monster -> state  () == SabreWulfMonster::State::_WAITING)
	{
		_monster -> game () -> sound (__SABREWULFNASTIEDIESOUND__) -> 
			play (__SABREWULF_NASTIECHANNELBASE__); // A simple sound when dieing...
		_monster -> setState (SabreWulfMonster::State::_DIEING);
	}
}

// ---
QGAMES::Vector FireBehaviour::changeMovementDirection ()
{
	assert (_sabreman);
	assert (_monster);

	int dx = 0; int dy = 0;
	QGAMES::Position sPos = _sabreman -> position ();
	QGAMES::Position mPos = _monster -> position ();
	dx = (sPos.posX () > mPos.posX ()) 
		? 1 : ((sPos.posX () < mPos.posX ()) ? -1 : 0);
	dy = (sPos.posY () > mPos.posY ()) 
		? 1 : ((sPos.posY () < mPos.posY ()) ? -1 : 0);

	// If the movement is not possible it is modified...
	// until the movement is possible or it is cero!
	QGAMES::Vector result = QGAMES::Vector (__BD dx, __BD dy, __BD 0);
	while (result != QGAMES::Vector::_cero && !_monster -> canMove (result, QGAMES::Vector::_cero))
	{
		if (dy == 0)
		{
			// It the movement is already horizontally, and it is not possible
			// the movementy is not more possible!
			if (dx != 0)
				dx = 0;
		}
		// If the monster can move vertically, 
		// and the movement is not possible, then tries a pure
		// Horizontal movement...
		else 
		{
			dy = 0;
		}

		result = QGAMES::Vector (__BD dx, __BD dy, __BD 0);
	}

	_lastMovement = result;
	return (result);
}

// ---
SabreWulfInmortal::SabreWulfInmortal (int id, const QGAMES::Forms& f, 
		const QGAMES::Entity::Data& d)
	: SabreWulfMonster (id, f, d) 
{
	// Nothing else to do so far...
}

// ---
void SabreWulfInmortal::initialize ()
{
	SabreWulfMonster::initialize ();
}

// ---
void SabreWulfInmortal::initializeAs (SabreWulfMonster::Aspect a, 
	int mP, const QGAMES::Position& pos, Sabreman* s)
{
	assert (a == _RHINOLEFT ||
		    a == _RHINORIGHT ||
			a == _SPEARMANBLUE || 
			a == _SPEARMANPINK || 
			a == _SPEARMANRED || 
			a == _WOLF || 
			a == _HIPPO); // It has to be a inmortal!

	SabreWulfMonster::initializeAs (a, mP, pos, s);

	// Puts the state and sets the aspect
	// The state depends on the type of inmortal
	// The hippo state at the beginning is just waiting
	// while the state of the rest ismoving!
	setState ((_aspect == SabreWulfMonster::Aspect::_HIPPO) 
		? SabreWulfMonster::State::_WAITING : SabreWulfMonster::State::_MOVING);

	// Additional initializations for specific monsters...
	if (_behaviour) delete (_behaviour);
	switch (_aspect)
	{
		case _RHINOLEFT:
		case _RHINORIGHT:
			{
				// If the inmortal is a rhino
				// The initial movement is something radom: right or left?
				bool rMov = ((rand() % 10) > 5);
				QGAMES::Vector initMovement = rMov	
					? QGAMES::Vector (__BD 1, __BD 0, __BD 0) : QGAMES::Vector (__BD -1, __BD 0, __BD 0);
				_decideTarget = true; // First time to decide whether the direction is or not right!
				_behaviour = new RhinoBehaviour (this, _sabreman, initMovement);
				setMove (initMovement); // Start to move to the right...
				_cInitialFrame = (rMov) ? _initialFrameRight : _initialFrameLeft;
				_cFinalFrame = (rMov) ? _finalFrameRight : _finalFrameLeft;
				setCurrentAspect (_cInitialFrame);
			}
			
			break;

		case _HIPPO:
			_behaviour = new HippoBehaviour (this, _sabreman);
			// There is anything else necessary to do, because
			// hippo initial form is sleeping one. The form ans teh aspect has
			// ben already set within the method setState.
			break;

		case _WOLF:
			{
				// If the inmortal is a wolf
				// The initial movement is something radom: right or left?
				bool rMov = ((rand() % 10) > 5);
				QGAMES::Vector initMovement = rMov	
					? QGAMES::Vector (__BD 1, __BD 0, __BD 0) : QGAMES::Vector (__BD -1, __BD 0, __BD 0);
				_decideTarget = true; // First time to decide whether the direction is or not right!
				_behaviour = new WolfBehaviour (this, _sabreman, initMovement);
				setMove (initMovement); // Start to move to the right...
				_cInitialFrame = (rMov) ? _initialFrameRight : _initialFrameLeft;
				_cFinalFrame = (rMov) ? _finalFrameRight : _finalFrameLeft;
				setCurrentAspect (_cInitialFrame);
			}			

			break;

		case _SPEARMANBLUE:
		case _SPEARMANPINK:
		case _SPEARMANRED:
			_behaviour = new SpearmanBehaviour (this, _sabreman);
			_decideTarget = true; // Spear starts always deciding the target...
			break;

		default:
			exit (1); // It shouldn't happen!
	}

	setPosition (position () - QGAMES::Vector (__BD 0, __BD _currentForm -> frameHeight (), __BD 0));
}

// ---
void SabreWulfInmortal::initializeAs (int a, int mP, const QGAMES::Position& pos, Sabreman* s)
{
	SabreWulfMonster::Aspect as = 
		SabreWulfMonster::Aspect::_APE; // Just to avoid bad constructions!
	
	if (a == 0x30) as = SabreWulfMonster::Aspect::_RHINOLEFT;
	else if (a == 0x32) as = SabreWulfMonster::Aspect::_RHINORIGHT;
	else if (a == 0x3C) as = (SabreWulfMonster::Aspect) 
		((int) SabreWulfMonster::Aspect::_SPEARMANBLUE + (rand () % 3));
	else if (a == 0x50) as = SabreWulfMonster::Aspect::_WOLF;
	else if (a == 0xA0) as = SabreWulfMonster::Aspect::_HIPPO;
	assert (as != SabreWulfMonster::Aspect::_APE); // It should happen...
	initializeAs (as, mP, pos, s);
}

// ---
void SabreWulfInmortal::updatePositions ()
{
	SabreWulfMonster::updatePositions ();
}

// ---
void SabreWulfInmortal::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	SabreWulfMonster::drawOn (s, p);
}

// ---
SabreWulfMonster::State SabreWulfInmortal::whenWaiting ()
{
	return (SabreWulfMonster::State::_WAITING);
}

// ---
SabreWulfMonster::State SabreWulfInmortal::whenGrowing ()
{
	return (SabreWulfMonster::State::_GROWING);
}

// ---
SabreWulfMonster::State SabreWulfInmortal::whenMoving ()
{
	assert (_map);
	SabreWulfMonster::whenMoving ();
	if (((SabreWulfMazePlace*) _map) -> isOutOfLimits (referenceZone (_data._orientation)))
		notifyFrom (QGAMES::Event (__INMORTALOUTOFLIMITS__, NULL), this);
	// Due to this instruction monsters can live the room whre they were born!
	return (SabreWulfMonster::State::_MOVING);
}

// ---
SabreWulfMonster::State SabreWulfInmortal::whenDieing ()
{
	return (SabreWulfMonster::State::_DIEING);
}

// ---
void SabreWulfInmortal::setAspectAndForm ()
{
	switch (_state)
	{
		case SabreWulfMonster::State::_WAITING:
			// The only inmortal able to wait is the hippo (sleeping)
			if (_aspect == SabreWulfMonster::Aspect::_HIPPO)
			{
				_initialFrameRight =  
				_finalFrameRight =
				_initialFrameLeft =
				_finalFrameLeft =
				_cInitialFrame =
				_cFinalFrame = 0;
				setCurrentForm (__SABREWULFHIPPOSLEEPFORM__); 
				setCurrentAspect (0); 
				setCurrentMovement (__MININT__);
				break;
			}
			// ...none other inmortal has special effects!
			else
			{
				_initialFrameRight =	0;  
				_finalFrameRight =		0;
				_initialFrameLeft =		0;
				_finalFrameLeft =		0;
				_cInitialFrame =		0;
				_cFinalFrame =			0;
				setCurrentForm ((*_forms.begin ()).first); // The first form...just for defining one! 
				setCurrentAspect (0); 
				setCurrentMovement (__MININT__);
				break;
			}

			break;

		case SabreWulfMonster::State::_MOVING:
			// The aspect when moving depends on the type of inmortal
			switch (_aspect)
			{
				case SabreWulfMonster::Aspect::_HIPPO:
					_initialFrameRight =	__SABREWULFHIPPORIGHTINIT__;
					_finalFrameRight =		__SABREWULFHIPPORIGHTEND__;
					_initialFrameLeft =		__SABREWULFHIPPOLEFTINIT__;
					_finalFrameLeft =		__SABREWULFHIPPOLEFTEND__;
					_cInitialFrame = _initialFrameRight;
					_cFinalFrame = _finalFrameRight;
					setCurrentForm (__SABREWULFHIPPOFORM__); 
					setCurrentAspect (_cInitialFrame); 
					setCurrentMovement (__SABREWULFINMORTALMOV__);
					break;

				case SabreWulfMonster::Aspect::_RHINOLEFT:
				case SabreWulfMonster::Aspect::_RHINORIGHT:
					_initialFrameRight =	__SABREWULFRHINORIGHTINIT__;
					_finalFrameRight =		__SABREWULFRHINORIGHTEND__;
					_initialFrameLeft =		__SABREWULFRHINOLEFTINIT__;
					_finalFrameLeft =		__SABREWULFRHINOLEFTEND__;
					_cInitialFrame = _initialFrameRight;
					_cFinalFrame = _finalFrameRight;
					setCurrentForm (__SABREWULFRHINOFORM__); 
					setCurrentAspect (_cInitialFrame); 
					setCurrentMovement (__SABREWULFINMORTALMOV__);
					break;

				case SabreWulfMonster::Aspect::_WOLF:
					_initialFrameRight =	__SABREWULFWOLFRIGHTINIT__;
					_finalFrameRight =		__SABREWULFWOLFRIGHTEND__;
					_initialFrameLeft =		__SABREWULFWOLFLEFTINIT__;
					_finalFrameLeft =		__SABREWULFWOLFLEFTEND__;
					_cInitialFrame = _initialFrameRight;
					_cFinalFrame = _finalFrameRight;
					setCurrentForm (__SABREWULFWOLFFORM__); 
					setCurrentAspect (_cInitialFrame); 
					setCurrentMovement (__SABREWULFINMORTALMOV__);
					break;

				case SabreWulfMonster::Aspect::_SPEARMANBLUE:
				case SabreWulfMonster::Aspect::_SPEARMANPINK:
				case SabreWulfMonster::Aspect::_SPEARMANRED:
					_initialFrameRight =	__SABREWULFSPEARMANRIGHTINIT__;
					_finalFrameRight =		__SABREWULFSPEARMANRIGHTEND__;
					_initialFrameLeft =		__SABREWULFSPEARMANLEFTINIT__;
					_finalFrameLeft =		__SABREWULFSPEARMANLEFTEND__;
					_cInitialFrame = _initialFrameRight;
					_cFinalFrame = _finalFrameRight;
					setCurrentForm (
						(_aspect == SabreWulfMonster::Aspect::_SPEARMANBLUE) ? __SABREWULFSPEARMANBLUEFORM__ :
						((_aspect == SabreWulfMonster::Aspect::_SPEARMANPINK) ? __SABREWULFSPEARMANPINKFORM__ :
							__SABREWULFSPEARMANREDFORM__)); 
					setCurrentAspect (_cInitialFrame); 
					setCurrentMovement (__SABREWULFINMORTALMOV__);
					break;

				default:
					exit (1); // It shouldn't happen!
			}

			break;

		// These states are not possible for an inmortal!
		case SabreWulfMonster::State::_GROWING:
		case SabreWulfMonster::State::__NOTALIVE:
		case SabreWulfMonster::State::_DIEING:
		default:
			// No aspect...it is not visible...
			_initialFrameRight =	0;  
			_finalFrameRight =		0;
			_initialFrameLeft =		0;
			_finalFrameLeft =		0;
			_cInitialFrame =		0;
			_cFinalFrame =			0;
			setCurrentForm ((*_forms.begin ()).first); // The first form...just for defining one! 
			setCurrentAspect (0); 
			setCurrentMovement (__MININT__);
			break;
	}
}

// ---
QGAMES::Vector RhinoBehaviour::changeMovementDirection ()
{
	assert (_monster);

	// If Rhino is moving, then keep track of that last movement
	QGAMES::Vector dir = _monster -> orientation ();
	if (dir != QGAMES::Vector::_cero)
		_lastOrientation = dir; // Keeps last direction not null...

	// If Rhino can't move then change the direction thjat it was following!
	if (!_monster -> canMove (_lastOrientation, QGAMES::Vector::_cero))
		_lastOrientation = -_lastOrientation; 

	// If has been decided that it is time to run away from
	// sabreman, the relative positions of both entities are taking into 
	// accounto to decide the direction of the movement...
	if (_toRunAwayFromSabreman)
	{
		_lastOrientation = 
			QGAMES::Vector ((_sabreman -> position ().posX () >= 
				_monster -> position ().posX ()) ? __BD -1 : __BD 1, __BD 0, __BD 0);
		_toRunAwayFromSabreman = false;
	}

	// Finally returns the orientation decided!
	return (_lastOrientation);
}

// ---
void RhinoBehaviour::whatToDoWhenSabremanHitsIt ()
{
	_toRunAwayFromSabreman = true;
}

// ---
QGAMES::Vector HippoBehaviour::changeMovementDirection ()
{
	if (_toStartToRun)
	{
		_lastOrientation = 
			QGAMES::Vector (__1IN2PROBABLE__ ? __BD -1 : __BD 1, __BD 0, __BD 0);
		_toStartToRun = false;
	}
	else
		_lastOrientation = RhinoBehaviour::changeMovementDirection ();
	return (_lastOrientation);
}

// ---
void HippoBehaviour::whatToDoWhenSabremanHitsIt ()
{
	assert (_sabreman);
	assert (_monster);

	// Hippo behaviour inherits from Rhino behaviour
	// If Hippo was waiting (it was sleeping) and sabreman
	// hits it, then hippo start to run from sabreman...
	if (_monster -> state () == SabreWulfMonster::State::_WAITING)
	{
		// Hippo starts to move, first time it is hit!
		_monster -> setState (SabreWulfMonster::State::_MOVING);
		_monster -> setOrientation (QGAMES::Vector::_cero); // For the application to decide later!
		_toRunAwayFromSabreman = true;
	}
	// The normal behaviour is similar to hippo...
	else
		RhinoBehaviour::whatToDoWhenSabremanHitsIt ();
}

// ---
SabreWulfMonster::State HippoBehaviour::newState ()
{
	// Hippo has 1% probabilities to change state!
	SabreWulfMonster::State result = _monster -> state ();
	if (__1IN1000PROBABLE__)
	{
		if (result == SabreWulfMonster::State::_WAITING)
		{
			// Hippo starts to move, first time it is hit!
			result = SabreWulfMonster::State::_MOVING;
			_toStartToRun = true;
		}
		else
		{
			// Hippo must stop...
			result = SabreWulfMonster::State::_WAITING;
			_lastOrientation = QGAMES::Vector::_cero;
			_monster -> setMove (_lastOrientation);
		}
	}

	return (result);
}

// ---
QGAMES::Vector WolfBehaviour::changeMovementDirection ()
{
	assert (_monster);

	// Wolf doesn't stop ever!
	// If pass through everything!
	QGAMES::Vector dir = _monster -> orientation ();
	if (dir != QGAMES::Vector::_cero)
		_lastOrientation = dir;
	if (!_monster -> canMove (_lastOrientation, QGAMES::Vector::_cero))
		_lastOrientation = -_lastOrientation; // Changes the direction if the movement is not possible!

	return (_lastOrientation);
}

// ---
QGAMES::bdata WolfBehaviour::changeMovementSpeedFactor ()
{
	assert (_monster);
	assert (_sabreman);

	// When sabreman is inthe same "row" the wolf is moving
	// ..and it is behing sabreman, its speed goes up to pursuit better sabreman!
	QGAMES::bdata result = 4;
	int mL = _monster -> mazePlaceNumber () / __NUMBEROFMAZEPLACESX__;
	int mC = _monster -> mazePlaceNumber () % __NUMBEROFMAZEPLACESX__;
	int sL = _sabreman -> mazePlaceNumber () / __NUMBEROFMAZEPLACESX__;
	int sC = _sabreman -> mazePlaceNumber () % __NUMBEROFMAZEPLACESX__;
	
	if ((mL == sL) && 
			((mC < sC && _monster -> orientation () == QGAMES::Vector (__BD 1, __BD 0, __BD 0)) ||
			 (mC > sC && _monster -> orientation () == QGAMES::Vector (__BD -1, __BD 0, __BD 0))))
		result = 6; // faster than sabreman, but not so fast as when sabreman is "blue" orchild

	return (result);
}

// ---
SpearmanBehaviour::SpearmanBehaviour (SabreWulfMonster* m, Sabreman* s)
	: InmortalBehaviour (m, s),
	  _state (_RANDOM),
	  _hit (false),
	  _movDirs (),
	  _firstTimeInState (true),
	  _path (), _oldpath (),
	  _lastMazePlaceVisited (0),
	  _positionToVisit (-1),
	  _lastPositionVisited (-1),
	  _lastDirection (QGAMES::Vector::_cero),
	  _ax (0), _ay (0),
	  _countAx (0), _countAy (0),
	  _incX (0), _incY (0),
	  _biggerX (true),
	  _counterRandom (0),
	  _counterRunningAway (0),
	  _randomMov (false)
{
	_movDirs.resize (8);
	_movDirs [0] = QGAMES::Vector (__BD  0, __BD -1, __BD 0);
	_movDirs [1] = QGAMES::Vector (__BD  1, __BD -1, __BD 0);
	_movDirs [2] = QGAMES::Vector (__BD  1, __BD  0, __BD 0);
	_movDirs [3] = QGAMES::Vector (__BD  1, __BD  1, __BD 0);
	_movDirs [4] = QGAMES::Vector (__BD  0, __BD  1, __BD 0);
	_movDirs [5] = QGAMES::Vector (__BD -1, __BD  1, __BD 0);
	_movDirs [6] = QGAMES::Vector (__BD -1, __BD  0, __BD 0);
	_movDirs [7] = QGAMES::Vector (__BD -1, __BD -1, __BD 0);
}

// ---
bool SpearmanBehaviour::canMove (const QGAMES::Vector& or, const QGAMES::Vector& a)
{
	if (_positionToVisit == -1 || _path.empty ()) // No path...
		return (true); // Not position to visiti already calculated...

	QGAMES::Position o = _monster -> position () +
		QGAMES::Vector (__BD (_monster -> currentForm () -> frameWidth () >> 1),
			__BD (_monster -> currentForm () -> frameHeight () >> 1), __BD 0);

	bool result = true;
	if (((int) o.posX () == (int) _path [_lastMazePlaceVisited]._positions [_positionToVisit].posX ()) &&
		or.posX () != 0)
		result = false;
	else
	if (((int) o.posY () == (int) _path [_lastMazePlaceVisited]._positions [_positionToVisit].posY ()) &&
		or.posY () != 0)
		result = false;
	return (result);
}

// ---
QGAMES::Vector SpearmanBehaviour::changeMovementDirection ()
{
	assert (_monster);
	assert (_sabreman);

	QGAMES::Vector result = QGAMES::Vector::_cero;

	SpearmanBehaviour::State oState = _state;
	_state = decideState ();
	// If spearman has stopped...
	// is because the rules to decide a movement didn't work so fine...
	// Then this is a kind of exit to calculate another new movement!
	if (_state != oState || _lastDirection == QGAMES::Vector::_cero)
	{
		_oldpath = _path; // Copy the last path to use it in the case spearman has to run away...
		_lastPositionVisited = _positionToVisit;
		_path = std::map <int, SabreWulfScene::Path> ();
		_firstTimeInState = true;
		// This variable is used by the next methods to control, what to do
		// when it is the first time sabreman is in a internal state.
	}
	
	switch (_state)
	{
		case SpearmanBehaviour::State::_RANDOM:
			if (_counterRandom++ >= (5 * 
				(_monster -> game () -> framesPerSecond () / numberStepsBeforeAsking ())))
			{
				_counterRandom = 0;
				_randomMov = __1IN4PROBABLE__;
				_firstTimeInState = true;
				_path = std::map <int, SabreWulfScene::Path> ();
			}
				
			result = (_randomMov) ? randomMovement () : randomDirection (false);

			break;

		case SpearmanBehaviour::State::_RUNNINGAWAY:
			result = randomDirection (false, true); 
			// False means he doesn't have to pursuit sabreman, and running away from him...
			break;

		case SpearmanBehaviour::State::_PURSUIT:
			result = randomDirection (true); 
			// True means he has to pursuit sabreman...
			break;

		default:
			exit (1); // It coun't happen!
	};

	_lastDirection = result;
	return (result);
}

// ---
QGAMES::bdata SpearmanBehaviour::changeMovementSpeedFactor ()
{
	QGAMES::bdata result = 3; // A little bit slower than sabreman normally!
	switch (_state)
	{
		case SpearmanBehaviour::State::_PURSUIT:
			result = 4;
			break; // Same speed than sabreman!

		case SpearmanBehaviour::State::_RUNNINGAWAY:
			result = 2;
			break; // run, run run!

		case SpearmanBehaviour::State::_RANDOM:
		default:
			break;
	};

	return (result);
}

// ---
QGAMES::Vector SpearmanBehaviour::movementDirection ()
{
	// When there is path described...
	if (_path.empty ())
		return (_lastDirection);

	// ...when there is a path to follow...
	QGAMES::Vector result = QGAMES::Vector::_cero;

	if (_biggerX)
	{
		if (_countAx++ >= _ax)
		{
			_countAx = 0;
			if (_countAy++ >= _ay)
				_countAy = 0;

			if (_ay != 0)
				result = QGAMES::Vector (__BD 0, __BD _incY, __BD 0);
			else
				result = QGAMES::Vector (__BD _incX, __BD 0, __BD 0);
		}
		else
			result = QGAMES::Vector (__BD _incX, __BD 0, __BD 0);
	}
	else
	{
		if (_countAy++ >= _ay)
		{
			_countAy = 0;
			if (_countAx++ >= _ax)
				_countAx = 0;

			if (_ax != 0)
				result = QGAMES::Vector (__BD _incX, __BD 0, __BD 0);
			else
				result = QGAMES::Vector (__BD 0, __BD _incY, __BD 0);
		}
		else
			result = QGAMES::Vector (__BD 0, __BD _incY, __BD 0);
	}

	return (confirmDirection (result));
}

// ---
void SpearmanBehaviour::whatToDoWhenSabremanHitsIt ()
{
	_hit = true;
}

// ---
void SpearmanBehaviour::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	#ifndef NDEBUG
	if (!_path.empty () && 
		_path.find (_lastMazePlaceVisited) != _path.end ())
	{
		std::vector <QGAMES::Position> pos = _path [_lastMazePlaceVisited]._positions;
		for (int i = 0; i < (int) pos.size (); i++)
		{
			s -> drawCircle (pos [i], QGAMES::Vector::_zNormal, __BD 10, __BD 10, __REDCOLOR__);
			if (i != 0)
				s -> drawLine (pos [i - 1], pos [i], __REDCOLOR__, 3);
		}
	}
	#endif
}

// ---
QGAMES::Vector SpearmanBehaviour::randomMovement ()
{
	// Spearman always moves!
	// First off all detects all possibilities to move...
	std::vector <int> possibilities;
	for (int i = 0; i < 8; i++)
	{
		if (_monster -> canMove (_movDirs [i], QGAMES::Vector::_cero))
			possibilities.push_back (i); 
			// This possibility is possible (see a couple of steps are taking into account)
	}

	// If there is no possible to move spearman
	// (I guess it couldn't happen, but just in case)
	// no movement is possible...
	// If the movement is possible detects one random among the total of possibilities
	return (possibilities.empty () 
		? QGAMES::Vector::_cero : _movDirs [possibilities [rand () % possibilities.size ()]]);
}

// ---
QGAMES::Vector SpearmanBehaviour::randomDirection (bool pS, bool rA)
{
	// Where is spearman? (int the current maze place)
	// The size of the monster is taken into account!
	QGAMES::Position o  = _monster -> position ()
		+ QGAMES::Vector (__BD (_monster -> currentForm () -> frameWidth () >> 1),
						  __BD (_monster -> currentForm () -> frameHeight () >> 1), __BD 0);

	// Is the first time passing over this point...then calculate a path!
	if (_firstTimeInState)
	{
		_firstTimeInState = false;
		_lastMazePlaceVisited = _monster -> mazePlaceNumber ();
		_positionToVisit = 0;
		if (rA && !_oldpath.empty () && _lastPositionVisited >= 1)
		{
			_path = ((SabreWulfScene*) _monster -> map () -> scene ()) ->
				reversePathFrom (_lastMazePlaceVisited, o, 3, 
					_oldpath [_lastMazePlaceVisited]._positions, _lastPositionVisited);  
		}
		else
		{
			_path = ((SabreWulfScene*) _monster -> map () -> scene ()) -> 
				randomPathFrom (_lastMazePlaceVisited, o, 3, -1,
					pS ? _sabreman -> mazePlaceNumber () : -1); // During 3 places maximum...
		}
	}

	// The path shouldn't be empty or the maze place spearman is in not to exist...
	if (_path.empty () || _path.find (_lastMazePlaceVisited) == _path.end ())
		return (QGAMES::Vector::_cero); // It should happen,but just in case...

	// If the monster is not already in the place it should be in
	// Then the cursor for positions starts again...
	if (_monster -> mazePlaceNumber () != _lastMazePlaceVisited)
	{ 
		_lastMazePlaceVisited = _monster -> mazePlaceNumber (); 
		_positionToVisit = 0; 
	}

	// If the position to visit doesn't exist, then the vector returned is cero
	// at this point it should happen, but any case it could happen!
	if (_path [_lastMazePlaceVisited]._positions.size () == 0)
		return (QGAMES::Vector::_cero); // It should happen,but just in case...

	// If spearman has already reched the last position defined
	// Then the cursor moves to the next position
	if (((int) o.posX () == (int) _path [_lastMazePlaceVisited]._positions [_positionToVisit].posX ()) &&
		((int) o.posY () == (int) _path [_lastMazePlaceVisited]._positions [_positionToVisit].posY ()))
	{
		_positionToVisit++;
		// If there is no more places to visti, then a new path is calculated!
		if (_positionToVisit >= (int) _path [_lastMazePlaceVisited]._positions.size ())
		{
			_positionToVisit = 0;
			_path = ((SabreWulfScene*) _monster -> map () -> scene ()) -> 
				randomPathFrom (_lastMazePlaceVisited, o, 3, -1,
					pS ? _sabreman -> mazePlaceNumber () : -1); // During 3 places more...
		}
	}

	// ...and finally, which is the next position to reach?
	QGAMES::Position pT = 
		_path [_lastMazePlaceVisited]._positions [_positionToVisit];
	// ...and which should be the direction of the movement?
	QGAMES::Vector dir = pT - o;

	// Calculates the variables to move spearman
	// The counters per axis...
	_countAx = _countAy = 0;
	// Which is going to be the movement is the x axis?
	_incX = dir.posX () > 0 ? 1 : (dir.posX () < 0) ? -1 : 0;
	// ...and in the y axis?
	_incY = dir.posY () > 0 ? 1 : (dir.posY () < 0) ? -1 : 0;
	// ...in order to count movements per axis later...
	// ...movement is bigger in x axis than in y axis?
	_biggerX = (abs (dir.posX ()) >= abs (dir.posY ()));
	QGAMES::Vector result = QGAMES::Vector::_cero;

	// Does it move in perfect diagonal (45º)?
	if (abs (dir.posX ()) == abs (dir.posY ()))
	{
		// ...does it move?
		if (dir.posX () != 0 && dir.posY () != 0)
		{
			_ax = _ay = 1; // ...as increment in x and y axis are the same, 1 pixel per axis!
			result = QGAMES::Vector (__BD _incX, __BD _incY, __BD 0); // fixed the initial movement...
		}
	}
	else
	// does it move in a diagonal with less than 45º?
	if (abs (dir.posX ()) > abs (dir.posY ()))
	{
		result = QGAMES::Vector (__BD _incX, __BD 0, __BD 0);
		if (dir.posY () == 0)
		{
			_ay = 0; _ax = 1;
		}
		else
		{
			_ay = 1; _ax = (int) (abs (dir.posX () / dir.posY ()));
		}
	}
	else
	{
		result = QGAMES::Vector (__BD 0, __BD _incY, __BD 0);
		if (dir.posX () == 0)
		{
			_ax = 0; _ay = 1;
		}
		else
		{
			_ax = 1; _ay = int (abs (dir.posY () / dir.posX ()));
		}
	}

	return (confirmDirection (result));
}

// ---
bool SpearmanBehaviour::isSabremanNear ()
{
	// Sabreman is near always the distance in maze place rooms are less than 2
	int mL = _monster -> mazePlaceNumber () / __NUMBEROFMAZEPLACESX__;
	int mC = _monster -> mazePlaceNumber () % __NUMBEROFMAZEPLACESX__;
	int sL = _sabreman -> mazePlaceNumber () / __NUMBEROFMAZEPLACESX__;
	int sC = _sabreman -> mazePlaceNumber () % __NUMBEROFMAZEPLACESX__;
	double dt = sqrt (((sL - mL) * (sL - mL)) + ((sC - mC) * (sC - mC))); 
	return (dt <= 2.0);
}

// ---
QGAMES::Vector SpearmanBehaviour::confirmDirection (const QGAMES::Vector& o,
		const QGAMES::Position& nP)
{
	QGAMES::Vector result = o;

	// ...but is it valid?, and if it not, is there any other valid?
	// Detects whether the new position has to be found clockwise or opposite!
	int valid = -1; int j = 0; int jX = -1;
	for (int i = 0; i < 8 && valid == -1; i++) 
		if (result == _movDirs [i]) valid = i;
	if (valid > 0 && valid < 5) jX = 1;
	if (_lastDirection != QGAMES::Vector::_cero &&
		_lastDirection != result)
		jX = (int) result.crossProduct (_lastDirection).normalize ().posZ ();

	// Look for the new valid direction!
	while (!_monster -> canMove (result, QGAMES::Vector::_cero) && j != 8)
	{
		j++;
		valid += jX;
		if (valid == 8) valid = 0;
		else if (valid == -1) valid = 7;
		result = _movDirs [valid]; // Try with other...
	}

	_lastDirection = (j != 8) ? result : QGAMES::Vector::_cero;
	return (result);
}

// ---
SpearmanBehaviour::State SpearmanBehaviour::decideState ()
{
	SpearmanBehaviour::State result = _state; // By default it will be the same...
	
	// If sabreman hits spearman, the behaviour is to runaway from him
	// But take into account that the variable counting 
	// the time separman is running away is not restarted
	// when sabreman hits several times spearman!
	if (_hit)
	{
		_hit = false;
		result = SpearmanBehaviour::State::_RUNNINGAWAY;
	}
	// If he doesn't, the state will depend on the previous state...
	// ...and if sometime has passed....
	else
	{
		// If spearman was running away from sabreman
		// the situation is maintained for a period of time
		// This method is called when spear moves a couple of steps
		switch (_state)
		{
			case SpearmanBehaviour::State::_RUNNINGAWAY:
				if (_counterRunningAway++ >= 5 * // 5 seconds pure running away...
					(_monster -> game () -> framesPerSecond () / numberStepsBeforeAsking ()))
				{
					_counterRunningAway = 0;
					result = SpearmanBehaviour::State::_RANDOM;
				}
			
				break;

			case SpearmanBehaviour::State::_RANDOM:
				if (isSabremanNear ())
					result = SpearmanBehaviour::State::_PURSUIT;
				break;
			
			case SpearmanBehaviour::State::_PURSUIT:
				if (!isSabremanNear ())
					result = SpearmanBehaviour::State::_RANDOM;
				break;
		}
	}

	return (result);
}

