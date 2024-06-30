#include <Common/charactersteps.hpp>
#include <Common/entity.hpp>
#include <Common/game.hpp>
#include <sstream>

// ---
QGAMES::ComplexCharacterControlStep::~ComplexCharacterControlStep ()
{
	for (int i = 0; i < (int) _steps.size (); i++)
		delete (_steps [i]);

	_steps = QGAMES::CharacterControlSteps (); 
}

// ---
QGAMES::SetOfOpenValues QGAMES::ComplexCharacterControlStep::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::CharacterControlStep::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_currentStep));

	QGAMES::SetOfOpenValues stV (std::string (__QGAMES_RUNTIMEVALUESCHRSTEPSTYPE__));
	for (int i = 0; i < (int) _steps.size (); i++)
		stV.addSetOfOpenValues (i, _steps [i] -> runtimeValues ());
	result.addSetOfOpenValues (lNE + 1, stV);

	return (result);
}

// ---
void QGAMES::ComplexCharacterControlStep::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existSetOfOpenValues (lNE)); 

	QGAMES::SetOfOpenValues cCfg = cfg;

	_currentStep = cCfg.openValue (lE).intValue ();
	cCfg.removeOpenValue (lE);

	QGAMES::SetOfOpenValues stV = cCfg.setOfOpenValues (lNE);
	assert (stV.name () == std::string (__QGAMES_RUNTIMEVALUESCHRSTEPSTYPE__));
	for (int i = 0; i < (int) _steps.size (); i++)
		if (stV.existSetOfOpenValues (i))
			_steps [i] -> initializeRuntimeValuesFrom (stV.setOfOpenValues (i));
	cCfg.removeSetOfOpenValues (lNE);

	QGAMES::CharacterControlStep::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::ComplexCharacterControlStep::initializeOn (QGAMES::Character* a)
{
	if (_steps.empty ())
		return;

	_steps [_currentStep = firstStep ()] -> initializeOn (a);
}

// ---
void QGAMES::ComplexCharacterControlStep::updateOn (QGAMES::Character* a)
{
	if (_currentStep == -1)
		return;

	_steps [_currentStep] -> updateOn (a);

	if (_steps [_currentStep] -> isEndOn (a))
	{
		_currentStep = nextStep ();
		if (_currentStep < (int) _steps.size ())
			_steps [_currentStep] -> initializeOn (a);
		else
			_currentStep = afterLastStep ();
	}
}

// ---
void QGAMES::ComplexCharacterControlStep::executeOn (QGAMES::Character* a)
{
	if (_currentStep == -1)
		return;

	_steps [_currentStep] -> executeOn (a);
}

// ---
QGAMES::CharacterControlSteps QGAMES::ComplexCharacterControlStep::cloneSteps () const
{
	QGAMES::CharacterControlSteps result;
	for (int i = 0; i < (int) _steps.size (); result.push_back (_steps [i++] -> clone ()));
	return (result);
}

// ---
QGAMES::SetOfOpenValues QGAMES::CharacterControlTimeStep::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::CharacterControlStep::runtimeValues ();

	int lE = result.lastOpenValueId ();

	// The current loop...
	result.addOpenValue (lE + 1, QGAMES::OpenValue (_currentLoop));

	return (result);
}

// --
void QGAMES::CharacterControlTimeStep::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = cfg;
	
	_currentLoop = cCfg.openValue (lE).intValue ();
	cCfg.removeOpenValue (lE); // Just treated it...

	QGAMES::CharacterControlStep::initializeRuntimeValuesFrom (cCfg);
}

// ---
bool QGAMES::CharacterControlTimeStep::isEndOn (QGAMES::Character* a)
{
	return (_currentLoop >= (int) ((a -> game () -> framesPerSecond () * __BD _seconds)));
}

// ---
bool QGAMES::CharacterControlWaitForStateStep::isEndOn (QGAMES::Character* a)
{
	return (a -> currentState () && a -> currentState () -> id () == _state);
}

// ---
void QGAMES::CharacterControlChangeStateStep::initializeOn (QGAMES::Character* a)
{
	a -> setCurrentStateDeferred (_state); // Take into account that the change is deferred!
}

// ---
bool QGAMES::CharacterControlChangeStateStep::isEndOn (QGAMES::Character* a)
{
	return (a -> currentState () && a -> currentState () -> id () == _state);
}

// ---
void QGAMES::CharacterControlJustMoveStep::initializeOn (QGAMES::Character* a)
{
	a -> setMove (_direction, _acceleration);
}

// ---
void QGAMES::CharacterControlMoveStep::initializeOn (QGAMES::Character* a)
{
	a -> setMove (_direction, _acceleration);
}

// ---
bool QGAMES::CharacterControlMoveStep::isEndOn (QGAMES::Character* a)
{
	return (!a -> canMove (_direction, _acceleration)); 
}

// ---
QGAMES::SetOfOpenValues QGAMES::CharacterControlDistanceStep::runtimeValues () const
{
	QGAMES::SetOfOpenValues oV = QGAMES::CharacterControlStep::runtimeValues ();

	int lE = oV.lastOpenValueId ();

	// The original position of the movement..
	std::stringstream sS; sS << _originalPosition;
	oV.addOpenValue (lE + 1, QGAMES::OpenValue (sS.str ()));

	return (oV);
}

// ---
void QGAMES::CharacterControlDistanceStep::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_originalPosition = QGAMES::Position (cCfg.openValue (lE).strValue ());
	cCfg.removeOpenValue (lE); // Just treated it...

	QGAMES::CharacterControlStep::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::CharacterControlDistanceStep::initializeOn (QGAMES::Character* a)
{
	_originalPosition = a -> position ();
}

// ---
bool QGAMES::CharacterControlDistanceStep::isEndOn (QGAMES::Character* a)
{
	return ((a -> position () - _originalPosition).module () >= _distance);
}

// ---
QGAMES::CharacterControlMoveNearStep::CharacterControlMoveNearStep (const QGAMES::Position& fPos, QGAMES::bdata dT)
	: CharacterControlStep (__QGAMES_CHRSTEPNEARDISTANCE__),
	  _originalPosition (QGAMES::Position::_cero),
	  _finalPosition (fPos),
	  _distance (dT)
{
	assert (_finalPosition.posX () != __MINBDATA__ ||
			_finalPosition.posY () != __MINBDATA__ ||
			_finalPosition.posZ () != __MINBDATA__); // Not all of them at the same time can be MINBDATA!
													 // MINBDATA meaning coordinate not to be taken into account
}

// ---
QGAMES::SetOfOpenValues QGAMES::CharacterControlMoveNearStep::runtimeValues () const
{
	QGAMES::SetOfOpenValues oV = QGAMES::CharacterControlStep::runtimeValues ();

	int lE = oV.lastOpenValueId ();

	// The original position of the movement..
	std::stringstream sS; sS << _originalPosition;
	oV.addOpenValue (lE + 1, QGAMES::OpenValue (sS.str ()));

	return (oV);
}

// ---
void QGAMES::CharacterControlMoveNearStep::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_originalPosition = QGAMES::Position (cCfg.openValue (lE).strValue ());
	cCfg.removeOpenValue (lE); // Just treated it...

	QGAMES::CharacterControlStep::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::CharacterControlMoveNearStep::initializeOn (QGAMES::Character* a)
{
	_originalPosition = a -> position ();
}

// ---
bool QGAMES::CharacterControlMoveNearStep::isEndOn (QGAMES::Character* a)
{
	QGAMES::Position fPos (
		(_finalPosition.posX () == __MINBDATA__) ? _originalPosition.posX () : _finalPosition.posX (),
		(_finalPosition.posY () == __MINBDATA__) ? _originalPosition.posY () : _finalPosition.posY (),
		(_finalPosition.posZ () == __MINBDATA__) ? _originalPosition.posZ () : _finalPosition.posZ ());

	return ((a -> position () - fPos).module () <= _distance);
}

// ---
void QGAMES::CharacterControlMoveInLimitsStep::initializeOn (QGAMES::Character* a)
{
	_originalPosition = a -> position ();
}

// ---
bool QGAMES::CharacterControlMoveInLimitsStep::isEndOn (QGAMES::Character* a)
{ 
	return (!_limits.hasIn (a -> position ()) || 
			(_originalPosition - a -> position ()).module () >= _distance);
}

// ---
QGAMES::CharacterControlStepsMonitor::CharacterControlStepsMonitor 
	(int id, const QGAMES::CharacterControlSteps& s, QGAMES::Character* a, bool c)
	: _id (id),
	  _steps (s),
	  _artist (a),
	  _ciclic (c),
	  _currentStep (-1)
{
	assert (_artist); // It has to be well defined...
}

// ---
QGAMES::CharacterControlStepsMonitor::~CharacterControlStepsMonitor ()
{
	for (QGAMES::CharacterControlSteps::const_iterator i = _steps.begin ();
			i != _steps.end (); i++)
		delete (*i);
	_steps.clear (); // Empty at all...
}

// ---
QGAMES::CharacterControlSteps QGAMES::CharacterControlStepsMonitor::stepsLeft () const
{
	QGAMES::CharacterControlSteps result;

	if (_currentStep == -2) 
		return (result); // If there is no more steps to execute...

	for (int i = (_currentStep == -1) ? 0 : _currentStep; i < (int) _steps.size (); i++)
		result.push_back (_steps [i]);
	return (result);
}

// ---
void QGAMES::CharacterControlStepsMonitor::addStepsFrom (QGAMES::CharacterControlStepsMonitor* m)
{
	assert (m);

	for (int i = 0; i < (int) m -> _steps.size (); i++)
		_steps.push_back (m -> _steps [i] -> clone ());
}

// ---
void QGAMES::CharacterControlStepsMonitor::addStepsLeftFrom (QGAMES::CharacterControlStepsMonitor* m)
{
	assert (m);

	if (m -> _currentStep == -2)
		return; // Nothing else to add...
	
	for (int i = (m -> _currentStep == -1) ? 0 : m -> _currentStep; i < (int) m -> _steps.size (); i++)
		_steps.push_back (m -> _steps [i] -> clone ());
}

// ---
QGAMES::SetOfOpenValues QGAMES::CharacterControlStepsMonitor::runtimeValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESCHRMONITORTYPE__));

	// The current step...
	result.addOpenValue (0, QGAMES::OpenValue (_currentStep));

	// And the information comming from that step (it any. Empty if not valid)
	result.addSetOfOpenValues (0, (_currentStep >= 0) 
		? _steps [_currentStep] -> runtimeValues () 
			: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESCHRSTEPTYPE__)));

	return (result);
}

// ---
void QGAMES::CharacterControlStepsMonitor::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	assert (cfg.name () == std::string (__QGAMES_RUNTIMEVALUESCHRMONITORTYPE__));
	assert (cfg.existOpenValue (0) && 
			cfg.existSetOfOpenValues (0));

	_currentStep = cfg.openValue (0).intValue ();

	QGAMES::SetOfOpenValues sVV = cfg.setOfOpenValues (0);
	assert (sVV.name () == std::string (__QGAMES_RUNTIMEVALUESCHRSTEPTYPE__));
	if (_currentStep >= 0 && sVV.numberValues () != 0)
			_steps [_currentStep] -> initializeRuntimeValuesFrom (sVV);
}

// ---
void QGAMES::CharacterControlStepsMonitor::initialize ()
{ 
	_currentStep = -1;
	// The steps aren't initialized at this point...
	// The will as they are used!!
}

// ---
void QGAMES::CharacterControlStepsMonitor::run ()
{
	if (_steps.size () == 0)
		return;

	if (_currentStep == -1) // Means it is first time to be executed...
	{
		_currentStep = 0;
		_steps [0] -> initializeOn (_artist);
		_steps [0] -> executeOn (_artist);
		notify (QGAMES::Event (__QGAMES_CHRSTEPSMONITORFIRSTSTEP__, this));
	}
	else if (_currentStep >= 0)
	{
		int nS = _currentStep + 1;
		if (nS >= (int) _steps.size () && _ciclic) nS = 0;
		bool lS = nS < (int) _steps.size (); 
		if (_steps [_currentStep] -> isEndOn (_artist))
		{
			if (lS)
			{
				_currentStep = nS;
				_steps [_currentStep] -> initializeOn (_artist);
				_steps [_currentStep] -> executeOn (_artist);
				if (_currentStep == 0)
					notify (QGAMES::Event ((_currentStep == 0) 
						? __QGAMES_CHRSTEPSMONITORSTARTSBACK__ : __QGAMES_CHRSTEPSMONITOROTHERSTEP__, this));
			}
			else
			{
				_currentStep = -2; // Means nothing else to do...
				notify (QGAMES::Event (__QGAMES_CHRSTEPSMONITORLASTSTEP__, this));
			}
		}
	}

	// If the monitors is still working the step is actualized...
	if (_currentStep != -2)
		_steps [_currentStep] -> updateOn (_artist);
}
