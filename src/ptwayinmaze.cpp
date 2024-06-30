#include <Platform/ptwayinmaze.hpp>
#include <Room/rmdefinitions.hpp>

QGAMES::MazeModel::PositionInMaze QGAMES::MazeModel::_noPosition = 
	QGAMES::MazeModel::PositionInMaze (__MININT__, __MININT__, __MININT__);
bool QGAMES::MazeModel::_TRACERECURSIVE = false;

// ---
QGAMES::SetOfOpenValues QGAMES::MazeModel::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESMAZEMODELTYPE__));

	for (int k = 0; k < _sizeZ; k++)
		for (int j = 0; j < _sizeY; j++)
			for (int i = 0; i < _sizeX; i++)
				result.addSetOfOpenValues ((k * _sizeX * _sizeY) + (j * _sizeX) + i, 
					_mazeModel [(k * _sizeX * _sizeY) + (j * _sizeX) + i]);

	return (result);
}

// ---
void QGAMES::MazeModel::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& cfg)
{
	assert (cfg.name () == std::string (__QGAMES_RUNTIMEVALUESMAZEMODELTYPE__));
	assert (cfg.numberValues () == (_sizeX * _sizeY * _sizeZ));

	_mazeModel = std::vector <SetOfOpenValues> ();
	for (int k = 0; k < _sizeZ; k++)
		for (int j = 0; j < _sizeY; j++)
			for (int i = 0; i < _sizeX; i++)
				_mazeModel [(k * _sizeX * _sizeY) + (j * _sizeX) + i] = 
					cfg.setOfOpenValues ((k * _sizeX * _sizeY) + (j * _sizeX) + i);
}

// ---
const QGAMES::SetOfOpenValues& QGAMES::MazeModel::mazeInfoAt (const QGAMES::MazeModel::PositionInMaze& p) const
{
	assert (p._positionX < _sizeX && p._positionY < _sizeY && p._positionZ < _sizeZ);

	return (_mazeModel [(p._positionZ * _sizeX * _sizeY) + (p._positionY * _sizeX) + p._positionX]);
}

// ---
void QGAMES::MazeModel::setMazeInfoAt 
	(const QGAMES::MazeModel::PositionInMaze& p, const QGAMES::SetOfOpenValues& oV)
{
	assert (p._positionX < _sizeX && p._positionY < _sizeY && p._positionZ < _sizeZ);

	_mazeModel [(p._positionZ * _sizeX * _sizeY) + (p._positionY * _sizeX) + p._positionX] = oV;
}

// ---
std::vector <QGAMES::SetOfOpenValues> QGAMES::MazeModel::mazeInfoBetween 
	(const QGAMES::MazeModel::PositionInMaze& p1, const QGAMES::MazeModel::PositionInMaze& p2) const
{
	assert (p2._positionX >= p1._positionX && p2._positionY >= p1._positionY && p2._positionZ >= p1._positionZ);

	std::vector <QGAMES::SetOfOpenValues> result;
	for (int k = p1._positionZ; k <= p2._positionZ; k++)
		for (int j = p1._positionY; j <= p2._positionY; j++)
			for (int i = p1._positionX; i <= p2._positionX; i++)
				result.push_back (mazeInfoAt (QGAMES::MazeModel::PositionInMaze (i, j, k)));

	return (result);
}

// ---
bool QGAMES::MazeModel::isPossibleToMoveTo (const QGAMES::MazeModel::PositionInMaze& p1, const QGAMES::Vector& d) const
{
	assert (verifyDirections ({ d }));
	
	std::vector <QGAMES::Vector> dirs = allPossibleDirectionsAt (p1);
	return (std::find (dirs.begin (), dirs.end (), d) != dirs.end ());
}

// ---
QGAMES::Vector QGAMES::MazeModel::directionToGetFromTo 
	(const QGAMES::MazeModel::PositionInMaze& p2, const QGAMES::MazeModel::PositionInMaze& p1) const
{
	QGAMES::Vector result = QGAMES::Vector::_noPoint;

	std::vector <QGAMES::Vector> pD = allPossibleDirectionsAt (p1);
	for (std::vector <QGAMES::Vector>::const_iterator i = pD.begin (); 
		i != pD.end () && result == QGAMES::Vector::_noPoint; i++)
		if (nextPositionToFollowing (p1, (*i)) == p2)
			result = (*i);

	return (result);
}

// ---
QGAMES::MazeModel::PathInMaze QGAMES::MazeModel::allPositionsConnected () const
{
	QGAMES::MazeModel::PathInMaze result;

	for (int i = 0; i < _sizeZ; i++)
	{
		for (int j = 0; j < _sizeY; j++)
		{
			for (int k = 0; k < _sizeX; k++)
			{
				QGAMES::MazeModel::PositionInMaze p (k, j, i);
				if (!allPossibleDirectionsAt (p).empty ())
					result.push_back (p);
			}
		}
	}

	return (result);
}

// ---
std::vector <QGAMES::MazeModel::PathInMaze> QGAMES::MazeModel::allWaysToGoTo 
	(const QGAMES::MazeModel::PositionInMaze& p1, const QGAMES::MazeModel::PositionInMaze& p2, 
	 const std::vector <QGAMES::Vector>& nD) const
{
	_itCounter = 0;

	std::vector <QGAMES::MazeModel::PathInMaze> result = 
		allWaysToGoToImplementation (p1, p2, QGAMES::MazeModel::PathInMaze { }, nD, true);

	return (result);
}

// ---
QGAMES::MazeModel::PathInMaze QGAMES::MazeModel::firstWayToGoTo 
	(const QGAMES::MazeModel::PositionInMaze& p1, const QGAMES::MazeModel::PositionInMaze& p2,
	 const std::vector <QGAMES::Vector>& nD) const
{
	_itCounter = 0;

	std::vector <QGAMES::MazeModel::PathInMaze> ways = 
		allWaysToGoToImplementation (p1, p2, QGAMES::MazeModel::PathInMaze { }, nD, true);

	QGAMES::MazeModel::PathInMaze result = ways.empty () ? QGAMES::MazeModel::PathInMaze () : ways [0];

	return (result);
}

// ---
QGAMES::MazeModel::PathInMaze QGAMES::MazeModel::shortestWayToGoTo
	(const QGAMES::MazeModel::PositionInMaze& p1, const QGAMES::MazeModel::PositionInMaze& p2,
	 const std::vector <QGAMES::Vector>& nD) const
{
	_itCounter = 0;

	QGAMES::MazeModel::PathInMaze result = 
		shortestWayToGoToImplementation (p1, p2, QGAMES::MazeModel::PathInMaze { }, nD, true);

	return (result);
}

// ---
std::vector <QGAMES::MazeModel::PathInMaze> QGAMES::MazeModel::allClosestWaysToGoTo 
	(const QGAMES::MazeModel::PositionInMaze& p1, const QGAMES::MazeModel::PositionInMaze& p2,
	 const std::vector <QGAMES::Vector>& nD) const
{
	_itCounter = 0;

	std::vector <QGAMES::MazeModel::PathInMaze> result = 
		allWaysToGoToImplementation (p1, p2, QGAMES::MazeModel::PathInMaze { },
			nD, false /** Now path are taken into account even if they are not possible */);

	return (result);
}

// ---
QGAMES::MazeModel::PathInMaze QGAMES::MazeModel::closestAndFirstWayToGoTo 
	(const QGAMES::MazeModel::PositionInMaze& p1, const QGAMES::MazeModel::PositionInMaze& p2, 
	 const std::vector <QGAMES::Vector>& nD) const
{
	_itCounter = 0;

	std::vector <QGAMES::MazeModel::PathInMaze> ways = 
		allWaysToGoToImplementation (p1, p2, QGAMES::MazeModel::PathInMaze { }, 
			nD, false /** Now path are taken into account even if they are not possible */);

	// We have all possible ways (ordered based on directions but many maybe not possible), 
	// but the closest has still to be calculated...
	QGAMES::bdata d = __MAXBDATA__;
	QGAMES::MazeModel::PathInMaze result;
	for (auto i : ways)
	{
		QGAMES::bdata nD;
		if ((nD = i [(int) i.size () - 1].asVector ().distanceTo (p2.asVector ())) < d)
		{
			d = nD;
			result = i;
		}
	}

	return (result);
}

// ---
QGAMES::MazeModel::PathInMaze QGAMES::MazeModel::closestAndShortestWayToGoTo 
	(const QGAMES::MazeModel::PositionInMaze& p1, const QGAMES::MazeModel::PositionInMaze& p2, 
	 const std::vector <QGAMES::Vector>& nD) const
{
	_itCounter = 0;

	QGAMES::MazeModel::PathInMaze result = 
		shortestWayToGoToImplementation (p1, p2, QGAMES::MazeModel::PathInMaze { }, 
			nD, false /** Now closest ways are considered. */);

	return (result);
}

// ---
QGAMES::MazeModel::PathInMaze QGAMES::MazeModel::limitPathFromFollowing 
	(const QGAMES::MazeModel::PositionInMaze& p, const QGAMES::Vector& d) const
{
	QGAMES::MazeModel::PathInMaze result;

	result.push_back (p);

	QGAMES::MazeModel::PositionInMaze nP = p;
	bool mP = true;
	while (mP)
	{
		nP = nextPositionToFollowing (nP, d);
		if (mP = (nP != QGAMES::MazeModel::_noPosition))
			result.push_back (nP);
	}

	return (result);
}

// ---
bool QGAMES::MazeModel::verifyDirections (const std::vector <QGAMES::Vector>& d) const
{
	bool result = true;
	for (std::vector <QGAMES::Vector>::const_iterator i = d.begin (); 
			i != d.end () && result; i++)
		result = ((*i).posX () == __BD 1 || (*i).posX () == __BD -1 || (*i).posX () == __BD 0) &&
				 ((*i).posY () == __BD 1 || (*i).posY () == __BD -1 || (*i).posY () == __BD 0) &&
				 ((*i).posZ () == __BD 1 || (*i).posZ () == __BD -1 || (*i).posZ () == __BD 0);
	return (result);
}

std::vector <QGAMES::Vector> QGAMES::MazeModel::removeDirectionsFrom 
	(const std::vector <QGAMES::Vector>& d, const std::vector <QGAMES::Vector>& dR) const
{
	assert (verifyDirections (d) && verifyDirections (dR));

	if (dR.empty ())
		return (d); // Nothing to remove...so why to continue?

	std::vector <QGAMES::Vector> result;
	for (auto i : d)
		if (std::find (dR.begin (), dR.end (), i) == dR.end ())
			result.push_back (i);

	return (result);
}

// ---
std::vector <QGAMES::Vector> QGAMES::MazeModel::possibleDirectionsAt (const QGAMES::MazeModel::PositionInMaze& p) const
{
	std::vector <QGAMES::Vector> result = allPossibleDirectionsAt (p);
	assert (verifyDirections (result));
	return (result);
}

// ---
std::vector <QGAMES::Vector> QGAMES::MazeModel::getValidAndOrderedDirectionsPossibleFromTo 
	(const QGAMES::MazeModel::PositionInMaze& p1, const QGAMES::MazeModel::PositionInMaze& p2, 
	 const std::vector <QGAMES::Vector>& nD) const
{
	// Determine the possible directions to go from this point in the maze
	std::vector <QGAMES::Vector> result = possibleDirectionsAt (p1);
	// It is needed to "extract" the forbiden directions (received as parameter)
	result = removeDirectionsFrom (result, nD);
	// As the method possibleDirectionsAt could be overloaded in other classes
	// it is necessary to check that the information received is ok, using very internal methods.
	assert ((int) result.size () <= maxPossibleConnectionsPerPosition ());
	assert (verifyDirections (result));
	// In some occasions the way the different possible directions are going to be checked is important
	// so, the right method to order those is invoked!
	result = orderDirections (p1, p2, result); 
	// ...and again the result has to be checked! (orderDirections is also a user defined method)
	assert (verifyDirections (result));

	return (result);
}

// ---
std::vector <QGAMES::MazeModel::PathInMaze> QGAMES::MazeModel::allWaysToGoToImplementation 
	(const QGAMES::MazeModel::PositionInMaze& p1, const QGAMES::MazeModel::PositionInMaze& p2, 
	 QGAMES::MazeModel::PathInMaze& pChk, const std::vector <QGAMES::Vector>& nD, bool e) const
{
	_itCounter++;

	// To save code later...
	auto addResult = [](std::vector <QGAMES::MazeModel::PathInMaze>& r, 
		const QGAMES::MazeModel::PositionInMaze& p, const QGAMES::MazeModel::PathInMaze& pth) -> void
		{
			QGAMES::MazeModel::PathInMaze nPath;
			nPath.push_back (p);
			nPath.insert (nPath.end (), pth.begin (), pth.end ());
			r.push_back (nPath);
		};

	std::vector <QGAMES::MazeModel::PathInMaze> result;

	// Did the algorithm reach the last position?...
	if (p2 == p1)
	{
		result.push_back (QGAMES::MazeModel::PathInMaze { p1 }); 

		if (_TRACERECURSIVE)
			std::cout << "(" << _itCounter << ") at position: " << p1 << " goal reached" << std::endl;

		_itCounter--;

		return (result);
	}

	// The algorithm hasn't reached the target too,
	// let's see wehether there are more possibilities or not!

	// Is this potentially a position already visited in the past?
	if (std::find (pChk.begin (), pChk.end (), p1) != pChk.end ())
	{
		_itCounter--;

		return (result); // If it is, no way to continue...
						 // This is just to avoid infinite invocations inside this recursive method...
	}

	// Otherwise, let's try to find a new way to reach the target

	std::vector <QGAMES::Vector> dirs = getValidAndOrderedDirectionsPossibleFromTo (p1, p2, nD);

	// Then start to check the different ways...
	// Take into account that "dirs" can be empty!
	for (auto i : dirs)
	{
		if (_TRACERECURSIVE)
			std::cout << "(" << _itCounter << ") at position: " << p1 << " checking direction " << i << std::endl;

		QGAMES::MazeModel::PositionInMaze pN = nextPositionToFollowing (p1,i);
		if (pN != QGAMES::MazeModel::_noPosition)
		{
			pChk.push_back (p1); // Eventually the initial point is gonna be considered already as visited...
			std::vector <QGAMES::MazeModel::PathInMaze> paths = 
				allWaysToGoToImplementation (pN, p2, pChk, { }, e); // (recursive) 
			pChk.pop_back (); // Extract the last point used as trail of breadcrumbs, to be rady for next iteration

			// A no empty paths means that at least a path exists...
			if (!paths.empty ())
			{
				// There are tow ways to look for the final solution:
				// Either to look for the precise one, or to look for the closests ones...
				// e == TRUE we are looking for the precise one, so all results have to be considered!
				if (e)
				{
					for (auto j : paths)
					{
						if (_TRACERECURSIVE)
						{
							std::cout << "(" << _itCounter << ") at position " << p1 << " new path valid: " << p1;
							for (auto k : j)
								std::cout << "|" << k;
							std::cout << std::endl;
						}

						// Result only have the paths getting the final destination...
						// Think backward!. From the detination one reacheed to the initial point!
						addResult (result, p1, j);
					}
				}
				else
				{
					// Again, thinking backward, and due to the situation that a no empty result is asllowed,
					// the paths can contain points really farway from the final destination, 
					// So just the closest to the final destination has to be taken into account
					// unless there is no anything else before...
					QGAMES::bdata d = __MAXBDATA__;
					std::vector <QGAMES::MazeModel::PathInMaze> pthsToAdd;
					for (auto j : paths)
					{
						QGAMES::bdata nD = j [j.size () - 1].asVector ().distanceTo (p2.asVector ());
						if (pthsToAdd.empty ())
						{
							d = nD;

							pthsToAdd.push_back (j);
						}
						else
						{
							if (nD < d)
							{
								d = nD;

								pthsToAdd.clear ();
								pthsToAdd.push_back (j);
							}
							else
							if (nD == d)
							{
								// Only it is added if it is different to the previous...
								if (std::find (pthsToAdd.begin (), pthsToAdd.end (), j) == pthsToAdd.end ())
									pthsToAdd.push_back (j);
							}
						}
					}

					for (auto j : pthsToAdd)
					{
						if (_TRACERECURSIVE)
						{
							std::cout << "(" << _itCounter << ") at position " << p1 << " new proxy path valid: " << p1;
							for (auto k : j)
								std::cout << "|" << k;
							std::cout << std::endl;
						}

						addResult (result, p1, j);
					}
				}
			}
			else
			{
				if (_TRACERECURSIVE)
					std::cout << "(" << _itCounter << ")" << " at position " << p1 << " paths from: " << p1 << " to " << p2 
							  << " following " << i << " not possible, just visited" << std::endl;
			}
		}
		// The method nextPositionFollowing should be aligned with possibleDirectionsAt
		// But as both can be defined later by the user it is necessary to control it!
		// This is simple solution to check the consistency of both!
		else
		{
			__WLOGPLUS (std::string ("nextPositionToFollowing not well defined for position ") + 
						(std::ostringstream () << p1).str () + std::string (" to direction ") + (std::ostringstream () << i).str ());
		}
	}

	// When it is not needed to find the precise position and there hasn't been any iteration through the directions...
	// At least a path with the original point has to be considered
	if (!e && result.empty ())
	{
		if (_TRACERECURSIVE)
			std::cout << "(" << _itCounter << ") at position: " << p1 << " nothing found. Keeep this point" << std::endl;

		result.push_back (QGAMES::MazeModel::PathInMaze { p1 }); 
	}

	_itCounter--;

	return (result);
}

// ---
QGAMES::MazeModel::PathInMaze QGAMES::MazeModel::shortestWayToGoToImplementation
	(const QGAMES::MazeModel::PositionInMaze& p1, const QGAMES::MazeModel::PositionInMaze& p2, 
	 QGAMES::MazeModel::PathInMaze& pChk, const std::vector <QGAMES::Vector>& nD, bool e) const
{
	_itCounter++;

	// To save code later...
	auto newResult = [](QGAMES::MazeModel::PathInMaze& r, 
		const QGAMES::MazeModel::PositionInMaze& p, const QGAMES::MazeModel::PathInMaze& pth) -> void
		{
			r.clear ();
			r.push_back (p); 
			r.insert (r.end (), pth.begin (), pth.end ()); 
		};

	std::vector <QGAMES::MazeModel::PositionInMaze> result;

	// Did the algorithm reach the last position?...
	if (p2 == p1)
	{
		result.push_back (p1);

		if (_TRACERECURSIVE)
			std::cout << "(" << _itCounter << ") at position: " << p1 << " goal reached" << std::endl;

		_itCounter--;

		return (result); // It does, the target point has been reached, and added to the result!
	}

	// The algorithm hasn't reached the target too,
	// let's see wehether there are more possibilities or not!

	// Is this potentially a position already visited in the past?
	if (std::find (pChk.begin (), pChk.end (), p1) != pChk.end ())
	{
		_itCounter--;

		return (result); // If it is, no way to continue...
						 // This is just to avoid infinite invocations inside this recursive method...
	}

	// Otherwise, let's try to find a new way to reach the target

	std::vector <QGAMES::Vector> dirs = getValidAndOrderedDirectionsPossibleFromTo (p1, p2, nD);

	// Then start to check the different ways...
	// Take into account that "dirs" can be empty!
	for (auto i : dirs)
	{
		if (_TRACERECURSIVE)
			std::cout << "(" << _itCounter << ") at position: " << p1 << " checking direction " << i << std::endl;

		QGAMES::MazeModel::PositionInMaze pN = nextPositionToFollowing (p1,i);
		if (pN != QGAMES::MazeModel::_noPosition)
		{
			pChk.push_back (p1); // Eventually the initial point is gonna be considered already as visited...
			QGAMES::MazeModel::PathInMaze path = 
				shortestWayToGoToImplementation (pN, p2, pChk, { }, e); // (recursive) 
			pChk.pop_back (); // Extract the last point used as trail of breadcrumbs, to be rady for next iteration

			// A no empty path means a path exists...
			if (!path.empty ())
			{
				// There are two ways to get the final position:
				// Looking for the one and the only, or looking for the closest to it
				// if e == TRUE, we are looking for the precise one!
				if (e)
				{
					// The way is only added, if the number of steps from p1 to p2 es less
					// than the previous result, otherwise th new result is ignored.
					if ((result.empty ()) || (!result.empty () && ((path.size () + 1) < result.size ())))
					{
						if (_TRACERECURSIVE)
						{
							if (!result.empty ())
								std::cout << "(" << _itCounter << ") at position " << p1 << " previous valid path forgotten" << std::endl;
							std::cout << "(" << _itCounter << ") at position " << p1 << " new path valid: " << p1;
							for (auto j : path) 
								std::cout << "|" << j;
							std::cout << std::endl;
						}

						newResult (result, p1, path);
					}
				}
				else
				{
					// The way is only added if the "distance" from the last point of the current path to the target point
					// is less that the last point of the previous result to it!
					if (result.empty ())
					{
						if (_TRACERECURSIVE)
						{
							std::cout << "(" << _itCounter << ") at position " << p1 << " new path valid: " << p1;
							for (auto j : path) 
								std::cout << "|" << j;
							std::cout << std::endl;
						}

						newResult (result, p1, path);
					}
					else
					{
						QGAMES::bdata dtPthP2 = path [path.size () - 1].asVector ().distanceTo (p2.asVector ());
						QGAMES::bdata dtResult = result [result.size () - 1].asVector ().distanceTo (p2.asVector ()); 
						if(dtPthP2 < dtResult || (dtPthP2 == dtResult && ((path.size () + 1) < result.size ())))
						{
							if (_TRACERECURSIVE)
							{
								std::cout << "(" << _itCounter << ") at position " << p1 << " previous valid proxy path forgotten" << std::endl;
								std::cout << "(" << _itCounter << ") at position " << p1 << " new proxy path valid: " << p1;
								for (auto j : path) 
									std::cout << "|" << j;
								std::cout << std::endl;
							}

							newResult (result, p1, path);
						}
					}
				}
			}
			else
			{
				if (_TRACERECURSIVE)
					std::cout << "(" << _itCounter << ")" << " at position " << p1 << " path from: " << p1 << " to " << p2 
							  << " following " << i << " not possible, just visited" << std::endl;
			}

		}
		// The method nextPositionFollowing should be aligned with possibleDirectionsAt
		// But as both can be defined later by the user it is necessary to control it!
		// This is simple solution to check the consistency of both!
		else
		{
			__WLOGPLUS (std::string ("nextPositionToFollowing method not well defined for position ") + 
						(std::ostringstream () << p1).str () + std::string (" to direction ") + (std::ostringstream () << i).str ());
		}
	}

	// When it is not needed to find the precise position and there hasn't been any iteration through the directions...
	// The origin point has to be added...
	if (!e && result.empty ())
	{
		if (_TRACERECURSIVE)
			std::cout << "(" << _itCounter << ") at position: " << p1 << " nothing found. Keeep this point" << std::endl;

		result.push_back (p1);
	}

	_itCounter--;

	return (result);
}
