/** \ingroup Game */
/** \ingroup Foundation */
/*@{*/

/**	
 *	@file	
 *	File: polygon.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a polygon made up of points. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_POLYGON__
#define __QGAMES_POLYGON__

#include <Common/position.hpp>

namespace QGAMES
{
	/** A polygon is a set of points. */
	class Polygon
	{
		public:
		Polygon ()
			: _points ()
							{ }
		Polygon (const Positions& p)
			: _points (p)
							{ }
		Polygon& operator = (const Polygon& p)
							{ _points = p._points; return (*this); }

		int numberPoints () const
							{ return (_points.size ()); }
		Position point (int p) const;
		const Positions& points () const
							{ return (_points); }

		bool hasIn (const Position& pos) const;
		Polygon rotate (const Position& e1, const Position& e2, bdata a) const;
		Polygon projectOver (const Position& c, const Vector& o) const;

		bool operator == (const Polygon& p) const;
		bool operator != (const Polygon& p) const
							{ 	return (!(*this == p)); }
		Polygon operator + (const Position& p) const;
		Polygon operator - (const Position& p) const;
		Polygon& operator += (const Position& p)
							{ return (*this = *this + p); }
		Polygon& operator -= (const Position& p)
							{ return (*this = *this - p); }
		friend std::ostream& operator << (std::ostream& o, const Polygon& r);

		private:
		Positions _points;
	};
};

#endif
 
// End of the file
/*@}*/
