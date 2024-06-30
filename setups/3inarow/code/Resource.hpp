/** \addtogroup 3InARow */
/*@{*/

#ifndef __RESOURCE_HPP__
#define __RESOURCE_HPP__

#include <Board/boardinclude.hpp>

/** Represents one of the pieces the players can put in one of the squares of the game.
	The group received as parameter matches the id of the player the`piece belongs to. */
class PieceThreeInARow : public QGAMES::BoardResource
{
	public:
	PieceThreeInARow (int id, int grp, 
			const QGAMES::BoardResourceProperties& pts = QGAMES::BoardResourceProperties ())
		: QGAMES::BoardResource (id, grp, pts)
							{ }

	virtual void drawOn (QGAMES::Screen* s, 
		const QGAMES::Position& p = QGAMES::Position::_noPoint);

	private:
	virtual QGAMES::BoardResource::State* createState ();
};

#endif
  
// End of the file
/*@}*/