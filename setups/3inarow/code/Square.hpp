/** \addtogroup 3InARow */
/*@{*/

#ifndef __SQUARE_HPP__
#define __SQUARE_HPP__

#include <Board/boardinclude.hpp>

/** Represents an square in the Tic Tac Toe game. */
class Square3InARow : public QGAMES::BoardSquare
{
	public:
	Square3InARow (int id, const QGAMES::BoardResources& rscs = QGAMES::BoardResources (),
			const QGAMES::BoardElementProperties& pts = QGAMES::BoardElementProperties ())
		: QGAMES::BoardSquare (id, rscs, pts)
							{ }

	/** Just to verify whether the position received as parameter is or not
		in the square. True when it is, and false when it is not. */
	bool isIn (const QGAMES::Position& pos);

	virtual void initialize ();
};

#endif
  
// End of the file
/*@}*/