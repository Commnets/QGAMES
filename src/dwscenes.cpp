#include <DragonwindLike/dwscenes.hpp>
#include <DragonwindLike/dwactionblocks.hpp>
#include <DragonwindLike/dwgame.hpp>
#include <DragonwindLike/dwbuilders.hpp>
#include <DragonwindLike/dwmaps.hpp>

// ---
bool DRAGONWIND::Scene::fundamental () const
{
	return (existsProperty (__DRAGONWIND_SCENEATTRFUNDAMENTAL__) 
		? QGAMES::toUpper (QGAMES::trim (property (__DRAGONWIND_SCENEATTRFUNDAMENTAL__))) == std::string (__YES_STRING__) 
		: false);
}

// ---
std::string DRAGONWIND::Scene::name () const
{
	return (existsProperty (__DRAGONWIND_SCENEATTRNAME__) 
		? property (__DRAGONWIND_SCENEATTRNAME__) : __DRAGONWIND_SCENEDEFATTRNAME__);
}

// ---
QGAMES::Vector DRAGONWIND::Scene::movement () const
{
	QGAMES::Vector result = __DRAGONWIND_SCENEDEFATTRMOVEMENT__;
	if (existsProperty (__DRAGONWIND_SCENEATTRMOVEMENT__))
	{
		std::vector <int> vD = QGAMES::getElementsFromAsInt (property (__DRAGONWIND_SCENEATTRMOVEMENT__), ',');
		assert (vD.size () == 3); // Just three no more no less...
		result = QGAMES::Vector (__BD vD [0], __BD vD [1], __BD vD [2]);
	}

	return (result);
}

// ---
QGAMES::bdata DRAGONWIND::Scene::liquidsFluidity () const
{
	return (existsProperty (__DRAGONWIND_SCENEATTRLIQUIDSFLUIDITY__) 
		? __BD (std::atof (property (__DRAGONWIND_SCENEATTRLIQUIDSFLUIDITY__).c_str ())) 
		: __DRAGONWIND_SCENEDEFATTRLIQUIDSFLUIDITY__);
}

// ---
bool DRAGONWIND::PlayingScene::isThingVisible (int t)
{
	bool result = false;
	for (int i = 0; 
		 i < (_numberOfThingsToCatchAdded + __DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__) && !result; i++)
	{
		DRAGONWIND::Thing* eC = dynamic_cast <DRAGONWIND::Thing*> 
			(_characteresAdded [__QGAMES_PTSOMETHINGTOCATCHBASEID__ + i]);
		if (eC && eC -> isVisible () && (eC -> definition () -> _type == eC -> thingTypeAssociatedToAspect (t)))
			result = true;
	}

	return (result);
}

// ---
void DRAGONWIND::PlayingScene::initialize ()
{
	DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
	assert (dG);

	// Adds the ninja...
	_ninja = dynamic_cast <DRAGONWIND::Ninja*> 
		(dG -> character (__DRAGONWIND_MAINARTISTBASEID__ + dG -> artistType ()));
	assert (_ninja); // Just in case...
	addCharacter (_ninja);
	_ninja -> setMap (activeMap ());

	_ninja -> setVisible (true);

	// Here all, the initialization...
	// The scene blocks are also initialized here (if they were defined in the conf file)
	DRAGONWIND::Scene::initialize ();

	// ..additionally several empty things to catch have to be added
	// This is because the full pocket that the main artist is carrying could be left in this scene
	// and there should be entities to hols the element...
	for (int i = _numberOfThingsToCatchAdded; 
		i < (_numberOfThingsToCatchAdded + __DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__); i++)
	{
		assert (i < __QGAMES_PTNUMBERSOMETHINGTOCATCH__); // Just in case...
		QGAMES::SomethingToCatch* sC = dynamic_cast <QGAMES::SomethingToCatch*> 
			(dG -> character (__QGAMES_PTSOMETHINGTOCATCHBASEID__ + i));
		assert (sC); // Just in case...
		addCharacter (sC);
		sC -> setFromNoDefinition (); // No definition needed as it is an empty entity so far...
		sC -> setMap (activeMap ());

		sC -> setVisible (false); // ...and obviously it is not visible...

		_characteresAdded [sC -> id ()] = sC; // To simplify unplug later...
	}

	// Adds all the shootings needed to the scene, to be possible to observe them and manage them later
	for (int i = 0; i < (_numberOfBadGuysAdded + 1) /** Plus the ninja. */; i++)
	{
		int sthBID = __DRAGONWIND_SHOOTINGBASEID__ + (i * __DRAGONWIND_MAXNUMBEROFSHOOTINGSPERCHR__);
		QGAMES::Characters sths = game () -> characters (sthBID, sthBID + __DRAGONWIND_MAXNUMBEROFSHOOTINGSPERCHR__);
		for (QGAMES::Characters::const_iterator j = sths.begin (); j != sths.end (); j++)
		{
			DRAGONWIND::Shooting* sth = dynamic_cast <DRAGONWIND::Shooting*> ((*j).second);
			assert (sth); // Just to be sure...
			addCharacter (sth);
			sth -> setFromNoDefinition ();
			sth -> setMap (activeMap ()); 

			sth -> setVisible (false);

			_characteresAdded [sth -> id ()] = sth; // To simplify unplug later...
		}
	}	

	// Load the things to shoot for the ninja and also for the bad guys...
	_ninja -> loadShootings (dG -> entities 
		(__DRAGONWIND_SHOOTINGBASEID__, __DRAGONWIND_SHOOTINGBASEID__ + __DRAGONWIND_MAXNUMBEROFSHOOTINGSPERCHR__));
	for (int i = 0; i < _numberOfBadGuysAdded; i++)
	{
		DRAGONWIND::BadGuy* bG = 
			dynamic_cast <DRAGONWIND::BadGuy*> (dG -> character (__QGAMES_PTBADGUYBASEID__ + i));
		assert (bG);

		int bID = __DRAGONWIND_SHOOTINGBASEID__ + ((i + 1) * __DRAGONWIND_MAXNUMBEROFSHOOTINGSPERCHR__);
		assert ((bID + __DRAGONWIND_MAXNUMBEROFSHOOTINGSPERCHR__) <= (__DRAGONWIND_SHOOTINGBASEID__ + __DRAGONWIND_MAXNUMBEROFSHOOTINGS__)); // Just in case...
		bG -> loadShootings (dG -> entities (bID, bID + __DRAGONWIND_MAXNUMBEROFSHOOTINGSPERCHR__)); 
	}

	// Assign (if any) the right action block to the bad guy...
	for (int i = 0; i < _numberOfBadGuysAdded; i++)
	{
		DRAGONWIND::BadGuy* bG = 
			dynamic_cast <DRAGONWIND::BadGuy*> (dG -> character (__QGAMES_PTBADGUYBASEID__ + i));
		assert (bG);

		if (bG -> behaviour () -> _automaticMovement != -1) // Means none...
		{
			DRAGONWIND::BadGuysActionBlock* aBlk = 
				dynamic_cast <DRAGONWIND::BadGuysActionBlock*> (actionBlock (bG -> behaviour () -> _automaticMovement));
			assert (aBlk); // it has to be...
			aBlk -> setBadGuy (bG);
		}
	}

	// The background map is initialized up and it automatically moves,
	// But in a playing scene the background doesn't move alone ever
	if (backgroundMap () != NULL)
		backgroundMap () -> stop (); // Don't move it...

	// Sets the background music if it has been defined in the configuration file, 
	// ..and it has been also allowed...
	_backgroundSound = -1;
	if (((DRAGONWIND::Game::Conf*) dG -> configuration ()) -> musicOn () &&
		existsProperty (__DRAGONWIND_SCENEATTRBACKMUSIC__))
	{
		_backgroundSound = std::atoi (property (__DRAGONWIND_SCENEATTRBACKMUSIC__).c_str ()); 
		dG -> sound (_backgroundSound) -> play (__QGAMES_BACKGROUNDSOUNDCHANNEL__);
	}

	adaptSceneToDifficultyLevel ();
}

// ---
void DRAGONWIND::PlayingScene::finalize ()
{
	// Clear the shootings...
	_ninja -> clearShootings ();
	for (int i = 0; i < _numberOfBadGuysAdded; i++)
	{
		DRAGONWIND::BadGuy* bG = dynamic_cast <DRAGONWIND::BadGuy*> 
			(_characteres [__QGAMES_PTBADGUYBASEID__ + i]);
		assert (bG);

		bG -> clearShootings ();
	}

	DRAGONWIND::Scene::finalize ();
	// The additional characters added to reflect the things being carried are also removed with this instruction...
	// The shootings also added are also removed after the execution of this instruction...

	_ninja -> setVisible (false);

	removeCharacter (_ninja);

	_ninja = NULL;

	if (_backgroundSound != -1 && game () -> sound (_backgroundSound) -> isPlaying ())
		game () -> sound (_backgroundSound) -> stop ();
}

// ---
void DRAGONWIND::PlayingScene::processEvent (const QGAMES::Event& evnt)
{
	switch (evnt.code ())
	{
		// A thing is trying to be left in this scene...
		case __DRAGONWIND_THINGLEFTEVENT__: 
			{
				DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
				assert (dG); // Just in case...

				// Let's find an not visible thing...(there should be at least one)
				DRAGONWIND::Thing* eC = NULL;
				for (int i = 0; 
						i < (_numberOfThingsToCatchAdded + __DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__) && !eC; i++)
					if (!_characteresAdded [__QGAMES_PTSOMETHINGTOCATCHBASEID__ + i] -> isVisible ())
						eC = dynamic_cast <DRAGONWIND::Thing*> 
							(_characteresAdded [__QGAMES_PTSOMETHINGTOCATCHBASEID__ + i]);
				assert (eC); // If the game is well built, this can't be null...

				// Creates a definition object for the things that is going to be left...
				QGAMES::Positions pos; pos.push_back (_ninja -> centerPosition ());
				QGAMES::SetOfOpenValues oVs = evnt.values ();
				QGAMES::PlatformGame::ThingToCatchDefinition* def = 
					dG -> addThingToCatch (world () -> id (), id (), 
						evnt.values ().openValue (__QGAMES_POCKETATTRIBUTEID__).intValue (),
						eC -> thingTypeAssociatedToAspect (evnt.values ().openValue (__QGAMES_POCKETATTRIBUTETYPE__).intValue ()),
						pos, -1, oVs.setOfOpenValues (0) /** The rest */);

				// The entity in the scene is now set with that definition
				eC -> setFromDefinition (def);
				eC -> setVisible (true); // ...and becomes visible again...
			}

			break;

		// Something has explode in the scene...
		case __DRAGONWIND_SHOOTINGTOEXPLODE__:
			explosionAround ((DRAGONWIND::Shooting*) evnt.data (), __BD 100);
			break;

		// A character has died in the scene
		case __DRAGONWIND_CHARACTERHASDIED__:
			{
				DRAGONWIND::Character* chr = ((DRAGONWIND::Character*) evnt.data ());
				DRAGONWIND::Character* hchr = chr -> whoHitTheMost (); // Set the points to the one who hit it the most if any
				if (hchr != NULL) // It could be when the character dies and nobody hit him/her before
					hchr -> setScore (hchr -> score () + chr -> behaviour () -> _pointsGivenWhenDie);
			}

			break;

		// The main artist has collisioned with a notification tile in the scene
		case __QGAMES_COLLISIONWITHTILE__:
			{
				// It is supposed all communications comes from this type of handler (@see ptartist.cpp)
				QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::NotificationInfo* dt =
					(QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::NotificationInfo*) evnt.data ();
				if (!dt -> _detectionState -> _used) // Never used before...
					dt -> _detectionState -> _used = treatCollisionWithNotificationTile 
						(dynamic_cast <DRAGONWIND::Character*> (dt -> _owner), 
							dt -> _tile -> numberFrame (), dynamic_cast <QGAMES::NotificationTile*> (dt -> _tile));
			}

			break;
	
		default:
			DRAGONWIND::Scene::processEvent (evnt);
	}
}

// ---
const DRAGONWIND::BadGuy* DRAGONWIND::PlayingScene::firstBadGuyType (int t) const
{
	const DRAGONWIND::BadGuy* result = NULL;
	for (QGAMES::Entities::const_iterator i = badGuys ().begin ();
			i != badGuys ().end () && !result; i++)
	{
		const DRAGONWIND::BadGuy* bG = dynamic_cast <const DRAGONWIND::BadGuy*> ((*i).second);
		if (bG != NULL && bG -> definition () -> _type == t)
			result = bG;
	}

	return (result);
}

// ---
QGAMES::Entities DRAGONWIND::PlayingScene::badGuysType (int t) const
{
	QGAMES::Entities result;
	for (QGAMES::Entities::const_iterator i = badGuys ().begin ();
			i != badGuys ().end (); i++)
	{
		DRAGONWIND::BadGuy* bG = dynamic_cast <DRAGONWIND::BadGuy*> ((*i).second);
		if (bG != NULL && bG -> definition () -> _type == t)
			result [bG -> id ()] = bG;
	}

	return (result);
}

// ---
const DRAGONWIND::BadGuysActionBlock* DRAGONWIND::PlayingScene::actionBlockControlling 
	(DRAGONWIND::BadGuy* bG) const
{
	assert (bG);

	DRAGONWIND::BadGuysActionBlock* result = NULL;
	for (QGAMES::SceneActionBlocks::const_iterator i = actionBlocks ().begin ();
			i != actionBlocks ().end () && !result; i++)
	{
		DRAGONWIND::BadGuysActionBlock* aB = dynamic_cast <DRAGONWIND::BadGuysActionBlock*> ((*i));
		if (aB && aB -> badGuy () == bG)
			result = aB;
	}

	return (result);
}

// ---
QGAMES::PlatformGame::ThingToCatchDefinition* DRAGONWIND::PlayingScene::createThingToCatchDefinitionFrom 
	(const QGAMES::PlatformMap::ObjectLocation& obj) const
{
	QGAMES::PlatformGame::ThingToCatchDefinition* result = 
		DRAGONWIND::Scene::createThingToCatchDefinitionFrom (obj);
	assert (result); // Just in case...

	const DRAGONWIND::Game::Conf* dGCfg = 
		dynamic_cast <const DRAGONWIND::Game::Conf*> (game () -> configuration ());
	assert (dGCfg); // Just in case...

	QGAMES::SetOfOpenValues oV;
	// The shootings available are reduce by 5 per difficulty level (0 in basic level) up to a maximum of 10
	if (result -> _type == __DRAGONWIND_TILEDEFININGARROW__) 
		oV.addOpenValue (__DRAGONWIND_GUNSTOCATCHNUMBERSHOOTS__, 
			QGAMES::OpenValue (50 - ((dGCfg -> difficulty () < 3) ? (dGCfg ->  difficulty () * 5) : 40)));
	else if (result -> _type == __DRAGONWIND_TILEDEFININGSTAR__) 
		oV.addOpenValue (__DRAGONWIND_GUNSTOCATCHNUMBERSHOOTS__, 
			QGAMES::OpenValue (50 - ((dGCfg -> difficulty () < 3) ? (dGCfg ->  difficulty () * 5) : 40)));
	else if (result -> _type == __DRAGONWIND_TILEDEFININGBOMB__) 
		oV.addOpenValue (__DRAGONWIND_GUNSTOCATCHNUMBERSHOOTS__, 
			QGAMES::OpenValue (6 - ((dGCfg -> difficulty () < 3) ? (dGCfg ->  difficulty () * 5) : 40)));
	// The number of lives available per heart are incremented by 1 per difficulty level (0 in minimum difficulty)
	// Up to a maximum of three!
	else if (result -> _type == __DRAGONWIND_TILEDEFININGHEART__)
		oV.addOpenValue (__DRAGONWIND_HEARTTOCATCHNUMBERLIVES__, 
			QGAMES::OpenValue (1 + ((dGCfg -> difficulty () < 3) ? dGCfg -> difficulty () : 2)));

	result -> _otherValues = oV; // Can be empty like when entried...

	return (result);
}

// ---
void DRAGONWIND::PlayingScene::explosionAround (DRAGONWIND::Shooting* sth, QGAMES::bdata rdx)
{
	assert (sth);

	for (QGAMES::Entities::const_iterator i = characters ().begin (); i != characters ().end (); i++)
	{
		DRAGONWIND::Character* chr = dynamic_cast <DRAGONWIND::Character*> ((*i).second);
		if (chr && // Something that can move...
			chr -> isVisible () && chr -> isAlive () && // Alive and visible first...
			chr -> centerPosition ().distanceTo (sth -> centerPosition ()) < rdx && // ...Near
			((dynamic_cast <DRAGONWIND::BadGuy*> (chr) && sth -> owner () != chr) || !dynamic_cast <DRAGONWIND::BadGuy*> (chr)) && // ...If the bomb is not mine in case of being a bad guy...
			chr -> behaviour () -> _canBeHurmedByEnemies) // ...Only if enemies can hurm you (bomb is an enemy)
			chr -> setEnergyToAdd (-chr -> behaviour () -> _energyLostPerExplosion);
	}
}

// ---
bool DRAGONWIND::PlayingScene::treatCollisionWithNotificationTile (DRAGONWIND::Character* a, 
	int nF, QGAMES::NotificationTile* t)
{
	bool result = false;

	DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
	assert (dG); // Just in case...

	// Only when who collisions is the main artist...
	if (a == dG -> mainArtist ())
	{
		DRAGONWIND::ToKnowTileFrameInformation* tInf = 
			dynamic_cast <DRAGONWIND::ToKnowTileFrameInformation*> (dG -> tileFrameInformation ());
		assert (tInf); // Just in case...

		// Has the main artist reached a main goal?
		if (tInf -> isTileConnectionGoalLocation (nF))
		{
			DRAGONWIND::Ninja* nj = dynamic_cast <DRAGONWIND::Ninja*> (a);
			assert (a); // Just in case...
			// This method only "takes note" that the ninja is about to reach a goal
			// No change of state is made in now. 
			// This method has been executed when a collision with a "notification tile" was produced,
			// So, likely there are more layers to check. Things like layers behind, etc... are not sure yet.
			nj -> toReachGoal (nF);

			result = true; // It has been weel treated...
		}
		else

		// The tiles refering to new initial points reached!!
		if (tInf -> isTileIntermediateGoalLocation (nF))
		{
			// Sets a new init point for this scene for the current user...
			dG -> setCurrentInitPoint (id (), tInf -> initPointAssociatedToIntermediateGoalTileFrame (nF));
			// The sound is associated to the ninja (in this case)
			game () -> sound (__DRAGONWIND_IMPORTANTEVENTSOUNDID__) -> play (__QGAMES_GAMESOUNDCHANNEL__); 

			result = true; // It's been well treated too
		}
		else

		// The tiles refering to active action blocks...
		if (tInf -> isTileActionBlockLocation (nF))
		{
			// Onlyh when the attributes to identify the block have been defined, 
			// the notification tile has effect...
			if (t -> parameters ().existOpenValue (__DRAGONWIND_NOTIFICATIONTILEBATTRLOCKID__) && 
				t -> parameters ().existOpenValue (__DRAGONWIND_NOTIFICATIONTILEATTRACTIVE__)) 
			{
				QGAMES::SceneActionBlock* aB = actionBlock 
					(std::stoi (t -> parameters ().openValue (__DRAGONWIND_NOTIFICATIONTILEBATTRLOCKID__).strValue ()));
				assert (aB); // It has to exist...
				aB -> setActive (t -> parameters ().openValue (__DRAGONWIND_NOTIFICATIONTILEATTRACTIVE__).strValue () == std::string (__YES_STRING__));

				game () -> sound (__DRAGONWIND_NINJAACTIVATESWITCHSOUNDID__) -> play (__QGAMES_GAMESOUNDCHANNEL__);

				result = true; // Ok
			}
		}
	}

	return (result);
}

// ---
void DRAGONWIND::PlayingScene::adaptSceneToDifficultyLevel ()
{
	DRAGONWIND::Game::Conf* dGCfg = dynamic_cast <DRAGONWIND::Game::Conf*> (game () -> configuration ());
	assert (dGCfg);

	int dL = (dGCfg -> difficulty () < 3) ? dGCfg -> difficulty () : 2;;

	// Something common related with all playing scenes is to change a couple of parameters
	// related with how bad guys use the energy!
	// The energy lost per explosion is less (they are stronger)
	// The energy lost per cycle is a little less (they are stronger)
	// ...sand so on and so for...
	for (QGAMES::Entities::const_iterator i = _badGuys.begin (); i != _badGuys.end (); i++)
	{
		DRAGONWIND::Character::Behaviour* bhv = 
			((DRAGONWIND::BadGuy*) (*i).second) -> behaviour ();
		if (bhv)
		{
			bhv -> _energyLostPerExplosion /= __BD 1 + (__BD dL / __BD 10);
			bhv -> _energyLostPerLoop /= __BD 1 + (__BD dL / __BD 10);
			bhv -> _shootingEnergy *= (int) (__BD 1 + (__BD dL / __BD 10));
			bhv -> _weakness /= __BD 1 + (__BD dL / __BD 10);
		}
	}

	// With the ninja happens more or less the other way around...
	DRAGONWIND::Character::Behaviour* bhv = _ninja -> behaviour ();
	assert (bhv);
	if (bhv)
	{
		bhv -> _energyLostPerExplosion *= __BD 1 + (__BD dL / __BD 10);
		bhv -> _energyLostPerLoop *= __BD 1 + (__BD dL / __BD 10);
		bhv -> _shootingEnergy /= (int) (__BD 1 + (__BD dL / __BD 10));
		bhv -> _weakness /= __BD 1 + (__BD dL / __BD 10);
	}

	// An now invoke the adjustments need over the action blocks...
	for (QGAMES::SceneActionBlocks::const_iterator i = actionBlocks ().begin (); 
			i != actionBlocks ().end (); i++)
	{
		DRAGONWIND::SceneActionBlock* aB = dynamic_cast <DRAGONWIND::SceneActionBlock*> ((*i));
		if (aB)
			aB -> adaptSceneToDifficultyLevel ();
	}
}

// ---
void DRAGONWIND::SceneExample::initialize ()
{
	if (_backgroundMapId == -1)
		_backgroundMapId = __DRAGONWIND_BACKGROUNDMAPSBASEID__ + 
			((rand () % __DRAGONWIND_NUMBERBACKGROUNDMAPS__) * 10); // It is choosen randomly first time only...
	setBackgroundMap (_backgroundMapId);

	setMap (__DRAGONWIND_EXAMPLEMAPID__);

	_wall = dynamic_cast <QGAMES::AdvancedTileLayer*> ((__TM activeMap ()) -> layer (std::string ("Solid_2")));
	assert (_wall);
	_wall -> setVisible (true);

	reStartAllOnOffSwitches ();
	reStartAllCounters ();

	DRAGONWIND::PlayingScene::initialize ();
}

// ---
void DRAGONWIND::SceneExample::updatePositions ()
{
	DRAGONWIND::PlayingScene::updatePositions ();

	if (counter (_COUNTERTOHIDEWALL) -> isEnd () && _wall -> isVisible ())
		_wall -> setVisible (false);

	// Put the messages if needed...
	if (!onOffSwitch (_SWITCHMESSAGES) -> isOn ())
	{
		DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
		std::string atName ((dG -> artistType () == 0) ? "Daiki" : "Akira");
		((DRAGONWIND::Ninja*) character (__DRAGONWIND_MAINARTISTBASEID__ + dG -> artistType ())) -> toSay
			(std::string ("My name is   ") + atName + std::string (", and   you already  know my story"),
				DRAGONWIND::DragonArtist::DialogProperties (__QGAMES_COURIER10WHITELETTERS__, 5, 13, 
					__QGAMES_SHADOWCOLOR__, __QGAMES_WHITECOLOR__, 255, __BD 1.5, 
						QGAMES::Vector (__BD -100, __BD -100, __BD 0)));
		((DRAGONWIND::Ninja*) character ( __DRAGONWIND_MAINARTISTBASEID__ + dG -> artistType ())) -> toSay
			(std::string ("Now take me  to goal!.    Move the con-trols quick."),
				DRAGONWIND::DragonArtist::DialogProperties (__QGAMES_COURIER10WHITELETTERS__, 5, 13, 
					__QGAMES_SHADOWCOLOR__, __QGAMES_WHITECOLOR__, 255, __BD 1.5, 
						QGAMES::Vector (__BD -100, __BD -100, __BD 0)));
		((DRAGONWIND::Ninja*) character (__DRAGONWIND_MAINARTISTBASEID__ + dG -> artistType ())) -> toSay
			(std::string ("Let's rescue   together my    father's ashes!"),
				DRAGONWIND::DragonArtist::DialogProperties (__QGAMES_COURIER10WHITELETTERS__, 5, 15, 
					__QGAMES_SHADOWCOLOR__, __QGAMES_WHITECOLOR__, 255, __BD 1.5, 
						QGAMES::Vector (__BD -100, __BD -100, __BD 0)));

		// Never again!
		onOffSwitch (_SWITCHMESSAGES) -> set (true);
	}
}

// ---
void DRAGONWIND::SceneExample::finalize ()
{
	DRAGONWIND::PlayingScene::finalize ();

	_wall = NULL;
}

// ---
__IMPLEMENTONOFFSWITCHES__ (DRAGONWIND::SceneExample::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (DRAGONWIND::SceneExample::_SWITCHMESSAGES, false));
}

// ---
__IMPLEMENTCOUNTERS__ (DRAGONWIND::SceneExample::Counters)
{
	addCounter (new QGAMES::Counter (DRAGONWIND::SceneExample::_COUNTERTOHIDEWALL, 240, 0, true, false));
}

// ---
void DRAGONWIND::SceneExampleNext::initialize ()
{
	if (_backgroundMapId == -1)
		_backgroundMapId = __DRAGONWIND_BACKGROUNDMAPSBASEID__ + 
			((rand () % __DRAGONWIND_NUMBERBACKGROUNDMAPS__) * 10); // It is choosen randomly first time only...
	setBackgroundMap (_backgroundMapId);

	setMap (__DRAGONWIND_EXAMPLEMAPID__ + 1);

	DRAGONWIND::PlayingScene::initialize ();
}
