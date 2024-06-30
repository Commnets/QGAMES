/** \ingroup ArcadeGame */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: fixscreenonartist.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 15/08/2019 \n
 *	Description: Defines an extension to fix the screen on a artist. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_FIXSCREENONARTIST__
#define __QGAMES_FIXSCREENONARTIST__

#include <Common/screen.hpp>

namespace QGAMES
{
	class Artist;

	/** Using this class the main artist is fixed in an specific position of the screen. \n
		When the screen moves, the artist stays at the same position. \n
		As the artist is always in a map, the position is maintained on the
		specific screen position unless the beginning or the end of the map
		is reached. */
	class ToFixScreenOnArtist : public Screen::ToFixScreen
	{
		public:
		ToFixScreenOnArtist (Screen* s, Artist* a, 
			bdata pW = (bdata) 0.5, bdata pH = (bdata) 0.5, bdata pD = (bdata) 0.5);

		const Artist* artist () const
							{ return (_artist); }
		Artist* artist () 
							{ return (_artist); }
		bool considerXAxis () const
							{ return (_considerXAxis); }
		void setConsiderXAxis (bool fX)
							{ _considerXAxis = fX; }
		bool considerYAxis () const
							{ return (_considerYAxis); }
		void setConsiderYAxis (bool fY)
							{ _considerYAxis = fY; }
		bool considerZAxis () const
							{ return (_considerZAxis); }
		void setConsiderZAxis (bool fZ)
							{ _considerZAxis = fZ; }

		/** @ see parent. */
		virtual void moveScreen ();

		protected:
		/** To move the screen a certain number of pixels. \n
			This method is used by the previos one, and by default
			it moves the screen the number of pixels received as parameter. \n
			It doesn't take into account whether there is or not background i.e with different layers. */
		virtual void moveScreenPixels (int x, int y, int z);

		protected:
		Artist* _artist;
		/** Percentaje of the screen left after the movement in X axis... */
		bdata _percentageX;
		/** Percentaje of the screen left after the movement in Y axis... */
		bdata _percentageY; 
		/** Percentaje of the screen left after the movement in Z axis... */
		bdata _percentageZ; 
		/** Has the x axis to be taken in this behaviour? */
		bool _considerXAxis; 
		/** And the y axis? */
		bool _considerYAxis; 
		/** And the z axis? */
		bool _considerZAxis; 

		// Implementation
		int _lastPosX;
		int _lastPosY;
		int _lastPosZ;
	};
}

#endif
  
// End of the file
/*@}*/
