#include <Board/boardgame.hpp>
#include <assert.h>

// ---
QGAMES::BoardGame::BoardGame (int id, const QGAMES::Boards& b, 
	const QGAMES::BoardGameRules& r, const QGAMES::BoardGamePlayers& p,
	const QGAMES::BoardResources& rscs)
		: QGAMES::BoardElement (id, rscs),
		  _state (NULL),
		  _boards (b),
		  _rules (r),
		  _players (p),
		  _currentBoard (NULL),
		  _turnOver (NULL)
{
	assert (_boards.size () != 0);
	assert (_players.size () != 0);

	_currentBoard = (*_boards.begin ()).second; // By default the active board is the first one...
	_turnOver = (*_players.begin ()).second; // By default the current player is the first one...

	// The boards received as parameter as assigned to this board game...
	for (QGAMES::Boards::const_iterator i = _boards.begin ();
			i != _boards.end (); i++)
	{
		observe ((*i).second);
		(*i).second -> setBoardGame (this);
	}

	// The rules are observed...
	for (QGAMES::BoardGameRules::const_iterator i = _rules.begin ();
			i != _rules.end (); i++)
		observe ((*i));

	// The players are assigned to this game...
	for (QGAMES::BoardGamePlayers::const_iterator i = _players.begin ();
			i != _players.end (); i++)
		(*i).second -> setBoardGame (this);
}

// ---
QGAMES::BoardGame::~BoardGame ()
{ 
	for (QGAMES::BoardResources::const_iterator i = _resources.begin ();
			i != _resources.end (); i++)
			delete ((*i).second);
	_resources.clear ();

	for (QGAMES::BoardGamePlayers::const_iterator i = _players.begin ();
			i != _players.end (); i++)
		delete ((*i).second);
	_players.clear ();

	for (QGAMES::BoardGameRules::const_iterator i = _rules.begin ();
			i != _rules.end (); i++)
		delete ((*i));
	_rules.clear ();

	// The boards are nor deleted, becuase they will deleted when the builder
	// is deleted, so no lines to delete the boards are needed...

	delete (_state); 
}

// ---
QGAMES::SetOfOpenValues QGAMES::BoardGame::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::BoardElement::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	// The information of the state if any, or empty if nothing...
	result.addSetOfOpenValues (lNE + 1, _state 
		? _state -> asSetOfOpenValues () 
		: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESBOARDSTATETYPE__)));

	// The information of each board...
	QGAMES::SetOfOpenValues bVV (std::string (__QGAMES_RUNTIMEVALUESBOARDSTYPE__));
	for (QGAMES::Boards::const_iterator i = _boards.begin (); i != _boards.end (); i++)
		bVV.addSetOfOpenValues ((*i).second -> id (), (*i).second -> runtimeValues ());
	result.addSetOfOpenValues (lNE + 2, bVV); // It could be empty...

	// Which is the current board if any? (or __MININT__ with nothing)
	result.addOpenValue (lE + 1, _currentBoard ? _currentBoard -> id () : __MININT__);

	// Adds the information of the rules
	int j = 0;
	QGAMES::SetOfOpenValues rVV (std::string (__QGAMES_RUNTIMEVALUESRULESTYPE__));
	for (QGAMES::BoardGameRules::const_iterator i = _rules.begin (); i != _rules.end (); i++, j++)
		rVV.addSetOfOpenValues (j, (*i) -> asSetOfOpenValues ());
	result.addSetOfOpenValues (lNE + 3, rVV);

	// ...and the one comming from the different players...
	QGAMES::SetOfOpenValues pVV (std::string (__QGAMES_RUNTIMEVALUESPLAYERSTYPE__));
	for (QGAMES::BoardGamePlayers::const_iterator i = _players.begin (); i != _players.end (); i++)
		pVV.addSetOfOpenValues ((*i).second -> id (), (*i).second -> runtimeValues ());
	result.addSetOfOpenValues (lNE + 4, pVV);

	// ...and finally (if any) the id of the current player (or __MININT__ if none)
	result.addOpenValue (lE + 2, QGAMES::OpenValue (_turnOver ? _turnOver -> id () : __MININT__));

	return (result);
}

// ---
void QGAMES::BoardGame::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1) &&
			cfg.existSetOfOpenValues (lNE) && cfg.existSetOfOpenValues (lNE - 1) && 
			cfg.existSetOfOpenValues (lNE - 2) && cfg.existSetOfOpenValues (lNE - 3));
	
	QGAMES::SetOfOpenValues cCfg = cfg;

	int tId = cCfg.openValue (lE).intValue ();
	if (tId != __MININT__)
		changeTurnTo (tId);
	cCfg.removeOpenValue (lE);

	QGAMES::SetOfOpenValues pVV = cCfg.setOfOpenValues (lNE);
	assert (pVV.name () == std::string (__QGAMES_RUNTIMEVALUESPLAYERSTYPE__));
	for (QGAMES::BoardGamePlayers::const_iterator i = _players.begin (); i != _players.end (); i++)
		// Only if the player exists...
		if (pVV.existSetOfOpenValues ((*i).second -> id ()))
			(*i).second -> initializeRuntimeValuesFrom (pVV.setOfOpenValues ((*i).second -> id ()));
	cCfg.removeSetOfOpenValues (lNE);

	int j = 0;
	QGAMES::SetOfOpenValues rVV = cCfg.setOfOpenValues (lNE - 1);
	assert (rVV.name () == std::string (__QGAMES_RUNTIMEVALUESRULESTYPE__));
	for (QGAMES::BoardGameRules::const_iterator i = _rules.begin (); i != _rules.end (); i++, j++)
		// Only if the rule exists...
		if (rVV.existSetOfOpenValues (j))
			(*i) -> fromSetOfOpenValues (rVV.setOfOpenValues (j));
	cCfg.removeSetOfOpenValues (lNE - 1);

	int bId = cCfg.openValue (lE - 1).intValue ();
	if (bId != __MININT__)
		changeBoardTo (bId);
	cCfg.removeOpenValue (lE);

	QGAMES::SetOfOpenValues bVV = cCfg.setOfOpenValues (lNE - 2);
	assert (bVV.name () == std::string (__QGAMES_RUNTIMEVALUESBOARDSTYPE__));
	for (QGAMES::Boards::const_iterator i = _boards.begin (); i != _boards.end (); i++)
		// Only if the board exists...
		if (bVV.existSetOfOpenValues ((*i).second -> id ()))
			(*i).second -> initializeRuntimeValuesFrom (bVV.setOfOpenValues ((*i).second -> id ()));
	cCfg.removeSetOfOpenValues (lNE - 2);

	// The state can be set later, after the runtime values were got
	// So, now the state can exist but not necessarily its runtime values!
	if (_state && cCfg.setOfOpenValues (lNE - 3).numberValues () != 0) // With values...
		_state -> fromSetOfOpenValues (cCfg.setOfOpenValues (lNE - 3));
	cCfg.removeSetOfOpenValues (lNE - 3);

	QGAMES::BoardElement::initializeRuntimeValuesFrom (cCfg);
}

// ---
QGAMES::BoardGamePlayer* QGAMES::BoardGame::player (int id)
{
	QGAMES::BoardGamePlayers::const_iterator i = _players.find (id);
	assert (i != _players.end ()); // Just to be sure...

	return ((*i).second);
}

// ---
void QGAMES::BoardGame::play ()
{
	// Play the turn for the current player...
	assert (_turnOver);

	_turnOver -> play ();
	verifyRules ();
}

// ---
void QGAMES::BoardGame::verifyRules ()
{
	bool next = true;
	for (QGAMES::BoardGameRules::const_iterator j = _rules.begin ();
			j != _rules.end () && next; j++)
		next = (*j) -> applyOver (this); // If the rule detects something then it will be notified...
}

// ---
void QGAMES::BoardGame::changeBoardTo (QGAMES::Board* b)
{
	assert (b);
	QGAMES::Boards::const_iterator i = _boards.find (b -> id ());
	assert (i != _boards.end ()); // It has to exist already...

	_currentBoard = b;
}

// ---
void QGAMES::BoardGame::changeBoardTo (int bId)
{
	QGAMES::Boards::const_iterator i = _boards.find (bId);
	assert (i != _boards.end ()); // It has to exist already...

	_currentBoard = (*i).second;
}

// ---
QGAMES::BoardSquare* QGAMES::BoardGame::square (int id)
{
	assert (_currentBoard);

	return (_currentBoard -> square (id));
}

// ---
QGAMES::BoardSquare* QGAMES::BoardGame::square (const QGAMES::BoardElementProperties& pts)
{
	assert (_currentBoard);

	return (_currentBoard -> square (pts));
}

// ---
QGAMES::BoardSquare* QGAMES::BoardGame::squareOn (const QGAMES::Position& pos)
{
	assert (_currentBoard);

	return (_currentBoard -> squareOn (pos));
}

// ---
void QGAMES::BoardGame::changeTurnTo (QGAMES::BoardGamePlayer* p)
{
	assert (p); // Can't be null...
	QGAMES::BoardGamePlayers::const_iterator i = _players.find (p -> id ());
	assert (i != _players.end ()); // It has to be defined already...

	_turnOver = p;
}

// ---
void QGAMES::BoardGame::changeTurnTo (int pId)
{
	QGAMES::BoardGamePlayers::const_iterator i = _players.find (pId);
	assert (i != _players.end ()); // It has to be defined already...

	_turnOver = (*i).second;
}

// ---
QGAMES::BoardGamePlayer::State* QGAMES::BoardGame::currentPlayerState ()
{
	assert (_turnOver);

	return (_turnOver -> state ());
}

// ---
QGAMES::BoardGamePlayer::State* QGAMES::BoardGame::playerState (int id)
{
	QGAMES::BoardGamePlayers::const_iterator i = _players.find (id);
	assert (i != _players.end ());

	return ((*i).second -> state ());
}

// ---
bool QGAMES::BoardGame::existResource (int id) const
{
	bool result = false;
	
	// First, it looks through the local resources...
	for (QGAMES::BoardResources::const_iterator i = _resources.begin ();
			i != _resources.end () && !result; i++)
		result = ((*i).second -> id () == id);

	// If nothing then looks through the players
	if (!result)
	{
		for (QGAMES::BoardGamePlayers::const_iterator i = _players.begin ();
				i != _players.end (); i++)
			result = (*i).second -> existResource (id);
	}

	// If nothing has been found yet
	// then it looks through the different boards the game is made up of...
	if (!result)
	{
		for (QGAMES::Boards::const_iterator i = _boards.begin ();
				i != _boards.end () && !result; i++)
			result = (*i).second -> existResource (id);
	}

	return (result);
}

// ---
QGAMES::BoardResource* QGAMES::BoardGame::resource (int id)
{
	QGAMES::BoardResource* result = NULL;
	
	// First, it looks through the local resources...
	for (QGAMES::BoardResources::const_iterator i = _resources.begin ();
			i != _resources.end () && !result; i++)
		if ((*i).second -> id () == id)
			result = (*i).second;

	// If nothing then looks through the players
	if (!result)
	{
		for (QGAMES::BoardGamePlayers::const_iterator i = _players.begin ();
				i != _players.end (); i++)
			if ((*i).second -> existResource (id)) // First of all, it verifies it exists...
				result = (*i).second -> resource (id);
	}

	// If nothing has been found yet
	// then it looks through the different boards the game is made up of...
	if (!result)
	{
		for (QGAMES::Boards::const_iterator i = _boards.begin ();
				i != _boards.end () && !result; i++)
			if ((*i).second -> existResource (id)) // First of all, verifies it can exist!
				result = (*i).second -> resource (id);
	}

	// No null result is allowed...
	assert (result);
	return (result);
}

// ---
QGAMES::BoardResource* QGAMES::BoardGame::resource (const QGAMES::BoardElementProperties& pts)
{
	QGAMES::BoardResource* result = NULL;
	
	// First, it looks through the local resources...
	for (QGAMES::BoardResources::const_iterator i = _resources.begin ();
			i != _resources.end () && !result; i++)
		if ((*i).second -> matchProperties (pts))
			result = (*i).second;

	// If nothing as been found, then it looks through the players...
	if (!result)
	{
		for (QGAMES::BoardGamePlayers::const_iterator i = _players.begin ();
				i != _players.end () && !result; i++)
			result = (*i).second -> resource (pts);
	}

	// If nothing has been found
	// then it looks through the different boards the game is made up of...
	if (!result)
	{
		for (QGAMES::Boards::const_iterator i = _boards.begin ();
				i != _boards.end () && !result; i++)
			result = (*i).second -> resource (pts);
	}

	// In this case, there can not be any resource matching the properties...
	return (result);}

// ---
void QGAMES::BoardGame::initialize ()
{
	QGAMES::BoardElement::initialize ();

	// Initialize the rules...
	for (QGAMES::BoardGameRules::const_iterator i = _rules.begin ();
			i != _rules.end (); i++)
		(*i) -> initialize (); 

	// ....the boards
	for (QGAMES::Boards::const_iterator i = _boards.begin ();
			i != _boards.end (); i++)
		(*i).second -> initialize ();

	// ...and the players
	for (QGAMES::BoardGamePlayers::const_iterator i = _players.begin ();
			i != _players.end (); i++)
		(*i).second -> initialize ();

	// Set the turn over the fist player in the list
	// Take care with the ids you propose to the different players.
	// The "less" id has to be for the player in the round and so on...
	_turnOver = (*_players.begin ()).second;
	// At construction time we have the same instruction, 
	// but this is just to reinitialize the game if needed!!

	// ...and obviously the state
	// Just invoking the state, a new one is created if there was nothing before
	state () -> initialize ();
}

// ---
void QGAMES::BoardGame::inEveryLoop ()
{
	assert (_currentBoard);

	_currentBoard -> inEveryLoop (); // just only with the current board...
	for (QGAMES::BoardGamePlayers::const_iterator i = _players.begin ();
			i != _players.end (); i++)
		(*i).second -> inEveryLoop (); // ...all players...

	QGAMES::BoardElement::inEveryLoop (); // Then it is time to do something with the resources...
										  // or whatever is done at parent level
}

// ---
void QGAMES::BoardGame::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);
	assert (_currentBoard);

	// Draws the current board...
	// Drawing the board, the squares and the resources inside will be also drawn...
	_currentBoard -> drawOn (s, p); 
	// Draws the players...
	for (QGAMES::BoardGamePlayers::const_iterator j = _players.begin ();
			j != _players.end (); j++)
		(*j).second -> drawOn (s, p); 

	// Finally draws the resources inside the game...
	QGAMES::BoardElement::drawOn (s, p);
}

// ---
void QGAMES::BoardGame::finalize ()
{
	for (QGAMES::BoardGamePlayers::const_iterator j = _players.begin ();
			j != _players.end (); j++)
		(*j).second -> finalize ();
	for (QGAMES::Boards::const_iterator i = _boards.begin ();
			i != _boards.end (); i++)
		(*i).second -> finalize ();

	// The rules have not to be finalized...

	QGAMES::BoardElement::finalize ();
}

// ---
void QGAMES::BoardGame::processEvent (const Event& evnt)
{
	if (evnt.code () == __QGAMES_BOARDNEXTPLAYER__)
	{
		QGAMES::BoardGamePlayer* p = 
			dynamic_cast <QGAMES::BoardGamePlayer*> ((QGAMES::BoardElement*) evnt.data ());
		assert (p); // The parameter has to be a player...

		bool found = false;
		QGAMES::BoardGamePlayers::const_iterator i = _players.begin ();
		for (;i != _players.end () && !found; i++)
				if ((*i).second -> id () == p -> id ())
					found = true;
		changeTurnTo ((i == _players.end ()) 
			? (*_players.begin ()).first : (*i).first); // Move the turn to the next in the list...
														// If he is the last, then the new will be the first...
	}

	notify (evnt); // Any case, the event is notified up (probably to the game application)
}
