/** \ingroup Game */
/** \ingroup Foundation */
/*@{*/

/**	
 *	@file	
 *	File: position.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a position in the space. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_POSITION__
#define __QGAMES_POSITION__

#include <Common/definitions.hpp>
#include <Common/genalgorithms.hpp>
#include <Common/matrix.hpp>
#include <math.h>
#include <ostream>
#include <istream>
#include <vector>

namespace QGAMES
{
	class Position;
	typedef Position Vector;
	typedef std::vector <Position> Positions;

	/** A position defines a coordinate in the space (3D possible). */
	class Position
	{
		public:
		Position ()
			: _posX (bdata0), _posY (bdata0), _posZ (bdata0)
							{ }
		Position (bdata px, bdata py, bdata pz = bdata0)
			: _posX (px), _posY (py), _posZ (pz)
							{ }
		Position (const std::string& p);
		Position& operator = (const Position& p)
							{ _posX = p._posX; _posY = p._posY; _posZ = p._posZ; 
							  return (*this); }

		bdata posX () const
							{ return (_posX); }
		bdata posY () const
							{ return (_posY); }
		bdata posZ () const
							{ return (_posZ); }

		Position plus (const Position& p) const
							{ return (Position (_posX + p._posX, _posY + p._posY, _posZ + p._posZ)); }
		Position less (const Position& p) const
							{ return (Position (_posX - p._posX, _posY - p._posY, _posZ - p._posZ)); }
		Vector divide (bdata d) const
							{ return ((d == bdata0) 
								? Vector::_noPoint : Vector (_posX / d, _posY / d, _posZ / d)); }
		Vector multiply (bdata d) const
							{ return (Vector (_posX * d, _posY * d, _posZ * d)); }
		Vector transformWith (const Matrix3& m) const;
		bdata distanceTo (const Position& p) const
							{ return (p.less (*this).module ()); }
		bdata module2 () const
							{ return (dotProduct (*this)); }
		bdata module () const
							{ return (sqrt (module2 ())); }
		bdata dotProduct (const Vector& v) const
							{ return ((_posX * v._posX) + (_posY * v._posY) + (_posZ * v._posZ)); }
		bdata angleWith (const Vector& v) const;
		Vector crossProduct (const Vector& v) const
							{ return (Vector (((_posY * v._posZ) - (_posZ * v._posY)), 
											  ((_posZ * v._posX) - (_posX * v._posZ)), 
											  ((_posX * v._posY) - (_posY * v._posX)))); }
		Vector& normalize ()
							{ if (*this != Vector ()) *this = *this / module (); return (*this); }
		Position rotate (const Position& e1, const Position& e2, bdata a) const;
		Position projectOver (const Position& c, const Vector& o) const;
		Position round () const
							{ return (Position ((bdata) ((int) _posX), (bdata) ((int) _posY), (bdata) ((int) _posZ))); }

		bool closeTo (const Position& p, bdata t = bdata1) const // The tolerance...
							{ return ((p - *this).module () <= t); }
		bool operator == (const Position& p) const
							{ return (closeTo (p, __QGAMES_ERROR)); } 
		bool operator != (const Position& p) const
							{ return (!closeTo (p, __QGAMES_ERROR)); } 

		Position operator + (const Position& p) const
							{ return (this -> plus (p)); }
		Position& operator += (const Position& p)
							{ return (*this = *this + p); }
		Position operator - (const Position& p) const
							{ return (this -> less (p)); }
		Position operator - () const
							{ return (*this * -1); }
		Position& operator -= (const Position& p)
							{ return (*this = *this - p); }
		Vector operator * (bdata i) const
							{ return (this -> multiply (i)); }
		friend Vector operator * (bdata d, const Vector& v) 
							{ return (v.multiply (d)); }
		friend Vector operator * (const Matrix3& m, const Vector& v) // For transformations...
							{ return (v.transformWith (m)); }
		Vector& operator *= (bdata d)
							{ return (*this = *this * d); }
		Vector operator / (bdata d) const 
							{ return (this -> divide (d)); }
		Vector& operator /= (bdata d)
							{ return (*this = *this / d); }

		friend std::ostream& operator << (std::ostream& o, const Position& p)
							{
								if (p == QGAMES::Position::_noPoint)
									o << std::string ("-");
								else
									o << (p.posX () * __BD 100) / __BD 100.0 << "," << 
										 (p.posY () * __BD 100) / __BD 100.0 << "," << 
										 (p.posZ () * __BD 100) / __BD 100.0;

								return (o);
							}
		friend std::istream& operator >> (std::istream& i, Position& p)
							{ std::string t; i >> t; p = QGAMES::Position (t); return (i); }

		// Managing list of points 
		static Position centerOf (const Positions& pos);

		private:
		void adjustCoordinates ();

		public:
		static Position _cero;
		static Position _noPoint;
		static Vector _xNormal;
		static Vector _yNormal;
		static Vector _zNormal;

		protected:
		bdata _posX, _posY, _posZ;
	};
}

#endif
 
// End of the file
/*@}*/
