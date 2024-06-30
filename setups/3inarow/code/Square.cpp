#include "Square.hpp"
#include "Defs.hpp"

// ---
bool Square3InARow::isIn (const QGAMES::Position& pos)
{
	return (pos.posX () >= _leftUpCorner.posX () && pos.posX () <= (_leftUpCorner.posX () + _width) &&
		pos.posY () >= _leftUpCorner.posY () && pos.posY () <= (_leftUpCorner.posY () + _height));
}

// ---
void Square3InARow::initialize ()
{
	QGAMES::BoardSquare::initialize ();

	// After invoking the parent method, the internal 
	// Variables can or not be initialized..
	// If not, a default better value will be assigned!
	_row = (_row == __QGAMES_SQUAREROWDEFAULTVALUE__) ? (id () / 10) : _row;
	_column = (_column == __QGAMES_SQUARECOLUMNDEFAULTVALUE__) ? (id () % 10) : _column;
	_depth = 0; // It is not necessary in a 2D game...
	_width = (_width == __QGAMES_SQUARESIZEXDEFAULTVALUE__) ? __SQUARESIZEX__ : _width;
	_height = (_height == __QGAMES_SQUARESIZEYDEFAULTVALUE__) ? __SQUARESIZEY__ : _height;
	_deep = 0; // It is not necessary in a 2D game...
	
	// The same with the position of the lef up corner...
	if (_leftUpCorner == QGAMES::Position (__BD __QGAMES_SQUAREPOSITIONXDEFAULTVALUE__, 
			__BD __QGAMES_SQUAREPOSITIONYDEFAULTVALUE__, __BD __QGAMES_SQUAREPOSITIONZDEFAULTVALUE__))
		_leftUpCorner = QGAMES::Position (__BD (__POSSQUAREC__((_column - 1))),
			__BD (__POSSQUARER__((_row - 1))), __BD 0);
}
