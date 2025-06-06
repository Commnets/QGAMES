/** \ingroup Battleship */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: bsinputhandler.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/11/2019 \n
 *	Description: Define a base for the input handler in a battleship like game.
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_INPUTHANDLER__
#define __BATTLESHIP_INPUTHANDLER__

#include <BattleshipLike/bsdefinitions.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace BATTLESHIP
{
	class InputHandlerBehaviour : public QGAMES::InputHandler::StandardBehavior
	{
		public:
		/** Public structure used just to comunicate the impulse when it is needed. 
			An object of this type is created at construction time.
			The same object is reused in any communication 
			considering that to process events is always sequential in QGAMES games. */
		struct NotificationData
		{
			NotificationData ()
				: _impulse (0), // To jump and to fire...
				  _upOn (false), _downOn (false), _rightOn (false), _leftOn (false)
							{ }

			void clear () 
							{ _impulse = 0; _downOn = _upOn = _leftOn = _rightOn = false; }

			int _impulse;
			bool _upOn;
			bool _downOn;
			bool _leftOn;
			bool _rightOn;
		};

		InputHandlerBehaviour ();

		~InputHandlerBehaviour ()
							{ delete (_notificationData); }

		/** @see parent. */
		virtual void onKeyboardStatus (const std::vector <bool>& kS);
		virtual void manageKeyOnState (QGAMES::GameState* st, int kP);
		virtual void manageJoystickMovementOnState (QGAMES::GameState* st, int nJ, const QGAMES::Vector& dr);
		virtual void manageJoystickButtonOnState (QGAMES::GameState* st, int nJ, bool p, int nB, int bF);

		protected:
		// Implementation
		NotificationData* _notificationData;
		int _force; // When shooting...
		int _impulse; // When jumping...(used to make spacial effects in thespace)
		bool _downMovementOn; // Just for joysticj implementation (the next three too)
		bool _upMovementOn;
		bool _rightMovementOn;
		bool _leftMovementOn;
	};
}

#endif
  
// End of the file
/*@}*/
