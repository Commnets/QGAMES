#include <Common/rectangle.hpp>
#include <Common/genalgorithms.hpp>
#include <vector>
#include <algorithm>

// A no rectangle...
const QGAMES::Rectangle QGAMES::Rectangle::_noRectangle = QGAMES::Rectangle ();

// ---
QGAMES::Rectangle::Rectangle ()
	: _position1 (), _position2 (), _position3 (), _position4 (), 
	  _orientation (QGAMES::Vector::_zNormal),
	  _plane (QGAMES::Rectangle::Plane::_XY),
	  _angle (__BD 0)
{
	// Nothing else to do...so far
}

// ---
QGAMES::Rectangle::Rectangle (const QGAMES::Position& p1, const QGAMES::Position& p2, 
	const QGAMES::Vector& o, QGAMES::bdata a)
	: _position1 (p1), _position2 (p2), 
	  _position3 (QGAMES::Position::_cero), _position4 (QGAMES::Position::_cero),
	  _orientation (o),
	  _plane (QGAMES::Rectangle::Plane::_OTHER),
	  _angle (a - (__BD 2 * __PI * __BD ((int) (a / (__BD 2 * __PI))))) // To align the angle into the first loop!
{
	// This method corrects all "defects" in parameters
	// Put the variables in the right way to allow comparations and checks later
	// Take into account little imperfection in the data. 
	// Those imperfections can be the outcome of previous calculations (rotations mainly).
	// In the same way verifies whether the axis is right, and correct it if needed.
	// Doing so later comparations (e.g to verify whether a point is in the rectrangle) are possible

	// Is it parallel to plane XY?
	if (abs (_position1.posZ () - _position2.posZ ()) <= QGAMES::__QGAMES_ERROR &&
		(abs (_orientation.angleWith (QGAMES::Vector::_zNormal)) <= QGAMES::__QGAMES_ERROR ||
		 abs (_orientation.angleWith (-QGAMES::Vector::_zNormal)) <= QGAMES::__QGAMES_ERROR))
		// Basic 2D Rectangle...
	{
		_orientation = QGAMES::Vector::_zNormal;
		_plane = QGAMES::Rectangle::Plane::_XY;
		QGAMES::bdata p1A, p1B, p2A, p2B, pZ;
		pZ = _position1.posZ ();
		p1A = _position1.posX (); p1B = _position1.posY ();
		p2A = _position2.posX (); p2B = _position2.posY ();
		orderMainPointsInAPlane (p1A, p1B, p2A, p2B); 
		// Order the points...
		// To order the point is important for further checks
		// e.g: To check the intersection netween two rectangles or to check
		// whether a point is in the rectangle.
		// Ordering the points could alter the angle, but maintaining
		// is not relevant in terms of checking!.
		QGAMES::bdata p3A, p3B, p4A, p4B;
		calculateRestOfPointsInAPlane (p3A, p3B, p4A, p4B, p1A, p1B, p2A, p2B, a);
		_position1 = QGAMES::Position (p1A, p1B, pZ);
		_position2 = QGAMES::Position (p2A, p2B, pZ);
		_position3 = QGAMES::Position (p3A, p3B, pZ);
		_position4 = QGAMES::Position (p4A, p4B, pZ);
	}
	else
	// ...or it is parallel to plane XZ?
	if (abs (_position1.posY () - _position2.posY ()) <= QGAMES::__QGAMES_ERROR &&
		(abs (_orientation.angleWith (QGAMES::Vector::_yNormal)) <= QGAMES::__QGAMES_ERROR ||
		 abs (_orientation.angleWith (-QGAMES::Vector::_yNormal)) <= QGAMES::__QGAMES_ERROR))
	{
		_orientation = QGAMES::Vector::_yNormal;
		_plane = QGAMES::Rectangle::Plane::_XZ;
		QGAMES::bdata p1A, p1B, p2A, p2B, pY;
		pY = _position1.posY ();
		p1A = _position1.posX (); p1B = _position1.posZ ();
		p2A = _position2.posX (); p2B = _position2.posZ ();
		orderMainPointsInAPlane (p1A, p1B, p2A, p2B); 
		// Order the points (see explanation above)
		QGAMES::bdata p3A, p3B, p4A, p4B;
		calculateRestOfPointsInAPlane (p3A, p3B, p4A, p4B, p1A, p1B, p2A, p2B, a);
		_position1 = QGAMES::Position (p1A, pY, p1B);
		_position2 = QGAMES::Position (p2A, pY, p2B);
		_position3 = QGAMES::Position (p3A, pY, p3B);
		_position4 = QGAMES::Position (p4A, pY, p4B);
	}
	else
	// ...or it is parallel to plave YZ?
	if (abs (_position1.posX () - _position2.posX ()) <= QGAMES::__QGAMES_ERROR &&
		(abs (_orientation.angleWith (QGAMES::Vector::_xNormal)) <= QGAMES::__QGAMES_ERROR ||
		 abs (_orientation.angleWith (-QGAMES::Vector::_xNormal)) <= QGAMES::__QGAMES_ERROR))
	{
		_orientation = QGAMES::Vector::_xNormal;
		_plane = QGAMES::Rectangle::Plane::_YZ;
		QGAMES::bdata p1A, p1B, p2A, p2B, pX;
		pX = _position1.posX ();
		p1A = _position1.posY (); p1B = _position1.posZ ();
		p2A = _position2.posY (); p2B = _position2.posZ ();
		orderMainPointsInAPlane (p1A, p1B, p2A, p2B);
		// Order the points (see explanation above)
		QGAMES::bdata p3A, p3B, p4A, p4B;
		calculateRestOfPointsInAPlane (p3A, p3B, p4A, p4B, p1A, p1B, p2A, p2B, a);
		_position1 = QGAMES::Position (pX, p1A, p1B);
		_position2 = QGAMES::Position (pX, p2A, p2B);
		_position3 = QGAMES::Position (pX, p3A, p3B);
		_position4 = QGAMES::Position (pX, p4A, p4B);
	}
	// Other...
	else
	{
		// Which would the angle with the z axis be?
		QGAMES::bdata agAxis = _orientation.angleWith (QGAMES::Vector::_zNormal);
		// Which would the center of the future rectangle be?
		QGAMES::Position mP = QGAMES::Position (
			(_position1.posX () + _position2.posX ()) / __BD 2, 
			(_position1.posY () + _position2.posY ()) / __BD 2, 
			(_position1.posZ () + _position2.posZ ()) / __BD 2);

		// Set the points known to a position in the plane XY and aligned with the axis XY...
		QGAMES::Position p1R = _position1.rotate (mP, mP + _orientation, -_angle);
		p1R = p1R.rotate (mP, mP + QGAMES::Vector::_yNormal, -agAxis);
		QGAMES::Position p2R = _position2.rotate (mP, mP + _orientation, -_angle);
		p2R = p2R.rotate (mP, mP + QGAMES::Vector::_yNormal, -agAxis);

		// Rotating the position we will have 2 points in a plabe paralel to the z axis
		// ...and aligned with the rest of the axis...
		// So the rest of the points can be calculated!
		QGAMES::bdata p1A, p1B, p2A, p2B, pZ;
		pZ = p1R.posZ ();
		p1A = p1R.posX (); p1B = p1R.posY ();
		p2A = p2R.posX (); p2B = p2R.posY ();
		orderMainPointsInAPlane (p1A, p1B, p2A, p2B); 
		// Order the points (see explanation above)
		QGAMES::bdata p3A, p3B, p4A, p4B;
		calculateRestOfPointsInAPlane (p3A, p3B, p4A, p4B, p1A, p1B, p2A, p2B, a);
		_position1 = QGAMES::Position (p1A, p1B, pZ).
			rotate (mP, mP + QGAMES::Vector::_yNormal, agAxis).
			rotate (mP, mP + _orientation, _angle);
		_position2 = QGAMES::Position (p2A, p2B, pZ).
			rotate (mP, mP + QGAMES::Vector::_yNormal, agAxis).
			rotate (mP, mP + _orientation, _angle);
		_position3 = QGAMES::Position (p3A, p3B, pZ).
			rotate (mP, mP + QGAMES::Vector::_yNormal, agAxis).
			rotate (mP, mP + _orientation, _angle);
		_position4 = QGAMES::Position (p4A, p4B, pZ).
			rotate (mP, mP + QGAMES::Vector::_yNormal, agAxis).
			rotate (mP, mP + _orientation, _angle);
	}
}

// ---
QGAMES::Rectangle::Rectangle (const std::string& ps)
{
	if (ps == std::string ("-"))
		*this = QGAMES::Rectangle::_noRectangle;
	else
	{
		std::vector <QGAMES::bdata> elmts = QGAMES::getElementsFromAsBDATA (ps, ',', 9);
		*this = QGAMES::Rectangle (QGAMES::Position (elmts [0], elmts [1], elmts [2]), 
								   QGAMES::Position (elmts [3], elmts [4], elmts [5]),
								   QGAMES::Vector (elmts [6], elmts [7], elmts [8]));
	}
}

// ---
QGAMES::Rectangle& QGAMES::Rectangle::operator = (const QGAMES::Rectangle& r)
{ 
	_position1 = r._position1; 
	_position2 = r._position2;
	_position3 = r._position3;
	_position4 = r._position4;
	_orientation = r._orientation;
	_plane = r._plane;
	_angle = r._angle;
	return (*this); 
}

// ---
QGAMES::Position QGAMES::Rectangle::center () const
{ 
	return ((_position2 + _position1) / 2); 
}

// ---
bool QGAMES::Rectangle::hasIn (const QGAMES::Position& p) const
{
	bool result = false;
	// Parallel to plane XY?
	if (_plane == QGAMES::Rectangle::Plane::_XY)
	{
		if (abs (_angle) <= QGAMES::__QGAMES_ERROR) // The rectangle is not twisted
		{
			result = abs (p.posZ () - _position1.posZ ()) <= QGAMES::__QGAMES_ERROR && // at the same level (necessary condition)
				p.posX () >= (_position1.posX () - QGAMES::__QGAMES_ERROR) &&
				p.posX () <= (_position2.posX () + QGAMES::__QGAMES_ERROR) &&
				p.posY () >= (_position1.posY () - QGAMES::__QGAMES_ERROR) && 
				p.posY () <= (_position2.posY () + QGAMES::__QGAMES_ERROR);
		}
		else
		{
			// Rotate the rectangle to be aligned..
			// ...and then makes the same test!
			QGAMES::Position c = center ();
			QGAMES::Position cF = c + QGAMES::Vector::_zNormal;
			result = rotate (c, cF, -_angle).
				hasIn (p.rotate (c, cF, -_angle)); // If true, then true!
		}
	}
	else
	// Parallel to plane XZ?
	if (_plane == QGAMES::Rectangle::Plane::_XZ)
	{
		if (abs (_angle) <= QGAMES::__QGAMES_ERROR) // The rectangle is not twisted...
		{
			result = abs (p.posY () - _position1.posY ()) <= QGAMES::__QGAMES_ERROR && // at the same level (necesary condition) 
				p.posX () >= (_position1.posX () - QGAMES::__QGAMES_ERROR) &&
				p.posX () <= (_position2.posX () + QGAMES::__QGAMES_ERROR) &&
				p.posZ () >= (_position1.posZ () - QGAMES::__QGAMES_ERROR) && 
				p.posZ () <= (_position2.posZ () + QGAMES::__QGAMES_ERROR);
		}
		else
		{
			// Rotate the rectangle to be aligned..
			// ...and then makes the same test!
			QGAMES::Position c = center ();
			QGAMES::Position cF = c + QGAMES::Vector::_yNormal;
			result = rotate (c, cF, -_angle).
				hasIn (p.rotate (c, cF, -_angle)); // If true, then true!
		}
	}
	else
	if (_plane == QGAMES::Rectangle::Plane::_YZ)
	{
		if (abs (_angle) <= QGAMES::__QGAMES_ERROR) // The rectangle is not twisted
		{
			result = abs (p.posX () - _position1.posX ()) <= QGAMES::__QGAMES_ERROR && // at the same level (necessary condition)
				p.posY () >= (_position1.posY () - QGAMES::__QGAMES_ERROR) &&
				p.posY () <= (_position2.posY () + QGAMES::__QGAMES_ERROR) &&
				p.posZ () >= (_position1.posZ () - QGAMES::__QGAMES_ERROR) && 
				p.posZ () <= (_position2.posZ () + QGAMES::__QGAMES_ERROR);
		}
		else
		{
			// Rotate the rectangle to be aligned..
			// ...and then makes the same test!
			QGAMES::Position c = center ();
			QGAMES::Position cF = c + QGAMES::Vector::_xNormal;
			result = rotate (c, cF, -_angle).
				hasIn (p.rotate (c, cF, -_angle)); // If true, then true!
		}
	}
	// Other?
	else
	{
		// Angle with the zaxis?
		QGAMES::bdata agAxis = _orientation.angleWith (QGAMES::Vector::_zNormal);
		// Wich is the center?
		QGAMES::Position mP = center ();
		// Creates a rectangle perpendicular to the z axis, 
		// ...and aligned to the x and y axis!
		QGAMES::Position p1R = _position1.rotate (mP, mP + QGAMES::Vector::_yNormal, -agAxis);
		QGAMES::Position p2R = _position2.rotate (mP, mP + QGAMES::Vector::_yNormal, -agAxis);
		QGAMES::Rectangle rR (p1R, p2R, QGAMES::Vector::_zNormal, _angle);
		// Same with the point to check
		QGAMES::Position pR = p.rotate (mP, mP + QGAMES::Vector::_yNormal, -agAxis);
		pR.rotate (mP, mP + QGAMES::Vector::_zNormal, -_angle);
		result = rR.hasIn (pR);
	}

	return (result);
}

// ---
bool QGAMES::Rectangle::isIn (const QGAMES::Rectangle& r) const
{
	// Both rectangles have to be in the same plane...
	// It means that they normal vectors are parallel
	if (_orientation.crossProduct (r._orientation).module2 () > QGAMES::__QGAMES_ERROR)
		return (false); // ...or almost! (module2 always positive)

	// Once we hve test they are in the same plane
	// One is other if all their points are in...
	return (r.hasIn (pos1 ()) && r.hasIn (pos2 ()) && 
			r.hasIn (pos3 ()) && r.hasIn (pos4 ()));
}

// ---
bool QGAMES::Rectangle::intersectsWith (const Rectangle& r) const
{
	bool result = false;

	// If they are in the same plane, and that plane is "simple", let's say paralell
	// to one of the basic planes, the formula is too easy!
	// The same formula could be used even when the planes are not paralell
	// to one of the coordinates, because th hasIn method takes into account this 
	// situation, but it is better to do it in another way! (read later why)
	if (_plane == r._plane && _plane != QGAMES::Rectangle::Plane::_OTHER)
	{
		// The intersection is only possible when the rectangles are in the same "level",
		// Otherwise they wouldn't cross ever!
		if ((_plane == QGAMES::Rectangle::Plane::_XY && 
				abs (_position1.posZ () - r._position1.posZ ()) <= QGAMES::__QGAMES_ERROR) ||
			(_plane == QGAMES::Rectangle::Plane::_XZ && 
				abs (_position1.posY () - r._position1.posY ()) <= QGAMES::__QGAMES_ERROR) ||
			(_plane == QGAMES::Rectangle::Plane::_YZ && 
				abs (_position1.posX () - r._position1.posX ()) <= QGAMES::__QGAMES_ERROR))
				result = hasIn (r._position1) || hasIn (r._position2) || 
						 hasIn (r._position3) || hasIn (r._position4) ||
						 r.hasIn (_position1) || r.hasIn (_position2) || 
						 r.hasIn (_position3) || r.hasIn (_position4) ||
						 hasIn (r.center ()) || r.hasIn (center ());
	}
	else
	{
		if (_orientation.crossProduct (r._orientation).module2 () <= QGAMES::__QGAMES_ERROR)
		{
			// It could be done as:
			// result = hasIn (r._position1)...
			// because the hasIn method takes into account whether the
			// rectangle is rotated
			// But doing so, the rotation of the rectangle would be done
			// many times. This way is then faster!

			// Which would the angle with the z axis be?
			QGAMES::bdata agAxis = _orientation.angleWith (QGAMES::Vector::_zNormal);
			// Set the points known to a position in the plane XY and aligned with the axis XY...
			QGAMES::Rectangle r1R = rotate (QGAMES::Position::_cero, _orientation, -_angle).
				rotate (QGAMES::Position::_cero, QGAMES::Vector::_yNormal, -agAxis);
			QGAMES::Rectangle r2R = r.rotate (QGAMES::Position::_cero, _orientation, -_angle).
				rotate (QGAMES::Position::_cero, QGAMES::Vector::_yNormal, -agAxis);
			result = r1R.intersectsWith (r2R);
		}
		else
		{
			// Calculates the vector of the intersection line between 
			// the two planes containing the rectangles.
			// It doesn't mean that the rectangles intersect yet!
			int tp = -1;
			QGAMES::Vector rI = _orientation.crossProduct (r._orientation); // The orientation of the line
			QGAMES::bdata A = __BD 0; QGAMES::bdata B = __BD 0; QGAMES::bdata C = __BD 0;
			QGAMES::bdata Ap = __BD 0; QGAMES::bdata Bp = __BD 0; QGAMES::bdata Cp = __BD 0;
			QGAMES::bdata l = _orientation.dotProduct (_position1); // The indepent term of the first plane equation
			QGAMES::bdata lp = r._orientation.dotProduct (r._position1); // The independent term of the second!
			if (abs (_orientation.posY ()) > QGAMES::__QGAMES_ERROR &&
				abs (r._orientation.posY ()) > QGAMES::__QGAMES_ERROR &&
				abs ((_orientation.posX () * r._orientation.posY ()) + 
					 (_orientation.posY () * r._orientation.posX ())) >= QGAMES::__QGAMES_ERROR)
			{
				tp = 0;
				A = _orientation.posY (); Ap = r._orientation.posY ();
				B = _orientation.posX (); Bp = r._orientation.posX ();
			}
			else
			if (abs (_orientation.posY ()) > QGAMES::__QGAMES_ERROR &&
				abs (r._orientation.posY ()) > QGAMES::__QGAMES_ERROR &&
				abs ((_orientation.posZ () * r._orientation.posY ()) + 
					 (_orientation.posY () * r._orientation.posZ ())) >= QGAMES::__QGAMES_ERROR)
			{
				tp = 1;
				A = _orientation.posY (); Ap = r._orientation.posY ();
				B = _orientation.posZ (); Bp = r._orientation.posZ ();
			}
			else
			if (abs (_orientation.posX ()) > QGAMES::__QGAMES_ERROR &&
				abs (r._orientation.posX ()) > QGAMES::__QGAMES_ERROR &&
				abs ((_orientation.posZ () * r._orientation.posX ()) + 
					 (_orientation.posX () * r._orientation.posZ ())) >= QGAMES::__QGAMES_ERROR)
			{
				tp = 2;
				A = _orientation.posX (); Ap = r._orientation.posX ();
				B = _orientation.posZ (); Bp = r._orientation.posZ ();
			}

			QGAMES::bdata a = (l - (lp * A / Ap)) / (B - (Bp * A / Ap));
			QGAMES::bdata b = (l - (a * B)) / A;
			QGAMES::Position pI = QGAMES::Position::_cero;
			if (tp == 0) pI = QGAMES::Position (a, b, __BD 0);
			else if (tp == 1) pI = QGAMES::Position (__BD 0, a, b);
			else if (tp == 2) pI = QGAMES::Position (a, __BD 0, b);

			// Now I have already definede the vector defining the intersection line (rI)
			// ...and one point the line pass over (pI)
			// The it is time to calculate another points of it
			QGAMES::Position pIp = rI + pI;

			// They would intersect if one point of the line belonging to both rectangles can be found!
			// If the distance between one corner of any rectangle and the intersection line calculated before
			// is less than the side of the right rectangle, it would mean that 
			// the intersection line is in the rectangle
			QGAMES::bdata d1 = ((_position1 - pI).crossProduct (_position1 - pIp)).module2 () / 
				(pIp - pI).module2 (); // http://mathworld.wolfram.com/Point-LineDistance3-Dimensional.html
			QGAMES::bdata d2 = ((r._position1 - pI).crossProduct (r._position1 - pIp)).module2 () / 
				(pIp - pI).module2 ();
			result = d1 <= (_position4 - _position1).module2 () && 
						d2 <= (r._position4 - r._position1).module2 ();

			// Probably they will intersect, but not in a rectangle...a line maximum!
		}
	}
	
	return (result);
}

// ---
QGAMES::Rectangle QGAMES::Rectangle::intersectionWith (const QGAMES::Rectangle& r) const
{
	// If there is no intersection, then result is a no rectangle...
	if (!intersectsWith (r))
		return (QGAMES::Rectangle::_noRectangle);

	// The intersection is, in general a polygon...
	// To be a rectangle they have to be in the same plane
	// and they have to be rotated equally...
	if (_plane != r._plane ||
		_orientation.crossProduct (r._orientation).module2 () > QGAMES::__QGAMES_ERROR)
		return (QGAMES::Rectangle::_noRectangle);
	if (_angle != r._angle)
		return (QGAMES::Rectangle::_noRectangle);
	// If the intersection is going to be not a rectangle
	// this is not the right method to process it,
	// so a no rectangle is returned!

	// Then it is time to calculate the result rectangle
	// First of all in easy way, when both rectangles are parallel to any axis!
	QGAMES::Rectangle result = QGAMES::Rectangle::_noRectangle;
	if (_plane == QGAMES::Rectangle::Plane::_XY)
	{
		// First, the orientation has to be the same...
		if (abs (_angle) <= QGAMES::__QGAMES_ERROR)
		{
			std::vector <bdata> pA; pA.resize (4);
			std::vector <bdata> pB; pB.resize (4);
			pA [0] = _position1.posX (); pA [1] = _position2.posX ();
			pA [2] = r._position1.posX (); pA [3] = r._position2.posX ();
			pB [0] = _position1.posY (); pB [1] = _position2.posY ();
			pB [2] = r._position1.posY (); pB [3] = r._position2.posY ();
			std::sort (pA.begin (), pA.end ()); // Short the point from less to more...
			std::sort (pB.begin (), pB.end ()); // Same...
			result = QGAMES::Rectangle (QGAMES::Position (pA [1], pB [1], _position1.posZ ()), 
				QGAMES::Position (pA [2], pB [2], _position1.posZ ()), 
				QGAMES::Vector::_zNormal); 
		}
		// Otherwise, it corrects the orientation and recalculates...
		else
		{
			QGAMES::Position c = center ();
			QGAMES::Position cF = c + QGAMES::Vector::_zNormal;
			QGAMES::Rectangle rR = 
				rotate (c, cF, -_angle).intersectionWith (r.rotate (c, cF, -_angle));
			// Everything has to be rotated around the same center (it could be any, but the same)
			return (rR.rotate (c, cF, _angle));
			// Put it back in the original position!!
		}
	}
	else
	if (_plane == QGAMES::Rectangle::Plane::_XZ)
	{
		// First, the orientation has to be the same...
		if (abs (_angle) <= QGAMES::__QGAMES_ERROR)
		{
			std::vector <bdata> pA; pA.resize (4);
			std::vector <bdata> pB; pB.resize (4);
			pA [0] = _position1.posX (); pA [1] = _position2.posX ();
			pA [2] = r._position1.posX (); pA [3] = r._position2.posX ();
			pB [0] = _position1.posZ (); pB [1] = _position2.posZ ();
			pB [2] = r._position1.posZ (); pB [3] = r._position2.posZ ();
			std::sort (pA.begin (), pA.end ()); // Short the point from less to more...
			std::sort (pB.begin (), pB.end ()); // Same...
			result = QGAMES::Rectangle (QGAMES::Position (pA [1], _position1.posY (), pB [1]), 
				QGAMES::Position (pA [2], _position1.posY (), pB [2]), 
				QGAMES::Vector::_yNormal); 
		}
		// Otherwise, it corrects the orientation and recalculates...
		else
		{
			QGAMES::Position c = center ();
			QGAMES::Position cF = c + QGAMES::Vector::_yNormal;
			QGAMES::Rectangle rR = 
				rotate (c, cF, -_angle).intersectionWith (r.rotate (c, cF, -_angle));
			// Everything has to be rotated around the same center (it could be any, but the same)
			return (rR.rotate (c, cF, _angle));
			// Put it back in the original position!!
		}
	}
	else
	if (_plane == QGAMES::Rectangle::Plane::_YZ)
	{
		// First, the orientation has to be the same...
		if (abs (_angle) <= QGAMES::__QGAMES_ERROR)
		{
			std::vector <bdata> pA; pA.resize (4);
			std::vector <bdata> pB; pB.resize (4);
			pA [0] = _position1.posY (); pA [1] = _position2.posY ();
			pA [2] = r._position1.posY (); pA [3] = r._position2.posY ();
			pB [0] = _position1.posZ (); pB [1] = _position2.posZ ();
			pB [2] = r._position1.posZ (); pB [3] = r._position2.posZ ();
			std::sort (pA.begin (), pA.end ()); // Short the point from less to more...
			std::sort (pB.begin (), pB.end ()); // Same...
			result = QGAMES::Rectangle (QGAMES::Position (_position1.posX (), pA [1], pB [1]), 
				QGAMES::Position (_position1.posX (), pA [2], pB [2]), 
				QGAMES::Vector::_xNormal); 
		}
		// Otherwise, it corrects the orientation and recalculates...
		else
		{
			QGAMES::Position c = center ();
			QGAMES::Position cF = c + QGAMES::Vector::_xNormal;
			QGAMES::Rectangle rR = 
				rotate (c, cF, -_angle).intersectionWith (r.rotate (c, cF, -_angle));
			// Everything has to be rotated around the same center (it could be any, but the same)
			return (rR.rotate (c, cF, _angle));
			// Put it back in the original position!!
		}
	}
	// At this point the rectangles are not paralell to any axis, 
	// but trhey are in teh same axis, so there is a complex way to calculate
	// the rectangle interscetion!
	else
	{
		QGAMES::bdata agAxis = _orientation.angleWith (QGAMES::Vector::_zNormal);
		// Set the points known to a position in the plane XY and aligned with the axis XY...
		QGAMES::Rectangle r1R = rotate (QGAMES::Position::_cero, _orientation, -_angle).
			rotate (QGAMES::Position::_cero, QGAMES::Vector::_yNormal, -agAxis);
		QGAMES::Rectangle r2R = r.rotate (QGAMES::Position::_cero, _orientation, -_angle).
			rotate (QGAMES::Position::_cero, QGAMES::Vector::_yNormal, -agAxis);
		result = r1R.intersectionWith (r2R);
		result = result.rotate (QGAMES::Position::_cero, QGAMES::Vector::_yNormal, agAxis),
			rotate (QGAMES::Position::_cero, _orientation, _angle); // Put it back...
	}

	return (result);
}

// ---
std::vector <QGAMES::Position> QGAMES::Rectangle::intersectionWith (const QGAMES::Position& p, const QGAMES::Vector& v) const
{
	std::vector <QGAMES::Position> result { };

	// If there is no intersection, then result are no points
	if (!intersectsWith (p, v))
		return (std::vector <QGAMES::Position> { /** Empty */ });

	// If the plane is one of the main three main...
	QGAMES::Position p0, p1;
	p0 = p1 = QGAMES::Position::_noPoint;
	if (_plane == QGAMES::Rectangle::Plane::_OTHER)
	{
		// Convert everything it into an XY rectangle roting it...
		QGAMES::Rectangle cR (*this);
		QGAMES::Position cP (p); 
		QGAMES::Vector cV (v);

		QGAMES::Position cRect = center ();
		QGAMES::bdata aRect = _orientation.angleWith (QGAMES::Vector::_xNormal);
		cR = cR.rotate (cRect, cRect + QGAMES::Vector::_xNormal, -aRect).
				rotate (cRect, cRect + QGAMES::Vector::_zNormal, -_angle);
		cP = cP.rotate (cRect, cRect + QGAMES::Vector::_xNormal, -aRect).
				rotate (cRect, cRect + QGAMES::Vector::_zNormal, -_angle);
		cV = cV.rotate (cRect, cRect + QGAMES::Vector::_xNormal, -_angle).
				rotate (cRect, cRect + QGAMES::Vector::_zNormal, -_angle); 
		std::vector <QGAMES::Position> iR = cR.intersectionWith (cP, cV); 
		for (auto i : iR) /** Pure C++11 */
			result.push_back 
				(i.rotate (cRect, cRect + QGAMES::Vector::_zNormal, _angle).
				   rotate (cRect, cRect + QGAMES::Vector::_xNormal, aRect));
	}
	else
	{
		QGAMES::bdata p0A, p0B, p1A, p1B;
		p0A = p0B = p1A = p1B = __BD 0;
		
		QGAMES::bdata vA, vB;
		vA = vB = __BD 0;
		QGAMES::bdata MA, NA, MB, NB;
		MA = MB = NA = NB = __BD 0;
		QGAMES::bdata pA, pB;
		pA = pB = __BD 0;
		if (_plane == QGAMES::Rectangle::Plane::_XY) 
		{ 
			vA = v.posX (); vB = v.posY (); 
			MA = _position1.posX (); MB = _position2.posX ();
			NA = _position1.posY (); NB = _position2.posY ();
			pA = p.posX (); pB = p.posY ();
		}
		else 
		if (_plane == QGAMES::Rectangle::Plane::_XZ) 
		{ 
			vA = v.posX (); vB = v.posZ ();
			MA = _position1.posX (); MB = _position2.posX ();
			NA = _position1.posZ (); NB = _position2.posZ ();
			pA = p.posX (); pB = p.posZ ();
		}
		else // Plane YZ
		{ 
			vA = v.posY (); vB = v.posZ (); 
			MA = _position1.posY (); MB = _position2.posY ();
			NA = _position1.posZ (); NB = _position2.posZ ();
			pA = p.posY (); pB = p.posZ ();
		}

		if (vA != __BD 0.0 && vB != __BD 0.0)
		{
			QGAMES::bdata d = (-vB / vA * pA) + pB;

			p0A = MA;
			p0B = (vB / vA * MA) + d;
			if (p0B > NB)
			{
				p0A = vA / vB * (NB - d);
				p0B = NB;
			}
			else
			if (p0B < NA)
			{
				p0A = vA / vB * (NA - d);
				p0B = NA;
			}

			p1A = MB;
			p1B = (vB / vA * MB) + d;
			if (p1B > NB)
			{
				p1A = vA / vB * (NB - d);
				p1B = NB;
			}
			else
			if (p1B < NA)
			{
				p1A = vA / vB * (NA - d);
				p1B = NA;
			}
		}
		else
		if (vA == __BD 0.0)
		{
			p0A = pA;
			p0B = NA;
			p1A = pA;
			p1B = NB;
		}
		else // vB == __BD 0.0
		{
			p0A = MA;
			p0B = pB;
			p1A = MB;
			p1B = pB;
		}

		if (p0A >= MA && p0A <= MB && p1A >= MA && p1A <= MB &&
			p0B >= NA && p0B <= NB && p1B >= NA && p1B <= NB)
		{
			// Fits the rectangle...
			if (_plane == QGAMES::Rectangle::Plane::_XY)
				{ p0 = QGAMES::Position (p0A, p0B, __BD 0); p1 = QGAMES::Position (p1A, p1B, __BD 0); }
			else if (_plane == QGAMES::Rectangle::Plane::_XZ)
				{ p0 = QGAMES::Position (p0A, __BD 0, p0B); p1 = QGAMES::Position (p1A, __BD 0, p1B); }
			else // Plane YZ
				{ p0 = QGAMES::Position (__BD 0, p0A, p0B); p1 = QGAMES::Position (__BD 0, p1A, p1B); }

			// So, put them into the result...
			result.push_back (p0); result.push_back (p1);
		}
		// Otherwise the intersection points are out of the rectangle...

	}

	return (result);
}

// ---
QGAMES::Rectangle QGAMES::Rectangle::rotate (const QGAMES::Position& e1, 
	const QGAMES::Position& e2, bdata a) const
{
	// If no angle... no rotation...
	if (a == __BD 0)
		return (*this);

	// If the rotation axis passes trought the center of the rectangle
	// and it is paralell to the orientation,
	// We have just only to che the angle!
	// First condition: rotation axis and orientation has to be paralell...so:
	bool rP = ((e2 - e1).crossProduct (_orientation).module2 () <= QGAMES::__QGAMES_ERROR);
	// Second condition: The vector from the center to one point of the axis
	// has to be paralle to the axis itself:
	bool rC = ((e2 - e1).crossProduct (e2 - center ()).module2 () <= QGAMES::__QGAMES_ERROR);

	// If the rotation axis is parallel to the orientation of the rectangle
	// the angle of the rectangle respect the plane it is on, has to be actualized
	// and it won't be needed to change the orientation itself.
	// In any case, just two points are rotated, and the rest are calculated
	return (Rectangle (
		_position1.rotate (e1, e2, a), 
		_position2.rotate (e1, e2, a),
		rP ? _orientation : _orientation.rotate (e1, e2, a), 
		rP ? _angle + a : _angle)); 
}

// ---
QGAMES::Rectangle QGAMES::Rectangle::rotateFromCenter (QGAMES::bdata a) const
{
	QGAMES::Position c = center ();
	return (rotate (c, c + _orientation, a));
}

// ---
QGAMES::Polygon QGAMES::Rectangle::projectOver (const Position& c, const Vector& o) const
{
	// Independent of the type of rectangle...
	QGAMES::Positions pts;
	pts.push_back (_position1);
	pts.push_back (_position3); // In order...
	pts.push_back (_position2);
	pts.push_back (_position4);
	return (QGAMES::Polygon (pts).projectOver (c, o));
}

// ---
QGAMES::Rectangle& QGAMES::Rectangle::toBase ()
{
	QGAMES::Polygon ply = projectOver (QGAMES::Position::_cero, _orientation);
	_position1 = ply.point (0); _position3 = ply.point (1); _position2 = ply.point (2); _position4 = ply.point (3);

	return (*this);
}

// ---
QGAMES::Rectangle QGAMES::Rectangle::bigger (QGAMES::bdata d) const
{
	QGAMES::Vector vH = ((_position3 - _position1).normalize ()) * d; // Vector from 1 to 2...
	QGAMES::Vector vV = ((_position4 - _position1).normalize ()) * d; // Vector from 1 to 3...
	return (QGAMES::Rectangle (_position1 - vH - vV, _position2 + vH + vV, 
		_position3 + vH - vV, _position4 - vH + vV, _orientation, _angle));
}

// ---
QGAMES::Rectangle QGAMES::Rectangle::scale (QGAMES::bdata d) const
{
	QGAMES::bdata  dL = (_position2 - _position1).module () * (__BD 1 - d) / __BD 2; // What to reduce the diagonal from a corner...
	QGAMES::Vector dG = (_position2 - _position1).normalize ();
	return (QGAMES::Rectangle (_position1 + (dG * dL), _position2 - (dG * dL), _orientation, _angle));
}

// ---
bool QGAMES::Rectangle::operator == (const QGAMES::Rectangle& r) const
{ 
	return (_position1 == r._position1 && 
			_position2 == r._position2 &&
			_position3 == r._position3 &&
			_position4 == r._position4 &&
			_orientation == r._orientation && 
			abs (_angle - r._angle) <= QGAMES::__QGAMES_ERROR); 
}

// ---
QGAMES::Rectangle::Rectangle (const QGAMES::Position& p1, const QGAMES::Position& p2, 
	const QGAMES::Position& p3, const QGAMES::Position& p4, const QGAMES::Vector& o, 
	QGAMES::bdata a)
		: _position1 (p1), _position2 (p2), _position3 (p3), _position4 (p4),
		 _orientation (o), _angle (a)
{
	// This constructor has to be used really carefully
	// The constructor takes the points as something good
	// No checks are done
	// If the points are not in the right order, 
	// further invocations to other methods could fail!

	// The plane is the only variable pending to be fixed...
	QGAMES::Vector oN = _orientation.normalize ();
	if (oN == QGAMES::Vector::_zNormal)
		_plane = QGAMES::Rectangle::Plane::_XY;
	else
	if (oN == QGAMES::Vector::_yNormal)
		_plane = QGAMES::Rectangle::Plane::_XZ;
	else
	if (oN == QGAMES::Vector::_xNormal)
		_plane = QGAMES::Rectangle::Plane::_YZ;
	else
		_plane = QGAMES::Rectangle::Plane::_OTHER;
}

// ---
void QGAMES::Rectangle::calculateRestOfPointsInAPlane (QGAMES::bdata& p3A, QGAMES::bdata& p3B, 
	QGAMES::bdata& p4A, QGAMES::bdata& p4B,
	QGAMES::bdata p1A, QGAMES::bdata p1B, QGAMES::bdata p2A, QGAMES::bdata p2B, QGAMES::bdata a)
{
	if (a == __BD 0)
	{
		p3A = p2A; p3B = p1B;
		p4A = p1A; p4B = p2B;
	}
	else
	{
		// It is going to simulate the point in the plane XY...
		// Because the point is already in a plane, it is really the same
		// The plane we play with!
		QGAMES::Position P1 (p1A, p1B, __BD 0); // Copy of the point 1 in plane XY
		QGAMES::Position P2 (p2A, p2B, __BD 0); // Copy of the point 2 in plane XY
		QGAMES::Position PC ((p1A + p2A) / __BD 2, (p1B + p2B) / __BD 2, __BD 0); // The center of the diagonal joining both...
		QGAMES::Position P1R = P1.rotate (PC, PC + QGAMES::Vector::_zNormal, -a); // Rotate the P1 around the center to "align" the diagonal
		QGAMES::Position P2R = P2.rotate (PC, PC + QGAMES::Vector::_zNormal, -a); // The same with P2...
		QGAMES::Position P3 (P2R.posX (), P1R.posY (), __BD 0); // Now the P3 can be calculated as above
		QGAMES::Position P4 (P1R.posX (), P2R.posY (), __BD 0); // Same with P4
		QGAMES::Position P3R = P3.rotate (PC, PC + QGAMES::Vector::_zNormal, a); // Rotate back P3
		QGAMES::Position P4R = P4.rotate (PC, PC + QGAMES::Vector::_zNormal, a); // ...and P4
		p3A = P3R.posX (); p3B = P3R.posY (); // The result coordinates are now the underrotated points...
		p4A = P4R.posX (); p4B = P4R.posY ();
	}
}

// ---
void QGAMES::Rectangle::orderMainPointsInAPlane (QGAMES::bdata& p1A, QGAMES::bdata& p1B,
	QGAMES::bdata& p2A, QGAMES::bdata& p2B)
{
	std::vector <bdata> pA; pA.resize (2);
	std::vector <bdata> pB; pB.resize (2);
	pA [0] = p1A; pA [1] = p2A;
	pB [0] = p1B; pB [1] = p2B;
	std::sort (pA.begin (), pA.end ()); // Short the point from less to more...
	std::sort (pB.begin (), pB.end ()); // Same...
	p1A = pA [0]; p2A = pA [1];
	p1B = pB [0]; p2B = pB [1];
}
