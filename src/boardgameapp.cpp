#include <Board/boardgameapp.hpp>
#include <assert.h>

// ---
void QGAMES::BoardGameApplication::drawOn (QGAMES::Screen* s)
{
	QGAMES::Game::drawOn (s);
	assert (_boardGame);

	_boardGame -> drawOn (s); // It should have always something, because it was init...
}

// ---
QGAMES::BoardGameApplication::~BoardGameApplication ()
{
	delete (_boardBuilder);
	_boardBuilder = NULL;
	delete (_boardGame); 
	_boardGame = NULL;
}

// ---
QGAMES::BoardGamePlayer* QGAMES::BoardGameApplication::getPlayer (int id)
{
	assert (_boardGame);

	return (_boardGame -> player (id));
}

// ---
QGAMES::BoardGamePlayer* QGAMES::BoardGameApplication::whoHasTurn ()
{
	assert (_boardGame);

	return (_boardGame -> whoHasTurn ());
}

// ---
QGAMES::BoardResource* QGAMES::BoardGameApplication::resource (int id)
{
	assert (_boardGame);

	return (_boardGame -> resource (id));
}

// ---
QGAMES::BoardResource* QGAMES::BoardGameApplication::resource 
	(const QGAMES::BoardElementProperties& pts)
{
	assert (_boardGame);

	return (_boardGame -> resource (pts));
}

// ---
QGAMES::BoardSquare* QGAMES::BoardGameApplication::square (int id)
{
	assert (_boardGame);

	return (_boardGame -> square (id));
}

// ---
QGAMES::BoardSquare* QGAMES::BoardGameApplication::square (const QGAMES::BoardElementProperties& pts)
{
	assert (_boardGame);

	return (_boardGame -> square (pts));
}

// ---
QGAMES::BoardSquare* QGAMES::BoardGameApplication::squareOn (const QGAMES::Position& pos)
{
	assert (_boardGame);

	return (_boardGame -> squareOn (pos));
}

// ---
std::string QGAMES::BoardGameApplication::defaultParameter (const std::string& p) const
{
	std::string result (__NULL_STRING__);

	if (p == std::string (__GAME_PROPERTYBOARDSFILE__))
		result = std::string (__GAME_BOARDSDEFINITIONSTDFILE__);
	else 
		result = QGAMES::Game::defaultParameter (p); // Looks at the parent class...

	return (result);

}

// ---
void QGAMES::BoardGameApplication::initialize ()
{
	if (isInitialized ())
		return; // Just once...

	QGAMES::Game::initialize ();

	// If the previous initialization was wrong, nothing can't continue...
	if (!isInitialized ())
		return;

	(_preLoad) ? boardBuilder () -> preLoad () : boardBuilder ();
	_boardGame = createBoardGame (); // Sets the initial game...
	assert (_boardGame);
	observe (_boardGame); // The board game is observed...
	_boardGame -> initialize ();
}

// ---
void QGAMES::BoardGameApplication::inEveryLoop ()
{
	QGAMES::Game::inEveryLoop ();

	assert (_boardGame);

	_boardGame -> inEveryLoop (); // It should have always something, because it was init...
}

// ---
void QGAMES::BoardGameApplication::finalize ()
{
	QGAMES::Game::finalize ();

	assert (_boardGame);

	_boardGame -> finalize (); // It should have alwways something becasue it was init...
}
