#include <BattleshipLike/bsactionblocksforufos.hpp>
#include <BattleshipLike/bsscenes.hpp>
#include <BattleshipLike/bsgame.hpp>

// ---
const BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::PositionFunction 
	BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::_SAMEPOSITION = 
		[](int, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties*) -> QGAMES::Vector { return (QGAMES::Vector::_cero); };
const BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::TimeFunction 
	BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::_SAMETIME =
		[](int, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties*) -> QGAMES::bdata { return (__BD 0); };
const BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::SingleActionBlockFunction 
	BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::__SINGLESTDUFOSCTIONBLOCK =
		[](int i, BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF) -> BATTLESHIP::StdUFOSceneActionBlock* 
			{ return (new BATTLESHIP::StdUFOSceneActionBlock (i, new StdUFOSceneActionBlock::Properties (), 1, cF -> clone ())); };

const BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackOneByOneCombatFormationFactory::CurveFunction 
	BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackOneByOneCombatFormationFactory::_NOMOVEMENTCURVE =
		[](int, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties*) -> int { return (__BATTLESHIP_UFOATTRTYPNOMOVEMENT__); };

const BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackFollowingFirstCombatFormationFactory::CurveFunction 
	BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackFollowingFirstCombatFormationFactory::_NOMOVEMENTCURVE =
		[](int, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties*) -> int { return (__BATTLESHIP_UFOATTRTYPNOMOVEMENT__); };

// ---
BATTLESHIP::StdUFOSceneActionBlock::Properties::Properties 
		(QGAMES::bdata tA, int pA, bool tE, QGAMES::bdata tM, bool r, bool rll, 
		 const QGAMES::Position& pos, const QGAMES::Rectangle& mZ, QGAMES::bdata mTA,
		 int eT, int mT, const std::map <int, double>& mTP,
		 int sT, QGAMES::bdata ttS, int ppT, int sF, QGAMES::bdata s)
	: BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties (tA, pA, tE, tM, r, rll, pos, mZ, mTA),
	  _type (eT),
	  _movementType (mT),
	  _movementTypeParameters (mTP),
	  _typeShooting (sT),
	  _timeToShoot (ttS),
	  _probabilityToShoot (ppT),
	  _shootingForce (sF),
	  _speed (s)
{
	BATTLESHIP::Game* bG = 
		dynamic_cast <BATTLESHIP::Game*> (QGAMES::Game::game ());
	assert (bG);

	BATTLESHIP::DataGame* dG = bG -> dataGame ();
	assert (dG);

	assert (_type == -1 || // Either random or in the list of possible types...
			std::find (dG -> ufoTypesList ().begin (), dG -> ufoTypesList ().end (), _type) != dG -> ufoTypesList ().end ());
	assert (_movementType == -1 || // Either random or in the list of possible types...
			dG -> curveforMovementTypeMap ().find (_movementType) != dG -> curveforMovementTypeMap ().end ());
	assert (_typeShooting == -1 || _typeShooting >= 0); // Either random or positive
	assert (_probabilityToShoot >= 0 && _probabilityToShoot <= 100); // Always between o and 100
	assert (_shootingForce != 0); // 0 would mean being stopped!
}

// ---
BATTLESHIP::StdUFOSceneActionBlock::Properties::Properties (const std::map <std::string, std::string>& prps)
	: BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties (prps),
	  _type (-1), 
	  _movementType (-1),
	  _typeShooting (0),
	  _timeToShoot (1),
	  _probabilityToShoot (50),
	  _shootingForce (1),
	  _speed (__BD 1)
{
	if (prps.find (std::string (__BATTLESHIP_UFOATTRTYPE__)) != prps.end ())
		_type = std::atoi ((*prps.find (__BATTLESHIP_UFOATTRTYPE__)).second.c_str ());
	if (prps.find (std::string (__BATTLESHIP_UFOATTRTYPEMOVEMENT__)) != prps.end ())
		_movementType = std::atoi ((*prps.find (__BATTLESHIP_UFOATTRTYPEMOVEMENT__)).second.c_str ());
	
	if (prps.find (std::string (__BATTLESHIP_UFOATTRMOVEMENTPARAMS__)) != prps.end ())
	{
		std::vector <QGAMES::bdata> tMTP =
			QGAMES::getElementsFromAsBDATA ((*prps.find (__BATTLESHIP_UFOATTRMOVEMENTPARAMS__)).second.c_str (), ',');
		int i = 0; for (auto e : tMTP) _movementTypeParameters [i++] = (double) e;
	}

	if (prps.find (std::string (__BATTLESHIP_UFOATTRTYPESHOOTING__)) != prps.end ())
		_typeShooting = std::atoi ((*prps.find (__BATTLESHIP_UFOATTRTYPESHOOTING__)).second.c_str ());
	if (prps.find (std::string (__BATTLESHIP_UFOATTRTIMETOSHOOT__)) != prps.end ())
		_timeToShoot = __BD (std::atof ((*prps.find (__BATTLESHIP_UFOATTRTIMETOSHOOT__)).second.c_str ()));
	if (prps.find (std::string (__BATTLESHIP_UFOATTRPRBTOSHOOT__)) != prps.end ())
		_probabilityToShoot = std::atoi ((*prps.find (__BATTLESHIP_UFOATTRPRBTOSHOOT__)).second.c_str ());
	if (prps.find (std::string (__BATTLESHIP_UFOATTRSHOOTINGFORCE__)) != prps.end ())
		_shootingForce = std::atoi ((*prps.find (__BATTLESHIP_UFOATTRSHOOTINGFORCE__)).second.c_str ());
	if (prps.find (std::string (__BATTLESHIP_UFOATTRSPEED__)) != prps.end ())
		_speed = __BD (std::atof ((*prps.find (__BATTLESHIP_UFOATTRSPEED__)).second.c_str ()));

	BATTLESHIP::StdUFOSceneActionBlock::Properties 
		(_timeToAppear, _probabilityToAppear, _toEmerge, _timeToMove, _regenerate, _roll,
		 _initialPosition, _movementZone, _maxTimeAction, 
		  _type, _movementType, _movementTypeParameters,
		 _typeShooting, _timeToShoot, _probabilityToShoot, _shootingForce, _speed);
}

// ---
QGAMES::SetOfOpenValues BATTLESHIP::StdUFOSceneActionBlock::Properties::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result = 
		BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties::asSetOfOpenValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_type));
	result.addOpenValue (lE + 2, QGAMES::OpenValue (_movementType));

	std::string str;
	for (auto e : _movementTypeParameters) str += std::to_string ((double) e.second) + std::string (","); 
	if (str.length () != 0) str = str.substr (0, str.length () - 1); // To remove the last comma (unnecesary)
	result.addOpenValue (lE + 3, QGAMES::OpenValue (str));

	result.addOpenValue (lE + 4, QGAMES::OpenValue (_typeShooting));
	result.addOpenValue (lE + 5, QGAMES::OpenValue (_timeToShoot));
	result.addOpenValue (lE + 6, QGAMES::OpenValue (_probabilityToShoot));
	result.addOpenValue (lE + 7, QGAMES::OpenValue (_shootingForce));
	result.addOpenValue (lE + 8, QGAMES::OpenValue (_speed));

	return (result);
}

// ---
void BATTLESHIP::StdUFOSceneActionBlock::Properties::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	int lE = oV.lastOpenValueId ();

	assert (oV.existOpenValue (lE) && oV.existOpenValue (lE - 1) &&
			oV.existOpenValue (lE - 2) && oV.existOpenValue (lE - 3) &&
			oV.existOpenValue (lE - 4) && oV.existOpenValue (lE - 5) &&
			oV.existOpenValue (lE - 6) && oV.existOpenValue (lE - 7));

	QGAMES::SetOfOpenValues cCfg = oV;
	_speed = cCfg.openValue (lE).bdataValue ();
	cCfg.removeOpenValue (lE);
	_shootingForce = cCfg.openValue (lE - 1).intValue ();
	cCfg.removeOpenValue (lE - 1);
	_probabilityToShoot = cCfg.openValue (lE - 2).intValue ();
	cCfg.removeOpenValue (lE - 2);
	_timeToShoot = cCfg.openValue (lE - 3).bdataValue ();
	cCfg.removeOpenValue (lE - 3);
	_typeShooting = cCfg.openValue (lE - 4).intValue ();
	cCfg.removeOpenValue (lE - 4);

	_movementTypeParameters = std::map <int, double> ();
	std::vector <QGAMES::bdata> mTPT = QGAMES::getElementsFromAsBDATA (cCfg.openValue (lE - 5).strValue (), ',');
	int i = 0; for (auto e : mTPT) _movementTypeParameters [i++] = (double) e;
	cCfg.removeOpenValue (lE - 5);

	_movementType = cCfg.openValue (lE - 6).intValue ();
	cCfg.removeOpenValue (lE - 6);
	_type = cCfg.openValue (lE - 7).intValue ();
	cCfg.removeOpenValue (lE - 7);

	BATTLESHIP::StdSpaceElementsSceneActionBlock::Properties::fromSetOfOpenValues (cCfg);
}

// ---
QGAMES::Position BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory::initialPositionFor (int i, 
	BATTLESHIP::UFO* ufo, BATTLESHIP::StdUFOSceneActionBlock::Properties* prps)
{
	assert (ufo);
	assert (prps);

	QGAMES::Position result = QGAMES::Position::_cero;

	BATTLESHIP::Game* bG = 
		dynamic_cast <BATTLESHIP::Game*> (QGAMES::Game::game ());
	assert (bG);

	BATTLESHIP::DataGame* dtGame = bG -> dataGame ();
	assert (dtGame);

	int uW, uH, uD;
	ufo -> maxVisualDimensions (uW, uH, uD);
	QGAMES::Rectangle mZ = ufo -> getMovingZone ();
	QGAMES::Rectangle vZ = QGAMES::Game::game () -> mainScreen () -> visualZone ();

	std::map <int, int> cToA = dtGame -> curveforMovementTypeMap ();
	std::vector <int> cToAL; for (auto i : cToA) cToAL.push_back (i.first);
	int tM = (prps -> _movementType == -1) ? cToAL [rand () % (int) cToAL.size ()] : prps -> _movementType;
	QGAMES::Position iPos = (prps -> _initialPosition == QGAMES::Position::_noPoint)
		? dtGame -> defaultInitialPointForMovementType (tM, vZ, mZ) : prps -> _initialPosition;
	return (mZ.pos1 () + QGAMES::Position (mZ.width () * iPos.posX (), mZ.height () * iPos.posY (), __BD 0));
}

// ---
QGAMES::FollowingACurveMovement::CurveTemplate* 
	BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory::curveFor 
		(int i, BATTLESHIP::UFO* ufo, BATTLESHIP::StdUFOSceneActionBlock::Properties* prps,
		std::map <int, double>& crvprms)
{
	assert (ufo);
	assert (prps);
	
	QGAMES::FollowingACurveMovement::CurveTemplate* result = NULL;

	BATTLESHIP::Game* bG = 
		dynamic_cast <BATTLESHIP::Game*> (QGAMES::Game::game ());
	assert (bG);

	BATTLESHIP::Game::Conf* cfg = 
		dynamic_cast <BATTLESHIP::Game::Conf*> (QGAMES::Game::game () -> configuration ());
	assert (cfg);

	BATTLESHIP::DataGame* dtGame = bG -> dataGame ();
	assert (dtGame);

	QGAMES::AdvancedMovementBuilder* bld = 
		dynamic_cast <QGAMES::AdvancedMovementBuilder*> (ufo -> game () -> movementBuilder ());
	assert (bld);

	QGAMES::Rectangle mZ = ufo -> getMovingZone ();
	QGAMES::Rectangle vZ = QGAMES::Game::game () -> mainScreen () -> visualZone ();

	QGAMES::bdata f = (prps -> _speed < __BD 0) 
		? __BD ((rand () % ((int) -prps -> _speed)) + 1) : prps -> _speed;
	f += (__BD cfg -> difficulty () / __BD 2); // Regarding the difficulty level thge speed is increment a little bit more...

	QGAMES::bdata tR = 0;

	// Creates the curve using either the parameters received or the default ones...
	std::map <int, int> cToA = dtGame -> curveforMovementTypeMap ();
	std::vector <int> cToAL; for (auto i : cToA) cToAL.push_back (i.first);
	int tM = (prps -> _movementType == -1) ? cToAL [rand () % (int) cToAL.size ()] : prps -> _movementType;
	if (!crvprms.empty ())
		result = bld -> curveBuilder () -> createCurve (cToA [tM], crvprms);
	else
	{
		result = bld -> curveBuilder () -> 
			createCurve (cToA [tM], crvprms = dtGame -> defaultParametersForMovementType (tM, f, vZ, mZ, ufo -> position (), tR));
		if (tR != __BD 0) 
			result -> rotate (QGAMES::Vector::_zNormal, tR);
	}

	// If the curve doesn't exist a no movement wil lbe taken...
	return (result);
}

// ---
BATTLESHIP::StdUFOSceneActionBlock::StdUFOSceneActionBlock (int id, 
		BATTLESHIP::StdUFOSceneActionBlock::Properties* prps, int nU,
		BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF)
			: StdSpaceElementsSceneActionBlock (id, prps),
			  _curvePositionFactory (cF),
			  _numberOfUFOS (nU)
{ 
	assert (_curvePositionFactory);
	assert (_numberOfUFOS > 0);
}

// ---
void BATTLESHIP::StdUFOSceneActionBlock::toChangeMovement (int tM, QGAMES::bdata sF, std::map <int, double>& crvprms)
{
	if (_spaceElements.empty ())
		return;

	BATTLESHIP::StdUFOSceneActionBlock::Properties* prps = 
		dynamic_cast <BATTLESHIP::StdUFOSceneActionBlock::Properties*> (properties ());
	assert (prps);
	BATTLESHIP::StdUFOSceneActionBlock::Properties cPrps (*prps);
	cPrps._movementType = tM; // Change the type of movement...
	cPrps._speed *= sF;

	int ct = 0;
	for (BATTLESHIP::SpaceElements::const_iterator i = _spaceElements.begin (); 
			i != _spaceElements.end (); i++, ct++)
	{
		if ((*i) -> isMoving ())
		{
			QGAMES::FollowingACurveMovement* mov = 
				dynamic_cast <QGAMES::FollowingACurveMovement*> ((*i) -> currentMovement ());
			assert (mov); // Just in case...
			mov -> setCurve (_curvePositionFactory -> curveFor (ct, dynamic_cast <BATTLESHIP::UFO*> (*i), &cPrps, crvprms));
		}
	}
}

// ---
void BATTLESHIP::StdUFOSceneActionBlock::initialize ()
{
	BATTLESHIP::StdSpaceElementsSceneActionBlock::initialize ();

	BATTLESHIP::StdUFOSceneActionBlock::Properties* prps = 
		dynamic_cast <BATTLESHIP::StdUFOSceneActionBlock::Properties*> (_properties);
	assert (prps);

	if (!counters () -> existCounter (_COUNTERTOFIRE))
		counters () -> addCounter (new QGAMES::Counter (_COUNTERTOFIRE, 0, 0, true, true));
	counter (_COUNTERTOFIRE) -> initialize (
		(int) (((prps -> _timeToShoot < __BD 0) 
			? ((rand () % (int) -prps -> _timeToShoot) + 1) 
			: prps -> _timeToShoot) * game () -> framesPerSecond ()), 0, true, true);
}

// ---
void BATTLESHIP::StdUFOSceneActionBlock::updatePositions ()
{
	BATTLESHIP::StdSpaceElementsSceneActionBlock::updatePositions ();

	BATTLESHIP::StdUFOSceneActionBlock::Properties* prps = 
		dynamic_cast <BATTLESHIP::StdUFOSceneActionBlock::Properties*> (properties ());
	assert (prps);

	if (onOffSwitch (_SWITCHELEMENTSUNDERCONTROL) -> isOn () &&
		onOffSwitch (_SWITCHELEMENTSINSCENE) -> isOn () && !onOffSwitch (_SWITCHELEMENTSEMERGING) -> isOn () &&
		counter (_COUNTERTOFIRE) -> isEnd () && (rand () % 100 <= prps -> _probabilityToShoot))
	{
		for (BATTLESHIP::SpaceElements::const_iterator i = _spaceElements.begin (); 
				i != _spaceElements.end (); i++)
		{
			BATTLESHIP::UFO* ufo = dynamic_cast <BATTLESHIP::UFO*> ((*i));
			assert (ufo);
			ufo -> toShoot (
				(prps -> _shootingForce < 0) 
					? (std::rand () % prps -> _shootingForce) + 1 : prps -> _shootingForce, 
				QGAMES::Vector (__BD 0, __BD 1, __BD 0), 0); // Force random when properties is negative...

			if (prps -> _timeToShoot < __BD 0)
				counter (_COUNTERTOFIRE) -> initialize 
					((int) ((rand () % (int) -prps -> _timeToShoot) + 1) * game () -> framesPerSecond (), 0, true, true);
		}
	}
}

// ---
void BATTLESHIP::StdUFOSceneActionBlock::getAndConfigureSpaceElements (const std::vector <int>& elmts)
{
	assert (elmts.empty () || (int) elmts.size () == _numberOfUFOS);

	BATTLESHIP::PlayingScene* scn = dynamic_cast <BATTLESHIP::PlayingScene*> (scene ());
	assert (scn); // just in case...

	_spaceElements = getListValidSpaceElementsTypeFrom (elmts, _numberOfUFOS, scn -> enemies ());

	BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (bG);
	BATTLESHIP::DataGame* dG = bG -> dataGame ();
	assert (dG);

	for (BATTLESHIP::SpaceElements::const_iterator i = _spaceElements.begin (); 
			i != _spaceElements.end (); i++)
	{
		(*i) -> initialize (); 

		(*i) -> setVisible (true);

		BATTLESHIP::StdUFOSceneActionBlock::Properties* prps = 
			dynamic_cast <BATTLESHIP::StdUFOSceneActionBlock::Properties*> (properties ());
		assert (prps);

		// Which aspect will it have?
		((BATTLESHIP::UFO*) (*i)) -> setType (prps -> _type);
		// Which type of shooting should it have?
		((BATTLESHIP::UFO*) (*i)) -> setPowerLevel (prps -> _typeShooting);
		// ..and which will the initial position be?
		(*i) -> setPosition (_curvePositionFactory -> initialPositionFor (0, ((BATTLESHIP::UFO*) (*i)), prps));
	}
}

// ---
void BATTLESHIP::StdUFOSceneActionBlock::startToMoveSpaceElements ()
{
	if (_spaceElements.empty ())
		return;

	BATTLESHIP::StdUFOSceneActionBlock::Properties* prps = 
		dynamic_cast <BATTLESHIP::StdUFOSceneActionBlock::Properties*> (properties ());
	assert (prps);

	int ct = 0;
	for (BATTLESHIP::SpaceElements::const_iterator i = _spaceElements.begin (); 
			i != _spaceElements.end (); i++, ct++)
	{
		if ((*i) -> isAlive ())
		{
			(*i) -> toMove (prps -> _movementType, QGAMES::Vector (__BD 0, __BD 1, __BD 0));

			std::map <int, double> crvprms = prps -> _movementTypeParameters; // A copy to be modified (if any)
			QGAMES::FollowingACurveMovement* mov = 
				dynamic_cast <QGAMES::FollowingACurveMovement*> ((*i) -> currentMovement ());
			assert (mov); // Just in case...
			mov -> setCurve (_curvePositionFactory -> curveFor (ct, dynamic_cast <BATTLESHIP::UFO*> (*i), prps, crvprms));
		}
	}
}

// ---
BATTLESHIP::StdSetUFOsSceneActionBlock::Properties::Properties 
		(QGAMES::bdata tA, int pA, bool tE, QGAMES::bdata tM, bool r, bool rll, 
		 const QGAMES::Position& pos, const QGAMES::Rectangle& mZ, QGAMES::bdata mTA,
		 int eT, int mT, const std::map <int, double>& mTP, 
		 int sT, QGAMES::bdata ttS, int ppT, int sF, QGAMES::bdata s, 
		 int cT, const std::vector <QGAMES::bdata>& cTAttrs, const std::map <int, double>& cTAAttrs,
		 int nU, QGAMES::bdata tt, int ttM, int ttP)
	: StdUFOSceneActionBlock::Properties (tA, pA, tE, tM, r, rll, pos, mZ, mTA, eT, mT, mTP, sT, ttS, ppT, sF, s),
	  _combatFormation (cT),
	  _combatFormationAttrs (cTAttrs),
	  _combatFormationAttackAttrs (cTAAttrs),
	  _numberUFOs (nU),
	  _timerEvery (tt),
	  _timerForMany (ttM),
	  _timerProbability (ttP)
	  
{
	// 1 is the minum
	// It is strange in a formation, but maybe some types!
	assert (_numberUFOs >= 1);

	// The properties of the timer...
	assert (_timerForMany == -1 || (_timerForMany > 0 && _timerForMany <= _numberUFOs));
	assert (_timerProbability >= 0 && _timerProbability <= 100);
}

// ---
BATTLESHIP::StdSetUFOsSceneActionBlock::Properties::Properties (const std::map <std::string, std::string>& prps)
	: StdUFOSceneActionBlock::Properties (prps),
	  _combatFormation (0),
	  _combatFormationAttrs (),
	  _combatFormationAttackAttrs (),
	  _numberUFOs (2),
	  _timerEvery (1),
	  _timerForMany (-1),
	  _timerProbability (100)
{
	if (prps.find (std::string (__BATTLESHIP_ACTIONBLOCKATTRCOMBATFORMATION__)) != prps.end ())
		_combatFormation = std::atoi ((*prps.find (__BATTLESHIP_ACTIONBLOCKATTRCOMBATFORMATION__)).second.c_str ());
	if (prps.find (std::string (__BATTLESHIP_ACTIONBLOCKATTRCOMBATFORMATIONATTRS__)) != prps.end ())
		_combatFormationAttrs = QGAMES::getElementsFromAsBDATA ((*prps.find (__BATTLESHIP_ACTIONBLOCKATTRCOMBATFORMATIONATTRS__)).second.c_str (), ',');

	if (prps.find (std::string (__BATTLESHIP_ACTIONBLOCKATTRCOMBATFORMATTACKPARAMS__)) != prps.end ())
	{
		std::vector <QGAMES::bdata> cTAAttrsT = 
			QGAMES::getElementsFromAsBDATA ((*prps.find (__BATTLESHIP_ACTIONBLOCKATTRCOMBATFORMATTACKPARAMS__)).second.c_str (), ',');
		int i = 0; for (auto e : cTAAttrsT) _combatFormationAttackAttrs [i++] = (double) e;
	}

	if (prps.find (std::string (__BATTLESHIP_ACTIONBLOCKATTRNUMBERUFOS__)) != prps.end ())
		_numberUFOs = std::atoi ((*prps.find (__BATTLESHIP_ACTIONBLOCKATTRNUMBERUFOS__)).second.c_str ());
	if (prps.find (std::string (__BATTLESHIP_ACTIONBLOCKATTRTIMEREVERY__)) != prps.end ())
		_timerEvery = __BD (std::atof ((*prps.find (__BATTLESHIP_ACTIONBLOCKATTRTIMEREVERY__)).second.c_str ()));
	if (prps.find (std::string (__BATTLESHIP_ACTIONBLOCKSETUFOSATTRFORHOWMANY__)) != prps.end ())
		_timerForMany = std::atoi ((*prps.find (__BATTLESHIP_ACTIONBLOCKSETUFOSATTRFORHOWMANY__)).second.c_str ());
	if (prps.find (std::string (__BATTLESHIP_ACTIONBLOCKSETUFOSATTRPROB__)) != prps.end ())
		_timerProbability = std::atoi ((*prps.find (__BATTLESHIP_ACTIONBLOCKSETUFOSATTRPROB__)).second.c_str ());

	BATTLESHIP::StdSetUFOsSceneActionBlock::Properties 
		(_timeToAppear, _probabilityToAppear, _toEmerge, _timeToMove, _regenerate, _roll,
		 _initialPosition, _movementZone, _maxTimeAction, 
		 _type, _movementType, _movementTypeParameters,
		 _typeShooting, _timeToShoot, _probabilityToShoot, _shootingForce, _speed, 
		 _combatFormation, _combatFormationAttrs, _combatFormationAttackAttrs,
		 _numberUFOs, _timerEvery, _timerForMany, _timerProbability);
}

// ---
QGAMES::SetOfOpenValues BATTLESHIP::StdSetUFOsSceneActionBlock::Properties::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result = 
		BATTLESHIP::StdUFOSceneActionBlock::Properties::asSetOfOpenValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_combatFormation));

	std::string cFAttrsT;
	for (auto e : _combatFormationAttrs) cFAttrsT += std::to_string ((double) e) + std::string (",");
	if (cFAttrsT.length () != 0) cFAttrsT = cFAttrsT.substr (0, cFAttrsT.length () - 1); // To remove comma if any...
	result.addOpenValue (lE + 2, QGAMES::OpenValue (cFAttrsT));

	std::string cFAAttrsT;
	for (auto e : _combatFormationAttackAttrs) cFAAttrsT += std::to_string (e.second) + std::string (",");
	if (cFAAttrsT.length () != 0) cFAAttrsT = cFAAttrsT.substr (0, cFAAttrsT.length () - 1); // To remove comma if any...
	result.addOpenValue (lE + 3, QGAMES::OpenValue (cFAAttrsT));

	result.addOpenValue (lE + 4, QGAMES::OpenValue (_numberUFOs));
	result.addOpenValue (lE + 5, QGAMES::OpenValue (_timerEvery));
	result.addOpenValue (lE + 6, QGAMES::OpenValue (_timerForMany));
	result.addOpenValue (lE + 7, QGAMES::OpenValue (_timerProbability));

	return (result);
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::Properties::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	int lE = oV.lastOpenValueId ();

	assert (oV.existOpenValue (lE) && oV.existOpenValue (lE - 1) && 
			oV.existOpenValue (lE - 2) && oV.existOpenValue (lE - 3) &&
			oV.existOpenValue (lE - 4) && oV.existOpenValue (lE - 5) &&
			oV.existOpenValue (lE - 6));

	QGAMES::SetOfOpenValues cCfg = oV;

	_timerProbability = cCfg.openValue (lE).intValue ();
	cCfg.removeOpenValue (lE);
	_timerForMany = cCfg.openValue (lE - 1).intValue ();
	cCfg.removeOpenValue (lE - 1);
	_timerEvery = cCfg.openValue (lE - 2).bdataValue ();
	cCfg.removeOpenValue (lE - 2);
	_numberUFOs = cCfg.openValue (lE - 3).intValue ();
	cCfg.removeOpenValue (lE - 3);

	_combatFormationAttackAttrs = std::map <int, double> ();
	std::vector <QGAMES::bdata> cFAAttrsT = QGAMES::getElementsFromAsBDATA (cCfg.openValue (lE - 4).strValue (), ',');
	int i = 0; for (auto e : cFAAttrsT) _combatFormationAttackAttrs [i++] = (double) e;
	cCfg.removeOpenValue (lE - 4);

	_combatFormationAttrs = QGAMES::getElementsFromAsBDATA (cCfg.openValue (lE - 5).strValue (), ',');
	cCfg.removeOpenValue (lE - 5);
	_combatFormation = cCfg.openValue (lE - 6).intValue ();
	cCfg.removeOpenValue (lE - 6);

	BATTLESHIP::StdUFOSceneActionBlock::Properties::fromSetOfOpenValues (cCfg);
}

// ---
BATTLESHIP::StdUFOSceneActionBlock* BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::individualActionBlock 
	(int i, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties* prps)
{
	assert (prps);

	BATTLESHIP::StdUFOSceneActionBlock* result = _createEmptyIndividualActionBlockFunction (i, _curvePositionFactory);
	result -> setProperties (new BATTLESHIP::StdUFOSceneActionBlock::Properties 
			(prps -> _timeToAppear, 
			 prps -> _probabilityToAppear,
			 prps -> _toEmerge,
			 prps -> _timeToMove + _timeFunction (i, prps),
			 false, // The regeneration is determined for the full set and not for the each individual!
			 prps -> _roll,
			 ((prps -> _initialPosition == QGAMES::Position::_noPoint) 
			 	? QGAMES::Position::_cero : prps -> _initialPosition) + _positionFunction (i, prps),
			 prps -> _movementZone,
			 __BD -1, // The max time will be delimited by the control...
			 prps -> _type,
			 prps -> _movementType,
			 prps -> _movementTypeParameters,
			 prps -> _typeShooting, 
			 prps -> _timeToShoot,
			 prps -> _probabilityToShoot,
			 prps -> _shootingForce,
			 prps -> _speed)); // Low speed, while waiting to attack...

	// Remember that by default the curve and position factory 
	// associated is the on received at construction time for the combat factory!!

	return (result);
}

// ---
QGAMES::SetOfOpenValues 
	BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackOneByOneCombatFormationFactory::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));

	std::string lst;
	for (std::map <int, bool>::const_iterator i = _justSparked.begin (); i != _justSparked.end (); i++)
		if ((*i).second)
			lst += ((i != _justSparked.begin ()) ? std::string (",") : std::string (__NULL_STRING__)) +
				   std::to_string ((*i).first);
	result.addOpenValue (0, QGAMES::OpenValue (lst));

	return (result);
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackOneByOneCombatFormationFactory::fromSetOfOpenValues 
	(const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));
	assert (oV.existOpenValue (0));

	_justSparked = std::map <int, bool> ();
	std::vector <int> lst = QGAMES::getElementsFromAsInt (oV.openValue (0).strValue (), ',');
	for (std::vector <int>::const_iterator i = lst.begin (); i != lst.end (); _justSparked [(*i++)] = true);
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackOneByOneCombatFormationFactory::whenTimerIsSparked 
	(BATTLESHIP::StdUFOSceneActionBlock* aB, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties* prps)
{
	assert (aB);
	assert (prps);

	if (!aB -> spaceElementsMoving ())
		return;

	if (_justSparked.find (aB -> id ()) == _justSparked.end ()) 
	{
		std::map <int, double> crvprms = prps -> _combatFormationAttackAttrs; // To be modified if any...
		aB -> toChangeMovement (_curveFunction (aB -> id (), prps), __BD 4, crvprms);

		_justSparked [aB -> id ()] = true; // Just move, never more!
	}
}

// ---
BATTLESHIP::StdSetUFOsSceneActionBlock::BlockAndFlyCombatFormationFactory::BlockAndFlyCombatFormationFactory 
		(BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF, int ePR, QGAMES::bdata sH, QGAMES::bdata sV, 
		 int aM, const BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::SingleActionBlockFunction& eABlock)
	: StayAndAttackOneByOneCombatFormationFactory 
			(cF, _SAMEPOSITION, _SAMETIME, eABlock, _NOMOVEMENTCURVE), 
	  // The default functions are initially enought except the one to create the standard UFO...
	  _elementsPerRow (ePR),
	  _horizontalSeparationBetweenElements (sH),
	  _verticalSeparationBetweenElements (sV),
	  _attackingMovement (aM)
{
	assert (_elementsPerRow > 0);
	assert (_horizontalSeparationBetweenElements > __BD 0 && _horizontalSeparationBetweenElements < __BD 1);
	assert (_verticalSeparationBetweenElements > __BD 0 && _verticalSeparationBetweenElements < __BD 1);

	// The UFOs are set in rows
	_positionFunction = [=](int nB, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties* prps) -> QGAMES::Vector 
		{ 
		  assert (prps);

		  int ePR = ((int) prps -> _combatFormationAttrs.size () >= 1) 
			? (int) prps -> _combatFormationAttrs [0] : _elementsPerRow; // The default one if not defined...	
		  QGAMES::bdata sBEH = ((int) prps -> _combatFormationAttrs.size () >= 3) 
			? prps -> _combatFormationAttrs [1] : _horizontalSeparationBetweenElements; 
		  QGAMES::bdata sBEV = ((int) prps -> _combatFormationAttrs.size () >= 3) 
			? prps -> _combatFormationAttrs [2] : _verticalSeparationBetweenElements; 

		  return (QGAMES::Vector (sBEH * __BD (nB % ePR), sBEV * __BD (nB / ePR), __BD 0)); 
		};

	// The time function is not changed, so all elements should start to move at the same time...

	// The attacking movement is like in the flies...
	_curveFunction = [=](int, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties* prps) -> int
		{
			assert (prps); 

			int aM = ((int) prps -> _combatFormationAttrs.size () >= 4)
				? (int) prps -> _combatFormationAttrs [3] : _attackingMovement;
			return (aM == -1 
				? (__1IN2PROBABLE__? __BATTLESHIP_XYUFOATTRTYPEATTACKING0__ : __BATTLESHIP_XYUFOATTRTYPEATTACKING1__)
				: aM); 
		};
}

// ---
QGAMES::SetOfOpenValues 
	BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackFollowingFirstCombatFormationFactory::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));

	// The movement...
	QGAMES::SetOfOpenValues mvnt;
	for (std::map <int, int>::const_iterator i = _movement.begin (); i != _movement.end (); i++)
		mvnt.addOpenValue ((*i).first, QGAMES::OpenValue ((*i).second));
	result.addSetOfOpenValues (0, mvnt);

	// and the curve parameters...
	QGAMES::SetOfOpenValues crvPrms;
	for (std::map <int, double>::const_iterator i = _curveParameters.begin (); i != _curveParameters.end (); i++)
		crvPrms.addOpenValue ((*i).first, QGAMES::OpenValue (__BD (*i).second));
	result.addSetOfOpenValues (1, crvPrms);

	return (result);
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackFollowingFirstCombatFormationFactory::fromSetOfOpenValues 
	(const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));
	assert (oV.existSetOfOpenValues (0) && oV.existSetOfOpenValues (1));

	_movement = std::map <int, int> ();
	QGAMES::SetOfOpenValues mvnt = oV.setOfOpenValues (0);
	for (int i = 0; i < mvnt.numberValues (); i++)
		_movement [i] = mvnt.openValue (i).intValue ();

	_curveParameters = std::map <int, double> ();
	QGAMES::SetOfOpenValues crvPrms = oV.setOfOpenValues (1);
	for (int i = 0; i < crvPrms.numberValues (); i++)
		_curveParameters [i] = (double) crvPrms.openValue (i).bdataValue ();
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackFollowingFirstCombatFormationFactory::whenTimerIsSparked 
	(BATTLESHIP::StdUFOSceneActionBlock* aB, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties* prps)
{
	assert (aB);
	assert (prps);

	// Once the element is moving no possibility to change...
	if (!aB -> spaceElementsMoving ())
		return;

	if (_movement.find (aB -> id ()) == _movement.end () && aB -> spaceElementsMoving ()) 
	{
		int crv = (_movement.find (0) == _movement.end ()) ?  _curveFunction (aB -> id (), prps) : _movement [0];

		// When there are no parameters assigned yet, even to the first element in the list, the ones defined in
		// the configuration files are taken, if any!
		if (_curveParameters.empty ()) 
			_curveParameters = prps -> _combatFormationAttackAttrs;
		
		aB -> toChangeMovement (crv, __BD 1.0, _curveParameters /** The same for all except the first. */);

		_movement [aB -> id ()] = crv; // The same curve always than the first one...
	}
}

// ---
BATTLESHIP::StdSetUFOsSceneActionBlock::QueueAndFlyCombatFormationFactory::QueueAndFlyCombatFormationFactory 
		(BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF, QGAMES::bdata t, int aM,
		 const BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::SingleActionBlockFunction& eABlock)
	: StayAndAttackFollowingFirstCombatFormationFactory (cF, _SAMEPOSITION, _SAMETIME, eABlock, _NOMOVEMENTCURVE), 
	  // The default functions are enought except the one used to create the default std ufo action block
	  _timeBetweenElements (t),
	  _attackingMovement (aM)
{
	assert (_timeBetweenElements > 0);

	// The position function is the default one...

	// The time function is changed to allow one after other...
	_timeFunction = [=](int nB, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties* prps) -> QGAMES::bdata 
		{
			assert (prps);

			QGAMES::bdata tBE = ((int) prps -> _combatFormationAttrs.size () >= 1) 
				? prps -> _combatFormationAttrs [0] : _timeBetweenElements; // The default one if not received

			return (nB * tBE); 
		};

	// The attacking movement always the same for all (line / circle / line)...
	_curveFunction = [=](int, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties* prps) -> int 
		{
			assert (prps);


			int aM = ((int) prps -> _combatFormationAttrs.size () >= 2)
				? (int) prps -> _combatFormationAttrs [1] : _attackingMovement;
			return (aM == -1 ? __BATTLESHIP_XYUFOATTRTYPEATTACKING2__ : aM); 
		};
}

// ---
BATTLESHIP::StdSetUFOsSceneActionBlock::DancingCombatFormationFactory::DancingCombatFormationFactory 
		(BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF,
		 const BATTLESHIP::StdSetUFOsSceneActionBlock::DancingCombatFormationFactory::Movements& crvs, int tC,
		 const BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::PositionFunction& pFunc,
		 const BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::TimeFunction& tFunc,
		 const BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::SingleActionBlockFunction& eABlock) 
	: StayAndAttackFollowingFirstCombatFormationFactory (cF, pFunc, tFunc, eABlock),
	  _curves (crvs),
	  _timesToChange (tC)
{ 
	assert (!_curves.empty ()); // There must be one at least...
	assert (_timesToChange != 0);
	
	_timesToChange = (_timesToChange <0) 
		? rand () % (-_timesToChange + 1) : _timesToChange;

	// The curve function...
	// Returning one random out of the list received as parameter!
	_curveFunction = [=](int, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties*) -> int 
		{ return (rand () % _curves.size ()); };
}

// ---
QGAMES::SetOfOpenValues BATTLESHIP::StdSetUFOsSceneActionBlock::DancingCombatFormationFactory::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result = 
		BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackFollowingFirstCombatFormationFactory::asSetOfOpenValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_timesExecuted));

	return (result);
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::DancingCombatFormationFactory::fromSetOfOpenValues 
	(const QGAMES::SetOfOpenValues& oV)
{
	int lE = oV.lastOpenValueId ();

	assert (oV.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = oV;
	_timesExecuted = cCfg.openValue (lE).intValue ();
	cCfg.removeOpenValue (lE);

	BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackFollowingFirstCombatFormationFactory::fromSetOfOpenValues (cCfg);
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::DancingCombatFormationFactory::initialize ()
{ 
	BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackFollowingFirstCombatFormationFactory::initialize (); 

	_timesExecuted = 0; 
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::DancingCombatFormationFactory::whenTimerIsSparked 
	(BATTLESHIP::StdUFOSceneActionBlock* aB, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties* prps)
{
	assert (aB);
	assert (prps);

	// Once the element is moving no possibility to change...
	if (!aB -> spaceElementsMoving ())
		return;

	// If something has been defined inside the properties attribute, then it is taken insted the default value
	// received at construction time...
	int tTC = (prps -> _combatFormationAttrs.size () >= 1) 
		? (int) prps -> _combatFormationAttrs [0] : _timesToChange;
	if (_timesExecuted++ < tTC && !_curveParameters.empty ())
		return; // Whether the time hasn't been reached and the function has been executed once from initialization...
				// So first time, the function will be executed!

	// The movement changes from time to time...
	// regarding the result of the curve funcion!
	_timesExecuted = 0; // Start back...
	
	// The movement is (for all) the movement of the first element
	// because all dance following the same curve!
	// The parameters are the ones defined per each curve unless they have been defined in the configuration file.
	_movement [aB -> id ()] = (aB -> id () == 0) ? _curveFunction (0, prps) : _movement [0];
	if (aB -> id () == 0)
		_curveParameters = (prps -> _combatFormationAttackAttrs.empty ()) 
			? _curves [_movement [0]]._parameters : prps -> _combatFormationAttackAttrs; 
	// After that _curveParameters could be empty meaning random!
	aB -> toChangeMovement (_curves [_movement [aB -> id ()]]._movementId, __BD 1.0, _curveParameters);
}

// ---
BATTLESHIP::StdSetUFOsSceneActionBlock::DancingInLinesCombatFormationFactory::DancingInLinesCombatFormationFactory 
		(BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF,
		 int t, const BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::SingleActionBlockFunction& eABlock)
	: BATTLESHIP::StdSetUFOsSceneActionBlock::DancingCombatFormationFactory 
		( cF,
		  { 
			// Line (all)		Parameters (length, speed, x direction, y direction, z direction)		 
			{ 1,				{ { 0, 100.0f },	{ 1, 1.0f },	{ 2,  1.0f },	{ 3,  0.0f },	{ 4, 0.0f } } }, // Right
			{ 1,				{ { 0, 100.0f },	{ 1, 1.0f },	{ 2,  1.0f },	{ 3,  1.0f },	{ 4, 0.0f } } }, // Right - Down
			{ 1,				{ { 0, 100.0f },	{ 1, 1.0f },	{ 2,  0.0f },	{ 3,  1.0f },	{ 4, 0.0f } } }, // Down
			{ 1,				{ { 0, 100.0f },	{ 1, 1.0f },	{ 2, -1.0f },	{ 3,  1.0f },	{ 4, 0.0f } } }, // Left - Down
			{ 1,				{ { 0, 100.0f },	{ 1, 1.0f },	{ 2, -1.0f },	{ 3,  0.0f },	{ 4, 0.0f } } }, // Left
			{ 1,				{ { 0, 100.0f },	{ 1, 1.0f },	{ 2, -1.0f },	{ 3, -1.0f },	{ 4, 0.0f } } }, // Left - Up
			{ 1,				{ { 0, 100.0f },	{ 1, 1.0f },	{ 2,  0.0f },	{ 3, -1.0f },	{ 4, 0.0f } } }, // Up
			{ 1,				{ { 0, 100.0f },	{ 1, 1.0f },	{ 2,  1.0f },	{ 3, -1.0f },	{ 4, 0.0f } } }, // Right - Up
			{ 1,				{ { 0, 100.0f },	{ 1, 1.0f },	{ 2,  0.0f },	{ 3,  0.0f },	{ 4, 0.0f } } }  // No movement
		  }, 
		  t, _SAMEPOSITION, _SAMETIME, eABlock
		)
{
	// Nothing else to do...
}

// ---
BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactories::CombatFormationFactories 
		(const std::vector <BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory*>& f) 
	: _factories (f)
{ 
	assert (!_factories.empty ()); // One minimum...

	std::for_each (_factories.begin (), _factories.end (),
		[](BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory* i) { assert (i != NULL); }); // None NULL...
}

// ---
BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactories::~CombatFormationFactories ()
{
	std::for_each (_factories.begin (), _factories.end (), 
		[](BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory* i) { delete (i); });
	_factories.clear ();
}

// ---
QGAMES::SetOfOpenValues BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactories::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result (std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));

	int nF = 0;
	QGAMES::SetOfOpenValues cF;
	for (auto i : _factories)
		cF.addSetOfOpenValues (nF++, i -> asSetOfOpenValues ());

	result.addOpenValue (0, QGAMES::OpenValue (nF));
	result.addSetOfOpenValues (0, cF);

	return (result);
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactories::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.name () == std::string (__QGAMES_RUNTIMEVALUESOTHERPRPSTYPE__));
	assert (oV.existOpenValue (0) && oV.existSetOfOpenValues (0));

	int nF = oV.openValue (0).intValue ();
	QGAMES::SetOfOpenValues cF = oV.setOfOpenValues (0);
	assert (_factories.size () == nF && (int) cF.nestedOpenValues ().size () == nF);
	for (int i = 0; i < nF; i++)
		_factories [i] -> fromSetOfOpenValues (cF.setOfOpenValues (i));
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactories::setOwner (BATTLESHIP::StdSetUFOsSceneActionBlock* o)
{
	std::for_each (_factories.begin (), _factories.end (), 
		[=](BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory* i) -> void { i -> setOwner (o); });
}

// ---
BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory* 
	BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactories::combatFormationFactoryForType (int t) const
{
	BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory* result = NULL;
	for (std::vector <BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory*>::const_iterator i = _factories.begin (); 
			i != _factories.end () && !result; i++)
		if ((*i) -> forCombatType (t)) result = (*i); // The first for the right type...

	assert (result); // There must be one at least...
	return (result);
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactories::initialize ()
{
	std::for_each (_factories.begin (), _factories.end (), 
		[](BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory* i) { i -> initialize (); });
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactories::finalize ()
{
	std::for_each(_factories.begin (), _factories.end (), 
		[](BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory* i) { i -> initialize (); });
}

// ---
BATTLESHIP::StdSetUFOsSceneActionBlock::StdSetUFOsSceneActionBlock (int id, 
		BATTLESHIP::StdSetUFOsSceneActionBlock::Properties* prps, 
		BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactories* cF)
	: BATTLESHIP::SceneActionBlock (id),
	  _properties (prps),
	  _combatFormationFactories (cF),
	  _actionBlocks (), // The set of action blocks...
	  _blockToSparkNextTime (0)
{
	assert (_properties != NULL);
	assert (_combatFormationFactories != NULL);

	_combatFormationFactories -> setOwner (this);
}

// ---
BATTLESHIP::StdSetUFOsSceneActionBlock::~StdSetUFOsSceneActionBlock ()
{ 
	delete (_properties); 
	_properties = NULL; 

	deleteActionBlocks ();

	delete (_combatFormationFactories);
	_combatFormationFactories = NULL;
}
	
// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::setProperties 
	(BATTLESHIP::StdSetUFOsSceneActionBlock::Properties* prps)
{
	assert (prps != NULL);

	delete (_properties);

	_properties = prps;
}

// ---
BATTLESHIP::SpaceElements BATTLESHIP::StdSetUFOsSceneActionBlock::spaceElementsControlled () const
{
	BATTLESHIP::SpaceElements result;
	for (auto i : _actionBlocks)
	{
		BATTLESHIP::StdSpaceElementsSceneActionBlock* sA;
		if ((sA = dynamic_cast <BATTLESHIP::StdSpaceElementsSceneActionBlock*> (i)) != NULL)
			result.insert (result.end (), 
				sA -> spaceElementsControlled ().begin (), sA -> spaceElementsControlled ().end ());
	}

	return (result);
}

// ---
int BATTLESHIP::StdSetUFOsSceneActionBlock::spaceElementsFlying () const
{
	int result = 0;
	for (auto i : _actionBlocks) 
		result += dynamic_cast <BATTLESHIP::StdSpaceElementsSceneActionBlock*> (i) // Just in case...
			? ((BATTLESHIP::StdSpaceElementsSceneActionBlock*) i) -> spaceElementsFlying () : 0;
	return (result);
}

// ---
QGAMES::SetOfOpenValues BATTLESHIP::StdSetUFOsSceneActionBlock::runtimeValues () const
{
	assert (_properties);
	assert (_combatFormationFactories);

	QGAMES::SetOfOpenValues result = BATTLESHIP::SceneActionBlock::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();
	
	result.addSetOfOpenValues (lNE + 1, _properties -> asSetOfOpenValues ());
	result.addSetOfOpenValues (lNE + 2, _combatFormationFactories -> asSetOfOpenValues ());

	result.addOpenValue (lE + 1, QGAMES::OpenValue ((int) _actionBlocks.size ()));
	int i = 0;
	QGAMES::SetOfOpenValues aBlks; // // Keeping the information of the action block itself...
	for (auto aB : _actionBlocks)
		aBlks.addSetOfOpenValues (i++, aB -> runtimeValues ());
	result.addSetOfOpenValues (lNE + 3, aBlks);

	result.addOpenValue (lE + 2, QGAMES::OpenValue (_blockToSparkNextTime));

	return (result);
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::initializeRuntimeValuesFrom 
	(const QGAMES::SetOfOpenValues& cfg)
{
	assert (_properties);
	assert (_combatFormationFactories);

	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1) &&
			cfg.existSetOfOpenValues (lNE) && cfg.existSetOfOpenValues (lNE - 1) && cfg.existSetOfOpenValues (lNE - 2));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_blockToSparkNextTime = cCfg.openValue (lE).intValue ();
	cCfg.removeOpenValue (lE);

	int nABlks = cCfg.openValue (lE - 1).intValue ();
	cCfg.removeOpenValue (lE - 1);
	QGAMES::SetOfOpenValues aBlks = cCfg.setOfOpenValues (lNE);
	cCfg.removeSetOfOpenValues (lNE);
	assert (nABlks == (int) _actionBlocks.size () && nABlks == (int) aBlks.nestedOpenValues ().size ());
	for (int i = 0; i < nABlks; i++)
		_actionBlocks [i] -> initializeRuntimeValuesFrom (aBlks.setOfOpenValues (i));

	_combatFormationFactories -> fromSetOfOpenValues (cCfg.setOfOpenValues (lNE - 1));
	cCfg.removeSetOfOpenValues (lNE - 1);
	_properties -> fromSetOfOpenValues (cCfg.setOfOpenValues (lNE - 2));
	cCfg.removeSetOfOpenValues (lNE - 2);

	BATTLESHIP::SceneActionBlock::initializeRuntimeValuesFrom (cCfg);
}

// ---
bool BATTLESHIP::StdSetUFOsSceneActionBlock::spaceElementsUnderControl () const
{
	bool result = false;
	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin ();
			i != _actionBlocks.end () && !result; i++)
		result |= ((BATTLESHIP::StdUFOSceneActionBlock*) (*i)) -> spaceElementsUnderControl ();

	return (result);
}

// ---
bool BATTLESHIP::StdSetUFOsSceneActionBlock::spaceElementMoving () const
{
	bool result = false;
	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin ();
			i != _actionBlocks.end () && !result; i++)
		result |= ((BATTLESHIP::StdUFOSceneActionBlock*) (*i)) -> spaceElementsMoving ();

	return (result);
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::initialize ()
{
	reStartAllCounters ();

	BATTLESHIP::SceneActionBlock::initialize ();

	BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory* f = 
		_combatFormationFactories -> combatFormationFactoryForType (_properties -> _combatFormation); // The same for all...
	f -> initialize (); // First time...

	for (int i = 0; i < _properties -> _numberUFOs; i++)
	{
		BATTLESHIP::StdUFOSceneActionBlock* ufoAB = f -> individualActionBlock (i, _properties);

		ufoAB -> setScene (scene ());

		ufoAB -> initialize ();

		observe (ufoAB);

		_actionBlocks.push_back (ufoAB);
	}

	counter (_COUNTERTIMER) -> initialize 
		((int) (((_properties -> _timerEvery < __BD 0) 
			? __BD ((rand () % (int) - _properties -> _timerEvery) + __BD 1)
			: _properties -> _timerEvery) * __BD game () -> framesPerSecond ()), 0, true, true);

	if (_properties -> _maxTimeAction != __BD -1)
		counter (_COUNTERMAXTIMEACTION) -> initialize
			((int) (_properties -> _maxTimeAction * __BD game () -> framesPerSecond ()), 0, true, false); // one way counter...

	_blockToSparkNextTime = 0;
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::inEveryLoop ()
{
	BATTLESHIP::SceneActionBlock::inEveryLoop ();

	std::for_each (_actionBlocks.begin (), _actionBlocks.end (), [](QGAMES::SceneActionBlock* i) { i -> inEveryLoop (); });
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::updatePositions ()
{
	BATTLESHIP::SceneActionBlock::updatePositions ();

	for (QGAMES::SceneActionBlocks::const_iterator i = _actionBlocks.begin ();
			i != _actionBlocks.end (); i++)
		(*i) -> updatePositions ();

	if (!spaceElementsUnderControl ())
	{
		notify (QGAMES::Event (__BATTLESHIP_NOUFOSMOVING__, this));

		// With this instruction no body is considered neither being moving nor alive, nor...
		_combatFormationFactories -> initialize ();

		// Remember that the regenration is not trated at ufo level, but at set level!
		if (_properties -> _regenerate) // Starts back if regeneration has been set...
			std::for_each (_actionBlocks.begin (), _actionBlocks.end (), [](QGAMES::SceneActionBlock* i) { i -> initialize (); });
	}

	if (counter (_COUNTERTIMER) -> isEnd ())
		whenTimerIsSparked ();

	// If the game is paused no more things then to do..
	BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (bG);
	if (bG -> isGamePaused () || bG -> isNotPlaying ())
		return;

	// When the time of the action block is over (if any) a notification is sent!
	if (_properties -> _maxTimeAction != __BD -1 && counter (_COUNTERMAXTIMEACTION) -> isEnd ())
		notify (QGAMES::Event (__BATTLESHIP_SPACEELEMENTACTIONBLOCKTIMEOVER__, this));
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::finalize ()
{
	BATTLESHIP::SceneActionBlock::finalize ();

	std::for_each (_actionBlocks.begin (), _actionBlocks.end (), 
		[=](QGAMES::SceneActionBlock* i) { unObserve (i); i -> finalize (); });

	deleteActionBlocks ();

	BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory* f = 
		_combatFormationFactories -> combatFormationFactoryForType (_properties -> _combatFormation); 
	f -> finalize ();
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::processEvent (const QGAMES::Event& evnt)
{
	BATTLESHIP::SceneActionBlock::processEvent (evnt);

	notify (evnt);
}

// ---
__IMPLEMENTCOUNTERS__ (BATTLESHIP::StdSetUFOsSceneActionBlock::Counters)
{
	addCounter (new QGAMES::Counter (BATTLESHIP::StdSetUFOsSceneActionBlock::_COUNTERTIMER, 
		0 /** It will be set later too. */, 0, true, true)); 
	addCounter (new QGAMES::Counter (BATTLESHIP::StdSetUFOsSceneActionBlock::_COUNTERMAXTIMEACTION, 
		0 /** It will be set later too. */, 0, true, true)); 
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::whenTimerIsSparked ()
{
	BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory* f = 
		_combatFormationFactories -> combatFormationFactoryForType (_properties -> _combatFormation); // Same for all blocks...

	if ((rand () % 100) <= _properties -> _timerProbability)
	{
		for (int i = 0; i < ((_properties -> _timerForMany == -1) 
				? _properties -> _numberUFOs : _properties -> _timerForMany); i++)
		{
			f -> whenTimerIsSparked (dynamic_cast <BATTLESHIP::StdUFOSceneActionBlock*> 
				(_actionBlocks [_blockToSparkNextTime]), _properties);
			if (++_blockToSparkNextTime >= _properties -> _numberUFOs)
				_blockToSparkNextTime = 0;
		}
	}
}

// ---
void BATTLESHIP::StdSetUFOsSceneActionBlock::deleteActionBlocks ()
{
	std::for_each (_actionBlocks.begin (), _actionBlocks.end (), 
		[](QGAMES::SceneActionBlock* i) { delete (i); });
	_actionBlocks.clear ();
}
