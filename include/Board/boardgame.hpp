/** \ingroup BoardGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: boardgame.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 12/11/2015 \n
 *	Description: Defines a board game. \n
 *				 A board game is made up of many boards. \n
 *				 Just only one of them can be active at any moment. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_BOARDGAME__
#define __QGAMES_BOARDGAME__

#include <Board/boardelement.hpp>
#include <Board/board.hpp>
#include <Board/boardgameplayer.hpp>
#include <Board/definitions.hpp>
#include <list>
#include <map>

namespace QGAMES
{
	class BoardGameRule;
	typedef std::list <BoardGameRule*> BoardGameRules;
	class BoardGame;
	typedef std::map <int, BoardGame*> BoardGames;

	/** Any game is made up of rules. \n
		The rules have to be verifies when the game runs. */
	class BoardGameRule : public Notifier
	{
		public:
		BoardGameRule ()
							{ }

		virtual ~BoardGameRule ()
							{ }

		virtual void initialize ()
							{ }

		/** To get the tuntime information of the rule,
			and to set it back. By default they don't do anything. */
		virtual SetOfOpenValues asSetOfOpenValues () const
						{ return (SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESRULETYPE__))); }
		virtual void fromSetOfOpenValues (const SetOfOpenValues& oV)
						{ assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESRULETYPE__)); }
		
		/** When the rule is applyied an event can be generated.
			That event will be observed by the game that can take decisions as a consequence. \n
			The rule returns true when the next rule has to be applied or false
			if it not needed because the conclusion if definitive. */
		virtual bool applyOver (BoardGame* b) = 0;
	};

	/** The game is the main part of the application. \n
		The game has boards and player playing in them, but it can have assigned resources
		and properties as any other element. \n
		A game has also an state. The state can be checked by any element being part of the game
		to actualize anything, so it has to be actualize any time the game runs. */
	class BoardGame : public BoardElement
	{
		public:
		/** The state of the game. 
			By default it has only the basic methods. */
		class State
		{
			public:
			State ()
							{ }

			virtual  ~State ()
							{ } // By default nothing...

			/** To get the tuntime information of the state,
				and to set it back. By default they don't do anything. */
			virtual SetOfOpenValues asSetOfOpenValues () const
							{ return (SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESBOARDSTATETYPE__))); }
			virtual void fromSetOfOpenValues (const SetOfOpenValues& oV)
							{ assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESBOARDSTATETYPE__)); }

			virtual void initialize () 
							{ } // By default nothing...
		};

		BoardGame (int id, const Boards& b, const BoardGameRules& rules, const BoardGamePlayers& p,
				const BoardResources& rscs = BoardResources ());
		virtual ~BoardGame ();

		/** @see parent. \n
			The information added is: \n
			The one comming from the state (setOfOpenValue), the one comming from each board (setOfOpenValues),
			the id if any of the current board (openValue), the information of the rules (setOfOpenValues),
			the information comming from each player (setOfOpenValues), and finally the id if any of the
			current player (openValue). */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		virtual int type () const
							{ return (__BOARDGAMETYPE__); }

		State* state ()
							{ if (_state == NULL) _state = createState ();
							  return (_state); }

		/** The method to play.
			Any time thid method is invoked a turn (hand) happens. 
			By default the play method of the current player is invoked. */
		virtual void play ();
		/** The method to verify the rules.
			The game observe the rules. So when something happens a notification can be launched.
			SO it is not needed this game to do something. */
		virtual void verifyRules ();

		// To manage the boards
		/** To get the boards. */
		const Boards& boards () const
							{ return (_boards); }
		/** To change the active board to one specific one.
			null is no admitted. */
		void changeBoardTo (Board* b);
		/** To change the board to one specific one based on its id.
			A board with the requested id has to exist, otherwise it fails. */
		void changeBoardTo (int bId);
		/** What is the current board. */
		Board* currentBoard ()
							{ return (_currentBoard); }

		// To manage the squares of the board game
		// A board has to be defined...
		/** To get an square based on its id. */
		BoardSquare* square (int id);
		/** To get an square based on a list of the properties.
			Take into account that several squares can fit these properties at the same time,
			so the square returnes (if any) is the first fitting those. */
		BoardSquare* square (const BoardElementProperties& pts);
		/** To know the square in a position...if any
			The method can return null. */
		BoardSquare* squareOn (const Position& pos);

		// To manage the rurles
		/** To get the rules. */
		const BoardGameRules& rules () const
							{ return (_rules); }

		// To manage the players of the board game
		/** To get the players. */
		const BoardGamePlayers& players () const
							{ return (_players); }
		/** To get a player. */
		BoardGamePlayer* player (int id);
		/** To change the player having the turn.
			NULL is not allowed. */
		void changeTurnTo (BoardGamePlayer* p);
		/** Same than previosu but receiving the id of the player as parameter.
			If the player doesn't exists, then it fails. */
		void changeTurnTo (int pId);
		BoardGamePlayer* whoHasTurn ()
							{ return (_turnOver); }
		/** What is the status of the current player? */
		BoardGamePlayer::State* currentPlayerState ();
		/** And of any other player?. 
			It it doesn't exist, then it will fail. */
		BoardGamePlayer::State* playerState (int id);

		// To manage the resources of the board game
		// A board has to be defined...
		virtual bool existResource (int id) const;
		/** To get a resource.
			The method looks through the local resources first and then through the boards
			until find it. */
		virtual BoardResource* resource (int id);
		/** To get a resource matching specific properties. 
			In this case, if none matches the properties, then a null is returned. 
			The method also looks through the local resources first and then 
			looks through the different boards, looking for th resource. */
		virtual BoardResource* resource (const BoardElementProperties& pts);

		virtual void initialize ();
		virtual void inEveryLoop ();
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		virtual void finalize ();

		/** In this implementation only the change of the turn is managed. */
		virtual void processEvent (const Event& evnt);

		protected:
		/** This method creates the state to control the game. 
			You have to define your own state for every game you define and
			to use thid method to instantiate it first time.
			Do not be concerned about how to delete it. The system deletes it automatically. */
		virtual State* createState () = 0;

		protected:
		State* _state;
		Boards _boards;
		Board* _currentBoard;
		BoardGameRules _rules;
		BoardGamePlayers _players;
		BoardGamePlayer* _turnOver;
	};
}

#endif
  
// End of the file
/*@}*/
