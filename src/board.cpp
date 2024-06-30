#include <Board/board.hpp>
#include <Board/boardgame.hpp>
#include <assert.h>
#include <sstream>

// ---
QGAMES::Board::Board (int id, const QGAMES::BoardSquares& sqs, 
	const QGAMES::BoardResources& rscs, const QGAMES::BoardElementProperties& pts)
	: QGAMES::BoardElement (id, rscs, pts),
	  _squares (sqs),
	  _game (NULL),
	  _leftUpCorner (QGAMES::Position::_cero)
{
	assert (_squares.size () != 0);

	for (QGAMES::BoardSquares::const_iterator i = _squares.begin (); 
			i != _squares.end (); i++)
	{
		observe ((*i).second); // The squares are observed...
		(*i).second -> setBoard (this);
	}
}

// ---
QGAMES::SetOfOpenValues QGAMES::Board::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::BoardElement::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	// Where the board is located...
	std::stringstream sS; sS << _leftUpCorner;
	result.addOpenValue (lE + 1, QGAMES::OpenValue (sS.str ()));

	// ...and the information comming from each square...
	QGAMES::SetOfOpenValues sVV (std::string (__QGAMES_RUNTIMEVALUESSQUARETYPE__));
	for (QGAMES::BoardSquares::const_iterator i = _squares.begin (); i != _squares.end (); i++)
		sVV.addSetOfOpenValues ((*i).second -> id (), (*i).second -> runtimeValues ());
	result.addSetOfOpenValues (lNE + 1, sVV);

	return (result);
}

// ---
void QGAMES::Board::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	QGAMES::SetOfOpenValues sVV = cCfg.setOfOpenValues (lNE);
	assert (sVV.name () == std::string (__QGAMES_RUNTIMEVALUESSQUARETYPE__));
	for (QGAMES::BoardSquares::const_iterator i = _squares.begin (); i != _squares.end (); i++)
		// Only if the square has info...
		if (sVV.existSetOfOpenValues ((*i).second -> id ()))
			(*i).second -> initializeRuntimeValuesFrom (sVV.setOfOpenValues ((*i).second -> id ()));
	cCfg.removeSetOfOpenValues (lNE);

	_leftUpCorner = cCfg.openValue (lE).strValue ();
	cCfg.removeOpenValue (lE);

	QGAMES::BoardElement::initializeRuntimeValuesFrom (cCfg);
}

// ---
QGAMES::BoardSquare* QGAMES::Board::square (int id)
{
	QGAMES::BoardSquares::const_iterator i = _squares.find (id);
	assert (i != _squares.end ());

	return ((*i).second);
}

// ---
QGAMES::BoardSquare* QGAMES::Board::square (const QGAMES::BoardElementProperties& pts)
{
	QGAMES::BoardSquare* result = NULL;
	for (QGAMES::BoardSquares::const_iterator i = _squares.begin ();
			i != _squares.end () && !result; i++)
		if ((*i).second -> matchProperties (pts))
			result = (*i).second;

	// The null result is allowed!!
	return (result);
}

// ---
QGAMES::BoardSquare* QGAMES::Board::squareOn (const QGAMES::Position& pos)
{
	QGAMES::BoardSquare* result = NULL;
	for (QGAMES::BoardSquares::const_iterator i = _squares.begin ();
			i != _squares.end () && !result; i++)
		if ((*i).second -> isIn (pos))
			result = (*i).second;

	// The null result is allowed!!
	return (result);
}

// ---
void QGAMES::Board::setBoardGame (QGAMES::BoardGame* g)
{
	assert (g);

	_game = g;
}

// ---
void QGAMES::Board::initialize ()
{
	QGAMES::BoardElement::initialize ();

	// Try to assign the position of the left up corner of the board
	// Reading the variable defined in the definition file
	// It there are nor anything, then a default value is assigned...
	_leftUpCorner = QGAMES::Position (
		__BD (existProperty (__QGAMES_BOARDPOSITIONXATTR__)
			? atoi (property (__QGAMES_BOARDPOSITIONXATTR__).c_str ()) 
			: __QGAMES_BOARDPOSITIONXDEFAULTVALUE__),
		__BD (existProperty (__QGAMES_BOARDPOSITIONYATTR__)
			? atoi (property (__QGAMES_BOARDPOSITIONYATTR__).c_str ()) 
			: __QGAMES_BOARDPOSITIONYDEFAULTVALUE__), 
		__BD (existProperty (__QGAMES_BOARDPOSITIONZATTR__)
			? atoi (property (__QGAMES_BOARDPOSITIONZATTR__).c_str ()) 
			: __QGAMES_BOARDPOSITIONZDEFAULTVALUE__));

	for (QGAMES::BoardSquares::const_iterator i = _squares.begin ();
			i != _squares.end (); i++)
		(*i).second -> initialize ();
}

// ---
void QGAMES::Board::inEveryLoop ()
{
	QGAMES::BoardElement::inEveryLoop ();
	for (QGAMES::BoardSquares::const_iterator i = _squares.begin ();
			i != _squares.end (); i++)
		(*i).second -> inEveryLoop ();
}

// ---
void QGAMES::Board::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	for (QGAMES::BoardSquares::const_iterator i = _squares.begin ();
			i != _squares.end (); i++)
		(*i).second -> drawOn (s, p);
	QGAMES::BoardElement::drawOn (s, p);
}

// ---
void QGAMES::Board::finalize ()
{
	QGAMES::BoardElement::finalize ();
	for (QGAMES::BoardSquares::const_iterator i = _squares.begin ();
			i != _squares.end (); i++)
		(*i).second -> finalize ();
}
