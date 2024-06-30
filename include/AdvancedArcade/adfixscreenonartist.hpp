/**\ingroup AdArcadeGame */
/**\ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: adgame.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/04/2018 \n
 *	Description: Defines a extension to maintain the screen focused always onto an artist. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ADFIXSCREENONARTIST__
#define __QGAMES_ADFIXSCREENONARTIST__

#include <AdvancedArcade/addefinitions.hpp>
#include <Arcade/arcadeinclude.hpp>

namespace QGAMES
{
	/** To control how to manage the screen, following always main character. 
		It is important to know which class this one inherits from. 
		@see ToFixScreenOnArtist for more detail.
		This class adds the possibility of moving a background at the same time the artist moves. */
	class ToFixScreenOnArtistInSceneWithBackground : public ToFixScreenOnArtist
	{
		public:
		ToFixScreenOnArtistInSceneWithBackground (Screen* s, Artist* a, 
				bdata pW = (bdata) 0.5, bdata pH = (bdata) 0.5, bdata pD = (bdata) 0.5)
			: ToFixScreenOnArtist (s, a, pW, pH, pD),
			  _lD (Vector::_noPoint)
							{ }

		protected:
		/** @see parent. */
		virtual void moveScreenPixels (int x, int y, int z);

		private:
		// Implementation
		Vector _lD;
	};
}

#endif
  
// End of the file
/*@}*/
