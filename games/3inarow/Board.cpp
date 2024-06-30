#include "Board.hpp"

// ---
void Board3InARow::initialize ()
{
	QGAMES::Board::initialize ();

	// After initializing the board element the left up corner 
	// could still be the default one. If it is,
	// the the right one in this game is assigned!
	if (_leftUpCorner == QGAMES::Position (__BD __QGAMES_BOARDPOSITIONXDEFAULTVALUE__,
			__BD __QGAMES_BOARDPOSITIONYDEFAULTVALUE__, __BD __QGAMES_BOARDPOSITIONZDEFAULTVALUE__))
		_leftUpCorner = QGAMES::Position (__BD __POSXBOARD__, __BD __POSYBOARD__, __BD 0);
}

// ---
void Board3InARow::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	// First of all, the background...
	QGAMES::Game::game () -> form (__3INAROWBACKGROUNDFORM__) ->
		drawOn (s, __BASICBACKGROUNDFORM__, QGAMES::Position (__BD 0, __BD 0, __BD 0), 255);
	QGAMES::Game::game () -> form (__3INAROWTABLEFORM__) -> 
		drawOn (s, __FORMBOARD__, _leftUpCorner, 255 /** Not transparent */);

	// Then the squares and the elements i it...
	QGAMES::Board::drawOn (s, p);
}
