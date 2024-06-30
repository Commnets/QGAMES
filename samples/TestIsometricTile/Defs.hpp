/** \ingroup Example */
/*@{*/

/**	
 *	@file	
 *	File: Defs.hpp \n
 *	Framework: Community Networks \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 24/10/2020 \n
 *	Description: Defines and general includes of Test Isometric Tiles games
 *	Versions: 1.0 Initial
 */

#ifndef __GAMETESTISOMETRICTILEDEFINES__
#define __GAMETESTISOMETRICTILEDEFINES__

#include <Arcade/arcadeinclude.hpp>

// Color
#define __TITM_C1__						QGAMES::Color (0, 255, 255, 100)
#define __TITM_C2__						QGAMES::Color (255, 255, 255, 255)
#define __TITM_C3__						QGAMES::Color (255, 255, 0, 100)
#define __TITM_C4__						QGAMES::Color (255, 0, 0, 255)
#define __TITM_C5__						QGAMES::Color (100, 100, 100, 100)

// About the tiles defining the map
#define __TITM_FW__						64
#define __TITM_FH__						64
#define __TITM_VW__						(__TITM_FW__ >> 1)			
#define __TITM_VL__						(__TITM_FH__ >> 1)
#define __TITM_VH__						(__TITM_FH__ - (__TITM_FW__ >> 1))
#define __TITM_BH__						(-__TITM_FH__ + __TITM_VH__) // It is already negative usually
#define __TITM_ITCXCNT__				7.5f
#define __TITM_ITYCNT__					7.5f
#define __TITM_ITXK__					9.5f
#define __TITM_ITYK__					9.5f
#define __TITM_ITXC__					7.5f
#define __TITM_ITYC__					7.5f

// About the world
#define __TITM_EXAMPLEWORLDID__			100
#define __TITM_EXAMPLESCENEID__			100
#define __TITM_EXAMPLEMAPID__			100
#define __TITM_CUBEBASEID__				0
#define __TITM_NUMBERCUBES__			10
#define __TITM_KNIGHTBASEID__			100
#define __TITM_NUMBERKNIGHTS__			10

#endif
  
// End of the file
/*@}*/