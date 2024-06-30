/** \ingroup ArcadeGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: world.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class representing a world. \n
 *				 A world is made up of scenes. \n
 *				 Only one scene can be active at any time. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_WORLD__
#define __QGAMES_WORLD__

#include <Common/element.hpp>
#include <Arcade/scene.hpp>
#include <vector>

namespace QGAMES 
{
	class World;
	typedef std::map <int, World*> Worlds;
	typedef std::map <std::string, std::string> WorldProperties;

	/** A word is an element, so it is part of the game, having a life cycle. 
		The world is made up of scenes. */
	class World : public Element
	{
		public:
		World (int c, const Scenes& s, const WorldProperties& p = WorldProperties ());
		/** The world owns all its scenes. 
			When the world is destroyed its scenes are also destroyed. */
		virtual ~World ();

		/** @see parent. \n
			The information added is the one comming from every scene (setOfOpenValues). */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** A world could have a name. 
			By default it has none. */
		const std::string& name () const
							{ return (_name); }
		void setName (const std::string& n)
							{ _name = n; }

		/** To verify whether the position received is or not empty at the active
			layer of the active map in the active scene. */
		bool emptyPositionAt (const Position& p) const;
		/** Same with a rectangle.
			The rectangle is empty if all positions in the rectangle are also empties. */
		bool emptyBoxAt (const Position& p, const Position& s) const;

		// To manage the properties
		/** To know the properties. \n
			It works under chain of responsability pattern. \n
			The property is looked out in the active scene if any. \n
			If it doesn't exist there then the source should be the world. \n
			Anyway the default behavior can be changed. */
		virtual const WorldProperties& properties () const
							{ return (_properties); }
		virtual std::string property (const std::string& p) const;
		virtual bool existsProperty (const std::string& p) const;

		Scene* activeScene ()
							{ return (_activeScene); }
		const Scene* activeScene () const
							{ return (_activeScene); }
		/** To change the active scene of the world. */
		void setScene (int ns);
		bool existScene (int ns) const
							{ return (_scenes.find (ns) != _scenes.end ()); }
		Scene* scene (int ns);
		const Scenes& scenes () const
							{ return (_scenes); }

		/** Init the active scene. So it should be already set up when 
			this method is invoked. */
		virtual void initialize ();
		/** To call the everyLoop method of the active scene.
			So same caution as above. */
		virtual void inEveryLoop ();
		/** To call the updatePositions method of the active scene.
			So same caution as above. */
		virtual void updatePositions ();
		/** To call the drawOn method of the active scene. \n
			So same caution as above. \n
			The method receives the position to draw the world. \n
			The position is transmited to the scene. If it is _noPoint 
			the internal position of the scene will be used eventually. */
		virtual void drawOn (Screen* s, const Position&p = Position::_noPoint);
		/** To call the updatePositions method of the active scene.
			So same caution as above. */
		virtual void finalize ();

		protected:
		std::string _name; /** The name of the world if any. */
		Scenes _scenes; /** The scenes of the world. */
		WorldProperties _properties; /** The world can have properties used in the loops. */
		Scene* _activeScene; /** Which is the active scene already? */
	};
}

#endif
  
// End of the file
/*@}*/

