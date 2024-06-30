#include <Platform/ptmaps.hpp>
#include <Platform/ptlayers.hpp>
#include <Platform/pttiles.hpp>

// ---
QGAMES::PlatformMap::PlatformMap (int c, const QGAMES::Layers& l, int w, int h, int tW, int tH,
			const QGAMES::MapProperties& p)
			: QGAMES::TiledMap (c, l, w, h, 0, tW, tH, 0, p),
			  _relevantLayers ()
{
	for (QGAMES::Layers::const_iterator i = _layers.begin (); i != _layers.end (); i++)
		assert (dynamic_cast <QGAMES::PlatformTileLayer*> ((*i)) || 
				dynamic_cast <QGAMES::SolidTileLayer*> ((*i)) ||
				dynamic_cast <QGAMES::LadderTileLayer*> ((*i)) ||
				dynamic_cast <QGAMES::LiquidTileLayer*> ((*i)) ||
				dynamic_cast <QGAMES::SliddingTileLayer*> ((*i)) ||
				dynamic_cast <QGAMES::KillingTileLayer*> ((*i)) ||
				dynamic_cast <QGAMES::DecorationTileLayer*> ((*i)) ||
				dynamic_cast <QGAMES::NotificationTileLayer*> ((*i)) ||
				dynamic_cast <QGAMES::LocationTileLayer*> ((*i)) ||
				dynamic_cast <QGAMES::ZoneObjectLayer*> ((*i)));
	// To be sure all layers belong to the right type...
}

// ---
QGAMES::Layers QGAMES::PlatformMap::relevantLayers ()
{
	if (_relevantLayers.empty ())
	{
		const QGAMES::Layers& l = layers (); // To avoid copies!!
		for (QGAMES::Layers::const_iterator i = l.begin (); i != l.end (); i++)
			if (dynamic_cast <QGAMES::NotificationTileLayer*> ((*i)) ||
				dynamic_cast <QGAMES::PlatformTileLayer*> ((*i)) || 
				dynamic_cast <QGAMES::SolidTileLayer*> ((*i)) || 
				dynamic_cast <QGAMES::LadderTileLayer*> ((*i)) || 
				dynamic_cast <QGAMES::SliddingTileLayer*> ((*i)) || 
				dynamic_cast <QGAMES::LiquidTileLayer*> ((*i)) || 
				dynamic_cast <QGAMES::KillingTileLayer*> ((*i)))
					_relevantLayers.push_back ((*i));
	}

	return (_relevantLayers);
}

// ---
QGAMES::LocationTileLayer* QGAMES::PlatformMap::locationLayer ()
{
	return ((QGAMES::LocationTileLayer*) ((const QGAMES::PlatformMap*) this) -> locationLayer ());
}

// ---
const QGAMES::LocationTileLayer* QGAMES::PlatformMap::locationLayer () const
{
	QGAMES::LocationTileLayer* result = NULL;
	const QGAMES::Layers& l = layers (); // To avoid copies!!
	for (QGAMES::Layers::const_iterator i = l.begin (); i != l.end () && !result; i++)
		if (dynamic_cast <QGAMES::LocationTileLayer*> ((*i)))
			result = ((QGAMES::LocationTileLayer*) (*i));
	return (result);
}

// ---
QGAMES::NotificationTileLayer* QGAMES::PlatformMap::notificationLayer ()
{
	return ((QGAMES::NotificationTileLayer*) ((const QGAMES::PlatformMap*) this) -> notificationLayer ());
}

// ---
const QGAMES::NotificationTileLayer* QGAMES::PlatformMap::notificationLayer () const
{
	QGAMES::NotificationTileLayer* result = NULL;
	const QGAMES::Layers& l = layers (); // To avoid copies!!
	for (QGAMES::Layers::const_iterator i = l.begin (); i != l.end () && !result; i++)
		if (dynamic_cast <QGAMES::NotificationTileLayer*> ((*i)))
			result = ((QGAMES::NotificationTileLayer*) (*i));
	return (result);
}

// ---
QGAMES::PlatformMap::ObjectLocations QGAMES::PlatformMap::positionsForObject (int nO) const
{ 
	const QGAMES::LocationTileLayer* lY = locationLayer ();
	QGAMES::LocationTileLayer::TileLocations locs = (lY != NULL) 
		? lY -> positionsForFrame (nO) : QGAMES::LocationTileLayer::TileLocations ();
	return (convertTileLocationsIntoObjetLocations (locs));
}

// ---
QGAMES::PlatformMap::ObjectLocations QGAMES::PlatformMap::positionsForObject (const std::vector <int>& nO) const
{ 
	const QGAMES::LocationTileLayer* lY = locationLayer (); 
	QGAMES::LocationTileLayer::TileLocations locs = (lY != NULL) 
		? lY -> positionsForFrame (nO) : QGAMES::LocationTileLayer::TileLocations ();
	return (convertTileLocationsIntoObjetLocations (locs));
}

// ---
QGAMES::PlatformMap::ObjectLocations QGAMES::PlatformMap::positionsForAllObjects () const
{ 
	const QGAMES::LocationTileLayer* lY = locationLayer (); 
	QGAMES::LocationTileLayer::TileLocations locs = (lY != NULL) 
		? lY -> positionsForAllFrames () : QGAMES::LocationTileLayer::TileLocations ();
	return (convertTileLocationsIntoObjetLocations (locs));
}

// ---
QGAMES::PlatformMap::NotificationBlockLocations QGAMES::PlatformMap::positionsForNotificationBlock (int nO) const
{ 
	const QGAMES::NotificationTileLayer* lY = notificationLayer (); 
	QGAMES::NotificationTileLayer::TileLocations locs = (lY != NULL) 
		? lY -> positionsForFrame (nO) : QGAMES::LocationTileLayer::TileLocations ();
	return (convertTileLocationsIntoNotificationBlocks (locs));
}

// ---
QGAMES::PlatformMap::NotificationBlockLocations QGAMES::PlatformMap::positionsForNotificationBlock (const std::vector <int>& nO) const
{ 
	const QGAMES::NotificationTileLayer* lY = notificationLayer (); 
	QGAMES::NotificationTileLayer::TileLocations locs = (lY != NULL) 
		? lY -> positionsForFrame (nO) : QGAMES::NotificationTileLayer::TileLocations ();
	return (convertTileLocationsIntoNotificationBlocks (locs));
}

// ---
QGAMES::PlatformMap::NotificationBlockLocations QGAMES::PlatformMap::positionsForAllNotificationBlocks () const
{ 
	const QGAMES::NotificationTileLayer* lY = notificationLayer (); 
	QGAMES::NotificationTileLayer::TileLocations locs = (lY != NULL) 
		? lY -> positionsForAllFrames () : QGAMES::NotificationTileLayer::TileLocations ();
	return (convertTileLocationsIntoNotificationBlocks (locs));
}

// ---
std::map <std::string, std::string> QGAMES::PlatformMap::propertiesForPosition (const QGAMES::Position& pos) const
{
	const QGAMES::ZoneObjectLayer* zL = zoneObjectLayer ();
	if (zL) return (zL -> firstZoneObjectFor (pos)._properties);
	else return (std::map <std::string, std::string> ());
}

// ---
QGAMES::PlatformMap::ObjectLocations QGAMES::PlatformMap::convertTileLocationsIntoObjetLocations 
	(const LocationTileLayer::TileLocations& locs) const
{
	QGAMES::PlatformMap::ObjectLocations result;
	for (QGAMES::LocationTileLayer::TileLocations::const_iterator i = locs.begin (); i != locs.end (); i++)
		result.push_back (QGAMES::PlatformMap::ObjectLocation 
			((*i)._numberFrame, positionForTileNumber ((*i)._position), 
				dynamic_cast <QGAMES::InformedTile*> ((*i)._tile) 
					? ((QGAMES::InformedTile*) (*i)._tile) -> parameters () 
					: QGAMES::SetOfOpenValues ())); // If the tile has additional information it is included...
	return (result); 
}

// ---
QGAMES::PlatformMap::NotificationBlockLocations 
	QGAMES::PlatformMap::convertTileLocationsIntoNotificationBlocks 
		(const QGAMES::NotificationTileLayer::TileLocations& locs) const
{
	QGAMES::PlatformMap::NotificationBlockLocations result;
	for (QGAMES::NotificationTileLayer::TileLocations::const_iterator i = locs.begin (); i != locs.end (); i++)
	{
		bool found = false;
		QGAMES::Positions pos;
		QGAMES::PlatformMap::NotificationBlockLocation nB ((*i)._numberFrame, pos);
		for (QGAMES::PlatformMap::NotificationBlockLocations::iterator j = result.begin (); j != result.end () && !found; j++)
		{
			if ((*j)._numberNotification == nB._numberNotification)
			{
				nB._positions = (*j)._positions; pos.push_back (positionForTileNumber ((*i)._position));
				result.erase (j);
				found = true;
			}

			result.push_back (nB); // All notifiction tiles with identical id are grouped in the same notification block...
		}
	}

	return (result); 
}

// ---
QGAMES::ZoneObjectLayer* QGAMES::PlatformMap::zoneObjectLayer ()
{
	return ((QGAMES::ZoneObjectLayer*) ((const QGAMES::PlatformMap*) this) -> zoneObjectLayer ());
}

// ---
const QGAMES::ZoneObjectLayer* QGAMES::PlatformMap::zoneObjectLayer () const
{
	QGAMES::ZoneObjectLayer* result = NULL;
	const QGAMES::Layers& l = layers (); // To avoid copies!!
	for (QGAMES::Layers::const_iterator i = l.begin (); i != l.end () && !result; i++)
		if (dynamic_cast <QGAMES::ZoneObjectLayer*> ((*i)))
			result = ((QGAMES::ZoneObjectLayer*) (*i));
	return (result);
}

// ---
QGAMES::Position QGAMES::PlatformMap::positionForTileNumber (int pos) const
{
	int tWidth = visualTileWidth ();
	int tHeight = visualTileHeight ();
	int tDepth = visualTileDepth ();
	assert (tWidth != 0 && tHeight != 0); 
	int mWidth = width () / tWidth;
	int mHeight = height () / tHeight; 
	int mDepth = (tDepth == 0) ? 1 : depth () / tDepth;

	return (QGAMES::Position (__BD (((pos / mDepth) % mWidth) * tWidth), 
							  __BD (((pos / mDepth) / mWidth) * tHeight), 
							  __BD ((pos % mDepth) * tDepth)));
}
