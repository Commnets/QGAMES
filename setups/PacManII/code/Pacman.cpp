#include "Pacman.hpp"
#include "Shield.hpp"
#include "Defs.hpp"

const int PacManII::PacMan::_FADE [20] = 
	{ 255, 240, 220, 200, 180, 160, 140, 120, 100, 80, 80, 100, 120, 140, 160, 180, 200, 220, 240, 255 };

// ---
void PacManII::PacMan::setShield (bool s)
{
	if (_shield = s)
	{
		reStartAllCounters ();

		game () -> sound (__PACMANII_ENERGYAROUNDPACMAN__) -> play (-1); // Free channel!
	}
	else
		game () -> sound (__PACMANII_ENERGYAROUNDPACMAN__) -> stop ();
}

// ---
void PacManII::PacMan::initialize ()
{
	PACMAN::PacMan::initialize ();

	_shield = false;

	reStartAllCounters ();
}

// ---
void PacManII::PacMan::updatePositions ()
{
	PACMAN::PacMan::updatePositions ();

	if (_shield)
	{
		if (counter (_COUNTERTOFADE) -> isEnd ())
			counter (_COUNTERFADEPOSITION) -> isEnd (); // Just to count...
	}
}

// ---
void PacManII::PacMan::drawOn (QGAMES::Screen* scr, const QGAMES::Position& p)
{
	PACMAN::PacMan::drawOn (scr, p);

	if (_shield)
	{
		QGAMES::Color cB = __QGAMES_WHITECOLOR__;
		cB.setAlpha (_FADE [counter (_COUNTERFADEPOSITION) -> value ()]);
		QGAMES::Position cP = centerPosition ();
		int r = visualLength () >> 1;

		for (int i = 0; i < 3; i++)
			scr -> drawCircle (cP, QGAMES::Vector::_zNormal, __BD (r - i), __BD (r - i), cB, false);
	}
}

// ---
void PacManII::PacMan::finalize ()
{
	PACMAN::PacMan::finalize ();

	if (_shield)
		game () -> sound (__PACMANII_ENERGYAROUNDPACMAN__) -> stop ();
}

// ---
void PacManII::PacMan::whenCollisionWith (QGAMES::Entity* e)
{
	PACMAN::PacmanElement* pE = dynamic_cast <PACMAN::PacmanElement*> (e);
	if (e == nullptr)
		return;

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr);

	if (pE -> isAlive ())
	{
		if (isEnemy (pE))
		{
			if (!_shield)
				PACMAN::PacMan::whenCollisionWith (e);

			// If the shield is in place, no collision is detected!!
		}
		else
		{
			PacManII::Shield* shd = dynamic_cast <PacManII::Shield*> (pE);
			if (shd != nullptr)
			{
				if (isNearOf (shd, __BD (visualLength () >> 1)))
				{
					setScore (score () + shd -> points ());

					game () -> soundBuilder () -> sound (__PACMANII_SOUNDSHIELDCAUGHT__) -> play (-1);

					QGAMES::Event (__PACMANII_SHIELDCAUGHT__, shd);
				}
			}
			else
				PACMAN::PacMan::whenCollisionWith (e);
		}
	}
}

// ---
__IMPLEMENTCOUNTERS__ (PacManII::PacMan::Counters)
{
	addCounter (new QGAMES::Counter (_COUNTERTOFADE, 1, 0, true, true));
	addCounter (new QGAMES::Counter (_COUNTERFADEPOSITION, 20, 0, true, true));
}
