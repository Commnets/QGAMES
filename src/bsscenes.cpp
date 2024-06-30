#include <BattleshipLike/bsscenes.hpp>
#include <BattleshipLike/bsactionblocks.hpp>
#include <BattleshipLike/bsgame.hpp>
#include <BattleshipLike/bsbuilders.hpp>
#include <Common/background.hpp>

// ---
const BATTLESHIP::PlayingScene::ExternalEventToCompleteSceneFuncion 
	BATTLESHIP::PlayingScene::_NOEXTERNALEVNT = [](BATTLESHIP::PlayingScene*) -> bool { return (false); };
const BATTLESHIP::PlayingScene::ExternalEventToCompleteSceneFuncion 
	BATTLESHIP::PlayingScene::_NOSTDUFOSINSCENEEXTERNALEVNT = [](BATTLESHIP::PlayingScene* pS) -> bool
		{
			assert (pS);

			bool result = false;

			for (QGAMES::SceneActionBlocks::const_iterator i = pS -> actionBlocks ().begin (); 
					i != pS -> actionBlocks ().end () && !result; i++)
			{
				// The two types of standard blocks controlling UFOS
				if (dynamic_cast <BATTLESHIP::StdUFOSceneActionBlock*> ((*i)) != nullptr)
					result |= ((BATTLESHIP::StdUFOSceneActionBlock*) (*i)) -> spaceElementsUnderControl ();
				if (dynamic_cast <BATTLESHIP::StdSetUFOsSceneActionBlock*> ((*i)) != nullptr)
					result |= ((BATTLESHIP::StdSetUFOsSceneActionBlock*) (*i)) -> spaceElementsUnderControl ();
			}

			return (!result);
		};

// ---
std::string BATTLESHIP::Scene::name () const
{
	return (existsProperty (__BATTLESHIP_SCENEATTRNAME__) 
		? property (__BATTLESHIP_SCENEATTRNAME__) : __BATTLESHIP_SCENEDEFATTRNAME__);
}

// ---
QGAMES::Vector BATTLESHIP::Scene::movement () const
{
	QGAMES::Vector result = __BATTLESHIP_SCENEDEFATTRMOVEMENT__;
	if (existsProperty (__BATTLESHIP_SCENEATTRMOVEMENT__))
	{
		std::vector <int> vD = QGAMES::getElementsFromAsInt (property (__BATTLESHIP_SCENEATTRMOVEMENT__), ',');
		assert (vD.size () == 3); // Just three no more no less...
		result = QGAMES::Vector (__BD vD [0], __BD vD [1], __BD vD [2]);
	}

	return (result);
}

// ---
BATTLESHIP::PlayingScene::PlayingScene (int c, const QGAMES::Maps& m, 
		const QGAMES::Scene::Connections& cn, const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL,
		const BATTLESHIP::PlayingScene::ExternalEventToCompleteSceneFuncion& eF)
	: BATTLESHIP::Scene (c, m, cn, p, ePL),
	  _externalEventSceneCompletedFunction (eF),
	  _speed (1),
	  _battleship (NULL),
	  _charactersAdded (), // Empty...
	  _asteroids (),
	  _thingsToCatch (),
	  _enemies (),
	  _backgroundSound (-1)
{ 
	// To remove a shooting from the scene in another loop different than the one processing the event...
	addBuoy (__BATTLESHIP_ENDSHOOTINGBUOY__, 
		new BATTLESHIP::PlayingScene::ToEndShootingBuoy (__BATTLESHIP_ENDSHOOTINGBUOY__));
}

// ---
void BATTLESHIP::PlayingScene::setSpeed (int s)
{
	assert (s >= 1 && s <= 10);

	_speed = s;

	// The active map is understood as a background map
	// Change the speed that the background object of the main map has...
	// Take into account that the active map has to be first of all an ObjectMap, 
	// otherwise there wont't be a background...
	QGAMES::ObjectMap* oM = dynamic_cast <QGAMES::ObjectMap*> (activeMap ()); 
	assert (oM);
	// There must be also a layer minimum....
	assert (oM -> objects ().size () > 0);
	QGAMES::ObjectLayer* oL = (*oM -> objects ().begin ()).second;
	// ...with at leat one object...
	// ...and with the right type!
	assert (oL -> objects ().size () > 0);
	QGAMES::ObjectProxy* oP = dynamic_cast <QGAMES::ObjectProxy*> (*oL -> objects ().begin ());
	assert (oP);
	// And that object has to have the type background...
	QGAMES::Background* bK = dynamic_cast <QGAMES::Background*> (oP -> ptr ());
	assert (bK);

	// Finaly the speed is changed...
	bK -> setMovementData (1 /** minimum loop */, _speed);
}

// ---
void BATTLESHIP::PlayingScene::sceneCompleted ()
{
	// The battleship celebrates the victory...
	battleship () -> toRotate ();

	// Stops the music
	if (_backgroundSound != -1 && game () -> sound (_backgroundSound) -> isPlaying ())
		game () -> sound (_backgroundSound) -> stop ();
	_backgroundSound = -1;

	// ...and the control game state is informed, to change the state...
	// It is informed through the battleship who is the one being observed by the play control
	battleship () -> notify (QGAMES::Event (__BATTLESHIP_SPACESHIPREACHEDGOAL__, battleship ()));

	// Just only when the sucess has been notified, the scene is marked as completed!
	// If it wan't the first time the scene is played, this action would have actually no effect...
	BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (bG);
	bG -> setSceneCompleted (id ());
}

// ---
void BATTLESHIP::PlayingScene::initialize ()
{
	BATTLESHIP::Game* dG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (dG);

	// Adds the battlesip...
	_battleship = dynamic_cast <BATTLESHIP::Battleship*> 
		(dG -> character (__BATTLESHIP_MAINARTISTBASEID__ + dG -> artistType ()));
	assert (_battleship); // Just in case...
	addCharacter (_battleship);
	_battleship -> setMap (activeMap ());

	dG -> character (__BATTLESHIP_BATTLESHIPTYPE1ID__) -> setVisible (false);
	dG -> character (__BATTLESHIP_BATTLESHIPTYPE2ID__) -> setVisible (false);
	_battleship -> setVisible (true);

	// Now it is time to add all characters than can appear in any scene potentially 
	// Those characters have to be defined in the entities.xml file.
	// In the DataGame class the maximum number of those has to be also defined...
	// Ideally, the maxNumber of each has to be aligned with the number defined in the entities.xml file
	// By there could be differences. Those differences are guarantted...

	// First the asteroids...
	for (int i = 0; i < __BATTLESHIP_MAXASTEROIDS__; i++)
	{
		BATTLESHIP::Asteroid* a = 
			dynamic_cast <BATTLESHIP::Asteroid*> (game () -> character (__BATTLESHIP_ASTEROIDBASEID__ + i));
		// If the element doesn't exist then an assertion will be issued!
		assert (a); // Just in case...
		addCharacter (a);
		a -> setMap (activeMap ());

		a -> setVisible (false);

		_asteroids [a -> id ()] = a;
		_charactersAdded [a -> id ()] = a;
	}

	// Then any other thing to be caught...
	for (int i = 0; i < __BATTLESHIP_MAXTHINGSTOCATCH__; i++)
	{
		BATTLESHIP::ThingToCatch* tC =
			dynamic_cast <BATTLESHIP::ThingToCatch*> (game () -> character (__BATTLESHIP_THINGTOCATCHBASEID__ + i));
		assert (tC); // Just in case...
		addCharacter (tC);
		tC -> setMap (activeMap ());

		tC -> setVisible (false);

		_thingsToCatch [tC -> id ()] = tC;
		_charactersAdded [tC -> id ()] = tC;
	}

	// and any other enemy...(if any)
	for (int i = 0; i < __BATTLESHIP_MAXNUMBERENEMIES__; i++)
	{
		BATTLESHIP::Spaceship* sS =
			dynamic_cast <BATTLESHIP::Spaceship*> (game () -> character (__BATTLESHIP_ENEMYBASEID__ + i));
		// If the element doesn't exist then an assertion will be issued!
		assert (sS); // Just in case...
		addCharacter (sS);
		sS -> setMap (activeMap ());

		sS -> setVisible (false);

		_enemies [sS -> id ()] = sS;
		_charactersAdded [sS -> id ()] = sS;
	}

	// The shootings should be added to the scene here, but it could be many. 
	// So, in that case, when the Scene's updatePositions method is executed, 
	// a big delay could happen, even if only a few of shootings were moving
	// only because the method would iterate over all of them...
	// So it is a better design decision, that they are added only when they are launched, 
	// and removed from the scene when they finish...

	// ...and finally the shootings
	// The shootings are only assigned to every spaceship (including the battleship)
	_battleship -> loadShootings (game () -> entities 
		(__BATTLESHIP_SPACESHIPSHOOTINGBASEID__, 
		 __BATTLESHIP_SPACESHIPSHOOTINGBASEID__ + __BATTLESHIP_SPACESHIPMAXSHOOTINGSPERSPACESHIP__ - 1));
	for (int i = 0; i < __BATTLESHIP_MAXNUMBERENEMIES__; i++)
	{
		BATTLESHIP::Spaceship* sS = dynamic_cast <BATTLESHIP::Spaceship*> 
			(game () -> character (__BATTLESHIP_ENEMYBASEID__ + i));
		assert (sS); // Just in case;
		int iS = __BATTLESHIP_SPACESHIPSHOOTINGBASEID__ + ((i + 1) * __BATTLESHIP_SPACESHIPMAXSHOOTINGSPERSPACESHIP__);
		sS -> loadShootings (game () -> entities (iS, iS + __BATTLESHIP_SPACESHIPMAXSHOOTINGSPERSPACESHIP__ - 1));
	}

	// Here all, the initialization...
	// The scene blocks are also initialized here (if they were defined in the conf file)
	BATTLESHIP::Scene::initialize ();

	// The background map (if any) is initialized above and it automatically moves,
	// But in a playing scene the background doesn't move alone ever
	if (backgroundMap () != NULL)
		backgroundMap () -> stop (); // Don't move it...

	// Sets the background music if it has been defined in the configuration file, 
	// ..and it has been also allowed...
	_backgroundSound = -1;
	if (((BATTLESHIP::Game::Conf*) dG -> configuration ()) -> musicOn () &&
		existsProperty (__BATTLESHIP_SCENEATTRBACKMUSIC__))
	{
		_backgroundSound = std::atoi (property (__BATTLESHIP_SCENEATTRBACKMUSIC__).c_str ()); 
		dG -> sound (_backgroundSound) -> play (__QGAMES_BACKGROUNDSOUNDCHANNEL__);
	}

	// The speed of the scene is algned with the difficulty level!
	BATTLESHIP::Game::Conf* cfg = dynamic_cast <BATTLESHIP::Game::Conf*> (game () -> configuration ());
	assert (cfg);
	setSpeed (1 + (cfg -> difficulty () << 1));
}

// ---
void BATTLESHIP::PlayingScene::updatePositions ()
{
	BATTLESHIP::Scene::updatePositions ();

	// Is there any external event to finish the scene?
	if (_externalEventSceneCompletedFunction (this))
		sceneCompleted ();
}

// ---
void BATTLESHIP::PlayingScene::finalize ()
{
	BATTLESHIP::Scene::finalize ();

	_battleship -> setVisible (false);

	removeCharacter (_battleship);

	_battleship = NULL;

	// Remove all characters added previously...
	for (QGAMES::Entities::const_iterator i = _charactersAdded.begin (); i != _charactersAdded.end (); i++)
	{
		(*i).second -> setVisible (false);
		removeCharacter (dynamic_cast <QGAMES::Character*> ((*i).second));
	}

	_charactersAdded = QGAMES::Entities ();
	_asteroids = QGAMES::Entities ();
	_thingsToCatch = QGAMES::Entities ();
	_enemies = QGAMES::Entities ();

	// ...and stops the sound if any
	if (_backgroundSound != -1 && game () -> sound (_backgroundSound) -> isPlaying ())
		game () -> sound (_backgroundSound) -> stop ();
}

// ---
void BATTLESHIP::PlayingScene::processEvent (const QGAMES::Event& evnt)
{
	switch (evnt.code ())
	{
		// When a shooting is launched, it has to be added to the scene...
		case __BATTLESHIP_SHOOTINGLAUNCHED__:
			addCharacter ((QGAMES::Character*) evnt.data ());
			break;

		// When the shooting finishes, it is removed from the scene...
		// Using a buoy to do it in a different loop than the one processign this event!
		case __BATTLESHIP_SHOOTINGTOEND__:
			{
				BATTLESHIP::PlayingScene::ToEndShootingBuoy* b = 
					dynamic_cast <BATTLESHIP::PlayingScene::ToEndShootingBuoy*> (buoy (__BATTLESHIP_ENDSHOOTINGBUOY__));
				assert (b); // Just in case...

				b -> addShooting ((BATTLESHIP::SpaceshipShooting*) evnt.data ());
				b -> active (true);
			}

			break;

		// When an element has been destroyed, then it is turned into unvisible...
		case __BATTLESHIP_SPACEELMNTHASDESTROYED__:
			((BATTLESHIP::SpaceElement*) evnt.data ()) -> setVisible (false);
			break;

		// When the block inside has finished...
		case __BATTLESHIP_SPACEELEMENTACTIONBLOCKTIMEOVER__:
			{
				BATTLESHIP::StdSpaceElementsSceneActionBlock* aB = 
					(BATTLESHIP::StdSpaceElementsSceneActionBlock*) (evnt.data ());
				aB -> finalize ();
				aB -> setActive (false); // That block is no longer valid...
			}

			break;

		default:
			break;
	}

	BATTLESHIP::Scene::processEvent (evnt);
}

// ---
void BATTLESHIP::PlayingScene::ToEndShootingBuoy::addShooting (BATTLESHIP::SpaceshipShooting* sht)
{ 
	assert (sht); 
	
	if (std::find_if (_shootings.begin (), _shootings.end (),
			[=](BATTLESHIP::SpaceshipShooting* i) -> bool { return (i == sht); }) == _shootings.end ())
		_shootings.push_back (sht); 
}

// ---
void* BATTLESHIP::PlayingScene::ToEndShootingBuoy::treatFor (QGAMES::Element* e)
{
	BATTLESHIP::PlayingScene* pS = dynamic_cast <BATTLESHIP::PlayingScene*> (e);
	assert (e); // Just in case, but it shouldn't be different ever...

	// If the same soothing were twice in the list, it would get erased just once...
	std::for_each (_shootings.begin (), _shootings.end (), 
		[=](BATTLESHIP::SpaceshipShooting* i) { pS -> removeCharacter (i); }); // removed from the scene...
	_shootings.clear (); // Done

	return (this);
}

// ---
BATTLESHIP::TimeLimitScene::TimeLimitScene (int c, const QGAMES::Maps& m, 
		const QGAMES::Scene::Connections& cn, const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL,
		const ExternalEventToCompleteSceneFuncion& eF)
	: BATTLESHIP::PlayingScene (c, m, cn, p, ePL, eF),
	  _sceneTime (0) // Defined next...
{
	_sceneTime = existsProperty (__BATTLESHIP_TIMELIMITSCENEATTRSCENETIME__) 
		? __BD (std::atof (property (__BATTLESHIP_TIMELIMITSCENEATTRSCENETIME__).c_str ())) 
		: __BATTLESHIP_TIMELIMITSCENEDEFATTRSCENETIME__;
}

// ---
void BATTLESHIP::TimeLimitScene::initialize ()
{
	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	counter (_COUNTERSCENEMOVING) -> initialize 
		((int) (_sceneTime * __BD game () -> framesPerSecond ()), 0, true, true);

	BATTLESHIP::PlayingScene::initialize ();
}

// ---
void BATTLESHIP::TimeLimitScene::updatePositions ()
{
	BATTLESHIP::PlayingScene::updatePositions ();

	BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (bG); // Just in case...
	if (bG -> isGamePaused () || bG -> isNotPlaying ())
		return; 

	// No sense to count down the limit time when the battleship is not alive!
	if (!battleship () -> isAlive ())
		return;

	// When the time finishes...
	if (counter (_COUNTERSCENEMOVING) -> isEnd ())
		sceneCompleted (); // ...the scene has been completed...
}

// ---
void BATTLESHIP::TimeLimitScene::processEvent (const QGAMES::Event& evnt)
{
	BATTLESHIP::PlayingScene::processEvent (evnt);

	if (evnt.code () == __BATTLESHIP_NOUFOSMOVING__)
	{
	}
}

// ---
__IMPLEMENTCOUNTERS__ (BATTLESHIP::TimeLimitScene::Counters)
{
	addCounter (new QGAMES::Counter (BATTLESHIP::TimeLimitScene::_COUNTERSCENEMOVING, 
		0 /** It will be set later too attending the configuration of the scene. */, 0, true, true)); 
}

// ---
__IMPLEMENTONOFFSWITCHES__ (BATTLESHIP::TimeLimitScene::OnOffSwitches)
{
	// None is implemented by default...
}

// ---
BATTLESHIP::SequentialActionBlocksScene::SequentialActionBlocksScene (int c, const QGAMES::Maps& m, 
		const QGAMES::Scene::Connections& cn, const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL,
		const ExternalEventToCompleteSceneFuncion& eF)
	: BATTLESHIP::PlayingScene (c, m, cn, p, ePL, eF),
	 _sequentialActionBlocks (),
	 _currentActionBlockControlled (0)
{
	_sequentialActionBlocks = QGAMES::getElementsFromAsInt (
		existsProperty (__BATTLESHIP_SEQACTIONBLOCKSCENEATTRABLIST__) 
			? property (__BATTLESHIP_SEQACTIONBLOCKSCENEATTRABLIST__).c_str ()
			: __BATTLESHIP_SEQACTIONBLOCKSCENEDEFATTRABLIST__, ','); // All
}

// ---
QGAMES::SetOfOpenValues BATTLESHIP::SequentialActionBlocksScene::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = BATTLESHIP::PlayingScene::runtimeValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, _currentActionBlockControlled);

	return (result);
}

// ---
void BATTLESHIP::SequentialActionBlocksScene::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = cfg;
	activeSequentialBlock (_currentActionBlockControlled = _currentActionBlockControlled = cCfg.openValue (lE).intValue ());
	cCfg.removeOpenValue (lE);
	
	BATTLESHIP::PlayingScene::initializeRuntimeValuesFrom (cCfg);
}

// ---
void BATTLESHIP::SequentialActionBlocksScene::initialize ()
{
	BATTLESHIP::PlayingScene::initialize ();

	activeSequentialBlock (_currentActionBlockControlled = 0);
}

// ---
void BATTLESHIP::SequentialActionBlocksScene::updatePositions ()
{
	BATTLESHIP::PlayingScene::updatePositions ();

	BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (bG); // Just in case...
	if (bG -> isGamePaused () || bG -> isNotPlaying ())
		return; 

	if (!battleship () -> isAlive ())
		return;

	if (_currentActionBlockControlled < 0 || 
		_currentActionBlockControlled >= (int) _sequentialActionBlocks.size ())
		sceneCompleted (); 
}

// ---
void BATTLESHIP::SequentialActionBlocksScene::processEvent (const QGAMES::Event& evnt)
{
	if ((evnt.code () == __BATTLESHIP_SPACEELEMENTACTIONBLOCKTIMEOVER__ || 
		 evnt.code () == __BATTLESHIP_NOUFOSMOVING__))
		activeSequentialBlock (++_currentActionBlockControlled); 
	// When the event is sent from the last action block 
	// then no more action blocks will be active and the scene will be completed! (@see previous method)

	BATTLESHIP::PlayingScene::processEvent (evnt);
}

// ---
void BATTLESHIP::SequentialActionBlocksScene::activeSequentialBlock (int nB)
{
	for (int i = 0; i < (int) _sequentialActionBlocks.size (); i++)
		actionBlock (_sequentialActionBlocks [i]) -> setActive (i == nB);
}

// ---
BATTLESHIP::ChallengeScene::ChallengeScene (int c, const QGAMES::Maps& m, int pG,
		const QGAMES::Scene::Connections& cn, const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL,
		const ExternalEventToCompleteSceneFuncion& eF)
	: BATTLESHIP::SequentialActionBlocksScene (c, m, cn, p, ePL, eF),
	  _pointsGivenPerEnemyKilled (pG),
	  _sceneTime (__BD 0),
	  _enemiesDestroyed (0),
	  _textEnemiesDestroyed (NULL),
	  _textPointsGiven (NULL)
{
	_sceneTime = existsProperty (__BATTLESHIP_TIMELIMITSCENEATTRSCENETIME__) 
		? __BD (std::atof (property (__BATTLESHIP_TIMELIMITSCENEATTRSCENETIME__).c_str ())) 
		: __BATTLESHIP_TIMELIMITSCENEDEFATTRSCENETIME__;
}

// ---
void BATTLESHIP::ChallengeScene::sceneCompleted ()
{
	// The battleship celebrates the victory...
	// and is sealed almost forever, just to avoid remaining shootings to damage the battleship if any!
	battleship () -> toRotate ();
	battleship () -> toSeal (__BD 999); 

	// Stops the music...
	if (_backgroundSound != -1 && game () -> sound (_backgroundSound) -> isPlaying ())
		game () -> sound (_backgroundSound) -> stop ();
	_backgroundSound = -1;

	// In this case, the reached-goal notification is not sent yet and the scene is not maked as completed yet!
	// It is needed to show first for a while the number of enemies killed
	// and the points get, because this is a challenge stage!

	onOffSwitch (_SWITCHSCENESHOWINGPOINTSGOT) -> set (true);
	counter (_COUNTERAFTERMOVINGSTATES) -> initialize ();
	counter (_COUNTERELEMENTSKILLED) -> initialize (_enemiesDestroyed + 2, 0, true, false);
}

// ---
void BATTLESHIP::ChallengeScene::initialize ()
{
	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	counter (_COUNTERSCENEMOVING) -> initialize 
		((int) (_sceneTime * __BD game () -> framesPerSecond ()), 0, true, true);

	BATTLESHIP::SequentialActionBlocksScene::initialize ();

	_enemiesDestroyed = 0;
	deleteTextEnemiesDestroyedAndPointsGiven ();
}

// --
void BATTLESHIP::ChallengeScene::updatePositions ()
{
	BATTLESHIP::SequentialActionBlocksScene::updatePositions ();

	BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (bG); // Just in case...
	if (bG -> isGamePaused () || bG -> isNotPlaying ())
		return; 

	// Tow possible states of the scene...
	// 1.- Notifying the number of enemies killed...
	if (onOffSwitch (_SWITCHSCENESHOWINGPOINTSGOT) -> isOn ())
	{
		// Two possible states: Which one to choose?
		switch (counter (_COUNTERAFTERMOVINGSTATES) -> value ())
		{
			// Counting up to the number of enemies killed...
			case 0:
				{
					deleteTextEnemiesDestroyedAndPointsGiven ();

					_textEnemiesDestroyed = game () -> textBuilder () -> createText (__QGAMES_RAVIE24GREENLETTERS__, 
						std::string ("Enemies killed: ") + 
						std::to_string (counter (_COUNTERELEMENTSKILLED) -> value ())); 

					game () -> sound (__QGAMES_BEEPWAVSOUND__) -> play (__QGAMES_BACKGROUNDSOUNDCHANNEL__); // Counting sound...

					if (counter (_COUNTERELEMENTSKILLED) -> isEnd ()) // Counter to next element killed finishes...
					{
						battleship () -> // add the points to the score...
							setScore (battleship () -> score () + (_enemiesDestroyed * _pointsGivenPerEnemyKilled));

						counter (_COUNTERSHOWINGENEMIESKILLED) -> initialize ();

						counter (_COUNTERAFTERMOVINGSTATES) -> isEnd  (); // Next state = 1
					}
				}

				break;

			// Then showing the final number of enemies killed for a while...
			case 1:
				{
					if (counter (_COUNTERSHOWINGENEMIESKILLED) -> isEnd ())
						counter (_COUNTERAFTERMOVINGSTATES) -> isEnd (); // Next state = 2
				}

				break;

			// Now it is time to create the text for points given also...
			case 2:
				{
					_textPointsGiven = game () -> textBuilder () -> createText (__QGAMES_RAVIE24GREENLETTERS__,
						std::string ("Points given: ") + std::to_string (_enemiesDestroyed * _pointsGivenPerEnemyKilled));

					game () -> sound (__QGAMES_SAVEWAVSOUND__) -> play (__QGAMES_BACKGROUNDSOUNDCHANNEL__); // Points given sound...

					counter (_COUNTERSHOWINGALSOPOINTSGIVEN) -> initialize ();

					counter (_COUNTERAFTERMOVINGSTATES) -> isEnd  (); // Next state = 3
				}

				break;

			// ..and showing them also for a while...
			case 3:
				{
					if (counter (_COUNTERSHOWINGALSOPOINTSGIVEN) -> isEnd ())
						counter (_COUNTERAFTERMOVINGSTATES) -> isEnd  (); // Next state = 3
				}

				break;

			// ...and finally the stage finishes!
			case 4:
				{	
					battleship () -> notify (QGAMES::Event (__BATTLESHIP_SPACESHIPREACHEDGOAL__, battleship ()));

					bG -> setSceneCompleted (id ());
				}
				
				break;

			default:
				assert (0); // It shoouldn't be here anyway...
				break; 
		}
	}

	// 2.- Just playing...
	else
	{
		if (!battleship () -> isAlive ())
			return;

		if (counter (_COUNTERSCENEMOVING) -> isEnd ())
			sceneCompleted (); // The scene is completed, but no notification to the game state control is still sent!
	}
}

// ---
void BATTLESHIP::ChallengeScene::drawOn (QGAMES::Screen* s, const QGAMES::Position& pos)
{
	BATTLESHIP::SequentialActionBlocksScene::drawOn (s, pos);

	if (onOffSwitch (_SWITCHSCENESHOWINGPOINTSGOT) -> isOn ())
	{
		if (_textEnemiesDestroyed)
		{
			QGAMES::Position iP = s -> position () + 
				QGAMES::Vector (__BD ((s -> visualWidth () - _textEnemiesDestroyed -> visualLength ()) >> 1),
								__BD ((s -> visualHeight () - _textEnemiesDestroyed -> visualHeight ()) >> 1), __BD 0) -
				QGAMES::Vector (__BD 0, __BD 100, __BD 0); // A little up!
			_textEnemiesDestroyed -> drawOn (s, (pos == QGAMES::Position::_noPoint) ? iP : pos + iP);
		}

		if (_textPointsGiven)
		{
			QGAMES::Position iP = s -> position () + 
				QGAMES::Vector (__BD ((s -> visualWidth () - _textPointsGiven -> visualLength ()) >> 1),
								__BD ((s -> visualHeight () - _textPointsGiven -> visualHeight ()) >> 1), __BD 0) -
				QGAMES::Vector (__BD 0, __BD 50, __BD 0); // A little up!
			_textPointsGiven -> drawOn (s, (pos == QGAMES::Position::_noPoint) ? iP : pos + iP);
		}
	}
}

// ---
void BATTLESHIP::ChallengeScene::finalize ()
{
	BATTLESHIP::SequentialActionBlocksScene::finalize ();

	deleteTextEnemiesDestroyedAndPointsGiven ();
}

// ---
void BATTLESHIP::ChallengeScene::processEvent (const QGAMES::Event& evnt)
{
	BATTLESHIP::SequentialActionBlocksScene::processEvent (evnt);

	if (evnt.code () == __BATTLESHIP_SPACEELMNTHASDESTROYED__)
		_enemiesDestroyed++; // One more...(everything is considered an enemy)
}

// ---
__IMPLEMENTCOUNTERS__ (BATTLESHIP::ChallengeScene::Counters)
{
	addCounter (new QGAMES::Counter (BATTLESHIP::ChallengeScene::_COUNTERSCENEMOVING, 
		0 /** Initialized later. */, 0, true, true));
	addCounter (new QGAMES::Counter (BATTLESHIP::ChallengeScene::_COUNTERAFTERMOVINGSTATES, 5, 0, true, false));
	addCounter (new QGAMES::Counter (BATTLESHIP::ChallengeScene::_COUNTERELEMENTSKILLED, 
		0 /** Initialized later. */, 0, true, false));
	addCounter (new QGAMES::Counter (BATTLESHIP::ChallengeScene::_COUNTERSHOWINGENEMIESKILLED,
		QGAMES::Game::game () -> framesPerSecond () * 3, 0, true, false));
	addCounter (new QGAMES::Counter (BATTLESHIP::ChallengeScene::_COUNTERSHOWINGALSOPOINTSGIVEN,
		QGAMES::Game::game () -> framesPerSecond () * 3, 0, true, false));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (BATTLESHIP::ChallengeScene::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (BATTLESHIP::ChallengeScene::_SWITCHSCENESHOWINGPOINTSGOT, false));
}

// ---
QGAMES::SetOfOpenValues BATTLESHIP::FocusingInOneElementScene::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = BATTLESHIP::PlayingScene::runtimeValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, 
		QGAMES::OpenValue ((_focusedElement == NULL) ? __MININT__ : _focusedElement -> id ()));

	return (result);
}

// ---
void BATTLESHIP::FocusingInOneElementScene::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = cfg;
	int fE = cCfg.openValue (lE).intValue ();
	cCfg.removeOpenValue (lE);
	if (fE != __MININT__)
	{
		BATTLESHIP::SpaceElement* spc = 
			dynamic_cast <BATTLESHIP::SpaceElement*> ((*enemies ().find (fE)).second);
		assert (spc);
		setFocusedElement (spc);
	}

	BATTLESHIP::PlayingScene::initializeRuntimeValuesFrom (cCfg);
}

// ---
void BATTLESHIP::FocusingInOneElementScene::sceneCompleted ()
{
	battleship () -> toRotate ();
	battleship () -> toSeal (__BD 999); 

	if (_backgroundSound != -1 && game () -> sound (_backgroundSound) -> isPlaying ())
		game () -> sound (_backgroundSound) -> stop ();
	_backgroundSound = -1;

	onOffSwitch (_SWITCHSCENESHOWINGENEMYKILLED) -> set (true);
	counter (_COUNTERSHOWINGFOCUSENEMYKILLED) -> initialize ();

	game () -> sound (__QGAMES_WICKWAVSOUND__) -> play (__QGAMES_BACKGROUNDSOUNDCHANNEL__); 
}

// ---
void BATTLESHIP::FocusingInOneElementScene::initialize ()
{
	// The focused element can be set later...
	if (_focusedElement != NULL)
	{
		// ...to asign it the bar of energy!
		BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
		assert (bG);

		bG -> badGuyEnergyLevelScoreObject () -> setVisible (true);
		bG -> badGuyEnergyLevelScoreObject () -> observe (_focusedElement);
	}

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	if (_textFocusedElementDestroyedSO == NULL)
		_textFocusedElementDestroyedSO = 
			game () -> textBuilder () -> createText (__QGAMES_RAVIE24ORANGELETTERS__, _textFocusedElementDestroyed);

	BATTLESHIP::PlayingScene::initialize ();
}

// ---
void BATTLESHIP::FocusingInOneElementScene::updatePositions ()
{
	BATTLESHIP::PlayingScene::updatePositions ();

	BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (bG); // Just in case...

	// Adds the score, if the focused element appears and this one was not already present.
	if (_focusedElement != NULL && 
		!bG -> badGuyEnergyLevelScoreObject () -> isVisible ())
	{
		bG -> badGuyEnergyLevelScoreObject () -> setVisible (true);
		bG -> badGuyEnergyLevelScoreObject () -> observe (_focusedElement);
	}

	if (bG -> isGamePaused () || bG -> isNotPlaying ())
		return; 

	// Just when the 
	if (onOffSwitch (_SWITCHSCENESHOWINGENEMYKILLED) -> isOn ())
	{
		if (counter (_COUNTERSHOWINGFOCUSENEMYKILLED) -> isEnd ())
		{
			battleship () -> notify (QGAMES::Event (__BATTLESHIP_SPACESHIPREACHEDGOAL__, battleship ()));

			bG -> setSceneCompleted (id ());
		}
	}
}

// ---
void BATTLESHIP::FocusingInOneElementScene::drawOn (QGAMES::Screen* s, const QGAMES::Position& pos)
{
	BATTLESHIP::PlayingScene::drawOn (s, pos);

	if (onOffSwitch (_SWITCHSCENESHOWINGENEMYKILLED) -> isOn ())
	{
		assert (_textFocusedElementDestroyedSO);
		QGAMES::Position iP = s -> position () + 
			QGAMES::Vector (__BD ((s -> visualWidth () - _textFocusedElementDestroyedSO -> visualLength ()) >> 1),
							__BD ((s -> visualHeight () - _textFocusedElementDestroyedSO -> visualHeight ()) >> 1), __BD 0) -
			QGAMES::Vector (__BD 0, __BD 50, __BD 0); // A little up!
		_textFocusedElementDestroyedSO -> drawOn (s, (pos == QGAMES::Position::_noPoint) ? iP : pos + iP);
	}
}

// ---
void BATTLESHIP::FocusingInOneElementScene::finalize ()
{
	deleteTextEnemyFocusedDestroyed ();

	if (_focusedElement != NULL)
	{
		BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
		assert (bG);

		bG -> badGuyEnergyLevelScoreObject () -> unObserve (focusedElement ());
	}		

	_focusedElement = NULL;

	BATTLESHIP::PlayingScene::finalize ();
}

// ---
void BATTLESHIP::FocusingInOneElementScene::processEvent (const QGAMES::Event& evnt)
{
	BATTLESHIP::PlayingScene::processEvent (evnt);

	if (evnt.code () == __BATTLESHIP_SPACEELMNTHASDESTROYED__ && evnt.data () == (void*) _focusedElement)
		sceneCompleted ();
}

// ---
__IMPLEMENTCOUNTERS__ (BATTLESHIP::FocusingInOneElementScene::Counters)
{
	addCounter (new QGAMES::Counter (BATTLESHIP::FocusingInOneElementScene::_COUNTERSHOWINGFOCUSENEMYKILLED,
		QGAMES::Game::game () -> framesPerSecond () * 3, 0, true, false));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (BATTLESHIP::FocusingInOneElementScene::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (BATTLESHIP::FocusingInOneElementScene::_SWITCHSCENESHOWINGENEMYKILLED, false));
}

// ---
void BATTLESHIP::TimeLimitSceneExample::initialize ()
{
	if (_backgroundMapId == -1)
		_backgroundMapId = __BATTLESHIP_COMPLEXSTARTSMOVINGBACKGROUNDMAPBASEID__ + 
			((rand () % __BATTLESHIP_NUMBERCOMPLEXSTARTMOVINGBACKGROUND__)); 

	// The background transparent is the base one for any playing sceneario...
	setBackgroundMap (__BATTLESHIP_TRANSPARENTMAPID__); 

	setMap (_backgroundMapId);

	_liveLaunched = false;

	BATTLESHIP::TimeLimitScene::initialize ();
}

// ---
void BATTLESHIP::TimeLimitSceneExample::finalize ()
{
	_backgroundMapId = -1; // To be selected again when started back...

	if (_liveLaunched)
		removeActionBlock (100000);

	BATTLESHIP::TimeLimitScene::finalize ();
}

// ---
void BATTLESHIP::TimeLimitSceneExample::processEvent (const QGAMES::Event& evnt)
{
	BATTLESHIP::TimeLimitScene::processEvent (evnt);

	if (evnt.code () == __BATTLESHIP_SPACEELEMENTDESTROYED__ && !_liveLaunched)
	{
		_liveLaunched = true;

		launchLive ();
	}
}

// ---
void BATTLESHIP::TimeLimitSceneExample::launchLive ()
{
	addActionBlock (new BATTLESHIP::StdLiveSceneActionBlock (100000, 
		new BATTLESHIP::StdLiveSceneActionBlock::Properties (__BD 0, 100, true, __BD 0, false, 
			false, QGAMES::Position::_noPoint, QGAMES::Rectangle::_noRectangle, 
			-1, BATTLESHIP::LiveToCatch::_LIVETOCATCH, 0, __BD 1)));
}

// ---
void BATTLESHIP::SequentialActionBlocksSceneExample::initialize ()
{
	setBackgroundMap (__BATTLESHIP_TRANSPARENTMAPID__); 

	setMap (__BATTLESHIP_COMPLEXSTARTSMOVINGBACKGROUNDMAPBASEID__);

	BATTLESHIP::SequentialActionBlocksScene::initialize ();
}
