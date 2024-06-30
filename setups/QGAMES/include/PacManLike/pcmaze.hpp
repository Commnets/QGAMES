/** \ingroup PACMAN */
/*@{*/

/**	
 *	@file	
 *	File: pcmaze.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 31/08/2021 \n
 *	Description: PACMAN maze main definition file. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMAN_MAZE__
#define __PACMAN_MAZE__

#include <PacManLike/pcdefs.hpp>

#include <advancedArcade/adinclude.hpp>
#include <Platform/ptwayinmaze.hpp>

namespace PACMAN
{
	/** 
	  *	An internal respresentation of the maze. \n
	  *	This Maze represents a 2D Maze where the artist can move in only 4 directions: up, down, right and left. \n
	  *	So, there are always 4 possible directions to connect a cell with the next: up, down, right o left. \n
	  *	In every position of the maze 4 OpenValues are kept to indicate whether those 4 directions are o not possible. \n
	  * In the same cell a 5th OpenValue indicates the number of zone the cell belong to. -1 means no zone assigned. \n
	  *	In same cells, when only the movement is a single direction is allowed,
	  *	a 6th element is defined, to indicate the position of the cell connected moving towards the
	  *	opposite direction to the only one allowed. It used to indicate tunnels! \n 
	  * It is possible to define in the maze mandatory direction zones. 
	  * That are zones where either vertical movement or horizontal on is madatory. \n 
	  * So, a 6th elementcould be defined to point restricted directions. 
	  * Those ones will be taken into account later by the algorithms used to find a path in the maze.
	  */
	class Map;
	class Maze final : public QGAMES::MazeModel
	{
		public:
		Maze () = delete; // Not possible (the constructor allowed is private)

		Maze (const Maze&) = default;

		Maze& operator = (const Maze&) = default;

		/** To know which is the area (if any) of a position. */
		int mazeZoneAt (const QGAMES::MazeModel::PositionInMaze& p) const
							{ return (isInMaze (p) 
								? (mazeInfoAt (p).existOpenValue (__PACMAN_MAZEINFOZONE__) 
									? mazeInfoAt (p).openValue (__PACMAN_MAZEINFOZONE__).intValue () : -1) 
								: -1); }

		/** To know the positions a maze zone is made of. 
			If the parameter were -1, then the positions selectd would be the ones with no area!. */
		const std::vector <QGAMES::MazeModel::PositionInMaze>& positionsForZone (int n) const;

		/** To generate a random position in the maze, but excluding zones. */
		QGAMES::MazeModel::PositionInMaze randomPosition (const std::vector <int>& z = { }) const;

		/** To know whether the position is or not a tunnel conection. */
		bool isTunnelConnectionAt (const QGAMES::MazeModel::PositionInMaze& p) const
							{ return (isInMaze (p) 
								? mazeInfoAt (p).existOpenValue (__PACMAN_MAZEINFOTUNNELCONNECTION__) : false); }

		/** ..and as a consequence, which is that connection (no position if there is no). */
		QGAMES::MazeModel::PositionInMaze tunnelConnectionAt (const QGAMES::MazeModel::PositionInMaze& p) const
							{ return (isTunnelConnectionAt (p) 
								? QGAMES::MazeModel::PositionInMaze 
									(QGAMES::Vector (mazeInfoAt (p).openValue (__PACMAN_MAZEINFOTUNNELCONNECTION__).strValue ())) 
								: QGAMES::MazeModel::_noPosition); }

		/** To know whether 2 positions are together. */
		bool arePositionsTogether (const QGAMES::MazeModel::PositionInMaze& p1, const QGAMES::MazeModel::PositionInMaze& p2) const;
		/** To know whether there are or not restricted movement directions at a position in the maze. */
		std::vector <QGAMES::Vector> restrictedDirectionsAt (const QGAMES::MazeModel::PositionInMaze& p) const;
		
		/** To get the pure grid x next position following a direction. \n
			If the position will be out of the maze, the last one possible will be returned. \n
			NOTE: This method tries to simulate one error in PacMan's game logic. \n
			When the direction grip position requested is up, the position 
			is also moved to the left in the same quantity of cells requested to be moved up. \n
			To indicate whether to apply the error or not a paramter (e) is defined.
			By default the error won't be applied. */
		QGAMES::MazeModel::PositionInMaze nextXGridPositionFollowing (const QGAMES::MazeModel::PositionInMaze& p,
			unsigned x, const QGAMES::Vector& d, bool e = false) const;

		/** In this case, because the size of th maze (usually 21 x 24), 
			the standard methods to calculate the path to go from a position to another
			will be very slow. \n
			So, a new one is be defined, but just only to calculate 2 steps. 
			The parameters received ar the initial point, the target, the list of directions to avoid
			and the preferred direction to chose if any. */
		QGAMES::MazeModel::PathInMaze next2StepsToGoTo (const QGAMES::MazeModel::PositionInMaze& p1, 
			const QGAMES::MazeModel::PositionInMaze& p2, const std::vector <QGAMES::Vector>& d, 
			const QGAMES::Vector& pD = QGAMES::Vector::_noPoint) const;
		/** In this case, the directions to avoid and the preferred direction to chose ar only for the initial movement...
			This method never looks back. */
		QGAMES::MazeModel::PathInMaze nextXStepsToGoTo (int x, const QGAMES::MazeModel::PositionInMaze& p1, 
			const QGAMES::MazeModel::PositionInMaze& p2, const std::vector <QGAMES::Vector>& d,
			const QGAMES::Vector& pD = QGAMES::Vector::_noPoint) const;

		/** Those methods are invoked from the PACMAN::Map to create a PACMAN::Maze. \n
			The methods guaranttee that nothing is wrong in the construction. */
		static Maze generateEmptyMaze (int x, int y);
		static Maze generateMazeFrom (int w, int h, const Map* mp);

		private:
		virtual int maxPossibleConnectionsPerPosition () const override
								{ return (4); } // LEFT, RIGH, UP & DOWN...
		virtual std::vector <QGAMES::Vector> allPossibleDirectionsAt (const QGAMES::MazeModel::PositionInMaze&) const override;
		virtual PositionInMaze nextPositionToFollowing 
			(const QGAMES::MazeModel::PositionInMaze& p, const QGAMES::Vector& d) const override;
		virtual std::vector <QGAMES::Vector> orderDirections (const QGAMES::MazeModel::PositionInMaze& p1, 
			const QGAMES::MazeModel::PositionInMaze& p2, const std::vector <QGAMES::Vector>& dirs) const override;

		/** The SetOfOpenValues has 4/5 values to indicate whether the movement to each of the 4 directions is possible. \n
			Value 0: LFT, Value 1: RIGHT, Value 2: UP, Value 3: DOWN. \n
			The 5th element, when exists, represents the zone where the cell is. \n
			The 6th define a possible connection with other cell (to simulat tunnels). \n
			The 7th ant the last is optional and describe restricted movement directions if any. */
		Maze (int sX, int sY, const std::vector <QGAMES::SetOfOpenValues>& mW)
			: QGAMES::MazeModel (sX, sY, 1 /* 2d maze always */, mW),
			  _positionsPerZone ()
							{ }

		private:
		// Implementation
		mutable std::map <int, std::vector <QGAMES::MazeModel::PositionInMaze>> _positionsPerZone;
	};
}

#endif
  
// End of the file
/*@}*/
