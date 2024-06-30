/** \ingroup Platform */
/** \ingroup Strcuture */
/*@{*/

/**	
 *	@file	
 *	File: ptwayinmaze.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 21/08/2018 \n
 *	Description: Same basic classes to manage things a maze.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_PTWAYINAMAZE__
#define __QGAMES_PTWAYINAMAZE__

#include <Platform/ptdefinitions.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace QGAMES
{
	class MazeModel
	{
		public:
		/** To describe a position in the maze. 
			The info is 3D coordinates. */
		struct PositionInMaze 
		{
			public:
			PositionInMaze ()
				: _positionX (0), _positionY (0), _positionZ (0)
							{ }

			PositionInMaze (const Vector& v)
				: _positionX ((int) v.posX ()), _positionY ((int) v.posY ()), _positionZ ((int) v.posZ ())
							{ }

			PositionInMaze (int pX, int pY, int pZ)
				: _positionX (pX), _positionY (pY), _positionZ (pZ)
							{ }

			Vector asVector () const
							{ return (Vector (__BD _positionX, __BD _positionY, __BD _positionZ)); }

			bool operator == (const PositionInMaze& p) const
							{ return (_positionX == p._positionX && 
									  _positionY == p._positionY &&
									  _positionZ == p._positionZ); }
			bool operator != (const PositionInMaze& p) const
							{ return (_positionX != p._positionX ||
									  _positionY != p._positionY ||
									  _positionZ != p._positionZ); }
			PositionInMaze operator + (const Vector& d) const
							{ return (PositionInMaze (_positionX + (int) d.posX (), 
													  _positionY + (int) d.posY (),
													  _positionZ + (int) d.posZ ())); }
			PositionInMaze& operator += (const Vector& d)
							{ *this = *this + d; return (*this); }
			PositionInMaze operator - (const Vector& d) const
							{ return (*this + (-d)); }
			PositionInMaze& operator -= (const Vector& d)
							{ return (*this += -d); }

			friend std::ostream& operator << (std::ostream& oS, const PositionInMaze& p)
							{ oS << p._positionX << "," << p._positionY << "," << p._positionZ;
							  return (oS); }
			friend std::istream& operator >> (std::istream& iS, PositionInMaze& p)
							{ iS >> p._positionX >> p._positionY >> p._positionZ; 
							  return (iS); }

			int _positionX;
			int _positionY;
			int _positionZ;
		};

		static PositionInMaze _noPosition;

		static bool _TRACERECURSIVE;

		typedef std::vector <PositionInMaze> PathInMaze;

		/**
		 *	To represent the maze three things are needed:
		 *	The size of the maze (it can be in three dimensions), 
		 * and a set of elements (numbers) to identify type of the room. */
		MazeModel ()
			: _sizeX (-1), _sizeY (-1), _sizeZ (-1), _mazeModel (),
			  _itCounter (0)
							{ }

		MazeModel (int sX, int sY, int sZ, const std::vector <SetOfOpenValues>& mM)
			: _sizeX (sX), _sizeY (sY), _sizeZ (sZ), _mazeModel (mM),
			  _itCounter (0)
							{ assert (_sizeX > 0 && _sizeY > 0 && _sizeZ > 0 &&
									  (int) _mazeModel.size () == (_sizeX * _sizeY * _sizeZ)); }

		virtual ~MazeModel () 
							{ }

		/** To get / set the info of the maze from a set of values. */
		virtual SetOfOpenValues asSetOfOpenValues () const;
		virtual void fromSetOfOpenValues (const SetOfOpenValues& cfg);

		/** To know the size of the maze. */
		int sizeX () const
							{ return (_sizeX); }
		int sizeY () const
							{ return (_sizeY); }
		int sizeZ () const
							{ return (_sizeZ); }

		/** To know whether a position is or not in Maze. */
		bool isInMaze (const PositionInMaze& p) const
							{ return (p._positionX < _sizeX && p._positionY < _sizeY && p._positionZ < _sizeZ &&
									  p._positionX >= 0 && p._positionY >= 0 && p._positionZ >= 0); }
		/** To know the direction between two positions. */
		Vector visualDirectionBetween (const PositionInMaze& p1, const PositionInMaze& p2) const
							{ return (p2.asVector () - p1.asVector ()); }

		// Mnanaging the info stored in the maze...
		/** To get the information about a position. */
		const SetOfOpenValues& mazeInfoAt (const PositionInMaze& p) const;
		/** To store information in a position. */
		void setMazeInfoAt (const PositionInMaze& p, const SetOfOpenValues& oV);
		/** Here it is a vector with first maze info at lowest z coordinate possible, y and x. */
		std::vector <SetOfOpenValues> mazeInfoBetween (const PositionInMaze& p1, const PositionInMaze& p2) const;
		/** To know whether is possible or not to move in one direction. */
		bool isPossibleToMoveTo (const PositionInMaze& p1, const Vector& d) const;
		/** To know the direction to go from a position to another. 
			One position should be next to the other, and it has to be reachable, 
			otherwise a null vector is returned. */
		Vector directionToGetFromTo (const PositionInMaze& p2, const PositionInMaze& p1) const;
		/** To get al positions in the maze that have any typ of connction with other. */
		PathInMaze allPositionsConnected () const;

		// Different ways of finding a path...
		/** To find all ways to go from a position to another. \n
			If the final position is not reachable from p1, then an empty vector is returned. */
		std::vector <PathInMaze> allWaysToGoTo (const PositionInMaze& p1, const PositionInMaze& p2,
			const std::vector <Vector>& nD = { }) const;
		/** To find the first way to go to. \n
			All way are calculated and only the first is finally returned. \n
			Which will be the first to be returned? It will depends on the ways directions are order. \n
			So for this algorithm the most important this is how orderDirections() is implemented. */
		PathInMaze firstWayToGoTo (const PositionInMaze& p1, const PositionInMaze& p2, const std::vector <Vector>& nD = { }) const;
		/** To find the shortest way between two positions in the maze. 
			empty vector if no way is possible at all. */
		PathInMaze shortestWayToGoTo (const PositionInMaze& p1, const PositionInMaze& p2, const std::vector <Vector>& nD = { }) const;
		/** To find all ways to go from a position to another. \n
			If the final position is reachable the outcome will be allWaysToGo. */
		std::vector <PathInMaze> allClosestWaysToGoTo (const PositionInMaze& p1, const PositionInMaze& p2,
			const std::vector <Vector>& nD = { }) const;
		/** To find the closest and the first way to go from a point to another.
			If the final position is reachable the outcome will be like firstWayToGoTo. */
		PathInMaze closestAndFirstWayToGoTo (const PositionInMaze& p1, const PositionInMaze& p2, 
			const std::vector <Vector>& nD = { }) const;
		/** To find the closest and th shortest way to go from a point to another. \n
			If the final position is reachable, the outcome will be like shortestWayToGoTo. */
		PathInMaze closestAndShortestWayToGoTo (const PositionInMaze& p1, const PositionInMaze& p2, 
			const std::vector <Vector>& nD = { }) const;
		/** To get the limit path possible advancing in the same direction. */
		PathInMaze limitPathFromFollowing (const PositionInMaze& p, const Vector& d) const;

		// To test things over the diffrent ways...
		/** To know whether a position is or not reachable from other. */
		bool isPositionReachable (const PositionInMaze& p1, const PositionInMaze& p2, const std::vector <Vector>& nD) const
							{ return (!shortestWayToGoTo (p1, p2, nD).empty ()); }

		protected:
		// To manage the information about connections
		// The methods can be overloaded to be adapted to the specific maze...
		/** How to interpret the possible directions in the maz depends on this.
			So, this method returns the max possibilities. \n
			By default: a 2D MAZE is considered and 8 the possibilities: \n
			RIGHT, RIGHT-DOWN, DOWN, DOWN-LEFT, LEFT, LEFT_UP, UP, UP-RIGHT */
		virtual int maxPossibleConnectionsPerPosition () const
							{ return (8 /** the allPossibleDirectionsAt default vector size. */); }
		/** To get all possible directions from a position in the maze. \n
			By default, all of them are possible whatever the position in the maze is. */
		virtual std::vector <Vector> allPossibleDirectionsAt (const PositionInMaze&) const
							{ return (std::vector <QGAMES::Vector> 
										({	QGAMES::Vector (__BD  1, __BD  0, __BD 0),			// RIGHT
											QGAMES::Vector (__BD  1, __BD  1, __BD 0),			// RIGHT-DOWN
											QGAMES::Vector (__BD  0, __BD  1, __BD 0),			// DOWN
											QGAMES::Vector (__BD -1, __BD  1, __BD 0),			// DOWN-LEFT
											QGAMES::Vector (__BD -1, __BD  0, __BD 0),			// LEFT
											QGAMES::Vector (__BD -1, __BD -1, __BD 0),			// LEFT-UP
											QGAMES::Vector (__BD  0, __BD -1, __BD 0),			// UP
											QGAMES::Vector (__BD  1, __BD -1, __BD 0) })); }	// UP-RIGHT
		/** To get the next position to another following a direction. \n 
			By default is to add the vector to the position and verifying whether the reesult is or not in the maze. \n
			If not, a _noPositionis returned. */
		virtual PositionInMaze nextPositionToFollowing (const PositionInMaze& p, const Vector& d) const
							{ PositionInMaze result = p + d; return (isInMaze (result) ? result : _noPosition); }
		/** To order the directions to a position starting from other.
			By default the method doesn't do anything. But it can be overloaded later. */
		virtual std::vector <Vector> orderDirections (const PositionInMaze&, const PositionInMaze&, 
			const std::vector <Vector>& dirs) const
							{ return (dirs); }

		protected:
		// Implementation
		/** To verify that a set of directions are right (-1,1 or 0 in every coordinate). \n
			It is used internally just to avoid repeating the same verifications once and another. */
		bool verifyDirections (const std::vector <Vector>& d) const;
		/** To extract a list of vectors from another. */
		std::vector <Vector> removeDirectionsFrom (const std::vector <Vector>& d, const std::vector <Vector>& dR) const;
		/** To get all possible directions from a position. */
		std::vector <Vector> possibleDirectionsAt (const PositionInMaze&) const;
		/** Using all the previous one. */
		std::vector <Vector> getValidAndOrderedDirectionsPossibleFromTo 
			(const PositionInMaze& p1, const PositionInMaze& p2, const std::vector <Vector>& nD) const;

		/** To get the list of all paths from a position to another. \n
			The method ins invoked by many others. \n
			The method is recursive, so the pre last parameter represents the previous already visited positions in a path
			to avoid repeating them. \n
			The last parameter (e) indicates whether the final position has to be the only one to be reached if possible,	
			or it could be enough finding the closest one. */		
		std::vector <PathInMaze> allWaysToGoToImplementation (const PositionInMaze& p1, const PositionInMaze& p2, 
			PathInMaze& pChk, const std::vector <Vector>& nD, bool e) const;
		/** The shortest way. The parameters have the same meaning that above. */
		PathInMaze shortestWayToGoToImplementation (const PositionInMaze& p1, const PositionInMaze& p2, 
			PathInMaze& pChk, const std::vector <Vector>& nD, bool e) const;

		protected:
		int _sizeX, _sizeY, _sizeZ;
		std::vector <SetOfOpenValues> _mazeModel;

		private:
		/** Just to trace recursive methods when defined. Very internal definition. */
		mutable int _itCounter;
	};
}

#endif
  
// End of the file
/*@}*/

