#include "Shield.hpp"
#include "Defs.hpp"

// ---
void PacManII::Shield::setStateToStandLookingTo (const QGAMES::Vector&)
{
	switch (type ())
	{
		case 0:
			setCurrentState (__PACMANII_SHIELDTYPE1__);
			break;

		case 1:
			setCurrentState (__PACMANII_SHIELDTYPE2__);
			break;

		case 2:
			setCurrentState (__PACMANII_SHIELDTYPE3__);
			break;

		default:
			assert (false);
	};
}

// ---
void PacManII::Shield::setStateToMoveTo (const QGAMES::Vector&)
{
	switch (type ())
	{
		case 0:
			setCurrentState (__PACMANII_SHIELDTYPE1__);
			break;

		case 1:
			setCurrentState (__PACMANII_SHIELDTYPE2__);
			break;

		case 2:
			setCurrentState (__PACMANII_SHIELDTYPE3__);
			break;

		default:
			assert (false);
	};
}
