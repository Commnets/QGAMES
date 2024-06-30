/** \ingroup Example */
/*@{*/

/**	
 *	@file	
 *	File: Defs.hpp \n
 *	Framework: PacmanII Game \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 21/01/2022 \n
 *	Description: Defines and general includes of PacmanII
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_PACMANGAMEDEFINES__
#define __PACMANII_PACMANGAMEDEFINES__

#include <PacManLike/pcdefs.hpp>

// Number of extended levels...
#define __PACMANII_MAXNUMBERXTENDEDLEVELS__						20

// Sounds
#define __PACMANII_ENERGYAROUNDPACMAN__							60000
#define __PACMANII_SOUNDSHIELDCAUGHT__							60001

// The entities
// Wormy
#define __PACMANII_WORMYBASEENTITYID__							60000
#define __PACMANII_WORMYMAZEMOVEMENTBASEID__					600000		
#define __PACMANII_WORMYATHOMEMOVEMENTBASEID__					610000
// Shield
#define __PACMANII_SHIELDBASEENTITYID__							60010
#define __PACMANII_SHIELDSTAYINGMOVEMENTBASEID__				600010
// The types of shields
#define __PACMANII_NUMBERTYPESSHIELD__							3
#define __PACMANII_SHIELDTYPE1__								0
#define __PACMANII_SHIELDTYPE2__								1
#define __PACMANII_SHIELDTYPE3__								2

// The scene blocks and it properties
// Wormy monster
#define __PACMANII_WORMYSCENEBLOCKTRACELENGTHATTR__				"TRACLENGTH"
#define __PACMANII_WORMYSCENEBLOCKBASEID__						13000
#define __PACMANII_WORMYSCENEBLOCKNUMBER__						1000
// Element to appear...
#define __PACMANII_ELMNTAPPRSCENEBLOCKENTITYIDATTR__			"ENTITYID"
#define __PACMANII_ELMNTAPPRSCENEBLOCKPOSITIONATTR__			"POSITION"
#define __PACMANII_ELMNTAPPRSCENEBLOCKBALLSEATENTOAPPEARATTR__	"BALLESTOAPPEAR"
#define __PACMANII_ELMNTAPPRSCENEBLOCKSECONDSTOAPPEARATTR__		"SECONDSTOAPPEAR"
#define __PACMANII_ELMNTAPPRSCENEBLOCKBLOCKTOACTIVATEATTR__		"BLOCKTOACTIVATE"
// Monster to appear and then move
#define __PACMANII_MONSTERAPPRANDMOVESCENEBLOCKBASEID__			12000
#define __PACMANII_MONSTERAPPRANDMOVESCENEBLOCKNUMBER__			1000

// Events
// Related with the shields..
#define __PACMANII_SHIELDCAUGHT__								60000

// The World and Scenes used for PacManII
#define __PACMANII_EXTENDEDWORLD__								60000
// The Scenes...
#define __PACMANII_MAXNUMBERADDITIONALMONSTERS__				5
#define __PACMANII_BLOCKSCENES1__								60000 // With monsters appearing...
#define __PACMANII_BLOCKSCENES2MS1__							70000
#define __PACMANII_BLOCKSCENES2MS2__							70100
#define __PACMANII_BLOCKSCENES2MS3__							70200
#define __PACMANII_BLOCKSCENES2MS4__							70300
#define __PACMANII_BLOCKSCENES3MS1__							71000 // With monsters appearing...
#define __PACMANII_BLOCKSCENES3MS2__							71100
#define __PACMANII_BLOCKSCENES3MS3__							71200
#define __PACMANII_BLOCKSCENES3MS4__							71300
// The Maps...
// The Ms. Pacman maps
#define __PACMANII_MISSPACMANTYPEMAPS__							4
// Maps with ms. Pacman 1 maze
#define __PACMANII_MISSPACMAN1BLUEMAP__							60000
#define __PACMANII_MISSPACMAN1REDMAP__							60200
#define __PACMANII_MISSPACMAN1GREENMAP__						60400
#define __PACMANII_MISSPACMAN1YELLOWMAP__						60600
// Maps with ms. Pacman 2 maze
#define __PACMANII_MISSPACMAN2BLUEMAP__							61000
#define __PACMANII_MISSPACMAN2REDMAP__							61200
#define __PACMANII_MISSPACMAN2GREENMAP__						61400
#define __PACMANII_MISSPACMAN2YELLOWMAP__						61600
// Maps with ms. Pacman 3 maze
#define __PACMANII_MISSPACMAN3BLUEMAP__							62000
#define __PACMANII_MISSPACMAN3REDMAP__							62200
#define __PACMANII_MISSPACMAN3GREENMAP__						62400
#define __PACMANII_MISSPACMAN3YELLOWMAP__						62600
// Maps with ms. Pacman 4 maze
#define __PACMANII_MISSPACMAN4BLUEMAP__							63000
#define __PACMANII_MISSPACMAN4REDMAP__							63200
#define __PACMANII_MISSPACMAN4GREENMAP__						63400
#define __PACMANII_MISSPACMAN4YELLOWMAP__						63600

#endif
  
// End of the file
/*@}*/