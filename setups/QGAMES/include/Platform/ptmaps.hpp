/** \ingroup Platform */
/** \ingroup Strcuture */
/*@{*/

/**	
 *	@file	
 *	File: ptmaps.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 05/05/2018 \n
 *	Description: The maps often used in a platform game.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_PTMAPS__
#define __QGAMES_PTMAPS__

#include <Platform/ptdefinitions.hpp>
#include <Platform/ptlayers.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace QGAMES
{
	/** The map where the play happens. */
	class PlatformMap : public TiledMap
	{
		public:
		/** Defines the location of an object
			This structure is used when looking into the location layer. */
		struct ObjectLocation
		{
			ObjectLocation ()
				: _numberObject (-1),
				  // The same than the frame of the tile associated...
				  _position (Position::_noPoint),
				  _parameters () // None by default...
							{ }
			ObjectLocation (int nO, const Position& pos, const SetOfOpenValues& oV)
				: _numberObject (nO), 
				  // The same than the frame of the tile associated...
				  _position (pos),
				  _parameters (oV)
							{ }

			int _numberObject;
			Position _position;
			SetOfOpenValues _parameters;
		};

		typedef std::vector <ObjectLocation> ObjectLocations;

		/** Defines a block of notification.
			Usually is related with a block of similar tiles near each others. 
			When an entity collision with them a notification is issued. */
		struct NotificationBlockLocation
		{
			NotificationBlockLocation ()
				: _numberNotification (-1),
				  _positions ()
							{ }
			NotificationBlockLocation (int nN, const Positions& pos)
				: _numberNotification (nN),
				  _positions (pos)
							{ }

			int _numberNotification;
			Positions _positions;
		};

		typedef std::vector <NotificationBlockLocation> NotificationBlockLocations;

		PlatformMap (int c, const Layers& l, int w, int h, int tW, int tH,
			const MapProperties& p = MapProperties ());

		/** @see parent. */
		virtual Layers relevantLayers ();

		// Accessing specific type of layers...
		/** To get the location layer, if exists (or null). 
			It is supposed there must be only one layer of this type per map. If there is more, the first is taken. */
		LocationTileLayer* locationLayer ();
		const LocationTileLayer* locationLayer () const;
		/** To get the notification layer, if exists (or null).
			It is supposed there must be only one layer of this type per map. If there is more, the first is taken. */
		NotificationTileLayer* notificationLayer ();
		const NotificationTileLayer* notificationLayer () const;

		/** To find all positions of an object. 
			The number of object requested is the same that the number of the frame in the location layer. */
		ObjectLocations positionsForObject (int no) const;
		/** To find the location of several tiles. */
		ObjectLocations positionsForObject (const std::vector <int>& nO) const;
		/** To find the location of all objects. */
		ObjectLocations positionsForAllObjects () const;

		/** To find all positions of a notificaion block represented by a tile frame. */
		NotificationBlockLocations positionsForNotificationBlock (int no) const;
		/** To find the location of several tiles. */
		NotificationBlockLocations positionsForNotificationBlock (const std::vector <int>& nO) const;
		/** To find the location of all notification tiles. */
		NotificationBlockLocations positionsForAllNotificationBlocks () const;

		/** To find the properties associated to a specific position in the screen if any. */
		std::map <std::string, std::string> propertiesForPosition (const Position& pos) const;

		protected:
		/** To convert the tile locations into object positions. */
		ObjectLocations convertTileLocationsIntoObjetLocations (const LocationTileLayer::TileLocations& locs) const;
		/** To convert the tile locations into notification blocks. */
		NotificationBlockLocations convertTileLocationsIntoNotificationBlocks 
			(const NotificationTileLayer::TileLocations& locs) const;

		/** To get the zone object layer, if it exists.
			It is supposed there must be only one layer of this type per map. If there is more, the first is taken. */
		ZoneObjectLayer* zoneObjectLayer ();
		const ZoneObjectLayer* zoneObjectLayer () const;

		/** To get the position of a tile having its number. */
		Position positionForTileNumber (int pos) const;

		protected:
		// Implementation
		/** Once the list if calculated first, then is kept into this variable for performance reasons. */
		Layers _relevantLayers;
	};
}

#endif
  
// End of the file
/*@}*/