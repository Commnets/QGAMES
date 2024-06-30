/** \ingroup ArcadeGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: map.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class representing a map. \n
 *				 A map a tiled representation of something. \n
 *				 A map is made up of several layers. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_MAP__
#define __QGAMES_MAP__

#include <Common/element.hpp>
#include <Common/entity.hpp>
#include <Common/position.hpp>
#include <Arcade/layer.hpp>
#include <Arcade/tile.hpp>
#include <map>

namespace QGAMES
{
	class Map;
	typedef std::map <int, Map*> Maps;
	typedef std::map <std::string, std::string> MapProperties;
	typedef std::map <std::string, std::vector <int>> EntitiesPerLayer;
	class Scene;
	class World;

	/** 
	 *	Any map is made up of layers. \n
	 *	Only one layer is active at the same time. \n
	 *	In a map the very important thing is the relevant layers. \n
	 *	The relevant layers are the important layers of the map. \n
	 *	Usually they are layers where the action happens.
	 *	e.g. There could be three layers: One with the tiles the artist is walking on,
	 *	Other one with platform moving, and the last one with very dangerous waters.
	 *	The active layer could not be the same than that the relevant layers. By default it is. \n
	 */
	class Map : public Element
	{
		public:
		Map (int c, const Layers& l, const MapProperties& p = MapProperties ());
		/** The map owns its layers.
			When the map is destroyed all its layers are also destroyed. */
		virtual ~Map ();

		/** @see parent.
			The information added is the one comming from each layer inside the map (setOfOpenValues). */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** To set the scene. */
		void setScene (Scene* s)
							{ _scene = s; }
		/** To know the scene. */
		const Scene* scene () const 
							{ return (_scene); }
		Scene* scene ()
							{ return (_scene); }
		/** To know the world. */
		World* world ();

		/** To know how wide, tall and deep the map is. 
			The final number depends on the map implementation. */
		virtual int width () const = 0;
		virtual int height () const = 0;
		virtual int depth () const = 0;

		// To know whether a position in the map is or not free.
		// Usually, these methods are invoked from the scene
		/** See definition at Scene. */
		virtual bool emptyPositionAt (const Position& p) const;
		/** See definition at Scene. */
		virtual bool emptyBoxAt (const Position& p, const Position& s) const;

		// To manage the properties
		const MapProperties& properties () const
							{ return (_properties); }
		virtual std::string property (const std::string& p) const // NULL If it doesn't exist...
							{ return ((_properties.find (p) == _properties.end ()) 
								? std::string (__NULL_STRING__) : (*_properties.find (p)).second); }
		virtual bool existsProperty (const std::string& p) const
							{ return ((_properties.find (p) != _properties.end ())); } 

		/** A map has always an active layer.
			These are the methods to manage it. */
		Layer* activeLayer () 
							{ return (_activeLayer); }
		void setLayer (int nl);
		bool existLayer (int nl) const;
		bool existLayer (const std::string& nl) const;
		/** To get a layer by its id. */
		Layer* layer (int nl);
		/** To get a layer by name. */
		Layer* layer (const std::string& nl);
		const Layers& layers () const
							{ return (_layers); }
		/** The relevant layers are the important layers. \n
			The list of relevant layers is used e.g. to check the collisions. \n
			By default these layers are only one which is the active layer.
			But the default behaviour can be modified to include a list 
			of them. */
		virtual Layers relevantLayers ();

		/** To get the list of objects created in the layer. */
		const ObjectLayers& objects () const
							{ return (_objects); }
		/** ..and the list of images. */
		const ImageLayers& images () const
							{ return (_images); }

		// Element's life cycle methods.
		/** Init the layers. \n
			So the layers should be already set up when this method is invoked. \n
			To set up the layers means to be included in the list of layers. */
		virtual void initialize ();
		/** To call the everyLoop method of the layers.
			So same caution as above. */
		virtual void inEveryLoop ();
		/** To call the updatePositions method of the layers.
			So same caution as above. */
		virtual void updatePositions ();
		/** This method is usually called before drawOn to take into account entities when drawing the map. \n
			The method receives the entities to take into account and whether the scene received a notification
			about changes in the position of any. In that case, what to draw can be rebuilt. */
		void considerEntitiesForDrawing (const Entities& et, const Entities& chrs, 
			const EntitiesPerLayer& epl, int eChgPos);
		/** To call the draw method of the layers. \n
			So same caution as above. \n
			A position can be pass as parameter. If not the internal position (if any)
			of the layer should be used or _cero in its stead. */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		virtual void finalize ();

		protected:
		/** To order the Elemnents before to draw them.
			By default it does nothing. */
		virtual Elements orderElementsToDrawOnScreen (const Elements& e, Screen* scr)
							{ return (e); }

		protected:
		// The internal attributes of the map
		Scene* _scene; /** A pointer to the scene. */
		Layers _layers; /** All layers. */
		ObjectLayers _objects; /** Object layers. They are "extracted" from the rest just to simply further acceses. */
		ImageLayers _images; /** Image layers. They are "extracted" from the rest just to simply further acceses. */ 
		Layer* _activeLayer; /** The very active later. */
		MapProperties _properties; /** The properties of the map. */

		private:
		// Implementation
		// Very internal lists used only before drawing
		Entities _entities;
		EntitiesPerLayer _entitiesPerLayer;
	};

	/** A tiled map made up of tile layers (among others) 
		Then there can happen special things on the tiled layers. */
	class TiledMap : public Map
	{
		public:
		/** The width and the height received are in number of tiles... */
		TiledMap (int c, const Layers& l, int w, int h, int d, int tW, int tH, int tD,
			const MapProperties& p = MapProperties ());

		/** To get the type of the tiled map.
			All tile layers have to be of the same type what is checked at construction time,
			but for this method only the type of the first one is got. */
		TileLayer::VisualOrientation visualOrientation () const;

		// To know how wide and tall a map is in pixels...
		/** @see parent's definition. */
		virtual int width () const
							{ return (_width * visualTileWidth ()); }
		/** @see parent's definition. */
		virtual int height () const
							{ return (_height * visualTileHeight ()); }
		/** @see parent's definition. 
			int a 2D world this value would be 0. */
		virtual int depth () const
							{ return (_depth * visualTileDepth ()); }
		
		/** To know the width and the height of a tile within the map
			as they have been defined inthe definition file. \n
			Take into account that this data is usually as the mesh (where the tiles are put later) has been defined.
			The tile size will be the data determinated by the visualTile methods. */
		/** The width. */
		int tileWidth () const
							{ return (_tileWidth); }
		/** The height. */
		int tileHeight () const
							{ return (_tileHeight); }
		/** The depth of every tile. Usually in 2SD and in 2.5D is 0. */
		int tileDepth () const
							{ return (_tileDepth); }

		/** To know the visual size. \n
			The visual size will depend on the type of projection. \n 
			By default the width is 1/2 of tilewidth in x axis and same in y axis under isometric projection,
			and width is 3/4 of tilewidth in x axis and 1/4 in y axis under cavalier projection. \n
			All can be defined in further classes. */
		virtual int visualTileWidth () const; // In the x axis...
		virtual int visualTileHeight () const; // In the y axis...
		virtual int visualTileDepth () const; // In the z axis...

		// As a tiled map is made up of tiles...
		// Then it is necessary to have methods to do something with them.
		/** See definition at Layer. */
		Tile* tileAt (const Position& p, Layer* l = NULL);
		/** See definition at Layer. */
		Tiles tilesAt (const Position& p, int n, Layer* l = NULL);
		/** See definition at Layer. */
		Tiles tilesAt (const std::vector <Position>& p, Layer* l = NULL);
		/** See definition at Artist. 
			If the tL attribute is null, the activeLayer will be used. */
		Tiles tilesAround (Entity *e, TileLayer* tL,
			bdata m = (bdata) 0.5, const Vector& pv = Vector::_cero);
		/** To know the tiles at an specific position and size. */
		Tiles tilesIn (TileLayer* tL, int pX, int pY, int pZ, int w, int h, int d);
		/** To know the position of a tile.
			If the tuile doesn't belong to the map, then a no position is returned. 
			The tile is located in the layer paremeter. If null, the active one is used. */
		Position tilePosition (const Tile* t, const TileLayer* tL) const;

		protected:
		/** To order the Elemnents before to draw them. \n
			list returned order the entities taking into account its Z and Y position.
			But also will depends on the type of tiled map (the layers). */
		virtual Elements orderElementsToDrawOnScreen (const Elements& e, Screen* scr);

		protected:
		// The internal attributes of the map
		int _width, _height, _depth; /** The width and the height and the depth in terms of tiles... */
		int _tileWidth, _tileHeight, _tileDepth; /** The weight and the height and the depth of every tile. */
		TileLayers _tiles; /** Tile layers. */
	};

	/** A simple object map. \n
		The curious thing is that the width and the height of the map are calculated using
		the positions of the elements compouning this one. */
	class ObjectMap : public Map
	{
		public:
		ObjectMap (int c, const Layers& l, int w, int h, int d, 
				const MapProperties& p = MapProperties ())
			: Map (c, l, p),
			  _width (w),
			  _height (h),
			  _depth (d)
							{ }

		/** See parent's description. 
			It is calculated based on the maximum element's position. */
		virtual int width () const
							{ return (_width); }
		/** See parent's description. 
			It is calculated based on the maximum element's position. */
		virtual int height () const
							{ return (_height); }
		/** See parent's description.
			It is calculated based on the maximum element's position.
			In a 2D Object map this value should be 0. */
		virtual int depth () const
							{ return (_depth); }

		protected:
		int _width;
		int _height;
		int _depth;
	};
}

// To simplify the access to any tiled map object...
#define __TM	(QGAMES::TiledMap*)

#endif
  
// End of the file
/*@}*/
