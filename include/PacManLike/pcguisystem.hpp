/** \ingroup PACMAN */
/*@{*/

/**	
 *	@file	
 *	File: pcguisystem.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: To manipulate the gui system of the PACMAN game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMAN_GUISYSTEM__
#define __PACMAN_GUISYSTEM__

#include <PacManLike/pcdefs.hpp>

#include <advancedArcade/adinclude.hpp>

namespace PACMAN
{
	/** Defines a new GUI System for the PACMAN game. \n
		This GUI system is used mainly for the options when playing. \n
		The options are drawn in a rectangle. */
	class GUISystem final : public QGAMES::GUISystem
	{
		public:
		GUISystem (QGAMES::CompositeWidget* mW, 
			const QGAMES::GUISystem::ControlKeys& k = QGAMES::GUISystem::ControlKeys ())
			: QGAMES::GUISystem (mW, k)
							{ }

		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_cero) override;
	};

	/** Defines a decorator to show the element when focused. \n
		The decorator draws a blinking rectangle around the option. */
	class WhenFocusDecorator final : public QGAMES::Widget::ComplexDecorator
	{
		public:
		WhenFocusDecorator ();

		virtual bool isBefore () override 
							{ return (true); }
	};

	/** Defines a widget to control the menus. \n
		This control put on and take off the decorator as a widget gets the focus. */
	class ControlWidget final : public QGAMES::SelectionOptionsGameState::MenuControlWidget
	{
		public:
		ControlWidget (int id, Decorator* d = NULL, 
			const QGAMES::ListOfWidgets& wdts = QGAMES::ListOfWidgets (), int wF = -1)
			: QGAMES::SelectionOptionsGameState::MenuControlWidget (id, d, wdts, wF)
							{ }

		virtual void processEvent (const QGAMES::Event& e) override;
	};
}

#endif
  
// End of the file
/*@}*/