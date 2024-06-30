/** \ingroup Example */
/*@{*/

/**	
 *	@file	
 *	File: TestAdArcade.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 14/04/2018 \n
 *	Description: Testing how the advanced arcade library works. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_TESTADARCADE__
#define __QGAMES_TESTADARCADE__

#include "Defs.hpp"

namespace TestAdArcade
{
	// -------The GUI System-------------------------------------------------------------------------
	/** Defines a decorator to show the element when focused. */
	class WhenFocusDecorator : public QGAMES::Widget::ComplexDecorator
	{
		public:
		WhenFocusDecorator ();

		/** @see parent. */
		bool isBefore () { return (true); }
	};

	/** Defines a widget to control the menus. */
	class ControlWidget : public QGAMES::SelectionOptionsGameState::MenuControlWidget
	{
		public:
		ControlWidget (int id, Decorator* d = NULL, 
			const QGAMES::ListOfWidgets& wdts = QGAMES::ListOfWidgets (), int wF = -1)
			: QGAMES::SelectionOptionsGameState::MenuControlWidget (id, d, wdts, wF)
							{ }

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& e);
	};

	/** The standard GUI System builder.
		It is used by most of the implementations. */
	class GUISystemBuilder : public QGAMES::StandardGUISystemBuilder
	{
		public:
		GUISystemBuilder (const std::string& fDef, QGAMES::FormBuilder* fB)
			:  QGAMES::StandardGUISystemBuilder (fDef, fB)
							{ }

		protected:
		/** @see parent. */
		virtual QGAMES::Widget* createWidget (const QGAMES::GUISystemBuilder::WidgetDefinition& def);
	};
	// --------------------------------------------------------------------------------

	// -------The input handler-------------------------------------------------------------------------
	class InputHandlerBehaviour : public QGAMES::InputHandler::StandardBehavior
	{
		public:
		InputHandlerBehaviour ()
			: QGAMES::InputHandler::StandardBehavior ()
							{ }

		/** @see parent. */
		virtual void onKeyboardStatus (const std::vector <bool>& kS);
		virtual void manageKeyOnState (QGAMES::GameState* st, int kP);
		virtual void manageJoystickMovementOnState (QGAMES::GameState* st, int nJ, const QGAMES::Vector& dr);
		virtual void manageJoystickButtonOnState (QGAMES::GameState* st, int nJ, bool p, int nB, int bF);
	};
	// --------------------------------------------------------------------------------

	// -------The score objects-------------------------------------------------------------------------
	/** The score object representing the left number of lives of the current player. */
	class NumberOfLivesScoreObject : public QGAMES::ScoreObject
	{
		public:
		NumberOfLivesScoreObject ();

		/** @see parent. 
			It hasn't either movements, nor animations, nor states to copy. */
		virtual QGAMES::Entity* clone () const
							{ return (new NumberOfLivesScoreObject ()); }

		/** @see parent. */
		virtual void drawOn (QGAMES::Screen* scr, const QGAMES::Position& p);
	};

	/** The score object to indicate the number of pointt of the current player. */
	class NumberOfPointsScoreObject : public QGAMES::ScoreObjectNumber
	{
		public:
		NumberOfPointsScoreObject ();

		/** @see parent. 
			It hasn't either movements, nor animations, nor states to copy. */
		virtual QGAMES::Entity* clone () const
							{ return (new NumberOfPointsScoreObject ()); }

		/** @see parent. */
		virtual void updatePositions ();

		protected:
		/** @see parent */
		virtual int formToDrawDigits ()
							{ return (__QGAMES_RAVIE24BLUELETTERS__); }
	};

	/** This score object is to indicate how many shooting the player has still. */
	class NumberOfShootingsScoreObject : public QGAMES::ScoreObjectNumber
	{
		public:
		NumberOfShootingsScoreObject ();

		/** @see parent. 
			It hasn't either movements, nor animations, nor states to copy. */
		virtual QGAMES::Entity* clone () const
							{ return (new NumberOfShootingsScoreObject ()); }

		/** @see parent. */
		virtual void updatePositions ();

		protected:
		/** @see parent */
		virtual int formToDrawDigits ()
							{ return (__QGAMES_RAVIE12GREENLETTERS__); }
	};
	// --------------------------------------------------------------------------------

	// -------The characters-------------------------------------------------------------------------
	/** Defines the basic entity managed by the user and used in this test. */
	class SpaceBattleShip : public QGAMES::Artist
	{
		public:
		SpaceBattleShip (int id, 
				const QGAMES::Forms& frms = QGAMES::Forms (), const QGAMES::Entity::Data& dt = QGAMES::Entity::Data ())
			: QGAMES::Artist (id, frms, dt),
			  _powerLevel (0)
							{ }

		virtual Entity* clone () const;

		/** @see parent. */
		virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a);

		// Controlling the movements...
		void toTheRight ();
		void toTheRightForward ();
		void toTheRightBackward ();
		bool goingOnlyRight () const;
		bool goingRight () const;
		bool goingRightForward () const;
		bool goingRightBackward () const;
		void toTheLeft ();
		void toTheLeftForward ();
		void toTheLeftBackward ();
		bool goingOnlyLeft () const;
		bool goingLeft () const;
		bool goingLeftForward () const;
		bool goingLeftBackward () const;
		void toForward ();
		bool goingForward () const;
		void toBackward ();
		bool goingBackward () const;
		void toShoot ();
		void toStay ();
		bool staying () const;
		void toRotate ();
		bool rotating () const;
		void toExplode ();
		bool exploding () const;
		void toDie ();
		bool died () const;
		bool moving () const
							{ return (!exploding () && !died ()); }

		// Things related with the shooting.
		/** To change the type of shooting. 
			When the power level is set, thenis also changed at configuration level. */
		int powerLevel () const
							{ return (_powerLevel); }
		/** When the power level is set, the max number of shooting can also be set.
			-1 means that the default number will be used.
			Other number will be the maximum possible. */
		void setPowerLevel (int pL, int mNS = -1);
		/** What these methods return will depend on the type of shooting in each moment. */
		int typeOfShooting () const
							{ return (_POWERDATA [_powerLevel][0]); }
		int maxNumberOfShootings () const
							{ return (_POWERDATA [_powerLevel][1]); }
		int instancesPerShooting () const
							{ return (_POWERDATA [_powerLevel][2]); }
		int shootingEnergy () const
							{ return (_POWERDATA [_powerLevel][3]); }
		int shootingsReaming () const
							{ return (_shootingsRemaining); }
		void reduceShootingsReaming (int a)
							{ if ((_shootingsRemaining = _shootingsRemaining - a) < 0) _shootingsRemaining = 0; }
		QGAMES::Position centralShootingPosition () const;

		/** @see parent. */
		virtual QGAMES::Rectangle collisionZone () const;

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p = QGAMES::Position::_noPoint);

		/** @see parent. */
		virtual void whenCollisionWith (QGAMES::Entity* e);

		private:
		/** The power level is get initially from the configuration object. */
		int _powerLevel;

		// Implementation
		int _shootingsRemaining;
		/** per type of power:
			type of shooting (animation), 
			max number of shootings simultaneously in the screen,
			instances of the shooting shooted at the same time,
			the energy of the shooting (neccesary to destroy an asteroid. 
			The max number of shootings before loading again. */
		static const int _POWERDATA [__GAMETEST_BATTLESHIPMAXPOWER__][5];
	};

	/** The class representing the battleship shooting. */
	class SpaceBattleShipShooting : public QGAMES::Artist
	{
		public:
		SpaceBattleShipShooting (int id, 
				const QGAMES::Forms& frms = QGAMES::Forms (), const QGAMES::Entity::Data& dt = QGAMES::Entity::Data ())
			: QGAMES::Artist (id, frms, dt),
			  _energy (0),
			  _dissapearNextLoop (false)
							{ }

		/** @see parent. */
		virtual Entity* clone () const;

		/** To get set the energy. 
			The energy is set when shooting using information from the power level of the battleship. */
		int energy () const
							{ return (_energy); }
		void setEnergy (int e)
							{ _energy = e; }

		// Controlling the movements
		/** The parameter includes the type of shooting to move. */
		void toMove (int type);
		bool moving () const;
		void toDissapearNextLoop ()
							{ _dissapearNextLoop = true; }
		void toDissapear ();
		bool dissapeared () const;

		/** @see parent. */
		virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
							{ return (position ().posY () > __BD -100); }

		/** @see parent. */
		virtual QGAMES::Rectangle collisionZone () const;

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

		/** @see parent. */
		virtual void whenCollisionWith (QGAMES::Entity* e);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		private:
		int _energy;

		// Implementation
		bool _dissapearNextLoop;
	};

	/** The entities representing the asteroids */
	class Asteroid : public QGAMES::Artist
	{
		public:
		Asteroid (int id, 
				const QGAMES::Forms& frms = QGAMES::Forms (), const QGAMES::Entity::Data& dt = QGAMES::Entity::Data ())
			: QGAMES::Artist (id, frms, dt),
			  _initialEnergy (100),
			  _energy (100), // It will be set when it starts to move
			  _explodingNextLoop (false),
			  _dissapearNextLoop (false)
							{ }

		int energy () const
							{ return (_energy); }
		int initialEnergy () const
							{ return (_initialEnergy); }

		/** @see parent. */
		virtual Entity* clone () const;

		// Controlling the movements
		/** The parameter includes the type of asteroid to move. */
		void toMove (int type);
		bool moving () const;
		void toExplode ();
		bool exploding () const;

		void toDissapearNextLoop ()
							{ _dissapearNextLoop = true; }
		void toDissapear ();
		bool dissapeared () const;

		/** @see parent. */
		virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
							{ return (position ().posY () < __BD (__GAMETEST_SCREENHEIGHT__ + 100)); }

		/** @see parent. */
		virtual QGAMES::Rectangle collisionZone () const;

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

		/** @see parent. */
		virtual void whenCollisionWith (QGAMES::Entity* e);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		private:
		/** Both are set when start to move, usingthetype as basic parameter. 
			When the asteroid is destroyed, then part of the initial energy it had is added to the player score. */
		int _initialEnergy;
		int _energy;

		// Implementation
		bool _explodingNextLoop;
		bool _dissapearNextLoop;

		/** energy of asteroid per type. */
		static const int _ENERGY [__GAMETEST_ASTEROIDNUMBERTYPES__];
	};

	/** The entity representing a shooting to catch in the space. */
	class ShootingToCatch : public QGAMES::Artist
	{
		public:
		ShootingToCatch (int id, 
				const QGAMES::Forms& frms = QGAMES::Forms (), const QGAMES::Entity::Data& dt = QGAMES::Entity::Data ())
			: QGAMES::Artist (id, frms, dt),
			  _dissapearNextLoop (false)
							{ }

		/** @see parent. */
		virtual Entity* clone () const;

		// Controlling the movements
		/** The parameter includes the type of asteroid to move. */
		void toMove (int type);
		bool moving () const;
		void toDissapearNextLoop ()
							{ _dissapearNextLoop = true; }
		void toDissapear ();
		bool dissapeared () const;

		/** @see parent. */
		virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
							{ return (position ().posY () < __BD (__GAMETEST_SCREENHEIGHT__ + 100)); }

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

		/** @see parent. */
		virtual void whenCollisionWith (QGAMES::Entity* e);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		private:
		// Implementation
		bool _dissapearNextLoop;
	};

	/** To create basic entities. */
	class EntityBuilder : public QGAMES::AdvancedEntityBuilder
	{
		public:
		EntityBuilder (const std::string& fDef, QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
			: QGAMES::AdvancedEntityBuilder (fDef, fB, mB)
							{ }
	
		private:
		virtual QGAMES::Entity* createEntity (const EntityDefinition& def);
	};
	// --------------------------------------------------------------------------------

	// -------The movements-------------------------------------------------------------------------
	/** To create basic movements. */
	class MovementBuilder : public QGAMES::AdvancedMovementBuilder
	{
		public:
		MovementBuilder (const std::string& fDef)
			: QGAMES::AdvancedMovementBuilder (fDef)
							{ }
	
		private:
		virtual QGAMES::Movement* createMovement (const MovementDefinition& def);
	};
	// --------------------------------------------------------------------------------

	// -------The world, scenes and maps-------------------------------------------------------------------------
	/** The class defining the head world. */
	class World : public QGAMES::World
	{
		public:
		World (int c, const QGAMES::Scenes& s, 
				const QGAMES::WorldProperties& p = QGAMES::WorldProperties ())
			: QGAMES::World (c, s, p)
							{ }

		/** @see parent. */
		void initialize ();

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);
	};

	/** The class defining the scene inside the world above used in the test. */
	class Scene : public QGAMES::Scene
	{
		public:
		Scene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
					const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
					const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
				: QGAMES::Scene (c, m, cn, p, ePL),
				  _level (0),
				  _freeShootings (),
				  _freeAsteroids (),
				  _shootingToCatch (NULL),
				  _timeInLevel (NULL)
							{ }

		/** To set the level of the scene. 
			When the level is set the is also changed at configuration level. */
		int level () const
							{ return (_level); }
		void setLevel (int l);

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		private:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
								{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
								{ return (new OnOffSwitches ()); }

		/** Try to shoot. */
		void tryToActivateNewShootingFrom (SpaceBattleShip* bS);

		private:
		/** The level is get initially from the one kept in the configuation object. */
		int _level;

		// Implementation
		std::list <QGAMES::Entity*> _freeShootings;
		std::list <QGAMES::Entity*> _freeAsteroids;
		ShootingToCatch* _shootingToCatch;
		QGAMES::LoopCounter* _timeInLevel;

		/** To count before s new shooting is possible. */
		static const int _COUNTERTOSHOOT = 0;
		/** To count before a new asteroid could appear. */
		static const int _COUNTERTOASTEROID = 1;
		/** To count before a new battleship shooting could appear on the scree, */
		static const int _COUNTERBATLESHIPSHOOTING = 2;
		/** Switch to control whether is possible to shoot or not. */
		static const int _SWITCHCANSHOOT = 0;

		/** per level:
			seconds to decide whether a new asteroid has to appear,
			probability a new asteroid appears,
			probability a new type of power appears,
			seconds in the level. */
		static const QGAMES::bdata _LEVEL [__GAMETEST_MAXNUMBERLEVELS__][4];
	};

	/** The extension to create worlds. */
	class WorldBuilder : public QGAMES::WorldBuilder
	{
		public: 
		WorldBuilder (const std::string& eDef, QGAMES::MapBuilder* mB)
			: QGAMES::WorldBuilder (eDef, mB)
							{ }

		private:
		virtual QGAMES::World* createWorldObject (int no, const QGAMES::Scenes& s, 
			const QGAMES::WorldProperties& p);
		virtual QGAMES::Scene* createSceneObject (int ns, const QGAMES::Maps& m, 
			const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL);
	};
	// --------------------------------------------------------------------------------

	// -------The Game States-------------------------------------------------------------------------
	/** 
	  *	PRINCIPAL GAME STATE:
	  *	The state to control the rest. 
	  *	What is important here is to define the first game state in the process and the rules to move to the next ones.
	  */
	class ControllingGameStates : public QGAMES::AdvancedGameStateControl
	{
		public:
		ControllingGameStates (QGAMES::GameState* psSt);

		protected:
		/** @see parent. */
		virtual std::string firstGameState () const
							{ return (std::string (__QGAMES_GAMESTATELOADINGNAME__)); }
		virtual std::string nextGameState (QGAMES::GameState* st, const QGAMES::Event& evnt);
	};

	/** A state that finishes when the return key or the mouse left button is pressed. */
	class UntilReturnKeyOrMouseLeftButtonIsPressed : public QGAMES::ForEverUntilInputHandlerEventGameState
	{
		public:
		UntilReturnKeyOrMouseLeftButtonIsPressed (int id, GameState* nS = NULL)
			: QGAMES::ForEverUntilInputHandlerEventGameState (id, new StopStateHandler, nS)
							{ }

		private:
		class StopStateHandler : public QGAMES::ForEverUntilInputHandlerEventGameState::Handler
		{
			public:
			StopStateHandler ()
				: QGAMES::ForEverUntilInputHandlerEventGameState::Handler ()
							{ }

			/** @see parent. */
			virtual bool stopWhen (const QGAMES::Event& evnt);
		};
	};

	/** 
	  * PRINCIPAL GAME STATE:
	  * The game starts with a Commty Logo appearing little by little. 
	  */
	class IntroLogoGameState : public QGAMES::SpendingTimeGameState
	{
		public:
		IntroLogoGameState ()
			: QGAMES::SpendingTimeGameState (__GAMETEST_INTROGAMESTATE__, __BD 10 /** Seconds on the screen after appearing */, 
				new QGAMES::ShowingAppearingFormGameState (__GAMETEST_INTROGAMESTATE__ + 1, 
					QGAMES::ShowingAppearingFormGameState::Properties 
						(__QGAMES_COMMTYNETSBIGLOGO__, 
						 0, /** Initial fade */ 
						 255, /** Final fade */
						 10, /** Fade increment */
						 1, /** loops before increment the fade */ 
						 QGAMES::Vector (__BD 0.5, __BD 0.5, __BD 0), /** Where. Relative position on the screen */ 
						 QGAMES::ShowingAppearingFormGameState::Properties::PosReference::_CENTER /** Position refered to the center of the logo */
						)))
							{ }
	};

	/** 
	  * The credit title game state has here two simple modifications
	  *	because the title with the name of the game is a little bit bigger (and in another color) tha the rest.
	  *	Additionally, when that title is on the screen, a battleship is shown also behind. 
	  *	Take care that the the nested game state passed as paremeter is nested in the battleship for game state
	  *	and not in the main one. 
	  */
	class PresentationTitlesGameState : public QGAMES::CreditTitlesGameState
	{
		public:
		PresentationTitlesGameState (int id, 
			const QGAMES::CreditTitlesGameState::Properties& prps, QGAMES::GameState* nS = NULL)
			: QGAMES::CreditTitlesGameState (id, prps, 
				new QGAMES::ShowingFixFormGameState (id * 1000, /** To avoid two states with the same id */ 
					QGAMES::ShowingFixFormGameState::Properties 
						(__GAMETEST_BATTLESHIPPRSENTATIONFORM__, 
						 255, /** Fade */ 
						 QGAMES::Position (__BD 0.5, __BD 0.5, __BD 0.5), /** Where. Relativeposition to the center of the screen */
						 QGAMES::ShowingFixFormGameState::Properties::PosReference::_CENTER /** Refered to the center of the form */
						), nS))
							{ }

		/** @see parent. */
		virtual void drawOn (QGAMES::Screen* s);

		protected:
		/** @see parent. */
		virtual QGAMES::ScoreObjectText* createText (const std::string& txt);
	};

	/** 
	  * PRINCIPAL GAME STATE:
	  * Then the presentation takes place.
	  * A set of titles appear on the screen until RETURN key o left mouse button are pressed.
	  * Behind the letters a set of backgrounds roll from right to left whilst a music plays.
	  * The state creates the nested states it needs, and destroy them when it is destroyed.
	  */
	class PresentationGameState : public UntilReturnKeyOrMouseLeftButtonIsPressed
	{
		public:
		PresentationGameState ();
		~PresentationGameState ();

		PresentationTitlesGameState* _creditTitles;
		QGAMES::ShowingFixFormGameState* _logoForm;
		QGAMES::RollingFormsGameState* _rollingBackground;
		QGAMES::PlayingASoundGameState* _playingSound;
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
					(__GAMETEST_BATTLESHIPID__, 
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

	/** 
	  * PRINCIPAL GAME STATE:
	  * This state shows the standard menu, but with a start background rolling behind and the 
	  * battleship moving in the middle right side of the screen (using the previous state).
	  * The state finishes when a valid option is selected.
	  * The state creates the nested stated it needs, and destroy them when it is destroyed as well.
	  */
	class SelectionGameState : public QGAMES::StandardInitialSelectionOptionsGameState
	{
		public:
		SelectionGameState ();
		~SelectionGameState ();

		/** @see parent. */
		virtual void onEnter ();

		private:
		QGAMES::ShowingFixFormGameState* _logoForm;
		BattleshipMovingGameState* _battleshipEntity;
		QGAMES::RollingFormsGameState* _rollingBackground;
		QGAMES::PlayingASoundGameState* _playingSound;
	};

	/** 
	  * This game state shows the number of the player that is about to start to play.
	  */
	class ShowingPlayerAboutToStart : public QGAMES::ShowingTextGameState
	{
		public:
		ShowingPlayerAboutToStart (int id, QGAMES::GameState* nS = NULL)
			: QGAMES::ShowingTextGameState (id, 
				QGAMES::ShowingTextGameState::Properties 
					(255, /** Fade */
					 QGAMES::Position (__BD 0.5, __BD 0.4, __BD 0), /** Relative position of the letters in the screen */ 
					 QGAMES::ShowingTextGameState::Properties::PosReference::_CENTER, /** Reference position of the text (center) */
					 QGAMES::Vector (__BD 1, __BD 0, __BD 0) /** Orientation of the letters of the text (horizontal) */
					), nS)
							{ }

		private:
		/** @see parent. */
		virtual void actualizeText ();
		virtual QGAMES::ScoreObjectText* createText ()
							{ return (new QGAMES::Ravie36BlueFont (std::string ("---"))); }
	};

	/** 
	  * The game state that shows the level of the scene where the current player is about to start to play.
	  */
	class ShowingLevelCurrentPlayer: public QGAMES::ShowingTextGameState
	{
		public:
		ShowingLevelCurrentPlayer (int id, QGAMES::GameState* nS = NULL)
			: QGAMES::ShowingTextGameState (id, 
				QGAMES::ShowingTextGameState::Properties 
					(255, /** Fade */
					 QGAMES::Position (__BD 0.5, __BD 0.6, __BD 0), /** Reletve position of the letters in the screen */
					 QGAMES::ShowingTextGameState::Properties::PosReference::_CENTER, /* Reference position of the text (center) */
					 QGAMES::Vector (__BD 1, __BD 0, __BD 0) /** Orientation of the text (horizontal) */
					), nS)
							{ }

		private:
		/** @see parent. */
		virtual void actualizeText ();
		virtual QGAMES::ScoreObjectText* createText ()
							{ return (new QGAMES::Ravie36BlueFont (std::string ("---"))); }
	};

	/**
	 * PRINCIPAL GAME STATE:
	 * This game state is shown when everything is about to start. 
	 * The player takes a short breathe before the battle starts.
	 * The game state crates what it needs and destroy the nested states when finishes.
	 * If RETURN key or left mouse button are pressed or certain time of time goes fly the game starts.
	 */
	class GameAboutToStart : public QGAMES::SpendingTimeGameState
	{
		public:
		GameAboutToStart ();
		~GameAboutToStart ();

		bool nameIntroduced () const
							{ return (_nameIntroduced); }

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void onExit ();

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		private:
		void deleteGUISystem ();

		private:
		UntilReturnKeyOrMouseLeftButtonIsPressed* _untilPressed;
		ShowingPlayerAboutToStart* _numberPlayer;
		ShowingLevelCurrentPlayer* _levelPlayer;
		QGAMES::ShowingFixFormGameState* _background;
		QGAMES::ShowingFixFormGameState* _stars;

		// Implementation
		QGAMES::GUISystem* _guiSystem;
		bool _nameIntroduced;	
	};

	/** 
	  * PRINCIPAL GAME STATE:
	  * When the player is playing.
	  * This the central game state. 
	  * The battleship fight against the elements and stars rolls in the back,
	  * with a emotional music playing (if it has been allowed at configuration time) also as background.
	  *	The game finishes whether when the level has concluded or when the player has dead.
	  * The reason of the exit is kept into a variable in order the game state control shall decide what is next.
	  * The game state creates what it needs, and the destroy everything when finishes.
	  */
	class Playing : public QGAMES::GameState
	{
		public:
		typedef enum { 
			_STILLPLAYING = 0, _LEVELACHIEVED, _PLAYERDIED } Result;

		Playing ();
		~Playing ();

		/** To get the rolling background 
			because a similar one is used in other states running just after this one
			and the internal status have to be the similar in order to create a 
			continuity effect. */
		QGAMES::RollingFormsGameState* rollingBackground ()
							{ return (_rollingBackground); }
		const QGAMES::RollingFormsGameState* rollingBackground () const
							{ return (_rollingBackground); }

		/** To know which was the result of the game state when it finishes. */
		Result result () const
							{ return (_result); }

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void onExit ();

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		private:
		Result _result;

		QGAMES::RollingFormsGameState* _rollingBackground;
		QGAMES::PlayingASoundGameState* _playingSound;
	};

	/** 
	 * PRINCIPAL GAME STATE:
	 * The player has been killed. This is the state to indicate it. 
	 * Who control what is next, is the Control Game State over this one. It could be to start whit the next plyer if any,
	 * or even to finishes the game if there is no more players.
	 * In this state the world is still visible and just a simple sound sounds.
	 * What it is important here is that when the game state finished the control is moved to the next player.
	 * This state uses the nested state defined at the previous one "rolling the stars", but it is not the owner.
	 */
	class PlayerHasDied : public QGAMES::SpendingTimeGameState
	{
		public:
		PlayerHasDied (Playing* ply);
		~PlayerHasDied ();

		/** @see parent. */
		virtual void onEnter ();
		virtual void onExit ();

		private:
		Playing* _playing; /** A reference to the previous state. */
		QGAMES::RollingFormsGameState* _rollingBackground; /** Remember this state doesn't own this other. */
		QGAMES::PlayingASoundGameState* _playingSound;
	};

	/**
	 * PRINCIPAL GAME STATE:
	 * When the level has completed, and it is time to go to the next level.
	 * What happen in this case, it that the game continues like the state previous,
	 * with the battleship in the middle of the screen and a sound playing in the background.
	 * The stars rolls in background in the same way the did it in the previous one.
	 * So this state uses that nested defined defined above, but it is not the owner
	 * in a similar way than happened in the PlayerHasDied state.
	 */
	class LevelCompleted : public QGAMES::SpendingTimeGameState
	{
		public:
		LevelCompleted (Playing* ply);
		~LevelCompleted ();

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s);
		virtual void onExit ();

		private:
		Playing* _playing;
		SpaceBattleShip* _battleship;
		QGAMES::RollingFormsGameState* _rollingBackground; /** Remember this state doesn't own this other. */
		QGAMES::PlayingASoundGameState* _playingSound;
	};

	/** The game state that shows the game has ended. */
	class ShowingGameHasEnded: public QGAMES::ShowingTextGameState
	{
		public:
		ShowingGameHasEnded (int id, QGAMES::GameState* nS = NULL)
			: QGAMES::ShowingTextGameState (id,
				QGAMES::ShowingTextGameState::Properties
					(255, /** Fade */
					 QGAMES::Position (__BD 0.5, __BD 0.5, __BD 0), /** Relative position in the screen */ 
					 QGAMES::ShowingTextGameState::Properties::PosReference::_CENTER, /** Reference position of the text (center) */
					 QGAMES::Vector (__BD 1, __BD 0, __BD 0) /** Orientation of the text (horizontal) */
					), nS)
							{ }

		private:
		/** @see parent. */
		virtual QGAMES::ScoreObjectText* createText ()
							{ return (new QGAMES::Ravie72OrangeFont (std::string ("Game Over"))); }
	};

	/** 
	 * PRINCIPAL GAME STATE:
	 * The game has finishes as nobody has enough lives to continue.
	 * Who control what is next, is the Control Game State over this one. In this case is to go back to initial menu.
	 * What happen in this game is as simple as a text is shown to indicate the situation with a image and stars in the background.
	 * A simple music is also indicating that situation.
	 */
	class GameHasEnded : public QGAMES::SpendingTimeGameState
	{
		public:
		GameHasEnded ();
		~GameHasEnded ();
		
		/** @see parent. */
		virtual void onExit ();

		private:
		ShowingGameHasEnded* _text;
		QGAMES::ShowingFixFormGameState* _background;
		QGAMES::ShowingFixFormGameState* _stars;
		QGAMES::PlayingASoundGameState* _playingSound;
	};

	/** 
	  * The specific class to show the different entries of the hall of fame. 
	  *	The class inherits from the one defined at AdvancedArcade library redefining how entries are created. 
	  */
	class ShowingEntriesHallOfFame : public QGAMES::ShowingHallOfFameGameState 
	{
		public:
		ShowingEntriesHallOfFame (int id, GameState* nS = NULL)
			: QGAMES::ShowingHallOfFameGameState (id, 
				QGAMES::ShowingHallOfFameGameState::Properties 
					(255, /** Fade */
					 QGAMES::Position (__BD 10 / __BD __GAMETEST_SCREENWIDTH__, 
									   __BD 10 / __BD __GAMETEST_SCREENHEIGHT__, 
									   __BD 0), /** Relative position of the first letter of the first entry*/
					 10 /** Separation between entries */
					), nS)
							{ }

		private:
		/** @see parent. */
		virtual std::map <std::string, QGAMES::SetOfOpenValues> orderHallOfFame (const std::map <std::string, QGAMES::SetOfOpenValues>& hF);
		// The text created is just with the score adjusted to 5 digits and the name of the player. */
		virtual QGAMES::ScoreObjectText* createTextFor (const std::string& eK, const QGAMES::SetOfOpenValues& sV);
	};

	/**
	  * PRINCIPAL GAME STATE:
	  * The hall of fame. The entries are show with a background of start rolling behind. 
	  * Everything until a return key or a mouse left button is pressed. 
	  */
	class HallOfFameGameState : public UntilReturnKeyOrMouseLeftButtonIsPressed
	{
		public:
		HallOfFameGameState ();
		~HallOfFameGameState ();

		UntilReturnKeyOrMouseLeftButtonIsPressed* _hallOfFame;
		QGAMES::ShowingFixFormGameState* _logoForm;
		QGAMES::RollingFormsGameState* _rollingBackground;
		QGAMES::PlayingASoundGameState* _playingSound;
	};
	// --------------------------------------------------------------------------------

	// -------The Game-------------------------------------------------------------------------
	class Game : public QGAMES::AdvancedArcadeGame
	{
		public:
		/** The configuration adds things important for every player. */
		class Conf : public QGAMES::AdvancedArcadeGame::Conf
		{
			public:
			Conf (int nP)
				: QGAMES::AdvancedArcadeGame::Conf (nP),
				  _score (), _weaponsLevel (), _level (), _playerName ()
							{ assert (_numberPlayers > 0 && _numberPlayers < __GAMETEST_MAXNUMBERPLAYERS__);
							  adjustToPlayers (nP); }

			/** @see parent. */
			virtual Configuration* clone () const
							{ return (new Conf (*this)); }

			// To Read & Set the internal attributes
			// nP from 1 to the max number of users
			int score (int nP) const 
							{ assert (nP > 0 && nP <= (int) _score.size ());
							  return (_score [nP - 1]); }
			void setScore (int nP, int s)
							{ assert (nP > 0 && nP <= (int) _score.size ());
							  _score [nP - 1] = (s >= 0) ? s : 0; }
			int weaponsLevel (int nP) const 
							{ assert (nP > 0 && nP <= (int) _weaponsLevel.size ());
							  return (_weaponsLevel [nP - 1]); }
			void setWeaponsLevel (int nP, int wL)
							{ assert (nP > 0 && nP <= (int) _weaponsLevel.size ());
							  _weaponsLevel [nP - 1] = (wL >= 0) ? wL : 0; }
			int level (int nP) const 
							{ assert (nP > 0 && nP <= (int) _level.size ());
							  return (_level [nP - 1]); }
			void setLevel (int nP, int l)
							{ assert (nP > 0 && nP <= (int) _level.size ());
							  _level [nP - 1] = (l >= 0) ? l : 0; }
			const std::string& playerName (int nP) const 
							{ assert (nP > 0 && nP <= (int) _playerName.size ());
							  return (_playerName [nP - 1]); }
			void setPlayerName (int nP, const std::string& n)
							{ assert (nP > 0 && nP <= (int) _playerName.size ());
							  _playerName [nP - 1] = n; }

			/** To change the hall of fame. */
			void actualizeHallOfFame ();

			protected:
			/** @see parent. */
			virtual void adjustToPlayers (int nP);

			/** @see parent. */
			virtual void cfgToStream (std::ostringstream& oS);
			virtual void cfgFromStream (std::istringstream& iS);

			protected:
			std::vector <int> _score;
			std::vector <int> _weaponsLevel;
			std::vector <int> _level;
			std::vector <std::string> _playerName;
		};

		Game ()
			: QGAMES::AdvancedArcadeGame ()
							{ }

		// Information about the players
		// nP = -1 will is equivalent to current player...
		/** The name of the player. */
		const std::string& playerName (int nP = -1) const
							{ return (((Game::Conf*) configuration ()) -> 
								playerName ((nP == -1) ? currentPlayer () : nP)); }
		void setPlayerName (const std::string& pN, int nP = -1)
							{ ((Game::Conf*) configuration ()) -> 
								setPlayerName ((nP == -1) ? currentPlayer () : nP, pN); }
		/** The level. */
		int level (int nP = -1) const
							{ return (((Game::Conf*) configuration ()) -> 
								level ((nP == -1) ? currentPlayer () : nP)); }
		void setLevel (int l, int nP = -1)
							{ ((Game::Conf*) configuration ()) -> 
								setLevel ((nP == -1) ? currentPlayer () : nP, l); }
		void setNextLevel (int nP = -1)
							{ setLevel (level (nP) + 1, nP); }
		/** The manage the level of weapons managed. */
		int weaponsLevel (int nP = -1) const
							{ return (((Game::Conf*) configuration ()) -> 
								weaponsLevel ((nP == -1) ? currentPlayer () : nP)); }
		void setweaponsLevel (int wL, int nP = -1)
							{ ((Game::Conf*) configuration ()) -> 
								setWeaponsLevel ((nP == -1) ? currentPlayer () : nP, wL); }
		void setNextWeaponsLevel (int nP = -1)
							{ setweaponsLevel (weaponsLevel (nP) + 1, nP); }
		/** The manage the score. */
		int score (int nP = -1) const
							{ return (((Game::Conf*) configuration ()) -> 
								score ((nP == -1) ? currentPlayer () : nP)); }
		void setScore (int s, int nP = -1)
							{ ((Game::Conf*) configuration ()) -> 
								setScore ((nP == -1) ? currentPlayer () : nP, s); }
		void addScore (int a, int nP = -1)
							{ setScore (score (nP) + a, nP); }
		/** To know the number of shootings still pending. 
			This number is not part of the configuration. 
			It always returns the number for the current player. */
		int shootingsRemaining ();

		/** Actualize the hall of fame, with the last results. */
		void actualizeHallOfFame ()
							{ ((Game::Conf*) configuration ()) -> actualizeHallOfFame (); }

		// The rest of the configuration attributes are not directly accesible

		/** @see parent. */
		virtual void addScoreObjects ();
		virtual void removeScoreObjects ();

		private:
		/** see parent. */
		virtual QGAMES::EntityBuilder* createEntityBuilder ()
							{ return (new EntityBuilder (parameter (__GAME_PROPERTYENTITIESFILE__), 
								formBuilder (), movementBuilder ())); }
		virtual QGAMES::GUISystemBuilder* createGUISystemBuilder ()
							{ return (new GUISystemBuilder (game () -> parameter (__GAME_PROPERTYGUISYSTEMFILE__), 
								game () -> formBuilder ())); }
		virtual QGAMES::MovementBuilder* createMovementBuilder () 
							{ return (new MovementBuilder (parameter (__GAME_PROPERTYMOVEMENTSFILE__))); }
		virtual QGAMES::InputHandler* createInputHandler ()
							{ return (implementation () -> createInputHandler (new InputHandlerBehaviour ())); }
		virtual QGAMES::Screens createScreens ()
							{ QGAMES::Screens result;
								result.insert (QGAMES::Screens::value_type (__QGAMES_MAINSCREEN__, 
									implementation () -> createScreen 
										(std::string ("Battleship Commander"),
										QGAMES::Position::_cero, __GAMETEST_SCREENWIDTH__, __GAMETEST_SCREENHEIGHT__, 100, 100)));
								return (result); }
		virtual QGAMES::WorldBuilder* createWorldBuilder ()
							{ return (new WorldBuilder (parameter (__GAME_PROPERTYWORLDSFILE__), 
								mapBuilder ())); }
		virtual QGAMES::MapBuilder* createMapBuilder ()
							{ QGAMES::MapBuilder* result = new QGAMES::MapBuilder (parameter (__GAME_MAPSOBJECTSFILE__));
								result -> addAddsOn (new QGAMES::ObjectMapBuilderAddsOn (objectBuilder ()));
								return (result); }
		virtual QGAMES::CharacterControlStepsMonitorBuilder* createCharacterMonitorBuilder ()
							{ return (new QGAMES::CharacterControlStepsMonitorBuilder 
								(parameter (__GAME_PROPERTUCHARACTERMONITORFILE__))); }

		/** @see parent. */
		virtual Configuration* createConfiguration ()
							{ return (new Conf (1)); } // Just one player in this game...

		/** @see parent. */
		virtual void initialize ();
		virtual void additionalTasks ();
	};
	// --------------------------------------------------------------------------------
}

#endif
  
// End of the file
/*@}*/
