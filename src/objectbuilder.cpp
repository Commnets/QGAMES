#include <Common/objectbuilder.hpp>
#include <Common/definitions.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/genalgorithms.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/game.hpp>

// ---
QGAMES::ObjectBuilder::ObjectBuilder (const std::string& mDef,
		QGAMES::FormBuilder* f)
	: _objects (),
	  _formBuilder (f), // It is optional, but no background object can be built up without it!
	  _definitionFile (mDef),
	  _basePath (QGAMES::onlyPath (mDef)),
	  _alreadyLoaded (false)
{
	assert (_formBuilder);

	if (_basePath == std::string (__NULL_STRING__)) 
		_basePath = std::string (".");
}

// ---
QGAMES::ObjectBuilder::~ObjectBuilder ()
{
	for (std::map <int, QGAMES::Object*>::const_iterator i = _objects.begin ();
			i != _objects.end (); i++)
		delete ((*i).second); // The builder ownw its objects...
	_objects.clear ();
}

// ---
void QGAMES::ObjectBuilder::preLoad ()
{
	if (_alreadyLoaded)
		return;

	readFile (__RL (_definitionFile));

	_alreadyLoaded = true;
}

// ---
QGAMES::Object* QGAMES::ObjectBuilder::object (int nO)
{
	__WLOGPLUS (std::string ("Loading object: ") + std::to_string (nO));

	if (!_alreadyLoaded)
		preLoad (); // If there is no elements already loaded, they are then loaded...

	std::map <int, QGAMES::Object*>::const_iterator i = _objects.find (nO);
	assert (i != _objects.end ());
	return ((*i).second);
}

// ---
QGAMES::GraphicalPolygon* QGAMES::ObjectBuilder::createGraphycalPolygon (int id, 
	const QGAMES::Position& pos, const QGAMES::Positions& ptions, 
	const QGAMES::Color& cl,
	const QGAMES::ObjectProperties& pties)
{
	return (new QGAMES::GraphicalPolygon (id, pos, ptions, cl, pties));
}

// ---
QGAMES::GraphicalPolyline* QGAMES::ObjectBuilder::createGraphycalPolyline (int id, 
	const QGAMES::Position& pos, const QGAMES::Positions& ptions,
	const QGAMES::Color& cl,
	const QGAMES::ObjectProperties& pties)
{
	return (new QGAMES::GraphicalPolyline (id, pos, ptions, cl, pties));
}

// ---
QGAMES::Image* QGAMES::ObjectBuilder::createImage (int id,
	const QGAMES::Position& pos, const std::string& fName, 
	const QGAMES::ObjectProperties& pties)
{
	return (new QGAMES::Image (id, pos, fName, pties));
}

// ---
QGAMES::Background* QGAMES::ObjectBuilder::createBackground (int id,
	const Position& pos, QGAMES::Sprite2D* f, int nT, const ObjectProperties& pties)
{
	return (new QGAMES::Background (id, pos, f, nT, pties));
}

// ---
QGAMES::Ellipse* QGAMES::ObjectBuilder::createEllipse (int id,
	const QGAMES::Position& pos, const QGAMES::Vector& v, QGAMES::bdata r1, 
	QGAMES::bdata r2, const QGAMES::Color& cl, const QGAMES::ObjectProperties& pties)
{
	return (new QGAMES::Ellipse (id, pos, v, r1, r2, cl, pties));
}

// ---
QGAMES::CompositeObject* QGAMES::ObjectBuilder::createComposite (int id, 
	const QGAMES::Position& pos, const QGAMES::Objects& objs, const QGAMES::ObjectProperties& pties)
{
	return (new QGAMES::CompositeObject (id, pos, objs, pties));
}

// ---
void QGAMES::ObjectBuilder::readFile (const std::string& fDef)
{
	__WLOGPLUS (std::string ("Reading objects definition file: ") + fDef);

	TiXmlDocument doc (fDef.c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?
	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the root element doesn't call "Objects" the code will work too!
	// This is done to speed up the loading...

	// Reading all elements...
	for (TiXmlElement* groupElement = rootElement -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_INCLUDENAME__) == 0)
		{
			std::string nF = groupElement -> Attribute (__QGAMES_INCLUDEATTRFILE__);
			readFile (__RL (nF));
		}
		else
		{
			QGAMES::Object* obj = NULL;
			if (strcmp (groupElement -> Value (), __QGAMES_IMAGEBLOCK__) == 0)
				obj = readImage (groupElement); // An image...
			else if (strcmp (groupElement -> Value (), __QGAMES_BACKGROUNDBLOCK__) == 0)
				obj = readBackground (groupElement); // A background...
			else if (strcmp (groupElement -> Value (), __QGAMES_POLYLINEBLOCK__) == 0)
				obj = readGraphicalPolyline (groupElement); // A Polyline...
			else if (strcmp (groupElement -> Value (), __QGAMES_POLYGONBLOCK__) == 0)
				obj = readGraphicalPolygon (groupElement); // A Polygon...
			else if (strcmp (groupElement -> Value (), __QGAMES_ELLIPSEBLOCK__) == 0)
				obj = readEllipse (groupElement); // An ellipse...
			else if (strcmp (groupElement -> Value (), __QGAMES_COMPOSITEBLOCK__) == 0)
				obj = readComposite (groupElement); // A composite made up of all of them...
			assert (obj != NULL); // Can't be null...
			_objects.insert (std::map <int, QGAMES::Object*>::value_type (obj -> id (), obj));
		}
	}
}

// ---
QGAMES::GraphicalPolygon* QGAMES::ObjectBuilder::readGraphicalPolygon (TiXmlElement* e)
{
	int id = -1; 
	std::string color (__NULL_STRING__);
	QGAMES::Positions ptions;
	std::map <std::string, std::string> pties;
	e -> Attribute (__QGAMES_POLYATTRID__, &id);
	color = e -> Attribute (__QGAMES_POLYATTRCOLOR__);
	assert (id != -1);

	for (TiXmlElement* groupElement = e -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_POLYPOINTBLOCK__) == 0)
			ptions.push_back (readPosition (groupElement));
		if (strcmp (groupElement -> Value (), __QGAMES_ATTRSBLOCK__) == 0)
			pties = readPropertiesSection (groupElement); // Only one block of properties...
	}

	// Create the graphical object....
	beforeCreateElement (id);
	QGAMES::GraphicalPolygon* result = createGraphycalPolygon 
		(id, QGAMES::Position::_cero, ptions, QGAMES::Color (color), pties);
	assert (result);
	result -> setExternalEngine (externalEngine ());
	afterCreateElement (result);
	return (result);
}

// ---
QGAMES::GraphicalPolyline* QGAMES::ObjectBuilder::readGraphicalPolyline (TiXmlElement* e)
{
	int id = -1;
	std::string color (__NULL_STRING__);
	QGAMES::Positions ptions;
	std::map <std::string, std::string> pties;
	e -> Attribute (__QGAMES_POLYATTRID__, &id);
	color = e -> Attribute (__QGAMES_POLYATTRCOLOR__);
	assert (id != -1);

	for (TiXmlElement* groupElement = e -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_POLYPOINTBLOCK__) == 0)
			ptions.push_back (readPosition (groupElement));
		if (strcmp (groupElement -> Value (), __QGAMES_ATTRSBLOCK__) == 0)
			pties = readPropertiesSection (groupElement); // Only one block of properties...
	}

	// Create the graphical object....
	beforeCreateElement (id);
	QGAMES::GraphicalPolyline* result = createGraphycalPolyline 
		(id, QGAMES::Position::_cero, ptions, QGAMES::Color (color), pties);
	assert (result);
	result -> setExternalEngine (externalEngine ());
	afterCreateElement (result);
	return (result);
}

// ---
QGAMES::Image* QGAMES::ObjectBuilder::readImage (TiXmlElement* e)
{
	int id = -1;
	std::string form (__NULL_STRING__);
	e -> Attribute (__QGAMES_IMAGEATTRID__, &id);
	form = _basePath + std::string (__PATH_SEPARATOR__) + 
		e -> Attribute (__QGAMES_IMAGEATTRFORM__);
	std::map <std::string, std::string> pties;
	assert (id != -1);

	// Read the attributes...if any
	for (TiXmlElement* groupElement = e -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
		if (strcmp (groupElement -> Value (), __QGAMES_ATTRSBLOCK__) == 0)
			pties = readPropertiesSection (groupElement); // Only one block of properties...

	// Create the image object and return...
	beforeCreateElement (id);
	QGAMES::Image* result = createImage (id, QGAMES::Position::_cero, form, pties);
	assert (result);
	result -> setExternalEngine (externalEngine ());
	afterCreateElement (result);
	return (result);
}

// ---
QGAMES::Background* QGAMES::ObjectBuilder::readBackground (TiXmlElement* e)
{
	// To create a background a reference to the form builder 
	// ...has to be set up...
	assert (_formBuilder);

	// Gets the id, it has to exist!
	int id = -1;
	e -> Attribute (__QGAMES_BKATTRID__, &id);
	assert (id != -1);

	// Gets the form...and it has to be an sprite2D!
	QGAMES::Sprite2D* frm = dynamic_cast <QGAMES::Sprite2D*> 
		(_formBuilder -> form (std::stoi (e -> Attribute (__QGAMES_BKATTRFORM__))));
	assert (frm);

	// Gets the number of times the background is repeted
	// This parameter is not mandatory!
	int nT = 1; // By default!
	e -> Attribute (__QGAMES_BKATTRTIMES__, &nT);

	// Read the attributes...if any
	std::map <std::string, std::string> pties;
	for (TiXmlElement* groupElement = e -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
		if (strcmp (groupElement -> Value (), __QGAMES_ATTRSBLOCK__) == 0)
			pties = readPropertiesSection (groupElement); // Only one block of properties...

	// Create the image object and return...
	beforeCreateElement (id);
	QGAMES::Background* result = createBackground (id, QGAMES::Position::_cero, frm, nT, pties);
	assert (result);
	result -> setExternalEngine (externalEngine ());
	afterCreateElement (result);
	return (result);
}

// ---
QGAMES::Ellipse* QGAMES::ObjectBuilder::readEllipse (TiXmlElement* e)
{
	int id = -1;
	double vX, vY, vZ, r1, r2;
	vX = vY = vZ = r1 = r2 = 0.0;
	std::string color (__NULL_STRING__);
	std::map <std::string, std::string> pties;
	e -> Attribute (__QGAMES_ELLIPSEATTRID__, &id);
	e -> Attribute (__QGAMES_ELLIPSEATTRVX__, &vX);
	e -> Attribute (__QGAMES_ELLIPSEATTRVY__, &vY);
	e -> Attribute (__QGAMES_ELLIPSEATTRVZ__, &vZ);
	e -> Attribute (__QGAMES_ELLIPSEATTRR1__, &r1);
	e -> Attribute (__QGAMES_ELLIPSEATTRR2__, &r2);
	color = e -> Attribute (__QGAMES_ELLIPSEATTRCOLOR__);
	assert (id != -1);

	// Read the attributes...if any
	for (TiXmlElement* groupElement = e -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
		if (strcmp (groupElement -> Value (), __QGAMES_ATTRSBLOCK__) == 0)
			pties = readPropertiesSection (groupElement);

	// Creates the ellipse and returns...
	beforeCreateElement (id);
	QGAMES::Ellipse* result = createEllipse (id, QGAMES::Position::_cero, QGAMES::Vector (__BD vX, __BD vY, __BD vZ),
		__BD r1, __BD r2, QGAMES::Color (color), pties);
	assert (result);
	result -> setExternalEngine (externalEngine ());
	afterCreateElement (result);
	return (result);
}

// ---
QGAMES::CompositeObject* QGAMES::ObjectBuilder::readComposite (TiXmlElement* e)
{
	int id = -1;
	double x, y, z;
	x = y = x = 0.0;
	QGAMES::Objects objs;
	std::map <std::string, std::string> pties;
	e -> Attribute (__QGAMES_COMPOSITEATTRID__, &id);
	e -> Attribute (__QGAMES_COMPOSITEATTRX__, &x);
	e -> Attribute (__QGAMES_COMPOSITEATTRY__, &y);
	e -> Attribute (__QGAMES_COMPOSITEATTRZ__, &z);
	assert (id != -1);

	// Read the elements this composite is made up of
	for (TiXmlElement* groupElement = e -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		// One element...
		if (strcmp (groupElement -> Value (), __QGAMES_ELEMENTSBLOCK__) == 0)
		{
			int idElmnt = -1;
			QGAMES::Object* obj = NULL;
			groupElement -> Attribute (__QGAMES_COMPOSITEATTRID__, &idElmnt);
			std::map <int, QGAMES::Object*>::const_iterator i = _objects.find (idElmnt);
			assert (i != _objects.end ());
			objs.push_back ((*i).second); // The composite doesn't own its objects...
		}

		// The attributes zone...
		// There should be one zone of this maximum (as DTD definition)
		// If there is more than one, then the properties will be relaced
		if (strcmp (groupElement -> Value (), __QGAMES_ATTRSBLOCK__) == 0)
			pties = readPropertiesSection (groupElement); // Only one block of properties...
	}

	// Creates the composite object and returns...
	beforeCreateElement (id);
	QGAMES::CompositeObject* result = createComposite (id, QGAMES::Position (__BD x, __BD y, __BD z), objs, pties);
	result -> setExternalEngine (externalEngine ());
	afterCreateElement (result);
	return (result);
}

// ---
std::map <std::string, std::string> QGAMES::ObjectBuilder::readPropertiesSection (TiXmlElement* e)
{
	std::map <std::string, std::string> result;
	for (TiXmlElement* defElement = e -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		// If the element doesn't call "Atribute" the code will work too!
		// This is done to speed up the loading...

		std::string id (__NULL_STRING__);  std::string v (__NULL_STRING__);
		id = defElement -> Attribute (__QGAMES_ATTRSATTRID__);
		v = defElement -> Attribute (__QGAMES_ATTRSATTRVALUE__);
		assert (id != std::string (__NULL_STRING__) && v != std::string (__NULL_STRING__)); // Right?...
		result.insert (std::map <std::string, std::string>::value_type (id, v));
	}

	// Returns the list of properties created...
	return (result);
}

// ---
QGAMES::Position QGAMES::ObjectBuilder::readPosition (TiXmlElement* e)
{
	double x, y, z;
	x = y = z = 0.0;
	e -> Attribute (__QGAMES_POLYPTATTRX__, &x);
	e -> Attribute (__QGAMES_POLYPTATTRX__, &y);
	e -> Attribute (__QGAMES_POLYPTATTRX__, &z);
	return (QGAMES::Position (__BD x, __BD y, __BD z));
}

