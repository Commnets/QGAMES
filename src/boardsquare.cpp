#include <Board/boardsquare.hpp>
#include <Board/board.hpp>
#include <assert.h>
#include <sstream>

// ---
QGAMES::SetOfOpenValues QGAMES::BoardSquare::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::BoardElement::runtimeValues ();

	int lE = result.lastOpenValueId ();
	
	// Just where it is located...
	std::stringstream sS; sS << _leftUpCorner;
	result.addOpenValue (lE + 1, QGAMES::OpenValue (sS.str ()));

	return (result);
}

// ---
void QGAMES::BoardSquare::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_leftUpCorner = QGAMES::Position (cCfg.openValue (lE).strValue ());
	cCfg.removeOpenValue (lE);

	QGAMES::BoardElement::initializeRuntimeValuesFrom (cCfg);
}

// ---
QGAMES::BoardSquare* QGAMES::BoardSquare::squareAt (int p)
{
	QGAMES::BoardSquares::const_iterator i = _squaresConnected.find (p);
	assert (i != _squaresConnected.end ());

	return ((*i).second);
}

// ---
void QGAMES::BoardSquare::setSquareAt (int p, QGAMES::BoardSquare* s)
{
	assert (_board); // The board is needed to verify that the square 
					 // to link is part of the board that this square belongs to...
	assert (s); // Obviouslly the square to link can not be null...
	QGAMES::BoardSquares::const_iterator i = _board -> squares ().find (s -> id ());
	assert (i != _board -> squares ().end ()); // ...and ot has to be part of the same board...
	// so it is not possible to link squares from different boards...

	QGAMES::BoardSquares::const_iterator j = _squaresConnected.find (p);
	if (j != _squaresConnected.end ())
		_squaresConnected.erase (j); // If there is already another square at the same position...
	_squaresConnected.insert (BoardSquares::value_type (p, s));
}

// ---
void QGAMES::BoardSquare::setBoard (QGAMES::Board* b)
{
	assert (b); // The board can not be null...

	_board = b;
}

// ---
void QGAMES::BoardSquare::initialize ()
{
	// It is considered that the board can be a 3D board game.
	// It means that it can have squares in any point in the space interconnected
	// The squares are defined among many other thing through it position in the box
	// This position is defined by the row, the colum, and the depth
	// These elements can be defined inthe definition file
	// In attributes of the square.
	// If they are no defined, the a basic value is assigned.
	_row = existProperty (__QGAMES_SQUAREROWATTR__) 
		? atoi (property (__QGAMES_SQUAREROWATTR__).c_str ()) 
		: __QGAMES_SQUAREROWDEFAULTVALUE__;
	_column = existProperty (__QGAMES_SQUARECOLUMNATTR__) 
		? atoi (property (__QGAMES_SQUARECOLUMNATTR__).c_str ()) 
		: __QGAMES_SQUARECOLUMNDEFAULTVALUE__;
	_depth = existProperty (__QGAMES_SQUAREDEPTHATTR__)
		? atoi (property (__QGAMES_SQUAREDEPTHATTR__).c_str ()) 
		: __QGAMES_SQUAREDEPTHDEFAULTVALUE__;

	// Each sqaure can have size.
	// The attributes defining the size can be also included
	// in the definition file.
	// If they aren't, then 0 is the default value.
	_width = existProperty (__QGAMES_SQUARESIZEXATTR__) 
		? atoi (property (__QGAMES_SQUARESIZEXATTR__).c_str ()) 
		: __QGAMES_SQUARESIZEXDEFAULTVALUE__;
	_height = existProperty (__QGAMES_SQUARESIZEYATTR__) 
		? atoi (property (__QGAMES_SQUARESIZEYATTR__).c_str ()) 
		: __QGAMES_SQUARESIZEYDEFAULTVALUE__;
	_depth = existProperty (__QGAMES_SQUARESIZEZATTR__) 
		? atoi (property (__QGAMES_SQUARESIZEZATTR__).c_str ()) 
		: __QGAMES_SQUARESIZEZDEFAULTVALUE__;

	// Finally the square can be at an specific point in the space
	// that specifc position can again be included in the definition file
	// If it is not, then a no point will be the default position!
	_leftUpCorner = QGAMES::Position (
		__BD (existProperty (__QGAMES_SQUAREPOSITIONXATTR__)
			? atoi (property (__QGAMES_SQUAREPOSITIONXATTR__).c_str ()) 
			: __QGAMES_SQUAREPOSITIONXDEFAULTVALUE__),
		__BD (existProperty (__QGAMES_SQUAREPOSITIONYATTR__)
			? atoi (property (__QGAMES_SQUAREPOSITIONYATTR__).c_str ()) 
			: __QGAMES_SQUAREPOSITIONYDEFAULTVALUE__), 
		__BD (existProperty (__QGAMES_SQUAREPOSITIONZATTR__)
			? atoi (property (__QGAMES_SQUAREPOSITIONZATTR__).c_str ()) 
			: __QGAMES_SQUAREPOSITIONZDEFAULTVALUE__));
}
