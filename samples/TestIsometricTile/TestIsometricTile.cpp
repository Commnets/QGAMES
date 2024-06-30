#include "TestIsometricTile.hpp"
#include <SDL/sdlinclude.hpp>

#define __GAMEICON__			70000

// --------------------------------------------------------------------------------
// ---
TestIsometric::FramerateScoreObject::FramerateScoreObject ()
	: QGAMES::ScoreObjectNumber (100, 60, QGAMES::Forms (), 16)
{
	_forms.insert (QGAMES::Forms::value_type (10000, 
		QGAMES::Game::game () -> form (10000)));
	setCurrentForm (10000);
	setPosition (QGAMES::Position (__BD 10, __BD 10, __BD 0));
}

// ---
void TestIsometric::FramerateScoreObject::updatePositions ()
{
	QGAMES::ScoreObjectNumber::updatePositions ();

	setNumber (game () -> currentLoopsPerSecond ());
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
//---
void TestIsometric::Entity::drawOn (QGAMES::Screen* scr, const QGAMES::Position& pos)
{
	QGAMES::Entity::drawOn (scr, pos);

	QGAMES::Box limits = roundingBox ();
	scr -> drawRectangle (limits.bottomRectangle (), __TITM_C5__, true);
	scr -> drawRectangle (limits.topRectangle (), __TITM_C1__, false);
	scr -> drawRectangle (limits.bottomRectangle (), __TITM_C1__, false);
	scr -> drawRectangle (limits.frontRectangle (), __TITM_C1__, false);
	scr -> drawRectangle (limits.backRectangle (), __TITM_C1__, false);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
QGAMES::Entity* TestIsometric::Cube::clone () const
{ 
	QGAMES::Entity* result = new TestIsometric::Cube (_id);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void TestIsometric::Cube::initialize ()
{ 
	setCurrentForm (FID); 
	setCurrentAspect (AID);

	setPositionOnBase (QGAMES::Position 
		(__BD __TITM_ITXC__ * __BD __TITM_VW__, 
		 __BD __TITM_ITYC__ * __BD __TITM_VL__, 
		 __BD __TITM_BH__));

	QGAMES::Entity::initialize ();
}

// ---
void TestIsometric::Cube::drawOn (QGAMES::Screen* scr, const QGAMES::Position& pos)
{
	// To draw the shadow...
	QGAMES::Box limits = roundingBox ();
	QGAMES::bdata shdSize = basePosition ().posZ () / __BD 3;
	if (shdSize < __BD -20) shdSize = __BD -20;
	QGAMES::Polygon bShadow = 
		limits.bottomRectangle ().bigger (-shdSize).projectOver (QGAMES::Position::_cero, QGAMES::Vector::_zNormal) +
		QGAMES::Vector (__BD 0, __BD 0, __BD __TITM_BH__);
	scr -> drawPolygon (bShadow, __TITM_C5__, true);

	// ...and on top the cube itself
	TestIsometric::Entity::drawOn (scr, pos);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
QGAMES::Entity* TestIsometric::Knight::clone () const
{ 
	QGAMES::Entity* result = new TestIsometric::Knight (_id);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void TestIsometric::Knight::initialize ()
{ 
	setCurrentForm (FID); 
	setCurrentAspect (AID); 

	setPositionOnBase (QGAMES::Position 
		(__BD __TITM_ITXK__ * __BD __TITM_VW__, 
		 __BD __TITM_ITYK__ * __BD __TITM_VL__, 
		 __BD __TITM_BH__));

	QGAMES::Entity::initialize ();
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestIsometric::InputHandlerBehaviour::onKeyboardEvent (QGAMES::KeyBoardEventData* dt)
{
	if (dt -> _internalCode == QGAMES::KeyCode::QGAMES_ESCAPE && !dt -> _on)
		game () -> exitGame (); 
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
//---
void TestIsometric::Scene::initialize ()
{
	setMap (__TITM_EXAMPLEMAPID__);

	addEntity (_cube = dynamic_cast <TestIsometric::Cube*> (game () -> entity (__TITM_CUBEBASEID__)));
	addEntity (_knight = dynamic_cast <TestIsometric::Knight*> (game () -> entity (__TITM_KNIGHTBASEID__)));

	// The entities use din the game are also initialized here
	QGAMES::Scene::initialize ();

	_movZDir = __BD 1;
}

// ---
void TestIsometric::Scene::updatePositions ()
{
	QGAMES::Scene::updatePositions ();

	if ((_cube -> basePosition ().posZ () < __BD -100) || (_cube -> basePosition ().posZ () > __TITM_BH__))
		_movZDir *= -1;

	_cube -> setPosition (_cube -> position () + QGAMES::Vector (__BD 0, __BD 0, _movZDir));
}

// ---
void TestIsometric::Scene::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Scene::drawOn (s);

#ifndef NDEBUG
	// Draws the base
	for (int i = 0; i <= 15; i++)
	{
		// Lines on the top...
		s -> drawLine (QGAMES::Position (__BD 0, __BD (i * __TITM_VL__), __BD __TITM_BH__), 
					   QGAMES::Position (__BD (15 * __TITM_VW__), __BD (i * __TITM_VL__), __BD __TITM_BH__), __TITM_C1__);
		s -> drawLine (QGAMES::Position (__BD (i * __TITM_VW__), __BD 0, __BD __TITM_BH__),
					   QGAMES::Position (__BD (i * __TITM_VW__), __BD (15 * __TITM_VL__), __BD __TITM_BH__), __TITM_C1__);

		// limits of the most visible tiles...
		s -> drawLine (QGAMES::Position (__BD (15 * __TITM_VW__), __BD (i * __TITM_VL__), __BD __TITM_BH__),
					   QGAMES::Position (__BD (15 * __TITM_VW__), __BD (i * __TITM_VL__), __BD __TITM_BH__ + __BD __TITM_VH__), 
					   __TITM_C1__);
		s -> drawLine (QGAMES::Position (__BD (i * __TITM_VW__), __BD (15 * __TITM_VL__), __BD __TITM_BH__),
					   QGAMES::Position (__BD (i * __TITM_VW__), __BD (15 * __TITM_VL__), __BD __TITM_BH__ + __BD __TITM_VH__), 
					   __TITM_C1__);
	}

	// Draws the center of the maze
	s -> drawLine (QGAMES::Position (__BD 7.5 * __BD __TITM_VW__ - __BD 10, __BD 7.5 * __BD __TITM_VL__, __BD __TITM_BH__),
				   QGAMES::Position (__BD 7.5 * __BD __TITM_VW__ + __BD 10, __BD 7.5 * __BD __TITM_VL__, __BD __TITM_BH__), 
				   __TITM_C4__);
	s -> drawLine (QGAMES::Position (__BD 7.5 * __BD __TITM_VW__, __BD 7.5 * __BD __TITM_VL__ - __BD 10, __BD __TITM_BH__),
				   QGAMES::Position (__BD 7.5 * __BD __TITM_VW__, __BD 7.5 * __BD __TITM_VL__ + __BD 10, __BD __TITM_BH__), 
				   __TITM_C4__);

	// Draws the limits
	for (int i = 0; i < 5; i++)
	{
		s -> drawLine (QGAMES::Position (__BD 0, __BD 0, __BD (-(i * 10) + __BD __TITM_BH__)),
					   QGAMES::Position (__BD (15 * __TITM_VW__), __BD 0, __BD (-(i * 10) + __BD __TITM_BH__)), 
					   __TITM_C2__);
		s -> drawLine (QGAMES::Position (__BD 0, __BD 0, __BD (-(i * 10) + __TITM_BH__)),
					   QGAMES::Position (__BD 0, __BD (15 * __TITM_VL__), __BD (-(i * 10) + __TITM_BH__)), __TITM_C2__);
		s -> drawLine (QGAMES::Position (__BD 0, __BD (15 * __TITM_VL__), __BD (-(i * 10) + __TITM_BH__)),
					   QGAMES::Position (__BD (15 * __TITM_VW__), __BD (15 * __TITM_VL__), __BD (-(i * 10) + __TITM_BH__)), 
					   __TITM_C2__);
		s -> drawLine (QGAMES::Position (__BD (15 * __TITM_VW__), __BD 0, __BD (-(i * 10) + __TITM_BH__)),
					   QGAMES::Position (__BD (15 * __TITM_VW__), __BD (15 * __TITM_VL__), __BD (-(i * 10) + __TITM_BH__)), 
					   __TITM_C2__);
	}
#endif
}

// ---
void TestIsometric::Scene::finalize ()
{
	QGAMES::Scene::finalize ();

	removeEntity (game () -> entity (__TITM_CUBEBASEID__));
	removeEntity (game () -> entity (__TITM_KNIGHTBASEID__));

	_cube = NULL;
	_knight = NULL;
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestIsometric::Map::initialize ()
{
	QGAMES::TiledMap::initialize ();

	for (auto i : layers ())
		i -> setPosition (QGAMES::Position::_cero);
}

// ---
void TestIsometric::Map::updatePositions ()
{
	QGAMES::TiledMap::updatePositions ();

	bool limit = false;
	for (auto i : layers ())
		if (i -> position ().posX () > __BD 300 || i -> position ().posX () < __BD 0)
			limit |= true;
	if (limit)
		_movXDir *= -1;

	for (auto i : layers ())
		i -> setPosition (i -> position () + QGAMES::Vector (__BD _movXDir, __BD 0, __BD 0));
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestIsometric::Game::Game ()
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
void TestIsometric::Game::initialize ()
{
	if (isInitialized ())
		return; // Only once...

	// Normal initialization...
	QGAMES::ArcadeGame::initialize ();
	if (!isInitialized ())
		return;

	// Moves the main screen to the center..
	mainScreen () -> windowAtCenter ();
	// ...and set the id icon...
	mainScreen () -> setIcon (form (__GAMEICON__));

	// Adds the score objects...
	addScoreObjects ();

	// Sets the camera to project in isometric
	mainScreen () -> currentCamera () -> setProjectType (QGAMES::Camera::ProjectionType::_QUICKDIEDRIC);
	// Locates the maze in the middle of the screen.
	mainScreen () -> setPosition (QGAMES::Position (__BD -(64 * 6), __BD 0, __BD (64 * 1)));

	// Adds the world to be used, initializes it, and set it as the main one!
	addWorld (__TITM_EXAMPLEWORLDID__);
	world (__TITM_EXAMPLEWORLDID__) -> initialize ();
	setWorld (__TITM_EXAMPLEWORLDID__);
}

// ---
void TestIsometric::Game::finalize ()
{
	removeScoreObjects ();

	QGAMES::ArcadeGame::finalize ();
}

// ---
void TestIsometric::Game::addScoreObjects ()
{
	addScoreObject (new TestIsometric::FramerateScoreObject ());
}

// ---
QGAMES::MapBuilder* TestIsometric::Game::createMapBuilder ()
{ 
	QGAMES::MapBuilder* result = 
		new QGAMES::MapBuilder (std::string (parameter (__GAME_MAPSOBJECTSFILE__)));
	result -> addAddsOn (new QGAMES::ObjectMapBuilderAddsOn (objectBuilder ()));
	result -> addAddsOn (new TestIsometric::MapBuilderAddsOn (dynamic_cast <QGAMES::Sprite2DBuilder*> (formBuilder ())));
	return (result); 
}
// --------------------------------------------------------------------------------
