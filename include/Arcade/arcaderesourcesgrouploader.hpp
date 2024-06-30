/** \ingroup ArcadeGame */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: arcaderesourcesloadergroup.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/04/2018 \n
 *	Description: Defines a class to load a group of resources related with the arcade version. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ARCADERESOURCESGROUPLOADER__
#define __QGAMES_ARCADERESOURCESGROUPLOADER__

#include <Common/resourcesgrouploader.hpp>

namespace QGAMES
{
	class MapBuilder;
	class WorldBuilder;

	/** To extend the standard resource group loader to take into account maps, scenes and worlds. */
	class ArcadeLibResourceGroupLoader : public CommonLibResourceGroupLoader
	{
		public:
		/** To load Maps. */
		class TypeMapLoader : public TypeLoader
		{
			public:
			TypeMapLoader (const std::string& type, MapBuilder* mBld);

			/** @see parent. */
			virtual void load (const std::vector <int>& rscs);

			private:
			MapBuilder* _mapBuilder;
		};

		/** Standard loader for maps. */
		class StdTypeMapLoader : public TypeMapLoader
		{
			public:
			StdTypeMapLoader ();
		};

		/** To load Worlds. */
		class TypeWorldLoader : public TypeLoader
		{
			public:
			TypeWorldLoader (const std::string& type, WorldBuilder* wBld);

			/** @see parent. */
			virtual void load (const std::vector <int>& rscs);

			private:
			WorldBuilder* _worldBuilder;
		};

		/** Standard loader for worlds. */
		class StdTypeWorldLoader : public TypeWorldLoader
		{
			public:
			StdTypeWorldLoader ();
		};

		ArcadeLibResourceGroupLoader (const std::string& fDef);
	};
}

#endif

// End of the file
/*@}*/