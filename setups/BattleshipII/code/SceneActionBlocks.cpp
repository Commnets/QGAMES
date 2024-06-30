#include "SceneActionBlocks.hpp"
#include "Game.hpp"

// ---
BattleshipII::MothershipUFOSetSceneActionBlock::MothershipUFOSetSceneActionBlock 
	(int id, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties* prps)
		: BATTLESHIP::StdSetUFOsSceneActionBlock (id,prps, new CombatFormationFactories 
			({ new BattleshipII::DancingInLinesForBigUFOSCombatFormationFactory
				(new BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory /** Rest default. */) })),
		  _firstTime (true)
{ 
	assert (prps -> _combatFormation == __BATTLESHIPII_BIGUFODANCINGINLINES__); // The only one allowed...
}

// ---
QGAMES::SetOfOpenValues BattleshipII::MothershipUFOSetSceneActionBlock::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = BATTLESHIP::StdSetUFOsSceneActionBlock::runtimeValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_firstTime));

	return (result);
}

// ---
void BattleshipII::MothershipUFOSetSceneActionBlock::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = cfg;
	_firstTime = cCfg.openValue (lE).boolValue ();
	cCfg.removeOpenValue (lE);

	BATTLESHIP::StdSetUFOsSceneActionBlock::initializeRuntimeValuesFrom (cCfg);
}

// ---
void BattleshipII::MothershipUFOSetSceneActionBlock::initialize ()
{
	BATTLESHIP::StdSetUFOsSceneActionBlock::initialize ();

	_firstTime = true;
}

// ---
void BattleshipII::MothershipUFOSetSceneActionBlock::updatePositions ()
{
	BATTLESHIP::StdSetUFOsSceneActionBlock::updatePositions ();

	if (!spaceElementsControlled ().empty () && spaceElementsControlled ()[0] -> isMoving () && _firstTime)
	{
		whenTimerIsSparked (); // Everything really for just this simple instruction!

		_firstTime = false;
	}
}

// ---
BattleshipII::SinusoideFromLeftBlockAndFlyCombatFormationFactory::SinusoideFromLeftBlockAndFlyCombatFormationFactory  
		(BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF, QGAMES::bdata sBE,
		 const BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::SingleActionBlockFunction& eABlock)
	: BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackOneByOneCombatFormationFactory 
		(cF, _SAMEPOSITION, _SAMETIME, eABlock, _NOMOVEMENTCURVE),
	  _separationBetweenElements (sBE)
{
	// The UFOS are located one beneath the other...
	_positionFunction = [=](int i, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties* prps) -> QGAMES::Position 
		{ 
			assert (prps);

			QGAMES::bdata sBE = ((int) prps -> _combatFormationAttrs.size () >= 1) 
				? prps -> _combatFormationAttrs [0] : _separationBetweenElements;

			return (QGAMES::Position (__BD 0, __BD i * __BD sBE, __BD 0)); 
		};

	// There is no time among the UFOs when starting to move, so the defaut time function is enough

	// The curve to follow is different depending whether the UFO id is odd or even
	_curveFunction = [=](int i, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties*) 
		-> int { return ((i % 2 == 0)
			? __BATTLESHIPII_SINUSOIDE1UFOSMOVEMENT__ : __BATTLESHIPII_SINUSOIDE2UFOSMOVEMENT__); };
}

// ---
BattleshipII::SinusoideFromRightBlockAndFlyCombatFormationFactory::SinusoideFromRightBlockAndFlyCombatFormationFactory 
		(BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF, QGAMES::bdata sBE,
		 const BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::SingleActionBlockFunction& eABlock)
	: BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackOneByOneCombatFormationFactory 
		(cF, _SAMEPOSITION, _SAMETIME, eABlock, _NOMOVEMENTCURVE),
	  _separationBetweenElements (sBE)
{
	// The UFOS are located one beneath the other...
	_positionFunction = [=](int i, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties* prps) -> QGAMES::Position 
		{ 
			assert (prps);

			QGAMES::bdata sBE = ((int) prps -> _combatFormationAttrs.size () >= 1) 
				? prps -> _combatFormationAttrs [0] : _separationBetweenElements;

			return (QGAMES::Position (__BD 0, __BD i * __BD sBE, __BD 0)); 
		};

	// There is no time among the UFOs when starting to move, so the defaut time function is enough

	// The curve to follow is different depending whether the UFO id is odd or even
	_curveFunction = [=](int i, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties*) 
		-> int { return ((i % 2 == 0)
			? __BATTLESHIPII_SINUSOIDE3UFOSMOVEMENT__ : __BATTLESHIPII_SINUSOIDE4UFOSMOVEMENT__); };
}

// ---
BattleshipII::DancingInLinesForBigUFOSCombatFormationFactory::DancingInLinesForBigUFOSCombatFormationFactory 
	(BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF, int t,
	 const BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::SingleActionBlockFunction& eABlock)
		: BATTLESHIP::StdSetUFOsSceneActionBlock::DancingInLinesCombatFormationFactory (cF, t, eABlock),
		  _firstMovement (true)
{
	_curveFunction = [=](int i, BATTLESHIP::StdSetUFOsSceneActionBlock::Properties*) -> int
		{
			assert (owner ());

			int result = 2; // down by default...
			if (_firstMovement)
				_firstMovement = false;
			else
			{
				BattleshipII::MothershipUFO* ufo =
					dynamic_cast <BattleshipII::MothershipUFO*> (owner () -> spaceElementsControlled ()[0]);
				assert (ufo); // Just in case...

				int fW, fH, fD;
				ufo -> maxVisualDimensions (fW, fH, fD);
				QGAMES::Rectangle mZ = ufo -> getMovingZone ();
				mZ = QGAMES::Rectangle (
					mZ.pos1 () + QGAMES::Vector (__BD (fW >> 1), __BD (fH >> 1), __BD 0), 
					mZ.pos2 () - QGAMES::Vector (__BD (fW >> 1), __BD (fH >> 1), __BD 0));
				
				// Possibilities (in the same order than the _curves)
				// 1  : right (1 << 0)
				// 2  : right - down (1 << 1)
				// 4  : down (1 << 2)
				// 8  : left - down (1 << 3)
				// 16 : left (1 << 4) 
				// 32 : left - up (1 << 5)
				// 64 : up (1 << 6)
				// 128: right - up (1 << 7)
				// 256: no movement (1 << 8)
				int p = (1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 | 256);
				int px, py; px = py = p; // all possibilities opened initially 
				if (ufo -> position ().posX () < mZ.pos1 ().posX ()) px &= (1 | 2 | 4 | 64 | 128 | 256);
				if (ufo -> position ().posX () > mZ.pos2 ().posX ()) px &= (4 | 8 | 16 | 32 | 64 | 256);
				if (ufo -> position ().posY () < mZ.pos1 ().posY ()) py &= (1 | 2 | 4 | 8 | 16 | 256);
				if (ufo -> position ().posY () > mZ.pos2 ().posY ()) py &= (1 | 16 | 32 | 64 | 128 | 256);
				p &= px & py;

				std::vector <int > pos;
				for (int i = 0; i <= 8 /** 1 >> 8 */; i++)
					if ((p & (1 << i)) != 0) pos.push_back (i);

				result = pos [rand () % (int) pos.size ()];
			}

			return (result);
		};
}
