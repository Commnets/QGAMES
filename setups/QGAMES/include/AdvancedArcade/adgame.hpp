/**\ingroup AdArcadeGame */
/**\ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: adgame.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Forni�s (Community Networks) \n
 *	Creation Date: 01/04/2018 \n
 *	Description: Defines the body of the advanced game. \n
 *				 This implementation is based on SDL, 
 *				 but it can be changed just in the macro in the preprocessor directives. 
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ADGAME__
#define __QGAMES_ADGAME__

#include <AdvancedArcade/addefinitions.hpp>
#include <Arcade/arcadeinclude.hpp>

namespace QGAMES
{
	/** This class defines things really common in all advanced games. 
		specially regarding configuration data. */
	class AdvancedArcadeGame : public ArcadeGame
	{
		public:
		// Implementation
		/** The id of the seconds counter. */
		static const int _COUNTERSECONDS = 0;

		/** The configuration class with the basic attributes to keep in a usual game. */
		class Conf : public Game::StdConfiguration
		{
			public:
			Conf (int nP);
			/** The default copy constructor is enough. */

			/** @see parent. */
			virtual Configuration* clone () const
							{ return (new Conf (*this)); }
			virtual void cleanTemporalData ()
							{ adjustToPlayers (_numberPlayers /** The current ones. */); }

			// Read & Set general internal attributes
			int numberPlayers () const
							{ return (_numberPlayers); }
			void setNumberPlayers (int nP)
							{ assert (nP > 0); 
							  if (nP == _numberPlayers) return; // Not needed any adjustment (that can destriy everything)
							  _numberPlayers = nP; adjustToPlayers (_numberPlayers); }
			bool useJoystick () const
							{ return (_joystick); }
			void setJoystick (bool j)
							{ _joystick = j; }
			int difficulty () const
							{ return (_difficulty); }
			void setDifficulty (int d)
							{ assert (d >= 0); 
							  _difficulty = d; }
			bool musicOn () const
							{ return (_music); }
			void setMusic (bool m)
							{ _music = m; }
			const std::vector <int>& controlKeys () const
							{ return (_keys); }
			void setControlKeys (const std::vector <int>& k)
							{ _keys = k; }
			const std::multimap <std::string, SetOfOpenValues>& hallOfFame () const
							{ return (_hallOfFame); }
			void setHallOfFame (const std::multimap <std::string, SetOfOpenValues>& hF)
							{ _hallOfFame = hF; }

			// Read & Set attributes per player
			/** nP from 1 to the max number of players. */
			int currentWorld (int nP) const 
							{ assert (nP > 0 && nP <= (int) _currentWorld.size ());
							  return (_currentWorld [nP - 1]); }
			void setCurrentWorld (int nP, int w)
							{ assert (nP > 0 && nP <= (int) _currentWorld.size ());
							  _currentWorld [nP - 1] = w; }
			int currentScene (int nP) const 
							{ assert (nP > 0 && nP <= (int) _currentScene.size ());
							  return (_currentScene [nP - 1]); }
			void setCurrentScene (int nP, int s)
							{ assert (nP > 0 && nP <= (int) _currentScene.size ());
							  _currentScene [nP - 1] = s; }
			int currentInitPoint (int nP, int nS) const // 0 means also no initial point defined for that scene (the default)
							{ assert (nP > 0 && nP <= (int) _currentInitPoint.size ());
							  std::map <int, int>::const_iterator i = _currentInitPoint [nP - 1].find (nS);
							  return ((i != _currentInitPoint [nP - 1].end ()) ? (*i).second : 0); }
			void setCurrentInitPoint (int nP, int nS, int iP)
							{ assert (nP > 0 && nP <= (int) _currentInitPoint.size ());
							  _currentInitPoint [nP - 1][nS] = iP; }
			const Position& lastPlayerPosition (int nP) const 
							{ assert (nP > 0 && nP <= (int) _lastPlayerPosition.size ());
							  return (_lastPlayerPosition [nP - 1]); }
			void setLastPlayerPosition (int nP, const Position& p)
							{ assert (nP > 0 && nP <= (int) _lastPlayerPosition.size ());
							  _lastPlayerPosition [nP - 1] = p; }
			int numberLives (int nP) const 
							{ assert (nP > 0 && nP <= (int) _numberLives.size ());
							  return (_numberLives [nP - 1]); }
			void setNumberLives (int nP, int nL)
							{ assert (nP > 0 && nP <= (int) _numberLives.size ());
							  _numberLives [nP - 1] = nL;
							  if (_numberLives [nP - 1] < 0) _numberLives [nP - 1] = 0; }
			bool sceneVisited (int nP, int nS) const
							{ assert (nP > 0 && nP <= (int) _sceneVisited.size ());
							  std::map <int, bool>::const_iterator i = _sceneVisited [nP - 1].find (nS);
							  return ((i != _sceneVisited [nP - 1].end ()) ? (*i).second : false); }
			void setSceneVisited (int nP, int nS, bool v = true)
							{ assert (nP > 0 && nP <= (int) _sceneVisited.size ());
							  _sceneVisited [nP - 1][nS] = v; }

			/** To get the name of the valid configuration names.
				Always without the path to the data dir. */
			std::vector <std::string> confFileNames () const;

			/** Every loop, but only in specific circumstances,
				the things to eat could appear back, or the guns to be reloaded. 
				This method should be used for this kind of things. */
			virtual void inEveryLoop ()
							{ }

			protected:
			/** Adjust the size of internal attributes according with the number of players. */
			virtual void adjustToPlayers (int nP);

			/** These are the important methods.
				They can be both overloaded later, but their default implementation
				iterates over the persistent elements kept into this class, saving the configurationson it. */
			virtual void cfgToStream (std::ostringstream& oS);
			virtual void cfgFromStream (std::istringstream& iS);

			protected:
			// Data
			// General configuration
			/** The number of players. */
			int _numberPlayers;
			/** Use the joystick instead the keyboard. */
			bool _joystick;
			/** The level of the difficulty of the game. */
			int _difficulty;
			/** Should the game play music? */
			bool _music;
			/** What are the keys used in the game. */
			std::vector <int> _keys;
			/** Hall of fame. */
			std::multimap <std::string, SetOfOpenValues> _hallOfFame;

			// Regarding the entities playing the game
			/** In which world are currently the players? */
			std::vector <int> _currentWorld; 
			/** In which scene are currently the players? */
			std::vector <int> _currentScene;
			/** Could a point be determined as the initial point for the different players? 
				Every user can have different init point per scene defined. */
			std::vector <std::map <int, int>> _currentInitPoint;
			/** What was the last position to remember per player? */
			std::vector <Position> _lastPlayerPosition; 
			/** The number of lives per player. */
			std::vector <int> _numberLives;
			/** To set whether the scene has or not already been visited. 
				It is e.g to determinate whether it is or not necessary to count down when starting to play. */
			std::vector <std::map <int, bool>> _sceneVisited;
		};

		/** If implementation == NULL, then a default implementaion will try to be used (SDL by default). */
		AdvancedArcadeGame (GameImplementation* impl = NULL); 

		/** @see parent. */
		/** After setting the configuration, part of it is moved also into other elements.
			In this case, into the input handler. 
			It has to inherit from InputHandler and has a StandarBehaviour or inherit from it inside. */
		virtual void setConfiguration (QGAMES::Game::Configuration* cfg);

		/** To load a configuration. 
			Returns 0 if everything is ok and -1 if not. */
		int loadConfiguration (const std::string& fN);
		/** To save a configuration. 
			Returns 0 if everything is ok and -1 if not. */
		int saveConfiguration (const std::string& fN);

		/** To get the number of players. */
		int numberPlayers () const
							{ return (((const Conf*) configuration ()) -> numberPlayers ()); }
		/** To get the number of player. */
		int currentPlayer () const
							{ return (_currentPlayer); }
		void setCurrentPlayer (int cP);
		void setNextPlayer ();

		/** To manage the number of lives. */
		int lives (int nP = -1) const
							{ return (((const Conf*) configuration ()) -> 
								numberLives ((nP == -1) ? currentPlayer () : nP)); }
		void setLives (int nL, int nP = -1)
							{ ((Conf*) configuration ()) -> 
								setNumberLives ((nP == -1) ? currentPlayer () : nP, nL); }
		void addLives (int tA, int nP = -1)
							{ setLives (lives (nP) + tA, nP); }

		/** Access to other configuration attributes */
		int currentWorld (int nP = -1) const 
							{ return (((const Conf*) configuration ()) -> 
								currentWorld ((nP == -1) ? currentPlayer () : nP)); }
		void setCurrentWorld (int w, int nP = -1)
							{ ((Conf*) configuration ()) -> 
								setCurrentWorld ((nP == -1) ? currentPlayer () : nP, w); }
		int currentScene (int nP = -1) const 
							{ return (((const Conf*) configuration ()) -> 
								currentScene ((nP == -1) ? currentPlayer () : nP)); }
		void setCurrentScene (int s,int nP = -1)
							{ ((Conf*) configuration ()) -> 
								setCurrentScene ((nP == -1) ? currentPlayer () : nP, s); }
		int currentInitPoint (int nS, int nP = -1) const 
							{ return (((const Conf*) configuration ()) -> 
								currentInitPoint ((nP == -1) ? currentPlayer () : nP, nS)); }
		void setCurrentInitPoint (int nS, int iP, int nP = -1)
							{ ((Conf*) configuration ()) -> 
								setCurrentInitPoint ((nP == -1) ? currentPlayer () : nP, nS, iP); }
		const Position& lastPlayerPosition (int nP = -1) const 
							{ return (((const Conf*) configuration ()) -> 
								lastPlayerPosition ((nP == -1) ? currentPlayer () : nP)); }
		void setLastPlayerPosition (const Position& p, int nP = -1)
							{ ((Conf*) configuration ()) -> 
								setLastPlayerPosition ((nP == -1) ? currentPlayer () : nP, p); }
		bool sceneVisited (int nS, int nP = -1) const
							{ return (((Conf*) configuration ()) -> 
								sceneVisited ((nP == -1) ? currentPlayer () : nP, nS)); }
		void setSceneVisited (int nS, bool v = true, int nP = -1)
							{ ((Conf*) configuration ()) -> 
								setSceneVisited ((nP == -1) ? currentPlayer () : nP, nS, v); }

		// The rest of the configuration attributes are not directly accessible
		
		/** To manage the seconds from the beginning of the game. */
		int seconds () const
							{ return (_numberOfSeconds); }
		void setSeconds (int nS)
							{ _numberOfSeconds = nS; }

		/** @see parent */
		virtual void pauseGame ();
		virtual void continueGame ();

		/** @see parent. 
			Just when the game is stopped (using the standard control game state as main state) the 
			draw will be on top of the rest of the drawings*/
		virtual bool drawGameBeforeState () const;

		/** @see parent. */
		virtual void processEvent (const Event& e);

		protected:
		/** @see parent. */
		virtual GameStateBuilder* createGameStateBuilder ();
		virtual TextBuilder* createTextBuilder ();
		// This one just only create the entity used during pause.
		// so it will be likely replaced in the real game
		virtual EntityBuilder* createEntityBuilder (); 
		virtual GUISystemBuilder* createGUISystemBuilder ();
		virtual MovementBuilder* createMovementBuilder ();
		virtual Configuration* createConfiguration ()
								{ return (new Conf (1)); } // One player by default...
		virtual Screens createScreens ();

		/** @see parent. */
		virtual std::string defaultParameter (const std::string& p) const;

		/** @see parent. */
		virtual void initialize ();
		virtual void inEveryLoop ();

		protected:
		int _currentPlayer;
		int _numberOfSeconds;

		// Implementation
		/** A copy of the configuration.
			The configuration is managed during the game, but it is needed to maintain a 
			copy of that while the current situation is not saved. */
		Conf* _copyOfConfiguration;
	};
}

#endif
  
// End of the file
/*@}*/