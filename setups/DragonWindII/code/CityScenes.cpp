#include "CityScenes.hpp"

// ---
void DragonTheRevenge::CityScene::initialize ()
{
	setBackgroundMap (__DRAGONWINDTHEREVENGE_CITYWORLDBACKGROUNDMAPID__);

	DRAGONWIND::PlayingScene::initialize ();
}

// ---
void DragonTheRevenge::CityScene0::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_CITYWORLDSCENE0MAPID__);

	DragonTheRevenge::CityScene::initialize ();
}

// ---
void DragonTheRevenge::CityScene1::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_CITYWORLDSCENE1MAPID__);

	DragonTheRevenge::CityScene::initialize ();

	_theBadGuysToKill = QGAMES::Entities ();
	std::vector <int> tM = { __DRAGONWIND_CATTYPEID__, __DRAGONWIND_DOGTYPEID__ };
	for (QGAMES::Entities::const_iterator i = characters ().begin (); i != characters ().end (); i++)
		if (dynamic_cast <DRAGONWIND::BadGuy*> ((*i).second) && 
			std::find (tM.begin (), tM.end (), ((DRAGONWIND::BadGuy*) ((*i).second)) -> definition () -> _type) != tM.end ())
			_theBadGuysToKill [(*i).first] = (*i).second; // The three elements to die...

	_blockMoatActionBlock = 
		dynamic_cast <DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock*> (actionBlock (73010));
	assert (_blockMoatActionBlock);
}

// ---
void DragonTheRevenge::CityScene1::finalize ()
{
	DragonTheRevenge::CityScene::finalize ();

	_blockMoatActionBlock = NULL;
}

// ---
void DragonTheRevenge::CityScene1::updatePositions ()
{
	DragonTheRevenge::CityScene::updatePositions ();

	bool someAlive = false;
	for (QGAMES::Entities::const_iterator i = _theBadGuysToKill.begin (); 
			i != _theBadGuysToKill.end () && !someAlive; someAlive |= ((DRAGONWIND::BadGuy*) (*i++).second) -> isAlive ());

	int lB = _blockMoatActionBlock -> setOfLayersActive ();
	int nB = someAlive ? 0 : 1;
	if (nB != lB && nB == 1)
		game () -> sound (__DRAGONWINDTHEREVENGE_OPENDOORSOUNDID__) -> play (__QGAMES_GAMESOUNDCHANNEL__);
	_blockMoatActionBlock -> activeSetOfLayers (nB);
}

// ---
void DragonTheRevenge::CityScene2::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_CITYWORLDSCENE2MAPID__);

	DragonTheRevenge::CityScene::initialize ();
}

// ---
void DragonTheRevenge::CityScene3::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_CITYWORLDSCENE3MAPID__);

	DragonTheRevenge::CityScene::initialize ();

	QGAMES::TiledMap* pM = dynamic_cast <QGAMES::TiledMap*> (activeMap ());
	assert (pM);
	_moveableLayersPerDifficulty [0] = 
		dynamic_cast <QGAMES::PlatformTileLayer*> (pM -> layer (std::string ("Base_7Easy")));
	_moveableLayersPerDifficulty [1] = 
		dynamic_cast <QGAMES::PlatformTileLayer*> (pM -> layer (std::string ("Base_7Medium")));
	_moveableLayersPerDifficulty [2] = 
		dynamic_cast <QGAMES::PlatformTileLayer*> (pM -> layer (std::string ("Base_7Difficult")));
	assert (_moveableLayersPerDifficulty [0] && 
			_moveableLayersPerDifficulty [1] &&
		    _moveableLayersPerDifficulty [2]);
}

// ---
void DragonTheRevenge::CityScene3::updatePositions ()
{
	DragonTheRevenge::CityScene::updatePositions ();

	DRAGONWIND::Game::Conf* cfg = dynamic_cast <DRAGONWIND::Game::Conf*> (game () -> configuration ());
	assert (cfg);

	for (int i = 0; i < 3; i++) 
		_moveableLayersPerDifficulty [i] -> setVisible ((cfg -> difficulty () > 2) 
			? (i == 2) : ((cfg -> difficulty () == i) ? true : false));
}

// ---
void DragonTheRevenge::CityScene3::finalize ()
{
	DragonTheRevenge::CityScene::finalize ();

	_moveableLayersPerDifficulty = QGAMES::AdvancedTileLayers ();
}

// ---
void DragonTheRevenge::CityScene4::initialize ()
{
	setMap (__DRAGONWINDTHEREVENGE_CITYWORLDSCENE4MAPID__);

	DragonTheRevenge::CityScene::initialize ();

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
void DragonTheRevenge::CityScene4::finalize ()
{
	_badGuyEnergyLevel -> setVisible (false);
	_badGuyEnergyLevel -> unObserve (_mainBadGuy);
	_badGuyEnergyLevel = NULL;

	DragonTheRevenge::CityScene::finalize ();
}
