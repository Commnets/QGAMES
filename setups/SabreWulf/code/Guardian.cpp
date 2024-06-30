#include "Guardian.hpp"

// ---
SabreWulfGuardian::SabreWulfGuardian (const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: QGAMES::Artist (__SABREWULFGUARDIAN__, f, d),
	  _moves (false),
	  _counter (0), _cAspect (0), _posMoved (0)
{
	// Invisible by default...
	setVisible (false);

	// Nothing esl so far...
}

// ---
QGAMES::Rectangle SabreWulfGuardian::collisionZone () const
{
	// A form has to exist...
	assert (_currentForm);

	// The collision zone is the center of the guardian...
	QGAMES::Vector v (__BD (_currentForm -> frameWidth () >> 2), 
		__BD (_currentForm -> frameHeight () >> 2), __BD 0);
	return (QGAMES::Rectangle (_data._position + v, _data._position + (3 * v)));
}

// ---
void SabreWulfGuardian::initialize ()
{
	QGAMES::Artist::initialize ();

	// By default, the guardian neither moves nor is visible..
	_moves = false;
	setVisible (false);

	// The guardian has an initial position...
	// That position will be determinated when the player enters the exit maze place
	// Otherwise, the position will be not important,
	// because the guardina will be invisible and he will not been drawn!

	// The initial aspet is always 0
	_cAspect = 0;
	// The number of positions moved to the right is initially 0...
	_posMoved = 0;
	// The number of positions to moved is the width of the maze dived by 4
	_maxPosMoved = (__ORIGINALMAPLENGTH__ >> 2);
	setCurrentMovement (__MININT__); // No movement...

	// Sets the current form...
	setCurrentForm (__SABREWULFGUARDIANFORM__);
	setCurrentAspect (0); // The basic aspect...
}

// ---
void SabreWulfGuardian::updatePositions ()
{
	// If the guardian is not visible, this method doesn't do anything...
	if (!isVisible ())
		return;

	// Moves the guardian if it is needed...
	if (_moves)
		if (_posMoved++ <= _maxPosMoved)
			_data._position = _data._position + QGAMES::Vector (__BD 1, __BD 0, __BD 0);

	// Changes the aspect...
	if (_counter++ == 6)
	{
		_counter = 0;
		if (++_cAspect >= __SABREWULFGUARDIANASPECTS__)
			_cAspect = 0;
		setCurrentAspect (_cAspect);
	}

	// Normal treatement...
	QGAMES::Artist::updatePositions ();
}

// ---
void SabreWulfGuardian::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Artist::drawOn (s, p);
}

// ---
void SabreWulfGuardian::whenCollisionWith (QGAMES::Entity* e)
{
	assert (e);
	// Nothing special to do so far...
}
