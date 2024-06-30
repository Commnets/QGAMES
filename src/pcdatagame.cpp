#include <PacManLike/pcdatagame.hpp>

// ---
PACMAN::DataGame::LevelDefinition* PACMAN::DataGame::LevelDefinition::cloneFor (int wId, int sId, int mId, int nI)
{
	return (new PACMAN::DataGame::LevelDefinition 
		(wId, sId, mId, nI,
		 pointsBall (), 
		 pointsPowerBall (),
		 secondsChasing (),
		 fruitConditions (),
		 scatterChaseCycles (),
		 leaveHomeConditions (),
		 pacmanSpeed (), 
		 pacmanSpeedWhenEatingDots (), 
		 pacmanSpeedWhenFrighting (), 
		 pacmanSpeedWhenEatingFrightingDots (),
		 ghostSpeed (), 
		 ghostSpeedWhenBeingFrighten (), 
		 ghostSpeedWhenExitingHome (), 
		 ghostSpeedWhenCrossingTunnel (),
		 elroyConditions ()));
}

// ---
PACMAN::DataGame::LevelDefinitions PACMAN::DataGame::classicCoreLevels ()
{
	DataGame::LevelDefinition::ScatterChaseCycles c1 =
		{ { 7.0f, 20.0f }, { 7.0f, 20.0f }, { 5.0f, 20.0f }, { 5.0f, 999999.0f } };
	DataGame::LevelDefinition::ScatterChaseCycles c2 =
		{ { 7.0f, 20.0f }, { 7.0f, 20.0f }, { 5.0f, 1033.0f }, { 0.0f, 999999.0f } };
	DataGame::LevelDefinition::ScatterChaseCycles c3 =
		{ { 5.0f, 20.0f }, { 5.0f, 20.0f }, { 5.0f, 1037.0f }, { 0.0f, 999999.0f } };

	// Original number of dots were a 70% bigger
	// The number of total dots to eat were originally 244. Now there is 170 maximum.
	// So the numbr of balls to eat before a monsters starts to move is 70% of the original one...
	// Times are not moved because it is not needed!
	DataGame::LevelDefinition::LeaveHomeConditions lH1 = 
		{ { 4.0f, { 0, 0, 21, 42 } }, { 4.0f, { 0, 5, 12, 22 } } };
	DataGame::LevelDefinition::LeaveHomeConditions lH2 = 
		{ { 3.0f, { 0, 0, 0, 35 } }, { 4.0f, { 0, 5, 12, 22 } } };
	DataGame::LevelDefinition::LeaveHomeConditions lH3 = 
		{ { 3.0f, { 0, 0, 0, 0 } }, { 4.0f, { 0, 5, 12, 22 } } };

	// The elroy situation in the original game only affects to blinky
	DataGame::LevelDefinition::ElroyConditions eC1 =
		{ { { true /** No more monsters. */, false }, { 10, 0 }, { 0.8f, 0.0f } }, 
		  { { true, false }, { 5, 0 }, { 0.85f, 0.0f } } };
	DataGame::LevelDefinition::ElroyConditions eC2 =
		{ { { true, false }, { 15 /** No more monsters. */, 0 }, { 0.9f, 0.0f } }, 
		  { { true, false }, { 7, 0 }, { 0.95f, 0.0f } } };
	DataGame::LevelDefinition::ElroyConditions eC3 =
		{ { { true, false }, { 20 /** No more monsters. */, 0 }, { 0.9f, 0.0f } }, 
		  { { true, false }, { 10, 0 }, { 0.95f, 0.0f } } };

	// As the number of normal balls in this version is estimated to be a 70% less than in the original one,
	// here the number of points per ball is 70% more than in the original one!
	// So instead 10 each, 17 should be taken. To avoid such awfull value...15 is choosen.
	// Sam wuth the power ball: 75 is the value choosen.
	// In the case of the fruit, they have to appear at 70 and (some levels) 170 dots eaten 
	// Applying the rule above they would be: 41 and 100, transformed into 40 and 100 respectively.
	DataGame::LevelDefinitions result =
		{ 
		  // Level 1
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICBLUEMAP__,
			-1, // Meaning none...
			15, 75, 8.0,
			{ { 4, 200, 40, 10.0, 0.0, 0 } }, // Cherries
			c1, lH1,
			0.80, 0.71, 0.90, 0.79, 
			0.75, 0.50, 0.40, 0.40,
			eC1 }),
		  // Level 2
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICBLUEMAP__,
			0, // The default one...
			15, 75, 8.0,
			{ { 0, 300, 40, 10.0, 0.0, 0 }, { 4, 100, 100, 10.0, 0.0, 0 } }, // Strawberry & Cherries
			c2, lH2,
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC2 }),
		  // Group II: Green
		  // Level 3
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICGREENMAP__,
			-1,
			15, 75, 8.0,
			{ { 3, 500, 50, 10.0, 0.0, 0 } }, // Peach
			c2, lH3, 
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC3 }),
		  // Level 4
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICGREENMAP__,
			-1,
			15, 75, 6.0,
			{ { 3, 500, 40, 10.0, 0.0, 0 }, { 0, 100, 100, 10.0, 0.0, 0 } }, // Peach & Strawberries 
			c2, lH3,
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC3 }),
		  // Level 5
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICGREENMAP__,
			0,
			15, 75, 6.0,
			{ { 1, 700, 40, 10.0, 0.0, 0 }, { 3, 500, 100, 10.0, 0.0, 0 }}, // Apple & Peach
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Group III: Yellow
		  // Level 6
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICYELLOWMAP__,
			-1,
			15, 75, 6.0,
			{ { 1, 700, 40, 10.0, 0.0, 0 } }, // Apple
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Level 7
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICYELLOWMAP__,
			-1,
			15, 75, 6.0,
			{ { 7, 1000, 40, 10.0, 0.0, 0 }, { 3, 500, 100, 10.0, 0.0, 0 } }, // Grapes & Peach
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Level 8
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICYELLOWMAP__,
			-1,
			15, 75, 6.0,
			{ { 7, 1000, 40, 10.0, 0.0, 0 }, { 1, 700, 100, 10.0, 0.0, 0 } }, // Grapes & Apple
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Level 9
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICYELLOWMAP__,
			0,
			15, 75, 6.0,
			{ { 10, 2000, 40, 10.0, 0.0, 0 }, { 1, 700, 40, 10.0, 0.0, 0 } }, // Star & Apple
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Group IV: Red
		  // Level 10
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICREDMAP__,
			-1,
			15, 75, 4.0,
			{ { 10, 2000, 40, 10.0, 0.0, 0 }, { 7, 1100, 1000, 10.0, 0.0, 0 } }, // Star & Grapes
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Level 11
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICREDMAP__,
			-1,
			15, 75, 4.0,
			{ { 11, 3000, 40, 10.0, 0.0, 0 }, { 7, 1000, 100, 10.0, 0.0, 0 } }, // Bell & Grapes
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Level 12
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICREDMAP__,
			-1,
			15, 75, 4.0,
			{ { 11, 3000, 40, 10.0, 0.0, 0 }, { 10, 2000, 100, 10.0, 0.0, 0 } }, // Bell & Star
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Level 13
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICREDMAP__,
			0,
			15, 75, 4.0,
			{ { 9, 5000, 40, 10.0, 0.0, 0 }, { 10, 2000, 100, 10.0, 0.0, 0 } }, // Key & Star
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Group V: Blue again...
		  // Level 14
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICBLUEMAP__,
			-1,
			15, 75, 4.0,
			{ { 9, 5000, 40, 10.0, 0.0, 0 }, { 11, 3000, 100, 10.0, 0.0, 0 } }, // Key & Bell
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Level 15
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICBLUEMAP__,
			-1,
			15, 75, 8.0,
			{ { 9, 5000, 40, 10.0, 0.0, 0 },  { 11, 3000, 40, 10.0, 0.0, 0 } }, // Key & Bell
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Level 16
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICBLUEMAP__,
			-1,
			15, 75, 8.0,
			{ { 9, 5000, 40, 10.0, 0.0, 0 }, { 9, 5000, 100, 10.0, 0.0, 0 } }, // Key & Key
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Level 17
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICBLUEMAP__,
			0,
			15, 75, 8.0,
			{ { 9, 5000, 40, 10.0, 0.0, 0 }, { 9, 5000, 100, 10.0, 0.0, 0 } }, // Key & Key
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Group V: One color per each...
		  // By the way, no more stops from here...
		  // Level 18
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICGREENMAP__,
			-1, 
			15, 75, 8.0,
			{ { 9, 5000, 40, 10.0, 0.0, 0 }, { 9, 5000, 100, 10.0, 0.0, 0 } }, // Key & Key
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Level 19
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICYELLOWMAP__,
			-1,
			15, 75, 6.0,
			{ { 9, 5000, 40, 10.0, 0.0, 0 } }, // Key
			c3, lH3,
			1.00, 0.87, 1.00, 0.87, 
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Level 20
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICREDMAP__,
			-1,
			15, 75, 4.0,
			{ { 9, 5000, 40, 10.0, 0.0, 0 } }, // Key
			c3, lH3,
			1.00, 0.87, 1.00, 0.87,
			0.95, 0.60, 0.40, 0.50,
			eC3 }),
		  // Level 21
		  new PACMAN::DataGame::LevelDefinition (
		  { __PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICBLUEMAP__,
			-1,
			15, 75, 4.0,
			{ { 9, 5000, 40, 10.0, 0.0, 0 }, { 9, 5000, 100, 10.0, 0.0, 0 } }, // Key & Key
			c3, lH3,
			1.00, 0.87, 1.00, 0.87,
			0.95, 0.60, 0.40, 0.50,
			eC3 })
		};
	
	return (result);
}

// ---
PACMAN::DataGame* PACMAN::DataGame::classicDataGame (int nL)
{
	PACMAN::DataGame::LevelDefinitions lD = PACMAN::DataGame::classicCoreLevels ();

	// There can't be less levels than the classic ones...
	assert (nL >= (int) lD.size ());

	// Repeats the last up to the number of levels...
	int lLD = (int) lD.size ();
	for (int i = lLD; i < nL && i < __PACMAN_MAXNUMBERLEVELS__; i++)
	{
		int nM = ((i % __PACMAN_BASICMAPNUMBERCOLORS__) == 0 
			? __PACMAN_BASICBLUEMAP__ 
			: ((i % __PACMAN_BASICMAPNUMBERCOLORS__) == 1 
				? __PACMAN_BASICGREENMAP__
				: ((i % __PACMAN_BASICMAPNUMBERCOLORS__) == 2 
					? __PACMAN_BASICYELLOWMAP__
					: __PACMAN_BASICREDMAP__))); 
		// One of each...

		// Repeat the last one...
		// But iterating the different colors!
		lD.push_back (lD [lLD - 1] -> cloneFor (__PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, nM, -1 /** No more stops. */));
	}

	return (new PACMAN::DataGame (lD, 10000)); // The new live it is at 10.000 points!
}

// ---
PACMAN::DataGame::DataGame (const PACMAN::DataGame::LevelDefinitions& l, int sL)
	: _levels (l),
	  _everyToGetAnExtraLive (sL)
{ 
	assert (!_levels.empty ()); 

	for (auto i : _levels)
		assert (dynamic_cast <PACMAN::DataGame::LevelDefinition*> (i) != nullptr);
} 

// ---
PACMAN::DataGame::DataGame (const PACMAN::DataGame& d)
	: _levels (
		[](const PACMAN::DataGame::LevelDefinitions& ls) -> PACMAN::DataGame::LevelDefinitions 
			{
				PACMAN::DataGame::LevelDefinitions result;
				for (auto i : ls)
					result.push_back (new PACMAN::DataGame::LevelDefinition (*i));
				return (result);
			}(d._levels)), 
	  _everyToGetAnExtraLive (d._everyToGetAnExtraLive)
{
	// Nothing else...
}

// ---
PACMAN::DataGame::~DataGame ()
{
	for (auto i : _levels)
		delete (i);
	// DataGame owns the definition of the levels...
}
