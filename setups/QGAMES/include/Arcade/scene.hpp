/** \ingroup ArcadeGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: scene.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines an scene. \n
 *				 Every scene is made up of several maps. \n
 *				 Just only one can be active in the world at any time. \n
 *				 The scenes can be connected with others. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_SCENE__
#define __QGAMES_SCENE__

#include <Common/element.hpp>
#include <Common/entity.hpp>
#include <Arcade/map.hpp>
#include <Arcade/envcondition.hpp>
#include <string>
#include <map>

namespace QGAMES
{
	class Scene;
	typedef std::map <int, Scene*> Scenes;
	typedef std::map <std::string, std::string> SceneProperties;
	typedef std::map <std::string, std::string> SceneActionBlockProperties;
	class World;

	/**
	 *	Some times the scene can be splitted into different blocks of action. \n
	 *	This usually happens when some "actions" are shared among scenes. \n
	 *	In those ocassions to package common actions into actionBlocks could be a way to reduce the size of the code.
	 *	This class has been created for this. Its use is not then mandatory at all. \n 
	 *  \n
	 *	Îf a block manages entities or characters they have to be added to the list of those in the scene. \n
	 *	The block is just usually to manage the behaviour not when they are drawn. \n
	 *	If the draw method were overloaded it would have to be to draw specific things related with the action. \n
	 *	The action blocks (regardin these aspects) must be drawn at the end.
	 */
	class SceneActionBlock : public Element
	{
		public:
		SceneActionBlock (int id, const SceneActionBlockProperties& prps = SceneActionBlockProperties ())
			: Element (id),
			  _active (true), // By default it is active once it is created
			  _properties (prps),
			  _scene (NULL) // Before using the block the scene variable should be set
							{ }

		/** @see parent. \n
			The information added is whether the action block is or not active. */ 
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		bool isActive () const
							{ return (_active); }
		virtual void setActive (bool a) // In some cases to stop execution could be interesting...so virtual!
							{ _active = a; }

		Scene* scene ()
							{ return (_scene); }
		const Scene* scene () const
							{ return (_scene); }
		void setScene (Scene* scn) // It could then be null...ckeck it before executing any other method...
							{ _scene = scn; }

		virtual void updatePositions ()
							{ } // Nothing is done by default...

		protected:
		bool _active;
		SceneActionBlockProperties _properties;

		private:
		Scene* _scene;
	};

	// To simplify the manage of a list of blocks...
	typedef std::vector <SceneActionBlock*> SceneActionBlocks;

	/** 
	 *	An scene represents where the current action of an arcade game happens. \n
	 *	The scene has an active map where the elements now interacting
	 *	with the artists of the arcade game are being represented. \n
	 *	An scene can be connected with others through a number of connection. \n
	 *	In some situations when the exit of the current scene is found,	
	 *	artist can move to another, the destination scene. \n
	 *	As any other element, the scene has a life cycle. \n
	 *  \n
	 *	In any scene there will be entities and characters. \n
	 *	The entities and characters can be draw just after a specific layer or at the end. \n
	 *  In this case entities are drawn always before the characters by default. But the behaviour can be changed. \n
	 *  \n
	 *	The scene can be splitted into different action blocks. \n
	 *  \n
	 *	Over the scene there can act several different environmental conditions.
	 */
	class Scene : public Element
	{
		public:
		friend SceneActionBlock;

		struct Connection
		{
			Connection ()
				: _id (-1),
				  _toScene (NULL),
				  _toPoint (-1)
							{ }

			bool operator == (const Connection& cn) const
							{ return (_id == cn._id && _toScene == cn._toScene && _toPoint == cn._toPoint); }
			bool operator != (const Connection& cn) const
							{ return (!(*this == cn)); }

			int _id;
			Scene* _toScene;
			int _toPoint;
		};

		/** To simflify the managemenet of list of connections. */
		typedef std::map <int, Connection> Connections;

		/** The scene is connected with other scenes. 
			There is no action block by default at construction level. */
		Scene (int c, const Maps& m, const Connections& cn = Connections (), 
			const SceneProperties& p = SceneProperties (), 
			const EntitiesPerLayer& ePL = EntitiesPerLayer ());
		/** The scene doesn't own its maps.
			The maps are own by the map builder. 
			When the scene is destroyed, its supervision finishes as well. */
		~Scene ();

		/** @see parent.
			The information added is: \n
			Per map the information of its layers (setOfOpenValues),
			and the information comming from the different scene blocks (setOfOpenValues). */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** A scene can have a name. 
			By default is hasn't it. */
		const std::string& name () const
							{ return (_name); }
		void setName (const std::string& n)
							{ _name = n; }

		/** To set the world being the reference. */
		void setWorld (World* w)
							{ _world = w; }
		/** To know the reference world. */
		World* world ()
							{ return (_world); }
		const World* world () const
							{ return (_world); }

		/** To set an environmanetal condition. 
			Setting a null condition removes the previous one if any. 
			Remember the scene owns the environmanetal condition, so when detroyed if any
			the conditions is destroyed as well. */
		void setEnvironmentalCondition (EnvironmentalCondition* ecd);
		/** To know the condition already in place.
			Null means no condition active. */
		EnvironmentalCondition* environmentalCondition ()
							{ return (_environmentalCondition); }

		/** See definition at world. */
		bool emptyPositionAt (const Position& p) const;
		/** See definition at world. */
		bool emptyBoxAt (const Position& p, const Position& s) const;

		// To manage the properties
		/** To know the properties.
			It works under chain of responsability pattern.
			The property is looked out in the active map if any.
			If it doesn't exist there then the source should be the scene. 
			Anyway the default behavior can be changed. */
		const SceneProperties& properties () const
							{ return (_properties); }
		virtual std::string property (const std::string& p) const;
		virtual bool existsProperty (const std::string& p) const;

		Map* activeMap ()
							{ return (_activeMap); }
		const Map* activeMap () const
							{ return (_activeMap); }
		/** Gets the relevant map.
			By default it is the active map unless there is none. In that case, it would be the first 
			in the list of maps,...if any!. It can be overlodaed. */
		virtual Map* relevantMap ()
							{ return ((_activeMap != NULL) 
								? _activeMap : ((_maps.size () != 0) ? (*_maps.begin ()).second : NULL)); }
		/** To change the active map. */
		void setMap (int nm);
		bool existMap (int nm) const
							{ return (_maps.find (nm) != _maps.end ()); }
		Map* map (int nm);
		const Maps& maps () const
							{ return (_maps); }

		/** To get the connections. */
		const Connections& connections () const
							{ return (_connections); }
		/** To verify whether the connection exists or not. */
		bool existsConnection (int nc);
		/** To get the scene coneccted in a point. Exception if the connection doesn't exist. */
		const Connection& connection (int nc);
		/** To add or remove a connection to another scene. */
		void addConnection (int nc, const Connection& dC);
		void removeConnection (int nc);

		/** To get the entitiesd being involved in the game. */
		Entities& entities ()
							{ return (_entities); }
		const Entities& entities () const
							{ return (_entities); }
		EntitiesPerLayer& entitiesPerLayer ()
							{ return (_entitiesPerLayer); }
		const EntitiesPerLayer& entitiesPerLayer () const
							{ return (_entitiesPerLayer); }
		/** To know the number of entities being part of the scene. */
		int numberOfEntities () const
							{ return ((int) _entities.size ()); }
		/** To know the entity's id. */
		std::vector <int> entitiesId () const;
		/** To add, remove or pick up an entity to the active scene. 
			The entities are part of the scenes. */
		/**	To know one entity added to the scene. */
		Entity* entity (int ne);
		const Entity* entity (int ne) const
							{ return ((const Entity*) entity (ne)); }
		bool existsEntity (int ne) const
							{ return (_entities.find (ne) != _entities.end ()); }
		/** When the entity is added, it is observed by the scene. */
		void addEntity (Entity* e);
		void removeEntity (Entity* e);
		void removeAllEntities ();

		/** To get the characteres. */
		Entities& characters ()
							{ return (_characteres); }
		const Entities& characters () const
							{ return (_characteres); }
		/** To know the number of characters being part of the scene. */
		int numberOfCharacters () const
							{ return ((int) _characteres.size ()); }
		/** To know the character's id. */
		std::vector <int> charactersId () const;
		/** To add, remove or pick up an entity to the active scene. 
			The characteres are also part of the scene, but the behaviour could be more 
			complicated. Remember a character is an entity that can be moved across
			the different place an scene is made of. */
		Entity* character (int nc);
		const Entity* character (int nc) const
							{ return ((const Entity*) character (nc)); }
		bool existsCharacter (int ne) const
							{ return (_characteres.find (ne) != _characteres.end ()); }
		/** When the character is added, it is observed by the scene. */
		void addCharacter (Character* c);
		void removeCharacter (Character* c);
		void removeAllCharacteres ();

		/** To get the actions blocks. The scene owns its own action blocks. */
		SceneActionBlocks& actionBlocks ()
							{ return (_actionBlocks); }
		const SceneActionBlocks& actionBlocks () const
							{ return (_actionBlocks); }
		bool existsActionBlock (int na) const;
		SceneActionBlock* actionBlock (int na);
		const SceneActionBlock* actionBlock (int na) const
							{ return ((const SceneActionBlock*) actionBlock (na)); }
		void addActionBlock (SceneActionBlock* aB, bool in = true);
		void removeActionBlock (int aId);
		void removeActionBlock (SceneActionBlock* aB)
							{ assert (aB); removeActionBlock (aB -> id ()); }

		// Element's life cycle...
		/** Init the active map, the entities and characters inside (if any).
			So the map should be already set up when this method is invoked. */
		virtual void initialize ();
		/** To call the everyLoop method of the active map, the entities and characters (if any).
			So same caution as above. */
		virtual void inEveryLoop ();
		/** To call the updatePositions method of the active map, the entities and characters (if any).
			So same caution as above. */
		virtual void updatePositions ();
		/** To call the drawOn method of the active map, the entities and characters (if any).
			So same caution as above. 
			The method receives the position to draw the elements.
			The position is transmited to the map. If it is _noPoint 
			the internal position of the scene will be used eventually.
			When an scene is defined in the definition file, it is possible to indicate
			if some specific entity should or not be drawn before or after an layer
			of the active map of the scene. That order is taken into account
			when this method is executed. */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		/** To call the updatePositions method of the active map.
			So same caution as above. */
		virtual void finalize ();
	
		/** When a notification comes from an element about a change
			it has to be taken into account in the way (e.g.) the scene is drawn. \n
			The events EVNCONDSTARTED and EVNCONDFINISHES are also treated here. */
		virtual void processEvent (const Event& e);

		protected:
		std::string _name; /** A potential name for the scene. By default it has none. */
		World* _world; /** A reference to the world owner. */
		EnvironmentalCondition* _environmentalCondition; /** The condition acting over the scene if any. It could be null. */
		Maps _maps; /** The maps of the scene. */
		Connections _connections; /** The scenes connected to this one. */
		SceneProperties _properties; /** The properties of the scene. */
		Map* _activeMap; /** Which is the acvtive map of the scene? */
		EntitiesPerLayer _entitiesPerLayer; /** Are there entities to be drawn before specific layers? */
		Entities _entities; /** The entities of the scene. */
		Entities _characteres; /** The characteres of the screen...incluging artists. */

		SceneActionBlocks _actionBlocks; /** The blocks of action. */

		// Implementation
		/** True when the map has to be rebuilt before drawing.
			It is true at construction time, and becomes true as well when an important element in the scene
			notifies its position has changed. */
		bool _rebuiltMapForDrawing;
	};
}

#endif
  
// End of the file
/*@}*/
