#include <DragonwindLike/dwdatagame.hpp>

DRAGONWIND::DataGame* DRAGONWIND::DataGame::_theDataGame = NULL;

// ---
DRAGONWIND::DataGame::DataGame ()
{
	// It can be created only once...
	assert (_theDataGame == NULL);

	_theDataGame = this;

	// The initial values are assigned
	// General attributes
	_maxNumberOfLives									= 3;
	_maxNumberOfThingsBeingCarried						= 3;
	_numberOfPossibleThingsBeingCarried					= 8;

	// Forms that can be configured for the game!
	_livesFormId										= 50004;
	_thingsToCarryFormId								= 50005; // With minimum _numberOfPossibleThingsBeingCarried different aspect inside...
	_foodFormId											= 50006;
	_highEnergyFoodFormId								= 50007;
	_numberOfFoodTypes									= 63;

	// Entities' id used in the presentation (@see dwentities.xml)
	_ninjaMalePresentationId							= 50000;
	_ninjaFemalePresentationId							= 50001;

	// Score object Ids
	_ninjaEnergyScoreObjectId							= 50000;
	_ninjaLivesScoreObjectId							= 50010;
	_ninjaThingsBeingCarriedScoreObjectId				= 50020;
	_ninjaThingBeingCarriedScoreObjectBaseId			= 50011;
	_ninjaShootingsLeftScoreObjectId					= 50021;
	_ninjaTimeCounterScoreObjectId						= 50031;
	_ninjaPointsScoreObjectId							= 50032;
	_ninjaNameScoreObjectId								= 50100;

	// Entities' id used in the game
	// Take care when configuring these. 
	// Id's up to 40000 are already ocuppied by food, thing to catch and bad guy definitions
	_ninjaMaleId										= 50100;
	_ninjaFemaleId										= 50101;
	_beesBaseId											= 50110;
	_birdsBaseId										= 50120;
	_maxNumberOfFlyingMonstersPerFlock					= 10;
	_flockOfBeesId										= 50130;
	_flockOfBirdsId										= 50140;
	_maxNumberOfFlocks									= 6;
	_maxNumberOfBadGuysPerScene							= 10;
	_shootingBaseId										= 51000;
	_maxNumberOfShootingsPerCharacter					= 10;

	// Sounds' id
	_ninjaThrowingSoundId								= __QGAMES_THROWWAVSOUND__; // @see addefinitions.hpp (all)
	_ninjaHitSoundId									= __QGAMES_HURTWAVSOUND__;
	_ninjaJumpSoundId									= __QGAMES_JUMPWAVSOUND__;
	_ninjaCatchSoundId									= __QGAMES_CATCHWAVSOUND__;
	_ninjaEatSoundId									= __QGAMES_EATWAVSOUND__;
	_badGuyThrowingSoundId								= __QGAMES_THROWWAVSOUND__;
	_badGuyHitSoundId									= __QGAMES_HURTWAVSOUND__;
	_ninjaDiesSoundId									= 50019;
	_beesFlyingSoundId									= 50015;
	_birdsFlyingSoundId									= 50016;
	_shieldSoundId										= 50020;
	_importantEventSoundId								= 50017;
	_ninjaActivateSwitchSoundId							= 50021;
}
