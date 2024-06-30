#include <Room/rmroombuilder.hpp>

// ---
QGAMES::RoomBuilder::RoomBuilder (const std::string& fDef, QGAMES::EntityBuilder* eB)
	: _entityBuilder (eB), 
	  _definitionFile (fDef),
	  _basePath (QGAMES::onlyPath (fDef))
{
	assert (_entityBuilder);

	if (_basePath == std::string (__NULL_STRING__)) 
		_basePath = std::string (".");

	readFile (__RL (_definitionFile));
}

// ---
void QGAMES::RoomBuilder::readFile (const std::string& fDef)
{
	__WLOGPLUS (std::string ("Reading rooms definition file: ") + fDef);

	TiXmlDocument doc (fDef.c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?
	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the root element doesn't call "Rooms" the code will work too!
	// This is done to speed up the loading...

	// Reading all elements...
	for (TiXmlElement* groupElement = rootElement -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_INCLUDENAME__) == 0)
		{
			std::string nF = groupElement -> Attribute (__QGAMES_INCLUDEATTRFILE__);
			readFile (__RL (nF));
		}
		if (strcmp (groupElement -> Value (), __ROOMGAME_BUILDERROOMBLOCK__) == 0)
		{
			QGAMES::RoomBuilder::RoomDefinition roomDef = readRoomDefinition (groupElement); 
			_roomDefinitions.insert 
				(QGAMES::RoomBuilder::RoomDefinitions::value_type (roomDef._id, roomDef));
		}
	}
}

// ---
QGAMES::Room* QGAMES::RoomBuilder::room (int nR)
{
	QGAMES::RoomBuilder::RoomDefinitions::const_iterator i = _roomDefinitions.find (nR);
	assert (i != _roomDefinitions.end ());

	QGAMES::Room* result = createRoomObject ((*i).second);
	assert (result != nullptr);

	// Is it based in another room
	// Take care using this funtion. It could be recursive...
	for (auto j : (*i).second._overRoom)
	{
		QGAMES::Room* baseRoom = room (j);
		result -> addAllFromRoom (baseRoom); // Copy...
		delete (baseRoom);
	}

	// Create the walls...
	QGAMES::RoomWalls walls;
	for (auto j : (*i).second._roomWalls)
	{
		QGAMES::RoomWall* wall = createRoomWallObject (j);
		assert (wall != nullptr);

		QGAMES::RoomWall::Holes hls;
		for (auto k : j._holes)
			hls.push_back (QGAMES::RoomWall::Hole ((float) k.posX (), (float) k.posY (), (float) k.posZ ()));
		wall -> removeBricksFromHoles (hls);

		QGAMES::RoomWall::WallElements wElmnts;
		for (auto k : j._decoration)
			wElmnts.insert (QGAMES::RoomWall::WallElements::value_type 
				(k._id, new QGAMES::RoomWall::WallElement 
					(k._id, dynamic_cast <QGAMES::RoomElement*> (entityBuilder () -> entity (k._entityId) -> clone ()),
					 QGAMES::RoomWall::Hole ((float) k._position.posX (), (float) k._position.posY (), (float) k._position.posZ ()))));
		wall -> addWallElements (wElmnts);

		walls.insert (QGAMES::RoomWalls::value_type (j._id, wall));
	}

	// Create the decoration
	QGAMES::Room::Decoration decoration;
	for (auto j : (*i).second._decoration)
		decoration.insert (QGAMES::Room::Decoration::value_type 
			(j._id, new QGAMES::Room::DecorativeElement 
				(j._id, dynamic_cast <QGAMES::RoomElement*> (entityBuilder () -> entity (j._entityId) -> clone ()), j._position)));

	// Add both...
	result -> addWalls (walls); 
	result -> addDecoration (decoration); 

	return (result);
}

// ---
QGAMES::RoomBuilder::RoomDefinition QGAMES::RoomBuilder::readRoomDefinition (TiXmlElement* elmnt)
{
	assert (elmnt != nullptr);

	QGAMES::RoomBuilder::RoomDefinition result;
	elmnt -> Attribute (__ROOMGAME_BUILDERROOMIDTAG__, &result._id);
	elmnt -> Attribute (__ROOMGAME_BUILDERROOMTYPETAG__, &result._type);

	for (TiXmlElement* groupElement = elmnt -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __ROOMGAME_BUILDEROVERROOMBLOCK__) == 0)
		{
			int oR;
			groupElement -> Attribute (__ROOMGAME_BUILDEROVERROOMIDTAG__, &oR);
			assert (oR != -1);
			if (std::find (result._overRoom.begin (), result._overRoom.end (), oR) == result._overRoom.end ())
				result._overRoom.push_back (oR); // It can't be repited!
		}
		else
		if (strcmp (groupElement -> Value (), __ROOMGAME_BUILDERWALLSBLOCK__) == 0)
		{
			QGAMES::RoomBuilder::RoomWallDefinitions nDef = 
				readRoomWallDefinitions (groupElement);
			result._roomWalls.insert (result._roomWalls.end (), nDef.begin (), nDef.end ());
		}
		else
		if (strcmp (groupElement -> Value (), __ROOMGAME_BUILDERROOMDECORATIONBLOCK__) == 0)
		{
			QGAMES::RoomBuilder::DecorationElementDefinitions nDef = 
				readDecorationElementDefinitions (groupElement);
			result._decoration.insert (result._decoration.end (), nDef.begin (), nDef.end ());
		}
	}

	assert (result._id != -1 && result._type != -1);

	return (result);
}

// ---
QGAMES::RoomBuilder::RoomWallDefinitions QGAMES::RoomBuilder::readRoomWallDefinitions (TiXmlElement* elmnt)
{
	assert (elmnt != nullptr);

	QGAMES::RoomBuilder::RoomWallDefinitions result;

	for (TiXmlElement* groupElement = elmnt -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
		if (strcmp (groupElement -> Value (), __ROOMGAME_BUILDERROOMWALLBLOCK__) == 0)
			result.push_back (readRoomWallDefinition (groupElement));

	return (result);
}

// ---
QGAMES::RoomBuilder::RoomWallDefinition QGAMES::RoomBuilder::readRoomWallDefinition (TiXmlElement* elmnt)
{
	assert (elmnt != nullptr);

	QGAMES::RoomBuilder::RoomWallDefinition result;
	elmnt -> Attribute (__ROOMGAME_BUILDERROOMWALLIDTAG__, &result._id);
	elmnt -> Attribute (__ROOMGAME_BUILDERROOMWALLBRICKIDTAG__, &result._brickEntityId);
	elmnt -> Attribute (__ROOMGAME_BUILDERROOMWALLBRICKFORMTAG__, &result._brickInitialFormId);
	elmnt -> Attribute (__ROOMGAME_BUILDERROOMWALLBRICKASPECTTAG__, &result._brickInitialAspectId);
	result._position = (elmnt -> Attribute (__ROOMGAME_BUILDERROOMWALLPOSTAG__) != nullptr)
		? QGAMES::Position (elmnt -> Attribute (__ROOMGAME_BUILDERROOMWALLPOSTAG__)) : QGAMES::Position::_cero;
	elmnt -> Attribute (__ROOMGAME_BUILDERROOMWALLBRICKSWIDTHTAG__, &result._bricksWidth);
	elmnt -> Attribute (__ROOMGAME_BUILDERROOMWALLBRICKSHEIGHTTAG__, &result._bricksHeight);
	elmnt -> Attribute (__ROOMGAME_BUILDERROOMWALLBRICKSDEPTHTAG__, &result._bricksDepth);

	for (TiXmlElement* groupElement = elmnt -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __ROOMGAME_BUILDERROOMDECORATIONBLOCK__) == 0)
		{
			QGAMES::RoomBuilder::DecorationElementDefinitions nDef = 
				readDecorationElementDefinitions (groupElement);
			result._decoration.insert (result._decoration.end (), nDef.begin (), nDef.end ());
		}
		else
		if (strcmp (groupElement -> Value (), __ROOMGAME_BUILDERROOMWALLHOLEBLOCK__) == 0)
			result._holes.push_back (readHolePosition (groupElement));
	}

	assert (result._id != -1 && 
			result._brickEntityId != -1 && result._brickInitialFormId != -1 && result._brickInitialAspectId != -1 &&
			result._bricksWidth > 0 && result._bricksHeight > 0 && result._bricksDepth > 0);

	return (result);
}

// ---
QGAMES::RoomBuilder::DecorationElementDefinitions QGAMES::RoomBuilder::readDecorationElementDefinitions (TiXmlElement* elmnt)
{
	assert (elmnt != nullptr);

	QGAMES::RoomBuilder::DecorationElementDefinitions result;

	for (TiXmlElement* groupElement = elmnt -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
		if (strcmp (groupElement -> Value (), __ROOMGAME_BUILDERROOMDECELEMENTBLOCK__) == 0)
			result.push_back (readDecorationElementDefinition (groupElement));

	return (result);
}

// ---
QGAMES::RoomBuilder::DecorationElementDefinition QGAMES::RoomBuilder::readDecorationElementDefinition (TiXmlElement* elmnt)
{
	assert (elmnt != nullptr);

	QGAMES::RoomBuilder::DecorationElementDefinition result;

	elmnt -> Attribute (__ROOMGAME_BUILDERROOMDECELEMENTIDTAG__, &result._id);
	elmnt -> Attribute (__ROOMGAME_BUILDERROOMDECELEMENTENTITYTAG__, &result._entityId);
	result._position = (elmnt -> Attribute (__ROOMGAME_BUILDERROOMDECELEMENTPOSTAG__) != nullptr)
		? QGAMES::Position (elmnt -> Attribute (__ROOMGAME_BUILDERROOMDECELEMENTPOSTAG__)) : QGAMES::Position::_cero;

	assert (result._id != -1 && result._entityId != -1);

	return (result);
}

// ---
QGAMES::Position QGAMES::RoomBuilder::readHolePosition (TiXmlElement* elmnt)
{
	assert (elmnt != nullptr);

	return (QGAMES::Position (elmnt -> Attribute (__ROOMGAME_BUILDERROOMWALLHOLEPOSTAG__)));
}
