#include <Board/boardbuilder.hpp>
#include <Board/definitions.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/genalgorithms.hpp>
#include <Common/game.hpp>

// ---
QGAMES::BoardBuilder::BoardBuilder (const std::string& fDef)
	: _definitions (),
	  _boards (),
	  _definitionFile (fDef),
	  _basePath (QGAMES::onlyPath (fDef))
{
	if (_basePath == std::string (__NULL_STRING__)) 
		_basePath = std::string (".");

	__WLOGPLUS (std::string ("Reading boards definition file: ") + fDef);

	TiXmlDocument doc (__RL (_definitionFile).c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the main element doesn't call "Boards" the system keep working!

	for (TiXmlElement* defElement = rootElement -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		QGAMES::BoardBuilder::BoardDefinition def = 
			readBoardDefinition (defElement);
		assert (def._id != -1);
		_definitions.insert (QGAMES::BoardBuilder::Definitions::value_type (def._id, def));
	}
}

// ---
QGAMES::BoardBuilder::~BoardBuilder ()
{
	for (QGAMES::Boards::const_iterator i = _boards.begin (); i != _boards.end (); i++)
		delete ((*i).second); // Deletes the boards built...
	_boards.clear ();

	for (QGAMES::BoardBuilder::AddsOnList::const_iterator j = _addsOn.begin (); 
			j != _addsOn.end (); j++)
		delete ((*j)); // Deletes the adds on needed to create the boards...
	_addsOn.clear ();
}

// ---
void QGAMES::BoardBuilder::addAddsOn (QGAMES::BoardBuilder::AddsOn *aO)
{
	assert (aO); // It has to be not null...

	bool found = false;
	for (QGAMES::BoardBuilder::AddsOnList::const_iterator i = _addsOn.begin ();
			i != _addsOn.end () && !found; i++)
		if ((*i) == aO)
			found = true;
	assert (!found); // It hasn't to exist before...

	_addsOn.push_back (aO); 
}
// ---
void QGAMES::BoardBuilder::preLoad ()
{
	for (QGAMES::BoardBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end (); i++)
		if (_boards.find ((*i).second._id) == _boards.end ())
			board ((*i).second._id);
}

// ---
QGAMES::Board* QGAMES::BoardBuilder::board (int nb)
{
	QGAMES::Boards::const_iterator i = _boards.find (nb);
	if (i != _boards.end ())
		return ((*i).second); // If the board has already been created, then it is returned...

	__WLOGPLUS (std::string ("Loading board: ") + std::to_string (nb));

	QGAMES::BoardBuilder::Definitions::const_iterator j = _definitions.find (nb);
	assert (j != _definitions.end ()); // Is doesn't exist...

	// Looks for the right addOn to create the board,
	// and create the board...
	QGAMES::BoardBuilder::AddsOn* addOn = NULL;
	for (QGAMES::BoardBuilder::AddsOnList::const_iterator k = _addsOn.begin ();
			k != _addsOn.end () && !addOn; k++)
		if ((*k) -> canCreateBoard ((*j).second._type))
			addOn = (*k);
	assert (addOn); // An addsOn for this type of board has to exist...
	beforeCreateElement (nb);
	QGAMES::Board* result = addOn -> createBoard ((*j).second, _basePath);
	assert (result); // createMap is an user's method...
	afterCreateElement (result);
	_boards.insert (QGAMES::Boards::value_type (nb, result));
	return (result); // Finally it is returned...
}

// ---
QGAMES::BoardBuilder::BoardDefinition QGAMES::BoardBuilder::readBoardDefinition (TiXmlElement* e)
{
	QGAMES::BoardBuilder::BoardDefinition result; 
	e -> Attribute (__QGAMES_BOARDSATTRID__, &result._id);
	e -> Attribute (__QGAMES_BOARDSATTRTYPE__, &result._type);
	result._fileDefinition = _basePath + std::string (__PATH_SEPARATOR__) + 
		e -> Attribute (__QGAMES_BOARDSATTRDEFFILE__);
	return (result);
}

// ---
QGAMES::Board* QGAMES::BasicBoardAddsOn::createBoard 
	(const QGAMES::BoardBuilder::BoardDefinition& def, const std::string& bP)
{
	TiXmlDocument doc (__RL (def._fileDefinition).c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the main element doesn't call "Board", the syste keep working...

	// Read all parts of the file defining the 
	QGAMES::BoardResources resources; QGAMES::BoardSquares squares;
	std::list <QGAMES::BasicBoardAddsOn::SquareConnectionDefinition> connections;
	for (TiXmlElement* groupElement = rootElement -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_BOARDRESOURCESTAG__) == 0)
			resources = createBoardResources (readResources (groupElement));
		if (strcmp (groupElement -> Value (), __QGAMES_BOARDSQUARESTAG__) == 0)
			squares = createBoardSquares (readSquares (groupElement));
		if (strcmp (groupElement -> Value (), __QGAMES_BOARDSQUARECONNECTIONSTAG__) == 0)
			connections = readConnections (groupElement);
	}

	// Once everything has been read, the instance is created...
	// and then the squares connected.
	QGAMES::Board* result = createBoardInstance (def._id, squares, resources);
	assert (result);

	setConnections (squares, connections);
	return (result); // The result is returned!
}

// ---
std::map <std::string, std::string> QGAMES::BasicBoardAddsOn::readProperties (TiXmlElement* e)
{
	std::map <std::string, std::string> result;

	for (TiXmlElement* element = e -> FirstChildElement ();
		element != NULL; element = element -> NextSiblingElement ())
	{
		// If the element doesn't call "Attribute" the system still work...

		std::string id = ""; std::string value = "";
		id = element -> Attribute (__QGAMES_BOARDDATAATTRID__);
		value = element -> Attribute (__QGAMES_BOARDDATAATTRVALUE__);
		result.insert (std::map <std::string, std::string>::value_type (id, value));
	}

	return (result);
}

// ---
std::map <int, QGAMES::BasicBoardAddsOn::ResourceDefinition> 
	QGAMES::BasicBoardAddsOn::readResources (TiXmlElement* e)
{
	std::map <int, QGAMES::BasicBoardAddsOn::ResourceDefinition> result;

	for (TiXmlElement* element = e -> FirstChildElement ();
		element != NULL; element = element -> NextSiblingElement ())
	{
		// If the element doesn't call "Resource" the system still work...

		QGAMES::BasicBoardAddsOn::ResourceDefinition def;
		element -> Attribute (__QGAMES_BOARDRESOURCEATTRID__, &def._id);
		element -> Attribute (__QGAMES_BOARDRESOURCEATTRGROUP__, &def._group);
		def._properties = readProperties (element);
		result.insert (std::map <int, 
			QGAMES::BasicBoardAddsOn::ResourceDefinition>::value_type (def._id, def));
	}

	return (result);
}

// ---
std::map <int, QGAMES::BasicBoardAddsOn::SquareDefinition> 
	QGAMES::BasicBoardAddsOn::readSquares (TiXmlElement* e)
{
	std::map <int, QGAMES::BasicBoardAddsOn::SquareDefinition> result;

	for (TiXmlElement* element = e -> FirstChildElement ();
		element != NULL; element = element -> NextSiblingElement ())
	{
		// If the element doesn't call "Square" the system still work...

		QGAMES::BasicBoardAddsOn::SquareDefinition def;
		element -> Attribute (__QGAMES_BOARDSQUAREATTRID__, &def._id);
		for (TiXmlElement* subElement = element -> FirstChildElement ();
			subElement != NULL; subElement = subElement -> NextSiblingElement ())
		{
			if (strcmp (subElement -> Value (), __QGAMES_BOARDRESOURCESTAG__) == 0)
				def._resources = readResources (subElement);
			if (strcmp (subElement -> Value (), __QGAMES_BOARDDATASTAG__) == 0)
				def._properties = readProperties (subElement);
		}

		result.insert (std::map <int, 
			QGAMES::BasicBoardAddsOn::SquareDefinition>::value_type (def._id, def));
	}

	return (result);
}

// ---
std::list <QGAMES::BasicBoardAddsOn::SquareConnectionDefinition> 
	QGAMES::BasicBoardAddsOn::readConnections (TiXmlElement* e)
{
	std::list <QGAMES::BasicBoardAddsOn::SquareConnectionDefinition> result;

	for (TiXmlElement* element = e -> FirstChildElement ();
		element != NULL; element = element -> NextSiblingElement ())
	{
		// If the element doesn't call "Connection" the system still work...

		SquareConnectionDefinition def;
		element -> Attribute (__QGAMES_BOARDSQUARECONNECTIONATTRFROM__, &def._from);
		element -> Attribute (__QGAMES_BOARDSQUARECONNECTIONATTRIN__, &def._in);
		element -> Attribute (__QGAMES_BOARDSQUARECONNECTIONATTRTO__, &def._to);
		result.push_back (def);
	}

	return (result);
}

// ---
QGAMES::BoardResources QGAMES::BasicBoardAddsOn::createBoardResources 
	(const std::map <int, QGAMES::BasicBoardAddsOn::ResourceDefinition>& rD)
{
	QGAMES::BoardResources result;
	for (std::map <int, QGAMES::BasicBoardAddsOn::ResourceDefinition>::const_iterator i = rD.begin ();
			i != rD.end (); i++)
	{
		QGAMES::BoardResource* rsce = 
			createResourceInstance ((*i).second._id, (*i).second._group, (*i).second._properties);
		assert (rsce);
		result.insert (QGAMES::BoardResources::value_type ((*i).second._id, rsce)); 
	}

	return (result);
}

// ---
QGAMES::BoardSquares QGAMES::BasicBoardAddsOn::createBoardSquares 
	(const std::map <int, QGAMES::BasicBoardAddsOn::SquareDefinition>& sD)
{
	QGAMES::BoardSquares result;
	for (std::map <int, QGAMES::BasicBoardAddsOn::SquareDefinition>::const_iterator i = sD.begin ();
			i != sD.end (); i++)
		result.insert (QGAMES::BoardSquares::value_type ((*i).second._id, 
			createSquareInstance ((*i).second._id, 
				createBoardResources ((*i).second._resources), (*i).second._properties))); 

	return (result);
}

// ---
void QGAMES::BasicBoardAddsOn::setConnections (const QGAMES::BoardSquares& bS, 
	const std::list <QGAMES::BasicBoardAddsOn::SquareConnectionDefinition>& sCD)
{
	for (std::list <QGAMES::BasicBoardAddsOn::SquareConnectionDefinition>::const_iterator i = sCD.begin ();
			i != sCD.end (); i++)
	{
		QGAMES::BoardSquares::const_iterator j = bS.find ((*i)._from);
		QGAMES::BoardSquares::const_iterator k = bS.find ((*i)._to);
		assert (j != bS.end () && k != bS.end ());
		(*j).second -> setSquareAt ((*i)._in, (*k).second);

		// The simetric connection has to be also defined into the definition file...
		// Otherwise the system will understand the connection is one way only!.
	}
}
