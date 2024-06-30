#include <DragonwindLike/dwsteps.hpp>
#include <DragonwindLike/dwartists.hpp>
#include <DragonwindLike/dwgame.hpp>

// ---
DRAGONWIND::CharacterControlStepDragonAction::Properties::Properties 
		(const std::map <std::string, std::string>& prps)
	: _type (0), // Meaning to stand still!!
		_direction (0),
		_backwards (false),
		_impulse (1), // Negative means random up to that value
		_seconds (-1) // Negative means random up to that value
{
	_type = (prps.find (std::string (__DRAGONWIND_CHRSTEPACTIONATTRTYPE__)) != prps.end ())
		? std::atoi ((*prps.find (std::string (__DRAGONWIND_CHRSTEPACTIONATTRTYPE__))).second.c_str ())
		: __DRAGONWIND_CHRSTEPACTIONATTRDEFTYPE__;
	_direction = (prps.find (std::string (__DRAGONWIND_CHRSTEPACTIONATTRDIR__)) != prps.end ())
		? std::atoi ((*prps.find (std::string (__DRAGONWIND_CHRSTEPACTIONATTRDIR__))).second.c_str ())
		: __DRAGONWIND_CHRSTEPACTIONATTRDEFDIR__;
	_backwards = (prps.find (std::string (__DRAGONWIND_CHRSTEPACTIONATTRBACKWARDS__)) != prps.end ())
		? QGAMES::toUpper (std::string ((*prps.find (std::string (__DRAGONWIND_CHRSTEPACTIONATTRBACKWARDS__))).second.c_str ())) == std::string (__YES_STRING__)
		: __DRAGONWIND_CHRSTEPACTIONATTRDEFBACKW__;
	_impulse = (prps.find (std::string (__DRAGONWIND_CHRSTEPACTIONATTRIMPULSE__)) != prps.end ())
		? std::atoi ((*prps.find (std::string (__DRAGONWIND_CHRSTEPACTIONATTRIMPULSE__))).second.c_str ())
		: __DRAGONWIND_CHRSTEPACTIONATTRDEFIMPULSE__;
	_seconds = (prps.find (std::string (__DRAGONWIND_CHRSTEPACTIONATTRSECONDS__)) != prps.end ())
		? __BD (std::atof ((*prps.find (std::string (__DRAGONWIND_CHRSTEPACTIONATTRSECONDS__))).second.c_str ()))
		: __DRAGONWIND_CHRSTEPACTIONATTRDEFSECONDS__;
	_pixels = (prps.find (std::string (__DRAGONWIND_CHRSTEPACTIONATTRPIXELS__)) != prps.end ())
		? std::atoi ((*prps.find (std::string (__DRAGONWIND_CHRSTEPACTIONATTRPIXELS__))).second.c_str ())
		: __DRAGONWIND_CHRSTEPACTIONATTRDEFPIXELS__;
	std::string lStr = (prps.find (std::string (__DRAGONWIND_CHRSTEPACTIONATTRLIMITS__)) != prps.end ())
		? (*prps.find (std::string (__DRAGONWIND_CHRSTEPACTIONATTRLIMITS__))).second.c_str ()
		: std::string (__DRAGONWIND_CHRSTEPACTIONATTRDEFLIMITS__);
	std::vector <QGAMES::bdata> lVDt = QGAMES::getElementsFromAsBDATA (lStr, ',', 6);
	_limits = QGAMES::Rectangle (QGAMES::Position (lVDt [0], lVDt [1], lVDt [2]),
								 QGAMES::Position (lVDt [3], lVDt [4], lVDt [5]), QGAMES::Vector::_zNormal);

	Properties (_type, _direction, _backwards, _impulse, _seconds, _pixels, _limits);
}

// ---
QGAMES::SetOfOpenValues DRAGONWIND::CharacterControlStepDragonAction::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::CharacterControlStep::runtimeValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_counter));
	result.addOpenValue (lE + 2, QGAMES::OpenValue (_counterLimit));
	result.addOpenValue (lE + 3, QGAMES::OpenValue (_timeLimit));
	result.addOpenValue (lE + 4, QGAMES::OpenValue (_pixelsCounter));
	result.addOpenValue (lE + 5, QGAMES::OpenValue (_pixelsLimit));
	result.addOpenValue (lE + 6, QGAMES::OpenValue (_pixelsCounterLimit));
	std::stringstream sS; sS << _originalPosition;
	result.addOpenValue (lE + 7, QGAMES::OpenValue (sS.str ()));
	result.addOpenValue (lE + 8, QGAMES::OpenValue (_realBackwards));

	return (result);
}

// ---
void DRAGONWIND::CharacterControlStepDragonAction::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE)		&& cfg.existOpenValue (lE - 1) &&
			cfg.existOpenValue (lE - 2) && cfg.existOpenValue (lE - 3) &&
			cfg.existOpenValue (lE - 4) && cfg.existOpenValue (lE - 5) &&
			cfg.existOpenValue (lE - 6) && cfg.existOpenValue (lE - 7));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_realBackwards = cCfg.openValue (lE).boolValue ();
	cCfg.removeOpenValue (lE);
	_originalPosition = QGAMES::Position (cCfg.openValue (lE - 1).strValue ());
	cCfg.removeOpenValue (lE - 1);
	_pixelsCounterLimit = cCfg.openValue (lE - 2).intValue ();
	cCfg.removeOpenValue (lE - 2);
	_pixelsLimit = cCfg.openValue (lE - 3).intValue ();
	cCfg.removeOpenValue (lE - 3);
	_pixelsCounter = cCfg.openValue (lE - 4).intValue ();
	cCfg.removeOpenValue (lE - 4);
	_timeLimit = cCfg.openValue (lE - 5).boolValue ();
	cCfg.removeOpenValue (lE - 5);
	_counterLimit = cCfg.openValue (lE - 6).intValue ();
	cCfg.removeOpenValue (lE - 6);
	_counter = cCfg.openValue (lE - 7).intValue ();
	cCfg.removeOpenValue (lE - 7);

	QGAMES::CharacterControlStep::initializeRuntimeValuesFrom (cCfg);
}

// ---
void DRAGONWIND::CharacterControlStepDragonAction::initializeOn (QGAMES::Character* a)
{
	DRAGONWIND::Character* chr = dynamic_cast <DRAGONWIND::Character*> (a);
	assert (chr); // It has to be, just in case...

	_counter = 0;
	_counterLimit = 
		(_properties._seconds == __BD -1) 
			? __MAXINT__ 
			: (((_properties._seconds < __BD 0) 
				? rand () % ((int) -_properties._seconds + 1) : (int) _properties._seconds)) * QGAMES::Game::game () -> framesPerSecond ();
	_timeLimit = false;

	_pixelsCounter = 0;
	_pixelsCounterLimit = _properties._pixels;
	_pixelsLimit = false;

	_originalPosition = chr -> position ();
	_realBackwards = _properties._backwards;

	switch (_properties._type)
	{
		case __DRAGONWIND_CHRSTEPACTIONSTAND__: // To stand
			chr -> toStand (_properties._direction);
			break;

		case __DRAGONWIND_CHRSTEPACTIONFALL__: // To fall
			chr -> setLookingToDirection (_properties._direction);
			chr -> toFall ();
			break;

		case __DRAGONWIND_CHRSTEPACTIONJUMP__: // To jump
			chr -> setLookingToDirection (_properties._direction);
			chr -> toJump ((_properties._impulse < 0) 
				? ((rand () % (-_properties._impulse)) + 1) : _properties._impulse, false);
			break;

		case __DRAGONWIND_CHRSTEPACTIONMOVE__: // To move
			{
				chr -> toMove ((_properties._direction == 0) 
					? QGAMES::Vector (__BD 1, __BD 0, __BD 0) : QGAMES::Vector (__BD -1, __BD 0, __BD 0));
				if (_properties._backwards) // Change the direction
					chr -> setMove (-chr -> direction ());
			}

			break;

		case __DRAGONWIND_CHRSTEPACTIONCLIMB__: // To climb
			chr -> setLookingToDirection (_properties._direction);
			chr -> toClimb ((_properties._direction == 2) 
				? QGAMES::Vector (__BD 0, __BD -1, __BD 0) : QGAMES::Vector (__BD 0, __BD 1, __BD 0));
			break;

		case __DRAGONWIND_CHRSTEPACTIONSLIDE__: // To slide
			chr -> setLookingToDirection (_properties._direction);
			chr -> toJump (_properties._impulse, true); 
			break;

		case __DRAGONWIND_CHRSTEPACTIONMOVEFRBTOLIMIT__: // Going back or forward to a position almost random...
			{
				// If the original position is out of the limits and 
				/// the proposed movement is ever to make that gap bigger, the best thing to do is to walk backwards...
				if ((_originalPosition.posX () <= _properties._limits.pos1 ().posX () && (_properties._direction == 1)) ||
					(_originalPosition.posX () >= _properties._limits.pos2 ().posX () && (_properties._direction == 0)))
					_realBackwards = true;

				// Then start to move...
				QGAMES::Vector vDir = (_properties._direction == 0) 
					? QGAMES::Vector (__BD 1, __BD 0, __BD 0) : QGAMES::Vector (__BD -1, __BD 0, __BD 0);
				chr -> toMove (vDir);
				if (_realBackwards) // Change the direction
				{
					chr -> setMove (-chr -> direction ());
					vDir = -vDir; // opposite!
				}

				// But how long?...
				QGAMES::Position fPos = _originalPosition + (__BD _properties._pixels * vDir);
				if (fPos.posX () > _properties._limits.pos2 ().posX ())
					_pixelsCounterLimit = (int) (_properties._limits.pos2 ().posX () - _originalPosition.posX ());
				else
				if (fPos.posX () < _properties._limits.pos1 ().posX ())
					_pixelsCounterLimit = (int) (_originalPosition.posX () - _properties._limits.pos1 ().posX ());
			}

			break;

		default:
			assert (0); // It shouldn't be here...
	}
}

// ---
bool DRAGONWIND::CharacterControlStepDragonAction::isEndOn (QGAMES::Character* a)
{
	DRAGONWIND::Character* chr = dynamic_cast <DRAGONWIND::Character*> (a);
	assert (chr); // It has to be, just in case...

	bool result = false;

	switch (_properties._type)
	{
		case __DRAGONWIND_CHRSTEPACTIONSTAND__: // Standing and the time over?
			result = (chr -> isStanding () && _timeLimit) || !chr -> isStanding ();
			break;

		case __DRAGONWIND_CHRSTEPACTIONFALL__: // Still falling?
			result = !chr -> isFalling (); // While falling the state can not be change, so it doesn't depend on the time!
			break;

		case __DRAGONWIND_CHRSTEPACTIONJUMP__: // Still jumping?
			result = !chr -> isJumping (); // Same than falling
			break;

		case __DRAGONWIND_CHRSTEPACTIONMOVE__: // Moving and the time over?
			result = (chr -> isRunning () && _timeLimit) || !chr -> isRunning ();
			break;

		case __DRAGONWIND_CHRSTEPACTIONCLIMB__: // Climbing and the time over?
			result = (chr -> isClimbing () && _timeLimit) || !chr -> isClimbing ();
			break;

		case __DRAGONWIND_CHRSTEPACTIONSLIDE__: // Still slidding?
			result = !chr -> isSlidding (); // Same than jumping
			break;

		case __DRAGONWIND_CHRSTEPACTIONMOVEFRBTOLIMIT__: // Still moving
			result = (chr -> isRunning () && _timeLimit) ||
					 (chr -> isRunning () && _pixelsLimit) || !chr -> isRunning ();
			break;

		default:
			assert (0); // It shouldn't be here...
	}

	return (result);
}

// ---
void DRAGONWIND::CharacterControlStepDragonAction::updateOn (QGAMES::Character* a)
{
	// Just update counter and timeLimit...
	_timeLimit = (_counter++ > _counterLimit);

	// Same with the pixels...
	_pixelsLimit = ((_pixelsCounter = (int) 
		(std::abs (a -> position ().posX () - _originalPosition.posX ()))) >= _pixelsCounterLimit);
}
