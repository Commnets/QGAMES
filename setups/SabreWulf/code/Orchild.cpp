#include "Orchild.hpp"
#include "Sabreman.hpp"

// ---
Orchild::Orchild (const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: QGAMES::Artist (__SABREWULFORCHILD__, f, d),
	  _counter (NULL)
{
	// Nothing else to do so far...
}

// ---
bool Orchild::isBig () const
{
	assert (_counter);
	return (_counter -> _percentaje >= 90);
}

// ---
Orchild::Type Orchild::type () const
{
	assert (_counter);

	Orchild::Type result;
	switch (_counter -> _color)
	{
		case 0:
			result = Orchild::Type::_RED;
			break;
		case 1:
			result = Orchild::Type::_PINK;
			break;
		case 2:
			result = Orchild::Type::_BLUE;
			break;
		case 3:
			result = Orchild::Type::_YELLOW;
			break;
		case 4:
			result = Orchild::Type::_WHITE;
			break;
		default:
			exit (1); // This situation is not possible...only when there is an error...
	}

	return (result);
}

// ---
QGAMES::Rectangle Orchild::collisionZone () const
{
	assert (_currentForm); // It has to have a form...
	return (QGAMES::Rectangle (position (), 
		position () + QGAMES::Vector (__BD _currentForm -> frameWidth (), 
			__BD _currentForm -> frameHeight (), __BD 0)));
}

// ---
void Orchild::initialize ()
{
	_counter = NULL; // No aspect...
	setCurrentForm ((*_forms.begin ()).first); // The form is the basic one...
	setCurrentAspect (0); // Same than the aspect...
	setCurrentMovement (__MININT__); // No movement...
	QGAMES::Artist::initialize ();
}

// ---
void Orchild::updatePositions ()
{
	assert (_counter);
	setOrchildAspect ();
	QGAMES::Artist::updatePositions ();
}

// ---
void Orchild::actualizeFrom (SabreWulfWorld::OrchildCounter* ct)
{
	assert (ct);
	_counter = ct;
	setOrchildAspect ();
}

// ---
void Orchild::whenCollisionWith (QGAMES::Entity* e)
{
	assert (e);
	if (dynamic_cast <Sabreman*> (e) != NULL)
	{
		// When sabreman collisions with a yellow orchild, the this one, starts from 0
		if (type () == Orchild::Type::_YELLOW)
		{
			_counter -> _counter = 0;
			_counter -> _growing = true;
			_counter -> _growingDirection = 1; 
			_counter -> _percentaje = 0; // Starts again...
		}
	}
}

// ---
void Orchild::setOrchildAspect ()
{
	assert (_counter);
	setCurrentForm (_counter -> _color + 
		((_counter -> _percentaje >= 90) ? __SABREMANBASEORCHILD__ : __SABREMANBASEORCHILDFLOWERS__));
	setCurrentAspect ((_counter -> _percentaje >= 90) 
		? 0 : (_counter -> _percentaje * __SABREMANNUMBERFLOWERASPECTS__ / 90));
	// From 0 to 90, the aspect is the flower growing...
	// From 0 to 100, the aspect is the flower full...
	setPosition (_counter -> _position - 
		QGAMES::Vector (__BD 0, __BD _currentForm -> frameHeight (), __BD 0));
}
