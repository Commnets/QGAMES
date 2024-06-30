#include "BoardBuilder.hpp"
#include "Board.hpp"
#include "Square.hpp"
#include "Resource.hpp"
#include "Defs.hpp"

// ---
QGAMES::Board* BoardBuilderAddsOn::createBoardInstance (int id, const QGAMES::BoardSquares& sqs, 
		const QGAMES::BoardResources& rscs)
{
	QGAMES::Board* result = NULL;
	if (id == __BOARD3INAROW__)
		result = new Board3InARow (sqs, rscs);
	else
		exit (1); // It shouldn't have this case...then just to control...
	return (result);
}

// ---
QGAMES::BoardSquare* BoardBuilderAddsOn::createSquareInstance (int id, 
		const QGAMES::BoardResources& rscs,
		const std::map <std::string, std::string> pts)
{
	QGAMES::BoardSquare* result = NULL;
	if (id == __SQUARE11__ || id == __SQUARE12__ || id == __SQUARE13__ ||
		id == __SQUARE21__ || id == __SQUARE22__ || id == __SQUARE23__ ||
		id == __SQUARE31__ || id == __SQUARE32__ || id == __SQUARE33__)
		result = new Square3InARow (id, rscs, pts);
	else
		exit (1); // It shouldn't happen this case...it is just to be sure...
	return (result);
}

// ---
QGAMES::BoardResource* BoardBuilderAddsOn::createResourceInstance (int id, int grp, 
		const std::map <std::string, std::string> pts)
{
	QGAMES::BoardResource* result = NULL;
	if (id == __RESOURCE11__ || id == __RESOURCE12__ || id == __RESOURCE13__ ||
		id == __RESOURCE21__ || id == __RESOURCE22__ || id == __RESOURCE23__)
		result = new PieceThreeInARow (id, grp, pts);
	else
		exit (1); // Big error in configuration...
	return (result);
}
