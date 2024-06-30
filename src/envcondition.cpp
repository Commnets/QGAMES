#include <Arcade/envcondition.hpp>
#include <Arcade/definitions.hpp>
#include <cassert>

// ---
QGAMES::EnvironmentalCondition::EnvironmentalCondition (int id, QGAMES::Scene* s, bool aOnce)
	: QGAMES::Element (id),
	  _scene (s),
	  _affectsOnlyOnce (aOnce)
{
	assert (_scene);
}

// ---
void QGAMES::EnvironmentalCondition::initialize ()
{
	QGAMES::Element::initialize ();

	notify (QGAMES::Event (__QGAMES_ENVCONDITIONSTARTED__, this));
}

// ---
void QGAMES::EnvironmentalCondition::finalize ()
{
	QGAMES::Element::finalize ();

	notify (QGAMES::Event (__QGAMES_ENVCONDITIONFINISHED__, this));
}

// ---
void QGAMES::EnvironmentalCondition::affectArtists (const QGAMES::Entities& ets)
{
	for (QGAMES::Entities::const_iterator i = ets.begin (); i != ets.end (); i++)
		if (dynamic_cast <QGAMES::Artist*> ((*i).second))
			((QGAMES::Artist*) (*i).second) -> whenWheatherCondition (this); // Only if they the are artists...
}

// ---
void QGAMES::EnvironmentalCondition::stopAffectingArtists (const QGAMES::Entities& ets)
{
	for (QGAMES::Entities::const_iterator i = ets.begin (); i != ets.end (); i++)
		if (dynamic_cast <QGAMES::Artist*> ((*i).second))
			((QGAMES::Artist*) (*i).second) -> whenNoWheatherCondition (this); 
}

// ---
QGAMES::ComplexEnvironmentalCondition::ComplexEnvironmentalCondition 
		(int id, QGAMES::Scene* s, const QGAMES::EnvironmentalConditions& cds)
	: QGAMES::EnvironmentalCondition (id, s),
	  _conditions (cds),
	  _conditionStatus ()
{
	for (EnvironmentalConditions::const_iterator i = _conditions.begin ();
			i != _conditions.end (); i++)
	{
		_conditionStatus [(*i)] = true; // All are active at the beginning...
		observe ((*i)); // Observe the condition in detail...
	}
}

// ---
void QGAMES::ComplexEnvironmentalCondition::addCondition (QGAMES::EnvironmentalCondition* cd)
{
	assert (cd); // Not null allowed...

	_conditions.push_back (cd); // It could be repeated...
	_conditionStatus [cd] = true; // It is active by default...
	observe (cd); // ...and the new condition is also observed
}

// ---
void QGAMES::ComplexEnvironmentalCondition::activateCondition (QGAMES::EnvironmentalCondition* cd)
{
	assert (cd);

	std::map <QGAMES::EnvironmentalCondition*, bool>::iterator i;
	if ((i = _conditionStatus.find (cd)) != _conditionStatus.end ())
	{
		(*i).first -> initialize ();
		(*i).second = true;
	}
}

// ---
void QGAMES::ComplexEnvironmentalCondition::desactivateCondition (QGAMES::EnvironmentalCondition* cd)
{
	assert (cd);

	std::map <QGAMES::EnvironmentalCondition*, bool>::iterator i;
	if ((i = _conditionStatus.find (cd)) != _conditionStatus.end ())
	{
		(*i).first -> finalize ();
		(*i).second = false;
	}
}

// ---
bool QGAMES::ComplexEnvironmentalCondition::existsCondition (int id) const
{
	bool result = false;
	for (QGAMES::EnvironmentalConditions::const_iterator i = _conditions.begin (); 
			i != _conditions.end () && !result; i++)
		if ((*i) -> id () == id)
			result = true;
	return (result);
}

// ---
QGAMES::EnvironmentalCondition* QGAMES::ComplexEnvironmentalCondition::findCondition (int id)
{
	QGAMES::EnvironmentalCondition* result = NULL;
	for (QGAMES::EnvironmentalConditions::const_iterator i = _conditions.begin (); 
			i != _conditions.end () && !result; i++)
		if ((*i) -> id () == id)
			result = (*i);
	assert (result != NULL);

	return (result);
}

// ---
void QGAMES::ComplexEnvironmentalCondition::initialize ()
{
	for (EnvironmentalConditions::const_iterator i = _conditions.begin ();
			i != _conditions.end (); i++)
		if (isActive ((*i))) (*i) -> initialize ();
	QGAMES::EnvironmentalCondition::initialize ();
}

// ---
void QGAMES::ComplexEnvironmentalCondition::inEveryLoop ()
{
	for (EnvironmentalConditions::const_iterator i = _conditions.begin ();
			i != _conditions.end (); i++)
		if (isActive ((*i))) (*i) -> inEveryLoop ();
	QGAMES::EnvironmentalCondition::inEveryLoop ();
}

// ---
void QGAMES::ComplexEnvironmentalCondition::updatePositions ()
{
	for (EnvironmentalConditions::const_iterator i = _conditions.begin ();
			i != _conditions.end (); i++)
		if (isActive ((*i))) (*i) -> updatePositions ();
	QGAMES::EnvironmentalCondition::updatePositions ();
}

// ---
void QGAMES::ComplexEnvironmentalCondition::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	for (EnvironmentalConditions::const_iterator i = _conditions.begin ();
			i != _conditions.end (); i++)
		if (isActive ((*i))) (*i) -> drawOn (s, p);
	// The environmental conditions have to be drawn be their own...
}

// ---
void QGAMES::ComplexEnvironmentalCondition::finalize ()
{
	for (EnvironmentalConditions::const_iterator i = _conditions.begin ();
			i != _conditions.end (); i++)
		if (isActive ((*i))) (*i) -> finalize ();
	QGAMES::EnvironmentalCondition::finalize ();
}

// ---
void QGAMES::ComplexEnvironmentalCondition::affectArtists (const QGAMES::Entities& ets)
{
	for (EnvironmentalConditions::const_iterator i = _conditions.begin ();
			i != _conditions.end (); i++)
		if (isActive ((*i))) (*i) -> affectArtists (ets);
}

// ---
void QGAMES::ComplexEnvironmentalCondition::stopAffectingArtists (const QGAMES::Entities& ets)
{
	for (EnvironmentalConditions::const_iterator i = _conditions.begin ();
			i != _conditions.end (); i++)
		if (isActive ((*i))) (*i) -> stopAffectingArtists (ets);
}

// ---
void QGAMES::ComplexEnvironmentalCondition::processEvent (const QGAMES::Event& evnt)
{
	QGAMES::EnvironmentalCondition::processEvent (evnt);

	notify (evnt); // Up...
}

// ---
bool QGAMES::ComplexEnvironmentalCondition::isActive (QGAMES::EnvironmentalCondition* cd)
{
	assert (cd);

	bool result = false;
	std::map <QGAMES::EnvironmentalCondition*, bool>::const_iterator i;
	if ((i = _conditionStatus.find (cd)) != _conditionStatus.end ())
		result = (*i).second;
	return (result);
}

// ---
void QGAMES::ComplexEnvironmentalCondition::deleteConditions ()
{
	for (EnvironmentalConditions::const_iterator i = _conditions.begin ();
			i != _conditions.end (); i++)
		delete ((*i));
	_conditions.clear ();
}

// ---
QGAMES::DensityFactorManager::DensityFactorManager ()
	: _targetDensity (__BD 1.0),
	  _initialFactor (__BD 1.0),
	  _finalFactor (__BD 1.0),
	  _incrementalFactor (__BD 0.1),
	  _numberOfThings (1),
	  _goingUp (true),
	  _currentFactor (__BD 1.0),
	  _thingsStatus ()
{
	// Nothing else to do
}

// ---
QGAMES::DensityFactorManager::DensityFactorManager (QGAMES::bdata d, 
		QGAMES::bdata iF, QGAMES::bdata fF, int nT, QGAMES::bdata i, bool up)
	: _targetDensity (d),
	  _initialFactor (iF),
	  _finalFactor (fF),
	  _incrementalFactor (i),
	  _numberOfThings (nT),
	  _goingUp (up),
	  _currentFactor (iF),
	  _thingsStatus ()
{
	assert (_targetDensity > 0);
	assert ((_goingUp && _initialFactor <= _finalFactor) ||
			(!_goingUp && _finalFactor <= _initialFactor));
	assert (_initialFactor >= 0 && _finalFactor >= 0);
	assert (_numberOfThings >= 0);
}

// ---
void QGAMES::DensityFactorManager::reverse ()
{
	QGAMES::bdata t = _initialFactor;
	_initialFactor = _finalFactor;
	_finalFactor = t;
	_goingUp = !_goingUp; // Reverse...
}

// ---
void QGAMES::DensityFactorManager::adaptTo (QGAMES::bdata iF, QGAMES::bdata fF, 
		QGAMES::bdata i, bool up)
{
	if (up)
	{
		if (_goingUp)
		{
			// Boths are going up...
			_initialFactor = std::min (_currentFactor, iF);
			_finalFactor = std::max (_currentFactor, fF);
		}
		else
		{
			// Original one is going down, and the modification is to going up!
			_initialFactor = std::min (_currentFactor, fF);
			_finalFactor = std::max (_currentFactor, iF);
		}
	}
	else
	{
		if (_goingUp)
		{
			// Original one is going up, and the modification is to going down!
			_initialFactor = std::max (_currentFactor, fF);
			_finalFactor = std::min (_currentFactor, iF);
		}
		else
		{
			// Boths are going down...
			_initialFactor = std::max (_currentFactor, iF);
			_finalFactor = std::min (_currentFactor, fF);
		}
	}

	_incrementalFactor = i;
	_goingUp = up;

	// Now it is time to see whether the density manager is set right or
	// it is needed to fix it...
	if ((_goingUp && (_initialFactor > _finalFactor)) ||
		(!_goingUp && (_initialFactor < _finalFactor))) _goingUp = !_goingUp;
}

// ---
void QGAMES::DensityFactorManager::initialize ()
{
	_currentFactor = _initialFactor;
	_thingsStatus = std::vector <bool> (_numberOfThings, _goingUp ? false : true);
	adjustThingsStatus ();
}

// ---
void QGAMES::DensityFactorManager::update ()
{
	_currentFactor += _incrementalFactor * (_goingUp ? 1 : -1);
	if (_goingUp && _currentFactor > _finalFactor) _currentFactor = _finalFactor;
	if (!_goingUp && _currentFactor < _initialFactor) _currentFactor = _initialFactor;
	adjustThingsStatus ();
}

// ---
void QGAMES::DensityFactorManager::adjustThingsStatus ()
{
	int cThings = std::count (_thingsStatus.begin (), _thingsStatus.end (), true);
	int nThings = (int) (_targetDensity * _currentFactor * __BD _numberOfThings);
	if (nThings == cThings)
		return; // Nothing has changed, so nothing to do...

	// If the number of things is 0...
	if (nThings == 0)
	{
		_thingsStatus = std::vector <bool> (_numberOfThings, false); // ...none is seen!
		return; // ...and return...
	}

	// In other case, the visible things are calculated...
	if (_goingUp)
	{
		int nT = (int) (_numberOfThings * (_targetDensity * _currentFactor));
		for (int i = 0; i < nT; i++)
			_thingsStatus [(int) (i / (_targetDensity * _currentFactor))] = true;
	}
	else
	{
		int nT = (int) (_numberOfThings * (_targetDensity * (_initialFactor - _currentFactor)));
		for (int i = 0; i < nT; i++)
			_thingsStatus [(int) (i / (_targetDensity * (_initialFactor - _currentFactor)))] = false;
	}
}
