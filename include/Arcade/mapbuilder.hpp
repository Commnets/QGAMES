/** \ingroup ArcadeGame */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: mapbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class to build a map. \n
 *				 An special class to build up tmx maps is also defined. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_MAPBUILDER__
#define __QGAMES_MAPBUILDER__

#include <Common/formbuilder.hpp>
#include <Common/objectbuilder.hpp>
#include <Common/elementbuilder.hpp>
#include <Arcade/map.hpp>
#include <tinyxml.h>
#include <Tmx.h>
#include <map>
#include <vector>
#include <string>

namespace QGAMES
{
	/** 
	  *	A map builder is a builder aimed to create maps (@see Map class). \n
	  *	There can be many different types of maps: Object maps, tiled map, etc...
	  *	and additionaly every type of map can be described in many different ways, so the map builder
	  *	has to be based on adds-on to understand any different possibility. \n
	  *	The map builder owns the adds-on that it manages. \n
	  * The two important variables per map definition are a type and a reference to another file. \n
	  * The type will determine the add-on to be used to create the specific type of map,
	  * and the file is the file where the detail description of the map (and it depends on it) is contained. 
	  * It is not the same the description for an object map tan for a TMX 2D tiled map.
	  * The map builder owns all maps created from it, so when it is destroyed the maps are also deleted.
	  */
	class MapBuilder : public ElementBuilder 
	{
		public:
		/** The structure defining a map. */
		struct MapDefinition
		{
			typedef enum { _SIMPLE, _SET } TypeBlock;

			MapDefinition ()
				: _id (-1),
				  _number (1),
				  _typeBlock (_SIMPLE),
				  _type (-1),
				  _definition (__NULL_STRING__)
							{ }

			int _id;
			int _number;
			TypeBlock _typeBlock;
			int _type;
			std::string _definition;
		};

		/** An add-on is an interface to create a specific type of map defined in a specific way. \n */
		class AddsOn
		{
			public:
			AddsOn ()
							{ }
			virtual ~AddsOn () 
							{ } // Just in case

			// Methods to create a map...
			/** Returns true when the addson is able to create a type of map. */
			virtual bool canCreateMap (int t) const = 0;
			/** Creates a map from a type from the definition structure. \n
				If the adds on is not able to create a type of map, it crashes. */
			virtual Map* createMap (const MapDefinition& def,
				const std::string& bP) = 0;

			// Before and after creating the map
			virtual void beforeCreatingMap (const MapDefinition& def)
							{ } // By default nothing is done
			virtual void afterCreatingMap (Map* mp)
							{ } // Bu default nothing is done
		};

		/** A typedef to simplify the access to the addsOn elements. */
		typedef std::list <AddsOn*> AddsOnList;

		/** 
		  *	The structure of a map definition file is as follows: \n
		  *	Where the type of the map defines the specific builder used to create it,
		  *	and the file is always a relative path to the place where the exe 
		  *	file is located. \n
		  *	<?xml version="1.0"?> \n
		  *	<Maps> \n
		  *		<Include file="XXX"/> \n
		  *		<Map id="X" type="X" file="XXX"/> \n 
		  *		... \n
		  *		<SetOfMaps baseId="X" number="Y" type="Z" file="XXX"/> \n
		  *		...
		  *	</Maps>	
		  */
		MapBuilder (const std::string& mDef);
		/** The map builder owns all maps created from it. \n
			When the map is destroyed all maps created with it are also destroyed. */
		virtual ~MapBuilder ();

		/** To get the number of definitions included in the definition file. */
		int numberResources () const;
		/** To get the id of every resource to be loaded. */
		std::vector <int> resourcesId () const;

		// To manage adds-on
		/** To get the adds-on. */
		const AddsOnList& addsOn () const
							{ return (_addsOn); }
		/** To add one add-on */
		void addAddsOn (AddsOn *aO);

		/** Once the definition file is read, it is possible to load all maps
			usually before the game starts. This is done to improve the performamce. */
		void preLoad ();

		/** To get a map. \n
			If the maps has already been loaded before, its reference is returned. \n
			Otherwise the builder tries to create it from scratch using the definition loaded at construction time,
			and with the appropiate add-on (one specific for the type defined in the definition structure). */
		Map* map (int nm);

		private:
		void readFile (const std::string& fDef);
		MapDefinition readMapDefinition (TiXmlElement* e);
		MapDefinition readSetOfMapsDefinition (TiXmlElement* e);

		protected:
		typedef std::map <int, MapDefinition> Definitions;

		Definitions _definitions;
		Definitions _setOfDefinitions;
		std::map <int, Map*> _maps;
		AddsOnList _addsOn;

		std::string _definitionFile;
		std::string _basePath;
	};

	/** A TmxMap is a map with the TMX format. \n
		Any TMX map has images (e.g. the tiles), 
		so a form builder (Sprite2DBuilder) is requiered (passed as parameter). 
		To know the structure os a tiled map see https://doc.mapeditor.org/en/stable/reference/tmx-map-format/
		and use Tiled Map Editor to create them (https://www.mapeditor.org). */
	class TMXMapBuilderAddsOn : public MapBuilder::AddsOn
	{
		public:
		TMXMapBuilderAddsOn (Sprite2DBuilder* sB = NULL)
			: MapBuilder::AddsOn (),
			  _formBuilder (sB),
			  _lastFormIdCreated (100000),
			  _lastTileIdCreated (100000),
			  _lastTileLayerIdCreated (100000),
			  _lastObjectLayerCreated (200000),
			  _lastImageLayerCreated (300000),
			  _lastObjectCreated (100000),
			  _basePath (__NULL_STRING__),
			  _createdForms ()
							{ assert (_formBuilder); } // It can't be null...

		/** True when the type of map if the type this add on knows to built up. */
		virtual bool canCreateMap (int t) const
							{ return (t == __QGAMES_TMXMAP__); }
		virtual Map* createMap (const MapBuilder::MapDefinition& def,
			const std::string& bP);

		protected:
		// To overload this methods...
		virtual Tile* createTile (int id, Form* form, int nf, 
			const TileProperties& p = TileProperties ()) = 0;
		virtual TileLayer* createTileLayer (int id, const std::string& n, 
			const Tiles& t, Map* m, TileLayer::VisualOrientation o, const LayerProperties& p = LayerProperties ()) = 0;
		virtual ObjectLayer* createObjectLayer (int id, const std::string& n, const Objects&, 
			Map* m, const LayerProperties& oP = LayerProperties ()) = 0;
		virtual ImageLayer* createImageLayer (int id, const std::string& n, Image* i, 
			Map* m, const LayerProperties& oP = LayerProperties ()) = 0;
		virtual Ellipse* createEllipse (int id, const Position& p, const Vector& o, bdata a1, bdata a2,
			const ObjectProperties& oPrp = ObjectProperties ());
		virtual GraphicalPolygon* createPolygon (int id, const Positions& pts, const Position& p,
			const ObjectProperties& oPrp = ObjectProperties ());
		virtual GraphicalPolyline* createPolyline (int id, const Positions& pts, const Position& p,
			const ObjectProperties& oPrp = ObjectProperties ());
		virtual Map* createMapObject (int id, const Layers& l, int w, int h, int d, int tW, int tH, int tD,
			const MapProperties& p = MapProperties ()) = 0;

		private:
		// To transform TMX structures into QGAMES ones...
		Form* createFormFrom (const Tmx::Tileset* tS, Tmx::MapOrientation o);
		TileLayer* createTileLayerFrom (const Tmx::Layer* l, const Tmx::Map* m, const Forms& f);
		ObjectLayer* createObjectLayerFrom (const Tmx::ObjectGroup* o, const Forms& f);
		ImageLayer* createImageLayerFrom (const Tmx::ImageLayer* l, const Forms& f);
		Form* findForm (const std::string& n, const Forms& f);

		protected:
		Sprite2DBuilder* _formBuilder;

		private:
		// Implementation...
		int _lastFormIdCreated;
		int _lastTileIdCreated;
		int _lastTileLayerIdCreated;
		int _lastObjectLayerCreated;
		int _lastImageLayerCreated;
		int _lastObjectCreated;
		std::string _basePath;
		using CreatedForms = std::map <std::string, std::map <std::string, int>>;
		CreatedForms _createdForms;
	};

	/** An Object Map Builder is a builder to create maps based on objects located 
		in the screen, so a reference to the ObjectBuilder is also neeed (@see ObjectBuilder). */
	class ObjectMapBuilderAddsOn : public MapBuilder::AddsOn
	{
		public:
		ObjectMapBuilderAddsOn (ObjectBuilder* oB = NULL)
			: MapBuilder::AddsOn (),
			  _objectBuilder (oB),
			  _basePath (__NULL_STRING__),
			  _lastObjectId (20000)
						{ }

		protected:
		/**
		 *	The structure of an Object Map is like this:
		  *	<?xml version="1.0"?> \n
		  *	<Map name="X" width="XXX" height="YYY" depth="ZZZ" (optional)> \n
		  *		<Layers> \n
		  *			<Layer id="X" name="XXX"> \n
		  *				<Element id="X" X="XX" Y="YY" Z="ZZ"/> \n
		  *				<Element id="X" X="XX" Y="YY" Z="ZZ"/> \n
		  *				... \n
		  *				<Attributes> \n
		  *					<Attribute id="X" value="X"/> \n
		  *					... \n
		  *				</Attributes> \n
		  *			</Layer> \n
		  *			... \n
		  *		</Layers> \n
		  *		<Attributes> \n
		  *			<Attribute id="X" value="X"/> \n
		  *			... \n
		  *		</Attributes> \n
		  *	</Map>
		  */
		virtual bool canCreateMap (int t) const
							{ return (t == __QGAMES_OBJECTMAP__); }
		virtual Map* createMap (const MapBuilder::MapDefinition& def,
			const std::string& bP);

		protected:
		virtual Map* createMap (int id, int w, int h, int d, const Layers& l, const MapProperties& pties);
		virtual Layer* createLayer (int id, const std::string& name, 
			Objects& objs, Map* m, const LayerProperties& lP);

		private:
		// To read the different parts of the file...
		Layers readLayersSection (TiXmlElement* e);
		std::map <std::string, std::string> readPropertiesSection (TiXmlElement* e);

		// To read the individual elements componing a part of the file...
		Layer* readLayer (TiXmlElement* e);

		protected:
		ObjectBuilder* _objectBuilder;

		private:
		std::string _basePath;
		int _lastObjectId;
	};
}

#endif
  
// End of the file
/*@}*/


