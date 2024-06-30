/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: gamestate.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class to represent the different states a game can have. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_GAMESTATE__
#define __QGAMES_GAMESTATE__

#include <map>
#include <Common/counter.hpp>
#include <Common/onoffswitch.hpp>
#include <Common/notifierobserver.hpp>

namespace QGAMES
{
	class Game;
	class Screen;
	class GameState;
	typedef std::map <std::string, GameState*> GameStates;

	/** 
	 *	GameState class. \n
	 *	This is probably one of the most important classes in the system. \n
	 *	It is used to control how the game flows. \n
	 *	\n
	 *	The game passes through states, and the states are like "little games". \n
	 *	An state has always an owner (which is the game). \n
	 *	The state can also have a nested state. This could be very useful e.g
	 *	to stop and show the game stopped or to reuse different states. \n
	 *	\n
	 *	Every GameState has a life-cycle consisting on: OnEnter when the state is activated,
	 *	updatePositions in every loop, drawOn and onExit when other GameState is selected. \n
	 *	\n
	 *	The GameState class also declares (like Element), switches and counters to simplify
	 *	doing things in every loop. \n
	 */
	class GameState : public Observer, public Notifier
	{
		public:
		GameState (int t, Game* g, GameState* nS = NULL);
		virtual ~GameState (); // By default the counters and switches are deleted

		int type () const
							{ return (_type); }
		/** To know the previous state (before setting up this one)...if any. So it can be null. */
		GameState* previousState () 
							{ return (_previousState); }
		/** To set the previous state.
			This is only information for the game maker. */
		void setPreviousState (GameState* st)
							{ _previousState = st; }
		/** To gets an internal state (nested). It is not mandatory. */
		GameState* nestedState ()
							{ return (_nestedState); }
		const GameState* nestedState () const
							{ return (_nestedState); }
		/** To set the nested internal state. \n
			When a nested state is setm then the lifecycle is also executed on it. */
		void setNestedState (GameState* nS);
		/** To get the reference to the game the state belongs to. */
		Game* game ()
							{ return (_game); }
		const Game* game () const
							{ return (_game); }

		// The life-cycle
		/** When the game state is set up, then this method is invoked. \n
			Default behaviour is just to pass the call to the nested state if any. */
		virtual void onEnter ()
							{ if (_nestedState) _nestedState -> onEnter (); }
		/** Every loop this method is invoked over the game state currently on. \n
			Default behaviour is just to pass the call to the nested state if any. */
		virtual void updatePositions ()
							{ if (_nestedState) _nestedState -> updatePositions (); }
		/** Same to draw. \n
			Default behaviour is just to pass the call to the nested state if any. */
		virtual void drawOn (Screen* s)
							{ if (_nestedState) _nestedState -> drawOn (s); }
		/** When the game state is set down, this method is invoked. \n
			Default behaviour is just to pass the call to the nested state if any. */
		virtual void onExit ()
							{ if (_nestedState) _nestedState -> onExit (); }
		
		/** @see parent. 
			The default implementation, just pass the event up. */
		virtual void processEvent (const Event& evnt)
							{ notify (evnt); }

		protected:
		// Implementation to access just flow control elements...
		// Counters...
		/** To set the counters. 
			If anything existed before it woud be deleted. */
		void setCounters (Counters* c);
		/** To get a reference to the counters. */
		Counters* counters ();
		/** To create the counters first time.
			By default, and to be compatible with all code already created. it returns NULL. */
		virtual Counters* createCounters ()
							{ return (NULL); }
		/** To access a counter.
			Due to counters are to control internal states only, it is a protected method. */
		Counter* counter (int nC)
							{ return (counters () -> counter (nC)); }
		/** To restart all counters. */
		void reStartAllCounters ()
							{ counters () -> reStartAll (); }

		// Switches
		/** To set the on off switches. */
		void setOnOffSwitches (OnOffSwitches* s);
		/** To get a reference to the switches. */
		OnOffSwitches* onOffSwitches ();
		/** To create the on off switches first time.
			By default, and to be compatible with all code already created. it returns NULL. */
		virtual OnOffSwitches* createOnOffSwitches ()
							{ return (NULL); }
		/** To access an on-off switch.
			Due to switches are to control internal states only, it is a protected method. */
		OnOffSwitch* onOffSwitch (int nC)
							{ return (onOffSwitches () -> onOffSwitch (nC)); }
		/** To restart all on off switches. */
		void reStartAllOnOffSwitches ()
							{ onOffSwitches () -> reStartAll (); }

		protected:
		int _type;
		Game* _game;
		GameState* _previousState;
		GameState* _nestedState;
		// Very internal...
		Counters* _counters;
		OnOffSwitches* _onOffSwitches;
	};
	
	/** Class GameStateLinked. \n
		This gameState is an state linked to others.
		The state to go to depends on the situation. */
	class GameStateLinked : public GameState
	{
		public:
		GameStateLinked (int t, Game* g, GameState* nS = NULL)
			: GameState (t, g, nS)
							{ }

		/** @see parent. */
		virtual void updatePositions ();

		protected:
		virtual bool stateFinihed () = 0;
		virtual const std::string& nextState () = 0;
	};

	/** Class GameStateControl. \n
		This game state is able to control how to flow among others. */
	class GameStateControl : public GameState
	{
		public:
		class FlowCondition
		{
			public:
			FlowCondition (int eId, GameState* gS)
				: _eventId (eId), _gameState (gS)
						{ }

			int _eventId;
			GameState* _gameState;
		};

		// when condition...to state...(key = condition id)
		typedef std::map <int, FlowCondition> FlowConditions; 
		// on state...conditions...(key = GameState ref)
		typedef std::map <GameState*, FlowConditions> FlowMap; 

		GameStateControl (int t, Game* g, const GameStates& sts, const FlowMap& fM)
			: GameState (t, g, NULL),
			  _states (sts),
			  _flow (fM), // Can be empty...
			  _nextGameState (NULL),
			  _lastState (NULL)
							{ }

		/** To set the states to control. It doesn't own them. */
		const GameStates& states () const
							{ return (_states); }
		void setStates (const GameStates& sts)
							{ _states = sts; }

		/** To get the current and the last game states executed. */
		GameState* currentState () 
							{ return (nestedState ()); }
		const GameState* currentState () const
							{ return (nestedState ()); }
		GameState* lastState ()
							{ return (_lastState); }
		const GameState* lastState () const
							{ return (_lastState); }

		/** @see parent. */
		virtual void onEnter ();
		virtual void updatePositions ();
		virtual void onExit ();

		/** @see parent. 
			By default only __QGAMES_GAMENESTEDSTATEFINISHED__ is taken into account. */
		virtual void processEvent (const Event& evnt);

		protected:
		// The next methods are not const type because something can be modified inside...Who knows!!
		/** To determinate which is the first game state of the list to be run. \n
			By default it will be the first element in the list of available states, 
			This behaviour can be modified later on. */
		virtual std::string firstGameState ();
		/** To determinate which is the next game state after the event. \n
			By default, next state is defined by the flow. \n
			What is returned is the name of the state. \n
			If the name returned were __NULL_STRING__ nothing should be done meaning the state continues as it is. */
		virtual std::string nextGameState (GameState* st, const Event& evnt);
		/** A number to identifiy wich is the current situation of the game after an event is received.
			The number 0 means basic situation. That is: 
			Yes, the control is running the game state what is being run now. \n
			This method is used by the default implementation of the previous just only
			whne the finish nested state event is received. */
		virtual int circunstanceWhen (GameState* st, const Event& evnt)
							{ return (0); }
		/** To know whether is time to finish the control state or not. 
			In the default Behaviour of the class, when the method returns true (false by default),
			then a __QGAMES_GAMENESTEDSTATEFINISHED__ event is issued. */
		virtual bool finishControl () 
							{ return (false); }

		protected:
		/** The list of states. */
		GameStates _states;
		/** The flow. */
		FlowMap _flow;

		// Implementation
		GameState* _nextGameState;
		GameState* _lastState;
	};

	/** A game state just to finish the game. \n
		When the game state starts the game then finishes. */
	class ExitGameGameState : public GameState
	{
		public:
		ExitGameGameState ();

		/** @see parent. */
		virtual void onEnter ();
	};
}

#endif
  
// End of the file
/*@}*/
