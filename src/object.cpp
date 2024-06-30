#include <Common/object.hpp>
#include <Common/screen.hpp>

// ---
bool QGAMES::Object::existObjProperty (const std::string& oP) const
{
	return (_properties.find (oP) != _properties.end ());
}

// ---
const std::string& QGAMES::Object::objectProperty (const std::string& oP) const
{
	QGAMES::ObjectProperties::const_iterator i = _properties.find (oP);
	assert (i != _properties.end ());

	return ((*i).second);
}

// ---
QGAMES::ObjectProxy::ObjectProxy (int id, const QGAMES::Position& pos, 
		QGAMES::Object* obj) 
	: QGAMES::Object (id, pos),
	  _object (obj)
{
	assert (_object);
}

// ---
void QGAMES::Image::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	QGAMES::Position pD = p;
	if (pD == QGAMES::Position::_noPoint)
		pD = _position; // If the basic received is not valid, the internal one is used...
	if (_fixPosition)
		pD += s -> position ();
	s -> drawImage (pD, _image, _fade);
}

// ---
void QGAMES::Ellipse::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	QGAMES::Position pD = p;
	if (pD == QGAMES::Position::_noPoint)
		pD = _position; // If the basic received is not valid, the internal one is used...
	if (_fixPosition)
		pD += s -> position ();
	s -> drawCircle (pD, _orientation, _axis1, _axis2, _color);
}

// ---
void QGAMES::GraphicalPolygon::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	QGAMES::Position pD = p;
	if (pD == QGAMES::Position::_noPoint)
		pD = _position; // If the basic received is not valid, the internal one is used...
	if (_fixPosition)
		pD += s -> position ();

	if (_positions.size () <= 1)
		return; // It is needed two points minimum...

	QGAMES::Positions::const_iterator i = _positions.begin ();
	QGAMES::Position p1 = (*i); QGAMES::Position iP = p1;
	do 
	{
		i++;
		if (i != _positions.end ())
		{
			QGAMES::Position p2 = (*i);
			s -> drawLine (pD + p1, pD + p2, _color);
			p1 = p2;
		}
	} while (i != _positions.end ());
	s -> drawLine (pD + p1,	pD + iP, _color); // Close the figure...
}

// ---
void QGAMES::GraphicalPolyline::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	QGAMES::Position pD = p;
	if (pD == QGAMES::Position::_noPoint)
		pD = _position; // If the basic received is not valid, the internal one is used...
	if (_fixPosition)
		pD += s -> position ();

	if (_positions.size () <= 1)
		return; // It is needed 2 points minimum...

	QGAMES::Positions::const_iterator i = _positions.begin ();
	QGAMES::Position p1 = (*i); 
	do 
	{
		i++;
		if (i != _positions.end ())
		{
			QGAMES::Position p2 = (*i);
			s -> drawLine (pD + p1,	pD + p2, _color);
			p1 = p2;
		}
	} while (i != _positions.end ());
}

// ---
void QGAMES::CompositeObject::setFade (int fd)
{
	for (QGAMES::Objects::iterator i = _objects.begin (); i != _objects.end (); i++)
		(*i) -> setFade (fd);
}

// ---
void QGAMES::CompositeObject::initialize ()
{
	for (QGAMES::Objects::iterator i = _objects.begin (); i != _objects.end (); i++)
		(*i) -> initialize ();
	QGAMES::Object::initialize ();
}

// ---
void QGAMES::CompositeObject::inEveryLoop ()
{
	QGAMES::Object::inEveryLoop ();
	for (QGAMES::Objects::iterator i = _objects.begin (); i != _objects.end (); i++)
		(*i) -> inEveryLoop ();
	QGAMES::Object::inEveryLoop ();
}

// ---
void QGAMES::CompositeObject::drawOn (Screen* s, const QGAMES::Position& p)
{
	assert (s);

	for (QGAMES::Objects::iterator i = _objects.begin (); i != _objects.end (); i++)
		(*i) -> drawOn (s);

	QGAMES::Object::drawOn (s, p);
}

// ---
void QGAMES::CompositeObject::finalize ()
{
	for (QGAMES::Objects::iterator i = _objects.begin (); i != _objects.end (); i++)
		(*i) -> finalize ();

	QGAMES::Object::finalize ();
}
