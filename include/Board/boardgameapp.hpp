/** \ingroup BoardGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: definitions.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2015 \n
 *	Description: Common definitions for a board game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_BOARDGAMEAPP__
#define __QGAMES_BOARDGAMEAPP__

#include <Common/game.hpp>
#include <Board/boardbuilder.hpp>
#include <Board/boardgame.hpp>

namespace QGAMES
{
	/** 
	 *	The board game is the central piece of any boar game. \n
	 *	It defines mainly the access point to any element of the game. \n
	 *	It defines also the builder used to create boards and the
	 *	what is going to be the board game element.
	 */
	class BoardGameApplication : public Game
	{
		public:
		BoardGameApplication (GameImplementation* imp, int fps = 60)
			: Game (imp, fps),
			  _boardGame (NULL),
			  _boardBuilder (NULL)
							{ }
		virtual ~BoardGameApplication ();

		// To manage the players of the application,
		// The board game has to have been defined!
		/** To get a player of the game.
			If the player has not been defined it fails. */
		BoardGamePlayer* getPlayer (int id);
		/** To get a reference to the player having the turn now. */
		BoardGamePlayer* whoHasTurn ();

		// To manage the resources of the application as a whole
		// The board game has to have been defined!
		/** To get a resource with an specific id. 
			The resources should have a unique id.
			The method looks through the the boards and the local resources until find it. 
			If it is not found the program fails. */
		BoardResource* resource (int id);
		/** To get a resource matching specific properties...whatever the properties are. 
			In this case, a null can be returned meaning none matches the conditions. */
		BoardResource* resource (const BoardElementProperties& pts);

		// To manage the squares of application as a whole
		// The board game has to have been defined!
		/** To get an square with an specific id. */
		BoardSquare* square (int id);
		/** To get an square with a specific list of properties.
			Remember that among those properties the position can be there. */
		BoardSquare* square (const BoardElementProperties& pts);
		/** To locate the square in a position...if any.
			The method can return null. */
		BoardSquare* squareOn (const Position& pos);

		/** To get the board builder. */
		BoardBuilder* boardBuilder () 
							{ return ((_boardBuilder == NULL) 
								? (_boardBuilder = createBoardBuilder ()) : _boardBuilder); } 
		/** To get the board game. 
			If it doesn't exist already a new one is created using the internal provate method
			that has to be overloaded for every specific game. */
		BoardGame* boardGame ()
							{ return ((_boardGame == NULL) 
								? (_boardGame = createBoardGame ()) : _boardGame); } 

		virtual void drawOn (Screen* s);

		protected:
		// The methods used to create the different builders...
		/** To get the instance of the builder used to create boards. */
		virtual BoardBuilder* createBoardBuilder () = 0;
		/** To get the instance of the board game managing the game itself. */
		virtual BoardGame* createBoardGame () = 0;

		/** @see parent. */
		virtual std::string defaultParameter (const std::string& p) const;

		virtual void initialize ();  
		virtual void inEveryLoop (); 
		virtual void finalize (); 

		protected:
		BoardGame* _boardGame;
		BoardBuilder* _boardBuilder;

	};
}

// To simplify the access to the board object...
#define __BGM	(QGAMES::BoardGameApplication*)

#endif
  
// End of the file
/*@}*/