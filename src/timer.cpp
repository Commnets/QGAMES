#include <Common/timer.hpp>
#include <assert.h>

QGAMES::Timer* QGAMES::Timer::_theTimer = NULL;

// ---
QGAMES::Timer::Timer ()
			: _loopsPerSecond (0),
			  _loopsPerSecondTemp (0),
			  _lastMilliSeconds (0),
			  _alreadyCalculated (false)
{
	assert (_theTimer == NULL); // Only one...

	_theTimer = this; 
}

// ---
QGAMES::Timer* QGAMES::Timer::timer ()
{
	return (_theTimer); 
}

// ---
void QGAMES::Timer::inEveryLoop ()
{
	if (_alreadyCalculated)
	{
		_loopsPerSecond = _loopsPerSecondTemp;
		start (); // again...
		return;
	}

	if (_lastMilliSeconds == 0)
		_lastMilliSeconds = actualMilliSeconds ();
	else
	{
		if (actualMilliSeconds () < (_lastMilliSeconds + 1000))
			_loopsPerSecondTemp++;
		else
			_alreadyCalculated = true;
	}
}
