#include "ActionBlolcks.hpp"
#include "Scenes.hpp"
#include "Defs.hpp"

// ---
DRAGONWIND::FlockOfMonsters* DragonTheRevenge::FlyingMonstersActionBlock::flock ()
{
	DRAGONWIND::FlockOfMonsters* result = NULL;

	DRAGONWIND::Game::Conf* cfg = dynamic_cast <DRAGONWIND::Game::Conf*> (game () -> configuration ());
	assert (cfg);
	assert (cfg -> difficulty () < 3); // Easy, Medium, Difficult

	switch (_properties -> _typeOfMonster)
	{
		case __DRAGONWINDTHEREVENGE_ACTIONBLOCKFMONSTERTYPEVULTURE__:
			result = dynamic_cast <DRAGONWIND::FlockOfMonsters*> 
				(game () -> entity (__DRAGONWINDTHEREVENGE_FLOACKOFVULTUREBASE__ + (cfg -> difficulty () << 1) + (rand () % 2)));
			break;

		case __DRAGONWINDTHEREVENGE_ACTIONBLOCKFMONSTERTYPEWASP__:
			result = dynamic_cast <DRAGONWIND::FlockOfMonsters*> 
				(game () -> entity (__DRAGONWINDTHEREVENGE_FLOACKOFWASPBASE__ + (cfg -> difficulty () << 1) + (rand () % 2)));
			break;

		case __DRAGONWINDTHEREVENGE_ACTIONBLOCKFMONSTERTYPEBAT__:
			result = dynamic_cast <DRAGONWIND::FlockOfMonsters*> 
				(game () -> entity (__DRAGONWINDTHEREVENGE_FLOACKOFBATBASE__ + (cfg -> difficulty () << 1) + (rand () % 2)));
			break;

		default:
			result = DRAGONWIND::FlyingMonstersActionBlock::flock ();
			break;
	}

	return (result);
}

// ---
DragonTheRevenge::MoveLiftsCiclicActionBlock::Properties::Properties 
		(const std::map <std::string, std::string>& dt)
	: _lifts ()
{
	if (dt.find (std::string (__DRAGONWINDTHEREVENGE_LIFTACTIONBLOCKLIFTLISTPARAMID__)) != dt.end ())
		_lifts = QGAMES::getElementsFrom 
			((*dt.find (__DRAGONWINDTHEREVENGE_LIFTACTIONBLOCKLIFTLISTPARAMID__)).second.c_str (), ',');
}

// ---
QGAMES::SetOfOpenValues DragonTheRevenge::MoveLiftsCiclicActionBlock::Properties::asSetOfOpenValues () const
{
	std::string lLst (__NULL_STRING__);

	if (!_lifts.empty ())
	{
		lLst = _lifts [0];
		for (int i = 1; i < (int) _lifts.size (); i++)
			lLst += std::string (",") + _lifts [i];
	}

	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));

	result.addOpenValue (0, QGAMES::OpenValue (lLst));

	return (result);
}

// ---
void DragonTheRevenge::MoveLiftsCiclicActionBlock::Properties::fromSetOfOpenValues 
	(const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));
	assert (oV.existOpenValue (0));

	_lifts = QGAMES::getElementsFrom (oV.openValue (0).strValue (), ',');
}

// ---
QGAMES::SetOfOpenValues DragonTheRevenge::MoveLiftsCiclicActionBlock::runtimeValues () const
{
	assert (_properties);

	QGAMES::SetOfOpenValues result = DRAGONWIND::SceneActionBlock::runtimeValues ();

	int lNE = result.lastNestedOpenValueId ();

	result.addSetOfOpenValues (lNE + 1, _properties -> asSetOfOpenValues ());

	// It is not needed to save here the status of the layers
	// because they are already being saved when the status of the scene is stored

	return (result);
}

// ---
void DragonTheRevenge::MoveLiftsCiclicActionBlock::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	assert (_properties);

	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_properties -> fromSetOfOpenValues (cCfg.setOfOpenValues (lNE));
	cCfg.removeSetOfOpenValues (lNE);

	// The status of the layers is not recovered here...
	// It is recovered when the status of its map is got back!

	DRAGONWIND::SceneActionBlock::initializeRuntimeValuesFrom (cCfg);
}

// ---
void DragonTheRevenge::MoveLiftsCiclicActionBlock::setActive (bool a)
{
	DRAGONWIND::SceneActionBlock::setActive (a);

	liftsToMove (a);
}

// ---
void DragonTheRevenge::MoveLiftsCiclicActionBlock::initialize ()
{
	DRAGONWIND::SceneActionBlock::initialize ();

	QGAMES::TiledMap* pM = dynamic_cast <QGAMES::TiledMap*> (scene () -> activeMap ());
	assert (pM); // Just in case...

	for (int i = 0; i < (int) _properties -> _lifts.size (); i++)
	{
		_lifts [i] = 
			dynamic_cast <QGAMES::TileLayer*> (pM -> layer (std::string (_properties -> _lifts [i])));
		assert (_lifts [i]); // Just in case...
	}
}

// ---
void DragonTheRevenge::MoveLiftsCiclicActionBlock::updatePositions ()
{
	DRAGONWIND::SceneActionBlock::updatePositions ();

	// Ideally, this instruction is not need
	// as the lifts only move when the action block is active
	// and only when it is active it is updated!
	if (areLiftsMoving ())
	{
		for (QGAMES::TileLayers::const_iterator i = _lifts.begin (); i != _lifts.end (); i++)
		{
			QGAMES::Layer::SetOfMovementsBehaviour* mBhv = 
				dynamic_cast <QGAMES::Layer::SetOfMovementsBehaviour*> ((*i).second -> movement ());
			assert (mBhv);

			if (mBhv -> hasCurrentMovementFinished ())
				(*i).second -> setPosition (QGAMES::Position::_cero);
		}
	}
}

// ---
void DragonTheRevenge::MoveLiftsCiclicActionBlock::finalize ()
{
	DRAGONWIND::SceneActionBlock::finalize ();

	_lifts = QGAMES::TileLayers ();
}

void DragonTheRevenge::MoveLiftsCiclicActionBlock::liftsToMove (bool m)
{
	for (QGAMES::TileLayers::const_iterator i = _lifts.begin (); i != _lifts.end (); 
			(*i++).second -> setMove (m));
}

bool DragonTheRevenge::MoveLiftsCiclicActionBlock::areLiftsMoving ()
{
	bool result = (_lifts.empty ()) ? false : true;
	for (QGAMES::TileLayers::const_iterator i = _lifts.begin (); i != _lifts.end () && result; 
			result &= (*i++).second -> moves ());
	return (result);
}

// ---
DragonTheRevenge::MoveLiftsMountainsScene2ActionBlock::Properties::Properties 
		(const std::map <std::string, std::string>& dt)
	: DragonTheRevenge::MoveLiftsCiclicActionBlock::Properties (dt),
	  _sToGoBack (__BD 2)
{
	if (dt.find (std::string (__DRAGONWINDTHEREVENGE_MEORLDSSCENE2BLOCKSECPARAMID__)) != dt.end ())
		_sToGoBack = __BD (std::atof ((*dt.find (__DRAGONWINDTHEREVENGE_MEORLDSSCENE2BLOCKSECPARAMID__)).second.c_str ()));
}

// ---
QGAMES::SetOfOpenValues 
	DragonTheRevenge::MoveLiftsMountainsScene2ActionBlock::Properties::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result = 
		DragonTheRevenge::MoveLiftsCiclicActionBlock::Properties::asSetOfOpenValues ();

	int lE = result.lastOpenValueId ();
	result.addOpenValue (lE + 1, QGAMES::OpenValue (_sToGoBack));

	return (result);
}

// ---
void DragonTheRevenge::MoveLiftsMountainsScene2ActionBlock::Properties::fromSetOfOpenValues 
	(const QGAMES::SetOfOpenValues& oV)
{
	int lE = oV.lastOpenValueId ();

	assert (oV.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = oV;

	_sToGoBack = cCfg.openValue (lE).bdataValue ();
	cCfg.removeOpenValue (lE);

	DragonTheRevenge::MoveLiftsCiclicActionBlock::Properties::fromSetOfOpenValues (cCfg);
}

// ---
void DragonTheRevenge::MoveLiftsMountainsScene2ActionBlock::initialize ()
{
	DragonTheRevenge::MoveLiftsCiclicActionBlock::initialize ();

	QGAMES::TiledMap* pM = dynamic_cast <QGAMES::TiledMap*> (scene () -> activeMap ());
	assert (pM); 

	_moveablePlatform = 
		dynamic_cast <QGAMES::TileLayer*> (pM -> layer (std::string ("Base_4"))); // The non ciclic lift
	assert (_moveablePlatform);

	reStartAllCounters ();
	
	// These methods are also invoked at parent level, 
	// but because a new platform to be moved has been considered just above
	// a new invocation is need...
	liftsToMove (initialActive ());
}

// ---
void DragonTheRevenge::MoveLiftsMountainsScene2ActionBlock::finalize ()
{
	DragonTheRevenge::MoveLiftsCiclicActionBlock::finalize ();

	_moveablePlatform = NULL;
}

void DragonTheRevenge::MoveLiftsMountainsScene2ActionBlock::liftsToMove (bool m)
{
	DragonTheRevenge::MoveLiftsCiclicActionBlock::liftsToMove (m);

	if (_moveablePlatform)
		_moveablePlatform -> setMove (m);
}
// ---
bool DragonTheRevenge::MoveLiftsMountainsScene2ActionBlock::areLiftsMoving ()
{
	return (DragonTheRevenge::MoveLiftsCiclicActionBlock::areLiftsMoving () && _moveablePlatform -> moves ());
}

// ---
__IMPLEMENTCOUNTERS__ (DragonTheRevenge::MoveLiftsMountainsScene2ActionBlock::Counters)
{
	addCounter (new QGAMES::Counter 
		(DragonTheRevenge::MoveLiftsMountainsScene2ActionBlock::_COUNTERTOGOBACK, 
			0 /** It will reset at initialize. */, 0, true, true));
}
