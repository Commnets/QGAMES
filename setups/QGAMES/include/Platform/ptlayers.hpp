/** \ingroup Platform */
/** \ingroup Strcuture */
/*@{*/

/**	
 *	@file	
 *	File: ptlayers.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 06/04/2018 \n
 *	Description: All common type of layers in a platform game are defined.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_PTLAYERS__
#define __QGAMES_PTLAYERS__

#include <Platform/ptdefinitions.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace QGAMES
{
	/** When the layer is just full of tiles where the entities can stand on. */
	class PlatformTileLayer : public AdvancedTileLayer
	{
		public:
		PlatformTileLayer (int c, const std::string& n, const Tiles& t, 
			Map* m, TileLayer::VisualOrientation o,
			const LayerProperties p = LayerProperties (), bool oT = false)
			: AdvancedTileLayer (c, n, t, m, o, p, oT)
							{ } // Visible by default...
	};

	/** When the layer is just full of tiles that the entities collision with. */
	class SolidTileLayer : public AdvancedTileLayer
	{
		public:
		SolidTileLayer (int c, const std::string& n, const Tiles& t, 
			Map* m, TileLayer::VisualOrientation o,
			const LayerProperties p = LayerProperties (), bool oT = false)
			: AdvancedTileLayer (c, n, t, m, o, p, oT)
							{ } // Visible by default...
	};

	/** When the layer is just full of tiles where the entities can go up & down. */
	class LadderTileLayer : public AdvancedTileLayer
	{
		public:
		LadderTileLayer (int c, const std::string& n, const Tiles& t, 
			Map* m, TileLayer::VisualOrientation o,
			const LayerProperties p = LayerProperties (), bool oT = false)
			: AdvancedTileLayer (c, n, t, m, o, p, oT)
							{ } // Visible by default...
	};

	/** Represents a liquid layer. 
		The water in the liquid can have several effects.
		Depending on the effect an entity on it will move in a way or another. */
	class LiquidTileLayer : public AdvancedTileLayer
	{
		public:
		LiquidTileLayer (int c, const std::string& n, const Tiles& t, 
			Map* m, TileLayer::VisualOrientation o,
			const LayerProperties p = LayerProperties (), bool oT = false)
				: AdvancedTileLayer (c, n, t, m, o, p, oT)
							{ }
	};

	/** When the layer seems to slide.
		Depending on the effect an entity in it will move in a way or another. */
	class SliddingTileLayer : public AdvancedTileLayer
	{
		public:
		SliddingTileLayer (int c, const std::string& n, const Tiles& t, 
			Map* m, TileLayer::VisualOrientation o,
			const LayerProperties p = LayerProperties (), bool oT = false)
			: AdvancedTileLayer (c, n, t, m, o, p, oT)
							{ }
	};

	/** When the layer is just full of tiles able to kill the entities. */
	class KillingTileLayer : public AdvancedTileLayer
	{
		public:
		KillingTileLayer (int c, const std::string& n, const Tiles& t, 
			Map* m, TileLayer::VisualOrientation o,
			const LayerProperties p = LayerProperties (), bool oT = false)
			: AdvancedTileLayer (c, n, t, m, o, p, oT)
							{ } // Visible by default...
	};

	/** When the layer is just full of tiles used to decorate. */
	class DecorationTileLayer : public AdvancedTileLayer
	{
		public:
		DecorationTileLayer (int c, const std::string& n, const Tiles& t, 
			Map* m, TileLayer::VisualOrientation o,
			const LayerProperties p = LayerProperties (), bool oT = false)
			: AdvancedTileLayer (c, n, t, m, o, p, oT)
							{ } // Visible by default...
	};

	/** This layer is usually not visible and it is just to indicate things to the game. 
		There can be several of those. By default two, notification and location, are defined. */
	class IdentificationTileLayer : public AdvancedTileLayer
	{
		public:
		/** An structure to identify where a tile is. */
		struct TileLocation
		{
			TileLocation ()
				: _numberFrame (-1), _position (-1), _tile (NULL)
							{ }
			TileLocation (int nF, int p, Tile* t)
				: _numberFrame (nF), _position (p), _tile (t)
							{ }

			int _numberFrame;
			int _position;
			Tile* _tile;
		};

		typedef std::vector <TileLocation> TileLocations;

		IdentificationTileLayer (int c, const std::string& n, const Tiles& t, 
			Map* m, TileLayer::VisualOrientation o,
			const LayerProperties p = LayerProperties (), bool oT = false)
			: AdvancedTileLayer (c, n, t, m, o, p, oT)
							{ setVisible (false); }

		/** To find the position of a specific frame. */
		int firstPositionForTile (int fr) const;
		/** To find all positions of a frame. */
		TileLocations positionsForFrame (int fr) const;
		/** To find the location of several tiles. */
		TileLocations positionsForFrame (const std::vector <int>& fr) const;
		/** To find the location of all tiles not null. */
		TileLocations positionsForAllFrames () const;
	};

	/** A layer with tiles that notifies events when an entity collision with them. */
	class NotificationTileLayer : public IdentificationTileLayer
	{
		public:
		NotificationTileLayer (int c, const std::string& n, const Tiles& t, 
			Map* m, TileLayer::VisualOrientation o,
			const LayerProperties p = LayerProperties (), bool oT = false)
			: IdentificationTileLayer (c, n, t, m, o, p, oT)
							{ }
	};

	/** Same but with tiles indicating the position of things. */
	class LocationTileLayer : public IdentificationTileLayer
	{
		public:
		LocationTileLayer (int c, const std::string& n, const Tiles& t, 
			Map* m, TileLayer::VisualOrientation o,
			const LayerProperties p = LayerProperties (), bool oT = false)
			: IdentificationTileLayer (c, n, t, m, o, p, oT)
							{ }
	};

	/** One layer to identify different zones of a tiled map. 
		This type of layer is used to mark up zones in the map. 
		A set of attributes can be applied to that zone and used later in the game to different puposes. */
	class ZoneObjectLayer : public ObjectLayer
	{
		public:
		/** An structure to identify where a zone is. */
		struct ZoneLocation
		{
			ZoneLocation ()
				: _id (-1),
				  _properties (),
				  _size (Rectangle::_noRectangle)
							{ }

			ZoneLocation (int id, const std::map <std::string, std::string>& prps, const Rectangle& r)
				: _id (id),
				  _properties (prps),
				  _size (r)
							{ }

			int _id;
			std::map <std::string, std::string> _properties;
			Rectangle _size;
		};

		typedef std::vector <ZoneLocation> ZoneLocations;

		ZoneObjectLayer (int c, const std::string& n, const Objects& o, Map* m = NULL,
				const LayerProperties& p = LayerProperties ())
			: ObjectLayer (c, n, o, m, p)
							{ setVisible (false); }

		/** To find the first zone with a certain position inside, if any. */
		ZoneLocation firstZoneObjectFor (const Position& pos) const;
	};
}

#endif
  
// End of the file
/*@}*/