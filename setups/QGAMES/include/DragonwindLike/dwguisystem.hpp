/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: dwguisystem.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 04/01/2019 \n
 *	Description: Defines the specific things related with GUI system of any Dragonwin like game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_GUISYSTEM__
#define __DRAGONWIND_GUISYSTEM__

#include <DragonwindLike/dwdefinitions.hpp>
#include <Platform/ptinclude.hpp>

namespace DRAGONWIND
{
	/** Defines a new GUI System. */
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

	/** Defines a decorator to show the element when focused. */
	class WhenFocusDecorator : public QGAMES::Widget::ComplexDecorator
	{
		public:
		WhenFocusDecorator ();

		/** @see parent. */
		bool isBefore () { return (true); }
	};

	/** Defines a widget to control the menus. */
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