/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: dwdatagame.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 26/07/2019 \n
 *	Description: Define an object with all things that can be configurated for this type of games. \n
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_DATAGAME__
#define __DRAGONWIND_DATAGAME__

#include <DragonwindLike/dwdefinitions.hpp>
#include <Platform/ptinclude.hpp>

namespace DRAGONWIND
{
	/** Singleton.
		This type of game can be reused for many other different similar platform games. 
		So it is necessary an object to keep configurable things such as the entities being involved, sounds used, ...
		In any case, a default value is given representing the standard dragonwind like game. 
		An object of this class is created when the dragowind game class is created. (@see Game class) 
		It is an structure just to simplify the access to the internal attributes. 
		The internal values can be created just after the initialization. */
	struct DataGame
	{
		public:
		DataGame ();
		virtual ~DataGame ()
							{ _theDataGame  = NULL; } 

		// General attributes
		int _maxNumberOfLives;
		int _maxNumberOfThingsBeingCarried;
		int _numberOfPossibleThingsBeingCarried;

		// Related with forms used to represent things to eat and things that can be carried!
		int _livesFormId;
		int _thingsToCarryFormId;
		int _foodFormId;
		int _highEnergyFoodFormId;
		int _numberOfFoodTypes;

		// Entities' id used in the presentation
		int _ninjaMalePresentationId;
		int _ninjaFemalePresentationId;

		// Score object Ids
		int _ninjaEnergyScoreObjectId;
		int _ninjaLivesScoreObjectId;
		int _ninjaThingsBeingCarriedScoreObjectId;
		int _ninjaThingBeingCarriedScoreObjectBaseId; // Up to _maxNumberOfThingsBeingCarried
		int _ninjaShootingsLeftScoreObjectId;
		int _ninjaTimeCounterScoreObjectId;
		int _ninjaPointsScoreObjectId;
		int _ninjaNameScoreObjectId;

		// Entities' id used in the game
		int _ninjaMaleId;
		int _ninjaFemaleId;
		int _beesBaseId;
		int _birdsBaseId;
		int _maxNumberOfFlyingMonstersPerFlock;
		int _flockOfBeesId;
		int _flockOfBirdsId;
		int _maxNumberOfFlocks;
		int _maxNumberOfBadGuysPerScene;
		int _shootingBaseId;
		int _maxNumberOfShootingsPerCharacter;
	
		// Sounds' id
		int _ninjaThrowingSoundId; // Including shooting...
		int _ninjaHitSoundId;
		int _ninjaJumpSoundId;
		int _ninjaCatchSoundId;
		int _ninjaActivateSwitchSoundId;
		int _ninjaEatSoundId;
		int _badGuyThrowingSoundId; // Including shooting...
		int _badGuyHitSoundId;
		int _ninjaDiesSoundId;
		int _beesFlyingSoundId;
		int _birdsFlyingSoundId;
		int _shieldSoundId;
		int _importantEventSoundId; // When e.g. a intermediate goal is reached...

		static DataGame* _theDataGame;
	};
}

#endif
  
// End of the file
/*@}*/
