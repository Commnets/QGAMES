/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: dwworlds.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 03/04/2019 \n
 *	Description: Defines the base for all worlds in dragonwind like games.
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_WORLDS__
#define __DRAGONWIND_WORLDS__

#include <DragonwindLike/dwdefinitions.hpp>
#include <Platform/ptinclude.hpp>

namespace DRAGONWIND
{
	/** Class to define common funtionality to any world in a DragonWind game. */
	class World : public QGAMES::PlatformWorld
	{
		public:
		World (int c, const QGAMES::Scenes& scns, 
				const QGAMES::WorldProperties& prps = QGAMES::WorldProperties ());

		// Managing the configuration...
		/** @see currentPlayerRuntimeValues. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		/** If the cfg structure weren't the right one, the initalization of the world would fail later. */
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
							{ _fullConfPerPlayer = cfg; }

		/** 
			To know the name of the world. 
			It is defined in the configuration data file. 
			If nothing has been defined, then a default one is returned. @see dwdefinitions.hpp.
			<Attributes>
				<Attribute id="NAME" value="???"/>
				...
			</Attributes>
		 */
		std::string name () const;

		/** @see parent. 
			The information kept in _fullConfPerPlayer is used to set the world for the specific player. \n
			And when finishes, the information get from currentPlayerRuntimeValues is saved in 
			_fullConfPerPlayer for the specific player. */
		virtual void initialize ();
		virtual void finalize ();

		protected:
		/** 
		  * The structure of the runtimeValues hasn't take into account many players. 
		  *	To this method just return the current information. 
		  *	This method invokes the runtimeValues of the parent.
		  * But it can be overloaded to add additional infomation 
		  * in the same way usually is done with runtimeValues (). 
		  */
		virtual QGAMES::SetOfOpenValues currentPlayerRuntimeValues () const
							{ return (QGAMES::PlatformWorld::runtimeValues ()); }

		protected:
		mutable QGAMES::SetOfOpenValues _fullConfPerPlayer;
	};

	/**
	 * An example world class is needn't.
	 * Just take into account that in the initialize method, the switches and counters have to be reset
	 * before invoking the parent class method, just because there they will be reset to a previous 
	 * value, if any. 
	 */
}

#endif
  
// End of the file
/*@}*/	
