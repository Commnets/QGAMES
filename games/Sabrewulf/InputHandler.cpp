#include "InputHandler.hpp"
#include "GameStates.hpp"
#include "Game.hpp"
#include "Sabreman.hpp"
#include "Defs.hpp"

// ---
void InputHandler::onJoystickAxisMoveEvent (QGAMES::JoystickMovementEventData* dt)
{
	// Take into account that if the joystick is not moving
	// this method will not be called, so there must be an exit option programmed
	// to stop the main character when the joystick is not being moved!

	// Only one joystick available...
	if (dt -> _numberJoystick == 0) 
	{
		if (_game -> activeState () -> type () == __SABREWULFPLAYINGSTATE__)
		{
			// If the device to control the character is not joystick when playing, nothing to do here...
			// Keyboard and joystick can not be active at the same time when playing
			// The reason is, no action in any of them generates a no movement of the main character
			// This method is call after processing any event, including keyboard events
			// So, if the joystick is being used to move the character, and a movement is generated
			// when the system enters this method, and the keyboard is not in use, the movement finishes...
			if (!_joystick)
				return; 

			// Notice that the movemnt is always incremental respect the previous one...
			// The joysticks points a position in the space. When move to right (i.e) 
			// the position starts to move towrds max position in the X. 
			// When the joystick is released, the position moves back to 0
			// If the joystick is maintained at the right, the position doesn't change.
			// This behaviour is used to determinate the direction of the main character
			// When is is the right and bigger than a fouth of the max position the position is 
			// incremented in one, and reduce in one if it isn't.
			// The x axis is the number 0
			if (dt -> _numberAxis == 0) 
			{
				if (dt -> _deadLine > 0)
				{
					if (dt -> _deadLine > (game () -> inputHandler () -> maxPosJoystick (dt -> _numberAxis) / 4))
						_mX += (_mX < 1) ? 1 : 0;
					else
						_mX -= (_mX > 0) ? 1 : 0;
				}
				else
				if (dt -> _deadLine < 0)
				{
					if (dt -> _deadLine < (game () -> inputHandler () -> minPosJoystick (dt -> _numberAxis) / 4))
						_mX += (_mX > -1) ? -1 : 0;
					else
						_mX -= (_mX < 0) ? -1 : 0;
				}
			}
			else
			// The y axis is the number 1
			if (dt -> _numberAxis == 1) 
			{
				if (dt -> _deadLine > 0)
				{
					if (dt -> _deadLine > (game () -> inputHandler () -> maxPosJoystick (dt -> _numberAxis) / 4))
						_mY += (_mY < 1) ? 1 : 0;
					else
						_mY -= (_mY > 0) ? 1 : 0;
				}
				else
				if (dt -> _deadLine < 0)
				{
					if (dt -> _deadLine < (game () -> inputHandler () -> minPosJoystick (dt -> _numberAxis) / 4))
						_mY += (_mY > -1) ? -1 : 0;
					else
						_mY -= (_mY < 0) ? -1 : 0;
				}
			}

			// Moves the main character...
			((SabreWulfWorld*) (((SabreWulfGame*) _game) -> activeWorld ())) -> 
				moveCharacter (QGAMES::Vector (__BD _mX, __BD _mY, __BD 0));
		}
		else
			if (_game -> activeState () -> type () == __SABREWULFSELECTSTATE__)
		{
			if (dt -> _numberAxis == 1) // Moving in the y axis to select...
			{
				// The system will move focus to the next or previous position
				// Just if is not already changing! (see declaration of SabreWulfSelectionState class).
				SabreWulfSelectionState* st = (SabreWulfSelectionState*) _game -> activeState ();
				if (dt -> _deadLine > 0)
				{
					if (dt -> _deadLine > (game () -> inputHandler () -> maxPosJoystick (dt -> _numberAxis) / 4))
						_dJ += (_dJ < 1) ? 1 : 0;
					else
						_dJ -= (_dJ > 0) ? 1 : 0;
				}
				else
				if (dt -> _deadLine < 0)
				{
					if (dt -> _deadLine < (game () -> inputHandler () -> minPosJoystick (dt -> _numberAxis) / 4))
						_dJ += (_dJ > -1) ? -1 : 0;
					else
						_dJ -= (_dJ < 0) ? -1 : 0;
				}
			}

			// Move to the option depending on the axis movement...
			SabreWulfSelectionState* st = (SabreWulfSelectionState*) _game -> activeState ();
			if (_dJ == 1) st -> nextOption ();
			else if (_dJ == -1) st -> previousOption ();
		}
	}
}

// ---
void InputHandler::onJoystickButtonEvent (QGAMES::JoystickButtonEventData* dt)
{
	// Only one joystick can be active at the same time,
	// and the different actions make only sense when the button is releasd
	if (dt -> _numberJoystick == 0)
	{
		if (!dt -> _on)
		{
			if (_game -> activeState () -> type () == __SABREWULFPLAYINGSTATE__)
			{
				// If the device to control the character is not joystick 
				// when playing, nothing to do here...
				if (!_joystick)
					return; 

				// Normally the fire button...
				if (dt -> _numberButton == 0) 
					((SabreWulfGame*) _game) -> noFight ();
			}
			else
			if (_game -> activeState () -> type () == __SABREWULFSELECTSTATE__ && 
					dt -> _numberButton == 0)
				((SabreWulfSelectionState*) _game -> activeState ()) -> optionSelected ();
			else
			if (_game -> activeState () -> type () == __SABREWULFDEMOSTATE__ && dt -> _numberButton == 0)
				((SabreWulfDemoState*) _game -> activeState ()) -> setWantToExit (true);
			else
			if (_game -> activeState () -> type () == __SABREWULFENDSTATE__ && dt -> _numberButton == 0)
				((SabreWulfEndState*) _game -> activeState ()) -> setWantToExit (true);
			else
			if (_game -> activeState () -> type () == __SABREWULFWINSTATE__ && dt -> _numberButton == 0)
				((SabreWulfWinState*) _game -> activeState ()) -> setWantToExit (true);
		}
		else
		{
			if (_game -> activeState () -> type () == __SABREWULFPLAYINGSTATE__)
			{
				// If the device to control the character is not joystick 
				// when playing, nothing to do here...
				if (!_joystick)
					return; 

				// Normally the fire button...
				if (dt -> _numberButton == 0) 
					((SabreWulfGame*) _game) -> fight ();
			}
		}
	}
}

// ---
void InputHandler::onKeyboardEvent (QGAMES::KeyBoardEventData* dt)
{
	int kPressed = -1;
	bool pKey = false;
	if (_lastKey != -1)
	{
		if (!dt -> _on)
		{
			kPressed = _lastKey;
			_lastKey = -1;
			pKey = true;
		}
	}
	else
		_lastKey = dt -> _internalCode;

	if (pKey)
	{
		int gS = _game -> activeState () -> type ();
		switch (gS)
		{
			case __SABREWULFSELECTSTATE__:
				manakeKeyOnSelectionState (kPressed);
				break;

			case __SABREWULFDEMOSTATE__:
				manageKeyOnDemoState (kPressed);
				break;

			case __SABREWULFPLAYINGSTATE__:
				manakeKeyOnPlayingState (kPressed);
				break;

			case __SABREWULFENDSTATE__:
				manakeKeyOnEndState (kPressed);
				break;

			case __SABREWULFWINSTATE__:
				manakeKeyOnWinState (kPressed);
				break;

			default:
				break;
		};
	}
}

// ---
void InputHandler::onMouseMovementEvent (QGAMES::MouseMovementEventData* dt)
{
	int gS = _game -> activeState () -> type ();
	switch (gS)
	{
		case __SABREWULFSELECTSTATE__:
			((SabreWulfSelectionState*) _game -> activeState ()) -> 
				optionAt (QGAMES::Position (__BD dt -> _x, __BD dt -> _y, __BD 0));
			break;
	}
}

// ---
void InputHandler::onMouseButtonEvent (QGAMES::MouseButtonEventData* dt)
{
	int gS = _game -> activeState () -> type ();
	switch (gS)
	{
		case __SABREWULFDEMOSTATE__:
			if (dt -> _button == QGAMES::MouseButtonCode::QGAMES_BUTTONLEFT && !dt -> _on)
				((SabreWulfDemoState*) _game -> activeState ()) -> setWantToExit (true);
			break;

		case __SABREWULFENDSTATE__:
			if (dt -> _button == QGAMES::MouseButtonCode::QGAMES_BUTTONLEFT && !dt -> _on)
				((SabreWulfEndState*) _game -> activeState ()) -> setWantToExit (true);
			break;

		case __SABREWULFWINSTATE__:
			if (dt -> _button == QGAMES::MouseButtonCode::QGAMES_BUTTONLEFT && !dt -> _on)
				((SabreWulfWinState*) _game -> activeState ()) -> setWantToExit (true);
			break;

		case __SABREWULFSELECTSTATE__:
			if (dt -> _button == QGAMES::MouseButtonCode::QGAMES_BUTTONLEFT && !dt -> _on) // To select an option...
				((SabreWulfSelectionState*) _game -> activeState ()) -> optionSelected ();
			break;

		case __SABREWULFPLAYINGSTATE__:
			if (dt -> _button == QGAMES::MouseButtonCode::QGAMES_BUTTONLEFT && !dt -> _on) // To pause o continue the game...
				_game -> isGamePaused () ? _game -> continueGame () : _game -> pauseGame ();
			break;

		default:
			break;
	}
}

// ---
void InputHandler::onKeyboardStatus (const std::vector <bool>& kS)
{
	if (_game -> activeState () -> type () == __SABREWULFPLAYINGSTATE__)
	{
		// If the device to control the main character is the joystick, nothing to do here...
		// Keyboard and joystick can not be active at the same time when playing
		// The reason is, no action in any of them generates a no movement of the main character
		// This method is call after processing any event, including keyboard events
		// So, if the joystick is being used to move the character, and a movement is generated
		// when the system enters this method, and the keyboard is not in use, the movement finishes...
		if (_joystick)
			return; 

		int dX = 0; int dY = 0;
		if (kS [QGAMES::KeyCode::QGAMES_S]) dX = 1;
		if (kS [QGAMES::KeyCode::QGAMES_A]) dX = -1;
		if (kS [QGAMES::KeyCode::QGAMES_L]) dY = 1;
		if (kS [QGAMES::KeyCode::QGAMES_P]) dY = -1;
		((SabreWulfWorld*) (((SabreWulfGame*) _game) -> activeWorld ())) 
			-> moveCharacter (QGAMES::Vector (__BD dX, __BD dY, __BD 0));
		// If no key is pressed the character stops!
	}
}

// ---
void InputHandler::manageKeyOnDemoState (int k)
{
	if (k == QGAMES::KeyCode::QGAMES_RETURN)
		((SabreWulfDemoState*) _game -> activeState ()) -> setWantToExit (true);
}
// ---
void InputHandler::manakeKeyOnSelectionState (int k)
{
	if (k == QGAMES::KeyCode::QGAMES_RETURN)
		((SabreWulfSelectionState*) _game -> activeState ()) -> optionSelected ();
	else
	if (k == QGAMES::KeyCode::QGAMES_DOWN)
		((SabreWulfSelectionState*) _game -> activeState ()) -> nextOption ();
	else
	if (k == QGAMES::KeyCode::QGAMES_UP)
		((SabreWulfSelectionState*) _game -> activeState ()) -> previousOption ();
}

// ---
void InputHandler::manakeKeyOnPlayingState (int k)
{
	SabreWulfWorld* w = (SabreWulfWorld*) (((SabreWulfGame*) _game) -> activeWorld ());
	#ifndef NDEBUG
	if (k == QGAMES::KeyCode::QGAMES_LEFT || 
		k == QGAMES::KeyCode::QGAMES_RIGHT ||
		k == QGAMES::KeyCode::QGAMES_UP ||
		k == QGAMES::KeyCode::QGAMES_DOWN) // Just to move a maze place more or less...
	{
		int rN = w -> mazePlaceNumber () + 
			((k == QGAMES::KeyCode::QGAMES_LEFT) 
				? -1 : ((k == QGAMES::KeyCode::QGAMES_RIGHT) 
					? 1 : ((k == QGAMES::KeyCode::QGAMES_UP) 
						? -__NUMBEROFMAZEPLACESX__ : __NUMBEROFMAZEPLACESX__)));
		if (rN >= __NUMBEROFMAZEPLACES__) rN = 0;
		if (rN < 0) rN = __NUMBEROFMAZEPLACES__ - 1;
		w -> setMazePlaceNumber (rN);
	}
	else
	if (k == QGAMES::KeyCode::QGAMES_X) // Just to activate the lines defining the limits of the map...
		((SabreWulfGame*) _game) -> onOffDrawLimits ();
	else
	#endif
	if (k == QGAMES::KeyCode::QGAMES_Z) // To pause/continue the game...
	{
		_game -> isGamePaused () ? _game -> continueGame () : _game -> pauseGame ();
		if (_game -> isGamePaused ()) QGAMES::SoundSystem::system () -> pause ();
		else QGAMES::SoundSystem::system () -> resume (); // Stops all sounds whilst pause is set!
	}
	else
	if (k == QGAMES::KeyCode::QGAMES_SPACE) // This is a switch to activate or desactivate fighting...
	{
		if (((SabreWulfGame*) _game) -> isFighting ())
			((SabreWulfGame*) _game) -> noFight (); 
		else
			((SabreWulfGame*) _game) -> fight ();
	}
}

// ---
void InputHandler::manakeKeyOnEndState (int k)
{
	if (k == QGAMES::KeyCode::QGAMES_RETURN)
		((SabreWulfEndState*) _game -> activeState ()) -> setWantToExit (true);
}

// ---
void InputHandler::manakeKeyOnWinState (int k)
{
	if (k == QGAMES::KeyCode::QGAMES_RETURN)
		((SabreWulfWinState*) _game -> activeState ()) -> setWantToExit (true);
}
