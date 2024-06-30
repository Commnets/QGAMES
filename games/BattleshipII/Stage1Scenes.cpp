#include "Stage1Scenes.hpp"

// ---
void BattleshipII::Stage1Scene::initialize ()
{
	setBackgroundMap (__BATTLESHIP_TRANSPARENTMAPID__); 

	setMap (__BATTLESHIP_COMPLEXSTARTSMOVINGBACKGROUNDMAP1ID__);

	BATTLESHIP::TimeLimitScene::initialize ();
}

// ---
BattleshipII::Stage1Scene3::Stage1Scene3 (const QGAMES::Maps& m, 
	const QGAMES::Scene::Connections& cn, const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
		: BattleshipII::Stage1Scene (__BATTLESHIPII_STAGE1SCENE3__, m, cn, p, ePL)
{
	onOffSwitches () -> addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHLIFEONTHESCREEN, false));
	onOffSwitches () -> addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHLIFEALREADYGOT, false));
}

// ---
void BattleshipII::Stage1Scene3::updatePositions ()
{
	BattleshipII::Stage1Scene::updatePositions ();

	if (onOffSwitch (_SWITCHLIFEONTHESCREEN) -> isOn ())
	{
		if (!dynamic_cast <BATTLESHIP::StdLiveSceneActionBlock*> 
				(actionBlock (_LIFEBLOCKNUMBER)) -> spaceElementsUnderControl ()) // It has exit the screen...
		{
			removeActionBlock (_LIFEBLOCKNUMBER);

			onOffSwitch (_SWITCHLIFEONTHESCREEN) -> set (false);
		}
	}
	else
	{
		if ((currentSceneTime () / sceneTime ()) > __BD 0.5 && // Since the mid time of the stage...
			!onOffSwitch (_SWITCHLIFEALREADYGOT) -> isOn () && // and when it hasn't been got already
			__1IN3PROBABLE__) // and 1 in three probable...
		{
			addActionBlock (new BATTLESHIP::StdLiveSceneActionBlock (_LIFEBLOCKNUMBER, 
				new BATTLESHIP::StdLiveSceneActionBlock::Properties 
					(__BD 0,							// Appears inmediatly
					 100,								// Always appear
					 false,								// No emerge
					  __BD 0,							// Moves inmediatly
					 false,								// Never regenerate 
					 false,								// Never rolls
					 QGAMES::Position::_noPoint,		// Random at the top
					 QGAMES::Rectangle::_noRectangle,   // The moving zone got will be th one of the heart
					 __BD -1,							// Never finishes 
					 BATTLESHIP::LiveToCatch::_LIVETOCATCH, // It is a life 
					 0,									// Moves normaly. Not used! 
					 __BD 1)));							// Moves slowly

			onOffSwitch (_SWITCHLIFEONTHESCREEN) -> set (true);
		}
	}
}

// ---
void BattleshipII::Stage1Scene3::finalize ()
{
	BattleshipII::Stage1Scene::finalize ();

	// If the fife action block was in the screen, it would be eliminated
	// So it is not storeable!
	if (onOffSwitch (_SWITCHLIFEONTHESCREEN) -> isOn ())
	{
		removeActionBlock (_LIFEBLOCKNUMBER);

		onOffSwitch (_SWITCHLIFEONTHESCREEN) -> set (false);
	}
}

// ---
void BattleshipII::Stage1Scene3::processEvent (const QGAMES::Event& evnt)
{
	BattleshipII::Stage1Scene::processEvent (evnt);

	if (evnt.code () == __BATTLESHIP_BATTLESHIPLIVESACTUALIZED__)
		onOffSwitch (_SWITCHLIFEALREADYGOT) -> set (true);
}
