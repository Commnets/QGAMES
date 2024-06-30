#include "InputHandler.hpp"
#include "Game.hpp"

// ---
DOOMCASTLE::InputHandlerBehaviour::InputHandlerBehaviour ()
	: QGAMES::InputHandler::StandardBehavior (),
	  _notificationData (new NotificationData),
	  _impulse (0),
	  _force (0),
	  _downMovementOn (false), _upMovementOn (false), _rightMovementOn (false), _leftMovementOn (false)
{
	// It is needed to unsure that there are a set ok valid keys selected from the early beginning
	// They will be updated later, but they are needed from the moment that the execution main loop starts
	// to avoid anormal situations reading the keyboard...
	setKeys (std::vector <int> (__DOOMCASTLE_KEYITERATE__ + 1, QGAMES::KeyCode::QGAMES_F12));
	// F12 is not used for anything in the basic game...
}

// ---
void DOOMCASTLE::InputHandlerBehaviour::onKeyboardStatus (const std::vector <bool>& kS)
{
	assert (_keys.size () == 10);

	DOOMCASTLE::Game::Conf* dGCfg = dynamic_cast <DOOMCASTLE::Game::Conf*> (game () -> configuration ());
	assert (dGCfg); // Just in case...

	// If using joystick has been configured, here makes no sense to continue...
	if (dGCfg -> useJoystick ())
		return;

	int evntId = -1;

	// To control the movements...
	int dMov = 0x00;
	dMov |= kS [_keys [__QGAMES_KEYUP__]]		? 0x01 : 0x00;
	dMov |= kS [_keys [__QGAMES_KEYRIGHT__]]	? 0x02 : 0x00;
	dMov |= kS [_keys [__QGAMES_KEYDOWN__]]		? 0x04 : 0x00;
	dMov |= kS [_keys [__QGAMES_KEYLEFT__]]		? 0x08 : 0x00;
	switch (dMov)
	{
		case 0x01:
			evntId = __DOOMCASTLE_MOVINGUPEVENT__;
			break;
		case 0x02:
			evntId = __DOOMCASTLE_MOVINGRIGHTEVENT__;
			break;
		case 0x03:
			evntId = __DOOMCASTLE_MOVINGUPRIGHTEVENT__;
			break;
		case 0x04:
			evntId = __DOOMCASTLE_MOVINGDOWNEVENT__;
			break;
		case 0x06:
			evntId = __DOOMCASTLE_MOVINGDOWNRIGHTEVENT__;
			break;
		case 0x08:
			evntId = __DOOMCASTLE_MOVINGLEFTEVENT__;
			break;
		case 0x09:
			evntId = __DOOMCASTLE_MOVINGUPLEFTEVENT__;
			break;
		case 0x0C:
			evntId = __DOOMCASTLE_MOVINGDOWNLEFTEVENT__;
			break;
		default:
			// Take into account that an strange combinations of keys, will stop the movement...
			evntId = __DOOMCASTLE_NOMOVINGEVENT__;
			break;
	}

	inputHandler () -> notify (QGAMES::Event (evntId, NULL));

	// To fire is just about keeping pressed the right key
	// Shooting can be something continiously but with intensity...
	if (kS [_keys [__QGAMES_KEYFIRE__]])
		_force++;
	else
	{
		if (_force != 0)
		{
			_notificationData -> _impulse = _force;
			_notificationData -> _downOn = kS [_keys [__QGAMES_KEYDOWN__]];
			_notificationData -> _upOn = kS [_keys [__QGAMES_KEYUP__]]; 
			_notificationData -> _leftOn = kS [_keys [__QGAMES_KEYLEFT__]];
			_notificationData -> _rightOn = kS [_keys [__QGAMES_KEYRIGHT__]];
			inputHandler () -> notify (QGAMES::Event (__DOOMCASTLE_FIREEVENT__, _notificationData));

			_notificationData -> clear ();
			_force = 0; // To start back...
		}
	}

	// To jump is just about keeping the right key pressed to accumulate "force"
	// and the notify that when the key is released...
	if (kS [_keys [__DOOMCASTLE_KEYJUMP__]])
		_impulse++;
	else
	{
		if (_impulse != 0)
		{
			_notificationData -> _impulse = _impulse;
			_notificationData -> _downOn = kS [_keys [__QGAMES_KEYDOWN__]];
			_notificationData -> _upOn = kS [_keys [__QGAMES_KEYUP__]]; 
			_notificationData -> _leftOn = kS [_keys [__QGAMES_KEYLEFT__]];
			_notificationData -> _rightOn = kS [_keys [__QGAMES_KEYRIGHT__]];
			inputHandler () -> notify (QGAMES::Event (__DOOMCASTLE_JUMPEVENT__, _notificationData));

			_notificationData -> clear (); // No longer used...
			_impulse = 0; // To start back...
		}
	}
}

// ---
void DOOMCASTLE::InputHandlerBehaviour::manageKeyOnState (QGAMES::GameState* st, int kP)
{
	// The state is not used
	// The observer will decide waht to do with the event...

	if (kP == _keys [__DOOMCASTLE_KEYCATCHLEAVE__])
		inputHandler () -> notify (QGAMES::Event (__DOOMCASTLE_CATCHLEAVESOMETHINGEVENT__, NULL));
	if (kP == _keys [__DOOMCASTLE_KEYITERATE__])
		inputHandler () -> notify (QGAMES::Event (__DOOMCASTLE_ITERATEELEMENTSEVENT__, NULL));
}

// ---
void DOOMCASTLE::InputHandlerBehaviour::manageJoystickMovementOnState 
	(QGAMES::GameState* st, int nJ, const QGAMES::Vector& dr)
{
	DOOMCASTLE::Game::Conf* dGCfg = dynamic_cast <DOOMCASTLE::Game::Conf*> (game () -> configuration ());
	assert (dGCfg); // Just in case...

	// If no use of the joystick has been configured, here makes no sense to continue...
	if (!dGCfg -> useJoystick ())
		return;

	int evntId = -1;

	// To control the movements...
	int dMov = 0x00;
	dMov |= (dr.posY () < 0) ? 0x01 : 0x00;
	dMov |= (dr.posX () > 0) ? 0x02 : 0x00;
	dMov |= (dr.posY () > 0) ? 0x04 : 0x00;
	dMov |= (dr.posX () < 0) ? 0x08 : 0x00;
	switch (dMov)
	{
		case 0x01:
			evntId = __DOOMCASTLE_MOVINGUPEVENT__;
			break;
		case 0x02:
			evntId = __DOOMCASTLE_MOVINGRIGHTEVENT__;
			break;
		case 0x03:
			evntId = __DOOMCASTLE_MOVINGUPRIGHTEVENT__;
			break;
		case 0x04:
			evntId = __DOOMCASTLE_MOVINGDOWNEVENT__;
			break;
		case 0x06:
			evntId = __DOOMCASTLE_MOVINGDOWNRIGHTEVENT__;
			break;
		case 0x08:
			evntId = __DOOMCASTLE_MOVINGLEFTEVENT__;
			break;
		case 0x09:
			evntId = __DOOMCASTLE_MOVINGUPLEFTEVENT__;
			break;
		case 0x0C:
			evntId = __DOOMCASTLE_MOVINGDOWNLEFTEVENT__;
			break;
		default:
			// Take into account that an strange combinations of keys, will stop the movement...
			evntId = __DOOMCASTLE_NOMOVINGEVENT__;
			break;
	}

	_downMovementOn = dMov & 0x04;
	_upMovementOn = dMov & 0x01;
	_leftMovementOn = dMov & 0x08;
	_rightMovementOn = dMov & 0x02;

	inputHandler () -> notify (QGAMES::Event (evntId, NULL));
}

// ---
void DOOMCASTLE::InputHandlerBehaviour::manageJoystickButtonOnState 
	(QGAMES::GameState* st, int nJ, bool p, int nB, int bF)
{
	DOOMCASTLE::Game::Conf* dGCfg = dynamic_cast <DOOMCASTLE::Game::Conf*> (game () -> configuration ());
	assert (dGCfg); // Just in case...

	// If no use of the joystick has been configured, here makes no sense to continue...
	if (!dGCfg -> useJoystick ())
		return;

	// If the joystick is not the first then nothing is taking into account...
	if (nJ != 0)
		return;

	// The first button in the first joystick...to fire!!
	if (nB == 0) 
	{
		if (!p && bF != 0)
		{
			_notificationData -> _impulse = bF; // The force comes from the input handler...
			_notificationData -> _downOn = _downMovementOn; 
			_notificationData -> _upOn = _upMovementOn;
			_notificationData -> _leftOn = _leftMovementOn;
			_notificationData -> _rightOn = _rightMovementOn;
			inputHandler () -> notify (QGAMES::Event (__DOOMCASTLE_FIREEVENT__, _notificationData));

			_notificationData -> clear (); // No longer used...
		}
	}
	else 
	// The second button is used to jump...the more it is pressed the taller the jump will be...
	// In the keyboard, the button used to jump can also be used to slide, if the down key is pressed at the same time
	// With the joystick is diffficult to move it down whilst run, so a slide can be executed instead of a wished jump!
	// To avoid that, this button is just for jumpin (to up), and another button is stablished to slide
	if (nB == 1) 
	{
		if (!p && bF != 0)
		{
			_notificationData -> _impulse = bF;
			_notificationData -> _downOn = false; // To force the jump... 
			_notificationData -> _upOn = _upMovementOn;
			_notificationData -> _leftOn = _leftMovementOn;
			_notificationData -> _rightOn = _rightMovementOn;
			inputHandler () -> notify (QGAMES::Event (__DOOMCASTLE_JUMPEVENT__, _notificationData));

			_notificationData -> clear (); // No longer used...
		}
	}
	// This button is just to slide (see previous paragraph)
	if (nB == 2)
	{
		if (!p && bF != 0)
		{
			_notificationData -> _impulse = bF;
			_notificationData -> _downOn = true; // TO force slidding... 
			_notificationData -> _upOn = _upMovementOn;
			_notificationData -> _leftOn = _leftMovementOn;
			_notificationData -> _rightOn = _rightMovementOn;
			inputHandler () -> notify (QGAMES::Event (__DOOMCASTLE_JUMPEVENT__, _notificationData));

			_notificationData -> clear (); // No longer used...
		}
	}
}
