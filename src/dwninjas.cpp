#include <DragonwindLike/dwninjas.hpp>
#include <DragonwindLike/dwgame.hpp>
#include <DragonwindLike/dwscenes.hpp>
#include <DragonwindLike/dwmovements.hpp>
#include <DragonwindLike/dwdatagame.hpp>

const int DRAGONWIND::Ninja::_SHIELDINTENSITY [] = 
	{ 100, 120, 140, 160, 180, 200, 220, 240, 255, 255, 240, 220, 200, 180, 160, 140, 120, 100 };

// ---
DRAGONWIND::Ninja::Ninja (int cId, const QGAMES::Forms& f, const QGAMES::PlatformArtist::Data& d)
	: DRAGONWIND::Character (cId, new Behaviour (), f, d),
	  _pocket (__DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__), // With the max elements to see...
	  _thingsAround (),
	  _goalReached (-1)
{ 
	assert (d._physics.find (0) != d._physics.end ());

	// Thelist of swicthes exist already...
	assert (onOffSwitches ()); // Just in case...
	onOffSwitches () -> 
		addOnOffSwitch (new QGAMES::OnOffSwitch (DRAGONWIND::Ninja::_SWITCHTOREACHGOALWHENPOSSIBLE, false));
	onOffSwitches () -> 
		addOnOffSwitch (new QGAMES::OnOffSwitch (DRAGONWIND::Ninja::_SWITCHSOUNDENERGY, false));
}

// ---
QGAMES::Entity* DRAGONWIND::Ninja::clone () const
{
	QGAMES::Entity* result = new DRAGONWIND::Ninja (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void DRAGONWIND::Ninja::setEnergyToAdd (QGAMES::bdata e)
{ 
	DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
	assert (dG);

	if (dG -> passwordString () != std::string ("ICF") ||
		(dG -> passwordString () == std::string ("ICF") && e > __BD 0))
		DRAGONWIND::Character::setEnergyToAdd (e); // Only if there is no password avoiding it!
} 

// ---
void DRAGONWIND::Ninja::setPlatformState (int id)
{
	int cS = platformState ();
	DRAGONWIND::DragonArtist::setPlatformState (id);
	if (platformState () == cS || id == -1)
		return; // Nothing has changed...security system!

	assert (isPlatformStatePossible (id));

	switch (platformStateToDragonState ())
	{
		case DRAGONWIND::DragonArtist::DragonState::_STAND:
			setCurrentState (lookingToDirection () == 0 
				? __DRAGONWIND_NINJASTANDRIGHTSTATEID__ : __DRAGONWIND_NINJASTANDLEFTSTATEID__);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_RUN:
			setCurrentState (lookingToDirection () == 0 
				? __DRAGONWIND_NINJARUNNINGRIGHTSTATEID__ : __DRAGONWIND_NINJARUNNINGLEFTSTATEID__);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_CLIMB:
			setCurrentState (lookingToDirection () == 0 
				? __DRAGONWIND_NINJACLIMBINGRIGHTSTATEID__ : __DRAGONWIND_NINJACLIMBINGLEFTSTATEID__);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_CLIMBFIXED:
			setCurrentState (lookingToDirection () == 0 
				? __DRAGONWIND_NINJACLIMBFIXEDRIGHTSTATEID__ : __DRAGONWIND_NINJACLIMBFIXEDLEFTSTATEID__);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_FALL:
			{
				QGAMES::PhysicsIncremetalMovement* prevMov = 
					dynamic_cast <QGAMES::PhysicsIncremetalMovement*> (currentMovement ());
				QGAMES::bdata prevSpd = (prevMov) ? prevMov -> currentSpeed ().module () : __BD 0;
				setCurrentState (lookingToDirection () == 0 
					? __DRAGONWIND_NINJAFALLRIGHTSTATEID__ : __DRAGONWIND_NINJAFALLLEFTSTATEID__);
				QGAMES::PhysicsIncremetalMovement* mov = 
					dynamic_cast <QGAMES::PhysicsIncremetalMovement*> (currentMovement ());
				assert (mov);

				mov -> setMaxSpeed (prevSpd + __BD _data._physics [0]); 
			}

			break;

		case DRAGONWIND::DragonArtist::DragonState::_FLOAT:
			{
				// It could happen that this state was ordered directly
				// From setPlatformStateDeferred instruction
				// It will be only valid when falling previously in any case...
				if (platformStateToDragonState (cS) == DRAGONWIND::DragonArtist::DragonState::_FALL)
					setCurrentState (lookingToDirection () == 0 
						? __DRAGONWIND_NINJAGLADERIGHTSTATEID__ : __DRAGONWIND_NINJAGLADELEFTSTATEID__);
			}
			
			break;

		case DRAGONWIND::DragonArtist::DragonState::_JUMP:
			setCurrentState (lookingToDirection () == 0 
				? __DRAGONWIND_NINJAJUMPRIGHTSTATEID__ : __DRAGONWIND_NINJAJUMPLEFTSTATEID__);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_SLIDE:
			setCurrentState (lookingToDirection () == 0 
				? __DRAGONWIND_NINJASLIDERIGHTSTATEID__ : __DRAGONWIND_NINJASLIDELEFTSTATEID__);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_THROW:
			setCurrentState (lookingToDirection () == 0 
				? __DRAGONWIND_NINJATHROWRIGHTSTATEID__ : __DRAGONWIND_NINJATHROWLEFTSTATEID__);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_SWIM:
			{
				setCurrentState (lookingToDirection () == 0 
					? __DRAGONWIND_NINJASWIMRIGHTSTATEID__ : __DRAGONWIND_NINJASWIMLEFTSTATEID__);
				
				// The movement is affected by the density of the water in the scene...
				DRAGONWIND::PlayingScene* scn = dynamic_cast <DRAGONWIND::PlayingScene*> (map () -> scene ());
				assert (scn); // Just in case...
				DRAGONWIND::SwimingPhysicsIncrementalMovement* mov = 
					dynamic_cast <DRAGONWIND::SwimingPhysicsIncrementalMovement*> (currentMovement ());
				assert (mov); // Just in case...
				mov -> setLiquidDensity (scn -> liquidsFluidity ()); 
			}

			break;

		case DRAGONWIND::DragonArtist::DragonState::_DIEING:
			{
				// To die when swiming is different that when running e.g.
				if (cS == DRAGONWIND::DragonArtist::_SWIM) 
					setCurrentState (lookingToDirection () == 0 
						? __DRAGONWIND_NINJASWIMRIGHTDIEINGDSTATEID__ : __DRAGONWIND_NINJASWIMLEFTDIEINGSTATEID__);
				else
					setCurrentState (lookingToDirection () == 0 
						? __DRAGONWIND_NINJADIEINGRIGHTSTATEID__ : __DRAGONWIND_NINJADIEINGLEFTSTATEID__);
			}

			break;

		case DRAGONWIND::DragonArtist::DragonState::_DIED:
			{
				// Final state after dieing is different when swiming than when running e.g.
				if (currentState () -> id () == __DRAGONWIND_NINJASWIMRIGHTDIEINGDSTATEID__ ||
					currentState () -> id () == __DRAGONWIND_NINJASWIMLEFTDIEINGSTATEID__) 
					setCurrentState (lookingToDirection () == 0 
						? __DRAGONWIND_NINJASWIMRIGHTDIEDSTATEID__ : __DRAGONWIND_NINJASWIMLEFTDIEDSTATEID__);
				else
					setCurrentState (lookingToDirection () == 0 
						? __DRAGONWIND_NINJADIEDRIGHTSTATEID__ : __DRAGONWIND_NINJADIEDLEFTSTATEID__);
			}

			break;

		case DRAGONWIND::DragonArtist::DragonState::_HAPPY:
			setCurrentState (lookingToDirection () == 0 
				? __DRAGONWIND_NINJAHAPPYRIGHTSTATEID__ : __DRAGONWIND_NINJAHAPPYLEFTSTATEID__);
			break;

		default:
			assert (0); // It shouldn't be here...
	}
}

// ---
bool DRAGONWIND::Ninja::isPlatformStatePossible (int pS) const
{
	return (DRAGONWIND::Character::isPlatformStatePossible (pS) ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_FLOAT ||
		    platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_CLIMB ||
		    platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_CLIMBFIXED ||
		    platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_SLIDE ||
		    platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_SWIM ||
		    platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_HAPPY);
}

// ---
void DRAGONWIND::Ninja::setPocket (const QGAMES::PlatformGame::Pocket& p)
{
	_pocket = p;

	std::string lOfV;
	std::vector <int> lOfT = _pocket.listOfElementsType ();
	for (int i = 0; i < (int) lOfT.size (); i++)
		lOfV += ((i == 0) ? std::string (__NULL_STRING__) : std::string (",")) + 
			std::to_string (lOfT [i] != -1 ? lOfT [i] : -1);

	QGAMES::SetOfOpenValues oV;
	oV.addOpenValue (__DRAGONWIND_NINJATHINGSCARRIEDVALUEPARAM__, QGAMES::OpenValue (lOfV));
	notify (QGAMES::Event (__DRAGONWIND_NINJATHINGSCARRIEDACTUALIZED__, this, oV));

	// Keeps the pocket...
	DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
	assert (dG); // Just in case...
	dG -> setPocket (pocket ());

	if (isPossibleToShoot ()) 
		reduceShootingsAvailable (0); // To update shootings available!
}

// ---
bool DRAGONWIND::Ninja::carriesTypeInPocket (int t) const
{
	std::vector <int> lOfT = _pocket.listOfElementsType ();
	return (std::find (lOfT.begin (), lOfT.end (), t) != lOfT.end ());
}

// ---
int DRAGONWIND::Ninja::numberOfTypesInPocket (int t) const
{
	std::vector <int> lOfT = _pocket.listOfElementsType ();
	return (std::count (lOfT.begin (), lOfT.end (), t));
}

// ---
void DRAGONWIND::Ninja::setScore (int s)
{
	DRAGONWIND::Character::setScore (s);

	// Keeps the score for the next time...
	DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
	assert (dG); // Just in case...
	dG -> setScore (score ());
}

// ---
bool DRAGONWIND::Ninja::toDie ()
{
	bool result = DRAGONWIND::Character::toDie ();
			
	if (result) 
		game () -> sound (__DRAGONWIND_NINJADIESSOUNDID__) -> play (__QGAMES_MAINCHARACTERSOUNDCHANNEL__);
	// If the player is not in a base, this sound will be heard, otherwise it won't

	// But if the player dies when he / she is not still on a base
	// the current movement should be replaced by falling!!
	if (onOffSwitch (_SWITCHTODIEWHENPOSSIBLE) -> isOn () && !isOnAPlatformState () && !isFalling ())
		toFall ();

	return (result);
}

// ---
bool DRAGONWIND::Ninja::toJump (int i, bool dP)
{
	bool result = DRAGONWIND::Character::toJump (i, dP);

	if (result)
		game () -> sound (__DRAGONWIND_NINJAJUMPSOUNDID__) -> play (__QGAMES_MAINCHARACTERSOUNDCHANNEL__);

	return (result);
}

// ---
bool DRAGONWIND::Ninja::toShoot (int i, bool uP, int tS)
{
	bool result = DRAGONWIND::Character::toShoot (i, uP, tS);

	if (result)
	{
		if (numberShootingsAvailable () <= 0)
			toCatchOrLeaveSomething (); 

		if (isStanding ())
			setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_THROW));

		game () -> sound (__DRAGONWIND_NINJATHOWINGSOUNDID__) -> play (__QGAMES_MAINCHARACTERSOUNDCHANNEL__);
	}

	return (result);
}

// ---
bool DRAGONWIND::Ninja::toSwim (int i, const QGAMES::Vector& dr)
{
	if (!isSwiming ())
		return (false); // Not posible this movement is not swming...

	// If could exit the water in some direction, the movement towards that direction wouldn't be possible
	QGAMES::Position mP (((canExitWaterLeft () && dr.posX () < __BD 0) ||
						  (canExitWaterRight () && dr.posX () > __BD 0)) ? __BD 0 : dr.posX (),
						 ((canExitWaterUp () && dr.posY () < __BD 0) ||
						  (canExitWaterDown () && dr.posY () > __BD 0)) ? __BD 0 : dr.posY (), __BD 0);
	if (mP == QGAMES::Position::_cero)
		return (false); // If there is no movement eventually, then finishes...

	DRAGONWIND::SwimingPhysicsIncrementalMovement* mov =
		dynamic_cast <DRAGONWIND::SwimingPhysicsIncrementalMovement*> (currentMovement ());
	assert (mov); // Just in case...
	QGAMES::Vector dA = mov -> directionInAxis ();
	mP = QGAMES::Vector (mP.posX () * dA.posX (), mP.posY () * dA.posY (), mP.posZ () * dA.posZ ());

	mov -> addImpulse (QGAMES::Vector (__BD 100 * mP.posX (), __BD 100 * mP.posY (), __BD 0), 
		(i > 5 ? 5 : i) * 12); // The impulse is up to 1 second long...
							   // The force of the impulse depnds on the movement, 
							   // and it is also a littl bit up to

	return (true);
}

// ---
bool DRAGONWIND::Ninja::toFloat ()
{ 
	bool result = false;

	if (platformStateToDragonState () == DRAGONWIND::DragonArtist::DragonState::_FALL) // Only when falling...
	{
		setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_FLOAT));

		result = true;
	}

	return (result);
}

// ---
bool DRAGONWIND::Ninja::toFloatDeferred () 
{ 
	bool result = false;

	if (platformStateToDragonState () == DRAGONWIND::DragonArtist::DragonState::_FALL) // Only when falling...
	{
		setPlatformStateDeferred (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_FLOAT));

		result = true;
	}

	return (result);
}

// ---
bool DRAGONWIND::Ninja::toBeHappy ()
{
	if (isHappy ())
		return (false); 

	if (!isOnAPlatformState ())
		return (false); 

	setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_HAPPY));
	setFixBasePosition (); 

	return (true);
}

// ---
bool DRAGONWIND::Ninja::toCatchOrLeaveSomething ()
{
	QGAMES::PlatformGame::PocketElement elementToLeave;

	DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
	assert (dG); // Just in case...

	if (!_thingsAround.empty ())
	{
		DRAGONWIND::Thing* th = dynamic_cast <DRAGONWIND::Thing*> ((*(_thingsAround.begin ())).second);
		assert (th); // Just in case...it shouldn't but who knows...
		assert (th ->  definition ());

		// Catch it and determine what it left add the same time, if any...
		QGAMES::PlatformGame::PocketElement pckElmnt
			(th -> definition () -> _id, 
			 th -> aspectAssociatedToThingType (th -> definition () -> _type), 
			 th ->  definition () -> _otherValues);
		elementToLeave = _pocket.toAddElement (pckElmnt); 
		whenThingIsAddedToPocked (pckElmnt); // some special to do probably in some cases... 
		// The other values could have attributes, e.g. the number of shootings left when it is a gun...
		dG -> removeThingToCatch (map () -> scene () -> id (), th -> definition () -> _id); // No longer in the game...
		th -> setFromNoDefinition (); // No valid description any more...
		th -> setVisible (false); // No longer in the screen...

		game () -> sound (__DRAGONWIND_NINJACATCHSOUNDID__) -> play (__QGAMES_MAINCHARACTERSOUNDCHANNEL__);
	}
	else
		elementToLeave = _pocket.toAddElement (QGAMES::PlatformGame::PocketElement ());

	setPocket (_pocket); 

	if (isPossibleToShoot ()) 
		reduceShootingsAvailable (0); // To update shootings available!
	else
	{
		QGAMES::SetOfOpenValues oV;
		oV.addOpenValue (__DRAGONWIND_ENTITYSHOOTINGSVALIEPARAM__, QGAMES::OpenValue (0));
		notify (QGAMES::Event (__DRAGONWIND_ENTITYSHOOTINGSACTUALIZED__, this, oV)); // ...or 0 is not!
	}

	// ...and leave if there is something!
	// To leave something has to be processed by the scene (which is the one with the list of sprites frees if any)
	if (elementToLeave._id != -1)
		notify (QGAMES::Event (__DRAGONWIND_THINGLEFTEVENT__, this, elementToLeave.likeSetOfOpenValues ()));

	return (true);
}

// ---
void DRAGONWIND::Ninja::whenGoalReached (int nG)
{ 
	DRAGONWIND::Game* g = dynamic_cast <DRAGONWIND::Game*> (game ());
	assert (g); // Just in case...

	DRAGONWIND::Scene* scn = dynamic_cast <DRAGONWIND::Scene*> (map () -> scene ());
	assert (scn);

	// Only happy when the goal reached is the main one, the scene is fundamental 
	// and also the scene hasn't been completed yet!
	if (_goalReached == __DRAGONWIND_MAINGOAL__ &&
		(scn -> fundamental () && !g -> sceneCompleted (scn -> id ())))
	{
		toStand (lookingToDirection ());
		toBeHappy ();
	}
}

// ---
void DRAGONWIND::Ninja::whenWheatherCondition (QGAMES::EnvironmentalCondition* eC)
{
	DRAGONWIND::Character::whenWheatherCondition (eC);

	if (dynamic_cast <QGAMES::Rain*> (eC))
		setEnergyToAdd (-(((QGAMES::Rain*) eC) -> currentDensity () * __BD 0.5));
	else
	if (dynamic_cast <QGAMES::Wind*> (eC))
		setEnergyToAdd (-(((QGAMES::Wind*) eC) -> currentForce () * __BD 0.5));
}

// ---
void DRAGONWIND::Ninja::initialize ()
{
	DRAGONWIND::Character::initialize ();

	DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
	assert (dG); // Just in case...

	setEnergy (dG -> playerEnergy ());

	setPocket (dG -> pocket ());

	setScore (dG -> score ());

	if (isPossibleToShoot ())
		reduceShootingsAvailable (0); // Just to actualize shooting available!

	_goalReached = -1; 
}

// ---
void DRAGONWIND::Ninja::updatePositions ()
{
	if (isThrowing () && currentAnimation () -> end ())
		toStand (lookingToDirection ());

	// When carrying a thunder, lightning can't hurt the ninja
	if (!(behaviour () -> _canBeHurmedByLightning = !carriesTypeInPocket (__DRAGONWIND_NINJATHINGSTHUNDERTYPE__)))
	{
		if (!onOffSwitch (_SWITCHSOUNDENERGY) -> isOn ())
		{
			onOffSwitch (_SWITCHSOUNDENERGY) -> set (true);
			game () -> sound (__DRAGONWIND_SHIELDSOUNDID__) -> play (__DRAGONWIND_MAINARTISTSECONDCHANNEL__);
		}
	}
	else
	{
		if (onOffSwitch (_SWITCHSOUNDENERGY) -> isOn ())
		{
			onOffSwitch (_SWITCHSOUNDENERGY) -> set (false);
			game () -> sound (__DRAGONWIND_SHIELDSOUNDID__) -> stop ();
		}
	}

	if (counter (_COUNTERSHIELDTIMER) -> isEnd ())
		counter (_COUNTERSHIELDASPECT) -> isEnd ();

	if (onOffSwitch (_SWITCHTOREACHGOALWHENPOSSIBLE) -> isOn () && isOnAPlatformState ())
	{
		onOffSwitch (_SWITCHTOREACHGOALWHENPOSSIBLE) -> set (false);

		whenGoalReached (_goalReached);

		QGAMES::SetOfOpenValues oV;
		oV.addOpenValue (__DRAGONWIND_NUMBERGOALPARAM__, QGAMES::OpenValue (_goalReached));
		notify (QGAMES::Event (__DRAGONWIND_NINJAREACHEDGOAL__, this, oV));
	}

	DRAGONWIND::Character::updatePositions ();

	_thingsAround = QGAMES::Entities ();
}

// ---
void DRAGONWIND::Ninja::finalize ()
{
	DRAGONWIND::Character::finalize ();

	if (onOffSwitch (_SWITCHSOUNDENERGY) -> isOn ())
	{
		onOffSwitch (_SWITCHSOUNDENERGY) -> set (false);
		game () -> sound (__DRAGONWIND_SHIELDSOUNDID__) -> stop ();
	}

	DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
	assert (dG);

	dG -> setPocket (pocket ()); 

	dG -> setPlayerEnergy (_energy);
}

// ---
void DRAGONWIND::Ninja::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	static QGAMES::Color c = __QGAMES_WHITECOLOR__;

	DRAGONWIND::Character::drawOn (s, p);

	if (carriesTypeInPocket (__DRAGONWIND_NINJATHINGSTHUNDERTYPE__))
	{
		QGAMES::Position cT = centerPosition ();
		c.setAlpha (_SHIELDINTENSITY [counter (_COUNTERSHIELDASPECT) -> value ()]);
		for (int i = 0; i < 3; i++)
			s -> drawCircle (cT, QGAMES::Vector::_zNormal, __BD (50 + i), __BD (50 + i), c, false);
	}
}

// ---
void DRAGONWIND::Ninja::whenCollisionWith (QGAMES::Entity* e)
{
	if (!isAlive ())
		return;

	DRAGONWIND::Character::whenCollisionWith (e);

	// Eating something...
	DRAGONWIND::Food* fD = dynamic_cast <DRAGONWIND::Food*> (e);
	if (fD)
	{
		// Increases the energy in the same "level" than the fruit...
		setEnergyToAdd (__BD (fD -> energyLevel () + 1)); 
		// The number of points added will also depend on the "level" of the fruit...
		setScore (score () + (100 * (fD -> energyLevel () + 1)));

		game () -> sound (__DRAGONWIND_NINJAEATSOUNDID__) -> play (__QGAMES_MAINCHARACTERSOUNDCHANNEL__);
	}

	// Is he/she over a thing to be caught? Then it is taken into account just if the player wants it!
	DRAGONWIND::Thing* tD = dynamic_cast <DRAGONWIND::Thing*> (e);
	if (tD) // Adds the element to the list of those around the entity...
		_thingsAround [tD -> id ()] = tD;

	// Have I received an impact from an enemy's shooting? (added to the behaviour defined at parent level)
	DRAGONWIND::Shooting* sth = dynamic_cast <DRAGONWIND::Shooting*> (e);
	if (sth && sth -> owner () != NULL && sth -> owner () != this) // Exists and the owner is not me...
	{
		if ((behaviour () -> _canBeHurmedByEnemies && isEnemy (sth -> owner ()) ||
			 behaviour () -> _canBeHurmedBySimilars && !isEnemy (sth -> owner ())) &&
			_behaviour -> canBeHurmedBy (sth -> definition () -> _type)) // supports the specific type...
			game () -> sound (__DRAGONWIND_NINJAHITSOUNDID__) -> play (__QGAMES_MAINCHARACTERSOUNDCHANNEL__);
		// The rest of action will have been taken into the parent class...
	}

	// Is there a collision with another character?...
	// What happen will depend on whether it is a similar or not...
	DRAGONWIND::Character* chr = dynamic_cast <DRAGONWIND::Character*> (e);
	if (chr && chr -> isAlive ())
	{
		if ((dynamic_cast <DRAGONWIND::BadGuy*> (chr) && behaviour () -> _canBeHurmedByEnemies) ||
			(dynamic_cast <DRAGONWIND::Ninja*> (chr) && behaviour () -> _canBeHurmedBySimilars))
			setEnergyToAdd (__BD -chr -> behaviour () -> _shootingEnergy); //...reduces the energy in the amount defined
	}
}

// ---
void DRAGONWIND::Ninja::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __QGAMES_MOVEMENTSPEEDLIMITREACHED__ && isFalling ())
		toFloatDeferred ();
	else
		DRAGONWIND::Character::processEvent (evnt);
}

// ---
__IMPLEMENTCOUNTERS__ (DRAGONWIND::Ninja::Counters)
{
	addCounter (new QGAMES::Counter 
		(DRAGONWIND::Ninja::_COUNTERSHIELDTIMER, 2, 0, true, true));
	addCounter (new QGAMES::Counter 
		(DRAGONWIND::Ninja::_COUNTERSHIELDASPECT, 
			sizeof (_SHIELDINTENSITY) / sizeof (_SHIELDINTENSITY [0]), 0, true, true));
}

// ---
void DRAGONWIND::Ninja::afterAllLayersTestedInWhenOnTile ()
{
	// Jumping when happy is an ethernal situation...
	// It happens again and again always that the previous finishes
	// The only way to exit this situation is because the game controller decide it so!!
	if (_layerBase && isHappy ())
	{
		toStand (lookingToDirection ());
		toBeHappy (); // If the ninja is happy he/she to continue being...
					  // The state never finished, because is the game (above) ho controls when...
	}
	else
		DRAGONWIND::Character::afterAllLayersTestedInWhenOnTile ();
}

// ---
int DRAGONWIND::Ninja::animationIdForState (DRAGONWIND::DragonArtist::DragonState st, int lD)
{
	int result = -1;

	switch (st)
	{
		case DRAGONWIND::DragonArtist::DragonState::_STAND:
			result = state ((lD == 0) 
					? __DRAGONWIND_NINJASTANDRIGHTSTATEID__ : __DRAGONWIND_NINJASTANDLEFTSTATEID__) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_RUN:
			result = state ((lD == 0) 
					? __DRAGONWIND_NINJARUNNINGRIGHTSTATEID__ : __DRAGONWIND_NINJARUNNINGLEFTSTATEID__) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_RUNATTACK: 
			break;

		case DRAGONWIND::DragonArtist::DragonState::_CLIMB:
			result = state ((lD == 0) 
					? __DRAGONWIND_NINJACLIMBINGRIGHTSTATEID__ : __DRAGONWIND_NINJACLIMBINGLEFTSTATEID__) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_CLIMBFIXED:
			result = state ((lD == 0) 
					? __DRAGONWIND_NINJACLIMBFIXEDRIGHTSTATEID__ : __DRAGONWIND_NINJACLIMBFIXEDLEFTSTATEID__) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_FALL:
			result = state ((lD == 0) 
					? __DRAGONWIND_NINJAFALLRIGHTSTATEID__ : __DRAGONWIND_NINJAFALLLEFTSTATEID__) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_FLOAT:
			result = state ((lD == 0) 
					? __DRAGONWIND_NINJAGLADERIGHTSTATEID__ : __DRAGONWIND_NINJAGLADELEFTSTATEID__) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_JUMP:
			result = state ((lD == 0) 
					? __DRAGONWIND_NINJAJUMPRIGHTSTATEID__ : __DRAGONWIND_NINJAJUMPLEFTSTATEID__) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_SLIDE:
			result = state ((lD == 0) 
					? __DRAGONWIND_NINJASLIDERIGHTSTATEID__ : __DRAGONWIND_NINJASLIDELEFTSTATEID__) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_SWIM:
			result = state ((lD == 0)
					? __DRAGONWIND_NINJASWIMRIGHTSTATEID__ : __DRAGONWIND_NINJASWIMLEFTSTATEID__) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_THROW:
			result = state ((lD == 0)
					? __DRAGONWIND_NINJATHROWRIGHTSTATEID__ : __DRAGONWIND_NINJATHROWLEFTSTATEID__) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_DIEING:
			result = state ((lD == 0) 
					? __DRAGONWIND_NINJADIEINGRIGHTSTATEID__ : __DRAGONWIND_NINJADIEINGLEFTSTATEID__) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_DIED:
			result = state ((lD == 0) 
					? __DRAGONWIND_NINJADIEDRIGHTSTATEID__ : __DRAGONWIND_NINJADIEDLEFTSTATEID__) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_HAPPY:
			result = state ((lD == 0) 
					? __DRAGONWIND_NINJAHAPPYRIGHTSTATEID__ : __DRAGONWIND_NINJAHAPPYLEFTSTATEID__) -> animationId ();
			break;

		default:
			assert (0); // It shouldn't be here at all...
	}

	return (result);
}

// ---
bool DRAGONWIND::Ninja::isPossibleToShoot () const
{
	return (!isSwiming () && 
			(lastPocketElement ()._type == __DRAGONWIND_NINJATHINGARROWTYPE__ || 
			 lastPocketElement ()._type == __DRAGONWIND_NINJATHINGSTARTYPE__ || 
			 lastPocketElement ()._type == __DRAGONWIND_NINJATHINGSBOMBTYPE__)); 
}

// ---
int DRAGONWIND::Ninja::typeOfShootingAvailable () const
{
	assert (isPossibleToShoot ()); 

	return (lastPocketElement ()._type);
}

// ---
int DRAGONWIND::Ninja::numberShootingsAvailable () const
{
	assert (isPossibleToShoot ()); 

	return (lastPocketElement ()._values.openValue (__DRAGONWIND_GUNSTOCATCHNUMBERSHOOTS__).intValue ());
}

// ---
void DRAGONWIND::Ninja::reduceShootingsAvailable (int nS)
{
	assert (isPossibleToShoot ()); 

	int s = numberShootingsAvailable () - nS; if (s < 0) s = 0; 

	lastPocketElement ()._values.removeOpenValue (__DRAGONWIND_GUNSTOCATCHNUMBERSHOOTS__);
	lastPocketElement ()._values.addOpenValue (__DRAGONWIND_GUNSTOCATCHNUMBERSHOOTS__, QGAMES::OpenValue (s));

	QGAMES::SetOfOpenValues oV;
	oV.addOpenValue (__DRAGONWIND_ENTITYSHOOTINGSVALIEPARAM__, QGAMES::OpenValue (s));
	notify (QGAMES::Event (__DRAGONWIND_ENTITYSHOOTINGSACTUALIZED__, this, oV));
}

// ---
void DRAGONWIND::Ninja::whenThingIsAddedToPocked (const QGAMES::PlatformGame::PocketElement& pElmnt)
{
	// When a heart is taken, the number of lives stord in are 
	// added to ninja's status up to the maximum...
	// and also the energy level is refulled up to the maximum...
	if (pElmnt._type == __DRAGONWIND_NINJATHINGHEARTTYPE__)
	{
		assert (_pocket.firstElement ()._type == pElmnt._type); // Just in case...

		DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
		assert (dG);

		int lAdd = 
			_pocket.firstElement ()._values.openValue (__DRAGONWIND_HEARTTOCATCHNUMBERLIVES__).intValue ();
		if (lAdd > 0)
		{
			dG -> setLives (dG -> lives () + lAdd);
			setEnergyToAdd (100); 

			_pocket.firstElement ()._values.removeOpenValue (__DRAGONWIND_HEARTTOCATCHNUMBERLIVES__);
			_pocket.firstElement ()._values.addOpenValue 
				(__DRAGONWIND_HEARTTOCATCHNUMBERLIVES__, QGAMES::OpenValue (0));

			QGAMES::SetOfOpenValues oV;
			oV.addOpenValue (__DRAGONWIND_NINJALIVESVALUEPARAMETER__, QGAMES::OpenValue (dG -> lives ()));
			notify (QGAMES::Event (__DRAGONWIND_NINJALIVESACTUALIZED__, this, oV));
		}
	}
}
