/** \ingroup Battleship */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: bsdatagame.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/11/2019 \n
 *	Description: Define an object with all things that can be configurated for this type of games. \n
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_DATAGAME__
#define __BATTLESHIP_DATAGAME__

#include <AdvancedArcade/adinclude.hpp>

namespace BATTLESHIP
{
	/** Singleton.
		This type of game can be reused for many other different similar platform games. 
		So it is necessary an object to keep configurable things such as the entities being involved, sounds used, ...
		In any case, a default value is given representing the standard Battleship like game. 
		An object of this class is created when the Battleship game class is created. (@see Game class) 
		It is an structure just to simplify the access to the internal attributes. 
		The internal values can be created just after the initialization. 
		This structure can be extended. It is instantiated in the constructor of the Game class.
		If you extend this class you should also extend Game class to instantiate the new DataGame. */
	struct DataGame
	{
		public:
		DataGame ();
		virtual ~DataGame ();

		// General attributes
		int _numberOfLives;
		int _numberOfThingsBeingCarried;
		int _numberOfPowerLevels;
		int _numberOfEnergyElementsTypes;
		int _numberOfShootingPillTypes;
		int _numberOfThingsToCatchTypes;
		int _numberOfShootingTypes;
		int _numberOfAsteroidTypes;
		int _numberOfUFOTypes;

		// Parameters related with the game
		int _maxNumberOfAsteroidsInScene;
		int _maxNumberOfShootingsPerEnemyInScene;
		int _maxNumberOfEnemiesInScene;
		int _maxNumberOfShootingsInScene;
		int _maxNumberOfEnergyElementsInScene;
		int _maxNumberOfThingsToCatchInScene;

		// Related with forms used to represent things to eat and things that can be carried!
		int _livesFormId;
		int _thingsToCarryFormId;
		int _numberOfThingsBeingCarriedTypes;

		// Sounds
		int _spaceshipFiringSoundId;
		int _battleshipGetSoundId;
		int _battleshipLoadEnergySoundId;
		int _spaceshipExplodingSoundId;
		int _spaceshipHitsAnotherElmntSoundId;

		// Score object Ids
		int _spaceElementEnergyScoreObjectId;
		int _battleshipLivesScoreObjectId;
		int _battleshipThingsBeingCarriedScoreObjectId;
		int _battleshipThingBeingCarriedScoreObjectBaseId; // Up to _maxNumberOfThingsBeingCarried
		int _spaceElementShootingsLeftScoreObjectId;
		int _battleshipTimeCounterScoreObjectId;
		int _battleshipPointsScoreObjectId;
		int _battleshipNameScoreObjectId;

		// Entities' id used in the game
		int _battleshipType1Id;
		int _battleshipType2Id;

		/** Data related with the power of shooting...
		  * It is an array with _numberOfPowerLevels in the lines
		  * and 5 different values meaning:
		  * 0: Type of shooting: From 0 up to _numberOfShootingTypes - 1
		  * 1: NUmber of maximum shootings in the scene at the same time
		  * 2: Max number of shootings (the total can't ever be greater than 1000)
		  * 3: Number of simultaneously shootings
		  * 4: The energy of every shooting
		  */
		int** _LEVELDATA;

		/** To indicate whether the things going around the spaceship are caught just passing over them or 
			a click is needed instead. It is false by default. */
		bool _clickNeededToCatchThings;

		// Configuration data to manage the different types of UFO
		/** To get the parameters that define the aspect of the ufo acoording with the type. */
		virtual std::vector <int> defaultStatesForUFOType (int uT) const;

		/** To get the list off all UFO types valid.
			The number of elements of the list should be the same that the number of tyes of ufos (@see attribute above). */
		virtual inline const std::vector <int>& ufoTypesList () const;

		// Configuration data for the different movements in the game
		/** Default values for the curves in the game. 
			@see defaultParametersForMovementType for the meaning of the parameters. */
		virtual QGAMES::Position defaultInitialPointForMovementType (int nC, 
			const QGAMES::Rectangle& vZ, const QGAMES::Rectangle& mZ);
	
		/** 
		  *	Default values for the curves of the game
		  *	It can be overloaded later if needed...
		  *	@param nC :	Id for the curve which default parameters are needed.
		  *	@paran sF :	Speed factor (it must be providded cause it can depend of the difficulty level.
		  *	@param vZ :	Visual zone, need to calculate the limis of some movements.
		  *	@param mZ :	Movement zone, also needed for the same reason.
		  *	@param iP :	Initial position of the curve. Needed in some cases.
		  * @param tR :	Angle to rotate the curve, if needed (some cases)	
		  */
		virtual std::map <int, double> defaultParametersForMovementType (int nC, 
			QGAMES::bdata sF, const QGAMES::Rectangle& vZ, const QGAMES::Rectangle& mZ, const QGAMES::Position& iP, 
			QGAMES::bdata& tR); // This last value is returned...

		/** To get the map of the curves associated to the movements defined in the game.
			This list can be incremented later. */
		virtual inline const std::map <int, int>& curveforMovementTypeMap () const;

		static DataGame* _theDataGame;

		protected:
		// Implementation
		/** Just to get speed. */
		mutable std::vector <int> _ufoTypesList;
		mutable std::map <int, int> _curveForMovementTypeMap; 
		// Both are filled up first time either ufoTypesList or curveforMovementTypeMap methods are invoked!
		// Because those method have been declared as const, the variables have to be mutable!
	};
}

#endif
  
// End of the file
/*@}*/
