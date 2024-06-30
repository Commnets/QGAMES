/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: inputhandler.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class to read the input events. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_INPUTHANDLER__
#define __QGAMES_INPUTHANDLER__

#include <Common/event.hpp>
#include <Common/notifierobserver.hpp>
#include <Common/definitions.hpp>
#include <Common/position.hpp>
#include <limits>
#include <vector>
#include <map>

namespace QGAMES
{
	class Game;
	class GameState;

	//! Keyboard scan codes
	enum KeyCode
	{
		QGAMES_KEYUNASSIGNED	= 0x00,
		QGAMES_ESCAPE			= 0x01,
		QGAMES_1				= 0x02,
		QGAMES_2				= 0x03,
		QGAMES_3				= 0x04,
		QGAMES_4				= 0x05,
		QGAMES_5				= 0x06,
		QGAMES_6				= 0x07,
		QGAMES_7				= 0x08,
		QGAMES_8				= 0x09,
		QGAMES_9				= 0x0A,
		QGAMES_0				= 0x0B,
		QGAMES_MINUS			= 0x0C,    // - on main keyboard
		QGAMES_EQUALS			= 0x0D,
		QGAMES_BACK				= 0x0E,    // backspace
		QGAMES_TAB				= 0x0F,
		QGAMES_Q				= 0x10,
		QGAMES_W				= 0x11,
		QGAMES_E				= 0x12,
		QGAMES_R				= 0x13,
		QGAMES_T				= 0x14,
		QGAMES_Y				= 0x15,
		QGAMES_U				= 0x16,
		QGAMES_I				= 0x17,
		QGAMES_O				= 0x18,
		QGAMES_P				= 0x19,
		QGAMES_LBRACKET			= 0x1A,
		QGAMES_RBRACKET			= 0x1B,
		QGAMES_RETURN			= 0x1C,    // Enter on main keyboard
		QGAMES_LCONTROL			= 0x1D,
		QGAMES_A				= 0x1E,
		QGAMES_S				= 0x1F,
		QGAMES_D				= 0x20,
		QGAMES_F				= 0x21,
		QGAMES_G				= 0x22,
		QGAMES_H				= 0x23,
		QGAMES_J				= 0x24,
		QGAMES_K				= 0x25,
		QGAMES_L				= 0x26,
		QGAMES_SEMICOLON		= 0x27,
		QGAMES_APOSTROPHE		= 0x28,
		QGAMES_GRAVE			= 0x29,    // accent
		QGAMES_LSHIFT			= 0x2A,
		QGAMES_BACKSLASH		= 0x2B,
		QGAMES_Z				= 0x2C,
		QGAMES_X				= 0x2D,
		QGAMES_C				= 0x2E,
		QGAMES_V				= 0x2F,
		QGAMES_B				= 0x30,
		QGAMES_N				= 0x31,
		QGAMES_M				= 0x32,
		QGAMES_COMMA			= 0x33,
		QGAMES_PERIOD			= 0x34,    // . on main keyboard
		QGAMES_SLASH			= 0x35,    // / on main keyboard
		QGAMES_RSHIFT			= 0x36,
		QGAMES_MULTIPLY			= 0x37,    // * on numeric keypad
		QGAMES_LMENU			= 0x38,    // left Alt
		QGAMES_SPACE			= 0x39,
		QGAMES_CAPITAL			= 0x3A,
		QGAMES_F1				= 0x3B,
		QGAMES_F2				= 0x3C,
		QGAMES_F3				= 0x3D,
		QGAMES_F4				= 0x3E,
		QGAMES_F5				= 0x3F,
		QGAMES_F6				= 0x40,
		QGAMES_F7				= 0x41,
		QGAMES_F8				= 0x42,
		QGAMES_F9				= 0x43,
		QGAMES_F10				= 0x44,
		QGAMES_NUMLOCK			= 0x45,
		QGAMES_SCROLL			= 0x46,    // Scroll Lock
		QGAMES_NUMPAD7			= 0x47,
		QGAMES_NUMPAD8			= 0x48,
		QGAMES_NUMPAD9			= 0x49,
		QGAMES_SUBTRACT			= 0x4A,    // - on numeric keypad
		QGAMES_NUMPAD4			= 0x4B,
		QGAMES_NUMPAD5			= 0x4C,
		QGAMES_NUMPAD6			= 0x4D,
		QGAMES_ADD				= 0x4E,    // + on numeric keypad
		QGAMES_NUMPAD1			= 0x4F,
		QGAMES_NUMPAD2			= 0x50,
		QGAMES_NUMPAD3			= 0x51,
		QGAMES_NUMPAD0			= 0x52,
		QGAMES_DECIMAL			= 0x53,    // . on numeric keypad
		QGAMES_OEM_102			= 0x56,    // < > | on UK/Germany keyboards
		QGAMES_F11				= 0x57,
		QGAMES_F12				= 0x58,
		QGAMES_F13				= 0x59,    //                     (NEC PC98)
		QGAMES_F14				= 0x5A,    //                     (NEC PC98)
		QGAMES_F15				= 0x5B,    //                     (NEC PC98)
		QGAMES_KANA				= 0x5C,    // (Japanese keyboard)
		QGAMES_ABNT_C1			= 0x5D,    // / ? on Portugese (Brazilian) keyboards
		QGAMES_CONVERT			= 0x5E,    // (Japanese keyboard)
		QGAMES_NOCONVERT		= 0x5F,    // (Japanese keyboard)
		QGAMES_YEN				= 0x60,    // (Japanese keyboard)
		QGAMES_ABNT_C2			= 0x61,    // Numpad . on Portugese (Brazilian) keyboards
		QGAMES_NUMPADEQUALS		= 0x62,    // = on numeric keypad (NEC PC98)
		QGAMES_PREVTRACK		= 0x63,    // Previous Track (KC_CIRCUMFLEX on Japanese keyboard)
		QGAMES_AT				= 0x64,    //                     (NEC PC98)
		QGAMES_COLON			= 0x65,    //                     (NEC PC98)
		QGAMES_UNDERLINE		= 0x66,    //                     (NEC PC98)
		QGAMES_KANJI			= 0x67,    // (Japanese keyboard)
		QGAMES_STOP				= 0x68,    //                     (NEC PC98)
		QGAMES_AX				= 0x69,    //                     (Japan AX)
		QGAMES_UNLABELED		= 0x6A,    //                        (J3100)
		QGAMES_NEXTTRACK		= 0x6B,    // Next Track
		QGAMES_NUMPADENTER		= 0x6C,    // Enter on numeric keypad
		QGAMES_RCONTROL			= 0x6D,
		QGAMES_MUTE				= 0x6E,    // Mute
		QGAMES_CALCULATOR		= 0x6F,    // Calculator
		QGAMES_PLAYPAUSE		= 0x70,    // Play / Pause
		QGAMES_MEDIASTOP		= 0x71,    // Media Stop
		QGAMES_VOLUMEDOWN		= 0x72,    // Volume -
		QGAMES_VOLUMEUP			= 0x73,    // Volume +
		QGAMES_WEBHOME			= 0x74,    // Web home
		QGAMES_NUMPADCOMMA		= 0x75,    // , on numeric keypad (NEC PC98)
		QGAMES_DIVIDE			= 0x76,    // / on numeric keypad
		QGAMES_SYSRQ			= 0x77,
		QGAMES_RMENU			= 0x78,    // right Alt
		QGAMES_PAUSE			= 0x79,    // Pause
		QGAMES_HOME				= 0x7A,    // Home on arrow keypad
		QGAMES_UP				= 0x7B,    // UpArrow on arrow keypad
		QGAMES_PGUP				= 0x7C,    // PgUp on arrow keypad
		QGAMES_LEFT				= 0x7D,    // LeftArrow on arrow keypad
		QGAMES_RIGHT			= 0x7E,    // RightArrow on arrow keypad
		QGAMES_END				= 0x7F,    // End on arrow keypad
		QGAMES_DOWN				= 0x80,    // DownArrow on arrow keypad
		QGAMES_PGDOWN			= 0x81,    // PgDn on arrow keypad
		QGAMES_INSERT			= 0x82,    // Insert on arrow keypad
		QGAMES_DELETE			= 0x83,    // Delete on arrow keypad
		QGAMES_LWIN				= 0x84,    // Left Windows key
		QGAMES_RWIN				= 0x85,    // Right Windows key
		QGAMES_APPS				= 0x86,    // AppMenu key
		QGAMES_POWER			= 0x87,    // System Power
		QGAMES_SLEEP			= 0x88,    // System Sleep
		QGAMES_WAKE				= 0x89,    // System Wake
		QGAMES_WEBSEARCH		= 0x8A,    // Web Search
		QGAMES_WEBFAVORITES		= 0x8B,    // Web Favorites
		QGAMES_WEBREFRESH		= 0x8C,    // Web Refresh
		QGAMES_WEBSTOP			= 0x8D,    // Web Stop
		QGAMES_WEBFORWARD		= 0x8E,    // Web Forward
		QGAMES_WEBBACK			= 0x8F,    // Web Back
		QGAMES_MYCOMPUTER		= 0x90,    // My Computer
		QGAMES_MAIL				= 0x91,    // Mail
		QGAMES_MEDIASELECT		= 0x92     // Media Select
	}; // From 0x00 to 0x92 (0x93 in total)

	// Mouse Button Codes
	enum MouseButtonCode
	{
		QGAMES_BUTTONUNASSIGNED	= 0x00,
		QGAMES_BUTTONLEFT		= 0x01,
		QGAMES_BUTTONRIGHT		= 0x02,
		QGAMES_BUTTONMIDDLE		= 0x03
	}; // From 0x00 to 0x03 (0x04 in total)

	/** Internal structure used in mouse state and joystick state. */
	struct AxisData
	{
		AxisData () : _abs (0), _rel (0) { }
		int _abs, _rel;
	};

	/** A structure to keep mouse state information. */
	struct MouseStateData
	{
		MouseStateData () : _X (), _Y (), _Z (), _numberButtons (0), _buttons ()
							{ }
		AxisData _X, _Y, _Z;
		int _numberButtons; // How many buttons are there?
		std::vector <bool> _buttons; // Which is the status of every button?
	};

	/** A structure to keep joystick state information. */
	struct JoystickStateData
	{
		JoystickStateData () : _Axis (), _numberButtons (0), _buttons () 
							{ }
		std::vector <AxisData> _Axis;
		int _numberButtons;
		std::vector <bool> _buttons;
	};

	/** A structure to keep joystick event information. */
	struct JoystickMovementEventData
	{
		JoystickMovementEventData ()
			: _numberJoystick (0), _numberAxis (0), _deadLine (0) 
							{ }
		int _numberJoystick;
		int _numberAxis;
		int _deadLine;
	};

	/** A structure to keep joystick event information. */
	struct JoystickButtonEventData
	{
		JoystickButtonEventData ()
			: _numberJoystick (0), _numberButton (0), _on (false) 
							{ }
		int _numberJoystick;
		int _numberButton;
		bool _on;
	};

	/** A structure to keep keyobard event information. */
	struct KeyBoardEventData
	{
		KeyBoardEventData ()
			: _key (0), _on (false), _repeat (false), _unicode (0), _internalCode (KeyCode::QGAMES_KEYUNASSIGNED) 
							{ }
		int _key; // The key will depend on the final library used...
		bool _on;
		bool _repeat;
		int _unicode; // The value will depend on the final library used...
		KeyCode _internalCode; // This code is a QGAMES code. It is the result of translating _key using translateKeyCode
	};

	/** A structure to keep mouse event information. */
	struct MouseMovementEventData
	{
		MouseMovementEventData ()
			: _x (0), _y (0), _z (0) 
							{ }
		int _x;
		int _y;
		int _z;
	};

	/** A structure to keep mouse event information. */
	struct MouseButtonEventData
	{
		MouseButtonEventData ()
			: _button (0), _on (false), _internalCode (MouseButtonCode::QGAMES_BUTTONUNASSIGNED) 
							{ }
		int _button;
		bool _on;
		MouseButtonCode _internalCode;
	};
	
	/** An input handler is an structure to read events from the keyboard, the joysticks,... \n
		These events will be traslated into actions in the game. \n
		It is a very pure virtual class. \n
		It has to be redefined depending on the library finally used (@see GameImplementation). \n
		This class is a notifier. By default notifies every event detected. */
	class InputHandler : public Notifier
	{
		public:
		/** The behaviour represents the way the input handler reacts.
			Input handler usually has to be redefined taken into account the libraries behind
			but the behaviour could be the same. */
		class Behaviour
		{
			public:
			Behaviour ()
				: _game (NULL),
				  _inputHandler (NULL)
							{ }

			/** Methods to set the reference to the game and to th einput handler owner... \n
				they are used when the inputhandler is initialized... */
			Game* game () 
							{ return (_game); }
			void setGame (Game* g);
			void setInputHandler (InputHandler* iH) 
							{ _inputHandler = iH; }
			InputHandler* inputHandler ()
							{ return (_inputHandler); }
			const InputHandler* inputHandler () const
							{ return (_inputHandler); }

			/** The could be redefined by type of behaviour. \n
				A default implementation is given: Not to do anything. */
			virtual void beforeTreatingEvents () { }
			virtual void afterTreatingEvents () { }

			/** They should be redefined in every game... \n
				By default nothing is done... */
			virtual void onJoystickAxisMoveEvent (JoystickMovementEventData* dt) { }
			virtual void onJoystickButtonEvent (JoystickButtonEventData* dt) { }
			virtual void onKeyboardEvent (KeyBoardEventData* dt) { }
			virtual void onMouseMovementEvent (MouseMovementEventData* dt) { }
			virtual void onMouseButtonEvent (MouseButtonEventData* dt) { }
			virtual void onKeyboardStatus (const std::vector <bool>& kS) { }
			virtual void onJoystickStatus (const std::vector <JoystickStateData>& jS) { }
			virtual void onMouseStatus (const MouseStateData& mS) { }

			protected:
			Game* _game;
			InputHandler* _inputHandler;
		};

		/** And standard basic behavior reading the keyboard when a key is released
			and controlling movement of the joystick in the axis x and y. 
			The Standard behaviour redefines only the behaviour for key pressed and joystick movement.
			This behaviour allows to select between the keyboard and the joystick as controlling method. */
		class StandardBehavior : public Behaviour
		{
			public:
			StandardBehavior ()
				: Behaviour (),
				 _joystick (false),
				 _joystickSensibility (4), // By default is 4, the lesser the more sensible the joystick is...
				 _joystickPower (4),
				 _keys (), // The keys used in the game (for different purposes)
				 _lastKey (-1),
				 _mX (), _mY (),
				 _jbForce (), _jbStatus ()
							{ }

			// To control the joystick...
			/** To know whether the joystick is or not active. */
			bool isJoystickActive () const
									{ return (_joystick); }
			/** Activate or desactivate the joystick */
			void activateJoystick (bool st)
									{ _joystick = st; }
			/** To know the joystick sensibility. */
			int joystickSensibility () const
									{ return (_joystickSensibility); }
			/** To changel the sensibility. Has to be positive. */
			void setJoystickSensibility (int s)
									{ assert (s > 0);
									  _joystickSensibility = s; }
			/** To know the maximum joystick power. */
			int joystickPower () const
									{ return (_joystickPower); }
			/** To set the maximum joystick power. */
			void setJoystickPower (int jP)
								{ assert (jP > 0);
								  _joystickPower = jP; }
			/** To reset the control of the joystick movement. */
			void resetJoystickMovement ()
									{ _mX.clear (); _mY.clear (); }
			/** To know the joystick current orientation. */
			Vector joystickOrientation (int nJ) const;

			// To control the keys
			/** To know what keys are used. */
			std::vector <int> keys () const
									{ return (_keys); }
			/** To change them. */
			void setKeys (const std::vector <int>& k)
									{ _keys = k; }

			/** @see parent. */
			virtual void beforeTreatingEvents ();

			/** @see parent. */
			virtual void onJoystickAxisMoveEvent (JoystickMovementEventData* dt); // The method manageJoystickMovementOnState is called
			virtual void onJoystickButtonEvent (JoystickButtonEventData* dt); // The method manageJoystickButtonOnState is called
			virtual void onKeyboardEvent (KeyBoardEventData* dt); // The method manageKeyOnState is called

			/** What to do with a key depending on the state of the game. \n
				st is the current game status, kP the key released, 
				nJ the number of joystick, dr the direction of the movement,
				nB the number of button released, and bF something reflecting the miliseconds the button was pressed
				before being released (to represent things like intensity i.e.).\n 
				They have to be redefined later. */
			virtual void manageKeyOnState (GameState* st, int kP) = 0;
			virtual void manageJoystickMovementOnState (GameState* st, int nJ, const Vector& dr) = 0;
			virtual void manageJoystickButtonOnState (GameState* st, int nJ, bool p, int nB, int bF) = 0; 
			// p indicates pressed (true) or released...

			protected:
			/** Is the joystick the main element to control the main character? */
			bool _joystick;
			/** The sensibility to detect movements. */
			int _joystickSensibility;
			/** Max power of the joystick. Has also to be a positive value. */
			int _joystickPower;
			/** The keys used during the game for many different purposes. 
				The intepretation depends on the specific game. */
			std::vector <int> _keys;

			// Implementation
			/** Last key pressed to control if it has been released. */
			int _lastKey;
			/** The directions of the last movement in the joystick.
				This is needed because the movement of the joystick is not just only one event.
				If the joystick is in a diagonal two or three events can be generated. 
				an it is necessary to acumulate the position to better control. */
			std::map <int, int> _mX, _mY;
			/** The force used to press a button, per joystick! */
			std::map <int, std::map <int, int>> _jbForce;
			/** The state (ont / off) of the buttons, per joystick!. */
			std::map <int, std::map <int, bool>> _jbStatus;
		};

		InputHandler (Game* g, Behaviour* bhv); 
		virtual ~InputHandler () 
							{ delete (_behaviour); }

		/** To get the game. */
		Game* game ()
							{ return (_game); }
		/** To get the behaviour. */
		Behaviour* behaviour () 
							{ return (_behaviour); }

		/** This method is called before the input handler is used. \n
			By default there is nothing to be executed. It will depend on the final external library used. */
		virtual void initialize ()
							{ }
		/** This method is called once the input handler is no longer needed. \n
			Usually, the method is called when the game finishes (in the finalize method of the 
			the GameImplementation class). \n
			By default the pending events in the potential queu are deleted. */
		virtual void finalize ()
							{ clearPendingEvents (); }
		/** This method is called before starting to read the events. \n
			Who is invoking it is the handleEvents method defined in the GameImplementation class. \n
			By default the equivalent method in the implementation in invoked. */
		virtual void beforeTreatingEvents ()
							{ _behaviour -> beforeTreatingEvents (); }
		/** The events have to be read. This method is in charge of it.
			It has to be redefined for the specific library managing the input handler. */
		virtual Events readEvents () = 0;
		/** Same method than begoreReadingEvents, but called after treat all the elements
			read with the input handler. */
		virtual void afterTreatingEvents ()
							{ _behaviour -> afterTreatingEvents (); }
		/** It could be neccessary to clean up the pending events of this input handler.
			This method is call always when the input handler finishes. */
		virtual void clearPendingEvents () = 0;

		// What to do when the events are read....By defauly nothing.
		/** When the joystick is moved. 
			By default a notification is thrown only. */
		virtual void onJoystickAxisMoveEvent (JoystickMovementEventData* dt)
							{ _behaviour -> onJoystickAxisMoveEvent (dt); // Uses the behaviour...
							  notify (Event (__QGAMES_MOVEJOYSTICKEVENT__, dt)); }
		/** When the joystick button is pressed.
			By default a notification is thrown only. */
		virtual void onJoystickButtonEvent (JoystickButtonEventData* dt)
							{ _behaviour -> onJoystickButtonEvent (dt);
							  notify (Event (__QGAMES_BUTTONJOYSTICKEVENT__, dt)); }
		/** When the keyboard is used. 
			By default a notification is thrown only. */
		virtual void onKeyboardEvent (KeyBoardEventData* dt)
							{ _behaviour -> onKeyboardEvent (dt);
							  notify (Event (__QGAMES_KEYBOARDEVENT__, dt)); }
		/** When the mouse is moved. 
			By default a notification is thrown only. */
		virtual void onMouseMovementEvent (MouseMovementEventData* dt)
							{ _behaviour -> onMouseMovementEvent (dt);
							  notify (Event (__QGAMES_MOUSEMOVEMENTEVENT__, dt)); }
		/** When the mouse button is pressed.
			By default a notification is thrown only. */
		virtual void onMouseButtonEvent (MouseButtonEventData* dt)
							{ _behaviour -> onMouseButtonEvent (dt);
							  notify (Event (__QGAMES_MOUSEBUTTONEVENT__, dt)); }
		/** Sometimes it is possible to read the full status of the keyboard.
			The method receives a full keyboard vector with the status of every key. 
			The implementation will depend on the library used. By default it does nothing. */
		virtual void onKeyboardStatus (const std::vector <bool>& kS)
							{ _behaviour -> onKeyboardStatus (kS); }
		/** Somethimes is possible to read the full status of the joystick.
			The method receives the full status of all joysticks. */
		virtual void onJoystickStatus (const std::vector <JoystickStateData>& jS)
							{ _behaviour -> onJoystickStatus (jS); }
		/** Sometimes is possible to read the ful status of the mouse.
			The implementation will depend on the type of graphical library. */
		virtual void onMouseStatus (const MouseStateData& mS)
							{ _behaviour -> onMouseStatus (mS); }

		// specific Information about mouse and joystick...
		/** Where is the mouse?. By default nowhere. */
		virtual MouseMovementEventData whereIsMouse ()
							{ MouseMovementEventData result; 
							  result._x = result._y = 0; 
							  return (result); }
		/** Which is the maximum position in an axis?. By default INT_MAX. 
			It doesn't depend on the axis. */
		virtual int maxPosJoystick (int nA)
							{ return (std::numeric_limits <int>::max ()); }
		/** Which is the minimum position in an axis?. By default INT_MIN. 
			It doesn't depend on the axis. */
		virtual int minPosJoystick (int nA)
							{ return (std::numeric_limits <int>::min ()); }

		// To get the status of the mouse and the joystick...
		/** The state of the mouse. By default nothing or not implemented. */
		virtual MouseStateData mouseState () = 0;
		/** The state of the joystick. 
			Receives the number of the joystick to test as parameter. 
			By default nothing or not implemeted. */
		virtual JoystickStateData joystickState (unsigned int nJ) = 0;

		/** An internal method that can be used to translate the keys from one system to the 
			symbols used by QGAMES. It can be overloaded later, but the default behaviour is just
			to return the same key. */
		virtual KeyCode translateKeyCode (int kC);
		/** An internal method that can be used to translate the buttons from one system
			to the values used in QGAMES. It can be overloaded later, but the default behaviour
			is just to return the same value. */
		virtual MouseButtonCode translateMouseButtonCode (int bC);

		/** To treat keyboard's and joystick's status. */
		/** In the case of the keyboard to get the status of the full keyboard is 
			something the graphics library should allow. By default nothing is possible. 
			This method sould read the status of the keyboard, put it into a vector of information and 
			call the method onKeyboardStatus. */
		virtual void treatKeyboardStatus () { }
		/** This method does the same but with the joystick.
			To read the status of the different joysticks the method joysticState is used, then
			the method onJoystickState is invoked. The graphic library is needed to read the number of joysticks. */
		virtual void treatJoystickStatus () { }
		/** This method does the same but for the mouse.
			In this case, because there is no more than a mouse in the system maximum, 
			the method mouseState is enough by default. */
		virtual void treatMouseStatus ()
							{ onMouseStatus (mouseState ()); }

		protected:
		Game* _game;
		Behaviour* _behaviour;
	};
}

#endif
  
// End of the file
/*@}*/
