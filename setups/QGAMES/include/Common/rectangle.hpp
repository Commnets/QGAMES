/** \ingroup Game */
/** \ingroup Foundation */
/*@{*/

/**	
 *	@file	
 *	File: rectangle.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a rectangle and the methods to manage it. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_RECTANGLE__
#define __QGAMES_RECTANGLE__

#include <Common/position.hpp>
#include <Common/polygon.hpp>
#include <vector>

namespace QGAMES
{
	class Rectangle;
	typedef std::vector <Rectangle> Rectangles;

	/** A Rectangle can exist also in the space. */
	class Rectangle
	{
		public:
		typedef enum { _XY = 0, _XZ = 1, _YZ = 2, _OTHER = 3 } Plane;

		static const Rectangle _noRectangle;

		Rectangle ();
		Rectangle (const Position& p1, const Position& p2, 
			const Vector& o = Vector (__BD 0, __BD 0, __BD 1), bdata a = __BD 0 /** Over 45 */);
		Rectangle (const std::string& ps);
		Rectangle& operator = (const Rectangle& r);

		const Position& pos1 () const
							{ return (_position1); }
		const Position& pos2 () const
							{ return (_position2); }
		const Position& pos3 () const
							{ return (_position3); }
		const Position& pos4 () const
							{ return (_position4); }
		const Position& orientation () const
							{ return (_orientation); }
		Position center () const;
		bdata width () const
							{ return ((_position3 - _position1).module ()); }
		bdata height () const
							{ return ((_position4 - _position1).module ()); }
		bdata diagonal () const
							{ return ((_position2 - _position1).module ()); }
		Plane plane () const
							{ return (_plane); }
		bdata angle () const
							{ return (_angle); }

		bool hasIn (const Position& p) const;
		bool isIn (const Rectangle& r) const;
		bool intersectsWith (const Rectangle& r) const;
		bool intersectsWith (const Position& p, const Vector& v) const // with a line...
							{ return (v != Vector::_cero && // It can be detectable
									  std::abs (v.dotProduct (_orientation)) < __QGAMES_ERROR &&
									  std::abs ((_position1 - p).dotProduct (_orientation)) < __QGAMES_ERROR); }
		Rectangle intersectionWith (const Rectangle& r) const;
		std::vector <Position> intersectionWith (const Position& p, const Vector& v) const;
		Rectangle rotate (const Position& e1, const Position& e2, bdata a) const;
		Rectangle rotateFromCenter (bdata a) const;
		Polygon projectOver (const Position& c, const Vector& o) const;
		Rectangle& toBase (); // Projects the rectangle on a plane parallel to its but passing by the origin.
		Rectangle bigger (bdata d) const; // Returns a rectangle d unit bigger, in the same
										  // plane, and with the same basic rotation angle than the original one...
		Rectangle scale (bdata d) const;

		bool operator == (const Rectangle& r) const;
		bool operator != (const Rectangle& r) const
							{ return (!(*this == r)); }
		Rectangle operator + (const Position& p) const
							{ return (Rectangle (_position1 + p, _position2 + p, 
								_position3 + p, _position4 + p, _orientation, _angle)); } 
		friend Rectangle operator + (const Position& p, const Rectangle& r)
							{ return (r + p); }
		Rectangle& operator += (const Position& p)
							{ *this = *this + p; return (*this); }
		Rectangle operator - (const Position& p) const
							{ return (Rectangle (_position1 - p, _position2 - p, 
								_position3 - p, _position4 - p, _orientation, _angle)); } 
		friend Rectangle operator - (const Position& p, const Rectangle& r)
							{ return (r - p); }
		Rectangle& operator -= (const Position& p)
							{ *this = *this - p; return (*this); }
		friend std::ostream& operator << (std::ostream& o, const Rectangle& r)
							{ return (o << r.pos1 () << "|" << r.pos2 () << "|" 
										<< r.pos3 () << "|" << r.pos4 ()) << 
										"[" << r._orientation << "|" << r._angle << "]"; } 

		private:
		// Implementation
		// Very internal constructor...
		Rectangle (const Position& p1, const Position& p2, const Position& p3,
			const Position& p4, const Vector& o, bdata a);

		// To simplify the calculations of points 3 & 4 in a plane...
		void calculateRestOfPointsInAPlane (bdata& p3A, bdata& p3B, bdata& p4A, bdata& p4B,
			bdata p1A, bdata p1B, bdata p2A, bdata p2B, bdata a);
		// Same but ordering...
		void orderMainPointsInAPlane (bdata& p1A, bdata& p1B, bdata& p2A, bdata& p2B);

		private:
		// Usually:
		// (1)-------(3)
		//  |		  |
		//  |         |
		// (4)-------(2)
		// Imaging anyway the different positions in the space the rectangle can has...
		Position _position1, _position2, _position3, _position4;
		Vector _orientation; // Could be redundant...
		// Usually too:
		// (Z)x-------> (x)
		//    | 
		//    | 
		//    | 
		//    | 
		//	  V (y)
		// z = x.crossproduct (y)
	
		// Implementation
		// To help the treatement...
		Plane _plane;
		bdata _angle;
	};
}

#endif

// End of the file
/*@}*/
