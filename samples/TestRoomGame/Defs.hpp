/** \ingroup Example */
/*@{*/

/**	
 *	@file	
 *	File: Defs.hpp \n
 *	Framework: TestRoom \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 15/01/2021 \n
 *	Description: Defines and general includes of Test Room Game
 *	Versions: 1.0 Initial
 */

#ifndef __GAMETESTROOMDEFINES__
#define __GAMETESTROOMDEFINES__

// Color
#define __TRMTM_C1__						QGAMES::Color (0, 255, 255, 100)
#define __TRMTM_C2__						QGAMES::Color (255, 255, 255, 255)
#define __TRMTM_C3__						QGAMES::Color (255, 255, 0, 100)
#define __TRMTM_C4__						QGAMES::Color (255, 0, 0, 255)
#define __TRMTM_C5__						QGAMES::Color (100, 100, 100, 100)

// The id of the entities
#define __TRMTM_BRICKTEMPLATEID__			0
#define __TRMTM_PICTURETEMPLATEID__			1
#define __TRMTM_KNIGHBASETID__				200
#define __TRMTM_NUMBERKNIGHTS__				10
#define __TRMTM_CUBEBASEID__				1000
#define __TRMTM_NUMBERCUBES__				10

// About the tiles defining the map
#define __TRMTM_FW__						64
#define __TRMTM_FH__						64
#define __TRMTM_VW__						(__TRMTM_FW__ >> 1)			
#define __TRMTM_VL__						(__TRMTM_FH__ >> 1)
#define __TRMTM_VH__						(__TRMTM_FH__ - (__TRMTM_FW__ >> 1))
#define __TRMTM_BH__						(-__TRMTM_FH__ + __TRMTM_VH__) // It is already negative usually
#define __TRMTM_ITCXCNT__					7.5f
#define __TRMTM_ITCYCNT__					7.5f

#define __TRMTM_MAXNUMBERPLAYERS__			10
#define __TRMTM_MAXNUMBERLIVES__			3

#define __TRMTM_WORLDEXAMPLE__				100
#define __TRMTM_SCENEEXAMPLE__				100
#define __TRMTM_ROOMEXAMPLE__				100
#define __TRMTM_TYPEROOMEXAMPLE__			100
#define __TRMTM_MAPEXAMPLE__				100

#endif
  
// End of the file
/*@}*/