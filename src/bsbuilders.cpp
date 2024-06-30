#include <BattleshipLike/bsbuilders.hpp>
#include <BattleshipLike/bsguisystem.hpp>
#include <BattleshipLike/bsgamestates.hpp>
#include <BattleshipLike/bsartists.hpp>
#include <BattleshipLike/bsworlds.hpp>
#include <BattleshipLike/bsscenes.hpp>

// ---
QGAMES::GUISystem* BATTLESHIP::GUISystemBuilder::createSystem 
	(const QGAMES::GUISystemBuilder::GUISystemDefinition& def, QGAMES::CompositeWidget* mW)
{
	if (def._id == __BATTLESHIP_OPTIONWHENPLAYINGGUISYSTEM__)
		return (new BATTLESHIP::GUISystem (mW));
	else
		return (QGAMES::StandardGUISystemBuilder::createSystem (def, mW));
}

// ---
QGAMES::Widget* BATTLESHIP::GUISystemBuilder::createWidget (const QGAMES::GUISystemBuilder::WidgetDefinition& def)
{
	QGAMES::Widget* result = NULL;

	if (def._id == __QGAMES_STDGUISYSTEMMAINWIDGET__ ||
		def._id == __QGAMES_STDGUISYSTEMMAINCONFWIDGET__ ||
		def._id == __BATTLESHIP_OPTIONSGUISYSTEMMAINWIDGET__) 
		result = new BATTLESHIP::ControlWidget (def._id);
	else
		result = QGAMES::StandardGUISystemBuilder::createWidget (def);

	return (result);
}

// ---
QGAMES::Entity* BATTLESHIP::EntityBuilder::createEntity (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	QGAMES::Entity* result = NULL;

	//The battleships...
	if (def._id == __BATTLESHIP_BATTLESHIPTYPE1ID__ ||
		def._id == __BATTLESHIP_BATTLESHIPTYPE2ID__)
		result = createBattleship (def);
	// The shootings...
	else 
	if (def._id >= __BATTLESHIP_SPACESHIPSHOOTINGBASEID__ && 
		def._id < (__BATTLESHIP_SPACESHIPSHOOTINGBASEID__ + __BATTLESHIP_SPACESHITPMAXSHOOTINGS__))
		result = createSpaceshipShooting (def);
	// The asteroids...
	else 
	if (def._id >= __BATTLESHIP_ASTEROIDBASEID__ && 
		def._id < (__BATTLESHIP_ASTEROIDBASEID__ + __BATTLESHIP_MAXASTEROIDS__))
		result = createAsteroid (def);
	// The things to catch...
	else
	if (def._id >= __BATTLESHIP_THINGTOCATCHBASEID__ &&
		def._id < (__BATTLESHIP_THINGTOCATCHBASEID__ + __BATTLESHIP_MAXTHINGSTOCATCH__))
		result = createThingToCatch (def);
	// The enemies...
	else
	if (def._id >= __BATTLESHIP_ENEMYBASEID__ &&
		def._id < (__BATTLESHIP_ENEMYBASEID__ + __BATTLESHIP_MAXNUMBERENEMIES__))
		result = createSpaceship (def);
	// If the object is not the requested one, then it is lokked in the parent class...
	else
		result = QGAMES::AdvancedEntityBuilder::createEntity (def);

	return (result);
}

// ---
BATTLESHIP::Spaceship* BATTLESHIP::EntityBuilder::createSpaceship 
	(const QGAMES::EntityBuilder::EntityDefinition& def)
{ 
	return (new BATTLESHIP::UFO (def._id));
}

// ---
BATTLESHIP::SpaceshipShooting* BATTLESHIP::EntityBuilder::createSpaceshipShooting 
	(const QGAMES::EntityBuilder::EntityDefinition& def)
{ 
	return (new BATTLESHIP::SpaceshipShooting (def._id)); 
}

// ---
BATTLESHIP::Asteroid* BATTLESHIP::EntityBuilder::createAsteroid 
	(const QGAMES::EntityBuilder::EntityDefinition& def)
{ 
	return (new BATTLESHIP::Asteroid (def._id));
}

// ---
BATTLESHIP::ThingToCatch* BATTLESHIP::EntityBuilder::createThingToCatch 
	(const QGAMES::EntityBuilder::EntityDefinition& def)
{ 
	BATTLESHIP::ThingToCatch* result = NULL;
	if (def._id >= __BATTLESHIP_THINGTOCATCHBASEID__ && 
		def._id < (__BATTLESHIP_THINGTOCATCHBASEID__ + __BATTLESHIP_MAXTHINGSTOCATCH__ - 3)) // 3 out of 5
		result = new BATTLESHIP::PillPowerToCatch (def._id);
	else
	if (def._id == (__BATTLESHIP_THINGTOCATCHBASEID__ + __BATTLESHIP_MAXTHINGSTOCATCH__ - 3)) // 1 out of 5
		result = new BATTLESHIP::EnergyElement (def._id);
	else
	if (def._id == (__BATTLESHIP_THINGTOCATCHBASEID__ + __BATTLESHIP_MAXTHINGSTOCATCH__ - 2)) // 1 out of 5
		result = new BATTLESHIP::LiveToCatch (def._id);
	else
	if (def._id == (__BATTLESHIP_THINGTOCATCHBASEID__ + __BATTLESHIP_MAXTHINGSTOCATCH__ - 1)) // 1 out of 5
		result = new BATTLESHIP::ShieldToCatch (def._id);

	return (result);
}

// ---
BATTLESHIP::Battleship* BATTLESHIP::EntityBuilder::createBattleship 
	(const QGAMES::EntityBuilder::EntityDefinition& def)
{ 
	return (new BATTLESHIP::Battleship (def._id)); 
}

// ---
QGAMES::World* BATTLESHIP::WorldBuilder::createWorldObject 
	(int no, const QGAMES::Scenes& s, const QGAMES::WorldProperties& p)
{
	return (new BATTLESHIP::World (no, s, p));
}

// ---
QGAMES::Scene* BATTLESHIP::WorldBuilder::createSceneObject (int ns, const QGAMES::Maps& m, 
	const QGAMES::Scene::Connections& cn, const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
{
	QGAMES::Scene* result = NULL;

	if (ns == __BATTLESHIP_TIMELIMITEXAMPLESCENEID__)
		result = new BATTLESHIP::TimeLimitSceneExample (ns, m, cn, p, ePL);
	else
	if (ns == __BATTLESHIP_SEQUENTIALACTIONBLOCKSEXAMPLESCENEID__)
		result = new BATTLESHIP::SequentialActionBlocksSceneExample (ns, m, cn, p, ePL);
	else
	if (ns >= __BATTLESHIP_SCENETIMELIMITBASEID__ && 
		ns < (__BATTLESHIP_SCENETIMELIMITBASEID__ + __BATTLESHIP_SCENETIMENUMBERSCENES__))
		result = new BATTLESHIP::TimeLimitScene (ns, m, cn, p, ePL);
	else
	if (ns >= __BATTLESHIP_SCENESEQACTIONBLOCKSBASEID__ &&
		ns < (__BATTLESHIP_SCENESEQACTIONBLOCKSBASEID__ + __BATTLESHIP_SCENESEQACTIONBLOCKSNUMBERSCENES__))
		result = new BATTLESHIP::SequentialActionBlocksScene (ns, m, cn, p, ePL);

	return (result);
}

// ---
QGAMES::SceneActionBlock* BATTLESHIP::WorldBuilder::createSceneActionBlockObject (int nAB, 
	const QGAMES::SceneActionBlockProperties& prps)
{
	QGAMES::SceneActionBlock* result = NULL;

	if (nAB >= __BATTLESHIP_ACTIONBLOCKASTEROIDBASEID__ && 
			nAB < (__BATTLESHIP_ACTIONBLOCKASTEROIDBASEID__ + __BATTLESHIP_ACTIONBLOCKASTEROIDNUMBER__))
		result = new BATTLESHIP::StdAsteroidSceneActionBlock 
			(nAB, new BATTLESHIP::StdAsteroidSceneActionBlock::Properties (prps) /** 1 by default. */);
	else
	if (nAB >= __BATTLESHIP_ACTIONBLOCKFUELPILLBASEID__ && 
			nAB < (__BATTLESHIP_ACTIONBLOCKFUELPILLBASEID__ + __BATTLESHIP_ACTIONBLOCKFUELPILLNUMBER__))
		result = new BATTLESHIP::StdEnergyElementSceneActionBlock 
			(nAB, new BATTLESHIP::StdEnergyElementSceneActionBlock::Properties (prps) /** 1 by default. */);
	else
	if (nAB >= __BATTLESHIP_ACTIONBLOCKTHINGTOCATCHBASEID__ && 
			nAB < (__BATTLESHIP_ACTIONBLOCKTHINGTOCATCHBASEID__ + __BATTLESHIP_ACTIONBLOCKTHINGTOCATCHNUMBER__))
		result = new BATTLESHIP::StdPillSceneActionBlock // All of them are created like pill action blocks by default!
			(nAB, new BATTLESHIP::StdPillSceneActionBlock::Properties (prps) /** 1 by default. */);
	else
	if (nAB >= __BATTLESHIP_ACTIONBLOCKUFOBASEID__ && 
			nAB < (__BATTLESHIP_ACTIONBLOCKUFOBASEID__ + __BATTLESHIP_ACTIONBLOCKUFONUMBER__))
		result = new BATTLESHIP::StdUFOSceneActionBlock 
			(nAB, new BATTLESHIP::StdUFOSceneActionBlock::Properties (prps) /** 1 and the default factory. */);
	else
	if (nAB >= __BATTLESHIP_ACTIONBLOCKSETUFOSBASEID__ && 
			nAB < (__BATTLESHIP_ACTIONBLOCKSETUFOSBASEID__ + __BATTLESHIP_ACTIONBLOCKSETUFOSNUMBER__))
		result = new BATTLESHIP::StdSetUFOsSceneActionBlock 
			(nAB, new BATTLESHIP::StdSetUFOsSceneActionBlock::Properties (prps) /** The default factory list. */);

	return (result);
}

// ---
QGAMES::Movement* BATTLESHIP::MovementBuilder::createMovement (const QGAMES::MovementBuilder::MovementDefinition& def)
{
	QGAMES::Movement* result = NULL;

	// The movements of all standard elements are either linear or nothing...

	// Battleship
	if (def._id == __BATTLESHIP_BATTLESHIP1RUNMOVEMENT__ ||
		def._id == __BATTLESHIP_BATTLESHIP2RUNMOVEMENT__) 
		result = new QGAMES::SimpleLinearMovement (def._id, def._variables);
	else 
	if (def._id == __BATTLESHIP_BATTLESHIP1STAYMOVEMENT__ ||
		def._id == __BATTLESHIP_BATTLESHIP2STAYMOVEMENT__)
		result = new QGAMES::NoMovement (def._id, def._variables);
	// Battleship shootings
	else
	if (def._id >= __BATTLESHIP_SPACESHIPSHOOTINGMOVEMENTBASEID__ && 
		def._id < (__BATTLESHIP_SPACESHIPSHOOTINGMOVEMENTBASEID__ + __BATTLESHIP_SPACESHITPMAXSHOOTINGS__))
		result = new QGAMES::SimpleLinearMovement (def._id, def._variables);
	else
	if (def._id >= __BATTLESHIP_SPACESHIPSHOOTINGNOMOVBASEID__ && 
		def._id < (__BATTLESHIP_SPACESHIPSHOOTINGNOMOVBASEID__ + __BATTLESHIP_SPACESHITPMAXSHOOTINGS__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	// Asteriods
	else
	if (def._id >= __BATTLESHIP_ASTEROIDMOVEMENTBASEID__ && 
		def._id < (__BATTLESHIP_ASTEROIDMOVEMENTBASEID__ + __BATTLESHIP_MAXASTEROIDS__))
		result = new QGAMES::SimpleLinearMovement (def._id, def._variables);
	else
	if (def._id >= __BATTLESHIP_ASTEROIDNOMOVEMENTBASEID__ && 
		def._id < (__BATTLESHIP_ASTEROIDNOMOVEMENTBASEID__ + __BATTLESHIP_MAXASTEROIDS__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	// Shooting to catch
	else
	if (def._id >= __BATTLESHIP_THINGTOCATCHMOVEMENTBASE__ &&
		def._id < (__BATTLESHIP_THINGTOCATCHMOVEMENTBASE__ + __BATTLESHIP_MAXTHINGSTOCATCH__))
		result = new QGAMES::SimpleLinearMovement (def._id, def._variables);
	else
	if (def._id >= __BATTLESHIP_THINGTOCATCHNOMOVEMENTBASE__ &&
		def._id < (__BATTLESHIP_THINGTOCATCHNOMOVEMENTBASE__ + __BATTLESHIP_MAXTHINGSTOCATCH__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	// Enemies to fight
	else
	if (def._id >= __BATTLESHIP_ENEMYMOVEMENTBASE__ &&
		def._id < (__BATTLESHIP_ENEMYMOVEMENTBASE__ + __BATTLESHIP_MAXNUMBERENEMIES__))
		result = new QGAMES::FollowingACurveMovement (def._id, def._variables); // With no curve by default...
	else
	if (def._id >= __BATTLESHIP_ENEMYNOMOVEMENTBASE__ &&
		def._id < (__BATTLESHIP_ENEMYNOMOVEMENTBASE__ + __BATTLESHIP_MAXNUMBERENEMIES__))
		result = new QGAMES::NoMovement (def._id, def._variables);
	// Just in case, the rest are looked in the parent class!
	else
		result = QGAMES::AdvancedMovementBuilder::createMovement (def);

	return (result);
}

// ---
QGAMES::GameState* BATTLESHIP::ForInitializeBattleshipGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new BATTLESHIP::InitializeBattleshipGameState (def -> _id, 
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* BATTLESHIP::ForShowingBattleshipMovingGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new BATTLESHIP::BattleshipMovingGameState (def -> _id, 
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* BATTLESHIP::ForShowingPlayerNameGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new BATTLESHIP::ShowingCurrentPlayerNameGameState (
		def -> _id,
		def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRTEXT__) -> second,
		std::atoi (def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRFONT__) -> second.c_str ()),
		QGAMES::ShowingTextGameState::Properties (def -> _attributes), // Other attributes...
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* BATTLESHIP::ForShowingWorldNameGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new BATTLESHIP::ShowingCurrentWorldNameGameState (
		def -> _id,
		def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRTEXT__) -> second,
		std::atoi (def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRFONT__) -> second.c_str ()),
		QGAMES::ShowingTextGameState::Properties (def -> _attributes), // Other attributes...
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* BATTLESHIP::ForShowingSceneNameGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new BATTLESHIP::ShowingCurrentSceneNameGameState (
		def -> _id,
		def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRTEXT__) -> second,
		std::atoi (def -> _attributes.find (__QGAMES_GAMESTATESHOWSTDTXTATTRFONT__) -> second.c_str ()),
		QGAMES::ShowingTextGameState::Properties (def -> _attributes), // Other attributes...
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* BATTLESHIP::ForCreditTitlesGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	int nT = __BATTLESHIP_GAMESTATECRTTITLEATTRNUMDEF__;
	if (def -> _attributes.find (__BATTLESHIP_GAMESTATECREDITTILEATTRNUM__) != def -> _attributes.end ())
		nT = std::atoi (def -> _attributes.find (__BATTLESHIP_GAMESTATECREDITTILEATTRNUM__) -> second.c_str ());
	std::string cT (__BATTLESHIP_GAMESTATECRTTITLEATTRCNTDEF__);
	if (def -> _attributes.find (__BATTLESHIP_GAMESTATECREDITTITLEATTRCNT__) != def -> _attributes.end ())
		cT = def -> _attributes.find (__BATTLESHIP_GAMESTATECREDITTITLEATTRCNT__) -> second;

	QGAMES::ForCreditTitlesGameStateAddOn::GameStateCreditTitlesDefinition* defC =
		(QGAMES::ForCreditTitlesGameStateAddOn::GameStateCreditTitlesDefinition*) def;
	return (new BATTLESHIP::CreditTitlesGameState (def -> _id, nT, cT,
		 QGAMES::CreditTitlesGameState::Properties (defC -> _titles, def -> _attributes),
		 (def->_nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* BATTLESHIP::ForPlayingControlGameStateAddOn::createControlGameState 
	(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
	 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl)
{
	return (new BATTLESHIP::PlayingControlGameState (def -> _id, sts, fl));
}

// ---
QGAMES::GameState* BATTLESHIP::ForPlayingGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new BATTLESHIP::PlayingGameState (def -> _id,
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* BATTLESHIP::ForOptionsWhenPlayingGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new BATTLESHIP::OptionsWhenPlayingGameState (def -> _id,
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* BATTLESHIP::ForMenuAddsOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new BATTLESHIP::SelectionGameState (def -> _id,
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* BATTLESHIP::ForShowingHallOfFameGameStateAddOn::createGameState 
	(const QGAMES::GameStateBuilder::GameStateDefinition* def)
{
	return (new BATTLESHIP::ShowingHallOfFameGameState (
		def -> _id, QGAMES::ShowingHallOfFameGameState::Properties (def -> _attributes),
		(def -> _nested != NULL) ? builder () -> gameState (def -> _nested -> _id) : NULL));
}

// ---
QGAMES::GameState* BATTLESHIP::ForGameStateControlAddon::createControlGameState
			(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
			 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl)
{
	return ((def -> _type == __BATTLESHIP_GAMESTATECTRLTYPE__)
		? new BATTLESHIP::GameStateControl (def -> _id, 
			builder () -> gameState (std::atoi (def -> _attributes.find (__QGAMES_GAMESTATEADVANCEDCTRLPAUSE__) -> second.c_str ())), sts, fl)
		: QGAMES::ForAdvancedGameStateControlAddon::createControlGameState (def, sts, fl));
}
