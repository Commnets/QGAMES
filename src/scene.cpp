#include <Arcade/scene.hpp>
#include <algorithm>

// ---
QGAMES::SetOfOpenValues QGAMES::SceneActionBlock::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Element::runtimeValues ();

	int lE = result.lastOpenValueId ();

	// Whether the block is or not active...
	result.addOpenValue (lE + 1, QGAMES::OpenValue (_active));

	return (result);
}

// ---
void QGAMES::SceneActionBlock::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	setActive (cCfg.openValue (lE).boolValue ()); // Because there could be derivated actions...
	cCfg.removeOpenValue (lE);

	QGAMES::Element::initializeRuntimeValuesFrom (cCfg);
}

// ---
QGAMES::Scene::Scene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn, 
	const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
	: QGAMES::Element (c),
	  _name (std::string (__NULL_STRING__)),
	  _world (NULL),
	  _environmentalCondition (NULL),
	  _maps (m),
	  _connections (cn),
	  _properties (p),
	  _activeMap (NULL),
	  _entities (),
	  _entitiesPerLayer (ePL),
	  _rebuiltMapForDrawing (true) // First time the map will be rebuilt!
{
	for (Maps::const_iterator i = _maps.begin (); i != _maps.end (); i++)
		observe ((*i).second);
}

// ---
QGAMES::Scene::~Scene ()
{
	for (QGAMES::Maps::const_iterator i = _maps.begin (); i != _maps.end (); i++)
		unObserve ((*i).second);
	_maps.clear ();

	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin (); 
			i != _actionBlocks.end (); delete ((*i++))); // Own the blocks...
	_actionBlocks.clear ();

	delete (_environmentalCondition);
	_environmentalCondition = NULL;
}

// ---
QGAMES::SetOfOpenValues QGAMES::Scene::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Element::runtimeValues ();

	int lNE = result.lastNestedOpenValueId ();

	// Saves the information of the maps...
	QGAMES::SetOfOpenValues mVV (__QGAMES_RUNTIMEVALUESMAPSTYPE__);
	for (QGAMES::Maps::const_iterator i = _maps.begin (); i != _maps.end (); i++)
		mVV.addSetOfOpenValues ((*i).second -> id (), (*i).second -> runtimeValues ());
	result.addSetOfOpenValues (lNE + 1, mVV); // It could be empty but it will exist anyway...
	
	// Saves also the information of the scene blocks...
	QGAMES::SetOfOpenValues bVV (__QGAMES_RUNTIMEVALUESBLOCKSTYPE__);
	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin (); 
			i != _actionBlocks.end (); i++)
		bVV.addSetOfOpenValues ((*i) -> id (), (*i) -> runtimeValues ());
	result.addSetOfOpenValues (lNE + 2, bVV); // It could be empty, but it will exist anyway...

	return (result);
}

// ---
void QGAMES::Scene::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existSetOfOpenValues (lNE) && 
			cfg.existSetOfOpenValues (lNE - 1));

	QGAMES::SetOfOpenValues cCfg = cfg;

	QGAMES::SetOfOpenValues mVV = cCfg.setOfOpenValues (lNE - 1);
	assert (mVV.name () == std::string (__QGAMES_RUNTIMEVALUESMAPSTYPE__)); // Just to guaranttee the structure is going ok...
	for (QGAMES::Maps::const_iterator i = _maps.begin (); i != _maps.end (); i++)
		// Only the maps with info
		if (mVV.existSetOfOpenValues ((*i).second -> id ()))
			(*i).second -> initializeRuntimeValuesFrom (mVV.setOfOpenValues ((*i).second -> id ()));
	cCfg.removeSetOfOpenValues (lNE -1);

	// Blocks are actualized after maps, because they (maps) can be affected by activation / desactivation...
	QGAMES::SetOfOpenValues bVV = cCfg.setOfOpenValues (lNE);
	assert (bVV.name () == std::string (__QGAMES_RUNTIMEVALUESBLOCKSTYPE__)); // Just to guaranttee the structure is going ok...
	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin ();
			i != _actionBlocks.end (); i++)
		// Only the blocks with info...
		if (bVV.existSetOfOpenValues ((*i) -> id ()))
			(*i) -> initializeRuntimeValuesFrom (bVV.setOfOpenValues ((*i) -> id ()));
	cCfg.removeSetOfOpenValues (lNE);

	QGAMES::Element::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::Scene::setEnvironmentalCondition (QGAMES::EnvironmentalCondition* ecd)
{
	// ecd == NULL means nothing...

	// Stops the old condition... if any
	if (_environmentalCondition != NULL)
	{
		_environmentalCondition -> finalize ();

		unObserve (_environmentalCondition);
	}
	
	// Assign the new...
	delete _environmentalCondition; // Deletes the previous one...
	_environmentalCondition = ecd;
	if (_environmentalCondition != NULL)
	{
		observe (_environmentalCondition);

		_environmentalCondition -> initialize ();
	}
}

// ---
bool QGAMES::Scene::emptyPositionAt (const QGAMES::Position& p) const
{ 
	assert (_activeMap);

	return (_activeMap -> emptyPositionAt (p)); 
}

// ---
bool QGAMES::Scene::emptyBoxAt (const QGAMES::Position& p, const QGAMES::Position& s) const
{ 
	assert (_activeMap);

	return (_activeMap -> emptyBoxAt (p, s)); 
}

// ---
std::string QGAMES::Scene::property (const std::string& p) const
{ 
	std::string result (__NULL_STRING__);
	if(_properties.find (p) != _properties.end ()) result = (*_properties.find (p)).second;
	else if (_activeMap != NULL && _activeMap -> existsProperty (p)) result = _activeMap -> property (p);
	return (result);
}

// ---
bool QGAMES::Scene::existsProperty (const std::string& p) const
{
	bool result = false;
	if (_properties.find (p) != _properties.end ()) result = true;
	else if (_activeMap != NULL && _activeMap -> existsProperty (p)) result = true;
	return (result);
}

// ---
void QGAMES::Scene::setMap (int nm)
{
	// ___MININT__ means none...

	if (nm == __MININT__)
	{
		if (_activeMap != NULL)
		{
			unObserve (_activeMap);
			_activeMap -> setScene (NULL);
			_activeMap = NULL;
		}
	}
	else
	{
		QGAMES::Maps::const_iterator i = _maps.find (nm);
		assert (i != _maps.end ());
		if (_activeMap != NULL)
		{
			unObserve (_activeMap);
			_activeMap -> setScene (NULL);
		}

		_activeMap = (*i).second;
		_activeMap -> setScene (this);
		observe (_activeMap);
	}
}

// ---
QGAMES::Map* QGAMES::Scene::map (int nm)
{
	QGAMES::Maps::iterator i = _maps.find (nm);
	assert (i != _maps.end ());

	return ((*i).second);
}

// ---
bool QGAMES::Scene::existsConnection (int nc)
{
	return (_connections.find (nc) != _connections.end ());
}

// ---
const QGAMES::Scene::Connection& QGAMES::Scene::connection (int nc)
{
	QGAMES::Scene::Connections::const_iterator i = _connections.find (nc);
	assert (i != _connections.end ());

	return ((*i).second);
}

// ---
void QGAMES::Scene::addConnection (int nc, const QGAMES::Scene::Connection& dC)
{
	assert (dC._toScene); // Can't be NULL

	QGAMES::Scene::Connections::iterator i = _connections.find (nc);
	// Only one single connection with the same id can exist simultaneously...
	if (i != _connections.end ())
		_connections.erase (i); 
	_connections.insert (QGAMES::Scene::Connections::value_type (nc, dC));
}

// ---
void QGAMES::Scene::removeConnection (int nc)
{
	QGAMES::Scene::Connections::const_iterator i = _connections.find (nc);
	assert (i != _connections.end ());
	_connections.erase (i);
}

// ---
std::vector <int> QGAMES::Scene::entitiesId () const
{
	std::vector <int> result;
	for (QGAMES::Entities::const_iterator i = _entities.begin (); i != _entities.end (); i++)
		result.push_back ((*i).first);
	return (result);
}

// ---
QGAMES::Entity* QGAMES::Scene::entity (int ne)
{
	QGAMES::Entities::const_iterator i = _entities.find (ne);
	assert (i != _entities.end ());

	return ((*i).second);
}

// ---
void QGAMES::Scene::addEntity (QGAMES::Entity* e)
{
	assert (e);

	// The scene initiation can be invoked many times...
	// so this method works a little bit different...
	// If the entity has already been added, then it does nothing!
	QGAMES::Entities::const_iterator i = _entities.find (e -> id ());
	if (i != _entities.end ()) 
		return;

	observe (e); // To observe what happens in the entity...
	_entities.insert (QGAMES::Entities::value_type (e -> id (), e));
}

// ---
void QGAMES::Scene::removeEntity (QGAMES::Entity* e)
{
	assert (e);

	QGAMES::Entities::iterator i = _entities.find (e -> id ());
	assert (i != _entities.end ()); 

	unObserve (e); // Just in case...
	_entities.erase (i);
}

// ---
void QGAMES::Scene::removeAllEntities ()
{
	for (QGAMES::Entities::iterator i = _entities.begin (); i != _entities.end (); _entities.erase (i++));
	_entities.clear ();
}

// ---
std::vector <int> QGAMES::Scene::charactersId () const
{
	std::vector <int> result;
	for (QGAMES::Entities::const_iterator i = _characteres.begin (); i != _characteres.end (); i++)
		result.push_back ((*i).first);
	return (result);
}

// ---
QGAMES::Entity* QGAMES::Scene::character (int nc)
{
	QGAMES::Entities::const_iterator i = _characteres.find (nc);
	assert (i != _characteres.end ());

	return ((*i).second);
}

// ---
void QGAMES::Scene::addCharacter (QGAMES::Character* c)
{
	assert (c);

	// The scene initiation can be invoked many times...
	// so this method works a little bit different...
	// If the character has already been added, then it does nothing!
	QGAMES::Entities::const_iterator i = _characteres.find (c -> id ());
	if (i != _characteres.end ()) 
		return;

	observe (c); // To observe what happens in the character...
	_characteres.insert (QGAMES::Entities::value_type (c -> id (), c));
}

// ---
void QGAMES::Scene::removeCharacter (QGAMES::Character* c)
{
	assert (c);

	QGAMES::Entities::iterator i = _characteres.find (c -> id ());
	assert (i != _characteres.end ());

	unObserve (c); // Just in case...
	_characteres.erase (i);
}

// ---
void QGAMES::Scene::removeAllCharacteres ()
{
	for (QGAMES::Entities::iterator i = _characteres.begin (); 
			i != _characteres.end (); _characteres.erase (i++));
	_characteres.clear ();
}

// ---
bool QGAMES::Scene::existsActionBlock (int na) const
{
	bool result = false;
	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin (); 
			i != _actionBlocks.end () && !result; result = ((*i++) -> id () == na));
	return (result);
}

// ---
QGAMES::SceneActionBlock* QGAMES::Scene::actionBlock (int na)
{
	QGAMES::SceneActionBlock* result = NULL;
	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin (); 
			i != _actionBlocks.end () && !result; i++)
		if ((*i) -> id () == na) result = (*i);
	return (result);
}

// ---
void QGAMES::Scene::addActionBlock (QGAMES::SceneActionBlock* aB, bool in)
{
	assert (aB);

	bool found = false;
	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin (); 
			i != _actionBlocks.end () && !found; i++)
		if ((*i) -> id () == aB -> id ()) found = true;

	if (!found)
	{
		aB -> setScene (this);
		observe (aB); // Just in case...
		if (in) aB -> initialize (); // Only if it has been defined...

		_actionBlocks.push_back (aB);
	}
}

// ---
void QGAMES::Scene::removeActionBlock (int aId)
{
	QGAMES::SceneActionBlock* toRemove = NULL;
	
	bool found = false;
	QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin ();
	while (i != _actionBlocks.end () && !found)
	{
		if ((*i) -> id () == aId)
			found = true;
		else 
			i++;
	}

	if (found)
	{
		(*i) -> finalize ();
		unObserve ((*i));

		delete ((*i));
		_actionBlocks.erase (i);
	}
}

// ---
void QGAMES::Scene::initialize ()
{
	assert (_activeMap);

	_activeMap -> initialize ();
	for (QGAMES::Entities::const_iterator i = _entities.begin (); i != _entities.end (); i++)
		(*i).second -> initialize ();
	for (QGAMES::Entities::const_iterator i = _characteres.begin (); i != _characteres.end (); i++)
		(*i).second -> initialize ();
	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin (); i != _actionBlocks.end (); i++)
		(*i) -> initialize ();

	// It is not neccessary to initialize the environmental condition
	// If it exists, because it has been initialized when set up!

	QGAMES::Element::initialize ();
}

// ---
void QGAMES::Scene::inEveryLoop ()
{
	assert (_activeMap);

	_activeMap -> inEveryLoop ();

	for (QGAMES::Entities::const_iterator i = _entities.begin (); i != _entities.end (); i++)
		(*i).second -> inEveryLoop ();
	for (QGAMES::Entities::const_iterator i = _characteres.begin (); i != _characteres.end (); i++)
		(*i).second -> inEveryLoop ();
	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin (); i != _actionBlocks.end (); i++)
		if ((*i) -> isActive ()) (*i) -> inEveryLoop ();

	if (_environmentalCondition)
		_environmentalCondition -> inEveryLoop (); // Affects per loop...

	QGAMES::Element::inEveryLoop ();
}

// ---
void QGAMES::Scene::updatePositions ()
{
	assert (_activeMap);

	_activeMap -> updatePositions ();

	for (QGAMES::Entities::const_iterator i = _entities.begin (); i != _entities.end (); i++)
		(*i).second -> updatePositions ();
	for (QGAMES::Entities::const_iterator i = _characteres.begin (); i != _characteres.end (); i++)
		(*i).second -> updatePositions ();
	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin (); i != _actionBlocks.end (); i++)
		if ((*i) -> isActive ()) (*i) -> updatePositions ();

	if (_environmentalCondition != NULL)
	{
		_environmentalCondition -> updatePositions ();

		if (!_environmentalCondition -> affectOnlyOnce ())
		{
			_environmentalCondition -> affectArtists (entities ());
			_environmentalCondition -> affectArtists (characters ());
		}
	}

	if (_externalEngine)
		_externalEngine -> whenUpdatePositions (this);
}

// ---
void QGAMES::Scene::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);
	assert (_activeMap);

	// Draws the map, with the right entities taken into account...
	if (_activeMap -> isVisible ())
	{
		_activeMap -> considerEntitiesForDrawing (_entities, _characteres, _entitiesPerLayer, _rebuiltMapForDrawing); 
		_activeMap -> drawOn (s, p);
		_rebuiltMapForDrawing = false; // Already used...
	}

	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin (); i != _actionBlocks.end (); i++)
		if ((*i) -> isActive ()) (*i) -> drawOn (s, p);

	if (_environmentalCondition != NULL && _environmentalCondition -> isVisible ()) 
		_environmentalCondition -> drawOn (s, p);

	QGAMES::Element::drawOn (s, p);
}

// ---
void QGAMES::Scene::finalize ()
{
	if (_activeMap)
		_activeMap -> finalize ();
	
	for (QGAMES::Entities::const_iterator i = _entities.begin (); i != _entities.end (); i++)
		(*i).second -> finalize ();
	for (QGAMES::Entities::const_iterator i = _characteres.begin (); i != _characteres.end (); i++)
		(*i).second -> finalize ();
	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin (); i != _actionBlocks.end (); i++)
		(*i) -> finalize ();

	// If conditions finalize them...
	if (_environmentalCondition != NULL)
		_environmentalCondition -> finalize ();

	QGAMES::Element::finalize ();
}

// ---
void QGAMES::Scene::processEvent (const QGAMES::Event& e)
{
	if (e.code () == __QGAMES_ENVCONDITIONSTARTED__)
	{
		QGAMES::EnvironmentalCondition* eC = ((QGAMES::EnvironmentalCondition*) e.data ());
		if (eC -> affectOnlyOnce ())
		{
			eC -> affectArtists (entities ());
			eC -> affectArtists (characters ());
		}
	}
	else
	if (e.code () == __QGAMES_ENVCONDITIONFINISHED__)
	{
		((QGAMES::EnvironmentalCondition*) e.data ()) -> stopAffectingArtists (entities ());
		((QGAMES::EnvironmentalCondition*) e.data ()) -> stopAffectingArtists (characters ());
	}
	else
	if (e.code () == __QGAMES_POSITIONCHANGED__)
		_rebuiltMapForDrawing = true; 
	// The position of an important element has changed...probably the map has to be rebuilt...
	
	QGAMES::Element::processEvent (e);
}
