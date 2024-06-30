#include <Board/boardgameplayer.hpp>
#include <Board/boardgame.hpp>
#include <assert.h>

// ---
QGAMES::SetOfOpenValues QGAMES::BoardGamePlayer::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::BoardElement::runtimeValues ();

	int lNE = result.lastNestedOpenValueId ();

	// The information of the state if any, or empty if nothing...
	result.addSetOfOpenValues (lNE + 1, _state 
		? _state -> asSetOfOpenValues () 
		: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESPLAYERSTATETYPE__)));

	return (result);
}

// ---
void QGAMES::BoardGamePlayer::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existSetOfOpenValues (lNE));
	
	QGAMES::SetOfOpenValues cCfg = cfg;

	// The state can be set later (it is not mandatory at the beginning), after the runtime values were got
	// So, now the state can exist but not necessarily its runtime values!
	if (_state && cCfg.setOfOpenValues (lNE).numberValues () != 0)
		_state -> fromSetOfOpenValues (cCfg.setOfOpenValues (lNE));
	cCfg.removeSetOfOpenValues (lNE);

	QGAMES::BoardElement::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::BoardGamePlayer::initialize ()
{
	QGAMES::BoardElement::initialize ();
	state () -> initialize (); 
	// The state is got and created it nothing existed before...
}

// ---
void QGAMES::BoardGamePlayer::setBoardGame (QGAMES::BoardGame* g)
{
	assert (g);

	_game = g;
}
