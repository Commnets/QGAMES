/** \ingroup BSII */
/*@{*/

/**	
 *	@file	
 *	File: Defs.hpp \n
 *	Game: Battleship II \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 27/03/2020 \n
 *	Description: Defines and general includes of BattleshipII game
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIPII_DEFINES__
#define __BATTLESHIPII_DEFINES__

#include <BattleshipLike/bsinclude.hpp>

// A standard configuration parameter to declare the number of lives
#define __BATTLESHIPII_MAXNUMBEROFLIVESPARAMETER__			"LIVES"
#define __BATTLESHIPII_DEFAULTMAXNUMBEROFLIVESPARAMETER__	"3"
// same related with showing or not the FPS
#define __BATTLESHIPII_SHOWFPSPARAMETER__					"FPS"
#define __BATTLESHIPII_DEFAULTSHOWFPSPARAMETER__			"NO"

// The number and type of enemies now have been redefined...
#define __BATTLESHIPII_UFOBASEID__							__BATTLESHIP_ENEMYBASEID__
#define __BATTLESHIPII_MAXNUMBERUFOS__						(__BATTLESHIP_MAXNUMBERENEMIES__ - 2)
#define __BATTLESHIPII_MOTHERSHIPBASEID__					(__BATTLESHIP_ENEMYBASEID__ + __BATTLESHIP_MAXNUMBERENEMIES__ - 2)
#define __BATTLESHIPII_MAXNUMBERMOTHERSHIPS__				2
// all less 2 for UFO and 2 for MothershipUFOS

// The new types of enemy...
#define __BATTLESHIPII_ENEMTYTYPE1000__						1000
#define __BATTLESHIPII_ENEMTYTYPE1001__						1001
#define __BATTLESHIPII_ENEMTYTYPE1010__						1010
#define __BATTLESHIPII_ENEMTYTYPE1011__						1011
#define __BATTLESHIPII_ENEMTYTYPE1012__						1012

// The new types of curves defined for the BattleshipII::CurveFactory 
// used in the BattleshipII::MovementBuilder, defined at BattleshipII::Game level 
#define __BATTLESHIPII_SINUSOIDEGOANDBACK__					1000

// The different types of movements that the UFOs have in a BattleshipII game
// A sinusoide from the angle 0 to the angle 2PI and back (from the left to the right)
#define __BATTLESHIPII_SINUSOIDE1UFOSMOVEMENT__				1000
// A sinusoide from the angle PI/2 to the angle 3PI/2 and back (from the left to the right)
#define __BATTLESHIPII_SINUSOIDE2UFOSMOVEMENT__				1001
// A sinusoide from the angle 0 to the angle 2PI and back (from the right to the left)
#define __BATTLESHIPII_SINUSOIDE3UFOSMOVEMENT__				1002
// A sinusoide from the angle PI/2 to the angle 3PI/2 and back (from the right to the left)
#define __BATTLESHIPII_SINUSOIDE4UFOSMOVEMENT__				1003

// The different types of combat blocks
#define __BATTLESHIPII_STAYANDMOVESINFROMLEFTTORIGHT__		1000
#define __BATTLESHIPII_STAYANDMOVESINFROMRIGHTTOLEFT__		1001
#define __BATTLESHIPII_BIGUFODANCINGINLINES__				1002

// The new type of action block for set of ufos to control motherships better...
#define __BATTLESHIPII_ACTIONBLOCKSETMOTHERSHIPBASEID__		75000
#define __BATTLESHIPII_ACTIONBLOCKSETMOTHERSHIPNUMBER__		1000


// The number and type of action blocks now have been redefined either
#define __BATTLESHIPII_ACTIONBLOCKUFOBASEID__				__BATTLESHIP_ACTIONBLOCKUFOBASEID__
#define __BATTLESHIPII_ACTIONBLOCKUFONUMBER__				(__BATTLESHIP_ACTIONBLOCKUFONUMBER__ - 100)
#define __BATTLESHIPII_ACTIONBLOCKMOTHERSHIPUFOBASEID__		(__BATTLESHIP_ACTIONBLOCKUFOBASEID__ - 100)
#define __BATTLESHIPII_ACTIONBLOCKMOTHERSHIPUFONUMBER__		100
// all less 100 for action blocks for UFOs and the last 100 for Mothership UFO action blocks...

// The worlds of the game (stages)
#define __BATTLESHIPII_STAGE1WORLDID__						50001 // Asteroids
#define __BATTLESHIPII_STAGE2WORLDID__						50002 // UFOs
#define __BATTLESHIPII_STAGE3WORLDID__						50003
#define __BATTLESHIPII_STAGE4WORLDID__						50004
#define __BATTLESHIPII_STAGE5WORLDID__						50005

// The scenes of the game
// Scenes for the stage 1
#define __BATTLESHIPII_STAGE1SCENE1__						70000
#define __BATTLESHIPII_STAGE1SCENE2__						70001
#define __BATTLESHIPII_STAGE1SCENE3__						70002
// Scenes for the stage 2
#define __BATTLESHIPII_STAGE2SCENE1__						70010
#define __BATTLESHIPII_STAGE2SCENE2__						70011
#define __BATTLESHIPII_STAGE2SCENE3__						70012
// Scenes for the stage 3 (only one, challenge)
#define __BATTLESHIPII_STAGE3SCENE1__						70020
// Scenes for the stage 4
#define __BATTLESHIPII_STAGE4SCENE1__						70030
#define __BATTLESHIPII_STAGE4SCENE2__						70031
#define __BATTLESHIPII_STAGE4SCENE3__						70032
// Scenes for the stage 5
#define __BATTLESHIPII_STAGE5SCENE1__						70040
#define __BATTLESHIPII_STAGE5SCENE2__						70041
#define __BATTLESHIPII_STAGE5SCENE3__						70042

#endif
  
// End of the file
/*@}*/