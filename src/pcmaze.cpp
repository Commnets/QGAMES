#include <PacManLike/pcmaze.hpp>
#include <PacManLike/pcgame.hpp>
#include <PacManLike/pcworlds.hpp>
#include <PacManLike/pcscenes.hpp>
#include <PacManLike/pcmaps.hpp>

// ---
const std::vector <QGAMES::MazeModel::PositionInMaze>& PACMAN::Maze::positionsForZone (int n) const
{
	std::map <int, std::vector <QGAMES::MazeModel::PositionInMaze>>::const_iterator i;
	if ((i = _positionsPerZone.find (n)) != _positionsPerZone.end ())
		return ((*i).second);

	for (int i = 0; i < sizeY (); i++)
	{
		for (int j = 0; j < sizeX (); j++)
		{
			QGAMES::MazeModel::PositionInMaze p (j, i, 0);
			if (mazeZoneAt (p) == n)
				_positionsPerZone [n].push_back (p);
		}
	}

	return (_positionsPerZone [n]);
}

// ---
QGAMES::MazeModel::PositionInMaze PACMAN::Maze::randomPosition (const std::vector <int>& z) const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	int i = 0;
	QGAMES::MazeModel::PathInMaze pP = allPositionsConnected ();
	assert (!pP.empty ()); // It can not be empty!

	int n = std::rand () % (int) pP.size ();
	int iN = n; // To avoid to repeat the same position...
	if (std::find (z.begin (), z.end (), mazeZoneAt (pP [n])) != z.end ())
	{
		while (result == QGAMES::MazeModel::_noPosition)
		{
			if (n++ >= (int) pP.size ()) 
				n = 0;

			assert (n != iN);

			if (std::find (z.begin (), z.end (), mazeZoneAt (pP [n])) == z.end ())
				result = pP [n];
		}
	}
	else
		result = pP [n];

	return (result);
}

// ---
bool PACMAN::Maze::arePositionsTogether 
	(const QGAMES::MazeModel::PositionInMaze& p1, const QGAMES::MazeModel::PositionInMaze& p2) const
{
	bool result = false;

	QGAMES::Positions drs = allPossibleDirectionsAt (p1);
	for (auto i : drs)
		if (nextPositionToFollowing (p1, i) == p2)
			result = true;

	return (result);
}

// ---
std::vector <QGAMES::Vector> PACMAN::Maze::restrictedDirectionsAt (const QGAMES::MazeModel::PositionInMaze& p) const
{
	std::vector <QGAMES::Vector> result;

	QGAMES::SetOfOpenValues sMDirs;
	if (mazeInfoAt (p).existSetOfOpenValues (__PACMAN_MAZEINFODIRZONE__))
		sMDirs = mazeInfoAt (p).setOfOpenValues (__PACMAN_MAZEINFODIRZONE__);
	for (int i = sMDirs.lastOpenValueId (); i >= 0; i--)
		result.push_back (QGAMES::Vector (sMDirs.openValue (i).strValue ()));

	return (result);
}

// ---
QGAMES::MazeModel::PositionInMaze PACMAN::Maze::nextXGridPositionFollowing 
	(const QGAMES::MazeModel::PositionInMaze& p, unsigned x, const QGAMES::Vector& d, bool e) const
{
	assert (verifyDirections ({ d }));

	QGAMES::MazeModel::PositionInMaze result = p;
	QGAMES::MazeModel::PositionInMaze tmp = p;
	bool out = false;
	for (unsigned i = 0; i < x && !out; i++)
	{
		QGAMES::Vector vE = (e && d == QGAMES::Vector (__BD 0, __BD -1, __BD 0)) 
			? QGAMES::Vector (__BD -1, __BD 0, __BD 0) : QGAMES::Vector::_cero;
		if (isInMaze (result + d + vE))
			result = result + d + vE;
		else
			out = true;
	}

	return (result);
}

// ---
QGAMES::MazeModel::PathInMaze PACMAN::Maze::next2StepsToGoTo (const QGAMES::MazeModel::PositionInMaze& p1, 
	const QGAMES::MazeModel::PositionInMaze& p2, const std::vector <QGAMES::Vector>& d, const QGAMES::Vector& pD) const
{
	std::vector <QGAMES::Vector> aD = getValidAndOrderedDirectionsPossibleFromTo (p1, p2, d);

	// If a preferred direction has been receive and it is part of the possible ones
	// It would be selected as the only one...
	if (pD != QGAMES::Vector::_noPoint && 
		std::find (aD.begin (), aD.end (), pD) != aD.end ())
		aD = { pD };

	QGAMES::MazeModel::PathInMaze result = { p1 };

	// ----LAMBDAS---
	// To know whether it is a "corner" or not...
	auto isCorner = [=](const QGAMES::MazeModel::PositionInMaze& p) -> bool
		{ 
			return ((p._positionX <= 0 && p._positionY <= 0) ||
					(p._positionX >= (sizeX () - 1) && p._positionY <= 0) ||
					(p._positionX >= (sizeX () - 1) && p._positionY >= (sizeY () - 1)) ||
					(p._positionX <= 0 && p._positionY >= (sizeY () - 1))); 
		};

	// To determine which corner is...
	auto numberCorner = [=](const QGAMES::MazeModel::PositionInMaze& p) -> int
		{
			int result = -1; // No corner...
			if (p._positionX <= 0 && p._positionY <= 0) result = 0; // left - up
			else if (p._positionX >= (sizeX () - 1) && p._positionY <= 0) result = 1; // right - up
			else if (p._positionX >= (sizeX () - 1) && p._positionY >= (sizeY () - 1)) result = 2; // right - down
			else if (p._positionX <= 0 && p._positionY >= (sizeY () - 1)) result = 3; // left - down
			return (result);
		};

	// To compare two distances. Returns true when the firsr is closer to the corner than the second
	auto isCloserThan = [=]
			(const QGAMES::MazeModel::PositionInMaze& p1, 
			 const QGAMES::MazeModel::PositionInMaze& p2, const QGAMES::MazeModel::PositionInMaze& p) -> bool
		{
			bool result = false;
			switch (numberCorner (p))
			{
				case  0: result = (p1._positionX - p2._positionX) < (p2._positionY - p1._positionY); break;
				case  1: result = (p1._positionX - p2._positionX) > (p1._positionY - p2._positionY); break;
				case  2: result = (p1._positionX - p2._positionX) > (p2._positionY - p1._positionY); break;
				case  3: result = (p1._positionX - p2._positionX) < (p1._positionY - p2._positionY); break;
				case -1: default: 
					result = (p.asVector () - p1.asVector ()).module2 () < (p.asVector () - p2.asVector ()).module2 (); break;
			}
			return (result);
		};

	// The manhattan distance...
	auto manhattanDistance = [=](const QGAMES::MazeModel::PositionInMaze& p1, 
								 const QGAMES::MazeModel::PositionInMaze& p2) -> int
			{ return (std::abs (p1._positionX - p2._positionX) + std::abs (p1._positionY - p2._positionY)); };
	// ----LAMBDAS---

	if (aD.empty ())
		return (result);

	if (p1 == p2)
		return (result);

	QGAMES::MazeModel::PositionInMaze pA = QGAMES::MazeModel::_noPosition;
	for (auto i : aD)
	{
		if (std::find (d.begin (), d.end (), i) == d.end ())
		{
			QGAMES::MazeModel::PositionInMaze nP = nextPositionToFollowing (p1, i);
			assert (nP != QGAMES::MazeModel::_noPosition); // It shouldn't but who knows!
			if (pA == QGAMES::MazeModel::_noPosition) pA = nP;
			else if (isCloserThan (nP, pA, p2)) pA = nP;
		}
	}

	if (pA != QGAMES::MazeModel::_noPosition)
		result.push_back (pA);

	return (result);
}

// ---
QGAMES::MazeModel::PathInMaze PACMAN::Maze::nextXStepsToGoTo (int x, const QGAMES::MazeModel::PositionInMaze& p1, 
			const QGAMES::MazeModel::PositionInMaze& p2, const std::vector <QGAMES::Vector>& d, const QGAMES::Vector& pD) const
{
	QGAMES::MazeModel::PathInMaze result = next2StepsToGoTo (p1, p2, d, pD);
	if (result.size () == 2 && result [1] != p2)
	{
		bool tE = false;
		do
		{
			QGAMES::Vector lD = 
				result [result.size () - 1].asVector () - result [result.size () - 2].asVector ();
			QGAMES::MazeModel::PathInMaze pR = next2StepsToGoTo (result [result.size () - 1], p2, { lD }); 
			// No more preferred directions
			if (pR.size () == 2)
			{
				if (std::find (result.begin (), result.end (), pR [1]) != result.end ())
					result.push_back (pR [1]);
				else
					tE = true; // step repeated...
			}
			else
				tE = true; // final point found...
		} while (!tE);
	}

	return (result);
}

// ---
PACMAN::Maze PACMAN::Maze::generateEmptyMaze (int x, int y)
{
	assert (x > 0 && y > 0);

	std::vector <QGAMES::SetOfOpenValues> mV;
	for (int i = 0; i < (x * y); i++)
	{
		QGAMES::SetOfOpenValues tV;
		for (int j = 0; j < 4; tV.addOpenValue (j++, QGAMES::OpenValue (true))); // The four possible directions are true...
		mV.push_back (tV);
	}

	return (PACMAN::Maze (x, y, mV));
}

// ---
PACMAN::Maze PACMAN::Maze::generateMazeFrom (int x, int y, const PACMAN::Map* mp)
{
	assert (x > 0 && y > 0 && mp != nullptr);

	__WLOGPLUS (std::string ("Generating maze info for map: ") + std::to_string (mp -> id ()));

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (QGAMES::Game::game ());
	assert (g != nullptr);
	PACMAN::TMXMapBuilder* tB = g -> tmxAddsOnMapBuilder ();

	__WLOGPLUS (std::string ("Connections"));

	// Generate the basic connections...
	std::vector <QGAMES::SetOfOpenValues> mV;
	for (auto i : mp -> directionsLayer () -> tiles ())
	{
		QGAMES::SetOfOpenValues tV;
		if (dynamic_cast <QGAMES::NullTile*> (i) != nullptr)
			for (int j = 0; j < 4 /** Th four directions. */; tV.addOpenValue (j++, QGAMES::OpenValue (false)));
		else
		{
			// The direction frames have the info of the possibilities of movement per
			std::map <int, std::vector <bool>>::const_iterator j = 
				tB -> directionFrames ().find (i -> numberFrame ());

			assert (j != tB -> directionFrames ().end () &&
					(int) (*j).second.size () == 4); // Four values have to be returned always!
			
			int k = 0;
			for (auto l : (*j).second)
				tV.addOpenValue (k++, l);
		}

		mV.push_back (tV);
	}

	assert ((int) mV.size () == (x * y));

	// ...and the basic maze...
	PACMAN::Maze result (x, y, mV);

	__WLOGPLUS (std::string ("Zones"));

	// Zones are always grouped by colors
	// Now it is time to add the info of the zone each position of the maze belongs to...
	// First the zones ar grouped by different colors
	int cH = 0;
	std::vector <std::vector <QGAMES::MazeModel::PositionInMaze>> znes 
		(__PACMAN_MAZENUMBERPOSSIBLEZONES__, std::vector <QGAMES::MazeModel::PositionInMaze> ());
	for (auto i : mp -> mazeZonesLayer () -> tiles ())
	{
		QGAMES::MazeModel::PositionInMaze pM (cH % x, cH / x, 0);

		int nZ = (dynamic_cast <QGAMES::NullTile*> (i) != nullptr) ? -1 : i -> numberFrame ();

		int pNZ = -1;
		for (int j = 0; j < (int) tB -> tilesZones ().size () && pNZ == -1; j++)
			if (tB -> tilesZones ()[j] == nZ) pNZ = j;
		if (pNZ != -1 && pNZ < __PACMAN_MAZENUMBERPOSSIBLEZONES__)
			znes [pNZ].push_back (pM);

		cH++;
	}

	// Now, per zone, and only for the zones not representing neither a tunnel nor monsters' home,
	// ...the restricted movements are calculated...
	static std::vector <int> hA = __PACMAN_MAZEHORIZONTALMOVEMENTZONES__;
	static std::vector <int> vA = __PACMAN_MAZEVERTICALMOVEMENTZONES__;
	for (int i = 0; i < __PACMAN_MAZENUMBERPOSSIBLEZONES__; i++)
	{
		for (auto j : znes [i])
		{
			// The information about the zone the position belogs to is added to the map...
			QGAMES::SetOfOpenValues mI = result.mazeInfoAt (j);
			mI.addOpenValue (__PACMAN_MAZEINFOZONE__, QGAMES::OpenValue (i));

			// If the zone is related with a set of mandatory movements
			std::vector <QGAMES::Vector> mM = { QGAMES::Vector::_noPoint, QGAMES::Vector::_noPoint }; 
			if (std::find (hA.begin (), hA.end (), i) != hA.end ())
				{ mM [0] = QGAMES::Vector (__BD 1, __BD 0, __BD 0); mM [1] = QGAMES::Vector (__BD -1, __BD 0, __BD 0); }
			else if (std::find (vA.begin (), vA.end (), i) != vA.end ())
				{ mM [0] = QGAMES::Vector (__BD 0, __BD 1, __BD 0); mM [1] = QGAMES::Vector (__BD 0, __BD -1, __BD 0); }
			if (mM [0] != QGAMES::Vector::_noPoint)
			{
				// ...and if it is possible to move to the directions restricted...
				if (result.isPossibleToMoveTo (j, mM [0]) && result.isPossibleToMoveTo (j, mM [1]))
				{
					// ..they are remov from the list of all possibilities
					std::vector <QGAMES::Vector> aDirs = result.allPossibleDirectionsAt (j);
					aDirs.erase (std::remove_if (aDirs.begin (), aDirs.end (), 
							[=](const QGAMES::Vector& v) -> bool { return (v == mM [0] || v == mM [1]); }), aDirs.end());
					
					// What remains (if any) is then the not allowed directions
					// ...and that info is added to the information of the map...
					if (!aDirs.empty ())
					{
						QGAMES::SetOfOpenValues Rdrs;
						for (auto k : aDirs)
							Rdrs.addOpenValue (Rdrs.lastOpenValueId () + 1, QGAMES::OpenValue ((std::stringstream () << k).str ()));
						mI.addSetOfOpenValues (__PACMAN_MAZEINFODIRZONE__, Rdrs); 
					}
				}
			}

			// All is st back...
			result.setMazeInfoAt (j, mI);
		}
	}

	__WLOGPLUS (std::string ("Tunnels"));

	// Now it is time to add the info of the tunnels...
	// First reads the info of the tunnels and determine whether there is tunnels
	int cT = 0;
	std::map <int, std::vector <QGAMES::MazeModel::PositionInMaze>> tunnelPos;
	for (auto i : mp -> mazeLocationsLayer () -> tiles ())
	{
		if (std::find (tB -> connectionFrames ().begin (), 
					   tB -> connectionFrames ().end (), i -> numberFrame ()) != tB -> connectionFrames ().end ())
		{
			// The tunnels have to be defined in the limits of the map...
			assert (cT % x == 0 || cT % x == (x - 1) || 
					cT / x == 0 || cT / x == (y - 1));

			tunnelPos [i -> numberFrame () - tB -> connectionFrames ()[0]].
				push_back (QGAMES::MazeModel::PositionInMaze (cT % x, cT / x, 0));
		}

		cT++;
	}

	// Checks whether the information of the tunnels is right...
	for (auto i : tunnelPos)
	{
		// A tunnel has always to have and entry and an exit position defined...
		assert (i.second.size () == 2); 
		// ...and only one direction allowed to in both parts of th tunnel...
		assert ((int) result.allPossibleDirectionsAt (i.second [0]).size () == 1 &&
				(int) result.allPossibleDirectionsAt (i.second [1]).size () == 1);
		// ...and the directions has to be the opposite
		assert (result.allPossibleDirectionsAt (i.second [0])[0] == -result.allPossibleDirectionsAt (i.second [1])[0]);
	}

	// ...and finally, if everything is ok, adds that info to the info related with the path...
	for (auto i : tunnelPos)
	{
		// one side...
		QGAMES::SetOfOpenValues mI1 = result.mazeInfoAt (i.second [0]);
		mI1.addOpenValue (__PACMAN_MAZEINFOTUNNELCONNECTION__, 
			QGAMES::OpenValue ((std::stringstream () << i.second [1].asVector ()).str ()));
		result.setMazeInfoAt (i.second [0], mI1);
		// ...and the other
		QGAMES::SetOfOpenValues mI2 = result.mazeInfoAt (i.second [1]);
		mI2.addOpenValue (__PACMAN_MAZEINFOTUNNELCONNECTION__, 
			QGAMES::OpenValue ((std::stringstream () << i.second [0].asVector ()).str ()));
		result.setMazeInfoAt (i.second [1], mI2);
	}

	return (result);
}

// ---
std::vector <QGAMES::Vector> PACMAN::Maze::allPossibleDirectionsAt (const PositionInMaze& p) const
{
	std::vector <QGAMES::Vector> result;

	// LEFT, RIGHT, UP, DOWN in the openValues,
	QGAMES::SetOfOpenValues mI = mazeInfoAt (p);
	if (mI.openValue (0).boolValue ()) result.push_back (QGAMES::Vector (__BD -1, __BD  0, __BD 0));
	if (mI.openValue (1).boolValue ()) result.push_back (QGAMES::Vector (__BD  1, __BD  0, __BD 0));
	if (mI.openValue (2).boolValue ()) result.push_back (QGAMES::Vector (__BD  0, __BD -1, __BD 0));
	if (mI.openValue (3).boolValue ()) result.push_back (QGAMES::Vector (__BD  0, __BD  1, __BD 0));
	// If the point is a tunnel,...
	if (result.size () == 1 && mI.existOpenValue (__PACMAN_MAZEINFOTUNNELCONNECTION__))
		result.push_back (-result [0]);	// then the opposite direction to the only one allowed is also added (continue)

	return (result);
}

// ---
QGAMES::MazeModel::PositionInMaze PACMAN::Maze::nextPositionToFollowing 
	(const QGAMES::MazeModel::PositionInMaze& p, const QGAMES::Vector& d) const
{
	QGAMES::MazeModel::PositionInMaze result = QGAMES::MazeModel::_noPosition;

	QGAMES::MazeModel::PositionInMaze nP = p + d; // The natural movement...
	if (!isInMaze (nP)) // but if it is out...
	{
		QGAMES::SetOfOpenValues mI = mazeInfoAt (p);
		if (mI.existOpenValue (__PACMAN_MAZEINFOTUNNELCONNECTION__)) // ...it could be a tunnel, if it is defined as it!
			result = QGAMES::MazeModel::PositionInMaze 
				(QGAMES::Vector (mI.openValue (__PACMAN_MAZEINFOTUNNELCONNECTION__).strValue ()));
	}
	else
	{
		std::vector <QGAMES::Vector> aP = allPossibleDirectionsAt (nP);
		if (std::find (aP.begin (), aP.end (), -d) != aP.end ()) 
			result = nP; 
	}

	return (result);
}

// ---
std::vector <QGAMES::Vector> PACMAN::Maze::orderDirections 
	(const QGAMES::MazeModel::PositionInMaze& p1, 
	 const QGAMES::MazeModel::PositionInMaze& p2, const std::vector <QGAMES::Vector>& dirs) const
{
	// By default: UP, LEFT, DOWN, RIGHT
	static std::vector <int> _O = { 0, 1, 2, 3 }; 
	static std::vector <QGAMES::Vector> _DO = 
		{ QGAMES::Vector (__BD  0, __BD -1, __BD 0),
		  QGAMES::Vector (__BD -1, __BD  0, __BD 0),
		  QGAMES::Vector (__BD  0, __BD  1, __BD 0),
		  QGAMES::Vector (__BD  1, __BD  0, __BD 0) };	

	assert (verifyDirections (dirs));

	std::vector <QGAMES::Vector> result;
	for (auto i : _O)
		if (std::find (dirs.begin (), dirs.end (), _DO [i]) != dirs.end ())
			result.push_back (_DO [i]);

	assert (dirs.size () == result.size ());

	return (result);
}
