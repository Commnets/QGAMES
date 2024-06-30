#include <Common/movementbuilder.hpp>
#include <Common/definitions.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/genalgorithms.hpp>
#include <Common/game.hpp>
#include <tinyxml.h>

// ---
QGAMES::MovementBuilder::MovementBuilder (const std::string& fDef)
	: _definitions (),
	  _setDefinitions (),
	  _movements (),
	  _definitionFile (fDef),
	  _basePath (QGAMES::onlyPath (fDef))
{
	if (_basePath == std::string (__NULL_STRING__)) 
		_basePath = std::string (".");

	readFile (__RL (_definitionFile));
}

// ---
QGAMES::MovementBuilder::~MovementBuilder ()
{
	QGAMES::Movements::const_iterator i;
	for (i = _movements.begin (); i != _movements.end (); i++)
		delete ((*i).second);
	_movements.clear ();
}

// ---
int QGAMES::MovementBuilder::numberResources () const
{
	int result = (int) _definitions.size ();
	for (QGAMES::MovementBuilder::Definitions::const_iterator i = _setDefinitions.begin ();
			i != _setDefinitions.end (); i++)
		result += (*i).second._number;
	return (result);
}

// ---
std::vector <int> QGAMES::MovementBuilder::resourcesId () const
{
	std::vector <int> result;
	for (QGAMES::MovementBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end (); i++)
		result.push_back ((*i).first);
	for (QGAMES::MovementBuilder::Definitions::const_iterator i = _setDefinitions.begin ();
			i != _setDefinitions.end (); i++)
		for (int j = 0; j < (*i).second._number; j++)
			result.push_back ((*i).second._id + j);
	return (result);
}

// ---
void QGAMES::MovementBuilder::preLoad ()
{
	// Load the movements...
	for (QGAMES::MovementBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end (); i++)
	{
		QGAMES::MovementBuilder::MovementDefinition def = (*i).second;
		if (_movements.find (def._id) == _movements.end ()) movement (def._id);
	}

	// Load the set of movements...
	for (QGAMES::MovementBuilder::Definitions::const_iterator i = _setDefinitions.begin ();
			i != _setDefinitions.end (); i++)
	{
		QGAMES::MovementBuilder::MovementDefinition def = (*i).second;
		for (int j = def._id; j < (def._id + def._number); j++)
			if (_movements.find (j) == _movements.end ()) movement (j);
	}
}

QGAMES::Movement* QGAMES::MovementBuilder::movement (int nm)
{
	QGAMES::Movements::const_iterator i = _movements.find (nm);
	if (i != _movements.end ())
		return ((*i).second);

	__WLOGPLUS (std::string ("Loading movement: ") + std::to_string (nm));
	
	// Look for a simple definition...
	QGAMES::Movement* result = NULL;
	QGAMES::MovementBuilder::Definitions::const_iterator j = _definitions.find (nm);
	QGAMES::MovementBuilder::MovementDefinition resultDef =
		(j == _definitions.end ()) ? groupDefinitionFor (nm) : (*j).second;
	assert (resultDef._id != -1); // Just in case, because the result of the groupDefinitionFor could be -1
	result = createMovement (resultDef);
	// createMovement is an user's method, ant it could fail...
	assert (result);
	// If it is ok, then the new movement is inserted into the list of total movements...
	_movements.insert (QGAMES::Movements::value_type (nm, result));
	// ...and returned to be used.
	return (result);
}

// ---
void QGAMES::MovementBuilder::readFile (const std::string& fDef)
{
	__WLOGPLUS (std::string ("Reading movements definition file: ") + fDef);

	TiXmlDocument doc (fDef.c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the root element doesn't call "Movements" the code will work too!
	// This is done to speed up the loading...

	for (TiXmlElement* defElement = rootElement -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		// The line can be an include line...
		// That line can be at any place in the movement definition file...
		if (strcmp (defElement -> Value (), __QGAMES_INCLUDENAME__) == 0)
		{
			assert (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)); 
			readFile (__RL (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)));
		}
		else
		// The other type of line can be a simple movement line...
		if (strcmp (defElement -> Value (), __QGAMES__MOVEMENTTAGNAME__) == 0)
		{
			QGAMES::MovementBuilder::MovementDefinition def = 
				readMovementDefinition (defElement);
			assert (def._id != -1); // Is it a valid movement?
			_definitions.insert (QGAMES::MovementBuilder::Definitions::value_type (def._id, def));
		}
		else
		// But also a complex definition
		if (strcmp (defElement -> Value (), __QGAMES__SETMOVEMENTSTAGNAME__) == 0)
		{
			QGAMES::MovementBuilder::MovementDefinition def = 
				readSetOfMovementsDefinition (defElement);
			assert (def._id != -1 && def._number != 0); // Is it a valid movement?
			_setDefinitions.insert (QGAMES::MovementBuilder::Definitions::value_type (def._id, def));
		}
	}
}

// ---
QGAMES::MovementBuilder::MovementDefinition 
	QGAMES::MovementBuilder::readMovementDefinition (TiXmlElement* m)
{
	QGAMES::MovementBuilder::MovementDefinition result;
	m -> Attribute (__QGAMES__MOVEMENTATTRID__, &result._id);
	for (TiXmlElement* dataElement = m -> FirstChildElement ();
		dataElement != NULL; dataElement = dataElement -> NextSiblingElement ())
	{
		// If the element doesn't call "Movement" the code will work too!
		// This is done to speed up the loading...

		int id = -1; double value;
		dataElement -> Attribute (__QGAMES__MOVEMENTPARAMATTRID__, &id);
		dataElement -> Attribute (__QGAMES__MOVEMENTPARAMATTRVALUE__, &value);
		if (id != -1)
			result._variables.insert (std::map <int, double>::value_type (id, value));
	}

	result._type = QGAMES::MovementBuilder::MovementDefinition::Type::_SIMPLE;

	return (result);
}

// ---
QGAMES::MovementBuilder::MovementDefinition 
	QGAMES::MovementBuilder::readSetOfMovementsDefinition (TiXmlElement* m)
{
	QGAMES::MovementBuilder::MovementDefinition result;
	m -> Attribute (__QGAMES__SETOFMOVEMENTSATTRID__, &result._id);
	m -> Attribute (__QGAMES__SETOFMOVEMENTSATTRNUMBER__, &result._number);
	for (TiXmlElement* dataElement = m -> FirstChildElement ();
		dataElement != NULL; dataElement = dataElement -> NextSiblingElement ())
	{
		// If the root element doesn't call "Variable" the code will work too!
		// This is done to speed up the loading...

		int id = -1; double value;
		dataElement -> Attribute (__QGAMES__MOVEMENTPARAMATTRID__, &id);
		dataElement -> Attribute (__QGAMES__MOVEMENTPARAMATTRVALUE__, &value);
		if (id != -1)
			result._variables.insert (std::map <int, double>::value_type (id, value));
	}

	result._type = QGAMES::MovementBuilder::MovementDefinition::Type::_SET;

	return (result);
}

// ---
QGAMES::MovementBuilder::MovementDefinition QGAMES::MovementBuilder::groupDefinitionFor (int nm)
{
	QGAMES::MovementBuilder::MovementDefinition result;
	for (QGAMES::MovementBuilder::Definitions::const_iterator i = _setDefinitions.begin (); 
			i != _setDefinitions.end () && result._id == -1; i++)
	{
		QGAMES::MovementBuilder::MovementDefinition def = (*i).second;
		if (nm >= def._id && nm < (def._id + def._number))
		{
			result._id = nm;
			result._type = QGAMES::MovementBuilder::MovementDefinition::Type::_SIMPLE;
			result._number = 0;
			result._variables = def._variables;
		}
	}

	return (result);
}
