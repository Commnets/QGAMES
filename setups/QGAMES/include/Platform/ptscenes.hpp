/** \ingroup Platform */
/** \ingroup Strcuture */
/*@{*/

/**	
 *	@file	
 *	File: ptscenes.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 31/12/2018 \n
 *	Description: The typical scenes used in the platform games.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_PTSCENES__
#define __QGAMES_PTSCENES__

#include <Platform/ptdefinitions.hpp>
#include <Platform/ptgame.hpp>
#include <Platform/ptmaps.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace QGAMES
{
	/** A platform scene is where the game usually takes place. 
		The patform scene reuses the characterss. */
	class PlatformScene : public SceneWithBackground
	{
		public:
		PlatformScene (int c, const Maps& m, const Connections& cn = Connections (), 
			const SceneProperties& p = SceneProperties (), 
			const EntitiesPerLayer& ePL = EntitiesPerLayer ());

		/** To get the bad guys. */
		const Entities& badGuys () const
							{ return (_badGuys); }

		/** To know and set the state of a door. 
			A door is a point of communication among scenes. 
			What this door communicates to is defined by the connections list. 
			This method is just something in the middle to check whether the door is opened or not. 
			In the case of a door state = 1 means opened, and 0 closed. */
		void setDoorOpen (int id, bool s);
		bool doorOpen (int id) const;

		/** @see parent. */
		virtual void initialize ();
		virtual void finalize ();

		/** To get all things to eat definition (default). 
			The number of the frames representing an eating things is defined in PlatformTMXMapBuilder class. 
			@see PlatformTMXMapBuilder. 
			If nothing is defined there, no eating will be loaded!
			Same with the rest. */
		PlatformGame::ThingsToEatDefinition thingsToEatDefinition () const;
		/** To get all things to catch definition (default). */
		PlatformGame::ThingsToCatchDefinition thingsToCatchDefinition () const;
		/** To get all bad guys definition (default). 
			In the setOfValues number 0 of the variable _otherValues under the definition
			the additional information found in the TMX map is also stored. */
		PlatformGame::BadGuysDefinition badGuysDefinition () const;
		/** To get all doors definition (default). */
		PlatformGame::ExitDoorsDefinition exitDoorsDefinition () const;

		/** To get the location of a certain objects in the scene (in the location layer) */
		PlatformMap::ObjectLocations positionsForObject (int nO) const;
		PlatformMap::ObjectLocations positionsForObject (const std::vector <int>& nO) const;

		/** To get the location of a certain notification block in the scene (in the notification layer) */
		PlatformMap::NotificationBlockLocations positionsForNotificationBlock (int nO) const;
		PlatformMap::NotificationBlockLocations positionsForNotificationBlock (const std::vector <int>& nO) const;

		protected:
		/** To create the definition from the location object.
			It can be redefined later, but a defualt implementation is provided as well. */
		virtual PlatformGame::ThingToEatDefinition* createThingToEatDefinitionFrom 
				(const PlatformMap::ObjectLocation& obj) const;
		virtual PlatformGame::ThingToCatchDefinition* createThingToCatchDefinitionFrom 
				(const PlatformMap::ObjectLocation& obj) const;
		virtual PlatformGame::BadGuyDefinition* createBadGuyDefinitionFrom 
				(const PlatformMap::ObjectLocation& obj) const;
		virtual PlatformGame::ExitDoorDefinition* createExitDoorDefinitionFrom 
				(const PlatformMap::NotificationBlockLocation& obj) const;

		protected:
		/** A reference to the doors belonging to this scene. */
		PlatformGame::ExitDoorsDefinition _exitDoors;

		// Implementation
		int _numberOfThingsToEatAdded; // For further references...
		int _numberOfThingsToCatchAdded;
		int _numberOfBadGuysAdded;
		Entities _characteresAdded;
		Entities _badGuys;
	};
}

#endif

// End of the file
/*@}*/