#include "TestCommonOGRE.hpp"
#include <Overlay/OgreOverlayManager.h>
#include <Overlay/OgreOverlayContainer.h>
#include <CommonOGRE/ogregame.hpp>

#define __OGREDEMOBLUE__		QGAMES::Color (0, 0, 255, 255)
#define __OGREDEMOTRSPBLUE__	QGAMES::Color (0, 0, 255, 100)
#define __OGREDEMOGREEN__		QGAMES::Color (0, 255, 0, 255)
#define __OGREDEMORED__			QGAMES::Color (255, 0, 0, 255)
#define __OGREDEMOLIGHTBLUE__	QGAMES::Color (0, 0, 255, 100)
#define __OGREDEMOLIGHTGREEN__	QGAMES::Color (0, 255, 0, 100)
#define __OGREDEMOLIGHTRED__	QGAMES::Color (255, 0, 0, 100)
#define __OGREDEMOWHITE__		QGAMES::Color (255, 255, 255, 255)
#define __OGREDEMOLIGHTWHITE__	QGAMES::Color (255, 255, 255, 100)
#define __OGRECOMMYLOGOID__		70001

// --------------------------------------------------------------------------------
// ---
QGAMES::Entity* TestOGRE::Entity::clone () const
{ 
	QGAMES::Entity* result = new TestOGRE::Entity (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Entity* TestOGRE::EntityBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	QGAMES::Entity* result = NULL;
	if (def._id >= __OGREHEADBASEENTITY__ && def._id < (__OGREHEADBASEENTITY__ + __OGREHEADNUMBERENTITIES__))
		result = new TestOGRE::Entity (def._id);
	else if (def._id >= __OGRESCOREOBJBASEENTITY__ && def._id < (__OGRESCOREOBJBASEENTITY__ + __OGRENUMBERSCOREOBJECTS__))
		result = new TestOGRE::ScoreObject (def._id);
	return (result);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestOGRE::InputHandlerBehaviour::onKeyboardEvent (QGAMES::KeyBoardEventData* dt)
{
	if (dt -> _internalCode == QGAMES::KeyCode::QGAMES_ESCAPE && !dt -> _on)
		game () -> exitGame (); 
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestOGRE::World::initialize ()
{
	setScene (__OGREHEADSCENE__);
	QGAMES::World::initialize ();

	reStartAllCounters ();

	_pX = __BD 100;
	_pY = __BD 0;
}

// ---
void TestOGRE::World::updatePositions ()
{
	QGAMES::World::updatePositions ();

	if (counter (_COUNTERCHANGEANGLE) -> isEnd ())
	{
		for (int i = 0; i < 3; i++)
			counter (_COUNTINGANGLE) -> isEnd (); // Just to increment, as many degrees as the loop is
		_pX = __BD (100.0f * cos (__BD counter (_COUNTINGANGLE) -> value () * __PI / __BD 180));
		_pY = __BD (100.0f * sin (__BD counter (_COUNTINGANGLE) -> value () * __PI / __BD 180));
	}
}

// ---
void TestOGRE::World::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::World::drawOn (s,p);

	// Draws a set of things...	
	// First of all keeps OGRE's camera before it is replaced
	QGAMES::Camera* lC = s -> currentCamera ();
	// Using the main camnera (instead of the camera used by the OGRE system)
	s -> setCurrentCamera (std::string (__QGAMES_MAINCAMERAID__));
	// It is time to know where the center is...
	int sW2 = s -> visualWidth () >> 1;
	int sH2 = s -> visualHeight () >> 1;

	// Set the camara at the center of thescreen and draws a guiding white-filled 
	s -> currentCamera () -> setPosition (QGAMES::Position::_cero);
	s -> drawCircle (QGAMES::Position (__BD sW2, __BD sH2, __BD 0), 
		QGAMES::Vector::_zNormal, __BD 100, __BD 100, __OGREDEMOLIGHTWHITE__, true); // Inside...
	s -> drawCircle (QGAMES::Position (__BD sW2, __BD sH2, __BD 0), 
		QGAMES::Vector::_zNormal, __BD 100, __BD 100, __OGREDEMOWHITE__, false); // Outside...
	// with also a commty lgo inthe center of that circle...
	QGAMES::Form* frm = QGAMES::Game::game () -> form (__OGRECOMMYLOGOID__);
	frm -> frame (0) -> // The first frame always...no more
		drawOn (s, QGAMES::Position (__BD sW2 - (frm -> width () >> 1), __BD sH2 - (frm -> height () >> 1), __BD 0), 100);
	frm -> frame (0) -> // Draws also the logo at the top left corner of the screen...
		drawOn (s, QGAMES::Position (__BD 10, __BD 10, __BD 0), 255);

	// Now the rest of the information moves around that guiding circle...
	s -> currentCamera () -> setPosition (QGAMES::Position (_pX, _pY, 0));

	// Rectangle. Normal to the Z axis...
	s -> drawRectangle 
		(QGAMES::Rectangle (QGAMES::Position (__BD (sW2 - 150), __BD (sH2 - 150), __BD 0),
							QGAMES::Position (__BD (sW2 + 150), __BD (sH2 + 150), __BD 0)),
		 __OGREDEMOTRSPBLUE__, true); // Inside
	s -> drawRectangle 
		(QGAMES::Rectangle (QGAMES::Position (__BD (sW2 - 150), __BD (sH2 - 150), __BD 0),
							QGAMES::Position (__BD (sW2 + 150), __BD (sH2 + 150), __BD 0)),
		 __OGREDEMOBLUE__, false); // Outside

	// 4 points
	s -> drawPoint (QGAMES::Position (__BD (sW2 - 25), __BD sH2, __BD 0), __OGREDEMOWHITE__);
	s -> drawPoint (QGAMES::Position (__BD (sW2 + 25), __BD sH2, __BD 0), __OGREDEMOWHITE__);
	s -> drawPoint (QGAMES::Position (__BD sW2, __BD (sH2 - 25), __BD 0), __OGREDEMOWHITE__);
	s -> drawPoint (QGAMES::Position (__BD sW2, __BD (sH2 + 25), __BD 0), __OGREDEMOWHITE__);

	// A line
	s -> drawLine 
		(QGAMES::Position (__BD (sW2 - 100), __BD (sH2 - 100), __BD 0),
		 QGAMES::Position (__BD (sW2 + 100), __BD (sH2 + 100), __BD 0),
		 __OGREDEMOGREEN__);

	// A triangle
	s -> drawTriangle 
		(QGAMES::Position (__BD (sW2 - 100), __BD (sH2 - 100), __BD 0),
		 QGAMES::Position (__BD (sW2 + 100), __BD (sH2 - 100), __BD 0),
		 QGAMES::Position (__BD (sW2 - 100), __BD (sH2 + 100), __BD 0),
		 __OGREDEMOLIGHTBLUE__, false); // There is only one plane plassing through three points...

	// A polygon
	QGAMES::Positions pos (5);
	pos [0] = QGAMES::Position (__BD (sW2 - 50), __BD (sH2 - 50), __BD 0);
	pos [1] = QGAMES::Position (__BD sW2, __BD (sH2 - 75), __BD 0);
	pos [2] = QGAMES::Position (__BD (sW2 + 50), __BD (sH2 - 50), __BD 0);
	pos [3] = QGAMES::Position (__BD (sW2 + 50), __BD (sH2 + 50), __BD 0);
	pos [4] = QGAMES::Position (__BD (sW2 - 50), __BD (sH2 + 50), __BD 0);
	s -> drawPolygon (QGAMES::Polygon (pos), __OGREDEMOLIGHTRED__, true);

	// A ellipse
	s -> drawCircle (QGAMES::Position (__BD sW2, __BD sH2, __BD 0), 
		QGAMES::Vector::_zNormal, __BD 200, __BD 50, __OGREDEMOLIGHTGREEN__, false);

	// Restore OGRE's camera
	s -> setCurrentCamera (lC);
}

// ---
__IMPLEMENTCOUNTERS__ (TestOGRE::World::Counters)
{
	addCounter (new QGAMES::Counter 
		(TestOGRE::World::_COUNTERCHANGEANGLE, 1, 0, true, true));
	addCounter (new QGAMES::Counter 
		(TestOGRE::World::_COUNTINGANGLE, 360, 0, true, true));
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestOGRE::Scene::initialize ()
{
	// Prepare the entities...
	for (int i = 0; i < __OGREHEADNUMBERENTITIESY__; i++)
	{
		for (int j = 0; j < __OGREHEADNUMBERENTITIESX__; j++)
		{
			QGAMES::Entity* ety = game () -> entity (1 + (i * __OGREHEADNUMBERENTITIESX__) + j);
			ety -> setCurrentForm (__OGREHEADFORM__);
			ety -> setPosition (QGAMES::Position (__BD (- (__OGREHEADNUMBERENTITIESX__ * (__OGRESEPAMONGENTITIES__ >> 1)) + 
				(j * __OGRESEPAMONGENTITIES__)), __BD 0, __BD (- __OGRESEPAMONGENTITIES__ - (i * __OGRESEPAMONGENTITIES__))));
			ety -> setVisible (((i * __OGREHEADNUMBERENTITIESX__) + j) % 2 == 0);
			addEntity (ety);
		}
	}


	// Sets the active map of the scene...
	setMap (__OGREHEADMAP__);

	// Initializa the counters and the switchs...
	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	QGAMES::Scene::initialize ();
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestOGRE::Scene::updatePositions ()
{
	if (counter (TestOGRE::Scene::_COUNTERCHANGESCENE) -> isEnd ())
	{
		onOffSwitch (TestOGRE::Scene::_SWITCHSTATEACTIVE) -> change ();
		for (int i = 0; i < __OGREHEADNUMBERENTITIESY__; i++)
		{
			for (int j = 0; j < __OGREHEADNUMBERENTITIESX__; j++)
			{
				QGAMES::Entity* ety = game () -> entity (1 + (i * __OGREHEADNUMBERENTITIESX__) + j); 
				ety -> setVisible (!ety -> isVisible ()); // Changes the status...
			}
		}
	}

	QGAMES::Scene::updatePositions ();
}

// ---
__IMPLEMENTCOUNTERS__ (TestOGRE::Scene::Counters)
{
	addCounter (new QGAMES::Counter 
		(TestOGRE::Scene::_COUNTERCHANGESCENE, 180, 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (TestOGRE::Scene::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (TestOGRE::Scene::_SWITCHSTATEACTIVE, false));
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::World* TestOGRE::WorldBuilder::createWorldObject (int no, const QGAMES::Scenes& s, 
		const QGAMES::WorldProperties& p)
{
	QGAMES::World* result = NULL;
	if (no == __OGREHEADWORLD__)
		result = new TestOGRE::World (no, s, p);
	return (result);
}

// ---
QGAMES::Scene* TestOGRE::WorldBuilder::createSceneObject (int ns, const QGAMES::Maps& m, 
		const QGAMES::Scene::Connections& cn, 
		const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
{
	QGAMES::Scene* result = NULL;
	if (ns == __OGREHEADSCENE__)
		result = new TestOGRE::Scene (ns, m, cn, p, ePL);
	return (result);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestOGRE::Game::Game ()
	: QGAMES::ArcadeGame (new OGREGame (), QGAMES::Worlds ()),
	  _cameraManager (NULL), // The camera is initialized in the initialization method...
	  _count (0), _up (1) // Initially up...(if the automatic loop is set!
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
void TestOGRE::Game::initialize ()
{
	if (isInitialized ())
		return; // Only once...

	// Normal initialization...
	QGAMES::ArcadeGame::initialize ();
	if (!isInitialized ())
		return;

	// Moves the main screen to the center..
	mainScreen () -> windowAtCenter ();
	// ...and check whether the icon can be or not changed...
	mainScreen () -> setIcon (form (70000));

	// Adds the score objects...
	addScoreObjects ();

	// Adds the world used in the game...
	addWorld (__OGREHEADWORLD__);
	// Sets and initialize it...
	setWorld (__OGREHEADWORLD__);
	world (__OGREHEADWORLD__) -> initialize ();

	// Sets the camera controller...
	_cameraManager = 
		dynamic_cast <QGAMES::CameraControlledByInputHandler*> 
			(mainScreen () -> camera (std::string (__OGREMAINSCENEMANAGERID__) + 
									  std::string (":") + 
									  std::string (__OGREMAINCAMERAID__)));
	assert (_cameraManager); // Just in case...
	_cameraManager -> setInputHandler (inputHandler ()); // To observe its movements...
	// ...and additionaly the main camera is adjusted to this game...
	_cameraManager -> setPosition (QGAMES::Position (__BD 0, __BD 0, __BD 100));
	_cameraManager -> pitch (__PI); // Look just to the opposite direction...(turn around)

	// ...and starts the sound...
	soundBuilder () -> sound (__OGREHEADINTROSOUND__) -> play (1);
}

// ---
void TestOGRE::Game::updatePositions ()
{
	// Moves the camera...
	_count += _up;
	if (_count > 90 || _count < -90)
		_up = -_up;
	_cameraManager -> yaw (__BD (0.15f * __PI / 180 * _up));

	// ...and update the test of the game...
	QGAMES::ArcadeGame::updatePositions ();
}

// ---
void TestOGRE::Game::finalize ()
{
	removeScoreObjects ();
	QGAMES::ArcadeGame::finalize ();
}

// ---
void TestOGRE::Game::addScoreObjects ()
{
	QGAMES::ScoreObject* obj1 = 
		dynamic_cast <QGAMES::ScoreObject*> (entityBuilder () -> entity (__OGRESCOREOBJBASEENTITY__));
	obj1 -> setCurrentForm (__OGREHEADFORM__);
	obj1 -> setPosition (QGAMES::Position (-50, 0, -350));
	obj1 -> setVisible (true);
	addScoreObject (obj1);

	QGAMES::ScoreObject* obj2 = 
		dynamic_cast <QGAMES::ScoreObject*> (entityBuilder () -> entity (__OGRESCOREOBJBASEENTITY__ + 1));
	obj2 -> setCurrentForm (__ROBOTFORM__);
	obj2 -> setPosition (QGAMES::Position (50, -50, -350));
	obj2 -> setVisible (true);
	addScoreObject (obj2);
	obj2 -> setCurrentAnimation (2); // Walk...
}
// --------------------------------------------------------------------------------
