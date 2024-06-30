#include <Common/counter.hpp>
#include <assert.h>

QGAMES::Counter::Counter (int id, int lV, int iV, bool u, bool rI)
	: _id (id),
	  _limitValue (lV),
	  _initialValue (iV),
	  _countingUp (u),
	  _autoReinitialize (rI),
	  _count (iV)
{
	assert ((_countingUp && (_limitValue >= _initialValue)) ||
			(!_countingUp && (_limitValue <= _initialValue)));
}

// ---
QGAMES::SetOfOpenValues QGAMES::Counter::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESCOUNTERTYPE__));

	result.addOpenValue (0, QGAMES::OpenValue (_limitValue));
	result.addOpenValue (1, QGAMES::OpenValue (_initialValue));
	result.addOpenValue (2, QGAMES::OpenValue (_countingUp));
	result.addOpenValue (3, QGAMES::OpenValue (_autoReinitialize));
	result.addOpenValue (4, QGAMES::OpenValue (_count));

	return (result);
}

// ---
void QGAMES::Counter::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESCOUNTERTYPE__));
	assert (oV.existOpenValue (0) && oV.existOpenValue (1) &&
			oV.existOpenValue (2) && oV.existOpenValue (3) &&
			oV.existOpenValue (4));

	_limitValue = oV.openValue (0).intValue ();
	_initialValue = oV.openValue (1).intValue ();
	_countingUp = oV.openValue (2).boolValue ();
	_autoReinitialize = oV.openValue (3).boolValue ();
	_count = oV.openValue (4).intValue ();
}

// ---
void QGAMES::Counter::initialize (int lV, int iV, bool u, bool rI)
{
	assert ((u && (lV >= iV)) || (!u && (lV <= iV)));

	_limitValue = lV;
	_initialValue = iV;
	_countingUp = u;
	_autoReinitialize = rI;
	_count = iV; // Init back when reitinalize...
}

// ---
bool QGAMES::Counter::isEnd ()
{ 
	bool result = false; 
	// counting up...
	if (_countingUp) 
	{
		if (++_count >= _limitValue) 
		{ 
			if (_autoReinitialize) _count = _initialValue;
			else _count--; // Not move up...
			result = true;
		}
	}
	// counting down...
	else 
	{
		if (--_count <= _limitValue) 
		{ 
			if (_autoReinitialize) _count = _initialValue;
			else _count++; // Not move down...
			result = true;
		}
	}

	return (result); 
}

// ---
void QGAMES::Counter::setValue (int nV)
{
	assert ((_countingUp && (nV >= _initialValue && nV <= _limitValue)) ||
			(!_countingUp && (nV <= _initialValue && nV >= _limitValue)));

	_count = nV;
}

// ---
QGAMES::SetOfOpenValues QGAMES::Counters::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESCOUNTERSTYPE__));

	for (int i = 0; i < (int) _counters.size (); i++)
		result.addSetOfOpenValues (i, _counters [i] -> asSetOfOpenValues ());

	return (result);
}

// ---
void QGAMES::Counters::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESCOUNTERSTYPE__));

	for (int i = 0; i < (int) _counters.size (); i++)
		if (oV.existSetOfOpenValues (i))
			_counters [i] -> fromSetOfOpenValues (oV.setOfOpenValues (i));
}

// ---
void QGAMES::Counters::initialize ()
{
	if (!_counters.empty ())
		delCounters ();

	createCounters ();
	reStartAll ();
}

// ---
void QGAMES::Counters::reStartAll ()
{
	for (int i = 0; i < (int) _counters.size (); i++)
		_counters [i] -> initialize ();
}

// ---
void QGAMES::Counters::addCounter (QGAMES::Counter* c)
{
	assert (c);
	assert (c -> id () == (int) _counters.size ()); // right order?

	_counters.push_back (c);
}

// ---
void QGAMES::Counters::replaceCounter (int nC, QGAMES::Counter* c)
{
	assert (c);
	assert (nC >=0 && nC < (int) _counters.size ());

	delete (_counters [nC]);
	_counters [nC] = c;
}

// ---
void QGAMES::Counters::delCounters ()
{
	for (int i = 0; i < (int) _counters.size (); i++)
		delete (_counters [i]);
	_counters.clear ();
}
