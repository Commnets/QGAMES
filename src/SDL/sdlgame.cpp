#include <SDL/sdlgame.hpp>
#include <SDL/sdldefinitions.hpp>
#include <SDL/sdlscreen.hpp>
#include <SDL/sdlinputhandler.hpp>
#include <SDL/sdlformbuilder.hpp>
#include <SDL/sdlsoundbuilder.hpp>
#include <FMOD/fmodsoundbuilder.hpp>
#include <Common/game.hpp>
#include <SDL_ttf.h>
#include <iostream>
#include <assert.h>

// ---
bool SDLGame::initialize ()
{
	int e = SDL_Init (!_usingFMOD ? SDL_INIT_EVERYTHING 
								  : (SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS | // Without AUDIO
									 SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER));
#ifndef NDEBUG
	if (e != 0)
		std::cout << SDL_GetError () << std::endl;
#endif

	e = TTF_Init ();
#ifndef NDEBUG
	if (e == -1)
		std::cout << TTF_GetError () << std::endl;
#endif

	return ((e == 0) ? true : false);
}

// ---
QGAMES::FormBuilder* SDLGame::createFormBuilder ()
{
	assert (game ()); // It has to be a game linked...

	return (new SDL2DSpriteBuilder (game () -> parameter (__GAME_PROPERTYFORMSFILE__), 
		(SDLScreen*) game () -> mainScreen ()));
}

// ---
QGAMES::SoundBuilder* SDLGame::createSoundBuilder ()
{
	assert (game ()); // It has to be a game linked...

	QGAMES::SoundBuilder* result = 
		new SDLSoundBuilder (game () -> parameter (__GAME_PROPERTYSOUNDSFILE__));
	if (_usingFMOD)
		result = new FMODSoundBuilder (game () -> parameter (__GAME_PROPERTYSOUNDSFILE__));

	return (result);
}

// ---
QGAMES::InputHandler* SDLGame::createInputHandler (QGAMES::InputHandler::Behaviour* bhv)
{
	assert (bhv);

	return (new SDLInputHandler (game (), bhv));
}

// ---
QGAMES::Screen* SDLGame::createScreen (const std::string& t, const QGAMES::Position& pos, 
		int w, int h, int pX, int pY)
{
	// This is just only because a bug in SDL version
	// They say they will solve it in further versions...we will see!!
	// This happens when the version of the sdl linked to is less than 2.0.20!
	SDL_version v; SDL_GetVersion (&v);
	bool scl = SDL_VERSIONNUM (v.major, v.minor, v.patch) < SDL_VERSIONNUM (2, 0, 20);
	float sX = scl ? (float) std::atof (game () -> parameter (__GAME_XSCALE__).c_str ()) : 1.0f;
	float sY = scl ? (float) std::atof (game () -> parameter (__GAME_YSCALE__).c_str ()) : 1.0f;
	return (new SDLScreen (t, pos, w, h, pX, pY, sX, sY));
}

// ---
void SDLGame::handleEvents ()
{
	SDL_Event evnt;

	SDLInputHandler* SDLiH = dynamic_cast <SDLInputHandler*> (_game -> inputHandler ());
	assert (SDLiH); // If it is not an SDL Input Handler...makes not sense to continue...

	SDLiH -> beforeTreatingEvents ();
	while (SDL_PollEvent (&evnt)) treatEvent (evnt);
	SDLiH -> treatKeyboardStatus ();
	SDLiH -> treatJoystickStatus ();
	SDLiH -> treatMouseStatus ();
	SDLiH -> afterTreatingEvents ();
}

// ---
void SDLGame::finalize ()
{
	SDL_Quit ();
	TTF_Quit ();
}

// ---
void SDLGame::treatEvent (const QGAMES::Event& evnt)
{
	// Nothing to do...
	// This method does not apply in the case of SDL library...
}

// ---
void SDLGame::treatEvent (SDL_Event& evnt)
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
				game () -> inputHandler () -> onJoystickAxisMoveEvent (dt);
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
				game () -> inputHandler () -> onJoystickButtonEvent (dt);
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
				game () -> inputHandler () -> onJoystickButtonEvent (dt);
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
				game () -> inputHandler () -> onKeyboardEvent (dt);
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
				game () -> inputHandler () -> onKeyboardEvent (dt);
				delete (dt);
			}

			break;

		case SDL_MOUSEMOTION:
			{
				QGAMES::MouseMovementEventData* dt = 
					new QGAMES::MouseMovementEventData;
				dt -> _x = evnt.motion.x;
				dt -> _y = evnt.motion.y;
				game () -> inputHandler () -> onMouseMovementEvent (dt);
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
				game () -> inputHandler () -> onMouseButtonEvent (dt);
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
				game () -> inputHandler () -> onMouseButtonEvent (dt);
				delete (dt);
			}

			break;

		default:
			break;
	}
}
