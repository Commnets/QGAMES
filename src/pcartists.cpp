#include <PacManLike/pcartists.hpp>
#include <PacManLike/pcmovements.hpp>
#include <PacManLike/pcgame.hpp>
#include <PacManLike/pcworlds.hpp>
#include <PacManLike/pcscenes.hpp>
#include <PacManLike/pcmaps.hpp>

// ---
QGAMES::MazeModel::PositionInMaze PACMAN::PacmanElement::nextXGridPosition (int x) const
{
	return ((pMap () == nullptr) 
		? QGAMES::MazeModel::_noPosition 
		: pMap () -> maze ().nextXGridPositionFollowing (currentMazePosition (), x, orientation (), true));
	// Notice that the orientation is taken into account 
	// and not the direction because the element could be already stopped
}

// ---
int PACMAN::PacmanElement::currentMazeArea () const
{
	return ((pMap () == nullptr) ? -1 : pMap () -> maze ().mazeZoneAt (currentMazePosition ()));
}

// ---
const PACMAN::Map* PACMAN::PacmanElement::pMap () const
{ 
	return (dynamic_cast <const Map*> (map ())); 
}

// ---
PACMAN::Map* PACMAN::PacmanElement::pMap ()
{ 
	return (dynamic_cast <Map*> (map ())); 
}

// ---
QGAMES::MazeModel::PositionInMaze PACMAN::PacmanElement::mapPositionToMazePosition (const QGAMES::Position& p) const
{ 
	return ((pMap () == nullptr) 
		? QGAMES::MazeModel::_noPosition : pMap () -> mapPositionToMazePosition (p));
}

// ---
QGAMES::Position PACMAN::PacmanElement::mazePositionToMapPosition (const QGAMES::MazeModel::PositionInMaze& p) const
{ 
	return ((pMap () == nullptr) 
		? QGAMES::Position::_noPoint : pMap () -> mazePositionToMapPosition (p)); 
}

// ---
bool PACMAN::PacmanElement::doesPositionMatchesTile (const QGAMES::Position& p) const
{ 
	if (pMap () == nullptr)
		return (false);

	QGAMES::Vector c (__BD (visualLength () >> 1), __BD (visualHeight () >> 1), __BD 0);
	return ((p + c) == mazePositionToMapPosition (mapPositionToMazePosition (p + c))); 
}

// ---
PACMAN::Artist::Artist (int cId, const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: PACMAN::PacmanElement (cId, f, d),
	  _lastMazePosition (QGAMES::MazeModel::_noPosition),
	  _pathInMaze (), // No defined by default...
	  _referenceArtists (),
	  _nextDirectionWhenPossible (QGAMES::Vector::_cero),
	  _referenceArtistLastMazePositions (),
	  _frozen (false)
{ 
	// Adds a buoy to be abble to "stop" out of the event manipulation process...
	assert (!buoys ().empty ()); // Just in case...
	buoys ().insert (QGAMES::Buoys::value_type
		(__PACMAN_TOSTOPBUOYID__, new PACMAN::Artist::ToStopBuoy ()));
}

// ---
void PACMAN::Artist::changeDirectionWhenPossibleTo (const QGAMES::Vector& d)
{ 
	// An Artist can change the direction of the movement when it is moving...
	// The change can happen in any moment between two tiles
	// but it is only executed inmediately when the requested new direction is the opposite to the previous one...
	// If it is not, the request is written down to be executed later.
	if (isMoving () && direction () == -d)
		toMove (d);
	else
		_nextDirectionWhenPossible = d;
}

// ---
void PACMAN::Artist::initialize ()
{
	PACMAN::PacmanElement::initialize ();

	_lastMazePosition = QGAMES::MazeModel::_noPosition;

	_pathInMaze = { };

	_referenceArtists = { };

	_nextDirectionWhenPossible = QGAMES::Vector::_cero;

	_referenceArtistLastMazePositions = { };

	_frozen = false;
}

// ---
void PACMAN::Artist::updatePositions ()
{
	if (!isVisible ())
		return;

	QGAMES::MazeModel::PositionInMaze lP = currentMazePosition ();

	if (!_frozen || (_frozen && !isAlive ()))
		PACMAN::PacmanElement::updatePositions ();

	if (currentMazePosition () != lP)
		_lastMazePosition = lP;

	for (auto i : _referenceArtists)
		_referenceArtistLastMazePositions [i -> id ()] = i -> currentMazePosition ();
}

// ---
void PACMAN::Artist::drawOn (QGAMES::Screen* scr, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	PACMAN::PacmanElement::drawOn (scr, p);

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr);

	if (g -> drawArtistTarget ())
	{
		QGAMES::Position posI = mazePositionToMapPosition (currentMazePosition ());
		if (posI != QGAMES::Position::_noPoint)
		{
			QGAMES::Position posT = mazePositionToMapPosition (targetMazePosition ());
			for (int i = 0; i < 3; i++)
				scr -> drawCircle (posT, QGAMES::Vector::_zNormal, __BD (16 - i), __BD (16 - i), trackingColor (), false);
			scr -> drawLine (posI, posT, trackingColor (), 3);
		}
	}

#ifndef NDEBUG
	scr -> drawRectangle (collisionZone (), __QGAMES_YELLOWCOLOR__, false);
#endif
}

// ---
void PACMAN::Artist::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __PACMAN_MOVEMENTDORIENTATIONCHANGED__)
	{
		QGAMES::SetOfOpenValues dC = evnt.values ();
		assert (dC.existOpenValue (0));
		QGAMES::Vector o (dC.openValue (0).strValue ());
		toMove (o);
	}
	
	PACMAN::PacmanElement::processEvent (evnt);
}

// ---
void* PACMAN::Artist::ToStopBuoy::treatFor (QGAMES::Element* e)
{
	PACMAN::Artist* art = dynamic_cast <PACMAN::Artist*> (e);
	assert (art != nullptr);

	art -> toStand ();

	return (this);
}

// ---
void PACMAN::Artist::toStopDeferred ()
{
	PACMAN::Artist::ToStopBuoy* b = 
		dynamic_cast <PACMAN::Artist::ToStopBuoy*> (buoy (__PACMAN_TOSTOPBUOYID__));
	assert (b != nullptr); // Just in case...

	b -> active (true); 
}

// ---
QGAMES::MazeModel::PathInMaze& PACMAN::Artist::recalculatePathInMaze (const QGAMES::Vector& mD)
{ 
	return (_pathInMaze = 
		((pMap () == nullptr) ? QGAMES::MazeModel::PathInMaze ({ QGAMES::MazeModel::_noPosition }) 
							  : pMap () -> maze ().next2StepsToGoTo (currentMazePosition (), targetMazePosition (), { }))); 
}

// ---
bool PACMAN::Artist::isArtistEnteringATunnel () const
{
	return ((pMap () == nullptr) 
		? false : pMap () -> maze ().isTunnelConnectionAt (currentMazePosition ()));
}

// ---
bool PACMAN::Artist::hasReferenceArtistsPositionChanged () const
{
	bool result = false;

	for (std::vector <PACMAN::Artist*>::const_iterator i = _referenceArtists.begin ();
			i != _referenceArtists.end () && !result; i++)
	{
		std::map <int, QGAMES::MazeModel::PositionInMaze>::const_iterator j = 
			_referenceArtistLastMazePositions.find ((*i) -> id ());
		if (j != _referenceArtistLastMazePositions.end () && (*i) -> currentMazePosition () != (*j).second)
			result = true;
	}

	return (result);
}

// ---
void PACMAN::Thing::toDisappear ()
{
	setStatus (PACMAN::Thing::Status::_NOTSHOWN);
}

// ---
void PACMAN::Thing::toStand ()
{
	if (isStanding ())
		return;

	setStatus (PACMAN::Thing::Status::_SHOWN);
}

// ---
void PACMAN::Thing::toMove (const QGAMES::Vector& d)
{
	setOrientation (d);

	if (isMoving ())
		return;

	setStatus (PACMAN::Thing::Status::_SHOWNANDMOVING);
}

// ---
void PACMAN::Thing::toBeEaten ()
{
	if (isMoving () || (!isMoving () && _status == PACMAN::Thing::Status::_EATEN))
		return; // It can be eaten when it doesn't exist or when it is moving...

	setStatus (PACMAN::Thing::Status::_EATEN);
}

// ---
void PACMAN::Thing::initialize ()
{
	PACMAN::Artist::initialize ();

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	setOrientation (QGAMES::Vector::_cero);
	setType (0); // Th default type...
	toDisappear ();
}

// ---
void PACMAN::Thing::updatePositions ()
{
	if (!isVisible ())
		return;

	PACMAN::Artist::updatePositions ();

	if (onOffSwitch (_SWITCHPOINTSVISIBLE) -> isOn ())
	{
		if (counter (_COUNTERPOINTSVISIBLE) -> isEnd ())
			toDisappear ();
	}
}

// ---
void PACMAN::Thing::drawOn (QGAMES::Screen* scr, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	if (onOffSwitch (_SWITCHPOINTSVISIBLE) -> isOn ())
	{
		PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
		assert (g != nullptr);
		QGAMES::ScoreObjectText * txt = 
			g -> textBuilder () -> createText (__PACMAN_NEWTOUR24GREENLETTERS__, std::to_string (_points));
		txt -> drawOn (scr,
			(p == QGAMES::Position::_noPoint) 
				? centerPosition () - 
					QGAMES::Vector (__BD (txt -> visualLength () >> 1), __BD (txt -> visualHeight () >> 1), __BD 0)
				: p);
		delete (txt);
	}
	else
	{
		//  Instead PACMAN::Artist::drawOn (scr, p), to avoid to draw the target, that is random by default...
		PACMAN::PacmanElement::drawOn (scr, p);

		PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
		assert (g != nullptr);

		if (g -> drawArtistTarget ())
		{
			QGAMES::Position posI = mazePositionToMapPosition (currentMazePosition ());
			if (posI != QGAMES::Position::_noPoint)
				for (int i = 0; i < 3; i++)
					scr -> drawCircle (posI, QGAMES::Vector::_zNormal, __BD (16 - i), __BD (16 - i), trackingColor (), false);
		}

#ifndef NDEBUG
			scr -> drawRectangle (collisionZone (), __QGAMES_YELLOWCOLOR__, false);
#endif
	}
}

// ---
void PACMAN::Thing::whenCollisionWith (QGAMES::Entity* e)
{
	if (dynamic_cast <PACMAN::PacMan*> (e) != nullptr && isAlive () && 
		((centerPosition () - e -> centerPosition ()).module () < __BD (visualLength () >> 1)))
	{
		toStand (); // It stands when it is eaten...

		toBeEaten ();
	}
}

// ---
void PACMAN::Thing::setStatus (const PACMAN::Thing::Status& st)
{
	_lastStatus = _status;
	_status = st;

	switch (_status)
	{
		case PACMAN::Thing::Status::_NOTSHOWN:
			{
				setVisible (false);

				onOffSwitch (_SWITCHPOINTSVISIBLE) -> set (false);

				whatToDoWhenStopStatusIsRequested (orientation ());
			}
			break;

		case PACMAN::Thing::Status::_SHOWN:
		case PACMAN::Thing::Status::_SHOWNANDMOVING:
			{
				setVisible (true);

				onOffSwitch (_SWITCHPOINTSVISIBLE) -> set (false);

				if (_status == PACMAN::Thing::Status::_SHOWN)
					whatToDoWhenStopStatusIsRequested (orientation ());
				else
					whatToDoWhenMovementStatusIsRequested (orientation ());
			}

			break;

		case PACMAN::Thing::Status::_EATEN:
			{
				setVisible (true);

				onOffSwitch (_SWITCHPOINTSVISIBLE) -> set (true);

				counter (_COUNTERPOINTSVISIBLE) -> initialize ();

				whatToDoWhenStopStatusIsRequested (orientation ());
			}

			break;

		default:
			assert (false);
	}

	_status = st;
}

// ---
QGAMES::MazeModel::PositionInMaze PACMAN::Thing::targetMazePosition () const
{ 
	QGAMES::MazeModel::PositionInMaze result = currentMazePosition ();

	if (isMoving ())
	{
		int ct = 0;
		while ((result = pMap () -> randomMazePosition ({ })) == currentMazePosition () && ct++ < 5); 
		// It might stop the game. This is th reason to include a max number of tries...
		assert (result != currentMazePosition ());
		// Just for dbug reasons...
	}

	return (result);
} 

// ---
void PACMAN::Thing::whatToDoWhenStopStatusIsRequested (const QGAMES::Vector& d)
{
	setOrientation (d);
	setStateToStandLookingTo (orientation ()); // Looking at the last direction...
	setMove (QGAMES::Vector::_cero); // ...but not moving... but never lossing the orientation
	_pathInMaze = { }; // ..and with no path to follow
	changeDirectionWhenPossibleTo (QGAMES::Vector::_cero); // ...finally it stops!
}

// ---
void PACMAN::Thing::whatToDoWhenMovementStatusIsRequested (const QGAMES::Vector& d)
{
	setOrientation (d); // Look to..
	setStateToMoveTo (orientation ()); // ..with the right aspect...
	setMove (orientation ()); // ...and moving also towards there...
	recalculatePathInMaze (); // ...and with a new path
}

// ---
QGAMES::MazeModel::PathInMaze& PACMAN::Thing::recalculatePathInMaze (const QGAMES::Vector& mD)
{
	if (pMap () == nullptr)
		return (_pathInMaze = { QGAMES::MazeModel::_noPosition });

	QGAMES::MazeModel::PositionInMaze cMP = currentMazePosition ();

	// Imposible to reverese the movement...
	std::vector <QGAMES::Vector> fD = { -orientation () }; 

	// When the thing is alive only
	if (isAlive ())
	{
		// The thing cannot enter monster's home:
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
	}

	// There can be duplicate elements in fD...it doesn't matter

	return (_pathInMaze = pMap () -> maze ().next2StepsToGoTo (cMP, targetMazePosition (), fD, mD)); 
}

// ---
__IMPLEMENTCOUNTERS__ (PACMAN::Thing::Counters)
{
	addCounter (new QGAMES::Counter 
		(_COUNTERPOINTSVISIBLE, 1 /** second max for points. */ * QGAMES::Game::game () -> framesPerSecond (), true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (PACMAN::Thing::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHPOINTSVISIBLE, false));
}
