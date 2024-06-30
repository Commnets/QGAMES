#include <DragonwindLike/dwthings.hpp>
#include <DragonwindLike/dwgame.hpp>

// ---
QGAMES::Entity* DRAGONWIND::Thing::clone () const
{
	QGAMES::Entity* result = new DRAGONWIND::Thing (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void DRAGONWIND::Thing::setPlatformState (int id)
{
	int cS = platformState ();
	DRAGONWIND::DragonArtist::setPlatformState (id);
	if (platformState () == cS || id == -1) // -1 means not necessary...
		return; // Nothing has changed...

	assert (isPlatformStatePossible (id));

	switch (platformStateToDragonState ())
	{
		case DRAGONWIND::DragonArtist::DragonState::_STAND:
			setCurrentState (__DRAGONWIND_THINGSTANDSTATEID__);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_FALL:
			setCurrentState (__DRAGONWIND_THINGFALLINGSTATEID__);
			break;

		default:
			assert (0); // It should be here...
	}
}

// ---
bool DRAGONWIND::Thing::isPlatformStatePossible (int pS) const
{
	return (platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_STAND ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_FALL);
}

// ---
void DRAGONWIND::Thing::setFromDefinition (QGAMES::PlatformGame::ElementDefinition* eD)
{
	DRAGONWIND::DragonArtist::setFromDefinition (eD);

	assert (eD);

	setPlatformState (definition () -> _state == -1 
		? dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_STAND)
		: definition () -> _state);

	// The aspect is set here instead in the setPlatformState method because is doesn't depend on that...
	setCurrentForm (__DRAGONWIND_NINJATHINGTOCARRYFORM__); // Always the same form...
	setCurrentAspect (aspectAssociatedToThingType (definition () -> _type)); // The aspect is associated to the type...

	// The fix base position is set on the parent class
	// But if at this point the entity is in a platform state and still with no fix base position, 
	// onew by default is set!
	if (isOnAPlatformState () && fixBasePosition () == QGAMES::Position::_noPoint)
		setFixBasePosition (basePosition ());
}

// ---
void DRAGONWIND::Thing::initialize ()
{
	DRAGONWIND::DragonArtist::initialize ();

	if (definition ()) // Depending whether there is or not definition...
	{
		setPlatformState (definition () -> _state == -1 
			? dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_STAND)
			: definition () -> _state);
		setPosition (*definition () -> _positions.begin ());
	}
	else
	{
		setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_STAND));
		setPosition (QGAMES::Position::_cero);
	}

	// The aspect is set here instead in the setPlatformState method because is doesn't depend on that...
	setCurrentForm (__DRAGONWIND_NINJATHINGTOCARRYFORM__); // Always the same form...
	setCurrentAspect (definition () ? aspectAssociatedToThingType (definition () -> _type) : 0); // Aspect alligned with the type of the basic one 
}

// ---
void DRAGONWIND::Thing::updatePositions ()
{
	if (!isVisible ())
		return;

	if (isBlocked () && !isStanding ())
		toStand (lookingToDirection ());

	DRAGONWIND::DragonArtist::updatePositions ();
}

// ---
void DRAGONWIND::Thing::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	DRAGONWIND::DragonArtist::drawOn (s, p);

	// Adiitional information whether it is a gun or a heart...
	QGAMES::ScoreObjectText* txt = NULL;
	if (_definition -> _type == __DRAGONWIND_TILEDEFININGARROW__ ||
		_definition -> _type == __DRAGONWIND_TILEDEFININGSTAR__ ||
		_definition -> _type == __DRAGONWIND_TILEDEFININGBOMB__)
		txt = game () -> textBuilder () -> createText (__QGAMES_RAVIE12BLUELETTERS__, 
			std::to_string (_definition -> _otherValues.openValue (__DRAGONWIND_GUNSTOCATCHNUMBERSHOOTS__).intValue ()));
	else
	if (_definition -> _type == __DRAGONWIND_TILEDEFININGHEART__)
		txt = game () -> textBuilder () -> createText (__QGAMES_RAVIE12ORANGELETTERS__, 
			std::to_string (_definition -> _otherValues.openValue (__DRAGONWIND_HEARTTOCATCHNUMBERLIVES__).intValue ()));
	
	if (txt)
		txt -> drawOn (s, (p != QGAMES::Position::_noPoint) ? p + centerPosition () : centerPosition ());
	delete (txt);
}

// ---
void DRAGONWIND::Thing::removeFromTheGame ()
{
	DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
	assert (dG); // Just in case...

	dG -> removeThingToCatch (map () -> scene () -> id (), definition () -> _id); // Remove from the conf...

	setFromNoDefinition (); // The definition is no longer needed...

	setVisible (false); // No longer visible...
}
