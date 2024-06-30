/** \ingroup Game */
/** \ingroup Foundation */
/*@{*/

/**	
 *	@file	
 *	File: box.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 18/05/2019 \n
 *	Description: Defines a box and the methods to manage it. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_BOX__
#define __QGAMES_BOX__

#include <Common/position.hpp>
#include <Common/rectangle.hpp>
#include <vector>

namespace QGAMES
{
	class Box;
	typedef std::vector <Box> Boxes;

	/** Class to represent a box in the space. \n
		The box is made up of six rectangles parallel two to two. 
		The box */
	class Box
	{
		public:
		static Box _noBox;

		Box ()
			: _origin (Position::_noPoint),
			  _vX (Vector::_noPoint), _vY (Vector::_noPoint), _vZ (Vector::_noPoint),
			  _wX (__BD 0), _wY (__BD 0), _wZ (__BD 0),
			  _rBase (Rectangle::_noRectangle), _rTop (Rectangle::_noRectangle),
			  _rFront (Rectangle::_noRectangle), _rBack (Rectangle::_noRectangle),
			  _rRight (Rectangle::_noRectangle), _rLeft (Rectangle::_noRectangle)
							{ }

		/**
		  * Simple constructor for the box.
		  * The data representing the box are:
		  * x,y,z doesn't mean necessary those axis, but we'll use this representation
		  * for a bettr metal visualization of what a box means. It is only a way to simplify the explanation.
		  * @param o	The back left bottom point of the box (or so) in the space.
		  * @param vX	The direction of X (or so) axis (It'll be normalized if needed).
		  * @param vY	The direction of Y (or so) axis (It'll be normalized if needeed).
		  * @param wx	Length in X axis.
		  * @param wy	Length in the Y axis.
		  * @param wz	Length in the Z axis.
		  */
		Box (const Position& o, const Vector& vx, const Vector& vy, bdata wx, bdata wy, bdata wz);

		/** In this case the rectangle is defined by the rectangle of the bootom face (or so),
			and the length of the height. */
		Box (const Rectangle& bottom, bdata wz);

		Box (const Box&) = default;

		Box& operator = (const Box&) = default;

		// Knowing the different aspects of the box
		const Position& origin () const
							{ return (_origin); }
		const Vector& vX () const
							{ return (_vX); }
		const Vector& vY () const
							{ return (_vY); }
		const Vector& vZ () const
							{ return (_vZ); }
		const bdata width () const
							{ return (_wX); }
		const bdata height () const
							{ return (_wY); }
		const bdata depth () const
							{ return (_wZ); }
		Position center () const
							{ return ((_rTop.pos1 () + _rBase.pos2 ()) / __BD 2); }

		const Rectangle& topRectangle () const
							{ return (_rTop); }
		const Rectangle& bottomRectangle () const
							{ return (_rBase); }
		const Rectangle& frontRectangle () const
							{ return (_rFront); }
		const Rectangle betweenFrontAndBackRectangle () const
							{ return (Rectangle ((_rFront.pos1 () + _rBack.pos1 ()) / __BD 2,
												 (_rFront.pos2 () + _rBack.pos2 ()) / __BD 2, _rFront.orientation ())); }
		const Rectangle& backRectangle () const
							{ return (_rBack); }
		const Rectangle& rightRectangle () const
							{ return (_rRight); }
		const Rectangle& leftRectangle () const
							{ return (_rLeft); }

		Box rotate (const Position& e1, const Position& e2, bdata a) const
							{ return (Box (_origin.rotate (e1, e2, a), _vX.rotate (e1, e2, a), _vY.rotate (e1, e2, a), 
										   _wX, _wY, _wZ)); }
		Box rorateFromCenter (const Vector& v, bdata a) const
							{ return (rotate (center (), center () + v, a)); }

		bool operator == (const Box& b) const
							{ return (_origin == b._origin && 
									  _vX == b._vX && _vY == b._vY && _vZ == b._vZ &&
									  _wX == b._wX && _wY == b._wY && _wZ == b._wZ); }

		bool operator != (const Box& b) const
							{ return (!(*this == b)); }
		Box operator + (const Vector& v) const
							{ return (Box (_origin + v, _vX, _vY, _wX, _wY, _wZ)); }
		friend Box operator + (const Vector& v, const Box& b)
							{ return (b + v); }
		Box& operator += (const Vector& v)
							{ *this = v + *this; return (*this); }

		/** Only two boxes with the same orientation can be added, and the result is a new one containing both. 
			This method is very important to control specific methods in composity entities e.g. */
		Box operator + (const Box& b) const;
		Box& operator += (const Box& b)
							{ *this = b + *this; return (*this); }
		Box operator - (const Vector& v) const
							{ return (*this + (-v)); }
		friend Box operator - (const Vector& v, const Box& b)
							{ return (b - v); }
		Box& operator -= (const Vector& v)
							{ *this = -v + *this; return (*this); }
		friend std::ostream& operator << (std::ostream& o, const Box& b)
							{ return (o << b.origin () << "|" 
										<< b.vX () << "|" << b.vY () << "|" << b.vZ () << "|" 
										<< b.width () << "|" << b.height () << "|" << b.depth ()); } 

		/** Creates a orthogonal cube containing all points passed as parameter.
			The box is aligned with the x and y axis. It is only for 3D positions. */
		static Box boxToInclude (const Positions& pos);
		/** Same but with the list of rectangles (more complicated). */
		static Box boxToInclude (const Rectangles& rects);
		/** Same but with thee list of boxes. Lets say is a way to calculate a limited box. */
		static Box boxToInclude (const Boxes& bxs);

		private:
		void calculateInternalPoints ();

		private:
		Position _origin;
		Vector _vX, _vY, _vZ;
		bdata _wX, _wY, _wZ;

		// Implementation
		Rectangle _rTop, _rBase, _rFront, _rBack, _rRight, _rLeft;
	};
}

#endif

// End of the file
/*@}*/