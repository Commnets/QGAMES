#include "Monsters.hpp"

// ---
void PacManII::Wormy::setTrailLength (int l)
{
	assert (l >= 0);

	_trailPositions.resize (l);
	if (_trailLength < l) // In other case the farest positions will be lost!
		for (int i = _trailLength; i < l; i++)
			_trailPositions [i] = QGAMES::Position::_noPoint;

	_trailLength = l;
}

// ---
void PacManII::Wormy::setReferenceArtists (const std::vector <PACMAN::Artist*>& r)
{
	_referenceArtists = { nullptr /** The pacman. */, nullptr /** Clyde */ }; 
	for (auto i : r)
	{
		if (dynamic_cast <PACMAN::PacMan*> (i) != nullptr) _referenceArtists [0] = i;
		else if (dynamic_cast <PACMAN::Clyde*> (i) != nullptr) _referenceArtists [1] = i;
	}

	assert (_referenceArtists [0] != nullptr && _referenceArtists [1] != nullptr);
}

// ---
void PacManII::Wormy::initialize ()
{
	PACMAN::StandardMonster::initialize ();

	_trailPositions = QGAMES::Positions (_trailLength, QGAMES::Position::_noPoint);
}

// ---
void PacManII::Wormy::updatePositions ()
{
	if (!isVisible ())
		return;

	QGAMES::Position p = position ();

	PACMAN::StandardMonster::updatePositions ();

	for (int i = (_trailLength - 2); i >= 0; i --)
		_trailPositions [i + 1] = _trailPositions [i];
	_trailPositions [0] = p;
}

// ---
void PacManII::Wormy::drawOn (QGAMES::Screen* scr, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	auto tileRectangleFor = [=](const QGAMES::Position& p, int i) -> QGAMES::Rectangle
		{ 
			QGAMES::Rectangle result = QGAMES::Rectangle 
				(p, p + QGAMES::Vector (__BD pMap () -> tileWidth (), __BD pMap () -> tileHeight (), __BD 0), QGAMES::Vector::_zNormal);

			return (result.scale ((__BD (_trailLength - (i >> 1))) / __BD _trailLength));
		};

	auto colorFor = [=](int i) -> QGAMES::Color
		{
			QGAMES::Color tC = trackingColor ();
			tC.setAlpha ((int) ((__BD (_trailLength - i) / __BD _trailLength) * __BD 255 * (__BD alphaLevel () / __BD 255)));
			// Proportional to the alpha level of the element outside...

			return (tC);
		};

	for (int i = (_trailLength - 1); i >= 0; i -= 2)
		if (_trailPositions [i] != QGAMES::Position::_noPoint)
			currentForm () -> frame (currentAspect ()) -> drawOn (scr, _trailPositions [i], colorFor (i).alpha ());

	// The monster is drawn always on top...
	PACMAN::StandardMonster::drawOn (scr, p);
}

// ---
bool PacManII::Wormy::hasCollisionWith (QGAMES::Entity* ety) const
{
	bool result = PACMAN::StandardMonster::hasCollisionWith (ety);

	if (!result)
	{
		QGAMES::Rectangles tR = trailRectangles ();
		for (int i = 0; i < (int) tR.size () && !result; i++)
			result = collisionZone ().intersectsWith (tR [i]);
	}

	return (result);
}

// ---
QGAMES::MazeModel::PositionInMaze PacManII::Wormy::targetMazePosition () const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	switch (status ())
	{
		case PACMAN::Monster::Status::_NOTDEFINED:
			result = currentMazePosition ();
			break;

		case PACMAN::Monster::Status::_CHASING:
			if (_referenceArtists [0] != nullptr && _referenceArtists [1] != nullptr)
			{
				if ((result = QGAMES::MazeModel::PositionInMaze (
						(_referenceArtists [0] -> currentMazePosition ().asVector () + 
						 _referenceArtists [1] -> currentMazePosition ().asVector ()) / __BD 2)) == currentMazePosition ())
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
QGAMES::MazeModel::PositionInMaze PacManII::Wormy::runAwayMazePosition () const
{
	return ((pMap () == nullptr) 
		? QGAMES::MazeModel::_noPosition : pMap () -> oneHomePosition ()); 
}

// ---
QGAMES::Rectangles PacManII::Wormy::trailRectangles () const
{
	QGAMES::Rectangles result;
	QGAMES::Rectangle cZ = collisionZone ();
	for (int i = 0; i < _trailLength && _trailPositions [i] != QGAMES::Position::_noPoint; i++)
		result.push_back (cZ.scale (__BD (_trailLength - (i >> 1)) / __BD _trailLength));
	return (result);
}
