#include <Dragonwindlike/dwbadguys.hpp>

// ---
QGAMES::Entity* DRAGONWIND::BadGuy::clone () const
{
	QGAMES::Entity* result = new DRAGONWIND::BadGuy (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void DRAGONWIND::BadGuy::setPlatformState (int id)
{
	int cS = platformState ();
	DRAGONWIND::DragonArtist::setPlatformState (id);
	if (platformState () == cS || id == -1) // -1 means not necessary...
		return; // Nothing has changed...

	assert (isPlatformStatePossible (id));

	switch (platformStateToDragonState ())
	{
		case DRAGONWIND::DragonArtist::DragonState::_STAND:
			setCurrentState (lookingToDirection () == 0 ? _statesId._standingRight: _statesId._standingLeft);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_RUN:
			setCurrentState (lookingToDirection () == 0 ? _statesId._runningRight : _statesId._runningLeft);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_JUMP:
			setCurrentState (lookingToDirection () == 0 ? _statesId._jumpingRight : _statesId._jumpingLeft);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_FALL:
			setCurrentState (lookingToDirection () == 0 ? _statesId._fallingRight : _statesId._fallingLeft);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_SLIDE:
			setCurrentState (lookingToDirection () == 0 ? _statesId._slidingRight : _statesId._slidingLeft);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_DIEING:
			setCurrentState (lookingToDirection () == 0 ? _statesId._dieingRight : _statesId._dieingLeft);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_DIED:
			setCurrentState (lookingToDirection () == 0 ? _statesId._diedRight : _statesId._diedLeft);
			break;

		default:
			assert (0); // It shouldn't be here...
	}
}

// ---
bool DRAGONWIND::BadGuy::isPlatformStatePossible (int pS) const
{
	return (platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_STAND ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_RUN ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_JUMP ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_FALL ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_SLIDE ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_DIEING ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_DIED);
}

// ---
void DRAGONWIND::BadGuy::setFromDefinition (QGAMES::PlatformGame::ElementDefinition* eD)
{
	assert (eD);

	// To fix the states has to be the first thing to do...
	_statesId = statesIdForType (eD -> _type);
	assert (_statesId._standingRight != -1);

	DRAGONWIND::Character::setFromDefinition (eD);

	// The behaviour of the bad guy is defined in the variable _otherValues of the definition
	// When the bad guy definition is created in the scene (@see ptsenes.cpp) a bad definition object
	// in the TMX map is also looked for. 
	// If it is found, the parameters there, are kept into _otherValues of the definition
	setBehaviour (new DRAGONWIND::Character::Behaviour (definition () -> _otherValues.setOfOpenValues (0)));

	setLookingToDirection (definition () -> _orientation.posX () >= __BD 0 ? 0 : 1); // 0 Means looking to the right...

	setPlatformState (definition () -> _state == -1 
		? dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_STAND)
		: definition () -> _state);

	// The fix base position is set on the parent class
	// But if at this point the entity is in a platform state and still with no fix base position, 
	// onew by default is set!
	if (isOnAPlatformState () && fixBasePosition () == QGAMES::Position::_noPoint)
		setFixBasePosition (basePosition ());
}

// ---
bool DRAGONWIND::BadGuy::toDie ()
{
	addControlStepsMonitor (NULL); // That's enough!

	return (DRAGONWIND::Character::toDie ());
}

// ---
bool DRAGONWIND::BadGuy::toShoot (int i, bool uP, int tS)
{
	bool result = DRAGONWIND::Character::toShoot (i, uP, tS);

	if (result)
		game () -> sound (__DRAGONWIND_BADGUYTHROWINGSOUNDID__) -> play (__QGAMES_ARTISTSOUNDCHANNEL__);
		// Throwing sound using a different channel than the main character...

	return (result);
}

// ---
void DRAGONWIND::BadGuy::finalize ()
{
	DRAGONWIND::Character::finalize ();

	// It has no longer to exist...
	addControlStepsMonitor (NULL);
}

// ---
void DRAGONWIND::BadGuy::whenCollisionWith (QGAMES::Entity* e)
{
	DRAGONWIND::Character::whenCollisionWith (e);

	// Have I received an impact from a ninja's shooting? (added to the behaviour defined at parent level)
	DRAGONWIND::Shooting* sth = dynamic_cast <DRAGONWIND::Shooting*> (e);
	if (sth && sth -> owner () != NULL && sth -> owner () != this) // Exists and the owner is not me...
	{
		if (behaviour () -> _canBeHurmedByEnemies && isEnemy (sth -> owner ()) ||
			behaviour () -> _canBeHurmedBySimilars && !isEnemy (sth -> owner ()))
			game () -> sound (__DRAGONWIND_BADGUYHITSOUNDID__) -> play (__QGAMES_ARTISTSOUNDCHANNEL__);
		// The rest of action will have been taken into the parent class...
	}

	// Is there a collision with another character?...
	// What happen will depend on whether it is a similar or not...
	DRAGONWIND::Character* chr = dynamic_cast <DRAGONWIND::Character*> (e);
	if (chr && chr -> isAlive ())
	{
		if ((dynamic_cast <DRAGONWIND::Ninja*> (chr) && behaviour () -> _canBeHurmedByEnemies) ||
			(dynamic_cast <DRAGONWIND::BadGuy*> (chr) && behaviour () -> _canBeHurmedBySimilars))
			setEnergyToAdd (__BD -chr -> behaviour () -> _shootingEnergy); //...reduces the energy in the amount defined
	}
}

// ---
void DRAGONWIND::BadGuy::processEvent (const QGAMES::Event& evnt)
{
	// Steps are sent up!...
	if (evnt.code () == __QGAMES_CHRSTEPSMONITORFIRSTSTEP__ ||
		evnt.code () == __QGAMES_CHRSTEPSMONITORSTARTSBACK__ ||
		evnt.code () == __QGAMES_CHRSTEPSMONITOROTHERSTEP__ ||
		evnt.code () == __QGAMES_CHRSTEPSMONITORLASTSTEP__)
		notify (QGAMES::Event (evnt.code (), this)); 

	DRAGONWIND::Character::processEvent (evnt);
}

// ---
void DRAGONWIND::BadGuy::adjustRelevants (QGAMES::bdata& m, QGAMES::Vector& pv)
{ 
	// The changes in the size of the robot frames are so big than it is needed to 
	// extend the influence zone when calculating the tile ocuppied by the entity...
	if (definition () -> _type == __DRAGONWIND_ROBOTTYPEID__) m = __BD 1; 
	else m = __BD 0.5;
	pv = QGAMES::Vector::_cero;
}

// ---
DRAGONWIND::BadGuy::StatesId DRAGONWIND::BadGuy::statesIdForType (int t)
{
	DRAGONWIND::BadGuy::StatesId result;

	switch (t)
	{
		case __DRAGONWIND_CATTYPEID__:
			result = __DRAGONWIND_CATSTATESID__; 
			break;

		case __DRAGONWIND_DOGTYPEID__:
			result = __DRAGONWIND_DOGSTATESID__; 
			break;

		case __DRAGONWIND_ROBOTTYPEID__:
			result = __DRAGONWIND_ROBOTSTATESID__; 
			break;

		case __DRAGONWIND_JACKTYPEID__:
			result = __DRAGONWIND_JACKSTATESID__; 
			break;

		default:
			break;
	}

	return (result);
}

// ---
int DRAGONWIND::BadGuy::animationIdForState (DRAGONWIND::DragonArtist::DragonState st, int lD)
{
	int result = -1;

	switch (st)
	{
		case DRAGONWIND::DragonArtist::DragonState::_STAND:
			result = state ((lD == 0) ? _statesId._standingRight : _statesId._slidingLeft) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_RUN:
			result = state ((lD == 0) ? _statesId._runningRight : _statesId._runningLeft) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_JUMP:
			result = state ((lD == 0) ? _statesId._jumpingRight : _statesId._jumpingLeft) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_FALL:
			result = state ((lD == 0) ? _statesId._fallingRight : _statesId._fallingLeft) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_SLIDE:
			result = state ((lD == 0) ? _statesId._slidingRight : _statesId._slidingLeft) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_DIEING:
			result = state ((lD == 0) ? _statesId._dieingRight : _statesId._dieingLeft) -> animationId ();
			break;

		case DRAGONWIND::DragonArtist::DragonState::_DIED:
			result = state ((lD == 0) ? _statesId._diedRight : _statesId._diedLeft) -> animationId ();
			break;

		default:
			assert (0); // It shouldn't be here at all...
	}

	return (result);
}
