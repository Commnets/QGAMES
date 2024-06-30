#include "Builders.hpp"
#include "Worlds.hpp"
#include "Artists.hpp"
#include "Scenes.hpp"
#include "SceneActionBlocks.hpp"

// ---
QGAMES::FollowingACurveMovement::CurveTemplate* BattleshipII::CurveBuilder::createCurve 
			(int t, const std::map <int, double>& prms)
{
	QGAMES::FollowingACurveMovement::CurveTemplate* result = NULL;

	if (t == __BATTLESHIPII_SINUSOIDEGOANDBACK__)
	{
		assert (prms.find (0) != prms.end () && // The range of the trace...
				prms.find (1) != prms.end () && // The period of the trace...
				prms.find (2) != prms.end () && // The speed factor. 
												// The bigger the number is the lesser the final speed will be...
				prms.find (3) != prms.end () && // x coordinate to the orientation
				prms.find (4) != prms.end () && // y
				prms.find (5) != prms.end () && // z
				prms.find (6) != prms.end () && // initial angle (in radians)
				prms.find (7) != prms.end () );  // final
		assert ((*prms.find (2)).second != __BD 0); // Not possible speed reduction factor be 0...

		QGAMES::FollowingACurveMovement::CurveTemplate* crv1 = createCurve (__QGAMES_SINUSOIDALCURVEMOVEMENTTRACE__, prms);

		std::map <int, double> oPrms = prms;
		double t = oPrms [6]; oPrms [6] = oPrms [7]; oPrms [7] = t; // The opposite
		QGAMES::FollowingACurveMovement::CurveTemplate* crv2 = createCurve (__QGAMES_SINUSOIDALCURVEMOVEMENTTRACE__, oPrms);

		QGAMES::FollowingACurveMovement::CurveTemplates crvs;
		crvs.push_back (crv1), crvs.push_back (crv2);

		result = new QGAMES::FollowingACurveMovement::ComplexCurveTemplate (crvs);
	}
	else
		result = QGAMES::CurveTemplateBuilder::createCurve (t, prms);

	return (result);
}

// ---
BATTLESHIP::Spaceship* BattleshipII::EntityBuilder::createSpaceship (const QGAMES::EntityBuilder::EntityDefinition& def)
{
	BATTLESHIP::Spaceship* result = NULL;
	if (def._id >= __BATTLESHIPII_MOTHERSHIPBASEID__ &&
		def._id < (__BATTLESHIPII_MOTHERSHIPBASEID__ + __BATTLESHIPII_MAXNUMBERMOTHERSHIPS__))
		result = new BattleshipII::MothershipUFO (def._id); 
	else
		result = BATTLESHIP::EntityBuilder::createSpaceship (def);

	return (result);
}

// ---
QGAMES::World* BattleshipII::WorldBuilder::createWorldObject (int no, const QGAMES::Scenes& s, 
		const QGAMES::WorldProperties& p)
{
	QGAMES::World* result = NULL;

	if (no == __BATTLESHIPII_STAGE1WORLDID__)
		result = new BattleshipII::Stage1World (s, p);
	else
	if (no == __BATTLESHIPII_STAGE2WORLDID__)
		result = new BattleshipII::Stage2World (s, p);
	else
	if (no == __BATTLESHIPII_STAGE3WORLDID__)
		result = new BattleshipII::Stage3World (s, p);
	else
	if (no == __BATTLESHIPII_STAGE4WORLDID__)
		result = new BattleshipII::Stage4World (s, p);
	else
	if (no == __BATTLESHIPII_STAGE5WORLDID__)
		result = new BattleshipII::Stage5World (s, p);
	else
		result = BATTLESHIP::WorldBuilder::createWorldObject (no, s, p);

	return (result);
}

// ---
QGAMES::Scene* BattleshipII::WorldBuilder::createSceneObject (int ns, const QGAMES::Maps& m, 
		const QGAMES::Scene::Connections& cn, 
		const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
{
	QGAMES::Scene* result = NULL;

	// World Stage 1...
	if (ns == __BATTLESHIPII_STAGE1SCENE1__)
		result = new BattleshipII::Stage1Scene1 (m, cn, p, ePL);
	else
	if (ns == __BATTLESHIPII_STAGE1SCENE2__)
		result = new BattleshipII::Stage1Scene2 (m, cn, p, ePL);
	else
	if (ns == __BATTLESHIPII_STAGE1SCENE3__)
		result = new BattleshipII::Stage1Scene3 (m, cn, p, ePL);
	else

	// World Stage 2...
	if (ns == __BATTLESHIPII_STAGE2SCENE1__)
		result = new BattleshipII::Stage2Scene1 (m, cn, p, ePL);
	else
	if (ns == __BATTLESHIPII_STAGE2SCENE2__)
		result = new BattleshipII::Stage2Scene2 (m, cn, p, ePL);
	else
	if (ns == __BATTLESHIPII_STAGE2SCENE3__)
		result = new BattleshipII::Stage2Scene3 (m, cn, p, ePL);
	else

	// World Stage 3...
	if (ns == __BATTLESHIPII_STAGE3SCENE1__)
		result = new BattleshipII::Stage3Scene1 (m, cn, p, ePL);
	else

	// World Stage 4...
	if (ns == __BATTLESHIPII_STAGE4SCENE1__)
		result = new BattleshipII::Stage4Scene1 (m, cn, p, ePL);
	else
	if (ns == __BATTLESHIPII_STAGE4SCENE2__)
		result = new BattleshipII::Stage4Scene2 (m, cn, p, ePL);
	else
	if (ns == __BATTLESHIPII_STAGE4SCENE3__)
		result = new BattleshipII::Stage4Scene3 (m, cn, p, ePL);
	else

	// World Stage 5...
	if (ns == __BATTLESHIPII_STAGE5SCENE1__)
		result = new BattleshipII::Stage5Scene1 (m, cn, p, ePL);
	else
	if (ns == __BATTLESHIPII_STAGE5SCENE2__)
		result = new BattleshipII::Stage5Scene2 (m, cn, p, ePL);
	else
	if (ns == __BATTLESHIPII_STAGE5SCENE3__)
		result = new BattleshipII::Stage5Scene3 (m, cn, p, ePL);

	// Any other
	else
		result = BATTLESHIP::WorldBuilder::createSceneObject (ns, m, cn, p, ePL);


	return (result);
}

// ---
QGAMES::SceneActionBlock* BattleshipII::WorldBuilder::createSceneActionBlockObject (int nAB, 
			const QGAMES::SceneActionBlockProperties& prps)
{
	QGAMES::SceneActionBlock* result = NULL;

	// New type of action block to control the motherships...
	if (nAB >= __BATTLESHIPII_ACTIONBLOCKMOTHERSHIPUFOBASEID__ && 
			nAB < (__BATTLESHIPII_ACTIONBLOCKMOTHERSHIPUFOBASEID__ + __BATTLESHIPII_ACTIONBLOCKMOTHERSHIPUFONUMBER__))
		result = new BattleshipII::MothershipUFOSceneActionBlock (nAB, 
			new BattleshipII::MothershipUFOSceneActionBlock::Properties (prps) /** Default factory curve. */);

	// The action blocks used for set of UFOS are still valid, but with more combat factories...
	else
	if (nAB >= __BATTLESHIP_ACTIONBLOCKSETUFOSBASEID__ && 
		nAB < (__BATTLESHIP_ACTIONBLOCKSETUFOSBASEID__ + __BATTLESHIP_ACTIONBLOCKSETUFOSNUMBER__))
	{
		result = new BATTLESHIP::StdSetUFOsSceneActionBlock 
			(nAB, new BATTLESHIP::StdSetUFOsSceneActionBlock::Properties (prps),
				new BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactories 
					({ new BATTLESHIP::StdSetUFOsSceneActionBlock::BlockAndFlyCombatFormationFactory 
							(new BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory /** Rest default. */), 
					   new BATTLESHIP::StdSetUFOsSceneActionBlock::QueueAndFlyCombatFormationFactory 
							(new BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory /** Rest default. */), 
					   new BATTLESHIP::StdSetUFOsSceneActionBlock::DancingInLinesCombatFormationFactory 
							(new BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory /** Rest default. */), 
					   
					   // The condition is really needed because this two new combat formations are added!
					   new BattleshipII::SinusoideFromLeftBlockAndFlyCombatFormationFactory
							(new BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory /** Rest default. */),
					   new BattleshipII::SinusoideFromRightBlockAndFlyCombatFormationFactory
							(new BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory /** Rest default. */),
					   new BattleshipII::DancingInLinesForBigUFOSCombatFormationFactory
							(new BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory /** Rest default. */) })); 
	}

	// This new type of action block is for controlling the motherships better!
	else
	if (nAB >= __BATTLESHIPII_ACTIONBLOCKSETMOTHERSHIPBASEID__ && 
		nAB < (__BATTLESHIPII_ACTIONBLOCKSETMOTHERSHIPBASEID__ + __BATTLESHIPII_ACTIONBLOCKSETMOTHERSHIPNUMBER__))
		result = new BattleshipII::MothershipUFOSetSceneActionBlock 
			(nAB, new BATTLESHIP::StdSetUFOsSceneActionBlock::Properties (prps));

	// And then the rest...
	else
		result = BATTLESHIP::WorldBuilder::createSceneActionBlockObject (nAB, prps);

	return (result);
}
