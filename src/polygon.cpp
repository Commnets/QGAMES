#include <Common/polygon.hpp>
#include <vector>

// ---
QGAMES::Position QGAMES::Polygon::point (int p) const
{
	if (p < 0 || p >= (int)_points.size ())
		return (QGAMES::Position (0,0,0));

	int j = 0;
	QGAMES::Positions::const_iterator i = _points.begin ();
	while (j++ < p) i++;
	return ((*i));
}

// ---
bool QGAMES::Polygon::hasIn (const QGAMES::Position& pos) const
{
	// Impossible? to calculate when the number of points of
	// The polygon is less than 3...
	if (_points.size () < 3)
		return (false);

	// Verify first all points are in the same plane...
	// It would be possible to calculate simplfy whether a point is or not in a polygin
	// when th polygon has all points in the same plane....
	bool samePlane = true;
	std::vector <QGAMES::Position> pV;
	pV.reserve (_points.size () + 1);
	std::copy (std::begin (_points), std::end (_points), std::back_inserter (pV));
	pV [_points.size ()] = pV [0]; // The polygons starts in the same point it finishes!!
	QGAMES::Vector nV = pV [0].crossProduct (pV [1]).normalize ();
	for (int i = 1; i < (int) (pV.size () - 1) && samePlane; i++)
	{
		QGAMES::Vector  nnV = pV [i].crossProduct (pV [i + 1]).normalize ();
		if (nnV != nV && nnV != (nV * (-1)))
			samePlane = false;
	}

	if (!samePlane)
		return (false);

	// Then, now it is time to calculate if the point is also in the same plane
	// and in the polygon: 
	// Adding all angles formed between the vectors joining two consecutive
	// corners and the suppossed point in the middle of the polygon, must be
	// 2PI...
	QGAMES::Vector v1 = pos - pV [0];
	QGAMES::Vector v2 = pos - pV [1];
	QGAMES::bdata ag = v1.angleWith (v2);
	for (int i = 1; i < (int) (pV.size () - 1); i++)
		ag += (pos - pV [i]).angleWith (pos - pV [i + 1]);
	return (ag == 2 * __PI); // Guay!!
}

// ---
QGAMES::Polygon QGAMES::Polygon::rotate (const QGAMES::Position& e1, 
	const QGAMES::Position& e2, QGAMES::bdata a) const
{
	// If no angle... no rotation
	if (a == __BD 0)
		return (*this);

	QGAMES::Positions pts;
	for (QGAMES::Positions::const_iterator i = _points.begin ();
			i != _points.end (); i++)
		pts.push_back ((*i).rotate (e1, e2, a));
	return (QGAMES::Polygon (pts));
}

// ---
QGAMES::Polygon QGAMES::Polygon::projectOver (const QGAMES::Position& c, const QGAMES::Vector& o) const
{
	QGAMES::Positions pts;
	for (QGAMES::Positions::const_iterator i = _points.begin ();
			i != _points.end (); i++)
		pts.push_back ((*i).projectOver (c, o));
	return (QGAMES::Polygon (pts));
}

// ---
bool QGAMES::Polygon::operator == (const QGAMES::Polygon& p) const
{
	if (numberPoints () != p.numberPoints ())
		return (false);
	if (numberPoints () == 0)
		return (true);
	if (numberPoints () == 1)
		return ((*_points.begin ()) == (*p._points.begin ()));

	bool foundFirst = false;
	QGAMES::Positions::const_iterator i = _points.begin ();
	QGAMES::Positions::const_iterator j = p._points.begin ();
	for (; j != p._points.end () && foundFirst; j++)
		if ((*j) == (*i))
			foundFirst = true;
	if (!foundFirst)
		return (false);

	i++;
	if (j == p._points.end ())
		j = p._points.begin ();
	bool result = true;
	while (i != _points.end () && result)
	{
		if ((*i) != (*j))
			result = false;
		if (++j == p._points.end ())
			j = p._points.begin ();
		i++;
	}

	return (result);
}

// ---
QGAMES::Polygon QGAMES::Polygon::operator + (const QGAMES::Position& p) const
{
	QGAMES::Positions pts;
	for (QGAMES::Positions::const_iterator i = _points.begin ();
			i != _points.end (); i++)
		pts.push_back ((*i) + p);
	return (QGAMES::Polygon (pts));
}

// ---
QGAMES::Polygon QGAMES::Polygon::operator - (const QGAMES::Position& p) const
{
	QGAMES::Positions pts;
	for (QGAMES::Positions::const_iterator i = _points.begin ();
			i != _points.end (); i++)
		pts.push_back ((*i) - p);
	return (QGAMES::Polygon (pts));
}

// ---
std::ostream& QGAMES::operator << (std::ostream& o, const QGAMES::Polygon& r)
{
	bool first = true;
	for (QGAMES::Positions::const_iterator i = r._points.begin ();
			i != r._points.end (); i++)
	{
		if (!first) o << "|";
		first = false;
		o << (*i);
	}

	return (o);
}
