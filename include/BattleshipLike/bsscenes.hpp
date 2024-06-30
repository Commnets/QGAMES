/** \ingroup Battleship */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: bsscenes.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 02/11/2019 \n
 *	Description: Defines the base for all scenes in Battleship like games.
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_SCENES__
#define __BATTLESHIP_SCENES__

#include <BattleshipLike/bsdefinitions.hpp>
#include <BattleshipLike/bsartists.hpp>
#include <BattleshipLike/bsactionblocks.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace BATTLESHIP
{
	/** Class to define common funtionality to any scene in a Battleship like game. */
	class Scene : public QGAMES::SceneWithBackground
	{
		public:
		Scene (int c, const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: QGAMES::SceneWithBackground (c, m, cn, p, ePL)
							{ }

		/** To know the name of the scene. 
			It is defined in the configuration data file. 
			If nothing has been defined, then a default one is returned. @see bsdefinitions.hpp.
			<Attributes>
				<Attribute id="NAME" value="???"/>
				...
			</Attributes>
		 */
		std::string name () const;

		/** To know which axis the scene moves towards.
			It has to be defined in the scene configuration data file.
			If nothing has been defined a default one is returned. @see bsdefinitions.hpp.
			<Attributes>
				<Attribute id="MOVEMENT" value="X,Y,Z"/>
				...
			</Attributes>
		 */
		QGAMES::Vector movement () const;
	};

	/** The basic scene used to play. \n
		In this scene the different elements of any arcade game are added. Those are: \n
		Asteroids, Other Spaceship enemies, Energy Elements, and things to be caught. \n
		Notice that the shootings are not added until they are shooted. 
		This is just to avoid thousands of entities are processed when they are not even visible. 
		The cost / benefit balance is not leveled in this case!!. 
		The class deefine a method to be launched when the scene has been completed. \n
		An additional method has been added to determine whether this happens. By default it returns false.
		The method is invoked always at the end of the updatePositions () method. */
	class PlayingScene : public Scene
	{
		public:
		typedef std::function <bool (PlayingScene*)> ExternalEventToCompleteSceneFuncion;

		static const ExternalEventToCompleteSceneFuncion _NOEXTERNALEVNT;
		static const ExternalEventToCompleteSceneFuncion _NOSTDUFOSINSCENEEXTERNALEVNT;

		PlayingScene (int c, const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer (),
				const ExternalEventToCompleteSceneFuncion& eF = _NOEXTERNALEVNT);

		/** To change the moving speed .*/
		int speed () const
							{ return (_speed); }
		void setSpeed (int s);

		/** To know te battleship. */
		const Battleship* battleship () const
							{ return (_battleship); }
		Battleship* battleship () 
							{ return (_battleship); }

		/** To get a reference to the different elements taking place in the game.
			The list can't be modified. */
		const QGAMES::Entities& asteroids () const
							{ return (_asteroids); }
		const QGAMES::Entities& thingsToCatch () const
							{ return (_thingsToCatch); }
		const QGAMES::Entities& enemies () const
							{ return (_enemies); }

		/** When the scene has been completed. 
			The default implementation just finihs the background music (if any),
			change the state of the battleship to Rotate and notify from it that the goal has been reached. */
		virtual void sceneCompleted ();

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		ExternalEventToCompleteSceneFuncion _externalEventSceneCompletedFunction;

		// The speed that the scene has...
		// It has to be a integer value between 1 and 10 maximum...
		int _speed;

		// Implementation
		/** Set when the game is initialized. */
		Battleship* _battleship;

		QGAMES::Entities _charactersAdded;
		QGAMES::Entities _asteroids;
		QGAMES::Entities _thingsToCatch;
		QGAMES::Entities _enemies;

		/** Only used when playing a background sound, 
			always in the background standard channel. */
		int _backgroundSound; 

		/** To end a shooting in a different loop than the one processing the events.
			When a shooting is no longer valid (out of screen or destroyed w.g) a notification is thrown from the object.
			This notification has to be used to remove the shooting from the scene. \n
			But it has to be done in a different loop to the one processign the event to avoid unexpected errors.
			To do that a Buoy is used. \n
			More than one shooting could be ended per loop. */
		class ToEndShootingBuoy : public QGAMES::Buoy
		{
			public:
			ToEndShootingBuoy (int id)
			: QGAMES::Buoy (id, 0), // Value is not used...
			  _shootings ()
							{ }

			int numberShootings () const
							{ return ((int) _shootings.size ()); }

			const SpaceshipShooting* shooting (int nS) const
							{ return ((nS >= 0 && nS < (int) _shootings.size ()) ? _shootings [nS] : NULL); }
			SpaceshipShooting* shooting (int nS)
							{ return ((nS >= 0 && nS < (int) _shootings.size ()) ? _shootings [nS] : NULL); }
			void addShooting (SpaceshipShooting* sht);

			/** @see parent. */
			virtual void* treatFor (QGAMES::Element* e);
			
			private:
			std::vector <SpaceshipShooting*> _shootings;
		};
	};

	/** The most common type of scene.
		A scene that finishes based on time. */
	class TimeLimitScene : public PlayingScene
	{
		public:
		/** To know the list of the action blocks to pass throught sequentially.
			It is defined in the configuration data file. 
			If nothing has been defined, then a default one is returned. @see bsdefinitions.hpp.
			<Attributes>
				<Attribute id="SEQUENTIALACTIONBLOCKS" value="x,y,..."/>
				...
			</Attributes>
		 */
		TimeLimitScene (int c, const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer (),
				const ExternalEventToCompleteSceneFuncion& eF = _NOEXTERNALEVNT);

		/** To know the scene time.
			and the current one. */
		const QGAMES::bdata sceneTime () const
							{ return (_sceneTime); }
		const QGAMES::bdata currentSceneTime () const
							{ return (__BD counter (_COUNTERSCENEMOVING) -> value () / __BD game () -> framesPerSecond ()); }

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }
		// None in needed in the default immplementation, just to add it later if needed...

		protected:
		// Implementation
		QGAMES::bdata _sceneTime;

		static const int _COUNTERSCENEMOVING = 0;
	};

	/** Another common type of scene.
		This one defines a set of sequential action blocks. 
		Every one of them has to emit a __BATTLESHIP_SPACEELEMENTACTIONBLOCKTIMEOVER__ event when it is finished.
		and the scene will move to the next. When the last is over, the scene finishes too. */
	class SequentialActionBlocksScene : public PlayingScene
	{
		public:
		/** To know the list of the action blocks to pass throught sequentially.
			It is defined in the configuration data file. 
			If nothing has been defined, then a default one is returned. @see bsdefinitions.hpp.
			<Attributes>
				<Attribute id="SEQUENTIALACTIONBLOCKS" value="x,y,..."/>
				...
			</Attributes>
		 */
		SequentialActionBlocksScene (int c, const QGAMES::Maps& m, 
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer (),
				const ExternalEventToCompleteSceneFuncion& eF = _NOEXTERNALEVNT);

		/** @see parent. 
			Neccessary to save the value of the current action block being controlled. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** To know the list of action blocks being controlled. */
		const std::vector <int>& sequentialActionBlocks () const
							{ return (_sequentialActionBlocks); }
		int currentActionBlockControlled () const
							{ return (_currentActionBlockControlled); }

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		// Implementation
		void activeSequentialBlock (int nB);

		protected:
		// Implementation
		std::vector <int> _sequentialActionBlocks;
		int _currentActionBlockControlled;
	};

	/** This is a challenge playing scene. \n
		Is like a sequential of action blocks with a time limit and that also finishes when all are destroyed or the time is off. \n
		After finishing with success, the number of enemies killed and also the points given are shown! \n
		So read it for a better understanding of the methods. */
	class ChallengeScene : public SequentialActionBlocksScene
	{
		public:
		ChallengeScene (int c, const QGAMES::Maps& m,
				int pG = 10, // Points given per enemy killed...10 by default
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer (),
				const ExternalEventToCompleteSceneFuncion& eF = _NOEXTERNALEVNT);

		virtual ~ChallengeScene ()
							{ deleteTextEnemiesDestroyedAndPointsGiven (); }

		/** To know the scene time, and the current one. */
		const QGAMES::bdata sceneTime () const
							{ return (_sceneTime); }
		const QGAMES::bdata currentSceneTime () const
							{ return (__BD counter (_COUNTERSCENEMOVING) -> value () / __BD game () -> framesPerSecond ()); }

		/** @see parent. */
		virtual void sceneCompleted ();

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& pos = QGAMES::Position::_noPoint);
		virtual void finalize ();

		/** @see parent. To count enemies. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }

		// Implementation
		inline void deleteTextEnemiesDestroyedAndPointsGiven ()
							{ delete (_textEnemiesDestroyed); delete (_textPointsGiven);
							  _textEnemiesDestroyed = _textPointsGiven = NULL; }

		protected:
		int _pointsGivenPerEnemyKilled;

		// Implementation
		QGAMES::bdata _sceneTime;
		int _enemiesDestroyed;
		QGAMES::ScoreObjectText* _textEnemiesDestroyed;
		QGAMES::ScoreObjectText* _textPointsGiven;

		static const int _COUNTERSCENEMOVING = 0;
		static const int _COUNTERAFTERMOVINGSTATES = 1;
		static const int _COUNTERELEMENTSKILLED = 2;
		static const int _COUNTERSHOWINGENEMIESKILLED = 3;
		static const int _COUNTERSHOWINGALSOPOINTSGIVEN = 4;
		static const int _SWITCHSCENESHOWINGPOINTSGOT = 0;
	};

	/** This type of scene is usually the one to fight with the bigger enemy.
		They usually appears at the end of every world. \n
		The challengue consists on destroying the UFO. \n
		Once this has happened the scene finishes. \n 
		In this type of scenes the energy left of the focused enemy is also shown!. 
		The focusd enemy has to be set in classs inheriting from this one. */
	class FocusingInOneElementScene : public PlayingScene
	{
		public:
		FocusingInOneElementScene (int c, const QGAMES::Maps& m, const std::string& tK,
				const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
				const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
				const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer (),
				const ExternalEventToCompleteSceneFuncion& eF = _NOEXTERNALEVNT)
			: PlayingScene (c, m, cn, p, ePL),
			  _focusedElement (NULL),
			  _textFocusedElementDestroyed (tK),
			  _textFocusedElementDestroyedSO (NULL)
							{ }

		virtual ~FocusingInOneElementScene ()
							{ deleteTextEnemyFocusedDestroyed (); }

		/** @see parent. 
			Just save / get also whether there is o no a focused element. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** To set and to know the element to be focused on. It can be NULL. */
		const SpaceElement* focusedElement () const
							{ return (_focusedElement); }
		SpaceElement* focusedElement ()
							{ return (_focusedElement); }
		void setFocusedElement (SpaceElement* spc)
							{ _focusedElement = spc; }

		/** @see parent. */
		virtual void sceneCompleted ();

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& pos = QGAMES::Position::_noPoint);
		virtual void finalize ();

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }

		// Implementation
		inline void deleteTextEnemyFocusedDestroyed ()
							{ delete (_textFocusedElementDestroyedSO); _textFocusedElementDestroyedSO = NULL; }

		protected:
		std::string _textFocusedElementDestroyed;
		SpaceElement* _focusedElement;

		// Implementation
		QGAMES::ScoreObjectText* _textFocusedElementDestroyedSO;

		static const int _COUNTERSHOWINGFOCUSENEMYKILLED = 0;
		static const int _SWITCHSCENESHOWINGENEMYKILLED = 0;
	};

	/** Just an example of the time limit. 
		A live appear any time the first element in the scene is killed. */
	class TimeLimitSceneExample : public TimeLimitScene
	{
		public:
		TimeLimitSceneExample (int c, const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: TimeLimitScene (c, m, cn, p, ePL), // No external event to finish the scene
			  _backgroundMapId (-1), // First time, it hasn't been defined yet...
			  _liveLaunched (false)
							{ }

		/** @see parent. */
		virtual void initialize ();
		virtual void finalize ();

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		void launchLive ();

		protected:
		/** The number of background map. */
		int _backgroundMapId;

		// Implementation
		bool _liveLaunched;
	};

	/** Just an example of the sequential action blocks. */
	class SequentialActionBlocksSceneExample : public SequentialActionBlocksScene
	{
		public:
		SequentialActionBlocksSceneExample (int c, const QGAMES::Maps& m, 
			   const QGAMES::Scene::Connections& cn = QGAMES::Scene::Connections (), 
			   const QGAMES::SceneProperties& p = QGAMES::SceneProperties (), 
			   const QGAMES::EntitiesPerLayer& ePL = QGAMES::EntitiesPerLayer ())
			: SequentialActionBlocksScene (c, m, cn, p, ePL) // No external event to finish the scene
							{ }

		/** @see parent. */
		virtual void initialize ();
	};
}

#endif
  
// End of the file
/*@}*/
