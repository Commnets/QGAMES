/** \ingroup ArcadeGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: arcadegame.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines an arcade game. \n
 *				 An arcade game is a game that takes place in a world (o many). \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ARCADEGAME__
#define __QGAMES_ARCADEGAME__

#include <Common/game.hpp>
#include <Arcade/world.hpp>
#include <Arcade/worldbuilder.hpp>
#include <Arcade/mapbuilder.hpp>

namespace QGAMES
{
	/** 
	 *	An arcade game is game that happens in a world. \n
	 *	In the arcade game, the main character (or several) moves throught 
	 *	different worlds reaching goals. \n
	 *	Besides needing object constructors, an arcade game requires builders of worlds and maps. \n
	 *	In an arcade game, the entities ("alive things" remember) have an important role.
	 */
	class ArcadeGame : public Game
	{
		public:
		ArcadeGame (GameImplementation* imp, const Worlds& w, int fps = 60)
			: Game (imp, fps),
			  _mapBuilder (NULL),
			  _worldBuilder (NULL),
			  _worlds (w),
			  _activeWorld (NULL)
							{ }
		virtual ~ArcadeGame ();

		// The builder os any arcade game...
		WorldBuilder* worldBuilder ()
							{ if (_worldBuilder == NULL)
								(_worldBuilder = createWorldBuilder ()) -> setExternalEngine (externalEngine ());
							  return (_worldBuilder); }
		MapBuilder* mapBuilder ()
							{ if (_mapBuilder == NULL)
								(_mapBuilder = createMapBuilder ()) -> setExternalEngine (externalEngine ());
							  return (_mapBuilder); }

		// Managing the worlds...
		/** Add a world. \n
			The same world can not be added twice. The world is not observed. */
		void addWorld (int nW);
		/** To verify whether a world already exists or not. */
		bool existWorld (int nW) const;
		/** To get a world. \n
			If the world doesn't exist a null pointer is returned.
			In debug mode the game stops. */
		World* world (int nW);
		/** To know whether a scene is or not defined within a world. */
		bool existWorldForScene (int scnId);
		/** To get the world where an scene is included in. */
		World* worldForScene (int scnId);
		/** To get the active world. \n
			Only one world can be active at the same time. \n
			If no active world exists a null pointer is returned. */
		World* activeWorld () 
							{ return (_activeWorld); }
		const World* activeWorld () const
							{ return (_activeWorld); }
		/** 
		  	To get the next world and scene in a specific connection point.
			@param oWrldId	The id of the world origin
			@param pScnId	The id of the scene origin
			@param cp		The connection point in the origin scene
			@param tWrldId	A reference to load the id of the target world
			@param tScnId	A reference to load the id of the target scene
			@param tCp		A reference to load the connection point inthe target scene.
			If the target variables are set to -1 would mean nothing is next. 
			It could be overloaded as the game needs, 
			but by default the scene connected and defined in the defnition file (@see WorldBuilder) is returned.
		  */
		virtual void nextWorldAndScene (int oWrldId, int oScnId, int cp, int& tWrldId, int& tScnId, int& tCp);
		/** Sets the active world.
			If the parameter is __MININT__, there will not be world active at all. */
		void setWorld (int nW);
		/** To get the list of worlds. */
		const Worlds& worlds () const
							{ return (_worlds); }

		/** To verify whether the connection point exists and if it is well defined. */
		bool isConncetionInfoFor (int oScnId, int cp);
		/** Get all the information for a destination point. */
		void connectionInfoFor (int oScnId, int cp, int& wrlId, int& tScndId, int& tCp);

		// Managing the artists
		/** To get an entity.
			When the entity is created, it is then observed. 
			So, using this method if something happens to the entity and 
			it sends an event after that, it would be received in this class. */
		Entity* artist (int nA);
		/** To add an entity like an artist.
			The system first of all tests whether the entity is or not an artist.
			If it is, then it is added .*/
		void addArtist (int nE);

		/** @see parent. The world is drawn. */
		virtual void drawOn (Screen* s);

		protected:
		// To get the builders...
		/** To get the instance of the builder to create the worlds. */
		virtual WorldBuilder* createWorldBuilder () = 0;
		/** To get the instance of the builder used to create the maps. */
		virtual MapBuilder* createMapBuilder () = 0;

		/** @see parent. */
		virtual std::string defaultParameter (const std::string& p) const;

		/** @see parent. */
		virtual void initialize (); /** Worlds and maps are preload if defined so. */
		virtual void inEveryLoop (); /** Iterates over the world also. */
		virtual void updatePositions (); /** Update the positions of the world. */
		virtual void additionalTasks () /** Important to detect collisions as part of the main loop. */
							{ detectCollisions (); }

		/** In an arcade game, it is very important to detect collisions. \n
			This is something that doesn't happen in another types of games.
			This method is for doing do. \n
			By default checks the collisions among all artist. */
		virtual void detectCollisions ();

		protected:
		/** This structure keeps a pair of entities. \n
			This pair is used to compare (e.g. in collisions). */
		struct PairOfEntities
		{
			PairOfEntities (Entity* a, Entity* b)
				: _a (a), _b (b)
							{ }
			Entity *_a, *_b;
		};

		typedef std::vector <PairOfEntities> SetOfPairOfEntities;

		/** Returns a set of pairs of the visible artists managed for the collisios. \n
			It can be invoked whenever is needed. This method is used mainly in detectCollisions method. \n
			The result is put into the variable passed as parameter. 
			This is declared in this way to speed up the execution only, avoiding the copy 
			of the outcome in another instance of the map. */
		virtual void pairOfVisibleArtists (SetOfPairOfEntities& p);
		/** Same but with the entities and characteres considered in the active scene. */
		void pairOfVisibleEntitiesInTheScene (SetOfPairOfEntities& p);

		protected:
		Worlds _worlds;
		World* _activeWorld;

		WorldBuilder* _worldBuilder;
		MapBuilder* _mapBuilder;

		Entities _artists; /** The artists being part of the game...
							   The user has to manage them... */
	};
}

// To simplify the access to the arcade object...
#define __AGM	(QGAMES::ArcadeGame*)

#endif
  
// End of the file
/*@}*/
