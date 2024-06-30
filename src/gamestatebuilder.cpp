#include <Common/gamestatebuilder.hpp>
#include <Common/definitions.hpp>
#include <Common/genalgorithms.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/game.hpp>

// ---
void QGAMES::GameStateBuilder::ControlGameStateDefinition::addFlowCondition (int on, int cdt, int to)
{
	if (_flow.find (on) != _flow.end ())
	{
		QGAMES::GameStateBuilder::ControlGameStateDefinition::FlowConditionList& cLst = _flow [on];
		if (cLst.find (cdt) == cLst.end ())
			cLst.insert (QGAMES::GameStateBuilder::ControlGameStateDefinition::FlowConditionList::value_type
				(cdt, QGAMES::GameStateBuilder::ControlGameStateDefinition::FlowCondition (cdt, to)));
		else
			assert (0); // Just one single condition over the same state...
	}
	else
	{
		QGAMES::GameStateBuilder::ControlGameStateDefinition::FlowConditionList cLst;
		cLst.insert (QGAMES::GameStateBuilder::ControlGameStateDefinition::FlowConditionList::value_type
			(cdt, QGAMES::GameStateBuilder::ControlGameStateDefinition::FlowCondition (cdt, to)));
		_flow [on] = cLst;
	}
}

// ---
QGAMES::GameState* QGAMES::GameStateBuilder::ForControlAddsOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	QGAMES::GameStateBuilder::ControlGameStateDefinition* cDef =
		(QGAMES::GameStateBuilder::ControlGameStateDefinition*) def; // To convert...
	QGAMES::GameStates stsGS; // It could be empty...
	QGAMES::GameStateControl::FlowMap flowGS; // It could be empty...

	for (QGAMES::GameStateBuilder::ControlGameStateDefinition::FlowList::const_iterator 
			i = cDef -> _flow.begin (); i != cDef -> _flow.end (); i++)
	{
		int stOnId = (*i).first;
		IdNameGameState stOn = _builder -> gameStateIdName (stOnId);
		QGAMES::GameStateBuilder::ControlGameStateDefinition::FlowConditionList stOnCdts = (*i).second;
		stsGS [stOn._name] = stOn._gameState; 

		QGAMES::GameStateControl::FlowConditions flowCondsGS;
		for (QGAMES::GameStateBuilder::ControlGameStateDefinition::FlowConditionList::const_iterator 
				j = stOnCdts.begin (); j != stOnCdts.end (); j++)
		{
			QGAMES::GameStateBuilder::ControlGameStateDefinition::FlowCondition fC = (*j).second;
			IdNameGameState stTo; 
			// If the state is -1, then no conexion is defined...otherwise it has to exist.
			if (fC._state != -1)
			{
				int stToId = fC._state;
				stTo = _builder -> gameStateIdName (stToId);
				stsGS [stTo._name] = stTo._gameState;
			}

			// ..and if that conexion is -1, the game state linked should be null...
			flowCondsGS.insert (QGAMES::GameStateControl::FlowConditions::value_type 
				(fC._conditionId, QGAMES::GameStateControl::FlowCondition (fC._conditionId, stTo._gameState))); 

		}

		// ...and insert it into the list of those...
		flowGS [stOn._gameState] = flowCondsGS;
	}

	// The way the flow in understood is (usually) always the same, but not the object finally created...
	return (createControlGameState (cDef, stsGS, flowGS));
}

// ---
void QGAMES::GameStateBuilder::ForControlAddsOn::readSpecificBlockOver (const std::string& blkName,
	QGAMES::GameStateBuilder::GameStateDefinition* def, TiXmlElement* e)
{
	if (blkName == std::string (__QGAMES_GAMESTATEFLOWBLOCK__))
	{
		for (TiXmlElement* defElement = e -> FirstChildElement ();
			defElement != NULL; defElement = defElement -> NextSiblingElement ())
		{
			int on = __MININT__; int cdt = __MININT__; int to = __MININT__;
			defElement -> Attribute (__QGAMES_GAMESTATEFLOWATTRON__, &on);
			defElement -> Attribute (__QGAMES_GAMESTATEFLOWATTRCONDITION__, &cdt);
			defElement -> Attribute (__QGAMES_GAMESTATEFLOWATTRTO__, &to);
			assert (on != __MININT__ && cdt != __MININT__ && to != __MININT__); // The all have to exist...
			((QGAMES::GameStateBuilder::ControlGameStateDefinition*) def) -> addFlowCondition (on, cdt, to);
		}
	}
	else
		assert (0); // No other type of block is allowed under this add on...

	// Other blocks are not treated here...
}

// ---
QGAMES::GameState* QGAMES::GameStateBuilder::ForControlAddsOn::createControlGameState 
	(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
	 const QGAMES::GameStates& sts,
	 const QGAMES::GameStateControl::FlowMap& fM)
{
	// By default the basic one...
	return (new QGAMES::GameStateControl (def -> _id, QGAMES::Game::game (), sts, fM));
}

// ---
QGAMES::GameStateBuilder::GameStateBuilder (const std::string& gDef)
	: _definitions (),
	  _fileRead (false),
	  _gameStates (),
	  _definitionFile (gDef),
	  _basePath (QGAMES::onlyPath (gDef))
{
	// Adds the standard game state addsOn
	addAddsOn (new QGAMES::GameStateBuilder::ForExitAddsOn (this));
	addAddsOn (new QGAMES::GameStateBuilder::ForControlAddsOn (this));

	if (_basePath == std::string (__NULL_STRING__)) 
		_basePath = std::string (".");
}

// ---
QGAMES::GameStateBuilder::~GameStateBuilder()
{
	// Delete the definitions...
	for (QGAMES::GameStateBuilder::GameStateDefinitions::const_iterator i = _definitions.begin ();
			i != _definitions.end (); i++)
		delete ((*i).second);
	_definitions.clear ();

	// Delete the addsOn
	for (QGAMES::GameStateBuilder::AddsOnList::const_iterator i = _addsOn.begin ();
			i != _addsOn.end (); i++)
		delete ((*i));
	_addsOn.clear ();
}

// ---
std::vector <int> QGAMES::GameStateBuilder::gameStatesId () const
{
	std::vector <int> result;
	for (QGAMES::GameStateBuilder::GameStateDefinitions::const_iterator i = _definitions.begin ();
			i != _definitions.end (); i++)
		result.push_back ((*i).first);
	return (result);
}

// ---
void QGAMES::GameStateBuilder::addAddsOn (AddsOn *aO)
{
	assert (aO);

	// Take care because an add on to cover the same type 
	// than other previously inserted could be added...
	_addsOn.push_back (aO);
}

// ---
QGAMES::GameState* QGAMES::GameStateBuilder::gameState (int nGS)
{
	// If the file hasn't been read yet...
	if (!_fileRead)
		readFile (__RL (_definitionFile));
	// Take care because something wrong can happen after the file is read...

	// Has it already been created?
	QGAMES::GameStateBuilder::IdNameGameStates::const_iterator i = _gameStates.find (nGS);
	if (i != _gameStates.end ())
		return ((*i).second._gameState);

	// Is there any definition for this ones?
	QGAMES::GameStateBuilder::GameStateDefinition* def = NULL;
	for (QGAMES::GameStateBuilder::GameStateDefinitions::const_iterator i = _definitions.begin ();
			i != _definitions.end () && !def; i++)
		def = (*i).second -> definitionFor (nGS); // Look even into the nested state definition...
	assert (def); // It has to...
	
	// Then, look for the right adds on to build it up...
	QGAMES::GameState* result = NULL;
	for (QGAMES::GameStateBuilder::AddsOnList::const_iterator j = _addsOn.begin (); 
			j != _addsOn.end () && !result; j++)
		if ((*j) -> canCreateGameState (def -> _type))
			result = (*j) -> createGameState (def);

	// Something has been created eventually?
	// But the game state created is not inserted into the game
	assert (result); // Can't be null...take care
	_gameStates [nGS] = QGAMES::GameStateBuilder::IdNameGameState (nGS, def -> _name, result, def -> _main);
	return (result);
}

// ---
QGAMES::GameState* QGAMES::GameStateBuilder::gameState (const std::string& nGS)
{
	// If the file hasn't been read yet...
	if (!_fileRead)
		readFile (__RL (_definitionFile));
	// Take care because something wrong can happen after the file is read...

	// Has it already been created?
	QGAMES::GameState* result = NULL;
	for (QGAMES::GameStateBuilder::IdNameGameStates::const_iterator i = _gameStates.begin (); 
			i != _gameStates.end () && !result; i++)
		if ((*i).second._name == nGS)
			result = (*i).second._gameState;
	if (result)
		return (result);

	// Is there any definition for this ones?
	QGAMES::GameStateBuilder::GameStateDefinition* def = NULL;
	for (QGAMES::GameStateBuilder::GameStateDefinitions::const_iterator i = _definitions.begin ();
		i != _definitions.end () && !def; i++)
		def = (*i).second -> definitionFor (nGS); // Look even into the nested state definition...
	assert (def); // It has to...

	__WLOGPLUS (std::string ("Loading game state: ") + nGS);

	// Then, look for the right adds on to build it up...
	for (QGAMES::GameStateBuilder::AddsOnList::const_iterator j = _addsOn.begin ();
		j != _addsOn.end () && !result; j++)
		if ((*j) -> canCreateGameState (def -> _type))
			result = (*j) -> createGameState (def);

	// Something has been created eventually?
	// But the game state created is not inserted into the game
	assert (result); // Can't be null...take care
	_gameStates [def -> _id] = QGAMES::GameStateBuilder::IdNameGameState (def -> _id, nGS, result, def -> _main);
	return (result);
}

// ---
void QGAMES::GameStateBuilder::preLoad ()
{
	// If the file hasn't been read yet...
	if (!_fileRead)
		readFile (__RL (_definitionFile));
	// Take care because something wrong can happen after the file is read...

	for (QGAMES::GameStateBuilder::GameStateDefinitions::const_iterator i = _definitions.begin ();
			i != _definitions.end (); i++)
		gameState ((*i).first); // Just try to get it, then it will be inserted in the list of those...
}

// ---
QGAMES::GameStateBuilder::IdNameGameStates QGAMES::GameStateBuilder::mainGameStatesIdName ()
{
	QGAMES::GameStateBuilder::IdNameGameStates result;

	// First of all...preload everything...just in case
	preLoad ();
	for (QGAMES::GameStateBuilder::IdNameGameStates::const_iterator i = _gameStates.begin ();
		i != _gameStates.end (); i++)
		if ((*i).second._main)
			result [(*i).first] = (*i).second;

	return (result);
}
// ---
void QGAMES::GameStateBuilder::readFile (const std::string& fDef)
{
	__WLOGPLUS (std::string ("Reading game states definition file: ") + fDef);

	TiXmlDocument doc (fDef.c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the root element doesn't call "GameStates" the code will work too!
	// This is done to speed up the loading...

	for (TiXmlElement* defElement = rootElement -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		// The line can be an include line...
		// That line can be at any place in the gameState definition file...
		if (strcmp (defElement -> Value (), __QGAMES_INCLUDENAME__) == 0)
		{
			assert (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)); 
			readFile (__RL (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)));
		}
		else
		// The other type of line is a simple gameState definition line...
		if (strcmp (defElement -> Value (), __QGAMES_GAMESTATETAGNAME__) == 0)
		{
			QGAMES::GameStateBuilder::GameStateDefinition* def = readGameStateDefinition (defElement);
			_definitions.insert (QGAMES::GameStateBuilder::GameStateDefinitions::value_type (def -> _id, def));
		}
	}

	_fileRead = true; // The file has been read already...
}

// ---
QGAMES::GameStateBuilder::GameStateDefinition* 
	QGAMES::GameStateBuilder::readGameStateDefinition (TiXmlElement* e)
{
	// Makes no sense to control whether the pointer received is null
	// Because just one method invokes this and can't be overloaded by the user.

	QGAMES::GameStateBuilder::GameStateDefinition* result = NULL;

	// The definition for the game state has to be read by the right adds on...
	// Because it could be different attending to the type of definition itself.
	int id = -1; int type = -1;
	std::string mn (__YES_STRING__); // By default the state is always main...
	std::string name;
	e -> Attribute (__QGAMES_GAMESTATEATTRID__, &id);
	if (e -> Attribute (__QGAMES_GAMESTATEATTRMAIN__) != NULL)
		mn = e -> Attribute (__QGAMES_GAMESTATEATTRMAIN__); // It is optional...
	e -> Attribute (__QGAMES_GAMESTATESTTRTYPE__, &type);
	name = e -> Attribute (__QGAMES_GAMESTATEATTRNAME__);
	assert (id != -1 && type != -1 && name != std::string (__NULL_STRING__)); 
	// They must exist. Name and nested id are optional...

	// Looks for the add on to use...
	QGAMES::GameStateBuilder::AddsOn* addOn = NULL;
	for (QGAMES::GameStateBuilder::AddsOnList::const_iterator i = _addsOn.begin (); 
			i != _addsOn.end () && !addOn; i++)
		if ((*i) -> canCreateGameState (type))
			addOn = (*i);
	assert (addOn); // If not found, impossible to continue...

	// Creates the basic game state definition...
	result = addOn -> createEmptyDefinition ();
	assert (result); // Can't be null...

	// Then adds to the result the very basic attributes
	result -> _id = id; 
	result -> _main = (mn == std::string (__YES_STRING__));
	result -> _type = type;
	result -> _name = name;
	// ...and also the information needed by the specific type of result...if any
	addOn -> readSpecificAttributesOver (result, e);

	// Then reads the blocks with information inside...
	for (TiXmlElement* aElement = e -> FirstChildElement ();
		aElement != NULL; aElement = aElement -> NextSiblingElement ())
	{
		// Is it a block for attributes...
		if (strcmp (aElement -> Value (), __QGAMES_GAMESTATEATTRSBLOCK__) == 0)
			result -> _attributes = readAttributesGroup (aElement);
		else
		if (strcmp (aElement -> Value (), __QGAMES_GAMESTATENESTEDBLOCK__) == 0)
		{	
			if (aElement -> FirstChildElement () != NULL) 
			// Just to be sure the nested element has been defined...
			{
				assert (!result -> _nested); // There can only be one nested class...
				result -> _nested = 
					readGameStateDefinition (aElement -> FirstChildElement ()); // Recursive...take care...
				result -> _nested -> _main = false; // No nested game state is ever a main one...
			}
		}
		// It is not, but it can be a specific block...
		else
			addOn -> readSpecificBlockOver (aElement -> Value (), result, aElement);
	}

	return (result);
}

// ---
std::map <std::string, std::string> QGAMES::GameStateBuilder::readAttributesGroup (TiXmlElement* g)
{
	assert (g);

	std::map <std::string, std::string> result;
	for (TiXmlElement* aElement = g -> FirstChildElement ();
		aElement != NULL; aElement = aElement -> NextSiblingElement ())
	{
		// If the element doesn't call "Attribute" the code will work too!
		// This is done to speed up the loading...
		
		std::string id, value;
		id = aElement -> Attribute (__QGAMES_GAMESTATEATTRATTRID__);
		value = aElement -> Attribute (__QGAMES_GAMESTATEATTRATTRVALUE__);
		assert (id != std::string (__NULL_STRING__)); // Is it a valid variable definition?...
		result.insert (std::map <std::string, std::string>::value_type (id, value));
	}

	return (result);
}
