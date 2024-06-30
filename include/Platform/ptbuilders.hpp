/** \ingroup Platform */
/** \ingroup Strcuture */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: ptbuilders.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 05/05/2018 \n
 *	Description: The builders used to create the bsaic platform game.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_PTBUILDERS__
#define __QGAMES_PTBUILDERS__

#include <Platform/ptdefinitions.hpp>
#include <Platform/ptartists.hpp>
#include <Platform/ptscenes.hpp>
#include <Platform/ptworlds.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace QGAMES
{
	/** The entity builder now distinguishes among the basic things part of a platform game. 
		They can be things to eat, things to catch and bad guys. */
	class PlatformEntityBuilder : public AdvancedEntityBuilder
	{
		public:
		PlatformEntityBuilder (const std::string& eDef, FormBuilder* fB, MovementBuilder* mB)
			: AdvancedEntityBuilder (eDef, fB, mB)
							{ }

		protected:
		/** @see parent. */
		virtual Entity* createEntity (const EntityDefinition& def);

		/** The constructors to create different things that are part of the game. 
			These constructors could be overloaded for a specific game. 
			The type of instances that they create are the very basic ones. */
		virtual SomethingToEat* createSomethingToEat (const EntityDefinition& def) = 0;
		virtual SomethingToCatch* createSomethingToCatch (const EntityDefinition& def) = 0;
		virtual BadGuy* createBadGuy (const EntityDefinition& def) = 0;
	};

	/** Like a facade to define information related with the tiles being part of the TMX map definition. 
		It can be extended later to consider more and more information.
		By default, the structure defines which tile frames in the location and notification layers of a Platform Map
		are used to identifiy the initial positions of things to eat, things to catch, bad guys o exist doors. */
	struct ToKnowTileFrameInformation
	{
		public:
		ToKnowTileFrameInformation ()
			: _thingsToEatLocationTileFrames (),
			  _thingsToCatchLocationTileFrames (),
			  _badGuysLocationTileFrames (),
			  _doorsNotificationBlockTileFrames ()
							{ }
		virtual ~ToKnowTileFrameInformation () // Just in case...
							{ }

		/** Methods to know whether a tile id is or not includedin one of the lists */
		bool isTileFrameForEatLocation (int tF) const
							{ return (std::find (_thingsToEatLocationTileFrames.begin (), 
								_thingsToEatLocationTileFrames.end (), tF) != _thingsToEatLocationTileFrames.end ()); }
		bool isTileFrameForThingLocation (int tF) const
							{ return (std::find (_thingsToCatchLocationTileFrames.begin (), 
								_thingsToCatchLocationTileFrames.end (), tF) != _thingsToCatchLocationTileFrames.end ()); }
		bool isTileFrameBadGuyLocation (int tF) const
							{ return (std::find (_badGuysLocationTileFrames.begin (), 
								_badGuysLocationTileFrames.end (), tF) != _badGuysLocationTileFrames.end ()); }
		bool isTileDoorsLocation (int tF) const
							{ return (std::find (_doorsNotificationBlockTileFrames.begin (), 
								_doorsNotificationBlockTileFrames.end (), tF) != _doorsNotificationBlockTileFrames.end ()); }

		/** List of the tiles used to identify things to eat in the locationLayer. */
		std::vector <int> _thingsToEatLocationTileFrames;
		/** Same for things to catch. */
		std::vector <int> _thingsToCatchLocationTileFrames;
		/** Same for bad guys. */
		std::vector <int> _badGuysLocationTileFrames;
		/** To get the list used to identify door notification blocks in the notification layer. */
		std::vector <int> _doorsNotificationBlockTileFrames;
	};

	/** The maps built using this add on (TMX standard) are always 2D/2.5D tiled maps.
		A map is made up of different layers. What each layer is is basically defined by its name. 
		So, if a layer contains "Base", it will be a "BaseLayer", 
		made up of "BaseTiles" except those excluded. @see next. */
	class PlatformTMXMapBuilder : public TMXMapBuilderAddsOn
	{
		public:
		PlatformTMXMapBuilder (Sprite2DBuilder* sB)
			: TMXMapBuilderAddsOn (sB),
			  _layerCounters (),
			  _baseExcludedTileSet (), _solidExcludedTileSet (), 
			  _ladderExcludedTileSet (), _sliddingExcludedTileSet (), 
			  _liquidExcludedTileSet (), _killingExcludedTileSet (), 
			  _notificationExcludedTileSet (), _locationExcludedTileSet (),
			  _tileFrameInformation (NULL)
							{ }

		~PlatformTMXMapBuilder ()
							{ delete (_tileFrameInformation); }

		/** To determinate what are the tile ids to be consider before creating
			a specific type of tiles and then layers. 
			The tile form will determinate exactly the type of tile unless it has been explicitly excluded. 
			By default none is excluded. */
		void setBaseTilesExcluded (const std::vector <int>& bT)
							{ _baseExcludedTileSet = bT; }
		void setSolidTilesExcluded (const std::vector <int>& bT)
							{ _solidExcludedTileSet = bT; }
		void setLadderTilesExcluded (const std::vector <int>& lT)
							{ _ladderExcludedTileSet = lT; }
		void setSliddingTilesExcluded (const std::vector <int>& lT)
							{ _sliddingExcludedTileSet = lT; }
		void setLiquidTilesExcluded (const std::vector <int>& lT)
							{ _liquidExcludedTileSet = lT; }
		void setKillingTilesExcluded (const std::vector <int>& kT)
							{ _killingExcludedTileSet = kT; }
		void setNotificationTilesExcluded (const std::vector <int>& nT)
							{ _notificationExcludedTileSet = nT; }
		void setLocationTilesExcluded (const std::vector <int>& lT)
							{ _locationExcludedTileSet = lT; }

		/** To get the aside (addon) used to get information about the tile frames used in the TMX Map definition. */
		const ToKnowTileFrameInformation* tileFrameInformation () const
							{ return ((const ToKnowTileFrameInformation*) 
								((PlatformTMXMapBuilder*) this) -> tileFrameInformation ()); }
		/** It can be oerloaded later to include more information about tiles. */
		ToKnowTileFrameInformation* tileFrameInformation () // THis is the one to overload!!
							{ return (_tileFrameInformation 
								? _tileFrameInformation : (_tileFrameInformation = createTileFrameInformationObject ())); }

		/** @see parent. */
		virtual void afterCreatingMap (QGAMES::Map* mp);

		protected:
		/** To determinate which type of tiles are contained by a form (given it name).
			The basic types are given by this method, but it can be extended with new ones. */
		virtual int tileTypeInForm (const std::string& frm, int nf);
		/** To determinate whether which type of layer is the onbe received as parameter.
			The basic layers are determinated by this method, but it can be extended with new ones. */
		virtual int layerType (const std::string& lName);

		/** @see parent. 
			Look at pttiles.hpp. */
		virtual Tile* createTile (int id, Form* form, int nf, 
			const TileProperties& p = TileProperties ());
		virtual TileLayer* createTileLayer (int id, const std::string& n, 
			const Tiles& t, Map* m, TileLayer::VisualOrientation o, 
			const LayerProperties& p = LayerProperties ());
		virtual ObjectLayer* createObjectLayer (int id, const std::string& n, const Objects&, 
			Map* m, const LayerProperties& oP = LayerProperties());
		// By default none is created...
		virtual ImageLayer* createImageLayer (int id, const std::string& n, Image* i, 
			Map* m, const LayerProperties& oP = LayerProperties ());
		// By default a basic PlatformMap is created...
		virtual Map* createMapObject (int id, const Layers& l, int w, int h, int d, 
			int tW, int tH, int tD, const MapProperties& p = MapProperties ());
		// The very basic tile information object is created.
		virtual ToKnowTileFrameInformation* createTileFrameInformationObject ()
							{ return (new ToKnowTileFrameInformation ()); }

		/** @see parent. */
		virtual void beforeCreatingMap (const MapBuilder::MapDefinition& def)			
							{ _layerCounters = std::vector <int> (); }

		protected:
		// Implementation
		std::vector <int> _layerCounters,
						  _baseExcludedTileSet, _solidExcludedTileSet, 
						  _ladderExcludedTileSet, _sliddingExcludedTileSet, 
						  _liquidExcludedTileSet, _killingExcludedTileSet, 
						  _notificationExcludedTileSet, _locationExcludedTileSet;
		ToKnowTileFrameInformation* _tileFrameInformation; // Temporal...
	};

	/** To create worlds and scenes. */
	class PlatformWorldBuilder : public WorldBuilder
	{
		public:
		PlatformWorldBuilder (const std::string& eDef, MapBuilder* mB)
			: WorldBuilder (eDef, mB)
							{ }

		protected:
		/** @see parent. 
			The simpliest version of the objetcs is created here.
			Usually those two methods would have to be changed in further versiones. */
		virtual World* createWorldObject (int no, const Scenes& s, const WorldProperties& p)
							{ return (new PlatformWorld (no, s, p)); }
		virtual Scene* createSceneObject (int ns, const Maps& m, const Scene::Connections& cn, 
			const SceneProperties& p, const EntitiesPerLayer& ePL)
							{ return (new PlatformScene (ns, m, cn, p, ePL)); }
	};
}

#endif
  
// End of the file
/*@}*/