#include <PacManLike/pcscenes.hpp>
#include <PacManLike/pcsceneactionblocks.hpp>
#include <PacManLike/pcmaps.hpp>
#include <PacManLike/pcgame.hpp>

// ---
PACMAN::Scene::Scene (int c, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn, 
		const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
	: QGAMES::Scene (c, m, cn, p, ePL),
	  _clapperBoard (false),
	  _percentageCleaned (__BD 0),
	  _totalNumberBallsToEat (-1), // initialized later
	  _totalNumberBallsEaten (-1), // initialized later
	  _numberBallsEaten (-1), // initialized later
	  _firstTimeUpdateMethod (true),
	  _numberRound (0),
	  _monsterActionBlocks (),
	  _numberMonstersMoving (),
	  _messagesToShown (),
	  _internalLoopCounter (0)
{
#ifndef NDEBUG
	for (auto i : maps ())
		assert (dynamic_cast <PACMAN::Map*> (i.second) != nullptr);
#endif
}

// ---
void PACMAN::Scene::startBlinking (QGAMES::bdata bT, int nB)
{
	// Only over the actve map if any...
	PACMAN::Map* mp = nullptr;
	if ((mp = dynamic_cast <PACMAN::Map*> (activeMap ())) != nullptr)
		mp -> startBlinking (bT, nB);
}

// ---
QGAMES::SetOfOpenValues PACMAN::Scene::runtimeValues () const
{
	QGAMES::SetOfOpenValues result;

	result.addOpenValue (0, QGAMES::OpenValue (_numberRound));
	result.addOpenValue (1, QGAMES::OpenValue (_totalNumberBallsToEat));
	result.addOpenValue (2, QGAMES::OpenValue (_totalNumberBallsEaten));
	/** Remember that _numberBallsEaten has not ben saved. */
	result.addOpenValue (3, QGAMES::OpenValue (_percentageCleaned));
	result.addSetOfOpenValues (0,
		(activeMap () == nullptr) ? QGAMES::SetOfOpenValues () : activeMap () -> runtimeValues ());

	// Saves also the information of the scene blocks...
	QGAMES::SetOfOpenValues bVV (__QGAMES_RUNTIMEVALUESBLOCKSTYPE__);
	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin (); 
			i != _actionBlocks.end (); i++)
		bVV.addSetOfOpenValues ((*i) -> id (), (*i) -> runtimeValues ());
	result.addSetOfOpenValues (1, bVV); 

	return (result);
}

// ---
void PACMAN::Scene::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	assert (cfg.existOpenValue (0) && cfg.existOpenValue (1) && 
			cfg.existOpenValue (2) && cfg.existOpenValue (3) &&
			cfg.existSetOfOpenValues (0) && cfg.existSetOfOpenValues (1));

	_numberRound = cfg.openValue (0).intValue ();
	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr);
	const PACMAN::DataGame::LevelDefinition& lD = g -> levelDefinition (g -> level ());
	counter (_COUNTERMONSTERSTIMETOLEAVEHOME) -> initialize 
		((int) (__BD lD.leaveHomeCondition (_numberRound).maxSecondsToLeave () * __BD g -> framesPerSecond ()), 0, true, true);

	_totalNumberBallsToEat = cfg.openValue (1).intValue ();
	_totalNumberBallsEaten = cfg.openValue (2).intValue ();
	/** _numberBallsEaten is not save because it represents the number of balls of the round. 
		And everey round starts with 0 ball eatn in it. */
	_percentageCleaned = cfg.openValue (3).bdataValue ();

	if (activeMap ())
	{
		activeMap () -> initializeRuntimeValuesFrom (cfg.setOfOpenValues (0));

		recalculateInternalVariablesForBallsEatenControl ();
	}

	QGAMES::SetOfOpenValues bVV = cfg.setOfOpenValues (1);
	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin ();
			i != _actionBlocks.end (); i++)
		if (bVV.existSetOfOpenValues ((*i) -> id ()))
			(*i) -> initializeRuntimeValuesFrom (bVV.setOfOpenValues ((*i) -> id ()));
}

// ---
bool PACMAN::Scene::isBlinking () const
{
	// Only over the actve map if any...
	const PACMAN::Map* mp = dynamic_cast <const PACMAN::Map*> (activeMap ());
	return ((mp != nullptr) ? mp -> isBlinking () : false);
}

// ---
void PACMAN::Scene::stopBlinking ()
{
	// Only over the actve map if any...
	PACMAN::Map* mp = nullptr;
	if ((mp = dynamic_cast <PACMAN::Map*> (activeMap ())) != nullptr)
		mp -> stopBlinking ();
}

// ---
int PACMAN::Scene::maxNumberBallsToEat () const
{
	if (_totalNumberBallsToEat != -1)
		return (_totalNumberBallsToEat);

	// Only over the actve map if any...
	const PACMAN::Map* mp = dynamic_cast <const PACMAN::Map*> (activeMap ());
	return (_totalNumberBallsToEat = (mp != nullptr) ? mp -> maxNumberBallsToEat () : 0);
}

// ---
int PACMAN::Scene::numberBallsEaten () const
{
	if (_totalNumberBallsEaten != -1)
		return (_totalNumberBallsEaten);

	// Only over the actve map if any...
	const PACMAN::Map* mp = dynamic_cast <const PACMAN::Map*> (activeMap ());
	return (_totalNumberBallsEaten = (mp != nullptr) ? mp -> numberBallsEaten () : 0);
}

// ---
std::string PACMAN::Scene::ballsEatenStatus () const
{
	// Only over the actve map if any...
	const PACMAN::Map* mp = dynamic_cast <const PACMAN::Map*> (activeMap ());
	return ((mp != nullptr) ? mp -> ballsEatenStatus () : std::string (__NULL_STRING__));
}

// ---
void PACMAN::Scene::setBallsEatenStatus (const std::string& st)
{
	// Only over the actve map if any...
	PACMAN::Map* mp = nullptr;
	if ((mp = dynamic_cast <PACMAN::Map*> (activeMap ())) != nullptr)
	{
		mp -> setBallsEatenStatus (st);

		recalculateInternalVariablesForBallsEatenControl ();
	}
}

// ---
QGAMES::MazeModel::PositionInMaze PACMAN::Scene::randomMazePosition (const std::vector <int>& fZ) const
{
	// Only over the actve map if any...
	const PACMAN::Map* mp = nullptr;
	if ((mp = dynamic_cast <const PACMAN::Map*> (activeMap ())) == nullptr)
		return (QGAMES::MazeModel::_noPosition);

	return (mp -> randomMazePosition (fZ));
}

// ---
int PACMAN::Scene::numberMonsters () const
{
	int result = 0;
	for (auto i : characters ())
		if (dynamic_cast <PACMAN::Monster*> (i.second) != nullptr)
			result++;
	return (result);
}

// ---
void PACMAN::Scene::playSiren (bool f)
{ 
	PACMAN::Game::Conf* cfg = dynamic_cast <PACMAN::Game::Conf*> (game () -> configuration ());
	assert (cfg != nullptr);
	if (!cfg -> musicOn ())
		return; // When the music hasn't been allowed (backgrond siren)

	SirenRate sR = SirenRate::_NORMAL;
	if (_percentageCleaned > 0.30 && _percentageCleaned < 0.60) sR = SirenRate::_FAST;
	if (_percentageCleaned > 0.60) sR = SirenRate::_VERYFAST;
	if (sR != _sirenRate || f)
	{
		_sirenRate = sR;

		game () -> sound (__PACMAN_SOUNDSIREN__ + static_cast <int> (sR)) -> play (__QGAMES_BACKGROUNDSOUNDCHANNEL__); 
	}
}

// ---
void PACMAN::Scene::stopBackgroundSounds ()
{
	// The siren is playing in the background channel if any...
	QGAMES::SoundSystem::system () -> pauseChannel (__QGAMES_BACKGROUNDSOUNDCHANNEL__);

	// When thratn, thre is also this other sound in the background...
	if (onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> isOn ())
		game () -> soundBuilder () -> sound (__PACMAN_SOUNDTHREATING__) -> stop ();
}

// ---
void PACMAN::Scene::stopAllElements ()
{
	for (auto i : characters ())
		if (dynamic_cast <PACMAN::Artist*> (i.second))
			static_cast <PACMAN::Artist*> (i.second) -> toStand ();
}

// ---
void PACMAN::Scene::hideAllMonsters ()
{
	for (auto i : characters ())
		if (dynamic_cast <PACMAN::Monster*> (i.second))
			static_cast <PACMAN::Monster*> (i.second) -> setVisible (false);
}

// ---
void PACMAN::Scene::hideAllThings ()
{
	for (auto i : characters ())
		if (dynamic_cast <PACMAN::Thing*> (i.second))
			static_cast <PACMAN::Thing*> (i.second) -> setVisible (false);
}

// ---
void PACMAN::Scene::initialize ()
{
	// The scene has to be initialized just after characters are added...
	QGAMES::Scene::initialize ();

	// Very internal variables to speed up the execution...
	recalculateInternalVariablesForBallsEatenControl ();

	// At the beginning no ball eaten at all...
	_numberBallsEaten = 0;

	// This variable is saved, so it could different than 0 later...
	_numberRound = 0; 

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr);
	const PACMAN::DataGame::LevelDefinition& lD = g -> levelDefinition (g -> level ());
	const PACMAN::DataGame::LevelDefinition::ScatterChaseCycle& sCC = 
		lD.scatterChaseCycle (counter (_COUNTERMONSTERCHASINGCYCLES) -> value ());
	counter (_COUNTERMONSTERSCHASING) -> initialize 
		((int) (sCC.secondsChase () * __BD g -> framesPerSecond ()), 0, true, false);
	counter (_COUNTERMONSTERSRUNNINGAWAY) -> initialize 
		((int) (sCC.secondsScatter () * __BD g -> framesPerSecond ()), 0, true, false);
	counter (_COUNTERMONSTERSTIMETOLEAVEHOME) -> initialize 
		((int) (__BD lD.leaveHomeCondition (_numberRound).maxSecondsToLeave () * __BD g -> framesPerSecond ()), 0, true, true);

	_firstTimeUpdateMethod = true;

	_messagesToShown =  { };

	_internalLoopCounter = 0;
}

// ---
void PACMAN::Scene::updatePositions ()
{
	QGAMES::Scene::updatePositions ();

	if (_firstTimeUpdateMethod)
	{
		_firstTimeUpdateMethod = false;

		// Transmit to all monsters reference to th others
		// Some of thm could need those for e.g. calculate the target position
		std::vector <PACMAN::Artist*> rf = { };
		for (auto i : characters ())
			if (dynamic_cast <PACMAN::Artist*> (i.second) != nullptr)
				rf.push_back (static_cast <PACMAN::Artist*> (i.second));
		for (auto i : rf) // All are transmited to the rest...and they will decide whether to accept them, part or none...
			i -> setReferenceArtists (rf);

		_monsterActionBlocks = { };
		_numberMonstersMoving = { };
		for (auto i : actionBlocks ())
			if (dynamic_cast <PACMAN::MonsterSceneActionBlock*> (i) != nullptr)
				_monsterActionBlocks.push_back (static_cast <PACMAN::MonsterSceneActionBlock*> (i));
	}

	if (!_clapperBoard)
		return; // It is not still working...

	// This piece of code is to control when and who (monster) has to start the movement
	// There is a general timer defined at configuration level:
	// Any time, that timer gets the limit, the next monster is launched
	// The value of this counter is determined by the level!
	// When all monsters are moving, then this piece of code is jumped!
	// The monster scene block controls also when a monster is launched. 
	// In that case, it will depend on the number of ball eaten by pacman.
	if (!onOffSwitch (_SWITCHALLMONSTERSMOVING) -> isOn () && counter (_COUNTERMONSTERSTIMETOLEAVEHOME) -> isEnd ())
		onOffSwitch (_SWITCHALLMONSTERSMOVING) -> set (!launchNextMonster ());

	// Deal with the threaten state!
	// Is it on?
	if (onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> isOn ())
	{
		// Is it time to finish the state?
		if (counter (_COUNTERMONSTERSBEINGTHREATEN) -> isEnd ())
		{
			onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> set (false);

			onOffSwitch (_SWITCHMONSTERSBLINKING) -> set (false);

			setMonstersBlinking (false, 0 /** not used. */);

			setMonstersBeingThreaten (false);

			game () -> soundBuilder () -> sound (__PACMAN_SOUNDTHREATING__) -> stop ();
		}
		// If not...
		else
		{
			// ...and the time passed is half the total, the monsters start to blink
			if (!onOffSwitch (_SWITCHMONSTERSBLINKING) -> isOn () &&
				counter (_COUNTERMONSTERSBEINGTHREATEN) -> value () >= 
					(counter (_COUNTERMONSTERSBEINGTHREATEN) -> limitValue () >> 1))
			{
				onOffSwitch (_SWITCHMONSTERSBLINKING) -> set (true);

				setMonstersBlinking (true, 
					counter (_COUNTERMONSTERSBEINGTHREATEN) -> value () >> 3 /** 1/8 per change, 4 cycles */);
			}
		}
	}

	// Deal with the chasing state!
	// Are they?
	if (onOffSwitch (_SWITCHMONSTERSCHASING) -> isOn ())
	{
		// is it the end of the scatter-chase cycle?
		if (counter (_COUNTERMONSTERSCHASING) -> isEnd ())
		{
			onOffSwitch (_SWITCHMONSTERSCHASING) -> set (false);

			// back to the scatter situation...
			setMonstersChasing (false);

			counter (_COUNTERMONSTERCHASINGCYCLES) -> isEnd (); // To add 1 to the number of cycles excuted and

			// ...preparing the context for the next cycle...
			PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
			assert (g != nullptr);
			const PACMAN::DataGame::LevelDefinition::ScatterChaseCycle& sCC = 
				g -> levelDefinition (g -> level ()).scatterChaseCycle (counter (_COUNTERMONSTERCHASINGCYCLES) -> value ());
			counter (_COUNTERMONSTERSCHASING) -> initialize 
				((int) (sCC.secondsChase () * __BD g -> framesPerSecond ()), 0, true, false);
			counter (_COUNTERMONSTERSRUNNINGAWAY) -> initialize 
				((int) (sCC.secondsScatter () * __BD g -> framesPerSecond ()), 0, true, false);
		}
	}
	// If not...
	else
	{
		// ..is it time to start chasing?
		if (counter (_COUNTERMONSTERSRUNNINGAWAY) -> isEnd ())
		{
			onOffSwitch (_SWITCHMONSTERSCHASING) -> set (true);

			setMonstersChasing (true);
		}
	}

	// When the monsters and pacman have been froozen
	if (onOffSwitch (_SWITCHARTISTSFROZEN) -> isOn () &&
		counter (_COUNTERARTISTSFROZEN) -> isEnd ())
	{
		onOffSwitch (_SWITCHARTISTSFROZEN) -> set (false);

		freezeMonstersAndPacmanForAWhile (false);
	}

	_internalLoopCounter++;

	// Update the time the messages are live...
	// and remove the ones already old...
	auto i = _messagesToShown.begin ();
	while (i != _messagesToShown.end ())
	{
		if ((_internalLoopCounter - (*i).second -> _startingLoop) >= (*i).second -> _numberLoops)
		{
			delete ((*i).second);

			i = _messagesToShown.erase (i);
		}
		else
			++i;
	}
}

// ---
void PACMAN::Scene::drawOn (QGAMES::Screen* scr, const QGAMES::Position& p)
{
	QGAMES::Scene::drawOn (scr, p);

	// Draw the messages if any...
	for (const auto& i : _messagesToShown)
		i.second -> _objTxt -> drawOn (scr, i.second -> _position);
}

// ---
void PACMAN::Scene::finalize ()
{
	QGAMES::Scene::finalize ();

	if (onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> isOn ())
		game () -> soundBuilder () -> sound (__PACMAN_SOUNDTHREATING__) -> stop ();

	_numberRound++;
}

// ---
void PACMAN::Scene::processEvent (const QGAMES::Event& evnt)
{
	// The scene can have several types of maps behind
	// That mapo alsop can be shared in other scenes
	// So the events comming from them can only be processed in the case the scene is the active one...
	if (world () == nullptr || 
		(world () != nullptr && world () -> activeScene () != this))
		return;

	if (evnt.code () == __PACMAN_BALLEATEN__)
	{
		PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
		assert (g != nullptr);
		assert (evnt.data () != nullptr); // It shouldn't but just in case...

		counter (_COUNTERMONSTERSTIMETOLEAVEHOME) -> initialize (); // Starts back any time a ball is eaten...

		_totalNumberBallsEaten++; 
		_numberBallsEaten++;
		_percentageCleaned = __BD _totalNumberBallsEaten / __BD _totalNumberBallsToEat;

		playSiren (); // The sound of the siren can change...

		// If the ball eaten has power, the status of the scsne changes!
		if (static_cast <PACMAN::TilePath*> (evnt.data ()) -> hasPower ()) 
		{
			onOffSwitch (_SWITCHMONSTERSBEINGTHREATEN) -> set (true);

			counter (_COUNTERMONSTERSBEINGTHREATEN) -> initialize  
				((int) (g -> levelDefinition ().secondsChasing () * __BD g -> currentLoopsPerSecond ()), 0, true, false);

			setMonstersBeingThreaten (true);

			game () -> soundBuilder () -> sound (__PACMAN_SOUNDTHREATING__) -> play (__QGAMES_GAMESOUNDCHANNEL__);
		}
	}
	else
	if (evnt.code () == __PACMAN_MONSTERSTARTEDTOMOVE__)
		_numberMonstersMoving.push_back (static_cast <PACMAN::MonsterSceneActionBlock*> (evnt.data ()) -> monsterNumber ());

	QGAMES::Scene::processEvent (evnt);
}

// ---
void PACMAN::Scene::showMessage (PACMAN::Scene::MessageToShow* m)
{
	assert (m != nullptr);

	m -> _startingLoop = _internalLoopCounter;
	m -> _objTxt = game () -> textBuilder () -> createText (m -> _font, m -> _text);
	
	_messagesToShown.insert (std::map <int, PACMAN::Scene::MessageToShow*>::value_type (m -> _startingLoop, m));
}

// ---
__IMPLEMENTCOUNTERS__ (PACMAN::Scene::Counters)
{
	// Many of them are better initialized along the game...
	addCounter (new QGAMES::Counter (_COUNTERMONSTERSCHASING, 1, 0, true, false));
	addCounter (new QGAMES::Counter (_COUNTERMONSTERSRUNNINGAWAY, 1, 0, true, false));
	addCounter (new QGAMES::Counter (_COUNTERMONSTERCHASINGCYCLES, 999999 /** Almost infinite. */, 0, true, false));
	addCounter (new QGAMES::Counter (_COUNTERMONSTERSBEINGTHREATEN, 1, 0, true, false));
	addCounter (new QGAMES::Counter (_COUNTERMONSTERSTIMETOLEAVEHOME, 1, 0, true, false));
	addCounter (new QGAMES::Counter (_COUNTERMONSTERTOLEAVE, 999999 /** Never ends. */, 0, true, false));
	addCounter (new QGAMES::Counter (_COUNTERARTISTSFROZEN, 
		(int) (QGAMES::Game::game () -> framesPerSecond () * 0.5 /** 0.5 seconds only. */), 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (PACMAN::Scene::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHMONSTERSCHASING, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHMONSTERSBEINGTHREATEN, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHALLMONSTERSMOVING, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHMONSTERSBLINKING, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch (_SWITCHARTISTSFROZEN, false));
}

// ---
void PACMAN::Scene::setMonstersChasing (bool c)
{
	for (auto i : characters ())
		if (dynamic_cast <PACMAN::Monster*> (i.second) != nullptr)
			(static_cast <PACMAN::Monster*> (i.second)) -> toChase (c);
}

// ---
void PACMAN::Scene::setMonstersBeingThreaten (bool o)
{
	for (auto i : characters ())
		if (dynamic_cast <PACMAN::Monster*> (i.second) != nullptr)
			(static_cast <PACMAN::Monster*> (i.second)) -> toBeThreaten (o);
}

// ---
void PACMAN::Scene::setMonstersBlinking (bool b, int bp)
{
	for (auto i : characters ())
		if (dynamic_cast <PACMAN::Monster*> (i.second) != nullptr)
			(static_cast <PACMAN::Monster*> (i.second)) -> toBlink (b, bp);
}

// ---
bool PACMAN::Scene::launchNextMonster ()
{
	int nMN = -1;
	do
	{
		nMN = counter (_COUNTERMONSTERTOLEAVE) -> value ();
		counter (_COUNTERMONSTERTOLEAVE) -> isEnd ();
	} while (std::find (_numberMonstersMoving.begin (), _numberMonstersMoving.end (), nMN) != _numberMonstersMoving.end ());

	bool atLeastOne = false;
	for (auto i : _monsterActionBlocks)
		atLeastOne |= i -> timeToStart (nMN /** adds one to the number of monster. */);

	return (atLeastOne);
}

// ---
void PACMAN::Scene::freezeMonstersAndPacmanForAWhile (bool s)
{
	onOffSwitch (_SWITCHARTISTSFROZEN) -> set (s);

	counter (_COUNTERARTISTSFROZEN) -> initialize ();

	for (auto i : characters ())
		if (dynamic_cast <PACMAN::Monster*> (i.second) != nullptr || 
			dynamic_cast <PACMAN::PacMan*> (i.second) != nullptr)
			(static_cast <PACMAN::Artist*> (i.second)) -> toFreeze (s);
}

// ---
void PACMAN::Scene::recalculateInternalVariablesForBallsEatenControl ()
{
	_totalNumberBallsToEat = -1;  
	maxNumberBallsToEat (); 
	_totalNumberBallsEaten = -1;
	numberBallsEaten ();
	_percentageCleaned = __BD _totalNumberBallsEaten / _totalNumberBallsToEat;
}
