/** \addtogroup 3InARow */
/*@{*/

#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include <Board/boardinclude.hpp>

/** Represents the state of the player.
	In this case there are only two variables: The square origin of a movement
	and the square target of that movement. If the first one is -1 it would mean
	the piece has to be selected from the ones not put yet. */
class Player3InARowState : public QGAMES::BoardGamePlayer::State
{
	public:
	Player3InARowState ()
		: _squareOrigin (-1), _squareTarget (-1)
							{ }

	int squareOrigin () const
							{ return (_squareOrigin); }
	void setSquareOrigin (int s)
							{ _squareOrigin = s; }
	int squareTarget () const
							{ return (_squareTarget); }
	void setSquareTarget (int s)
							{ _squareTarget = s; }

	virtual void initialize ()
							{ _squareOrigin = -1; _squareTarget = -1; }

	private:
	int _squareOrigin;
	int _squareTarget;
};

/** Represents a player on the game. */
class Player3InARow : public QGAMES::BoardGamePlayer
{
	public:
	Player3InARow (int id, const QGAMES::BoardResources& rscs = QGAMES::BoardResources ())
		: QGAMES::BoardGamePlayer (id, rscs)
							{ }

	virtual void play ();

	private:
	virtual QGAMES::BoardGamePlayer::State* createState ();
};

#endif
  
// End of the file
/*@}*/