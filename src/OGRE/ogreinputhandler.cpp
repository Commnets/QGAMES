#include <CommonOGRE/ogreinputhandler.hpp>
#include <CommonOGRE/ogrescreen.hpp>
#include <Common/game.hpp>

// ---
OGREInputHandler::Handler::Handler (OGREInputHandler* iH, Ogre::Root* r, Ogre::RenderWindow* w)
		: _inputHandler (iH),
		  _window (w), 
		  _lastFrameEvent (NULL),
		  _root (r)
{
	assert (_window);
	assert (_root);

	_root -> addFrameListener (this);
}

// ---
OGREInputHandler::Handler::~Handler()
{
	_root -> removeFrameListener (this);

	delete (_lastFrameEvent);
	_lastFrameEvent = NULL;
}


// ---
bool OGREInputHandler::Handler::frameRenderingQueued (const Ogre::FrameEvent& evt)
{
	if (_window -> isClosed ())
		return (false); // No more events when the window is closed...

	// Keep a trace of the last frame event...
	delete (_lastFrameEvent);
	_lastFrameEvent = new Ogre::FrameEvent (evt);

	return (true);
}

// ---
bool OGREInputHandler::Handler::frameEnded (const Ogre::FrameEvent& evt)
{
	SDL_Event evnt;

	_inputHandler -> beforeTreatingEvents ();
	while (SDL_PollEvent (&evnt)) 
		_inputHandler -> treatEvent (evnt);
	_inputHandler -> treatKeyboardStatus ();
	_inputHandler -> treatJoystickStatus ();
	_inputHandler -> treatMouseStatus ();
	_inputHandler -> afterTreatingEvents ();

	return (true); // To continue...
}

// ---
OGREInputHandler::OGREInputHandler (Ogre::Root* r, QGAMES::Game* g, QGAMES::InputHandler::Behaviour* bhv)
		: SDLInputHandler (g, bhv),
		  _root (r),
		  _handler (NULL)
{
	assert (r);

	// The input handler is always created over the main screen
	// So this method has always to be invoked after the screens are created
	// This is the normal behaviour in the the default game behaviour

	OGREScreen* scr = dynamic_cast <OGREScreen*> (game () -> mainScreen ());
	assert (scr); // Just in case...
	Ogre::RenderWindow* window = scr -> window ();
	_handler = new OGREInputHandler::Handler (this, r, window);
}

// ---
OGREInputHandler::~OGREInputHandler ()
{
	delete (_handler);
	_handler = NULL;
}

// ---
void OGREInputHandler::treatEvent (SDL_Event& evnt)
{
	switch (evnt.type)
	{
		case SDL_QUIT:
			game () -> exitGame ();
			break;

		case SDL_JOYAXISMOTION:
			{
				QGAMES::JoystickMovementEventData* dt = 
					new QGAMES::JoystickMovementEventData;
				dt -> _numberJoystick = evnt.jaxis.which;
				dt -> _numberAxis = evnt.jaxis.axis;
				dt -> _deadLine = evnt.jaxis.value;
				onJoystickAxisMoveEvent (dt);
				delete (dt);
			}

			break;

		case SDL_JOYBUTTONDOWN:
			{
				QGAMES::JoystickButtonEventData* dt =
					new QGAMES::JoystickButtonEventData;
				dt -> _numberJoystick = evnt.jaxis.which;
				dt -> _numberButton = evnt.jbutton.button;
				dt -> _on = true;
				onJoystickButtonEvent (dt);
				delete (dt);
			}

			break;
			
		case SDL_JOYBUTTONUP:
			{
				QGAMES::JoystickButtonEventData* dt =
					new QGAMES::JoystickButtonEventData;
				dt -> _numberJoystick = evnt.jaxis.which;
				dt -> _numberButton = evnt.jbutton.button;
				dt -> _on = false;
				onJoystickButtonEvent (dt);
				delete (dt);
			}

			break;

		case SDL_KEYDOWN:
			{
				QGAMES::KeyBoardEventData* dt = 
					new QGAMES::KeyBoardEventData;
				dt -> _key = evnt.key.keysym.scancode;
				dt -> _on = (evnt.key.state == SDL_PRESSED) ? true : false;
				dt -> _repeat = (evnt.key.repeat) ? true : false;
				dt -> _unicode = evnt.key.keysym.sym;
				dt -> _internalCode = game () -> inputHandler () -> translateKeyCode (dt -> _key); 
				onKeyboardEvent (dt);
				delete (dt);
			}

			break;

		case SDL_KEYUP:
			{
				QGAMES::KeyBoardEventData* dt = 
					new QGAMES::KeyBoardEventData;
				dt -> _key = evnt.key.keysym.scancode;
				dt -> _repeat = false;
				dt -> _on = false;
				dt -> _unicode = evnt.key.keysym.sym;
				dt -> _internalCode = game () -> inputHandler () -> translateKeyCode (dt -> _key); 
				onKeyboardEvent (dt);
				delete (dt);
			}

			break;

		case SDL_MOUSEMOTION:
			{
				QGAMES::MouseMovementEventData* dt = 
					new QGAMES::MouseMovementEventData;
				dt -> _x = evnt.motion.x;
				dt -> _y = evnt.motion.y;
				onMouseMovementEvent (dt);
				delete (dt);
			}

			break;

		case SDL_MOUSEBUTTONDOWN:
			{
				QGAMES::MouseButtonEventData* dt = 
					new QGAMES::MouseButtonEventData;
				dt -> _button = evnt.button.button;
				dt -> _on = true;
				dt -> _internalCode = game () -> inputHandler () -> translateMouseButtonCode (dt -> _button);
				onMouseButtonEvent (dt);
				delete (dt);
			}

			break;

		case SDL_MOUSEBUTTONUP:
			{
				QGAMES::MouseButtonEventData* dt = 
					new QGAMES::MouseButtonEventData;
				dt -> _button = evnt.button.button;
				dt -> _on = false;
				dt -> _internalCode = game () -> inputHandler () -> translateMouseButtonCode (dt -> _button);
				onMouseButtonEvent (dt);
				delete (dt);
			}

			break;

		default:
			break;
	}
}
