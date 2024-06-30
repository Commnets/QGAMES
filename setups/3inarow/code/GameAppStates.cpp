#include "GameAppStates.hpp"
#include "GameApp.hpp"
#include "Game.hpp"
#include "Player.hpp"

// ---
PresentationText3InARow::PresentationText3InARow (const std::string& t)
		: QGAMES::ScoreObjectText (0, t, QGAMES::Forms (), 0)
{
	_forms.insert (QGAMES::Forms::value_type (__INTROLETTERSFORM__, 
		QGAMES::Game::game () -> form (__INTROLETTERSFORM__)));
	setText (t);
}

// ---
void GameApplicationIntroState::onEnter ()
{
	_wantToExit = false;
	_text = new PresentationText3InARow (std::string ("COPYRIGHT 2015 Ignacio Cea Fornies"));
	_text -> setSpace (-2); // Letters are tight!!
	_counter = 0;
	_light = 255;
	_startDissapear = false;

	// The introduction sound...
	_game -> sound (__SOUNDINTRODUCTION__) -> play (-1);
}

// ---
void GameApplicationIntroState::updatePositions ()
{
	if (_startDissapear)
	{
		if (++_counter >= (_game -> framesPerSecond () / 30))
		{
			_counter = 0;
			_light -= 10;
			if (_light <= 0)
			{
				_light = 0;
				_startDissapear = false;
				_game -> setState (__GAMESTATEPLAYINGNAME__); // Starts to play...
			}
		}
	}
	else
	if (++_counter >= (_game -> framesPerSecond () * 18))
	{
		_startDissapear = true;
		_counter = 0;
	}

	// The user has selected to exit the state and move the
	// game to the next point...
	if (_wantToExit)
		_game -> setState (__GAMESTATEPLAYINGNAME__);
}

// ---
void GameApplicationIntroState::drawOn (QGAMES::Screen* s)
{
	QGAMES::Game::game () -> form (__3INAROWBACGROUNDINITIALFORM__) ->
		drawOn (s, __INITIALBACGROUNDFORM__, QGAMES::Position (__BD 0, __BD 0, __BD 0), _light);
	QGAMES::Game::game () -> form (__3INAROWLOGOINITIALFORM__) ->
		drawOn (s, __LITTLELOGOFORM__, QGAMES::Position (__BD 20, __BD 460, __BD 0), _light);
	_text -> setAlphaLevel (_light);
	_text -> drawOn (s, QGAMES::Position (__BD 150, __BD 460, __BD 0));
}

// ---
void GameApplicationIntroState::onExit ()
{
	_game -> sound (__SOUNDINTRODUCTION__) -> stop (); // Stops the sound if any...
}

// ---
void GameApplicationSelectSquareState::squareRight ()
{
	if (_squareSelected == __SQUARE11__) _squareSelected = __SQUARE12__;
	else if (_squareSelected == __SQUARE12__) _squareSelected = __SQUARE13__;
	else if (_squareSelected == __SQUARE21__) _squareSelected = __SQUARE22__;
	else if (_squareSelected == __SQUARE22__) _squareSelected = __SQUARE23__;
	else if (_squareSelected == __SQUARE31__) _squareSelected = __SQUARE32__;
	else if (_squareSelected == __SQUARE32__) _squareSelected = __SQUARE33__;
}

// ---
void GameApplicationSelectSquareState::squareLeft ()
{
	if (_squareSelected == __SQUARE12__) _squareSelected = __SQUARE11__;
	else if (_squareSelected == __SQUARE13__) _squareSelected = __SQUARE12__;
	else if (_squareSelected == __SQUARE22__) _squareSelected = __SQUARE21__;
	else if (_squareSelected == __SQUARE23__) _squareSelected = __SQUARE22__;
	else if (_squareSelected == __SQUARE32__) _squareSelected = __SQUARE31__;
	else if (_squareSelected == __SQUARE33__) _squareSelected = __SQUARE32__;
}

// ---
void GameApplicationSelectSquareState::squareUp ()
{
	if (_squareSelected == __SQUARE21__) _squareSelected = __SQUARE11__;
	else if (_squareSelected == __SQUARE22__) _squareSelected = __SQUARE12__;
	else if (_squareSelected == __SQUARE23__) _squareSelected = __SQUARE13__;
	else if (_squareSelected == __SQUARE31__) _squareSelected = __SQUARE21__;
	else if (_squareSelected == __SQUARE32__) _squareSelected = __SQUARE22__;
	else if (_squareSelected == __SQUARE33__) _squareSelected = __SQUARE23__;
}

// ---
void GameApplicationSelectSquareState::squareDown ()
{
	if (_squareSelected == __SQUARE11__) _squareSelected = __SQUARE21__;
	else if (_squareSelected == __SQUARE12__) _squareSelected = __SQUARE22__;
	else if (_squareSelected == __SQUARE13__) _squareSelected = __SQUARE23__;
	else if (_squareSelected == __SQUARE21__) _squareSelected = __SQUARE31__;
	else if (_squareSelected == __SQUARE22__) _squareSelected = __SQUARE32__;
	else if (_squareSelected == __SQUARE23__) _squareSelected = __SQUARE33__;
}

// ---
void GameApplicationSelectSquareState::squareSelected ()
{
	_game -> setState (__GAMESTATEPLAYINGNAME__);
	// When exit the state nothing happens (except the last square highlighted is unhighlighted back), 
	// but the internal variable with the square selected remains...if any
	// So ir can be requested later on!
}

// ---
void GameApplicationSelectSquareState::squareClickedOn (int x, int y)
{
	QGAMES::BoardSquare* sq = 
		((GameApplication*) _game) -> squareOn (QGAMES::Position (__BD x, __BD y, __BD 0));
	if (sq != NULL)
		_squareSelected = sq -> id ();
}

// ---
void GameApplicationSelectSquareState::squareSelectedOn (int x, int y)
{
	QGAMES::BoardSquare* sq = 
		((GameApplication*) _game) -> squareOn (QGAMES::Position (__BD x, __BD y, __BD 0));
	if (sq != NULL)
	{
		_squareSelected = sq -> id ();
		squareSelected ();
	}
}

// ---
void GameApplicationSelectSquareState::onEnter ()
{
	_counter = 0; _light = 255; _direction = -1;
	if (_squareSelected == -1)
		_squareSelected = __SQUARE11__; // always starts from the left up corner!
	_turnOver = ((GameApplication*) _game) -> whoHasTurn () -> id ();
}

// ---
void GameApplicationSelectSquareState::updatePositions ()
{
	if (++_counter >= (_game -> framesPerSecond () / 30))
	{
		_counter = 0;
		_light += (10 * _direction);
		if (_light >= 255 || _light <= 100) _direction = -_direction;
		if (_light >= 255) _light = 255; // To change the direction of the light if any...
		if (_light <= 100) _light = 100;
	}
}

void GameApplicationSelectSquareState::drawOn (QGAMES::Screen* s)
{
	Square3InARow* sq = (Square3InARow*) ((GameApplication*) _game) -> square (_squareSelected);
	QGAMES::Position leftUp = sq -> leftUpCorner (); // The first corner...
	QGAMES::Position leftDown = leftUp + QGAMES::Vector (__BD 0, __BD sq -> height (), __BD 0);
	QGAMES::Position rightUp = leftUp + QGAMES::Vector (__BD sq -> width (), __BD 0, __BD 0);
	QGAMES::Position rightDown = leftUp + QGAMES::Vector (__BD sq -> width (), __BD sq -> height (), __BD 0);
	QGAMES::Color cl (_light, 0, 0, 255);
	if (_turnOver == __PLAYER2__)
		cl = QGAMES::Color (0, 0, _light, 255); // Other color used for the player 2...

	// The square is really made up of four lines...
	for (int i = 0; i < 4; i++)
	{
		s -> drawLine (leftUp + QGAMES::Position (__BD i, __BD 0, __BD 0), 
			leftDown + QGAMES::Position (__BD i, __BD 0, __BD 0), cl);
		s -> drawLine (leftDown - QGAMES::Position (__BD 0, __BD i, __BD 0), 
			rightDown - QGAMES::Position (__BD 0, __BD i, __BD 0), cl);
		s -> drawLine (rightDown - QGAMES::Position (__BD i, __BD 0, __BD 0), 
			rightUp - QGAMES::Position (__BD i, __BD 0, __BD 0), cl);
		s -> drawLine (rightUp + QGAMES::Position (__BD 0, __BD i, __BD 0), 
			leftUp + QGAMES::Position (__BD 0, __BD i, __BD 0), cl);
	}
}

// ---
void GameApplicationPlayingState::onEnter ()
{
	// If when enter the state comes from other different
	// than the selection of a square, it would mean
	// it is needed to restart the state!
	if (_previousState != NULL && _previousState -> type () != __GAMESTATESELECTSQUARE__)
		restart ();

	// Just to start...detects:
	// A status of the board...
	int** st = ((TheGame*) (((GameApplication*) _game) -> boardGame ())) -> boardStatus ();
	// The id of the player having the turn already...
	int trn = ((GameApplication*) _game) -> whoHasTurn () -> id ();
	// Whether all the pieces of that player are or not already on the board
	int nP = 0; 
	for (int r = 0; r < 3; r++) for (int c = 0; c < 3; c++) if (st [r][c] == trn) nP++;
	// Get the selection status, just in case it is needed...
	GameApplicationSelectSquareState* sS = (GameApplicationSelectSquareState*) 
		_game -> state (__GAMESTATESELECTSQUARENAME__);
	// ...and the id of the square selected!
	int sSID = sS -> idSquareSelected ();
	// ..and the square selected...is also part of the variables
	Square3InARow* sq = NULL;
	if (sSID != -1)
		sq = (Square3InARow*) ((GameApplication*) _game) -> square (sSID);

	// Depending on the situation, what to do is really different:
	switch (_nextInternalSituation )
	{
		// it is supposed, it's time to select the square origin
		// But just when all pieces are on the board!
		case _SELECTINGORIGIN:
			{
				// All pieces are on the board, 
				// so it is needed to select the piece to move first
				// That initial square has to have something valid!
				if (nP == 3) 
				{
					_nextInternalSituation = _SELECTINGTARGET; 
					// Next action, to select the target
					_game -> setState (__GAMESTATESELECTSQUARENAME__); 
					// ..and now to select the origin (it must be filled)
				}
				// They aren't so, it is only needed to select the target position
				// That target position must be empy!
				else
				{
					_squareOrigin = -1; 
					// No origin needed...
					_nextInternalSituation = _PLAYING; 
					// Next action, to play!
					_game -> setState (__GAMESTATESELECTSQUARENAME__); 
					// ...and now to select the target (it must be empty)
				}
			}

			break;
		
		// It is supposed, it's time to select the target
		case _SELECTINGTARGET:
			{
				// In this state we arrive from selecting the square origin,
				// so before continuing it is neccessary to double check
				// whether the square origin selected was or not right...
				// It is right only when the selected item is not empty
				// and it has a on top belonging to the player having the turn
				assert (sq); // It shouldn't happen...just in case.
				if (st [sq -> row () - 1][sq -> column () - 1] == trn)
				{
					_squareOrigin = sSID;
					_nextInternalSituation = _PLAYING; 
					_game -> setState (__GAMESTATESELECTSQUARENAME__); 
				}
				// It is not, then it is needed to select
				// back the square origin
				else
				{
					_nextInternalSituation = _SELECTINGTARGET; 
					_game -> setState (__GAMESTATESELECTSQUARENAME__); 
				}
			}

			break;

		case _PLAYING:
			{
				// Before playing, we need to be sure 
				// the square selected has to be empty...
				assert (sq); // It shouldn't happen...just in case.
				if (st [sq -> row () - 1][sq -> column () - 1] == 0)
				{
					// Sets information collected into the state of the plater and...
					_squareTarget = sSID;
					QGAMES::BoardGamePlayer* tP = ((GameApplication*) _game) -> whoHasTurn ();
					((Player3InARowState*) tP -> state ()) -> setSquareOrigin (_squareOrigin);
					((Player3InARowState*) tP -> state ()) -> setSquareTarget (_squareTarget);
					_squareOrigin = _squareTarget = -1; // Initial state...
					((GameApplication*) _game) -> boardGame () -> play (); 
					// ...then to play!!
					_nextInternalSituation = _SELECTINGORIGIN;
					// Next action to select the origin...
					_game -> setState (__GAMESTATEPLAYINGNAME__);
					// ...but still playing!!

				}
				// If it is not, then it is needed to select back
				// the target square!!
				else
				{
					_nextInternalSituation = _PLAYING; 
					// Still playing...
					_game -> setState (__GAMESTATESELECTSQUARENAME__); 
					// but back to select the target!!
				}
			}
			break;

		default:
			break;
	};
}

// ---
void GameApplicationWinsState::onEnter ()
{
	_counter = 0;
	_wantToExit = false;
	_game -> sound (__SOUNDEND__) -> play (-1);
}

// ---
void GameApplicationWinsState::updatePositions ()
{
	if (++_counter >= (_game -> framesPerSecond () * 5))
	{
		_counter = 0;
		((GameApplication*) _game) -> reset (); // Reset the game, and restart gain...
		_game -> setState (__GAMESTATEINTRONAME__);
	}

	// The user has selected to exit the state and move the
	// game to the next point...
	if (_wantToExit)
	{
		((GameApplication*) _game) -> reset (); // Reset the game, and restart gain...
		_game -> setState (__GAMESTATEINTRONAME__);
	}
}

// ---
void GameApplicationWinsState::drawOn (QGAMES::Screen* s)
{
	QGAMES::Game::game () -> form (__3INAROWBACGROUNDINITIALFORM__) ->
		drawOn (s, __INITIALBACGROUNDFORM__, QGAMES::Position (__BD 0, __BD 0, __BD 0), 255);
	QGAMES::Game::game () -> form (__3INAROWLOGOINITIALFORM__) ->
		drawOn (s, __LITTLELOGOFORM__, QGAMES::Position (__BD 20, __BD 460, __BD 0), 255);
	QGAMES::Game::game () -> form (__WINFORM__) ->
		drawOn (s, __ICONTRHOPHYFORM__, QGAMES::Position (__BD 272, __BD 132, __BD 0), 255);
}

// ---
void GameApplicationWinsState::onExit ()
{
	_game -> sound (__SOUNDEND__) -> stop (); // Stops the sound if any...
}
