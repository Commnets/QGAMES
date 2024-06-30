#include <DragonwindLike/dwmovements.hpp>

// ---
QGAMES::Movement* DRAGONWIND::SwimingPhysicsIncrementalMovement::clone () const
{
	DRAGONWIND::SwimingPhysicsIncrementalMovement* result = 
		new DRAGONWIND::SwimingPhysicsIncrementalMovement (*this);
	result -> setData (data ());
	return (result);
}

// ---
QGAMES::SetOfOpenValues DRAGONWIND::SwimingPhysicsIncrementalMovement::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::PhysicsIncremetalMovement::runtimeValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_liquidDensity));
	std::stringstream sS; sS << _liquidForce;
	result.addOpenValue (lE + 2, QGAMES::OpenValue (sS.str ()));

	return (result);
}

// ---
void DRAGONWIND::SwimingPhysicsIncrementalMovement::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_liquidForce = cCfg.openValue (lE).strValue ();
	cCfg.removeOpenValue (lE);
	_liquidDensity = cCfg.openValue (lE - 1).bdataValue ();
	cCfg.removeOpenValue (lE - 1);

	QGAMES::PhysicsIncremetalMovement::initializeRuntimeValuesFrom (cCfg);
}

// ---
void DRAGONWIND::SwimingPhysicsIncrementalMovement::setLiquidDensity (QGAMES::bdata lD)
{
	assert (lD > 1);

	_liquidDensity = lD; 
}

// ---
void DRAGONWIND::SwimingPhysicsIncrementalMovement::initialize ()
{
	QGAMES::PhysicsIncremetalMovement::initialize ();

	_liquidDensity = __BD 1; // The default one is like water...

	_liquidForce = QGAMES::Vector::_cero;
}

// ---
void DRAGONWIND::SwimingPhysicsIncrementalMovement::initializeFrom (const QGAMES::Movement* m)
{
	QGAMES::PhysicsIncremetalMovement::initializeFrom (m);

	const DRAGONWIND::SwimingPhysicsIncrementalMovement* movS =
		dynamic_cast <const DRAGONWIND::SwimingPhysicsIncrementalMovement*> (m);
	if (movS)
		_liquidDensity = movS -> _liquidDensity;
}

// ---
void DRAGONWIND::SwimingPhysicsIncrementalMovement::prepareMovement 
	(QGAMES::IncrementalMovement::Data* d, const QGAMES::Vector& dr, const QGAMES::Vector& a, QGAMES::Entity* e)
{
	assert (e);

	QGAMES::PhysicsIncremetalMovement::PData* pD = pData (d);
	assert (pD);

	QGAMES::PhysicsIncremetalMovement::prepareMovement (d, dr, a, e);

	// Something minimum, just to stop...and related with the initial speed...
	setMinSpeed (pD -> _currentSpeed.module () / _liquidDensity); 
}

// ---
void DRAGONWIND::SwimingPhysicsIncrementalMovement::calculateNextMovement 
	(QGAMES::IncrementalMovement::Data* d, const QGAMES::Vector& dr, const QGAMES::Vector& a, QGAMES::Entity* e)
{
	assert (e);

	QGAMES::PhysicsIncremetalMovement::PData* pD = pData (d);
	assert (pD);

	QGAMES::PhysicsIncremetalMovement::calculateNextMovement (d, dr, a, e);

	// The friction cuold affect like a force opposed to the movement (determine by the speed/acceleration)
	// The friction starts when there is movement, if not there is not friction...
	if (_liquidDensity != __BD 0)
	{
		if (pD -> _currentSpeed.module () > _minSpeed)
		{
			setAdditionalAcceleration (-_liquidForce);
			setAdditionalAcceleration (_liquidForce = 
				QGAMES::Vector (-pD -> _currentSpeed.posX () * (_liquidDensity - __BD 1), 
								-pD -> _currentSpeed.posY () * ((_liquidDensity / __BD 2) - __BD 1), // Half in this axis...
								-pD -> _currentSpeed.posZ () * (_liquidDensity - __BD 1)));
		}
		else
		{
			if (_liquidForce != QGAMES::Vector::_cero)
			{
				setSpeed (QGAMES::Vector::_cero);
				setAdditionalAcceleration (-_liquidForce); // Removes the friction of the water...
				_liquidForce = QGAMES::Vector::_cero;
			}
		}
	}
}

// ---
void DRAGONWIND::SwimingPhysicsIncrementalMovement::stopDuringMovement 
	(const QGAMES::Vector& dr, const QGAMES::Vector& a, QGAMES::Entity* e)
{
	// When it is not possible to advance in a direction the speed in that direction is set to 0...
	int aA = 0x00;
	if (dr.posX () != 0) aA |= 0x01;
	if (dr.posY () != 0) aA |= 0x02;
	if (dr.posZ () != 0) aA |= 0x04;
	// Not possible that various at the same time, but just in case...

	QGAMES::PhysicsIncremetalMovement::PData* pD = pData ();
	assert (pD);

	// Stop the speed and acceleration in the dimension that can't progress...
	pD -> _currentSpeed = QGAMES::Vector (
		((aA & 0x01) ? __BD 0 : __BD 1) * pD -> _currentSpeed.posX (),
		((aA & 0x02) ? __BD 0 : __BD 1) * pD -> _currentSpeed.posY (),
		((aA & 0x04) ? __BD 0 : __BD 1) * pD -> _currentSpeed.posZ ());
	// The external forces (so the accelration are not modified)

	calculateInternalParametersForMovement ();

	toAdjustNextSpeedAndAcceleration ();
}
