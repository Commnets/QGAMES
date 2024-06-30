/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: bsguisystem.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/11/2019 \n
 *	Description: Defines the specific things related with GUI system of any Battleship like game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_GUISYSTEM__
#define __BATTLESHIP_GUISYSTEM__

#include <BattleshipLike/bsdefinitions.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace BATTLESHIP
{
	/** Defines a new GUI System for the battleship game. \n
		This GUI system is used mainly for the options when playing. \n
		The options are drawn in a rectangle. */
	class GUISystem : public QGAMES::GUISystem
	{
		public:
		GUISystem (QGAMES::CompositeWidget* mW, 
			const QGAMES::GUISystem::ControlKeys& k = QGAMES::GUISystem::ControlKeys ())
			: QGAMES::GUISystem (mW, k)
							{ }

		/** @see parent. */
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_cero);
	};

	/** Defines a decorator to show the element when focused. \n
		The decorator draws a blinking rectangle around the option. */
	class WhenFocusDecorator : public QGAMES::Widget::ComplexDecorator
	{
		public:
		WhenFocusDecorator ();

		/** @see parent. */
		bool isBefore () { return (true); }
	};

	/** Defines a widget to control the menus. \n
		This control put on and take off the decorator as a widget gets the focus. */
	class ControlWidget : public QGAMES::SelectionOptionsGameState::MenuControlWidget
	{
		public:
		ControlWidget (int id, Decorator* d = NULL, 
			const QGAMES::ListOfWidgets& wdts = QGAMES::ListOfWidgets (), int wF = -1)
			: QGAMES::SelectionOptionsGameState::MenuControlWidget (id, d, wdts, wF)
							{ }

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& e);
	};
}

#endif
  
// End of the file
/*@}*/