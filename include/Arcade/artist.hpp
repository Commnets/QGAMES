/** \ingroup ArcadeGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: entity.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class to represent an artist. \n
 *				 An artist is a character moving in arcade game. \n
 *				 Remember that the arcade game a 2D game made up of several worlds,
 *				 in which ones the artist can move. \n
 *	Versions: 1.0 Initial
 */

#include <Common/entity.hpp>
#include <Arcade/tile.hpp>

#ifndef __QGAMES_ARTIST__
#define __QGAMES_ARTIST__
  
namespace QGAMES
{
	class Map;
	class TileLayer;
	class EnvironmentalCondition;

	/**
	 *	An artist is a principal actor in an arcade game. \n
	 *	The artist should always be part of a map. \n
	 *	Remember an arcade game always happens in a world, and the world is made
	 *	up of several scenes, and any scene is represented by a map. Getting the map, the world can be also got. \n
	 *  When there is an environmental condition over the scene. It can affects the artist.
	 */
	class Artist : public Character
	{
		public:
		Artist (int cId, const Forms& f = Forms (), const Data& d = Data ())
			: Character (cId, f, d),
			  _map (NULL)
							{ }

		// Managing the map
		Map* map () 
							{ return (_map); }
		const Map* map () const
							{ return (_map); }
		virtual void setMap (Map* m)
							{ _map = m; /** It could be null... */ }

		/** The artist can be affected by an evironmental condition. 
			By default, nothing is done. */
		virtual void whenWheatherCondition (EnvironmentalCondition* eC)
							{ }
		virtual void whenNoWheatherCondition (EnvironmentalCondition* eC)
							{ }

		/** @see parent. */
		virtual void updatePositions ();

		protected:
		/** The map where the artist is in. 
			Could be mull. */
		Map* _map;
	};

	/**	
	 *	An artist in a tiled map is an artist moving throught a tiled map. ;-) \n
	 *	Through this class the system can detect which elements (tiles) the artist is on,
	 *	and which ones the artists is collisioning with (tiles). \n
	 *	This class allows to add a handler to manage what happen when the artist collision or is on an specific tile.
	 *	The class owns the handlers added. \n
	 *	This type of artist can also be defined as "being attachd to a tile", what means that iterating
	 *	throught the tiles of a tilemap would be possible to know if whether is there or not an artist there. \n
	 *	In this situation, any time the artist moves, the attached tile (if not null) is refreshed. 
	 *	@see TiledMap definition. 
	 */
	class ArtistInATiledMap : public Artist
	{
		public:
		/** A class to manage extra what happen when the artist hits a specific type of tile. */
		class TileCollisionHandler
		{
			public:
			TileCollisionHandler (int tp)
				: _type (tp)
							{ }
			virtual ~TileCollisionHandler ()
							{ } // Just in case...

			int type () const
							{ return (_type); }

			/** Initialize the handler, if needed. By default it is not. */
			virtual void initialize ()
							{ }

			/** It must be redefined later. \n
				The handlers are managed in the everyLoop method of the ArtistInATiledMap class. \n
				When a collision with a tile happens and a handler has been defined for this, this method is executed. */
			virtual void actionFor (ArtistInATiledMap* a, Tile*) = 0;

			protected:
			int _type;
		};

		/** To simplify managing lists of tile hit handlers. 
			The key is the type of the tile registered. */
		typedef std::map <int, TileCollisionHandler*> TileCollisionHandlers;

		/** The standard tile collision handler, just notify an event when it isinvoked.
			It can defined to be executed just once unless it has _repeat is true. */
		class StandardTileCollisionHandler : public TileCollisionHandler
		{
			public:
			/** The parameter is to indicate the threashold must repeat or not. */
			StandardTileCollisionHandler (int tp, bool r)
				: TileCollisionHandler (tp),
				  _repeat (r), _used (false)
							{ }

			bool repeat () const
							{ return (_repeat); }
			bool used () const
							{ return (_used); }
			void setUsed (bool u)
							{ _used = u; }

			/** @see parent. */
			virtual void initialize ()
							{ _used = false; }
			virtual void actionFor (ArtistInATiledMap* a, Tile* t);

			private:
			bool _repeat;

			// Implementation
			bool _used;
		};

		/** A tile collision handler for a type of tiles but keeping tracking per tile frame. \n
			It it's been used is something that has to be set out of this class and using the notification. */
		class TileCollisionHandlerPerFrame : public TileCollisionHandler
		{
			public:
			struct DetectionState
			{
				DetectionState (int nF, bool u = false)
					: _numberFrame (nF), _used (u)
							{ }

				int numberFrame () const
							{ return (_numberFrame); }

				bool _used;

				private:
				// Can't be changed once created...
				int _numberFrame;
			};

			typedef std::map <int, DetectionState*> DetectionStates;

			struct NotificationInfo
			{
				NotificationInfo ()
					: _detectionState (NULL), _tile (NULL), _owner (NULL)
							{ }
				NotificationInfo (DetectionState* dS, Tile* t)
					: _detectionState (dS), _tile (t), _owner (NULL)
							{ }

				DetectionState* _detectionState;
				Tile* _tile;
				ArtistInATiledMap* _owner;
			};

			typedef std::map <int, NotificationInfo*> NotificationInfos;

			TileCollisionHandlerPerFrame (int tp, bool r)
				: TileCollisionHandler (tp),
				  _repeat (r), _detectionStates (), _notificationInfos ()
							{ }
			~TileCollisionHandlerPerFrame ()
							{ initializeDetectionStates (); deleteNotificationInfo (); }

			bool repeat () const
							{ return (_repeat); }
			bool used (int nF) const;
			void setUsed (int nF, bool u)
							{ detectionState (nF) -> _used = u; }

			/** @see parent. */
			virtual void initialize ()
							{ initializeDetectionStates (); deleteNotificationInfo (); }
			virtual void actionFor (ArtistInATiledMap* a, Tile*);

			private:
			// Implementation. They can't be modified...
			void initializeDetectionStates ();
			void deleteNotificationInfo ();
			DetectionState* detectionState (int nF);
			NotificationInfo* notificationInfo (int nF);

			protected:
			bool _repeat;
			DetectionStates _detectionStates;

			// Implementation
			NotificationInfos _notificationInfos;
		};

		enum TilePosition 
		{
			UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3, 
			FRONT = 4, BEHIND = 5, OTHER = 6 
		};

		ArtistInATiledMap (int cId, const Forms& f = Forms (), const Data& d = Data ())
			: Artist (cId, f, d),
			  _attachedToTile (false),
			  _tileAttached (DOWN),
			  _refTileAttached (NULL),
			  _tileHitHandlers (),
			  _calculus ()
							{ }
		/** To destroy the handlers if any. */
		~ArtistInATiledMap ();

		/** To know the rectangle taken into account to look for tiles around. \n
			The positions taken into account will be different depending on the type of tile map the artist is walking trough */
		Rectangle tileRectangle () const;

		/** To know whether the artist is or not attached to a tile. */
		bool artistAttachedToTile () const
							{ return (_attachedToTile); }
		/** To set whether the artist has to be attached to a tile or not. \n
			It can be useful when the map is being drawn.
			In that situations, the artist is drawn when the tile is drawn. */
		void attachArtistToTile (bool t)
							{ _attachedToTile = t; }
		/** To set the tile the artist is attached to, in case of. */
		void setTileAttached (TilePosition p)
							{ _tileAttached = p; }
		/** Method to calculate
			the tiles occupied by the entity, taking into account the internal data
			(position and others) and the adjustments to that. \n
			The dt parameter is used to adjust the position of the artist.
			e.g. If the artist is in a layer and it moves, then the
			adjustement should be the layer's position. \n
			This method can be used externally. e.g it is invoked from
			the tileLayer to know the tiles to return!. */
		void tilesOcuppied (int& pX, int& pY, int& pZ, int& w, int& h, int& d,
				const QGAMES::Position& dt, QGAMES::bdata m, const QGAMES::Vector& pv);

		/** @see parent. 
			A redefinition to ensure the map is always a tiled map. 
			See also parent's definition. */
		virtual void setMap (Map* m);
		/** @see parent.
			The initialization, inits also all tile handlers. */
		virtual void initialize ()
							{ initializeTileHitHandlers (); }
		/** @see parent.
			When the positions of the artist are updated,
			The system detects the tiles collision with it or beneeth it
			and invoke the methods whenCollisionWith or whenOnTile to determinate
			what to do in consecuence. */
		virtual void updatePositions ();

		/** In this kind of artists is possible to detect
			what happen in a collision with a tile, or when it is over a tile. */
		virtual void whenCollisionWith (Entity* e)
							{ Artist::whenCollisionWith (e); } // To repeat to do not confuse with next...

		/** When collision with a tile... 
			By default it verifies only the handlers. \n
			The method receives the tile set to be verified, 
			the position of those tiles in the global set of tiles,
			An the lñayer where the tiles received are. */
		virtual void whenCollisionWith (const Tiles& t, TilePosition tP, TileLayer* l = NULL);
		/** When it is over a tile. Over means that the tiles tested are behind the artist.
			By default it does nothing. */
		virtual void whenOnTile (const Tiles& t, TileLayer* l = NULL)
							{ }
		/** Open exit to do more thing with the tiles behind the artist. */
		virtual void whenOver (const Tiles& t, TileLayer* tL, int w, int h, int d)
							{ }

		protected:
		// Support methods related with tiles management, all of them can be overloaded...	
		/** 
		 * This method is to determinate the tiles relevant in a layer. \n
		 * This method is invoked from updatePositions, to test the different collisions
		 * of the entities with the tiles around. 
		 */
		virtual SetOfTiles tilesRelevant (TileLayer* tL, 
			bdata m = (bdata) 0.5, const Vector& pv = Vector::_cero);

		/** This method is used by tilesRelevant method to determinate
			if there is somtehing to add to the influence reticula normally decided. */
		virtual void adjustRelevants (bdata& m, Vector& pv)
							{ m = (bdata) 0.5; pv = Vector::_cero; }

		/** 
		 * To know what are the relevant tile depending on the direction of the entity. \n
		 * The method is used by the method tilesRelevant also. It can be overloaded. \n
		 * The default implementation of the method has not take into account the 3D. \n
-		 * The relevant positions when the artist frame is three tiles width/height (int)
		 *	.|U|. \n
		 *	L|.|R \n
		 *	.|D|.		Where U = UP, R = RIGHT, L = LEFT, D = DOWN \n
		 * When it is 2: \n
		 * .|U \n
		 * L|RD			Where U = UP, L = LEFT, RD = RIGHT & DOWN \n
		 * Try to imagine in 3D! \n
		 */
		virtual void relevantTilePositions (int w, int h, int d, 
			std::vector <int>& up, std::vector <int>& down, 
			std::vector <int>& right, std::vector <int>& left, 
			std::vector <int>& front, std::vector <int>& back);

		/** To tests collisions eith tiles. */
		virtual void whenCollisionWithTiles (const SetOfTiles& ts, QGAMES::TileLayer* tL);

		/** To register a handler. 
			The parameter is the type of the type to treat with the handler. */
		void registerTileHitHandler (int t, TileCollisionHandler* h)
							{ if (_tileHitHandlers.find (t) == _tileHitHandlers.end ()) 
								_tileHitHandlers [t] = h; }
		/** To initialize all tile handlers. */
		void initializeTileHitHandlers ();

		protected:
		/** To indicate whether the artist is or not linked to a tile. */
		bool _attachedToTile;
		/** The tile attached. */
		TilePosition _tileAttached;
		/** A list of the tile handlers. */
		TileCollisionHandlers _tileHitHandlers;

		// Implementation
		/** Tile Attached. */
		Tile* _refTileAttached;

		private:
		/** Very internal structure, used when the tiles around
			of an artist are calculated. It tries to avoid the calculus when
			no internal data needed to that calculus has changed. */
		struct TilesOcuppiedCalculus
		{
			TilesOcuppiedCalculus ()
				: _form (0), _aspect (0), _position (Position::_noPoint),
				  _delta (Position::_cero), _m ((bdata) 0), _pV (Vector::_noPoint),
				  _pX (0), _pY (0), _pZ (0), _w (0), _h (0), _d (0)
							{ }
			TilesOcuppiedCalculus (int f, int a, const Position& pos, 
					const Position& dt, bdata m, const Vector& pV)
				: _form (f), _aspect (a), _position (pos),
				  _delta (dt), _m (m), _pV (pV),
				  _pX (0), _pY (0), _pZ (0), _w (0), _h (0), _d (0) // The result of the calculus...
							{ }
			TilesOcuppiedCalculus (ArtistInATiledMap* a, 
				const Position& dt, bdata m, const Vector& pV);

			bool operator == (const TilesOcuppiedCalculus& t) const
							{ return (_form == t._form && 
									  _aspect == t._aspect && 
									  _position == t._position &&
									  _delta == t._delta && _m == t._m && _pV == t._pV); }
			bool operator != (const TilesOcuppiedCalculus& t) const
							{ return (_form != t._form || 
									  _aspect != t._aspect || 
									  _position != t._position ||
									  _delta != t._delta || _m != t._m || _pV != t._pV); }

			void putResultInto (int& pX, int& pY, int& pZ, int& w, int& h, int& d)
							{ pX = _pX; pY = _pY; pZ = _pZ; w = _w; h = _h; d = _d; }
			void actualize (ArtistInATiledMap* a, const Position& dt, bdata m, const Vector& pV,
				int pX, int pY, int pZ, int w, int h, int d);

			// The data from the entity...
			int _form;
			int _aspect;
			Position _position;
			// Related with the calculus...
			QGAMES::Position _delta; // This delta comes from the layer position eventually!
			bdata _m;
			Vector _pV;

			// The very result of the calculus
			// If none of the other three variuables don't change
			int _pX, _pY, _pZ, _w, _h, _d;
		};

		TilesOcuppiedCalculus _calculus;
	};

	// To define a list of artists ina tiled map...
	typedef std::list <ArtistInATiledMap*> ArtistInATiledMapList;
}

// To simplify the access to any artist object...
#define __AT	(QGAMES::Artist*)
#define __ATT	(QGAMES::ArtistInATiledMap*)

#endif

// End of the file
/*@}*/
