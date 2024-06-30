/** \ingroup BoardGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: boardgameplayer.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 12/11/2015 \n
 *	Description: Defines the player who plays in a board game. \n
 *				 In a board game there can be many players.	\n	
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_BOARDGAMEPLAYER__
#define __QGAMES_BOARDGAMEPLAYER__

#include <Board/boardelement.hpp>
#include <Board/definitions.hpp>
#include <map>

namespace QGAMES
{
	class BoardGamePlayer;
	typedef std::map <int, BoardGamePlayer*> BoardGamePlayers;
	class BoardGame;

	/** 
	 *	The player plays in a game. \n
	 *	The player has always an state. When the game runs and a player plays
	 *	his / her status has to be updated. 
	 */
	class BoardGamePlayer : public BoardElement
	{
		public:
		/** The status of the player during the game. */
		class State
		{
			public:
			State ()
							{ }

			virtual ~State ()
							{ }

			/** To get the tuntime information of the state,
				and to set it back. By default they don't do anything. */
			virtual SetOfOpenValues asSetOfOpenValues () const
							{ return (SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESPLAYERSTATETYPE__))); }
			virtual void fromSetOfOpenValues (const SetOfOpenValues& oV)
							{ assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESPLAYERSTATETYPE__)); }

			virtual void initialize () 
							{ }
		};

		BoardGamePlayer (int id, const BoardResources& rscs = BoardResources ())
			: BoardElement (id, rscs),
			  _state (NULL),
			  _game (NULL)
							{ }

		/** @see parent. \n
			The information added is just the one comming from the state. */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		virtual int type () const
							{ return (__BOARDPLAYERTYPE__); }

		State* state ()
							{ return ((_state == NULL)
								? (_state = createState ()) : _state); }

		/** The player has to play its turn, then the state has to be updated. 
			The method has to be overloaded later. */
		virtual void play () = 0;
		/** After playing, and verifying the rules, it is possible to decide
			that the movement has to be "unplayer". In this case this
			method has to be used. By default it has an empty definition. */
		virtual void unPlay () 
							{ }

		virtual void initialize ();

		BoardGame* boardGame ()
							{ return (_game); }
		void setBoardGame (BoardGame* g);

		protected:
		/** This method is to create the state. \n
			Overload this method to instantiate the right state class for your game. */
		virtual State* createState () = 0;

		protected:
		State* _state;
		BoardGame* _game;
	};
}

#endif
  
// End of the file
/*@}*/