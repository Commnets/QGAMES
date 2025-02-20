#include <Common/gameimplementation.hpp>
#include <Common/game.hpp>

// ---
void QGAMES::GameImplementation::handleEvents ()
{
	QGAMES::InputHandler* iH = _game -> inputHandler (); // If it doesn't exist, it will be created, and if not...crash!!
	iH -> beforeTreatingEvents ();
	QGAMES::Events evnts = iH -> readEvents ();
	for (QGAMES::Events::const_iterator i = evnts.begin (); 
			i != evnts.end (); i++)
		treatEvent ((*i)); // The event data is deleted in that method...
	iH -> treatKeyboardStatus (); // Just in case...
	iH -> treatJoystickStatus (); // Just in case...
	iH -> treatMouseStatus (); // Just in case...
	iH -> afterTreatingEvents ();
	// There is no any specific invocation to on KeyboardStatus method
	// It has to be specifically invoked depending on the graphical library!
}

// ---
void QGAMES::GameImplementation::treatEvent (const Event& evnt)
{
	switch (evnt.code ())
	{
		case __QGAMES_MOVEJOYSTICKEVENT__:
			{
				JoystickMovementEventData* dt = (JoystickMovementEventData*) evnt.data ();
				assert (dt); // Event is something can be generated by the user...
				_game -> inputHandler () -> onJoystickAxisMoveEvent (dt);
				delete (dt);
			}
			break;

		case __QGAMES_BUTTONJOYSTICKEVENT__:
			{
				JoystickButtonEventData* dt = (JoystickButtonEventData*) evnt.data ();
				assert (dt); // Event is something can be generated by the user...
				_game -> inputHandler () -> onJoystickButtonEvent (dt);
				delete (dt);
			}
			break;

		case __QGAMES_KEYBOARDEVENT__:
			{
				KeyBoardEventData* dt = (KeyBoardEventData*) evnt.data ();
				assert (dt); // Event is something can be generated by the user...
				_game -> inputHandler () -> onKeyboardEvent (dt);
				delete (dt);
			}
			break;

		case __QGAMES_MOUSEMOVEMENTEVENT__:
			{
				MouseMovementEventData* dt = (MouseMovementEventData*) evnt.data ();
				assert (dt); // Event is something can be generated by the user...
				_game -> inputHandler () ->	onMouseMovementEvent (dt);
				delete (dt);
			}
			break;

		case __QGAMES_MOUSEBUTTONEVENT__:
			{
				MouseButtonEventData* dt = (MouseButtonEventData*) evnt.data ();
				assert (dt); // Event is something can be generated by the user...
				_game -> inputHandler () -> onMouseButtonEvent (dt);
				delete (dt);
			}
			break;

		default:
			break;
	}
}
