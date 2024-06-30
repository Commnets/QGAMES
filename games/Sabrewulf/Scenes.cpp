#include "Scenes.hpp"
#include "Maps.hpp"
#include "Defs.hpp"
#include "Worlds.hpp"
#include "Game.hpp"

// ---
SabreWulfScene::SabreWulfScene (const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn,
		const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
	: QGAMES::Scene (__SABREWULFSCENE__, m, cn, p, ePL),
	  _mazePlaceNumber (__FIRSTMAZEPLACE__),
	  _locations (),
	  _allLocations (),
	  _add (),
	  _addTp()
	  // ---
	  #ifndef NDEBUG
	  ,_onOffDrawLimits (true)
	  #endif
	  // ---
{
	_allLocations.resize (__NUMBEROFMAZEPLACES__);
	for (int i = 0; i < __NUMBEROFMAZEPLACES__; i++)
		_allLocations [i] = Locations ();

	_add.resize (4);
	_addTp.resize (4);
	_add [0] = 1; _addTp [0] = SabreWulfScene::Location::ExitType::_RIGHTE;
	_add [1] = __NUMBEROFMAZEPLACESX__; _addTp [1] = SabreWulfScene::Location::_DOWNE;
	_add [2] = -1; _addTp [2] = SabreWulfScene::Location::_LEFTE;
	_add [3] = -__NUMBEROFMAZEPLACESX__; _addTp [3] = SabreWulfScene::Location::_UPE;
}

// ---
void SabreWulfScene::setMazePlaceNumber (int r)
{
	assert (_activeMap);
	assert (r < __NUMBEROFMAZEPLACES__);

	_mazePlaceNumber= r;
	((SabreWulfMazePlace*) _activeMap) -> setMazePlaceNumber (r);
	_locations = calculateLocations (_mazePlaceNumber);
}

// ---
int SabreWulfScene::firstNastieFree ()
{
	int result = -1;
	for (int i = 0; i < __SABREWULFMAXNUMBERNASTIES__ && result == -1; i++)
		if (((SabreWulfNastie*) entity (__SABREWULFNASTIESBASE__ + i)) -> state () ==
				SabreWulfMonster::State::__NOTALIVE)
			result = i;
	return (result);
}

// ---
int SabreWulfScene::numberOfNasties (int nP)
{
	int result = 0;
	for (int i = 0; i < __SABREWULFMAXNUMBERNASTIES__; i++)
	{
		SabreWulfNastie* nT = (SabreWulfNastie*) entity (__SABREWULFNASTIESBASE__ + i);
		if (nT -> mazePlaceNumber () == nP && nT -> state () != SabreWulfMonster::State::__NOTALIVE)
			result++;
	}

	return (result);
}

// ---
QGAMES::Position SabreWulfScene::randomFreePosition (int mP)
{
	int ct = 0;
	SabreWulfScene::Locations locs = calculateLocations (mP);
	int pos = rand () % locs.size (); 
	QGAMES::Position result = (*locs.begin ()).second._zone.center ();
	for (SabreWulfScene::Locations::const_iterator i = locs.begin ();
			i != locs.end () && ct != pos; i++, ct++)
		result = (*i).second._zone.center ();
	return (result);
}

// ---
QGAMES::Position SabreWulfScene::limitPosition (int mn, const QGAMES::Position& pos, 
		const QGAMES::Vector& or)
{
	// If the orientation has no a valid value...
	// It is not possible to continue...
	QGAMES::Position result = pos;
	if (or == QGAMES::Vector::_cero)
		return (result);

	// Select all zones valid...
	Locations vLocs;
	QGAMES::Rectangle l = QGAMES::Rectangle::_noRectangle;
	SabreWulfScene::Locations locs = calculateLocations (mn);
	for (int i = 0; i < (int) locs.size (); i++)
		if (locs [i]._zone.hasIn (pos))
			vLocs [i] = locs [i];
	if (vLocs.empty ())
		return (result); 
	// Incredible, but it could happen. Sabreman movements take into account
	// the position of the corner most oriented to the movement
	// When this method is calling, the position could be any!

	bool found = false;
	while (!found)
	{
		for (Locations::const_iterator j = vLocs.begin (); 
				j != vLocs.end () && !found; j++)
			if (!(*j).second._zone.hasIn (result)) // The minimum zone...
				found = true;
		if (!found)			
			result += or;
	}

	return (result);
}

// ---
std::map <int, SabreWulfScene::Path> SabreWulfScene::randomPathFrom 
	(int mZ, const QGAMES::Position& p, int nP, int mE, int tMP)
{
	std::map <int, SabreWulfScene::Path> result;

	// If there is no more places to join...
	// Then the last point in the maze place number l
	// is the center of the first location possible!
	if (nP == 1)
	{
		SabreWulfScene::Path pth = localPathFrom (mZ, p);
		result [pth._mazePlaceNumber] = pth;
		return (result);
	}

	// If there is more pleces to calculate...
	// Select a random direction to go!
	int pR = rand () % 4;
	// Or a specific one if a target maze place has been received
	if (tMP != -1)
	{
		int dt = tMP - mZ;
		pR = (dt < 0) 
			? ((dt <= -__NUMBEROFMAZEPLACESX__) ? 3 : 0)  // Up or right
			: ((dt >= __NUMBEROFMAZEPLACESX__) ? 1 : 2);  // Down or left...
	}

	bool found = false;
	std::vector <QGAMES::Position> pResult;
	for (int i = 0; i < 4 && !found; i++)
	{
		if ((mZ + _add [pR]) != mE)
		{
			// If there is a connection between the entry point
			// ...and the next maze place randomically selected, then
			// a connection has been found and it will be added later!
			if (!(pResult = conexionBetween (mZ, p, mZ + _add [pR])).empty ())
				found = true;
		}
		
		if (!found)
			if (++pR >= 4) pR = 0; // (negative clockwise)
	}

	// If the path has been found, it is added to the lists
	// if paths and a new path is looked in the next maze place!
	// and in this way recursivelly until reached the end!
	if (found)
	{
		SabreWulfScene::Path pth; 
		pth._mazePlaceNumber = mZ;
		pth._nextMazePlaceNumber = mZ + _add [pR];
		pth._positions = pResult;
		result [pth._mazePlaceNumber] = pth;

		// The last position is the exit position...
		QGAMES::Position lPos = pth._positions [pth._positions.size () - 1];
		// The first locationin the new maze place is the exit
		// position of the location connected to that one...
		QGAMES::Position fPos = connectedLocationFor (mZ, _addTp [pR] , lPos).exitPosition ();
		std::map <int, SabreWulfScene::Path> rPth = 
			randomPathFrom (mZ + _add [pR], fPos, nP - 1, mZ, tMP);
		if (!rPth.empty ())
		{
			for (std::map <int, SabreWulfScene::Path>::const_iterator i = rPth.begin ();
					i != rPth.end (); i++)
				result [(*i).first] = (*i).second;
		}
	}
	// If it has not be found
	// There is no exit...and a random path is added just with in the last place...
	else
	{
		SabreWulfScene::Path pth = localPathFrom (mZ, p);
		result [pth._mazePlaceNumber] = pth;
		return (result);
	}

	return (result);
}

// ---
std::map <int, SabreWulfScene::Path> SabreWulfScene::reversePathFrom (int mZ,
	const QGAMES::Position& pos, int nP, 
	const std::vector <QGAMES::Position>& pth, int cP)
{
	// If there is no initial path, or the number of rooms to patss through is just only one
	// ...a random one is generated...
	if (pth.empty () || 
		cP == 0 || cP >= (int) pth.size () || 
		nP <= 1)
		return (randomPathFrom (mZ, pos, nP));

	// Build the first place of the reverse path...
	std::map <int, SabreWulfScene::Path> result;
	std::vector <QGAMES::Position> pthResult;
	for (int i = (cP - 1); i >= 0; i--)
		pthResult.push_back (pth [i]);

	// Adds the exit position
	// ...if something can not be added, then an error is generated...
	bool error = false;
	QGAMES::Position lP = pthResult [pthResult.size () - 1];
	SabreWulfScene::Locations locs = calculateLocations (mZ);
	std::vector <int> eLocs = locationIdsFor (mZ, lP);
	if (!eLocs.empty ())
	{
		SabreWulfScene::Location eLoc = locs [eLocs [0]];
		QGAMES::Position eP = eLoc.exitPosition ();
		if (eP != QGAMES::Position::_cero)
		{
			pthResult.push_back (eP);
			SabreWulfScene::Path nPth;
			nPth._mazePlaceNumber = mZ;
			nPth._nextMazePlaceNumber = eLoc.nextMazePlace (mZ);
			nPth._positions = pthResult;

			SabreWulfScene::Location cLoc = 
				connectedLocationFor (mZ, eLoc._exitType, eLoc.exitPosition ());
			result = randomPathFrom (nPth._nextMazePlaceNumber, cLoc.exitPosition (), nP - 1, mZ);
			result [nPth._mazePlaceNumber] = nPth;
		}
		else
			error = true;
	}
	else
		error = true;

	// If an error has been happen, then a random path is returned...
	// Take care because in this cases, sabreman can be hit!
	return (error ? randomPathFrom (mZ, pos, nP) : result);
}

// ---
void SabreWulfScene::initialize ()
{
	setMap (__SABREWULFMAP__);
	QGAMES::Scene::initialize ();

	setMazePlaceNumber (__FIRSTMAZEPLACE__);

	// The basic data...
	_locations = Locations ();
	// ...and the buffer...
	_allLocations.clear (); _allLocations.resize (__NUMBEROFMAZEPLACES__);
	for (int i = 0; i < __NUMBEROFMAZEPLACES__; i++)
		_allLocations [i] = Locations ();
}

// ---
void SabreWulfScene::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Scene::drawOn (s, p);

	#ifndef NDEBUG
	if (_onOffDrawLimits)
	{
		// Draws the place to walk over...
		for (int i = 0; i < (int) _locations.size (); i++)
			if (_locations [i]._type == Location::_DECIDE)
				s -> drawRectangle (_locations [i]._zone, __BLUECOLOR__);

		// Draws the exit zones of the scene
		// drawing arrows at end of each one...
		for (int i = 0; i < (int) _locations.size (); i++)
		{
			if (_locations [i]._type == Location::_EXIT)
			{
				s -> drawRectangle (_locations [i]._zone, __GREENCOLOR__);
				QGAMES::Form* frm = world () -> game () -> form (__SABREWULFARROWSFORM__);
				QGAMES::Vector vH (__BD (_locations [i]._zone.width () / 2), __BD 0, __BD 0);
				QGAMES::Vector vV (__BD 0, __BD (_locations [i]._zone.height () / 2), __BD 0);
				QGAMES::Vector vFH (__BD (frm -> frameWidth () / 2), __BD 0, __BD 0);
				QGAMES::Vector vFV (__BD 0, __BD (frm -> frameHeight () / 2), __BD 0);
				if (_locations [i]._exitType == Location::_LEFTE) 
					frm -> drawOn (s, __SABREWULFARROWLEFT__, _locations [i]._zone.pos1 () + 
						vV - vFV - vFH, 100);
				if (_locations [i]._exitType == Location::_UPE)
					frm -> drawOn (s, __SABREWULFARROWUP__, _locations [i]._zone.pos1 () + 
						vH - vFH - vFV, 100);
				if (_locations [i]._exitType == Location::_RIGHTE)
					frm -> drawOn (s, __SABREWULFARROWRIGHT__, _locations [i]._zone.pos3 () + 
						vV - vFV - vFH, 100);
				if (_locations [i]._exitType == Location::_DOWNE)
					frm -> drawOn (s, __SABREWULFARROWDOWN__, _locations [i]._zone.pos2 () - 
						vH - vFH - vFV, 100);
			}
		}
	}
	#endif
}

// ---
void SabreWulfScene::processEvent (const QGAMES::Event& e)
{
	if (e.code () == __SABREMANOUTOFLIMITS__)
		notify (e); // Notify it to the world to change the maze place...
	else if (e.code () == __SABREMANOUTEXITZONE__)
		notify (e); // Notify sabreman is exiting the maze...
}

// ---
void SabreWulfScene::processEvent (const QGAMES::Event& e, QGAMES::Notifier* n)
{
	if (e.code () == __INMORTALOUTOFLIMITS__)
		notifyFrom (e, n); 
}

// ---
#ifndef NDEBUG
void SabreWulfScene::onOffDrawLimits ()
{
	assert (_activeMap);
	_onOffDrawLimits = !_onOffDrawLimits;
	((SabreWulfMazePlace*) _activeMap) -> onOffDrawLimits ();
}
#endif
// ---

// ---
QGAMES::Position SabreWulfScene::Location::center () const
{
	QGAMES::Vector v (__BD ((int) ((_zone.pos2 ().posX () - _zone.pos1 ().posX ()) / 2)),
		__BD ((int) ((_zone.pos2 ().posY () - _zone.pos1 ().posY ()) / 2)), __BD 0);
	return (_zone.pos1 () + v);
}

// ---
QGAMES::Position SabreWulfScene::Location::exitPosition () const
{
	QGAMES::Position result = QGAMES::Position::_cero;

	if (_exitType == SabreWulfScene::Location::ExitType::_RIGHTE)
		result = _zone.pos3 () + ((_zone.pos2 () - _zone.pos3 ()) / 2);
	else if (_exitType == SabreWulfScene::Location::ExitType::_LEFTE)
		result = _zone.pos1 () + ((_zone.pos4 () - _zone.pos1 ()) / 2);
	else if (_exitType == SabreWulfScene::Location::ExitType::_UPE)
		result = _zone.pos1 () + ((_zone.pos3 () - _zone.pos1 ()) / 2);
	else if (_exitType == SabreWulfScene::Location::ExitType::_DOWNE)
		result = _zone.pos4 () + ((_zone.pos2 () - _zone.pos4 ()) / 2);

	return (result);
}

// ---
QGAMES::Position SabreWulfScene::Location::thirdPartClosestTo (const QGAMES::Position& p)
{
	QGAMES::bdata w = _zone.width (); QGAMES::bdata h = _zone.height ();
	if (w == 0 || h == 0)
		return (QGAMES::Position::_cero);

	// Creates the three rectangles...
	std::vector <QGAMES::Rectangle> parts;
	if (w >= h)
	{
		QGAMES::Vector pt (w / 3, __BD 0, __BD 0);
		QGAMES::Position pt2 = _zone.pos1 () + pt;
		QGAMES::Position pt3 = pt2 + pt;
		parts.push_back (QGAMES::Rectangle (_zone.pos1 (), pt2 + QGAMES::Vector (__BD 0, h, __BD 0)));
		parts.push_back (QGAMES::Rectangle (pt2, pt3 + QGAMES::Vector (__BD 0, h, __BD 0)));
		parts.push_back (QGAMES::Rectangle (pt3, _zone.pos2 ()));
	}
	else
	{
		QGAMES::Vector pt (__BD 0, h / 3, __BD 0);
		QGAMES::Position pt2 = _zone.pos1 () + pt;
		QGAMES::Position pt3 = pt2 + pt;
		parts.push_back (QGAMES::Rectangle (_zone.pos1 (), pt2 + QGAMES::Vector (w, __BD 0, __BD 0)));
		parts.push_back (QGAMES::Rectangle (pt2, pt3 + QGAMES::Vector (w, __BD 0, __BD 0)));
		parts.push_back (QGAMES::Rectangle (pt3, _zone.pos2 ()));
	}

	// Detects the closest center...
	QGAMES::Position result = parts [0].center ();
	QGAMES::bdata dt = result.distanceTo (p);
	for (int i = 1; i < 3; i++)
	{
		QGAMES::Position ct = parts [i].center ();
		if (ct.distanceTo (p) < dt)
			result = ct;
	}

	return (result);
}

// ---
int SabreWulfScene::Location::nextMazePlace (int mZ)
{
	int result = mZ;
	if (_exitType == SabreWulfScene::Location::ExitType::_LEFTE) result -= 1;
	else if (_exitType == SabreWulfScene::Location::ExitType::_RIGHTE) result += 1;
	else if (_exitType == SabreWulfScene::Location::ExitType::_UPE) result -= __NUMBEROFMAZEPLACESX__;
	else if (_exitType == SabreWulfScene::Location::ExitType::_DOWNE) result += __NUMBEROFMAZEPLACESX__;
	return (result);
}

// ---
const SabreWulfScene::Locations& SabreWulfScene::calculateLocations (int mP)
{
	// Try to get first the locations from the buffer...
	if (!_allLocations [mP].empty ())
		return (_allLocations [mP]);

	// Once the maze has been set...
	// It is necessary to calculate what we call "the movement paths"
	// which are the places where the user moves throught
	// Every place can the player is in can be either a "PASS" or a "DECIDE" or an "EXIT" place.
	// If it is a "DECIDE" place it means the plyer can decide to change
	// the movement direction to other places...
	// In an "EXIT" place the connection with the next room happens
	_allLocations [mP] = Locations (); // Clear...
	std::vector <QGAMES::Rectangle> places = ((SabreWulfMazePlace*) _activeMap) -> spaces (mP);
	for (int i = 0; i < (int) places.size (); i ++)
	{
		Location nLoc;
		nLoc._id = i;
		nLoc._zone = places [i];
		nLoc._type = Location::_ISOLATE;
		if (!_allLocations [mP].empty ())
		{
			for (int j = (i - 1); j >= 0; j--)
			{
				Location loc = _allLocations [mP][j];
				int jP = joinPositionBetween (nLoc, loc);
				if (jP != 0) // There is a join possibility...
				{
					// 0: left, 1: up, 2: right, 3: down
					if (jP & 8) // Left...and the other one by the right (as a consequence)
						{ nLoc._conexions [0] = loc._id; loc._conexions [2] = nLoc._id; }
					if (jP & 4) // Up...and the other one by the down part (as a consequence)
						{ nLoc._conexions [1] = loc._id; loc._conexions [3] = nLoc._id; }
					if (jP & 2) // Right...and the other one by the left part (as a consequence)
						{ nLoc._conexions [2] = loc._id; loc._conexions [0] = nLoc._id; }
					if (jP & 1) // Down...and the other one by the up part (as a consequence)
						{ nLoc._conexions [3] = loc._id; loc._conexions [1] = nLoc._id; }
					// Other values mean no connection exists!
					// One link allowed!

					// The old location has to be also rectified!
					_allLocations [mP][j] = loc;
				}
			}
		}

		_allLocations [mP][nLoc._id] = nLoc;
	}

	// Real "exist ways" are those one with the corners out the playing zone...
	for (int i = 0; i < (int) _allLocations [mP].size (); i++)
	{
		Location loc = _allLocations [mP][i];

		// Type of exit location...
		if (loc._zone.pos1 ().posX () >= __BD (__MAPORIGINX__ + (__ORIGINALMAPINITX__ * __XSCALE__)) &&
			loc._zone.pos1 ().posY () >= __BD (__MAPORIGINY__ + (__ORIGINALMAPINITY__ * __YSCALE__)) &&
			loc._zone.pos2 ().posX () <= __BD (__MAPORIGINX__ + (__ORIGINALMAPLENGTH__ * __XSCALE__)) &&
			loc._zone.pos2 ().posY () <= __BD (__MAPORIGINY__ + 
				((__ORIGINALMAPHEIGHT__ + __ORIGINALMAPINITY__) * __YSCALE__)))
				loc._type = Location::_DECIDE;
			else
				loc._type = Location::_EXIT;

		// If it is an exit...wich type?
		if (loc._type == Location::_EXIT)
		{
			if (loc._zone.pos1 ().posX () <= __BD __MAPORIGINX__)
				loc._exitType = Location::_LEFTE;
			if (loc._zone.pos1 ().posY () <= __BD (__MAPORIGINY__ + (__ORIGINALMAPINITY__ * __YSCALE__)))
				loc._exitType = Location::_UPE;
			if (loc._zone.pos2 ().posX () >=  __BD (__MAPORIGINX__ + (__ORIGINALMAPLENGTH__ * __XSCALE__)))
				loc._exitType = Location::_RIGHTE;
			if (loc._zone.pos2 ().posY () >= __BD (__MAPORIGINY__ + 
				((__ORIGINALMAPHEIGHT__ + __ORIGINALMAPINITY__) * __YSCALE__)))
					loc._exitType = Location::_DOWNE;
		}

		_allLocations [mP][i] = loc;
	}

	return (_allLocations [mP]);
}

// ---
std::vector <int> SabreWulfScene::locationIdsFor (int mP, const QGAMES::Position& p)
{
	std::vector <int> result;
	const SabreWulfScene::Locations& locs = calculateLocations (mP);
	for (SabreWulfScene::Locations::const_iterator i = locs.begin (); i != locs.end (); i++)
		if ((*i).second._zone.hasIn (p))
			result.push_back ((*i).second._id);
	return (result);
}

// ---
SabreWulfScene::Location SabreWulfScene::firstExitLocation (int mP, 
	SabreWulfScene::Location::ExitType eT, const QGAMES::Position& p)
{
	SabreWulfScene::Location result;

	QGAMES::bdata dt = __MAXBDATA__;
	const SabreWulfScene::Locations& locs = calculateLocations (mP);
	for (SabreWulfScene::Locations::const_iterator i = locs.begin ();
			i != locs.end (); i++)
	{
		if ((*i).second._exitType == eT)
		{
			QGAMES::bdata dtT = p.distanceTo ((*i).second.exitPosition ());
			if (dtT < dt) // With the first
			{
				dt = dtT;
				result = (*i).second; 
			}
		}
	}
	
	return (result);
}

// ---
SabreWulfScene::Location SabreWulfScene::connectedLocationFor 
	(int mP, SabreWulfScene::Location::ExitType eT, const QGAMES::Position& p)
{
	SabreWulfScene::Location loc = firstExitLocation (mP, eT, p);
	if (loc._id != -1) // Is there any?
	{
		int nMP = mP;
		SabreWulfScene::Location::ExitType nET = eT;
		if (eT == SabreWulfScene::Location::ExitType::_RIGHTE) 
			{ nET = SabreWulfScene::Location::ExitType::_LEFTE; nMP = mP + 1; }
		else if (eT == SabreWulfScene::Location::ExitType::_LEFTE) 
			{ nET = SabreWulfScene::Location::ExitType::_RIGHTE; nMP = mP - 1; }
		else if (eT == SabreWulfScene::Location::ExitType::_DOWNE) 
			{ nET = SabreWulfScene::Location::ExitType::_UPE; nMP = mP + __NUMBEROFMAZEPLACESX__; }
		else if (eT == SabreWulfScene::Location::ExitType::_UPE) 
			{ nET = SabreWulfScene::Location::ExitType::_DOWNE; nMP = mP - __NUMBEROFMAZEPLACESX__; }
		loc = firstExitLocation (nMP, nET, p); // Maintain the point...
	}

	return (loc);
}

// ---
int SabreWulfScene::joinPositionBetween (const SabreWulfScene::Location& l1, 
	const SabreWulfScene::Location& l2) const
{
	if (!QGAMES::squareIntersectSquare (
		l1._zone.pos1 ().posX (), l1._zone.pos2 ().posX (), 
			l1._zone.pos1 ().posY (), l1._zone.pos2 ().posY (),
		l2._zone.pos1 ().posX (), l2._zone.pos2 ().posX (), 
			l2._zone.pos1 ().posY (), l2._zone.pos2 ().posY ()))
		return (0); // No conexions...

	// If there is an intersection...
	// it doesn't matter the size of this to calculate the relative positions
	// What really cares is the relative position of the main points...
	int result = 0;
	if ((l1._zone.pos1 ().posX () > l2._zone.pos1 ().posX ()) &&
		(l1._zone.pos2 ().posX () > l2._zone.pos2 ().posX ())) // l1 is at the right of l2...
		result |= 8;
	if ((l1._zone.pos1 ().posX () < l2._zone.pos1 ().posX ()) &&
		(l1._zone.pos2 ().posX () < l2._zone.pos2 ().posX ())) // l2 is at the right of l1...
		result |= 2; 
	if ((l1._zone.pos1 ().posY () > l2._zone.pos1 ().posY ()) &&
		(l1._zone.pos2 ().posY () > l2._zone.pos2 ().posY ())) // l1 is at the down side of l2...
		result |= 4;
	if ((l1._zone.pos1 ().posY () < l2._zone.pos1 ().posY ()) &&
		(l1._zone.pos2 ().posY () < l2._zone.pos2 ().posY ())) // l2 is at the up side of l1...
		result |= 1; 
	return (result);
}

// ---
std::vector <QGAMES::Position> SabreWulfScene::pathFrom (int mZ, const QGAMES::Position& p1, 
		const QGAMES::Position& p2, std::vector <int> lE)
{
	std::vector <QGAMES::Position> result;

	std::vector <int> locIds1 = locationIdsFor (mZ, p1);
	// If p1 is in many places, deletes those one already read!
	std::vector <int> locIds1T;
	for (int i = 0; i < (int) locIds1.size (); i++)
		if (std::find (lE.begin (), lE.end (), locIds1 [i]) == lE.end ())
			locIds1T.push_back (locIds1 [i]); // It hasn't been visted before!
	locIds1 = locIds1T;
	// It doesn't matter p2 is in many places...
	std::vector <int> locIds2 = locationIdsFor (mZ, p2);
	if (locIds1.empty () || locIds2.empty ())
		return (result); // The point are in some no reacheable place!

	// Try to look for a common location for both points
	// If it is found, then the p2 is directly introduced into the result queue
	// Because it can be reched from p1
	bool found = false;
	for (int i = 0; i < (int) locIds1.size () && !found; i++)
		for (int j = 0; j < (int) locIds2.size () && !found; j++)
			if (locIds1 [i] == locIds2 [j])
				found = true;
	if (found)
	{
		result.push_back (p2);
		return (result);
	}

	// If not, try to look for a connection between both points
	// Even in the case there are many locations for the same point
	QGAMES::Position lPos = QGAMES::Position::_cero;

	// A potential path to connect both points...
	// if it is found it will be added to the result path
	std::vector <QGAMES::Position> pResult;
	SabreWulfScene::Locations locs = calculateLocations (mZ);
	SabreWulfScene::Location loc1 = locs [locIds1 [0]]; // The initial loc is one where p1 is...
	SabreWulfScene::Location locE; // Let's try to determinate the next location...
	
	// Iterate over the conexions of the initial location, loking for a connection between both locs
	// As the method is recursive, a list with all locations alredy tested for connection
	// is kept, and passed to the next invokation, to avoid they will repeated
	// and then generate a deadlock...
	for (int j = 0; j < 4 && !found; j++)
	{
		if (loc1._conexions [j] != -1 && 
			std::find (lE.begin (), lE.end (), loc1._conexions  [j]) == lE.end ())
		{
			std::vector <int> nlE; 
			std::copy (lE.begin (), lE.end (), std::back_inserter (nlE));
			nlE.push_back (locIds1 [0]);
			// If there is connection path valid comming from the recursive call, then it is valid!
			locE = locs [loc1._conexions [j]]; // This is the next location potentially!...
			lPos = locE._zone.center (); // ...and the point to go is the center of it
			if (!(pResult = pathFrom (mZ, lPos, p2, nlE)).empty ())
				found = true; // The connection was found...
		}
	}

	// If a connection between entry and exit point has been reached
	// The the partial result is add to the final result
	if (found)
	{
		QGAMES::Position pA = loc1.thirdPartClosestTo (lPos);
		QGAMES::bdata dA = pA.distanceTo (p1);
		QGAMES::Position pE = locE.thirdPartClosestTo (p1);
		QGAMES::bdata dE = pE.distanceTo (p1);
		QGAMES::bdata dLPos = lPos.distanceTo (p1);

		// If there is sorter way between the initial point
		// ...and the center of the next location to go
		// it is added as previous...
		if (dLPos > dA || dLPos > dE)
			result.push_back ((dA <= dE) ? pA : pE);
		result.push_back (lPos);

		// The rest of the way is added
		for (int j = 0; j < (int) pResult.size (); j++)
			result.push_back (pResult [j]);
		std::unique (result.begin (), result.end ());
	}
		
	// The final result is returned...
	return (result);
}

// ---
std::vector <QGAMES::Position> SabreWulfScene::conexionBetween 
	(int mZ1, const QGAMES::Position& p, int mZ2)
{
	// Determinate the type of exit that is being tried to locate!
	SabreWulfScene::Location::ExitType eT = 
		SabreWulfScene::Location::ExitType::_NONE;
	if ((mZ2 - mZ1) == 1) // mZ2 is in the right...
		eT = SabreWulfScene::Location::_RIGHTE;
	else if ((mZ1 - mZ2) == 1) // mZ2 is in the left...
		eT = SabreWulfScene::Location::_LEFTE;
	else if ((mZ2 - mZ1) == __NUMBEROFMAZEPLACESX__) // mZ2 is in the down side...
		eT = SabreWulfScene::Location::_DOWNE;
	else if ((mZ1 - mZ2) == __NUMBEROFMAZEPLACESX__) // mZ2 is in the upper side...
		eT = SabreWulfScene::Location::_UPE;

	// No possibilities of having some type of connection...
	if (eT == SabreWulfScene::Location::_NONE)
		return (std::vector <QGAMES::Position> ());

	// ..if there is, try to determinate it!
	// There should be a location connecting the mZ1 to the mZ2...
	SabreWulfScene::Locations locs = calculateLocations (mZ1);

	// At this point we are not sure there is an exit location
	// of the type you are looking for...
	// Additionally, there can be several of those. Then it is time to find
	// ..and choose one if any randomically!
	std::vector <int> sLocs; 
	for (SabreWulfScene::Locations::const_iterator i = locs.begin (); 
			i != locs.end ();  i++)
		if ((*i).second._type == SabreWulfScene::Location::Type::_EXIT &&
			(*i).second._exitType == eT)
			sLocs.push_back ((*i).first);

	// If there is no exit of the type...
	// return and empty collection
	if (sLocs.size () == 0)
		return (std::vector <QGAMES::Position> ());
	// If there is only one, try to find the path
	// But it could be still empty...depending on the methdod invoked!
	if (sLocs.size () == 1)
		return (pathFrom (mZ1, p, locs [sLocs [0]].exitPosition ()));

	// At this point...
	// We know several exits of the type requested exist
	// Then the code will try to find the exit of one, starting randomically!
	// This is to generate more dynamism
	int i = 0;
	bool found = false;
	std::vector <QGAMES::Position> pResult;
	int nSLoc = rand () % sLocs.size ();
	for (int i = 0; i < (int) sLocs.size () && !found; i++)
	{
		if (!(pResult = pathFrom (mZ1, p, locs [sLocs [nSLoc]].exitPosition ())).empty ())
			found = true;
		if (++nSLoc >= (int) sLocs.size ())
			nSLoc = 0;
	}

	// Returns finally what found...and it can be empty!
	return (pResult);
}

// ---
SabreWulfScene::Path SabreWulfScene::localPathFrom (int mZ, const QGAMES::Position& p)
{
	SabreWulfScene::Locations locs = calculateLocations (mZ);
		
	bool wfound = false;
	std::vector <QGAMES::Position> way;
	for (int i = 0; i < 3 && !wfound; i++) // Three oportunitties maximum to find something...
		if (!(way = pathFrom (mZ, p, locs [rand () % locs.size ()].center ())).empty ())
			wfound = true;

	if (!wfound)
	{
		QGAMES::Position endPos = p;
		std::vector <int> locIds = locationIdsFor (mZ, p);
		if (!locIds.empty ())
			endPos = locs [locIds [0]]._zone.center (); 
		// The end position is the center of the zone where the position received is located!
		way.push_back (endPos);
	}
		
	SabreWulfScene::Path result;
	result._mazePlaceNumber = mZ;
	result._nextMazePlaceNumber = -1; // It is local...there is more maze place to link to...
	result._positions = way;
	return (result);
}
