/** \ingroup PACMAN */
/*@{*/

/**	
 *	@file	
 *	File: pcgamestates.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: To manipulate the different game states of the game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMAN_GAMESTATES__
#define __PACMAN_GAMESTATES__

#include <PacManLike/pcdefs.hpp>
#include <PacManLike/pcartists.hpp>
#include <PacManLike/pcworlds.hpp>

#include <advancedArcade/adinclude.hpp>

namespace PACMAN
{
	/** A very simple game state to load the things that the game needs. */
	class InitializePACMANGameState final : public QGAMES::GameState
	{
		public:
		InitializePACMANGameState (int id, QGAMES::GameState* nS = NULL)
			: QGAMES::GameState (id, QGAMES::Game::game (), nS)
							{ }

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions (); // Just to say that the state has finished
	};

	/** Monster moves up and down a little bit. */
	class MonsterMovingGameState final : public QGAMES::GameState
	{
		public:
		/** This structure is used to determinate where the entity showing the game paused is drawn. */
		struct Properties
		{
			Properties ()
				: _fade (255),
				  _position (QGAMES::Position (__BD 0.5, __BD 0.5, __BD 0)), // Center position in a 2D screen...
				  _iterations (-1), // Meaning infinite...
				  _bigPacman (false)
							{ }

			/**
			  *	@param f	Alpha grade to show the entity. From 0 to 255.
			  *	@param p	Relative position in the screen where to show the left up corner of the text. \n
			  *				Coordinates from 0 to 1 (percentage) both included. 
			  * @param i	Number of itertions (from left to right and back) the monters have to move.
			  * @param bP	Whether the pacman has to be or not big when chasing monsters!
			  */
			Properties (int f, const QGAMES::Position& p, int i, bool bP)
				: _fade (f),
				  _position (p),
				  _iterations (i),
				  _bigPacman (bP)
							{ assert ((_fade >= 0 && _fade <= 255) &&
									  (_iterations > 0 || _iterations == -1)); }

			/** 
				A constructor based on parameters.
				It is used in the game state constructor.
				The structure of the game state control definition file :
				...
				<Attributes>
					<Attribute id="FADE" value="{0..255}"/>
					<Attribute id="POSITION" value="{0..1},{0..1},{0..1}"/>
					<Attribute id="ITERATIONS" value="0..|-1"/>
					<Attribute id="BIGPACMAN" value="YES|NO"/> 
				</Attributes>
				...
			*/
			Properties (const std::map <std::string, std::string>& prps);

			// Default copy constructor is enough
			Properties (const Properties& prps) = default;

			Properties& operator = (const Properties& prps) = default;
		
			int _fade;
			/** Indicates a relative position.
				So 0,0,0 means left up front corner, and 1,1,1 right bottom back one.
				0.5 indicates the center the of visualization area. */
			QGAMES::Position _position;
			/** To indicate how many iterations the pacman has to do. -1 means infinite. */
			int _iterations;
			/** To indicate whether the pacman has to be shoy big or not. */
			bool _bigPacman;
		};

		MonsterMovingGameState () = delete;

		MonsterMovingGameState (int id, const Properties& prps, QGAMES::GameState* ns = nullptr)
			: QGAMES::GameState (id, QGAMES::Game::game (), ns),
			  _properties (prps),
			  _artists (5, nullptr) // Pacman + the 4 monsters...
							{ }

		MonsterMovingGameState (const MonsterMovingGameState&) = delete;

		MonsterMovingGameState& operator = (const MonsterMovingGameState&) = delete;

		~MonsterMovingGameState ();

		virtual void onEnter () override;
		virtual void updatePositions () override;
		virtual void drawOn (QGAMES::Screen* scr) override;
		virtual void onExit () override;

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters () override
							{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches () override
							{ return (new OnOffSwitches ()); }

		void setInitialPositionFrom (const QGAMES::Position& iPos);

		private:
		Properties _properties;

		// Implementation
		/** The list of the entities neging managed. */
		std::vector <PACMAN::Artist*> _artists;

		static const int _COUNTERMOVE = 0;
		static const int _COUNTERITERATIONS = 1;
		static const int _SWITCHRIGHT = 0;
	};

	/** A game state to draw the name of the current player. */
	class ShowingCurrentPlayerNameGameState final : public QGAMES::ShowingStandardTextGameState
	{
		public:
		ShowingCurrentPlayerNameGameState () = delete;

		ShowingCurrentPlayerNameGameState (int id, const std::string& txt, int fTp,
				const QGAMES::ShowingStandardTextGameState::Properties& prps = 
					QGAMES::ShowingStandardTextGameState::Properties (), QGAMES::GameState* nS = nullptr)
			: QGAMES::ShowingStandardTextGameState (id, txt, fTp, prps, nS),
			  _guiSystem (nullptr)
							{ }

		ShowingCurrentPlayerNameGameState (const ShowingCurrentPlayerNameGameState&) = delete;

		ShowingCurrentPlayerNameGameState& operator = (const ShowingCurrentPlayerNameGameState&) = delete;

		~ShowingCurrentPlayerNameGameState ()
							{ deleteGUISystem (); }

		virtual void onEnter () override;
		virtual void onExit () override;

		virtual void processEvent (const QGAMES::Event& evnt);

		private:
		void actualizeText () override;

		// Implementation
		void deleteGUISystem ();

		private:
		// Implementation
		QGAMES::GUISystem* _guiSystem;
	};

	/** A game state to draw the name of the level where the current player is on. */ 
	class ShowingCurrentLevelGameState final : public QGAMES::ShowingStandardTextGameState
	{
		public:
		ShowingCurrentLevelGameState (int id, const std::string& txt, int fTp,
				const Properties& prps = Properties (), GameState* nS = NULL)
			: QGAMES::ShowingStandardTextGameState (id, txt, fTp, prps, nS)
							{ }

		private:
		/** @see parent. */
		void actualizeText ();
	};

	/** The specific class to show the different entries of the hall of fame.  */
	class ShowingHallOfFameGameState final : public QGAMES::ShowingHallOfFameGameState 
	{
		public:
		ShowingHallOfFameGameState () = delete;

		ShowingHallOfFameGameState (int id, 
			const QGAMES::ShowingHallOfFameGameState::Properties& prps = QGAMES::ShowingHallOfFameGameState::Properties (),
				QGAMES::GameState* nS = nullptr)
			: QGAMES::ShowingHallOfFameGameState (id, prps, nS) 
							{ }

		ShowingHallOfFameGameState (const ShowingHallOfFameGameState&) = delete;

		ShowingHallOfFameGameState& operator = (const ShowingHallOfFameGameState&) = delete;

		private:
		virtual std::multimap <std::string, QGAMES::SetOfOpenValues> orderHallOfFame 
			(const std::multimap <std::string, QGAMES::SetOfOpenValues>& hF) override;
		virtual QGAMES::ScoreObjectText* createTextFor (const std::string& eK, const QGAMES::SetOfOpenValues& sV) override;
	};

	/** Complex state to control when playing. */
	class PlayingGameState; // Defined later but used here...
	class GameStateControl; // ...The same
	class PlayingControlGameState final : public QGAMES::GameStateControl
	{
		public:
		friend PACMAN::GameStateControl;

		PlayingControlGameState () = delete;

		PlayingControlGameState (int id, 
				const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fM)
			: QGAMES::GameStateControl (id, QGAMES::Game::game (), sts, fM),
			  _pacman (nullptr),
			  _nextLevel (-1) // Meaning to take the one defined in the configuration
							{ }

		PlayingControlGameState (const PlayingControlGameState&) = delete;

		PlayingControlGameState& operator = (const PlayingControlGameState&) = delete;

		/** To get the state that gives the control to the player.
			It should exist, but is has to be controlled outside here. */
		const PlayingGameState* playingState () const
							{ return ((const PlayingGameState*) playingState ()); }
		PlayingGameState* playingState ();

		virtual void onEnter () override;
		virtual void onExit () override;

		private:
		virtual int circunstanceWhen (QGAMES::GameState* st, const QGAMES::Event& evnt) override;

		// Implementation
		/** When playing status finishes, this is to set which the next level will be the nxt time to enter here. */
		void setNextLevel (int l)
							{ _nextLevel = l; }

		private:
		// Implementation
		/** A reference to the dragon artist being controlled by this playing state. */
		PacMan* _pacman;
		/** The number of the next level. */
		int _nextLevel;
	};

	/** Just to play. The control is on Player's hands eventually. */
	class PlayingGameState final : public QGAMES::GameState
	{
		public:
		typedef enum { _MOVING = 0, _DIED, _REACHGOAL, _OPTIONS } MainCharacterState;

		PlayingGameState (int id, QGAMES::GameState* nS = nullptr)
			: QGAMES::GameState (id, QGAMES::Game::game (), nS),
			  _lastMainCharacterState (_MOVING),
			  _pacman (nullptr)
							{ }

		void setLastMainCharacterState (MainCharacterState st)
							{ _lastMainCharacterState = st; }
		MainCharacterState lastMainCharacterState () const
							{ return (_lastMainCharacterState); }

		virtual void onEnter () override;
		virtual void onExit () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		private:
		/** As the game goes on, the situation of the artist being controlled can vary. */
		MainCharacterState _lastMainCharacterState;

		// Implementation
		/** A reference to the dragon artist being controlled by this playing state. */
		PacMan* _pacman;
	};

	/** A game state to stop pacman elements moving. */
	class StopPacmanElemntsMovingGameState final : public QGAMES::GameState
	{
		public:
		/** This structure is used to determinate bhaviour when blinking. */
		struct Properties
		{
			Properties ()
				: _stopAll (true),
				  _hideMonsters (false),
				  _hideThings (false),
				  _vanishPacman (false)
							{ }

			/**
			  *	@param sA	To stop all pacelements' movement?.
			  * @param hM	Hide monsters?
			  * @param hT	Hide things?
			  * @param vP	Vanish Pacman?
			  */
			Properties (bool sA, bool hM, bool hT, bool vP)
				: _stopAll (sA),
				  _hideMonsters (hM),
				  _hideThings (hT),
				  _vanishPacman (vP)
							{ }

			/** 
				A constructor based on parameters.
				It is used in the game state constructor.
				The structure of the game state control definition file :
				...
				<Attributes>
					<Attribute id = "STOPALL" value = "YES|NO"/>
					<Attribute id = "HIDEMONSTERS" value = "YES|NO"/>
					<Attribute id = "HIDETHINGS" value = "YES|NO"/>
					<Attribute id = "VANISHPACMAN" value = "YES|NO"/>
				</Attributes>
				...
			*/
			Properties (const std::map <std::string, std::string>& prps);

			// Default copy constructor is enough
			Properties (const Properties& prps) = default;

			Properties& operator = (const Properties& prps) = default;
		
			bool _stopAll;
			bool _hideMonsters;
			bool _hideThings;
			bool _vanishPacman;
		};

		StopPacmanElemntsMovingGameState () = delete;

		StopPacmanElemntsMovingGameState (int t, const Properties& prps, QGAMES::GameState* nS = nullptr)
			: QGAMES::GameState (t, QGAMES::Game::game (), nS),
			  _properties (prps),
			  _mazeWorld (nullptr),
			  _pacman (nullptr)
							{ }

		StopPacmanElemntsMovingGameState (const StopPacmanElemntsMovingGameState&) = delete;

		StopPacmanElemntsMovingGameState& operator = (const StopPacmanElemntsMovingGameState&) = delete;

		virtual void onEnter () override;
		virtual void updatePositions () override;
		virtual void onExit () override;

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters () override
							{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches () override
							{ return (new OnOffSwitches ()); }

		private:
		Properties _properties;

		// Implementation
		/** A reference to the current world. */
		World* _mazeWorld;
		/** and to pacman. */
		PacMan* _pacman;

		static const int _COUNTERTOCHANGESTATUS = 0;
		static const int _COUNTERVANISHSTATUS = 1;
		static const int _SWITCHVANISHCOMPLETED = 0;
		static const int _FADE [14];
	};

	/** A game state to blink the maze. */
	class BlinkMazePlayingGameState final : public QGAMES::GameState
	{
		public:
		/** This structure is used to determinate bhaviour when blinking. */
		struct Properties
		{
			Properties ()
				: _blinkingTime (__BD 0.25),
				  _numberBlinks (5)
							{ }

			/**
			  *	@param bT	Blinking time in seconds. Always bigger than 0.
			  *	@param nB	Number of Blinks. Always also bigger than 0 o -1 meaning never ends.
			  */
			Properties (QGAMES::bdata bT, int nB)
				: _blinkingTime (bT),
				  _numberBlinks (nB)
							{ assert (_blinkingTime > __BD 0 && 
									  (_numberBlinks > 0 || _numberBlinks == -1)); }

			/** 
				A constructor based on parameters.
				It is used in the game state constructor.
				The structure of the game state control definition file :
				...
				<Attributes>
					<Attribute id = "BLINKINGTIME" value = "{0..}"/>
					<Attribute id = "NUMBERBLINKS" value = "{0..|-1}"/>
				</Attributes>
				...
			*/
			Properties (const std::map <std::string, std::string>& prps);

			// Default copy constructor is enough
			Properties (const Properties& prps) = default;

			Properties& operator = (const Properties& prps) = default;
		
			QGAMES::bdata _blinkingTime;
			/** if -1 means never ends. */
			int _numberBlinks;
		};

		BlinkMazePlayingGameState () = delete;

		BlinkMazePlayingGameState (int t, const Properties& prps = Properties (), QGAMES::GameState* nS = nullptr)
			: QGAMES::GameState (t, QGAMES::Game::game (), nS),
			  _properties (prps),
			  _mazeWorld (nullptr)
							{ }

		BlinkMazePlayingGameState (const BlinkMazePlayingGameState&) = delete;

		BlinkMazePlayingGameState& operator = (const BlinkMazePlayingGameState&) = delete;

		virtual void onEnter () override;
		virtual void updatePositions () override;
		virtual void onExit () override;

		private:
		/** The properties. */
		Properties _properties;

		// Implementation
		/** A reference to the current map. */
		World* _mazeWorld;
	};

	/** A game state to select different options while playing.
		These options could be like: save the game, exit the game, 
		or other related with the guns and things caught. */
	class OptionsWhenPlayingGameState final : public QGAMES::SelectionOptionsGameState
	{
		public:
		OptionsWhenPlayingGameState () = delete;

		OptionsWhenPlayingGameState (int id, GameState* ns = nullptr)
			: QGAMES::SelectionOptionsGameState (id, createMenus (), new SelectionHandler, ns)
							{ }

		OptionsWhenPlayingGameState (const OptionsWhenPlayingGameState&) = delete;

		OptionsWhenPlayingGameState& operator = (const OptionsWhenPlayingGameState&) = delete;

		virtual void onEnter () override;
		virtual void onExit () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		private:
		static SelectionOptionsGameState::Menus createMenus ();

		private:
		class SelectionHandler : public SelectionOptionsGameState::Handler
		{
			public:
			SelectionHandler ()
				: SelectionOptionsGameState::Handler ()
							{ }

			/** @see parent. */
			virtual int whenWidgetSelected (QGAMES::Widget* wdt) override;
		};
	};

	/** A specific object to control the states. 
		It can be defined later to support variations in the intermission state. */
	class GameStateControl : public QGAMES::AdvancedGameStateControl
	{
		public:
		GameStateControl (int id, GameState* psState, const QGAMES::GameStates& sts, 
				const QGAMES::AdvancedGameStateControl::FlowMap& fM)
			: QGAMES::AdvancedGameStateControl (id, psState, sts, fM)
							{ }

		protected:
		/** @see parent. */
		virtual int circunstanceWhen (QGAMES::GameState* st, const QGAMES::Event& evnt) override;

		// Implementation
		/** Get the intermission screen to be displayed and returns the circusntance 
			to be managed in the method abovee. By default it is the standard one (3), 
			meaning pacman is chased and the other way around after leaving both the screen by the right. */
		virtual int intermissionCircunstanceFor (int iN) const
							{ return (3); }
	};
}

#endif
  
// End of the file
/*@}*/