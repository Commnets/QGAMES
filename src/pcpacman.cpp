#include <PacManLike/pcpacman.hpp>
#include <PacManLike/pcmovements.hpp>
#include <PacManLike/pcgame.hpp>
#include <PacManLike/pcmaps.hpp>

// ---
PACMAN::PacMan::PacMan (int cId, const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: PACMAN::Artist (cId, f, d),
	  _alive (true),
	  _score (0),
	  _status (PACMAN::PacMan::Status::_NOTDEFINED),
	  _lastStatus (PACMAN::PacMan::Status::_NOTDEFINED),
	  _hasEaten (false),
	  _lastMulScoreNotified (0)
{
	// Adds a buoy to be able to "chase" out of the process event manipulation...
	assert (!buoys ().empty ()); // Just in case...
	buoys ().insert (QGAMES::Buoys::value_type
		(__PACMAN_TOCHASEBUOYID__, new PACMAN::PacMan::ToChaseBuoy ()));
}

// ---
void PACMAN::PacMan::toStand ()
{
	// The orientation is not changed...
	if (!isStanding ())
		setStatus (PACMAN::PacMan::Status::_STOPPED);
}

// ---
void PACMAN::PacMan::toMove (const QGAMES::Vector& d)
{
	// The orientation is changed always...
	setOrientation (d);

	if (!isMoving ())
		setStatus (PACMAN::PacMan::Status::_MOVING);
	else
		setStatus (status ()); // Keeps the same...
}

// ---
void PACMAN::PacMan::toChase (bool c)
{
	if (isMoving ())
		setStatus (c ? PACMAN::PacMan::Status::_CHASING : PACMAN::PacMan::Status::_MOVING);
}

// ---
void PACMAN::PacMan::toChaseDeferred (bool o)
{
	PACMAN::PacMan::ToChaseBuoy* b = 
		dynamic_cast <PACMAN::PacMan::ToChaseBuoy*> (buoy (__PACMAN_TOCHASEBUOYID__));
	assert (b != nullptr); // Just in case...

	b -> setChasing (o);

	b -> active (true); 
}

// ---
bool PACMAN::PacMan::isEnemy (const PACMAN::PacmanElement* elmnt) const
{ 
	return (dynamic_cast <const PACMAN::Monster*> (elmnt) != nullptr); 
}

// ---
void PACMAN::PacMan::setScore (int s) 
{
	_score = s; 

	QGAMES::SetOfOpenValues oV;
	oV.addOpenValue (__PACMAN_SCOREVALUEPARAMETER__, QGAMES::OpenValue (_score));
	notify (QGAMES::Event (__PACMAN_SCOREACTUALIZED__, this, oV));

	int mS = 0;
	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr);
	if ((mS = _score / g -> dataGame ().everyToGetAnExtraLive ()) > _lastMulScoreNotified) 
	{
		_lastMulScoreNotified = mS;

		notify (QGAMES::Event (__PACMAN_PACMANPOINTSHIGHLIGHTREACHED__, this, oV));
	}

	g -> setScore (_score); // Keep it...
}

// ---
void PACMAN::PacMan::setFruitsEaten (const std::map <int, bool>& fE)
{
	_fruitsEaten = fE;

	std::string dt;
	for (auto i : _fruitsEaten)
		if (i.second)
			dt += std::string (",") + std::to_string (i.first);
	if (dt.length () > 1)
		dt = dt.substr (1); // To remove the first element, that it would be a comma...

	QGAMES::SetOfOpenValues oV;
	oV.addOpenValue (__PACMAN_FRUITSCARRIEDVALUEPARAMETER__, QGAMES::OpenValue (dt));
	notify (QGAMES::Event (__PACMAN_FRUITSCARRIEDACTUALIZED__, this, oV));

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr);
	g -> setFruitsEaten (_fruitsEaten);
}

// ---
void PACMAN::PacMan::initialize ()
{
	PACMAN::Artist::initialize ();

	setOrientation (QGAMES::Vector (__BD 1, __BD 0, __BD 0));
	toStand ();

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr);

	_score = g -> score ();
	_lastMulScoreNotified = _score / g -> dataGame ().everyToGetAnExtraLive ();

	// To be notified...it can't be done before to avoid adding not valid extra lives!
	setScore (_score); 
	setFruitsEaten (g -> fruitsEaten ());
}

// ---
void PACMAN::PacMan::updatePositions ()
{
	if (!isVisible ())
		return;

	PACMAN::Artist::updatePositions ();

	if (doesPositionMatchesTile (position ()))
	{
		if (isStanding ())
		{
			if (_nextDirectionWhenPossible != QGAMES::Vector::_cero)
				toMove (_nextDirectionWhenPossible);
		}
	}
}

// ---
void PACMAN::PacMan::finalize ()
{
	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr);
	g -> setScore (_score);
	g -> setFruitsEaten (_fruitsEaten);

	PACMAN::Artist::finalize ();
}

// ---
void PACMAN::PacMan::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMAN_LIMITMOVEMENTREACHED__)
		toStopDeferred ();

	PACMAN::Artist::processEvent (evnt);
}

// ---
void PACMAN::PacMan::whenCollisionWith (QGAMES::Entity* e)
{
	PACMAN::PacmanElement* pE = dynamic_cast <PACMAN::PacmanElement*> (e);
	if (e == nullptr)
		return;

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr);

	if (pE -> isAlive ())
	{
		if (isEnemy (pE))
		{
			PACMAN::Monster* mter = dynamic_cast <PACMAN::Monster*> (pE);
			if (mter != nullptr && 
				isNearOf (pE, __BD (visualLength () >> 1)))
			{
				if (mter -> isDangerous ())
				{
					if (!g -> passwordWellIntroduced ()) // Unlimited lives = password well introduced
						notify (QGAMES::Event (__PACMAN_PACMANDESTROYED__, this));
				}
				else
				{
					game () -> soundBuilder () -> sound (__PACMAN_SOUNDEATGHOST__) -> play (-1);

					// The points will be added in the scene obsrving pacman
					// This is because the number of points to add could depend on (e.g.) the number of eatn monsters.
					notify (QGAMES::Event (__PACMAN_PACMANEATMONSTER__, mter));
				}
			}
		}
		else
		{
			PACMAN::Fruit* frt = dynamic_cast <PACMAN::Fruit*> (pE);
			if (frt != nullptr && 
				isNearOf (frt, __BD (visualLength () >> 1)))
			{
				setScore (score () + frt -> points ());

				addFruitEaten (frt -> type ());

				game () -> soundBuilder () -> sound (__PACMAN_SOUNDEATFRUIT__) -> play (-1);

				QGAMES::Event (__PACMAN_PACMANEATFRUIT__, frt);
			}
		}
		
	}
}

// ---
void PACMAN::PacMan::setStatus (const PACMAN::PacMan::Status& st)
{
	// In the case of pacman all changes among types of status are allowed
	// It is not the case in Monster...
	
	_lastStatus = st;
	_status = st;
	
	switch (st)
	{
		case PACMAN::PacMan::Status::_NOTDEFINED:
		case PACMAN::PacMan::Status::_STOPPED:
			whatToDoWhenStopStatusIsRequested (orientation ());
			break;

		case PACMAN::PacMan::Status::_MOVING:
		case PACMAN::PacMan::Status::_CHASING:
			whatToDoWhenMovementStatusIsRequested (orientation ());
			break;

		default:
			assert (false); // It shouldn't be here, but just in case...
	}
}

// ---
QGAMES::MazeModel::PositionInMaze PACMAN::PacMan::targetMazePosition () const
{
	if (pMap () == nullptr)
		return (QGAMES::MazeModel::_noPosition);

	if (isStanding ())
		return (currentMazePosition ());

	QGAMES::Vector dir = direction (); // It is 0 when no moving...
	if (isMoving () && 
		_nextDirectionWhenPossible != QGAMES::Vector::_cero && 
		pMap () -> maze ().isPossibleToMoveTo (currentMazePosition (), _nextDirectionWhenPossible))
		dir = _nextDirectionWhenPossible;
	QGAMES::MazeModel::PathInMaze pth = pMap () -> maze ().limitPathFromFollowing (currentMazePosition (), dir);
	assert (!pth.empty ());
	return (pth [(int) pth.size () - 1]);
}

// ---
void PACMAN::PacMan::whatToDoWhenStopStatusIsRequested (const QGAMES::Vector& d)
{
	setOrientation (d);
	setStateToStandLookingTo (orientation ()); // Looking to the last direction...
	setMove (orientation ()); // ...but not moving... (although oriented to the last position)
	_pathInMaze = { }; // ..and with no path to follow
	changeDirectionWhenPossibleTo (QGAMES::Vector::_cero); // ...finally it stops!
}

// ---
void PACMAN::PacMan::whatToDoWhenMovementStatusIsRequested (const QGAMES::Vector& d)
{
	setOrientation (d); // Look to..
	setStateToMoveTo (orientation ()); // ..with the right aspect...
	setMove (orientation ()); // ...and moving also towards there...
	recalculatePathInMaze (); // ...and with a new path
	adaptSpeed (); // That depends on many many things...
}

// ---
void PACMAN::PacMan::whatToDoOnCurrentPosition ()
{
	PACMAN::TilePath* t = dynamic_cast <PACMAN::TilePath*> 
		(pMap () -> tileAt (position (), pMap () -> mazeLayer ()));
	if (t == nullptr)
		return;

	if (t -> canBeEaten ())
	{
		if (!t -> alreadyEaten ())
		{
			PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
			assert (g != nullptr);

			t -> eaten (true); // Shoots whatever is needed in the pacman an others....

			game () -> sound (__PACMAN_SOUNDCHOMP__) -> play (__QGAMES_MAINCHARACTERSOUNDCHANNEL__); // Chomp...

			_hasEaten = true;

			adaptSpeed ();

			if (t -> hasPower ())
				toChaseDeferred (true);

			setScore (score () + 
				(t -> hasPower () 
					? g -> dataGame ().levelDefinition (g -> level ()).pointsPowerBall ()
					: g -> dataGame ().levelDefinition (g -> level ()).pointsBall ()));
		}
		else
		{
			_hasEaten = false;

			adaptSpeed ();
		}
	}
}

// ---
void PACMAN::PacMan::setStateToStandLookingTo (const QGAMES::Vector& d)
{
	if (d == QGAMES::Vector (__BD 1, __BD 0, __BD 0))
		setCurrentState (__PACMAN_PACMANSTATESTANDLOOKINGRIGHT__, true);
	else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
		setCurrentState (__PACMAN_PACMANSTATESTANDLOOKINGLEFT__, true);
	else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
		setCurrentState (__PACMAN_PACMANSTATESTANDLOOKINGUP__, true);
	else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
		setCurrentState (__PACMAN_PACMANSTATESTANDLOOKINGDOWN__, true);
	else
		assert (false); // No other orientations are possible..
}

// ---
void PACMAN::PacMan::setStateToMoveTo (const QGAMES::Vector& d)
{
	if (d == QGAMES::Vector (__BD 1, __BD 0, __BD 0))
		setCurrentState (__PACMAN_PACMANSTATEEATINGLOOKINGRIGHT__, true);
	else if (d == QGAMES::Vector (__BD -1, __BD 0, __BD 0))
		setCurrentState (__PACMAN_PACMANSTATEEATINGLOOKINGLEFT__, true);
	else if (d == QGAMES::Vector (__BD 0, __BD -1, __BD 0))
		setCurrentState (__PACMAN_PACMANSTATEEATINGLOOKINGUP__, true);
	else if (d == QGAMES::Vector (__BD 0, __BD 1, __BD 0))
		setCurrentState (__PACMAN_PACMANSTATEEATINGLOOKINGDOWN__, true);
	else
		assert (false); // No other orientations are possible...
}

// ---
void* PACMAN::PacMan::ToChaseBuoy::treatFor (QGAMES::Element* e)
{
	PACMAN::PacMan* pcm = dynamic_cast <PACMAN::PacMan*> (e);
	assert (pcm != nullptr);

	pcm -> toChase (_chasing);

	return (this);
}

// ---
void PACMAN::PacMan::adaptSpeed ()
{
	if (!isMoving ())
		return;

	PACMAN::Game* pG = dynamic_cast <PACMAN::Game*> (game ());
	assert (pG != nullptr); // Just in case...
	PACMAN::MazeMovement* mM = dynamic_cast <PACMAN::MazeMovement*> (currentMovement ());
	assert (mM != nullptr); // is moving?
	const PACMAN::DataGame::LevelDefinition& lD = pG -> dataGame ().levelDefinition (pG -> level ());
	mM -> setSpeed (_status == PACMAN::PacMan::Status::_CHASING 
		? (_hasEaten ? __BD lD.pacmanSpeedWhenEatingFrightingDots () : __BD lD.pacmanSpeedWhenFrighting ()) 
		: (_hasEaten ? __BD lD.pacmanSpeedWhenEatingDots () : __BD lD.pacmanSpeed ()));
}

// ---
QGAMES::MazeModel::PathInMaze& PACMAN::PacMan::recalculatePathInMaze (const QGAMES::Vector& mD)
{
	if (pMap () == nullptr)
		return (_pathInMaze = { QGAMES::MazeModel::_noPosition });

	// The mandatory direction is not taken into account ever

	// Calculate whether the pacman is or not at the entry of monster's home
	// and if it is, what is the direction to enter, because it has to be avoided ever
	QGAMES::Vector dirMonstersHome = QGAMES::Vector::_noPoint; // By default it doesn't exit!
	if ((currentMazePosition () == pMap () -> monsterExitingHomePosition ()))
	{
		int sz = 0;
		if ((sz = (int) pMap () -> maze ().positionsForZone (__PACMAN_MAZEMONSTERSHOMEZONE__).size ()) != 0)
		{
			QGAMES::MazeModel::PathInMaze pE = pMap () -> maze ().next2StepsToGoTo 
				(currentMazePosition (), pMap () -> maze ().positionsForZone (__PACMAN_MAZEMONSTERSHOMEZONE__)[sz >> 1], { });
			if ((int) pE.size () > 1)
				dirMonstersHome = pE [1].asVector () - pE [0].asVector ();
		}
	}

	QGAMES::Vector dir = direction ();
	if (isMoving () && 
		_nextDirectionWhenPossible != QGAMES::Vector::_cero &&
		(dirMonstersHome == QGAMES::Vector::_noPoint || 
		 (dirMonstersHome != QGAMES::Vector::_noPoint && _nextDirectionWhenPossible != dirMonstersHome)) &&
		pMap () -> maze ().isPossibleToMoveTo (currentMazePosition (), _nextDirectionWhenPossible))
		dir = _nextDirectionWhenPossible;

	QGAMES::MazeModel::PathInMaze pth = pMap () -> maze ().limitPathFromFollowing (currentMazePosition (), dir);

	int i = 0;
	_pathInMaze = { currentMazePosition () };
	if (pth.size () > 1)
		for (QGAMES::MazeModel::PathInMaze::const_iterator j = ++pth.begin (); j != pth.end () && i++ < 5; j++)
			_pathInMaze.push_back ((*j)); // 5 positions in advance (if possible) for pacman...

	return (_pathInMaze);
}
