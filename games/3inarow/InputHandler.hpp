/** \addtogroup 3InARow */
/*@{*/

#ifndef __INPUTHANDLER_HPP__
#define __INPUTHANDLER_HPP__

#include <Board/boardinclude.hpp>

/** Just the input hadler. */
class InputHandler : public QGAMES::InputHandler::Behaviour
{
	public:
	InputHandler ()
		: QGAMES::InputHandler::Behaviour (),
		  _lastKey (-1)
							{ }

	virtual void onJoystickAxisMoveEvent (QGAMES::JoystickMovementEventData* dt) { }
	virtual void onJoystickButtonEvent (QGAMES::JoystickButtonEventData* dt) { }
	virtual void onKeyboardEvent (QGAMES::KeyBoardEventData* dt);
	virtual void onMouseMovementEvent (QGAMES::MouseMovementEventData* dt) { }
	virtual void onMouseButtonEvent (QGAMES::MouseButtonEventData* dt);

	private:
	void manageKeyOnIntroState (int k);
	void manageKeyOnSelectSquareState (int k);
	void manageKeyOnPlayingState (int k);
	void manageKeyOnWinState (int k);

	private:
	int _lastKey;
};

#endif
  
// End of the file
/*@}*/