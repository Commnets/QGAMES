#include <Common/onoffswitch.hpp>
#include <assert.h>

QGAMES::OnOffSwitch::OnOffSwitch (int id, bool iV)
	: _id (id),
	  _initialValue (iV),
	  _value (iV)
{
	// Nothing else to do
}

// ---
QGAMES::SetOfOpenValues QGAMES::OnOffSwitch::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESSWITCHTYPE_));

	result.addOpenValue (0, _initialValue);
	result.addOpenValue (1, _value);

	return (result);
}

// ---
void QGAMES::OnOffSwitch::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESSWITCHTYPE_));
	assert (oV.existOpenValue (0) && oV.existOpenValue (1));

	_initialValue = oV.openValue (0).boolValue ();
	_value = oV.openValue (1).boolValue ();
}

// ---
QGAMES::SetOfOpenValues QGAMES::OnOffSwitches::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESSWITCHESTYPE__));

	for (int i = 0; i < (int) _onOffSwitches.size (); i++)
		result.addSetOfOpenValues (i, _onOffSwitches [i] -> asSetOfOpenValues ());

	return (result);
}

// ---
void QGAMES::OnOffSwitches::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESSWITCHESTYPE__));

	for (int i = 0; i < (int) _onOffSwitches.size (); i++)
		if (oV.existSetOfOpenValues (i))
			_onOffSwitches [i] -> fromSetOfOpenValues (oV.setOfOpenValues (i));
}

// ---
void QGAMES::OnOffSwitches::initialize ()
{
	if (!_onOffSwitches.empty ())
		delOnOffSwitches ();

	createOnOffSwitches ();
	reStartAll ();
}

// ---
void QGAMES::OnOffSwitches::reStartAll ()
{
	for (int i = 0; i < (int) _onOffSwitches.size (); i++)
		_onOffSwitches [i] -> initialize ();
}

// ---
void QGAMES::OnOffSwitches::addOnOffSwitch (QGAMES::OnOffSwitch* s)
{
	assert (s);

	_onOffSwitches.push_back (s);
}

// ---
void QGAMES::OnOffSwitches::delOnOffSwitches ()
{
	for (int i = 0; i < (int) _onOffSwitches.size (); i++)
		delete (_onOffSwitches [i]);
	_onOffSwitches.clear ();
}
