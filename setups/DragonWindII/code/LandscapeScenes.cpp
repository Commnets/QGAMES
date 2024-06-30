#include "LandscapeScenes.hpp"

// ---
const int DragonTheRevenge::LandscapeScene1::_SWITCHTOSHOWWALLS [_NUMBERWALLS] = { 0, 1 };


// ---
void DragonTheRevenge::LandscapeScene::initialize ()
{
	setBackgroundMap (__DRAGONWINDTHEREVENGE_LANDSCAPEWORLDBACKGROUNDMAPID__);

	DRAGONWIND::PlayingScene::initialize ();
}

// ---
void DragonTheRevenge::LandscapeScene0::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_LANDSCAPEWORLDSCENE0MAPID__);

	DragonTheRevenge::LandscapeScene::initialize ();
}

// ---
QGAMES::SetOfOpenValues DragonTheRevenge::LandscapeScene1::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = DragonTheRevenge::LandscapeScene::runtimeValues ();

	int lNE = result.lastNestedOpenValueId ();

	result.addSetOfOpenValues (lNE + 1, 
		_gorilla ? _gorilla -> runtimeValues () 
			: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__)));

	return (result);
}

// ---
void DragonTheRevenge::LandscapeScene1::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	QGAMES::SetOfOpenValues cVV = cCfg.setOfOpenValues (lNE);
	assert (cVV.name () == std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__));
	if (_gorilla && cVV.numberValues () != 0)
		_gorilla -> initializeRuntimeValuesFrom (cVV);
	cCfg.removeSetOfOpenValues (lNE);

	DragonTheRevenge::LandscapeScene::initializeRuntimeValuesFrom (cCfg);
}

// ---
void DragonTheRevenge::LandscapeScene1::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_LANDSCAPEWORLDSCENE1MAPID__);

	DragonTheRevenge::LandscapeScene::initialize ();

	setBackgroundMap (__DRAGONWINDTHEREVENGE_LANDSCAPEWITHCAVEWORLDBKMAPID__);
	backgroundMap () -> initialize ();
	backgroundMap () -> stop ();

	_blockRemoveableActionBlocks = 
		std::vector <DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock*> (_NUMBERWALLS);
	for (int i = 0; i < _NUMBERWALLS; i++)
	{
		_blockRemoveableActionBlocks [i] = 
			dynamic_cast <DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock*> 
				(actionBlock (73000 + i)); // The base is 73000 in this screen...
		assert (_blockRemoveableActionBlocks [i]);
	}

	_gorilla = firstBadGuyType (__DRAGONWIND_KONGTYPEID__);
	assert (_gorilla);
	_gorillaActionBlock = actionBlockControlling (_gorilla);
	assert (_gorillaActionBlock);

	reStartAllOnOffSwitches ();
}

// ---
void DragonTheRevenge::LandscapeScene1::updatePositions ()
{
	DragonTheRevenge::LandscapeScene::updatePositions ();

	for (int i = 0; i < _NUMBERWALLS; i++)
		_blockRemoveableActionBlocks [i] -> activeSetOfLayers 
			(onOffSwitch (_SWITCHTOSHOWWALLS [i]) -> isOn () ? 0 : 1);

	if (isThingVisible (__DRAGONWIND_NINJATHINGCOINTYPE__))
		onOffSwitch (_SWITCHCOINSHOWN) -> set (true); 

	if (onOffSwitch (_SWITCHCOINSHOWN) -> isOn () &&
		_gorilla -> stepsMonitor () != NULL &&
		_gorilla -> stepsMonitor () -> id () != __DRAGONWINDTHEREVENGE_LANDSCAPEWORLDSCENE1GORILLAABID__)
		_gorilla -> addControlStepsMonitor (game () -> characterMonitorBuilder () -> 
			monitorFor (__DRAGONWINDTHEREVENGE_LANDSCAPEWORLDSCENE1GORILLAABID__, _gorilla));
}

// ---
void DragonTheRevenge::LandscapeScene1::finalize ()
{
	DragonTheRevenge::LandscapeScene::finalize ();

	_blockRemoveableActionBlocks = 
		std::vector <DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock*> ();

	_gorilla = NULL;
	_gorillaActionBlock = NULL;
}

// ---
void DragonTheRevenge::LandscapeScene1::explosionAround (DRAGONWIND::Shooting* sth, QGAMES::bdata rdx)
{
	DragonTheRevenge::LandscapeScene::explosionAround (sth, rdx);

	if ((sth -> centerPosition () - QGAMES::Position (__BD 264, __BD 1680, __BD 0)).module () < (rdx * __BD 2))
		onOffSwitch (_SWITCHTOSHOWWALLS [0]) -> set (false);
	if ((sth -> centerPosition () - QGAMES::Position (__BD 1454, __BD 1680, __BD 0)).module () < (rdx * __BD 2))
		onOffSwitch (_SWITCHTOSHOWWALLS [1]) -> set (false);
}

// ---
__IMPLEMENTONOFFSWITCHES__ (DragonTheRevenge::LandscapeScene1::OnOffSwitches)
{
	for (int i = 0; i < _NUMBERWALLS; i++)
		addOnOffSwitch (new QGAMES::OnOffSwitch 
			(DragonTheRevenge::LandscapeScene1::_SWITCHTOSHOWWALLS [i], true));
	addOnOffSwitch (new QGAMES::OnOffSwitch (DragonTheRevenge::LandscapeScene1::_SWITCHCOINSHOWN, false));
}

// ---
void DragonTheRevenge::LandscapeScene2::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_LANDSCAPEWORLDSCENE2MAPID__);

	DragonTheRevenge::LandscapeScene::initialize ();

	setBackgroundMap (__DRAGONWINDTHEREVENGE_CAVEWORLDBKMAPID__);
	backgroundMap () -> initialize ();
	backgroundMap () -> stop ();
}

// ---
void DragonTheRevenge::LandscapeScene3::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_LANDSCAPEWORLDSCENE3MAPID__);

	DragonTheRevenge::LandscapeScene::initialize ();

	setBackgroundMap (__DRAGONWINDTHEREVENGE_CAVEWORLDBKMAPID__);
	backgroundMap () -> initialize ();
	backgroundMap () -> stop ();
}

// ---
void DragonTheRevenge::LandscapeScene4::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_LANDSCAPEWORLDSCENE4MAPID__);

	DragonTheRevenge::LandscapeScene::initialize ();

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
void DragonTheRevenge::LandscapeScene4::finalize ()
{
	_badGuyEnergyLevel -> setVisible (false);
	_badGuyEnergyLevel -> unObserve (_mainBadGuy);
	_badGuyEnergyLevel = NULL;

	DragonTheRevenge::LandscapeScene::finalize ();
}
