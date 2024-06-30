#include "TestRoomGame.hpp"
#include <SDL/sdlinclude.hpp>

#define __GAMEICON__			70000

// --------------------------------------------------------------------------------
// ---
TestRoomGame::FramerateScoreObject::FramerateScoreObject ()
	: QGAMES::ScoreObjectNumber (100, 60, QGAMES::Forms (), 16)
{
	_forms.insert (QGAMES::Forms::value_type (10000, 
		QGAMES::Game::game () -> form (10000)));
	setCurrentForm (10000);
	setPosition (QGAMES::Position (__BD 10, __BD 10, __BD 0));
}

// ---
void TestRoomGame::FramerateScoreObject::updatePositions ()
{
	QGAMES::ScoreObjectNumber::updatePositions ();

	setNumber (game () -> currentLoopsPerSecond ());
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Entity* TestRoomGame::Brick::clone () const
{
	QGAMES::Entity* result = new TestRoomGame::Brick (game () -> uniqueId ()); 

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void TestRoomGame::Brick::initialize ()
{
	if (currentForm () == nullptr)
	{
		setCurrentForm (FID);
		setCurrentAspect (AID);
	}

	QGAMES::Entity::initialize ();
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Entity* TestRoomGame::Picture::clone () const
{
	QGAMES::Entity* result = new TestRoomGame::Picture (game () -> uniqueId ()); 

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void TestRoomGame::Picture::initialize ()
{
	if (currentForm () == nullptr)
	{
		setCurrentForm (FID);
		setCurrentAspect (AID);
	}

	QGAMES::Entity::initialize ();
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Entity* TestRoomGame::Knight::clone () const
{
	QGAMES::Entity* result = new TestRoomGame::Knight (_id); 

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void TestRoomGame::Knight::initialize ()
{
	setCurrentForm (FID);
	setCurrentAspect (AID);

	QGAMES::Entity::initialize ();
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Entity* TestRoomGame::Cube::clone () const
{
	QGAMES::Entity* result = new TestRoomGame::Cube (_id); 

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void TestRoomGame::Cube::initialize ()
{
	setCurrentForm (FID);
	setCurrentAspect (AID);

	QGAMES::Entity::initialize ();
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Entity* TestRoomGame::EntityBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	QGAMES::Entity* result = NULL;

	if (def._id == __TRMTM_BRICKTEMPLATEID__)
		return (new TestRoomGame::Brick (def._id));
	else
	if (def._id == __TRMTM_PICTURETEMPLATEID__)
		return (new TestRoomGame::Picture (def._id));
	else
	if (def._id >= __TRMTM_KNIGHBASETID__ && 
		def._id < (__TRMTM_KNIGHBASETID__ + __TRMTM_NUMBERKNIGHTS__))
		return (new TestRoomGame::Knight (def._id));
	else
	if (def._id >= __TRMTM_CUBEBASEID__ &&
		def._id < (__TRMTM_CUBEBASEID__ + __TRMTM_NUMBERCUBES__))
		return (new TestRoomGame::Cube (def._id));

	return (result);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestRoomGame::InputHandlerBehaviour::onKeyboardEvent (QGAMES::KeyBoardEventData* dt)
{
	if (dt -> _internalCode == QGAMES::KeyCode::QGAMES_ESCAPE && !dt -> _on)
		game () -> exitGame (); 
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
TestRoomGame::MazeModel::MazeModel ()
	: QGAMES::MazeModel ()
{
	// TODO
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
TestRoomGame::Room::Room ()
	: QGAMES::Room (
		__TRMTM_ROOMEXAMPLE__, 
		__TRMTM_TYPEROOMEXAMPLE__,
		QGAMES::RoomWalls 
		{ 
			{ 
				0,
				new QGAMES::RoomWall ( 
					0,
					__TRMTM_BRICKTEMPLATEID__,
					QGAMES::Position (__BD 50, __BD 50, __BD __TRMTM_BH__),
					8 /* length */, 3 /* high */, 1 /* depth */,
					{
						{
							0,
							new QGAMES::RoomWall::WallElement (
								0,
								dynamic_cast <QGAMES::RoomElement*> 
									(QGAMES::Game::game () -> entity (__TRMTM_PICTURETEMPLATEID__)),
								QGAMES::RoomWall::Hole (2.0f /** length */, 2.0f /** height */, 1.0f /** depth */)
							)
						}
					})
			},
			{ 
				1,
				new QGAMES::RoomWall ( 
					0,
					__TRMTM_BRICKTEMPLATEID__,
					QGAMES::Position (__BD 50, __BD 400, __BD __TRMTM_BH__),
					8 /* length */, 3 /* high */, 1 /* depth */,
					{
						{
							0,
							new QGAMES::RoomWall::WallElement (
								0,
								dynamic_cast <QGAMES::RoomElement*> 
									(QGAMES::Game::game () -> entity (__TRMTM_PICTURETEMPLATEID__)),
								QGAMES::RoomWall::Hole (1.0f, 2.0f, 1.0f)
							)
						}
					})
			},
			{ 
				2,
				new QGAMES::RoomWall ( 
					0,
					__TRMTM_BRICKTEMPLATEID__,
					QGAMES::Position (__BD 100, __BD 150, __BD __TRMTM_BH__),
					1 /* length */, 2 /* high */, 5 /* depth */,
					{
						{
							0,
							new QGAMES::RoomWall::WallElement (
								0,
								dynamic_cast <QGAMES::RoomElement*> 
									(QGAMES::Game::game () -> entity (__TRMTM_PICTURETEMPLATEID__)),
								QGAMES::RoomWall::Hole (1.0f, 0.0f, 0.0f)
							)
						}
					})
			},
			{ 
				3,
				new QGAMES::RoomWall ( 
					0,
					__TRMTM_BRICKTEMPLATEID__,
					QGAMES::Position (__BD 400, __BD 150, __BD __TRMTM_BH__),
					1 /* length */, 5 /* high */, 5 /* depth */,
					{
						{
							0,
							new QGAMES::RoomWall::WallElement (
								0,
								dynamic_cast <QGAMES::RoomElement*> 
									(QGAMES::Game::game () -> entity (__TRMTM_PICTURETEMPLATEID__)),
								QGAMES::RoomWall::Hole (0.25f, 1.0f, 1.0f)
							)
						}
					})
			}
		},
		QGAMES::Room::Decoration
		{
			{
				0,
				new QGAMES::Room::DecorativeElement (
					0,
					dynamic_cast <QGAMES::RoomElement*> 
						(QGAMES::Game::game () -> entity (__TRMTM_PICTURETEMPLATEID__)),
					QGAMES::Position (__BD 200, __BD 200, __BD __TRMTM_BH__)
				)
			}
		}
	) 
{
	// Nothing else to do...
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestRoomGame::Scene::Scene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn, 
	const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
//		: QGAMES::BasicRoomScene (c, new TestRoomGame::Room (), m, cn, p, ePL),
		: QGAMES::BasicRoomScene (c,
				dynamic_cast <Game*> (QGAMES::Game::game ()) -> roomBuilder () -> room (__TRMTM_ROOMEXAMPLE__ + 1), m, cn, p, ePL),
		  _knight (nullptr)
{ 
}

// ---
void TestRoomGame::Scene::initialize ()
{
	setMap (__TRMTM_MAPEXAMPLE__);

	_knight = dynamic_cast <TestRoomGame::Knight*> (game () -> entity (__TRMTM_KNIGHBASETID__));
	addEntity (_knight);

	QGAMES::BasicRoomScene::initialize ();

	_knight -> setPositionOnBase (QGAMES::Position (__BD (5 * __TRMTM_VW__), __BD (6 * __TRMTM_VL__), __BD __TRMTM_BH__));
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestRoomGame::World::initialize ()
{
	setScene (__TRMTM_SCENEEXAMPLE__);

	QGAMES::WorldBasedOnRooms::initialize ();
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::World* TestRoomGame::WorldBuilder::createWorldObject (int no, const QGAMES::Scenes& s, 
		const QGAMES::WorldProperties& p)
{
	QGAMES::World* result = NULL;

	if (no == __TRMTM_WORLDEXAMPLE__)
		result = new TestRoomGame::World (no, s, p);

	return (result);
}

// ---
QGAMES::Scene* TestRoomGame::WorldBuilder::createSceneObject (int ns, const QGAMES::Maps& m, 
		const QGAMES::Scene::Connections& cn, 
		const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
{
	QGAMES::Scene* result = NULL;

	if (ns == __TRMTM_SCENEEXAMPLE__)
		result = new TestRoomGame::Scene (ns, m, cn, p, ePL);

	return (result);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestRoomGame::Game::Game ()
	: QGAMES::ArcadeGame (new SDLGame (), QGAMES::Worlds (), 120)
{ 
#ifdef NDEBUG
	// If the data parameter exists, then changes the default temporal file...
	// It makes sen se only when running release version of the app!
	if (dynamic_cast <QGAMES::InZipResourceReader*> (QGAMES::ResourceReader::resourceReader ()) != NULL)
		((QGAMES::InZipResourceReader*) QGAMES::ResourceReader::resourceReader ()) -> 
			setTempLocation (parameter (std::string (__GAME_DATADIRPROPERTYNAME__)));
#endif
}

// ---
void TestRoomGame::Game::initialize ()
{
	if (isInitialized ())
		return; // Only once...

	QGAMES::ArcadeGame::initialize ();
	if (!isInitialized ())
		return;

	mainScreen () -> windowAtCenter ();
	mainScreen () -> setIcon (form (__GAMEICON__));

	// Adds the score objects...
	addScoreObjects ();

	// Sets the camera to project in orthogonal
	mainScreen () -> currentCamera () -> setProjectType (QGAMES::Camera::ProjectionType::_QUICKDIEDRIC);
	mainScreen () -> setPosition (QGAMES::Position (__BD -(64 * 6), __BD 0, __BD (64 * 1)));

	// Adds the world to be used, initializes it, and set it as the main one!
	addWorld (__TRMTM_WORLDEXAMPLE__);
	world (__TRMTM_WORLDEXAMPLE__) -> initialize ();
	setWorld (__TRMTM_WORLDEXAMPLE__);
}

// ---
void TestRoomGame::Game::finalize ()
{
	removeScoreObjects ();

	QGAMES::ArcadeGame::finalize ();
}

// ---
void TestRoomGame::Game::addScoreObjects ()
{
	addScoreObject (new TestRoomGame::FramerateScoreObject ());
}

// ---
QGAMES::MapBuilder* TestRoomGame::Game::createMapBuilder ()
{ 
	QGAMES::MapBuilder* result = 
		new QGAMES::MapBuilder (std::string (parameter (__GAME_MAPSOBJECTSFILE__)));
	result -> addAddsOn (new QGAMES::ObjectMapBuilderAddsOn (objectBuilder ()));
	result -> addAddsOn (new TestRoomGame::MapBuilderAddsOn (dynamic_cast <QGAMES::Sprite2DBuilder*> (formBuilder ())));
	return (result); 
}
// --------------------------------------------------------------------------------
