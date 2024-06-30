/** \ingroup Dragonwind */
/*@{*/

/**	
 *	@file	
 *	File: dwdefinitions.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 04/01/2019 \n
 *	Description: Defines Dragonwind like library definitions. \n
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_DEFINITIONS__
#define __DRAGONWIND_DEFINITIONS__

#include <DragonwindLike/dwdatagame.hpp>

#define __DRAGONWINDDATAGAME__						DRAGONWIND::DataGame::_theDataGame

// The basic parameteres of the game
#define __DRAGONWIND_GAMEINITWORLDPARAMETER__		"INITIALWORLD"
#define __DRAGONWIND_GAMEINITWORLDDEFPARAMETER__	"50000"
#define __DRAGONWIND_GAMEINITSCENEPARAMETER__		"INITIALSCENE"
#define __DRAGONWIND_GAMEINITSCENEDEFPARAMETER__	"60000"
#define __DRAGONWIND_GAMEPASSWORDPARAMETER__		"PASSWORD"
#define __DRAGONWIND_GAMEPASSWORDDEFPARAMETER__		__NULL_STRING__ // None...

/** The additional keys used in this type of games */
#define __DRAGONWIND_KEYITERATE__					7
#define __DRAGONWIND_KEYCATCHLEAVE__				8
#define __DRAGONWIND_KEYJUMP__						9

// Game states types
/** Name of the player playing */
#define __DRAGONWIND_GAMESTATEPLAYERNAMETYPE__		50000
/** Special version of the credit titles to consider different fonts and the logo in one of them */
#define __DRAGONWIND_GAMESTATECREDITTITLESTYPE__	50090
#define __DRAGONWIND_GAMESTATECREDITTILEATTRNUM__	"NUMBERTITLE"
#define __DRAGONWIND_GAMESTATECRTTITLEATTRNUMDEF__	2
#define __DRAGONWIND_GAMESTATECREDITTITLEATTRCNT__	"CONTENTTITLE"
#define __DRAGONWIND_GAMESTATECRTTITLEATTRCNTDEF__	"Dragonwind"
/** Name of the current world */
#define __DRAGONWIND_GAMESTATEWORLDNAMETYPE__		50010
/** Name of the current scene */
#define __DRAGONWIND_GAMESTATESCENENAMETYPE__		50011
/** To create the DRAGONWIND menu */
#define __DRAGONWIND_GAMESTATEMENUTYPE__			50100
/** To create the Playing Game State Control */
#define __DRAGONWIND_GAMESTATECONTROLPLAYINGTYPE__	50080
/** To create the Playing Game State (inside Playing Game State Control) */
#define __DRAGONWIND_GAMESTATEPLAYINGTYPE__			50081
/** To create the Options when Playing Game State */
#define __DRAGONWIND_GAESTATEOPTIONSWHENPLAYTYPE__	50082
/** Special version of the hall of the fame */
#define __DRAGONWIND_GAMESTATEHALLOFFAMETYPE__		50070
/** Special version to control all the game */
#define __DRAGONWIND_GAMESTATECTRLTYPE__			50101
/** To initialize the gam, and to avoid doing so at init method in game class. */
#define __DRAGONWIND_GAMESTATEINITIALIZEGAMETYPE__	50102

// Game states
#define __DRAGONWIND_STANDARDCONTROLGAMESTATE__		50000 // Another one can be defined, but this is the standard one!
#define __DRAGONWIND_PAUSEGAMESTATE__				50001
#define __DRAGONWIND_COMMTYLOGOGAMESTATE__			50002
#define __DRAGONWIND_LOADINGGAMESTATE__				50003
#define __DRAGONWIND_INTRODUCTIONGAMESTATE__		50004
#define __DRAGONWIND_MENUGAMESTATE__				50009
#define __DRAGONWIND_PLAYERNAMEGAMESTATE__			50014
#define __DRAGONWIND_PLAYERDIEDGAMESTATE__			50019
#define __DRAGONWIND_GAMEFINISHESGAMESTATE__		50022
#define __DRAGONWIND_HALLOFFAMEGAMESTATE__			50025
#define __DRAGONWIND_EXITGAMESTATE__				50028
#define __DRAGONWIND_CONTROLPLAYINGGAMESTATE__		50100
#define __DRAGONWIND_BEFOREPLAYINGGAMESTATE__		50101
#define __DRAGONWIND_PLAYINGGAMESTATE__				50107
#define __DRAGONWIND_OPTIONSWHENPLAYINGGAMESTATE__	50200
#define __DRAGONWIND_PLAYERGAMEOVERGAMESTATE__		50022

// GUI Systems
/** To ask for the name of the player */
#define __DRAGONWIND_PLAYERNAMEGUISYSTEM__			50000
#define __DRAGONWIND_PLAYERNAMEOPTIONID__			50002
/** Additional option to select the instructions to be shown in the standard main menu (@see adguisystem.xml) */
#define __DRAGONWIND_GUISYSTEMINSTRUCTIONSWDT__		50010
/** Additional option to select the type of artist playing in the standard main manu (@see adguisystem.xml) */
#define __DRAGONWIND_GUISYSTEMTYPEARTISTWDT__		50011
/** Additional options to select the keys used to catch and leave things in the standard keys menu (@see adguisystem.xml) */
#define __DRAGONWIND_GUISYSTEMCFGKEYITERATEWDT__	50110
#define __DRAGONWIND_GUISYSTEMCFGKEYCATCHLEAVEWDT__	50111
#define __DRAGONWIND_GUISYSTEMCFGKEYJUMPWDT__		50112
/** Particualr GUI System used for the options when playing */
#define __DRAGONWIND_OPTIONWHENPLAYINGGUISYSTEM__	50200
#define __DRAGONWIND_OPTIONSGUISYSTEMMAINWIDGET__	50201
#define __DRAGONWIND_GUISYSTEMSAVEGAMEWDT__			50202
#define __DRAGONWIND_GUISYSTEMEXITGAMEWDT__			50203
#define __DRAGONWIND_GUISYSTEMCONTINUEGAMEWDT__		50204

// Other...
/** The number of buoy used to execute task over the shooting in a deferred way */
#define __DRAGONWIND_STHTASKEXECDEFEREDBUOYID__		50000
/** The attribute in the ThingsToBeCaught describing the number of shoots left when there are */
#define __DRAGONWIND_GUNSTOCATCHNUMBERSHOOTS__		3 
/** The attribute in ThingsToBeCaught describing the number of lives left */
#define __DRAGONWIND_HEARTTOCATCHNUMBERLIVES__		3
/** The attribute in ThingsToBeEaten describing the aspect of the food. */
#define __DRAGONWIND_FOODASPECTATTRIBUTE__			0
/** The maximum wide of any thing being carried in the screen in pixels... */
#define __DRAGONWIND_NINJATHINGTOCARRYPIXELSWIDE__	20
/** The number of the main goal (it is the same than the tile representing the main goal) */
#define __DRAGONWIND_MAINGOAL__						0
#define __DRAGONWIND_NUMBERGOALPARAM__				0

// ---Confgurable things-----------------------------------------------------------------------------
// Things that can be configurated (@see DataGame class)
/** General game attributes */
#define __DRAGONWIND_MAXNUMBEROFLIVES__				__DRAGONWINDDATAGAME__ -> _maxNumberOfLives
#define __DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__	__DRAGONWINDDATAGAME__ -> _maxNumberOfThingsBeingCarried
#define __DRAGONWIND_NINJATHINGTYPES__				__DRAGONWINDDATAGAME__ -> _numberOfPossibleThingsBeingCarried

// Important configurable forms used in the game
/** The form to represent the lives in the game (2 aspects, one for male player, and another one for the female one) */
#define __DRAGONWIND_NINJALIVESFORM__				__DRAGONWINDDATAGAME__ -> _livesFormId
/** The form to represent the things to carry (same number that things can be used in Location.png) 
	With minimum __DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__ different aspect inside */
#define __DRAGONWIND_NINJATHINGTOCARRYFORM__		__DRAGONWINDDATAGAME__ -> _thingsToCarryFormId
/** The form used to represent the food **/
#define __DRAGONWIND_NINJAFOODFORM__				__DRAGONWINDDATAGAME__ -> _foodFormId
/** The form used to hight light food with high energy. */
#define __DRAGONWIND_NINJAHIGHENERGYFOODFORM__		__DRAGONWINDDATAGAME__ -> _highEnergyFoodFormId
/** Number of different foods */
#define __DRAGONWIND_NINJAFOODTYPES__				__DRAGONWINDDATAGAME__ -> _numberOfFoodTypes

// Id of the entities used in presentation (main menu)
#define __DRAGONWIND_NINJAMALEPRESENTATIONID__		__DRAGONWINDDATAGAME__ -> _ninjaMalePresentationId
#define __DRAGONWIND_NINJAFEMALEPRESENTATIONID__	__DRAGONWINDDATAGAME__ -> _ninjaFemalePresentationId
#define __DRAGONWIND_NINJAPRESENTATIONBASICSTATE__	0 // The state used for that previous entity (by default it is running)

// Ids for the entities used as score objects
// There could be many (the energy of the bad guy that the main artist is fighting against e.g)
#define __DRAGONWIND_ENTITYENERGYSCOBJID__			__DRAGONWINDDATAGAME__ -> _ninjaEnergyScoreObjectId 
#define __DRAGONWIND_NINJALIVESSCOBJID__			__DRAGONWINDDATAGAME__ -> _ninjaLivesScoreObjectId
// Up to __DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__
#define __DRAGONWIND_NINJATHINGBEINGCARRIEDID__		__DRAGONWINDDATAGAME__ -> _ninjaThingBeingCarriedScoreObjectBaseId 
#define __DRAGONWIND_NINJATHINGSBEGINGCARRIEDID__	__DRAGONWINDDATAGAME__ -> _ninjaThingsBeingCarriedScoreObjectId
// There could be many (the number of shootings one important bad guy still have e.g)
#define __DRAGONWIND_ENTITYSHOOTINGSSTILLLEFTID__	__DRAGONWINDDATAGAME__ -> _ninjaShootingsLeftScoreObjectId 
#define __DRAGONWIND_GAMETIMECOUNTERID__			__DRAGONWINDDATAGAME__ -> _ninjaTimeCounterScoreObjectId
#define __DRAGONWIND_GAMESCOREID__					__DRAGONWINDDATAGAME__ -> _ninjaPointsScoreObjectId
// There can be many
#define __DRAGONWIND_GAMESCOREFIXTEXTBASEID__		__DRAGONWINDDATAGAME__ -> _ninjaNameScoreObjectId 

// Ids for the entities playing in the game (food, things and bad guys are Platform types)
#define __DRAGONWIND_MAINARTISTNINJAMALE__			__DRAGONWINDDATAGAME__ -> _ninjaMaleId
#define __DRAGONWIND_MAINARTISTNINJAFEMALE__		__DRAGONWINDDATAGAME__ -> _ninjaFemaleId
#define __DRAGONWIND_MAINARTISTBASEID__				__DRAGONWIND_MAINARTISTNINJAMALE__
// Standard movements of the main artist for the default implementation
// The only way to increment/change them is to consider another new main artist
#define __DRAGONWIND_NINJAMALESTANDMOVID__			50000
#define __DRAGONWIND_NINJAMALERUNMOVID__			50001
#define __DRAGONWIND_NINJAMALEJUMPMOVID__			50002
#define __DRAGONWIND_NINJAMALESLIDEMOVID__			50003
#define __DRAGONWIND_NINJAMALECLIMBMOVID__			50004
#define __DRAGONWIND_NINJAMALEATTACKMOVID__			50005
#define __DRAGONWIND_NINJAMALEGLADMOVID__			50006
#define __DRAGONWIND_NINJAMALEFLOATMOVID__			50007
#define __DRAGONWIND_NINJAMALEHAPPYMOVID__			50008
#define __DRAGONWIND_NINJAFEMALESTANDMOVID__		50100
#define __DRAGONWIND_NINJAFEMALERUNMOVID__			50101
#define __DRAGONWIND_NINJAFEMALEJUMPMOVID__			50102
#define __DRAGONWIND_NINJAFEMALESLIDEMOVID__		50103
#define __DRAGONWIND_NINJAFEMALECLIMBMOVID__		50104
#define __DRAGONWIND_NINJAFEMALEATTACKMOVID__		50105
#define __DRAGONWIND_NINJAFEMALEGLADMOVID__			50106
#define __DRAGONWIND_NINJAFEMALEFLOATMOVID__		50107
#define __DRAGONWIND_NINJAFEMALEHAPPYMOVID__		50108
// Standard states of the main artist for the default implementation
// The only way to increment/change them is to define another main character
#define __DRAGONWIND_NINJASTANDRIGHTSTATEID__		0
#define __DRAGONWIND_NINJARUNNINGRIGHTSTATEID__		1
#define __DRAGONWIND_NINJAJUMPRIGHTSTATEID__		2
#define __DRAGONWIND_NINJASLIDERIGHTSTATEID__		3
#define __DRAGONWIND_NINJACLIMBINGRIGHTSTATEID__	4
#define __DRAGONWIND_NINJATHROWRIGHTSTATEID__		6
#define __DRAGONWIND_NINJAGLADERIGHTSTATEID__		7
#define __DRAGONWIND_NINJADIEINGRIGHTSTATEID__		10
#define __DRAGONWIND_NINJAFALLRIGHTSTATEID__		11
#define __DRAGONWIND_NINJADIEDRIGHTSTATEID__		12
#define __DRAGONWIND_NINJACLIMBFIXEDRIGHTSTATEID__	13
#define __DRAGONWIND_NINJAHAPPYRIGHTSTATEID__		14
#define __DRAGONWIND_NINJASTANDLEFTSTATEID__		20
#define __DRAGONWIND_NINJARUNNINGLEFTSTATEID__		21
#define __DRAGONWIND_NINJAJUMPLEFTSTATEID__			22		
#define __DRAGONWIND_NINJASLIDELEFTSTATEID__		23
#define __DRAGONWIND_NINJACLIMBINGLEFTSTATEID__		24
#define __DRAGONWIND_NINJATHROWLEFTSTATEID__		26
#define __DRAGONWIND_NINJAGLADELEFTSTATEID__		27
#define __DRAGONWIND_NINJADIEINGLEFTSTATEID__		30
#define __DRAGONWIND_NINJAFALLLEFTSTATEID__			31
#define __DRAGONWIND_NINJADIEDLEFTSTATEID__			32
#define __DRAGONWIND_NINJACLIMBFIXEDLEFTSTATEID__	33
#define __DRAGONWIND_NINJAHAPPYLEFTSTATEID__		34
#define __DRAGONWIND_NINJASWIMRIGHTSTATEID__		40
#define __DRAGONWIND_NINJASWIMRIGHTDIEINGDSTATEID__	41
#define __DRAGONWIND_NINJASWIMRIGHTDIEDSTATEID__	41
#define __DRAGONWIND_NINJASWIMLEFTSTATEID__			50
#define __DRAGONWIND_NINJASWIMLEFTDIEINGSTATEID__	51
#define __DRAGONWIND_NINJASWIMLEFTDIEDSTATEID__		51

// Up to @see __DRAGONWIND_MAXNUMBEROFFLYMONSTERSFLOCK__
#define __DRAGONWIND_BEEBASEID__					__DRAGONWINDDATAGAME__ -> _beesBaseId 
// Up to @see __DRAGONWIND_MAXNUMBEROFFLYMONSTERSFLOCK__
#define __DRAGONWIND_BIRDBASEID__					__DRAGONWINDDATAGAME__ -> _birdsBaseId 
#define __DRAGONWIND_MAXNUMBEROFFLYMONSTERSFLOCK__	__DRAGONWINDDATAGAME__ -> _maxNumberOfFlyingMonstersPerFlock
// Up to @see __DRAGONWIND_MAXNUMBEROFFLOACKS__
#define __DRAGONWIND_FLOACKOFBEESBASE__				__DRAGONWINDDATAGAME__ -> _flockOfBeesId 
// Up to @see __DRAGONWIND_MAXNUMBEROFFLOACKS__
#define __DRAGONWIND_FLOACKOFBIRDSBASE__			__DRAGONWINDDATAGAME__ -> _flockOfBirdsId 
// Usually thought to consider x per level of difficulty!
#define __DRAGONWIND_MAXNUMBEROFFLOACKS__			__DRAGONWINDDATAGAME__ -> _maxNumberOfFlocks
#define __DRAGONWIND_MAXNUMBEROFBADGUYSPERSCENE__	__DRAGONWINDDATAGAME__ -> _maxNumberOfBadGuysPerScene
// Standard movements for flocks
// The only why to increment/change them is to define new types of flocks
#define __DRAGONWIND_BEEFLYMOVEMENTBASEID__			56000
#define __DRAGONWIND_BEESTANDMOVEMENTBASEID__		56010
#define __DRAGONWIND_BIRDFLYMOVEMENTBASEID__		56100
#define __DRAGONWIND_BIRDSTANDMOVEMENTBASEID__		56110
// Standard states for flocks
// The only why to increment/change them is to define new types of flocks
#define __DRAGONWIND_MONSTERFLYINGRIGHTSTATEID__	0
#define __DRAGONWIND_MONSTERSTANDRIGHTSTATEID__		1
#define __DRAGONWIND_MONSTERFLYINGLEFTSTATEID__		2
#define __DRAGONWIND_MONSTERSTANDLEFTSTATEID__		3

#define __DRAGONWIND_SHOOTINGBASEID__				__DRAGONWINDDATAGAME__ -> _shootingBaseId
#define __DRAGONWIND_MAXNUMBEROFSHOOTINGSPERCHR__	__DRAGONWINDDATAGAME__ -> _maxNumberOfShootingsPerCharacter
// Simple calculation. The one added referes to the main artist (that also shoots!!)
#define __DRAGONWIND_MAXNUMBEROFSHOOTINGS__			(__DRAGONWIND_MAXNUMBEROFBADGUYSPERSCENE__ + 1) * __DRAGONWIND_MAXNUMBEROFSHOOTINGSPERCHR__
// Standard movements for shootings in the default implementation
// The only why to increment/change them is to define new types of shooting
#define __DRAGONWIND_SHOOTINGSTANDBASEMOVID__		57000
#define __DRAGONWIND_SHOOTINGSPARABOLICBASEMOVID__	57500
#define __DRAGONWIND_SHOOTINGSBOUNCINGBASEMOVID__	58000
#define __DRAGONWIND_SHOOTINGSROLLINGBASEMOVID__	58500
#define __DRAGONWIND_SHOOTINGSFALLINGBASEMOVID__	59000

// Standard movements for other elements inthe game
// They can't be changed or modified unless another type for food or things to be caught or badguys are defined
#define __DRAGONWIND_FOODSTANDMOVBASEID__			50300
#define __DRAGONWIND_FOODFALLMOVBASEID__			51300
#define __DRAGONWIND_FOODINTHEAIRMOVBASEID__		52300
#define __DRAGONWIND_THINGSTANDMOVBASEID__			53300
#define __DRAGONWIND_THINGFALLMOVBASEID__			54300
#define __DRAGONWIND_BADGUYSTANDMOVBASEID__			60000
#define __DRAGONWIND_BADGUYRUNMOVBASEID__			61000
#define __DRAGONWIND_BADGUYJUMPMOVBASEID__			62000
#define __DRAGONWIND_BADGUYSLIDEMOVBASEID__			63000
// Standard states for the other elements in the game
// They can't be configured at all. THe only way to change / extend them is to create a new "type"
#define __DRAGONWIND_FOODSTANDSTATEID__				0
#define __DRAGONWIND_FOODFALLINGSTATEID__			1
#define __DRAGONWIND_FOODINAIRSTATEID__				2
#define __DRAGONWIND_THINGSTANDSTATEID__			0
#define __DRAGONWIND_THINGFALLINGSTATEID__			1

// The sounds
/** When ninja throws something. */
#define __DRAGONWIND_NINJATHOWINGSOUNDID__			__DRAGONWINDDATAGAME__ -> _ninjaThrowingSoundId 
/** When ninja is hit by a weapon. */
#define __DRAGONWIND_NINJAHITSOUNDID__				__DRAGONWINDDATAGAME__ -> _ninjaHitSoundId
/** When ninja jumps. */
#define __DRAGONWIND_NINJAJUMPSOUNDID__				__DRAGONWINDDATAGAME__ -> _ninjaJumpSoundId
/** When ninja catches something. */
#define __DRAGONWIND_NINJACATCHSOUNDID__			__DRAGONWINDDATAGAME__ -> _ninjaCatchSoundId
/** When ninja activate or desactivate a switch. */
#define __DRAGONWIND_NINJAACTIVATESWITCHSOUNDID__	__DRAGONWINDDATAGAME__ -> _ninjaActivateSwitchSoundId
/** When ninja eats something. */
#define __DRAGONWIND_NINJAEATSOUNDID__				__DRAGONWINDDATAGAME__ -> _ninjaEatSoundId
/** When a bad guy thorws something. */
#define __DRAGONWIND_BADGUYTHROWINGSOUNDID__		__DRAGONWINDDATAGAME__ -> _badGuyThrowingSoundId 
/** When bad guy is hit by something */
#define __DRAGONWIND_BADGUYHITSOUNDID__				__DRAGONWINDDATAGAME__ -> _badGuyHitSoundId
/** When the ninja dies. */
#define __DRAGONWIND_NINJADIESSOUNDID__				__DRAGONWINDDATAGAME__ -> _ninjaDiesSoundId
/** While bees are in the screen */
#define __DRAGONWIND_BEESFLYINGSOUNDID__			__DRAGONWINDDATAGAME__ -> _beesFlyingSoundId
/** While birds are in the screen */
#define __DRAGONWIND_BIRDSFLYINGSOUNDID__			__DRAGONWINDDATAGAME__ -> _birdsFlyingSoundId
/** When a shield is protecting the entity. */
#define __DRAGONWIND_SHIELDSOUNDID__				__DRAGONWINDDATAGAME__ -> _shieldSoundId
/** When an important event happens in the game. */
#define __DRAGONWIND_IMPORTANTEVENTSOUNDID__		__DRAGONWINDDATAGAME__ -> _importantEventSoundId
/** Channel used for flock related sounds (@see addefinitions.hpp to know the other channels used) */
#define __DRAGONWIND_FLOCKSOUNDCHANNEL__			6
/** Second channel used by the main artist. */
#define __DRAGONWIND_MAINARTISTSECONDCHANNEL__		7
// --------------------------------------------------------------------------------

// Type of standard shootings
#define __DRAGONWIND_NINJATHINGARROWTYPE__			0 // Thrown, fly, hit and dissapear
#define __DRAGONWIND_TILEDEFININGARROW__			10
#define __DRAGONWIND_NINJATHINGSBOMBTYPE__			1 // Thrown, bounce and explode
#define __DRAGONWIND_TILEDEFININGBOMB__				11
#define __DRAGONWIND_NINJATHINGSTARTYPE__			6 // Thrown, fly, hit and dissapear
#define __DRAGONWIND_TILEDEFININGSTAR__				16
#define __DRAGONWIND_BADGUYTHINGPOLLBALLTYPE__		20 // Thrown, roll, hit and dissapear

// Other things that can be carried by the main character
#define __DRAGONWIND_NINJATHINGCOINTYPE__			2
#define __DRAGONWIND_NINJATHINGHEARTTYPE__			3
#define __DRAGONWIND_TILEDEFININGHEART__			13
#define __DRAGONWIND_NINJATHINGPAPERTYPE__			4
#define __DRAGONWIND_NINJATHINGQUESTIONTYPE__		5
#define __DRAGONWIND_NINJATHINGSTHUNDERTYPE__		7

// The events
/** When the energy of the ninja has been actualized */
#define __DRAGONWIND_ENTITYENERGYACTUALIZED__		50000
#define __DRAGONWIND_ENTITYENERGYVALUEPARAMETER__	0 // An bdata
#define __DRAGONWIND_ENTITYENERGYIS0__				50001
/** When the lives for the ninja are actualized */
#define __DRAGONWIND_NINJALIVESACTUALIZED__			50002
#define __DRAGONWIND_NINJALIVESVALUEPARAMETER__		0 // An int
/** When something is taken by the ninja */
#define __DRAGONWIND_NINJATHINGSCARRIEDACTUALIZED__	50003
#define __DRAGONWIND_NINJATHINGSCARRIEDVALUEPARAM__	0 // A string
/** When the shootings are actualized */
#define __DRAGONWIND_ENTITYSHOOTINGSACTUALIZED__	50004
#define __DRAGONWIND_ENTITYSHOOTINGSVALIEPARAM__	0 // An int
/** When the time spent is actualized */
#define __DRAGONWIND_GAMETIMEACTUALIZED__			50005
#define __DRAGONWIND_GAMETIMEVALUEPARAMETER__		0 // An int (in seconds)
/** When the score is actualized */
#define __DRAGONWIND_GAMESCOREACTUALIZED__			50006
#define __DRAGONWIND_GAMESCOREVALUEPARAMETER__		0 // An int
#define __DRAGONWIND_FLOCKKILLED__					50007
#define __DRAGONWIND_CHARACTERHASDIED__				50008
#define __DRAGONWIND_NINJAREACHEDGOAL__				50100
/** From the input handler */
#define __DRAGONWIND_MOVINGUPEVENT__				50009
#define __DRAGONWIND_MOVINGDOWNEVENT__				50010
#define __DRAGONWIND_MOVINGLEFTEVENT__				50011
#define __DRAGONWIND_MOVINGRIGHTEVENT__				50012
#define __DRAGONWIND_MOVINGUPLEFTEVENT__			50013
#define __DRAGONWIND_MOVINGDOWNLEFTEVENT__			50014
#define __DRAGONWIND_MOVINGUPRIGHTEVENT__			50015
#define __DRAGONWIND_MOVINGDOWNRIGHTEVENT__			50016
#define __DRAGONWIND_JUMPEVENT__					50017
#define __DRAGONWIND_FIREEVENT__					50018
#define __DRAGONWIND_CATCHLEAVESOMETHINGEVENT__		50019
#define __DRAGONWIND_ITERATEELEMENTSEVENT__			50020
#define __DRAGONWIND_NOMOVINGEVENT__				50021
/** To indicate that something has been left in the room */
#define __DRAGONWIND_THINGLEFTEVENT__				50022
#define __DRAGONWIND_THINGLEFTIDPARAMETER__			0 // It is an int...
#define __DRAGONWIND_THINGLEFTTYPEPARAMETER__		1 // It is an int also...
/** To indicete things related with the shootings */
#define __DRAGONWIND_SHOOTINGTOEND__				50023
#define __DRAGONWIND_SHOOTINGTOEXPLODE__			50024
/** When a dialog from any artist has finished. */
#define __DRAGONWIND_DIALOGHASFINISHED__			50025

// The buoys used in the dargon wind games...
#define __DRAGONWIND_DIEDEFEREDBUOYID__				50000

// The main attributes used in the definition of the properties for a composite entity (flock of monsters)
#define __DRAGONWIND_FLOCKATTRNUMBEROFMONSTERS__	"NUMBEROFMONSTERS"
#define __DRAGONWIND_FLOCKATTRRADIX__				"RADIX"
#define __DRAGONWIND_FLOCKATTRTIMETOMOVE__			"TIMETOMOVE"
#define __DRAGONWIND_FLOCKATTRTIMEMOVING__			"TIMEMOVING"
#define __DRAGONWIND_FLOCATTRSHOOTINGENERGY__		"SHOOTINGENERGY"
#define __DRAGONWIND_FLOCKATTRDIEWHENHIT__			"DIEWHENHIT"

// The parameters used when setting up the behaviour of a character from a list of open values...
#define __DRAGONWIND_BHVOPENVALUEHURMEDEMISPARAM__	0
#define __DRAGONWIND_BHVOPENVALUEHURMEDSIMSPARAM__	1
#define __DRAGONWIND_BHVOPENVALUEHURMEDFLYSPARAM__  2
#define __DRAGONWIND_BHVOPENVALUEHURMEDLIGHTPARAM__	3
#define __DRAGONWIND_BHVOPENVALUEGUNSHURMPARAM__	4
#define __DRAGONWIND_BHVOPENVALUEENERGYLOSTPARAM__	5
#define __DRAGONWIND_BHVOPENVALUEENERGYTPEXPARAM__	6
#define __DRAGONWIND_BHVOPENVALUEPTSWHENDIEPARAM__	7
#define __DRAGONWIND_BHVOPENVALUESHTENERGYPARAM__	8
#define __DRAGONWIND_BHVOPENVALUEAUTOMOVPARAM__		9
#define __DRAGONWIND_BHVOPENVALUEWEAKNESSPARAM__	10
#define __DRAGONWIND_RUNTIMEVALUESBEHAVIOURTYPE__	"BEHAVIOUR"

// Related with the steps and steps builder for DRAGONWIND::Characters
#define __DRAGONWIND_CHRSTEPACTION__				50000
#define __DRAGONWIND_CHRSTEPACTIONATTRTYPE__		"Type"
#define __DRAGONWIND_CHRSTEPACTIONSTAND__			0
#define __DRAGONWIND_CHRSTEPACTIONFALL__			1
#define __DRAGONWIND_CHRSTEPACTIONJUMP__			2
#define __DRAGONWIND_CHRSTEPACTIONMOVE__			3
#define __DRAGONWIND_CHRSTEPACTIONCLIMB__			4
#define __DRAGONWIND_CHRSTEPACTIONSLIDE__			5
#define __DRAGONWIND_CHRSTEPACTIONMOVEFRBTOLIMIT__	10
#define __DRAGONWIND_CHRSTEPACTIONATTRDEFTYPE__		__DRAGONWIND_CHRSTEPACTIONSTAND__ 
#define __DRAGONWIND_CHRSTEPACTIONATTRDIR__			"Direction"
#define __DRAGONWIND_CHRSTEPACTIONATTRDEFDIR__		0 // To the right
#define __DRAGONWIND_CHRSTEPACTIONATTRIMPULSE__		"Impulse"
#define __DRAGONWIND_CHRSTEPACTIONATTRDEFIMPULSE__	2 // Up to 10 (negative number means random up to that number)
#define __DRAGONWIND_CHRSTEPACTIONATTRSECONDS__		"Seconds"
#define __DRAGONWIND_CHRSTEPACTIONATTRDEFSECONDS__	__BD -1 // Means ethernal
#define __DRAGONWIND_CHRSTEPACTIONATTRBACKWARDS__	"Backwards"
#define __DRAGONWIND_CHRSTEPACTIONATTRDEFBACKW__	false 
#define __DRAGONWIND_CHRSTEPACTIONATTRPIXELS__		"Pixels"
#define __DRAGONWIND_CHRSTEPACTIONATTRDEFPIXELS__	0
#define __DRAGONWIND_CHRSTEPACTIONATTRLIMITS__		"Limits"
#define __DRAGONWIND_CHRSTEPACTIONATTRDEFLIMITS__	"0,0,0,0,0,0"

// Related with the action blocks...
#define __DRAGONWIND_ACTIONBLOCKFLYMONSTERBASEID__	70000
#define __DRAGONWIND_ACTIONBLOCKFLYMONSTERNUMBER__	1000
#define __DRAGONWIND_ACTIONBLOCKFMONSTERATTRTYPE__	"TypeOfMonster"
#define __DRAGONWIND_ACTIONBLOCKFMONSTERTYPEBEE__	0
#define __DRAGONWIND_ACTIONBLOCKFMONSTERTYPEBIRD__	1
#define __DRAGONWIND_ACTIONBLOCKFMONSTERATTRTIME__	"TimeToAppear"
#define __DRAGONWIND_ACTIONBLOCKMOVMONSTERBASEID__	71000
#define __DRAGONWIND_ACTIONBLOCKMOVMONSTERNUMBER__	1000
#define __DRAGONWIND_ACTIONBLOCKMONSTERATTRMOVID__	"MonitorId"
#define __DRAGONWIND_ACTIONBLOCKMMONSTERATTRFIRE__	"Fire"
#define __DRAGONWIND_ACTIONBLOCKMMONSTERATTRTFIRE__	"TypeOfFire"
#define __DRAGONWIND_ACTIONBLOCKMMONSTERATTRIFIRE__	"ImpulseOfFire"
#define __DRAGONWIND_ACTIONBLOCKMMONSTERATTRSTOF__	"SecondsToFire"
#define __DRAGONWIND_ACTIONBLOCKMMONSTERATTRSBTF__	"SecondsBetweenFires"
#define __DRAGONWIND_ACTIONBLOCKMMONSTERATTRSFIR__	"SecondsFiring"
#define __DRAGONWIND_ACTIONBLOCKENVCONDBASEID__		72000
#define __DRAGONWIND_ACTIONBLOCKENVCONDNUMBER__		1000
#define __DRAGONWIND_ACTIONBLOCKENVCONDATTRTYPE__	"TypeCondition"
#define __DRAGONWIND_ACTIONBLOCKENVCONDATTRSSTART__	"SecondsToStart"
#define __DRAGONWIND_ACTIONBLOCKENVCONDATTRSENV__	"SecondsCondition"
#define __DRAGONWIND_ACTIONBLOCKENVCONDTYPETHUN__	0
#define __DRAGONWIND_ACTIONBLOCKENVCONDTYPERAIN__	1
#define __DRAGONWIND_ACTIONBLOCKENVCONDTYPEEATRH__	2
#define __DRAGONWIND_ACTIONBLOCKENVCONDTYPEWIND__	3
#define __DRAGONWIND_ACTIONBLOCKENVCONDTYPELIGHT__	4
#define __DRAGONWIND_ACTIONBLOCKENVCONDTYPERAINB__	5
#define __DRAGONWIND_ACTIONBLOCKENVCONDTYPESTORM__	6
#define __DRAGONWIND_ACTIONBLOCKENVCONDTYPECLOUD__	7
#define __DRAGONWIND_ACTIONBLOCKACTLAYERSBASEID__	73000
#define __DRAGONWIND_ACTIONBLOCKACTLAYERNUMBER__	1000
#define __DRAGONWIND_ACTIONBLOCKACTLAYERATTRNUM__	"NumberSets"
#define __DRAGONWIND_ACTIONBLOCKACTLAYERATTRSET__	"Set"
#define __DRAGONWIND_ACTIONBLOCLKACTLAYERATTRDEF__	"DefaultSet"
#define __DRAGONWIND_ACTIONBLOCKMOVELAYERSBASEID__	74000
#define __DRAGONWIND_ACTIONBLOCKMOVLAYERSNUMBER__	1000
#define __DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRSET__	"Set"
#define __DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRDIR__	"Direction"
#define __DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRLONG__	"Length"
#define __DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRSPD__	"Speed"
#define __DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRSND__	"Sound"
#define __DRAGONWIND_ACTIONBLOCKMOVLAYERSATTRSCHN__	"SoundChannel"

// Tile parameters
#define __DRAGONWIND_NOTIFICATIONTILEBATTRLOCKID__	0
#define __DRAGONWIND_NOTIFICATIONTILEATTRACTIVE__	1

// Layer parameters
#define __DRAGONWIND_LIQUIDLAYERPOISONPRM__			"POISON"

// All DRAGONWIND scenes can define a couple of fundamental attributes...
#define __DRAGONWIND_SCENEATTRFUNDAMENTAL__			"FUNDAMENTAL"
#define __DRAGONWIND_SCENEATTRNAME__				"NAME"
#define __DRAGONWIND_SCENEDEFATTRNAME__				"---"
#define __DRAGONWIND_SCENEATTRMOVEMENT__			"MOVEMENT"
#define __DRAGONWIND_SCENEDEFATTRMOVEMENT__			QGAMES::Vector (__BD 1, __BD 0, __BD 0)
#define __DRAGONWIND_SCENEATTRBACKMUSIC__			"BACKGROUNDMUSIC"
#define __DRAGONWIND_SCENEATTRLIQUIDSFLUIDITY__		"LIQUIDSFLUIDITY"
#define __DRAGONWIND_SCENEDEFATTRLIQUIDSFLUIDITY__	(QGAMES::bdata) 0.85
// The same with the world...
#define __DRAGONWIND_WORLDATTRNAME__				"NAME"
#define __DRAGONWIND_WORLDDEFATTRNAME__				"---"

// ---Related to the example included in the library -----------------------------------------------------------------------------
// The basic world, scene and map used into the basic standard definition
#define __DRAGONWIND_EXAMPLEWORLDID__				50000
#define __DRAGONWIND_EXAMPLESCENEID__				60000
#define __DRAGONWIND_EXAMPLEMAPID__					60000

// The background used in some wheather events
#define __DRAGONWIND_BLANKBACKGROUNDFORM__			50160

// Just in the very basic example...
// The basic background maps used into the basic definition
#define __DRAGONWIND_BACKGROUNDMAPSBASEID__			50100
#define __DRAGONWIND_NUMBERBACKGROUNDMAPS__			6 // Number of standard backgrounds used
#define __DRAGONWIND_DESSERTBKMAPID__				50100
#define __DRAGONWIND_DARFORESTBKMAPID__				50110
#define __DRAGONWIND_CITYBKMAPID__					50120
#define __DRAGONWIND_LANDSCAPEBKMAPID__				50130
#define __DRAGONWIND_MOUNTAINSBKMAPID__				50140
#define __DRAGONWIND_TEMPLEBKMAPID__				50150
// --------------------------------------------------------------------------------

#endif
  
// End of the file
/*@}*/