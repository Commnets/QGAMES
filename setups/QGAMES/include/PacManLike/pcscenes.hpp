/** \ingroup PACMAN */
/*@{*/

/**	
 *	@file	
 *	File: pcscenes.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: To define any scene of a PACMAN like game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMAN_SCENES__
#define __PACMAN_SCENES__

#include <PacManLike/pcdefs.hpp>
#include <PacManLike/pcartists.hpp>
#include <PacManLike/pcmaze.hpp>

#include <advancedArcade/adinclude.hpp>

namespace PACMAN
{
	class MonsterSceneActionBlock;

	/** 
	  * Basic Scene of the game. \n
	  *	Maps can be only PACMAN ones. \n
	  *	The Scene has a siren in the background. The rate of the siren goes up as map has less balls to eat. \n
	  *	When the a power ball is eaten the scene turns into a thraten state. The monsters (and only theem) start to runway. \n
	  * The scene controls the time in this state, pushing the monsters to blink when the time to finish is close to last. \n
	  * The scene is also responsable of launching the monsters from home to the maze, 
	  * taking into account what has been defined in the configuration class. \n
	  *	The game can be extended in terms of scenes. 
	  */
	class Scene : public QGAMES::Scene
	{
		public:
		struct MessageToShow final
		{
			MessageToShow () = delete;

			MessageToShow (const std::string& txt, int fnt, const QGAMES::Position& p, int nLoops)
				: _text (txt),
				  _font (fnt),
				  _position (p), 
				  _numberLoops (nLoops),
				  _startingLoop (0), // Fixed when inserted...
				  _objTxt (nullptr)
							{ }

			MessageToShow (const MessageToShow&) = delete;

			virtual ~MessageToShow ()
							{ delete (_objTxt); }

			MessageToShow& operator = (const MessageToShow&) = delete; 

			const std::string _text;
			const int _font;
			const QGAMES::Position _position;
			const int _numberLoops;

			// Implementation
			mutable int _startingLoop;
			mutable QGAMES::ScoreObjectText* _objTxt;
		};

		Scene () = delete;

		Scene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ());

		Scene (const Scene& scn) = delete;

		Scene& operator = (const Scene& scn) = delete;

		/** The pacman scene can not delegate thes methods in the standard ones.
			The status of the switches and counters will be then saved, and this is not what we want to happen. \n
			Everytime a screen is lunched back the only important information to keep is the status of the balls eaten,
			because the rest will start as initialized. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const override;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg) override;

		/** Know whether the scene is or not already working. 
			This is mainly for counters and switches consideration. 
			It is invoked from GameState classes. */
		void setClapperBoard (bool o)
							{ _clapperBoard = o; }
		bool clapperBoard () const
							{ return (_clapperBoard); }

		/** To know whether the scene is or not in chasing mode. */
		bool chasingMode () const
							{ return (onOffSwitch (_SWITCHMONSTERSCHASING) -> isOn ()); }

		/** To control maze blinking. \n
			The first parameter indicates time beyween blinks, and the second the number of blinks to execute. 
			-1 will meani forever. */
		void startBlinking (QGAMES::bdata bT, int nB);
		bool isBlinking () const;
		void stopBlinking ();

		/** To control and manage the ball status of the scene. */
		int maxNumberBallsToEat () const;
		int numberBallsEaten () const;
		int numberBallsEatenRound () const
							{ return (_numberBallsEaten); }
		std::string ballsEatenStatus () const;
		void setBallsEatenStatus (const std::string& st);

		/** To get a random position of the maze, but not at home. */
		QGAMES::MazeModel::PositionInMaze randomMazePosition (const std::vector <int>& fZ) const;

		/** To know the number of round. */
		int numberRound () const
							{ return (_numberRound); }

		/** To know information about the monsters. */
		int numberMonsters () const;

		/** The siren will be played when the actual rate changes or when it was force (f = true). */
		void playSiren (bool f = false);
		void stopBackgroundSounds ();

		/** To stop all elments in pacman. */
		void stopAllElements ();
		/** To hide monsters. */
		void hideAllMonsters ();
		/** To hide things. */
		void hideAllThings ();

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void drawOn (QGAMES::Screen* scr, const QGAMES::Position& p = QGAMES::Position::_noPoint) override;
		virtual void finalize () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		/** Shows a message in the scene. Can be useful for many things. 
			The message is shown for a while. After that period it disappear. */
		void showMessage (MessageToShow* m);

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters () override
							{ return (new Counters); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches)
		virtual QGAMES::OnOffSwitches* createOnOffSwitches () override
							{ return (new OnOffSwitches); }

		// Implementation
		/** To switch on/off, the chasing status of the monsters */
		void setMonstersChasing (bool c);
		/** To switch on/off, the threaten status of the monsters. */
		void setMonstersBeingThreaten (bool o);
		/** The monsters to blink. */
		void setMonstersBlinking (bool b, int bp);
		/** To launch another monster. 
			Returns true, when at least one monster was launched. */
		bool launchNextMonster ();
		/** To stop/no monsters & pacman movement for a while 
			only if they are alive. */
		void freezeMonstersAndPacmanForAWhile (bool s);
		/** To recalculate all internal variables controlling the status of the balls eaten. */
		void recalculateInternalVariablesForBallsEatenControl ();


		protected:
		bool _clapperBoard;

		/** The counters and the switches. */
		static const int _COUNTERMONSTERSCHASING = 0;
		static const int _COUNTERMONSTERSRUNNINGAWAY = 1;
		static const int _COUNTERMONSTERCHASINGCYCLES = 2;
		static const int _COUNTERMONSTERSBEINGTHREATEN = 3;
		static const int _COUNTERMONSTERSTIMETOLEAVEHOME = 4;
		static const int _COUNTERMONSTERTOLEAVE = 5;
		static const int _COUNTERARTISTSFROZEN = 6;
		static const int _SWITCHMONSTERSCHASING = 0;
		static const int _SWITCHMONSTERSBEINGTHREATEN = 1;
		static const int _SWITCHALLMONSTERSMOVING = 2;
		static const int _SWITCHMONSTERSBLINKING = 3;
		static const int _SWITCHARTISTSFROZEN = 4;

		// Implementation
		/** The percentage of the maze cleaned. */
		QGAMES::bdata _percentageCleaned;
		/* Number balls to eate, eaten and eaten in this round. */
		mutable int _totalNumberBallsToEat, _totalNumberBallsEaten, _numberBallsEaten;
		/** To indicta whether it is the first time to execute the updatePositions method. \n
			First time that method is executed, it is set who refers to whom. */
		bool _firstTimeUpdateMethod;
		/** To indicate th number of round. */
		int _numberRound;
		/** A list of the action blocks relatd with monsters.
			It is usefull later. */
		std::vector <MonsterSceneActionBlock*> _monsterActionBlocks;
		/** The list of monster numbers already started. */
		std::vector <int> _numberMonstersMoving;
		/** The list of the messages being shown. */
		std::map <int, MessageToShow*> _messagesToShown;
		/** Very internal to count loops inside the scene.
			It used in relation with the messages shown. 
			Game Loops are not used to reduce the load. */
		int _internalLoopCounter;

		/** Used to play the siren. */
		enum class SirenRate { _NORMAL = 0, _FAST = 1, _VERYFAST = 2 } _sirenRate;
	};
}

#include <PacManLike/pcstandardscenes.hpp>

#endif
  
// End of the file
/*@}*/