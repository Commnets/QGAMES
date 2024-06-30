/** \ingroup BoardGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: boardsquare.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 12/11/2015 \n
 *	Description: Defines a board square within a board. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_BOARDSQUARE__
#define __QGAMES_BOARDSQUARE__

#include <Board/boardelement.hpp>
#include <Board/definitions.hpp>
#include <map>

namespace QGAMES
{
	class BoardSquare;
	typedef std::map <int, BoardSquare*> BoardSquares;
	class Board;

	/** A BoardSquare is a part of board. \n
		The board square can have resources associated, and properties defining it. */
	class BoardSquare : public BoardElement
	{
		public:
		BoardSquare (int id, const BoardResources& rscs = BoardResources (),
				const BoardElementProperties& pts = BoardElementProperties ())
			: BoardElement (id, rscs, pts),
			  _squaresConnected (),
			  _board (NULL), // Nothing can be done before setting up this variable...
			  _row (__QGAMES_SQUAREROWDEFAULTVALUE__), 
			  _column (__QGAMES_SQUARECOLUMNDEFAULTVALUE__), 
			  _depth (__QGAMES_SQUAREDEPTHDEFAULTVALUE__),
			  _width (__QGAMES_SQUARESIZEXDEFAULTVALUE__), 
			  _height (__QGAMES_SQUARESIZEYDEFAULTVALUE__), 
			  _deep (__QGAMES_SQUARESIZEZDEFAULTVALUE__),
			  _leftUpCorner (Position::_cero)
							{ }

		/** @see parent. \n
			The only information added is the position (openValue),
			because the rest are part of the definition or they can't be changed (with the default methods). */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);
		
		virtual int type () const
							{ return (__BOARDSQUARETYPE__); }

		// To know the row, the column and the depth where the square is in.
		// The squares represent zones inthe borad, so they have to have a position in it.
		// These are the parameterrs describing that position...
		/** The row. */
		int row () const
								{ return (_row); }
		/** The column. */
		int column () const
								{ return (_column); }
		/** The depth of the square...if any in 3D. */
		int depth () const
								{ return (_depth); }
		// To know the size of the square
		/** The width. */
		int width () const
								{ return (_width); }
		/** The height. */
		int height () const
								{ return (_height); }
		/** The deep of the box. */
		int deep () const
								{ return (_deep); }
		// To know the left up corner position
		/** As a position. */
		const Position& leftUpCorner () const
								{ return (_leftUpCorner); }

		// To manage the squares...
		const BoardSquares& squaresConnceted () const
							{ return (_squaresConnected); }
		BoardSquare* squareAt (int p);
		/** This method links this square to another, 
			but the other is not linked to this. \n
			This is because in many games the links are only in one direction. \n
			Don't forget to link the other one if needed. */
		void setSquareAt (int p, BoardSquare* s);

		/** To know whethet the square is or not is a position. 
			How to calculate if the square is or not in a position depends on the type of square,
			so the mthod has to be overloaded inheriting from this. */
		virtual bool isIn (const Position& pos)
							{ return (false); }

		// To navigate to the board...
		Board* board ()
							{ return (_board); }
		void setBoard (Board* b); // This method is called automatically at board construction time...

		virtual void initialize ();

		protected:
		BoardSquares _squaresConnected;
		Board* _board;
		int _row, _column, _depth;
		int _width, _height, _deep;
		QGAMES::Position _leftUpCorner;
	};
}

#endif
  
// End of the file
/*@}*/