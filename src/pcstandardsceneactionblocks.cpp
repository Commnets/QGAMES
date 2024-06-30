#include <PacManLike/pcstandardsceneactionblocks.hpp>
#include <PacManLike/pcgame.hpp>
#include <PacManLike/pcworlds.hpp>
#include <PacManLike/pcscenes.hpp>
#include <PacManLike/pcmaps.hpp>

// ---
PACMAN::MonsterSceneActionBlock::Properties::Properties (const QGAMES::SceneActionBlockProperties& prps)
	: _initializeElement (true), _entityId (0), _numberInMap (0), _points (0)
{
	if (prps.find (std::string (__PACMAN_MONSTERSCENEBLOCKLOOKPOSITIONATTR__)) != prps.end ())
	{
		std::string r ((*prps.find (std::string (__PACMAN_MONSTERSCENEBLOCKLOOKPOSITIONATTR__))).second.c_str ()); 
		_initializeElement = QGAMES::toUpper (r) == std::string (__YES_STRING__);
	}

	if (prps.find (std::string (__PACMAN_MONSTERSCENEBLOCKENTITYIDATTR__)) != prps.end ())
		_entityId = std::atoi ((*prps.find (std::string (__PACMAN_MONSTERSCENEBLOCKENTITYIDATTR__))).second.c_str ());
	if (prps.find (std::string (__PACMAN_MONSTERSCENEBLOCKNUMBEERMONSTERATTR__)) != prps.end ())
		_numberInMap = std::atoi ((*prps.find (std::string (__PACMAN_MONSTERSCENEBLOCKNUMBEERMONSTERATTR__))).second.c_str ());
	if (prps.find (std::string (__PACMAN_MONSTERSCENEBLOCKPOINTSATTR__)) != prps.end ())
		_points = std::atoi ((*prps.find (std::string (__PACMAN_MONSTERSCENEBLOCKPOINTSATTR__))).second.c_str ());

	PACMAN::MonsterSceneActionBlock::Properties (_initializeElement, _entityId, _numberInMap, _points);
}

// ---
bool PACMAN::MonsterSceneActionBlock::timeToStart (int nM)
{
	assert (_monster != nullptr);

	bool result = false;
	if (_monster -> monsterNumber () == nM && !onOffSwitch (_SWITCHTOSTART) -> isOn ())
		onOffSwitch (_SWITCHTOSTART) -> set (result = true);

	return (result);
}

// ---
void PACMAN::MonsterSceneActionBlock::initialize ()
{
	PACMAN::SceneActionBlock::initialize ();

	_monster = dynamic_cast <PACMAN::Monster*> (game () -> character (_properties._entityId));
	assert (_monster != nullptr);

	scene () -> addCharacter (_monster);

	PACMAN::Map* aM = dynamic_cast <PACMAN::Map*> (scene () -> activeMap ());
	assert (aM != nullptr);
	_monster -> setMap (aM);

	// The element can be initialized from outside...
	if (_properties._initializeElement)
	{
		_monster -> initialize ();

		_monster -> setPositionFromMazePosition (aM -> monsterInitialPosition (_properties._numberInMap));
	}

	_monster -> setVisible (true);

	_monster -> setOrientation (QGAMES::Vector (__BD 0, __BD -1, __BD 0));
	_monster -> toStand (); // The block starts looking up!

	observe (_monster);

	_monster -> setPoints (_properties._points);

	reStartAllOnOffSwitches ();

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr);
}

// ---
void PACMAN::MonsterSceneActionBlock::updatePositions ()
{
	PACMAN::SceneActionBlock::updatePositions ();

	PACMAN::Scene* scn = pScene ();
	assert (scn != nullptr);

	if (!scn -> clapperBoard ())
		return;

	assert (_monster != nullptr);

	if (onOffSwitch (_SWITCHMOVING) -> isOn ())
	{
		if (_monster -> status () == PACMAN::Monster::Status::_ATHOME)
			onOffSwitch (_SWITCHMOVING) -> set (false);
		else
			elronCondition ();
	}
	else
	{
		if (_monster -> isAtHome ())
		{
			// There could be intrnal conditions to push monster to start the movement..
			if (!onOffSwitch (_SWITCHTOSTART) -> isOn () && readyToStart ())
				onOffSwitch (_SWITCHTOSTART) -> set (true);

			if (onOffSwitch (_SWITCHTOSTART) -> isOn ())
			{
				_monster -> toMove (_monster -> directionToStartMovement ()); // Never chase...

				onOffSwitch (_SWITCHMOVING) -> set (true);
			}
		}
		else
		{
			_monster -> toMove (_monster -> directionToStartMovement ()); // Never chase...
			if (scn -> chasingMode () == true) 
				_monster -> toChase (true);

			onOffSwitch (_SWITCHTOSTART) -> set (true);
			onOffSwitch (_SWITCHMOVING) -> set (true);
		}

		if (onOffSwitch (_SWITCHMOVING) -> isOn ())
			notify (QGAMES::Event (__PACMAN_MONSTERSTARTEDTOMOVE__, this));
	}
}

//
void PACMAN::MonsterSceneActionBlock::finalize ()
{
	PACMAN::SceneActionBlock::finalize ();

	unObserve (_monster);

	_monster -> setMap (nullptr);

	scene () -> removeCharacter (_monster);

	_monster = nullptr; 
}

// ---
void PACMAN::MonsterSceneActionBlock::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMAN_LIMITMOVEMENTREACHED__)
	{
		assert (evnt.data () == static_cast <void*> (_monster));

		PACMAN::Scene* scn = pScene ();
		assert (scn != nullptr);

		// Never stops!
		// In demo modes and unwantd stop could happen when pacman position is reached
		_monster -> toChaseDeferred (scn -> chasingMode ());
	}

	PACMAN::SceneActionBlock::processEvent (evnt);
}

// ---
bool PACMAN::MonsterSceneActionBlock::readyToStart ()
{
	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr);
	PACMAN::Scene* scn = pScene ();
	assert (scn != nullptr);

	return (scn -> numberBallsEatenRound () >= g -> dataGame ().levelDefinition 
		(g -> level ()).leaveHomeCondition (scn -> numberRound ()).dotsToLeaveFor (_monster -> monsterNumber ()));
}

// ---
bool PACMAN::MonsterSceneActionBlock::elronCondition ()
{
	if (!_monster -> hasElroyPossibility ())
		return (true); // No needed...

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr);
	PACMAN::Scene* scn = pScene ();
	assert (scn != nullptr);

	PACMAN::DataGame::LevelDefinition::ElroyCondition eC = 
		g -> levelDefinition (g -> level ()).elroyCondition (scn -> numberRound ());

	bool result = false;

	if (_monster -> elroyCondition () == -1) // Not set still...
	{
		if ((scn -> maxNumberBallsToEat () - scn -> numberBallsEaten ()) <= eC.numberBallLeft (_monster -> monsterNumber ()))
		{
			_monster -> setElroyCondition (scn -> numberRound ()); // If it is accepted it won't ever more -1

			result = true;
		}
	}

	if (_monster -> elroyCondition () != -1) // Already set...
	{
		if (_monster -> isAlive () && 
			!_monster -> isChasing () && eC.chasingWhenScatter (_monster -> monsterNumber ()))
		{
			_monster -> toChase (true);

			result = true;
		}
	}

	return (result);
}

// ---
__IMPLEMENTONOFFSWITCHES__ (PACMAN::MonsterSceneActionBlock::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (PACMAN::MonsterSceneActionBlock::_SWITCHTOSTART, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch (PACMAN::MonsterSceneActionBlock::_SWITCHMOVING, false));
}
