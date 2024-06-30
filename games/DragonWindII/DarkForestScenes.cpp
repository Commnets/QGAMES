#include "DarkforestScenes.hpp"

const int DragonTheRevenge::DarkForestScene0::_COUNTERTOSHOWDISAPPEARHOLE [_NUMBERHOLES] = { 0, 1 };
const int DragonTheRevenge::DarkForestScene0::_SWITCHHOLEVISIBLE [_NUMBERHOLES] = { 0, 1 };

// ---
void DragonTheRevenge::DarkForestScene::initialize ()
{
	setBackgroundMap (__DRAGONWINDTHEREVENGE_DARKFORESTWORLDBACKGROUNDMAPID__);

	DRAGONWIND::PlayingScene::initialize ();
}

// ---
void DragonTheRevenge::DarkForestScene0::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_DARKFORESTWORLDSCENE0MAPID__);

	DragonTheRevenge::DarkForestScene::initialize ();

	QGAMES::TiledMap* pM = dynamic_cast <QGAMES::TiledMap*> (activeMap ());
	assert (pM); // Just in case...

	QGAMES::AdvancedTileLayers tL1;
	tL1 [0] = dynamic_cast <QGAMES::AdvancedTileLayer*> (pM -> layer (std::string ("Base_2")));
	tL1 [1] = dynamic_cast <QGAMES::AdvancedTileLayer*> (pM -> layer (std::string ("Solid_2")));
	assert (tL1 [0] && tL1 [1]);
	QGAMES::AdvancedTileLayers tL2;
	tL2 [0] = dynamic_cast <QGAMES::AdvancedTileLayer*> (pM -> layer (std::string ("Base_3")));
	tL2 [1] = dynamic_cast <QGAMES::AdvancedTileLayer*> (pM -> layer (std::string ("Solid_3")));
	assert (tL2 [0] && tL2 [1]);
	
	_layersHole.resize (_NUMBERHOLES);
	_layersHole [0] = tL1;
	_layersHole [1] = tL2;

	reStartAllCounters ();
	reStartAllOnOffSwitches ();
}

// ---
void DragonTheRevenge::DarkForestScene0::updatePositions ()
{
	DragonTheRevenge::DarkForestScene::updatePositions ();

	for (int i = 0; i < _NUMBERHOLES; i++)
	{
		if (onOffSwitch (_SWITCHHOLEVISIBLE [i]) -> isOn ())
		{
			if (!isHoleDisappearing (i) && counter (_COUNTERTOSHOWDISAPPEARHOLE [i]) -> isEnd ())
				dissapearHole (i);
			if (hasHoleDissapear (i))
				onOffSwitch (_SWITCHHOLEVISIBLE [i]) -> set (false);
		}
		else
		{
			if (!isHoleAppearing (i) && counter (_COUNTERTOSHOWDISAPPEARHOLE [i]) -> isEnd ())
				appearHole (i);
			if (hasHoleAppear (i))
				onOffSwitch (_SWITCHHOLEVISIBLE [i]) -> set (true);
		}
	}
}

// ---
void DragonTheRevenge::DarkForestScene0::finalize ()
{
	DragonTheRevenge::DarkForestScene::finalize ();

	_layersHole = std::vector <QGAMES::AdvancedTileLayers> ();
}

// ---
bool DragonTheRevenge::DarkForestScene0::isHoleDisappearing (int h)
{
	assert (h >= 0 && h < (int) _layersHole.size ());

	bool result = true;
	for (QGAMES::AdvancedTileLayers::const_iterator i = _layersHole [h].begin (); 
			i != _layersHole [h].end () && result; 
			result &= (((QGAMES::AdvancedTileLayer*) (*i++).second)) -> isDisappearing ());
	
	return (result);
}

// ---
void DragonTheRevenge::DarkForestScene0::dissapearHole (int h)
{
	assert (h >= 0 && h < (int) _layersHole.size ());

	for (QGAMES::AdvancedTileLayers::const_iterator i = _layersHole [h].begin (); 
			i != _layersHole [h].end (); 
			(((QGAMES::AdvancedTileLayer*) (*i++).second)) -> disappear (_secondsToVanishPerFrame));
}

// ---
bool DragonTheRevenge::DarkForestScene0::hasHoleDissapear (int h)
{
	assert (h >= 0 && h < (int) _layersHole.size ());

	bool result = true;
	for (QGAMES::AdvancedTileLayers::const_iterator i = _layersHole [h].begin (); 
			i != _layersHole [h].end () && result; 
			result &= (((QGAMES::AdvancedTileLayer*) (*i++).second)) -> transparent (__BD 0));
	
	return (result);
}

// ---
bool DragonTheRevenge::DarkForestScene0::isHoleAppearing (int h)
{
	assert (h >= 0 && h < (int) _layersHole.size ());

	bool result = true;
	for (QGAMES::AdvancedTileLayers::const_iterator i = _layersHole [h].begin (); 
			i != _layersHole [h].end () && result; 
			result &= (((QGAMES::AdvancedTileLayer*) (*i++).second)) -> isAppearing ());
	
	return (result);
}

// ---
void DragonTheRevenge::DarkForestScene0::appearHole (int h)
{
	assert (h >= 0 && h < (int) _layersHole.size ());

	for (QGAMES::AdvancedTileLayers::const_iterator i = _layersHole [h].begin (); 
			i != _layersHole [h].end (); 
			(((QGAMES::AdvancedTileLayer*) (*i++).second)) -> appear (_secondsToVanishPerFrame));
}

// ---
bool DragonTheRevenge::DarkForestScene0::hasHoleAppear (int h)
{
	assert (h >= 0 && h < (int) _layersHole.size ());

	bool result = true;
	for (QGAMES::AdvancedTileLayers::const_iterator i = _layersHole [h].begin (); 
			i != _layersHole [h].end () && result; 
			result &= (((QGAMES::AdvancedTileLayer*) (*i++).second)) -> solid (__BD 1));
	
	return (result);
}

// ---
__IMPLEMENTCOUNTERS__ (DragonTheRevenge::DarkForestScene0::Counters)
{
	for (int i = 0; i < _NUMBERHOLES; i++)
		addCounter (new QGAMES::Counter 
			(DragonTheRevenge::DarkForestScene0::_COUNTERTOSHOWDISAPPEARHOLE [i], 
				(int) (QGAMES::Game::game () -> framesPerSecond () * 2.1), 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (DragonTheRevenge::DarkForestScene0::OnOffSwitches)
{
	for (int i = 0; i < _NUMBERHOLES;i++)
		addOnOffSwitch (new QGAMES::OnOffSwitch 
			(DragonTheRevenge::DarkForestScene0::_SWITCHHOLEVISIBLE [i], false));
}

// ---
void DragonTheRevenge::DarkForestScene1::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_DARKFORESTWORLDSCENE1MAPID__);

	DragonTheRevenge::DarkForestScene::initialize ();

	_blockRemoveableWallActionBlock = 
		dynamic_cast <DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock*> (actionBlock (73015));
	assert (_blockRemoveableWallActionBlock);

	reStartAllOnOffSwitches ();

	if (!_ninja -> carriesTypeInPocket (__DRAGONWINDTHEREVENGE_NINJATHINGLIGHTTYPE__))
		_ninja -> toSay (std::string ("Light is needed,  look for it before"),
			DRAGONWIND::DragonArtist::DialogProperties (__QGAMES_COURIER10WHITELETTERS__, 5, 9, 
				__QGAMES_SHADOWCOLOR__, __QGAMES_WHITECOLOR__, 255, __BD 1.5, 
				QGAMES::Vector (__BD -100, __BD -100, __BD 0)));
}

// ---
void DragonTheRevenge::DarkForestScene1::updatePositions ()
{
	DragonTheRevenge::DarkForestScene::updatePositions ();

	_blockRemoveableWallActionBlock -> activeSetOfLayers 
		(onOffSwitch (_SWITCHTOSHOWWALL) -> isOn () ? 0 : -1);
}

// ---
void DragonTheRevenge::DarkForestScene1::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	int sW = s -> visualWidth ();
	int sH = s -> visualHeight ();

	DRAGONWIND::Game* g = dynamic_cast <DRAGONWIND::Game*> (game ());
	assert (g);
	DRAGONWIND::Ninja* n = dynamic_cast <DRAGONWIND::Ninja*> (g -> mainArtist ());
	assert (n);
	QGAMES::Position pN =
		n -> centerPosition () + (__BD 10 * n -> orientation ());
	int rdx = n -> carriesTypeInPocket (__DRAGONWINDTHEREVENGE_NINJATHINGLIGHTTYPE__) ? 150 : 50;

	QGAMES::Mask* mask = new QGAMES::OUTCircleMask (10000 /* id */, 3,
			QGAMES::OUTCircleMask::Properties (rdx, QGAMES::Position 
				(pN.posX () / __BD sW, pN.posY () / __BD sH, __BD 0), __QGAMES_WHITECOLOR__), sW, sH);
	QGAMES::Mask* oMask = mask -> opposite ();

	QGAMES::Screen::Buffer* bA = mask -> bufferOver (s);			
	QGAMES::Screen::Buffer* gSA = s -> openBuffer (QGAMES::Screen::Buffer::_ADDBLENDMODE);
	DragonTheRevenge::DarkForestScene::drawOn (s, p);
	s -> drawRectangle (QGAMES::Rectangle 
		(QGAMES::Position (__BD 0, __BD 0, __BD 0), QGAMES::Position (__BD sW, __BD sH, __BD 0)), 
			QGAMES::Color (0, 0, 0, 220), true);
	bA -> moveToBuffer (gSA);
	s -> closeBuffer (gSA);

	QGAMES::Screen::Buffer* bB = oMask -> bufferOver (s);
	QGAMES::Screen::Buffer* gSB = s -> openBuffer (QGAMES::Screen::Buffer::_PUREBLENDMODE);
	DragonTheRevenge::DarkForestScene::drawOn (s, p);
	bB -> moveToBuffer (gSB);
	s -> closeBuffer (gSB);

	gSB -> moveToScreen ();
	gSA -> moveToScreen ();								
	
	delete (gSB);
	delete (gSA);

	delete (oMask);
	delete (mask);
}

// ---
void DragonTheRevenge::DarkForestScene1::finalize ()
{
	DragonTheRevenge::DarkForestScene::finalize ();

	_blockRemoveableWallActionBlock = NULL;
}

// ---
void DragonTheRevenge::DarkForestScene1::explosionAround (DRAGONWIND::Shooting* sth, QGAMES::bdata rdx)
{
	DragonTheRevenge::DarkForestScene::explosionAround (sth, rdx);

	if ((sth -> centerPosition () - QGAMES::Position (__BD 1656, __BD 168, __BD 0)).module () < (rdx * __BD 2))
		onOffSwitch (_SWITCHTOSHOWWALL) -> set (false);
}

// ---
__IMPLEMENTONOFFSWITCHES__ (DragonTheRevenge::DarkForestScene1::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(DragonTheRevenge::DarkForestScene1::_SWITCHTOSHOWWALL, true));
}

// ---
void DragonTheRevenge::DarkForestScene2::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_DARKFORESTWORLDSCENE2MAPID__);

	DragonTheRevenge::DarkForestScene::initialize ();

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
void DragonTheRevenge::DarkForestScene2::finalize ()
{
	_badGuyEnergyLevel -> setVisible (false);
	_badGuyEnergyLevel -> unObserve (_mainBadGuy);
	_badGuyEnergyLevel = NULL;

	DragonTheRevenge::DarkForestScene::finalize ();
}
