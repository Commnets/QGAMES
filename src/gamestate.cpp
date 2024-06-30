#include <Common/gamestate.hpp>
#include <Common/game.hpp>

// ---
QGAMES::GameState::GameState (int t, QGAMES::Game* g, QGAMES::GameState* nS)
	: QGAMES::Observer (),
	  _type (t),
	  _game (g),
	  _previousState (NULL),
	  _nestedState (nS), // Can be null...
	  _counters (NULL),
	  _onOffSwitches (NULL)
{
	assert (_game);

	if (_nestedState != NULL)
		observe (_nestedState);
}

// ---
QGAMES::GameState::~GameState ()
{
	delete (_counters);
	_counters = NULL;
	delete (_onOffSwitches);
	_onOffSwitches = NULL;

	if (_nestedState != NULL)
		unObserve (_nestedState);
	// The state doesn't own the nested state...
	// So it also has to be declared in the main game declaration!
}

// ---
void QGAMES::GameState::setNestedState (GameState* nS)
{
	if (_nestedState != NULL)
	{
		_nestedState -> onExit ();
		unObserve (_nestedState);
	}

	_nestedState = nS; 
	if (_nestedState != NULL)
	{
		observe (_nestedState);
		_nestedState -> onEnter ();
	}
}

// ---
void QGAMES::GameState::setCounters (QGAMES::Counters* c)
{
	assert (c);

	delete (_counters);
	_counters = c;
	_counters -> initialize ();
}

// ---
QGAMES::Counters* QGAMES::GameState::counters ()
{
	if (_counters == NULL)
		setCounters (createCounters ());
	return (_counters);
}

// ---
void QGAMES::GameState::setOnOffSwitches (OnOffSwitches* s)
{
	assert (s);

	delete (_onOffSwitches);
	_onOffSwitches = s;
	_onOffSwitches -> initialize ();
}

// ---
QGAMES::OnOffSwitches* QGAMES::GameState::onOffSwitches ()
{
	if (_onOffSwitches == NULL)
		setOnOffSwitches (createOnOffSwitches ());
	return (_onOffSwitches);
}

// ---
void QGAMES::GameStateLinked::updatePositions ()
{
	if (stateFinihed ())
		_game -> setState (nextState ());
}

// ---
void QGAMES::GameStateControl::onEnter ()
{
	QGAMES::GameState::onEnter ();

	QGAMES::GameStates::const_iterator i = _states.find (firstGameState ());
	assert (i != _states.end ());
	setNestedState ((*i).second);
}

// ---
void QGAMES::GameStateControl::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	if (_nextGameState)
	{
		_lastState = nestedState ();
		setNestedState (_nextGameState);
		_nextGameState = NULL; // Just used...
	}

	if (finishControl ())
		notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
}

// ---
void QGAMES::GameStateControl::onExit ()
{
	setNestedState (NULL); // No nested state any more... 
						   // This will also execute the onExit method og the nested state...
						   // It has to be before executing onExit of the GameState class to avoid finalizing twice. 

	QGAMES::GameState::onExit ();
}

// ---
void QGAMES::GameStateControl::processEvent (const Event& evnt)
{
	// Only when finish signal is issued from the current game state...
	if (evnt.code () == __QGAMES_GAMENESTEDSTATEFINISHED__)
	{
		// If the current substate being executed has to finished
		// the _nextGameState variable has to be set.
		// To change directly (here) the substate executed will break the code
		// This method is invoked from the substate to be substituted.
		// To change the nested state (to change the substate) deltes the notificators too,
		// creating an inconsistency!

		// The nexGameState method must return the name of the next substate
		// or OUT if it wants to leave the control itself
		// or NULL_STRING if everything must continue in the same way!
		std::string nS = nextGameState (nestedState (), evnt);
		if (nS == std::string ("OUT"))
			notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
		else
		if (nS != std::string (__NULL_STRING__)) // If it is null, the same state will continue...
		{
			QGAMES::GameStates::const_iterator i = _states.find (nS);
			assert (i != _states.end ()); // To be sure it has been defined...
			_nextGameState = (*i).second;
		}
	}
	else
		QGAMES::GameState::processEvent (evnt); // Process the event upstairs..
}

// ---
std::string QGAMES::GameStateControl::firstGameState ()
{
	return (_states.empty () ? std::string (__NULL_STRING__) : (*_states.begin ()).first); 
	// In the default implementation the first state to execute is the very first
	// Take care with it as the states are in the map in an alphabetic order...
}

// ---
std::string QGAMES::GameStateControl::nextGameState (GameState* st, const Event& evnt)
{
	std::string result (__NULL_STRING__);

	QGAMES::GameStateControl::FlowMap::const_iterator i = _flow.find (st);
	if (i != _flow.end ())
	{
		QGAMES::GameStateControl::FlowConditions::const_iterator j = 
			(*i).second.find (circunstanceWhen (st, evnt)); // By default it is always 0...
		if (j != (*i).second.end ())
		{
			QGAMES::GameState* nst = (*j).second._gameState;
			if (nst != NULL) // If the state to go to is null, then this state has to finishs...
			{
				for (QGAMES::GameStates::const_iterator k = _states.begin (); 
						k != _states.end () && result == std::string (__NULL_STRING__); k++)
					if ((*k).second == nst)
						result = (*k).first;
			}
			else
				result = std::string ("OUT"); //..so this word must be returned!
		}
	}

	// If no flow is found, __NULL_STRING__ should be returned, meaning nothing should changed at all...
	// If no conexion is found, then OUT is returned, meaning the state has to finish...
	return (result);
}

// ---
QGAMES::ExitGameGameState::ExitGameGameState ()
			: QGAMES::GameState (__QGAMES_EXITGAMEGAMESTATE__, QGAMES::Game::game (), NULL)
{ 
	// It has to be defined here to avoid recursive inclusion...
}

// ---
void QGAMES::ExitGameGameState::onEnter ()
{ 
	game () -> exitGame (); 
}
