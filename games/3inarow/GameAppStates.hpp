/** \addtogroup 3InARow */
/*@{*/

#ifndef __GAMEAPPSTATES_HPP__
#define __GAMEAPPSTATES_HPP__

#include "Defs.hpp"
#include "Square.hpp"
#include <Board/boardinclude.hpp>

/** A class to define text drawn
	during the presentation. */
class PresentationText3InARow : public QGAMES::ScoreObjectText
{
	public:
	PresentationText3InARow (const std::string& t = std::string (__NULL_STRING__));

	virtual Entity* clone () const
							{ return (new PresentationText3InARow (_text)); }

	protected:
	/** @see parent. */
	virtual int formToDrawLetters ()
							{ return (__INTROLETTERSFORM__); }
};

/** Representing the introduction state,
	in which the basic background of the game and the logo of 
	the game maker appears. */
class GameApplicationIntroState : public QGAMES::GameState
{
	public:
	GameApplicationIntroState (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEINTRO__, g),
		  _wantToExit (false),
		  _text (NULL),
		  _counter (0),
		  _light (255),
		  _startDissapear (false)
							{ }

	void setWantToExit (bool wE)
							{ _wantToExit = wE; }

	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	bool _wantToExit;
	PresentationText3InARow* _text;
	int _counter;
	int _light;
	bool _startDissapear;
};

/** Represents the state in which the player selects the square to move a piece. */
class GameApplicationSelectSquareState : public QGAMES::GameState
{
	public:
	GameApplicationSelectSquareState (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATESELECTSQUARE__, g),
		  _squareSelected (-1),
		  _counter (0), _light (255), _direction (-1),
		  _turnOver (-1)
							{ }

	int idSquareSelected () const
							{ return (_squareSelected); }

	void squareRight ();
	void squareLeft ();
	void squareUp ();
	void squareDown ();
	void squareSelected ();
	/** When the mouse is used to select one square. */
	void squareClickedOn (int x, int y);
	/** Whewn the mouse button is left. */
	void squareSelectedOn (int x, int y);

	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);

	private:
	int _squareSelected;
	int _turnOver;

	// Implementation
	int _counter;
	int _light;
	int _direction;
};

/** The state used when the play happens. */
class GameApplicationPlayingState : public QGAMES::GameState
{
	public:
	GameApplicationPlayingState (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEPLAYING__, g),
		  _nextInternalSituation (_SELECTINGORIGIN),
		  _squareOrigin (-1), _squareTarget (-1)
							{ }

	virtual void restart ()
							{ _nextInternalSituation = _SELECTINGORIGIN;
							  _squareOrigin = _squareTarget = -1; }
	virtual void onEnter ();

	private:
	typedef enum 
		{ _SELECTINGORIGIN = 0, _SELECTINGTARGET, _PLAYING } InternalSituations;
	InternalSituations _nextInternalSituation;
	int _squareOrigin, _squareTarget;
};

/** Class representing the state of the game when someone wins. */
class GameApplicationWinsState : public QGAMES::GameState
{
	public:
	GameApplicationWinsState (QGAMES::Game* g)
		: QGAMES::GameState (__GAMESTATEWINNER__, g),
		  _counter (0),
		  _wantToExit (false)
							{ }

	void setWantToExit (bool wE)
							{ _wantToExit = wE; }

	virtual void onEnter ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s);
	virtual void onExit ();

	private:
	int _counter;
	bool _wantToExit;
}; 

#endif
  
// End of the file
/*@}*/
