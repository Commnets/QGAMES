/** \ingroup BoardGame */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: boardresourcesloadergroup.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/04/2018 \n
 *	Description: Defines a class to load a group of resources related with the board version. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_BOARDRESOURCESGROUPLOADER__
#define __QGAMES_BOARDRESOURCESGROUPLOADER__

#include <Common/resourcesgrouploader.hpp>

namespace QGAMES
{
	class BoardBuilder;

	class BoardLibResourceGroupLoader : public CommonLibResourceGroupLoader
	{
		public:
		/** To load boards. */
		class TypeBoardLoader : public TypeLoader
		{
			public:
			TypeBoardLoader (const std::string& type, BoardBuilder* bBld);

			/** @see parent. */
			virtual void load (const std::vector <int>& rscs);

			private:
			BoardBuilder* _boardBuilder;
		};

		/** Standard loader for boards. */
		class StdTypeBoardLoader : public TypeBoardLoader
		{
			public:
			StdTypeBoardLoader ();
		};

		BoardLibResourceGroupLoader (const std::string& fDef);
	};
}

#endif

// End of the file
/*@}*/