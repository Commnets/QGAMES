/** \addtogroup SabreWulf */
/*@{*/

#ifndef __DEFS__
#define __DEFS__

// Main parameters of the screen game...
#define __GAMESNAME__					"ICF SabreWulf Game"
#define __SCREENXPOS__					350
#define __SCREENYPOS__					150
#define __ORIGINALSPECTRUMWIDTH__		256
#define __ORIGINALSPECTRUMHEIGHT__		192
#define __ORIGINALMAPINITX__			0
#define __ORIGINALMAPINITY__			16
#define __ORIGINALMAPLENGTH__			__ORIGINALSPECTRUMWIDTH__
#define __ORIGINALMAPHEIGHT__			(__ORIGINALSPECTRUMHEIGHT__ - __ORIGINALMAPINITY__)
#define __SCREENWIDTH__					640 // (256 * 2.5)
#define __SCREENHEIGHT__				480 // (192 * 2.5)
#define __MAPORIGINX__					64
#define __MAPORIGINY__					48
#define __MINVISIBLEPOSX__				(__MAPORIGINX__)
#define __MAXVISIBLEPOSX__				(__MAPORIGINX__ + (__ORIGINALMAPLENGTH__ << 1))
#define __MINVISIBLEPOSY__				(__MAPORIGINY__ + (__ORIGINALMAPINITY__ << 1))
#define __MAXVISIBLEPOSY__				(__MAPORIGINY__ + ((__ORIGINALMAPHEIGHT__ + __ORIGINALMAPINITY__) << 1))
#define __GRAYCOLOR__					QGAMES::Color (25,25,25,255) // Very soft...
#define __GREENCOLOR__					QGAMES::Color (0,102,0,255) // Dark...
#define __BLUECOLOR__					QGAMES::Color (0,0,102,255) // Dark...
#define __REDCOLOR__					QGAMES::Color (255,0,0,255) // Light...
#define __GRAYCOLORTRANSPARENT__		QGAMES::Color (25,25,25,10) // Really soft & transparent...

// Define the main parameters of the original game...
#define __NUMBEROFMAZEPLACESX__			16
#define __NUMBEROFMAZEPLACESY__			16
#define __NUMBEROFMAZEPLACES__			(__NUMBEROFMAZEPLACESX__ * __NUMBEROFMAZEPLACESY__)
#define __NUMBEROFMAZEPLACETYPES__		48
#define __NUMBEROFRANDPOSITIONSPEROBJ__	4
#define __NUMBEROFAMULTERRANDPOS__		8
#define __NUMBEROFAMULETPIECES__		4
#define __NUMBERPOSSIBILITIESORCHILD__	16
#define __NUMBERPOSSIBILITIESOBJECT__	16
#define __NUMBERPOSIBILITIESNATIE__		21
#define __NUMBERPOSIBILITIESINMORTALS__	9 // Exclugin sabreman, the guardian and the fire...
#define __NUMBERNASTIESPERPLACE__		3
#define __NUMBEROBJECTSPERMAZEPLACE__	2
#define __NUMBERPOSSIBLITIESFOROBJS__	4
#define __NUMBEROFAMULETPIECES__		4
#define __BASEORCHILDCOLOR__			0x42
#define __BASEOBJECTCOLOR__				0x42
#define __BASENATIECOLOR__				0x64 // It really describes the base per sprite!
#define __XSCALE__						2
#define __YSCALE__						2
#define __FIRSTMAZEPLACE__				0xA8 // 0xE2 Spearman // 0x44 Spearman // 0xA7 Spearman // 0xA8 original
#define __LASTMAZEPLACE__				0x88 // (136)
#define __INITIALNUMBEROFLIVES__		5
#define __MAXNUMBEROFLIVES__			9
#define __SPEEDLOSSINGLIVE				1 // In seconds...

// The definitions of the sprites amaking up the maze
#define __SPRITE70BC__					0x70BC
#define __SPRITE71B3__					0x71B3
#define __SPRITE7298__					0x7298
#define __SPRITE72F6__					0x72F6
#define __SPRITE7462__					0x7462
#define __SPRITE7523__					0x7523
#define __SPRITE771F__					0x771F
#define __SPRITE785E__					0x785E
#define __SPRITE78F2__					0x78F2
#define __SPRITE7947__					0x7947
#define __SPRITE7981__					0x7981
#define __SPRITE7B11__					0x7B11
#define __SPRITE7BB7__					0x7BB7
#define __SPRITE7C0C__					0x7C0C
#define __SPRITE7CCD__					0x7CCD
#define __SPRITE7E4B__					0x7E4B
#define __SPRITE8047__					0x8047
#define __SPRITE81C5__					0x71C5
#define __SPRITE8382__					0x8382
#define __SPRITE83AA__					0x83AA
#define __SPRITE83D2__					0x83D2
#define __SPRITE8427__					0x8427
#define __SPRITE847C__					0x847C
#define __SPRITE8558__					0x8558
#define __SPRITE85C8__					0x85C8
#define __SPRITE86DA__					0x86DA
#define __SPRITE8702__					0x8702
#define __SPRITE872A__					0x872A
#define __SPRITE8806__					0x8806
#define __SPRITE89C3__					0x89C3
#define __SPRITE8B80__					0x8B80
#define __SPRITE8C5C__					0x8C5C
#define __SPRITE8CCC__					0x8CCC
#define __SPRITE8D3C__					0x8D3C
#define __SPRITE8E18__					0x8E18
#define __SPRITE8F2A__					0x8F2A
#define __SPRITE90A8__					0x90A8
#define __SPRITE93C4__					0x93C4
#define __SPRITE955D__					0x95DD
#define __SPRITE95CD__					0x95CD
#define __SPRITE9673__					0x9673
// The base of the sprite defining the map...
#define __SPRITEMAPBASE__				10000
// The sprites to form the border in the presentation screen...
#define __SPRITETOPBORDER1__			20011
#define __SPRITETOPBORDER2__			20012
#define __SPRITEBOTTOMBORDER1__			20013
#define __SPRITEBOTTOMBORDER2__			20014
#define __SPRITESIDEBORDER__			20010

// The icon representing the game
#define __SABREWULFICO__				0
// The sabrewulf background
#define __SABREWULFBACKGROUND__			1
// The ground background 
#define __SABREWULFGROUND0__			20000
// Other...
#define __SABREWULFGROUND1__			20001
// The sabrewulf logo
#define __SABREWULFLOGO__				2
// The community networks logo
#define __SABREWULFCOMMTY__				3
// The letters used to present thins
#define __SABREWULFLETTERS__			101
// The little letters used
#define __SABREWULFLITTLELETTERS__		102
// Forms used in the selection
// A keyboard
#define __SABREWULFKEYBOARDFORM__		4
// A joystick
#define __SABREWULFJOYSTICKFORM__		5
// The level of difficulty
#define __SABREWULFLEVELFORM__			8
// To start the game
#define __SABREWULFGOFORM__				6
// The bar of energy
#define __SABREWULFENERGYFORM__			9
// The arrows
#define __SABREWULFARROWSFORM__			10
#define __SABREWULFARROWLEFT__			1
#define __SABREWULFARROWRIGHT__			0
#define __SABREWULFARROWUP__			3
#define __SABREWULFARROWDOWN__			2
// The form when the game is paused...
#define __SABREWULFPAUSEGAME__			7

// The forms of the entities playing...
// The sabreman
#define __SABREMANNORMALFORM__			600
#define __SABREMANSPEEDFORM__			601
#define __SABREMANSTOPPEDRIGHTFORM__	602
#define __SABREMANSTOPPEDLEFTFORM__		603
#define __SABREMANINMORTALFORM__		604
#define __SABREMANGOINGBACKFORM__		605
#define __SABREMANFIGHTINGFORM__		606
#define __SABREMANFLYRIGHTFORM__		607
#define __SABREMANFLYLEFTFORM__			608
#define __SABREMANSITRIGHTFORM__		609
#define __SABREMANSITLEFTFORM__			610
#define __SABREMANDIERIGHTFORM__		611
#define __SABREMANDIELEFTFORM__			612
// When sabreman is neither fighting nor sitting...
#define __SABREMANNINITLEFTASPECT__		0		
#define __SABREMANNENDLEFTASPECT__		3
#define __SABREMANNINITRIGHTASPECT__	4	
#define __SABREMANNENDRIGHTASPECT__		7
#define __SABREMANNINITUPASPECT__		8
#define __SABREMANNENDUPASPECT__		11
#define __SABREMANNINITDOWNASPECT__		12
#define __SABREMANNENDDOWNASPECT__		15
#define __SABREMANFINITLEFTASPECT__		0
#define __SABREMANFENDLEFTASPECT__		3
#define __SABREMANFINITRIGHTASPECT__	4	
#define __SABREMANFENDRIGHTASPECT__		7
#define __SABREMANFINITUPASPECT__		8
#define __SABREMANFENDUPASPECT__		11
#define __SABREMANFINITDOWNASPECT__		12
#define __SABREMANFENDDOWNASPECT__		15
// When sabreman is sat down...
#define __SABREMANSITASPECT__			0
// When sabreman is fighting
#define __SABREMANFIGHTINITLEFTASPECT__	0
#define __SABREMANFIGHTENDLEFTASPECT__	7
#define __SABREMANFIGHTINITRIGHTASPECT__	8
#define __SABREMANFIGHTENDRIGHTASPECT__		15

// The forms of the orchild
#define __SABREMANBASEORCHILDFLOWERS__	700
#define __SABREMANBASEORCHILD__			750
#define __SABREMANNUMBERFLOWERASPECTS__	7

// The forms of the objects than can appear in
// a place of the maze...
#define __SABREWULFOBJECTS__			800
#define __SABREMANLITTLEAMULET__		801
#define __SABREWULFAMULET__				802
#define __SABREWULFAEMPTYAMULET__		803

// The forms of the different nasties
#define __SABREWULFBASENASTIEFORM__		900
#define __SABREWULFNUMNASTIEASPECTS__	21
#define __SABREWULFNASTIELEFTINIT__		0
#define __SABREWULFNASTIELEFTEND__		1
#define __SABREWULFNASTIERIGHTINIT__	2
#define __SABREWULFNASTIERIGHTEND__		3
#define __SABREWULFBASENASTIEBORNFORM__	950
#define __SABREWULFNASTIEBORNASPECTS__	6
#define __SABREWULFBASENASTIEDIEFORM__	980
#define __SABREWULFNASTIEDIEASPECTS__	6
#define __SABREWULFFIREFORM__			922
#define __SABREWULFFIRELEFTINIT__		0
#define __SABREWULFFIRELEFTEND__		8
#define __SABREWULFFIRERIGHTINIT__		0
#define __SABREWULFFIRERIGHTEND__		8
#define __SABREWULFFIREBORNFORM__		972
#define __SABREWULFFIREBORNASPECTS__	4
#define __SABREWULFFIREDIEFORM__		__SABREWULFFIREBORNFORM__
#define __SABREWULFFIREDIEASPECTS__		__SABREWULFFIREBORNASPECTS__

// The guardian
#define __SABREWULFGUARDIANFORM__		940
#define __SABREWULFGUARDIANASPECTS__	2
// The rhino
#define __SABREWULFRHINOFORM__			941
#define __SABREWULFRHINOLEFTINIT__		0
#define __SABREWULFRHINOLEFTEND__		1
#define __SABREWULFRHINORIGHTINIT__		2
#define __SABREWULFRHINORIGHTEND__		3
// The hippo
#define __SABREWULFHIPPOFORM__			942
#define __SABREWULFHIPPOSLEEPFORM__		943
#define __SABREWULFHIPPOLEFTINIT__		0
#define __SABREWULFHIPPOLEFTEND__		1
#define __SABREWULFHIPPORIGHTINIT__		2
#define __SABREWULFHIPPORIGHTEND__		3
// Wolf
#define __SABREWULFWOLFFORM__			944
#define __SABREWULFWOLFLEFTINIT__		0
#define __SABREWULFWOLFLEFTEND__		3
#define __SABREWULFWOLFRIGHTINIT__		4
#define __SABREWULFWOLFRIGHTEND__		7
// Spearman
#define __SABREWULFSPEARMANBLUEFORM__	945
#define __SABREWULFSPEARMANPINKFORM__	946
#define __SABREWULFSPEARMANREDFORM__	947
#define __SABREWULFSPEARMANLEFTINIT__	0
#define __SABREWULFSPEARMANLEFTEND__	3
#define __SABREWULFSPEARMANRIGHTINIT__	4
#define __SABREWULFSPEARMANRIGHTEND__	7

// The form of the next game underwurlde...
#define __SABREWULFUNDERWURLDE__		1010

// The sounds
// The sound to start the game...
#define __SABREWULFSTARTMUSIC__			126
// Musics during the selection and demo
#define __SABREWULFTITLEMUSIC__			127
#define __SABREWULFMAXNUMTITLEMUSICS__	5
// The music before the game starts, just when the start button has been selected...
#define __SABREMANSTARTGAMEMUSIC__		107
// Sound when sabreman appears
#define __SABREMANAPPEARSSOUND__		108
// Sound while sabreman walks
#define __SABREMANWALKINGSOUND__		123
// When Sabreman dies
#define __SABREMANDIEINGSOUND__			107
// Sabremand finds a piece of the amulet
#define __SABREMANFINDSAMULET__			100
// The song of the end when sabreman wins
#define __SABREWULFWINMUSIC__			116
// The song of the end when sabreman losses
#define __SABREWULFFAILMUSIC__			116
// When sabreman get something
#define __SABREMANGETSOBJECT__			114
// When sabreman hits a flower
#define __SABREMANFLOWER__				109
// When the effect of the flower ends
#define __SABREMANENDFLOWER__			108
// When the nasties appears
#define __SABREWULFNASTIEAPPEARS__		101
// When sabreman kills a nastie
#define __SABREWULFNASTIEDIESOUND__		118
// Sound when sabreman dies whilst flying to the vegetation...
#define __SABREWULFFLYDISOUND__			105
// Hitting the animals...
#define __SABREWULFHITANIMALSOUND__		122
// The sound channels
#define __SABREWULF_BACKGROUNDCHANNEL__ 1
#define __SABREWULF_PLAYERCHANNEL__		2
#define __SABREWULF_NASTIECHANNELBASE__	3
#define __SABREWULF_INMORTALCHANNEL__	4

// Used in the states...
// During the selection
#define __SABREWULFNUMBEROPTIONS__		4
// The max number of levels
#define __SABREWULFNUMBEROFLEVELS__		3

// The worlds of the game:
// Basic world
#define __SABREWULFWORLD__				1
// The scenes of the game
// Basic scene
#define __SABREWULFSCENE__				11
// The maps of the scenes
// The basic map
#define __SABREWULFMAP__				111

// The states
// The introduction
#define __SABREWULFINTROSTATE__			1
#define __SABREWULFINTROSTATENAME__		"Intro"
// The selection
#define __SABREWULFSELECTSTATE__		2
#define __SABREWULFSELECTSTATENAME__	"Selection"
// The demo part
#define __SABREWULFDEMOSTATE__			3
#define __SABREWULFDEMOSTATENAME__		"Demo"
// When sabreman starts to move
#define __SABREWULFSTARTSTATE__			4
#define __SABREWULFSTARTSTATENAME__		"Starts"
// Finally playing the game
#define __SABREWULFPLAYINGSTATE__		5
#define __SABREWULFPLAYINGSTATENAME__	"Playing"
// When the players dies
#define __SABREWULFDIESTATE__			6
#define __SABREWULFDIESTATENAME__		"Die"
// End of the game
#define __SABREWULFENDSTATE__			7
#define __SABREWULFENDSTATENAME__		"End"
// When the player wins
#define __SABREWULFWINSTATE__			8
#define __SABREWULFWINSTATENAME__		"Win"
// When sabreman get a piece of the amulet
#define __SABREWULFAMULETSTATE__		9
#define __SABREWULFAMULETSTATENAME__	"Amulet"
// When sabreman dies and fly accross the vegetation
#define __SABREWULFFLYDIESTATE__		10
#define __SABREWULFFLYDIESTATENAME__	"FlyDie"
// When sabreman dies for ever
#define __SABREWULFDIEFOREVERSTATE__	11
#define __SABREWULFDIEFOREVERSTATENAME__	"ForEver"

// The entities making up the score of the game...
// The frame rounding the rest
#define __SABREWULFFRAMESCOREOBJ__		1000
// The entity representing the bar of energy
#define __SABREWULFENERGYSCOREOBJ__		1001
// The entity representing the lives
#define __SABREWULFLIVESSCOREOBJ__		1002
// The entity representing the time
#define __SABREWULFTIMESCOREOBJ__		1003
// The entity representing the points
#define __SABREWULFPOINTSSCOREOBJ__		1004

// The entities are of the game:
// The sabreman
#define __SABREWULFSABREMAN__			500
// The orchild
#define __SABREWULFORCHILD__			100
// The objects managed in the system
// In the same entity there can be many different forms
// There can be many instances of this entity at the same time
// in the sam maze place, so this is really the 
// base for the object identification.
#define __SABREWULFOBJECTSARTIST__		200
#define __SABREWULFOBJECTSPERPLACE__	3 // Maximum...
// Controlling the nasties: The base reference, and the number of nasties maximum simultaneously!
#define __SABREWULFNASTIESBASE__		300
#define __SABREWULFMAXNUMBERNASTIES__	(__NUMBERNASTIESPERPLACE__ * 2)
// Controlling th einmortals
#define __SABREWULFINMORTALSBASE__		(__SABREWULFNASTIESBASE__ + __SABREWULFMAXNUMBERNASTIES__ + 1)
#define __SABREWULFNUMBERINMORTALS__	9
// The guardian watching the exit door
#define __SABREWULFGUARDIAN__			350
// The fire...that appears when sabreman stays a while in the same place...
#define __SABREWULFFIRE__				(__SABREWULFNASTIESBASE__ + __SABREWULFMAXNUMBERNASTIES__)

// The movements of the game:
// Sabreman's movement
#define __SABREWULFSABREMANMOVS__		100
// Nasties's movement
#define __SABREWULFMONSTERBASEMOV__		200
// Fire's movement
#define __SABREWULFFIREMOVS__			(__SABREWULFMONSTERBASEMOV__ + __SABREWULFMAXNUMBERNASTIES__)
// Inmortals's movement
#define __SABREWULFINMORTALMOV__		300

// The events...
// When sabreman is out of limits
#define __SABREMANOUTOFLIMITS__			1000
// Sabreman is in the exit zone
#define __SABREMANOUTEXITZONE__			1001
// The inmortal is out of the limits
#define __INMORTALOUTOFLIMITS__			1002

// The buoys
// The buoy id to change the maze place
#define __SABREWULFTOCHANGEROOMBUOYID__	1
// The buoy id to exit the maze
#define __SABREWULFTOEXITMAZEBUOYID__	2
// The buoy id to change the maze place where the inmortal is in
#define __INMORTALTOCHANGEROOMBUOYID__	3

#endif
  
// End of the file
/*@}*/