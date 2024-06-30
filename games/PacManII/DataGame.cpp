#include "DataGame.hpp"
#include "Monsters.hpp"
#include "Defs.hpp"

// ---
PACMAN::DataGame::LevelDefinition* PacManII::DataGame::LevelDefinition::cloneFor (int wId, int sId, int mId, int nI)
{
	return (new PacManII::DataGame::LevelDefinition 
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
		 elroyConditions (),
		 additionalMonsters ()));
}

// ---
PacManII::DataGame* PacManII::DataGame::trainingDataGame ()
{
	DataGame::LevelDefinition::ScatterChaseCycles sC =
		{ { 7.0f, 20.0f }, { 7.0f, 20.0f }, { 5.0f, 20.0f }, { 5.0f, 999999.0f } };
	DataGame::LevelDefinition::LeaveHomeConditions lH = 
		{ { 4.0f, { 0, 0, 21, 42 } }, { 4.0f, { 0, 5, 12, 22 } } };
	DataGame::LevelDefinition::ElroyConditions eC =
		{ { { true /** No more monsters. */, false }, { 10, 0 }, { 0.8f, 0.0f } }, 
		  { { true, false }, { 5, 0 }, { 0.85f, 0.0f } } };
	PacManII::DataGame::LevelDefinition::AdditionalMonster aM1 
		(__PACMANII_WORMYBASEENTITYID__, PacManII::Wormy::_NUMBER, QGAMES::MazeModel::_noPosition, 10, __BD 5.0, 600, 20); 
	// Wormy 1: after 10 balls eaten or after 5.0 seconds no eaten anything. 600 points and 20 images length...
	PacManII::DataGame::LevelDefinition::AdditionalMonster aM2
		(__PACMANII_WORMYBASEENTITYID__ + 1, PacManII::Wormy::_NUMBER, QGAMES::MazeModel::_noPosition, 20, __BD 7.0, 600, 20); 
	// Wormy 2: after 20 balls eaten or after 7.0 seconds no eaten anything. 600 points and 20 images length...

	// Just 4 levels to train...
	DataGame::LevelDefinitions result =
		{ 
		  // Level 1
		  new PacManII::DataGame::LevelDefinition (
		  { __PACMANII_EXTENDEDWORLD__, __PACMANII_BLOCKSCENES3MS1__, __PACMANII_MISSPACMAN1BLUEMAP__,
			0, // The standard one...
			15, 75, 8.0,
			{ { 0, 200, 40, 10.0, 0.25, 0 } }, 
			sC, lH,
			0.80, 0.71, 0.90, 0.79, 
			0.75, 0.50, 0.40, 0.40,
			eC,
			{ aM1 } }),
		  // Level 2
		  new PacManII::DataGame::LevelDefinition (
		  { __PACMANII_EXTENDEDWORLD__, __PACMANII_BLOCKSCENES3MS2__, __PACMANII_MISSPACMAN2REDMAP__,
			0,
			15, 75, 8.0,
			{ { 1, 300, 40, 10.0, 0.25, 1 }, { 0, 300, 100, 10.0, 0.25, 2 } }, // two fruits... 
			sC, lH,
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC,
			{ aM1 } }),
		  // Level 3
		  new PacManII::DataGame::LevelDefinition (
		  { __PACMANII_EXTENDEDWORLD__, __PACMANII_BLOCKSCENES3MS3__, __PACMANII_MISSPACMAN3GREENMAP__,
			0,
			15, 75, 8.0,
			{ { 2, 400, 50, 10.0, 0.25, 0 } },
			sC, lH, 
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC,
			{ aM1, aM2 } }),
		  // Level 4
		  new PacManII::DataGame::LevelDefinition (
		  { __PACMANII_EXTENDEDWORLD__, __PACMANII_BLOCKSCENES3MS4__, __PACMANII_MISSPACMAN4YELLOWMAP__,
			0,
			15, 75, 8.0,
			{ { 2, 400, 50, 10.0, 0.25, 1 } },
			sC, lH, 
			0.90, 0.79, 0.95, 0.83, 
			0.85, 0.55, 0.40, 0.45,
			eC,
			{ aM1, aM2 } })
		};

	return (new PacManII::DataGame (result, 5000)); // Easier to get a new live, as we are testing!
}

// ---
PacManII::DataGame* PacManII::DataGame::missDataGame (int nL)
{
	// The same "difficuly" than classic game...
	PACMAN::DataGame::LevelDefinitions lD = PACMAN::DataGame::classicCoreLevels ();

	// There can't be less levels than the classical ones...
	assert (nL >= (int) lD.size ());

	// ...but the image of miss pacman (and chaning th different maps as the standard says...
	int lLD = (int) lD.size ();
	PacManII::DataGame::LevelDefinitions nLD;
	for (int i = 0; i < nL && i < __PACMAN_MAXNUMBERLEVELS__; i++)
	{
		// nB = Every three scenes, the type of miss pacman maze has to change.
		// tB = There are 4 types of miss pacman maze.
		// eNB = The number of scene within the block of every three.
		// nB = 0 && eNB = 1,2 
		// means that the very two first mazes are always miss pacman type 1 (tB = 0) and miss pacman type 2 (tB = 1) respectively

		int nB = (i + 1) / 3; // Block number (0, 1, 2, ...)
		int tB = nB % 4; // There are 4 types of ms. pacman maze possible, and they repeat every block of 3!.
		int eNB = (i + 1) % 3; // The element in that block (0, 1, 2, 0, 1, 2, ...) 

		// The number of scene to apply
		int nScn = 
			(nB == 0) 
				? ((eNB == 1) // For the block 0, it starts in 1...
					? __PACMANII_BLOCKSCENES2MS1__  
					: __PACMANII_BLOCKSCENES2MS2__)
				: ((tB == 0) // For the rest of blocks, the mazes repeteats every 4!
							? __PACMANII_BLOCKSCENES2MS1__ 
							: ((tB == 1) 
								? __PACMANII_BLOCKSCENES2MS2__ 
								: ((tB == 2)
									? __PACMANII_BLOCKSCENES2MS3__  
									: __PACMANII_BLOCKSCENES2MS4__))); 

		// The number of map to apply...
		int nM = 
			(nB == 0) 
				? ((eNB == 1) // The first two levels dont't repeat...
					? __PACMANII_MISSPACMAN1BLUEMAP__  
					: __PACMANII_MISSPACMAN2GREENMAP__)
				: ((tB == 0) 
							? __PACMANII_MISSPACMAN1BLUEMAP__ // We define the type 1 always is blue...
							: ((tB == 1) 
								? __PACMANII_MISSPACMAN2REDMAP__ // ...type 2 always in red...
								: ((tB == 2)
									? __PACMANII_MISSPACMAN3GREENMAP__  // ...type 3 always in green...
									: __PACMANII_MISSPACMAN4YELLOWMAP__))); // ...and type 4 always in yellow...

		// The first levels are like classic ones in terms of difficulty,...
		// except for the case of the fruit that moves (225% normal speed) 
		// for a while (20.0 seconds) and also appears in the tunnels!
		PACMAN::DataGame::LevelDefinition* oLD = (i < lLD) ? lD [i] : lD [lLD - 1];
		if (i < lLD)
		{
			nLD.push_back ( 
				new PACMAN::DataGame::LevelDefinition (
					__PACMANII_EXTENDEDWORLD__,
					nScn, 
					nM,
					(*oLD).intermissionAfterEnd (),
					(*oLD).pointsBall (), 
					(*oLD).pointsPowerBall (),
					(*oLD).secondsChasing (),
					changeFruitTimeSpeedAndPosition 
						((*oLD).fruitConditions (), 20.0f, 0.25f, 1 /** Meaning sequential if more than one */),
					(*oLD).scatterChaseCycles (),
					(*oLD).leaveHomeConditions (),
					(*oLD).pacmanSpeed (), 
					(*oLD).pacmanSpeedWhenEatingDots (), 
					(*oLD).pacmanSpeedWhenFrighting (), 
					(*oLD).pacmanSpeedWhenEatingFrightingDots (),
					(*oLD).ghostSpeed (), 
					(*oLD).ghostSpeedWhenBeingFrighten (), 
					(*oLD).ghostSpeedWhenExitingHome (), 
					(*oLD).ghostSpeedWhenCrossingTunnel (),
					(*oLD).elroyConditions ()));
		}
		// The rest are a copy of the last one in terms of difficulty and number of fruits
		// but adapting always the maze!
		else
			nLD.push_back (oLD -> cloneFor (__PACMANII_EXTENDEDWORLD__, nScn, nM, (*oLD).intermissionAfterEnd ()));
	}

	// The classical levels are no longer used...
	for (auto i : lD)
		delete (i); // ...so to delete them...

	return (new PacManII::DataGame (nLD, 10000));
}

// ---
PacManII::DataGame* PacManII::DataGame::mineDataGame (int nL)
{
	assert (nL >= 1);

	PacManII::DataGame::LevelDefinition::AdditionalMonster aM1 
		(__PACMANII_WORMYBASEENTITYID__, PacManII::Wormy::_NUMBER, QGAMES::MazeModel::_noPosition, 100, __BD 5.0, 600, 12); 
	// Wormy: When 100 balls were eaten or after 5 seconds no eaten anything. 600 points and 12 images length...

	// Get the core levels...
	PACMAN::DataGame::LevelDefinitions lD = PACMAN::DataGame::classicCoreLevels ();

	// ...and introduce Wormy monsters (only one) scenes randomly just in a set of them (1 every 5)...
	int lLD = (int) lD.size ();
	int nELA = (lLD / 5); // Every 5 more or less...
	if (nELA > __PACMANII_MAXNUMBERXTENDEDLEVELS__) nELA = __PACMANII_MAXNUMBERXTENDEDLEVELS__;
	std::vector <int> wL (nELA, -1); // -1 means not used...
	for (int i = 0; i < nELA; i++) 
	{
		int pWL = rand () % lLD;
		while (std::find (wL.begin (), wL.end (), pWL) != wL.end ())
			if (++pWL == lLD) pWL = 0; // If already used, get the next...
		assert (wL [i] == -1);
		wL [i] = pWL; // Write it down as used!

		// Replace the selected level definition
		// by a new one with the same level of complexity, but inclusing monsters...
		PACMAN::DataGame::LevelDefinition* oLD = lD [pWL];
		lD [pWL] = new PacManII::DataGame::LevelDefinition
			(
				// Some conditions of the level, but with Wormy...
				__PACMANII_EXTENDEDWORLD__,
				__PACMANII_BLOCKSCENES1__, 
				__PACMAN_BASICBLUEMAP__ + ((rand () % __PACMAN_BASICMAPNUMBERCOLORS__) * 100),
				-1, // It will be fixed later...
				(*oLD).pointsBall (), 
				(*oLD).pointsPowerBall (),
				(*oLD).secondsChasing (),
				(*oLD).fruitConditions (),
				(*oLD).scatterChaseCycles (),
				(*oLD).leaveHomeConditions (),
				(*oLD).pacmanSpeed (), 
				(*oLD).pacmanSpeedWhenEatingDots (), 
				(*oLD).pacmanSpeedWhenFrighting (), 
				(*oLD).pacmanSpeedWhenEatingFrightingDots (),
				(*oLD).ghostSpeed (), 
				(*oLD).ghostSpeedWhenBeingFrighten (), 
				(*oLD).ghostSpeedWhenExitingHome (), 
				(*oLD).ghostSpeedWhenCrossingTunnel (),
				(*oLD).elroyConditions (),
				{ aM1 }); // Same definition but including monsters...

		delete (oLD);
	}

	// Now it is time to add the rest of the levels...
	// Getting the attributes of a random one, but with a miss pacman aspect...
	for (int i = lLD; i < nL && i < __PACMAN_MAXNUMBERLEVELS__; i++)
	{
		PACMAN::DataGame::LevelDefinition* lDR = lD [rand () % lLD]; // Which one to repeat?

		// Number of type of miss map to be choosen...
		int nTMM = rand () % __PACMANII_MISSPACMANTYPEMAPS__;
		// Number of the color of that miss map to be choosen...
		int nCMM = rand () % __PACMAN_BASICMAPNUMBERCOLORS__;

		// The would be two possible type of scenes to be selected: One with wormy and the other whithout...
		int nScn = ((dynamic_cast <PacManII::DataGame::LevelDefinition*> (lDR) == nullptr)
			? std::vector <int> 
				({ __PACMANII_BLOCKSCENES2MS1__, 
				   __PACMANII_BLOCKSCENES2MS2__, 
				   __PACMANII_BLOCKSCENES2MS3__, 
				   __PACMANII_BLOCKSCENES2MS4__ })
			: std::vector <int> 
				({ __PACMANII_BLOCKSCENES3MS1__, 
				   __PACMANII_BLOCKSCENES3MS2__, 
				   __PACMANII_BLOCKSCENES3MS3__, 
				   __PACMANII_BLOCKSCENES3MS4__ }))[nTMM];

		// Select the map to be considered, attending to the type of miss map selected,
		// and regarding the number of the color with the type of map selected above!
		int nMP = ((nTMM == 0) 
			? std::vector <int> 
				({ __PACMANII_MISSPACMAN1BLUEMAP__, 
				   __PACMANII_MISSPACMAN1REDMAP__, 
				   __PACMANII_MISSPACMAN1GREENMAP__, 
				   __PACMANII_MISSPACMAN1YELLOWMAP__ })
			: (nTMM == 1) 
				? std::vector <int> 
					({ __PACMANII_MISSPACMAN2BLUEMAP__, 
					   __PACMANII_MISSPACMAN2REDMAP__, 
					   __PACMANII_MISSPACMAN2GREENMAP__, 
					   __PACMANII_MISSPACMAN2YELLOWMAP__ })
				: (nTMM == 2) 
					? std::vector <int> 
						({ __PACMANII_MISSPACMAN3BLUEMAP__, 
						   __PACMANII_MISSPACMAN3REDMAP__, 
						   __PACMANII_MISSPACMAN3GREENMAP__, 
						   __PACMANII_MISSPACMAN3YELLOWMAP__ })
					: std::vector <int> 
						({ __PACMANII_MISSPACMAN4BLUEMAP__, 
						   __PACMANII_MISSPACMAN4REDMAP__, 
						   __PACMANII_MISSPACMAN4GREENMAP__, 
						   __PACMANII_MISSPACMAN4YELLOWMAP__ }))[nCMM];

		// Clone the scene taking into account whether the original one has or nor monsters!
		// However the fruit behaviour is always changed: Now it moves (25% standard speed) and for 20 seconds after appearing!
		if (dynamic_cast <PACMAN::DataGame::LevelDefinition*> (lDR) != nullptr)
		{
			lD.push_back 
				(new PACMAN::DataGame::LevelDefinition (
					__PACMANII_EXTENDEDWORLD__, 
					nScn, 
					nMP,
					-1, // It will be fixed later...
					(*lDR).pointsBall (), 
					(*lDR).pointsPowerBall (),
					(*lDR).secondsChasing (),
					changeFruitTimeSpeedAndPosition 
						((*lDR).fruitConditions (), 20.0f, 0.25f, 1 /** Meaning sequential from 0 if more than 1. */),
					(*lDR).scatterChaseCycles (),
					(*lDR).leaveHomeConditions (),
					(*lDR).pacmanSpeed (), 
					(*lDR).pacmanSpeedWhenEatingDots (), 
					(*lDR).pacmanSpeedWhenFrighting (), 
					(*lDR).pacmanSpeedWhenEatingFrightingDots (),
					(*lDR).ghostSpeed (), 
					(*lDR).ghostSpeedWhenBeingFrighten (), 
					(*lDR).ghostSpeedWhenExitingHome (), 
					(*lDR).ghostSpeedWhenCrossingTunnel (),
					(*lDR).elroyConditions ()));
		}
		else
		{
			lD.push_back 
				(new PacManII::DataGame::LevelDefinition (
					__PACMANII_EXTENDEDWORLD__, 
					nScn, 
					nMP,
					-1, // It will be fixed later...
					(*lDR).pointsBall (), 
					(*lDR).pointsPowerBall (),
					(*lDR).secondsChasing (),
					changeFruitTimeSpeedAndPosition 
						((*lDR).fruitConditions (), 20.0f, 0.25f, 1 /** Meaning sequential from 0 if more than 1. */),
					(*lDR).scatterChaseCycles (),
					(*lDR).leaveHomeConditions (),
					(*lDR).pacmanSpeed (), 
					(*lDR).pacmanSpeedWhenEatingDots (), 
					(*lDR).pacmanSpeedWhenFrighting (), 
					(*lDR).pacmanSpeedWhenEatingFrightingDots (),
					(*lDR).ghostSpeed (), 
					(*lDR).ghostSpeedWhenBeingFrighten (), 
					(*lDR).ghostSpeedWhenExitingHome (), 
					(*lDR).ghostSpeedWhenCrossingTunnel (),
					(*lDR).elroyConditions (),
					static_cast <PacManII::DataGame::LevelDefinition*> (lDR) ->additionalMonsters ()));
		}
	}

	// And now it is time to mix all of them...
	int nLD = (int) lD.size ();
	std::vector <int> wLR (nLD, -1); // -1 means not used...
	PACMAN::DataGame::LevelDefinitions lDR;
	std::vector <int> (nLD, -1); // -1 meaning not used...
	for (int i = 0; i < nLD; i++)
	{
		int pWL = rand () % nLD;
		while (std::find (wLR.begin (), wLR.end (), pWL) != wLR.end ())
			if (++pWL == nLD) pWL = 0; // If already used,, get the next...
		assert (wLR [i] == -1);
		wLR [i] = pWL; // Write it down as used!

		// The levels having an intermedite mission screen...
		// NOTE than in this loop the levels start from 0!
		std::vector <int> iMLevels ({ 1, 4, 8, 12, 16 }); 
		lDR.push_back (lD [pWL] -> cloneFor 
			(lD [pWL] -> worldTypeId (), lD [pWL] -> sceneTypeId (), lD [pWL] -> mapTypeId (), // Same levels...
				(std::find (iMLevels.begin (), iMLevels.end (), i) == iMLevels.end ()) ? -1 : 0 /** The standard one. */));
	}

	// The elements of lD are no longer needed...
	for (auto i : lD)
		delete (i);

	return (new PacManII::DataGame (lDR, 10000));
}

// ---
PacManII::DataGame* PacManII::DataGame::hardDataGame (int nL)
{
	assert (nL >= 1);

	// Hardest conditions in clasical pacman...
	DataGame::LevelDefinition::ScatterChaseCycles sC =
		{ { 5.0f, 20.0f }, { 5.0f, 20.0f }, { 5.0f, 1037.0f }, { 0.0f, 999999.0f } };
	DataGame::LevelDefinition::LeaveHomeConditions lH = 
		{ { 3.0f, { 0, 0, 0, 0 } }, { 4.0f, { 0, 5, 12, 22 } } };
	DataGame::LevelDefinition::ElroyConditions eC =
		{ { { true, false }, { 20 /** No more monsters. */, 0 }, { 0.9f, 0.0f } }, 
		  { { true, false }, { 10, 0 }, { 0.95f, 0.0f } } };

	// To add even more difficult additional monsters will be considered
	PacManII::DataGame::LevelDefinition::AdditionalMonster aM1 
		(__PACMANII_WORMYBASEENTITYID__, PacManII::Wormy::_NUMBER, QGAMES::MazeModel::_noPosition, 100, __BD 5.0, 600, 20);
	// Wormy: After 100 balls, of 5 seconds not doing anything, 600 points for eating it, and 20 ghost length...
	PacManII::DataGame::LevelDefinition::AdditionalMonster aM2 
		(__PACMANII_WORMYBASEENTITYID__, PacManII::Wormy::_NUMBER, QGAMES::MazeModel::_noPosition, 70, __BD 4.0, 600, 30);
	// Wormy: After only 70 balls, of 4 seconds not doing anything, 600 points for eating it, and 30 ghost length...
	PacManII::DataGame::LevelDefinition::AdditionalMonster aM3 
		(__PACMANII_WORMYBASEENTITYID__, PacManII::Wormy::_NUMBER, QGAMES::MazeModel::_noPosition, 50, __BD 3.0, 1000, 40);
	// Crazy wormy: After only 50 balls, of 3 seconds not doing anything, 1000! points for eating it, and 40 ghost length...

	// Then...It is considered the max level of difficulty in the classical version
	PACMAN::DataGame::LevelDefinition* slD = // Just a template used in many... No monsters but hard!
		  new PACMAN::DataGame::LevelDefinition (
			  { 0, 0, 0, // To define better later...
			    -1, // It will be also fixed later...
				15, 75, 6.0,
				{ { 0, 200, 40, 10.0, 0.0, 0 } }, 
				sC, lH,
				1.00, 0.87, 1.00, 0.87, 
				0.95, 0.60, 0.40, 0.50, 
				eC });

	// Now it is time to buid up the different levels...
	// There aree a couple of levels having an intermission screen after
	// NOTE that for the purpose of this loop the levels start from 0
	std::vector <int> iMLevels ({ 1, 4, 8, 12, 16 });
	PACMAN::DataGame::LevelDefinitions lD;
	for (int i = 0; i < nL && i < __PACMAN_MAXNUMBERLEVELS__; i++)
	{
		int iMSCR = (std::find (iMLevels.begin (), iMLevels.end (), i) == iMLevels.end ()) ? -1 : 0; // The standard one...

		// The first levels doesn't have monsters and are classic (and green)
		if (i < (3 * __PACMAN_BASICMAPNUMBERCOLORS__))
			lD.push_back (slD -> cloneFor 
				(__PACMAN_BASICWORLD__, __PACMAN_BASICBLOCKSCENES__, __PACMAN_BASICGREENMAP__, iMSCR));
		else
		// Classic but with a monster...
		if (i == (3 * __PACMAN_BASICMAPNUMBERCOLORS__))
			lD.push_back (new PacManII::DataGame::LevelDefinition
				(
					__PACMANII_EXTENDEDWORLD__,
					__PACMANII_BLOCKSCENES1__,
					__PACMAN_BASICBLUEMAP__,
					iMSCR, 
					(*slD).pointsBall (), 
					(*slD).pointsPowerBall (),
					(*slD).secondsChasing (),
					(*slD).fruitConditions (),
					(*slD).scatterChaseCycles (),
					(*slD).leaveHomeConditions (),
					(*slD).pacmanSpeed (), 
					(*slD).pacmanSpeedWhenEatingDots (), 
					(*slD).pacmanSpeedWhenFrighting (), 
					(*slD).pacmanSpeedWhenEatingFrightingDots (),
					(*slD).ghostSpeed (), 
					(*slD).ghostSpeedWhenBeingFrighten (), 
					(*slD).ghostSpeedWhenExitingHome (), 
					(*slD).ghostSpeedWhenCrossingTunnel (),
					(*slD).elroyConditions (),
					{ aM1 }));
		else
		// Classic but with a monster stronger...
		if (i == ((3 * __PACMAN_BASICMAPNUMBERCOLORS__) + 1))
			lD.push_back (new PacManII::DataGame::LevelDefinition
				(
					__PACMANII_EXTENDEDWORLD__,
					__PACMANII_BLOCKSCENES1__,
					__PACMAN_BASICGREENMAP__,
					iMSCR, 
					(*slD).pointsBall (), 
					(*slD).pointsPowerBall (),
					(*slD).secondsChasing (),
					(*slD).fruitConditions (),
					(*slD).scatterChaseCycles (),
					(*slD).leaveHomeConditions (),
					(*slD).pacmanSpeed (), 
					(*slD).pacmanSpeedWhenEatingDots (), 
					(*slD).pacmanSpeedWhenFrighting (), 
					(*slD).pacmanSpeedWhenEatingFrightingDots (),
					(*slD).ghostSpeed (), 
					(*slD).ghostSpeedWhenBeingFrighten (), 
					(*slD).ghostSpeedWhenExitingHome (), 
					(*slD).ghostSpeedWhenCrossingTunnel (),
					(*slD).elroyConditions (),
					{ aM2 }));
		else
		// Classic but with a monster more stronger...
		if (i == ((3 * __PACMAN_BASICMAPNUMBERCOLORS__) + 2))
			lD.push_back (new PacManII::DataGame::LevelDefinition
				(
					__PACMANII_EXTENDEDWORLD__,
					__PACMANII_BLOCKSCENES1__,
					__PACMAN_BASICYELLOWMAP__,
					iMSCR,					
					(*slD).pointsBall (), 
					(*slD).pointsPowerBall (),
					(*slD).secondsChasing (),
					(*slD).fruitConditions (),
					(*slD).scatterChaseCycles (),
					(*slD).leaveHomeConditions (),
					(*slD).pacmanSpeed (), 
					(*slD).pacmanSpeedWhenEatingDots (), 
					(*slD).pacmanSpeedWhenFrighting (), 
					(*slD).pacmanSpeedWhenEatingFrightingDots (),
					(*slD).ghostSpeed (), 
					(*slD).ghostSpeedWhenBeingFrighten (), 
					(*slD).ghostSpeedWhenExitingHome (), 
					(*slD).ghostSpeedWhenCrossingTunnel (),
					(*slD).elroyConditions (),
					{ aM3 }));
		else
		// Classic but with two monsters...
		if (i == ((3 * __PACMAN_BASICMAPNUMBERCOLORS__) + 3))
			lD.push_back (new PacManII::DataGame::LevelDefinition
				(
					__PACMANII_EXTENDEDWORLD__,
					__PACMANII_BLOCKSCENES1__,
					__PACMAN_BASICREDMAP__,
					iMSCR,
					(*slD).pointsBall (), 
					(*slD).pointsPowerBall (),
					(*slD).secondsChasing (),
					(*slD).fruitConditions (),
					(*slD).scatterChaseCycles (),
					(*slD).leaveHomeConditions (),
					(*slD).pacmanSpeed (), 
					(*slD).pacmanSpeedWhenEatingDots (), 
					(*slD).pacmanSpeedWhenFrighting (), 
					(*slD).pacmanSpeedWhenEatingFrightingDots (),
					(*slD).ghostSpeed (), 
					(*slD).ghostSpeedWhenBeingFrighten (), 
					(*slD).ghostSpeedWhenExitingHome (), 
					(*slD).ghostSpeedWhenCrossingTunnel (),
					(*slD).elroyConditions (),
					{ aM1, aM2 }));
		// From now onwards the screens are Ms. Pacman...
		else
		{
			int nB = i / 3; // Block number (0, 1, 2, ...)
			int eNB = i % 3; // The element in that block (0, 1, 2, 0, 1, 2, ...) 
			int tB = nB % 4; 

			// The number of scene to apply
			int nScn = ( 
				(nB == 0) 
					? ((eNB == 1) 
						? std::vector <int> 
							({ __PACMANII_BLOCKSCENES2MS1__, 
							  __PACMANII_BLOCKSCENES3MS1__ })   
						: std::vector <int> 
							({ __PACMANII_BLOCKSCENES2MS2__, 
							   __PACMANII_BLOCKSCENES3MS2__ }))
					: ((tB == 0) 
						? std::vector <int> 
							({ __PACMANII_BLOCKSCENES2MS2__, 
							   __PACMANII_BLOCKSCENES3MS2__ }) 
								: ((tB == 1) 
									? std::vector <int> 
										({ __PACMANII_BLOCKSCENES2MS2__, 
										   __PACMANII_BLOCKSCENES3MS2__ }) 
									: ((tB == 2)
										? std::vector <int> 
											({ __PACMANII_BLOCKSCENES2MS3__, 
											   __PACMANII_BLOCKSCENES3MS3__ }) 
										: std::vector <int> 
											({__PACMANII_BLOCKSCENES2MS4__, 
											  __PACMANII_BLOCKSCENES2MS4__ }))))
				)[__1IN2PROBABLE__]; 

			// The number of map to apply...
			int nM = 
				(nB == 0) 
					? ((eNB == 1) // The first two levels dont't repeat...
						? __PACMANII_MISSPACMAN1BLUEMAP__  
						: __PACMANII_MISSPACMAN2GREENMAP__)
					: ((tB == 0) 
								? __PACMANII_MISSPACMAN1BLUEMAP__ // type 1 always is blue...
								: ((tB == 1) 
									? __PACMANII_MISSPACMAN2REDMAP__ // type 2 always in green...
									: ((tB == 2)
										? __PACMANII_MISSPACMAN3GREENMAP__  // type 3 always in yellow...
										: __PACMANII_MISSPACMAN4YELLOWMAP__))); // ...and type 4 always in red...
																				
			// With or without monsters... (no so difficult)
			lD.push_back (
				new PACMAN::DataGame::LevelDefinition (
					__PACMANII_EXTENDEDWORLD__, 
					nScn, 
					nM,
					iMSCR,
					(*slD).pointsBall (), 
					(*slD).pointsPowerBall (),
					(*slD).secondsChasing (),
					changeFruitTimeSpeedAndPosition 
						((*slD).fruitConditions (), 100.0f, 0.25f, 1 /** Meaning sequential if more than 1. */),
					(*slD).scatterChaseCycles (),
					(*slD).leaveHomeConditions (),
					(*slD).pacmanSpeed (), 
					(*slD).pacmanSpeedWhenEatingDots (), 
					(*slD).pacmanSpeedWhenFrighting (), 
					(*slD).pacmanSpeedWhenEatingFrightingDots (),
					(*slD).ghostSpeed (), 
					(*slD).ghostSpeedWhenBeingFrighten (), 
					(*slD).ghostSpeedWhenExitingHome (), 
					(*slD).ghostSpeedWhenCrossingTunnel (),
					(*slD).elroyConditions ()));
		}
	}

	// The templates are no longer needed...
	delete (slD);

	return (new PacManII::DataGame (lD, 10000));
}

// ---
PacManII::DataGame::DataGame (const PacManII::DataGame& d)
	: PACMAN::DataGame 
		([=](const PACMAN::DataGame::LevelDefinitions& ls) -> PACMAN::DataGame::LevelDefinitions 
			{ 
				PACMAN::DataGame::LevelDefinitions result; 
				for (auto i : ls)
				{
					// Uses the right copy constructor...
					if (dynamic_cast <PacManII::DataGame::LevelDefinition*> (i) != nullptr)
						result.push_back (new PacManII::DataGame::LevelDefinition 
							(*(static_cast <PacManII::DataGame::LevelDefinition*> (i))));
					else
						result.push_back (new PACMAN::DataGame::LevelDefinition (*i));
				}

				return (result);
			}(d.levels ()), 
		 d.everyToGetAnExtraLive ())
{
	// Nothing special to build up here...
}

// ---
PACMAN::DataGame::LevelDefinition::FruitConditions PacManII::DataGame::changeFruitTimeSpeedAndPosition
	(const PACMAN::DataGame::LevelDefinition::FruitConditions& fC, double t, double spd, int p)
{
	PACMAN::DataGame::LevelDefinition::FruitConditions result;

	int ct = 0;
	for (auto i : fC)
		result.push_back (
			PACMAN::DataGame::LevelDefinition::FruitCondition (
				i.bonusSymbolId (),
				i.bonusPoints (),
				i.numberBallsEatenToAppear (),
				t,
				spd,
				(p == 0) ? i.positionId () : ct++));

	return (result);
}
