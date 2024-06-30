/** \ingroup Game */
/** \ingroup Screen */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: maskbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/09/2018 \n
 *	Description: Defines a way to create masks from a file. \n
 *				 It is not mandatory to use the builder to create a mask,
 *				 But if the mask is created using the build, it will also be destroyed with the builder later. \n
 *				 The builder is destroyed when the game class is destroyed. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_MASKBUILDER__
#define __QGAMES_MASKBUILDER__

#include <Common/mask.hpp>

class TiXmlElement;

namespace QGAMES
{
	/** To create masks. */
	class MaskBuilder 
	{
		public:
		/**
		  * The structure of a mask definition file is as follows: \n
		  *	The name of the different parameters will depend on the type of mask. \n
		  *	<?xml version="1.0"?> \n
		  *	<Masks> \n
		  *		<Mask id="X" blendingType="X" width="XXX" height="YYY"> \n
		  *			<Parameter id="CCC" value="CCC"/> \n
		  *			... \n
		  *		</Mask> \n
		  *		... \n
		  *	</Masks>
		  */
		MaskBuilder (const std::string& mDef);
		virtual ~MaskBuilder ();

		/** To get the mask. */
		Mask* mask (int nm);

		protected:
		struct MaskDefinition
		{
			MaskDefinition ()
				: _id (-1),
				  _type (-1),
				  _blendingType (0),
				  _width (-1), // If they are not defined, full screen will be understood at the right moment.
				  _height (-1)
							{ }

			int _id;
			int _type;
			int _blendingType;
			int _width, _height;
			std::map <std::string, std::string> _parameters;
		};

		/** Can be overloaded, but the default behaviour creates the default masks.
			@see file mask.hpp for further details. */
		virtual Mask* createMask (const MaskDefinition& mDef);

		private:
		void readFile (const std::string& fDef);
		MaskDefinition readMaskDefinition (TiXmlElement* e);

		protected:
		typedef std::map <int, MaskDefinition> Definitions;
		Definitions _definitions;
		std::map <int, Mask*> _masks;

		// Implementation
		std::string _definitionFile;
		std::string _basePath;
	};
}

#endif
  
// End of the file
/*@}*/