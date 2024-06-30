#include <BattleshipLike/bsdatagame.hpp>
#include <BattleshipLike/bsdefinitions.hpp>

BATTLESHIP::DataGame* BATTLESHIP::DataGame::_theDataGame = NULL;

// ---
BATTLESHIP::DataGame::DataGame ()
	: _ufoTypesList (),
	  _curveForMovementTypeMap ()
{
	// It can be created only once...
	assert (_theDataGame == NULL);

	_theDataGame = this;

	// The initial values are assigned
	// General attributes
	_numberOfLives										= 3;
	_numberOfThingsBeingCarried							= 3; // By default they are not managed...
	_numberOfPowerLevels								= 10;
	_numberOfShootingPillTypes							= 4;
	_numberOfThingsToCatchTypes							= _numberOfShootingPillTypes + 3; // The 4 types of missile + energy element+ life + shield
	_numberOfShootingTypes								= 8;
	_numberOfAsteroidTypes								= 6;
	_numberOfUFOTypes									= 3; // To be aligned!

	// Data related with the game
	_maxNumberOfAsteroidsInScene						= 50;
	_maxNumberOfShootingsPerEnemyInScene				= 50;
	_maxNumberOfEnemiesInScene							= 50; // Apart of asteriods, because they don't shoot...
	_maxNumberOfShootingsInScene						= (1 /** the battleship */ + _maxNumberOfEnemiesInScene) * 
														  _maxNumberOfShootingsPerEnemyInScene;
	_maxNumberOfThingsToCatchInScene					= 5;

	// Forms that can be configured for the game!
	_livesFormId										= 50009;
	_thingsToCarryFormId								= 50300;
	_numberOfThingsBeingCarriedTypes					= 10;

	// Sounds
	_spaceshipFiringSoundId								= 50008;
	_battleshipGetSoundId								= __QGAMES_CATCHWAVSOUND__;
	_battleshipLoadEnergySoundId						= __QGAMES_EATWAVSOUND__;
	_spaceshipExplodingSoundId							= __QGAMES_EXPLOSIONWAVSOUND__;
	_spaceshipHitsAnotherElmntSoundId					= __QGAMES_BEEPWAVSOUND__;

	// Score object Ids
	_spaceElementEnergyScoreObjectId					= 50000;
	_battleshipLivesScoreObjectId						= 50010;
	_battleshipThingsBeingCarriedScoreObjectId			= 50020;
	_battleshipThingBeingCarriedScoreObjectBaseId		= 50011;
	_spaceElementShootingsLeftScoreObjectId				= 50021;
	_battleshipTimeCounterScoreObjectId					= 50031;
	_battleshipPointsScoreObjectId						= 50032;
	_battleshipNameScoreObjectId						= 50100;

	// Entities of the game Ids
	_battleshipType1Id									= 50000;
	_battleshipType2Id									= 50001;

	// To stablish the level data
	// Only 10 default power level values maximum are defined
	// If the game allows more, then always the last is replicated...
	const int LEVELDATADEFAULTVALUES [10][5] = 
	{ { 0, 2  , 200, 1, 40 },
	  { 1, 5  , 200, 2, 40 }, 
	  { 1, 10 , 200, 2, 40 }, 
	  { 2, 15 , 200, 3, 40 }, 
	  { 2, 20 , 200, 3, 40 }, 
	  { 3, 30 , 400, 3, 40 }, 
	  { 4, 50 , 400, 4, 40 }, 
	  { 5, 60 , 400, 5, 40 }, 
	  { 6, 60 , 600, 5, 40 }, 
	  { 7, 100, 600, 5, 40 } 
	};

	_LEVELDATA = new int* [_numberOfPowerLevels];
	for (int i = 0; i < _numberOfPowerLevels; i++)
	{
		_LEVELDATA [i] = new int [5];
		for (int j = 0; j < 5; j++)
			_LEVELDATA [i][j] = LEVELDATADEFAULTVALUES [(i >= 10) ? 9 : i][j];
	}

	_clickNeededToCatchThings = false;
}

// ---
BATTLESHIP::DataGame::~DataGame ()
{ 
	delete [] _LEVELDATA; 
	
	_theDataGame  = NULL; 
}

// ---
std::vector <int> BATTLESHIP::DataGame::defaultStatesForUFOType (int uT) const
{
	std::vector <int> result;

	switch (uT)
	{
		case __BATTLESHIP_ENEMTYTYPE1__:
			result = std::vector <int> 
				{ 0, __BATTLESHIP_ENEMYBLANKSTATE__, 1, __BATTLESHIP_ENEMYEXPLODINGSTATE__, __BATTLESHIP_ENEMYBLANKSTATE__ } ;
			break;

		case __BATTLESHIP_ENEMTYTYPE2__:
			result = std::vector <int> 
				{ 2, __BATTLESHIP_ENEMYBLANKSTATE__, 3, __BATTLESHIP_ENEMYEXPLODINGSTATE__, __BATTLESHIP_ENEMYBLANKSTATE__ } ;
			break;

		case __BATTLESHIP_ENEMTYTYPE3__:
			result = std::vector <int> 
				{ 4, __BATTLESHIP_ENEMYBLANKSTATE__, 5, __BATTLESHIP_ENEMYEXPLODINGSTATE__, __BATTLESHIP_ENEMYBLANKSTATE__ } ;
			break;

		default:
			assert (0); // It shouldn't be here...
	}

	return (result);
}

// ---
const std::vector <int>& BATTLESHIP::DataGame::ufoTypesList () const
{
	if (_ufoTypesList.empty ())
		_ufoTypesList = std::vector <int> 
			{
				__BATTLESHIP_ENEMTYTYPE1__,
				__BATTLESHIP_ENEMTYTYPE2__,
				__BATTLESHIP_ENEMTYTYPE3__
			};

	return (_ufoTypesList);
}

// ---
QGAMES::Position BATTLESHIP::DataGame::defaultInitialPointForMovementType (int nC, 
	const QGAMES::Rectangle& vZ, const QGAMES::Rectangle& mZ)
{
	QGAMES::Position result = QGAMES::Position::_cero;

	switch (nC)
	{
		// At the top of the movement zone, taking into account the size...
		case __BATTLESHIP_UFOATTRTYPEMOVLINEAR0__:
			result = QGAMES::Vector (__BD (rand () % ((int) mZ.width ())), __BD 0, __BD 0);
			break;

		// At the top of the movement zone, taking into account the size...
		case __BATTLESHIP_UFOATTRTYPEMOVLINEAR1__:
			result = QGAMES::Vector (__BD 0, __BD (rand () % ((int) mZ.height ())),  __BD 0);
			break;

		// Random position in the middle of the visible zone...
		case __BATTLESHIP_UFOATTRTYPNOMOVEMENT__:
		case __BATTLESHIP_UFOATTRTYPEMOVCIRCULAR0__:
		case __BATTLESHIP_UFPATTRTYPEMOVEDANCING0__:
		case __BATTLESHIP_UFPATTRTYPEMOVEDANCING1__:
		case __BATTLESHIP_XYUFOATTRTYPEATTACKING0__:
		case __BATTLESHIP_XYUFOATTRTYPEATTACKING1__:
		case __BATTLESHIP_XYUFOATTRTYPEATTACKING2__:
		case __BATTLESHIP_XYUFOATTRTYPEATTACKING3__:
		case __BATTLESHIP_XYUFOATTRTYPEATTACKING4__:
			result = 
				QGAMES::Position (__BD (((rand () % 80) + 10) / __BD 100), // Between 10% and 90%
								  __BD (((rand () % 80) + 10) / __BD 100), __BD 0);
			break;

		// This type of curve shouldn't be allowed...
		default:
			assert (0); 
	}

	return (result);
}

// ---
std::map <int, double> BATTLESHIP::DataGame::defaultParametersForMovementType (int nC, 
	QGAMES::bdata sF, const QGAMES::Rectangle& vZ, const QGAMES::Rectangle& mZ, const QGAMES::Position& iP,
	QGAMES::bdata& tR)
{
	std::map <int, double> result { };
	tR = __BD 0; // Nothing...

	switch (nC)
	{
		case __BATTLESHIP_UFOATTRTYPNOMOVEMENT__:
			result = std::map <int, double> { };
			break;

		// The movement length is, maximum, the width of the movement zone...
		case __BATTLESHIP_UFOATTRTYPEMOVLINEAR0__:
			{
				QGAMES::Vector v = (QGAMES::Position (mZ.pos4 () + 
					QGAMES::Vector (__BD (rand () % (int) mZ.width ()), __BD 0, __BD 0)) - iP).normalize ();
				result = std::map <int, double> 
					{ { 0, (double) (v.module () < sF ? sF : v.module ()) }, // length... 
					  { 1, (double) sF }, // speed aceleration factor...
					  { 2, (double) v.posX () }, { 3, (double) v.posY () }, { 4, (double) v.posZ () } // direction 
					};
			}

			break;

		// The movement length is, maximum, the height of the movement zone...
		case __BATTLESHIP_UFOATTRTYPEMOVLINEAR1__:
			{
				QGAMES::Vector v = (QGAMES::Position (mZ.pos3 () + 
					QGAMES::Vector (__BD 0, __BD (rand () % (int) mZ.height ()), __BD 0)) - iP).normalize ();
				result = std::map <int, double> 
					{ { 0, (double) (v.module () < sF ? sF : v.module ()) }, // length... 
					  { 1, (double) sF }, // speed aceleration factor...
					  { 2, (double) v.posX () }, { 3, (double) v.posY () }, { 4, (double) v.posZ () } // direction 
					};
			}

			break;

		// The radix is a random between 50 and 100,
		// The movement plane is the axis XY,
		// The initial angle is 0, and the final angle 2PI...
		case __BATTLESHIP_UFOATTRTYPEMOVCIRCULAR0__:
			{
				double rdx = __BD ((rand () % 50) + 50);
				result = std::map <int, double> 
					{ { 0, (double) rdx }, // radix
					  { 1, (double) sF }, // speed aceleration factor 
					  { 2, 0.0 }, { 3, 0.0 }, { 4, 1.0 }, // plane
					  { 5, (double) 0 }, { 6, 2 * (double) __PI } // initial and final angle
					};
			}

			break;

		// 20 pixels length movement
		// in the (positive, so to the right) x axis
		case __BATTLESHIP_UFPATTRTYPEMOVEDANCING0__:
			{
				result = std::map <int, double> 
					{ { 0, 20.0 }, // length... 
					  { 1, (double) sF }, // speed aceleration factor...
					  { 2, 1.0 }, { 3, 0.0 }, { 4, 0.0 } // orientation
					};
			}

			break;

		// 20 pixels length movement
		// in the (positive, so to the bottom) y axis 
		case __BATTLESHIP_UFPATTRTYPEMOVEDANCING1__:
			{
				result = std::map <int, double> 
					{ { 0, 20.0 }, // length... 
					  { 1, (double) sF }, // speed aceleration factor...
					  { 2, 0.0 }, { 3, 1.0 }, { 4, 0.0 } // orientation
					};
			}

			break;

		// Circle radix between 50 and 100
		// Final angle from 0 to -PI/3
		// So the attacking curve will be from -PI up to that value...
		// The length of the line will have 1000 pixels long...
		case __BATTLESHIP_XYUFOATTRTYPEATTACKING0__:
			{
				double rdx = (double) ((rand () % 50) + 50);
				double eA =  ((double) -(rand () % (int) (__PI / __BD 3 * __BD 100))) / 100.0; 
				result = std::map <int, double> 
					{ { 0, rdx },
					  { 1, (double) sF },
					  { 2, eA },
					  { 3, 1000.0 } // Long enough to exit a very long screen at least!
					};
			}

			break;

		// Circle radix between 50 and 100
		// Final angle from PI to 5PI/4
		// So the attacking curve will be from -PI up to that value...
		// The length of the line will have 1000 pixels long...
		case __BATTLESHIP_XYUFOATTRTYPEATTACKING1__:
			{
				double rdx = (double) ((rand () % 50) + 50);
				double eA =  (((double) (rand () % (int) (__PI / __BD 4 * __BD 100))) / 100.0) + (double) __PI; 
				result = std::map <int, double> 
					{ { 0, rdx },
					  { 1, (double) sF },
					  { 2, eA },
					  { 3, 1000.0 } // Long enough to exit a very long screen at least! 
					};
			}

			break;

		// The line to follow depends on the initial position
		// Just when it is in the middle of the visual zon yhe movement will be an horizontal or vertical line
		// The arc will be from 0 radians to 5,75 up to 6,28 (full circle)
		// The distance to move is half the visible zone!
		case __BATTLESHIP_XYUFOATTRTYPEATTACKING2__:
		case __BATTLESHIP_XYUFOATTRTYPEATTACKING3__:
			{
				static std::map <int, QGAMES::bdata> tAngle1 =
					{ {  0, __BD 5 * __PI / __BD 3 }, 
					  {  1, __BD 5 * __PI / __BD 3 }, 
					  {  2, __BD 7 * __PI / __BD 6}, 
					  {  3, __BD 7 * __PI / __BD 6},
					  { 10, __BD 5 * __PI / __BD 3 }, 
					  { 11, __BD 5 * __PI / __BD 3 }, 
					  { 12, __BD 7 * __PI / __BD 6}, 
					  { 13, __BD 7 * __PI / __BD 6},
					  { 20, __PI / __BD 6}, 
					  { 21, __PI / __BD 6 }, 
					  { 22, __BD 2 * __PI / __BD 3 }, 
					  { 23, __BD 2 * __PI / __BD 3 },
					  { 30, __PI / __BD 6}, 
					  { 31, __PI / __BD 6 }, 
					  { 32, __BD 2 * __PI / __BD 3 }, 
					  { 33, __BD 2 * __PI / __BD 3 }
					}; 

				// First determine which cuadrant the ufo is in, taking into account the following cuadrant
				// The angle of the movement will depend on that initial position
				// -----------
				// 00|01|02|03
				// -----------
				// 10|11|12|13
				// -----------
				// 20|21|22|23
				// -----------
				// 30|31|32|33
				// -------------------------
				int zP = iP.posY () < __BD 0
					? 0 : (iP.posY () > vZ.pos2 ().posY ()
						? 30 : (iP.posY () > vZ.center ().posY ()
							? 20 : 10));
				zP += iP.posX () < __BD 0
					? 0 : (iP.posX () > vZ.pos2 ().posX ()
						? 3 : (iP.posX () > vZ.center ().posX ()
							? 2 : 1));

				double rdx = (double) ((rand () % 50) + 100); // random radix
				double iA = tAngle1 [zP] + 
					(double) (rand () % (int) (__PI / __BD 6 * __BD 100)) / 100.0; 
				// base + From 0 to __PI / 6 (30 grados)

				// Special angles, when the movement starts in the middle of the screen...
				if (iP.posY () == (vZ.height () / __BD 2))
					iA = (iP.posX () < (vZ.width () / __BD 2)) ? __BD 0 : __PI;
				if (iP.posX () == (vZ.width () / __BD 2))
					iA = (iP.posY () < (vZ.height () / __BD 2)) ? (__BD 3 * __PI / __BD 2) : (__PI / __BD 2);

				QGAMES::Vector v (__BD (std::cos (iA)), __BD -(std::sin (iA)), __BD 0);
				if (v.posX () < __BD 0) v = -v; // Just to get the points ordered...
				std::vector <QGAMES::Position> ps = mZ.intersectionWith (iP, v);
				assert ((int) ps.size () == 2); // The position might have been defined inside the moving zone!
				QGAMES::bdata dtance = 
					((ps [1] - ps [0]).module () -
						std::min ( // The min distance....
							(ps [0] - iP).module (),
							(ps [1] - iP).module ())) / __BD 2;

				result = std::map <int, double> 
					{ { 0, (double) dtance },
					  { 1, rdx }, 
					  { 2, iA },
					  { 3, __1IN2PROBABLE__ 
							? ((nC == __BATTLESHIP_XYUFOATTRTYPEATTACKING2__) 
								? __BD  2 * __PI : __BD  (__RANDOM__ (330) + 30) * __PI / __BD 180) 
							: ((nC == __BATTLESHIP_XYUFOATTRTYPEATTACKING2__) 
								? __BD -2 * __PI : __BD -(__RANDOM__ (330) + 30) * __PI / __BD 180) },
					  { 4, (double) sF }
					};
			}

			break;


		case __BATTLESHIP_XYUFOATTRTYPEATTACKING4__:
			{
				static std::map <int, QGAMES::bdata> tAngle2 =
					{ {  0, __BD 5 * __PI / __BD 3 }, 
					  {  1, __BD 5 * __PI / __BD 3 }, 
					  {  2, __BD 7 * __PI / __BD 6}, 
					  {  3, __BD 7 * __PI / __BD 6},
					  { 10, __BD 5 * __PI / __BD 3 }, 
					  { 11, __BD 5 * __PI / __BD 3 }, 
					  { 12, __BD 7 * __PI / __BD 6}, 
					  { 13, __BD 7 * __PI / __BD 6},
					  { 20, __PI / __BD 6}, 
					  { 21, __PI / __BD 6 }, 
					  { 22, __BD 2 * __PI / __BD 3 }, 
					  { 23, __BD 2 * __PI / __BD 3 },
					  { 30, __PI / __BD 6}, 
					  { 31, __PI / __BD 6 }, 
					  { 32, __BD 2 * __PI / __BD 3 }, 
					  { 33, __BD 2 * __PI / __BD 3 }
					};

				// First determine which cuadrant the ufo is in, taking into account the following cuadrant
				// The angle of the movement will depend on that initial position
				// -----------
				// 00|01|02|03
				// -----------
				// 10|11|12|13
				// -----------
				// 20|21|22|23
				// -----------
				// 30|31|32|33
				// -------------------------
				int zP = iP.posY () < __BD 0
					? 0 : (iP.posY () > vZ.pos2 ().posY ()
						? 30 : (iP.posY () > vZ.center ().posY ()
							? 20 : 10));
				zP += iP.posX () < __BD 0
					? 0 : (iP.posX () > vZ.pos2 ().posX ()
						? 3 : (iP.posX () > vZ.center ().posX ()
							? 2 : 1));

				double rng = (double) ((rand () % 50) + 50); // random...
				double prd = (double) ((rand () % 25) + 50); // random...
				QGAMES::bdata iA = tAngle2 [zP] + __BD ((rand () % (int) (__PI / __BD 6 * __BD 100)) / 100.0); 
				// base + From 0 to __PI / 6 (30 grados)

				// Special angles, when the movement starts in the middle of the screen...
				if (iP.posY () == (vZ.height () / __BD 2))
					iA = (iP.posX () < (vZ.width () / __BD 2)) ? __BD 0 : __PI;
				if (iP.posX () == (vZ.width () / __BD 2))
					iA = (iP.posY () < (vZ.height () / __BD 2)) ? (__BD 3 * __PI / __BD 2) : (__PI / __BD 2);

				result = std::map <int, double> 
					{ { 0, rng },
					  { 1, prd },
					  { 2, (double) sF },
					  { 3, 0.0 }, { 4, 0.0 }, { 5, 1.0 }, // XY Plane...
					  { 6, 0.0 },
					  { 7, (double) (__BD 2 * __PI) }
					};

				tR = -iA;
			}

			break;

		// Not allowed type of curve...
		default:
			assert (0); // It shouldn't be here...

	}

	return (result);
}

// ---
const std::map <int, int>& BATTLESHIP::DataGame::curveforMovementTypeMap () const
{
	if (_curveForMovementTypeMap.empty ())
		_curveForMovementTypeMap = std::map <int, int>
			{ 
				{ __BATTLESHIP_UFOATTRTYPNOMOVEMENT__	, __QGAMES_NOMOVEMENTTRACE__				}, 
				{ __BATTLESHIP_UFOATTRTYPEMOVLINEAR0__	, __QGAMES_LINEARCURVEMOVEMENTTRACE___		},
				{ __BATTLESHIP_UFOATTRTYPEMOVLINEAR1__	, __QGAMES_LINEARCURVEMOVEMENTTRACE___		},
				{ __BATTLESHIP_UFOATTRTYPEMOVCIRCULAR0__, __QGAMES_CIRCULARCURVEMOVEMENTTRACE___	},
				{ __BATTLESHIP_UFPATTRTYPEMOVEDANCING0__, __QGAMES_LINEARDANCINGCURVEMOVTRACE___	},
				{ __BATTLESHIP_UFPATTRTYPEMOVEDANCING1__, __QGAMES_LINEARDANCINGCURVEMOVTRACE___	},
				{ __BATTLESHIP_XYUFOATTRTYPEATTACKING0__, __QGAMES_XYCIRCLEANDLINERIGHTMOVTRACE__	},
				{ __BATTLESHIP_XYUFOATTRTYPEATTACKING1__, __QGAMES_XYCIRCLEANDLINELEFTMOVTRACE__	},
				{ __BATTLESHIP_XYUFOATTRTYPEATTACKING2__, __QGAMES_XYLINECIRCLELINEMOVTRACE__		},
				{ __BATTLESHIP_XYUFOATTRTYPEATTACKING3__, __QGAMES_XYLINECIRCLELINEMOVTRACE__		},
				{ __BATTLESHIP_XYUFOATTRTYPEATTACKING4__, __QGAMES_SINUSOIDALCURVEMOVEMENTTRACE__	}
			};

	return (_curveForMovementTypeMap);
}
