#include <DragonwindLike/dwshootings.hpp>

// ---
DRAGONWIND::Shooting::Shooting (int cId, const QGAMES::Forms& f, const QGAMES::PlatformArtist::Data& d)
	: DRAGONWIND::DragonArtist (cId, f, d),
	  _impulse (1),
	  _behaviour (NULL),
	  _statesId (),
	  _owner (NULL) // By default, take care to set it before any movement...
{ 
	// Register an additional buoy to the one inserted previously at the entity constructor...
	// This buoy is to execute tasks on the shooting attending to its aspect!
	assert (!buoys ().empty ()); // Just in case...
	addBuoy (__DRAGONWIND_STHTASKEXECDEFEREDBUOYID__, new DRAGONWIND::Shooting::ToExecuteTaskOverBehaviourBuoy);
}

// ---
QGAMES::Entity* DRAGONWIND::Shooting::clone () const
{
	QGAMES::Entity* result = new DRAGONWIND::Shooting (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void DRAGONWIND::Shooting::setPlatformState (int id)
{
	int cS = platformState ();
	DRAGONWIND::DragonArtist::setPlatformState (id);
	if (platformState () == cS || id == -1) // -1 means not necessary...
		return; // Nothing has changed...

	assert (isPlatformStatePossible (id));

	// Up direction is the same than to the left...
	// Always continue with the same animation than the previous one if possible!!
	// This is becuase it supossed that the shooting is always a thing!
	switch (platformStateToDragonState ())
	{
		case DRAGONWIND::DragonArtist::DragonState::_STAND:
			setCurrentState (lookingToDirection () == 0 ? _statesId._standingRight: _statesId._standingLeft, true);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_INTHEAIR:
			setCurrentState (lookingToDirection () == 0 ? _statesId._flyingRight : _statesId._flyingLeft, true);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_THROW:
			setCurrentState (lookingToDirection () == 0 ? _statesId._bouncingRight : _statesId._bouncingLeft, true);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_ROLL:
			setCurrentState (lookingToDirection () == 0 ? _statesId._rollingRight : _statesId._rollingLeft, true); 
			break;

		case DRAGONWIND::DragonArtist::DragonState::_FALL:
			setCurrentState (lookingToDirection () == 0 ? _statesId._fallingRight : _statesId._fallingLeft, true);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_DIEING:
			setCurrentState (_statesId._exploding);
			break;

		default:
			assert (0); // It shouldn't be here...
	}
}

// ---
bool DRAGONWIND::Shooting::isPlatformStatePossible (int pS) const
{
	return (platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_STAND ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_INTHEAIR ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_THROW ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_FALL ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_ROLL ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_DIEING);
}

// ---
void DRAGONWIND::Shooting::toLaunchFrom (DRAGONWIND::Character* chr, int o, int f)
{
	if (isVisible ())
		return; // Imposible to launch again when the shooting is already "moving" (it is supossed!)

	setVisible (true);

	_owner = chr;

	assert (_behaviour); // It is mandatory...

	setLookingToDirection (o);

	QGAMES::SetOfOpenValues oV;
	oV.addOpenValue (__DRAGONWIND_STHBHVTSKSTARTTOFLYPOWERPRM__, QGAMES::OpenValue (__BD f));
	// This method is always invoked from another point different than the one 
	// processing an event comming from the shooting itself, so a defered invocation is not needed at all!
	executeActionOverBehaviour (__DRAGONWIND_STHBHVTSKSTARTTOFLY__, oV); 

	_impulse = f;
}

// ---
void DRAGONWIND::Shooting::toBeThrown (bool b)
{
	if ((!b && isInTheAir ()) || (b && isThrowing ()))
		return; // Only once

	if (!b) 
		setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_INTHEAIR));
	else
	{
		setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_THROW));

		QGAMES::PhysicsIncremetalMovement* m = 
			dynamic_cast <QGAMES::PhysicsIncremetalMovement*> (currentMovement ());
		assert (m);
		m -> setMinSpeed (__BD 30); // When reached a __QGAMES_NOTBOUNCINGMOVEMENTANYMORE__ will be issued...
	}

	setFixBasePosition (); // No base position defined when throwing...
}

// ---
void DRAGONWIND::Shooting::toRoll ()
{
	if (isRolling ())
		return;

	// First stops...
	setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_STAND));
	// To avoid to inherit parameters fromthe previous movement...
	setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_ROLL));

	// Modify the speed of the shooting attending to the impulse...
	setMove (direction () * factorForImpulse (__BD _impulse), acceleration ());

	setFixBasePosition (QGAMES::Position (basePosition ().posX (),
		_layerBasePosition.posY () + __BD 5, __BD 0)); // A little bit in the base...
	adjustPositionToBase ();
}

// ---
void DRAGONWIND::Shooting::toExplode ()
{
	if (isDieing () || isDied ())
		return; // Only once...

	QGAMES::Position cP = centerPosition ();
	setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_DIEING));
	adjustPositionToCenter (cP); // In this case the position has to be aligned with the old center...

	notify ((QGAMES::Event (__DRAGONWIND_SHOOTINGTOEXPLODE__, this))); 

	// Sounds if visible or close to be visible...
	if (game () -> mainScreen () -> isVisible 
		(QGAMES::Rectangle (cP - QGAMES::Vector (__BD 200, __BD 200, __BD 0), 
							cP + QGAMES::Vector (__BD 200, __BD 200, __BD 0))))
		game () -> sound (__QGAMES_EXPLOSIONWAVSOUND__) -> play (__QGAMES_GAMESOUNDCHANNEL__); 
}

// ---
void DRAGONWIND::Shooting::updatePositions ()
{
	if (!isVisible ())
		return; // It has to be visible...

	if (currentAnimation () && currentAnimation () -> end ())
		executeActionOverBehaviour (__DRAGONWIND_STHBHVTSKLASTSTATE__);
	// When in some of the states the animation is not cyclic and it finishes, then go to last state
	// e.g this happen when the shooting explode. The explosion is not cyclic, 
	// so when it finishes, the shooting finishes too...

	DRAGONWIND::DragonArtist::updatePositions ();
}

// ---
void DRAGONWIND::Shooting::whenCollisionWith (QGAMES::Entity* e)
{
	DRAGONWIND::Character* chr = dynamic_cast <DRAGONWIND::Character*> (e);
	if (chr && _owner != NULL && _owner != chr) // Has a shooting hit me and it doesn't belong to me?
	{
		if (_statesId._hurm &&
			chr -> behaviour () -> _canBeHurmedByEnemies && _owner -> isEnemy (chr) ||
			chr -> behaviour () -> _canBeHurmedBySimilars && !_owner -> isEnemy (chr))
		{
			setFromNoDefinition ();
			setVisible (false); // The end of its life!!
		}
	}
}

// ---
void DRAGONWIND::Shooting::setFromDefinition (QGAMES::PlatformGame::ElementDefinition* eD)
{
	delete (_definition); // The old (possible) definition os not longer valid...
	DRAGONWIND::DragonArtist::setFromDefinition (eD);

	assert (eD);

	// Atten ding to the type of shooting...
	// ..the states are set
	_statesId = statesIdForType (eD -> _type);
	assert (_statesId._standingLeft != -1);
	// ...and also the behaviour!
	delete (_behaviour); 
	_behaviour = behaviourForType (eD -> _type);
	assert (_behaviour);
}

// ---
void DRAGONWIND::Shooting::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __QGAMES_MOVEMENTNOTPOSSIBLE__) // Movement is not possible...
	{
		if (!dynamic_cast <QGAMES::BouncePhysicsIncrementalMovement*> (currentMovement ())) // ...but it is not bouncing!
			executeActionOverBehaviourDeferred (__DRAGONWIND_STHBHVTSKSTOPPED__);
	}
	else 
	if (evnt.code () == __QGAMES_NOTBOUNCINGMOVEMENTANYMORE__) // Stop bouncing...
		executeActionOverBehaviourDeferred (__DRAGONWIND_STHBHVTSKSTOPPED__); // Now it can proceed!!
	else
	if (evnt.code () == __QGAMES_BOUNCINGMOVEMENT__ && 
		_lastLayerBase != NULL && _lastLayerBase -> movement () != NULL) // Bouncing on a base in movement...
	{
		const QGAMES::Vector& cM = ((QGAMES::AdvancedTileLayer*) _lastLayerBase) -> 
			movement () -> averageVectorMoved ();
		if (cM != QGAMES::Vector::_cero)
		{
			QGAMES::BouncePhysicsIncrementalMovement* cV =
				dynamic_cast <QGAMES::BouncePhysicsIncrementalMovement*> (currentMovement ());
			assert (cV);

			QGAMES::bdata fX = __BD 1; 
			if (cM.posX () != __BD 0)
				if (cM.posX () * cV -> currentSpeed ().posX () > __BD 0)
					fX /= __BD 1 + ((cM.posX () < __BD 1) ? __BD 1 : cM.posX ());
				else fX *= __BD 1 + ((cM.posX () < __BD 1) ? __BD 1 : cM.posX ());
			if (fX > __BD 1.8) fX = __BD 1.8; if (fX < __BD 0.5) fX = __BD 0.5;
			QGAMES::bdata fY = __BD 1; 

			if (cM.posY () != __BD 0)
				if (cM.posY () > __BD 0 && cV -> currentSpeed ().posY () > __BD 0)
					fY /= __BD 1 + ((cM.posY () < __BD 1) ? __BD 1 : cM.posY ());
				else fY *= __BD 1 + ((cM.posY () < __BD 1) ? __BD 1 : cM.posY ());
			if (fY > __BD 1.8) fY = __BD 1.8; if (fY < __BD 0.5) fY = __BD 0.5;

			QGAMES::bdata fZ = __BD 1; 
			if (cM.posZ () != __BD 0)
				if (cM.posZ () > __BD 0 && cV -> currentSpeed ().posZ () > __BD 0) 
					fZ /= __BD 1 + ((cM.posZ () < __BD 1) ? __BD 1 : cM.posZ ());
				else fZ *= __BD 1 + ((cM.posZ () < __BD 1) ? __BD 1 : cM.posZ ());
			if (fZ > __BD 1.8) fZ = __BD 1.8; if (fZ < __BD 0.5) fZ = __BD 0.5;

			cV -> setSpeed (QGAMES::Vector (cV -> currentSpeed ().posX () * fX,
											cV -> currentSpeed ().posY () * fY,
											cV -> currentSpeed ().posZ () * fZ));
		}
	}
	else
		DRAGONWIND::DragonArtist::processEvent (evnt);
}

// ---
void DRAGONWIND::Shooting::afterAllLayersTestedInWhenOnTile ()
{
	if (_layerBase)
	{
		if (!isOnAPlatformState ())
			executeActionOverBehaviourDeferred (__DRAGONWIND_STHBHVTSKONPLATFORM__);
	}
	else
	if (isOnAPlatformState ()) // To fall, because a hole has been found!
		toFall ();
}

// ---
void DRAGONWIND::Shooting::Behaviour::executeTask (int tsk, const QGAMES::SetOfOpenValues& oV)
{
	switch (tsk)
	{
		case __DRAGONWIND_STHBHVTSKSTARTTOFLY__:
			whenStartedToFly (oV.openValue (__DRAGONWIND_STHBHVTSKSTARTTOFLYPOWERPRM__).bdataValue ());
			break;

		case __DRAGONWIND_STHBHVTSKONPLATFORM__:
			whenOnPlatform ();
			break;

		case __DRAGONWIND_STHBHVTSKHITBASE__:
			whenHitBase ();
			break;

		case __DRAGONWIND_STHBHVTSKSTOPPED__:
			whenStopped ();
			break;

		case __DRAGONWIND_STHBHVTSKLASTSTATE__:
			whenFinishingLastState ();
			break;

		default:
			assert (0); // It shouldn't be here...
	}
}

// ---
void DRAGONWIND::Shooting::Behaviour::whenFinishingLastState ()
{ 
	_shooting -> notify ((QGAMES::Event (__DRAGONWIND_SHOOTINGTOEND__, _shooting)));

	_shooting -> setFromNoDefinition ();
	_shooting -> setVisible (false); // Ready to be reused...
}

// ---
void DRAGONWIND::Shooting::executeActionOverBehaviour (int tsk, const QGAMES::SetOfOpenValues& oV)
{
	assert (_behaviour);

	_behaviour -> executeTask (tsk, oV);
}

// ---
void DRAGONWIND::Shooting::executeActionOverBehaviourDeferred (int tsk, const QGAMES::SetOfOpenValues& oV)
{
	DRAGONWIND::Shooting::ToExecuteTaskOverBehaviourBuoy* b = 
		dynamic_cast <DRAGONWIND::Shooting::ToExecuteTaskOverBehaviourBuoy*> (buoy (__DRAGONWIND_STHTASKEXECDEFEREDBUOYID__));
	assert (b); // Just in case...

	b -> setTask (tsk);
	b -> setTaskData (oV);
	b -> active (true); // Activate to change the state in the next loop...
}

// ---
void DRAGONWIND::Shooting::JustFly::whenStartedToFly (QGAMES::bdata f)
{ 
	_shooting -> setCurrentMovement (__MININT__); // No previous movement, just in case...

	_shooting -> toBeThrown (false); // Not bouncing...
	
	_shooting -> setMove 
		(_shooting -> direction () * _shooting -> factorForImpulse (f), _shooting -> acceleration ());
} 

// ---
void DRAGONWIND::Shooting::FlyAndBounce::whenStartedToFly (QGAMES::bdata f)
{ 
	_shooting -> setCurrentMovement (__MININT__); // No previous movement, just in case...

	_shooting -> toBeThrown (true); // Bouncing...

	_shooting -> setMove 
		(_shooting -> direction () * _shooting -> factorForImpulse (f), _shooting -> acceleration ());
}

// ---
QGAMES::bdata DRAGONWIND::Shooting::factorForImpulse (QGAMES::bdata i)
{
	QGAMES::bdata result = i;

	if (result < __BD 1) result = __BD 1;
	if (result > __BD 12) result = __BD 12;
	result = ((result / __BD 12) + __BD 1) / _statesId._weightCoef;

	return (result);
}

// ---
DRAGONWIND::Shooting::StatesId DRAGONWIND::Shooting::statesIdForType (int t)
{
	DRAGONWIND::Shooting::StatesId result;

	switch (t)
	{
		case __DRAGONWIND_NINJATHINGARROWTYPE__:
			result = __DRAGONWIND_ARROWSHOOTINGSTATESID__;
			break;

		case __DRAGONWIND_NINJATHINGSTARTYPE__:
			result = __DRAGONWIND_STARSHOOTINGSTATESID__;
			break;

		case __DRAGONWIND_NINJATHINGSBOMBTYPE__:
			result = __DRAGONWIND_BOMBSHOOTINGSTATESID__;
			break;

		case __DRAGONWIND_BADGUYTHINGPOLLBALLTYPE__:
			result = __DRAGONWIND_POLLBALLSHOOTINGSTATESID__;
			break;

		default:
			break;
	}

	return (result);
}

// ---
DRAGONWIND::Shooting::Behaviour* DRAGONWIND::Shooting::behaviourForType (int t)
{
	DRAGONWIND::Shooting::Behaviour* result = NULL;

	switch (t)
	{
		case __DRAGONWIND_NINJATHINGARROWTYPE__:
			result = new DRAGONWIND::Shooting::JustFly (this);
			break;

		case __DRAGONWIND_NINJATHINGSTARTYPE__:
			result = new DRAGONWIND::Shooting::JustFly (this);
			break;

		case __DRAGONWIND_NINJATHINGSBOMBTYPE__:
			result = new DRAGONWIND::Shooting::FlyAndBounceAndExplode (this);
			break;

		case __DRAGONWIND_BADGUYTHINGPOLLBALLTYPE__:
			result = new DRAGONWIND::Shooting::FlyAndRoll (this);
			break;

		default:
			break;
	}

	return (result);
}
