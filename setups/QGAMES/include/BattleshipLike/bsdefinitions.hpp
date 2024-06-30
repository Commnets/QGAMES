/** \ingroup Battleship */
/*@{*/

/**	
 *	@file	
 *	File: bsdefinitions.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/11/2019 \n
 *	Description: Defines Battleship like library definitions. \n
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_DEFINITIONS__
#define __BATTLESHIP_DEFINITIONS__

#include <BattleshipLike/bsdatagame.hpp>
	
#define __BATTLESHIPDATAGAME__									BATTLESHIP::DataGame::_theDataGame

// The basic parameteres of the game
#define __BATTLESHIP_GAMEINITWORLDPARAMETER__					"INITIALWORLD"
#define __BATTLESHIP_GAMEINITWORLDDEFPARAMETER__				"50000"
#define __BATTLESHIP_GAMEINITSCENEPARAMETER__					"INITIALSCENE"
#define __BATTLESHIP_GAMEINITSCENEDEFPARAMETER__				"60000"
#define __BATTLESHIP_GAMEPASSWORDPARAMETER__					"PASSWORD"
#define __BATTLESHIP_GAMEPASSWORDDEFPARAMETER__					"NONE"
#define __BATTLESHIP_GAMEINITIALPOWERPARAMETER__				"POWER"
#define __BATTLESHIP_GAMEINITIALPOWERDEFPARAMETER__				"0"
#define __BATTLESHIP_GAMEINITIALSHOOTINGSLEFTPARAMETER__		"SHOOTINGSLEFT"
#define __BATTLESHIP_GAMEINITIALSHOOTINGSLEFTDEFPARAMETER__		"200"

/** The additional keys used in this type of games */
#define __BATTLESHIP_KEYITERATE__								7
#define __BATTLESHIP_KEYCATCHLEAVE__							8
#define __BATTLESHIP_KEYJUMP__									9

// The parameters used to identify elements in the battleship's elements transported
#define __BATTLESHIP_ELMTSTRANSATTRIBUTEID__					0
#define __BATTLESHIP_ELMTSTTRANSATTRIBUTETYPE__					1

// Game states types
/** Name of the player playing */
#define __BATTLESHIP_GAMESTATEPLAYERNAMETYPE__					50000
/** Special version of the credit titles to consider different fonts and the logo in one of them */
#define __BATTLESHIP_GAMESTATECREDITTITLESTYPE__				50090
#define __BATTLESHIP_GAMESTATECREDITTILEATTRNUM__				"NUMBERTITLE"
#define __BATTLESHIP_GAMESTATECRTTITLEATTRNUMDEF__				2
#define __BATTLESHIP_GAMESTATECREDITTITLEATTRCNT__				"CONTENTTITLE"
#define __BATTLESHIP_GAMESTATECRTTITLEATTRCNTDEF__				"Battleship"
/** Name of the current world */
#define __BATTLESHIP_GAMESTATEWORLDNAMETYPE__					50010
/** Name of the current scene */
#define __BATTLESHIP_GAMESTATESCENENAMETYPE__					50011
/** To create the BATTLESHIP menu */
#define __BATTLESHIP_GAMESTATEMENUTYPE__						50100
/** To create the Playing Game State Control */
#define __BATTLESHIP_GAMESTATECONTROLPLAYINGTYPE__				50080
/** To create the Playing Game State (inside Playing Game State Control) */
#define __BATTLESHIP_GAMESTATEPLAYINGTYPE__						50081
/** To create the Options when Playing Game State */
#define __BATTLESHIP_GAESTATEOPTIONSWHENPLAYTYPE__				50082
/** Special version of the hall of the fame */
#define __BATTLESHIP_GAMESTATEHALLOFFAMETYPE__					50070
/** Special version to control all the game */
#define __BATTLESHIP_GAMESTATECTRLTYPE__						50101
/** To initialize the game, and to avoid doing so at init method in game class. */
#define __BATTLESHIP_GAMESTATEINITIALIZEGAMETYPE__				50102
/** To show the battleship moving in the right side of the screen. */
#define __BATTLESHIP_GAMESTATEMOVINGBATTLESHIPTYPE__			50103

// Game states
#define __BATTLESHIP_STANDARDCONTROLGAMESTATE__					50000 
// Another one can be defined, but this is the standard one!
#define __BATTLESHIP_PAUSEGAMESTATE__							50001
#define __BATTLESHIP_COMMTYLOGOGAMESTATE__						50002
#define __BATTLESHIP_LOADINGGAMESTATE__							50003
#define __BATTLESHIP_INTRODUCTIONGAMESTATE__					50004
#define __BATTLESHIP_MENUGAMESTATE__							50009
#define __BATTLESHIP_PLAYERNAMEGAMESTATE__						50014
#define __BATTLESHIP_PLAYERDIEDGAMESTATE__						50019
#define __BATTLESHIP_GAMEFINISHESGAMESTATE__					50022
#define __BATTLESHIP_HALLOFFAMEGAMESTATE__						50025
#define __BATTLESHIP_EXITGAMESTATE__							50028
#define __BATTLESHIP_CONTROLPLAYINGGAMESTATE__					50100
#define __BATTLESHIP_BEFOREPLAYINGGAMESTATE__					50101
#define __BATTLESHIP_PLAYINGGAMESTATE__							50107
#define __BATTLESHIP_OPTIONSWHENPLAYINGGAMESTATE__				50200
#define __BATTLESHIP_PLAYERGAMEOVERGAMESTATE__					50022

// GUI Systems
/** To ask for the name of the player */
#define __BATTLESHIP_PLAYERNAMEGUISYSTEM__						50000
#define __BATTLESHIP_PLAYERNAMEOPTIONID__						50002
/** Additional option to select the instructions to be shown in the standard main menu (@see adguisystem.xml) */
#define __BATTLESHIP_GUISYSTEMINSTRUCTIONSWDT__					50010
/** Additional option to select the type of artist playing in the standard main manu (@see adguisystem.xml) */
#define __BATTLESHIP_GUISYSTEMTYPEARTISTWDT__					50011
/** Additional options to select the keys used to catch and leave things in the standard keys menu (@see adguisystem.xml) */
#define __BATTLESHIP_GUISYSTEMCFGKEYITERATEWDT__				50110
#define __BATTLESHIP_GUISYSTEMCFGKEYCATCHLEAVEWDT__				50111
#define __BATTLESHIP_GUISYSTEMCFGKEYJUMPWDT__					50112
/** Particualr GUI System used for the options when playing */
#define __BATTLESHIP_OPTIONWHENPLAYINGGUISYSTEM__				50200
#define __BATTLESHIP_OPTIONSGUISYSTEMMAINWIDGET__				50201
#define __BATTLESHIP_GUISYSTEMSAVEGAMEWDT__						50202
#define __BATTLESHIP_GUISYSTEMEXITGAMEWDT__						50203
#define __BATTLESHIP_GUISYSTEMCONTINUEGAMEWDT__					50204

// Other...
/** The maximum wide of any thing being carried in the screen in pixels... */
#define __BATTLESHIP_BATTLESHIPTHINGTOCARRYPIXELSWIDE__			20

// ---Confgurable things-----------------------------------------------------------------------------
// Things that can be configurated (@see DataGame class)
/** General game attributes */
#define __BATTLESHIP_NUMBEROFLIVES__							__BATTLESHIPDATAGAME__ -> _numberOfLives
#define __BATTLESHIP_NUMBERTHINGSBEINGCARRIED__					__BATTLESHIPDATAGAME__ -> _numberOfThingsBeingCarried
#define __BATTLESHIP_ENERGYELEMENTSNUMBERTYPES__				__BATTLESHIPDATAGAME__ -> _numberOfEnergyElementsTypes
#define __BATTLESHIP_NUMBEROSHOOTINGPILLTYPES__					__BATTLESHIPDATAGAME__ -> _numberOfShootingPillTypes
#define __BATTLESHIP_NUMBEROFTHINGTOCATCHTYPES__				__BATTLESHIPDATAGAME__ -> _numberOfThingsToCatchTypes
#define __BATTLESHIP_NUMBERSHOOTINGTYPES__						__BATTLESHIPDATAGAME__ -> _numberOfShootingTypes
#define __BATTLESHIP_NUMBEROFPOWERLEVELS__						__BATTLESHIPDATAGAME__ -> _numberOfPowerLevels
#define __BATTLESHIP_NUMBEROFASTEROIDTYPES__					__BATTLESHIPDATAGAME__ -> _numberOfAsteroidTypes
#define __BATTLESHIP_NUMBEROFUFOTYPES__							__BATTLESHIPDATAGAME__ -> _numberOfUFOTypes

// Important configurable forms used in the game
/** The form to represent the lives in the game (2 aspects, one for male player, and another one for the female one) */
#define __BATTLESHIP_BATTLESHIPLIVESFORM__						__BATTLESHIPDATAGAME__ -> _livesFormId
/** The form representing the things to carry in a score object. */
#define __BATTLESHIP_BATTLESHIPTHINGTOCARRYFORM__				__BATTLESHIPDATAGAME__ -> _thingsToCarryFormId
/** Max number of aspects in the previous form. */
#define __BATTLESHIP_BATTLESHIPTHINGSTOCARRYNUMBERTYPES__		__BATTLESHIPDATAGAME__ -> _numberOfThingsBeingCarriedTypes

// Ids for the entities used as score objects
// There could be many (the energy of the bad guy that the main artist is fighting against e.g)
#define __BATTLESHIP_SPACEELMNTENERGYSCOBJID__					__BATTLESHIPDATAGAME__ -> _spaceElementEnergyScoreObjectId 
#define __BATTLESHIP_BATTLESHIPLIVESSCOBJID__					__BATTLESHIPDATAGAME__ -> _battleshipLivesScoreObjectId
// Up to __BATTLESHIP_MAXNUMBERTHINGSBEINGCARRIED__
#define __BATTLESHIP_BATTLESHIPTHINGBEINGCARRIEDID__			__BATTLESHIPDATAGAME__ -> _battleshipThingBeingCarriedScoreObjectBaseId 
#define __BATTLESHIP_BATTLESHIPTHINGSBEGINGCARRIEDID__			__BATTLESHIPDATAGAME__ -> _battleshipThingsBeingCarriedScoreObjectId
// There could be many (the number of shootings one important bad guy still have e.g)
#define __BATTLESHIP_SPACEELMNTSHOOTINGSSTILLLEFTID__			__BATTLESHIPDATAGAME__ -> _spaceElementShootingsLeftScoreObjectId 
#define __BATTLESHIP_GAMETIMECOUNTERID__						__BATTLESHIPDATAGAME__ -> _battleshipTimeCounterScoreObjectId
#define __BATTLESHIP_GAMESCOREID__								__BATTLESHIPDATAGAME__ -> _battleshipPointsScoreObjectId
// There can be many
#define __BATTLESHIP_GAMESCOREFIXTEXTBASEID__					__BATTLESHIPDATAGAME__ -> _battleshipNameScoreObjectId

// Ids for the entities playing in the game (food, things and bad guys are Platform types)
// The Battleship ids...
#define __BATTLESHIP_BATTLESHIPTYPE1ID__						__BATTLESHIPDATAGAME__ -> _battleshipType1Id
#define __BATTLESHIP_BATTLESHIPTYPE2ID__						__BATTLESHIPDATAGAME__ -> _battleshipType2Id
#define __BATTLESHIP_MAINARTISTBASEID__							__BATTLESHIP_BATTLESHIPTYPE1ID__
// ...and their movements...
#define __BATTLESHIP_BATTLESHIP1RUNMOVEMENT__					50000
#define __BATTLESHIP_BATTLESHIP1STAYMOVEMENT__					50001
#define __BATTLESHIP_BATTLESHIP2RUNMOVEMENT__					50002
#define __BATTLESHIP_BATTLESHIP2STAYMOVEMENT__					50003
// ...and their states
#define __BATTLESHIP_BATTLESHIPMOVINGRIGHTSTATE__				0
#define __BATTLESHIP_BATTLESHIPMOVINGRIGHTFORWARDSTATE__		1
#define __BATTLESHIP_BATTLESHIPMOVINGRIGHTBACKWARDSTATE__		2
#define __BATTLESHIP_BATTLESHIPMOVINGLEFTSTATE__				3
#define __BATTLESHIP_BATTLESHIPMOVINGLEFTFORWARDSTATE__			4
#define __BATTLESHIP_BATTLESHIPMOVINGLEFTBACKWARDSTATE__		5
#define __BATTLESHIP_BATTLESHIPMOVINGFORWARDSTATE__				6
#define __BATTLESHIP_BATTLESHIPMOVINGBACKWARDSTATE__			7
#define __BATTLESHIP_BATTLESHIPSTAYINGSTATE__					8
#define __BATTLESHIP_BATTLESHIPROTATINGSTATE__					9
#define __BATTLESHIP_BATTLESHIPEXPLODINGSTATE__					10
#define __BATTLESHIP_BATTLESHIPBLANKSTATE__						11
// The monitors to control how the battleship moves at presentation time
#define __BATTLESHIP_PRESENTATIONMONITORBASEID__				50000
#define __BATTLESHIP_PRESENTATIONMONITORNUMBERMOVS__			4

// In the defafaul implementation no other type of spaceships are considered...

// The shootings
#define __BATTLESHIP_SPACESHIPSHOOTINGBASEID__					550000
#define __BATTLESHIP_SPACESHIPMAXSHOOTINGSPERSPACESHIP__		__BATTLESHIPDATAGAME__ -> _maxNumberOfShootingsPerEnemyInScene
#define __BATTLESHIP_SPACESHITPMAXSHOOTINGS__					__BATTLESHIPDATAGAME__ -> _maxNumberOfShootingsInScene
// ...and their movements...
#define __BATTLESHIP_SPACESHIPSHOOTINGMOVEMENTBASEID__			550000
#define __BATTLESHIP_SPACESHIPSHOOTINGNOMOVBASEID__				570000
// ...and their common states...
#define __BATTLESHIP_SPACESHIPSHOOTINGEXPLODINGSTATE__			__BATTLESHIPDATAGAME__ -> _numberOfShootingTypes // Almost the last
#define __BATTLESHIP_SPACESHIPSHOOTINGBLANKSTATE__				__BATTLESHIP_SPACESHIPSHOOTINGEXPLODINGSTATE__ + 1 // The last state

// The asteroids
#define __BATTLESHIP_ASTEROIDBASEID__							53000
#define __BATTLESHIP_MAXASTEROIDS__								__BATTLESHIPDATAGAME__ -> _maxNumberOfAsteroidsInScene
// ...and their movements...
#define __BATTLESHIP_ASTEROIDMOVEMENTBASEID__					53000
#define __BATTLESHIP_ASTEROIDNOMOVEMENTBASEID__					53100
// ...and their common states
#define __BATTLESHIP_ASTEROIDEXPLODINGSTATE__					(__BATTLESHIPDATAGAME__ -> _numberOfAsteroidTypes << 1) // The next state
#define __BATTLESHIP_ASTEROIDBLANKSTATE__						__BATTLESHIP_ASTEROIDEXPLODINGSTATE__ + 1 // The next of the next

// The batteship things to catch in the space 
// By default just only one at the same time in the scene is considered
#define __BATTLESHIP_THINGTOCATCHBASEID__						54000
#define __BATTLESHIP_MAXTHINGSTOCATCH__							__BATTLESHIPDATAGAME__ -> _maxNumberOfThingsToCatchInScene
// ...and their movements...
#define __BATTLESHIP_THINGTOCATCHMOVEMENTBASE__					54000
#define __BATTLESHIP_THINGTOCATCHNOMOVEMENTBASE__				54100
// ...and their common states...
#define __BATTLESHIP_THINGTOCATCHEXPLODINGSTATE__				(__BATTLESHIPDATAGAME__ -> _numberOfThingsToCatchTypes << 1)
#define __BATTLESHIP_THINGTOCATCHBLANKSTATE__					__BATTLESHIP_THINGTOCATCHEXPLODINGSTATE__ + 1

// The enemies
#define __BATTLESHIP_ENEMYBASEID__								56000
#define __BATTLESHIP_MAXNUMBERENEMIES__							__BATTLESHIPDATAGAME__ -> _maxNumberOfEnemiesInScene
// ...the basic type of enemies...
#define __BATTLESHIP_ENEMTYTYPE1__								0
#define __BATTLESHIP_ENEMTYTYPE2__								1
#define __BATTLESHIP_ENEMTYTYPE3__								2
// ...and the different movements...
#define __BATTLESHIP_ENEMYMOVEMENTBASE__						56000
#define __BATTLESHIP_ENEMYNOMOVEMENTBASE__						56100
// ...and their common states states...
#define __BATTLESHIP_ENEMYEXPLODINGSTATE__						(__BATTLESHIPDATAGAME__ -> _numberOfUFOTypes << 1)
#define __BATTLESHIP_ENEMYBLANKSTATE__							__BATTLESHIP_ENEMYEXPLODINGSTATE__ + 1

// The sounds
/** When a spaceship fires. */
#define __BATTLESHIP_SPACESHIPFIRINGSOUNDID__					__BATTLESHIPDATAGAME__ -> _spaceshipFiringSoundId 
/** When the batleship gets something in the middle of the space. */
#define __BATTLESHIP_BATTLESHIPGETSOUNDID__						__BATTLESHIPDATAGAME__ -> _battleshipGetSoundId
/** When the battelship loads energy. */
#define __BATTLESHIP_BATTLESHIPLOADENERGYSOUNDID__				__BATTLESHIPDATAGAME__ -> _battleshipLoadEnergySoundId
/** When a spaceship explodes. */
#define __BATTLESHIP_SPACESHIPEXPLODESOUNDID__					__BATTLESHIPDATAGAME__ -> _spaceshipExplodingSoundId
/** When the spaceship hits another. */
#define __BATTLESHIP_SPACESHIPHITSANOTHERELMNTSOUNDID__			__BATTLESHIPDATAGAME__ -> _spaceshipHitsAnotherElmntSoundId
// --------------------------------------------------------------------------------

// The events
/** When the energy of the space element has been actualized */
#define __BATTLESHIP_SPACEELMNTENERGYACTUALIZED__				50000
#define __BATTLESHIP_SPACEELMNTENERGYVALUEPARAMETER__			0 // A bdata
#define __BATTLESHIP_SPACEELMNTENERGYIS0__						50001
/** When the lives for the battleship are actualized */
#define __BATTLESHIP_BATTLESHIPLIVESACTUALIZED__				50002
#define __BATTLESHIP_BATTLESHIPLIVESVALUEPARAMETER__			0 // An int
/** When the things carrid by the battleship are actualized. */
#define __BATTLESHIP_BATTLESHIPTHINGSCARRIEDACTUALIZED__		50003
#define __BATTLESHIP_BATTLESHIPTHINGSCARRIEDVALUEPARAM__		0 // A string
/** When the shootings are actualized */
#define __BATTLESHIP_SPACESHIPSHOOTINGSACTUALIZED__				50004
#define __BATTLESHIP_SPACESHIPSHOOTINGSVALUEPARAM__				0 // An int
/** When the time spent is actualized */
#define __BATTLESHIP_GAMETIMEACTUALIZED__						50005
#define __BATTLESHIP_GAMETIMEVALUEPARAMETER__					0 // An int (in seconds)
/** When the score is actualized */
#define __BATTLESHIP_SPACESHIPSCOREACTUALIZED__					50006
#define __BATTLESHIP_SPACESHIPSCOREVALUEPARAMETER__				0 // An int
/** Related with space element, spaceship or battleship: when destroyed, reach the goal, etc... */
#define __BATTLESHIP_SPACEELMNTHASDESTROYED__					50008
#define __BATTLESHIP_SPACESHIPREACHEDGOAL__						50100
#define __BATTLESHIP_BATTLESHIPLEFTTHING__						50101
/** From the input handler */
#define __BATTLESHIP_MOVINGUPEVENT__							50009
#define __BATTLESHIP_MOVINGDOWNEVENT__							50010
#define __BATTLESHIP_MOVINGLEFTEVENT__							50011
#define __BATTLESHIP_MOVINGRIGHTEVENT__							50012
#define __BATTLESHIP_MOVINGUPLEFTEVENT__						50013
#define __BATTLESHIP_MOVINGDOWNLEFTEVENT__						50014
#define __BATTLESHIP_MOVINGUPRIGHTEVENT__						50015
#define __BATTLESHIP_MOVINGDOWNRIGHTEVENT__						50016
#define __BATTLESHIP_JUMPEVENT__								50017
#define __BATTLESHIP_FIREEVENT__								50018
#define __BATTLESHIP_CATCHLEAVESOMETHINGEVENT__					50019
#define __BATTLESHIP_ITERATEELEMENTSEVENT__						50020
#define __BATTLESHIP_NOMOVINGEVENT__							50021
/** To indicate things related with the shootings */
#define __BATTLESHIP_SHOOTINGLAUNCHED__							50022
#define __BATTLESHIP_SHOOTINGTOEND__							50023
#define __BATTLESHIP_SHOOTINGTOEXPLODE__						50024
/** To indicate things related with the all space elements. */
#define __BATTLESHIP_SPACEELEMENTOUTOFSCENE__					50025
#define __BATTLESHIP_SPACEELEMENTCAUGHT__						50026
#define __BATTLESHIP_SPACEELEMENTDESTROYED__					50027
/** Things related with the action block controlling the space elements. */
#define __BATTLESHIP_SPACEELEMENTACTIONBLOCKELMNTCREATED__		50028
#define __BATTLESHIP_SPACEELEMENTACTIONBLOCKELMNTMEMERGED__		50029
#define __BATTLESHIP_SPACEELEMENTACTIONBLOCKELMNTMOVING__		50030
#define __BATTLESHIP_SPACEELEMENTACTIONBLOCKTIMEOVER__			50031
/** Things related with the action block controlling a set of UFOs */
#define __BATTLESHIP_NOUFOSMOVING__								50032
/** Thing related whit any block. */
#define __BATTLESHIP_ACTIONBLOCKISOVER__						50033

// The buoy used to change the state of a space element
#define __BATTLESHIP_CHANGESPACEELEMENTSTATEBUOY__				0
// The buoy used to finish a shooting in a loop different than the one processing the events...
#define __BATTLESHIP_ENDSHOOTINGBUOY__							1
// The buouy using inthe shooting to explode defrerred
#define __BATTLESHIP_TOEXPLODESHOOTINGBUOY__					0

// The parameters used when setting up the behaviour of a space element from a list of open values...
#define __BATTLESHIP_BHVOPENVALUEHURMEDEMISPARAM__				0
#define __BATTLESHIP_BHVOPENVALUEHURMEDSIMSPARAM__				1
#define __BATTLESHIP_BHVOPENVALUEENERGYLOSTPARAM__				2
#define __BATTLESHIP_BHVOPENVALUEENERGYDETRACTEDPARAM__			3
#define __BATTLESHIP_BHVOPENVALUEPTSWHENDIEPARAM__				4
#define __BATTLESHIP_BHVOPENVALUEAUTOMOVPARAM__					5
#define __BATTLESHIP_BHVOPENVALUESHTENERGYPARAM__				6
#define __BATTLESHIP_BHVOPENVALUESHTTYPEPARAM__					7
#define __BATTLESHIP_BHVOPENVALUESHIELDFACTORPARAM__			8

// All BATTLESHIP scenes can define a couple of fundamental attributes...
#define __BATTLESHIP_SCENEATTRNAME__							"NAME"
#define	__BATTLESHIP_SCENEDEFATTRNAME__							"---"
#define __BATTLESHIP_SCENEATTRMOVEMENT__						"MOVEMENT"
#define __BATTLESHIP_SCENEDEFATTRMOVEMENT__						QGAMES::Vector (__BD 1, __BD 0, __BD 0)
#define __BATTLESHIP_SCENEATTRBACKMUSIC__						"BACKGROUNDMUSIC"
// The same with the world...
#define __BATTLESHIP_WORLDATTRNAME__							"NAME"
#define	__BATTLESHIP_WORLDDEFATTRNAME__							"---"

// The TimeScene finishes when the time is over...
#define __BATTLESHIP_TIMELIMITSCENEATTRSCENETIME__				"SCENETIME"
#define __BATTLESHIP_TIMELIMITSCENEDEFATTRSCENETIME__			__BD 120 // Two minutes when "frames per second" is 60

// The SequentialActionBlocks need to move sequentially through a list of action blocks...
#define __BATTLESHIP_SEQACTIONBLOCKSCENEATTRABLIST__			"SEQUENTIALACTIONBLOCKS"
#define __BATTLESHIP_SEQACTIONBLOCKSCENEDEFATTRABLIST__			"" // None if nothing is defined

// Related with the action blocks
// Wait time action block
#define __BATTLESHIP_ACTIONBLOCKWAITTIMEBASEID__				60000
#define __BATTLESHIP_ACTIONBLOCKWAITTIMENUMBER__				1000
#define __BATTLESHIP_WAITTIMEBLOCKATTRSECONDSTOWAIT__			"TimeToWait"

// Common variables to simple space element blocks
#define __BATTLESHIP_SPACEELMNTBLOCKATTRSECONDSTOAPPEAR__		"TimeToAppear"
#define __BATTLESHIP_SPACEELMNTBLOCKATTRPRBTOAPPEAR__			"ProbabilityToAPpear"
#define __BATTLESHIP_SPACEELMNTBLOCKATTRTOEMERGE__				"Emerge"
#define __BATTLESHIP_SPACEELMNTBLOCKATTRTOMOVE__				"TimeToMove"
#define __BATTLESHIP_SPACEELMNTBLOCKATTRREGENERATE__			"Regenerate"
#define __BATTLESHIP_SPACEELMNTBLOCKATTRROLL__					"Roll"
#define __BATTLESHIP_SPACEELMNTBLOCKATTRINITPOSITION__			"InitialPosition"
#define __BATTLESHIP_SPACEELMNTBLOCKATTRMOVZONE__				"MoveZone"
#define __BATTLESHIP_SPACEELMNTBLOCKATTRMAXTIMEACTION__			"MaxTimeAction"
// Asteroid blocks
#define __BATTLESHIP_ACTIONBLOCKASTEROIDBASEID__				70000
#define __BATTLESHIP_ACTIONBLOCKASTEROIDNUMBER__				1000
#define __BATTLESHIP_ASTEROIDBLOCKATTRTYPE__					"Type"
#define __BATTLESHIP_ASTEROIDBLOCKATTRTYPEMOVEMENT__			"MovType"
#define __BATTLESHIP_ASTEROIDBLOCKATTRPOINTSGIVEN__				"PointsGiven"
#define __BATTLESHIP_ASTEROIDBLOCKATTRSPEED__					"Speed"
// Fuel Pill blocks
#define __BATTLESHIP_ACTIONBLOCKFUELPILLBASEID__				71000
#define __BATTLESHIP_ACTIONBLOCKFUELPILLNUMBER__				1000
#define __BATTLESHIP_FUELPILLBLOCKATTRENERGYADDED__				"EnergyAdded"
// Things to catch blocks
#define __BATTLESHIP_ACTIONBLOCKTHINGTOCATCHBASEID__			72000
#define __BATTLESHIP_ACTIONBLOCKTHINGTOCATCHNUMBER__			1000
#define __BATTLESHIP_THINGTOCATCHBLOCKATTRTYPE__				"Type"
#define __BATTLESHIP_THINGTOCATCHBLOCKATTRTYPEMOVEMENT__		"MovType"
#define __BATTLESHIP_THINGTOCATCHBLOCKATTRSPEED__				"Speed"
// Pill of shootings blocks
#define __BATTLESHIP_SHOOTPILLBLOCKATTRTYPEADDED__				"TypeShooting"
// UFO Block
#define __BATTLESHIP_ACTIONBLOCKUFOBASEID__						73000
#define __BATTLESHIP_ACTIONBLOCKUFONUMBER__						1000
#define __BATTLESHIP_UFOATTRTYPE__								"Type"
#define __BATTLESHIP_UFOATTRTYPEMOVEMENT__						"MovType"
#define __BATTLESHIP_UFOATTRMOVEMENTPARAMS__					"MovTypeParameters"

// Types of movement associated to the action block to control UFOs
// A no movement...
#define __BATTLESHIP_UFOATTRTYPNOMOVEMENT__						0 
// Mov following a line from the upper part of the window to the bottom part of it
#define __BATTLESHIP_UFOATTRTYPEMOVLINEAR0__					1 
// Mov following a line from the left part of the window to the right part of it
#define __BATTLESHIP_UFOATTRTYPEMOVLINEAR1__					2
// A circular movement
#define __BATTLESHIP_UFOATTRTYPEMOVCIRCULAR0__					3
// Moving horizontally a little bit to the right and a little bit to the left and back...
#define __BATTLESHIP_UFPATTRTYPEMOVEDANCING0__					4
// Moving horizontally a little bit to the top and a little bit to the top and back...
#define __BATTLESHIP_UFPATTRTYPEMOVEDANCING1__					5
// Attacking from the right...
#define __BATTLESHIP_XYUFOATTRTYPEATTACKING0__					6
// Attacking from the left
#define __BATTLESHIP_XYUFOATTRTYPEATTACKING1__					7
// Attacking following a line, then a circle, then a line again...
#define __BATTLESHIP_XYUFOATTRTYPEATTACKING2__					8
// Attacking following a line, then an arc, then a line again...
#define __BATTLESHIP_XYUFOATTRTYPEATTACKING3__					9
// Attacking following a sinusoidal line...
#define __BATTLESHIP_XYUFOATTRTYPEATTACKING4__					10
// The types of formations to attack
#define __BATTLESHIP_BLOCKANDFLYCOMBATFORMATIONTYPE__			0 // Staying in a block before attacking individually!
#define __BATTLESHIP_QUEUEANDFLYCOMBATFORMATIONTYPE__			1 // Staying before attacking following the leader!
#define __BATTLESHIP_DANCINGINLINESCOMBATFORMATIONTYPE__		2 // Staying before moving all like block!

#define __BATTLESHIP_UFOATTRTYPESHOOTING__						"TypeShooting"
#define __BATTLESHIP_UFOATTRTIMETOSHOOT__						"TimeToShoot"
#define __BATTLESHIP_UFOATTRPRBTOSHOOT__						"ProbabilityToShoot"
#define __BATTLESHIP_UFOATTRSHOOTINGFORCE__						"ShootingForce"
#define __BATTLESHIP_UFOATTRSPEED__								"Speed"
// Set of UFOSs action block
#define __BATTLESHIP_ACTIONBLOCKSETUFOSBASEID__					74000
#define __BATTLESHIP_ACTIONBLOCKSETUFOSNUMBER__					1000
#define __BATTLESHIP_ACTIONBLOCKATTRCOMBATFORMATION__			"CombatFormation"
#define __BATTLESHIP_ACTIONBLOCKATTRCOMBATFORMATIONATTRS__		"CombatFormationAttrs"
#define __BATTLESHIP_ACTIONBLOCKATTRCOMBATFORMATTACKPARAMS__	"CombatFormationAttackParameters"
#define __BATTLESHIP_ACTIONBLOCKATTRNUMBERUFOS__				"NumberUFOS"
#define __BATTLESHIP_ACTIONBLOCKATTRTIMEREVERY__				"TimerEvery"
#define __BATTLESHIP_ACTIONBLOCKSETUFOSATTRFORHOWMANY__			"TimerForHowMany"
#define __BATTLESHIP_ACTIONBLOCKSETUFOSATTRPROB__				"TimerProbability"
// Different lines of UFOs just moving...
#define __BATTLESHIP_SETUFOSFORMATLINES0__						0

// The type of scenes
// Scene with limit of time
#define __BATTLESHIP_SCENETIMELIMITBASEID__						60000
#define __BATTLESHIP_SCENETIMENUMBERSCENES__					100
// Scene to process a sequential list of action blocks
#define __BATTLESHIP_SCENESEQACTIONBLOCKSBASEID__				61000
#define __BATTLESHIP_SCENESEQACTIONBLOCKSNUMBERSCENES__			100

// ---Related to the example included in the library -----------------------------------------------------------------------------
// The basic world, scene and map used into the basic standard definition
#define __BATTLESHIP_EXAMPLEWORLDID__							50000
#define __BATTLESHIP_TIMELIMITEXAMPLESCENEID__					60000 // a specific implementation of time limit...
#define __BATTLESHIP_SEQUENTIALACTIONBLOCKSEXAMPLESCENEID__		60100 // a specific implementation of sequential action blocks...
#define __BATTLESHIP_SIMPLESMOVINGBACKGROUNDMAPID__				60000
#define __BATTLESHIP_TRANSPARENTMAPID__							70000
#define __BATTLESHIP_COMPLEXSTARTSMOVINGBACKGROUNDMAP1ID__		50100
#define __BATTLESHIP_COMPLEXSTARTSMOVINGBACKGROUNDMAP2ID__		50101
#define __BATTLESHIP_COMPLEXSTARTSMOVINGBACKGROUNDMAP3ID__		50102
#define __BATTLESHIP_COMPLEXSTARTSMOVINGBACKGROUNDMAP4ID__		50103
#define __BATTLESHIP_COMPLEXSTARTSMOVINGBACKGROUNDMAP5ID__		50104
#define __BATTLESHIP_COMPLEXSTARTSMOVINGBACKGROUNDMAP6ID__		50105
#define __BATTLESHIP_COMPLEXSTARTSMOVINGBACKGROUNDMAPBASEID__	50100
#define __BATTLESHIP_NUMBERCOMPLEXSTARTMOVINGBACKGROUND__		6
// --------------------------------------------------------------------------------

#endif
  
// End of the file
/*@}*/