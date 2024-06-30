#include "Player.hpp"
#include "Game.hpp"
#include "Square.hpp"
#include "GameApp.hpp"

// ---
void Player3InARow::play ()
{
	// What to do depends whether there is or not an square origin...
	// It is needed to select the square to move.
	// If there is not square origin, then the resource to move is one
	// The resources still assigned to the player having the turn
	QGAMES::BoardResource* piece = NULL;
	int sO = ((Player3InARowState*) state ()) -> squareOrigin ();
	if (sO == -1)
	{
		for (QGAMES::BoardResources::const_iterator i = _resources.begin ();
				i != _resources.end () && piece == NULL; i++)
			if ((*i).second -> boardSquare () == NULL)
				piece = (*i).second; // The piece not assigned to any square yet!
		assert (piece); // If everything works fine, the piece has to have been found!
	}
	// If there is an square origin the resource to move is 
	// the one assigned to the square selected.
	else
	{
		int rscId [6] = { __RESOURCE11__, __RESOURCE12__, __RESOURCE13__, 
			__RESOURCE21__, __RESOURCE22__, __RESOURCE23__ };
		for (int i = 0; i < 6 && piece == NULL; i++)
		{
			QGAMES::BoardResource* rsc = _game -> resource (rscId [i]);
			QGAMES::BoardSquare* sq = rsc -> boardSquare ();
			if (sq != NULL && sq -> id () == sO)
				piece = rsc;
		}

		assert (piece);
		// Following the rules of the game, piece can't be null after
		// these instructions...so assert it is just in case!!
	}

	// Once the resource to put in the target square has been
	// detected, then the movement happens
	Square3InARow* tSquare = ((Square3InARow*) 
		(_game -> square (((Player3InARowState*) state ()) -> squareTarget ())));
	piece -> setBoardSquare (tSquare);
	_game -> game () -> sound (__SOUNDSELECTBASE__ + id ()) -> play (-1); // Selection right...

	// Init the status back, thinking in the next situation...
	state () -> initialize ();
}

// ---
QGAMES::BoardGamePlayer::State* Player3InARow::createState ()
{
	QGAMES::BoardGamePlayer::State* result = new Player3InARowState ();
	result -> initialize ();
	return (result);
}


