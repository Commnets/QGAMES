#include <DragonwindLike/dwflyingmonsters.hpp>
#include <DragonwindLike/dwgame.hpp>

// ---
QGAMES::Entity* DRAGONWIND::FlyingMonster::clone () const
{
	QGAMES::Entity* result = new DRAGONWIND::FlyingMonster (_id, _properties, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void DRAGONWIND::FlyingMonster::moveToDestinationPoint (const QGAMES::Position& p, 
	const QGAMES::Vector& iS, QGAMES::bdata mS, QGAMES::bdata a)
{
	assert (mS > __BD 0 && a > __BD 0);

	_destinationPoint = p;

	if (position () != _destinationPoint)
	{
		setCurrentState (_destinationPoint.posX () >= position ().posX () 
			? __DRAGONWIND_MONSTERFLYINGRIGHTSTATEID__ : __DRAGONWIND_MONSTERFLYINGLEFTSTATEID__);
		QGAMES::IncrementalLinearMovement* mov = 
			dynamic_cast <QGAMES::IncrementalLinearMovement*> (currentMovement ());
		assert (mov); // To be sure...
		mov -> setMovePath (position (), _destinationPoint, iS, mS, a, true);
	}
	else
		setCurrentState (position ().posX () >= 0 
			? __DRAGONWIND_MONSTERSTANDRIGHTSTATEID__ : __DRAGONWIND_MONSTERSTANDLEFTSTATEID__);
}

// ---
void DRAGONWIND::FlyingMonster::initialize ()
{
	_destinationPoint = QGAMES::Position::_cero;

	setCurrentState (__DRAGONWIND_MONSTERSTANDRIGHTSTATEID__);

	QGAMES::Character::initialize ();
}

// ---
void DRAGONWIND::FlyingMonster::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (isVisible ())
		QGAMES::Character::drawOn (s, p);
}

// ---
void DRAGONWIND::FlyingMonster::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __QGAMES_MOVEMENTLIMITREACHED__ ||
		evnt.code () == __QGAMES_MOVEMENTNOTPOSSIBLE__)
		notify (QGAMES::Event (evnt.code (), this));
}

// ---
void DRAGONWIND::FlyingMonster::whenCollisionWith (QGAMES::Entity* e)
{
	DRAGONWIND::Character* chr = dynamic_cast <DRAGONWIND::Character*> (e);
	if (chr && chr -> isAlive () && !chr -> isSwiming () && _properties._dieWhenHit) 
		// Only when the character is alive and not swming...
		setVisible (false);
}

// ---
QGAMES::Position DRAGONWIND::FlockOfMonsters::FlyingZone::randomPosition () const
{
	int pX = rand () % 100; int pY = rand () % 100; int pZ = rand () % 100;
	return (QGAMES::Position 
		(_frontZone.pos1 ().posX () + (__BD _frontZone.width () * __BD pX / __BD 100),
		 _frontZone.pos1 ().posY () + (__BD _frontZone.height () * __BD pY / __BD 100),
		 _frontZone.pos1 ().posZ () + (__BD _depth * __BD pZ / __BD 100))); // Coordinate Z usually is 0...
}

// ---
DRAGONWIND::FlockOfMonsters::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _numberOfMonsters (1),
	  _radix (__BD 50),
	  _timeToMove (__BD 1),
	  _timeMoving (__BD 3),
	  _shootingEnergy (1),
	  _dieWhenHit (true),
	  _flyingZone ()
{
	if (prps.find (__DRAGONWIND_FLOCKATTRNUMBEROFMONSTERS__) != prps.end ()) 
		_numberOfMonsters = std::atoi ((*prps.find (__DRAGONWIND_FLOCKATTRNUMBEROFMONSTERS__)).second.c_str ());
	if (prps.find (__DRAGONWIND_FLOCKATTRRADIX__) != prps.end ()) 
		_radix = __BD (std::atof ((*prps.find (__DRAGONWIND_FLOCKATTRRADIX__)).second.c_str ()));
	if (prps.find (__DRAGONWIND_FLOCKATTRTIMETOMOVE__) != prps.end ()) 
		_timeToMove = __BD (std::atof ((*prps.find (__DRAGONWIND_FLOCKATTRTIMETOMOVE__)).second.c_str ()));
	if (prps.find (__DRAGONWIND_FLOCKATTRTIMEMOVING__) != prps.end ()) 
		_timeMoving = __BD (std::atof ((*prps.find (__DRAGONWIND_FLOCKATTRTIMEMOVING__)).second.c_str ()));
	if (prps.find (__DRAGONWIND_FLOCATTRSHOOTINGENERGY__) != prps.end ()) 
		_shootingEnergy = std::atoi ((*prps.find (__DRAGONWIND_FLOCATTRSHOOTINGENERGY__)).second.c_str ());
	if (prps.find (__DRAGONWIND_FLOCKATTRDIEWHENHIT__) != prps.end ()) 
		_dieWhenHit = QGAMES::toUpper (QGAMES::trim 
			(std::string ((*prps.find (__DRAGONWIND_FLOCKATTRDIEWHENHIT__)).second))) == std::string (__YES_STRING__);

	DRAGONWIND::FlockOfMonsters::Properties::Properties 
		(_numberOfMonsters, _radix, _timeToMove, _timeMoving, 
		 _shootingEnergy, _dieWhenHit, _flyingZone);
}

// ---
DRAGONWIND::FlockOfMonsters::FlockOfMonsters (int cId, 
		const QGAMES::Entities& eties, const DRAGONWIND::FlockOfMonsters::Properties& prps,
		const QGAMES::Entity::Data& dt)
	: QGAMES::CompositeEntity (cId, eties, dt),
	  _properties (prps),
	  _leader (NULL),
	  _leadReachedGoal (false)
{
	assert ((int) entities ().size () >= 1 &&
			(int) entities ().size () >= _properties._numberOfMonsters);
	assert (_data._physics.find (0) != _data._physics.end () &&
			_data._physics.find (1) != _data._physics.end ()); // Speed and acceleration defined mandatory!

	// Just to be sure, there is no other entities inside than flying monsters
	// and with just two different animations (0 looking to the right and 1 looking to the left) minimum
	for (QGAMES::Entities::const_iterator i = entities ().begin (); i != entities ().end (); i++)
	{
		assert (dynamic_cast <DRAGONWIND::FlyingMonster*> ((*i).second)); // Flying monster inside?
		assert ((*i).second -> existState (__DRAGONWIND_MONSTERFLYINGRIGHTSTATEID__) &&
				(*i).second -> existState (__DRAGONWIND_MONSTERSTANDRIGHTSTATEID__) &&
				(*i).second -> existState (__DRAGONWIND_MONSTERFLYINGLEFTSTATEID__) &&
				(*i).second -> existState (__DRAGONWIND_MONSTERSTANDLEFTSTATEID__)); // ...and with 4 animations?
	}

	setVisible (false);
}

// ---
QGAMES::Entity* DRAGONWIND::FlockOfMonsters::clone () const
{
	QGAMES::Entities eties;
	for (QGAMES::Entities::const_iterator i = _entities.begin (); i != _entities.end (); i++)
		eties [(*i).first] = (*i).second -> clone (); // To clone the composition, is is needed to clone everything...
	
	QGAMES::Entity* result = new DRAGONWIND::FlockOfMonsters (_id, eties, _properties, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void DRAGONWIND::FlockOfMonsters::initialize ()
{
	_leader = NULL;
	_leadReachedGoal = false;

	int i = 0;

	QGAMES::Position cIPos = _properties._flyingZone.randomPosition ();
	for (QGAMES::Entities::const_iterator j = entities ().begin (); j != entities ().end (); j++, i++)
	{
		DRAGONWIND::FlyingMonster* fM =	
			(DRAGONWIND::FlyingMonster*) (*j).second; // Guarantted at construction time...
		if (!_leader) _leader = fM;

		observe (fM);

		if (fM != _leader)
		{
			QGAMES::bdata ang = __BD (rand () % 360) * __BD 2 * __PI / __BD 360;
			QGAMES::bdata rdx = __BD (rand () % 100) * _properties._radix / __BD 100;
			(*j).second -> setPosition (cIPos + 
				QGAMES::Vector (rdx * __BD std::cos (ang), 
								rdx * __BD std::sin (ang), 
								__BD 0)); // Around....
		}
		else
			(*j).second -> setPosition (cIPos);

		fM -> setProperties (DRAGONWIND::FlyingMonster::Properties 
			(_properties._shootingEnergy, _properties._dieWhenHit));

		fM -> setVisible (i < _properties._numberOfMonsters ? true : false);
	}

	QGAMES::CompositeEntity::initialize ();

	reStartAllCounters ();
	counter (_COUNTERTOMOVE) -> initialize 
		((int) (_properties._timeToMove * __BD game () -> framesPerSecond ()), 0, true, true);
	counter (_COUNTERMOVING) -> initialize 
		((int) (_properties._timeMoving * __BD game () -> framesPerSecond ()), 0, true, true);

	reStartAllOnOffSwitches ();
}

// ---
void DRAGONWIND::FlockOfMonsters::updatePositions ()
{
	if (!isVisible ())
		return; // The best way for not to mov the flock is to set its visibility to false...

	bool startToMove = false;

	if (onOffSwitch (_SWITCHMOVING) -> isOn ())
	{
		if (counter (_COUNTERMOVING) -> isEnd () || _leadReachedGoal)
		{
			_leadReachedGoal = false;

			DRAGONWIND::Game::Conf* cfg = dynamic_cast <DRAGONWIND::Game::Conf*> (game () -> configuration ());
			assert (cfg); // Just in case...

			if (!(startToMove = (rand () % 10) < (((cfg -> difficulty () + 1) * 3)) ? true : false))
			{
				onOffSwitch (_SWITCHMOVING) -> set (false);
				counter (_COUNTERTOMOVE) -> initialize 
					((int) (_properties._timeToMove * __BD game () -> framesPerSecond ()), 0, true, true);

				for (QGAMES::Entities::const_iterator j = entities ().begin (); j != entities ().end (); j++)
				{
					DRAGONWIND::FlyingMonster* fM =	
						(DRAGONWIND::FlyingMonster*) (*j).second; // Guarantted at construction time...
					if (fM -> isVisible ())
					{
						int sId = fM -> currentState () -> id ();
						fM -> setCurrentState 
							((sId == __DRAGONWIND_MONSTERFLYINGRIGHTSTATEID__) 
								? __DRAGONWIND_MONSTERSTANDRIGHTSTATEID__
								: ((sId == __DRAGONWIND_MONSTERFLYINGLEFTSTATEID__) 
									? __DRAGONWIND_MONSTERSTANDLEFTSTATEID__ : __MININT__ /** It shouldn't happen. */));
						// Changes the flying animation by a standing one...
					}
				}
			}
		}
	}
	else
	{
		if (counter (_COUNTERTOMOVE) -> isEnd ())
		{
			onOffSwitch (_SWITCHMOVING) -> set (true);
			counter (_COUNTERMOVING) -> initialize 
				((int) (_properties._timeMoving * __BD game () -> framesPerSecond ()), 0, true, true);

			startToMove = true;
		}
	}

	if (startToMove)
	{
		_properties._flyingZone.setFrontZoneToCurrentScreenSize ();
		QGAMES::Position cFPos = _properties._flyingZone.randomPosition ();
		QGAMES::Vector cFVec = cFPos - _leader -> position ();
		for (QGAMES::Entities::const_iterator j = entities ().begin (); j != entities ().end (); j++)
		{
			DRAGONWIND::FlyingMonster* fM =	
				(DRAGONWIND::FlyingMonster*) (*j).second; // Guarantted at construction time...
			if (fM -> isVisible ())
				fM -> moveToDestinationPoint (fM -> position () + cFVec, 
					QGAMES::Vector::_cero /** Consider previous speed. */,
					__BD _data._physics [0], __BD _data._physics [1]);
		}
	}

	// Still alive?
	bool fV = false;
	for (QGAMES::Entities::const_iterator i = entities ().begin (); 
		i != entities ().end () && !fV; fV = (*i++).second -> isVisible ()); // When one visible, not killed...
	if (!fV)
		notify (QGAMES::Event (__DRAGONWIND_FLOCKKILLED__, this));

	QGAMES::CompositeEntity::updatePositions ();
}

// ---
void DRAGONWIND::FlockOfMonsters::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	QGAMES::CompositeEntity::drawOn (s, p);
}

// ---
void DRAGONWIND::FlockOfMonsters::finalize ()
{
	QGAMES::CompositeEntity::finalize ();

	for (QGAMES::Entities::const_iterator j = entities ().begin (); j != entities ().end (); j++)
	{
		(*j).second -> setVisible (false);
		unObserve ((*j).second);
	}
}

// ---
void DRAGONWIND::FlockOfMonsters::whenCollisionWith (QGAMES::Entity* e)
{
	//... but with which specific monster of the flock has collisioned?
	for (QGAMES::Entities::const_iterator i = _entities.begin (); 
			i != _entities.end (); i++)
		if ((*i).second ->  isVisible () && (*i).second -> hasCollisionWith (e))
			(*i).second -> whenCollisionWith (e);
}

// ---
void DRAGONWIND::FlockOfMonsters::processEvent (const QGAMES::Event& evnt)
{
	if ((evnt.code () == __QGAMES_MOVEMENTLIMITREACHED__ || evnt.code () == __QGAMES_MOVEMENTNOTPOSSIBLE__) && 
		 evnt.data () == (void*) _leader)
		_leadReachedGoal = true;

	QGAMES::CompositeEntity::processEvent (evnt);
}

// ---
__IMPLEMENTCOUNTERS__ (DRAGONWIND::FlockOfMonsters::Counters)
{
	addCounter (new QGAMES::Counter 
		(DRAGONWIND::FlockOfMonsters::_COUNTERTOMOVE, 0 /** changed later */, 0, true, true));
	addCounter (new QGAMES::Counter 
		(DRAGONWIND::FlockOfMonsters::_COUNTERMOVING, 0 /** changed later */, 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (DRAGONWIND::FlockOfMonsters::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (DRAGONWIND::FlockOfMonsters::_SWITCHMOVING, false));
}

// ---
QGAMES::Entity* DRAGONWIND::FlockOfBees::clone () const
{
	QGAMES::Entities eties;
	for (QGAMES::Entities::const_iterator i = _entities.begin (); i != _entities.end (); i++)
		eties [(*i).first] = (*i).second -> clone (); // To clone the composition, is is needed to clone everything...
	
	QGAMES::Entity* result = new DRAGONWIND::FlockOfBees (_id, eties, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void DRAGONWIND::FlockOfBees::initialize ()
{
	DRAGONWIND::FlockOfMonsters::initialize ();

	game () -> sound (__DRAGONWIND_BEESFLYINGSOUNDID__) -> play (__DRAGONWIND_FLOCKSOUNDCHANNEL__);
}

// ---
void DRAGONWIND::FlockOfBees::finalize ()
{
	DRAGONWIND::FlockOfMonsters::finalize ();

	game () -> sound (__DRAGONWIND_BEESFLYINGSOUNDID__) -> stop ();
}

// ---
QGAMES::Entity* DRAGONWIND::FlockOfBirds::clone () const
{
	QGAMES::Entities eties;
	for (QGAMES::Entities::const_iterator i = _entities.begin (); i != _entities.end (); i++)
		eties [(*i).first] = (*i).second -> clone (); // To clone the composition, is is needed to clone everything...
	
	QGAMES::Entity* result = new DRAGONWIND::FlockOfBirds (_id, eties, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void DRAGONWIND::FlockOfBirds::initialize ()
{
	DRAGONWIND::FlockOfMonsters::initialize ();

	game () -> sound (__DRAGONWIND_BIRDSFLYINGSOUNDID__) -> play (__DRAGONWIND_FLOCKSOUNDCHANNEL__);
}

// ---
void DRAGONWIND::FlockOfBirds::finalize ()
{
	DRAGONWIND::FlockOfMonsters::finalize ();

	game () -> sound (__DRAGONWIND_BIRDSFLYINGSOUNDID__) -> stop ();
}
