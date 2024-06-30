#include <PacManLike/pcmonsters.hpp>
#include <PacManLike/pcmovements.hpp>
#include <PacManLike/pcgame.hpp>
#include <PacManLike/pcmaps.hpp>

// ---
PACMAN::Monster::Monster (int cId, int mN, const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: PACMAN::Artist (cId, f, d),
	  _monsterNumber (mN),
	  _points (100), // By default...
	  _status (PACMAN::Monster::Status::_NOTDEFINED),
	  _lastStatus (PACMAN::Monster::Status::_NOTDEFINED),
	  _elroyCondition (-1)
{ 
	assert (_monsterNumber >= 0);

	// Adds a buoy to be able to "chase" out of the process event manipulation...
	assert (!buoys ().empty ()); // Just in case...
	buoys ().insert (QGAMES::Buoys::value_type
		(__PACMAN_TOCHASEBUOYID__, new PACMAN::Monster::ToChaseBuoy ()));
}

// ---
bool PACMAN::Monster::isEnemy (const PACMAN::PacmanElement* elmnt) const
{ 
	return (dynamic_cast <const PACMAN::PacMan*> (elmnt) != nullptr); 
}

// ---
bool PACMAN::Monster::isAtHome () const
{
	return ((pMap () == nullptr) 
		? false : pMap () -> maze ().mazeZoneAt (currentMazePosition ()) == __PACMAN_MAZEMONSTERSHOMEZONE__);
}

// ---
bool PACMAN::Monster::isInATunnelHall () const
{
	return ((pMap () == nullptr) 
		? false : pMap () -> maze ().mazeZoneAt (currentMazePosition ()) == __PACMAN_MAZETUNELLPATHZONE__);
}

// ---
QGAMES::Vector PACMAN::Monster::directionToStartMovement () const
{
	if (pMap () == nullptr)
		return (QGAMES::Vector::_noPoint);

	QGAMES::MazeModel::PositionInMaze tP = QGAMES::MazeModel::_noPosition;
	if (isAtHome ())
	{
		assert (status () == PACMAN::Monster::Status::_ATHOME); // Impossible to be in another ststus...

		tP = pMap () -> monsterExitingHomePosition ();
	}
	else
		tP = (status () == PACMAN::Monster::Status::_ATHOME) // Blinky e.g is in this status but "out of home"
			? runAwayMazePosition () : targetMazePosition ();

	assert (currentMazePosition () != QGAMES::MazeModel::_noPosition && 
			tP != QGAMES::MazeModel::_noPosition);

	QGAMES::MazeModel::PathInMaze way = pMap () -> maze ().next2StepsToGoTo (currentMazePosition (), tP, { });

	assert (way.size () > 1);

	return (way [1].asVector () - way [0].asVector ());
}

// ---
void PACMAN::Monster::toStand ()
{
	// The orientation is not changed ever...
	if (!isStanding ())
		setStatus (PACMAN::Monster::Status::_ATHOME);
	else
		setStatus (status ());
}

// ---
void PACMAN::Monster::toMove (const QGAMES::Vector& d)
{
	// The orientation is changed always...
	setOrientation (d);

	if (!isMoving ())
	{
		if (isAtHome ())
			setStatus (PACMAN::Monster::Status::_EXITINGHOME);
		else
			setStatus (PACMAN::Monster::Status::_RUNNINGWAY);
	}
	else
		setStatus (status ()); // Keeps the same...
}

// ---
void PACMAN::Monster::toChase (bool f)
{
	if (isStanding () || !isAlive ())
		return; // When standing, it is at home, so impossible to be in this state!
	{
		if (f)
		{
			if (status () == PACMAN::Monster::Status::_RUNNINGWAY ||
				(status () == PACMAN::Monster::Status::_EXITINGHOME && !isAtHome ()))
				setStatus (PACMAN::Monster::Status::_CHASING);
		}
		else
		{
			if (status () == PACMAN::Monster::Status::_CHASING ||
				(status () == PACMAN::Monster::Status::_EXITINGHOME && !isAtHome ()))
				setStatus (PACMAN::Monster::Status::_RUNNINGWAY);
		}
	}
}

// ---
void PACMAN::Monster::toChaseDeferred (bool o)
{
	PACMAN::Monster::ToChaseBuoy* b = 
		dynamic_cast <PACMAN::Monster::ToChaseBuoy*> (buoy (__PACMAN_TOCHASEBUOYID__));
	assert (b != nullptr); // Just in case...

	b -> setChasing (o);

	b -> active (true); 
}

// ---
void PACMAN::Monster::toBeThreaten (bool f)
{
	if (isStanding () && !isAlive ())
		return; // When standing, it is at home, so impossible to be affected by this action!
	else
	{
		if (f)
		{
			// To be threaten is possible when moving through the maze...
			if (status () == PACMAN::Monster::Status::_CHASING ||
				status () == PACMAN::Monster::Status::_RUNNINGWAY)
				setStatus (PACMAN::Monster::Status::_TOBEEATEN); // Notice that the orientation is not modifyed!
		}
		else
		{
			// And when the monster is not being threaten is chasing pacman!
			if (status () == PACMAN::Monster::Status::_TOBEEATEN)
				setStatus (PACMAN::Monster::Status::_CHASING); // The orientation is not modified either!
		}
	}
}

// ---
void PACMAN::Monster::toBlink (bool b, int bp)
{
	if (_status != PACMAN::Monster::Status::_TOBEEATEN)
		return;

	if (b)
	{
		if (!onOffSwitch (_SWITCHBLINKACTIVE) -> isOn ())
		{
			onOffSwitch (_SWITCHBLINKACTIVE) -> set (true);
			onOffSwitch (_SWITCHBLINKSITUATION) -> set (true);

			counter (_COUNTERBLINKSITUATION) -> initialize (bp, 0, true, true);

			setStatus (status ());
		}
	}
	else
	{
		if (onOffSwitch (_SWITCHBLINKACTIVE) -> isOn ())
			onOffSwitch (_SWITCHBLINKACTIVE) -> set (false);
	}
}

// ---
bool PACMAN::Monster::canReverseMovement () const
{
	return (
		(
			(_lastStatus != _status) &&
		    (
				(_lastStatus == PACMAN::Monster::Status::_ATHOME && _status == PACMAN::Monster::Status::_EXITINGHOME) ||
				(_lastStatus == PACMAN::Monster::Status::_ATHOME && _status == PACMAN::Monster::Status::_RUNNINGWAY) ||
				(_lastStatus == PACMAN::Monster::Status::_ATHOME && _status == PACMAN::Monster::Status::_CHASING) ||
				(_lastStatus == PACMAN::Monster::Status::_EXITINGHOME && _status == PACMAN::Monster::Status::_RUNNINGWAY) ||
				(_lastStatus == PACMAN::Monster::Status::_EXITINGHOME && _status == PACMAN::Monster::Status::_CHASING) ||
				(_lastStatus == PACMAN::Monster::Status::_CHASING && _status == PACMAN::Monster::Status::_RUNNINGWAY) ||
				(_lastStatus == PACMAN::Monster::Status::_RUNNINGWAY && _status == PACMAN::Monster::Status::_CHASING) ||
				(_lastStatus == PACMAN::Monster::Status::_CHASING && _status == PACMAN::Monster::Status::_TOBEEATEN) ||
				(_lastStatus == PACMAN::Monster::Status::_RUNNINGWAY && _status == PACMAN::Monster::Status::_TOBEEATEN)
			)
		));
}

// ---
void PACMAN::Monster::initialize ()
{
	PACMAN::Artist::initialize ();

	_lastStatus = PACMAN::Monster::Status::_NOTDEFINED;

	_points = 100; // It has to be set froum outside...

	// Always at home initially...
	// The position is set by the scene up!
	setOrientation (QGAMES::Vector (__BD 0, __BD -1, __BD 0));
	setStatus (PACMAN::Monster::Status::_NOTDEFINED);

	_elroyCondition = -1; // No condition at the beginning...

	reStartAllCounters ();
	reStartAllOnOffSwitches ();
}

// ---
void PACMAN::Monster::updatePositions ()
{
	if (!isVisible ())
		return;

	PACMAN::Artist::updatePositions ();

	if (isInATunnelHall ())
		adaptSpeed ();

	if (onOffSwitch (_SWITCHBLINKACTIVE) -> isOn () &&
		counter (_COUNTERBLINKSITUATION) -> isEnd ())
	{
		onOffSwitch (_SWITCHBLINKSITUATION) -> change ();
		setStatus (status ()); // Just to blink...
	}
}

// ---
void PACMAN::Monster::finalize ()
{
	PACMAN::Artist::finalize ();

	// When finalize, the monster can be used in other places, so it has to stop blinking...
	onOffSwitch (_SWITCHBLINKACTIVE) -> set (false);
	onOffSwitch (_SWITCHBLINKSITUATION) -> set (false);
}

// ---
void PACMAN::Monster::processEvent (const QGAMES::Event& evnt)
{
	// The monster reaches a limitation in the movement
	// (if the maze is well designed) either when arrives at home (after being eaten) or after exiting home
	if (evnt.code () == __PACMAN_LIMITMOVEMENTREACHED__)
	{
		// After being eaten, everything starts back (see the block controlling the monster)
		// Stopping the monsteer, it will be understood it is at home!
		if (status () == PACMAN::Monster::Status::_BEINGEATEN)
			toStopDeferred ();
		// At any other circunstance, the movement can't stop, so continue the movement...
		else
			notify (QGAMES::Event (evnt.code (), this));
	}

	PACMAN::Artist::processEvent (evnt);
}

// ---
void PACMAN::Monster::whenCollisionWith (QGAMES::Entity* e)
{
	PACMAN::PacmanElement* pE = dynamic_cast <PACMAN::PacmanElement*> (e);
	if (e == nullptr)
		return;

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr);

	if (pE -> isAlive ())
	{
		if (isAlive () && isEnemy (pE))
		{
			PACMAN::PacMan* pcm = dynamic_cast <PACMAN::PacMan*> (pE);
			if (pcm != nullptr && 
				isNearOf (pE, __BD (visualLength () >> 1)))
			{
				if (!isDangerous ())
				{
					setStatus (PACMAN::Monster::Status::_BEINGEATEN);

					onOffSwitch (_SWITCHBLINKACTIVE) -> set (false);
				}
			}
		}
	}
}

// ---
__IMPLEMENTCOUNTERS__ (PACMAN::Monster::Counters)
{
	addCounter (new QGAMES::Counter (_COUNTERBLINKSITUATION , 1 /** Initilized later. */, true, false));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (PACMAN::Monster::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHBLINKACTIVE, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHBLINKSITUATION, false));
}

// ---
void PACMAN::Monster::setStatus (const PACMAN::Monster::Status& st)
{
	// In release compilation version all changes are possible,
	// but under debug mode the right combination is ckecked first!

	_lastStatus = _status;
	_status = st;

	switch (st)
	{
		case PACMAN::Monster::Status::_NOTDEFINED:
		case PACMAN::Monster::Status::_ATHOME:
			whatToDoWhenStopStatusIsRequested (orientation ());
			break;

		case PACMAN::Monster::Status::_EXITINGHOME:
		case PACMAN::Monster::Status::_CHASING:
		case PACMAN::Monster::Status::_RUNNINGWAY:
		case PACMAN::Monster::Status::_TOBEEATEN:
		case PACMAN::Monster::Status::_BEINGEATEN:
			whatToDoWhenMovementStatusIsRequested (orientation ());
			break;

		default:
			assert (false); // It shouldn't be heerem but just in case...
	}
}

// ---
void PACMAN::Monster::whatToDoWhenStopStatusIsRequested (const QGAMES::Vector& d)
{
	setOrientation (d);
	setStateToStandLookingTo (orientation ()); // Looking at the last direction...
	setMove (orientation ()); // ...but not moving... but never lossing the orientation
	_pathInMaze = { }; // ..and with no path to follow
	changeDirectionWhenPossibleTo (QGAMES::Vector::_cero); // ...finally it stops!
}

// ---
void PACMAN::Monster::whatToDoWhenMovementStatusIsRequested (const QGAMES::Vector& d)
{
	setOrientation (d); // Look to..
	setStateToMoveTo (orientation ()); // ..with the right aspect...
	setMove (orientation ()); // ...and moving also towards there...
	QGAMES::Vector pD = 
		((isMoving () && _status != PACMAN::Monster::Status::_EXITINGHOME) && 
			(_lastStatus == PACMAN::Monster::Status::_EXITINGHOME || _lastStatus == PACMAN::Monster::Status::_ATHOME))
			? preferredDirectionAtStartingToMove () : QGAMES::Vector::_noPoint;
	recalculatePathInMaze (pD); // ...and with a new path
	adaptSpeed (); // That depends on many many things...
}

// ---
void* PACMAN::Monster::ToChaseBuoy::treatFor (QGAMES::Element* e)
{
	PACMAN::Monster* mter = dynamic_cast <PACMAN::Monster*> (e);
	assert (mter != nullptr);

	mter -> toChase (_chasing);

	return (this);
}

// ---
QGAMES::MazeModel::PathInMaze& PACMAN::Monster::recalculatePathInMaze (const QGAMES::Vector& mD)
{
	if (pMap () == nullptr)
		return (_pathInMaze = { QGAMES::MazeModel::_noPosition });

	QGAMES::MazeModel::PositionInMaze cMP = currentMazePosition ();

	// When status changes to reverse th direction can be allowed in certain circunstances
	std::vector <QGAMES::Vector> fD = { }; 
	if (!canReverseMovement ()) fD = { -orientation () }; 
	_lastStatus = _status;

	// When the monster is alive...
	if (isAlive ())
	{
		// The monster cannot enter back home:
		// So, calculate whether the monster is or not at the entry of home
		// and if it is, what is the direction to enter, because it has to be avoided unless it is going back home...
		QGAMES::Vector dirMonstersHome = QGAMES::Vector::_noPoint; // By default it doesn't exit!
		if (cMP == pMap () -> monsterExitingHomePosition ())
		{
			int sz = 0;
			if ((sz = (int) pMap () -> maze ().positionsForZone (__PACMAN_MAZEMONSTERSHOMEZONE__).size ()) != 0)
			{
				QGAMES::MazeModel::PathInMaze pE = pMap () -> maze ().next2StepsToGoTo 
					(cMP, pMap () -> maze ().positionsForZone (__PACMAN_MAZEMONSTERSHOMEZONE__)[sz >> 1], { });
				if ((int) pE.size () > 1)
					dirMonstersHome = pE [1].asVector () - pE [0].asVector ();
			}

			if (dirMonstersHome != QGAMES::Vector::_noPoint) 
				fD.push_back (dirMonstersHome);
		}

		// ...and also there could be some restricted directions (when passing through an area of these)
		std::vector <QGAMES::Vector> rD = pMap () -> maze ().restrictedDirectionsAt (cMP);
		fD.insert (fD.end (), rD.begin (), rD.end ());
	}

	// There can be duplicate elements in fD...it doesn't matter

	return (_pathInMaze = pMap () -> maze ().next2StepsToGoTo (cMP, targetMazePosition (), fD, mD)); 
}

// ---
QGAMES::MazeModel::PositionInMaze PACMAN::Monster::runAwayMazePosition () const
{
	return ((pMap () == nullptr) 
		? QGAMES::MazeModel::_noPosition : pMap () -> monsterRunAwayPosition (monsterNumber ())); 
}

// ---
void PACMAN::StandardMonster::setReferenceArtists (const std::vector <PACMAN::Artist*>& r)
{
	_referenceArtists = { nullptr }; 
	for (auto i : r)
		if (dynamic_cast <PACMAN::PacMan*> (i) != nullptr) _referenceArtists [0] = i;

	assert (_referenceArtists [0] != nullptr);
}

// ---
void PACMAN::StandardMonster::updatePositions ()
{
	PACMAN::Monster::updatePositions ();

	// TODO...
}

// ---
QGAMES::MazeModel::PositionInMaze PACMAN::StandardMonster::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	if (pMap () == nullptr)
		return (result);

	switch (status ())
	{
		case PACMAN::Monster::Status::_NOTDEFINED:
		case PACMAN::Monster::Status::_CHASING:
			assert (false); // Not possible at this point (_CHASING will depend on the type of monster)
	
		case PACMAN::Monster::Status::_ATHOME:
			result = currentMazePosition (); 
			// Don't move (to a different tile i mean. Probably the could move up down a little bit)
			break;

		case PACMAN::Monster::Status::_EXITINGHOME:
			result = pMap () -> monsterExitingHomePosition (); // The same for all...
			break;

		case PACMAN::Monster::Status::_RUNNINGWAY:
		case PACMAN::Monster::Status::_TOBEEATEN:
			result = runAwayMazePosition ();
			break;

		case PACMAN::Monster::Status::_BEINGEATEN:
			result = pMap () -> monsterReturningPositionAfterDieing  ();
			break;

		// Chasing status will deepend on the specific monster...
	}

	return (result);
}

// ---
void PACMAN::StandardMonster::setStateToStandLookingTo (const QGAMES::Vector& d)
{
	switch (status ())
	{
		case PACMAN::Monster::Status::_NOTDEFINED:
		case PACMAN::Monster::Status::_ATHOME:
			{
				if (d == QGAMES::Vector (__BD 1, __BD 0, __BD 0))
					setCurrentState (__PACMAN_MONSTERSTATEATHOMELOOKINGRIGHT__, true);
				else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
					setCurrentState (__PACMAN_MONSTERSTATEATHOMELOOKINGLEFT__, true);
				else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMAN_MONSTERSTATEATHOMELOOKINGUP__, true);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMAN_MONSTERSTATEATHOMELOOKINGDOWN__, true);
				else
					assert (false); // No state possible fot other direction...
			}

			break;

		default:
			// To stand is not possible in other status than athome...
			assert (false);
	}
}

// ---
void PACMAN::StandardMonster::setStateToMoveTo (const QGAMES::Vector& d)
{
	switch (status ())
	{
		// _ATHOME is considered "standing"

		case PACMAN::Monster::Status::_EXITINGHOME:
		case PACMAN::Monster::Status::_CHASING:
		case PACMAN::Monster::Status::_RUNNINGWAY:
			{
				if (d == QGAMES::Vector (__BD 1, __BD 0, __BD 0))
					setCurrentState (__PACMAN_MONSTERSTATEMOVINGANDLOOKINGRIGHT__, true);
				else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
					setCurrentState (__PACMAN_MONSTERSTATEMOVINGANDLOOKINGLEFT__, true);
				else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
					setCurrentState (__PACMAN_MONSTERSTATEMOVINGANDLOOKINGUP__, true);
				else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
					setCurrentState (__PACMAN_MONSTERSTATEMOVINGANDLOOKINGDOWN__, true);
				else
					assert (false); // No state possible for other direction...
			}

			break;

		case PACMAN::Monster::Status::_TOBEEATEN:
			{
				if (onOffSwitch (_SWITCHBLINKACTIVE) -> isOn () && onOffSwitch (_SWITCHBLINKSITUATION) -> isOn ())
					setCurrentState (__PACMAN_MONSTERSTATERUNNINGAWAYBLINK__, true);
				else
					setCurrentState (__PACMAN_MONSTERSTATERUNNINGAWAY__, true);
			}

			break;

		case PACMAN::Monster::Status::_BEINGEATEN:
			setCurrentState (__PACMAN_MONSTERSTATEEATEN__, true);
			break;

		default:
			assert (false);
	}
}

// ---
void PACMAN::StandardMonster::adaptSpeed ()
{
	if (!isMoving ())
		return;

	PACMAN::Game* pG = dynamic_cast <PACMAN::Game*> (game ());
	assert (pG != nullptr); // Just in case...
	PACMAN::MazeMovement* mM = dynamic_cast <PACMAN::MazeMovement*> (currentMovement ());
	assert (mM != nullptr); // is moving?

	// To choose the right speed is key in games pacman-type
	// There are many different possibilities, depending on where the monster is in: 
	// Not the same in a tunnel or at hom than in other positions of the maze, and
	// not the same chasing or running away than be threaten by pacman!
	const PACMAN::DataGame::LevelDefinition& lD = pG -> dataGame ().levelDefinition (pG -> level ());
	if (!isAlive ())
		mM -> setSpeed (__BD lD.ghostSpeed () * __BD 2);
	else
	if (isInATunnelHall ())
		mM -> setSpeed (__BD lD.ghostSpeedWhenCrossingTunnel ());
	else
	if (elroyCondition () != -1 && status () != PACMAN::Monster::Status::_TOBEEATEN) // Elroy condition only applies in normal state
		mM -> setSpeed (__BD lD.elroyCondition (elroyCondition ()).ghostSpeed (_monsterNumber));
	else 
		mM -> setSpeed 
			(status () == PACMAN::Monster::Status::_TOBEEATEN
				? __BD lD.ghostSpeedWhenBeingFrighten () 
				: ((status () == PACMAN::Monster::Status::_EXITINGHOME && isAtHome ()) 
						? __BD lD.ghostSpeedWhenExitingHome () : __BD lD.ghostSpeed ()));
}

// ---
void PACMAN::Blinky::setElroyCondition (int eC)
{
	if (eC != -1 && isAlive () && status () != PACMAN::Monster::Status::_TOBEEATEN)
	{
		_elroyCondition = eC;
		adaptSpeed ();
	}
}

// ---
QGAMES::MazeModel::PositionInMaze PACMAN::Blinky::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	switch (status ())
	{
		case PACMAN::Monster::Status::_NOTDEFINED:
			result = currentMazePosition ();
			break;

		case PACMAN::Monster::Status::_CHASING:
			if (_referenceArtists [0] != nullptr) // It shouldn't, but who knows...
			{
				if ((result = _referenceArtists [0] -> currentMazePosition ()) == currentMazePosition ())
					result = nextXGridPosition (1);
			}
			// When there is no references enough...go home!
			else
				result = runAwayMazePosition (); 
			break;

		// The target position for the rest of the status is calculated like the standard one...
		default:
			result = PACMAN::StandardMonster::targetMazePosition ();
	}

	return (result);
}

// ---
QGAMES::MazeModel::PositionInMaze PACMAN::Pinky::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	switch (status ())
	{
		case PACMAN::Monster::Status::_NOTDEFINED:
			result = currentMazePosition ();
			break;

		case PACMAN::Monster::Status::_CHASING:
			if (_referenceArtists [0] != nullptr) // It shouldn't happen ver, but who knows!
			{
				// In the original pac man, the position taken as reference was 4 tiles ahead from pacman and not 2...	
				// But in this version tiles are twice than in the original one, so they are considered 2 instead of 4!
				if ((result = _referenceArtists [0] -> nextXGridPosition (2)) == currentMazePosition ())
					result = nextXGridPosition (1);
			}
			// When there is no references enough...go home!
			else
				result = runAwayMazePosition (); 
			break;

		// The target position for the rest of the status is calculated like the standard one...
		default:
			result = PACMAN::StandardMonster::targetMazePosition ();
	}

	return (result);
}

// ---
void PACMAN::Inky::setReferenceArtists (const std::vector <PACMAN::Artist*>& r)
{
	_referenceArtists = { nullptr /** The pacman. */, nullptr /** Blinky */ }; 
	for (auto i : r)
	{
		if (dynamic_cast <PACMAN::PacMan*> (i) != nullptr) _referenceArtists [0] = i;
		else if (dynamic_cast <PACMAN::Blinky*> (i) != nullptr) _referenceArtists [1] = i;
	}

	assert (_referenceArtists [0] != nullptr && _referenceArtists [1] != nullptr);
}

// ---
QGAMES::MazeModel::PositionInMaze PACMAN::Inky::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	switch (status ())
	{
		case PACMAN::Monster::Status::_NOTDEFINED:
			result = currentMazePosition ();
			break;

		case PACMAN::Monster::Status::_CHASING:
			if (_referenceArtists [0] != nullptr && 
				_referenceArtists [1] != nullptr) // It shouldn't happen ever, but who knows!
			{
				// In the original pac man, the position taken as reference was 2 tiles ahead from pacman and not 1...	
				// But in this version tiles are twice than in the original one, so they are considered 1 instead of 4!
				if ((result = QGAMES::MazeModel::PositionInMaze (
						_referenceArtists [1] -> currentMazePosition ().asVector () + 
							(__BD 2 * (_referenceArtists [0] -> nextXGridPosition (1).asVector () -
								  _referenceArtists [1] -> currentMazePosition ().asVector ())))) == currentMazePosition ())
					result = nextXGridPosition (1);
			}
			// When there is no references enough...go home!
			else
				result = runAwayMazePosition (); 

			break;

		// The target position for the rest of the status is calculated like the standard one...
		default:
			result = PACMAN::StandardMonster::targetMazePosition ();
	}

	return (result);
}

// ---
QGAMES::MazeModel::PositionInMaze PACMAN::Clyde::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	switch (status ())
	{
		case PACMAN::Monster::Status::_NOTDEFINED:
			result = currentMazePosition ();
			break;

		case PACMAN::Monster::Status::_CHASING:
			if (_referenceArtists [0] != nullptr) // It shouldn't happen vr, but who knows!
			{
				// In the original pac man, the distancee considered was 8 tiles instead of 4,
				// but as in this version tiles are doubl sized than in the original one, 4 has been choseen!.
				if ((_referenceArtists [0] -> currentMazePosition ().asVector () - 
					 currentMazePosition ().asVector ()).module () > __BD 4)
				{
					if ((result = _referenceArtists [0] -> currentMazePosition ()) == currentMazePosition ())
						result = nextXGridPosition (1);
				}
				else
					result = runAwayMazePosition ();
			}
			// When there is no references enough...go home!
			else
				result = runAwayMazePosition (); 
			break;

		// The target position for the rest of the status is calculated like the standard one...
		default:
			result = PACMAN::StandardMonster::targetMazePosition ();
	}

	return (result);
}
