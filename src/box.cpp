#include <Common/box.hpp>

QGAMES::Box QGAMES::Box::_noBox = QGAMES::Box ();

// ---
QGAMES::Box::Box (const QGAMES::Position& o, const QGAMES::Vector& vx, const QGAMES::Vector& vy, 
		QGAMES::bdata wx, QGAMES::bdata wy, QGAMES::bdata wz)
	: _origin (o), 
	  _vX (vx), _vY (vy), _vZ (vx.crossProduct (vy)),
	  _wX (wx), _wY (wy), _wZ (wz)
{ 
	_vX.normalize (); _vY.normalize (); _vZ.normalize ();
	// To normalize them...

	assert (std::abs (_vX.dotProduct (_vY)) < __QGAMES_ERROR);
	// The original references hace to be orthogonal between them
	// Just in case....

	calculateInternalPoints (); 
}

// ---
QGAMES::Box::Box (const QGAMES::Rectangle& bottom, QGAMES::bdata wz)
	: _origin (Position::_noPoint),
	  _vX (Vector::_noPoint), _vY (Vector::_noPoint), _vZ (Vector::_noPoint),
	  _wX (__BD 0), _wY (__BD 0), _wZ (__BD 0),
	  _rBase (Rectangle::_noRectangle), _rTop (Rectangle::_noRectangle),
	  _rFront (Rectangle::_noRectangle), _rBack (Rectangle::_noRectangle),
	  _rRight (Rectangle::_noRectangle), _rLeft (Rectangle::_noRectangle)
{

	_origin = bottom.pos1 ();
	_vX = bottom.pos3 () - bottom.pos1 (); 
	_vY = bottom.pos4 () - bottom.pos1 ();
	_vZ = _vX.crossProduct (_vY);
	_wX = _vX.module (); _wY = _vY.module (); _wZ = wz;
	_vX.normalize (); _vY.normalize (); _vZ.normalize ();

	calculateInternalPoints ();
}

// ---
QGAMES::Box QGAMES::Box::operator + (const QGAMES::Box& b) const
{
	// Only two boxes with the same orientation can be added
	if (_vX != b._vX || _vY != b._vY || _vZ != b._vZ)
		return (QGAMES::Box::_noBox);

	QGAMES::Position nO (std::min (_origin.posX (), b._origin.posX ()),
						 std::min (_origin.posY (), b._origin.posY ()),
						 std::min (_origin.posZ (), b._origin.posZ ()));

	QGAMES::bdata nWX = (nO.posX () == _origin.posX ()) 
							? b._wX + _vX.dotProduct (b._origin - _origin)
							: _wX + _vX.dotProduct (_origin - b._origin);
	QGAMES::bdata nWY = (nO.posY () == _origin.posY ()) 
							? b._wY + _vY.dotProduct (b._origin - _origin)
							: _wY + _vY.dotProduct (_origin - b._origin);
	QGAMES::bdata nWZ = (nO.posZ () == _origin.posZ ()) 
							? b._wZ + _vZ.dotProduct (b._origin - _origin)
							: _wZ + _vZ.dotProduct (_origin - b._origin);

	return (QGAMES::Box (nO, _vX, _vY, nWX, nWY, nWZ));
}

// ---
QGAMES::Box QGAMES::Box::boxToInclude (const QGAMES::Positions& pos)
{
	if (pos.size () == 0)
		return (QGAMES::Box::_noBox);

	QGAMES::Rectangles result;

	QGAMES::bdata xMax, yMax, zMax;
	QGAMES::bdata xMin, yMin, zMin;
	xMax = yMax = zMax = __MINBDATA__;
	xMin = yMin = zMin = __MAXBDATA__;
	for (QGAMES::Positions::const_iterator i = pos.begin (); i != pos.end (); i++)
	{
		QGAMES::Position p = (*i);

		if (p.posX () > xMax) xMax = p.posX ();
		if (p.posY () > yMax) yMax = p.posY ();
		if (p.posY () > zMax) zMax = p.posZ ();
		if (p.posX () < xMin) xMin = p.posX ();
		if (p.posY () < yMin) yMin = p.posY ();
		if (p.posY () < zMin) zMin = p.posZ ();
	}

	QGAMES::Position org (xMin, yMin, zMin);
	QGAMES::bdata wx = xMax - xMin; 
	QGAMES::bdata wy = yMax - yMin;
	QGAMES::bdata wz = zMax - zMin;
	QGAMES::Vector vx (wx, __BD 0, __BD 0);
	QGAMES::Vector vy (__BD 0, wy, __BD 0);

	return (QGAMES::Box (org, vx, vy, wx, wy, wz));
}

// ---
QGAMES::Box QGAMES::Box::boxToInclude (const QGAMES::Rectangles& rects)
{
	QGAMES::Positions pos;
	for (QGAMES::Rectangles::const_iterator i = rects.begin (); i != rects.end (); i++)
	{ 
		pos.push_back ((*i).pos1 ()); 
		pos.push_back ((*i).pos2 ()); 
		pos.push_back ((*i).pos3 ()); 
		pos.push_back ((*i).pos4 ());
	}

	return (QGAMES::Box::boxToInclude (pos));
}

// ---
QGAMES::Box QGAMES::Box::boxToInclude (const QGAMES::Boxes& bxs)
{
	QGAMES::Rectangles rects;
	for (QGAMES::Boxes::const_iterator i = bxs.begin (); i != bxs.end (); i++)
	{
		rects.push_back ((*i).topRectangle ());
		rects.push_back ((*i).bottomRectangle ());
		rects.push_back ((*i).leftRectangle ());
		rects.push_back ((*i).rightRectangle ());
		rects.push_back ((*i).frontRectangle ());
		rects.push_back ((*i).backRectangle ());
	}

	return (QGAMES::Box::boxToInclude (rects));
}

// ---
void QGAMES::Box::calculateInternalPoints ()
{
	// Bottom face positions (or so)
	QGAMES::Position pD1 = _origin;
	QGAMES::Position pD2 = pD1 + (_wX * _vX) + (_wY * _vY);
	QGAMES::Position pD3 = pD1 + (_wX * _vX);
	QGAMES::Position pD4 = pD1 + (_wY * _vY);

	// Top face positions (or so)
	QGAMES::Position pU1 = _origin + (_wZ * _vZ);
	QGAMES::Position pU2 = pU1 + (_wX * _vX) + (_wY * _vY);
	QGAMES::Position pU3 = pU1 + (_wX * _vX);
	QGAMES::Position pU4 = pU1 + (_wY * _vY);

	// The different internal final faces...
	_rTop   = QGAMES::Rectangle (pU1, pU2, _vZ, __BD 0);
	_rBase  = QGAMES::Rectangle (pD1, pD2, _vZ, __BD 0);
	_rFront = QGAMES::Rectangle (pU4, pD2, _vY, __BD 0); 
	_rBack  = QGAMES::Rectangle (pU1, pD3, _vY, __BD 0); 
	_rRight = QGAMES::Rectangle (pU2, pD3, _vX, __BD 0); 
	_rLeft  = QGAMES::Rectangle (pU1, pD4, _vX, __BD 0);
}
