#include <Dragonwindlike/dwbuilders.hpp>
#include <DragonwindLike/dwguisystem.hpp>
#include <DragonwindLike/dwgamestates.hpp>
#include <DragonwindLike/dwartists.hpp>
#include <DragonwindLike/dwworlds.hpp>
#include <DragonwindLike/dwscenes.hpp>
#include <DragonwindLike/dwmaps.hpp>
#include <DragonwindLike/dwlayers.hpp>
#include <DragonwindLike/dwsteps.hpp>
#include <DragonwindLike/dwactionblocks.hpp>
#include <DragonwindLike/dwmovements.hpp>

int DRAGONWIND::ToKnowTileFrameInformation::_defaultTileFramesRepresentingSomethingToEat [] = 
	{ 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };
int DRAGONWIND::ToKnowTileFrameInformation::_defaultTileFramesRepresentingSomethingToCatch [] = 
	{ 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
int DRAGONWIND::ToKnowTileFrameInformation::_defaultTileFramesRepresentingBadGuysPosition [] = 
	{  0,  1,  2,  3,  4,  5,  6,  7,  8,  9 };
int DRAGONWIND::ToKnowTileFrameInformation::_defaultTileFramesRepresentingDoorsPosition [] = 
	{ 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };
int DRAGONWIND::ToKnowTileFrameInformation::_defaultTileFramesRepresentingConnectionGoals [] =
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int DRAGONWIND::ToKnowTileFrameInformation::_defaultTileFramesRepresentingGoalsPosition [] = 
	{ 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
int DRAGONWIND::ToKnowTileFrameInformation::_defaultTileFramesRepresentingInitialPositions [] =
	{ 30, 31, 32, 33, 34, 35, 36, 37, 38, 39 };
int DRAGONWIND::ToKnowTileFrameInformation::_defaultTileFramesRepresentingActionBlocs [] =
	{ 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };

// ---
QGAMES::GUISystem* DRAGONWIND::GUISystemBuilder::createSystem 
	(const QGAMES::GUISystemBuilder::GUISystemDefinition& def, QGAMES::CompositeWidget* mW)
{
	if (def._id == __DRAGONWIND_OPTIONWHENPLAYINGGUISYSTEM__)
		return (new DRAGONWIND::GUISystem (mW));
	else
		return (QGAMES::StandardGUISystemBuilder::createSystem (def, mW));
}

// ---
QGAMES::Widget* DRAGONWIND::GUISystemBuilder::createWidget (const QGAMES::GUISystemBuilder::WidgetDefinition& def)
{
	QGAMES::Widget* result = NULL;

	if (def._id == __QGAMES_STDGUISYSTEMMAINWIDGET__ ||
		def._id == __QGAMES_STDGUISYSTEMMAINCONFWIDGET__ ||
		def._id == __DRAGONWIND_OPTIONSGUISYSTEMMAINWIDGET__) 
		result = new DRAGONWIND::ControlWidget (def._id);
	else
		result = QGAMES::StandardGUISystemBuilder::createWidget (def);

	return (result);
}

// ---
QGAMES::Entity* DRAGONWIND::EntityBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	QGAMES::Entity* result = NULL;

	QGAMES::Entity::Data dt;
	dt._capacities = def._capacities;
	dt._physics = def._physics;
	dt._parameters = def._parameters;

	// Creates the entities used in the presentation...
	// The ninja male...
	if (def._id == __DRAGONWIND_NINJAMALEPRESENTATIONID__)
		result = new DRAGONWIND::MenuNinjaMale (def._id);
	else 
	// ...and the female..
	if (def._id == __DRAGONWIND_NINJAFEMALEPRESENTATIONID__)
		result = new DRAGONWIND::MenuNinjaFemale (def._id);
	else
	// Creates the main artists...
	// Both are the same object (the aspect is manaje when drawing)
	if (def._id == __DRAGONWIND_MAINARTISTNINJAMALE__ ||
		def._id == __DRAGONWIND_MAINARTISTNINJAFEMALE__) // The main artists...
		result = new DRAGONWIND::Ninja (def._id, QGAMES::Forms (), dt);
	// Creates the shootings...
	else
	if (def._id >= __DRAGONWIND_SHOOTINGBASEID__ && 
		def._id < (__DRAGONWIND_SHOOTINGBASEID__ + __DRAGONWIND_MAXNUMBEROFSHOOTINGS__))
		result = new DRAGONWIND::Shooting (def._id, QGAMES::Forms (), dt);
	else
	// Creates the bees...
	if (def._id >= __DRAGONWIND_BEEBASEID__ &&
		def._id < (__DRAGONWIND_BEEBASEID__ + __DRAGONWIND_MAXNUMBEROFFLYMONSTERSFLOCK__))
		result = new DRAGONWIND::FlyingMonster (def._id);
	else
	// Crestaes the birds...
	if (def._id >= __DRAGONWIND_BIRDBASEID__ &&
		def._id < (__DRAGONWIND_BIRDBASEID__ + __DRAGONWIND_MAXNUMBEROFFLYMONSTERSFLOCK__))
		result = new DRAGONWIND::FlyingMonster (def._id);
	else
		result = QGAMES::PlatformEntityBuilder::createEntity (def);

	return (result);
}

// ---
QGAMES::CompositeEntity* DRAGONWIND::EntityBuilder::createCompositeEntity 
	(const QGAMES::EntityBuilder::EntityDefinition& def)
{
	QGAMES::CompositeEntity* result = NULL;

	QGAMES::Entities eties = entities (def._entities);
	QGAMES::Entities etiesClone;
	for (QGAMES::Entities::const_iterator i = eties.begin (); i != eties.end (); i++)
		etiesClone [(*i).first] = (*i).second -> clone ();

	QGAMES::Entity::Data dt;
	dt._capacities = def._capacities;
	dt._physics = def._physics;
	dt._parameters = def._parameters;

	// Creates the bees
	if (def._id >= __DRAGONWIND_FLOACKOFBEESBASE__ &&
		def._id < (__DRAGONWIND_FLOACKOFBEESBASE__ + __DRAGONWIND_MAXNUMBEROFFLOACKS__))
		result = new DRAGONWIND::FlockOfBees (def._id, etiesClone, dt);
	else
	// ...and the birds
	if (def._id >= __DRAGONWIND_FLOACKOFBIRDSBASE__ &&
		def._id < (__DRAGONWIND_FLOACKOFBIRDSBASE__ + __DRAGONWIND_MAXNUMBEROFFLOACKS__))
		result = new DRAGONWIND::FlockOfBirds (def._id, etiesClone, dt);
	else
		result = QGAMES::PlatformEntityBuilder::createCompositeEntity (def);

	return (result);
}

// ---
QGAMES::SomethingToEat* DRAGONWIND::EntityBuilder::EntityBuilder::createSomethingToEat 
	(const QGAMES::EntityBuilder::EntityDefinition& def)
{
	return (new DRAGONWIND::Food (def._id));
}

// ---
QGAMES::SomethingToCatch* DRAGONWIND::EntityBuilder::EntityBuilder::createSomethingToCatch 
	(const QGAMES::EntityBuilder::EntityDefinition& def)
{
	return (new DRAGONWIND::Thing (def._id));
}

// ---
QGAMES::BadGuy* DRAGONWIND::EntityBuilder::EntityBuilder::createBadGuy 
	(const QGAMES::EntityBuilder::EntityDefinition& def)
{
	return (new DRAGONWIND::BadGuy (def._id));
}

// ---
QGAMES::Movement* DRAGONWIND::MovementBuilder::createMovement (const QGAMES::MovementBuilder::MovementDefinition& def)
{
	QGAMES::Movement* result = NULL;

	// Creates the movements of the main artists...
	if (def._id == __DRAGONWIND_NINJAMALESTANDMOVID__ ||
		def._id == __DRAGONWIND_NINJAFEMALESTANDMOVID__)
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	if (def._id == __DRAGONWIND_NINJAMALERUNMOVID__ ||
		def._id == __DRAGONWIND_NINJAFEMALERUNMOVID__)
		result = new QGAMES::PhysicsIncremetalMovement (def._id, def._variables);
	else
	if (def._id == __DRAGONWIND_NINJAMALEJUMPMOVID__ ||
		def._id == __DRAGONWIND_NINJAFEMALEJUMPMOVID__)
		result = new QGAMES::PhysicsIncremetalMovement (def._id, def._variables);
	else
	if (def._id == __DRAGONWIND_NINJAMALESLIDEMOVID__ ||
		def._id == __DRAGONWIND_NINJAFEMALESLIDEMOVID__)
		result = new QGAMES::PhysicsIncremetalMovement (def._id, def._variables);
	else
	if (def._id == __DRAGONWIND_NINJAMALECLIMBMOVID__ ||
		def._id == __DRAGONWIND_NINJAFEMALECLIMBMOVID__)
		result = new QGAMES::SimpleLinearMovement (def._id, def._variables);
	else
	if (def._id == __DRAGONWIND_NINJAMALEATTACKMOVID__ ||
		def._id == __DRAGONWIND_NINJAFEMALEATTACKMOVID__)
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	if (def._id == __DRAGONWIND_NINJAMALEGLADMOVID__ ||
		def._id == __DRAGONWIND_NINJAFEMALEGLADMOVID__)
		result = new QGAMES::PhysicsIncremetalMovement (def._id, def._variables);
	else
	if (def._id == __DRAGONWIND_NINJAMALEFLOATMOVID__ ||
		def._id == __DRAGONWIND_NINJAFEMALEFLOATMOVID__)
		result = new DRAGONWIND::SwimingPhysicsIncrementalMovement (def._id, def._variables);
	else
	if (def._id == __DRAGONWIND_NINJAMALEHAPPYMOVID__ ||
		def._id == __DRAGONWIND_NINJAFEMALEHAPPYMOVID__)
		result = new QGAMES::PhysicsIncremetalMovement (def._id, def._variables);
	else
	// Creates the movements of the shootings...
	if (def._id >= __DRAGONWIND_SHOOTINGSTANDBASEMOVID__ &&
		def._id < (__DRAGONWIND_SHOOTINGSTANDBASEMOVID__ + __DRAGONWIND_MAXNUMBEROFSHOOTINGS__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	if (def._id >= __DRAGONWIND_SHOOTINGSPARABOLICBASEMOVID__ &&
		def._id < (__DRAGONWIND_SHOOTINGSPARABOLICBASEMOVID__ + __DRAGONWIND_MAXNUMBEROFSHOOTINGS__))
		result = new QGAMES::PhysicsIncremetalMovement (def._id, def._variables);
	else
	if (def._id >= __DRAGONWIND_SHOOTINGSBOUNCINGBASEMOVID__ &&
		def._id < (__DRAGONWIND_SHOOTINGSBOUNCINGBASEMOVID__ + __DRAGONWIND_MAXNUMBEROFSHOOTINGS__))
		result = new QGAMES::BouncePhysicsIncrementalMovement (def._id, def._variables);
	else
	if (def._id >= __DRAGONWIND_SHOOTINGSROLLINGBASEMOVID__ &&
		def._id < (__DRAGONWIND_SHOOTINGSROLLINGBASEMOVID__ + __DRAGONWIND_MAXNUMBEROFSHOOTINGS__))
		result = new QGAMES::PhysicsIncremetalMovement (def._id, def._variables);
	else
	if (def._id >= __DRAGONWIND_SHOOTINGSFALLINGBASEMOVID__ &&
		def._id < (__DRAGONWIND_SHOOTINGSFALLINGBASEMOVID__ + __DRAGONWIND_MAXNUMBEROFSHOOTINGS__))
		result = new QGAMES::PhysicsIncremetalMovement (def._id, def._variables);
	else
	// Creates the movements of the food
	if (def._id >= __DRAGONWIND_FOODSTANDMOVBASEID__ && 
		def._id < (__DRAGONWIND_FOODSTANDMOVBASEID__ + __QGAMES_PTNUMBERSOMETHINGTOEAT__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	if (def._id >= __DRAGONWIND_FOODFALLMOVBASEID__ && 
		def._id < (__DRAGONWIND_FOODFALLMOVBASEID__ + __QGAMES_PTNUMBERSOMETHINGTOEAT__))
		result = new QGAMES::PhysicsIncremetalMovement (def._id, def._variables);
	else
	if (def._id >= __DRAGONWIND_FOODINTHEAIRMOVBASEID__ && 
		def._id < (__DRAGONWIND_FOODINTHEAIRMOVBASEID__ + __QGAMES_PTNUMBERSOMETHINGTOEAT__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	// Creates the movements of the things
	if (def._id >= __DRAGONWIND_THINGSTANDMOVBASEID__ && 
		def._id < (__DRAGONWIND_THINGSTANDMOVBASEID__ + __QGAMES_PTNUMBERSOMETHINGTOCATCH__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	if (def._id >= __DRAGONWIND_THINGFALLMOVBASEID__ && 
		def._id < (__DRAGONWIND_THINGFALLMOVBASEID__ + __QGAMES_PTNUMBERSOMETHINGTOCATCH__))
		result = new QGAMES::PhysicsIncremetalMovement (def._id, def._variables);
	else
	// Creates the movements for the flying elements...
	if (def._id >= __DRAGONWIND_BEEFLYMOVEMENTBASEID__ && 
		def._id < (__DRAGONWIND_BEEFLYMOVEMENTBASEID__ + __DRAGONWIND_MAXNUMBEROFFLYMONSTERSFLOCK__))
		result = new QGAMES::IncrementalLinearMovement (def._id, def._variables);
	else
	if (def._id >= __DRAGONWIND_BIRDFLYMOVEMENTBASEID__ &&
		def._id < (__DRAGONWIND_BIRDFLYMOVEMENTBASEID__ + __DRAGONWIND_MAXNUMBEROFFLYMONSTERSFLOCK__))
		result = new QGAMES::IncrementalLinearMovement (def._id, def._variables);
	else
	if (def._id >= __DRAGONWIND_BEESTANDMOVEMENTBASEID__ && 
		def._id < (__DRAGONWIND_BEESTANDMOVEMENTBASEID__ + __DRAGONWIND_MAXNUMBEROFFLYMONSTERSFLOCK__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	if (def._id >= __DRAGONWIND_BIRDSTANDMOVEMENTBASEID__ &&
		def._id < (__DRAGONWIND_BIRDSTANDMOVEMENTBASEID__ + __DRAGONWIND_MAXNUMBEROFFLYMONSTERSFLOCK__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	if (def._id >= __DRAGONWIND_BADGUYSTANDMOVBASEID__ && 
		def._id < (__DRAGONWIND_BADGUYSTANDMOVBASEID__ + __QGAMES_PTNUMBERBADGUY__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	else
	if (def._id >= __DRAGONWIND_BADGUYRUNMOVBASEID__ && 
		def._id < (__DRAGONWIND_BADGUYRUNMOVBASEID__ + __QGAMES_PTNUMBERBADGUY__))
		result = new QGAMES::PhysicsIncremetalMovement (def._id, def._variables);
	else
	if (def._id >= __DRAGONWIND_BADGUYJUMPMOVBASEID__ && 
		def._id < (__DRAGONWIND_BADGUYJUMPMOVBASEID__ + __QGAMES_PTNUMBERBADGUY__))
		result = new QGAMES::PhysicsIncremetalMovement (def._id, def._variables);
	else
	if (def._id >= __DRAGONWIND_BADGUYSLIDEMOVBASEID__ && 
		def._id < (__DRAGONWIND_BADGUYSLIDEMOVBASEID__ + __QGAMES_PTNUMBERBADGUY__))
		result = new QGAMES::PhysicsIncremetalMovement (def._id, def._variables);
	else
		result = QGAMES::AdvancedMovementBuilder::createMovement (def);

	return (result);
}

// ---
QGAMES::World* DRAGONWIND::WorldBuilder::createWorldObject (int no, const QGAMES::Scenes& s, 
			const QGAMES::WorldProperties& p)
{ 
	// By default all instances created will be DRAGONWIND's 
	// So the parent class is not invoked ever!
	// The classes inheriting from this one could distinguish among subtypes of DRAGONWIND's worlds
	return (new DRAGONWIND::World (no, s, p)); 
}

// ---
QGAMES::Scene* DRAGONWIND::WorldBuilder::createSceneObject (int ns, const QGAMES::Maps& m, 
			const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
{ 
	QGAMES::Scene* result = NULL;
	if (ns == __DRAGONWIND_EXAMPLESCENEID__)
		result = new DRAGONWIND::SceneExample (ns, m, cn, p, ePL);
	else
	if (ns == (__DRAGONWIND_EXAMPLESCENEID__ + 1))
		result = new DRAGONWIND::SceneExampleNext (ns, m, cn, p, ePL);
	else
	// By default all instances created will be DRAGONWIND's 
	// So the parent class is not invoked ever!
	// The classes inheriting from this one could distinguish among subtypes of DRAGONWIND's scenes
	result = new DRAGONWIND::PlayingScene (ns, m, cn, p, ePL);

	return (result);
}

// ---
QGAMES::SceneActionBlock* DRAGONWIND::WorldBuilder::createSceneActionBlockObject (int nAB, 
			const QGAMES::SceneActionBlockProperties& prps)
{
	QGAMES::SceneActionBlock* result = NULL;

	if (nAB >= __DRAGONWIND_ACTIONBLOCKFLYMONSTERBASEID__ && 
		nAB < (__DRAGONWIND_ACTIONBLOCKFLYMONSTERBASEID__ + __DRAGONWIND_ACTIONBLOCKFLYMONSTERNUMBER__))
		result = new DRAGONWIND::FlyingMonstersActionBlock (nAB, 
				new DRAGONWIND::FlyingMonstersActionBlock::Properties (prps));
	else
	if (nAB >= __DRAGONWIND_ACTIONBLOCKMOVMONSTERBASEID__ &&
		nAB < (__DRAGONWIND_ACTIONBLOCKMOVMONSTERBASEID__ + __DRAGONWIND_ACTIONBLOCKMOVMONSTERNUMBER__))
		result = new DRAGONWIND::BadGuysActionBlock (nAB, new DRAGONWIND::BadGuysActionBlock::Properties (prps));
	else
	if (nAB >= __DRAGONWIND_ACTIONBLOCKENVCONDBASEID__ &&
		nAB < (__DRAGONWIND_ACTIONBLOCKENVCONDBASEID__ + __DRAGONWIND_ACTIONBLOCKENVCONDNUMBER__))
		result = new DRAGONWIND::EnvironmentalConditionActionBlock (nAB, 
			new DRAGONWIND::EnvironmentalConditionActionBlock::Properties (prps));
	else
	if (nAB >= __DRAGONWIND_ACTIONBLOCKACTLAYERSBASEID__ &&
		nAB < (__DRAGONWIND_ACTIONBLOCKACTLAYERSBASEID__ + __DRAGONWIND_ACTIONBLOCKACTLAYERNUMBER__))
		result = new DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock (nAB, 
			new DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock::Properties (prps));
	else
	if (nAB >= __DRAGONWIND_ACTIONBLOCKMOVELAYERSBASEID__ &&
		nAB < (__DRAGONWIND_ACTIONBLOCKMOVELAYERSBASEID__ + __DRAGONWIND_ACTIONBLOCKMOVLAYERSNUMBER__))
		result = new DRAGONWIND::MoveLinearASetOfLayersActionBlock (nAB, 
			new DRAGONWIND::MoveLinearASetOfLayersActionBlock::Properties (prps));
	else
		result = QGAMES::PlatformWorldBuilder::createSceneActionBlockObject (nAB, prps);

	return (result);
}

// ---
void DRAGONWIND::WorldBuilder::afterCreateElement (QGAMES::Element* elmnt)
{
	QGAMES::PlatformWorldBuilder::afterCreateElement (elmnt);

	if (dynamic_cast <DRAGONWIND::SceneActionBlock*> (elmnt))
		((DRAGONWIND::SceneActionBlock*) elmnt) -> setInitialActive 
			(((DRAGONWIND::SceneActionBlock*) elmnt) -> isActive ()); // To keep the initial state of active...
}

// ---
DRAGONWIND::ToKnowTileFrameInformation::ToKnowTileFrameInformation ()
	: QGAMES::ToKnowTileFrameInformation ()
	  // The very basic initialization for the lists are enough...
{
	_thingsToEatLocationTileFrames = 
		std::vector <int> (_defaultTileFramesRepresentingSomethingToEat, 
			_defaultTileFramesRepresentingSomethingToEat + (sizeof (_defaultTileFramesRepresentingSomethingToEat) / sizeof (int)));
	_thingsToCatchLocationTileFrames =
		std::vector <int> (_defaultTileFramesRepresentingSomethingToCatch, 
			_defaultTileFramesRepresentingSomethingToCatch + (sizeof (_defaultTileFramesRepresentingSomethingToCatch) / sizeof (int)));
	_badGuysLocationTileFrames = 
		std::vector <int> (_defaultTileFramesRepresentingBadGuysPosition,
			_defaultTileFramesRepresentingBadGuysPosition + (sizeof (_defaultTileFramesRepresentingBadGuysPosition) / sizeof (int)));
	_doorsNotificationBlockTileFrames = 
		std::vector <int> (_defaultTileFramesRepresentingDoorsPosition, 
			_defaultTileFramesRepresentingDoorsPosition + (sizeof (_defaultTileFramesRepresentingDoorsPosition) / sizeof (int)));
	_connectionGoalsNotificationTileFrames =
		std::vector <int> (_defaultTileFramesRepresentingConnectionGoals, 
			_defaultTileFramesRepresentingConnectionGoals + (sizeof (_defaultTileFramesRepresentingConnectionGoals) / sizeof (int)));
	_intermediateGoalsNotificationTileFrames =
		std::vector <int> (_defaultTileFramesRepresentingGoalsPosition, 
			_defaultTileFramesRepresentingGoalsPosition + (sizeof (_defaultTileFramesRepresentingGoalsPosition) / sizeof (int)));
	_initialPositionsTileFrames =
		std::vector <int> (_defaultTileFramesRepresentingInitialPositions, 
			_defaultTileFramesRepresentingInitialPositions + (sizeof (_defaultTileFramesRepresentingInitialPositions) / sizeof (int)));
	_actionBlocksTileFrames =
		std::vector <int> (_defaultTileFramesRepresentingActionBlocs, 
			_defaultTileFramesRepresentingActionBlocs + (sizeof (_defaultTileFramesRepresentingActionBlocs) / sizeof (int)));
}

// ---
QGAMES::TileLayer* DRAGONWIND::TMXMapBuilder::createTileLayer (int id, const std::string& n, 
	const QGAMES::Tiles& t, QGAMES::Map* m, QGAMES::TileLayer::VisualOrientation o, 
	const QGAMES::LayerProperties& p)
{
	// The liquid layer here is a little bit more special...
	return ((layerType (n) == __QGAMES_PTLIQUIDTILETYPE__)
		? new DRAGONWIND::LiquidTileLayer (id, n, t, m, o, p, false)
		: QGAMES::PlatformTMXMapBuilder::createTileLayer (id, n, t, m, o, p));
}

// ---
QGAMES::Map* DRAGONWIND::TMXMapBuilder::createMapObject (int id, const QGAMES::Layers& l, int w, int h, int d,
			int tW, int tH, int tD, const QGAMES::MapProperties& p)
{
	// By default all instances created will be DRAGONWIND's 
	// So the parent class is not invoked ever!
	// The classes inheriting from this one could distinguish among subtypes of DRAGONWIND's maps
	return (new DRAGONWIND::Map (id, l, w, h, tW, tH, p)); 
}

// ---
QGAMES::GameState* DRAGONWIND::ForInitializeDragonWindGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new DRAGONWIND::InitializeDragonWindGameState (def -> _id, 
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* DRAGONWIND::ForShowingPlayerNameGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new DRAGONWIND::ShowingCurrentPlayerNameGameState (
		def -> _id,
		def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRTEXT__) -> second,
		std::atoi (def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRFONT__) -> second.c_str ()),
		QGAMES::ShowingTextGameState::Properties (def -> _attributes), // Other attributes...
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* DRAGONWIND::ForShowingWorldNameGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new DRAGONWIND::ShowingCurrentWorldNameGameState (
		def -> _id,
		def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRTEXT__) -> second,
		std::atoi (def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRFONT__) -> second.c_str ()),
		QGAMES::ShowingTextGameState::Properties (def -> _attributes), // Other attributes...
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* DRAGONWIND::ForShowingSceneNameGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new DRAGONWIND::ShowingCurrentSceneNameGameState (
		def -> _id,
		def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRTEXT__) -> second,
		std::atoi (def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRFONT__) -> second.c_str ()),
		QGAMES::ShowingTextGameState::Properties (def -> _attributes), // Other attributes...
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* DRAGONWIND::ForCreditTitlesGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	int nT = __DRAGONWIND_GAMESTATECRTTITLEATTRNUMDEF__;
	if (def -> _attributes.find (__DRAGONWIND_GAMESTATECREDITTILEATTRNUM__) != def -> _attributes.end ())
		nT = std::atoi (def -> _attributes.find (__DRAGONWIND_GAMESTATECREDITTILEATTRNUM__) -> second.c_str ());
	std::string cT (__DRAGONWIND_GAMESTATECRTTITLEATTRCNTDEF__);
	if (def -> _attributes.find (__DRAGONWIND_GAMESTATECREDITTITLEATTRCNT__) != def -> _attributes.end ())
		cT = def -> _attributes.find (__DRAGONWIND_GAMESTATECREDITTITLEATTRCNT__) -> second;

	QGAMES::ForCreditTitlesGameStateAddOn::GameStateCreditTitlesDefinition* defC =
		(QGAMES::ForCreditTitlesGameStateAddOn::GameStateCreditTitlesDefinition*) def;
	return (new DRAGONWIND::CreditTitlesGameState (def -> _id, nT, cT,
		 QGAMES::CreditTitlesGameState::Properties (defC -> _titles, def -> _attributes),
		 (def->_nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* DRAGONWIND::ForPlayingControlGameStateAddOn::createControlGameState 
	(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
	 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl)
{
	return (new DRAGONWIND::PlayingControlGameState (def -> _id, sts, fl));
}

// ---
QGAMES::GameState* DRAGONWIND::ForPlayingGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new DRAGONWIND::PlayingGameState (def -> _id,
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* DRAGONWIND::ForOptionsWhenPlayingGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new DRAGONWIND::OptionsWhenPlayingGameState (def -> _id,
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* DRAGONWIND::ForMenuAddsOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new DRAGONWIND::SelectionGameState (def -> _id,
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* DRAGONWIND::ForShowingHallOfFameGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new DRAGONWIND::ShowingHallOfFameGameState (
		def -> _id, QGAMES::ShowingHallOfFameGameState::Properties (def -> _attributes),
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* DRAGONWIND::ForGameStateControlAddon::createControlGameState
			(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
			 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl)
{
	return ((def -> _type == __DRAGONWIND_GAMESTATECTRLTYPE__)
		? new DRAGONWIND::GameStateControl (def -> _id, 
			builder () -> gameState (std::atoi (def -> _attributes.find (__QGAMES_GAMESTATEADVANCEDCTRLPAUSE__) -> second.c_str ())), sts, fl)
		: QGAMES::ForAdvancedGameStateControlAddon::createControlGameState (def, sts, fl));
}

// ---
QGAMES::CharacterControlStep* DRAGONWIND::CharacterControlStepsMonitorBuilder::createStep 
	(const QGAMES::CharacterControlStepsMonitorBuilder::StepDefinition& def)
{
	QGAMES::CharacterControlStep* result = NULL;
	if (def._type == __DRAGONWIND_CHRSTEPACTION__)
		result = new DRAGONWIND::CharacterControlStepDragonAction (def._values);
	else
		result = QGAMES::CharacterControlStepsMonitorBuilder::createStep (def);

	return (result);
}
