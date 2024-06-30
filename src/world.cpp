#include <Arcade/world.hpp>

// ---
QGAMES::World::World (int c, const Scenes& s, const WorldProperties& p)
	: QGAMES::Element (c),
	  _name (std::string (__NULL_STRING__)),
	  _scenes (s),
	  _properties (p),
	  _activeScene (NULL)
{
	// All scenes are observed...
	for (QGAMES::Scenes::const_iterator i = _scenes.begin (); 
			i != _scenes.end (); i++)
		observe ((*i).second);
}

// ---
QGAMES::World::~World ()
{
	for (Scenes::const_iterator i = _scenes.begin (); i != _scenes.end (); i++)
		delete ((*i).second);
	_scenes.clear ();
}

// ---
QGAMES::SetOfOpenValues QGAMES::World::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Element::runtimeValues ();

	int lNE = result.lastNestedOpenValueId ();

	// Saves the information of the scenes...
	QGAMES::SetOfOpenValues sVV (__QGAMES_RUNTIMEVALUESSCENESTYPE__);
	for (QGAMES::Scenes::const_iterator i = _scenes.begin (); i != _scenes.end (); i++)
		sVV.addSetOfOpenValues ((*i).second -> id (), (*i).second -> runtimeValues ());
	result.addSetOfOpenValues (lNE + 1, sVV);

	return (result);
}

// ---
void QGAMES::World::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	QGAMES::SetOfOpenValues sVV = cCfg.setOfOpenValues (lNE);
	assert (sVV.name () == std::string (__QGAMES_RUNTIMEVALUESSCENESTYPE__)); // Just to guaranttee the structure is going ok...
	for (QGAMES::Scenes::const_iterator i = _scenes.begin (); i != _scenes.end (); i++)
		// Only the scenes with info...
		if (sVV.existSetOfOpenValues ((*i).second -> id ()))
			(*i).second -> initializeRuntimeValuesFrom (sVV.setOfOpenValues ((*i).second -> id ()));
	cCfg.removeSetOfOpenValues (lNE);

	QGAMES::Element::initializeRuntimeValuesFrom (cCfg);
}

// ---
bool QGAMES::World::emptyPositionAt (const Position& p) const
{ 
	assert (_activeScene);

	return (_activeScene -> emptyPositionAt (p)); 
}

// ---
bool QGAMES::World::emptyBoxAt (const QGAMES::Position& p, const QGAMES::Position& s) const
{ 
	assert (_activeScene);

	return (_activeScene -> emptyBoxAt (p, s)); 
}

// ---
std::string QGAMES::World::property (const std::string& p) const
{ 
	std::string result (__NULL_STRING__);
	if (_properties.find (p) != _properties.end ()) result = (*_properties.find (p)).second;
	else if (_activeScene != NULL && _activeScene -> existsProperty (p)) result = _activeScene -> property (p);
	return (result);
}

// ---
bool QGAMES::World::existsProperty (const std::string& p) const
{
	bool result = false;
	if (_properties.find (p) != _properties.end ()) result = true;
	else if (_activeScene != NULL && _activeScene -> existsProperty (p)) result = true;
	return (result);
}

// ---
void QGAMES::World::setScene (int ns)
{
	// __MININT__ means none

	if (ns == __MININT__)
	{
		if (_activeScene != NULL)
		{
			unObserve (_activeScene);
			_activeScene -> setWorld (NULL);
			_activeScene = NULL;
		}
	}
	else
	{
		QGAMES::Scenes::const_iterator i = _scenes.find (ns);
		assert (i != _scenes.end ());
		if (_activeScene != NULL)
		{
			unObserve (_activeScene);
			_activeScene -> setWorld (NULL);
		}

		_activeScene = (*i).second;
		_activeScene -> setWorld (this);
		observe (_activeScene);
	}
}

// ---
QGAMES::Scene* QGAMES::World::scene (int ns)
{
	QGAMES::Scenes::iterator i = _scenes.find (ns);
	assert (i != _scenes.end ());

	return ((*i).second);
}

// ---
void QGAMES::World::initialize ()
{
	assert (_activeScene);

	_activeScene -> initialize ();
	QGAMES::Element::initialize ();
}

// ---
void QGAMES::World::inEveryLoop ()
{
	assert (_activeScene);

	_activeScene -> inEveryLoop ();
	QGAMES::Element::inEveryLoop ();
}

// ---
void QGAMES::World::updatePositions ()
{
	assert (_activeScene);

	_activeScene -> updatePositions ();
	if (_externalEngine)
		_externalEngine -> whenUpdatePositions (this);
}

// ---
void QGAMES::World::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (_activeScene);

	_activeScene -> drawOn (s, p);

	QGAMES::Element::drawOn (s, p);
}

// ---
void QGAMES::World::finalize ()
{
	if (_activeScene != NULL)
		_activeScene -> finalize ();

	QGAMES::Element::finalize ();
}
