#include <Common/inputhandler.hpp>
#include <Common/game.hpp>
#include <iostream>

// ---
void QGAMES::InputHandler::Behaviour::setGame (Game* g)
{ 
	assert (g); // It can be null...
	_game = g; 
}

// ---
QGAMES::Vector QGAMES::InputHandler::StandardBehavior::joystickOrientation (int nJ) const
{
	std::map <int, int>::const_iterator i;
	if ((i = _mX.find (nJ)) == _mX.end ()) 
		return (QGAMES::Vector::_cero); // If there is no position kept, then resturs cero...
	return (QGAMES::Vector (__BD (*i).second, __BD (*_mY.find (nJ)).second, __BD (0)));
}

// ---
void QGAMES::InputHandler::StandardBehavior::beforeTreatingEvents ()
{
	// Treat many things related with the use of joysticks!

	// Valid to actualize the "impulse" generated while keeping pressed the buttons of the joystick...
	for (std::map <int, std::map <int, bool>>::const_iterator i = _jbStatus.begin (); 
			i != _jbStatus.end (); i++)
		for (std::map <int, bool>::const_iterator j = (*i).second.begin (); j != (*i).second.end (); j++)
			if ((*j).second) // the button is on...
				_jbForce [(*i).first][(*j).first]++; // ...then the force is incremented...

	std::vector <int> wD;
	QGAMES::Vector dr = QGAMES::Vector::_cero; // Not to move by default...
	for (std::map <int, int>::const_iterator i = _mX.begin (); i != _mX.end (); i++)
	{
		int nJ = (*i).first;
		dr = QGAMES::Vector (__BD (*i).second, __BD _mY [nJ], __BD 0);
		if (dr == QGAMES::Vector::_cero)
			wD.push_back (nJ);
		manageJoystickMovementOnState (game () -> activeState (), nJ, dr);
	}

	for (int i = 0; i < (int) wD.size (); i++)
		{ _mX.erase (_mX.find (wD [i])); _mY.erase (_mY.find (wD [i])); }
}

// ---
void QGAMES::InputHandler::StandardBehavior::onJoystickAxisMoveEvent (QGAMES::JoystickMovementEventData* dt)
{
	assert (dt);

	// Creates the right entry...if it doesn't exist before!
	std::map <int, int>::const_iterator i;
	if ((i = _mX.find (dt -> _numberJoystick)) == _mX.end ())
		_mX [dt -> _numberJoystick] = _mY [dt -> _numberJoystick] = 0; // Null...

	// Notice that the movemnt is always incremental respect the previous one...
	// The joysticks points a position in the space. When move to right (i.e) 
	// the position starts to move towards max position in the X. 
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
			if (dt -> _deadLine > (game () -> inputHandler () -> maxPosJoystick (dt -> _numberAxis) / _joystickSensibility))
				_mX [dt -> _numberJoystick] += (_mX [dt -> _numberJoystick] < 1) ? 1 : 0;
			else
				_mX [dt -> _numberJoystick] -= (_mX [dt -> _numberJoystick] > 0) ? 1 : 0;
		}
		else
		if (dt -> _deadLine < 0)
		{
			if (dt -> _deadLine < (game () -> inputHandler () -> minPosJoystick (dt -> _numberAxis) / _joystickSensibility))
				_mX [dt -> _numberJoystick] += (_mX [dt -> _numberJoystick] > -1) ? -1 : 0;
			else
				_mX [dt -> _numberJoystick] -= (_mX [dt -> _numberJoystick] < 0) ? -1 : 0;
		}
		else
			_mX [dt -> _numberJoystick] = 0; // Stops...
	}
	else
	// The y axis is the number 1
	if (dt -> _numberAxis == 1) 
	{
		if (dt -> _deadLine > 0)
		{
			if (dt -> _deadLine > (game () -> inputHandler () -> maxPosJoystick (dt -> _numberAxis) / _joystickSensibility))
				_mY [dt -> _numberJoystick] += (_mY [dt -> _numberJoystick] < 1) ? 1 : 0;
			else
				_mY [dt -> _numberJoystick] -= (_mY [dt -> _numberJoystick] > 0) ? 1 : 0;
		}
		else
		if (dt -> _deadLine < 0)
		{
			if (dt -> _deadLine < (game () -> inputHandler () -> minPosJoystick (dt -> _numberAxis) / _joystickSensibility))
				_mY [dt -> _numberJoystick] += (_mY [dt -> _numberJoystick] > -1) ? -1 : 0;
			else
				_mY [dt -> _numberJoystick] -= (_mY [dt -> _numberJoystick] < 0) ? -1 : 0;
		}
		else
			_mY [dt -> _numberJoystick] = 0; // Stops...
	}

	// Just in case the movement is more than the power associated...
	if (_mX [dt -> _numberJoystick] > _joystickPower) _mX [dt -> _numberJoystick] = _joystickPower;
	if (_mX [dt -> _numberJoystick] < -_joystickPower) _mX [dt -> _numberJoystick] = -_joystickPower;
	if (_mY [dt -> _numberJoystick] > _joystickPower) _mY [dt -> _numberJoystick] = _joystickPower;
	if (_mY [dt -> _numberJoystick] < -_joystickPower) _mY [dt -> _numberJoystick] = -_joystickPower;
}

// ---
void QGAMES::InputHandler::StandardBehavior::onJoystickButtonEvent (QGAMES::JoystickButtonEventData* dt)
{
	assert (dt);

	// When the button is first time pressed...
	if (dt -> _on)
	{
		manageJoystickButtonOnState (game () -> activeState (), 
			dt -> _numberJoystick, true, dt -> _numberButton, 0); // With no force
		_jbStatus [dt -> _numberJoystick][dt -> _numberButton] = true; // ...pressed back...
		_jbForce [dt -> _numberJoystick][dt -> _numberButton] = 0; // ...and strating from 0 with the force...
	}
	else
	// ...or finally releasd...
	// The force is actualized into the beforeTreatingEvents method...
	{
		manageJoystickButtonOnState (game () -> activeState (), 
			dt -> _numberJoystick, false, dt -> _numberButton, _jbForce [dt -> _numberJoystick][dt -> _numberButton]);
		_jbStatus [dt -> _numberJoystick][dt -> _numberButton] = false; // No longer pressed...
	}
}

// ---
void QGAMES::InputHandler::StandardBehavior::onKeyboardEvent (QGAMES::KeyBoardEventData* dt)
{
	assert (dt);

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
		manageKeyOnState (game () -> activeState (), kPressed);
}

// ---
QGAMES::InputHandler::InputHandler (QGAMES::Game* g, QGAMES::InputHandler::Behaviour* bhv)
		: _game (g),
		  _behaviour (bhv)
{ 
	assert (_game);
	assert (_behaviour);

	_behaviour -> setGame (g); // Sets the reference to the game in the behviour...
	_behaviour -> setInputHandler (this); // Sets a reference to the inputhandler owning this behaviour...
}

// ---
QGAMES::KeyCode QGAMES::InputHandler::translateKeyCode (int kC)
{
	// Very basic translation, if it is limits...
	// Usually this translation will be overloadad, because this one has no sense in a normal graphic frameowrk...
	return ((kC >= 0 && kC <= 0x92) ? (QGAMES::KeyCode) kC : QGAMES::KeyCode::QGAMES_KEYUNASSIGNED);
}

// ---
QGAMES::MouseButtonCode QGAMES::InputHandler::translateMouseButtonCode (int bC)
{
	// Very basic translation
	// Usually this translation will be overloadad, because this one has no sense in a normal graphic frameowrk...
	return ((bC >= 0 && bC <= 0x03) ? (QGAMES::MouseButtonCode) bC : QGAMES::MouseButtonCode::QGAMES_BUTTONUNASSIGNED);
}
