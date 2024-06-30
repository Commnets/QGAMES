#include "TempleScenes.hpp"

const int DragonTheRevenge::TempleScene2::_SWITCHDOOROPEN [_NUMBERDOORS] = { 0, 1, 2 };

// ---
void DragonTheRevenge::TempleScene::initialize ()
{
	setBackgroundMap (__DRAGONWINDTHEREVENGE_TEMPLEWORLDBACKGROUNDMAPID__);

	DRAGONWIND::PlayingScene::initialize ();

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
void DragonTheRevenge::TempleScene::finalize ()
{
	_badGuyEnergyLevel -> setVisible (false);
	_badGuyEnergyLevel -> unObserve (_mainBadGuy);
	_badGuyEnergyLevel = NULL;

	DRAGONWIND::PlayingScene::finalize ();
}

// ---
void DragonTheRevenge::TempleScene0::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_TEMPLEWORLDSCENE0MAPID__);

	DragonTheRevenge::TempleScene::initialize ();
}

// ---
void DragonTheRevenge::TempleScene1::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_TEMPLEWORLDSCENE1MAPID__);

	DragonTheRevenge::TempleScene::initialize ();
}

// ---
void DragonTheRevenge::TempleScene2::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_TEMPLEWORLDSCENE2MAPID__);

	DragonTheRevenge::TempleScene::initialize ();

	_doorActionBlocks = 
		std::vector <DRAGONWIND::MoveLinearASetOfLayersActionBlock*> (_NUMBERDOORS);
	for (int i = 0; i < _NUMBERDOORS; i++)
	{
		_doorActionBlocks [i] = 
			dynamic_cast <DRAGONWIND::MoveLinearASetOfLayersActionBlock*> (actionBlock (74005 + i));
		assert (_doorActionBlocks [i]);
	}

	reStartAllOnOffSwitches ();

	if (_ninja -> numberOfTypesInPocket (__DRAGONWIND_NINJATHINGPAPERTYPE__) != 3)
		_ninja -> toSay (std::string ("The paper to open   doors are possible"),
			DRAGONWIND::DragonArtist::DialogProperties (__QGAMES_COURIER10WHITELETTERS__, 5, 10, 
				__QGAMES_SHADOWCOLOR__, __QGAMES_WHITECOLOR__, 255, __BD 1.5, 
				QGAMES::Vector (__BD -100, __BD -100, __BD 0)));

	onOffSwitch (_SWITCHDOOROPEN [0]) -> 
		set (_ninja -> numberOfTypesInPocket (__DRAGONWIND_NINJATHINGPAPERTYPE__) > 0);
	onOffSwitch (_SWITCHDOOROPEN [1]) -> 
		set (_ninja -> numberOfTypesInPocket (__DRAGONWIND_NINJATHINGPAPERTYPE__) > 1);
	onOffSwitch (_SWITCHDOOROPEN [2]) -> 
		set (_ninja -> numberOfTypesInPocket (__DRAGONWIND_NINJATHINGPAPERTYPE__) > 2);

	QGAMES::Entities bG = badGuysType (__DRAGONWIND_KONGTYPEID__);
	assert ((int) bG.size () == 2); // Only 2 monkeys...
	QGAMES::Entities::const_iterator bGi = bG.begin ();
	_monkey1 = dynamic_cast <DRAGONWIND::BadGuy*> ((*bGi++).second); 
	_monkey2 = dynamic_cast <DRAGONWIND::BadGuy*> ((*bGi).second);
}

// ---
void DragonTheRevenge::TempleScene2::updatePositions ()
{
	DragonTheRevenge::TempleScene::updatePositions ();

	onOffSwitch (_SWITCHDOOROPEN [0]) -> 
		set (_ninja -> numberOfTypesInPocket (__DRAGONWIND_NINJATHINGPAPERTYPE__) > 0);
	onOffSwitch (_SWITCHDOOROPEN [1]) -> 
		set (_ninja -> numberOfTypesInPocket (__DRAGONWIND_NINJATHINGPAPERTYPE__) > 1);
	onOffSwitch (_SWITCHDOOROPEN [2]) -> 
		set (_ninja -> numberOfTypesInPocket (__DRAGONWIND_NINJATHINGPAPERTYPE__) > 2);

	for (int i = 0; i < _NUMBERDOORS; i++)
	{
		if (onOffSwitch (_SWITCHDOOROPEN [i]) -> isOn ())
			_doorActionBlocks [i] -> toMoveForward ();
		else
			_doorActionBlocks [i] -> toMoveBackward ();
	}

	if (onOffSwitch (_SWITCHEARTHQUAKE) -> isOn () && counter (_COUNTEREARTHQUAKE) -> isEnd ())
	{
		setEnvironmentalCondition (NULL);

		onOffSwitch (_SWITCHEARTHQUAKE) -> set (false);
	}
}

// ---
void DragonTheRevenge::TempleScene2::finalize ()
{
	_doorActionBlocks = 
		std::vector <DRAGONWIND::MoveLinearASetOfLayersActionBlock*> ();

	DragonTheRevenge::TempleScene::finalize ();
}

// ---
void DragonTheRevenge::TempleScene2::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __QGAMES_CHRSTEPSMONITORSTARTSBACK__ &&
		(evnt.data () == (void*) _monkey1 || evnt.data () == (void*) _monkey2) && 
		!onOffSwitch (_SWITCHEARTHQUAKE) -> isOn ())
	{
		setEnvironmentalCondition (new QGAMES::Earthquake (this));

		onOffSwitch (_SWITCHEARTHQUAKE) -> set (true); // When the monkey steps the floor 
	}

	DragonTheRevenge::TempleScene::processEvent (evnt);
}

// ---
__IMPLEMENTONOFFSWITCHES__ (DragonTheRevenge::TempleScene2::OnOffSwitches)
{
	for (int i = 0; i < _NUMBERDOORS; i++)
		addOnOffSwitch (new QGAMES::OnOffSwitch 
			(DragonTheRevenge::TempleScene2::_SWITCHDOOROPEN [i], true));
	addOnOffSwitch (new QGAMES::OnOffSwitch (DragonTheRevenge::TempleScene2::_SWITCHEARTHQUAKE, false));
}

__IMPLEMENTCOUNTERS__ (DragonTheRevenge::TempleScene2::Counters)
{
	addCounter (new QGAMES::Counter 
		(_COUNTEREARTHQUAKE, (int) (QGAMES::Game::game () -> framesPerSecond () * 0.25), 0, true, true));
}
