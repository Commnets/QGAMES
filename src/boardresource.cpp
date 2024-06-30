#include <Board/boardresource.hpp>
#include <Board/boardelement.hpp>
#include <Board/boardsquare.hpp>
#include <assert.h>

// ---
QGAMES::SetOfOpenValues QGAMES::BoardResource::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Element::runtimeValues ();

	int lNE = result.lastNestedOpenValueId ();

	// The information of the resource if any, or empty if none...
	result.addSetOfOpenValues (lNE + 1, 
		_boardElement ? _boardElement -> runtimeValues () 
			: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__)));
	// ...and the one comming from the square if any, or empty if none...
	result.addSetOfOpenValues (lNE + 2,
		_boardSquare ? _boardSquare -> runtimeValues () 
			: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__)));

	return (result);
}

// ---
void QGAMES::BoardResource::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existSetOfOpenValues (lNE) && cfg.existSetOfOpenValues (lNE - 1));

	QGAMES::SetOfOpenValues cCfg = cfg;

	// The boardSquare can be set later, after the runtime values were got
	// So, now the boardSquare can exist but not necessarily its runtime values!
	QGAMES::SetOfOpenValues bSVV = cCfg.setOfOpenValues (lNE);
	assert (bSVV.name () == std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__));
	if (_boardSquare && bSVV.numberValues () != 0)
		_boardSquare ->  initializeRuntimeValuesFrom (bSVV);
	cCfg.removeSetOfOpenValues (lNE);

	// The boardElement can be set later, after the runtime values were got
	// So, now the boardElement can exist but not necessarily its runtime values!
	QGAMES::SetOfOpenValues bEVV = cCfg.setOfOpenValues (lNE - 1);
	assert (bEVV.name () == std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__));
	if (_boardElement && bEVV.numberValues () != 0)
		_boardElement -> initializeRuntimeValuesFrom (bEVV);
	cCfg.removeSetOfOpenValues (lNE - 1);

	QGAMES::Element::initializeRuntimeValuesFrom (cCfg);
}

// ---
const std::string& QGAMES::BoardResource::property (const std::string& n) const
{
	QGAMES::BoardElementProperties::const_iterator i = _properties.find (n);
	assert (i != _properties.end ());

	return ((*i).second);
}

// ---
bool QGAMES::BoardResource::matchProperties (const QGAMES::BoardResourceProperties& pts) const
{
	bool result = true;
	for (QGAMES::BoardElementProperties::const_iterator i = pts.begin ();
			i != pts.end () && result; i++)
		result &= (existProperty ((*i).first) && (*i).second == property ((*i).first));
	return (result);
}

// ---
void QGAMES::BoardResource::setBoardElement (QGAMES::BoardElement* elmnt)
{
	assert (elmnt);

	// If it is the same element, nothing happens...
	if (_boardElement == elmnt)
		return;

	// If the resource belonged to an element already...
	if (_boardElement != NULL)
	{
		// It has to be desassign this resource
		_boardElement -> desassignResource (this); 
		// ..and something can happen either in the resource or in the element...
		whenDesassignedTo (_boardElement); 
	}
	
	// The new assignation...
	_boardElement = elmnt; 
	// It has to be assigned to the element...
	_boardElement -> assignResource (this); 
	// ...and something can happen either in the element or in the resource...
	whenAssignedTo (_boardElement); 
}

// ---
void QGAMES::BoardResource::setBoardElementSimple (QGAMES::BoardElement* elmnt)
{
	assert (elmnt);

	_boardElement = elmnt;
}

// ---
void QGAMES::BoardResource::setBoardSquare (QGAMES::BoardSquare* bs)
{
	// The resource still belongs to the same element,
	// But is is located at an specific square

	// If the square is the same than before, nothing happens...
	if (_boardSquare == bs)
		return;

	// If the element is in a square already...
	if (_boardSquare != NULL)
	{
		// The resource is not observed by the the square it was in any more...
		_boardSquare -> unObserve (this); 
		// Something can happen either or in the square or in the resource...
		whenExitingSquare (_boardSquare); 
	}

	// The new assignation
	_boardSquare = bs;
	if (_boardSquare != NULL)
	{
		// Just if it is not null, the resource is observed by the square it is in now...
		_boardSquare -> observe (this); 
		// ...and something can happen either in the square or in the new owner.
		whenEnteringSquare (_boardSquare); 
	}
}

// ---
void QGAMES::BoardResource::initialize ()
{
	QGAMES::Element::initialize ();
	state () -> initialize (); // getting the state the default one is created, 
							   // if nothing existed before
}
