#include <Common/loopcounter.hpp>
#include <Common/definitions.hpp>

// ---
void QGAMES::LoopCounter::count ()
{
	if (_stop)
		return; // Nothing to do when stopped...

	if (++_counter >= _blocks)
	{
		if (_cyclic)
			_counter = 0;

		if (_action) 
			_action -> doAction ();

		notify (QGAMES::Event (__QGAMES_LOOPCOUNTEREVENT__, this));
	}
}
