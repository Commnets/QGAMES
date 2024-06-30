#include "TestSDL.hpp"

// --------------------------------------------------------------------------------
// ---
void TestSDL::SelectionDecorator::decorate (QGAMES::Screen* scr, const QGAMES::Position& p, QGAMES::Widget* wdt)
{
	_form -> drawOn (scr, 0, p + wdt -> position () - QGAMES::Vector (__BD 15, __BD 5, __BD 0));
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestSDL::MainWidget::initialize ()
{ 
	QGAMES::CompositeWidget::initialize (); 
	_inputDeviceType = 0; _startSelected = false; 
	widget (__GAMETEST_KEYBOARDWIDGETID__) -> 
		setDecorator (new TestSDL::SelectionDecorator (QGAMES::Game::game () -> 
			form (__GAMETEST_SELECTIONFORMDECORATOR__)));
}

// ---
void TestSDL::MainWidget::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Vector toAdd (__BD 20, __BD 20, __BD 0);
	s -> drawRoundRectangle (QGAMES::Rectangle (position () - toAdd, 
			position () + QGAMES::Vector (__BD width (), __BD height (), __BD 0) + toAdd), 
		5, __GAMETEST_YELLOWCOLOR__, true); 
	s -> drawRoundRectangle (QGAMES::Rectangle (position () - (toAdd / 2), 
			position () + QGAMES::Vector (__BD width (), __BD height (), __BD 0) + (toAdd / 2)), 
		5, __GAMETEST_GRAYCOLOR__, true); 
	QGAMES::CompositeWidget::drawOn (s, p);
}

// ---
void TestSDL::MainWidget::processEvent (const QGAMES::Event& e)
{
	if (e.code () == __QGAMES_WIDGETSELECTEDEVENT__)
	{
		QGAMES::Widget* ntf = (QGAMES::Widget*) (e.data ());
		if (ntf -> id () == __GAMETEST_KEYBOARDWIDGETID__) 
		{ 
			_inputDeviceType = 0;
			widget (__GAMETEST_JOYSTICKWIDGETID__) -> setDecorator (NULL);
			widget (__GAMETEST_KEYBOARDWIDGETID__) -> 
				setDecorator (new TestSDL::SelectionDecorator (QGAMES::Game::game () -> 
					form (__GAMETEST_SELECTIONFORMDECORATOR__)));
		}
		else
		if (ntf -> id () == __GAMETEST_JOYSTICKWIDGETID__)
		{
			_inputDeviceType = 1;
			widget (__GAMETEST_KEYBOARDWIDGETID__) -> setDecorator (NULL);
			widget (__GAMETEST_JOYSTICKWIDGETID__) -> 
				setDecorator (new TestSDL::SelectionDecorator (QGAMES::Game::game () -> 
					form (__GAMETEST_SELECTIONFORMDECORATOR__)));
		}
		else
		if (ntf -> id () == __GAMETEST_STARTWIDGETID__)
			_startSelected = true;
	}
	else
		QGAMES::CompositeWidget::processEvent (e);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Widget* TestSDL::GUISystemBuilder::createWidget (const QGAMES::GUISystemBuilder::WidgetDefinition& def)
{
	return ((def._id == __GAMETEST_MAINWIDGETID__) // Whatever the type is...
		? new TestSDL::MainWidget (def._id) : QGAMES::StandardGUISystemBuilder::createWidget (def));
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestSDL::InputHandlerBehaviour::onKeyboardStatus (const std::vector <bool>& kS)
{
	if (game () -> activeState () -> type () != __GAMETEST_STATEPLAY__)
		return;

	TestSDL::MainCharacter* chr = 
		dynamic_cast <TestSDL::MainCharacter*> (game () -> entity (__GAMETEST_MAINCHARACTERID__));
	assert (chr); // Just in case...

	if (chr -> falling () || chr -> jumping ())
		return; // No keys accepted...

	bool kP = false;
	if (kS [QGAMES::KeyCode::QGAMES_S]) { kP = true; chr -> toTheRight (); }
	if (kS [QGAMES::KeyCode::QGAMES_A]) { kP = true; chr -> toTheLeft (); }
	if (kS [QGAMES::KeyCode::QGAMES_SPACE]) { kP = true; chr -> toJump (); }
	if (!kP) chr -> toStay ();
}

// ---
void TestSDL::InputHandlerBehaviour::manageKeyOnState (QGAMES::GameState* st, int kP)
{
	if (st -> type () != __GAMETEST_STATEPLAY__)
		return;

	if (kP == QGAMES::KeyCode::QGAMES_Z)
	{
		if (game () -> isGamePaused ()) game () -> continueGame ();
		else game () -> pauseGame ();
	}
}

// ---
void TestSDL::InputHandlerBehaviour::manageJoystickMovementOnState (QGAMES::GameState* st, int nJ, const QGAMES::Vector& dr)
{
	if (st -> type () != __GAMETEST_STATEPLAY__ || nJ != 0)
		return;

	TestSDL::MainCharacter* chr = 
		dynamic_cast <TestSDL::MainCharacter*> (game () -> entity (__GAMETEST_MAINCHARACTERID__));
	assert (chr); // Just in case...
	if (dr.posX () == 0) chr -> toStay ();
	else if (dr.posX () > 0) chr -> toTheRight ();
	else chr -> toTheLeft ();
}

// ---
void TestSDL::InputHandlerBehaviour::manageJoystickButtonOnState (QGAMES::GameState* st, int nJ, bool p, int nB, int bF)
{
	if (st -> type () != __GAMETEST_STATEPLAY__ || nJ != 0)
		return;

	TestSDL::MainCharacter* chr = 
		dynamic_cast <TestSDL::MainCharacter*> (game () -> entity (__GAMETEST_MAINCHARACTERID__));
	assert (chr); // Just in case...
	if (!p && nB == 0)
		chr -> toJump ();
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestSDL::MainCharacter::MainCharacter (int id, const QGAMES::Forms& frms, const QGAMES::Entity::Data& dt)
	: QGAMES::ArtistInATiledMap (id, frms, dt),
	  _layersToTest (), _layerUnder (NULL)
{ 
	registerTileHitHandler (__GAMETEST_LIMITTILE__, 
		new StandardTileCollisionHandler (__GAMETEST_LIMITTILE__, false));
	QGAMES::Buoys buoys;
	buoys.insert (QGAMES::Buoys::value_type (__GAMETEST_CHANGESTATEBUOYID__, 
		new TestSDL::MainCharacter::ToChangeStateBuoy ()));
	setBuoys (buoys);
}

// ---
QGAMES::Entity* TestSDL::MainCharacter::clone () const
{ 
	QGAMES::Entity* result = new TestSDL::MainCharacter (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
bool TestSDL::MainCharacter::canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
{
	return (!(((position () + d).posX () > (__MAPWIDTHINPIXELS___ - currentForm () -> 
				frameWidthForFrame (currentAspect ())) && lookingToRight ()) || 
		((position () + d).posX () < 0 && !lookingToRight ())));
}

// ---
bool TestSDL::MainCharacter::lookingToRight ()
{
	QGAMES::Entity::Animation* a = currentAnimation ();
	return ((a && 
				(a -> id () == __GAMETEST_MAINCHRANIMATIONFALLRIGHT__ ||
				 a -> id () == __GAMETEST_MAINCHRANIMATIONJUMPRIGHT__ ||
				 a -> id () == __GAMETEST_MAINCHRANIMATIONRUNRIGHT__ ||
				 a -> id () == __GAMETEST_MAINCHRANIMATIONSTAYRIGHT__)) ||
			 (!a && orientation ().posX () >= 0));
}

// ---
void TestSDL::MainCharacter::toTheRight ()
{
	if (!goingRight ())
		setCurrentState (__GAMETEST_MAINCHRRUNNINGRIGHTSTATE__);
}

// ---
bool TestSDL::MainCharacter::goingRight ()
{
	return (currentState () && currentState () -> id () == __GAMETEST_MAINCHRRUNNINGRIGHTSTATE__);
}

// ---
void TestSDL::MainCharacter::toTheLeft ()
{ 
	if (!goingLeft ())
		setCurrentState (__GAMETEST_MAINCHRRUNNINGLEFTSTATE__);
}

// ---
bool TestSDL::MainCharacter::goingLeft ()
{
	return (currentState () && currentState () -> id () == __GAMETEST_MAINCHRRUNNINGLEFTSTATE__);
}

// ---
void TestSDL::MainCharacter::toFall ()
{
	if (!falling ())
		setCurrentState (lookingToRight () ? __GAMETEST_MAINCHRFALLINGRIGHTSTATE__ : __GAMETEST_MAINCHRFALLINGLEFTSTATE__);
}

// ---
bool TestSDL::MainCharacter::falling ()
{
	return (currentState () && 
			(currentState () -> id () == __GAMETEST_MAINCHRFALLINGRIGHTSTATE__ || 
			 currentState () -> id () == __GAMETEST_MAINCHRFALLINGLEFTSTATE__));
}

// ---
void TestSDL::MainCharacter::toJump ()
{
	if (!jumping ())
		setCurrentState (lookingToRight () ? __GAMETEST_MAINCHRJUMPINGRIGHTSTATE__ : __GAMETEST_MAINCHRJUMPINGLEFTSTATE__);
}

// ---
bool TestSDL::MainCharacter::jumping ()
{
	return (currentState () && 
			(currentState () -> id () == __GAMETEST_MAINCHRJUMPINGRIGHTSTATE__ || 
			 currentState () -> id () == __GAMETEST_MAINCHRJUMPINGLEFTSTATE__));
}

// ---
void TestSDL::MainCharacter::toStay ()
{
	if (!staying ())
		setCurrentState (lookingToRight () ? __GAMETEST_MAINCHRSTAYINGRIGHTSTATE__ : __GAMETEST_MAINCHRSTAYINGLEFTSTATE__);
}

// ---
bool TestSDL::MainCharacter::staying ()
{
	return (currentState () && 
			(currentState () -> id () == __GAMETEST_MAINCHRSTAYINGRIGHTSTATE__ || 
			 currentState () -> id () == __GAMETEST_MAINCHRSTAYINGLEFTSTATE__));
}

// ---
void TestSDL::MainCharacter::initialize ()
{
	// The initial position is top left of the space (with in) the world...
	setPosition (QGAMES::Position (__BD 10, __BD -10, __BD 0));
	setOrientation (QGAMES::Vector (__BD 1, __BD 0, __BD 0));
	toFall (); // Falling to the right...

	QGAMES::ArtistInATiledMap::initialize ();
}

// ---
void TestSDL::MainCharacter::updatePositions ()
{
	if (_layerUnder != NULL && _layerUnder -> movement () != NULL)
		setPosition (position () + _layerUnder -> movement () -> vectorMoved ());

	_layerUnder = NULL;
	if (map ()) 
		_layersToTest = (__TM map ()) -> relevantLayers ();

	QGAMES::ArtistInATiledMap::updatePositions ();
}

// ---
void TestSDL::MainCharacter::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::ArtistInATiledMap::drawOn (s, p);

#ifndef NDEBUG
	s -> drawRectangle (collisionZone (), QGAMES::Color (120,120,120,255));

	int ePosX, ePosY, ePosZ, ntW, ntH, ntD;
	QGAMES::bdata m = (QGAMES::bdata) 0.0;
	QGAMES::Vector pv (__BD 0, __BD 0, __BD 0);
	adjustRelevants (m, pv);
	tilesOcuppied (ePosX, ePosY, ePosZ, ntW, ntH, ntD, QGAMES::Position::_cero, m, pv);

	int tWidth = (__TM map ()) -> tileWidth ();
	int tHeight = (__TM map ()) -> tileHeight ();
	int mWidth = map () -> width () / tWidth;
	int mHeight = map () -> height () / tHeight; 
	int nE = ePosY * mWidth + ePosX;
	for (int i = 0; i < ntH; i++)
	{
		for (int j = 0; j < ntW; j++)
		{
			int iPos = nE + (i * mWidth + j);
			QGAMES::Position pos1 (__BD ((iPos % mWidth) * tWidth), __BD ((iPos / mWidth) * tHeight), __BD 0);
			QGAMES::Position pos2 = pos1 + QGAMES::Vector (__BD tWidth, __BD tHeight, __BD 0);
			s -> drawRectangle (QGAMES::Rectangle (pos1, pos2), QGAMES::Color (255,255,0,255));
		}
	}
#endif
}

// ---
void TestSDL::MainCharacter::whenCollisionWith (const QGAMES::Tiles& t, 
			QGAMES::ArtistInATiledMap::TilePosition tP, QGAMES::TileLayer* l)
{
	bool end = false;
	for (QGAMES::Tiles::const_iterator i = t.begin (); i != t.end () && !end; i++)
		if ((end = ((*i) -> type () == __GAMETEST_LIMITTILE__)))
			notify (QGAMES::Event (__GAMETEST_GAMEFINISHED__, this)); // The game finishes when the freezer is bumped...
}

// ---
void TestSDL::MainCharacter::whenOnTile (const QGAMES::Tiles& t, QGAMES::TileLayer* l)
{
	_layersToTest.remove (l);

	QGAMES::Tile* rT = NULL;
	for (int i = 0; i < (int) t.size () && !rT; i++)
		if (t [i] != QGAMES::NullTile::_nullTile)
			rT = t [i];

	if (rT && rT -> type () == __GAMETEST_BASETILE__)
		_layerUnder = l;

	// Al layers tested...
	if (_layersToTest.empty ())
	{
		if (!_layerUnder)
		{
			if (!jumping ())
				toFall ();
		}
		else
		{
			if (falling () || jumping ())
				toStay ();
		}
	}
}

// ---
void TestSDL::MainCharacter::processEvent (const QGAMES::Event& e)
{
	if (e.code () == __QGAMES_MOVEMENTNOTPOSSIBLE__)
	{
		QGAMES::bdata tp = __BD -1; 
		if (jumping ()) tp = __BD 0; // Means to fall...
		else if (goingLeft () || goingRight ()) tp = __BD 1; // Means to stay...
		if (tp != __BD -1)
		{
			QGAMES::Buoy* b = buoy (__GAMETEST_CHANGESTATEBUOYID__);
			b -> setValue (tp); b -> active (true);
		}
	}
	else
		QGAMES::ArtistInATiledMap::processEvent (e);
}

// ---
void* TestSDL::MainCharacter::ToChangeStateBuoy::treatFor (QGAMES::Element* e)
{
	TestSDL::MainCharacter* chr = dynamic_cast <TestSDL::MainCharacter*> (e);
	assert (chr);
	if (value () == __BD 0) // Means to fall...
		chr -> toFall ();
	else if (value () == __BD 1) // Means to stay...
		chr -> toStay ();
	return (this);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Entity* TestSDL::EntityBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	QGAMES::Entity* result = NULL;

	if (def._id >= __GAMETEST_SCOREOBJECTBASEID__ && def._id < (__GAMETEST_SCOREOBJECTBASEID__ + __GAMETEST_NUMBERSCOREOBJECTS__))
		result = new TestSDL::ScoreObject (def._id);
	else if (def._id == __GAMETEST_MAINCHARACTERID__)
		result = new TestSDL::MainCharacter (def._id);

	return (result);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Movement* TestSDL::MovementBuilder::createMovement (const QGAMES::MovementBuilder::MovementDefinition& def)
{
	QGAMES::Movement* result = NULL;

	if (def._id == __GAMETEST_MAINCHRRUNMOVEMENT__)
		result = new QGAMES::SimpleLinearMovement (def._id, def._variables);
	else if (def._id == __GAMETEST_MAINCHRSTAYMOVEMENT__)
		result = new QGAMES::NoMovement (def._id, def._variables);
	else if (def._id == __GAMETEST_MAINCHRJUMPMOVEMENT__)
		result = new QGAMES::SimpleXYParabolicMovement (def._id, def._variables);
	else if (def._id == __GAMETEST_MAINCHRFALLMOVEMENT__)
		result = new QGAMES::SimpleLinearMovement (def._id, def._variables);

	return (result);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestSDL::World::initialize ()
{
	setScene (__GAMETEST_SCENEID__);
	QGAMES::Artist* e = 
		__AT (__AGM QGAMES::Game::game ()) -> artist (__GAMETEST_MAINCHARACTERID__);
	QGAMES::World::initialize ();
	e -> setMap (_activeScene -> activeMap ());
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestSDL::Scene::initialize ()
{
	QGAMES::Artist* e = 
		__AT (__AGM QGAMES::Game::game ()) -> artist (__GAMETEST_MAINCHARACTERID__);
	addEntity (e);
	setMap (__GAMETEST_MAPID__);

	QGAMES::Scene::initialize ();
}

// ---
void TestSDL::Scene::finalize ()
{
	QGAMES::Scene::finalize ();

	removeEntity (__AT (__AGM QGAMES::Game::game ()) -> artist (__GAMETEST_MAINCHARACTERID__));
	setMap (__MININT__);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Layers TestSDL::Map::relevantLayers ()
{
	if (_relevantLayers.empty ())
		for (QGAMES::TileLayers::const_iterator i = _tiles.begin (); i != _tiles.end (); i++)
			if ((*i).second -> name ().find (std::string (__GAMETEST_LAYERBASENAME__)) != std::string::npos ||
				(*i).second -> name ().find (std::string (__GAMETEST_LAYERPLATFORM__)) != std::string::npos)
				_relevantLayers.push_back ((*i).second);
	return (_relevantLayers);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestSDL::PlatformLayer::initialize ()
{
	QGAMES::TileLayer::initialize ();

	QGAMES::Layer::LinearMovementBehaviour* lM = 
		new QGAMES::Layer::LinearMovementBehaviour (0, 0, QGAMES::Position::_cero);
	// ...and then sets the rest of the parameters!
	lM -> setSpeed (existsProperty (std::string (__GAMETEST_LAYERPARAMSPEED__)) 
		? std::stoi (layerProperty (std::string (__GAMETEST_LAYERPARAMSPEED__))) : __GAMETEST_LAYERDEFAULTSPEED__);
	lM  -> setSteps (existsProperty (std::string (__GAMETEST_LAYERPARAMSTEPLENGTH__))
		? std::stoi (layerProperty (std::string (__GAMETEST_LAYERPARAMSTEPLENGTH__))) : __GAMETEST_LAYERDEFAULTSTEPLENGTH__);
	std::string defDir (__GAMETEST_LAYERDEFAULTDIRECTION__);
	if (existsProperty (std::string (__GAMETEST_LAYERPARAMDIRECTION__)))
		defDir = layerProperty (std::string (__GAMETEST_LAYERPARAMDIRECTION__));
	std::vector <QGAMES::bdata> dir = QGAMES::getElementsFromAsBDATA (defDir, ',', 3);
	lM -> setDirection (QGAMES::Vector (dir [0], dir [1], dir [2]));
	setMovement (_internalMovement = new QGAMES::Layer::BounceMovementBehaviour (lM));
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::World* TestSDL::WorldBuilder::createWorldObject (int no, const QGAMES::Scenes& s, 
		const QGAMES::WorldProperties& p)
{
	QGAMES::World* result = NULL;
	if (no == __GAMETEST_WORLDID__)
		result = new TestSDL::World (no, s, p);
	return (result);
}

// ---
QGAMES::Scene* TestSDL::WorldBuilder::createSceneObject (int ns, const QGAMES::Maps& m, 
		const QGAMES::Scene::Connections& cn, 
		const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
{
	QGAMES::Scene* result = NULL;
	if (ns == __GAMETEST_SCENEID__)
		result = new TestSDL::Scene (ns, m, cn, p, ePL);
	return (result);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
QGAMES::Tile* TestSDL::MapsBuilderAddsOn::createTile (int id, QGAMES::Form* form, int nf, 
		const QGAMES::TileProperties& p)
{
	QGAMES::Tile* result = NULL;
	if ((nf >= 15 && nf <= 26) ||
		(nf >= 34 && nf <= 41) ||
		(nf == 10) || (nf == 11) || (nf == 12))
		result = new QGAMES::Tile (id, form, nf, __GAMETEST_BASETILE__, p); // The base...
	else
	if (nf == 45 || nf == 46 || nf == 60 || nf == 61)
		result = new QGAMES::Tile (id, form, nf, __GAMETEST_LIMITTILE__, p); // The limit...to exit
	else
		result = new QGAMES::Tile (id, form, nf, __GAMETEST_BKTILE__, p); // A normal tile...

	return (result);
}

// ---
QGAMES::TileLayer* TestSDL::MapsBuilderAddsOn::createTileLayer (int id, const std::string& n, 
		const QGAMES::Tiles& t, QGAMES::Map* m, QGAMES::TileLayer::VisualOrientation o,
		const QGAMES::LayerProperties& p)
{
	QGAMES::TileLayer* result = NULL;
	if (n.find (std::string (__GAMETEST_LAYERPLATFORM__)) != std::string::npos)
			result = new TestSDL::PlatformLayer (id, n, t, m, o, p);
	else
		result = new QGAMES::TileLayer (id, n, t, m, o, p);

	return (result);
}

// ---
QGAMES::Map* TestSDL::MapsBuilderAddsOn::createMapObject (int id, const QGAMES::Layers& l, int w, int h, int d, 
		int tW, int tH, int tD, const QGAMES::MapProperties& p)
{
	QGAMES::Map* result = NULL;
	if (id == __GAMETEST_MAPID__)
		result = new TestSDL::Map (id, l, w, h, tW, tH, p);
	return (result);
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
void TestSDL::StatePresentation::onEnter ()
{
	game () -> sound (__GAMETEST_BKSOUND1__) -> play (__GAMETEST_CHANNELSOUND__);
	game () -> mainScreen () -> 
		setPosition (QGAMES::Position ((QGAMES::bdata) 0, (QGAMES::bdata) 0, (QGAMES::bdata) 0));
	game () -> mainScreen () -> setFix (NULL);
	game () -> removeScoreObjects ();

	_options = game () -> guiSystemBuilder () -> system (__GAMETEST_GUISYSTEM__);
	_options -> initialize (); 
	_options -> mainWidget () -> initialize ();
	_options -> mainWidget () -> setPosition (QGAMES::Position (__BD 100, __BD 50,__BD 0));
}

// ---
void TestSDL::StatePresentation::updatePositions ()
{
	if (((TestSDL::MainWidget*) (_options -> mainWidget ())) -> startSelected ())
		game () -> setState (__GAMETEST_STATENAMEPLAY__);
}

// ---
void TestSDL::StatePresentation::drawOn (QGAMES::Screen* s)
{
	game () -> form (__GAMETEST_BACKGROUNDFORM1__) -> drawOn (s, 0);

	// Lines
	// Vertical lines...
	for (int i = 0; i < game () -> mainScreen () -> visualWidth (); i += 20)
		for (int j = 0; j < game () -> mainScreen () -> visualHeight (); j += 10)
			s -> drawLine (QGAMES::Position (__BD i, __BD j, __BD 0), 
				QGAMES::Position (__BD i, __BD (j + 7), __BD 0), __GAMETEST_GRAYCOLOR__);
	// Horizontal lines...
	for (int i = 0; i < game () -> mainScreen () -> visualHeight (); i += 20)
		for (int j = 0; j < game () -> mainScreen () -> visualWidth (); j += 10)
			s -> drawLine (QGAMES::Position (__BD j, __BD i, __BD 0), 
				QGAMES::Position (__BD (j + 7), __BD i, __BD 0), __GAMETEST_GRAYCOLOR__);

	// Just a couple of drawings...
	// A circle
	s -> drawCircle (QGAMES::Position (__BD 50, __BD 200, __BD 0), QGAMES::Vector::_zNormal, 
		__BD 50, __BD 50, __GAMETEST_BLACKCOLOR__, true); 
	s -> drawCircle (QGAMES::Position (__BD 50, __BD 200, __BD 0), QGAMES::Vector::_zNormal, 
		__BD 45, __BD 45, __GAMETEST_BLUECOLOR__, true); 
	// A triangle
	s -> drawTriangle (QGAMES::Position (__BD 400, __BD 300, __BD 0), 
		QGAMES::Position (__BD 500, __BD 350, __BD 0), QGAMES::Position (__BD 380, __BD 380, __BD 0), __GAMETEST_BLACKCOLOR__, true);
	s -> drawTriangle (QGAMES::Position (__BD 402, __BD 305, __BD 0), 
		QGAMES::Position (__BD 495, __BD 350, __BD 0), QGAMES::Position (__BD 385, __BD 375, __BD 0), __GAMETEST_GREENCOLOR__, true);

	// Draws the commenets logo
	QGAMES::Form* lFrm = game () -> form (__GAMETEST_COMMNETSLOGO__);
	lFrm -> frame (0) -> drawOn (s, 
		QGAMES::Position (__BD (game () -> mainScreen () -> visualWidth () - lFrm -> width () - 10),
			__BD (game () -> mainScreen () -> visualHeight () - lFrm -> height () - 10), __BD 0));
}

// ---
void TestSDL::StatePresentation::onExit ()
{
	// Before goinf the conf is saved...
	TestSDL::Game::ConfData* cfg = (TestSDL::Game::ConfData*) game () -> configuration (); 
	cfg -> _inputDeviceType = (((TestSDL::MainWidget*) _options -> mainWidget ()) -> inputDeviceType () == 0) 
		? TestSDL::Game::ConfData::_KEYBOARD : TestSDL::Game::ConfData::_JOYSTICK;
	cfg -> _playerName = ((TestSDL::MainWidget*) _options -> mainWidget ()) -> playerName ();

	delete (_options);
	_options = NULL;
}

// ---
void TestSDL::StatePlay::onEnter ()
{
	TestSDL::Game::ConfData* cfg = (TestSDL::Game::ConfData*) game () -> configuration (); 
	((TestSDL::InputHandlerBehaviour*) game () -> inputHandler () -> behaviour ()) -> 
		activateJoystick (cfg -> _inputDeviceType == TestSDL::Game::ConfData::_JOYSTICK);

	game () -> sound (__GAMETEST_BKSOUND1__) -> play (__GAMETEST_CHANNELSOUND__);
	(__AGM game ()) -> setWorld (__GAMETEST_WORLDID__);
	(__AGM game ()) -> activeWorld () -> initialize ();
	game () -> observe ((__AGM game ()) -> activeWorld ());
	game () -> mainScreen () -> setPosition (QGAMES::Position::_cero);
	game () -> mainScreen () -> setFix (new QGAMES::ToFixScreenOnArtist (game () -> mainScreen (), 
		__AT (game () -> entity (__GAMETEST_MAINCHARACTERID__)), __BD 0.40, __BD 0.40));
	game () -> addScoreObjects ();
}

// ---
void TestSDL::StatePlay::drawOn (QGAMES::Screen* s)
{
	// Draws the commenets logo
	QGAMES::Form* lFrm = game () -> form (__GAMETEST_COMMNETSLOGO__);
	lFrm -> frame (0) -> drawOn (s, 
		QGAMES::Position (__BD (game () -> mainScreen () -> visualWidth () - lFrm -> width () - 10),
			__BD (game () -> mainScreen () -> visualHeight () - lFrm -> height () - 10), __BD 0) + 
		game () -> mainScreen () -> position ());
}

// ---
void TestSDL::StatePlay::onExit ()
{
	(__AGM game ()) -> setWorld (__MININT__);
}

// ---
void TestSDL::StateEnd::onEnter ()
{
	game () -> sound (__GAMETEST_BKSOUND1__) -> play (__GAMETEST_CHANNELSOUND__);
	game () -> mainScreen () -> setPosition (QGAMES::Position::_cero);
	game () -> mainScreen () -> setFix (NULL);
	game () -> removeScoreObjects ();
}

// ---
void TestSDL::StateEnd::updatePositions ()
{
	if (counter (TestSDL::StateEnd::_COUNTERCHANGEZOOM) -> isEnd ())
		onOffSwitch (TestSDL::StateEnd::_SWITCHZOOMOUT) -> change ();
}

// ---
void TestSDL::StateEnd::drawOn (QGAMES::Screen* s)
{
	QGAMES::ZoommerOnScreen* z = _game -> mainScreen () -> currentCamera () -> zoommer ();
	z -> setEffect (QGAMES::ZoommerOnScreen::Effect::_CENTER);
	z -> setValue (z -> value () + (__BD -0.005 * __BD (onOffSwitch (TestSDL::StateEnd::_SWITCHZOOMOUT) -> isOn () ? 1 : -1)));
	game () -> form (__GAMETEST_BACKGROUNDFORM2__) -> drawOn (s, 0, QGAMES::Position::_cero);

	// Draws the commenets logo
	QGAMES::Form* lFrm = game () -> form (__GAMETEST_COMMNETSLOGO__);
	lFrm -> frame (0) -> drawOn (s, 
		QGAMES::Position (__BD (game () -> mainScreen () -> visualWidth () - lFrm -> width () - 10),
			__BD (game () -> mainScreen () -> visualHeight () - lFrm -> height () - 10), __BD 0));
}

// ---
__IMPLEMENTCOUNTERS__ (TestSDL::StateEnd::Counters)
{
	addCounter (new QGAMES::Counter 
		(TestSDL::StateEnd::_COUNTERCHANGEZOOM, 100, 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (TestSDL::StateEnd::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (TestSDL::StateEnd::_SWITCHZOOMOUT, false));
}
// --------------------------------------------------------------------------------


// --------------------------------------------------------------------------------
// ---
int TestSDL::Game::ConfData::confToData (char*& str)
{
	std::ostringstream dt;
	dt << _inputDeviceType << std::endl;
	dt << _playerName << std::endl;
	dt << _playerScore << std::endl;

	int result = dt.str ().size ();
	str = new char [result];
	memcpy (str, dt.str ().c_str (), result);
	for (int i = 0; i < result; i++)
	{
		if (str [i] == '\n') str [i] = 0x00; // Removes the end of line or end of file character...
		str [i] += ' '; // Just to avoid the manipulation of the file...
	}

	return (result);
}

// ---
void TestSDL::Game::ConfData::dataToConf (char* str, int nB)
{
	char* istr = new char [nB + 1];
	memcpy (istr, str, nB);
	for (int i = 0; i < nB; i++)
	{
		istr [i] -= ' ';
		if (istr [i] == 0x00) istr [i] = '\n'; // Restablishes the end of line or end file character...
	}

	istr [nB] = 0;

	std::istringstream dt (istr);
	delete [] istr;
	int iD;
	dt >> iD; 
	_inputDeviceType = (iD == 0) ? TestSDL::Game::ConfData::_KEYBOARD : TestSDL::Game::ConfData::_JOYSTICK;
	dt >> _playerName;
	dt >> _playerScore;
}
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---
TestSDL::Game::Game ()
	: QGAMES::ArcadeGame (new SDLGame (true /** FMOD Just to test */), QGAMES::Worlds ())
{ 
#ifdef NDEBUG
	if (dynamic_cast <QGAMES::InZipResourceReader*> (QGAMES::ResourceReader::resourceReader ()) != NULL)
		((QGAMES::InZipResourceReader*) QGAMES::ResourceReader::resourceReader ()) -> 
			setTempLocation (parameter (std::string (__GAME_DATADIRPROPERTYNAME__)));
#endif
}

// ---
void TestSDL::Game::addScoreObjects ()
{
	// The score objects
	for (int i = 0; i < __GAMETEST_NUMBERSCOREOBJECTS__; i++)
	{
		QGAMES::ScoreObject* sO = 
			dynamic_cast <QGAMES::ScoreObject*> (entityBuilder () -> entity (__GAMETEST_SCOREOBJECTBASEID__ + i));
		sO -> setCurrentForm (__GAMETEST_SCOREOBJECTBASICFORM__ + i);
		sO -> setCurrentAspect (__GAMETEST_SCOREOBJECTASPECT__);
		sO -> setPosition
			(QGAMES::Position (__BD (10 + i * 70), __BD (mainScreen () -> visualHeight () - 80), __BD 0));
		sO -> setVisible (true);
		addScoreObject (sO);
	}
}

// ---
void TestSDL::Game::processEvent (const QGAMES::Event& e)
{
	QGAMES::ArcadeGame::processEvent (e);
	if (e.code () == __GAMETEST_GAMEFINISHED__ && 
		e.data () == entity (__GAMETEST_MAINCHARACTERID__))
		setState (__GAMETEST_STATENAMEEND__);
}

// ---
void TestSDL::Game::initialize ()
{
	if (isInitialized ())
		return; // Only once...

	// Normal initialization...
	QGAMES::ArcadeGame::initialize ();
	if (!isInitialized ())
		return;

	// Moves the main screen to the center..
	mainScreen () -> windowAtCenter ();
	mainScreen () -> setIcon (form (__GAMETEST_GAMEICON__));

	// Adds the states...
	addState (new TestSDL::StatePresentation (this), __GAMETEST_STATENAMEPRESENTATION__);
	addState (new TestSDL::StatePlay (this), __GAMETEST_STATENAMEPLAY__);
	addState (new TestSDL::StateEnd (this), __GAMETEST_STATENAMEEND__);

	// Adds the score objects...
	addScoreObjects ();

	// Adds the main artist...
	addArtist (__GAMETEST_MAINCHARACTERID__);

	// Adds the world used in the game...
	addWorld (__GAMETEST_WORLDID__);

	// The initial state...
	setState (__GAMETEST_STATENAMEPRESENTATION__);
}

// ---
void TestSDL::Game::finalize ()
{
	removeScoreObjects ();
	QGAMES::ArcadeGame::finalize ();
}
// --------------------------------------------------------------------------------
