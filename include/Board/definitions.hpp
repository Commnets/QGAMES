/** \ingroup BoardGame */
/*@{*/

/**	
 *	@file	
 *	File: definitions.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2015 \n
 *	Description: Common definitions for a board game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_BOARDDEFINITIONS__
#define __QGAMES_BOARDDEFINITIONS__

// Types of boards
#define __QGAMES__BOARDSIMPLETYPE__					1

#define __GAME_PROPERTYBOARDSFILE__					"BOARDSFILE"
#define __GAME_BOARDSDEFINITIONSTDFILE__			"boards.xml" // ...in the same directory than the executed file...

// Tags where the board is defined...
#define __QGAMES_BOARDSATTRID__						"id"
#define __QGAMES_BOARDSATTRTYPE__					"type"
#define __QGAMES_BOARDSATTRDEFFILE__				"file"
// Tags to define ans standard board...
#define __QGAMES_BOARDRESOURCESTAG__				"Resources"
#define __QGAMES_BOARDRESOURCETAG__					"Resource"
#define __QGAMES_BOARDRESOURCEATTRID__				"id"
#define __QGAMES_BOARDRESOURCEATTRGROUP__			"group"
#define __QGAMES_BOARDDATASTAG__					"Datas"
#define __QGAMES_BOARDDATATAG__						"Data"
#define __QGAMES_BOARDDATAATTRID__					"id"
#define __QGAMES_BOARDDATAATTRVALUE__				"value"
#define __QGAMES_BOARDSQUARESTAG__					"Squares"
#define __QGAMES_BOARDSQUARETAG__					"Square"
#define __QGAMES_BOARDSQUAREATTRID__				"id"
#define __QGAMES_BOARDSQUARECONNECTIONSTAG__		"SquareConnections"
#define __QGAMES_BOARDSQUARECONNECTIONTAG__			"SquareConnection"
#define __QGAMES_BOARDSQUARECONNECTIONATTRFROM__	"from"
#define __QGAMES_BOARDSQUARECONNECTIONATTRIN__		"in"
#define __QGAMES_BOARDSQUARECONNECTIONATTRTO__		"to"

// Types of board games
#define __QGAMES__BOARDGAMESIMPLETYPE__				1

// Tags where the games are defined....
#define __QGAMES_BOARDGAMESATTRID__					"id"
#define __QGAMES_BOARDGAMESATTRTYPE__				"type"
#define __QGAMES_BOARDGAMESATTRDEFFILE__			"file"
// Tags to define ans standard board...
#define __QGAMES_BOARDBOARDSTAG__					"Boards"
#define __QGAMES_BOARDBOARDATTRID__					"id"
#define __QGAMES_BOARDRULESTAG__					"Rules"
#define __QGAMES_BOARDRULETYPEID__					"type"
#define __QGAMES_BOARDPLAYERSTAG__					"Players"
#define __QGAMES_BOARDPLAYERATTRID__				"id"
#define __QGAMES_BOARDPLAYERATTRTYPE__				"type"

// Name of the properties that can define the elements...
// In the square...
#define __QGAMES_SQUAREROWATTR__					"row"
#define __QGAMES_SQUAREROWDEFAULTVALUE__			-1
#define __QGAMES_SQUARECOLUMNATTR__					"colum"
#define __QGAMES_SQUARECOLUMNDEFAULTVALUE__			-1
#define __QGAMES_SQUAREDEPTHATTR__					"depth"
#define __QGAMES_SQUAREDEPTHDEFAULTVALUE__			-1
#define __QGAMES_SQUAREPOSITIONXATTR__				"posX"
#define __QGAMES_SQUAREPOSITIONXDEFAULTVALUE__		0
#define __QGAMES_SQUAREPOSITIONYATTR__				"posY"
#define __QGAMES_SQUAREPOSITIONYDEFAULTVALUE__		0
#define __QGAMES_SQUAREPOSITIONZATTR__				"posZ"
#define __QGAMES_SQUAREPOSITIONZDEFAULTVALUE__		0
#define __QGAMES_SQUARESIZEXATTR__					"width"
#define __QGAMES_SQUARESIZEXDEFAULTVALUE__			0
#define __QGAMES_SQUARESIZEYATTR__					"height"
#define __QGAMES_SQUARESIZEYDEFAULTVALUE__			0
#define __QGAMES_SQUARESIZEZATTR__					"deep"
#define __QGAMES_SQUARESIZEZDEFAULTVALUE__			0
// In the board
#define __QGAMES_BOARDPOSITIONXATTR__				"posX"
#define __QGAMES_BOARDPOSITIONXDEFAULTVALUE__		0
#define __QGAMES_BOARDPOSITIONYATTR__				"posY"
#define __QGAMES_BOARDPOSITIONYDEFAULTVALUE__		0
#define __QGAMES_BOARDPOSITIONZATTR__				"posZ"
#define __QGAMES_BOARDPOSITIONZDEFAULTVALUE__		0

#define __QGAMES_STDTYPEBOARDSLOADER__				"Boards"

// Types of board elements
#define __BOARDTYPE__								10
#define __BOARDGAMETYPE__							20
#define __BOARDSQUARETYPE__							30
#define __BOARDPLAYERTYPE__							40

// The most commons events in a game...
#define __QGAMES_BOARDNEXTPLAYER__					9000
#define __QGAMES_BOARDPLAYEROUT__					9001

// Runtime values structure
#define __QGAMES_RUNTIMEVALUESSQUARETYPE__			"SQUARE"
#define __QGAMES_RUNTIMEVALUESRESOURCESTYPE__		"RESOURCES"
#define __QGAMES_RUNTIMEVALUESBOARDSTYPE__			"BOARDS"
#define __QGAMES_RUNTIMEVALUESBOARDSTATETYPE__		"BOARDSTATE"
#define __QGAMES_RUNTIMEVALUESRULESTYPE__			"RULES"
#define __QGAMES_RUNTIMEVALUESRULETYPE__			"RULE"
#define __QGAMES_RUNTIMEVALUESPLAYERSTYPE__			"PLAYERS"
#define __QGAMES_RUNTIMEVALUESPLAYERSTATETYPE__		"PLAYERSTATE"

#endif
  
// End of the file
/*@}*/
