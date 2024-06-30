#include <Common/widgetsystem.hpp>
#include <Common/game.hpp>
#include <iostream>

QGAMES::GUISystem* QGAMES::GUISystem::_theSystem = NULL;

// ---
QGAMES::GUISystem::GUISystem (QGAMES::CompositeWidget* mW, const QGAMES::GUISystem::ControlKeys& k)
	: QGAMES::Notifier (), 
	  QGAMES:: Observer (),
	  _mainWidget (mW), // All elements in the gui system has to depend on this one...
	  _focusedWidget (NULL), // The widget having now the focus. None at the beginning!
	  _controlKeys (k), // To manage the interface...
	  _inputHandler (NULL),
	  _externalEngine (NULL),
	  _externalData (NULL),
	  _toNotify (QGAMES::NoEvent ()), // No event 
	  _iterator (NULL)
{
	// Main widget is mandatory...(and it has to be a composite on)
	assert (_mainWidget); 

	// There can only be one...
	// If a system exists previously, then it is first deleted
	if (_theSystem != NULL)
		delete (_theSystem);
	_theSystem = this;

	// Then observe the main widget too...
	observe (_mainWidget);

	// Creates an initil iterator...
	// Take care becuase this iterator becomes unseful if the main widget s not complete at this point
	// which is the case, when the syste has been built from the gui system builder.
	createIterator ();

	// Starts to observe the input handler, because the changes in the gui system
	// will depend on the events there (mainly on the keyboard events and on the mouse events...
	_inputHandler = game () -> inputHandler ();
	if (_inputHandler != NULL) // It could be null, but it is strange...just in case
		observe (_inputHandler);
}

// ---
QGAMES::GUISystem::~GUISystem ()
{
	delete (_mainWidget);
	_mainWidget = NULL;
	unObserve (game () -> inputHandler ());
	_theSystem = NULL; // No system so far...

	if (_externalEngine) _externalEngine -> whenDeleteGUISystem (this);
	delete (_externalData);
}

// ---
QGAMES::Game* QGAMES::GUISystem::game ()
{
	return (QGAMES::Game::game ());
}

// ---
void QGAMES::GUISystem::initialize ()
{ 
	createIterator ();

	moveFocusToFirstWidget ();

	if (_focusedWidget)
		_focusedWidget -> setFocus (true); 

	if (_externalEngine) _externalEngine -> whenInitialize (this);
}

// ---
void QGAMES::GUISystem::inEveryLoop ()
{
	_mainWidget -> inEveryLoop ();
	if (_toNotify != QGAMES::Event::_noEvent)
		notify (_toNotify); // Just if something was pending...
	_toNotify = QGAMES::Event::_noEvent; // ...after this nothing else to say...
	// The reason to do so is because a direct notification from the processEvent method cab cause something
	// in the widget itseld (ie being deleted), an then the system can crashes)
	// It is better to separate both moments...

	if (_externalEngine) _externalEngine -> whenInEveryLoop (this);
}

// ---
void QGAMES::GUISystem::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (_mainWidget -> isVisible ())
		_mainWidget -> drawOn (s, p); // Draws the main element...

	if (_externalEngine) _externalEngine -> whenDrawOn (this, s, p);
}

// ---
void QGAMES::GUISystem::processEvent (const QGAMES::Event& evnt)
{
	switch (evnt.code ())
	{
		case __QGAMES_KEYBOARDEVENT__:
			{
				QGAMES::KeyBoardEventData* dt = (QGAMES::KeyBoardEventData*) (evnt.data ());
				if (!dt -> _on) // Release the key...
				{
					if (dt -> _internalCode == _controlKeys._nextBlockKey)
						moveFocusToNextWidget ();
					else if (dt -> _internalCode == _controlKeys._previousBlockKey)
						moveFocusToPreviousWidget ();
					else if (dt -> _internalCode == _controlKeys._selectKey)
					{
						if (_focusedWidget != NULL)
							_focusedWidget -> whenSelected ();
					}
					else
						_focusedWidget -> whenOnKey (dt -> _internalCode, (char) dt -> _unicode);
						// Otherwise the key is injected into the widget...
				}
			}

			break;

		case __QGAMES_MOUSEMOVEMENTEVENT__:
			{
				QGAMES::MouseStateData msSt = _inputHandler -> mouseState ();
				moveFocusToWidgetAt (msSt._X._rel, msSt._Y._rel);
			}

			break;

		case __QGAMES_MOUSEBUTTONEVENT__:
			{
				QGAMES::MouseButtonEventData* dt = (QGAMES::MouseButtonEventData*) (evnt.data ());
				if (!dt ->_on && dt -> _internalCode == _controlKeys._selectButton)
				{
					if (_focusedWidget != NULL)
						_focusedWidget -> whenSelected ();
				}
			}

			break;

		default:
			notify (evnt); // If the event is not know then is notified up...just is case!
			break;
	}
}

// ---
void QGAMES::GUISystem::moveFocusToFirstWidget ()
{
	if (!_iterator)
		createIterator ();

	_focusedWidget = _iterator -> first ();
	while (_focusedWidget != NULL && !_focusedWidget -> isActive ())
		moveFocusToNextWidget ();
}

// ---
void QGAMES::GUISystem::moveFocusToNextWidget ()
{
	if (!_iterator)
	{
		moveFocusToFirstWidget (); // EEnough...
		return;
	}

	bool found = false;
	QGAMES::Widget* oW = _focusedWidget;
	do 
	{
		_focusedWidget = _iterator -> next (); // Move to next...
		if (_focusedWidget == NULL) 
			_focusedWidget = _iterator -> first (); // It there is no more, then first again!
		if (_focusedWidget -> isActive ()) // Only one active can be selected!
			found = true;
	} while (!found && _focusedWidget != oW);

	if (found)
	{
		_focusedWidget -> setFocus (true); // Set the focus, if there is something to focus!
		if (oW != NULL) 
			oW -> setFocus (false); // The old one, if it exists, loses the focus...
	}
}

// ---
void QGAMES::GUISystem::moveFocusToPreviousWidget ()
{
	if (!_iterator)
	{
		moveFocusToFirstWidget (); //Enough...
		return;
	}

	bool found = false;
	QGAMES::Widget* oW = _focusedWidget;
	QGAMES::CompositeWidget::Iterator* tmpIterator = 
		new QGAMES::CompositeWidget::Iterator (_mainWidget);
	QGAMES::Widget* lW = NULL;
	QGAMES::Widget* nW = tmpIterator -> first ();

	while (nW != NULL && nW != _focusedWidget)
	{
		// Before iterating preserves which one was the last to select it later...
		lW = (nW != NULL && nW -> isActive ()) ? nW : lW; 
		nW = tmpIterator -> next ();
	}

	if (lW == NULL)
	{
		nW = tmpIterator -> next ();
		while (nW != NULL)
		{
			// Before iterating preserves which one was the last to select it later...
			lW = (nW != NULL && nW -> isActive ()) ? nW : lW; 
			nW = tmpIterator -> next ();
		}

		if (lW != NULL)
			found = true;
	}
	else
		found = true;

	if (found)
	{
		(_focusedWidget = lW) -> setFocus (true);
		if (oW != NULL) oW -> setFocus (false);
		
		// The iterator has to be where the focus is now...
		_iterator -> moveTo (_focusedWidget);
	}

	// Deletes the temporary iterator...
	delete (tmpIterator);
}

// ---
void QGAMES::GUISystem::moveFocusToWidgetAt (int px, int py)
{
	QGAMES::Widget* oW = _focusedWidget;
	
	QGAMES::Widget* nW = _mainWidget -> widgetAt (px, py);
	if (nW == NULL || nW == oW || !nW -> isActive ())
		return; // If there is no widget inside or the selected one is the same or it is not active...nothing has to change!

	_focusedWidget = nW;
	_focusedWidget -> setFocus (true);
	if (oW != NULL)
		oW -> setFocus (false);

	// The iterator has to be in the same position that the focus is now...
	_iterator -> moveTo (_focusedWidget);
}

// ---
void QGAMES::GUISystem::createIterator ()
{
	delete (_iterator); // Destroys any previous one...
	_iterator = new QGAMES::CompositeWidget::Iterator (_mainWidget);
}
