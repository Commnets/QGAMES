#include "MountainsScenes.hpp"
		
const int DragonTheRevenge::MountainsScene0::_SWITCHTOSHOWSOLIDWALL [_NUMBERSOLIDS] = { 0, 1 };
const int DragonTheRevenge::MountainsScene1::_COUNTERSOLIDTOMOVE [_NUMBERSOLIDS] = { 0, 1, 2, 3 };
const int DragonTheRevenge::MountainsScene1::_SWITCHSOLIDMOVING [_NUMBERSOLIDS] = { 0, 1, 2, 3 };

// ---
void DragonTheRevenge::MountainsScene::initialize ()
{
	setBackgroundMap (__DRAGONWINDTHEREVENGE_MOUNTAINSWORLDBACKGROUNDMAPID__);

	DRAGONWIND::PlayingScene::initialize ();
}

// ---
void DragonTheRevenge::MountainsScene0::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_MOUNTAINSWORLDSCENE0MAPID__);

	DragonTheRevenge::MountainsScene::initialize ();

	_blockRemoveableActionBlocks = 
		std::vector <DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock*> (_NUMBERSOLIDS);
	for (int i = 0; i < _NUMBERSOLIDS; i++)
	{
		_blockRemoveableActionBlocks [i] = 
			dynamic_cast <DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock*> 
				(actionBlock (73005 + i)); // The base is 73005 in this screen...
		assert (_blockRemoveableActionBlocks [i]);
	}

	_doorActionBlock = dynamic_cast <DRAGONWIND::MoveLinearASetOfLayersActionBlock*> (actionBlock (74000));
	assert (_doorActionBlock);

	reStartAllOnOffSwitches ();

	if (!_ninja -> carriesTypeInPocket (__DRAGONWINDTHEREVENGE_NINJATHINGKEYTYPE__))
		_ninja -> toSay (std::string ("A key is needed to open door"),
			DRAGONWIND::DragonArtist::DialogProperties (__QGAMES_COURIER10WHITELETTERS__, 5, 10, 
				__QGAMES_SHADOWCOLOR__, __QGAMES_WHITECOLOR__, 255, __BD 1.5, 
				QGAMES::Vector (__BD -100, __BD -100, __BD 0)));

	onOffSwitch (_SWITCHDOOROPEN) -> 
		set (_ninja -> carriesTypeInPocket (__DRAGONWINDTHEREVENGE_NINJATHINGKEYTYPE__));
}

// ---
void DragonTheRevenge::MountainsScene0::updatePositions ()
{
	DragonTheRevenge::MountainsScene::updatePositions ();

	onOffSwitch (_SWITCHDOOROPEN) -> 
		set (_ninja -> carriesTypeInPocket (__DRAGONWINDTHEREVENGE_NINJATHINGKEYTYPE__));

	for (int i = 0; i < _NUMBERSOLIDS; i++)
		_blockRemoveableActionBlocks [i] -> activeSetOfLayers 
			(onOffSwitch (_SWITCHTOSHOWSOLIDWALL [i]) -> isOn () ? 0 : -1);

	if (onOffSwitch (_SWITCHDOOROPEN) -> isOn ())
		_doorActionBlock -> toMoveForward ();
	else
		_doorActionBlock -> toMoveBackward ();
}

// ---
void DragonTheRevenge::MountainsScene0::finalize ()
{
	DragonTheRevenge::MountainsScene::finalize ();

	_blockRemoveableActionBlocks = 
		std::vector <DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock*> ();

	_doorActionBlock = NULL;
}

// ---
void DragonTheRevenge::MountainsScene0::explosionAround (DRAGONWIND::Shooting* sth, QGAMES::bdata rdx)
{
	DragonTheRevenge::MountainsScene::explosionAround (sth, rdx);

	if ((sth -> centerPosition () - QGAMES::Position (__BD 864, __BD 96, __BD 0)).module () < (rdx * __BD 2))
		onOffSwitch (_SWITCHTOSHOWSOLIDWALL [0]) -> set (false);
	if ((sth -> centerPosition () - QGAMES::Position (__BD 1728, __BD 120, __BD 0)).module () < (rdx * __BD 2))
		onOffSwitch (_SWITCHTOSHOWSOLIDWALL [1]) -> set (false);
}

// ---
__IMPLEMENTONOFFSWITCHES__ (DragonTheRevenge::MountainsScene0::OnOffSwitches)
{
	for (int i = 0; i < _NUMBERSOLIDS; i++)
		addOnOffSwitch (new QGAMES::OnOffSwitch 
			(DragonTheRevenge::MountainsScene0::_SWITCHTOSHOWSOLIDWALL [i], true));

	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(DragonTheRevenge::MountainsScene0::_SWITCHDOOROPEN, false));
}

// ---
void DragonTheRevenge::MountainsScene1::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_MOUNTAINSWORLDSCENE1MAPID__);

	DragonTheRevenge::MountainsScene::initialize ();

	QGAMES::TiledMap* pM = dynamic_cast <QGAMES::TiledMap*> (activeMap ());
	assert (pM); // Just in case...

	_solidLayers [0] = dynamic_cast <QGAMES::AdvancedTileLayer*> (pM -> layer (std::string ("Solid_1")));
	_solidLayers [1] = dynamic_cast <QGAMES::AdvancedTileLayer*> (pM -> layer (std::string ("Solid_2")));
	_solidLayers [2] = dynamic_cast <QGAMES::AdvancedTileLayer*> (pM -> layer (std::string ("Solid_3")));
	_solidLayers [3] = dynamic_cast <QGAMES::AdvancedTileLayer*> (pM -> layer (std::string ("Solid_4")));
	assert (_solidLayers [0] && _solidLayers [1] && _solidLayers [2] && _solidLayers [3]);

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	for (int i = 0; i < _NUMBERSOLIDS; i++)
		counter (_COUNTERSOLIDTOMOVE [i]) -> initialize 
			((int) (2.1 * (rand () % (game () -> framesPerSecond ()) + 1)), 0, true, true);

	for (int i = 0; i < _NUMBERSOLIDS; i++)
		movePillar (i, onOffSwitch (_SWITCHSOLIDMOVING [i]) -> isOn ());
}

// ---
void DragonTheRevenge::MountainsScene1::updatePositions ()
{
	for (int i = 0; i < _NUMBERSOLIDS; i++)
		if (onOffSwitch (_SWITCHSOLIDMOVING [i]) -> isOn () && !isPillarMoving (i))
			movePillar (i, true);

	DragonTheRevenge::MountainsScene::updatePositions ();

	for (int i = 0; i < _NUMBERSOLIDS; i++)
		onOffSwitch (_SWITCHSOLIDMOVING [i]) -> set (isPillarMoving (i));

	for (int i = 0; i < _NUMBERSOLIDS; i++)
	{
		if (!onOffSwitch (_SWITCHSOLIDMOVING [i]) -> isOn () &&
			counter (_COUNTERSOLIDTOMOVE [i]) -> isEnd ())
		{
			onOffSwitch (_SWITCHSOLIDMOVING [i]) -> set (true);

			counter (_COUNTERSOLIDTOMOVE [i]) -> initialize 
				((int) (1.1 * (rand () % (game () -> framesPerSecond ()) + 1)), 0, true, true);
		}
	}
}

// ---
void DragonTheRevenge::MountainsScene1::finalize ()
{
	DragonTheRevenge::MountainsScene::finalize ();

	_solidLayers = QGAMES::AdvancedTileLayers ();
}

// ---
bool DragonTheRevenge::MountainsScene1::isPillarMoving (int nP)
{
	assert (nP >= 0 && nP < _NUMBERSOLIDS);

	return (_solidLayers [nP] -> moves () &&
			!_solidLayers [nP] -> movement () -> hasFinished ());
}

// ---
void DragonTheRevenge::MountainsScene1::movePillar (int nP, bool m)
{
	assert (nP >= 0 && nP < _NUMBERSOLIDS);

	if (m)
	{
		QGAMES::Layer::MovementBehaviour* mov = _solidLayers [nP] -> movement ();
		assert (mov);
		mov -> initialize ();
	}

	_solidLayers [nP] -> setMove (m);
}

// ---
__IMPLEMENTCOUNTERS__ (DragonTheRevenge::MountainsScene1::Counters)
{
	for (int i = 0; i < _NUMBERSOLIDS; i++)
		addCounter (new QGAMES::Counter 
			(DragonTheRevenge::MountainsScene1::_COUNTERSOLIDTOMOVE [i], 
				(int) (QGAMES::Game::game () -> framesPerSecond () * 2.1), 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (DragonTheRevenge::MountainsScene1::OnOffSwitches)
{
	for (int i = 0; i < _NUMBERSOLIDS; i++)
		addOnOffSwitch (new QGAMES::OnOffSwitch 
			(DragonTheRevenge::MountainsScene1::_SWITCHSOLIDMOVING [i], false));
}

// ---
void DragonTheRevenge::MountainsScene2::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_MOUNTAINSWORLDSCENE2MAPID__);

	DragonTheRevenge::MountainsScene::initialize ();

	_doorActionBlock = dynamic_cast <DRAGONWIND::MoveLinearASetOfLayersActionBlock*> (actionBlock (74001));
	assert (_doorActionBlock);

	if (!_ninja -> carriesTypeInPocket (__DRAGONWINDTHEREVENGE_NINJATHINGHAMMERTYPE__))
		_ninja -> toSay (std::string ("A hammer is   needed to open doors"),
			DRAGONWIND::DragonArtist::DialogProperties (__QGAMES_COURIER10WHITELETTERS__, 5, 14, 
				__QGAMES_SHADOWCOLOR__, __QGAMES_WHITECOLOR__, 255, __BD 1.5, 
				QGAMES::Vector (__BD -100, __BD -100, __BD 0)));

	onOffSwitch (_SWITCHDOOROPEN) -> 
		set (_ninja -> carriesTypeInPocket (__DRAGONWINDTHEREVENGE_NINJATHINGHAMMERTYPE__));
}

// ---
void DragonTheRevenge::MountainsScene2::updatePositions ()
{
	DragonTheRevenge::MountainsScene::updatePositions ();

	onOffSwitch (_SWITCHDOOROPEN) -> 
		set (_ninja -> carriesTypeInPocket (__DRAGONWINDTHEREVENGE_NINJATHINGHAMMERTYPE__));

	if (onOffSwitch (_SWITCHDOOROPEN) -> isOn ())
		_doorActionBlock -> toMoveForward ();
	else
		_doorActionBlock -> toMoveBackward ();
}

// ---
void DragonTheRevenge::MountainsScene2::finalize ()
{
	DragonTheRevenge::MountainsScene::finalize ();

	_doorActionBlock = NULL;
}

// ---
__IMPLEMENTONOFFSWITCHES__ (DragonTheRevenge::MountainsScene2::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(DragonTheRevenge::MountainsScene2::_SWITCHDOOROPEN, false));
}

// ---
void DragonTheRevenge::MountainsScene3::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_MOUNTAINSWORLDSCENE3MAPID__);

	DragonTheRevenge::MountainsScene::initialize ();

	setBackgroundMap (__DRAGONWINDTHEREVENGE_MOUNTAINSWORLDLARGEWORLDBKMAPID__);
	backgroundMap () -> initialize ();
	backgroundMap () -> stop ();
}

// ---
void DragonTheRevenge::MountainsScene4::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_MOUNTAINSWORLDSCENE4MAPID__);

	DragonTheRevenge::MountainsScene::initialize ();

	assert (!_badGuys.empty ());
	_mainBadGuy = firstBadGuyType (__DRAGONWIND_JACKTYPEID__);
	assert (_mainBadGuy);

	DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
	assert (dG);
	_badGuyEnergyLevel = dG -> badGuyEnergyLevelScoreObject ();
	assert (_badGuyEnergyLevel);
	_badGuyEnergyLevel -> observe (_mainBadGuy);
	_badGuyEnergyLevel -> setVisible (true);
}

// ---
void DragonTheRevenge::MountainsScene4::finalize ()
{
	_badGuyEnergyLevel -> setVisible (false);
	_badGuyEnergyLevel -> unObserve (_mainBadGuy);
	_badGuyEnergyLevel = NULL;

	DragonTheRevenge::MountainsScene::finalize ();
}
