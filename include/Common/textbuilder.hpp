/** \ingroup Game */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: textbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 11/03/2019 \n
 *	Description: Defines a class to create text score objects from the type and the text itself. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_TEXTBUILDER__
#define __QGAMES_TEXTBUILDER__

namespace QGAMES
{
	/** The builder never owns the texts that it creates. 
		The one receiving the object must be unsure it deletes it. */
	class TextBuilder
	{
		public:
		TextBuilder ()
							{ }
		virtual ~TextBuilder () 
							{ }

		/** To determinate the next font to the one received as parameter in terms of size.
			By default the same received as parameter is returned. */
		virtual int nextSizeTo (int fnt)
							{ return (fnt); }
		/** The same but with the previous font.
			By default the same received as parameter is returned. */
		virtual int previousSizeTo (int fnt)
							{ return (fnt); }
		/** Looks for the font closest in color to the one received as parameter.
			By default the same received as parameter is returned. */
		virtual int inColor (int fnt, const Color& cl)
							{ return (fnt); }
		/** To create the text, receiving the font type id and the text. 
			This method has to be redefined when needed. 
			By default a game doesn't use any text builder. */
		virtual ScoreObjectText* createText (int fnt, const std::string& txt) = 0;
	};
}

#endif
  
// End of the file
/*@}*/
