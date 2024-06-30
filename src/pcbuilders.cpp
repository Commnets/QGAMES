#include <PacManLike/pcbuilders.hpp>
#include <PacManLike/pcletters.hpp>
#include <PacManLike/pcguisystem.hpp>
#include <PacManLike/pcartists.hpp>
#include <PacManLike/pcmovements.hpp>
#include <PacManLike/pcgamestates.hpp>
#include <PacManLike/pcworlds.hpp>
#include <PacManLike/pcscenes.hpp>
#include <PacManLike/pcmaps.hpp>
#include <PacManLike/pcsceneactionblocks.hpp>

// ---
PACMAN::TextBuilder::TextBuilder ()
	: QGAMES::AdvancedTextBuilder ()
{
	QGAMES::AdvancedTextBuilder::FontDataPerSize fNT;

	// NewTour 24...
	QGAMES::AdvancedTextBuilder::FontDataList fNT24;
	fNT24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMAN_NEWTOUR24BLUELETTERS__, __PACMAN_NEWTOURLETTERS__, 24, __QGAMES_BLUECOLOR__));
	fNT24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMAN_NEWTOUR24GREENLETTERS__, __PACMAN_NEWTOURLETTERS__, 24, __QGAMES_GREENCOLOR__));
	fNT24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMAN_NEWTOUR24REDLETTERS__, __PACMAN_NEWTOURLETTERS__, 24, __QGAMES_REDCOLOR__));
	fNT24.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMAN_NEWTOUR24YELLOWLETTERS__, __PACMAN_NEWTOURLETTERS__, 24, __QGAMES_YELLOWCOLOR__));
	fNT [24] = fNT24;

	// NewTour 36...
	QGAMES::AdvancedTextBuilder::FontDataList fNT36;
	fNT36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMAN_NEWTOUR36BLUELETTERS__, __PACMAN_NEWTOURLETTERS__, 36, __QGAMES_BLUECOLOR__));
	fNT36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMAN_NEWTOUR36GREENLETTERS__, __PACMAN_NEWTOURLETTERS__, 36, __QGAMES_GREENCOLOR__));
	fNT36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMAN_NEWTOUR36REDLETTERS__, __PACMAN_NEWTOURLETTERS__, 36, __QGAMES_REDCOLOR__));
	fNT36.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMAN_NEWTOUR36YELLOWLETTERS__, __PACMAN_NEWTOURLETTERS__, 36, __QGAMES_YELLOWCOLOR__));
	fNT [36] = fNT36;

	// NewTour 48...
	QGAMES::AdvancedTextBuilder::FontDataList fNT48;
	fNT48.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMAN_NEWTOUR48BLUELETTERS__, __PACMAN_NEWTOURLETTERS__, 48, __QGAMES_BLUECOLOR__));
	fNT48.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMAN_NEWTOUR48GREENLETTERS__, __PACMAN_NEWTOURLETTERS__, 48, __QGAMES_GREENCOLOR__));
	fNT48.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMAN_NEWTOUR48REDLETTERS__, __PACMAN_NEWTOURLETTERS__, 48, __QGAMES_REDCOLOR__));
	fNT48.push_back ( 
		QGAMES::AdvancedTextBuilder::FontData 
			(__PACMAN_NEWTOUR48YELLOWLETTERS__, __PACMAN_NEWTOURLETTERS__, 48, __QGAMES_YELLOWCOLOR__));
	fNT [48] = fNT48;

	_fontData [__PACMAN_NEWTOURLETTERS__] = fNT;
}

// ---
QGAMES::ScoreObjectText* PACMAN::TextBuilder::createText (int fnt, const std::string& txt)
{
	QGAMES::ScoreObjectText* result = nullptr;
	
	switch (fnt)
	{
		case __PACMAN_NEWTOUR24BLUELETTERS__:
			result = new PACMAN::NewTour24BlueFont (txt);
			break;

		case __PACMAN_NEWTOUR24GREENLETTERS__:
			result = new PACMAN::NewTour24GreenFont (txt);
			break;

		case __PACMAN_NEWTOUR24REDLETTERS__:
			result = new PACMAN::NewTour24RedFont (txt);
			break;

		case __PACMAN_NEWTOUR24YELLOWLETTERS__:
			result = new PACMAN::NewTour24YellowFont (txt);
			break;

		case __PACMAN_NEWTOUR36BLUELETTERS__:
			result = new PACMAN::NewTour36BlueFont (txt);
			break;

		case __PACMAN_NEWTOUR36GREENLETTERS__:
			result = new PACMAN::NewTour36GreenFont (txt);
			break;

		case __PACMAN_NEWTOUR36REDLETTERS__:
			result = new PACMAN::NewTour36RedFont (txt);
			break;

		case __PACMAN_NEWTOUR36YELLOWLETTERS__:
			result = new PACMAN::NewTour36YellowFont (txt);
			break;

		case __PACMAN_NEWTOUR48BLUELETTERS__:
			result = new PACMAN::NewTour48BlueFont (txt);
			break;

		case __PACMAN_NEWTOUR48GREENLETTERS__:
			result = new PACMAN::NewTour48GreenFont (txt);
			break;

		case __PACMAN_NEWTOUR48REDLETTERS__:
			result = new PACMAN::NewTour48RedFont (txt);
			break;

		case __PACMAN_NEWTOUR48YELLOWLETTERS__:
			result = new PACMAN::NewTour48YellowFont (txt);
			break;

		default:
			result = QGAMES::AdvancedTextBuilder::createText (fnt, txt);
	}

	return (result);
}

// ---
QGAMES::GUISystem* PACMAN::GUISystemBuilder::createSystem 
	(const QGAMES::GUISystemBuilder::GUISystemDefinition& def, QGAMES::CompositeWidget* mW)
{
	if (def._id == __PACMAN_OPTIONWHENPLAYINGGUISYSTEM__)
		return (new PACMAN::GUISystem (mW));
	else
		return (QGAMES::StandardGUISystemBuilder::createSystem (def, mW));
}

// ---
QGAMES::Widget* PACMAN::GUISystemBuilder::createWidget (const QGAMES::GUISystemBuilder::WidgetDefinition& def)
{
	QGAMES::Widget* result = nullptr;

	if (def._id == __QGAMES_STDGUISYSTEMMAINWIDGET__ ||
		def._id == __QGAMES_STDGUISYSTEMMAINCONFWIDGET__ ||
		def._id == __PACMAN_OPTIONSGUISYSTEMMAINWIDGET__) 
		result = new PACMAN::ControlWidget (def._id);
	else
		result = QGAMES::StandardGUISystemBuilder::createWidget (def);

	return (result);
}

// ---
QGAMES::GameState* PACMAN::ForInitializePACMANGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PACMAN::InitializePACMANGameState (def -> _id, 
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PACMAN::ForShowingMonsterMovingGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PACMAN::MonsterMovingGameState (def -> _id, 
		PACMAN::MonsterMovingGameState::Properties (def -> _attributes),
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PACMAN::ForShowingPlayerNameGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PACMAN::ShowingCurrentPlayerNameGameState (
		def -> _id,
		def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRTEXT__) -> second,
		std::atoi (def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRFONT__) -> second.c_str ()),
		QGAMES::ShowingTextGameState::Properties (def -> _attributes), // Other attributes...
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PACMAN::ForShowingLevelGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PACMAN::ShowingCurrentLevelGameState (
		def -> _id,
		def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRTEXT__) -> second,
		std::atoi (def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRFONT__) -> second.c_str ()),
		QGAMES::ShowingTextGameState::Properties (def -> _attributes), // Other attributes...
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PACMAN::ForPlayingControlGameStateAddOn::createControlGameState 
	(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
	 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl)
{
	return (new PACMAN::PlayingControlGameState (def -> _id, sts, fl));
}

// ---
QGAMES::GameState* PACMAN::ForPlayingGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PACMAN::PlayingGameState (def -> _id,
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PACMAN::ForStopPacmanElemntsMovingGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PACMAN::StopPacmanElemntsMovingGameState (def -> _id, 
		PACMAN::StopPacmanElemntsMovingGameState::Properties (def -> _attributes),
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PACMAN::ForBlinkMazePlayingGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PACMAN::BlinkMazePlayingGameState (
		def -> _id, PACMAN::BlinkMazePlayingGameState::Properties (def -> _attributes),
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PACMAN::ForOptionsWhenPlayingGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PACMAN::OptionsWhenPlayingGameState (def -> _id,
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PACMAN::ForShowingHallOfFameGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new PACMAN::ShowingHallOfFameGameState (
		def -> _id, QGAMES::ShowingHallOfFameGameState::Properties (def -> _attributes),
		(def -> _nested != nullptr) ? builder () -> gameState (def -> _nested -> _id) : nullptr));
}

// ---
QGAMES::GameState* PACMAN::ForGameStateControlAddon::createControlGameState
			(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
			 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl)
{
	return ((def -> _type == __PACMAN_GAMESTATECTRLTYPE__)
		? new PACMAN::GameStateControl (def -> _id, 
			builder () -> gameState (std::atoi (def -> _attributes.find (__QGAMES_GAMESTATEADVANCEDCTRLPAUSE__) -> second.c_str ())), sts, fl)
		: QGAMES::ForAdvancedGameStateControlAddon::createControlGameState (def, sts, fl));
}

// ---
QGAMES::Movement* PACMAN::MovementBuilder::createMovement 
	(const QGAMES::MovementBuilder::MovementDefinition& def)
{
	QGAMES::Movement* result = nullptr;

	// Pacman Movement's
	if (def._id >= __PACMAN_PACMANSTAYMOVEMENTBASEID__ &&
		def._id < (__PACMAN_PACMANSTAYMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	else 
	if (def._id >= __PACMAN_PACMANEATINGMOVEMENTBASEID__ &&
		def._id < (__PACMAN_PACMANEATINGMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__))
		result = new PACMAN::MazeMovement (def._id, def._variables);
	else
	// Monsters' chasing movements
	if ((def._id >= __PACMAN_INKYMAZEMOVEMENTBASEID__ &&
		 def._id < (__PACMAN_INKYMAZEMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__)) ||
		(def._id >= __PACMAN_BLINKYMAZEMOVEMENTBASEID__ &&
		 def._id < (__PACMAN_BLINKYMAZEMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__)) ||		
		(def._id >= __PACMAN_PINKYMAZEMOVEMENTBASEID__ &&
		 def._id < (__PACMAN_PINKYMAZEMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__)) ||		
		(def._id >= __PACMAN_CLYDEMAZEMOVEMENTBASEID__ &&
		 def._id < (__PACMAN_CLYDEMAZEMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__)))		
		result = new PACMAN::MazeMovement (def._id, def._variables);
	else
	// Monsters' at-home movements
	if ((def._id >= __PACMAN_INKYATHOMEMOVEMENTBASEID__ &&
		 def._id < (__PACMAN_INKYATHOMEMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__)) ||
		(def._id >= __PACMAN_BLINKYATHOMEMOVEMENTBASEID__ &&
		 def._id < (__PACMAN_BLINKYATHOMEMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__)) ||		
		(def._id >= __PACMAN_PINKYATHOMEMOVEMENTBASEID__ &&
		 def._id < (__PACMAN_PINKYATHOMEMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__)) ||		
		(def._id >= __PACMAN_CLYDEATHOMEMOVEMENTBASEID__ &&
		 def._id < (__PACMAN_CLYDEATHOMEMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__)))		
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	// Fruit's staying movement
	if (def._id >= __PACMAN_FRUITSTAYINGMOVEMENTBASEID__ &&
		 def._id < (__PACMAN_FRUITSTAYINGMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	// Fruit's maze movement
	if (def._id >= __PACMAN_FRUITMAZEMOVEMENTBASEID__ &&
		 def._id < (__PACMAN_FRUITMAZEMOVEMENTBASEID__ + __PACMAN_NUMBERENTITIES__))
		result = new PACMAN::MazeMovement (def._id, def._variables);
	else
		result = QGAMES::AdvancedMovementBuilder::createMovement (def);

	return (result);
}

// ---
QGAMES::Entity* PACMAN::EntityBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	QGAMES::Entity* result = nullptr;

	// There can be a couple of similar running entities...
	if (def._id >= __PACMAN_PACMANBASEENTITYID__ && 
		def._id < (__PACMAN_PACMANBASEENTITYID__ + __PACMAN_NUMBERENTITIES__)) 
			result = new PACMAN::PacMan (def._id);
	else 
	if (def._id >= __PACMAN_INKYBASEENTITYID__ && 
		def._id < (__PACMAN_INKYBASEENTITYID__ + __PACMAN_NUMBERENTITIES__))
			result = new PACMAN::Inky (def._id);
	else 
	if (def._id >= __PACMAN_BLINKYBASEENTITYID__ && 
		def._id < (__PACMAN_BLINKYBASEENTITYID__ + __PACMAN_NUMBERENTITIES__))
			result = new PACMAN::Blinky (def._id);
	else 
	if (def._id >= __PACMAN_PINKYBASEENTITYID__ && 
		def._id < (__PACMAN_PINKYBASEENTITYID__ + __PACMAN_NUMBERENTITIES__))
			result = new PACMAN::Pinky (def._id);
	else
	if (def._id >= __PACMAN_CLYDEBASEENTITYID__ && 
		def._id < (__PACMAN_CLYDEBASEENTITYID__ + __PACMAN_NUMBERENTITIES__)) 
			result = new PACMAN::Clyde (def._id);
	else 
	if (def._id >= __PACMAN_FRUITBASEENTITYID__ && 
		def._id < (__PACMAN_FRUITBASEENTITYID__ + __PACMAN_NUMBERENTITIES__)) 
			result = new PACMAN::Fruit (def._id);
	else
		result = QGAMES::AdvancedEntityBuilder::createEntity (def);

	return (result);
}

// ---
PACMAN::TMXMapBuilder::TMXMapBuilder (QGAMES::Sprite2DBuilder* sB)
	: QGAMES::TMXMapBuilderAddsOn (sB),
	  _TILESLIMITDARK				({ 0,1,2,3,4,5,6,7,8,9,10,12,13,14,15,16,17,20,21,22,23,24,25,26 }),
	  _TILESLIMITBRIGHT				({ 50,51,52,53,54,55,56,57,58,59,60,62,63,64,65,66,67,70,71,72,73,74,75,76 }),
	  _TILESPOWERBALL				({ 27,28,57,58 }),
	  _TILESNORMALBALL				({ 29,59 }),
	  _TILESPATH					({ 11,61,18,19,68,69 }),
	  _TILESPATHLIMITRIGHT			({ 30,31,32,33,34,35,36,37,38,39 }),
	  _TILESPATHLIMITLEFT			({ 40,41,42,43,44,45,46,47,48,49}),
	  _TILESHOMEPACMAN				({ 20,21,22,23,24,25,26,27,28,29 }),
	  _TILESHOMEMONSTERS			({ 10,11,12,13,14,15,16,17,18,19 }),
	  _TILESRUNAWAYMONSTERS			({ 0,1,2,3,4,5,6,7,8,9 }),
	  _TILETHINGS					({ 30, 31, 32, 33, 34, 35, 36, 37, 38, 39 }),
	  _TILEEXITMONSTERSHOME			(50),
	  _TILERETURNMONSTERAFTERDIE	(52),
	  _TILESCONNECTION				({ 51,53,54,55,56,57,58,59 }),
	  _TILESZONES					({ 0, 1, 2, 3, 4, 5, 6, 7 }),
	  _TILESDIRECTIONSMAZE			({	{  0, { false,	true,	false,	true	} }, // LEFT, RIGHT, UP, DOWN
										{  1, { true,	true,	false,	true	} },
										{  2, { true,	false,	false,	true	} },
										{  3, { false,	false,	true,	false	} },
										{  4, { false,	true,	false,	false	} },
										{  5, { true,	true,	false,	false	} },
										{  6, { true,	false,	false,	false	} },
										{  7, { false,	true,	true,	true	} },
										{  8, { true,	true,	true,	true	} },
										{  9, { true,	false,	true,	true	} },
										{ 10, { false,	false,	true,	true	} },
										{ 11, { false,	true,	true,	false	} },
										{ 12, { true,	true,	true,	false	} },
										{ 13, { true,	false,	true,	false	} },
										{ 14, { false,	false,	true,	false	} },
										{ 15, { false,	false,	false,	false	} }})
{
	assert (_TILESLIMITBRIGHT.size () == _TILESLIMITDARK.size ());
	// The have to have the same size to change between them...
}

// ---
int PACMAN::TMXMapBuilder::brightFrameFor (int nF) const
{
	int result = nF;

	if (isInType (nF, _TILESLIMITDARK)) // If not, it can't change...
		result = _TILESLIMITBRIGHT [positionInType (nF, _TILESLIMITDARK)];

	return (result);
}

// ---
int PACMAN::TMXMapBuilder::darkFrameFor (int nF) const
{
	int result = nF;

	if (isInType (nF, _TILESLIMITBRIGHT)) // If not, it can't chang...
		result = _TILESLIMITDARK [positionInType (nF, _TILESLIMITBRIGHT)];

	return (result);
}

// ---
QGAMES::Tile* PACMAN::TMXMapBuilder::createTile (int id, QGAMES::Form* form, int nf, const QGAMES::TileProperties& p)
{
	QGAMES::Tile* result = nullptr;

	if (form -> name () == std::string (__PACMAN_FORMTILESMAZENAME__))
	{
		if (isInType (nf, _TILESLIMITDARK) || isInType (nf, _TILESLIMITBRIGHT)) 
			result = new PACMAN::TileLimit (id, form, nf, p);
		else if (isInType (nf, _TILESPOWERBALL)) 
			result = new PACMAN::TilePath (id, form, nf, PACMAN::TilePath::Type::_POWERBALL,	p);
		else if (isInType (nf, _TILESNORMALBALL)) 
			result = new PACMAN::TilePath (id, form, nf,  PACMAN::TilePath::Type::_BALL, p);
		else if (isInType (nf, _TILESPATH)) 
			result = new PACMAN::TilePath (id, form, nf,  PACMAN::TilePath::Type::_EMPTY, p);
		else if (isInType (nf, _TILESPATHLIMITRIGHT)) 
			result = new PACMAN::TilePath (id, form, nf,  PACMAN::TilePath::Type::_RIGHTLIMIT, p);
		else if (isInType (nf, _TILESPATHLIMITLEFT)) 
			result = new PACMAN::TilePath (id, form, nf,  PACMAN::TilePath::Type::_LEFTLIMIT, p);
		else 
			result = new QGAMES::NullTile (id); /// Just in case, but a well defined maze whould reach this point ever...
	}
	else
		result = new QGAMES::Tile (id, form, nf, 0, p); // When it is not a corner, all of them will be equal...

	return (result);
}

// ---
QGAMES::TileLayer* PACMAN::TMXMapBuilder::createTileLayer (int id, const std::string& n, 
	const QGAMES::Tiles& t, QGAMES::Map* m, QGAMES::TileLayer::VisualOrientation o, const QGAMES::LayerProperties& p)
{
	QGAMES::TileLayer* result = nullptr;

	if (n == std::string (__PACMAN_ARTISTSLOCATIONSLAYERNAME__))
		result = new PACMAN::ArtistsLocationsLayer (id, n, t, m, p);
	else if (n == std::string (__PACMAN_MAZELOCATIONSLAYERNAME__))
		result = new PACMAN::MazeLocationsLayer (id, n, t, m, p);
	else if (n == std::string (__PACMAN_MAZEZONESLAYERNAME__))
		result = new PACMAN::MazeZonesLayer (id, n, t, m, p);
	else if (n == std::string (__PACMAN_DIRECTIONSLAYERNAME__))
		result = new PACMAN::DirectionsLayer (id, n, t, m, p);
	else if (n == std::string (__PACMAN_MAZELAYERNAME__))
		result = new PACMAN::MazeLayer (id, n, t, m, p);
	else if (n == std::string (__PACMAN_BACKGROUNDLAYERNAME__))
		result = new PACMAN::BackgroundLayer (id, n, t, m, p);

	// A layer not well defined becomes null...

	return (result);
}

// ---
QGAMES::ObjectLayer* PACMAN::TMXMapBuilder::createObjectLayer (int id, const std::string& n, const QGAMES::Objects&, 
	QGAMES::Map* m, const QGAMES::LayerProperties& oP)
{
	return (nullptr); // Not used in Pacman II
}

// ---
QGAMES::ImageLayer* PACMAN::TMXMapBuilder::createImageLayer (int id, const std::string& n, QGAMES::Image* i, 
	QGAMES::Map* m, const QGAMES::LayerProperties& oP)
{
	return (nullptr); // Not used in Pacman II
}

// ---
QGAMES::Map* PACMAN::TMXMapBuilder::createMapObject (int id, const QGAMES::Layers& l, 
	int w, int h, int d, int tW, int tH, int tD, const QGAMES::MapProperties& p)
{
	// By default all are the same...
	return (new PACMAN::Map (id, l, w, h, d, tW, tH, tD, p));
}

// ---
int PACMAN::TMXMapBuilder::positionInType (int nF, const std::vector <int>& t) const
{
	assert (isInType (nF, t));

	bool found = false;
	int result = 0;
	for (std::vector <int>::const_iterator i = t.begin (); i != t.end () && !found; i++)
		if ((*i) != nF) result++; 
		else found = true;

	assert (found); // Not possible other result...

	return (result);
}

// ---
QGAMES::World* PACMAN::WorldBuilder::createWorldObject (int no, const QGAMES::Scenes& s, const QGAMES::WorldProperties& p)
{
	// By deefault, all are the same...
	return (new PACMAN::World (no, s, p));
}

// ---
QGAMES::Scene* PACMAN::WorldBuilder::createSceneObject (int ns, const QGAMES::Maps& m, 
	const QGAMES::Scene::Connections& cn, const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
{
	QGAMES::Scene* result = nullptr;

	if (ns == __PACMAN_BASICBLOCKSCENES__) 
		// The monsters and the fruit are defined in the constructor...
		result = new PACMAN::BasicScene (ns, m, cn, p, ePL);
	else
	if (ns == __PACMAN_TESTINGBLOCKSCENES__)
		// The monsters and the fruit are defined in the constructor...
		result = new PACMAN::BasicScene (ns, m, cn, p, ePL);
	else
		// The elments to play with have to b defined in the XML file as action blocks...
		result = new PACMAN::StandardScene (ns, m, cn, p, ePL);

	return (result);
}

// ---
QGAMES::SceneActionBlock* PACMAN::WorldBuilder::createSceneActionBlockObject (int nAB, 
	const QGAMES::SceneActionBlockProperties& prps)
{
	QGAMES::SceneActionBlock* result = nullptr;

	if (nAB >= __PACMAN_MONSTERSCENEBLOCKBASEID__ && 
		nAB < (__PACMAN_MONSTERSCENEBLOCKBASEID__ + __PACMAN_MONSTERSCENEBLOCKNUMBER__))
		result = new PACMAN::MonsterSceneActionBlock (nAB, PACMAN::MonsterSceneActionBlock::Properties (prps));
	else
	if (nAB >= __PACMAN_FRUITSCENEBLOCKBASEID__ && 
		nAB < (__PACMAN_FRUITSCENEBLOCKBASEID__ + __PACMAN_FRUITSCENEBLOCKNUMBER__))
		result = new PACMAN::ThingSceneActionBlock (nAB, PACMAN::ThingSceneActionBlock::Properties (prps));

	return (result);
}
