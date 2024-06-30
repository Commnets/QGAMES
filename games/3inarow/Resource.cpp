#include "Resource.hpp"
#include "Square.hpp"
#include "Defs.hpp"

// ---
void PieceThreeInARow::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	// The resource (that is a potential piece in the board)
	// has to be assigned to a specific square to be drawn...
	// Otherwise nothing is drawn
	if (boardSquare () != NULL) 
	{
		int sId = boardSquare () -> id ();
		int frm = (group () == __GROUPPLAYER1__) ? __FORMPLAYER1__ : __FORMPLAYER2__;
		boardElement () -> game () -> form (__3INAROWPIECEFORM__) -> 
			frame (frm) -> drawOn (s, ((Square3InARow*) boardSquare ()) -> leftUpCorner () + 
				QGAMES::Position (__BD 2, __BD 2, __BD 0), 255); 
		// The form select depends on the id of the board element owning it...
		// It is supposed this element is a player!
	}
}

// ---
QGAMES::BoardResource::State* PieceThreeInARow::createState ()
{
	// The default status is enough...
	// The status of the piece is not used at all!!
	QGAMES::BoardResource::State* result = new QGAMES::BoardResource::State ();
	result -> initialize (); // It does nothing by default...
	return (result);
}
