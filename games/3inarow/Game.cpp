#include "Game.hpp"
#include "Square.hpp"

// ---
TheGame::TheGame (const QGAMES::Boards& b, const QGAMES::BoardGameRules& rules, 
	const QGAMES::BoardGamePlayers& p,
	const QGAMES::BoardResources& rscs)
		: QGAMES::BoardGame (__GAMEID__, b, rules, p, rscs)
{
	_buoys.insert (QGAMES::Buoys::value_type (1, new GameBuoyWhenWin ()));
	// It is not needed to delete the buoys as they are deleted in the
	// default delete method of the class element, inheriting from this one!
}

// ---
bool VerifyThreeInARow::applyOver (QGAMES::BoardGame* b)
{
	assert (b);

	int** status = ((TheGame*) b) -> boardStatus ();

	// Starts to verify if there is any line with thre elements of the same color...
	// First the rows...
	int result = 0; 
	for (int r = 0; r < 3; r++)
		if (status [r][0] != 0 && 
			((status [r][0] == status [r][1] && (status [r][1]) == status [r][2])))
			result = status [r][0];
	
	// Then the columns, if nothing was found in the rows...
	if (result == 0)
		for (int c = 0; c < 3; c++)
			if (status [0][c] != 0 && 
				((status [0][c] == status [1][c]) && (status [1][c] == status [2][c])))
				result = status [0][c];

	// Finally the main diagonals...
	if (result == 0)
		if (status [0][0] != 0 && 
			((status [0][0] == status [1][1]) && (status [1][1] == status [2][2])))
			result = status [0][0];
	// ...and the reverse diagonal...
	if (status [0][2] != 0 && 
		((status [0][2] == status [1][1]) && (status [1][1] == status [2][0])))
			result = status [0][2];

	// If the result is not 0...we have a winner!!
	if (result != 0) 
	{
		#ifndef NDEBUG
		std::cout << "Game finishes. Player " << result << "wins" << std::endl;
		#endif

		notify (QGAMES::Event (__WEHAVEAWINNER__, b -> player (result)));
		((GameState*) b -> state ()) -> setWhoWins (result); // Modify the status of the game...
	}

	delete [] status;
	return (true);
}

// ---
int** TheGame::boardStatus ()
{
	// Creates a matrix with the status of the board...
	// INitially everything is 0
	int** status = new int*[3];
	for (int r = 0; r < 3; r++)
	{
		status [r] = new int [3];
		for (int c = 0; c < 3; status [r][c++] = 0);
	}

	// Then fills up the status of the board
	// with the information about the pieces inside
	// If 0, there is no piece. A number different than 0
	// means the id of player owning the piece!!
	// Iterates over all resources (pieces)...
	int rscId [6] = { __RESOURCE11__, __RESOURCE12__, __RESOURCE13__, 
		__RESOURCE21__, __RESOURCE22__, __RESOURCE23__ };
	for (int i = 0; i < 6; i++)
	{
		QGAMES::BoardResource* rsc = resource (rscId [i]);
		if (rsc -> boardSquare () != NULL) //...if the resource is located in an square...
		{
			Square3InARow* sq = ((Square3InARow*) rsc -> boardSquare ());
			status [sq -> row () - 1][sq -> column () - 1] = rsc -> boardElement () -> id (); 
			// ...assign the id of the player to the box...
		}
	}

	return (status);
	// Remember to destroy the element returned when used!
}

// ---
void TheGame::play ()
{
	QGAMES::BoardGame::play ();
	changeTurnTo ((whoHasTurn () -> id () == __PLAYER1__) ? __PLAYER2__ : __PLAYER1__);
	// Always change the turn, once the rules have been verified!
	((GameState*) state ()) -> setTurnOver (whoHasTurn () -> id ()); // Modify the status of the game
}

// ---
void TheGame::reset ()
{
	int rscId [6] = { __RESOURCE11__, __RESOURCE12__, __RESOURCE13__, 
		__RESOURCE21__, __RESOURCE22__, __RESOURCE23__ };
	for (int i = 0; i < 6; i++)
	{
		QGAMES::BoardResource * rsc = resource (rscId [i]);
		rsc -> setBoardSquare (NULL); // No square assigned...
	}

	changeTurnTo (__PLAYER1__);
}

// ---
void TheGame::initialize ()
{
	// When the game is reset, the resources (that are the different pieces of the game)
	// should belong again to the players, depending on the group of the resource
	// During the game the resources will be assigned to squares, but they will maintain
	// The relation with the players...
	int rscId [6] = { __RESOURCE11__, __RESOURCE12__, __RESOURCE13__, 
		__RESOURCE21__, __RESOURCE22__, __RESOURCE23__ };
	for (int i = 0; i < 6; i++)
	{
		QGAMES::BoardResource * rsc = resource (rscId [i]);
		int p = (rsc -> group () == __GROUPPLAYER1__) ? __PLAYER1__ : __PLAYER2__;
		rsc -> setBoardElement (player (p)); 
		// ...so next time this methos is executed, it will not effect at all!!
	}

	// Init the rest of the elements beaing part of the board game including:
	// the boards, the players, and the elements (if any..hopefully not)
	QGAMES::BoardGame::initialize ();
}

// ---
void TheGame::processEvent (const QGAMES::Event& e)
{
	// When the board sends the notification about a winner exists
	// the state can not be changed directly, because the main
	// loop is still processing the play action of the a player
	// in the state playing.
	// After that instructions, the status will change again to play
	// asking for a new position.
	// So, if the status is changed here, it will go back to the playing
	// status inmediatly. So the resources used is a bouy
	// The buoys are procesed in the main loop in the in the everyloop method
	// That method will look for the active bouys of the elements in
	// and will process their treat methods...
	if (e.code () == __WEHAVEAWINNER__)
	{
		buoy (__BUOYWHENWINID__) -> active (true); // Active the buoy...
		buoy (__BUOYWHENWINID__) -> setValue (__BD (whoHasTurn () -> id ())); 
		// The value of the buoy is the player winning...
	}
}

// ---
QGAMES::BoardGame::State* TheGame::createState ()
{
	QGAMES::BoardGame::State* result = new GameState ();
	result -> initialize ();
	return (result);
}

// ---
void* TheGame::GameBuoyWhenWin::treatFor (QGAMES::Element* e)
{
	// The traet method is who launches the change of the status eventually!
	e -> notify (QGAMES::Event (__CHANGESTATUSTOWIN__, 
		((TheGame*) e) -> player ((int) _value))); // Notify the action using the game...
	return (NULL);; // It doesn't matter...
}

