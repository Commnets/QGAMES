#include <PacManLike/pcfruit.hpp>
#include <PacManLike/pcgame.hpp>
#include <PacManLike/pcworlds.hpp>
#include <PacManLike/pcscenes.hpp>
#include <PacManLike/pcmaps.hpp>

// ---
void PACMAN::Fruit::setStateToStandLookingTo (const QGAMES::Vector&)
{
	switch (_type)
	{
		case 0:
			setCurrentState (__PACMAN_FRUIT01STATESTAYING__);
			break;

		case 1:
			setCurrentState (__PACMAN_FRUIT02STATESTAYING__);
			break;

		case 2:
			setCurrentState (__PACMAN_FRUIT03STATESTAYING__);
			break;

		case 3:
			setCurrentState (__PACMAN_FRUIT04STATESTAYING__);
			break;

		case 4:
			setCurrentState (__PACMAN_FRUIT05STATESTAYING__);
			break;

		case 5:
			setCurrentState (__PACMAN_FRUIT06STATESTAYING__);
			break;

		case 6:
			setCurrentState (__PACMAN_FRUIT07STATESTAYING__);
			break;

		case 7:
			setCurrentState (__PACMAN_FRUIT08STATESTAYING__);
			break;

		case 8:
			setCurrentState (__PACMAN_FRUIT09STATESTAYING__);
			break;

		case 9:
			setCurrentState (__PACMAN_FRUIT10STATESTAYING__);
			break;

		case 10:
			setCurrentState (__PACMAN_FRUIT11STATESTAYING__);
			break;

		case 11:
			setCurrentState (__PACMAN_FRUIT12STATESTAYING__);
			break;

		default:
			assert (false); // It should be here!!
	};
}

// ---
void PACMAN::Fruit::setStateToMoveTo (const QGAMES::Vector&)
{
	switch (_type)
	{
		case 0:
			setCurrentState (__PACMAN_FRUIT01STATEMOVING__);
			break;

		case 1:
			setCurrentState (__PACMAN_FRUIT02STATEMOVING__);
			break;

		case 2:
			setCurrentState (__PACMAN_FRUIT03STATEMOVING__);
			break;

		case 3:
			setCurrentState (__PACMAN_FRUIT04STATEMOVING__);
			break;

		case 4:
			setCurrentState (__PACMAN_FRUIT05STATEMOVING__);
			break;

		case 5:
			setCurrentState (__PACMAN_FRUIT06STATEMOVING__);
			break;

		case 6:
			setCurrentState (__PACMAN_FRUIT07STATEMOVING__);
			break;

		case 7:
			setCurrentState (__PACMAN_FRUIT08STATEMOVING__);
			break;

		case 8:
			setCurrentState (__PACMAN_FRUIT09STATEMOVING__);
			break;

		case 9:
			setCurrentState (__PACMAN_FRUIT10STATEMOVING__);
			break;

		case 10:
			setCurrentState (__PACMAN_FRUIT11STATEMOVING__);
			break;

		case 11:
			setCurrentState (__PACMAN_FRUIT12STATEMOVING__);
			break;

		default:
			assert (false); // It should be here!!
	};
}
