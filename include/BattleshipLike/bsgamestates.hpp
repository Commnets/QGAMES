/** \ingroup Battleship */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: bsgamestates.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/11/2019 \n
 *	Description: Defines specific game states for Battleship like projects. \n
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_GAMESTATES__
#define __BATTLESHIP_GAMESTATES__

#include <BattleshipLike/bsdefinitions.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace BATTLESHIP
{
	class Battleship;

	/** A very simple game state to load the things that the game needs.
		This is like executing the specific initialize method for battleship like games.
		The reason for loading the game using a game state is to load the resources first using a loading game state. */
	class InitializeBattleshipGameState : public QGAMES::GameState
	{
		public:
		InitializeBattleshipGameState (int id, QGAMES::GameState* nS = NULL)
			: QGAMES::GameState (id, QGAMES::Game::game (), nS)
							{ }

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions (); // Just to say that the state has finished
	};

	/** 
	  * A very complex and interesting game state. 
	  * The battleship moves in the middle right side of the screen.
	  * There are a couple of randoms movements defined into the monitors.xml file (read it).
	  * When a movement finishes another one is choosen.
	  */
	class BattleshipMovingGameState : public QGAMES::ShowingEntityGameState
	{
		public:
		BattleshipMovingGameState (int id, QGAMES::GameState* ns = NULL)
			: QGAMES::ShowingEntityGameState (id, 
				QGAMES::ShowingEntityGameState::Properties 
					(__BATTLESHIP_BATTLESHIPTYPE1ID__, 
					 255, /** Fade */
					 false, /** Not to control the position (because it is controlled externally). */
					 QGAMES::Position::_cero, /** So, the position of the entity doesn't matter */
					 QGAMES::ShowingEntityGameState::Properties::PosReference::_TOPLEFT /** Neither the reference */
					), ns),
			  _currentMonitor (NULL),
			  _loadNextMonitor (false) 
							{ }

		~BattleshipMovingGameState ();

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void onExit ();

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		private:
		// Implementation
		/** Which is the monitor being executed (and observed) now. */
		QGAMES::CharacterControlStepsMonitor* _currentMonitor;
		/** Indicates that in the next execution of "updatePositions" method a new monitor should be loaded. */
		bool _loadNextMonitor;
	};

	/** A game state to draw the name of the current player. 
		The constructor receives a text to be drawn behind the name of the current player.
		If the name has not been introduced ever, then a GUI system is displayed to ask for it.
		In this case, just when the name is introduced, an event is sent up. */
	class ShowingCurrentPlayerNameGameState : public QGAMES::ShowingStandardTextGameState
	{
		public:
		ShowingCurrentPlayerNameGameState (int id, const std::string& txt, int fTp,
				const QGAMES::ShowingStandardTextGameState::Properties& prps = 
					QGAMES::ShowingStandardTextGameState::Properties (), QGAMES::GameState* nS = NULL)
			: QGAMES::ShowingStandardTextGameState (id, txt, fTp, prps, nS),
			  _guiSystem (NULL)
							{ }
		~ShowingCurrentPlayerNameGameState ()
							{ deleteGUISystem (); }

		/** @see parent. */
		virtual void onEnter ();
		virtual void onExit ();

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		/** @see parent. */
		void actualizeText ();

		private:
		// Implementation
		void deleteGUISystem ();

		private:
		// Implementation
		QGAMES::GUISystem* _guiSystem;
	};

	/** A game state to draw the name of the world where the current player is on. */ 
	class ShowingCurrentWorldNameGameState : public QGAMES::ShowingStandardTextGameState
	{
		public:
		ShowingCurrentWorldNameGameState (int id, const std::string& txt, int fTp,
				const Properties& prps = Properties (), GameState* nS = NULL)
			: QGAMES::ShowingStandardTextGameState (id, txt, fTp, prps, nS)
							{ }

		protected:
		/** @see parent. */
		void actualizeText ();
	};

	/** A game state to draw the name of the scene where the current player is on. */ 
	class ShowingCurrentSceneNameGameState : public QGAMES::ShowingStandardTextGameState
	{
		public:
		ShowingCurrentSceneNameGameState (int id, const std::string& txt, int fTp,
				const Properties& prps = Properties (), GameState* nS = NULL)
			: QGAMES::ShowingStandardTextGameState (id, txt, fTp, prps, nS)
							{ }

		protected:
		/** @see parent. */
		void actualizeText ();
	};

	/** A specific class to modify a little bit the 
		titles when the name of the game is presented: Bigger and with a logo behind. */
	class CreditTitlesGameState : public QGAMES::CreditTitlesGameState
	{
		public:
		CreditTitlesGameState (int id, int nT, const std::string& cT,
				const Properties& prps = Properties (), GameState* nS = NULL);

		/** @see parent. */
		virtual void drawOn (QGAMES::Screen* s);

		protected:
		/** @see parent.
			The title is bigger and in another color. */
		virtual QGAMES::ScoreObjectText* createText (const std::string& txt);

		protected:
		int _numberTitle;
		std::string _contentTitle;

		// Implementation
		// It could be null, meaning the game state hasn't been well defined.
		QGAMES::ShowingFixFormGameState* _logoGameState;
	};

	/** The specific class to show the different entries of the hall of fame. 
		The class inherits from the one defined at AdvancedArcade library redefining how entries are created. */
	class ShowingHallOfFameGameState : public QGAMES::ShowingHallOfFameGameState 
	{
		public:
		ShowingHallOfFameGameState (int id, 
			const QGAMES::ShowingHallOfFameGameState::Properties& prps = 
				QGAMES::ShowingHallOfFameGameState::Properties (),
			QGAMES::GameState* nS = NULL)
			: QGAMES::ShowingHallOfFameGameState (id, prps, nS) 
							{ }

		private:
		/** @see parent. */
		virtual std::multimap <std::string, QGAMES::SetOfOpenValues> orderHallOfFame 
			(const std::multimap <std::string, QGAMES::SetOfOpenValues>& hF);
		virtual QGAMES::ScoreObjectText* createTextFor (const std::string& eK, const QGAMES::SetOfOpenValues& sV);
		/** The text created is just with the score adjusted to 5 digits and the name of the player. */
	};

	/** The menu in Battleship games includes keys to manage when catching or leaving things
		and to iterate things already caught in the list of those. */
	class SelectionGameState : public QGAMES::StandardInitialSelectionOptionsGameState
	{
		protected:
		class SelectionHandler;

		public:
		SelectionGameState (int id, QGAMES::GameState* nS = NULL)
			: QGAMES::StandardInitialSelectionOptionsGameState (id, nS, createStandardMenus (), new SelectionHandler)
							{ }

		protected:
		/** @see parent. */
		virtual void initializeGUISystem ();
		virtual void finalizeGUISystem ();

		protected:
		class SelectionHandler : public StandardInitialSelectionOptionsGameState::SelectionHandler
		{
			public:
			friend SelectionGameState;

			SelectionHandler ()
				: StandardInitialSelectionOptionsGameState::SelectionHandler ()
							{ }

			/** @see parent. */
			virtual int whenWidgetSelected (QGAMES::Widget* wdt);
		};
	};

	/** 
	 * Complex state to control when playing. 
	 * When playing, there are a flow of sub game states according to the state of the battleship.
	 * When this game state starts and the scene hasn't been visited yet, 
	 * then a counting down sub game state is presented.
	 * Otherwise, the full control of the artist is allowed, while the space battlehip is alive.
	 * When the artist died, then no control is allowed while a background music plays.
	 * When the music finishes then, the control finishes to determinate what to do next.
	 */
	class PlayingGameState; // Defined later but used here...
	class PlayingControlGameState : public QGAMES::GameStateControl
	{
		public:
		PlayingControlGameState (int id, 
				const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fM)
			: QGAMES::GameStateControl (id, QGAMES::Game::game (), sts, fM),
			  _battleship (NULL)
							{ }

		/** To get the state that gives the control to the player.
			It should exist, but is has to be controlled outside here. */
		const PlayingGameState* playingState () const
							{ return ((const PlayingGameState*) playingState ()); }
		PlayingGameState* playingState ();

		/** @see parent. */
		virtual void onEnter ();
		virtual void onExit ();

		protected:
		/** @see parent. */
		virtual int circunstanceWhen (QGAMES::GameState* st, const QGAMES::Event& evnt);

		protected:
		// Implementation
		/** A reference to the dragon artist being controlled by this playing state. */
		Battleship* _battleship;
	};

	/** Just to play. The control is on Player's hands eventually. */
	class PlayingGameState : public QGAMES::GameState
	{
		public:
		typedef enum { _MOVING = 0, _DIED, _REACHGOAL, _OPTIONS } MainCharacterState;

		PlayingGameState (int id, QGAMES::GameState* nS = NULL)
			: QGAMES::GameState (id, QGAMES::Game::game (), nS),
			  _lastMainCharacterState (_MOVING),
			  _battleship (NULL)
							{ }

		void setLastMainCharacterState (MainCharacterState st)
							{ _lastMainCharacterState = st; }
		MainCharacterState lastMainCharacterState () const
							{ return (_lastMainCharacterState); }

		/** @see parent. */
		virtual void onEnter ();
		virtual void onExit ();

		/** @see parent. 
			The amain artist is observed. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		/** As the game goes on, the situation of the artist being controlled can vary. */
		MainCharacterState _lastMainCharacterState;

		// Implementation
		/** A reference to the dragon artist being controlled by this playing state. */
		Battleship* _battleship;
	};

	/** A game state to select different options while playing.
		These options could be like: save the game, exit the game, 
		or other related with the guns and things caught. */
	class OptionsWhenPlayingGameState : public QGAMES::SelectionOptionsGameState
	{
		public:
		OptionsWhenPlayingGameState (int id, GameState* ns = NULL)
			: QGAMES::SelectionOptionsGameState (id, createMenus (), new SelectionHandler, ns)
							{ }

		/** @see parent. 
			Just to observe the input handler. */
		virtual void onEnter ();
		virtual void onExit ();

		/** @see parent. 
			Just to treat the ESC. That key finishes the menu. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		static SelectionOptionsGameState::Menus createMenus ();

		protected:
		class SelectionHandler : public SelectionOptionsGameState::Handler
		{
			public:
			SelectionHandler ()
				: SelectionOptionsGameState::Handler ()
							{ }

			/** @see parent. */
			virtual int whenWidgetSelected (QGAMES::Widget* wdt);
		};
	};

	/** A specific object to control the states. */
	class GameStateControl : public QGAMES::AdvancedGameStateControl
	{
		public:
		GameStateControl (int id, GameState* psState, const QGAMES::GameStates& sts, 
				const QGAMES::AdvancedGameStateControl::FlowMap& fM)
			: QGAMES::AdvancedGameStateControl (id, psState, sts, fM)
							{ }

		protected:
		/** @see parent. */
		virtual int circunstanceWhen (QGAMES::GameState* st, const QGAMES::Event& evnt);
	};
}

#endif
  
// End of the file
/*@}*/