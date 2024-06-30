/** \addtogroup 3InARow */
/*@{*/

#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include "Defs.hpp"
#include <Board/boardinclude.hpp>

/** The board of the tic tac toe game. */
class Board3InARow : public QGAMES::Board
{
	public:
	Board3InARow (const QGAMES::BoardSquares& sqs, 
			const QGAMES::BoardResources& rscs = QGAMES::BoardResources (),
			const QGAMES::BoardElementProperties& pts = QGAMES::BoardElementProperties ())
		: QGAMES::Board (__BOARD3INAROW__, sqs, rscs, pts)
							{ }

	virtual void initialize ();
	virtual void drawOn (QGAMES::Screen* s, 
		const QGAMES::Position& p = QGAMES::Position::_noPoint);
};

#endif
  
// End of the file
/*@}*/