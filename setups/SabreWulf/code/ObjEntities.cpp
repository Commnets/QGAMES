#include "ObjEntities.hpp"
#include "Movements.hpp"
#include "Maps.hpp"
#include "Game.hpp"
#include "Maps.hpp"

// ---
SabreWulfObject::SabreWulfObject (int id, const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: QGAMES::Artist (id, f, d),
	  _counter (NULL)
{
	// Invisible by default...
	setVisible (false);

	// Nothing else to do so far...
}

// ---
SabreWulfObject::Type SabreWulfObject::type () const
{
	assert (_counter);

	SabreWulfObject::Type result;
	switch (_counter -> _type)
	{
		case 0: case 8:
			result = SabreWulfObject::_CHEST;
			break;
		case 1:	case 9:
			result = SabreWulfObject::_RING;
			break;
		case 2:	case 10:
			result = SabreWulfObject::_BALL;
			break;
		case 3:	case 11:
			result = SabreWulfObject::_HAT;
			break;
		case 4:	case 12: case 13:
			result = SabreWulfObject::_DIAMOND;
			break;
		case 5:
			result = SabreWulfObject::_LIFE;
			break;
		case 6: case 14:
			result = SabreWulfObject::_DOLLARS;
			break;
		case 7: case 15:
			result = SabreWulfObject::_SYRINGE;
			break;
		case 16: case 17: case 18: case 19:
			result = SabreWulfObject::_AMULET;
			break;
		default:
			exit (1); // This situation is not possible unless an error comes..
	}

	return (result);
}

// ---
int SabreWulfObject::amuletPieceNumber () const
{
	if (_counter == NULL)
		return (__MININT__); // No piece possible...
	return ((type () == SabreWulfObject::Type::_AMULET) // If an amulet, the counter keeps where piece it is...
		? (_counter -> _type - __NUMBERPOSSIBILITIESOBJECT__) : __MININT__);
}

// ---
QGAMES::Rectangle SabreWulfObject::collisionZone () const
{
	assert (_currentForm); // It has to have a form to calculate the size...
	// If the object is not vislbe, then it has not collision zone,
	// otherwise it will be determinated by its aspect!
	return (isVisible () ? QGAMES::Rectangle (position (), 
		position () + QGAMES::Vector (__BD _currentForm -> frameWidth (), 
			__BD _currentForm -> frameHeight (), __BD 0)) : QGAMES::Rectangle::_noRectangle);
}

// ---
void SabreWulfObject::initialize ()
{
	_counter = NULL; // No aspect...
	setVisible (false); // Not visible!
	setCurrentForm ((*_forms.begin ()).first); // The form is the basic one...
	setCurrentAspect (0); // Same than the aspect...
	QGAMES::Artist::initialize ();
}

// ---
void SabreWulfObject::updatePositions ()
{
	setObjectAspect ();
	QGAMES::Artist::updatePositions ();
}

// ---
void SabreWulfObject::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Artist::drawOn (s, p);
}

// ---
void SabreWulfObject::actualizeFrom (SabreWulfWorld::ObjectCounter* ct)
{
	// Counter can be null...
	_counter = ct;
	setObjectAspect ();
}

// ---
void SabreWulfObject::whenCollisionWith (QGAMES::Entity* e)
{
	assert (e);
	if (dynamic_cast <Sabreman*> (e) != NULL)
	{
		// This control is unncessary unless there is a mistake in the code
		// Reason is this method would be invoked unless a colission with something 
		// visible happens...just in case the code is here!
		if (_counter != NULL)
		{
			_counter -> _caught = true; // The base thing has been caught...
			setVisible (false); // from now on, the object is not longer visible...
		}
	}

	// The collision with any other type of objects is not important
}

// ---
void SabreWulfObject::setObjectAspect ()
{
	if (_counter == NULL)
	{
		setVisible (false);
		setCurrentForm ((*_forms.begin ()).first); // The form is the basic one...
		setCurrentAspect (0); // Same than the aspect...
		return; // No counter, not visible...
	}

	setCurrentForm ((_counter -> _type >= 0 && _counter -> _type < __NUMBERPOSSIBILITIESOBJECT__) 
		? __SABREWULFOBJECTS__ : __SABREMANLITTLEAMULET__);
	setCurrentAspect ((_counter -> _type >= 0 && _counter -> _type < __NUMBERPOSSIBILITIESOBJECT__) 
		? _counter -> _type : (_counter -> _type - __NUMBERPOSSIBILITIESOBJECT__));
	setPosition (_counter -> _position - 
		QGAMES::Vector (__BD 0, __BD _currentForm -> frameHeight (), __BD 0));

	setVisible (!_counter -> _caught); 
	// If the object has not been caught already, then it will be visible...
	// Otherwise it will be invisible!!
}
