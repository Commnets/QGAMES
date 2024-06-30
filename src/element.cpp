#include <Common/element.hpp>
#include <Common/game.hpp>

bool QGAMES::Element::ExternalData::storeInCache (const std::string& vN, const std::string& vV)
{
	bool result = true;

	std::map <std::string, std::string>::const_iterator i = _cache.find (vN);
	if (i == _cache.end ())
		_cache [vN] = vV; // The elements is new in the cache...
	else
	{
		if ((*i).second != vV)
			_cache [vN] = (*i).second; // The element has changed in the cache...
		else
			result = false; // No changes in the cache...
	}

	return (result);
}

// ---
QGAMES::Element::~Element () 
{ 
	deleteBuoys ();
	delete (_counters);
	_counters = NULL;
	delete (_onOffSwitches);
	_onOffSwitches = NULL;

	if (_externalEngine)
		_externalEngine -> whenDeleteElement (this);
	delete (_externalData);
	_externalData = NULL;
}

// ---
QGAMES::SetOfOpenValues QGAMES::Element::runtimeValues () const
{
	// There is no parent class...
	// This is the first one!

	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__));

	// Whether the element is or not visible...
	result.addOpenValue (0, QGAMES::OpenValue (_isVisible));

	// The counters...if any
	// The direct access to the variable _counters is used insted the internal method counters ()
	// This last method will try to create the counters, 
	// failling whether they haven't been defined by the player for this class.
	QGAMES::SetOfOpenValues cVV = _counters 
		? _counters -> asSetOfOpenValues () 
		: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESCOUNTERSTYPE__));
	result.addSetOfOpenValues (0, cVV);

	// Then the switches...
	// The direct access to the variable _onOffSwitches is used insted the internal method onOffSwitches ()
	// This last method will try to create the onOffSwitches, 
	// failling whether they haven't been defined by the player for this class.
	QGAMES::SetOfOpenValues sVV = _onOffSwitches
		? _onOffSwitches -> asSetOfOpenValues ()
		: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESSWITCHESTYPE__));
	result.addSetOfOpenValues (1, sVV);

	return (result);
}

// ---
void QGAMES::Element::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	assert (cfg.name () == std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__));

	assert (cfg.existOpenValue (0) &&
			cfg.existSetOfOpenValues (0) && cfg.existSetOfOpenValues (1));

	setVisible (cfg.openValue (0).boolValue ());

	// The counters & switches have to exist but also a value for them...
	// The counters and switched are not created at construction time usually
	// So, the runtime values could be saved when they are not created yet...
	QGAMES::SetOfOpenValues sVV = cfg.setOfOpenValues (1);
	assert (sVV.name () == std::string (__QGAMES_RUNTIMEVALUESSWITCHESTYPE__)); 
	if (_onOffSwitches && sVV.numberValues () != 0)
		_onOffSwitches -> fromSetOfOpenValues (sVV);

	QGAMES::SetOfOpenValues cVV = cfg.setOfOpenValues (0);
	assert (cVV.name () == std::string (__QGAMES_RUNTIMEVALUESCOUNTERSTYPE__)); 
	if (_counters && cVV.numberValues () != 0)
		_counters -> fromSetOfOpenValues (cVV);
}

// ---
QGAMES::Game* QGAMES::Element::game ()
{
	assert (QGAMES::Game::game ());

	return (QGAMES::Game::game ());
}

// ---
const QGAMES::Game* QGAMES::Element::game () const
{
	assert (QGAMES::Game::game ());

	return (QGAMES::Game::game ());
}

// ---
void QGAMES::Element::addBuoy (int id, QGAMES::Buoy* b)
{
	QGAMES::Buoys::iterator i = _buoys.find (id);
	assert (i == _buoys.end ());

	_buoys.insert (QGAMES::Buoys::value_type (id, b)); // Adds a new one...
}

// ---
void QGAMES::Element::addBuoys (QGAMES::Buoys b)
{
	for (QGAMES::Buoys::const_iterator i = b.begin (); i != b.end (); i++)
		addBuoy ((*i).first, (*i).second); // Adds one by one...they haven't to exit before!
}

// ---
void QGAMES::Element::setBuoys (QGAMES::Buoys b)
{
	deleteBuoys ();
	_buoys = b;
}

// ---
void QGAMES::Element::setBuoy (int id, QGAMES::Buoy* b)
{
	QGAMES::Buoys::iterator i = _buoys.find (id);
	assert (i != _buoys.end ());

	delete ((*i).second);
	(*i).second = b;// Replace
}

// ---
void QGAMES::Element::activeBuoy (int id, bool a)
{
	QGAMES::Buoys::const_iterator i = _buoys.find (id);
	assert (i != _buoys.end ());

	(*i).second -> active (a);
}

// ---
QGAMES::Buoy* QGAMES::Element::buoy (int id)
{
	QGAMES::Buoys::const_iterator i = _buoys.find (id);
	assert (i != _buoys.end ());

	return ((*i).second);
}

// ---
const QGAMES::Buoy* QGAMES::Element::buoy (int id) const
{
	QGAMES::Buoys::const_iterator i = _buoys.find (id);
	assert (i != _buoys.end ());

	return ((*i).second);
}

// ---
void QGAMES::Element::inEveryLoop ()
{
	for (QGAMES::Buoys::const_iterator i = _buoys.begin ();
			i != _buoys.end (); i++)
	{
		if ((*i).second -> isActive ())
		{
			(*i).second -> treatFor (this);
			(*i).second -> active (false); // desactive...
		}
	}

	if (_externalEngine) 
		_externalEngine -> whenInEveryLoop (this);
}

void QGAMES::Element::deleteBuoys ()
{
	for (QGAMES::Buoys::const_iterator i = _buoys.begin ();
			i != _buoys.end (); i++)
		delete (*i).second;
	_buoys.clear ();
}

// ---
void QGAMES::Element::setCounters (QGAMES::Counters* c)
{
	assert (c);

	delete (_counters);
	c -> initialize ();
	_counters = c;
}

// ---
QGAMES::Counters* QGAMES::Element::counters ()
{
	if (_counters == NULL)
		setCounters (createCounters ());
	return (_counters);
}

// ---
void QGAMES::Element::setOnOffSwitches (OnOffSwitches* s)
{
	assert (s);

	delete (_onOffSwitches);
	s -> initialize ();
	_onOffSwitches = s;
}

// ---
QGAMES::OnOffSwitches* QGAMES::Element::onOffSwitches ()
{
	if (_onOffSwitches == NULL)
		setOnOffSwitches (createOnOffSwitches ());
	return (_onOffSwitches);
}
