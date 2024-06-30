#include <SDL/sdlinputhandler.hpp>
#include <SDL/sdldefinitions.hpp>
#include <iostream>
#include <map>
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <Windows.h> // To get the position of the mouse in the screen...
#endif

// Initialization of the internal conversion variables..
std::map <int, QGAMES::KeyCode> SDLInputHandler::_keyCodes = initKeyCodes ();
std::map <int, QGAMES::MouseButtonCode> SDLInputHandler::_mouseButtonCodes = initMouseButtonCodes ();

//---
void SDLInputHandler::initialize ()
{
	QGAMES::InputHandler::initialize ();
	initializeJoysticks ();
}

//---
void SDLInputHandler::finalize ()
{
	QGAMES::InputHandler::finalize();
	finalizeJoysticks ();
}

//---
QGAMES::Events SDLInputHandler::readEvents ()
{
	return (QGAMES::Events ());
}

// ---
void SDLInputHandler::clearPendingEvents ()
{
	SDL_FlushEvents (SDL_FIRSTEVENT, SDL_LASTEVENT);
}

// ---
QGAMES::MouseMovementEventData SDLInputHandler::whereIsMouse ()
{
	QGAMES::MouseMovementEventData result;
	SDL_GetMouseState (&result._x, &result._y);
	return (result);
}

// ---
QGAMES::MouseStateData SDLInputHandler::mouseState ()
{
	QGAMES::MouseStateData result;

	SDL_GetMouseState (&result._X._rel, &result._Y._rel);
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	POINT mP; GetCursorPos (&mP);
	result._X._abs = mP.x;
	result._Y._abs = mP.y;
#else
	// There is no funtion available to know the absolute position of the mouse 
	// cursor until SDL version 2.0.4
	result._X._abs = result._X._rel; 
	result._Y._abs = result._Y._rel;
#endif
	// No data for Z axis for a typical mouse...
	result._Z._abs = result._Z._rel = 0;

	// Get the state of each mouse button...
	result._numberButtons = 5; // left, center, right, mouse wheel up, mouse wheel down!
	result._buttons.resize (result._numberButtons);
	for (int i = 0; i < result._numberButtons; i++)
		result._buttons [i] = (SDL_GetMouseState (NULL, NULL) & SDL_BUTTON (i)) != 0;

	return (result);
}

// ---
QGAMES::JoystickStateData SDLInputHandler::joystickState (unsigned int nJ)
{
	QGAMES::JoystickStateData result;
	if (nJ >= (unsigned int) _joysticks.size ())
		return (result); // No state...the joystick doesn't exist at all!

	result._Axis.resize (SDL_JoystickNumAxes (_joysticks [nJ]));
	for (int i = 0; i < (int) result._Axis.size (); i++)
		result._Axis [i]._abs = result._Axis [i]._rel = SDL_JoystickGetAxis (_joysticks [nJ], i);

	// There is no possibility to get the rel position until version 2.0.4...
	result._numberButtons = SDL_JoystickNumButtons (_joysticks [nJ]);
	result._buttons.resize (result._numberButtons);
	for (int i = 0; i < (int) result._numberButtons; i++)
		result._buttons [i] = SDL_JoystickGetButton (_joysticks [nJ], i) != 0;

	return (result);
}

// ---
QGAMES::KeyCode SDLInputHandler::translateKeyCode (int kC)
{
	// If the key hasn't been defined, unassigned is returned!
	std::map <int, QGAMES::KeyCode>::const_iterator i = _keyCodes.find (kC);
	return ((i != _keyCodes.end ()) ? (*i).second : QGAMES::KeyCode::QGAMES_KEYUNASSIGNED);
}

// ---
QGAMES::MouseButtonCode SDLInputHandler::translateMouseButtonCode (int bC)
{
	// If the button hasn't been defined, unassigned is returned!
	std::map <int, QGAMES::MouseButtonCode>::const_iterator i = _mouseButtonCodes.find (bC);
	return ((i != _mouseButtonCodes.end ()) ? (*i).second : QGAMES::MouseButtonCode::QGAMES_BUTTONUNASSIGNED);
}

// ---
void SDLInputHandler::treatKeyboardStatus ()
{
	const unsigned __int8* k = SDL_GetKeyboardState (NULL);

	// 0x92 positions maximum...initially all are false
	// There are not 0x93 positions in the list, because there are not 0x93 SDL equivalents either!
	// This is the reason to initialize all of them to false...
	std::vector <bool> keyStatus (0x93, false); 
	keyStatus [QGAMES::KeyCode::QGAMES_ESCAPE]			=	(k [SDL_SCANCODE_ESCAPE] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_1]				=	(k [SDL_SCANCODE_1] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_2]				=	(k [SDL_SCANCODE_2] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_3]				=	(k [SDL_SCANCODE_3] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_4]				=	(k [SDL_SCANCODE_4] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_5]				=	(k [SDL_SCANCODE_5] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_6]				=	(k [SDL_SCANCODE_6] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_7]				=	(k [SDL_SCANCODE_7] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_8]				=	(k [SDL_SCANCODE_8] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_9]				=	(k [SDL_SCANCODE_9] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_0]				=	(k [SDL_SCANCODE_0] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_MINUS]			=	(k [SDL_SCANCODE_MINUS] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_EQUALS]			=	(k [SDL_SCANCODE_EQUALS] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_BACK]			=	(k [SDL_SCANCODE_BACKSPACE] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_TAB]				=	(k [SDL_SCANCODE_TAB] != 0);	
	keyStatus [QGAMES::KeyCode::QGAMES_Q]				=	(k [SDL_SCANCODE_Q] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_W]				=	(k [SDL_SCANCODE_W] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_E]				=	(k [SDL_SCANCODE_E] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_R]				=	(k [SDL_SCANCODE_R] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_T]				=	(k [SDL_SCANCODE_T] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_Y]				=	(k [SDL_SCANCODE_Y] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_U]				=	(k [SDL_SCANCODE_U] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_I]				=	(k [SDL_SCANCODE_I] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_O]				=	(k [SDL_SCANCODE_O] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_P]				=	(k [SDL_SCANCODE_P] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_LBRACKET]		=	(k [SDL_SCANCODE_LEFTBRACKET] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_RBRACKET]		=	(k [SDL_SCANCODE_RIGHTBRACKET] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_RETURN]			=	(k [SDL_SCANCODE_RETURN] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_LCONTROL]		=	(k [SDL_SCANCODE_LCTRL] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_A]				=	(k [SDL_SCANCODE_A] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_S]				=	(k [SDL_SCANCODE_S] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_D]				=	(k [SDL_SCANCODE_D] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F]				=	(k [SDL_SCANCODE_F] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_G]				=	(k [SDL_SCANCODE_G] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_H]				=	(k [SDL_SCANCODE_H] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_J]				=	(k [SDL_SCANCODE_J] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_K]				=	(k [SDL_SCANCODE_K] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_L]				=	(k [SDL_SCANCODE_L] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_SEMICOLON]		=	(k [SDL_SCANCODE_SEMICOLON] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_APOSTROPHE]		=	(k [SDL_SCANCODE_APOSTROPHE] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_GRAVE]			=	(k [SDL_SCANCODE_GRAVE] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_LSHIFT]			=	(k [SDL_SCANCODE_LSHIFT] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_BACKSLASH]		=	(k [SDL_SCANCODE_BACKSLASH] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_Z]				=	(k [SDL_SCANCODE_Z] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_X]				=	(k [SDL_SCANCODE_X] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_C]				=	(k [SDL_SCANCODE_C] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_V]				=	(k [SDL_SCANCODE_V] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_B]				=	(k [SDL_SCANCODE_B] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_N]				=	(k [SDL_SCANCODE_N] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_M]				=	(k [SDL_SCANCODE_M] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_COMMA]			=	(k [SDL_SCANCODE_COMMA] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_PERIOD]			=	(k [SDL_SCANCODE_PERIOD] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_SLASH]			=	(k [SDL_SCANCODE_SLASH] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_RSHIFT]			=	(k [SDL_SCANCODE_RSHIFT] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_MULTIPLY]		=	(k [SDL_SCANCODE_KP_MULTIPLY] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_LMENU]			=	(k [SDL_SCANCODE_MENU] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_SPACE]			=	(k [SDL_SCANCODE_SPACE] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F1]				=	(k [SDL_SCANCODE_F1] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F2]				=	(k [SDL_SCANCODE_F2] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F3]				=	(k [SDL_SCANCODE_F3] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F4]				=	(k [SDL_SCANCODE_F4] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F5]				=	(k [SDL_SCANCODE_F5] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F6]				=	(k [SDL_SCANCODE_F6] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F7]				=	(k [SDL_SCANCODE_F7] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F8]				=	(k [SDL_SCANCODE_F8] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F9]				=	(k [SDL_SCANCODE_F9] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F10]				=	(k [SDL_SCANCODE_F10] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_NUMLOCK]			=	(k [SDL_SCANCODE_NUMLOCKCLEAR] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_SCROLL]			=	(k [SDL_SCANCODE_SCROLLLOCK] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_NUMPAD7]			=	(k [SDL_SCANCODE_KP_7] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_NUMPAD8] 		=	(k [SDL_SCANCODE_KP_8] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_NUMPAD9] 		=	(k [SDL_SCANCODE_KP_9] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_SUBTRACT]		=	(k [SDL_SCANCODE_MINUS] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_NUMPAD4] 		=	(k [SDL_SCANCODE_KP_4] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_NUMPAD5] 		=	(k [SDL_SCANCODE_KP_5] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_NUMPAD6] 		=	(k [SDL_SCANCODE_KP_6] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_ADD]     		=	(k [SDL_SCANCODE_KP_PLUS] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_NUMPAD1] 		=	(k [SDL_SCANCODE_KP_1] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_NUMPAD2] 		=	(k [SDL_SCANCODE_KP_2] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_NUMPAD3] 		=	(k [SDL_SCANCODE_KP_3] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_NUMPAD0] 		=	(k [SDL_SCANCODE_KP_0] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_DECIMAL] 		=	(k [SDL_SCANCODE_KP_DECIMAL] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F11]     		=	(k [SDL_SCANCODE_F11] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F12]     		=	(k [SDL_SCANCODE_F12] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F13]     		=	(k [SDL_SCANCODE_F13] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F14]     		=	(k [SDL_SCANCODE_F14] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_F15] 			=	(k [SDL_SCANCODE_F15] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_CAPITAL]			=	(k [SDL_SCANCODE_CAPSLOCK] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_NUMPADEQUALS]	=	(k [SDL_SCANCODE_KP_EQUALS] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_AT]          	=	(k [SDL_SCANCODE_KP_AT] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_COLON]       	=	(k [SDL_SCANCODE_KP_COLON] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_STOP]        	=	(k [SDL_SCANCODE_STOP] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_NUMPADENTER] 	=	(k [SDL_SCANCODE_KP_ENTER] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_RCONTROL] 		=	(k [SDL_SCANCODE_RCTRL] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_MUTE]        	=	(k [SDL_SCANCODE_MUTE] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_CALCULATOR]  	=	(k [SDL_SCANCODE_CALCULATOR] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_PLAYPAUSE]   	=	(k [SDL_SCANCODE_PAUSE] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_MEDIASTOP]   	=	(k [SDL_SCANCODE_STOP] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_VOLUMEDOWN]  	=	(k [SDL_SCANCODE_VOLUMEDOWN] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_VOLUMEUP]   		=	(k [SDL_SCANCODE_VOLUMEUP] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_WEBHOME]     	=	(k [SDL_SCANCODE_HOME] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_NUMPADCOMMA] 	=	(k [SDL_SCANCODE_KP_COMMA] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_DIVIDE]      	=	(k [SDL_SCANCODE_KP_DIVIDE] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_SYSRQ]       	=	(k [SDL_SCANCODE_SYSREQ] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_RMENU]       	=	(k [SDL_SCANCODE_MENU] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_PAUSE]       	=	(k [SDL_SCANCODE_PAUSE] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_HOME]        	=	(k [SDL_SCANCODE_HOME] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_UP]          	=	(k [SDL_SCANCODE_UP] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_PGUP]        	=	(k [SDL_SCANCODE_PAGEUP] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_LEFT]        	=	(k [SDL_SCANCODE_LEFT] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_RIGHT]       	=	(k [SDL_SCANCODE_RIGHT] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_END]				=	(k [SDL_SCANCODE_END] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_DOWN]			=	(k [SDL_SCANCODE_DOWN] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_PGDOWN]      	=	(k [SDL_SCANCODE_PAGEDOWN] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_INSERT]      	=	(k [SDL_SCANCODE_INSERT] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_DELETE]      	=	(k [SDL_SCANCODE_DELETE] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_LWIN]        	=	(k [SDL_SCANCODE_LGUI] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_RWIN]        	=	(k [SDL_SCANCODE_RGUI] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_APPS]        	=	(k [SDL_SCANCODE_APPLICATION] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_POWER]			=	(k [SDL_SCANCODE_POWER] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_SLEEP]       	=	(k [SDL_SCANCODE_SLEEP] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_WEBSEARCH]  		=	(k [SDL_SCANCODE_AC_SEARCH] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_WEBFAVORITES]	=	(k [SDL_SCANCODE_AC_BOOKMARKS] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_WEBREFRESH]  	=	(k [SDL_SCANCODE_AC_REFRESH] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_WEBSTOP]     	=	(k [SDL_SCANCODE_AC_STOP] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_WEBFORWARD]  	=	(k [SDL_SCANCODE_AC_FORWARD] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_WEBBACK]     	=	(k [SDL_SCANCODE_AC_BACK] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_MYCOMPUTER]  	=	(k [SDL_SCANCODE_COMPUTER] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_MAIL] 			=	(k [SDL_SCANCODE_MAIL] != 0);
	keyStatus [QGAMES::KeyCode::QGAMES_MEDIASELECT] 	=	(k [SDL_SCANCODE_MEDIASELECT] != 0);
	
	onKeyboardStatus (keyStatus);
}

// ---
void SDLInputHandler::treatJoystickStatus ()
{
	std::vector <QGAMES::JoystickStateData> st;
	for (int i = 0; i < (int) _joysticks.size (); i++)
		st.push_back (joystickState (i));
	onJoystickStatus (st); // Treat the full status...
}

//---
void SDLInputHandler::initializeJoysticks ()
{
	int e = SDL_InitSubSystem (SDL_INIT_JOYSTICK);
#ifndef NDEBUG
	if (e != 0)
		std::cout << SDL_GetError () << std::endl;
#endif

	if (SDL_NumJoysticks () > 0)
	{
		for (int i = 0; i < SDL_NumJoysticks (); i++)
		{
			SDL_Joystick* joy = SDL_JoystickOpen (i);
			if (joy)
				_joysticks.push_back (joy);
		}
	}

	SDL_JoystickEventState (SDL_ENABLE);
}

//---
void SDLInputHandler::finalizeJoysticks ()
{
	for (SDLJoysticks::iterator i = _joysticks.begin ();
			i != _joysticks.end (); i++)
		SDL_JoystickClose ((*i));
}

// ---
std::map <int, QGAMES::KeyCode> SDLInputHandler::initKeyCodes ()
{
	std::map <int, QGAMES::KeyCode> result;
	// Inpuyt key code...					// Translated key code...
	result [SDL_SCANCODE_ESCAPE]			=	QGAMES::KeyCode::QGAMES_ESCAPE;
	result [SDL_SCANCODE_1]					=	QGAMES::KeyCode::QGAMES_1;
	result [SDL_SCANCODE_2]					=	QGAMES::KeyCode::QGAMES_2;
	result [SDL_SCANCODE_3]					=	QGAMES::KeyCode::QGAMES_3;
	result [SDL_SCANCODE_4]					=	QGAMES::KeyCode::QGAMES_4;
	result [SDL_SCANCODE_5]					=	QGAMES::KeyCode::QGAMES_5;
	result [SDL_SCANCODE_6]					=	QGAMES::KeyCode::QGAMES_6;
	result [SDL_SCANCODE_7]					=	QGAMES::KeyCode::QGAMES_7;
	result [SDL_SCANCODE_8]					=	QGAMES::KeyCode::QGAMES_8;
	result [SDL_SCANCODE_9]					=	QGAMES::KeyCode::QGAMES_9;
	result [SDL_SCANCODE_0]					=	QGAMES::KeyCode::QGAMES_0;
	result [SDL_SCANCODE_MINUS] 			=	QGAMES::KeyCode::QGAMES_MINUS;
	result [SDL_SCANCODE_EQUALS] 			=	QGAMES::KeyCode::QGAMES_EQUALS;
	result [SDL_SCANCODE_BACKSPACE] 		=	QGAMES::KeyCode::QGAMES_BACK;
	result [SDL_SCANCODE_TAB] 				=	QGAMES::KeyCode::QGAMES_TAB;
	result [SDL_SCANCODE_Q] 				=	QGAMES::KeyCode::QGAMES_Q;
	result [SDL_SCANCODE_W] 				=	QGAMES::KeyCode::QGAMES_W;
	result [SDL_SCANCODE_E] 				=	QGAMES::KeyCode::QGAMES_E;
	result [SDL_SCANCODE_R] 				=	QGAMES::KeyCode::QGAMES_R;
	result [SDL_SCANCODE_T] 				=	QGAMES::KeyCode::QGAMES_T;
	result [SDL_SCANCODE_Y] 				=	QGAMES::KeyCode::QGAMES_Y;
	result [SDL_SCANCODE_U] 				=	QGAMES::KeyCode::QGAMES_U;
	result [SDL_SCANCODE_I] 				=	QGAMES::KeyCode::QGAMES_I;
	result [SDL_SCANCODE_O] 				=	QGAMES::KeyCode::QGAMES_O;
	result [SDL_SCANCODE_P] 				=	QGAMES::KeyCode::QGAMES_P;
	result [SDL_SCANCODE_LEFTBRACKET] 		=	QGAMES::KeyCode::QGAMES_LBRACKET;
	result [SDL_SCANCODE_RIGHTBRACKET] 		=	QGAMES::KeyCode::QGAMES_RBRACKET;
	result [SDL_SCANCODE_RETURN] 			=	QGAMES::KeyCode::QGAMES_RETURN;
	result [SDL_SCANCODE_LCTRL] 			=	QGAMES::KeyCode::QGAMES_LCONTROL;
	result [SDL_SCANCODE_A] 				=	QGAMES::KeyCode::QGAMES_A;
	result [SDL_SCANCODE_S] 				=	QGAMES::KeyCode::QGAMES_S;
	result [SDL_SCANCODE_D] 				=	QGAMES::KeyCode::QGAMES_D;
	result [SDL_SCANCODE_F] 				=	QGAMES::KeyCode::QGAMES_F;
	result [SDL_SCANCODE_G] 				=	QGAMES::KeyCode::QGAMES_G;
	result [SDL_SCANCODE_H] 				=	QGAMES::KeyCode::QGAMES_H;
	result [SDL_SCANCODE_J] 				=	QGAMES::KeyCode::QGAMES_J;
	result [SDL_SCANCODE_K] 				=	QGAMES::KeyCode::QGAMES_K;
	result [SDL_SCANCODE_L] 				=	QGAMES::KeyCode::QGAMES_L;
	result [SDL_SCANCODE_SEMICOLON] 		=	QGAMES::KeyCode::QGAMES_SEMICOLON;
	result [SDL_SCANCODE_APOSTROPHE]		=	QGAMES::KeyCode::QGAMES_APOSTROPHE;
	result [SDL_SCANCODE_GRAVE] 			=	QGAMES::KeyCode::QGAMES_GRAVE;
	result [SDL_SCANCODE_LSHIFT] 			=	QGAMES::KeyCode::QGAMES_LSHIFT;
	result [SDL_SCANCODE_BACKSLASH] 		=	QGAMES::KeyCode::QGAMES_BACKSLASH;
	result [SDL_SCANCODE_Z] 				=	QGAMES::KeyCode::QGAMES_Z;
	result [SDL_SCANCODE_X] 				=	QGAMES::KeyCode::QGAMES_X;
	result [SDL_SCANCODE_C] 				=	QGAMES::KeyCode::QGAMES_C;
	result [SDL_SCANCODE_V] 				=	QGAMES::KeyCode::QGAMES_V;
	result [SDL_SCANCODE_B] 				=	QGAMES::KeyCode::QGAMES_B;
	result [SDL_SCANCODE_N] 				=	QGAMES::KeyCode::QGAMES_N;
	result [SDL_SCANCODE_M] 				=	QGAMES::KeyCode::QGAMES_M;
	result [SDL_SCANCODE_COMMA] 			=	QGAMES::KeyCode::QGAMES_COMMA;
	result [SDL_SCANCODE_PERIOD] 			=	QGAMES::KeyCode::QGAMES_PERIOD;
	result [SDL_SCANCODE_SLASH] 			=	QGAMES::KeyCode::QGAMES_SLASH;
	result [SDL_SCANCODE_RSHIFT]			=	QGAMES::KeyCode::QGAMES_RSHIFT;
	result [SDL_SCANCODE_KP_MULTIPLY] 		=	QGAMES::KeyCode::QGAMES_MULTIPLY;
	result [SDL_SCANCODE_MENU] 				=	QGAMES::KeyCode::QGAMES_LMENU;
	result [SDL_SCANCODE_SPACE] 			=	QGAMES::KeyCode::QGAMES_SPACE;
	result [SDL_SCANCODE_F1] 				=	QGAMES::KeyCode::QGAMES_F1;
	result [SDL_SCANCODE_F2] 				=	QGAMES::KeyCode::QGAMES_F2;
	result [SDL_SCANCODE_F3] 				=	QGAMES::KeyCode::QGAMES_F3;
	result [SDL_SCANCODE_F4] 				=	QGAMES::KeyCode::QGAMES_F4;
	result [SDL_SCANCODE_F5] 				=	QGAMES::KeyCode::QGAMES_F5;
	result [SDL_SCANCODE_F6] 				=	QGAMES::KeyCode::QGAMES_F6;
	result [SDL_SCANCODE_F7] 				=	QGAMES::KeyCode::QGAMES_F7;
	result [SDL_SCANCODE_F8] 				=	QGAMES::KeyCode::QGAMES_F8;
	result [SDL_SCANCODE_F9] 				=	QGAMES::KeyCode::QGAMES_F9;
	result [SDL_SCANCODE_F10] 				=	QGAMES::KeyCode::QGAMES_F10;    
	result [SDL_SCANCODE_NUMLOCKCLEAR] 		=	QGAMES::KeyCode::QGAMES_NUMLOCK; 
	result [SDL_SCANCODE_SCROLLLOCK] 		=	QGAMES::KeyCode::QGAMES_SCROLL; 
	result [SDL_SCANCODE_KP_7] 				=	QGAMES::KeyCode::QGAMES_NUMPAD7; 
	result [SDL_SCANCODE_KP_8] 				=	QGAMES::KeyCode::QGAMES_NUMPAD8; 
	result [SDL_SCANCODE_KP_9] 				=	QGAMES::KeyCode::QGAMES_NUMPAD9; 
	result [SDL_SCANCODE_MINUS] 			=	QGAMES::KeyCode::QGAMES_SUBTRACT;
	result [SDL_SCANCODE_KP_4] 				=	QGAMES::KeyCode::QGAMES_NUMPAD4; 
	result [SDL_SCANCODE_KP_5] 				=	QGAMES::KeyCode::QGAMES_NUMPAD5; 
	result [SDL_SCANCODE_KP_6] 				=	QGAMES::KeyCode::QGAMES_NUMPAD6; 
	result [SDL_SCANCODE_KP_PLUS] 			=	QGAMES::KeyCode::QGAMES_ADD;     
	result [SDL_SCANCODE_KP_1] 				=	QGAMES::KeyCode::QGAMES_NUMPAD1; 
	result [SDL_SCANCODE_KP_2] 				=	QGAMES::KeyCode::QGAMES_NUMPAD2; 
	result [SDL_SCANCODE_KP_3] 				=	QGAMES::KeyCode::QGAMES_NUMPAD3; 
	result [SDL_SCANCODE_KP_0] 				=	QGAMES::KeyCode::QGAMES_NUMPAD0; 
	result [SDL_SCANCODE_KP_DECIMAL] 		=	QGAMES::KeyCode::QGAMES_DECIMAL; 
	result [SDL_SCANCODE_F11] 				=	QGAMES::KeyCode::QGAMES_F11;     
	result [SDL_SCANCODE_F12] 				=	QGAMES::KeyCode::QGAMES_F12;     
	result [SDL_SCANCODE_F13] 				=	QGAMES::KeyCode::QGAMES_F13;     
	result [SDL_SCANCODE_F14] 				=	QGAMES::KeyCode::QGAMES_F14;     
	result [SDL_SCANCODE_F15] 				=	QGAMES::KeyCode::QGAMES_F15; 
	result [SDL_SCANCODE_CAPSLOCK]			=	QGAMES::KeyCode::QGAMES_CAPITAL;
	result [SDL_SCANCODE_KP_EQUALS] 		=	QGAMES::KeyCode::QGAMES_NUMPADEQUALS;
	result [SDL_SCANCODE_KP_AT] 			=	QGAMES::KeyCode::QGAMES_AT;          
	result [SDL_SCANCODE_KP_COLON]			=	QGAMES::KeyCode::QGAMES_COLON;       
	result [SDL_SCANCODE_STOP] 				=	QGAMES::KeyCode::QGAMES_STOP;        
	result [SDL_SCANCODE_KP_ENTER] 			=	QGAMES::KeyCode::QGAMES_NUMPADENTER; 
	result [SDL_SCANCODE_RCTRL] 			=	QGAMES::KeyCode::QGAMES_RCONTROL;    
	result [SDL_SCANCODE_MUTE] 				=	QGAMES::KeyCode::QGAMES_MUTE;        
	result [SDL_SCANCODE_CALCULATOR] 		=	QGAMES::KeyCode::QGAMES_CALCULATOR;  
	result [SDL_SCANCODE_PAUSE] 			=	QGAMES::KeyCode::QGAMES_PLAYPAUSE;   
	result [SDL_SCANCODE_STOP] 				=	QGAMES::KeyCode::QGAMES_MEDIASTOP;   
	result [SDL_SCANCODE_VOLUMEDOWN] 		=	QGAMES::KeyCode::QGAMES_VOLUMEDOWN;  
	result [SDL_SCANCODE_VOLUMEUP] 			=	QGAMES::KeyCode::QGAMES_VOLUMEUP;    
	result [SDL_SCANCODE_HOME] 				=	QGAMES::KeyCode::QGAMES_WEBHOME;     
	result [SDL_SCANCODE_KP_COMMA] 			=	QGAMES::KeyCode::QGAMES_NUMPADCOMMA; 
	result [SDL_SCANCODE_KP_DIVIDE] 		=	QGAMES::KeyCode::QGAMES_DIVIDE;      
	result [SDL_SCANCODE_SYSREQ] 			=	QGAMES::KeyCode::QGAMES_SYSRQ;       
	result [SDL_SCANCODE_MENU] 				=	QGAMES::KeyCode::QGAMES_RMENU;       
	result [SDL_SCANCODE_PAUSE] 			=	QGAMES::KeyCode::QGAMES_PAUSE;       
	result [SDL_SCANCODE_HOME] 				=	QGAMES::KeyCode::QGAMES_HOME;        
	result [SDL_SCANCODE_UP] 				=	QGAMES::KeyCode::QGAMES_UP;          
	result [SDL_SCANCODE_PAGEUP] 			=	QGAMES::KeyCode::QGAMES_PGUP;        
	result [SDL_SCANCODE_LEFT] 				=	QGAMES::KeyCode::QGAMES_LEFT;        
	result [SDL_SCANCODE_RIGHT] 			=	QGAMES::KeyCode::QGAMES_RIGHT;       
	result [SDL_SCANCODE_END] 				=	QGAMES::KeyCode::QGAMES_END;         
	result [SDL_SCANCODE_DOWN] 				=	QGAMES::KeyCode::QGAMES_DOWN;        
	result [SDL_SCANCODE_PAGEDOWN] 			=	QGAMES::KeyCode::QGAMES_PGDOWN;      
	result [SDL_SCANCODE_INSERT] 			=	QGAMES::KeyCode::QGAMES_INSERT;      
	result [SDL_SCANCODE_DELETE] 			=	QGAMES::KeyCode::QGAMES_DELETE;      
	result [SDL_SCANCODE_LGUI] 				=	QGAMES::KeyCode::QGAMES_LWIN;        
	result [SDL_SCANCODE_RGUI] 				=	QGAMES::KeyCode::QGAMES_RWIN;        
	result [SDL_SCANCODE_APPLICATION] 		=	QGAMES::KeyCode::QGAMES_APPS;        
	result [SDL_SCANCODE_POWER] 			=	QGAMES::KeyCode::QGAMES_POWER;       
	result [SDL_SCANCODE_SLEEP] 			=	QGAMES::KeyCode::QGAMES_SLEEP;       
	result [SDL_SCANCODE_AC_SEARCH] 		=	QGAMES::KeyCode::QGAMES_WEBSEARCH;   
	result [SDL_SCANCODE_AC_BOOKMARKS] 		=	QGAMES::KeyCode::QGAMES_WEBFAVORITES;
	result [SDL_SCANCODE_AC_REFRESH] 		=	QGAMES::KeyCode::QGAMES_WEBREFRESH;  
	result [SDL_SCANCODE_AC_STOP] 			=	QGAMES::KeyCode::QGAMES_WEBSTOP;     
	result [SDL_SCANCODE_AC_FORWARD] 		=	QGAMES::KeyCode::QGAMES_WEBFORWARD;  
	result [SDL_SCANCODE_AC_BACK] 			=	QGAMES::KeyCode::QGAMES_WEBBACK;     
	result [SDL_SCANCODE_COMPUTER] 			=	QGAMES::KeyCode::QGAMES_MYCOMPUTER;  
	result [SDL_SCANCODE_MAIL]				=	QGAMES::KeyCode::QGAMES_MAIL;        
	result [SDL_SCANCODE_MEDIASELECT]		=	QGAMES::KeyCode::QGAMES_MEDIASELECT;
	return (result);
}

// ---
std::map <int, QGAMES::MouseButtonCode> SDLInputHandler::initMouseButtonCodes ()
{
	std::map <int, QGAMES::MouseButtonCode> result;
	// Input button code...			// Translated button code...
	result [SDL_BUTTON_LEFT]				=	QGAMES::MouseButtonCode::QGAMES_BUTTONLEFT;
	result [SDL_BUTTON_RIGHT]				=	QGAMES::MouseButtonCode::QGAMES_BUTTONRIGHT;
	result [SDL_BUTTON_MIDDLE]				=	QGAMES::MouseButtonCode::QGAMES_BUTTONMIDDLE;
	return (result);
}
