#include <BattleshipLike/bsufo.hpp>

// ---
QGAMES::Entity* BATTLESHIP::UFO::clone () const
{
	QGAMES::Entity* result = new BATTLESHIP::UFO 
		(_id, dynamic_cast <BATTLESHIP::Spaceship::Behaviour*> (_behaviour -> clone ()), _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
bool BATTLESHIP::UFO::isEnemy (const BATTLESHIP::SpaceElement* ss) const
{ 
	return (dynamic_cast <const BATTLESHIP::Battleship*> (ss) != NULL); 
} 

// ---
QGAMES::Position BATTLESHIP::UFO::shootingPosition (int nS, int tS, int nP) const
{
	assert (tS > nS && nS >= 0);

	QGAMES::Vector sPos ((((__BD (tS - 1)) / __BD 2) - __BD nS) * 
		__BD ((tS == 1) ? 0 : (((visualLength () - 20) >> 1) / (tS >> 1))), __BD 0, __BD 0);
	return (position () + sPos + QGAMES::Vector
		(__BD (currentForm () -> frameWidthForFrame (currentAspect ()) >> 1), __BD (visualHeight () >> 1) /** at the bootom */, __BD 0));
}
// ---
void BATTLESHIP::UFO::initialize ()
{
	setType (__BATTLESHIP_ENEMTYTYPE1__); // The default...

	BATTLESHIP::Spaceship::initialize ();
}

// ---
QGAMES::Rectangle BATTLESHIP::UFO::collisionZone () const
{
	QGAMES::Rectangle rect = BATTLESHIP::Spaceship::collisionZone ();
	QGAMES::Vector dg = __BD 0.25 * (rect.pos2 () - rect.pos1 ());
	return (QGAMES::Rectangle (rect.pos1 () + dg, rect.pos2 () - dg));
}

// ---
void BATTLESHIP::UFO::setType (int t)
{ 
	BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (bG);

	BATTLESHIP::DataGame* dG = bG -> dataGame ();
	assert (dG);

	assert (t == -1 || 
			std::find (dG -> ufoTypesList ().begin (), dG -> ufoTypesList ().end (), t) != dG -> ufoTypesList ().end ());

	_statesId = BATTLESHIP::UFO::StatesId (dG -> defaultStatesForUFOType 
		(_type = ((t == -1) ? dG -> ufoTypesList ()[rand () % __BATTLESHIP_NUMBEROFUFOTYPES__] : t)));

	setStateFor (spaceElementState (), orientation ());
}

// ---
void BATTLESHIP::UFO::setStateFor (int st, const QGAMES::Vector& o)
{
	switch (st)
	{
		case _FROZEN:
			setCurrentState (_statesId._frozenState);
			break;

		case _HIDDEN:
			setCurrentState (_statesId._hiddenState);
			break;

		case _MOVING:
			setCurrentState (_statesId._movingState);
			break;

		case _EXPLODING:
			setCurrentState (_statesId._explodingState);
			break;

		case _DESTROYED:
			setCurrentState (_statesId._destroyedState);
			break;

		default:
			assert (0); // No others states have been defined in the default implementation of asteroid
	}
}

// ---
bool BATTLESHIP::BigUFO::hasCollisionWith (QGAMES::Entity* ety) const
{
	assert (ety);

	bool result = false;
	const QGAMES::Rectangles& wZ = weakAreasForFormAndAspect (currentForm () -> id (), currentAspect ());
	for (QGAMES::Rectangles::const_iterator i = wZ.begin (); i != wZ.end () && !result; 
		result = ety -> collisionZone ().intersectsWith ((*i++) + centerPosition ())); // Just valid for 2D...

	return (result);
}

// ---
const QGAMES::Positions& BATTLESHIP::BigUFO::centralShootingPositionsForFormAndAspect (int f, int a) const
{
	std::map <int, std::map <int, QGAMES::Positions>>::const_iterator i = _centralShootingPositions.find (f);
	if (i == _centralShootingPositions.end ())
	{
		assert (_forms.find (f) != _forms.end () && 
				((*_forms.find (f)).second -> numberFrames () > a));

		_centralShootingPositions [f][a] = calculateCentralShootingPositionsForFormAndAspect (f, a);
	}
	else
	{
		std::map <int, QGAMES::Positions>::const_iterator j = (*i).second.find (a);
		if (j == (*i).second.end ())
		{
			assert (((*_forms.find (f)).second -> numberFrames () > a));

			_centralShootingPositions [f][a] = calculateCentralShootingPositionsForFormAndAspect (f, a);
		}
	}

	return (_centralShootingPositions [f][a]);
}

// ---
const QGAMES::Rectangles& BATTLESHIP::BigUFO::weakAreasForFormAndAspect (int f, int a) const
{
	std::map <int, std::map <int, QGAMES::Rectangles>>::const_iterator i = _weakAreas.find (f);
	if (i == _weakAreas.end ())
	{
		assert (_forms.find (f) != _forms.end () && 
				((*_forms.find (f)).second -> numberFrames () > a));

		_weakAreas [f][a] = calculateWeakAreasForFormAndAspect (f, a);
	}
	else
	{
		std::map <int, QGAMES::Rectangles>::const_iterator j = (*i).second.find (a);
		if (j == (*i).second.end ())
		{
			assert (((*_forms.find (f)).second -> numberFrames () > a));

			_weakAreas [f][a] = calculateWeakAreasForFormAndAspect (f, a);
		}
	}

	return (_weakAreas [f][a]);
}

// ---
void BATTLESHIP::BigUFO::drawOnLimits (QGAMES::Screen* s, const QGAMES::Position& p)
{
	BATTLESHIP::UFO::drawOnLimits (s, p);

	for (auto i : weakAreasForFormAndAspect (currentForm () -> id (), currentAspect ()))
		s -> drawRectangle (i + centerPosition (), __QGAMES_REDCOLOR__); // To sign out the zones...
}
