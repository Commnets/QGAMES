#include <Platform/ptlayers.hpp>

// ---
int QGAMES::IdentificationTileLayer::firstPositionForTile (int fr) const
{
	int result = -1;

	int counter = 0;
	for (QGAMES::Tiles::const_iterator i = tiles ().begin (); 
		i != tiles ().end () && result == -1; i++, counter++)
		if ((*i) != QGAMES::NullTile::_nullTile && (*i) -> numberFrame () == fr)
			result = counter; // The tiles are ordered: left, up, front to right, down, end.

	return (result);
}

// ---
QGAMES::IdentificationTileLayer::TileLocations QGAMES::IdentificationTileLayer::positionsForFrame (int fr) const
{
	std::vector <QGAMES::IdentificationTileLayer::TileLocation> result;

	int counter = 0;
	for (QGAMES::Tiles::const_iterator i = tiles ().begin (); 
		i != tiles ().end (); i++, counter++)
		if ((*i) != QGAMES::NullTile::_nullTile && (*i) -> numberFrame () == fr)
			result.push_back (QGAMES::IdentificationTileLayer::TileLocation (fr, counter, (*i)));

	return (result);
}

// ---
QGAMES::IdentificationTileLayer::TileLocations QGAMES::IdentificationTileLayer::positionsForFrame 
	(const std::vector <int>& fr) const
{
	std::vector <QGAMES::IdentificationTileLayer::TileLocation> result;

	int counter = 0;
	for (QGAMES::Tiles::const_iterator i = tiles ().begin (); 
		i != tiles ().end (); i++, counter++)
		if ((*i) != QGAMES::NullTile::_nullTile && 
			std::find (fr.begin (), fr.end (), (*i) -> numberFrame ()) != fr.end ())
			result.push_back (QGAMES::IdentificationTileLayer::TileLocation 
				((*i) -> numberFrame (), counter, (*i)));

	return (result);
}

// ---
QGAMES::IdentificationTileLayer::TileLocations QGAMES::IdentificationTileLayer::positionsForAllFrames () const
{
	std::vector <QGAMES::IdentificationTileLayer::TileLocation> result;

	int counter = 0;
	for (QGAMES::Tiles::const_iterator i = tiles ().begin (); 
		i != tiles ().end (); i++, counter++)
		if ((*i) != QGAMES::NullTile::_nullTile)
			result.push_back (QGAMES::IdentificationTileLayer::TileLocation 
				((*i) -> numberFrame (), counter, (*i)));

	return (result);
}

// ---
QGAMES::ZoneObjectLayer::ZoneLocation QGAMES::ZoneObjectLayer::firstZoneObjectFor (const QGAMES::Position& pos) const
{
	QGAMES::ZoneObjectLayer::ZoneLocation result;

	for (QGAMES::Objects::const_iterator i = objects ().begin (); 
		i != objects ().end () && result._id == -1; i++)
	{
		QGAMES::GraphicalPolygon* plg = dynamic_cast <QGAMES::GraphicalPolygon*> ((*i));
		if (plg && plg -> positions ().size () == 4 && // It has to be a rectangle...
			QGAMES::Rectangle (plg -> positions ()[0], plg -> positions ()[2]).hasIn (pos))
				result = QGAMES::ZoneObjectLayer::ZoneLocation (plg -> id (), plg -> properties (), 
					QGAMES::Rectangle (plg -> positions ()[0], plg -> positions ()[2]));
	}

	return (result);
}
