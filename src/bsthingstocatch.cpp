#include <BattleshipLike/bsthingstocatch.hpp>

// ---
QGAMES::Entity* BATTLESHIP::PillPowerToCatch::clone () const
{
	QGAMES::Entity* result = new BATTLESHIP::PillPowerToCatch 
		(_id, _behaviour -> clone (), _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void BATTLESHIP::PillPowerToCatch::setPower (int p)
{ 
	_power = p; 

	// Never out of the limits...
	if (_power < 0) 
		_power = 0;
	if (_power >= __BATTLESHIPDATAGAME__ -> _numberOfPowerLevels) 
		_power = __BATTLESHIPDATAGAME__ -> _numberOfPowerLevels - 1;
}

// ---
void BATTLESHIP::PillPowerToCatch::whenCaughtBy (BATTLESHIP::Spaceship* spc)
{
	assert (spc);

	BATTLESHIP::Battleship* bS = dynamic_cast <BATTLESHIP::Battleship*> (spc);
	if (spc != NULL) // Only valid for the battleship...
	{
		bS -> setPowerLevel (power ()); // Get the power of the pill...

		bS -> setScore (bS -> score () + behaviour () -> _pointsGivenWhenDestroyed); // If any...

		game () -> sound (__BATTLESHIP_BATTLESHIPGETSOUNDID__) -> play (__QGAMES_ARTISTSOUNDCHANNEL__);
	}
}

// ---
QGAMES::Entity* BATTLESHIP::EnergyElement::clone () const
{
	QGAMES::Entity* result = new BATTLESHIP::EnergyElement 
		(_id, _behaviour -> clone (), _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
QGAMES::bdata BATTLESHIP::EnergyElement::eneryLevel () const
{
	assert (behaviour ());

	return (-behaviour () -> _energyDetractedWhenHit);
}

// ---
void BATTLESHIP::EnergyElement::setEnergyLevel (QGAMES::bdata eL)
{
	assert (behaviour ());

	behaviour () -> _energyDetractedWhenHit = -eL;

	setStateFor (spaceElementState (), orientation ());
}

// ---
void BATTLESHIP::EnergyElement::whenCaughtBy (BATTLESHIP::Spaceship* spc)
{
	assert (spc);

	BATTLESHIP::Battleship* bS = dynamic_cast <BATTLESHIP::Battleship*> (spc);
	if (spc != NULL) // Only valid for the battleship...
	{
		spc -> setEnergyToAdd (-behaviour () -> _energyDetractedWhenHit); // Gets te energy of the fuel...
	
		spc -> setScore (spc -> score () + (int) behaviour () -> _pointsGivenWhenDestroyed); // Adds some points....

		game () -> sound (__BATTLESHIP_BATTLESHIPLOADENERGYSOUNDID__) -> play (__QGAMES_ARTISTSOUNDCHANNEL__); 
	}
}

// ---
QGAMES::Entity* BATTLESHIP::LiveToCatch::clone () const
{
	QGAMES::Entity* result = new BATTLESHIP::LiveToCatch 
		(_id, _behaviour -> clone (), _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void BATTLESHIP::LiveToCatch::whenCaughtBy (BATTLESHIP::Spaceship* spc)
{
	assert (spc);

	BATTLESHIP::Battleship* bS = dynamic_cast <BATTLESHIP::Battleship*> (spc);
	if (spc != NULL) // Only valid for the battleship...
	{
		BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
		assert (bG);

		bG -> setLives (bG -> lives () + 1);

		QGAMES::SetOfOpenValues oV;
		oV.addOpenValue (__BATTLESHIP_BATTLESHIPLIVESVALUEPARAMETER__, QGAMES::OpenValue (bG -> lives ()));
		bS -> notify (QGAMES::Event (__BATTLESHIP_BATTLESHIPLIVESACTUALIZED__, bS, oV)); // From the battleship...

		game () -> sound (__QGAMES_CATCHWAVSOUND__) -> play (__QGAMES_ARTISTSOUNDCHANNEL__);
	}
}

// ---
QGAMES::Entity* BATTLESHIP::ShieldToCatch::clone () const
{
	QGAMES::Entity* result = new BATTLESHIP::ShieldToCatch 
		(_id, _behaviour -> clone (), _forms, _data);

	result -> setMovements (movements ());
	result -> setAnimations (cloneAnimations ());
	result -> setStates (cloneStates ());

	return (result);
}

// ---
void BATTLESHIP::ShieldToCatch::whenCaughtBy (BATTLESHIP::Spaceship* spc)
{
	assert (spc);

	BATTLESHIP::Battleship* bS = dynamic_cast <BATTLESHIP::Battleship*> (spc);
	if (spc != NULL) // Only valid for the battleship...
	{
		bS -> toSeal (secondsShield ());

		game () -> sound (__QGAMES_CATCHWAVSOUND__) -> play (__QGAMES_ARTISTSOUNDCHANNEL__);
	}
}
