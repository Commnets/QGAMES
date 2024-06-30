#include "Maps.hpp"
#include "Sabrewulf.hpp"
#include "Game.hpp"

// ---
SabreWulfMazePlace::SabreWulfMazePlace (int c, const QGAMES::Layers& l, int w, int h,
	const QGAMES::MapProperties& p)
	: QGAMES::ObjectMap (c, l, w, 0, h, p),
	  _mazePlaceNumber (__FIRSTMAZEPLACE__),
	  _limits (),
	  _emptySpaces (),
	  _allLimits (),
	  _allEmptySpaces (),
	  _exitingZone (QGAMES::Rectangle::_noRectangle)
	  // ---
	  #ifndef NDEBUG
	  ,_onOffDrawLimits (true)
	  #endif
	  // ---
{
	// The buffer with the limits...
	_allLimits.resize (__NUMBEROFMAZEPLACES__);
	_allEmptySpaces.resize (__NUMBEROFMAZEPLACES__);
	for (int i = 0; i < __NUMBEROFMAZEPLACES__; i++)
		_allLimits [i] = _allEmptySpaces [i] = std::vector <QGAMES::Rectangle> ();

	// Fix the exiting zone...
	// This data doesn't change along the game!
	QGAMES::Vector size (__BD 32, __BD 32, __BD 0);
	QGAMES::Position cter (__BD (__SCREENWIDTH__ >> 1), __BD (__SCREENHEIGHT__ >> 1), __BD 0);
	_exitingZone = QGAMES::Rectangle (cter - size, cter + size);
}

// ---
void SabreWulfMazePlace::setMazePlaceNumber (int mP)
{
	assert (mP >= 0 && mP < __NUMBEROFMAZEPLACES__);

	_mazePlaceNumber = mP;

	// Calculate the limits of the map...
	_limits = calculateLimitsFor (_mazePlaceNumber);
	// ...and the empty spaces
	_emptySpaces = calculateEmptySpacesFor (_mazePlaceNumber);
	// Both are used by the scene, to determinate later
	// if the entities moves or not by the right place!
}

// ---
QGAMES::Layers SabreWulfMazePlace::relevantLayers ()
{
	return (QGAMES::Layers ()); // No relevant layers...
}

// ---
bool SabreWulfMazePlace::isPositionFree (const QGAMES::Position& pos) const
{
	// It can be done testing whether the empty spaces or the spaces with something...
	// Normally it is better to use the spaces where saberman can go through,
	// because there are usually much less than the others!!
	bool result = false;
	for (int i = 0; i < (int) _emptySpaces.size () && !result; i++)
	if (_emptySpaces [i].hasIn (pos))
		result = true; // If the position is in a emptySpace, then it is valid...
	return (result);
}

// ---
bool SabreWulfMazePlace::isPositionFree (const QGAMES::Position& pos, int mN)
{
	bool result = false;
	std::vector <QGAMES::Rectangle> eS = calculateEmptySpacesFor (mN);
	for (int i = 0; i < (int) eS.size () && !result; i++)
	if (eS [i].hasIn (pos))
		result = true; // If the position is in a emptySpace, then it is valid...
	return (result);
}

// ---
bool SabreWulfMazePlace::isOutOfLimits (const QGAMES::Position& pos) const
{
	return (pos.posX () <= __MINVISIBLEPOSX__ || pos.posX () >= __MAXVISIBLEPOSX__ ||
		pos.posY () <= __MINVISIBLEPOSY__ || pos.posY () >= __MAXVISIBLEPOSY__);
}

// ---
bool SabreWulfMazePlace::isInExitZone (const QGAMES::Position& pos) const
{
	// If the current maze place is not the exit place, then the answer will be false always
	// In other case, the answer will depend on the position is or not in the exit rectangle...
	return ((_mazePlaceNumber != __LASTMAZEPLACE__) 
		? false : ((_exitingZone.hasIn (pos)) ? true : false));
}

// ---
void SabreWulfMazePlace::initialize ()
{
	QGAMES::ObjectMap::initialize ();

	// The first maze place location...
	_mazePlaceNumber = __FIRSTMAZEPLACE__;

	// The basic data...
	_limits.clear (); _limits.resize (0);
	_emptySpaces.clear (); _emptySpaces.resize (0);
	// The buffer...
	_allLimits.resize (__NUMBEROFMAZEPLACES__);
	_allEmptySpaces.resize (__NUMBEROFMAZEPLACES__);
	for (int i = 0; i < __NUMBEROFMAZEPLACES__; i++)
		_allLimits [i] = _allEmptySpaces [i] = std::vector <QGAMES::Rectangle> ();
}

// ---
void SabreWulfMazePlace::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::ObjectMap::drawOn (s, p);

	// The map itself...
	_scene -> world () -> game () -> 
		form (__SPRITEMAPBASE__ + SabreWulfMaze::_data._mazePlaces [_mazePlaceNumber]) -> 
			drawOn (s, 0, QGAMES::Position (__BD __MAPORIGINX__, 
				__BD (__MAPORIGINY__ + (__ORIGINALMAPINITY__ << 1)), __BD 0));

	#ifndef NDEBUG
	// Only in debug mode...
	// Draws a vertical net to easilly ubicate the different elements
	for (int i = 0; i <= ((__ORIGINALSPECTRUMWIDTH__ << 1) / 0x10); i++)
	{
		QGAMES::Position pos1 (__BD (__MAPORIGINX__ + (0x10 * i)), __BD __MAPORIGINY__, __BD 0);
		QGAMES::Position pos2 = pos1 + 
			QGAMES::Vector (__BD 0, __BD (__ORIGINALSPECTRUMHEIGHT__ << 1), __BD 0);
		s -> drawLine (pos1, pos2, __GRAYCOLOR__); 
	}

	// Draws the horizontal part of the net...
	for (int i = 0; i <= ((__ORIGINALSPECTRUMHEIGHT__ << 1) / 0x10); i++)
	{
		QGAMES::Position pos1 (__BD __MAPORIGINX__, __BD (__MAPORIGINY__ + (0x10 * i)), __BD 0);
		QGAMES::Position pos2 = pos1 + 
			QGAMES::Vector (__BD (__ORIGINALSPECTRUMWIDTH__ << 1), __BD 0, __BD 0);
		s -> drawLine (pos1, pos2, __GRAYCOLOR__);
	}

	// Draws the limits of the playing zone...
	QGAMES::Vector vH (__BD 32, __BD 0, __BD 0);
	QGAMES::Vector vV (__BD 0, __BD 32, __BD 0);
	QGAMES::Position p1 (__BD __MAPORIGINX__, __BD __MAPORIGINY__, __BD 0);
	QGAMES::Position p2 (__BD (__MAPORIGINX__ + (__ORIGINALMAPLENGTH__ * __XSCALE__)), 
		__BD (__MAPORIGINY__ + ((__ORIGINALMAPHEIGHT__ + __ORIGINALMAPINITY__) * __YSCALE__)), __BD 0);
	QGAMES::Position p3 (__BD (__MAPORIGINX__ + (__ORIGINALMAPLENGTH__ * __XSCALE__)), 
		__BD __MAPORIGINY__, __BD 0);
	QGAMES::Position p4 (__BD __MAPORIGINX__, 
		__BD (__MAPORIGINY__ + ((__ORIGINALMAPHEIGHT__ + __ORIGINALMAPINITY__) * __YSCALE__)), __BD 0);
	s -> drawLine (p1, p1 + vH, __BLUECOLOR__);
	s -> drawLine (p1, p1 + vV, __BLUECOLOR__);
	s -> drawLine (p2, p2 - vH, __BLUECOLOR__);
	s -> drawLine (p2, p2 - vV, __BLUECOLOR__);
	s -> drawLine (p3, p3 - vH, __BLUECOLOR__);
	s -> drawLine (p3, p3 + vV, __BLUECOLOR__);
	s -> drawLine (p4, p4 + vH, __BLUECOLOR__);
	s -> drawLine (p4, p4 - vV, __BLUECOLOR__);

	// Draws the exit zone, if the map is there...
	if (_mazePlaceNumber == __LASTMAZEPLACE__)
		s -> drawRectangle (_exitingZone, __BLUECOLOR__);
	#endif
}

// ---
const std::vector <QGAMES::Rectangle>& SabreWulfMazePlace::calculateLimitsFor (int mP)
{
	assert (mP >= 0 && mP < __NUMBEROFMAZEPLACES__);

	// If the limits has already been calculated...
	if (!_allLimits [mP].empty ())
		return (_allLimits [mP]); // ...get it

	// Determinates the limits of the maze place
	// to make the test quicker later on...
	_allLimits [mP].clear ();
	SabreWulfMaze::MazeForms mfrms = 
		SabreWulfMaze::_data._typeOfMazePlaces 
			[SabreWulfMaze::_data._mazePlaces [mP]]._forms;
	_allLimits [mP].resize (mfrms.size ());
	for (int i = 0; i < (int) mfrms.size (); i++)
	{
		SabreWulfMaze::MazeForm mfrm = mfrms [i];
		QGAMES::Form* frm = world () -> game () -> form (mfrm._form);
		QGAMES::Position pos (__BD mfrm._position._x, __BD mfrm._position._y, __BD 0);
		QGAMES::Rectangle zone (pos, 
			pos + QGAMES::Vector (__BD frm -> width (), __BD frm -> height (), __BD 0));

		_allLimits [mP][i] = zone;
	}

	return (_allLimits [mP]);
}

// ---
const std::vector <QGAMES::Rectangle>& SabreWulfMazePlace::calculateEmptySpacesFor (int mP)
{
	assert (mP >= 0 && mP < __NUMBEROFMAZEPLACES__);

	// If the empty spaces has already been calculated...
	if (!_allEmptySpaces [mP].empty ())
		return (_allEmptySpaces [mP]); // ...get it

	// Calculates the limits for that room, because it is needed...
	calculateLimitsFor (mP);

	// Gets the forms...
	SabreWulfMaze::MazeForms mfrms = 
		SabreWulfMaze::_data._typeOfMazePlaces 
			[SabreWulfMaze::_data._mazePlaces [mP]]._forms;

	// Determinates the empty spaces where the player can move in...
	// Those empty spaces are also rectangles...
	// First of order put all x and y coordinates from less to more...
	_allEmptySpaces [mP].clear ();
	std::vector <QGAMES::bdata> posX, posY;
	for (int i = 0; i < (int) mfrms.size (); i++)
	{
		SabreWulfMaze::MazeForm mfrm = mfrms [i];
		QGAMES::Form* frm = world () -> game () -> form (mfrm._form);
		posX.push_back (__BD mfrm._position._x); 
		posX.push_back (__BD (mfrm._position._x + frm -> width ()));
		posY.push_back (__BD mfrm._position._y); 
		posY.push_back (__BD (mfrm._position._y + frm -> height ()));
	}

	std::sort (posX.begin (), posX.end ());
	posX.erase (std::unique (posX.begin (), posX.end ()), posX.end ());
	std::sort (posY.begin (), posY.end ());
	posY.erase (std::unique (posY.begin (), posY.end ()), posY.end ());

	// Now creates, per line, the maximum empty rectangle possible,
	// joining all rectangles close each other
	for (int j = 0; j < (int) (posY.size () - 1); j++)
	{
		// First of all, put in a temporal vector 
		// all rectangles bigger than 8 pixels wide!
		std::vector <QGAMES::Rectangle> tmpEmpty;
		for (int i = 0; i < (int) (posX.size () - 1); i++)
		{
			QGAMES::Rectangle pZ (QGAMES::Position (posX [i], posY [j]),
				QGAMES::Position (posX [i + 1], posY  [j + 1]));

			bool insert = true;
			for (int k = 0; k < (int) _allLimits [mP].size () && insert; k++)
				if (pZ.isIn (_allLimits [mP][k])) // If the rectangle is a limit, then it is not valid...
					insert = false;
			
			// The rectangles to walk in can't be lesser than 8 pixels...
			if (insert && pZ.width () > __BD 8 && pZ.height () > __BD 8)
				tmpEmpty.push_back (pZ.bigger (8)); // 8 pixels bigger, just to 
													// allow the entities walk a little bit over the grass!
		}

		// Then, try to add the all followed rectangles in the line!
		// It could be possible to join all of them if they are one over the next...
		int nR = 0;
		if (!tmpEmpty.empty ())
		{
			int init = 0; int count = 1;
			QGAMES::bdata posXLimit = tmpEmpty [init].pos3 ().posX ();
			while (init < (int) tmpEmpty.size ())
			{
				bool canJoin = true;
				while (canJoin && count < (int) tmpEmpty.size ())
				{
					if (tmpEmpty [count].pos1 ().posX () > posXLimit)
					{
						canJoin = false;
						posXLimit = tmpEmpty [count].pos3 ().posX ();
					}
					else
					{
						posXLimit = tmpEmpty [count].pos3 ().posX ();
						count++;
					}
				}

				nR++;
				_allEmptySpaces [mP].push_back (QGAMES::Rectangle (tmpEmpty [init].pos1 (),
					tmpEmpty [(init == count) ? init : (count - 1)].pos2 ()));
				init = (init == count) ? (init + 1) : count;
				count++;
			}
		}

		// If all rectangles joins in only one, and that one
		// goes from left to right, its better to divide it in three
		// One from indicating left way, one indicating right way, and one more
		// Indicating something in the middle!!
		// This is something interesting i.e. when sabreman flies when die
		// instead of flying along the full screen when he dies just in the opposite side
		// sabreman flies just to the middle maximum!
		if (nR == 1)
		{
			QGAMES::Rectangle lastR = _allEmptySpaces [mP][_allEmptySpaces [mP].size () - 1];
			if (lastR.pos1 ().posX () <= __BD __MAPORIGINX__ &&
				lastR.pos2 ().posX () >= __BD (__MAPORIGINX__ + (__ORIGINALMAPLENGTH__ * __XSCALE__)))
			{
				QGAMES::Position lastRC = lastR.center ();
				QGAMES::Rectangle lastRL (lastR.pos1 (), 
					QGAMES::Position (lastRC.posX () - __BD 20, lastR.pos2 ().posY ()));
				QGAMES::Rectangle lastRR (QGAMES::Position (lastRC.posX () +  __BD 20, lastR.pos1 ().posY ()), 
					lastR.pos2 ());
				QGAMES::Rectangle lastRI (QGAMES::Position (lastRC.posX () - __BD 30, lastR.pos1 ().posY ()),
					QGAMES::Position (lastRC.posX () + __BD 30, lastR.pos2 ().posY ()));
				_allEmptySpaces [mP][_allEmptySpaces [mP].size () - 1] = lastRL;
				_allEmptySpaces [mP].push_back (lastRI);
				_allEmptySpaces [mP].push_back (lastRR);
			}
		}
	}

	return (_allEmptySpaces [mP]);
}
