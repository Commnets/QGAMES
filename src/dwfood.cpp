#include <DragonwindLike/dwfood.hpp>
#include <DragonwindLike/dwgame.hpp>

// ---
QGAMES::Entity* DRAGONWIND::Food::clone () const
{
	QGAMES::Entity* result = new DRAGONWIND::Food (_id, _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void DRAGONWIND::Food::setPlatformState (int id)
{
	int cS = platformState ();
	DRAGONWIND::DragonArtist::setPlatformState (id);
	if (platformState () == cS || id == -1) // -1 means not necessary...
		return; // Nothing has changed...

	assert (isPlatformStatePossible (id));

	switch (platformStateToDragonState ())
	{
		case DRAGONWIND::DragonArtist::DragonState::_STAND:
			setCurrentState (__DRAGONWIND_FOODSTANDSTATEID__);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_FALL:
			setCurrentState (__DRAGONWIND_FOODFALLINGSTATEID__);
			break;

		case DRAGONWIND::DragonArtist::DragonState::_INTHEAIR:
			setCurrentState (__DRAGONWIND_FOODINAIRSTATEID__);
			break;

		default:
			assert (0); // It should be here...
	}
}

// ---
bool DRAGONWIND::Food::isPlatformStatePossible (int pS) const
{
	return (platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_STAND ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_FALL ||
			platformStateToDragonState (pS) == DRAGONWIND::DragonArtist::DragonState::_INTHEAIR);
}

// ---
void DRAGONWIND::Food::setFromDefinition (QGAMES::PlatformGame::ElementDefinition* eD)
{
	DRAGONWIND::DragonArtist::setFromDefinition (eD);

	assert (eD);

	DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
	assert (dG); // Just in case...

	DRAGONWIND::ToKnowTileFrameInformation* tInf = 
		dynamic_cast <DRAGONWIND::ToKnowTileFrameInformation*> (dG -> tileFrameInformation ());
	assert (tInf); // Just in case...

	_energyLevel = energyAssociatedToFoodType (tInf -> typeOfFoodAssociatedToFoodTileFrame (definition () -> _type));
	_inTheAir = foodTypeToBeInTheAir (tInf -> typeOfFoodAssociatedToFoodTileFrame (definition () -> _type));

	setPlatformState (definition () -> _state == -1 
		? (_inTheAir
				? dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_INTHEAIR)
				: dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_STAND))
		: definition () -> _state);

	// The aspect is set here instead of inside the setPlatformState method,
	// Because it is set randomically anytime that init method is executed
	setCurrentForm (__DRAGONWIND_NINJAFOODFORM__); // Always the same form...

	int asp = rand () % __DRAGONWIND_NINJAFOODTYPES__;
	if (eD -> _otherValues.existOpenValue (__DRAGONWIND_FOODASPECTATTRIBUTE__))
		asp = eD -> _otherValues.openValue (__DRAGONWIND_FOODASPECTATTRIBUTE__).intValue ();
	else
	{
		QGAMES::SetOfOpenValues oV;
		oV.addOpenValue (__DRAGONWIND_FOODASPECTATTRIBUTE__, asp);
		eD -> _otherValues = oV; // Keep the aspect thinking about the next time...
	}

	setCurrentAspect (asp); 

	// The fix base position is set on the parent class
	// But if at this point the entity is in a platform state and still with no fix base position, 
	// onew by default is set!
	if (isOnAPlatformState () && fixBasePosition () == QGAMES::Position::_noPoint)
		setFixBasePosition (basePosition ());
}

// ---
void DRAGONWIND::Food::initialize ()
{
	DRAGONWIND::DragonArtist::initialize ();

	if (definition ()) // Depending whether there is or not definition...
	{
		setPlatformState (definition () -> _state == -1 
			? (_inTheAir 
					? dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_INTHEAIR)
					: dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_STAND))
			: definition () -> _state);
		setPosition (*definition () -> _positions.begin ());
	}
	else
	{
		setPlatformState (dragonStateToPlatformState (DRAGONWIND::DragonArtist::DragonState::_STAND));
		setPosition (QGAMES::Position::_cero);
	}

	// Sets the aspect
	// It will depend on there is or not definition set already...
	setCurrentForm (__DRAGONWIND_NINJAFOODFORM__); // Always the same form...
	setCurrentAspect (definition () && definition () -> _otherValues.existOpenValue (0) 
		? definition () -> _otherValues.openValue (0).intValue () : 0);
}

// ---
void DRAGONWIND::Food::updatePositions ()
{
	if (!isVisible ())
		return;

	DRAGONWIND::DragonArtist::updatePositions ();
}

// ---
void DRAGONWIND::Food::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	DRAGONWIND::DragonArtist::drawOn (s, p);

	if (isHighEnergy ())
		game () -> form (__DRAGONWIND_NINJAHIGHENERGYFOODFORM__) -> 
			drawOn (s, 0, (p == QGAMES::Position::_noPoint) ? position () : p + position ());
}

// ---
void DRAGONWIND::Food::whenCollisionWith (QGAMES::Entity* e)
{
	DRAGONWIND::Ninja* nj = dynamic_cast <DRAGONWIND::Ninja*> (e);
	if (nj) // It has effect only over ninjas...that's it: No other character can "eat"
		removeFromTheGame ();
}

// ---
int DRAGONWIND::Food::energyAssociatedToFoodType (int t)
{
	return (((t >> 1) + 1) * (((t % 2) == 1) ? 2 : 1));
}

// ---
bool DRAGONWIND::Food::isHighEnergy () const
{
	return (_energyLevel >= 6);
}

// ---
void DRAGONWIND::Food::removeFromTheGame ()
{
	DRAGONWIND::Game* dG = dynamic_cast <DRAGONWIND::Game*> (game ());
	assert (dG); // Just in case...

	dG -> removeThingToEat (map () -> scene () -> id (), definition () -> _id); // Remove from the conf...

	setFromNoDefinition (); // The definition is no longer needed...

	setVisible (false); // No longer visible...
}
