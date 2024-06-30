#include "Stage3Scenes.hpp"

// ---
void BattleshipII::Stage3Scene::initialize ()
{
	setBackgroundMap (__BATTLESHIP_TRANSPARENTMAPID__); 

	setMap (__BATTLESHIP_COMPLEXSTARTSMOVINGBACKGROUNDMAP3ID__);

	BATTLESHIP::ChallengeScene::initialize ();

	_enemiesDestroyed = 100;
}
