#include <PacManLike/pcinputhandler.hpp>
#include <PacManLike/pcgame.hpp>

// ---
PACMAN::InputHandlerBehaviour::InputHandlerBehaviour ()
	: QGAMES::InputHandler::StandardBehavior (),
	  _notificationData (new NotificationData),
	  _force (0), _downMovementOn (false), _upMovementOn (false), _rightMovementOn (false), _leftMovementOn (false)
{
	// It is needed to unsure that there are a set ok valid keys selected from the early beginning
	// They will be updated later, but they are needed from the moment that the execution main loop starts
	// to avoid anormal situations reading the keyboard...
	setKeys (std::vector <int> (7, QGAMES::KeyCode::QGAMES_F12));
	// F12 is not used for anything in the basic game...
}

// ---
void PACMAN::InputHandlerBehaviour::onKeyboardStatus (const std::vector <bool>& kS)
{
	assert (_keys.size () == 7);

	PACMAN::Game::Conf* pcCfg = dynamic_cast <PACMAN::Game::Conf*> (game () -> configuration ());
	assert (pcCfg != nullptr); // Just in case...

	// If using joystick has been configured, here makes no sense to continue...
	if (pcCfg -> useJoystick ())
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
			evntId = __PACMAN_MOVINGUPEVENT__;
			break;
		case 0x02:
			evntId = __PACMAN_MOVINGRIGHTEVENT__;
			break;
		case 0x03:
			evntId = __PACMAN_MOVINGUPRIGHTEVENT__;
			break;
		case 0x04:
			evntId = __PACMAN_MOVINGDOWNEVENT__;
			break;
		case 0x06:
			evntId = __PACMAN_MOVINGDOWNRIGHTEVENT__;
			break;
		case 0x08:
			evntId = __PACMAN_MOVINGLEFTEVENT__;
			break;
		case 0x09:
			evntId = __PACMAN_MOVINGUPLEFTEVENT__;
			break;
		case 0x0C:
			evntId = __PACMAN_MOVINGDOWNLEFTEVENT__;
			break;
		default:
			// Take into account that an strange combinations of keys, could stop the movement...
			evntId = __PACMAN_NOMOVINGEVENT__;
			break;
	}

	inputHandler () -> notify (QGAMES::Event (evntId, nullptr /** not needed. */));

	// Fire could be used to create special effects in some levels...
	if (kS [_keys [__QGAMES_KEYFIRE__]])
		_force++;
	else
	{
		if (_force != 0)
		{
			_notificationData -> _force = _force;
			_notificationData -> _downOn = kS [_keys [__QGAMES_KEYDOWN__]];
			_notificationData -> _upOn = kS [_keys [__QGAMES_KEYUP__]]; 
			_notificationData -> _leftOn = kS [_keys [__QGAMES_KEYLEFT__]];
			_notificationData -> _rightOn = kS [_keys [__QGAMES_KEYRIGHT__]];
			inputHandler () -> notify (QGAMES::Event (__PACMAN_FIREEVENT__, _notificationData));

			_notificationData -> clear ();
			_force = 0; // To start back...
		}
	}
}

// ---
void PACMAN::InputHandlerBehaviour::manageJoystickMovementOnState (QGAMES::GameState* st, int nJ, const QGAMES::Vector& dr)
{
	PACMAN::Game::Conf* pcCfg = dynamic_cast <PACMAN::Game::Conf*> (game () -> configuration ());
	assert (pcCfg != nullptr); // Just in case...

	// If no use of the joystick has been configured, here makes no sense to continue...
	if (!pcCfg -> useJoystick ())
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
			evntId = __PACMAN_MOVINGUPEVENT__;
			break;
		case 0x02:
			evntId = __PACMAN_MOVINGRIGHTEVENT__;
			break;
		case 0x03:
			evntId = __PACMAN_MOVINGUPRIGHTEVENT__;
			break;
		case 0x04:
			evntId = __PACMAN_MOVINGDOWNEVENT__;
			break;
		case 0x06:
			evntId = __PACMAN_MOVINGDOWNRIGHTEVENT__;
			break;
		case 0x08:
			evntId = __PACMAN_MOVINGLEFTEVENT__;
			break;
		case 0x09:
			evntId = __PACMAN_MOVINGUPLEFTEVENT__;
			break;
		case 0x0C:
			evntId = __PACMAN_MOVINGDOWNLEFTEVENT__;
			break;
		default:
			// Take into account that an strange combinations of keys, will stop the movement...
			evntId = __PACMAN_NOMOVINGEVENT__;
			break;
	}

	_downMovementOn = dMov & 0x04;
	_upMovementOn = dMov & 0x01;
	_leftMovementOn = dMov & 0x08;
	_rightMovementOn = dMov & 0x02;

	inputHandler () -> notify (QGAMES::Event (evntId, nullptr /** not needed. */));
}

// ---
void PACMAN::InputHandlerBehaviour::manageJoystickButtonOnState 
	(QGAMES::GameState* st, int nJ, bool p, int nB, int bF)
{
	PACMAN::Game::Conf* dGCfg = dynamic_cast <PACMAN::Game::Conf*> (game () -> configuration ());
	assert (dGCfg != nullptr); // Just in case...

	// If no use of the joystick has been configured, here makes no sense to continue...
	if (!dGCfg -> useJoystick ())
		return;

	// If the joystick is not the first one, then nothing to do...
	if (nJ != 0)
		return;

	// The first button in the first joystick...to fire!!
	if (nB == 0) 
	{
		if (!p && bF != 0)
		{
			_notificationData -> _force = bF; 
			_notificationData -> _downOn = _downMovementOn; 
			_notificationData -> _upOn = _upMovementOn;
			_notificationData -> _leftOn = _leftMovementOn;
			_notificationData -> _rightOn = _rightMovementOn;
			inputHandler () -> notify (QGAMES::Event (__PACMAN_FIREEVENT__, _notificationData));

			_notificationData -> clear (); // No longer used...
		}
	}
}
