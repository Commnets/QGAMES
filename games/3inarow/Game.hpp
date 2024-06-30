/** \addtogroup 3InARow */
/*@{*/

#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "Defs.hpp"
#include <Board/boardinclude.hpp>

/** In this case, the state keeps who has the turn, and who wins. */
class GameState : public QGAMES::BoardGame::State
{
	public:
	GameState ()
		: _turnOver (0),
		  _whoWins (0)
							{ }

	virtual void initialize ()
							{ _turnOver = 0; _whoWins = 0; }

	int whoWins () const
							{ return (_whoWins); }
	void setWhoWins (int p)
							{ _whoWins = p; }
	int turnOver () const
							{ return (_turnOver); }
	void setTurnOver (int p)
							{ _turnOver = p; }

	private:
	int _turnOver;
	int _whoWins;
};

/** The unique rule in the game.
	it is responsable for verifying whether the board has or not a winner. 
	If it has, an event indicating it is thrown. */
class VerifyThreeInARow : public QGAMES::BoardGameRule
{
	public:
	VerifyThreeInARow ()
		: QGAMES::BoardGameRule ()
							{ }

	virtual bool applyOver (QGAMES::BoardGame* b);
};

/** The implementation of the game.
	It adds a method to know the status of the board. */
class TheGame : public QGAMES::BoardGame
{
	public:
	TheGame (const QGAMES::Boards& b, const QGAMES::BoardGameRules& rules, 
		const QGAMES::BoardGamePlayers& p,
		const QGAMES::BoardResources& rscs = QGAMES::BoardResources ());

	/** To know the status of the board.
		It returns a matrix with the status of every cell.
		The number in the cell indicates who (player) owns the cell there. */
	int** boardStatus ();

	virtual void play ();

	/** To reinit the game. */
	void reset ();

	virtual void initialize ();
	
	virtual void processEvent (const QGAMES::Event& e);

	private:
	virtual State* createState ();

	private:
	class GameBuoyWhenWin : public QGAMES::Buoy
	{
		public:
		GameBuoyWhenWin ()
			: QGAMES::Buoy (__BUOYWHENWINID__, __BD 0)
							{ }

		virtual void* treatFor (Element* e);
	};
};

#endif
  
// End of the file
/*@}*/
