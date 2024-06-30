#include <Common/position.hpp>
#include <Common/matrix.hpp>
#include <Common/genalgorithms.hpp>

QGAMES::Position QGAMES::Position::_cero =
	QGAMES::Position (bdata0, bdata0 ,bdata0);
QGAMES::Position QGAMES::Position::_noPoint =
	QGAMES::Position (__MINBDATA__, __MINBDATA__, __MINBDATA__);
QGAMES::Vector QGAMES::Vector::_xNormal = 
	QGAMES::Vector ((QGAMES::bdata) 1, (QGAMES::bdata) 0, (QGAMES::bdata) 0);
QGAMES::Vector QGAMES::Vector::_yNormal = 
	QGAMES::Vector ((QGAMES::bdata) 0, (QGAMES::bdata) 1, (QGAMES::bdata) 0);
QGAMES::Vector QGAMES::Vector::_zNormal = 
	QGAMES::Vector ((QGAMES::bdata) 0, (QGAMES::bdata) 0, (QGAMES::bdata) 1);

// ---
QGAMES::Position::Position (const std::string& p)
	: _posX (bdata0), _posY (bdata0), _posZ (bdata0)
{
	if (p == std::string ("-"))
	{
		_posX = _posY = _posZ = __MINBDATA__;
	}
	else
	{
		std::vector <QGAMES::bdata> elmts = QGAMES::getElementsFromAsBDATA (p, ',', 3);
		_posX = elmts [0]; _posY = elmts [1]; _posZ = elmts [2];
	}
}

// ---
QGAMES::Vector QGAMES::Position::transformWith (const QGAMES::Matrix3& m) const
{
	QGAMES::bdata rc[3] = { __BD 0, __BD 0, __BD 0 };
	QGAMES::bdata vc[3] = { posX (), posY (), posZ () };

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			rc [i] += m.element (i, j) * vc [j];

	return (QGAMES::Vector (rc [0], rc [1], rc [2]));
}

// ---
QGAMES::bdata QGAMES::Position::angleWith (const QGAMES::Vector& v) const
{
	QGAMES::bdata v1M = module ();
	QGAMES::bdata v2M = v.module ();
	if (v1M == __BD 0 || v2M == __BD 0)
		return (__MAXBDATA__);
	return (acos ((float) (dotProduct (v) / (v1M * v2M))));
}

// ---
QGAMES::Position QGAMES::Position::rotate (const QGAMES::Position& e1, 
	const QGAMES::Position& e2, QGAMES::bdata a) const
{
	// If no angle... no rotation...
	if (a == __BD 0)
		return (*this);

	// The axis AB: A = e1, B = e2; AB = e2 - e1;
	// n = normal vector parallel to AB passing through the origin: (e2 - e1)/|(e2 - e2)|
	// P, point to rotate = this;
	// v = [n . (P - A)] * n, because it is used twice in the formula and creating a variable is faster... 
	// You can check the formual at: http://www.josechu.com/mates/giros_espacio_es.htm
	Vector n = (e2 - e1).normalize ();
	Vector v = (n.dotProduct (*this - e1)) * n;
	Position Pp = e1 + v + (cos (a) * ((*this - e1) - v)) + (sin (a) * (n.crossProduct (*this - e1)));
	Pp.adjustCoordinates (); // Just because sin and cos functions returns very complex data...
	return (Pp);
}

// ---
QGAMES::Position QGAMES::Position::projectOver (const QGAMES::Position& c, const QGAMES::Vector& o) const
{ 
	// This formula is to project a point (this) on a plane.
	// The plane is represented by a point (c) and a normal vector to the plane (perpendicular)...
	Vector oR = o; if (oR.module () != (QGAMES::bdata) 1) oR.normalize (); // The normal vector has to have module 1...
	return (*this - (oR * ((*this - c).dotProduct (oR)))); 
	// c-this is a vector linking c and this point, 
	// Then the distance between the point (this) and the plane is calculated using the dot product. 
	// (knowing that the dot product is the length of the shadow of a vector over other)
	// Then a vector parallel to the normal one but with the length of the previous shadow is calculated.
	// Finally the point original point (this) is moved down that distance because it is the same that
	// the distance to the plane...
}

// ---
QGAMES::Position QGAMES::Position::centerOf (const QGAMES::Positions& pos)
{
	if (pos.size () == 0)
		return (QGAMES::Position::_noPoint);

	QGAMES::bdata cx, cy , cz; cx = cy = cz = __BD 0;
	for (QGAMES::Positions::const_iterator i = pos.begin (); i != pos.end (); i++)
		{ cx += (*i).posX (); cy += (*i).posY (); cz += (*i).posZ (); }

	return (QGAMES::Position (cx / __BD pos.size (), cy / __BD pos.size (), cz / __BD pos.size ()));
}

// ---
void QGAMES::Position::adjustCoordinates ()
{
	_posX = adjustDecimals (_posX);
	_posY = adjustDecimals (_posY);
	_posZ = adjustDecimals (_posZ);
}
