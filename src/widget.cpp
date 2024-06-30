#include <Common/widget.hpp>
#include <Common/rectangle.hpp>
#include <Common/screen.hpp>
#include <Common/event.hpp>
#include <Common/form.hpp>
#include <Common/game.hpp>
#include <Common/genalgorithms.hpp>
#include <iostream>

// ---
QGAMES::Widget::ComplexDecorator::~ComplexDecorator ()
{
	for (QGAMES::Widget::ListOfDecorators::const_iterator i = _decorators.begin (); 
			i != _decorators.end (); i++)
		delete ((*i));
}

// ---
void QGAMES::Widget::ComplexDecorator::decorate (Screen* scr, const Position& p, Widget* wdt)
{
	assert (scr);
	assert (wdt);

	for (QGAMES::Widget::ListOfDecorators::const_iterator i = _decorators.begin (); 
			i != _decorators.end (); i++)
		(*i) -> decorate (scr, p, wdt);
}

// ---
void QGAMES::Widget::ComplexDecorator::addDecorator (QGAMES::Widget::Decorator* d)
{
	assert (d);

	bool exists = false;
	for (QGAMES::Widget::ListOfDecorators::const_iterator i = _decorators.begin (); 
			i != _decorators.end () && !exists; i++)
		if ((*i) == d) exists = true;
	if (!exists)
		_decorators.push_back (d);
}

// ---
void QGAMES::Widget::RoundingDecorator::decorate (QGAMES::Screen* scr, 
	const QGAMES::Position& p, QGAMES::Widget* wdt)
{
	assert (scr);

	QGAMES::Vector sep = QGAMES::Vector (__BD _separation, __BD _separation, __BD 0);

	QGAMES::Vector vAdd;
	for (int i = 0; i < _thickness; i++)
	{
		vAdd = QGAMES::Vector (__BD i, __BD i, __BD 0);
		scr -> drawRectangle (QGAMES::Rectangle (p + wdt -> position () + vAdd - sep, 
			p + wdt -> position () + QGAMES::Vector (__BD wdt -> width (), __BD wdt -> height (), __BD 0) - vAdd + (3 * sep)), 
				_colorBoxLimit, false);
	}

	scr -> drawRectangle (QGAMES::Rectangle (p + wdt -> position () + vAdd - sep, 
		p + wdt -> position () + QGAMES::Vector (__BD wdt -> width (), __BD wdt -> height (), __BD 0) - vAdd + (3 * sep)), 
			_colorInside, true); 
}

// ---
void QGAMES::Widget::RoundingDecorator::initializeFrom (const QGAMES::Widget::Properties& prp)
{
	std::string cR = "255,255,255,255"; // Pure white by default...
	if (prp.find (__QGAMES_ROUNDDECATTRCOLORROUND__) != prp.end ())
		cR = (*(prp.find (__QGAMES_ROUNDDECATTRCOLORROUND__))).second;
	std::vector <int> cRV = QGAMES::getElementsFromAsInt (cR, ',',4);
	_colorBoxLimit = QGAMES::Color (cRV [0], cRV [1], cRV [2], cRV [3]);

	int tk = 3;
	if (prp.find (__QGAMES_ROUNDDECATTRTHICKNESS__) != prp.end ())
		tk = atoi ((*(prp.find (__QGAMES_ROUNDDECATTRTHICKNESS__))).second.c_str ());
	_thickness = tk;

	std::string cI = "255,255,255,255"; // Pure white by default...
	if (prp.find (__QGAMES_ROUNDDECATTRCOLORINSIDE__) != prp.end ())
		cI = (*(prp.find (__QGAMES_ROUNDDECATTRCOLORINSIDE__))).second;
	std::vector <int> cIV = QGAMES::getElementsFromAsInt (cI, ',',4);
	_colorInside = QGAMES::Color (cIV [0], cIV [1], cIV [2], cIV [3]);

	int sp = 5;
	if (prp.find (__QGAMES_ROUNDDECATTRSEPARATION__) != prp.end ())
		sp = atoi ((*(prp.find (__QGAMES_ROUNDDECATTRSEPARATION__))).second.c_str ());
	_separation = sp;
}

// ---
void QGAMES::Widget::BlinkingBackgroundDecorator::decorate (QGAMES::Screen* scr, 
	const QGAMES::Position& p, QGAMES::Widget* wdt)
{
	assert (scr);

	QGAMES::bdata val = __BD _currentFade / __BD 255;
	QGAMES::Color col ((int) (_backgroundColor.red () * val), 
					   (int) (_backgroundColor.green () * val), 
					   (int) (_backgroundColor.blue () * val), 255);
	QGAMES::Vector sep = QGAMES::Vector (__BD _separation, __BD _separation, __BD 0);
	scr -> drawRectangle (QGAMES::Rectangle (p + wdt -> position () - sep, 
		p + wdt -> position () + QGAMES::Vector (__BD wdt -> width (), __BD wdt -> height (), __BD 0) + (3 * sep)), 
			col, true);

	// Move the fade any time the decorator is executed...
	_currentFade += (_counter * _fadeGrowth);
	if (_currentFade > _finalFade) 
		{ _currentFade = _finalFade; _counter = -_counter; }
	if (_currentFade < _initialFade) 
		{ _currentFade = _initialFade; _counter = -_counter; }
}

// ---
void QGAMES::Widget::BlinkingBackgroundDecorator::initializeFrom (const QGAMES::Widget::Properties& prp)
{
	std::string cR = "255,255,255,255"; // Pure white by default...
	if (prp.find (__QGAMES_BLINKINGBKDECATTRBACKCOLOR__) != prp.end ())
		cR = (*(prp.find (__QGAMES_BLINKINGBKDECATTRBACKCOLOR__))).second;
	std::vector <int> cRV = QGAMES::getElementsFromAsInt (cR, ',',4);
	_backgroundColor = QGAMES::Color (cRV [0], cRV [1], cRV [2], cRV [3]);

	int s = 0;// No seperation by default...
	if (prp.find (__QGAMES_BLINKINGBKDECATTRSEPARATION__) != prp.end ())
		s = atoi ((*(prp.find (__QGAMES_BLINKINGBKDECATTRSEPARATION__))).second.c_str ());
	_separation= s;

	int iF = 0; // From 0 to...
	if (prp.find (__QGAMES_BLINKINGBKDECATTRINIFADE__) != prp.end ())
		iF = atoi ((*(prp.find (__QGAMES_BLINKINGBKDECATTRINIFADE__))).second.c_str ());
	_initialFade = iF;

	int fF = 255; // 255 by default...
	if (prp.find (__QGAMES_BLINKINGBKDECATTRFINALFADE__) != prp.end ())
		fF = atoi ((*(prp.find (__QGAMES_BLINKINGBKDECATTRFINALFADE__))).second.c_str ());
	_finalFade = fF;

	int fG = 1; // ...and going 1 by one (it is to slow but could be the default behaviour)
	if (prp.find (__QGAMES_BLINKINGBKDECATTRFADEGROWTH__) != prp.end ())
		fF = atoi ((*(prp.find (__QGAMES_BLINKINGBKDECATTRFADEGROWTH__))).second.c_str ());
	_fadeGrowth = fG;
}

// ---
QGAMES::Widget::Decorator* QGAMES::Widget::ComplexDecorator::clone () const
{
	QGAMES::Widget::ListOfDecorators lD;
	QGAMES::Widget::ComplexDecorator* result = new QGAMES::Widget::ComplexDecorator ();
	for (QGAMES::Widget::ListOfDecorators::const_iterator i = _decorators.begin (); 
			i != _decorators.end (); i++)
		lD.push_back ((*i) -> clone ());
	result -> _decorators = lD;
	return (result);
}

// ---
void QGAMES::Widget::setFocus (bool f)
{
	bool oF = _hasFocus;

	// If the situation with the widget has changed...
	if ((_hasFocus = f) != oF) 
	{
		if (_parent != NULL)
		{
			// If the parent is a composite object te intenal 
			// widget with the focus has also to be set o unset...
			if (dynamic_cast <QGAMES::CompositeWidget*> (_parent) != NULL)
			{
				// If the parent's focused widget is not this, and this is getting focus...
				if (((QGAMES::CompositeWidget*) _parent) -> focusedWidget () != this && _hasFocus)
				{
					// ...Move the focus to this one also at parent's level..
					_parent -> setFocusedWidget (this);
					if (!_parent -> hasFocus ())
						_parent -> setFocus (true); // The parent has also to have the focus...
													// if it doesn't have it already. Doing so, 
													// whenGetFocusd method for this widget is executed also...
					else
						whenGetFocus (); // The parent has already the focus, but this widget hasn't so...
				}
				// If the parent's focused widget is this, but this one is lossing the focus...
				else if (((QGAMES::CompositeWidget*) _parent) -> focusedWidget () == this && !_hasFocus)
				{
					// Move the focus to nothing...event at parent's level...
					_parent -> setFocusedWidget (NULL); 
					_parent -> setFocus (false); // The parent also loses the focus...
					// When executing the previous method, the whenLoseFocus for this widget is also executed...
				}
				else
				{
					if (_hasFocus) whenGetFocus ();
					else whenLoseFocus ();
				}
			}
			else
			{
				if (_hasFocus) whenGetFocus ();
				else whenLoseFocus ();
			}
		}
		else
		{
			if (_hasFocus) whenGetFocus ();
			else whenLoseFocus ();
		}

		notify (QGAMES::Event 
			(_hasFocus ? __QGAMES_WIDGETGETFOCUSEVENT__ : __QGAMES_WIDGETLOSESFOCUSEVENT__, this));
	}
}

// ---
QGAMES::Position QGAMES::Widget::position () const
{ 
	return (_position + (_parent ? _parent -> position () : Vector::_cero)); 
}

// ---
QGAMES::Position QGAMES::Widget::absolutePosition () const 
{ 
	return (_position + (_parent ? _parent -> absolutePosition () : Vector::_cero)); 
}

// ---
void QGAMES::Widget::setDecorator (QGAMES::Widget::Decorator* d)
{
	delete (_decorator); // The old one is deleted...
	_decorator = d; // The new one can be NULL
}

// ---
void QGAMES::Widget::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	// To take into account where the screen is, and where the object is really...
	QGAMES::Position fPos = absolutePosition () + s -> position () + p;

	// The decorator if any...
	if (_decorator && _decorator -> isBefore ())
		_decorator -> decorate (s, s -> position () + p, this);

	// Then draws the widget depending on its state
	if (_hasFocus) drawOnWhenHasFocus (s, fPos);
	else drawOnWhenNoHasFocus (s, fPos);

	// Or... the decorator if any...
	if (_decorator && !_decorator -> isBefore ())
		_decorator -> decorate (s, s -> position () + p, this);

	QGAMES::Element::drawOn (s, fPos); // Just in case...(linking with the external engine)
}

// ---
void QGAMES::Widget::whenSelected ()
{
	notify (QGAMES::Event (__QGAMES_WIDGETSELECTEDEVENT__, this));
}

// ---
QGAMES::CompositeWidget::Iterator::Iterator (QGAMES::CompositeWidget* cW)
	: _compositeWidget (cW),
	  _widgets (cW -> widgets ()),
	  _iterator (cW -> widgets ().begin ()),
	  _internalIterator (NULL),
	  _currentWidget (NULL)
{
	assert (_compositeWidget);
}

// ---
QGAMES::Widget* QGAMES::CompositeWidget::Iterator::first ()
{
	QGAMES::Widget* result = NULL;
	_iterator = _widgets.begin ();
	if (_iterator == _widgets.end ())
		result = NULL;
	else
	{
		if (dynamic_cast <QGAMES::CompositeWidget*> (*_iterator))
		{
			delete (_internalIterator);
			_internalIterator = 
				new QGAMES::CompositeWidget::Iterator ((QGAMES::CompositeWidget*)(*_iterator));
			result = _internalIterator -> first ();
		}
		else
			result = (*_iterator);
	}

	// Sets the last
	_currentWidget = result;
	return (result);
}

// ---
QGAMES::Widget* QGAMES::CompositeWidget::Iterator::next ()
{
	QGAMES::Widget* result = NULL;

	bool tryToMoveHereToNext = false;
	if (_internalIterator != NULL)
	{
		if (_internalIterator -> isLast ())
			tryToMoveHereToNext = true;
		else
			result = _internalIterator -> next ();
	}
	else
		tryToMoveHereToNext = true;

	if (tryToMoveHereToNext)
	{
		if (_iterator != _widgets.end ())
			_iterator++;
		if (_iterator == _widgets.end ())
			result = NULL;
		else
		{
			if (dynamic_cast <QGAMES::CompositeWidget*> (*_iterator))
			{
				delete (_internalIterator);
				_internalIterator = 
					new QGAMES::CompositeWidget::Iterator ((QGAMES::CompositeWidget*)(*_iterator));
				result = _internalIterator -> first ();
			}
			else
				result = (*_iterator);
		}
	}

	// Sets the last one...
	_currentWidget = result;
	return (result);
}

// ---
bool QGAMES::CompositeWidget::Iterator::moveTo (QGAMES::Widget* wdt)
{
	QGAMES::Widget* cW = first ();
	while (cW != wdt && !isLast ())
		cW =  next ();
	return (!isLast ()); // Return whether it has or not been found...
}

// ---
QGAMES::CompositeWidget::~CompositeWidget ()
{
	for (QGAMES::ListOfWidgets::const_iterator i = _widgets.begin (); 
			i != _widgets.end (); i++)
		delete ((*i));
	_widgets.clear ();
}

// ---
void QGAMES::CompositeWidget::setFocus (bool f)
{
	// Just when there is no parent at all, or it is not a composite widget
	// Because otherwise the setFocus method at parent class will manage that situation
	// invoking the right methods at parent level...
	if (_parent == NULL || dynamic_cast <QGAMES::CompositeWidget*> (_parent) == NULL)
	{
		// The composite has internal objects
		// When lose the focus none can have it!
		bool oF = _hasFocus;
		if (oF != f) // When change
		{
			if (f) // ...and get focus
			{
				if (_widgetFocused == -1) 
					{ _widgetFocused = 0; correctWidgetFocused (); }
				if (_widgetFocused != -1) _widgets [_widgetFocused] -> setFocus (f);
			}

			// Otherwise the internal variable _widgetFocused will remain...
			// for the next time the composite gains focus.
		}

		// Take into account that _hasFocus attribute is not set
		// Becuase it is set or unset at the parent's method invoked later (just after this comment)
	}

	QGAMES::Widget::setFocus (f);
}

// ---
void QGAMES::CompositeWidget::moveFocusToNextWidget ()
{ 
	int oWF = _widgetFocused;
	_widgetFocused++; correctWidgetFocused ();
	if (oWF != -1)
		_widgets [oWF] -> setFocus (false);
	if (_widgetFocused != -1) 
		_widgets [_widgetFocused] -> setFocus (true);
}

// ---
void QGAMES::CompositeWidget::moveFocusToPreviousWidget ()
{ 
	int oWF = _widgetFocused;
	_widgetFocused--; correctWidgetFocused ();
	if (oWF != -1)
		_widgets [oWF] -> setFocus (false);
	if (_widgetFocused != -1) 
		_widgets [_widgetFocused] -> setFocus (true);
}

// ---
int QGAMES::CompositeWidget::width ()
{
	int result = 0;
	
	int fPosX;
	for (QGAMES::ListOfWidgets::const_iterator i = _widgets.begin (); i != _widgets.end (); i++)
		if ((fPosX = (int) ((*i) -> box ().pos2 ()).posX ()) > result)
			result = fPosX;

	return (result - ((int) absolutePosition ().posX ()) + 1);
}

// ---
int QGAMES::CompositeWidget::height ()
{
	int result = 0;
	
	int fPosY;
	for (QGAMES::ListOfWidgets::const_iterator i = _widgets.begin (); i != _widgets.end (); i++)
		if ((fPosY = (int) ((*i) -> box ().pos2 ()).posY ()) > result)
			result = fPosY;

	return (result - ((int) absolutePosition ().posY ()) + 1);
}

// ---
bool QGAMES::CompositeWidget::addWidget (QGAMES::Widget* wdt)
{
	assert (wdt);
	assert (!existsWidget (wdt -> id ()));
	
	_widgets.push_back (wdt);
	wdt -> setParent (this); //The parent is this one...
	observe (wdt); // Observe this element...

	return (true);
}

// ---
QGAMES::Widget* QGAMES::CompositeWidget::widget (int id)
{
	QGAMES::Widget* result = NULL;
	for (QGAMES::ListOfWidgets::const_iterator i = _widgets.begin (); 
			i != _widgets.end () && result == NULL; i++)
		if ((*i) -> id () == id) result = (*i);
	return (result);
}

// ---
const QGAMES::Widget* QGAMES::CompositeWidget::widget (int id) const
{
	QGAMES::Widget* result = NULL;
	for (QGAMES::ListOfWidgets::const_iterator i = _widgets.begin (); 
			i != _widgets.end () && result == NULL; i++)
		if ((*i) -> id () == id) result = (*i);
	return (result);
}

// ---
QGAMES::Widget* QGAMES::CompositeWidget::firstWidget ()
{
	QGAMES::Widget* result = NULL;
	if (!_widgets.empty ())
		result = (*_widgets.begin ());
	return (result);
}

// ---
QGAMES::Widget* QGAMES::CompositeWidget::lastWidget ()
{
	QGAMES::Widget* result = NULL;
	if (!_widgets.empty ())
		result = (*_widgets.rbegin ());
	return (result);
}

// ---
QGAMES::Widget* QGAMES::CompositeWidget::widgetAt (int px, int py)
{
	QGAMES::Widget* result = NULL;
	for (ListOfWidgets::const_iterator i = _widgets.begin (); 
		i != _widgets.end () && !result; i++)
	{
		if (dynamic_cast <QGAMES::CompositeWidget*> ((*i)))
		{
			result = ((QGAMES::CompositeWidget*) (*i)) -> widgetAt (px, py);
			if (result == NULL)
			{
				if ((*i) -> isInPosition (px, py))
					result = (*i);
			}
		}
		else
		{
			if ((*i) -> isInPosition (px, py))
				result = (*i);
		}
	}

	return (result);
}

// ---
bool QGAMES::CompositeWidget::existsWidget (int id) const
{
	bool result = false;
	for (QGAMES::ListOfWidgets::const_iterator i = _widgets.begin (); 
			i != _widgets.end () && !result; i++)
		if ((*i) -> id () == id) result = true;
	return (result);
}

// ---
void QGAMES::CompositeWidget::removeWidget (int id)
{
	// Looks for the widget...
	int posWidget = -1;
	int counter = 0;
	QGAMES::Widget* wDelete = NULL;
	for (QGAMES::ListOfWidgets::const_iterator i = _widgets.begin (); 
			i != _widgets.end () && posWidget == -1; i++, counter++)
		if ((*i) -> id () == id) { posWidget = counter; wDelete = (*i); }

	if (posWidget == -1)
		return; // The widget doesn't exist...

	// Moves all widgets after the one to delete one step back in the vector...
	counter = posWidget + 1;
	while (counter < (int) _widgets.size ())
		{ _widgets [counter - 1] = _widgets [counter]; counter++; }
	_widgets.pop_back (); // ...then extracts the last one (it will be now at the previous position)
	unObserve (wDelete); // Stops to observe the widget...
	delete (wDelete); // ...and finally deletes the found item!

	// Correct the focus if any...
	if (posWidget == _widgetFocused)
		moveFocusToPreviousWidget ();
}

// ---
QGAMES::Widget* QGAMES::CompositeWidget::focusedWidget ()
{
	return ((_widgetFocused == -1) ? NULL : _widgets [_widgetFocused]);
}

// ---
QGAMES::Widget* QGAMES::CompositeWidget::lastFocusedWidget ()
{
	return ((_widgetFocused == -1) 
		? NULL : (dynamic_cast <QGAMES::CompositeWidget*> (_widgets [_widgetFocused]) 
			? ((QGAMES::CompositeWidget*) _widgets [_widgetFocused]) -> lastFocusedWidget () : _widgets [_widgetFocused]));
}

// ---
void QGAMES::CompositeWidget::setFocusedWidget (Widget* wdt)
{
	if (wdt == NULL)
		_widgetFocused = -1; // None...
	else
		setFocusedWidget (wdt -> id ());
}

// ---
void QGAMES::CompositeWidget::setFocusedWidget (int id)
{
	if (!existsWidget (id))
		_widgetFocused = -1; // None...
	else
	{
		int nFocus = -1;
		int counter = 0;
		for (ListOfWidgets::const_iterator i = _widgets.begin (); 
				i != _widgets.end () && nFocus == -1; i++, counter++)
			if ((*i) -> id () == id)
				nFocus = counter;
	
		// Here the nFocus has to be a valid number, because the widget did exist!...
		// Otherwise there is an error in the code...
		_widgetFocused = nFocus;
	}
}

// ---
void QGAMES::CompositeWidget::whenSelected ()
{
	if (_widgetFocused != -1) 
		_widgets [_widgetFocused] -> whenSelected ();
}

// ---
void QGAMES::CompositeWidget::inEveryLoop ()
{
	for (ListOfWidgets::const_iterator i = _widgets.begin (); i != _widgets.end (); (*i++) -> inEveryLoop ());
	QGAMES::Element::inEveryLoop ();
}

// ---
void QGAMES::CompositeWidget::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	for (ListOfWidgets::const_iterator i = _widgets.begin (); i != _widgets.end (); i++)
		if ((*i) -> isVisible ())
			(*i) -> drawOn (s, p);
	QGAMES::Element::drawOn (s, p);
}

// ---
void QGAMES::CompositeWidget::finalize ()
{
	for (ListOfWidgets::const_iterator i = _widgets.begin (); i != _widgets.end (); (*i++) -> finalize ());
	QGAMES::Element::finalize ();
}

// ---
void QGAMES::CompositeWidget::whenGetFocus ()
{
	if (_widgetFocused != -1) 
		_widgets [_widgetFocused] -> whenGetFocus ();
}

// ---
void QGAMES::CompositeWidget::whenLoseFocus ()
{
	if (_widgetFocused != -1) 
		_widgets [_widgetFocused] -> whenLoseFocus ();
}

// ---
void QGAMES::CompositeWidget::drawOnWhenHasFocus (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	if (_widgetFocused != -1) 
		_widgets [_widgetFocused] -> drawOnWhenHasFocus (s, _position + p);
}

// ---
void QGAMES::CompositeWidget::drawOnWhenNoHasFocus (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	if (_widgetFocused != -1) 
		_widgets [_widgetFocused] -> drawOnWhenHasFocus (s, _position + p);
}

// ---
void QGAMES::CompositeWidget::correctWidgetFocused ()
{
	if (!_widgets.empty ())
	{
		if (_widgetFocused < 0) _widgetFocused = (int) (_widgets.size () - 1);
		else if (_widgetFocused >= (int) _widgets.size ()) _widgetFocused = 0;
	}
	else
		_widgetFocused = -1; // Not valid so far...
}

// ---
QGAMES::LabelWidget::LabelWidget (int id, QGAMES::Form* frm, QGAMES::Widget::Decorator* d)
	: QGAMES::Widget (id, d),
	  _form (frm),
	  _configuration (Configuration ()), // A default one...
	  _aspects ()
{
	// Take care because form can be null...
}

// ---
int QGAMES::LabelWidget::width ()
{
	if (!_form)
		return (0);

	int result = 0;
	if (_configuration._direction == QGAMES::LabelWidget::Direction::_VERTICAL_DOWN ||
		_configuration._direction == QGAMES::LabelWidget::Direction::_VERTICAL_UP)
	{
		int tmp;
		for (int i = 0; i < (int) _aspects.size (); i++)
			if ((tmp = _form -> frameWidthForFrame (_aspects [i])) > result)
				result = tmp; // Get the widdest...
	}
	else
	{
		for (int i = 0; i < (int) _aspects.size (); i++)
			result += ((i != 0) ? _configuration._pixelsAmong : 0) 
				+ _form -> frameWidthForFrame (_aspects [i]);
	}

	return (result);
}

// ---
int QGAMES::LabelWidget::height ()
{
	if (!_form)
		return (0);

	int result = 0;
	if (_configuration._direction == QGAMES::LabelWidget::Direction::_HORIZONTAL_RIGHT ||
		_configuration._direction == QGAMES::LabelWidget::Direction::_HORIZONTAL_LEFT)
	{
		int tmp;
		for (int i = 0; i < (int) _aspects.size (); i++)
			if ((tmp = _form -> frameHeightForFrame (_aspects [i])) > result)
				result = tmp; // Get the highest...
	}
	else
	{
		for (int i = 0; i < (int) _aspects.size (); i++)
			result += ((i != 0) ? _configuration._pixelsAmong : 0) 
				+ _form -> frameHeightForFrame (_aspects [i]);
	}

	return (result);
}

// ---
QGAMES::Rectangle QGAMES::LabelWidget::box ()
{
	// If the label is drawn horizontally or vertically (but in the normal way)
	// The standard method is enough...
	QGAMES::Rectangle result = QGAMES::Widget::box ();

	if (_configuration._direction == QGAMES::LabelWidget::Direction::_HORIZONTAL_LEFT)
		result = QGAMES::Rectangle (position (), position () - QGAMES::Vector (__BD width (), -__BD height (), __BD 0));
	else if (_configuration._direction == QGAMES::LabelWidget::Direction::_VERTICAL_UP)
		result = QGAMES::Rectangle (position (), position () - QGAMES::Vector (-__BD width (), __BD height (), __BD 0));

	return (result);
}

// ---
void QGAMES::LabelWidget::setLabel (QGAMES::Form* frm, int f)
{
	_form = frm; // The form can become null, then nothing will be drawn!
	std::vector <int> a; a.push_back (f);
	setLabel (a); // Just a simple aspect...
}

// ---
void QGAMES::LabelWidget::setLabel (const std::string& txt)
{
	std::vector <int> chars (txt.length (), ' ' - _configuration._chrBase);
	for (int i = 0; i < (int) txt.length (); i++)
		chars [i] = txt [i] - _configuration._chrBase;
	setLabel (chars);
}

// ---
void QGAMES::LabelWidget::initializeFrom (const QGAMES::Widget::Properties& prp)
{
	QGAMES::Widget::initializeFrom (prp);

	QGAMES::LabelWidget::Configuration cfg;
	
	_form = NULL;
	if (prp.find (__QGAMES_LABELWIDGETATTRFORM__) != prp.end ())
		_form = game () -> formBuilder () -> 
			form (atoi ((*(prp.find (__QGAMES_LABELWIDGETATTRFORM__))).second.c_str ()));

	int chrB = 32; 
	if (prp.find (__QGAMES_LABELWIDGETATTRBASECHR__) != prp.end ())
		chrB = atoi ((*(prp.find (__QGAMES_LABELWIDGETATTRBASECHR__))).second.c_str ());
	cfg._chrBase = chrB;

	int aF = 255;
	if (prp.find (__QGAMES_LABELWIDGETATTRALPHAON__) != prp.end ())
		aF = atoi ((*(prp.find (__QGAMES_LABELWIDGETATTRALPHAON__))).second.c_str ());
	cfg._alphaFocus = aF;

	int aNF = 255;
	if (prp.find (__QGAMES_LABELWIDGETATTRALPHAOFF__) != prp.end ())
		aNF = atoi ((*(prp.find (__QGAMES_LABELWIDGETATTRALPHAOFF__))).second.c_str ());
	cfg._alphaNoFocus = aNF;

	int pA = 0;
	if (prp.find (__QGAMES_LABELWIDGETATTRPIXELSAMONG__) != prp.end ())
		pA = atoi ((*(prp.find (__QGAMES_LABELWIDGETATTRPIXELSAMONG__))).second.c_str ());
	cfg._pixelsAmong = pA;

	QGAMES::LabelWidget::Direction dr = QGAMES::LabelWidget::Direction::_HORIZONTAL_RIGHT;
	if (prp.find (__QGAMES_LABELWIDGETATTRDIRECTION__) != prp.end ())
	{
		std::string v = (*(prp.find (__QGAMES_LABELWIDGETATTRDIRECTION__))).second;
		if (v == std::string (__QGAMES_LABELWIDGETATTRDIRHR__)) dr = QGAMES::LabelWidget::Direction::_HORIZONTAL_RIGHT;
		else if (v == std::string (__QGAMES_LABELWIDGETATTRDIRHL__)) dr = QGAMES::LabelWidget::Direction::_HORIZONTAL_LEFT;
		else if (v == std::string (__QGAMES_LABELWIDGETATTRDIRVD__)) dr = QGAMES::LabelWidget::Direction::_VERTICAL_DOWN;
		else if (v == std::string (__QGAMES_LABELWIDGETATTRDIRVU__)) dr = QGAMES::LabelWidget::Direction::_VERTICAL_UP;
	}
	
	cfg._direction= dr;

	std::string txt (__NULL_STRING__);
	if (prp.find (__QGAMES_LABELWIDGETATTRTEXT__) != prp.end ())
		txt = (*(prp.find (__QGAMES_LABELWIDGETATTRTEXT__))).second;

	setConfiguration (cfg);
	setLabel (txt);
}

// ---
void QGAMES::LabelWidget::setLabel (const std::vector <int>& a)
{
	if (!_form)
	{
		_aspects = std::vector <int> ();
		return;
	}

	_aspects = a;

	// Verifies whether a selected aspect doesn't exist, then removes it...
	for (std::vector <int>::iterator i = _aspects.begin (); i != _aspects.end (); i++)
	{
		if ((*i) < 0 || (*i) >= _form -> numberFrames ())
		{
			_aspects.erase (i);
			i = _aspects.begin ();
		}
	}

	// At the end only visible aspects are part of the form...
}

// ---
void QGAMES::LabelWidget::drawWithAlpha (QGAMES::Screen* s, const QGAMES::Position& p, int a)
{
	assert (s);

	if (!_form)
		return;

	QGAMES::Position dP = p;
	for (int i = 0; i < (int) _aspects.size (); i++)
	{
		// If it is the first character and the writting direction is to the left...
		// it is necessary to take into accoun that the drawing system will draw anyway to the right
		// so it is ncessary to move the initial position a little bit to the left (just the width of the character)
		if ((i == 0) && _configuration._direction == QGAMES::LabelWidget::Direction::_HORIZONTAL_LEFT)
			dP -= QGAMES::Vector (__BD _form -> frameWidthForFrame (_aspects [0]), __BD 0, __BD 0);

		_form -> frame (_aspects [i]) -> drawOn (s, dP, a);

		switch (_configuration._direction)
		{
			case QGAMES::LabelWidget::Direction::_HORIZONTAL_RIGHT:
				dP += QGAMES::Vector (__BD (_configuration._pixelsAmong + _form -> frameWidthForFrame (_aspects [i])), 
						__BD 0, __BD 0);
				break;
			case QGAMES::LabelWidget::Direction::_HORIZONTAL_LEFT:
				dP -= QGAMES::Vector (__BD (_configuration._pixelsAmong + _form -> frameWidthForFrame (_aspects [i])),
						__BD 0, __BD 0);
				break;
			case QGAMES::LabelWidget::Direction::_VERTICAL_DOWN:
				dP += QGAMES::Vector (__BD 0, 
					__BD (_configuration._pixelsAmong + _form -> frameHeightForFrame (_aspects [i])), __BD 0);
				break;
			case QGAMES::LabelWidget::Direction::_VERTICAL_UP:
				dP -= QGAMES::Vector (__BD 0, 
					__BD (_configuration._pixelsAmong + _form -> frameHeightForFrame (_aspects [i])), __BD 0);
				break;
		}
	}
}

// ---
void QGAMES::IteratingLabelWidget::setValues (const std::vector <std::string> vals)
{
	assert (!vals.empty ());

	_stringValues = vals;
	_aspectValues = std::vector <int> ();
	_currentValue = 0;

	setLabel (_stringValues [0]);
}

// ---
void QGAMES::IteratingLabelWidget::setValues (const std::vector <int> vals)
{
	assert (!vals.empty ());

	_stringValues = std::vector <std::string> ();
	_aspectValues = vals;
	_currentValue = 0;

	setLabel (_aspectValues [0]);
}

// ---
void QGAMES::IteratingLabelWidget::setCurrentValue (int v)
{
	_currentValue = v;
	if (_currentValue < 0 || 
		(!_stringValues.empty () && _currentValue > (int) _stringValues.size ()) ||
		(!_aspectValues.empty () && _currentValue > (int) _aspectValues.size ()))
		_currentValue = 0; // If out the limits...

	if (!_stringValues.empty ()) setLabel (_stringValues [_currentValue]);
	else setLabel (_aspectValues [_currentValue]);
}

// ---
void QGAMES::IteratingLabelWidget::initializeFrom (const QGAMES::LabelWidget::Properties& prp)
{
	QGAMES::LabelWidget::initializeFrom (prp);

	_stringValues = std::vector <std::string> ();
	if (prp.find (__QGAMES_ITERLABELWIDGETATTRSTRVALUES__) != prp.end ())
		_stringValues = 
			QGAMES::getElementsFrom ((*prp.find (__QGAMES_ITERLABELWIDGETATTRSTRVALUES__)).second, ',');

	_aspectValues = std::vector <int> ();
	if (prp.find (__QGAMES_ITERLABELWIDGETATTRINTVALUES__) != prp.end ())
		_aspectValues = 
			QGAMES::getElementsFromAsInt ((*prp.find (__QGAMES_ITERLABELWIDGETATTRINTVALUES__)).second, ',');

	// Both can't be null simultaneosuly!
	assert (!_stringValues.empty () || !_aspectValues.empty ());
	
	_currentValue = 0;

	if (!_stringValues.empty ()) setLabel (_stringValues [0]);
	else setLabel (_aspectValues [0]);
}

// ---
void QGAMES::IteratingLabelWidget::whenSelected ()
{
	QGAMES::LabelWidget::whenSelected ();

	// Select the next value valid in the list
	// f there is no any more, then back to the first
	if (++_currentValue >= (!_stringValues.empty () 
		? (int) _stringValues.size () : (int) _aspectValues.size ())) _currentValue = 0;
	// Set the value back
	if (!_stringValues.empty ()) setLabel (_stringValues [_currentValue]);
	else setLabel (_aspectValues [_currentValue]);
}

// ---
void QGAMES::KeyLabelWidget::whenOnKey (KeyCode kC, char c)
{
	// Only valid for iterating label widgets made up of strings...
	if (_stringValues.size () == 0)
		return; // ...otherwise, nothing to do

	bool nextFound = false; // Not found yet...
	int initValue = _currentValue; // The next element in list to validate...
	if (++_currentValue >= (int) _stringValues.size ()) _currentValue = 0; // Starting for the next...
	while (!nextFound && _currentValue != initValue)
	{
		if (_stringValues [_currentValue] != std::string (__NULL_STRING__) && // Only when letter matches (dows't matter upper or lower)
			std::toupper ((int) _stringValues [_currentValue][0 /** The first letter */]) == (int) std::toupper (c)) 
			nextFound = true;
		else
			if (++_currentValue >= (int) _stringValues.size ())
				_currentValue = 0;
	}

	if (nextFound)
	{
		setLabel (_stringValues [_currentValue]);

		notify (QGAMES::Event (__QGAMES_WIDGETSELECTEDEVENT__, this));
	}
}

// ---
QGAMES::IntroWidget::IntroWidget (int id, QGAMES::Form* frm, QGAMES::Widget::Decorator* d)
	: QGAMES::Widget (id, d),
	  _form (frm),
	  _configuration (Configuration ()), // A default one...
	  _labelAspects (),
	  _textAspects (),
	  _cursorPosition (0) // at the beginning
{
	// Take care because form can be null...
}

// ---
int QGAMES::IntroWidget::width ()
{
	if (!_form)
		return (0);

	// First calculates the width of the label...
	int lW = 0;
	for (int i = 0; i < (int) _labelAspects.size (); i++)
		lW += ((i != 0) ? _configuration._pixelsAmong : 0) 
			+ _form -> frameWidthForFrame (_labelAspects [i]);
	// Then the width of the intro, taking into account the maximum number of chars possible
	// ...and with the standard width...
	int lI = (_form -> frameWidth () * _configuration._numberChars) + 
		((_configuration._pixelsAmong - 1) * _configuration._numberChars);

	// Then calculates the total length, taking into account where the text is respect to the label...
	int result = lW + lI; // Suppose it is at the right...
	if (_configuration._location == QGAMES::IntroWidget::IntroLocation::_INTRO_DOWN)
		result = (lW > lI) ? lW : lI; // The largest one, in the case the label is above o below the intro field...
	
	return (result);
}

// ---
int QGAMES::IntroWidget::height ()
{
	if (!_form)
		return (0);

	// For the height the standard height for the frame is used (take carre when define it into files)...
	// This is done is this way because not all the characters have been introduced at this point already!
	int result = _form -> frameHeight ();
	if (_configuration._location == QGAMES::IntroWidget::IntroLocation::_INTRO_DOWN)
		result = (result * 2) + _configuration._pixelsAmong; // twice plus the separation...

	return (result);
}

// ---
void QGAMES::IntroWidget::setText (const std::string& txt)
{
	_text = txt;
	if ((int) _text.length () > _configuration._numberChars)
		_text = _text.substr (0, _configuration._numberChars); // No more characters than allowed
	std::vector <int> chars ((int) _text.length (), ' ' - _configuration._chrBase);
	for (int i = 0; i < (int) _text.length (); i++)
		chars [i] = _text [i] - _configuration._chrBase;
	setText (chars);
}

// ---
void QGAMES::IntroWidget::setLabel (QGAMES::Form* frm, int f)
{
	_form = frm; // The form can become null, then nothing will be drawn!
	std::vector <int> a; a.push_back (f);
	setLabel (a); // Just a simple aspect...
}

// ---
void QGAMES::IntroWidget::setLabel (const std::string& txt)
{
	std::vector <int> chars (txt.length (), ' ' - _configuration._chrBase);
	for (int i = 0; i < (int) txt.length (); i++)
		chars [i] = txt [i] - _configuration._chrBase;
	setLabel (chars);
}

// ---
void QGAMES::IntroWidget::initializeFrom (const QGAMES::Widget::Properties& prp)
{
	QGAMES::Widget::initializeFrom (prp);

	QGAMES::IntroWidget::Configuration cfg;
	
	_form = NULL;
	if (prp.find (__QGAMES_INTROWIDGETATTRFORM__) != prp.end ())
		_form = game () -> formBuilder () -> 
			form (atoi ((*(prp.find (__QGAMES_INTROWIDGETATTRFORM__))).second.c_str ()));

	int chrB = 32; 
	if (prp.find (__QGAMES_INTROWIDGETATTRBASECHR__) != prp.end ())
		chrB = atoi ((*(prp.find (__QGAMES_INTROWIDGETATTRBASECHR__))).second.c_str ());
	cfg._chrBase = chrB;

	int aF = 255;
	if (prp.find (__QGAMES_INTROWIDGETATTRALPHAON__) != prp.end ())
		aF = atoi ((*(prp.find (__QGAMES_INTROWIDGETATTRALPHAON__))).second.c_str ());
	cfg._alphaFocus = aF;

	int aNF = 255;
	if (prp.find (__QGAMES_INTROWIDGETATTRALPHAOFF__) != prp.end ())
		aNF = atoi ((*(prp.find (__QGAMES_INTROWIDGETATTRALPHAOFF__))).second.c_str ());
	cfg._alphaNoFocus = aNF;

	int pA = 0;
	if (prp.find (__QGAMES_INTROWIDGETATTRPIXELSAMONG__) != prp.end ())
		pA = atoi ((*(prp.find (__QGAMES_INTROWIDGETATTRPIXELSAMONG__))).second.c_str ());
	cfg._pixelsAmong = pA;

	int nC = 0;
	if (prp.find (__QGAMES_INTROWIDGETATTRNUMBERCHARS__) != prp.end ())
		nC = atoi ((*(prp.find (__QGAMES_INTROWIDGETATTRNUMBERCHARS__))).second.c_str ());
	cfg._numberChars = nC;

	std::string mk (__NULL_STRING__);
	if (prp.find (__QGAMES_INTROWIDGETATTRMASK__) != prp.end ())
		mk = (*(prp.find (__QGAMES_INTROWIDGETATTRMASK__))).second;
	cfg._mask = mk;

	std::string cl = "255,255,255,255"; // Pure white by default...
	if (prp.find (__QGAMES_INTROWIDGETATTRCURSORCOLOR__) != prp.end ())
		cl = (*(prp.find (__QGAMES_INTROWIDGETATTRCURSORCOLOR__))).second;
	std::vector <int> clV = QGAMES::getElementsFromAsInt (cl, ',',4);
	cfg._cursorColor = QGAMES::Color (clV [0], clV [1], clV [2], clV [3]);

	QGAMES::IntroWidget::IntroLocation lc = QGAMES::IntroWidget::IntroLocation::_INTRO_RIGHT;
	if (prp.find (__QGAMES_INTROWIDGETATTRLOCATION__) != prp.end ())
	{
		std::string v = (*(prp.find (__QGAMES_INTROWIDGETATTRLOCATION__))).second;
		if (v == std::string (__QGAMES_INTROWIDGETATTRLOCATIOND__)) lc = QGAMES::IntroWidget::IntroLocation::_INTRO_DOWN;
	}
	
	cfg._location= lc;

	std::string txt (__NULL_STRING__);
	if (prp.find (__QGAMES_INTROWIDGETATTRTEXT__) != prp.end ())
		txt = (*(prp.find (__QGAMES_INTROWIDGETATTRTEXT__))).second;

	setConfiguration (cfg);
	setLabel (txt);
}

// ---
void QGAMES::IntroWidget::whenOnKey (QGAMES::KeyCode kC, char c)
{
	std::string cT = text ();

	switch (kC)
	{
		case QGAMES::KeyCode::QGAMES_RIGHT:
			if (_cursorPosition < (int) cT.length ())
				_cursorPosition++; // Can be after trhe last character...
			break;

		case QGAMES::KeyCode::QGAMES_LEFT:
			if (_cursorPosition > 0)
				_cursorPosition--;
			break;

		case QGAMES::KeyCode::QGAMES_END:
			_cursorPosition = cT.length (); // It is after the last character...
			break;

		case QGAMES::KeyCode::QGAMES_HOME:
			_cursorPosition = 0; // At the beginning...
			break;

		case QGAMES::KeyCode::QGAMES_BACK:
			if (cT.length () > 0 && _cursorPosition > 0) 
			{
				setText (cT.substr (0, _cursorPosition - 1) + cT.substr (_cursorPosition));
				_cursorPosition--;
			}
			break;

		case QGAMES::KeyCode::QGAMES_DELETE:
			if (_cursorPosition < (int) cT.length ())
				setText (cT.substr (0, _cursorPosition) + cT.substr (_cursorPosition + 1));
			break;

		case QGAMES::KeyCode::QGAMES_RETURN:
			whenSelected (); // The intro means selected...
			break;

		default:
			{
				if (_cursorPosition < _configuration._numberChars)
				{
					std::string oTxt = _text;
					std::string nTxt = cT.substr (0, _cursorPosition) + c + cT.substr (_cursorPosition);
					if (oTxt != nTxt && isTextValid (nTxt)) // Is the text valid and different?
					{
						setText (nTxt);
						_cursorPosition++; // Add it eventually..
					}
				}
			}
			break;
	}
}

// ---
bool QGAMES::IntroWidget::isTextValid (const std::string& txt) const
{
	bool result = true;

	if (_configuration._mask != std::string (__NULL_STRING__))
	{
		for (int i = 0; i < (int) _configuration._mask.length (); i++)
		{
			if (i < (int) txt.length ())
			{
				char chr = txt [i];
				bool letter = (chr >= 65 && chr <= 90) ||
							  (chr >= 97 && chr <= 122) ||
							  (chr == 32); // A letter?
				bool number = chr >= 48 && chr <= 57; // A number?
				bool symbol = (chr >= 33 && chr <= 47) || 
							  (chr >= 58 && chr <= 64) || 
							  (chr >= 91 && chr <= 96) || 
							  (chr >= 123 && chr <= 127); // A symbol?
				switch (_configuration._mask [i])
				{
					case 'a':
						result = letter;
						break;
					case '9':
						result = number;
						break;
					case 'A':
						result = letter || symbol;
						break;
					case 'x':
						result = letter || number;
						break;
					case 'X':
						result = letter || number || symbol;
						break;
					default:
						break;
				}
			}
		}
	}

	return (result);
}

// ---
void QGAMES::IntroWidget::drawWithAlpha (QGAMES::Screen* s, const QGAMES::Position& p, int a)
{
	assert (s);

	if (!_form)
		return;

	QGAMES::Position iP = p;
	QGAMES::Position dP = p;

	// First of all, the label is drawn...
	for (int i = 0; i < (int) _labelAspects.size (); i++)
	{
		if (_labelAspects [i] >= 0 && _labelAspects [i] < _form -> numberFrames ())
			_form -> frame (_labelAspects [i]) -> drawOn (s, dP, a); // Just only if the frame for that aspect exists...
		dP += QGAMES::Vector (__BD (_configuration._pixelsAmong + 
			_form -> frameWidthForFrame (_labelAspects [i])), __BD 0, __BD 0);
	}

	QGAMES::Position cP = dP;

	// Then, taking into account where the intro fiedls is, draws the text...
	if (_configuration._location == QGAMES::IntroWidget::IntroLocation::_INTRO_DOWN)
		dP = iP + QGAMES::Vector (__BD 0, __BD (_form -> frameHeight () + _configuration._pixelsAmong), __BD 0);
	for (int i = 0; i < (int) _textAspects.size (); i++)
	{
		if (_textAspects [i] >= 0 && _textAspects [i] < _form -> numberFrames ()) 
			_form -> frame (_textAspects [i]) -> drawOn (s, dP, a); // Just only if the frame for that aspect exists...
		dP += QGAMES::Vector (__BD (_configuration._pixelsAmong + 
			_form -> frameWidthForFrame (_textAspects [i])), __BD 0, __BD 0);
		if (i < _cursorPosition)
			cP = dP; // The cursor advances at the same time that the letter, just to get the right position...
	}

	// Draws the cursor eventually...if it has the focus...
	if (_hasFocus)
	{
		cP = cP + QGAMES::Vector (__BD 0, __BD _form -> frameHeight (), __BD 0);
		s -> drawRectangle (QGAMES::Rectangle (cP, cP + QGAMES::Vector (__BD (_form -> frameWidth () >> 1), __BD 3, __BD 0)), 
			_configuration._cursorColor, true);
	}
}

// ---
std::vector <int> QGAMES::IntroWidget::clearAspects (const std::vector <int>& a)
{
	std::vector <int> result;

	if (_form)
	{
		result = a;

		// Verifies whether a selected aspect doesn't exist, then removes it...
		for (std::vector <int>::iterator i = result.begin (); i != result.end (); i++)
		{
			if ((*i) < 0 || (*i) >= _form -> numberFrames ())
			{
				result.erase (i);
				i = result.begin ();
			}
		}

		// At the end only visible aspects are part of the form...
	}

	return (result);
}
