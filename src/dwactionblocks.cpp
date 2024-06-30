#include <DragonwindLike/dwactionblocks.hpp>
#include <DragonwindLike/dwgame.hpp>
#include <iterator>

// ---
void DRAGONWIND::SceneActionBlock::initialize ()
{
	QGAMES::SceneActionBlock::initialize ();

	setActive (initialActive ());
}

// ---
DRAGONWIND::FlyingMonstersActionBlock::Properties::Properties (const std::map <std::string, std::string>& dt)
	: _typeOfMonster (__DRAGONWIND_ACTIONBLOCKFMONSTERTYPEBEE__),
	  _timeToAppear (__BD 2)
{
	if (dt.find (std::string (__DRAGONWIND_ACTIONBLOCKFMONSTERATTRTYPE__)) != dt.end ())
		_typeOfMonster = std::atoi ((*dt.find (__DRAGONWIND_ACTIONBLOCKFMONSTERATTRTYPE__)).second.c_str ());
	if (dt.find (std::string (__DRAGONWIND_ACTIONBLOCKFMONSTERATTRTIME__)) != dt.end ())
		_timeToAppear = __BD (std::atof ((*dt.find (__DRAGONWIND_ACTIONBLOCKFMONSTERATTRTIME__)).second.c_str ()));
}

// ---
QGAMES::SetOfOpenValues DRAGONWIND::FlyingMonstersActionBlock::Properties::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));

	result.addOpenValue (0, QGAMES::OpenValue (_typeOfMonster));
	result.addOpenValue (1, QGAMES::OpenValue (_timeToAppear));

	return (result);
}

// ---
void DRAGONWIND::FlyingMonstersActionBlock::Properties::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));
	assert (oV.existOpenValue (0) && oV.existOpenValue (1));

	_typeOfMonster = oV.openValue (0).intValue ();
	_timeToAppear = oV.openValue (1).bdataValue ();
}

// ---
DRAGONWIND::FlyingMonstersActionBlock::FlyingMonstersActionBlock 
		(int id, DRAGONWIND::FlyingMonstersActionBlock::Properties* prps)
	: DRAGONWIND::SceneActionBlock (id),
	  _properties (prps),
	  _monsters (NULL)
{
	assert (_properties);

	addBuoy (_FINALIZEFLOCKBUOY, new DRAGONWIND::FlyingMonstersActionBlock::ToFinalizeFlock ());
}

// ---
QGAMES::SetOfOpenValues DRAGONWIND::FlyingMonstersActionBlock::runtimeValues () const
{
	assert (_properties);
	// The flock can be null...

	QGAMES::SetOfOpenValues result = DRAGONWIND::SceneActionBlock::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	// Adds the information of the properties of the block
	// They usually don't change ever, but just in case...
	result.addSetOfOpenValues (lNE + 1, _properties -> asSetOfOpenValues ());

	// Adds the id of the flock if any, and __MININT__ otherwise
	result.addOpenValue (lE + 1, _monsters ? _monsters -> id () : __MININT__);
	// Adds the status of the flock if any, and empty otherwise
	result.addSetOfOpenValues (lNE + 2, _monsters 
		? _monsters -> runtimeValues () 
		: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__)));

	return (result);
}

// ---
void DRAGONWIND::FlyingMonstersActionBlock::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	assert (_properties);

	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existOpenValue (lE) &&
			cfg.existSetOfOpenValues (lNE) && cfg.existSetOfOpenValues (lNE - 1));

	QGAMES::SetOfOpenValues cCfg = cfg;

	int fID = cCfg.openValue (lE).intValue ();
	if (fID != __MININT__)
	{
		_monsters = dynamic_cast <DRAGONWIND::FlockOfMonsters*> 
			(game () -> entityBuilder () -> entity (fID)); // The builder has to exist...

		_monsters -> initialize ();

		observe (_monsters);

		scene () -> addEntity (_monsters);
		// The monster is initialized and added to the scene...
	}

	cCfg.removeOpenValue (lE);

	//... and now its values changes!!
	// The monster variable might not exist first time the inter values were saved
	// So, at this point it is needed to verify that there are valid intervalues values to set up the monster!
	QGAMES::SetOfOpenValues mVV = cCfg.setOfOpenValues (lNE);
	assert (mVV.name () == std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__));
	if (_monsters && mVV.numberValues () != 0)
		_monsters -> initializeRuntimeValuesFrom (mVV);
	cCfg.removeSetOfOpenValues (lNE);

	_properties -> fromSetOfOpenValues (cCfg.setOfOpenValues (lNE - 1));
	cCfg.removeSetOfOpenValues (lNE - 1);

	DRAGONWIND::SceneActionBlock::initializeRuntimeValuesFrom (cCfg);
}

// ---
void DRAGONWIND::FlyingMonstersActionBlock::setActive (bool a)
{
	DRAGONWIND::SceneActionBlock::setActive (a);

	if (!scene ())
		return; // Nothing can be done without scene...
	// When the block is being created, this method is invoked but nothing can be done until scene is set...

	// When the block is disactivated, the flock has to disaapear from the screen...
	if (!isActive ()) finalize ();
	// Never invoke initialize here because a recursive invocation will be set...
	// DRAGONWIND::SceneBlockAction call setActive back!
}

// ---
void DRAGONWIND::FlyingMonstersActionBlock::initialize ()
{
	DRAGONWIND::SceneActionBlock::initialize ();

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	counter (_COUNTERTOAPPEAR) -> initialize 
		((int) (_properties -> _timeToAppear * __BD QGAMES::Game::game () -> framesPerSecond ()), 0, true, true);
}

// ---
void DRAGONWIND::FlyingMonstersActionBlock::updatePositions ()
{
	DRAGONWIND::SceneActionBlock::updatePositions ();

	assert (scene ());

	// 2 posibilities for the monsters to appear...
	// 1.- They are not in the screen, but counter for them to appear is over...
	// 2.- They should be at the screen but the _monsters pointer is null...
	// This last situation shouldn't happen, but there could be a mistake in the code...
	if ((!onOffSwitch (_MONSTERSONSCREEN) -> isOn () && counter (_COUNTERTOAPPEAR) -> isEnd ()) || 
		(onOffSwitch (_MONSTERSONSCREEN) -> isOn () && !_monsters)) 
	{
		onOffSwitch (_MONSTERSONSCREEN) -> set (true); // Any way...

		initializeFlock ();
	}
}

// ---
void DRAGONWIND::FlyingMonstersActionBlock::finalize ()
{
	DRAGONWIND::SceneActionBlock::finalize ();

	assert (scene ());

	if (_monsters)
		finalizeFlock ();
}

// ---
void DRAGONWIND::FlyingMonstersActionBlock::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __DRAGONWIND_FLOCKKILLED__)
	{
		assert (evnt.data () == (void*) _monsters); // It shouldn't happen but...

		buoy (_FINALIZEFLOCKBUOY) -> active (true);
	}
	else
		QGAMES::SceneActionBlock::processEvent (evnt);
}

// ---
DRAGONWIND::FlockOfMonsters* DRAGONWIND::FlyingMonstersActionBlock::flock ()
{
	DRAGONWIND::FlockOfMonsters* result = NULL;

	DRAGONWIND::Game::Conf* cfg = dynamic_cast <DRAGONWIND::Game::Conf*> (game () -> configuration ());
	assert (cfg);
	assert (cfg -> difficulty () < 3); // Easy, Medium, Difficult

	switch (_properties -> _typeOfMonster)
	{
		case __DRAGONWIND_ACTIONBLOCKFMONSTERTYPEBEE__: // Bees...
			result = dynamic_cast <DRAGONWIND::FlockOfMonsters*> 
				(game () -> entity (__DRAGONWIND_FLOACKOFBEESBASE__ + (cfg -> difficulty () << 1) + (rand () % 2)));
			// Two versions of the flock per difficulty level...
			break;

		case __DRAGONWIND_ACTIONBLOCKFMONSTERTYPEBIRD__: // Birds...
			result = dynamic_cast <DRAGONWIND::FlockOfMonsters*> 
				(game () -> entity (__DRAGONWIND_FLOACKOFBIRDSBASE__ + (cfg -> difficulty () << 1) + (rand () % 2)));
			// Two versions of the flock per difficulty level...
			break;

		default:
			assert (0); // It should be here...
			break;
	}

	return (result);
}

// ---
void DRAGONWIND::FlyingMonstersActionBlock::initializeFlock ()
{
	if (_monsters)
		finalizeFlock (); // Just in case...

	_monsters = flock ();
	assert (_monsters);

	_monsters -> initialize ();

	observe (_monsters); // To take care about them here...

	scene () -> addEntity (_monsters); // And be observed but there...

	_monsters -> setVisible (true);
}

// ---
void DRAGONWIND::FlyingMonstersActionBlock::finalizeFlock ()
{
	assert (_monsters);

	_monsters -> finalize ();

	_monsters -> setVisible (false);

	unObserve (_monsters);

	scene () -> removeEntity (_monsters);

	_monsters = NULL; // Not more monsters till the next iteration...
}

// ---
__IMPLEMENTCOUNTERS__ (DRAGONWIND::FlyingMonstersActionBlock::Counters)
{
	// It will be redefined when init or when it is used first time
	addCounter (new QGAMES::Counter 
		(DRAGONWIND::FlyingMonstersActionBlock::_COUNTERTOAPPEAR, 
			2 * QGAMES::Game::game () -> framesPerSecond (), 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (DRAGONWIND::FlyingMonstersActionBlock::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (DRAGONWIND::FlyingMonstersActionBlock::_MONSTERSONSCREEN, false));
}

// ---
void* DRAGONWIND::FlyingMonstersActionBlock::ToFinalizeFlock::treatFor (QGAMES::Element* e)
{
	DRAGONWIND::FlyingMonstersActionBlock* fB = dynamic_cast <DRAGONWIND::FlyingMonstersActionBlock*> (e);
	assert (fB); // Just in case...

	fB -> finalizeFlock ();

	fB -> onOffSwitch (DRAGONWIND::FlyingMonstersActionBlock::_MONSTERSONSCREEN) -> set (false); // To start up back...

	return (NULL);
}

// ---
DRAGONWIND::BadGuysActionBlock::Properties::Properties (const std::map <std::string, std::string>& dt)
	: _monitorId (-1), 
	  _toFire (false),
	  _fireType (), // Empty...
	  _fireImpulse (-1),
	  _sToFire (__BD 2), _sBetweenFires (__BD 2), _sFiring (__BD 2)
{
	_fireType.push_back (0); // The default one...

	if (dt.find (std::string (__DRAGONWIND_ACTIONBLOCKMONSTERATTRMOVID__)) != dt.end ())
		_monitorId = std::atoi ((*dt.find (__DRAGONWIND_ACTIONBLOCKMONSTERATTRMOVID__)).second.c_str ());
	if (dt.find (std::string (__DRAGONWIND_ACTIONBLOCKMMONSTERATTRFIRE__)) != dt.end ())
		_toFire = QGAMES::toUpper (std::string ((*dt.find (__DRAGONWIND_ACTIONBLOCKMMONSTERATTRFIRE__)).second)) == std::string (__YES_STRING__);
	if (dt.find (std::string (__DRAGONWIND_ACTIONBLOCKMMONSTERATTRTFIRE__)) != dt.end ())
		_fireType = QGAMES::getElementsFromAsInt 
			(((*dt.find (__DRAGONWIND_ACTIONBLOCKMMONSTERATTRTFIRE__)).second.c_str ()), ',');
	if (dt.find (std::string (__DRAGONWIND_ACTIONBLOCKMMONSTERATTRIFIRE__)) != dt.end ())
		_fireImpulse = std::atoi ((*dt.find (__DRAGONWIND_ACTIONBLOCKMMONSTERATTRIFIRE__)).second.c_str ());
	if (dt.find (std::string (__DRAGONWIND_ACTIONBLOCKMMONSTERATTRSTOF__)) != dt.end ())
		_sToFire = __BD (std::atof ((*dt.find (__DRAGONWIND_ACTIONBLOCKMMONSTERATTRSTOF__)).second.c_str ()));
	if (dt.find (std::string (__DRAGONWIND_ACTIONBLOCKMMONSTERATTRSBTF__)) != dt.end ())
		_sBetweenFires = __BD (std::atof ((*dt.find (__DRAGONWIND_ACTIONBLOCKMMONSTERATTRSBTF__)).second.c_str ()));
	if (dt.find (std::string (__DRAGONWIND_ACTIONBLOCKMMONSTERATTRSFIR__)) != dt.end ())
		_sFiring = __BD (std::atof ((*dt.find (__DRAGONWIND_ACTIONBLOCKMMONSTERATTRSFIR__)).second.c_str ()));

	if ((int) _fireType.size () == 0)
		_fireType = std::vector <int> (0, 0); // Minimum one...
}

// ---
QGAMES::SetOfOpenValues DRAGONWIND::BadGuysActionBlock::Properties::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));

	result.addOpenValue (0, QGAMES::OpenValue (_monitorId));
	result.addOpenValue (1, QGAMES::OpenValue (_toFire));
	std::string fTStr = std::to_string (_fireType [0]);
	for (int i = 1; i < (int) _fireType.size (); 
			fTStr += std::string (",") + std::to_string (_fireType [i++]));
	result.addOpenValue (2, QGAMES::OpenValue (fTStr));
	result.addOpenValue (3, QGAMES::OpenValue (_fireImpulse));
	result.addOpenValue (4, QGAMES::OpenValue (_sToFire));
	result.addOpenValue (5, QGAMES::OpenValue (_sBetweenFires));
	result.addOpenValue (6, QGAMES::OpenValue (_sFiring));

	return (result);
}

// ---
void DRAGONWIND::BadGuysActionBlock::Properties::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));
	assert (oV.existOpenValue (0) && oV.existOpenValue (1) &&
			oV.existOpenValue (2) && oV.existOpenValue (3) &&
			oV.existOpenValue (4) && oV.existOpenValue (5) &&
			oV.existOpenValue (6));

	_monitorId = oV.openValue (0).intValue ();
	_toFire = oV.openValue (1).boolValue ();
	_fireType = QGAMES::getElementsFromAsInt (oV.openValue (2).strValue (),',');
	_fireImpulse = oV.openValue (3).intValue ();
	_sToFire = oV.openValue (4).bdataValue ();
	_sBetweenFires = oV.openValue (5).bdataValue ();
	_sFiring = oV.openValue (6).bdataValue ();
}

// ---
DRAGONWIND::BadGuysActionBlock::BadGuysActionBlock (int id, Properties* prps)
	: DRAGONWIND::SceneActionBlock (id),
	  _properties (prps),
	  _character (NULL)
{ 
	assert (_properties);
}

// ---
QGAMES::SetOfOpenValues DRAGONWIND::BadGuysActionBlock::runtimeValues () const
{
	assert (_properties);

	QGAMES::SetOfOpenValues result = DRAGONWIND::SceneActionBlock::runtimeValues ();

	int lNE = result.lastNestedOpenValueId ();

	// Adds the information of the properties of the block
	// They usually don't change ever, but just in case...
	result.addSetOfOpenValues (lNE + 1, _properties -> asSetOfOpenValues ());

	// Adds the status of the character (if any)
	// The monitor (if it is being moved using one) will be also kept.
	// ...and also the specific status of that monitor
	result.addSetOfOpenValues (lNE + 2, 
		_character ? _character -> runtimeValues () 
			: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__)));

	return (result);
}

// ---
void DRAGONWIND::BadGuysActionBlock::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	assert (_properties);

	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existSetOfOpenValues (lNE) && cfg.existSetOfOpenValues (lNE - 1));

	QGAMES::SetOfOpenValues cCfg = cfg;

	// The character might be added after a first initializacion 
	// So at this point no initial values could exist over the initial ones to add
	// This is the reason to check whether not only the setOfOpenValues exists but also if there are values in it!!
	QGAMES::SetOfOpenValues cVV = cCfg.setOfOpenValues (lNE);
	assert (cVV.name () == std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__));
	if (_character && cVV.numberValues () != 0)
		_character -> initializeRuntimeValuesFrom (cVV);
	cCfg.removeSetOfOpenValues (lNE);

	_properties -> fromSetOfOpenValues (cCfg.setOfOpenValues (lNE - 1));
	cCfg.removeSetOfOpenValues (lNE - 1);

	DRAGONWIND::SceneActionBlock::initializeRuntimeValuesFrom (cCfg);
}

// ---
void DRAGONWIND::BadGuysActionBlock::setBadGuy (DRAGONWIND::BadGuy* bG)
{
	assert (bG);

	_character = bG; 

	// Notice that the bad guy is not observed by this block so far...
}

// ---
void DRAGONWIND::BadGuysActionBlock::initialize ()
{
	DRAGONWIND::SceneActionBlock::initialize ();

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	initCounterToFire ();
	initCounterBetweenFires ();
	initCounterFiring ();
}

// ---
void DRAGONWIND::BadGuysActionBlock::updatePositions ()
{
	DRAGONWIND::SceneActionBlock::updatePositions ();

	assert (_character); // Can't be null here...

	if (!_character -> isAlive ())
		return; // The action block is not valid for who is not living!

	// He is not moving...
	if (!onOffSwitch (_SWITCHTOMOVE) -> isOn ())
	{
		if (_character -> isOnABase () && _character -> isPlatformStateStable ())
		{
			onOffSwitch (_SWITCHTOMOVE) -> set (true); // Ready to move...

			_character -> addControlStepsMonitor 
				(game () -> characterMonitorBuilder () -> monitorFor (_properties -> _monitorId, _character));
		}
	}

	// Now we are sure that the bad guy is already moving,...
	// What it has to be controlled is the possibility to shoot...
	// Only when shooting is something available!
	if (onOffSwitch (_SWITCHTOMOVE) -> isOn () && _properties -> _toFire)
	{
		// Already firing...
		if (onOffSwitch (_SWITCHFIRING) -> isOn ())
		{
			// To fire...
			if (counter (_CONTERBETWEENFIRES) -> isEnd ())
			{
				int fT = (_properties -> _fireType.size () > 1)
					? _properties -> _fireType [rand () % (int) _properties -> _fireType.size ()] 
					: _properties -> _fireType [0];
				_character -> toShoot (
						(_properties -> _fireImpulse < __BD 0) 
							? ((rand () % (-_properties -> _fireImpulse)) + 1) 
							: _properties -> _fireImpulse, false, fT); 

				initCounterBetweenFires ();
			}

			// ..end of the time firing?
			if (counter (_COUNTERFIRING) -> isEnd ()) 
			{
				onOffSwitch (_SWITCHFIRING) -> set (false); // ...not firing any more...

				initCounterFiring ();
			}
		}
		// Not firing already...
		else
		{
			// ...end of the time before firing?
			if (counter (_COUNTERTOFIRE) -> isEnd ())
			{
				onOffSwitch (_SWITCHFIRING) -> set (true); // ...firing again...

				initCounterToFire ();
			}
		}
	}
}

// ---
void DRAGONWIND::BadGuysActionBlock::finalize ()
{
	DRAGONWIND::SceneActionBlock::finalize ();

	assert (_character);

	if (_character)
		_character -> addControlStepsMonitor (NULL); // Just in case...

	_character = NULL; // No needed anymore...
}

// ---
void DRAGONWIND::BadGuysActionBlock::initCounterToFire ()
{
	int sTF = (int) (((_properties -> _sToFire < __BD 0) 
		? __BD (rand () % ((int) (-_properties -> _sToFire * __BD 100))) / __BD 100 
		: _properties -> _sToFire) * __BD game () -> framesPerSecond ());
	counter (_COUNTERTOFIRE) -> initialize (sTF, 0, true, true);
}

// ---
void DRAGONWIND::BadGuysActionBlock::initCounterBetweenFires ()
{
	int sBF = (int) (((_properties -> _sBetweenFires < __BD 0) 
		? __BD (rand () % ((int) (-_properties -> _sBetweenFires * __BD 100))) / __BD 100 
		: _properties -> _sBetweenFires) * __BD game () -> framesPerSecond ());
	counter (_CONTERBETWEENFIRES) -> initialize (sBF, 0, true, true);
}

// ---
void DRAGONWIND::BadGuysActionBlock::initCounterFiring ()
{
	int sF = (int) (((_properties -> _sFiring < __BD 0) 
		? __BD (rand () % ((int) (-_properties -> _sFiring * __BD 100))) / __BD 100 
		: _properties -> _sFiring) * __BD game () -> framesPerSecond ());
	counter (_COUNTERFIRING) -> initialize (sF, 0, true, true);
}

// ---
__IMPLEMENTCOUNTERS__ (DRAGONWIND::BadGuysActionBlock::Counters)
{
	// They will all redefined when the block is intialized and taken into account the propeties...
	addCounter (new QGAMES::Counter (DRAGONWIND::BadGuysActionBlock::_COUNTERTOFIRE, 
		2 * QGAMES::Game::game () -> framesPerSecond (), true, true)); 
	addCounter (new QGAMES::Counter (DRAGONWIND::BadGuysActionBlock::_CONTERBETWEENFIRES, 
		2 * QGAMES::Game::game () -> framesPerSecond (), true, true)); 
	addCounter (new QGAMES::Counter (DRAGONWIND::BadGuysActionBlock::_COUNTERFIRING, 
		2 * QGAMES::Game::game () -> framesPerSecond (), true, true)); 
}

// ---
__IMPLEMENTONOFFSWITCHES__ (DRAGONWIND::BadGuysActionBlock::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (DRAGONWIND::BadGuysActionBlock::_SWITCHTOMOVE, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch (DRAGONWIND::BadGuysActionBlock::_SWITCHFIRING, false));
}

// ---
DRAGONWIND::EnvironmentalConditionActionBlock::Properties::Properties 
	(const std::map <std::string, std::string>& dt)
{
	if (dt.find (std::string (__DRAGONWIND_ACTIONBLOCKENVCONDATTRTYPE__)) != dt.end ())
		_typeCondition = std::atoi ((*dt.find (__DRAGONWIND_ACTIONBLOCKENVCONDATTRTYPE__)).second.c_str ());
	if (dt.find (std::string (__DRAGONWIND_ACTIONBLOCKENVCONDATTRSSTART__)) != dt.end ())
		_sToStart = __BD (std::atof ((*dt.find (__DRAGONWIND_ACTIONBLOCKENVCONDATTRSSTART__)).second.c_str ()));
	if (dt.find (std::string (__DRAGONWIND_ACTIONBLOCKENVCONDATTRSENV__)) != dt.end ())
		_sCondition = __BD (std::atof ((*dt.find (__DRAGONWIND_ACTIONBLOCKENVCONDATTRSENV__)).second.c_str ()));

	_otherProperties = dt;
}

// ---
QGAMES::SetOfOpenValues DRAGONWIND::EnvironmentalConditionActionBlock::Properties::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));

	result.addOpenValue (0, QGAMES::OpenValue (_typeCondition));
	result.addOpenValue (1, QGAMES::OpenValue (_sToStart));
	result.addOpenValue (2, QGAMES::OpenValue (_sCondition));

	// Other properties are not saved...

	return (result);
}

// ---
void DRAGONWIND::EnvironmentalConditionActionBlock::Properties::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));
	assert (oV.existOpenValue (0) && oV.existOpenValue (1) && oV.existOpenValue (2));

	_typeCondition = oV.openValue (0).intValue ();
	_sToStart = oV.openValue (1).bdataValue ();
	_sCondition = oV.openValue (2).bdataValue ();

	// Other properties are not recovered...
}

// ---
DRAGONWIND::EnvironmentalConditionActionBlock::EnvironmentalConditionActionBlock (int id, Properties* prps)
	: DRAGONWIND::SceneActionBlock (id),
	  _properties (prps),
	  _environmentalCondition (NULL)
{ 
	assert (_properties);
}

// ---
DRAGONWIND::EnvironmentalConditionActionBlock::~EnvironmentalConditionActionBlock ()
{ 
	delete (_properties); 
	_properties = NULL; 

	_environmentalCondition = NULL;
}


// ---
QGAMES::SetOfOpenValues DRAGONWIND::EnvironmentalConditionActionBlock::runtimeValues () const
{
	assert (_properties);

	QGAMES::SetOfOpenValues result = DRAGONWIND::SceneActionBlock::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	// Adds the information of the properties of the block
	// They usually don't change ever, but just in case...
	result.addSetOfOpenValues (lNE + 1, _properties -> asSetOfOpenValues ());

	// Adds the status of the enviromental condition (if any)...
	result.addOpenValue (lE + 1, (_environmentalCondition == NULL) 
		? __MININT__ /** Meaning none. */ : properties () -> _typeCondition); // Saves the type of condition if any...
	result.addSetOfOpenValues (lNE + 2, _environmentalCondition 
		? _environmentalCondition -> runtimeValues () 
			: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__))); // It could be empty...

	return (result);
}

// ---
void DRAGONWIND::EnvironmentalConditionActionBlock::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	assert (_properties);

	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existOpenValue (lE) &&
			cfg.existSetOfOpenValues (lNE) && cfg.existSetOfOpenValues (lNE - 1));

	QGAMES::SetOfOpenValues cCfg = cfg;

	// Read the environmental condition if any...
	if (cCfg.openValue (lE).intValue () != __MININT__)
	{
		_environmentalCondition = conditionForType (cCfg.openValue (lE).intValue ());

		assert (_environmentalCondition);

		scene () -> setEnvironmentalCondition (_environmentalCondition);
	}

	cCfg.removeOpenValue (lE);

	// Sets its values if any...
	// It could happen that the environment wasn't set when the intervalues were saved
	// At this point, the environmental could exist already but with no initial values!
	QGAMES::SetOfOpenValues eVV = cCfg.setOfOpenValues (lNE);
	assert (eVV.name () == std::string (__QGAMES_RUNTIMEVALUESELMNTTYPE__));
	if (_environmentalCondition && eVV.numberValues () != 0)
		_environmentalCondition -> initializeRuntimeValuesFrom (eVV);
	cCfg.removeSetOfOpenValues (lNE);

	// Now the properties...
	_properties -> fromSetOfOpenValues (cCfg.setOfOpenValues (lNE - 1));
	cCfg.removeSetOfOpenValues (lNE - 1);

	DRAGONWIND::SceneActionBlock::initializeRuntimeValuesFrom (cCfg);
}

// ---
void DRAGONWIND::EnvironmentalConditionActionBlock::setActive (bool a)
{
	DRAGONWIND::SceneActionBlock::setActive (a);

	if (!scene ())
		return; // Makes no sense to be here when the scene has not been set yet...

	// If the block is disactivated, then the environmental phenomenon has to stop...
	// If it is actived, it will continue!, but is not needed to initialize it back!
	if (!isActive ()) finalize ();
	// Never invoke initialize here because a recursive invocation will be set...
	// DRAGONWIND::SceneBlockAction call setActive back!
}

// ---
void DRAGONWIND::EnvironmentalConditionActionBlock::initialize ()
{
	DRAGONWIND::SceneActionBlock::initialize ();

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	initCounterToStart ();
	initCounterCondition ();

	onOffSwitch (_SWITCHETERNAL) -> set (_properties -> _sCondition == __BD -1);
}

// ---
void DRAGONWIND::EnvironmentalConditionActionBlock::updatePositions ()
{
	DRAGONWIND::SceneActionBlock::updatePositions ();

	assert (scene ()); // It can't be null...

	// If there is no environmental condition yet...
	if (!_environmentalCondition)
	{
		// ...and the counter for it to appear has finished...
		if (counter (_COUNTERTOSTART) -> isEnd ())
		{
			_environmentalCondition = conditionForType (_properties -> _typeCondition); // ...appears...

			// Notice that the environmental condition is not observed by this block up to now...

			assert (_environmentalCondition);

			scene () -> setEnvironmentalCondition (_environmentalCondition);

			initCounterToStart ();
		}
	}
	// ...but if it is already one...
	else
	{
		// ...until the time is over...
		if (!onOffSwitch (_SWITCHETERNAL) -> isOn () && counter (_COUNTERCONDITION) -> isEnd ())
		{
			scene () -> setEnvironmentalCondition (NULL);

			_environmentalCondition = NULL;

			initCounterCondition ();
		}
	}
}

// ---
void DRAGONWIND::EnvironmentalConditionActionBlock::finalize ()
{
	DRAGONWIND::SceneActionBlock::finalize ();

	if (_environmentalCondition != NULL)
	{
		scene () -> setEnvironmentalCondition (NULL);

		_environmentalCondition = NULL;
	}
}

// ---
__IMPLEMENTCOUNTERS__ (DRAGONWIND::EnvironmentalConditionActionBlock::Counters)
{
	// They will all redefined when the block is intialized and taken into account the propeties...
	addCounter (new QGAMES::Counter (DRAGONWIND::EnvironmentalConditionActionBlock::_COUNTERTOSTART, 
		0, 0, true, true)); 
	addCounter (new QGAMES::Counter (DRAGONWIND::EnvironmentalConditionActionBlock::_COUNTERCONDITION, 
		0, 0, true, true)); 
}

// ---
__IMPLEMENTONOFFSWITCHES__ (DRAGONWIND::EnvironmentalConditionActionBlock::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (DRAGONWIND::EnvironmentalConditionActionBlock::_SWITCHETERNAL, true));
}

// ---
QGAMES::EnvironmentalCondition* DRAGONWIND::EnvironmentalConditionActionBlock::conditionForType (int t)
{
	QGAMES::EnvironmentalCondition* result = NULL;

	if (t == -1)
		t = rand () % 8; // From 0 to 7... if random has beenselected...

	switch (t)
	{
		case __DRAGONWIND_ACTIONBLOCKENVCONDTYPETHUN__ :
			result = new QGAMES::Thunders (scene (), 
				QGAMES::Thunders::Properties (_properties -> _otherProperties));
			break;

		case __DRAGONWIND_ACTIONBLOCKENVCONDTYPERAIN__:
			result = new QGAMES::Rain (scene (), 
				QGAMES::Rain::Properties (_properties -> _otherProperties));
			break;

		case __DRAGONWIND_ACTIONBLOCKENVCONDTYPEEATRH__:
			result = new QGAMES::Earthquake (scene (), 
				QGAMES::Earthquake::Properties (_properties -> _otherProperties));
			break;

		case __DRAGONWIND_ACTIONBLOCKENVCONDTYPEWIND__:
			result = new QGAMES::Wind (scene (), 
				QGAMES::Wind::Properties (_properties -> _otherProperties));
			break;

		case __DRAGONWIND_ACTIONBLOCKENVCONDTYPELIGHT__:
			result = new QGAMES::Lightnings (scene (), 
				QGAMES::Lightnings::Properties (_properties -> _otherProperties));
			break;

		case __DRAGONWIND_ACTIONBLOCKENVCONDTYPERAINB__:
			result = new QGAMES::Rainbow (scene (), _properties -> _otherProperties);
			break;

		case __DRAGONWIND_ACTIONBLOCKENVCONDTYPESTORM__:
			result = new QGAMES::Storm (scene (), 
				new QGAMES::Thunder (scene (), _properties -> _otherProperties),
				new QGAMES::Lightning (scene (), _properties -> _otherProperties),
				new QGAMES::Rain (scene (), QGAMES::Rain::Properties (_properties -> _otherProperties)), 
				new QGAMES::Rainbow (scene (), _properties -> _otherProperties));
			break;

		case __DRAGONWIND_ACTIONBLOCKENVCONDTYPECLOUD__:
			result = new QGAMES::Clouds (scene (), 
				QGAMES::Clouds::Properties (_properties -> _otherProperties));
			break;

		default:
			assert (0);
	}

	return (result);
}

// ---
void DRAGONWIND::EnvironmentalConditionActionBlock::initCounterToStart ()
{
	int sTS = (int) (((_properties -> _sToStart < __BD 0) 
		? __BD (rand () % ((int) (-_properties -> _sToStart * __BD 100))) / __BD 100 
		: _properties -> _sToStart) * __BD game () -> framesPerSecond ());
	counter (_COUNTERTOSTART) -> initialize (sTS, 0, true, true);
}

// ---
void DRAGONWIND::EnvironmentalConditionActionBlock::initCounterCondition ()
{
	int sC = (_properties -> _sCondition == -1) 
		? 0 // Ethernal...
		: ((int) (((_properties -> _sCondition < __BD 0) 
			? __BD (rand () % ((int) (-_properties -> _sCondition * __BD 100))) / __BD 100 
			: _properties -> _sCondition) * __BD game () -> framesPerSecond ()));
	counter (_COUNTERCONDITION) -> initialize (sC, 0, true, true);
}

// ---
DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock::Properties::Properties 
		(const std::map <std::string, std::string>& prps)
	: _defaultSetOfLayersActived (-1),
	  _setsOfLayers ()
{
	int nSets = 0;
	if (prps.find (std::string (__DRAGONWIND_ACTIONBLOCKACTLAYERATTRNUM__)) != prps.end ())
		nSets = std::stoi ((*prps.find (std::string (__DRAGONWIND_ACTIONBLOCKACTLAYERATTRNUM__))).second.c_str ());

	for (int i = 0; i < nSets; i++)
	{
		std::map <std::string, std::string>::const_iterator j = 
			prps.find (std::string (__DRAGONWIND_ACTIONBLOCKACTLAYERATTRSET__) + std::to_string (i));
		assert (j != prps.end ());
		_setsOfLayers [i] = QGAMES::getElementsFrom ((*j).second, ',');
	}

	_defaultSetOfLayersActived = -1;
	if (prps.find (std::string (__DRAGONWIND_ACTIONBLOCLKACTLAYERATTRDEF__)) != prps.end ())
		_defaultSetOfLayersActived = 
			std::stoi ((*prps.find (std::string (__DRAGONWIND_ACTIONBLOCLKACTLAYERATTRDEF__))).second.c_str ());
}

// ---
QGAMES::SetOfOpenValues DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock::Properties::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));

	result.addOpenValue (0, QGAMES::OpenValue (_defaultSetOfLayersActived));

	// Other properties are not saved...

	return (result);
}

// ---
void DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock::Properties::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));
	assert (oV.existOpenValue (0));

	_defaultSetOfLayersActived = oV.openValue (0).intValue ();

	// Other properties are not recovered...
}

DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock::SwitchVisibilityBetweenASetOfLayersActionBlock 
	(int id, Properties* prps)
	: DRAGONWIND::SceneActionBlock (id),
	  _properties (prps),
	  _setsOfLayers (),
	  _setOfLayersActived (-1)
{
	assert (_properties != NULL);
}

// ---
QGAMES::SetOfOpenValues DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock::runtimeValues () const
{
	assert (_properties);

	QGAMES::SetOfOpenValues result = DRAGONWIND::SceneActionBlock::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	result.addOpenValue (lE + 1, _setOfLayersActived);

	result.addSetOfOpenValues (lNE + 1, _properties -> asSetOfOpenValues ());

	// It is not needed to save here the status of the layers
	// because they are already being saved when the status of the scene is stored

	return (result);
}

// ---
void DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock::initializeRuntimeValuesFrom 
	(const QGAMES::SetOfOpenValues& cfg)
{
	assert (_properties);

	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	activeSetOfLayers (cCfg.openValue (lE).intValue ());
	cCfg.removeOpenValue (lE);

	_properties -> fromSetOfOpenValues (cCfg.setOfOpenValues (lNE));
	cCfg.removeSetOfOpenValues (lNE);

	// The status of the layers is not recovered here...
	// It is recovered when the status of its map is got back!

	DRAGONWIND::SceneActionBlock::initializeRuntimeValuesFrom (cCfg);
}

// ---
void DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock::activeSetOfLayers (int nS)
{
	_setOfLayersActived = nS;

	for (std::map <int, QGAMES::AdvancedTileLayers>::const_iterator i = _setsOfLayers.begin ();
			i != _setsOfLayers.end (); i++)
	{
		bool v = (*i).first == nS;
		for (QGAMES::AdvancedTileLayers::const_iterator j = (*i).second.begin (); j != (*i).second.end (); j++)
		{
			QGAMES::AdvancedTileLayer* aL = (*j).second;
			if ((aL -> isVisible () && !v) || (!aL -> isVisible () && v))
				aL -> setVisible (v);
		}
	}
}

// ---
void DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock::initialize ()
{
	DRAGONWIND::SceneActionBlock::initialize ();

	for (std::map <int, std::vector <std::string>>::const_iterator i = _properties -> _setsOfLayers.begin ();
			i != _properties -> _setsOfLayers.end (); i++)
	{
		QGAMES::AdvancedTileLayers lys;
		for (int j = 0; j < (int) (*i).second.size (); j++)
		{
			QGAMES::AdvancedTileLayer* aTL = 
				dynamic_cast <QGAMES::AdvancedTileLayer*> (scene () -> activeMap () -> layer ((*i).second [j]));
			assert (aTL);
			lys [j] = aTL;
		}

		_setsOfLayers [(*i).first] = lys;
	}

	activeSetOfLayers (_properties -> _defaultSetOfLayersActived);
}

// ---
void DRAGONWIND::SwitchVisibilityBetweenASetOfLayersActionBlock::finalize ()
{
	DRAGONWIND::SceneActionBlock::finalize ();

	_setsOfLayers = std::map <int, QGAMES::AdvancedTileLayers> ();
}

// ---
DRAGONWIND::MoveLinearASetOfLayersActionBlock::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _setOfLayers (), 
	  _direction (QGAMES::Vector::_cero),
	  _steps (0),
	  _sound (-1),
	  _soundChannel (__QGAMES_GAMESOUNDCHANNEL__)
{
	if (prps.find (std::string (__DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRSET__)) != prps.end ())
		_setOfLayers = QGAMES::getElementsFrom 
			((*prps.find (std::string (__DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRSET__))).second.c_str (), ',');
	if (prps.find (std::string (__DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRDIR__)) != prps.end ())
		_direction = QGAMES::Vector ((*prps.find (std::string (__DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRDIR__))).second.c_str ());
	if (prps.find (std::string (__DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRLONG__)) != prps.end ())
		_steps = std::atoi ((*prps.find (std::string (__DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRLONG__))).second.c_str ());
	if (prps.find (std::string (__DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRSPD__)) != prps.end ())
		_speed = std::atoi ((*prps.find (std::string (__DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRSPD__))).second.c_str ());
	if (prps.find (std::string (__DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRSND__)) != prps.end ())
		_sound = std::atoi ((*prps.find (std::string (__DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRSND__))).second.c_str ());
	if (prps.find (std::string (__DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRSCHN__)) != prps.end ())
		_soundChannel = std::atoi ((*prps.find (std::string (__DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRSCHN__))).second.c_str ());
}

// ---
QGAMES::SetOfOpenValues DRAGONWIND::MoveLinearASetOfLayersActionBlock::Properties::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));

	std::stringstream pS; pS << _direction;
	result.addOpenValue (0, QGAMES::OpenValue (pS.str ()));
	result.addOpenValue (1, _steps);
	result.addOpenValue (2, _speed);
	result.addOpenValue (3, _sound);
	result.addOpenValue (4, _soundChannel);

	// Other properties are not saved... (mainly the layers involved)

	return (result);
}

// ---
void DRAGONWIND::MoveLinearASetOfLayersActionBlock::Properties::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));
	assert (oV.existOpenValue (0) && oV.existOpenValue (1) && 
			oV.existOpenValue (2) && oV.existOpenValue (3) &&
			oV.existOpenValue (4));

	_direction = QGAMES::Vector (oV.openValue (0).strValue ());
	_steps = oV.openValue (1).intValue ();
	_speed = oV.openValue (2).intValue ();
	_sound = oV.openValue (3).intValue ();
	_soundChannel = oV.openValue (4).intValue ();

	// Other properties are not recovered...
}

DRAGONWIND::MoveLinearASetOfLayersActionBlock::MoveLinearASetOfLayersActionBlock (int id, Properties* prps)
	: DRAGONWIND::SceneActionBlock (id),
	  _properties (prps),
	  _setOfLayers (),
	  _moving (false)
{
	assert (_properties != NULL);
}

// ---
QGAMES::SetOfOpenValues DRAGONWIND::MoveLinearASetOfLayersActionBlock::runtimeValues () const
{
	assert (_properties);

	QGAMES::SetOfOpenValues result = DRAGONWIND::SceneActionBlock::runtimeValues ();

	int lNE = result.lastNestedOpenValueId ();

	result.addSetOfOpenValues (lNE + 1, _properties -> asSetOfOpenValues ());

	// It is not needed to save here the status of the layers
	// because they are already being saved when the status of the scene is stored

	return (result);

}

// ---
void DRAGONWIND::MoveLinearASetOfLayersActionBlock::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	assert (_properties);

	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_properties -> fromSetOfOpenValues (cCfg.setOfOpenValues (lNE));
	cCfg.removeSetOfOpenValues (lNE);

	// The status of the layers is not recovered here...
	// It is recovered when the status of its map is got back!

	DRAGONWIND::SceneActionBlock::initializeRuntimeValuesFrom (cCfg);
}

// ---
void DRAGONWIND::MoveLinearASetOfLayersActionBlock::toMoveForward ()
{
	if (isMovingBackwards () || hasMovedBackwards ())
	{
		for (QGAMES::AdvancedTileLayers::const_iterator i = _setOfLayers.begin (); i != _setOfLayers.end (); i++)
		{
			(*i).second -> setMovement (new QGAMES::Layer::LinearMovementBehaviour (_properties -> _speed, 
				 (int) (__BD _properties -> _steps * _properties -> _direction - (*i).second -> position ()).module (), 
				 _properties -> _direction));
		}

		_moving = true;

		if (_properties -> _sound != -1)
			game () -> sound (_properties -> _sound) -> play (_properties -> _soundChannel);
	}
}

// ---
bool DRAGONWIND::MoveLinearASetOfLayersActionBlock::isMovingForwards ()
{
	// Just checking one is enough...
	// All of them moves in the same direction and at the same speed...
	return (!_setOfLayers.empty () && _setOfLayers [0] -> movement () != NULL && 
		((QGAMES::Layer::LinearMovementBehaviour*) _setOfLayers [0] -> movement ()) -> direction () == _properties->_direction);
}

// ---
bool DRAGONWIND::MoveLinearASetOfLayersActionBlock::hasMovedForwards ()
{
	return (!_setOfLayers.empty () && (int) _setOfLayers [0] -> position ().module () == _properties -> _steps);
}

// ---
void DRAGONWIND::MoveLinearASetOfLayersActionBlock::toMoveBackward ()
{
	if (isMovingForwards () || hasMovedForwards ())
	{
		for (QGAMES::AdvancedTileLayers::const_iterator i = _setOfLayers.begin (); i != _setOfLayers.end (); i++)
		{
			(*i).second -> setMovement (new QGAMES::Layer::LinearMovementBehaviour (_properties -> _speed, 
				 (int) ((*i).second -> position ().module ()), -_properties -> _direction));
		}

		_moving = true;

		if (_properties -> _sound != -1)
			game () -> sound (_properties -> _sound) -> play (_properties -> _soundChannel);
	}
}

// ---
bool DRAGONWIND::MoveLinearASetOfLayersActionBlock::isMovingBackwards ()
{
	return (!_setOfLayers.empty () && _setOfLayers [0] -> movement () != NULL && 
		((QGAMES::Layer::LinearMovementBehaviour*) _setOfLayers [0] -> movement ()) -> direction () == -_properties->_direction);
}

// ---
bool DRAGONWIND::MoveLinearASetOfLayersActionBlock::hasMovedBackwards ()
{
	return (!_setOfLayers.empty () && _setOfLayers [0] -> position ().closeTo (QGAMES::Position::_cero));
}

// ---
void DRAGONWIND::MoveLinearASetOfLayersActionBlock::setActive (bool a)
{ 
	DRAGONWIND::SceneActionBlock::setActive (a);

	if (!a) toMoveBackward (); 
}

// ---
void DRAGONWIND::MoveLinearASetOfLayersActionBlock::initialize ()
{
	DRAGONWIND::SceneActionBlock::initialize ();

	for (int i = 0; i < (int) _properties -> _setOfLayers.size (); i++)
	{
		QGAMES::AdvancedTileLayer* aTL = dynamic_cast <QGAMES::AdvancedTileLayer*> 
			(scene () -> activeMap () -> layer (_properties -> _setOfLayers [i]));
		assert (aTL);
		_setOfLayers [i] = aTL;
	}

	toMoveBackward (); // The original position...
}

// ---
void DRAGONWIND::MoveLinearASetOfLayersActionBlock::updatePositions ()
{
	DRAGONWIND::SceneActionBlock::updatePositions ();

	if (_moving && (hasMovedBackwards () || hasMovedForwards ()))
	{
		_moving = false;

		if (_properties -> _sound != -1)
			game () -> sound (_properties -> _sound) -> stop ();
	}
}

// ---
void DRAGONWIND::MoveLinearASetOfLayersActionBlock::finalize ()
{
	DRAGONWIND::SceneActionBlock::finalize ();

	_setOfLayers = QGAMES::AdvancedTileLayers ();
}
