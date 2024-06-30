/** \ingroup Example */
/*@{*/

/**	
 *	@file	
 *	File: Defs.hpp \n
 *	Framework: Test SDL \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 15/01/2018 \n
 *	Description: Defines and genral includes of Test SDL
 *	Versions: 1.0 Initial
 */

#ifndef __GAMETESTSDLDEFINES__
#define __GAMETESTSDLDEFINES__

#include <Arcade/arcadeinclude.hpp>
#include <SDL/sdlinclude.hpp>

/** The limitis of the game. */
#define __GAMETEST_SCREENWIDTH__				640
#define __GAMETEST_SCREENHEIGHT__				480
#define __MAPWIDTHINPIXELS___					960
#define __MAPHEIGHTINPIXELS__					800

/** Some colors. */
#define __GAMETEST_YELLOWCOLOR__				QGAMES::Color (255, 255, 0, 255)
#define __GAMETEST_GRAYCOLOR__					QGAMES::Color (100, 100, 100, 255)
#define __GAMETEST_BLUECOLOR__					QGAMES::Color (0, 0, 255, 255)
#define __GAMETEST_BLACKCOLOR__					QGAMES::Color (0, 0, 0, 255)
#define __GAMETEST_GREENCOLOR__					QGAMES::Color (0, 255, 0, 255)
#define __GAMETEST_REDCOLOR__					QGAMES::Color (255, 0, 0, 255)

/** The form representing the logo. */
#define __GAMETEST_GAMEICON__					15
#define __GAMETEST_COMMNETSLOGO__				16

/** Constants related with the game. */
// The gui system
#define __GAMETEST_GUISYSTEM__					10
#define __GAMETEST_MAINWIDGETID__				1 // It is composite...
#define __GAMETEST_KEYBOARDWIDGETID__			2
#define __GAMETEST_JOYSTICKWIDGETID__			3
#define __GAMETEST_NAMEWIDGETID__				5
#define __GAMETEST_STARTWIDGETID__				6
#define __GAMETEST_SELECTIONFORMDECORATOR__		14
// Game states...
#define __GAMETEST_STATEPRESENTATION__			1
#define __GAMETEST_STATENAMEPRESENTATION__		"STATEPRESENTATION"
#define __GAMETEST_STATEPLAY__					2
#define __GAMETEST_STATENAMEPLAY__				"STATEPLAY"
#define __GAMETEST_STATEEND__					3
#define __GAMETEST_STATENAMEEND__				"STATEEND"
// Game sounds
#define __GAMETEST_BKSOUND1__					10
#define __GAMETEST_BKSOUND2__					11
#define __GAMETEST_CHANNELSOUND__				1
// Background
#define __GAMETEST_BACKGROUNDFORM1__			100
#define __GAMETEST_BACKGROUNDFORM2__			101
// Game Score Objects
#define __GAMETEST_SCOREOBJECTBASEID__			10
#define __GAMETEST_NUMBERSCOREOBJECTS__			4
#define __GAMETEST_SCOREOBJECTBASICFORM__		10
#define __GAMETEST_SCOREOBJECTASPECT__			0
// The main character
#define __GAMETEST_MAINCHARACTERID__			100
#define __GAMETEST_MAINCHRANIMATIONRUNRIGHT__	0
#define __GAMETEST_MAINCHRANIMATIONRUNLEFT__	1
#define __GAMETEST_MAINCHRANIMATIONFALLRIGHT__	2
#define __GAMETEST_MAINCHRANIMATIONFALLLEFT__	3
#define __GAMETEST_MAINCHRANIMATIONJUMPRIGHT__	4
#define __GAMETEST_MAINCHRANIMATIONJUMPLEFT__	5
#define __GAMETEST_MAINCHRANIMATIONSTAYRIGHT__	6
#define __GAMETEST_MAINCHRANIMATIONSTAYLEFT__	7
#define __GAMETEST_MAINCHRRUNMOVEMENT__			100
#define __GAMETEST_MAINCHRSTAYMOVEMENT__		101
#define __GAMETEST_MAINCHRJUMPMOVEMENT__		102
#define __GAMETEST_MAINCHRFALLMOVEMENT__		103
#define __GAMETEST_MAINCHRSTAYINGRIGHTSTATE__	0
#define __GAMETEST_MAINCHRSTAYINGLEFTSTATE__	1
#define __GAMETEST_MAINCHRRUNNINGRIGHTSTATE__	2
#define __GAMETEST_MAINCHRRUNNINGLEFTSTATE__	3
#define __GAMETEST_MAINCHRFALLINGRIGHTSTATE__	4
#define __GAMETEST_MAINCHRFALLINGLEFTSTATE__	5
#define __GAMETEST_MAINCHRJUMPINGRIGHTSTATE__	6
#define __GAMETEST_MAINCHRJUMPINGLEFTSTATE__	7
// The world
#define __GAMETEST_WORLDID__					1
#define __GAMETEST_SCENEID__					1
#define __GAMETEST_MAPID__						1
#define __GAMETEST_LAYERBASENAME__				"Base"
#define __GAMETEST_LAYERPLATFORM__				"Platform"
#define __GAMETEST_LAYERPARAMSPEED__			"Speed"
#define __GAMETEST_LAYERDEFAULTSPEED__			1
#define __GAMETEST_LAYERPARAMSTEPLENGTH__		"Length"
#define __GAMETEST_LAYERDEFAULTSTEPLENGTH__		1
#define __GAMETEST_LAYERPARAMDIRECTION__		"Direction"
#define __GAMETEST_LAYERDEFAULTDIRECTION__		"1,0,0"
// The tiles
#define __GAMETEST_BASETILE__					0
#define __GAMETEST_LIMITTILE__					1
#define __GAMETEST_BKTILE__						2
// The  buoys...
#define __GAMETEST_CHANGESTATEBUOYID__			0

/** The events. */
#define __GAMETEST_GAMEFINISHED__				1000

#endif
  
// End of the file
/*@}*/