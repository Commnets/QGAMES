#include <Arcade/worldbuilder.hpp>
#include <Arcade/definitions.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/genalgorithms.hpp>
#include <Common/game.hpp>

// ---
QGAMES::WorldBuilder::WorldBuilder (const std::string& eDef, 
		QGAMES::MapBuilder* mB)
	: _definitions (),
	  _worlds (),
	  _mapBuilder (mB),
	  _definitionFile (eDef),
	  _basePath (QGAMES::onlyPath (eDef))
{
	assert (_mapBuilder);

	if (_basePath == std::string (__NULL_STRING__)) 
		_basePath = std::string (".");

	readFile (__RL (_definitionFile));
}

// ---
QGAMES::WorldBuilder::~WorldBuilder ()
{
	QGAMES::Worlds::const_iterator i;
	for (i = _worlds.begin (); i != _worlds.end (); i++)
		delete ((*i).second);
	_worlds.clear ();
}

// ---
std::vector <int> QGAMES::WorldBuilder::resourcesId () const
{
	std::vector <int> result;
	for (QGAMES::WorldBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end (); i++)
		result.push_back ((*i).first);
	return (result);
}
// ---
void QGAMES::WorldBuilder::preLoad ()
{
	for (QGAMES::WorldBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end (); i++)
		if (_worlds.find ((*i).second._id) == _worlds.end ())
			world ((*i).second._id);
}

// ---
QGAMES::World* QGAMES::WorldBuilder::world (int nw)
{
	QGAMES::Worlds::const_iterator i = _worlds.find (nw);
	if (i != _worlds.end ())
		return ((*i).second);

	__WLOGPLUS (std::string ("Loading world: ") + std::to_string (nw));
	
	QGAMES::WorldBuilder::Definitions::const_iterator j = _definitions.find (nw);
	assert (j != _definitions.end ()); // Is doesn't exist...

	beforeCreateElement (nw);
	QGAMES::World* world = createWorld ((*j).second);
	assert (world); // createWorld is an user's method...
	world -> setExternalEngine (externalEngine ());
	afterCreateElement (world);

	_worlds.insert (QGAMES::Worlds::value_type (nw, world));

	// @see createWorld info
	// This piece of code is just to add the connections to the scenes of the world...
	for (std::map <int, QGAMES::WorldBuilder::SceneDefinition>::const_iterator k = (*j).second._scenes.begin ();
			k != (*j).second._scenes.end (); k++)
	{
		QGAMES::WorldBuilder::SceneDefinition defC = (*k).second;
		QGAMES::Scenes::const_iterator l = world -> scenes ().find ((*k).first);
		if (l != world -> scenes ().end ()) // To add the scenes the original scene has to be defined first...
			addSceneConnections ((*l).second, world -> scenes (), defC);
	}

	return (world);
}

// ---
void QGAMES::WorldBuilder::readFile (const std::string& nF)
{
	__WLOGPLUS (std::string ("Reading worlds definition file: ") + nF);

	TiXmlDocument doc (nF.c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the main element doesn't call "Worlds" the system still works...

	for (TiXmlElement* defElement = rootElement -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		if (strcmp (defElement -> Value (), __QGAMES_INCLUDENAME__) == 0)
		{
			assert (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)); 
			readFile (__RL (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)));
		}
		else
		if (strcmp (defElement -> Value (), __QGAMES_WORLDTAGNAME__) == 0)
		{
			QGAMES::WorldBuilder::WorldDefinition def = 
				readWorldDefinition (defElement);
			assert (def._id != -1); // Is it a valid world?
			_definitions.insert (QGAMES::WorldBuilder::Definitions::value_type (def._id, def));
		}
	}
}

// ---
QGAMES::WorldBuilder::WorldDefinition QGAMES::WorldBuilder::readWorldDefinition (TiXmlElement* e)
{
	assert (e);

	QGAMES::WorldBuilder::WorldDefinition result;
	e -> Attribute (__QGAMES_WORLDSATTRID__, &result._id);
	for (TiXmlElement* groupElement = e -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_WORLDSCENESBLOCK__) == 0)
			result._scenes = readScenesDefinition (groupElement);
		if (strcmp (groupElement -> Value (), __QGAMES_WORLDSETOFSCENESBLOCK__) == 0)
			result._setOfScenes = readSetOfScenesDefinition (groupElement);
		if (strcmp (groupElement -> Value (), __QGAMES_WORLDSATTRIBUTESBLOCK__) == 0)
			result._variables = readVariables (groupElement);
	}

	return (result);	
}

// ---
std::map <int, QGAMES::WorldBuilder::SceneDefinition> 
	QGAMES::WorldBuilder::readScenesDefinition (TiXmlElement* e)
{
	assert (e);

	std::map <int, QGAMES::WorldBuilder::SceneDefinition> result;
	for (TiXmlElement* defElement = e -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		if (strcmp (defElement -> Value (), __QGAMES_WORLDSCENETAGNAME__) == 0)
		{
			QGAMES::WorldBuilder::SceneDefinition def = readSceneDefinition (defElement);
			assert (def._id != -1); // Is it a right scene definition?...
			result.insert (std::map <int, 
				QGAMES::WorldBuilder::SceneDefinition>::value_type (def._id, def));
		}
	}

	return (result);	
}

// ---
QGAMES::WorldBuilder::SceneDefinition QGAMES::WorldBuilder::readSceneDefinition (TiXmlElement* e)
{
	assert (e);

	QGAMES::WorldBuilder::SceneDefinition result;
	e -> Attribute (__QGAMES_WORLDSSCENESATTRID__, &result._id);
	result._type = QGAMES::WorldBuilder::SceneDefinition::Type::_SIMPLE;
	for (TiXmlElement* groupElement = e -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_WORLDSSCENESACTIONSBLOCK__) == 0)
			result._sceneBlocks = readScenesBlockDefinition (groupElement);
		if (strcmp (groupElement -> Value (), __QGAMES_WORLDSSCENESATTRSBLOCK__) == 0)
			result._variables = readVariables (groupElement);
		if (strcmp (groupElement -> Value (), __QGAMES_WORLDSSCENESCONEXIONSBLOCK__) == 0)
			result._conexions = readConexions (groupElement);
		if (strcmp (groupElement -> Value (), __QGAMES_WORLDSSCENESMAPSBLOCK__) == 0)
			result._maps = readMaps (groupElement);
		if (strcmp (groupElement -> Value (), __QGAMES_WORLDSSCENEENTITIESBLOCK__) == 0)
			result._entitiesInMaps = readEntitiesInMaps (groupElement);
	}

	return (result);	
}

// ---
std::map <int, QGAMES::WorldBuilder::SceneDefinition> 
	QGAMES::WorldBuilder::readSetOfScenesDefinition (TiXmlElement* e)
{
	assert (e);

	std::map <int, QGAMES::WorldBuilder::SceneDefinition> result;
	for (TiXmlElement* defElement = e -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		if (strcmp (defElement -> Value (), __QGAMES_WORLDSSCENETEMPTAGNAME__) == 0)
		{
			QGAMES::WorldBuilder::SceneDefinition def = readTemplateSceneDefinition (defElement);
			assert (def._id != -1 && def._number > 0 && def._conexions.empty ()); // Is it a right scene definition?...
			result.insert (std::map <int, 
				QGAMES::WorldBuilder::SceneDefinition>::value_type (def._id, def));
		}
	}

	return (result);	
}

// ---
QGAMES::WorldBuilder::SceneDefinition QGAMES::WorldBuilder::readTemplateSceneDefinition (TiXmlElement* e)
{
	assert (e);

	QGAMES::WorldBuilder::SceneDefinition result;
	e -> Attribute (__QGAMES_WORLDSSCENTEMPATTRBASEID__, &result._id);
	e -> Attribute (__QGAMES_WORLDSSCENETEMPATTRNUMBER__, &result._number);
	result._type = QGAMES::WorldBuilder::SceneDefinition::Type::_SET;
	for (TiXmlElement* groupElement = e -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_WORLDSSCENESACTIONSBLOCK__) == 0)
			result._sceneBlocks = readScenesBlockDefinition (groupElement);
		if (strcmp (groupElement -> Value (), __QGAMES_WORLDSSCENESATTRSBLOCK__) == 0)
			result._variables = readVariables (groupElement);
		if (strcmp (groupElement -> Value (), __QGAMES_WORLDSSCENESCONEXIONSBLOCK__) == 0)
			result._conexions = readConexions (groupElement);
		if (strcmp (groupElement -> Value (), __QGAMES_WORLDSSCENESMAPSBLOCK__) == 0)
			result._maps = readMaps (groupElement);
		if (strcmp (groupElement -> Value (), __QGAMES_WORLDSSCENEENTITIESBLOCK__) == 0)
			result._entitiesInMaps = readEntitiesInMaps (groupElement);
	}

	return (result);	
}

// ---
std::map <int, QGAMES::WorldBuilder::SceneBlockDefinition> 
	QGAMES::WorldBuilder::readScenesBlockDefinition (TiXmlElement* e)
{
	assert (e);

	std::map <int, QGAMES::WorldBuilder::SceneBlockDefinition> result;
	for (TiXmlElement* defElement = e -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		QGAMES::WorldBuilder::SceneBlockDefinition def = readSceneBlockDefinition (defElement);
		assert (def._id != -1); // Is it a right scene definition?...
		result.insert (std::map <int, 
			QGAMES::WorldBuilder::SceneBlockDefinition>::value_type (def._id, def));
	}

	return (result);	
}

// ---
QGAMES::WorldBuilder::SceneBlockDefinition QGAMES::WorldBuilder::readSceneBlockDefinition (TiXmlElement* e)
{
	assert (e);

	QGAMES::WorldBuilder::SceneBlockDefinition result;
	e -> Attribute (__QGAMES_WORLDSSCENESACTIONATTRID__, &result._id);
	std::string aB = (e -> Attribute (__QGAMES_WORLDSSCENESACTIONATTRACTIVE__)) 
		? QGAMES::toUpper (std::string (e -> Attribute (__QGAMES_WORLDSSCENESACTIONATTRACTIVE__))) : std::string (__NULL_STRING__);
	if (aB == std::string (__NO_STRING__)) result._active = false; 
	// By default (or if it doesn't exist) the block is active...
	for (TiXmlElement* groupElement = e -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_WORLDSSCENESACTIONATTRSBLOCK__) == 0)
			result._variables = readVariables (groupElement);
	}

	return (result);	
}

// ---
std::map <int, QGAMES::WorldBuilder::SceneConexion> 
	QGAMES::WorldBuilder::readConexions (TiXmlElement* e)
{
	assert (e);

	std::map <int, QGAMES::WorldBuilder::SceneConexion> result;
	for (TiXmlElement* defElement = e -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		QGAMES::WorldBuilder::SceneConexion sC;
		defElement -> Attribute (__QGAMES_WORLDSSCENESCONATTRID__, &sC._id);
		defElement -> Attribute (__QGAMES_WORLDSSCENESCONATTRWITHID__, &sC._toScene);
		defElement -> Attribute (__QGAMES_WORLDSCENESCONATTRATID__, &sC._toPoint);
		// Is it a right connection definition? (the point is not mandatory)
		assert (sC._id != -1 && sC._toScene != -1); 
		result.insert (std::map <int, QGAMES::WorldBuilder::SceneConexion>::value_type (sC._id, sC));
	}

	return (result);
}

// ---
std::map <int, std::string> QGAMES::WorldBuilder::readMaps (TiXmlElement* e)
{
	assert (e);

	std::map <int, std::string> result;
	for (TiXmlElement* defElement = e -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		int id = -1; 
		std::string fName (__NULL_STRING__);
		defElement -> Attribute (__QGAMES_WORLDSSCENESMAPSATTRID__, &id);
		fName = _basePath + std::string (__PATH_SEPARATOR__) + 
			defElement -> Attribute (__QGAMES_WORLDSSCENESMAPSNAMEATTRID__);
		assert (id != -1 && fName != std::string (__NULL_STRING__)); // Is it a right map definition?...
		result.insert (std::map <int, std::string>::value_type (id, fName));
	}

	return (result);
}

// ---
std::map <std::string, std::vector <int>> QGAMES::WorldBuilder::readEntitiesInMaps (TiXmlElement* e)
{
	assert (e);

	std::map <std::string, std::vector <int>> result;
	for (TiXmlElement* defElement = e -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		int fromId = -1; int n = -1;
		std::string eName (__NULL_STRING__);
		// Has a single element been defined?...
		if (strcmp (defElement -> Value (), __QGAMES_WORLDSCENEENTITYTAGNAME__) == 0)
		{
			n = 1;
			defElement -> Attribute (__QGAMES_WORLDSSCENEENTITYATTRID__, &fromId);
		}
		else
		// ..os is it a group of them?
		if (strcmp (defElement -> Value (), __QGAMES_WORLDSCENESETENTITIESTAGNAME__) == 0)
		{
			defElement -> Attribute (__QGAMES_WORLDSCENESETENTATTRFROM__, &fromId);
			defElement -> Attribute (__QGAMES_WORLDSCENESETENTATTRNUMBER__, &n);
		}

		assert (fromId != -1 && n != -1);
		// Is it a right definition...

		eName = defElement -> Attribute (__QGAMES_WORLDSSCENEENTITYLAYERATTRID__);

		std::vector <int> ePL;
		std::map <std::string, std::vector <int>>::const_iterator i;
		if ((i = result.find (eName)) != result.end ())
			ePL = (*i).second; // To add a new element is something that already exists...
		for (int j = 0; j < n; j++)
			ePL.push_back (fromId + j);
		if (i != result.end ())
			result.erase (i); // The previous array is not longer valid...
		result.insert (std::map <std::string, std::vector <int>>::value_type (eName, ePL));
	}

	return (result);
}

// ---
std::map <std::string, std::string> QGAMES::WorldBuilder::readVariables (TiXmlElement* e)
{
	assert (e);

	std::map <std::string, std::string> result;
	for (TiXmlElement* defElement = e -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		// It the main element doesn't call "Attribute", the system reads the information anyway...

		std::string id (__NULL_STRING__);  std::string v (__NULL_STRING__);
		id = defElement -> Attribute (__QGAMES_WORLDSSCENESATTRSATTRID__);
		v = defElement -> Attribute (__QGAMES_WORLDSSCENESATTRSVALUEATTRID__);
		assert (id != std::string (__NULL_STRING__) && v != std::string (__NULL_STRING__)); // Right?...
		result.insert (std::map <std::string, std::string>::value_type (id, v));
	}

	return (result);
}

// ---
QGAMES::World* QGAMES::WorldBuilder::createWorld (const QGAMES::WorldBuilder::WorldDefinition& def)
{
	QGAMES::World* result = NULL;

	QGAMES::Scenes scns;

	auto newScene = [&](const QGAMES::WorldBuilder::SceneDefinition& scnDef)
		{
			QGAMES::Scene* result = createScene (scnDef);
			assert (result != NULL); // createScene is an user's method...
			result -> setExternalEngine (externalEngine ());
			afterCreateElement (result);
			scns.insert	(QGAMES::Scenes::value_type (result -> id (), result));
		};

	// Createth scenes...
	for (auto i : def._scenes)
	{
		beforeCreateElement (i.second._id);
		newScene (i.second); 
	}

	// Create the set of scenes...
	for (auto i : def._setOfScenes)
	{
		QGAMES::WorldBuilder::SceneDefinition scnDef = i.second;
		scnDef._type = QGAMES::WorldBuilder::SceneDefinition::Type::_SIMPLE;
		scnDef._number = 1;
		for (int j = 0; j < i.second._number; j++)
		{
			scnDef._id = i.second._id + j;
			std::map <int, std::string> nMaps;
			for (auto k : i.second._maps)
				nMaps [k.first + j] = k.second;
			scnDef._maps = nMaps;
			newScene (scnDef); // And addd to the list of...
		}
	}

	QGAMES::WorldProperties pts;
	for (std::map <std::string, std::string>::const_iterator l = def._variables.begin ();
			l != def._variables.end (); l++)
		pts.insert (QGAMES::WorldProperties::value_type ((*l).first, (*l).second));

	beforeCreateElement (def._id);
	result = createWorldObject (def._id, scns, pts);
	assert (result != NULL);
	afterCreateElement (result);

	// The scene is created with no connections
	// A connection can link the scene of a world with the scene ot another
	// When it happens the connection looks for that other scene in the other world
	// invoking the method world back generating (potentially) an infinite recursive queue of calls
	// So the connection must be added after the worl object is created and added to the list of those

	return (result);
}

// ---
QGAMES::Scene* QGAMES::WorldBuilder::createScene (const QGAMES::WorldBuilder::SceneDefinition& def)
{
	QGAMES::Scene* result = NULL;

	__WLOGPLUS (std::string ("Loading scene: ") + std::to_string (def._id));

	QGAMES::Maps mps;
	for (std::map <int, std::string>::const_iterator i = def._maps.begin (); 
			i != def._maps.end (); i++)
		mps.insert (QGAMES::Maps::value_type ((*i).first, _mapBuilder -> map ((*i).first)));

	QGAMES::SceneProperties pts;
	for (std::map <std::string, std::string>::const_iterator l = def._variables.begin ();
			l != def._variables.end (); l++)
		pts.insert (QGAMES::WorldProperties::value_type ((*l).first, (*l).second));

	// The conexions are added later,...in the main creation routine...
	// Because all scenes have to be loaded first to identify what to connect to.
	beforeCreateElement (def._id);
	result = createSceneObject (def._id, mps, QGAMES::Scene::Connections (), 
		pts, def._entitiesInMaps);
	assert (result);
	afterCreateElement (result);

	// Add the action blocks if any...
	for (std::map <int, QGAMES::WorldBuilder::SceneBlockDefinition>::const_iterator j = def._sceneBlocks.begin (); 
			j != def._sceneBlocks.end (); j++)
	{
		beforeCreateElement (def._id);
		QGAMES::SceneActionBlock* blk = createSceneActionBlock ((*j).second);
		assert (blk != NULL); // createScene is an user's method...
		afterCreateElement (blk);

		result -> addActionBlock (blk, false);
	}

	return (result);
}

// ---
QGAMES::SceneActionBlock* QGAMES::WorldBuilder::createSceneActionBlock 
	(const QGAMES::WorldBuilder::SceneBlockDefinition& def)
{
	QGAMES::SceneActionBlock* result = createSceneActionBlockObject (def._id, def._variables);
	assert (result);
	result -> setActive (def._active); // Can be born not active, and to be actived later during the game!

	return (result);
}

// ---
void QGAMES::WorldBuilder::addSceneConnections (QGAMES::Scene* s, const QGAMES::Scenes& scns, 
		QGAMES::WorldBuilder::SceneDefinition& def)
{
	for (std::map <int, QGAMES::WorldBuilder::SceneConexion>::const_iterator i = 
			def._conexions.begin (); i != def._conexions.end (); i++)
	{
		QGAMES::Scene::Connection cN;
		cN._id = (*i).second._id; // equal to (*i).first...
		
		QGAMES::Scenes::const_iterator k = scns.find ((*i).second._toScene);
		if (k != scns.end ()) 
			cN._toScene = (*k).second; // The real link...
		else
		{
			int oWId;
			if ((oWId = worldIdForScene ((*i).second._toScene)) != -1)
			{
				// The connection could be defined in another not-still-loaded world...
				// If it is the case, the two worlds will be linked and loaded in a single call!
				__WLOGPLUS (std::string ("Scene ") + std::to_string (s -> id ()) + 
					std::string (" linked to World ") + std::to_string (oWId) + 
					std::string (" in Scene ") + std::to_string ((*i).second._toScene));

				QGAMES::World* nWrld = world (oWId);
				cN._toScene = nWrld -> scene ((*i).second._toScene); // The real link...
			}
		}

		// A potential target. 
		// It could be -1 meaning nothing
		cN._toPoint = (*i).second._toPoint; 

		// The scene has to exit to be really added...
		if (cN._toScene != NULL)
			s -> addConnection ((*i).first, cN);
	}
}

// ---
int QGAMES::WorldBuilder::worldIdForScene (int scnId)
{
	int result = -1;

	for (QGAMES::WorldBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end () && result == -1; i++)
	{
		for (std::map <int, QGAMES::WorldBuilder::SceneDefinition>::const_iterator j = 
				(*i).second._scenes.begin (); j != (*i).second._scenes.end () && result == -1; j++)
			if ((*j).second._id == scnId)
				result = (*i).first;
	}

	return (result);
}
