/**	
 *	@file	
 *	File: main.cpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 16/04/2018 \n
 *	Description: Main programm for testing Room Game
 *	Versions: 1.0 Initial
 */

#include "stdafx.h"
#include <chrono>
#include <Room/rminclude.hpp>

std::ostream& operator << (std::ostream& o, const QGAMES::MazeModel::PathInMaze& pth)
{
	if (pth.empty ())
		return (o);

	o << pth [0];
	for (int i = 1; i < (int) pth.size (); i++)
		o << "|" << pth [i];

	return (o);
}

std::ostream& operator << (std::ostream& o, const std::vector <QGAMES::MazeModel::PathInMaze>& pths)
{
	if (pths.empty ())
		return (o);

	o << pths [0];
	for (int i = 1; i < (int) pths.size (); i++)
		o << std::endl << pths [i];

	return (o);
}

class MyMaze : public QGAMES::MazeModel
{
	public:
	MyMaze (int x, int y, const std::vector <std::string>& mp)
		: QGAMES::MazeModel (x, y, 1, std::vector <QGAMES::SetOfOpenValues> (x * y, QGAMES::SetOfOpenValues())),
		  _map (mp)
							{ }

	protected:
	virtual int maxPossibleConnectionsPerPosition () const override
							{ return (4); }
	virtual std::vector <QGAMES::Vector> allPossibleDirectionsAt (const QGAMES::MazeModel::PositionInMaze& p) const override;
	virtual QGAMES::MazeModel::PositionInMaze nextPositionToFollowing (const QGAMES::MazeModel::PositionInMaze& p, 
		const QGAMES::Vector& d) const override;

	private:
	std::vector <std::string> _map;
};

// ---
std::vector <QGAMES::Vector> MyMaze::allPossibleDirectionsAt (const QGAMES::MazeModel::PositionInMaze& p) const
{
	if (!isInMaze (p))
		return (std::vector <QGAMES::Vector> ());

	std::vector <QGAMES::Vector> result;
	std::string pI = _map [(p._positionY * sizeX ()) + p._positionX];
	assert ((int) pI.length () == 4);

	if (pI [0] == '1') result.push_back (QGAMES::Vector (__BD  0, __BD -1, __BD 0));
	if (pI [1] == '1') result.push_back (QGAMES::Vector (__BD  1, __BD  0, __BD 0));
	if (pI [2] == '1') result.push_back (QGAMES::Vector (__BD  0, __BD  1, __BD 0));
	if (pI [3] == '1') result.push_back (QGAMES::Vector (__BD -1, __BD  0, __BD 0));

	return (result);
}

// ---
QGAMES::MazeModel::PositionInMaze MyMaze::nextPositionToFollowing 
	(const QGAMES::MazeModel::PositionInMaze& p, const QGAMES::Vector& d) const
{
	std::vector <QGAMES::Vector> pD = allPossibleDirectionsAt (p);

	if (pD.empty ())
		return (QGAMES::MazeModel::_noPosition);

	return ((std::find (pD.begin (), pD.end (), d) != pD.end ()) ? p + d : QGAMES::MazeModel::_noPosition);
}

void testMaze (int x, int y, const std::vector <std::string>& m)
{
	MyMaze maze (x, y, m);

	QGAMES::MazeModel::PositionInMaze p1 (0,0,0);			// Reachable
	QGAMES::MazeModel::PositionInMaze p2 (0,y - 2,0);		// Reachable
	QGAMES::MazeModel::PositionInMaze p3 (x - 1,y - 1,0);	// Non reachable

	std::vector <QGAMES::MazeModel::PathInMaze> pths;
	QGAMES::MazeModel::PathInMaze pth;

	std::chrono::time_point<std::chrono::system_clock> startT, endT;

	std::cout << std::endl << std::endl << "Starting the test for: " << x << "," << y << std::endl;

	// Reachable
	std::cout << "REACHABLE {" << std::endl;

	std::cout << "ALLWAYSTOGO-----------------------------" << std::endl;
	startT = std::chrono::system_clock::now ();
	pths = maze.allWaysToGoTo (p1, p2);
	endT = std::chrono::system_clock::now ();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endT - startT).count() << " miliseconds" << std::endl;
	if (pths.empty ()) std::cout << "NONE" << std::endl;
	else std::cout << pths.size () << std::endl << pths << std::endl;

	std::cout << "FIRSTWAYTOGO----------------------------" << std::endl;
	startT = std::chrono::system_clock::now ();
	pth = maze.firstWayToGoTo (p1, p2);
	endT = std::chrono::system_clock::now ();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endT - startT).count() << " miliseconds" << std::endl;
	if (pth.empty ()) std::cout << "NONE" << std::endl;
	else std::cout << pth << std::endl;

	std::cout << "SHORTESTWAYTOGO-------------------------" << std::endl;
	startT = std::chrono::system_clock::now ();
	pth = maze.shortestWayToGoTo (p1, p2);
	endT = std::chrono::system_clock::now ();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endT - startT).count() << " miliseconds" << std::endl;
	if (pth.empty ()) std::cout << "NONE" << std::endl;
	else std::cout << pth << std::endl;

	std::cout << "ALLCLOSESTWAYSTOGO----------------------" << std::endl;
	startT = std::chrono::system_clock::now ();
	pths = maze.allClosestWaysToGoTo (p1, p2);
	endT = std::chrono::system_clock::now ();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endT - startT).count() << " miliseconds" << std::endl;
	if (pths.empty ()) std::cout << "NONE" << std::endl;
	else std::cout << pths.size () << std::endl << pths << std::endl;

	std::cout << "CLOSESTANDFIRSTWAYTOGO------------------" << std::endl;
	startT = std::chrono::system_clock::now ();
	pth = maze.closestAndFirstWayToGoTo (p1, p2);
	endT = std::chrono::system_clock::now ();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endT - startT).count() << " miliseconds" << std::endl;
	if (pth.empty ()) std::cout << "NONE" << std::endl;
	else std::cout << pth << std::endl;

	std::cout << "CLOSESTANDSHORTESTWAYTOGO---------------" << std::endl;
	startT = std::chrono::system_clock::now ();
	pth = maze.closestAndShortestWayToGoTo (p1, p2);
	endT = std::chrono::system_clock::now ();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endT - startT).count() << " miliseconds" << std::endl;
	if (pth.empty ()) std::cout << "NONE" << std::endl;
	else std::cout << pth << std::endl;

	std::cout << "} REACHABLE" << std::endl;

	// Non reachable
	std::cout << "UNREACHABLE {" << std::endl;

	std::cout << "ALLWAYSTOGO-----------------------------" << std::endl;
	startT = std::chrono::system_clock::now ();
	pths = maze.allWaysToGoTo (p1, p3);
	endT = std::chrono::system_clock::now ();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endT - startT).count() << " miliseconds" << std::endl;
	if (pths.empty ()) std::cout << "NONE" << std::endl;
	else std::cout << pths.size () << std::endl << pths << std::endl;

	std::cout << "FIRSTWAYTOGO----------------------------" << std::endl;
	startT = std::chrono::system_clock::now ();
	pth = maze.firstWayToGoTo (p1, p3);
	endT = std::chrono::system_clock::now ();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endT - startT).count() << " miliseconds" << std::endl;
	if (pth.empty ()) std::cout << "NONE" << std::endl;
	else std::cout << pth << std::endl;

	std::cout << "SHORTESTWAYTOGO-------------------------" << std::endl;
	startT = std::chrono::system_clock::now ();
	pth = maze.shortestWayToGoTo (p1, p3);
	endT = std::chrono::system_clock::now ();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endT - startT).count() << " miliseconds" << std::endl;
	if (pth.empty ()) std::cout << "NONE" << std::endl;
	else std::cout << pth << std::endl;

	std::cout << "ALLCLOSESTWAYSTOGO----------------------" << std::endl;
	startT = std::chrono::system_clock::now ();
	pths = maze.allClosestWaysToGoTo (p1, p3);
	endT = std::chrono::system_clock::now ();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endT - startT).count() << " miliseconds" << std::endl;
	if (pths.empty ()) std::cout << "NONE" << std::endl;
	else std::cout << pths.size () << std::endl << pths << std::endl;

	std::cout << "CLOSESTANDFIRSTWAYTOGO------------------" << std::endl;
	startT = std::chrono::system_clock::now ();
	pth = maze.closestAndFirstWayToGoTo (p1, p3);
	endT = std::chrono::system_clock::now ();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endT - startT).count() << " miliseconds" << std::endl;
	if (pth.empty ()) std::cout << "NONE" << std::endl;
	else std::cout << pth << std::endl;

	std::cout << "CLOSESTANDSHORTESTWAYTOGO---------------" << std::endl;
	startT = std::chrono::system_clock::now ();
	pth = maze.closestAndShortestWayToGoTo (p1, p3);
	endT = std::chrono::system_clock::now ();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(endT - startT).count() << " miliseconds" << std::endl;
	if (pth.empty ()) std::cout << "NONE" << std::endl;
	else std::cout << pth << std::endl;

	std::cout << "} UNREACHABLE" << std::endl;
}

#ifndef _CONSOLE
#include <SDL.h>
#ifdef __cplusplus
#define C_LINKAGE "C"
#else
#define C_LINKAGE
#endif /* __cplusplus */
#if _MSC_VER >= 1900
extern C_LINKAGE FILE __iob_func[3] = { *stdin,*stdout,*stderr };
#endif
extern C_LINKAGE int main(int argc, char *argv[])
#else
int _tmain (int argc, char *argv [])
#endif /* _CONSOLE */
{
#ifdef NDEBUG
	// In the release version, the resources are in a zip file
	// This instruction reads them and store in temporaly files when needed!
	// The resource reader is deleted at the end, and all the temporal file with them!
	QGAMES::InZipResourceReader rR 
		(std::string (".") + __PATH_SEPARATOR__ + std::string ("temp"));
#endif

	QGAMES::MazeModel::_TRACERECURSIVE = false;

	testMaze (2, 3, 
		// UP, RIGHT, DOWN, LEFT
		std::vector <std::string> { "0110", "0011",
									"1110", "1001",
									"1000", "0000" });

	testMaze (3, 4,
		// UP, RIGHT, DOWN, LEFT
		std::vector <std::string> { "0110", "0111", "0011",
									"1110", "1111", "1011",
									"1100", "1101", "1001",
									"0000", "0000", "0000" });

	testMaze (4, 5, 
		// UP, RIGHT, DOWN, LEFT
		std::vector <std::string> { "0110", "0111", "0111", "0011",
									"1110", "1111", "1111", "1011",
									"1110", "1111", "1111", "1011",
									"1100", "1101", "1101", "1001",
									"0000", "0000", "0000", "0000" });

	testMaze (5, 6, 
		// UP, RIGHT, DOWN, LEFT
		std::vector <std::string> { "0110", "0111", "0111", "0111", "0011",
									"1110", "1111", "1111", "1111", "1011",
									"1110", "1111", "1111", "1111", "1011",
									"1110", "1111", "1111", "1111", "1011",
									"1100", "1101", "1101", "1101", "1001",
									"0000", "0000", "0000", "0000", "0000" });

	testMaze (6, 7, 
		// UP, RIGHT, DOWN, LEFT
		std::vector <std::string> { "0110", "0111", "0111", "0111", "0111", "0011",
									"1110", "1111", "1111", "1111", "1111", "1011",
									"1110", "1111", "1111", "1111", "1111", "1011",
									"1110", "1111", "1111", "1111", "1111", "1011",
									"1110", "1111", "1111", "1111", "1111", "1011",
									"1100", "1101", "1101", "1101", "1101", "1001",
									"0000", "0000", "0000", "0000", "0000", "0000" });

	testMaze (7, 8, 
		// UP, RIGHT, DOWN, LEFT
		std::vector <std::string> { "0110", "0111", "0111", "0111", "0111", "0111", "0011",
									"1110", "1111", "1111", "1111", "1111", "1111", "1011",
									"1110", "1111", "1111", "1111", "1111", "1111", "1011",
									"1110", "1111", "1111", "1111", "1111", "1111", "1011",
									"1110", "1111", "1111", "1111", "1111", "1111", "1011",
									"1110", "1111", "1111", "1111", "1111", "1111", "1011",
									"1100", "1101", "1101", "1101", "1101", "1101", "1001",
									"0000", "0000", "0000", "0000", "0000", "0000", "0000" });

	return (0);
}
