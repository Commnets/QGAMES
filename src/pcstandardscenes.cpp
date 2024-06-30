#include <PacManLike/pcstandardscenes.hpp>
#include <PacManLike/pcmaps.hpp>
#include <PacManLike/pcgame.hpp>
#include <PacManLike/pcsceneactionblocks.hpp>

// ---
void PACMAN::StandardScene::initialize ()
{
	// Loads the pacman...
	addCharacter (_pacman = dynamic_cast <PACMAN::PacMan*> (game () -> character (__PACMAN_PACMANBASEENTITYID__)));
	_pacman -> setMap (activeMap ());

	PACMAN::Scene::initialize ();

	_pacman -> setCurrentState (__PACMAN_PACMANSTATESTANDLOOKINGRIGHT__);

	_pacman -> setVisible (true);

	PACMAN::Map* aM = dynamic_cast <PACMAN::Map*> (activeMap ());
	assert (aM != nullptr);
	_pacman -> setPositionFromMazePosition (aM -> pacmanInitialPosition (0 /** There can't be more. */));

	_numberMonstersEaten = 0;
}

// ---
void PACMAN::StandardScene::updatePositions ()
{
	PACMAN::Scene::updatePositions ();

	// Regarding the status of the map...
	if (_percentageCleaned == __BD 1)
		notify (QGAMES::Event (__PACMAN_PACMANREACHEDGOAL__, _pacman));

	// To chase or not...(depends on what monsters do)
	if ((onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> isOn () && !_pacman -> isChasing ()) ||
		(!onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> isOn () && _pacman -> isChasing ()))
		_pacman -> toChase (onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> isOn ()); // Not always...

	// When monsters are not longer threaten, the list of monsters eaten is cleaned
	if (!onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> isOn ())
		_numberMonstersEaten = 0;
}

// ---
void PACMAN::StandardScene::finalize ()
{
	PACMAN::Scene::finalize ();

	_pacman -> setMap (nullptr);

	removeCharacter (_pacman);

	_pacman = nullptr;
}

// ---
void PACMAN::StandardScene::processEvent (const QGAMES::Event& evnt)
{
	// The scene can have several types of maps behind
	// That mapo alsop can be shared in other scenes
	// So the events comming from them can only be processed in the case the scene is the active one...
	if (world () == nullptr || 
		(world () != nullptr && world () -> activeScene () != this))
		return;

	if (evnt.code () == __PACMAN_BALLEATEN__)
	{
		// If a powe ball has been eaten the number of monsters aten is set back to 0
		// The rest of the process is done in the parent class!
		if (static_cast <PACMAN::TilePath*> (evnt.data ()) -> hasPower ())
			_numberMonstersEaten = 0;
	}
	else
	if (evnt.code () == __PACMAN_PACMANEATMONSTER__)
	{
		PACMAN::Monster* mtr = static_cast <PACMAN::Monster*> (evnt.data ());

		freezeMonstersAndPacmanForAWhile (true);

		// As the number of monsters eaten is bigger, the number of points grow (double)
		int nPts = mtr -> points () << (_numberMonstersEaten++);
		showMessage (new PACMAN::Scene::MessageToShow 
			(std::to_string (nPts), __PACMAN_NEWTOUR24YELLOWLETTERS__, 
				_pacman -> position (), (int) (game () -> framesPerSecond () * 1 /** One second. */)));
		_pacman -> setScore (_pacman -> score () + nPts);

		if (_numberMonstersEaten == numberMonsters ())
			_pacman -> setScore (_pacman -> score () + 12000); // Additional!!
	}

	PACMAN::Scene::processEvent (evnt);
}

// ---
PACMAN::BasicScene::BasicScene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn, 
		const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
	: StandardScene (c, m, cn, p, ePL)
{  
	// Action blocks to control the monsters...
	// 200 points when it is the first in being eaten. The rest double the points of th previous!
	addActionBlock (new PACMAN::MonsterSceneActionBlock 
		(0, PACMAN::MonsterSceneActionBlock::Properties (true, __PACMAN_BLINKYBASEENTITYID__, PACMAN::Blinky::_NUMBER, 200)), false);
	addActionBlock (new PACMAN::MonsterSceneActionBlock 
		(1, PACMAN::MonsterSceneActionBlock::Properties (true, __PACMAN_PINKYBASEENTITYID__, PACMAN::Pinky::_NUMBER, 200)), false);
	addActionBlock (new PACMAN::MonsterSceneActionBlock 
		(2, PACMAN::MonsterSceneActionBlock::Properties (true, __PACMAN_INKYBASEENTITYID__, PACMAN::Inky::_NUMBER, 200)), false);
	addActionBlock (new PACMAN::MonsterSceneActionBlock 
		(3, PACMAN::MonsterSceneActionBlock::Properties (true, __PACMAN_CLYDEBASEENTITYID__, PACMAN::Clyde::_NUMBER, 200)), false);
	addActionBlock (new PACMAN::ThingSceneActionBlock (4, PACMAN::ThingSceneActionBlock::Properties ()), false);
	// The parameters will be later adjusted to the level...
}

// ---
void PACMAN::BasicScene::initialize ()
{
	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr);

	std::vector <int> bS, bP, bE, pId;
	std::vector <QGAMES::bdata> sD, spd;
	for (auto i : g -> levelDefinition (g -> level ()).fruitConditions ())
	{
		bS.push_back (i.bonusSymbolId ());
		bP.push_back (i.bonusPoints ());
		bE.push_back (i.numberBallsEatenToAppear ());
		sD.push_back (__BD i.secondsBonusToDisappear ());
		spd.push_back (__BD i.speed ());
		pId.push_back (i.positionId ());
	}

	// The properties have to be change before initializing for them to take real effect
	dynamic_cast <PACMAN::ThingSceneActionBlock*> (actionBlock (4)) -> setProperties 
			(PACMAN::ThingSceneActionBlock::Properties (__PACMAN_FRUITBASEENTITYID__, bS, bP, bE, sD, spd, pId));

	PACMAN::StandardScene::initialize ();
}
