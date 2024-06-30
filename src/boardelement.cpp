#include <Board/boardelement.hpp>
#include <Board/definitions.hpp>
#include <assert.h>

// ---
QGAMES::BoardElement::BoardElement (int id, const QGAMES::BoardResources& rscs, 
	const QGAMES::BoardElementProperties& pts)
			: QGAMES::Element (id),
			  _resources (rscs),
			  _properties ()
{
	for (QGAMES::BoardResources::const_iterator i = _resources.begin ();
			i != _resources.end (); i++)
	{
		observe ((*i).second); // The resources are initially observed by the element (owner)
		(*i).second -> setBoardElementSimple (this); // ...the resource is assigned to this element
	}
}

// ---
QGAMES::BoardElement::~BoardElement ()
{
	for (QGAMES::BoardResources::const_iterator i = _resources.begin ();
			i != _resources.end (); i++)
		delete ((*i).second);
	_resources.clear ();
}

// ---
QGAMES::SetOfOpenValues QGAMES::BoardElement::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Element::runtimeValues ();

	int lNE = result.lastNestedOpenValueId ();

	// Just the information comming from the resources...
	QGAMES::SetOfOpenValues rVV (std::string (__QGAMES_RUNTIMEVALUESRESOURCESTYPE__));
	for (QGAMES::BoardResources::const_iterator i = _resources.begin (); i != _resources.end (); i++)
		rVV.addSetOfOpenValues ((*i).second -> id (), (*i).second -> runtimeValues ());
	result.addSetOfOpenValues (lNE + 1, rVV);

	return (result);
}

// ---
void QGAMES::BoardElement::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	QGAMES::SetOfOpenValues rVV = cCfg.setOfOpenValues (lNE);
	assert (rVV.name () == std::string (__QGAMES_RUNTIMEVALUESRESOURCESTYPE__)); // To guaranttee the structure is being built ok...
	for (QGAMES::BoardResources::const_iterator i = _resources.begin (); i != _resources.end (); i++)
	{
		// Only if the resource exists...
		if (rVV.existSetOfOpenValues ((*i).second -> id ()))
			(*i).second -> initializeRuntimeValuesFrom (rVV.setOfOpenValues ((*i).second -> id ()));
	}

	cCfg.removeSetOfOpenValues (lNE);

	QGAMES::Element::initializeRuntimeValuesFrom (cCfg);
}

// ---
QGAMES::BoardResource* QGAMES::BoardElement::firstResource ()
{
	assert (!_resources.empty ());

	return ((*_resources.begin ()).second);
}

// ---
QGAMES::BoardResource* QGAMES::BoardElement::resource (int id)
{
	QGAMES::BoardResources::const_iterator i = _resources.find (id);
	assert (i != _resources.end ()); // It has to exists...

	return ((*i).second);
}

// ---
QGAMES::BoardResource* QGAMES::BoardElement::resource (const QGAMES::BoardElementProperties& pts)
{
	QGAMES::BoardResource* result = NULL;
	for (QGAMES::BoardResources::const_iterator i = _resources.begin ();
			i != _resources.end () && !result; i++)
		if ((*i).second -> matchProperties (pts))
			result = (*i).second;

	return (result); // Can be null, if none matches the properties...
}

// ---
const std::string& QGAMES::BoardElement::property (const std::string& n) const
{
	QGAMES::BoardElementProperties::const_iterator i = _properties.find (n);
	assert (i != _properties.end ());

	return ((*i).second);
}

// ---
bool QGAMES::BoardElement::matchProperties (const QGAMES::BoardElementProperties& pts) const
{
	bool result = true;
	for (QGAMES::BoardElementProperties::const_iterator i = pts.begin ();
			i != pts.end () && result; i++)
		result &= (existProperty ((*i).first) && (*i).second == property ((*i).first));
	return (result);
}

// ---
void QGAMES::BoardElement::initialize ()
{
	QGAMES::Element::initialize ();
	for (QGAMES::BoardResources::const_iterator i = _resources.begin ();
			i != _resources.end (); i++)
		(*i).second -> initialize ();
}

// ---
void QGAMES::BoardElement::inEveryLoop ()
{
	QGAMES::Element::inEveryLoop ();
	for (QGAMES::BoardResources::const_iterator i = _resources.begin ();
			i != _resources.end (); i++)
		(*i).second -> inEveryLoop ();
}

// ---
void QGAMES::BoardElement::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	for (QGAMES::BoardResources::const_iterator i = _resources.begin ();
			i != _resources.end (); i++)
		(*i).second -> drawOn (s, p);
	QGAMES::Element::drawOn (s, p);
}

// ---
void QGAMES::BoardElement::finalize ()
{
	QGAMES::Element::finalize ();
	for (QGAMES::BoardResources::const_iterator i = _resources.begin ();
			i != _resources.end (); i++)
		(*i).second -> finalize ();
}

// ---
void QGAMES::BoardElement::assignResource (QGAMES::BoardResource* rsce)
{
	assert (rsce); // It has to be no null...
	QGAMES::BoardResources::const_iterator i = _resources.find (rsce -> id ());
	assert (i == _resources.end ()); // It has not to exist already...

	_resources.insert (BoardResources::value_type (rsce -> id (), rsce));
	observe (rsce); // The resource has to be observe...

	whenAssignResource (rsce);
}

// ---
void QGAMES::BoardElement::desassignResource (QGAMES::BoardResource* rsce)
{
	assert (rsce); // It has to be no null...

	QGAMES::BoardResources::const_iterator i = _resources.find (rsce -> id ());
	assert (i != _resources.end ()); // It has to exist already...

	_resources.erase (i);
	unObserve (rsce); // The resource is not observe any more...

	whenDesassignResource (rsce);
}
