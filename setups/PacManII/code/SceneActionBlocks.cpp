#include "SceneActionBlocks.hpp"
#include "Monsters.hpp"
#include "Defs.hpp"

const int PacManII::ElementToAppearSceneActionBlock::_FADE [14] =
	{ 0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 255 };

// ---
PacManII::WormyMonsterSceneActionBlock::Properties::Properties (const QGAMES::SceneActionBlockProperties& prps)
	: _monsterProperties (prps),
	  _trailLength (12)
{
	if (prps.find (std::string (__PACMANII_WORMYSCENEBLOCKTRACELENGTHATTR__)) != prps.end ())
		_trailLength = std::atoi ((*prps.find (std::string (__PACMANII_WORMYSCENEBLOCKTRACELENGTHATTR__))).second.c_str ());

	PacManII::WormyMonsterSceneActionBlock::Properties (_monsterProperties, _trailLength);
}

// ---
void PacManII::WormyMonsterSceneActionBlock::initialize ()
{
	PACMAN::MonsterSceneActionBlock::initialize ();

	assert (dynamic_cast <PacManII::Wormy*> (_monster) != nullptr);
	static_cast <PacManII::Wormy*> (_monster) -> setTrailLength (_properties._trailLength);
}

// ---
PacManII::ElementToAppearSceneActionBlock::Properties::Properties (const QGAMES::SceneActionBlockProperties& prps)
	: _entityId (0),
	  _position (QGAMES::MazeModel::_noPosition),
	  _ballsEatenToAppear (40),
	  _secondsMaxToAppear (__BD 2.0),
	  _blockToControlElement (-1)
{
	if (prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKENTITYIDATTR__)) != prps.end ())
		_entityId = std::atoi ((*prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKENTITYIDATTR__))).second.c_str ());
	if (prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKPOSITIONATTR__)) != prps.end ())
		_position = QGAMES::MazeModel::PositionInMaze 
			(QGAMES::Vector ((*prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKPOSITIONATTR__))).second.c_str ()));
	if (_position == QGAMES::MazeModel::PositionInMaze (-1, -1, 0))
		_position = QGAMES::MazeModel::_noPosition;
	if (prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKBALLSEATENTOAPPEARATTR__)) != prps.end ())
		_ballsEatenToAppear = std::atoi ((*prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKBALLSEATENTOAPPEARATTR__))).second.c_str ());
	if (prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKSECONDSTOAPPEARATTR__)) != prps.end ())
		_secondsMaxToAppear = __BD std::atof ((*prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKSECONDSTOAPPEARATTR__))).second.c_str ());
	if (prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKBLOCKTOACTIVATEATTR__)) != prps.end ())
		_ballsEatenToAppear = std::atoi ((*prps.find (std::string (__PACMANII_ELMNTAPPRSCENEBLOCKBLOCKTOACTIVATEATTR__))).second.c_str ());

	PacManII::ElementToAppearSceneActionBlock::Properties 
		(_entityId, _position, _ballsEatenToAppear, _secondsMaxToAppear, _blockToControlElement);
}

// ---
QGAMES::SetOfOpenValues PacManII::ElementToAppearSceneActionBlock::runtimeValues () const
{
	QGAMES::SetOfOpenValues result;

	// counters and onOffSwitches will return the object already created that at this point could be nullptr
	result.addOpenValue (0, (onOffSwitches () == nullptr)
		? -1 : (QGAMES::OpenValue (onOffSwitch (_SWITCHAPPEARED) -> isOn () ? 1 : 0)));
	// The number of cycle has to be saved to avoid the monster to appear twice in th same conditions!

	return (result);
}

// ---
void PacManII::ElementToAppearSceneActionBlock::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	assert (cfg.existOpenValue (0));

	if (cfg.openValue (0).intValue () != -1)
		onOffSwitch (_SWITCHAPPEARED) -> set ((cfg.openValue (0).intValue () == 1) ? true : false);
	if (_properties._blockToControlElement != -1) // It could belong to a non active block element...
		scene () -> actionBlock (_properties._blockToControlElement) -> setActive (onOffSwitch (_SWITCHAPPEARED) -> isOn ());
}

// ---
void PacManII::ElementToAppearSceneActionBlock::initialize ()
{
	PACMAN::SceneActionBlock::initialize ();

	assert (scene () -> actionBlock (_properties._blockToControlElement) != nullptr);

	scene () -> actionBlock (_properties._blockToControlElement) -> setActive (false);

	_element = dynamic_cast <PACMAN::PacmanElement*> (game () -> character (_properties._entityId));
	assert (_element != nullptr);

	scene () -> addCharacter (_element);

	PACMAN::Map* aM = dynamic_cast <PACMAN::Map*> (scene () -> activeMap ());
	assert (aM != nullptr);
	_element -> setMap (aM);

	_element -> initialize ();

	_element -> setVisible (false);

	PACMAN::Scene* scn = pScene ();
	assert (scn != nullptr);
	_element -> setPositionFromMazePosition 
		((_properties._position == QGAMES::MazeModel::_noPosition) 
			? scn -> randomMazePosition ({ __PACMAN_MAZEMONSTERSHOMEZONE__ }) : _properties._position);

	observe (_element);

	observe (scn -> activeMap ());

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	scene () -> actionBlock (_properties._blockToControlElement) -> setActive (false);

	counter (_COUNTERTOAPPEAR) -> initialize 
		((int) (_properties._secondsMaxToAppear * __BD game () -> framesPerSecond ()), 0, true, false);
}

// ---
void PacManII::ElementToAppearSceneActionBlock::updatePositions ()
{
	PACMAN::SceneActionBlock::updatePositions ();

	// If the monster has already appeared...
	// the control has to be hand over to the other action block...
	// so there is nothing to do here...
	if (onOffSwitch (_SWITCHAPPEARED) -> isOn ())
		return;

	PACMAN::Scene* scn = dynamic_cast <PACMAN::Scene*> (scene ());
	assert (scn != nullptr);

	// If it is not appearing...
	if (!onOffSwitch (_SWITCHAPPEARING) -> isOn ())
	{
		// ...controls whether it is time to do so...
		// And if it is, starts from transparent status!
		if (counter (_COUNTERTOAPPEAR) -> isEnd () || 
			scn -> numberBallsEaten () >= _properties._ballsEatenToAppear)
		{
			_element -> setVisible (true);

			_element -> setAlphaLevel (_FADE [0]); // The very basic level of fade...

			onOffSwitch (_SWITCHAPPEARING) -> set (true);
		}
	}
	else
	{
		if (counter (_COUNTERTOFADE) -> isEnd ())
		{
			if (counter (_COUNTERFADE) -> isEnd ())
			{
				// Just in case...
				assert (scene () -> actionBlock (_properties._blockToControlElement) != nullptr);

				scene () -> actionBlock (_properties._blockToControlElement) -> setActive (true);

				scene () -> actionBlock (_properties._blockToControlElement) -> initialize (); // It is needed again...

				onOffSwitch (_SWITCHAPPEARING) -> set (false); // No longer...

				onOffSwitch (_SWITCHAPPEARED) -> set (true);
			}
			else
				_element -> setAlphaLevel (_FADE [counter (_COUNTERFADE) -> value ()]);
		}
	}
}

// ---
void PacManII::ElementToAppearSceneActionBlock::finalize ()
{
	PACMAN::SceneActionBlock::finalize ();

	if (scene () -> actionBlock (_properties._blockToControlElement) != nullptr)
		scene () -> actionBlock (_properties._blockToControlElement) -> setActive (false);
	// If could be already removed from the list for the scene controlling the block...

	unObserve (_element);

	unObserve (scene () -> activeMap ());

	_element -> setMap (nullptr);

	if (scene () -> existsCharacter (_element-> id ()))
		scene () -> removeCharacter (_element); 
	// It could also be already removed by the controled block!
	// Because both are pointing to the same element...

	_element = nullptr;
}

// ---
void PacManII::ElementToAppearSceneActionBlock::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMAN_BALLEATEN__)
		counter (_COUNTERTOAPPEAR) -> initialize (); // Starts back to count the max seconds to appear...

	PACMAN::SceneActionBlock::processEvent (evnt);
}

// ---			
__IMPLEMENTCOUNTERS__ (PacManII::ElementToAppearSceneActionBlock::Counters)
{
	addCounter (new QGAMES::Counter (_COUNTERTOAPPEAR, 1 /** Better later. */, 0, true, false));
	addCounter (new QGAMES::Counter (_COUNTERTOFADE, 1, 0, true, true));
	addCounter (new QGAMES::Counter (_COUNTERFADE, 
		sizeof (PacManII::ElementToAppearSceneActionBlock::_FADE) / sizeof (int), 0, true, false));
}

__IMPLEMENTONOFFSWITCHES__ (PacManII::ElementToAppearSceneActionBlock::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHAPPEARING, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHAPPEARED, false));
}
