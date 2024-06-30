/** \ingroup Game */
/** \ingroup Foundation */
/*@{*/

/**	
 *	@file	
 *	File: color.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class to represent a color. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_COLOR__
#define __QGAMES_COLOR__

#include <string>

namespace QGAMES
{
	class Color
	{
		public:
		Color ()
			: _red (0), _green (0), _blue (0), _alpha (255)
							{ }
		Color (int r, int g, int b, int a = 255)
			: _red (r), _green (g), _blue (b), _alpha (a)
							{ adjust (); }
		Color (const std::string& c);

		bool operator == (const Color& cl) const
							{ return (_red == cl.red () && 
									  _green == cl.green () && 
									  _blue == cl.blue () && 
									  _alpha == cl.alpha ()); }
		bool operator != (const Color& cl) const
							{ return (_red != cl.red () ||
									  _green != cl.green () || 
									  _blue != cl.blue () || 
									  _alpha != cl.alpha ()); }

		int red () const
							{ return (_red); }
		int green () const
							{ return (_green); }
		int blue () const
							{ return (_blue); }
		int alpha () const
							{ return (_alpha); }
		void setAlpha (int a)
							{ _alpha = a; adjust (); }

		private:
		// To avoid no valid numbers...
		void adjust ();

		private:
		int _red, _green, _blue, _alpha;
	};
}

#endif
  
// End of the file
/*@}*/
