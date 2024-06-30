#include <BattleshipLike/bsactionblocksforthingstocatch.hpp>
#include <BattleshipLike/bsscenes.hpp>

// ---
BATTLESHIP::StdPillSceneActionBlock::Properties::Properties 
		(QGAMES::bdata tA, int pA, bool tE, QGAMES::bdata tM, bool r, bool rll, 
		 const QGAMES::Position& pos, const QGAMES::Rectangle& mZ, QGAMES::bdata mTA,
		 int eT, int mT, int pL, QGAMES::bdata s)
	: BATTLESHIP::StdThingToCathActionBlock::Properties (tA, pA, tE, tM, r, rll, pos, mZ, mTA, eT, mT, s),
	  _powerLevel (pL)
{ 
	assert (_powerLevel == -1 || _powerLevel == -2 || _powerLevel >= 0);
}

// ---
BATTLESHIP::StdPillSceneActionBlock::Properties::Properties (const std::map <std::string, std::string>& prps)
	: BATTLESHIP::StdThingToCathActionBlock::Properties (prps),
	  _powerLevel (-1)
{
	if (prps.find (std::string (__BATTLESHIP_SHOOTPILLBLOCKATTRTYPEADDED__)) != prps.end ())
		_powerLevel = std::atoi ((*prps.find (__BATTLESHIP_SHOOTPILLBLOCKATTRTYPEADDED__)).second.c_str ());

	BATTLESHIP::StdPillSceneActionBlock::Properties 
		(_timeToAppear, _probabilityToAppear, _toEmerge, _timeToMove, _regenerate, _roll,
		 _initialPosition, _movementZone, _maxTimeAction, _type, _movementType, _powerLevel, _speed);
}

// ---
QGAMES::SetOfOpenValues BATTLESHIP::StdPillSceneActionBlock::Properties::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result = 
		BATTLESHIP::StdThingToCathActionBlock::Properties::asSetOfOpenValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_powerLevel));

	return (result);
}

// ---
void BATTLESHIP::StdPillSceneActionBlock::Properties::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	int lE = oV.lastOpenValueId ();

	assert (oV.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = oV;
	_powerLevel = cCfg.openValue (lE).intValue ();
	cCfg.removeOpenValue (lE);

	BATTLESHIP::StdThingToCathActionBlock::Properties::fromSetOfOpenValues (cCfg);
}

// ---
void BATTLESHIP::StdPillSceneActionBlock::getAndConfigureSpaceElements (const std::vector <int>& elmts)
{
	// Here any thing to catch is configurated..
	BATTLESHIP::StdThingToCathActionBlock::getAndConfigureSpaceElements (elmts);

	BATTLESHIP::PlayingScene* scn = dynamic_cast <BATTLESHIP::PlayingScene*> (scene ());
	assert (scn); // just in case...

	for (BATTLESHIP::SpaceElements::const_iterator i = _spaceElements.begin (); 
			i != _spaceElements.end (); i++)
	{
		assert (dynamic_cast <BATTLESHIP::PillPowerToCatch*> ((*i)) != NULL); // Just in case...

		BATTLESHIP::StdPillSceneActionBlock::Properties* prps = 
			dynamic_cast <BATTLESHIP::StdPillSceneActionBlock::Properties*> (properties ());
		assert (prps);

		// This is a pill of energy (si validator function), so...
		// The next level of energy is assigned...
		// Take really care with the value -3!!!!
		int pL = (prps -> _powerLevel == -1) 
			? (rand () % __BATTLESHIP_NUMBEROFPOWERLEVELS__) 
			: ((prps -> _powerLevel == -2) 
				? (scn -> battleship () -> powerLevel () + 1) 
				: ((prps -> _powerLevel == -3) ? (scn -> battleship () -> powerLevel () - 1) : prps -> _powerLevel));
		((BATTLESHIP::PillPowerToCatch*) (*i)) -> setPower (pL);
	}
}

// ---
BATTLESHIP::StdEnergyElementSceneActionBlock::Properties::Properties 
		(QGAMES::bdata tA, int pA, bool tE, QGAMES::bdata tM, bool r, bool rll, 
		 const QGAMES::Position& pos, const QGAMES::Rectangle& mZ, QGAMES::bdata mTA,
		 int eT, QGAMES::bdata eL, int mT, QGAMES::bdata s)
	: BATTLESHIP::StdThingToCathActionBlock::Properties (tA, pA, tE, tM, r, rll, pos, mZ, mTA, eT, mT, s),
	  _energy (eL)
{ 
	assert (_energy == -1000 || _energy == 1000 || 
			(_energy >= __BD -100 && _energy <= __BD 100)); 
}

// ---
BATTLESHIP::StdEnergyElementSceneActionBlock::Properties::Properties (const std::map <std::string, std::string>& prps)
	: BATTLESHIP::StdThingToCathActionBlock::Properties (prps),
	  _energy (10)
{
	if (prps.find (std::string (__BATTLESHIP_FUELPILLBLOCKATTRENERGYADDED__)) != prps.end ())
		_energy = __BD (std::atof ((*prps.find (__BATTLESHIP_FUELPILLBLOCKATTRENERGYADDED__)).second.c_str ()));

	BATTLESHIP::StdEnergyElementSceneActionBlock::Properties 
		(_timeToAppear, _probabilityToAppear, _toEmerge, _timeToMove, _regenerate, _roll,
		 _initialPosition, _movementZone, _maxTimeAction, _type, _energy, _movementType, _speed);
}

// ---
QGAMES::SetOfOpenValues BATTLESHIP::StdEnergyElementSceneActionBlock::Properties::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result = 
		BATTLESHIP::StdThingToCathActionBlock::Properties::asSetOfOpenValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_energy));

	return (result);
}

// ---
void BATTLESHIP::StdEnergyElementSceneActionBlock::Properties::fromSetOfOpenValues 
		(const QGAMES::SetOfOpenValues& oV)
{
	int lE = oV.lastOpenValueId ();

	assert (oV.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = oV;
	_energy = cCfg.openValue (lE).bdataValue ();
	cCfg.removeOpenValue (lE);

	BATTLESHIP::StdThingToCathActionBlock::Properties::fromSetOfOpenValues (cCfg);
}

void BATTLESHIP::StdEnergyElementSceneActionBlock::getAndConfigureSpaceElements (const std::vector <int>& elmts)
{
	// Here any thing to catch is configured...
	BATTLESHIP::StdThingToCathActionBlock::getAndConfigureSpaceElements (elmts);

	for (BATTLESHIP::SpaceElements::const_iterator i = _spaceElements.begin (); 
			i != _spaceElements.end (); i++)
	{
		assert (dynamic_cast <BATTLESHIP::EnergyElement*> ((*i)) != NULL); // Just in case...

		BATTLESHIP::StdEnergyElementSceneActionBlock::Properties* prps = 
			dynamic_cast <BATTLESHIP::StdEnergyElementSceneActionBlock::Properties*> (properties ());
		assert (prps);

		// What energy level will it provide to the battleship?
		((BATTLESHIP::EnergyElement*) (*i)) -> setEnergyLevel (prps -> _energy == -1000 
			? __BD -((rand () % 100) + 1) 
			: ((prps -> _energy == 1000) 
				? __BD ((rand () % 100) + 1) : prps -> _energy));
	}
}

