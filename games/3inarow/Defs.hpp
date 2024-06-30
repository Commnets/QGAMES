/** \addtogroup 3InARow */
/*@{*/

#ifndef __DEFS__
#define __DEFS__

// Main parameters of the screen game...
#define __GAMESNAME__					"ICF 3 In a Row Game"
#define __SCREENXPOS__					50
#define __SCREENYPOS__					50
#define __SCREENWIDTH__					800
#define __SCREENHEIGHT__				520
#define __POSXBOARD__					10
#define __POSYBOARD__					10
#define __SQUARESIZEX__					100
#define __SQUARESIZEY__					100
#define __SQUARESPACEX__				45
#define __SQUARESPACEY__				45
#define __FIRSTSQUAREX__				(58 + __POSXBOARD__)
#define __FIRSTSQUAREY__				(55 + __POSYBOARD__)
#define __POSSQUAREC__(c) \
	(__FIRSTSQUAREX__ + (c * (__SQUARESIZEX__ + __SQUARESPACEX__)))
#define __POSSQUARER__(r) \
	(__FIRSTSQUAREY__ + (r * (__SQUARESIZEY__ + __SQUARESPACEY__)))

#define __TONEROUNDINGSQUARE__			QGAMES::Color (255,255,255,255)
#define __BUOYWHENWINID__				1

// The basic resources in a board game...
#define __GAMEID__						1
#define __BOARD3INAROW__				1
#define __FORMBOARD__					0
#define __BASICBACKGROUNDFORM__			0
#define __INITIALBACGROUNDFORM__		0
#define __LITTLELOGOFORM__				0
#define __ICONTRHOPHYFORM__				0
#define __MAINBOARDID__					__BOARD3INAROW__
#define __SQUARE11__					11
#define __SQUARE12__					12
#define __SQUARE13__					13
#define __SQUARE21__					21
#define __SQUARE22__					22
#define __SQUARE23__					23
#define __SQUARE31__					31
#define __SQUARE32__					32
#define __SQUARE33__					33
#define __RESOURCE11__					11
#define __RESOURCE12__					12
#define __RESOURCE13__					13
#define __RESOURCE21__					21
#define __RESOURCE22__					22
#define __RESOURCE23__					23
#define __GROUPPLAYER1__				1
#define __GROUPPLAYER2__				2
#define __PLAYER1__						1
#define __PLAYER2__						2
#define __FORMPLAYER1__					0
#define __FORMPLAYER2__					1
#define __ENTITYANIMATING3INAROW__		1
#define __MOVEMENTENTITYANIMATING__		1
#define __SOUNDSELECTBASE__				0 // The real one played is this one plus id's player!
#define __SOUNDINTRODUCTION__			3
#define __SOUNDEND__					4

// The forms
#define	__3INAROWTABLEFORM__			1
#define __3INAROWPIECEFORM__			2
#define __3INAROWBACKGROUNDFORM__		3
#define __3INAROWBACGROUNDINITIALFORM__	4
#define __3INAROWLOGOINITIALFORM__		5
#define __APPLICATIONICONFORM__			6
#define __INTROLETTERSFORM__			7
#define __WINFORM__						8

// The states
#define __GAMESTATEINTRO__				0
#define __GAMESTATEINTRONAME__			"Intro"
#define __GAMESTATEDEMO__				1
#define __GAMESTATEDEMONAME__			"Name"
#define __GAMESTATESELECTOPTIONS__		2
#define __GAMESTATESELECTOPTIONSNAME__	"Options"
#define __GAMESTATESELECTSQUARE__		3
#define __GAMESTATESELECTSQUARENAME__	"Select"
#define __GAMESTATEPLAYING__			4
#define __GAMESTATEPLAYINGNAME__		"Playing"
#define __GAMESTATEWINNER__				5
#define __GAMESTATEWINNERNAME__			"Winning"

// The events
#define __WEHAVEAWINNER__				1000
#define __CHANGESTATUSTOWIN__			1001

#endif
  
// End of the file
/*@}*/