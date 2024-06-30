/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: widgetsystem.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 28/12/2018\n
 *	Description: Defines a class to represent a gui system. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_WIDGETSYSTEM__
#define __QGAMES_WIDGETSYSTEM__

#include <Common/notifierobserver.hpp>
#include <Common/inputhandler.hpp>
#include <Common/widget.hpp>

namespace QGAMES
{
	/** A widget system. \n
		There can only be one widget system alive at the same time,
		but it can be created and deleted wherever is needed. \n
		The system defines also a reference to a potential external engine. */
	class GUISystem : public Notifier, public Observer
	{
		public:
		/** What the potential external engine could need. */
		class ExternalData
		{
			public:
			virtual ~ExternalData ()
				{ } // Just in case...
		};

		/** Every widget system needs to have a set ok keys and mouse buttons defined to control it. */
		class ControlKeys
		{
			public:
			ControlKeys ()
				: _nextBlockKey (KeyCode::QGAMES_DOWN), _previousBlockKey (KeyCode::QGAMES_UP),
				  _selectKey (KeyCode::QGAMES_RETURN),
				  _selectButton (MouseButtonCode::QGAMES_BUTTONLEFT)
							{ }
			ControlKeys (KeyCode nBK, KeyCode pBK, KeyCode sK, MouseButtonCode sB)
				: _nextBlockKey (nBK), _previousBlockKey (pBK),
				  _selectKey (sK), _selectButton (sB)
							{ }
			
			public:
			KeyCode _nextBlockKey, _previousBlockKey, _selectKey;
			MouseButtonCode _selectButton;
		};

		GUISystem (CompositeWidget* mW, const ControlKeys& k = ControlKeys ());
		~GUISystem ();

		/** To set the external engine, if any. */
		_ExEngine* externalEngine () 
							{ return (_externalEngine); }
		void setExternalEngine (_ExEngine* egn)
							{ _externalEngine = egn; }
		/** Set additional data, if any. */
		ExternalData* externalData ()
							{ return (_externalData); }
		void setExternalData (ExternalData* eD)
							{ delete (_externalData); _externalData = eD; }

		/** To change the control keys. */
		const ControlKeys& controlKeys () const
							{ return (_controlKeys); }
		void setControlKeys (const ControlKeys& cK)
							{ _controlKeys = cK; }

		static GUISystem* system ()
							{ return (_theSystem); }
		
		Game* game ();

		/** To get a reference to the main widget. */
		Widget* mainWidget ()
							{ return (_mainWidget); }
		/** To get the widget with the curent focus. */
		Widget* focusedWidget ()
							{ return (_focusedWidget); }

		virtual void initialize ();
		virtual void inEveryLoop ();
		virtual void drawOn (Screen* s, const Position& p = Position::_cero);

		/** see parent. */
		virtual void processEvent (const Event& evnt);

		protected:
		// Internal methods to simplify the code...
		// All these methods take into account whether the selected widget is visible
		void moveFocusToFirstWidget ();
		void moveFocusToNextWidget ();
		void moveFocusToPreviousWidget ();
		void moveFocusToWidgetAt (int px, int py);

		private:
		void createIterator ();

		protected:
		CompositeWidget* _mainWidget;
		Widget* _focusedWidget;
		ControlKeys _controlKeys;
		InputHandler* _inputHandler;
		_ExEngine* _externalEngine;
		ExternalData* _externalData;

		// Implementation
		/** To notify in the next loop, when something has happened. */
		Event _toNotify;
		CompositeWidget::Iterator* _iterator;

		static GUISystem* _theSystem;
	};
};

#endif
  
// End of the file
/*@}*/