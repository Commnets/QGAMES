/** \ingroup BoardGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: board.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 12/11/2015 \n
 *	Description: Defines a board within a board game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_BOARD__
#define __QGAMES_BOARD__

#include <Board/boardelement.hpp>
#include <Board/boardsquare.hpp>
#include <Board/definitions.hpp>

namespace QGAMES
{
	class Board;
	typedef std::map <int, Board*> Boards;
	class BoardGame;

	/** 
	 *	A Board is the place where the game takes place. \n
	 *	The board is made up of squares. All them are observed. \n
	 *	So if something happens there, the result will be transmited to the board. 
	 */
	class Board : public BoardElement
	{
		public:
		Board (int id, const BoardSquares& sqs, 
				const BoardResources& rscs = BoardResources (),
				const BoardElementProperties& pts = BoardElementProperties ());

		/** @see parent. \n
			The information added is the one comming from the squared behind (setOfOpenValue) and
			the position of left up corner (openValue). */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		virtual int type () const
							{ return (__BOARDTYPE__); }

		// To know the left up corner position
		/** As a position. */
		const Position& leftUpCorner () const
								{ return (_leftUpCorner); }

		// To manage the squares
		/** To get the squares. */
		const BoardSquares& squares () const
							{ return (_squares); }
		/** To get an square with an specific id. */
		BoardSquare* square (int id);
		/** To get an square matching a set of properties.
			If there is none, then null will be returned. */
		BoardSquare* square (const BoardElementProperties& pts);
		/** To know the square in a position...if any
			so the method can return null. */
		BoardSquare* squareOn (const Position& pos);

		BoardGame* boardGame () 
							{ return (_game); }
		void setBoardGame (BoardGame* g); // This method is automatically called at boardgame construction time...

		virtual void initialize ();
		virtual void inEveryLoop ();
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		virtual void finalize ();

		protected:
		BoardSquares _squares;
		BoardGame* _game;
		Position _leftUpCorner;
	};
}

#endif
  
// End of the file
/*@}*/