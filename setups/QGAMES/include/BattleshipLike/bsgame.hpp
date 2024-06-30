/** \ingroup Battleship */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: bsgame.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/11/2019 \n
 *	Description: Defines the body of the Battleship games.
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_GAME__
#define __BATTLESHIP_GAME__

#include <BattleshipLike/bsdefinitions.hpp>
#include <BattleshipLike/bsdatagame.hpp>
#include <BattleshipLike/bsbuilders.hpp>
#include <BattleshipLike/bsgamestates.hpp>
#include <BattleshipLike/bsscoreobjects.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace BATTLESHIP
{
	/** This class defines things really common in all advanced games. */
	class Game : public QGAMES::AdvancedArcadeGame
	{
		public:
		/** Represent a potential element carried by the battlehip. \n
			That is stored into the configuration data file. \n
			If it hasn't been defined, an empty Transported Element with just one null element will be kept only. */
		struct TransportedElement
		{
			TransportedElement ()
				: _id (-1), _type (-1), _values ()
								{ }
			TransportedElement (int id, int tp, const QGAMES::SetOfOpenValues& oV = QGAMES::SetOfOpenValues ())
				: _id (id), _type (tp), _values (oV)
								{ }

			QGAMES::SetOfOpenValues likeSetOfOpenValues () const;

			friend std::ostream& operator << (std::ostream& oS, const TransportedElement& tE);
			friend std::istream& operator >> (std::istream& iS, TransportedElement& tE);

			int _id;
			int _type;
			QGAMES::SetOfOpenValues _values;
		};

		/** All transported elements are grouped into this class. */
		struct TransportedElements
		{
			public:
			TransportedElements ()
				: _elements (3, TransportedElement ()) // The usual size could be three...
								{ }

			TransportedElements (int nE)
				: _elements (nE, TransportedElement ())
								{ assert (nE > 0); } // TThe size of the pocket has to be positive...

			void toIterateElements ()
								{ _elements [0] = toAddElement (TransportedElement ()); }
			TransportedElement toAddElement (const TransportedElement& pE);
			const TransportedElement& fistElement () const
								{ return (_elements [0]); }
			TransportedElement& firstElement ()
								{ return (_elements [0]); }
			const TransportedElement& lastElement () const
								{ return (_elements [_elements.size () - 1]); }
			TransportedElement& lastElement ()
								{ return (_elements [_elements.size () - 1]); }
			std::vector <int> listOfElementsType () const;
			bool isEmpty () const;

			friend std::ostream& operator << (std::ostream& oS, const TransportedElements& tE);
			friend std::istream& operator >> (std::istream& iS, TransportedElements& tE);

			std::vector <TransportedElement> _elements;
		};

		/** The configuration. */
		class Conf : public QGAMES::AdvancedArcadeGame::Conf
		{
			public:
			Conf (int nP, int dW, int dS);

			/** @see parent. */
			virtual Configuration* clone () const
							{ Conf* result = new Conf (*this); 
							  result -> cloneImplementation (this); 
							  return (result); }

			/** To know and set the default and scene used for the players in the game. */
			int defaultWorld () const
							{ return (_defaultWorld); }
			int defaultScene () const
							{ return (_defaultScene); }
			void setWorldAndDefaultScene (int dW, int dS)
							{ _defaultWorld = dW; _defaultScene = dS; }

			/** The elements transported if any. */
			const TransportedElements& transportedElements (int nP) const
							{ assert (nP > 0 && nP <= (int) _transportedElements.size ());
							  return (_transportedElements [nP - 1]); }
			void setTransportedElements (int nP, const TransportedElements& tE)
							{ assert (nP > 0 && nP <= (int) _transportedElements.size ());
							  _transportedElements [nP - 1] = tE; }

			// To Read & Set the internal attributes
			// nP from 1 to the max number of users
			int score (int nP) const 
							{ assert (nP > 0 && nP <= (int) _score.size ());
							  return (_score [nP - 1]); }
			void setScore (int nP, int s)
							{ assert (nP > 0 && nP <= (int) _score.size ());
							  _score [nP - 1] = (s >= 0) ? s : 0; }
			int level (int nP) const 
							{ assert (nP > 0 && nP <= (int) _level.size ());
							  return (_level [nP - 1]); }
			void setLevel (int nP, int l)
							{ assert (nP > 0 && nP <= (int) _level.size ());
							  _level [nP - 1] = (l >= 0) ? l : 0; }
			int shootingsLeft (int nP) const 
							{ assert (nP > 0 && nP <= (int) _shootingsLeft.size ());
							  return (_shootingsLeft [nP - 1]); }
			void setShootingsLeft (int nP, int l)
							{ assert (nP > 0 && nP <= (int) _shootingsLeft.size ());
							  _shootingsLeft [nP - 1] = (l >= 0) ? l : 0; }
			const std::string& playerName (int nP) const 
							{ assert (nP > 0 && nP <= (int) _playerName.size ());
							  return (_playerName [nP - 1]); }
			void setPlayerName (int nP, const std::string& n)
							{ assert (nP > 0 && nP <= (int) _playerName.size ());
							  _playerName [nP - 1] = n; }
			int artistType (int nP) const
							{ assert (nP > 0 && nP <= (int) _artistType.size ());
							  return (_artistType [nP - 1]); }
			void setArtistType (int nP, int l)
							{ assert (nP > 0 && nP <= (int) _artistType.size ());
							  _artistType [nP - 1] = (l >= 0) ? l : 0; }
			int playerSeconds (int nP) const
							{ assert (nP > 0 && nP <= (int) _playerSeconds.size ());
							  return (_playerSeconds [nP - 1]); }
			void setPlayerSeconds (int nP, int s) // The value can't ever be less than 0
							{ assert (nP > 0 && nP <= (int) _playerSeconds.size ());
							  _playerSeconds [nP - 1] = (s >= 0) ? s : 0; }
			bool sceneCompleted (int nP, int nS) const
							{ assert (nP > 0 && nP <= (int) _sceneCompleted.size ());
							  std::map <int, bool>::const_iterator i = _sceneCompleted [nP - 1].find (nS);
							  return ((i != _sceneCompleted [nP - 1].end ()) ? (*i).second : false); }
			void setSceneCompleted (int nP, int nS, bool c = true)
							{ assert (nP > 0 && nP <= (int) _sceneCompleted.size ());
							  _sceneCompleted [nP - 1][nS] = c; }

			/** To change the hall of fame. */
			void actualizeHallOfFame ();

			protected:
			/** @see parent. */
			virtual void adjustToPlayers (int nP);

			/** @see parent. */
			virtual void cfgToStream (std::ostringstream& oS);
			virtual void cfgFromStream (std::istringstream& iS);

			protected:
			/** The elements transported if any. */
			std::vector <TransportedElements> _transportedElements; 
			std::vector <int> _score;
			std::vector <int> _level;
			std::vector <int> _shootingsLeft;
			std::vector <std::string> _playerName;
			std::vector <int> _artistType; // e.g. 0 Male, 1 Female,...It must be managed by the implementation...
			std::vector <int> _playerSeconds;
			std::vector <std::map <int, bool>> _sceneCompleted;

			// Implementation
			int _defaultWorld; /** The defalt world and scene used for any player. */
			int _defaultScene;
		};

		friend InitializeBattleshipGameState; // To allow the initialization of the game using a state...

		Game (QGAMES::GameImplementation* impl = NULL);

		~Game ()
							{ removeScoreObjects (); 
							  delete (_theDataGame); _theDataGame = NULL; }

		/** To get the data game. */
		const DataGame* dataGame () const
							{ return (_theDataGame); }
		DataGame* dataGame ()
							{ return (_theDataGame); }

		/** To activate or desactivate the detection of collisions. */
		void setDetectCollisions (bool dC)
							{ _detectCollisions = dC; }

		/** To know whether the game is or not on hold. 
			That's it, it is in playing state but with the options on top. */
		bool isNotPlaying () const;

		// Information about the players
		// nP = -1 will is equivalent to current player...
		/** The elements transported if any. */
		const TransportedElements& transportedElements (int nP = -1) const
							{ return (((Game::Conf*) configuration ()) -> 
								transportedElements ((nP == -1) ? currentPlayer () : nP)); }
		void setTransportedElements (const TransportedElements& tE, int nP = -1)
							{ ((Game::Conf*) configuration ()) -> 
								setTransportedElements ((nP == -1) ? currentPlayer () : nP, tE); }

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
		/** Manage the number of shootings left. */
		int shootingsLeft (int nP = -1) const
							{ return (((Game::Conf*) configuration ()) -> 
								shootingsLeft ((nP == -1) ? currentPlayer () : nP)); }
		void setShootingsLeft (int l, int nP = -1)
							{ ((Game::Conf*) configuration ()) -> 
								setShootingsLeft ((nP == -1) ? currentPlayer () : nP, l); }
		/** The manage the score. */
		int score (int nP = -1) const
							{ return (((Game::Conf*) configuration ()) -> 
								score ((nP == -1) ? currentPlayer () : nP)); }
		void setScore (int s, int nP = -1)
							{ ((Game::Conf*) configuration ()) -> 
								setScore ((nP == -1) ? currentPlayer () : nP, s); }
		void addScore (int a, int nP = -1)
							{ setScore (score (nP) + a, nP); }
		/** The type af artist playing. */
		int artistType (int nP = -1) const
							{ return (((Game::Conf*) configuration ()) -> 
								artistType ((nP == -1) ? currentPlayer () : nP)); }
		void setArtistType (int l, int nP = -1)
							{ ((Game::Conf*) configuration ()) -> 
								setArtistType ((nP == -1) ? currentPlayer () : nP, l); }
		/** The number of seconds the player is taking. */
		int playerSeconds (int nP = -1) const
							{ return (((Game::Conf*) configuration ()) -> 
								playerSeconds ((nP == -1) ? currentPlayer () : nP)); }
		void setPlayerSeconds (int s, int nP = -1)
							{ ((Game::Conf*) configuration ()) -> 
								setPlayerSeconds ((nP == -1) ? currentPlayer () : nP, s); }
		void addToPlayerSeconds (int s, int nP = -1) // Add a number of seconds to the current value (it can be negative)
							{ setPlayerSeconds (playerSeconds (nP) + s, nP); }
		bool sceneCompleted (int nS, int nP = -1) const
							{ return (((Conf*) configuration ()) -> 
								sceneCompleted ((nP == -1) ? currentPlayer () : nP, nS)); }
		void setSceneCompleted (int nS, bool c = true, int nP = -1)
							{ ((Conf*) configuration ()) -> 
								setSceneCompleted ((nP == -1) ? currentPlayer () : nP, nS, c); }

		/** Actualize the hall of fame, with the last results. */
		void actualizeHallOfFame ()
							{ ((Game::Conf*) configuration ()) -> actualizeHallOfFame (); }

		/** To get a reference to the main artist id for a specific player.
			It will depend on who is playing in every moment. 
			It can be overloaded to return another character. 
			By default it will depend on the type of artist playing. */
		virtual Battleship* battleship (int nP = -1);

		/** @see parent. */
		virtual void addScoreObjects ();
		virtual void removeScoreObjects ();

		/** To get a reference to the score object to control and show the energy of a specific usually bad gay. */
		const EntityEnergy* badGuyEnergyLevelScoreObject () const
							{ return (_energyMainBadGuy); }
		EntityEnergy* badGuyEnergyLevelScoreObject ()
							{ return (_energyMainBadGuy); }

		/** To know whether the password has or not been set. */
		bool isPasswordSet () const;

		protected:
		/** @see parent. */
		virtual Configuration* createConfiguration () // One player by default...
							{ return (new Conf (1, __BATTLESHIP_EXAMPLEWORLDID__, __BATTLESHIP_TIMELIMITEXAMPLESCENEID__)); } 

		/** @see parent. */
		virtual QGAMES::GUISystemBuilder* createGUISystemBuilder ()
							{ return (new GUISystemBuilder (game () -> parameter (__GAME_PROPERTYGUISYSTEMFILE__), 
								game () -> formBuilder ())); }
		virtual QGAMES::GameStateBuilder* createGameStateBuilder ();
		virtual QGAMES::EntityBuilder* createEntityBuilder ()
							{ return (new EntityBuilder (parameter (__GAME_PROPERTYENTITIESFILE__), 
								formBuilder (), movementBuilder ())); }
		virtual QGAMES::MovementBuilder* createMovementBuilder () 
							{ return (new MovementBuilder (parameter (__GAME_PROPERTYMOVEMENTSFILE__))); }
		virtual QGAMES::WorldBuilder* createWorldBuilder ()
							{ return (new WorldBuilder (parameter (__GAME_PROPERTYWORLDSFILE__), 
								mapBuilder ())); }
		virtual QGAMES::MapBuilder* createMapBuilder ();
		virtual QGAMES::CharacterControlStepsMonitorBuilder* createCharacterMonitorBuilder ()
							{ return (new QGAMES::CharacterControlStepsMonitorBuilder 
								(parameter (__GAME_PROPERTUCHARACTERMONITORFILE__))); }

		/** @see parent. */
		virtual std::string defaultParameter (const std::string& p) const;

		/** @see parent. */
		virtual void initialize ();
		virtual void inEveryLoop (); 

		/** @see parent. */
		virtual void additionalTasks ();

		/** @see parent. 
			To include the overlay layer over the world when playing. */
		virtual bool drawGameBeforeState () const
							{ return ((activeState () && activeState () -> nestedState () &&
								      activeState () -> nestedState () -> type () == __BATTLESHIP_CONTROLPLAYINGGAMESTATE__) ? true : false); }

		/** @see parent. */
		virtual void detectCollisions ();

		/** To initilize the entities used by the game and the worlds. 
			It has to be invoked from the InitializeDragonWindGameState o directly. 
			But it has to be invoked anyway before starting the game. 
			It can be overloaded. */
		virtual void initializeEntitiesAndWorlds ();

		protected:
		// A reference to the data of the game
		DataGame* _theDataGame;

		// The scoreObjects...
		EntityEnergy* _energy;
		EntityEnergy* _energyMainBadGuy;
		BattleshipLives* _lives;
		BattleshipThingsBeingCarried* _things;
		ShootsLeftCounter* _shootsLeft;
		TimeCounter* _timeCounter;
		ScoreCounter* _scoreCounter;
		FixText* _playerName;

		/** To control whether it makes or nor sense to detect collisions. */
		bool _detectCollisions;
	};
}

#endif
  
// End of the file
/*@}*/