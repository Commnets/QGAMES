/** \ingroup DWTheRevege */
/*@{*/

/**	
 *	@file	
 *	File: ActionBlocks.hpp \n
 *	Game: DragonwindII (The Revenge) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 03/04/2020 \n
 *	Description: Specific action blocks for Dragonwind II. \n
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWINDTHEREVENGE_ACTIONBLOCKS__
#define __DRAGONWINDTHEREVENGE_ACTIONBLOCKS__

#include <DragonwindLike/dwinclude.hpp>

namespace DragonTheRevenge
{
	/** To include other flying entities in the Dragonwind II. */
	class FlyingMonstersActionBlock : public DRAGONWIND::FlyingMonstersActionBlock
	{
		public:
		DragonTheRevenge::FlyingMonstersActionBlock (int id, Properties* prps = new Properties ())
			: DRAGONWIND::FlyingMonstersActionBlock (id, prps)
							{ }

		protected:
		/** @see parent. */
		virtual DRAGONWIND::FlockOfMonsters* flock ();
	};

	/** To move lifts in a ciclic way. */
	class MoveLiftsCiclicActionBlock : public DRAGONWIND::SceneActionBlock
	{
		public:
		struct Properties
		{
			Properties ()
				: _lifts () 
							{ }

			Properties (const std::vector <std::string>& l)
				: _lifts (l)
							{ }

			/**
			  * In base to the attributes defined in the worlds.xml file. \n
			  *	<Attributes> \n
			  *		<Attribute id="Lifts" value="...,..."/> \n
			  *	</Attributes>
			  */
			Properties (const std::map <std::string, std::string>& dt);

			/** To get the properties as a set of openvalues. \n
				The configuration is saved as part of the runtime info of the action block
				because it can be change along the game, and after being initialized. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			// The name of the layers with the lift...
			std::vector <std::string> _lifts; 
		};

		MoveLiftsCiclicActionBlock (int id, Properties* prps = new Properties ())
			: DRAGONWIND::SceneActionBlock (id),
			  _properties (prps)
							{ }

		~MoveLiftsCiclicActionBlock ()
							{ delete (_properties); _properties = NULL; }

		/** To get the properties. */
		Properties* properties ()
							{ return (_properties); }
		const Properties* properties () const
							{ return (_properties); }

		/** @see parent. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** @see parent. 
			starts or stops the moevement of the lifts. */
		virtual void setActive (bool a);

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		protected:
		/** To move the lifts. */
		virtual void liftsToMove (bool m);
		virtual bool areLiftsMoving ();

		protected:
		Properties* _properties;
		QGAMES::TileLayers _lifts;
	};

	/** An special block to move lifts in the screen 2 of the world of mountains. */
	class MoveLiftsMountainsScene2ActionBlock: public MoveLiftsCiclicActionBlock
	{
		public:
		struct Properties : public MoveLiftsCiclicActionBlock::Properties 
		{
			Properties ()
				: MoveLiftsCiclicActionBlock::Properties (),
				  _sToGoBack (2) // Negative value means random up to that number in positive
							{ }

			Properties (const std::vector <std::string>& lfts, QGAMES::bdata sTGB)
				: MoveLiftsCiclicActionBlock::Properties (lfts),
				  _sToGoBack (sTGB)
							{ }

			/**
			  * In base to the attributes defined in the worlds.xml file. \n
			  *	<Attributes> \n
			  *		<Attribute id="Lifts" value="...,..."/> \n
			  *		<Attribute id="SecondsToGoBack" value="0.."/> \n
			  *	</Attributes>
			  */
			Properties (const std::map <std::string, std::string>& dt);

			/** To get the properties as a set of openvalues. \n
				The configuration is saved as part of the runtime info of the action block
				because it can be change along the game, and after being initialized. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			QGAMES::bdata _sToGoBack; 
		};

		MoveLiftsMountainsScene2ActionBlock (int id, Properties* prps = new Properties ())
			: MoveLiftsCiclicActionBlock (id ,prps),
			  _moveablePlatform (NULL)
							{ }

		/** @see parent. */
		virtual void initialize ();
		virtual void finalize ();

		protected:
		/** @see parent.  */
		virtual void liftsToMove (bool m);
		virtual bool areLiftsMoving ();
	
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }

		protected:
		QGAMES::TileLayer* _moveablePlatform;

		static const int _COUNTERTOGOBACK = 0;
	};
}

#endif

// End of the file
/*@}*/
