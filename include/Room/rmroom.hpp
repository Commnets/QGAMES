/** \ingroup Room */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: rmroom.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 31/10/2019 \n
 *	Description: In a Roomlike game type, the scenes are always similar to "rooms". \n
 *				 Most of the classes defined here are merely structures to group others,
 *				 and simplify the construction of a room.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_RMROOM__
#define __QGAMES_RMROOM__

#include <Room/rmdefinitions.hpp>
#include <Platform/ptinclude.hpp>
#include <typeinfo>

namespace QGAMES
{
	class RoomWall;
	class Room;
	class BasicRoom;
	class TiledRoom;

	/** The element of the roomwall. 
		It is an entity setting an initial form, aspect and position. */
	class RoomElement : public QGAMES::Entity
	{
		public:
		RoomElement (int eId, int fId, int aId, const Position& pos = Position::_cero, 
				const Forms& f = Forms (), const Data& d = Data ());
	};

	/** 
	  * A wall is usually a limit in the path of the entities moving in the room.
	  * The position of the wall is relative to the positions of the room it is in.
	  * The wall can be made up of little entities (bricks). 
	  * The id of that template entity is passed as parameter.
	  */
	class RoomWall
	{
		public:
		friend Room;

		/** The hole. 
			The reeal position in th wall is determined regarging the size of every brick,
			so a hole position is defined in terms of "number of bricks" per dimension. */
		struct Hole
		{
			Hole () = delete;

			Hole (float pL, float pH, float pD)
				: _positionLength (pL), _positionHeight (pH), _positionDepth (pD)
							{ }

			Hole (const Hole&) = default;

			Hole& operator = (const Hole&) = default;

			bool operator == (const Hole& h) const
							{ return (_positionLength == h._positionLength && 
									  _positionHeight == h._positionHeight &&
									  _positionDepth  == h._positionDepth); }

			/** Where */
			float _positionLength, _positionHeight, _positionDepth;
		};

		using Holes = std::vector <Hole>;

		/** An element in the wall.
			A wall element is a RoomElement in a Hole. 
			The WallElement owns the RoomElement received. */
		struct WallElement
		{
			WallElement () = delete;

			WallElement (int id, RoomElement* rE, const Hole& h);

			WallElement (const WallElement& wE);

			WallElement& operator = (const WallElement& wE);

			~WallElement ()
							{ delete (_wallElement); _wallElement = nullptr; }

			bool operator == (const WallElement& wE) const
							{ return (typeid (*_wallElement) == typeid (*wE._wallElement)); }

			/** Idntification */
			int _id;
			/** Who? Form and Aspect? */
			RoomElement* _wallElement;
			/** Where? */
			Hole _where;
		};

		/** Bricks are also WallElements. Maybe the purest ones!. */
		using WallElements = std::map <int, WallElement*>;
		using Bricks = std::map <int, WallElement*>; 

		RoomWall () = delete;

		/** 
		  *	A wall is defined basically by a brick and and the number of them. \n
		  *	The directions will have different meaning regarding the type of projection. \n
		  *	So, as an example, in isometric and cavalier: length = x axis, height = -z axis and depth = y axis.
		  * @param id		Id of the wall. It has to be unique in the room.
		  *	@param brickEId	The number of the entity being the base for any brick in the wall.
		  * @param iP		The position of the left back corner of the first brick in the wall. It is built upwards from that point.
		  *	@param nBL		Length of the wall in number of bricks.
		  *	@param nBH		Height of the wall in number of bricks.
		  *	@param nBD		Depth of the wall in number of bricks.
		  * @param d		A list with the elements decorating the wall. Theis positions are realtive to iP.
		  * The Wall owns the bricks and the decoration elements in it.
		  */
		RoomWall (int id, int brickEId, 
				  const Position& iP,
				  int nBL, int nBH, int nBD,
				  const WallElements& d = { });

		RoomWall (const RoomWall& rW) = delete;

		RoomWall& operator = (const RoomWall& rW) = delete;

		virtual RoomWall* clone () const;

		~RoomWall ()
							{ deleteBricks (); deleteWallElements (); }
		
		/** To know the identification of the room. */
		int id () const
							{ return (_id); }

		/** To know whether the wall has or not been initialized already. */
		bool initialized () const
							{ return (_initialized); }

		// Managing the bricks
		/** To know the id of the entity representing the base brick the wall is made up of. */
		int brickEntityId () const
							{ return (_brickEntityId); }
		/** To know the number of bricks the wall is made up of. */
		int numberBricks () const
							{ return (_numberBricksLength * _numberBricksHigh * _numberBricksDepth); }
		const Bricks& bricks () const
							{ return (_bricks); }
		bool existsBrick (int id) const
							{ return (_bricks.find (id) != _bricks.end ()); }
		bool existsBrickOnHole (const Hole& h) const
							{ return (existsBrick (idBrickOnHole (h))); }
		const WallElement* brick (int id) const;
		WallElement* brick (int id)
							{ return ((WallElement*) ((const RoomWall*) this) -> brick (id)); }
		const WallElement* brickOnHole (const Hole& h) const
							{ return (brick (idBrickOnHole (h))); }
		/** To hide/show the bricks of specific holes. */
		void setVisibleBricksFromHoles (const Holes& h, bool v);
		/** Add new bricks in specific holes.
			If the wall had been initialized, the bricks will be added to the scene too. */
		void addBricksInHoles (const Holes& h);
		/** To create a hole in the wall. 
			If the wall had been initialized, the bricks will be deleted from the scene too. */
		void removeBricksFromHoles (const Holes& h);

		// To manage the decoration in the wall
		/** To manange the decoration. 
			To change the decoration should be executed once the life cycle is running. */ 
		const WallElements& wallElements () const
							{ return (_wallElements); }
		bool existsWallElement (int id) const
							{ return (_wallElements.find (id) != _wallElements.end ()); }
		const WallElement* wallElement (int id) const;
		WallElement* wallElement (int id)
							{ return ((WallElement*) ((const RoomWall*) this) -> wallElement (id)); }
		void addWallElement (WallElement* wE);
		void addWallElements (const WallElements& wE)
							{ for (auto i : wE) addWallElement (i.second); }
		void removeWallElement (int id);

		/** To move the wall. */
		void move (const Vector& v);

		/** To initialize and finalize. */
		virtual void initializeOn (Scene* scn);
		virtual void finalize ();

		/** To draw the wall. */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint); 

		/** To determinate whether has collisioned with another entity. */
		bool hasCollisionWith (Entity* e) const;
		/** To know which entity has collisioned against if any (it can return null). */
		Entity* firstElementCollisioningWith (Entity* e);
		
		protected:
		/** To generate an unique id for the hole. */
		int idBrickOnHole (const Hole& h) const
							{ return ((int) ((h._positionDepth * (_numberBricksLength * _numberBricksHigh)) +
											 (h._positionHeight * _numberBricksLength) + h._positionLength)); }

		private:
		/** Very internal functions to delete the bricks and the decoration. */
		void deleteBricks ()
							{ for (auto i : _bricks) delete (i.second); _bricks.clear (); }
		void deleteWallElements ()
							{ for (auto i : _wallElements) delete (i.second); _wallElements.clear (); }
		
		protected:
		/** The identification of the wall. It should be unique in the room at least. */
		int _id;
		/** The id of the entity representing every brick of the wall. */
		int _brickEntityId;
		/** The position from which locate the bricks. */
		Position _initialPosition;
		/** The number of bricks to locate in both directions. */
		int _numberBricksLength, _numberBricksHigh, _numberBricksDepth;
		/** The decorarion of the wall (if any). */
		WallElements _wallElements;

		// Implementation 
		RoomElement* _baseBrick;
		Vector _lengthDirection, _heightDirection, _depthDirection;
		Bricks _bricks; // They are all owned by the wall...
		bool _initialized; // To indicate whether the wall has or not ben already initialized!
		Scene* _scene;
	};

	/** To manage all walls in a room. */
	typedef std::map <int, RoomWall*> RoomWalls;

	/** It is made up of a base, the walls inside, the doors and other decorative elements. 
		The room owns the walls. */
	class Room
	{
		public:
		friend BasicRoom;
		friend TiledRoom;

		struct DecorativeElement
		{
			DecorativeElement () = delete;

			DecorativeElement (int id, RoomElement* e, const Position& p);

			DecorativeElement (const DecorativeElement& dE);

			DecorativeElement& operator = (const DecorativeElement& dE);

			~DecorativeElement ()
							{ delete (_element); _element = nullptr; }

			bool operator == (const DecorativeElement& dE) const
							{ return (typeid (*_element) == typeid (*dE._element)); }

			int _id;
			RoomElement* _element;
			Position _where;
		};

		using Decoration = std::map <int, DecorativeElement*>;

		Room () = delete;

		/** The walls are owned by the room.
			So, they are destroyed when the room does.
			The decoration is also own by the room. */
		Room (int id, int t, const RoomWalls& w = { }, const Decoration& d = { })
			: _id (id), 
			  _type (t),
			  _walls (w), 
			  _decoration (d), // The position passed is the absolute one in the room...
			  _initialized (false),
			  _scene (nullptr)
							{ }

		Room (const Room& r) = delete;

		Room& operator = (const Room& r) = delete;

		virtual Room* clone () const;

		~Room ()
							{ deleteWalls (); deleteDecoration (); }
		
		/** To know whether the room has or not been initialized already. */
		bool initialized () const
							{ return (_initialized); }

		/** A reference to the scene this room is in. 
			Take care changing the scene once the game is running. It could generate fails and crashes. */
		const Scene* scene () const
							{ return (_scene); }
		Scene* scene ()
							{ return (_scene); }
		void setScene (Scene* scn) 
							{ _scene = scn; }

		/** The id and the type.
			Neither both can be changed after creation. */ 
		int id () const
							{ return (_id); }
		int type () const
							{ return (_type); }

		/** Managing the walls. 
			Adding and removing walls should be executed once the life cycle is running. */
		const RoomWalls& walls () const
							{ return (_walls); }
		bool existsWall (int id) const
							{ return (_walls.find (id) != _walls.end ()); }
		const RoomWall* wall (int id) const;
		RoomWall* wall (int id)
							{ return ((RoomWall*) (((const Room*) this) -> wall (id))); }
		void addWall (RoomWall* w);
		void addWalls (const RoomWalls& rW)
							{ for (auto i : rW) addWall (i.second); }
		void removeWall (int id);

		/** Managing the decoration. 
			Changing the decoration should be executed once the life cycle is running. */
		const Decoration& decoration () const
							{ return (_decoration); }
		bool existsDecorativeElement (int id) const
							{ return (_decoration.find (id) != _decoration.end ()); }
		const DecorativeElement* decorativeElement (int id) const;
		DecorativeElement* decorativeElement (int id)
							{ return ((DecorativeElement*) (((const Room*) this) -> decorativeElement (id))); }
		void addDecorativeElement (DecorativeElement* dE);
		void addDecoration (const Decoration& d)
							{ for (auto i : d) addDecorativeElement (i.second); }
		void removeDecorativeElement (int id);

		/** Adds all thing from a previous room. (ccing) */
		void addAllFromRoom (Room* r);

		/** To initialize and finalize. */
		virtual void initializeOn (Scene* scn);
		virtual void finalize ();

		/** To draw the room. */
		virtual void drawOn (Screen* s, const Position& p);

		private:
		/** To delet both the walls and the decoration. */
		void deleteWalls ()
							{ for (auto i : _walls) delete (i.second); _walls.clear (); }
		void deleteDecoration ()
							{ for (auto i : _decoration) delete (i.second); _decoration.clear (); }

		protected:
		/** id of the room. */
		int _id;
		/** The type of room */
		int _type;
		/** The walls being part of the room. */
		RoomWalls _walls;
		/** The elements that the characters in the scene can interact with. 
			Those elements are drawn as part of the scene. */
		Decoration _decoration;

		// Implementation
		/** A reference to the scene where the room is in. */
		Scene* _scene;
		/** The initial position of the interactivee elements. */
		using MapOfDecorationElementsPosition = std::map <Entity*, Position>;
		MapOfDecorationElementsPosition _decorationOriginalPositions;
		/** To mark when the room has been initialized. */
		bool _initialized;
	};

	/** The very basic room.
		The room is just inheriting from SceneWithBackground. 
		The complement to the previous one. */
	class BasicRoomScene : public SceneWithBackground
	{
		public:
		BasicRoomScene (int c, Room* r,
				const Maps& m, const Scene::Connections& cn = Scene::Connections (), 
				const SceneProperties& p = SceneProperties (), 
				const EntitiesPerLayer& ePL = EntitiesPerLayer ());

		~BasicRoomScene ()
							{ delete (_room); }

		/** @see parent. */
		virtual void initialize () override
							{ _room -> initializeOn (this); SceneWithBackground::initialize (); }
		virtual void drawOn (Screen* scr, const Position& p) override
							{ SceneWithBackground::drawOn (scr, p); _room -> drawOn (scr, p); }
		virtual void finalize () override
							{ _room -> finalize (); SceneWithBackground::finalize (); }

		protected:
		/** The information about the room. */
		Room* _room;
	};

	/** A room in a tiled game. 
		Once the room attribute has been assigned at construction, it shouldn't be modified ever. \n
		If the room object is (just in case) destroyed, the room attribute would have to be reassigned again. \n
		When the game is based on tiles and platforms, the base room to use is this one. */
	class PlatformRoomScene : public PlatformScene
	{
		public:
		PlatformRoomScene (int c, Room* r,  
				const Maps& m, const Scene::Connections& cn = Scene::Connections (), 
				const SceneProperties& p = SceneProperties (), 
				const EntitiesPerLayer& ePL = EntitiesPerLayer ());

		~PlatformRoomScene ()
							{ delete (_room); }

		/** @see parent. */
		virtual void initialize () override
							{ _room -> initializeOn (this); PlatformScene::initialize (); }
		virtual void drawOn (Screen* scr, const Position& p) override
							{ PlatformScene::drawOn (scr, p); _room -> drawOn (scr, p); }
		virtual void finalize () override
							{ _room -> finalize (); PlatformScene::finalize (); }

		protected:
		/** The information about the room. */
		Room* _room;
	};

	/** A world made up of rooms. */
	class WorldBasedOnRooms : public World
	{
		public:
		/** The scenes received must be rooms.
			The world receives also a reference to the map model representing the scenes. */
		WorldBasedOnRooms (int c, const Scenes& s, MazeModel* m,
			const WorldProperties& p = WorldProperties ());

		// Managing the configuration...
		/** @see parent. */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		protected:
		MazeModel* _model;
	};
}

#endif
  
// End of the file
/*@}*/
