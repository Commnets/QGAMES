#include <PacManLike/pcsceneactionblocks.hpp>
#include <PacManLike/pcgame.hpp>
#include <PacManLike/pcworlds.hpp>
#include <PacManLike/pcscenes.hpp>
#include <PacManLike/pcmaps.hpp>
#include <PacManLike/pcmovements.hpp>

// ---
const PACMAN::Scene* PACMAN::SceneActionBlock::pScene () const
{
	return (dynamic_cast <const PACMAN::Scene*> (scene ()));
}

// ---
PACMAN::Scene* PACMAN::SceneActionBlock::pScene ()
{
	return (dynamic_cast <PACMAN::Scene*> (scene ()));
}

// ---
const PACMAN::Map* PACMAN::SceneActionBlock::pMap () const
{
	return (pScene () == nullptr 
		? nullptr : dynamic_cast <const PACMAN::Map*> (pScene () -> activeMap ()));
}

// --
PACMAN::Map* PACMAN::SceneActionBlock::pMap ()
{
	return (pScene () == nullptr 
		? nullptr : dynamic_cast <PACMAN::Map*> (pScene () -> activeMap ()));
}

// ---
PACMAN::ThingSceneActionBlock::Properties::Properties (const QGAMES::SceneActionBlockProperties& prps)
	: _entityId (0),
	  _types ({ 0 }), _points ({ 200 }),
	  _ballsEatenToAppear({ 40 }), _secondsToDisappear ({ __BD 10 }),
	  _speeds ({ 0.0 }),
	  _positionsId ({ -1 })
{
	if (prps.find (std::string (__PACMAN_THINGSCENEBLOCKENTITYIDATTR__)) != prps.end ())
		_entityId = std::atoi ((*prps.find (std::string (__PACMAN_THINGSCENEBLOCKENTITYIDATTR__))).second.c_str ());
	if (prps.find (std::string (__PACMAN_THINGSCENEBLOCKTYPEATTR__)) != prps.end ())
		_types = QGAMES::getElementsFromAsInt 
			(std::string ((*prps.find (std::string (__PACMAN_THINGSCENEBLOCKTYPEATTR__))).second.c_str ()), ',');
	if (prps.find (std::string (__PACMAN_THINGSCENEBLOCKPOINTSATTR__)) != prps.end ())
		_points = QGAMES::getElementsFromAsInt
			(std::string ((*prps.find (std::string (__PACMAN_THINGSCENEBLOCKPOINTSATTR__))).second.c_str ()), ',');
	if (prps.find (std::string (__PACMAN_THINGSCENEBLOCKSECONDSTOAPPEARATTR__)) != prps.end ())
		_ballsEatenToAppear = QGAMES::getElementsFromAsInt 
			(std::string ((*prps.find (std::string (__PACMAN_THINGSCENEBLOCKSECONDSTOAPPEARATTR__))).second.c_str ()), ',');
	if (prps.find (std::string (__PACMAN_THINGSCENEBLOCKSECONDSTODISAPPEARATTR__)) != prps.end ())
		_secondsToDisappear = QGAMES::getElementsFromAsBDATA
			(std::string ((*prps.find (std::string (__PACMAN_THINGSCENEBLOCKSECONDSTODISAPPEARATTR__))).second.c_str ()), ',');
	if (prps.find (std::string (__PACMAN_THINGSCENEBLOCKSPEEDATTR__)) != prps.end ())
		_speeds = QGAMES::getElementsFromAsBDATA
			(std::string ((*prps.find (std::string (__PACMAN_THINGSCENEBLOCKSPEEDATTR__))).second.c_str ()), ',');
	if (prps.find (std::string (__PACMAN_THINGSCENEBLOCKPOSITIONIDATTR__)) != prps.end ())
		_positionsId = QGAMES::getElementsFromAsInt
			(std::string ((*prps.find (std::string (__PACMAN_THINGSCENEBLOCKPOSITIONIDATTR__))).second.c_str ()), ',');

	PACMAN::ThingSceneActionBlock::Properties (_entityId, _types, _points, _ballsEatenToAppear, 
		_secondsToDisappear, _speeds, _positionsId);
}

// ---
QGAMES::SetOfOpenValues PACMAN::ThingSceneActionBlock::runtimeValues () const
{
	QGAMES::SetOfOpenValues result;

	// counters and onOffSwitches will return the object already created that at this point could be nullptr
	result.addOpenValue (0, (onOffSwitches () == nullptr) 
		? -1 : (QGAMES::OpenValue (onOffSwitch (_SWITCHBLOCKACTIVE) -> isOn () ? 1 : 0)));
	result.addOpenValue (1, (counters () == nullptr) 
		? -1 : QGAMES::OpenValue (counter (_COUNTERNUMBERTHING) -> value ()));
	// The number of thing has to be saved to avoid this to appear twice in several rounds!

	return (result);
}

// ---
void PACMAN::ThingSceneActionBlock::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	assert (cfg.existOpenValue (0) && cfg.existOpenValue (1));

	if (cfg.openValue (0).intValue () != -1)
		onOffSwitch (_SWITCHBLOCKACTIVE) -> set ((cfg.openValue (0).intValue () == 1) ? true : false);
	if (cfg.openValue (1).intValue () != -1)
		counter (_COUNTERNUMBERTHING) -> setValue (cfg.openValue (1).intValue ());
}

// ---
void PACMAN::ThingSceneActionBlock::initialize ()
{
	PACMAN::SceneActionBlock::initialize ();

	_thing = dynamic_cast <PACMAN::Thing*> (game () -> character (_properties._entityId));
	assert (_thing != nullptr);

	scene () -> addCharacter (_thing);

	PACMAN::Map* aM = dynamic_cast <PACMAN::Map*> (scene () -> activeMap ());
	assert (aM != nullptr);
	_thing -> setMap (aM);

	_thing -> initialize ();

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	assert (!_properties._types.empty ()); // It shouldn't happen ever (it is verifid at construction time), but who knows!
	counter (_COUNTERNUMBERTHING) -> initialize ((int) _properties._types.size (), 0, true, false); 
}

// ---
void PACMAN::ThingSceneActionBlock::updatePositions ()
{
	PACMAN::SceneActionBlock::updatePositions ();

	PACMAN::Scene* scn = pScene ();
	assert (scn != nullptr);
	PACMAN::Map* mp = pMap ();
	assert (mp != nullptr);

	if (!scn -> clapperBoard ())
		return;

	if (!onOffSwitch (_SWITCHBLOCKACTIVE) -> isOn ())
		return;

	if (onOffSwitch (_SWITCHTHINGONSCENE) -> isOn ())
	{
		bool tD = false;
		if (_thing -> isAlive ()) // It could be aten whilst it is on the screen...
		{
			if (counter (_COUNTERTODISAPPEARTHING) -> isEnd ())
			{
				_thing -> toDisappear ();

				tD = true;
			}
		}
		else
			tD = true;

		if (tD)
		{
			onOffSwitch (_SWITCHTHINGONSCENE) -> set (false);

			onOffSwitch (_SWITCHBLOCKACTIVE) -> set (!counter (_COUNTERNUMBERTHING) -> isEnd ());
		}
	}
	else
	{
		if (scn -> numberBallsEaten () >= 
			_properties._ballsEatenToAppear [counter (_COUNTERNUMBERTHING) -> value ()]) 
		{
			PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
			assert (g != nullptr);

			_thing -> setType (_properties._types [counter (_COUNTERNUMBERTHING) -> value ()]);

			_thing -> setPoints (_properties._points [counter (_COUNTERNUMBERTHING) -> value ()]);

			// The initial positiojn of the fruit will deepend of what has ben defined in the properties
			// If a -1 number has ben defined, it will mean random!
			int pId = _properties._positionsId [counter (_COUNTERNUMBERTHING) -> value ()];
			_thing -> setPositionFromMazePosition 
				(pId == -1 ? mp -> randomMazePosition ({ __PACMAN_MAZEMONSTERSHOMEZONE__ }) : mp -> thingPosition (pId));

			QGAMES::bdata spd = _properties._speeds [counter (_COUNTERNUMBERTHING) -> value ()];
			if (spd == __BD 0)
				_thing -> toStand (); 
			else
			{
				_thing -> toMove (QGAMES::Vector::_cero); // A thing has no direction to look at?...

				PACMAN::MazeMovement* m = dynamic_cast <PACMAN::MazeMovement*> (_thing -> currentMovement ());
				assert (m != nullptr);
				m -> setSpeed (spd);
			}

			if (initialMazePosition () != QGAMES::MazeModel::_noPosition) // Only if something has been de
				_thing -> setPositionFromMazePosition (initialMazePosition ());

			counter (_COUNTERTODISAPPEARTHING) -> initialize ((int) 
				(_properties._secondsToDisappear [counter (_COUNTERNUMBERTHING) -> value ()] * 
					__BD g -> framesPerSecond ()), 0, true, false);

			onOffSwitch (_SWITCHTHINGONSCENE) -> set (true);
		}
	}
}

//
void PACMAN::ThingSceneActionBlock::finalize ()
{
	PACMAN::SceneActionBlock::finalize ();

	_thing -> setMap (nullptr);

	scene () -> removeCharacter (_thing);

	_thing = nullptr; 
}

// ---
__IMPLEMENTCOUNTERS__ (PACMAN::ThingSceneActionBlock::Counters)
{
	addCounter (new QGAMES::Counter 
		(PACMAN::ThingSceneActionBlock::_COUNTERNUMBERTHING, 1, 0 , true, false));
	addCounter (new QGAMES::Counter 
		(PACMAN::ThingSceneActionBlock::_COUNTERTODISAPPEARTHING, 1, 0, true, false)); 
	// All initialized later better...
}

// ---
__IMPLEMENTONOFFSWITCHES__ (PACMAN::ThingSceneActionBlock::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(PACMAN::ThingSceneActionBlock::_SWITCHBLOCKACTIVE, true));
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(PACMAN::ThingSceneActionBlock::_SWITCHTHINGONSCENE, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(PACMAN::ThingSceneActionBlock::_SWITCHTHINGSHOWN, false));
}
