#include <Arcade/mapbuilder.hpp>
#include <Arcade/definitions.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/genalgorithms.hpp>
#include <Common/resourcesreader.hpp>
#include <Common/game.hpp>
#include <iostream>

// ---
QGAMES::MapBuilder::MapBuilder (const std::string& mDef)
	: _definitions (),
	  _setOfDefinitions (),
	  _maps (),
	  _definitionFile (mDef),
	  _basePath (QGAMES::onlyPath (mDef))
{ 
	if (_basePath == std::string (__NULL_STRING__)) 
		_basePath = std::string (".");

	readFile (__RL (_definitionFile));
	// Because there can be include line, this method can be called recursevely...
	// At this point the definition file, has the file to read,
	// and the list of definitions is empty...
}

// ---
QGAMES::MapBuilder::~MapBuilder ()
{
	// Deletes the maps...
	std::map <int, Map*>::iterator i;
	for (i = _maps.begin (); i != _maps.end (); i++)
		delete ((*i).second);
	_maps.clear ();
	
	// ..and the adds on...
	QGAMES::MapBuilder::AddsOnList::const_iterator j;
	for (j = _addsOn.begin (); j != _addsOn.end (); j++)
		delete ((*j));
	_addsOn.clear ();
}

// ---
int QGAMES::MapBuilder::numberResources () const
{ 
	int result = (int) _definitions.size ();
	for (auto i : _setOfDefinitions)
		result += i.second._number; // In the sets it can be greater than 1...
	return (result);
}

// ---
std::vector <int> QGAMES::MapBuilder::resourcesId () const
{
	std::vector <int> result;

	for (auto i : _definitions)
		result.push_back (i.first);

	for (auto i : _setOfDefinitions)
	{
		for (int j = 0; j < i.second._number; j++)
			result.push_back (i.first + j);
	}

	return (result);
}

// ---
void QGAMES::MapBuilder::addAddsOn (QGAMES::MapBuilder::AddsOn *aO)
{
	assert (aO); // It has to be not null...

	bool found = false;
	for (QGAMES::MapBuilder::AddsOnList::const_iterator i = _addsOn.begin ();
			i != _addsOn.end () && !found; i++)
		if ((*i) == aO)
			found = true;
	assert (!found); // It can't exist already!

	_addsOn.push_back (aO); 
}

// ---
void QGAMES::MapBuilder::preLoad ()
{
	for (QGAMES::MapBuilder::Definitions::const_iterator i = _definitions.begin ();
			i != _definitions.end (); i++)
		if(_maps.find ((*i).second._id) == _maps.end ())
			map ((*i).second._id);
}

// ---
QGAMES::Map* QGAMES::MapBuilder::map (int nm)
{
	QGAMES::Maps::const_iterator i = _maps.find (nm);
	if (i != _maps.end ())
		return ((*i).second);

	__WLOGPLUS (std::string ("Loading map: ") + std::to_string (nm));

	// Look for the id of the map...
	// It can be part of a set of maps...
	QGAMES::MapBuilder::MapDefinition mpDef; // Inially invalid
	QGAMES::MapBuilder::Definitions::const_iterator j = _definitions.find (nm);
	if (j != _definitions.end ())
		mpDef = (*j).second;
	else
	{
		for (auto k : _setOfDefinitions)
		{
			if (nm >= k.second._id && nm < (k.second._id + k.second._number))
			{
				mpDef = k.second;
				mpDef._id = nm;
				mpDef._number = 1;
				mpDef._typeBlock = QGAMES::MapBuilder::MapDefinition::TypeBlock::_SIMPLE;
			}
		}
	}

	// and it has to be found...
	assert (mpDef._id != -1);

	// Looks for the right addOn to create the map,
	// and create the map...
	QGAMES::MapBuilder::AddsOn* addOn = NULL;
	for (QGAMES::MapBuilder::AddsOnList::const_iterator k = _addsOn.begin ();
			k != _addsOn.end () && !addOn; k++)
		if ((*k) -> canCreateMap (mpDef._type))
			addOn = (*k);
	assert (addOn); // An addsOn for this type of maps has to exist...

	beforeCreateElement (nm);
	addOn -> beforeCreatingMap (mpDef); // Before going to create the map
	QGAMES::Map* result = addOn -> createMap (mpDef, _basePath);
	assert (result); // createMap is an user's method...
	result -> setExternalEngine (externalEngine ());
	addOn -> afterCreatingMap (result); // After the creation of the map
	afterCreateElement (result);

	_maps.insert (QGAMES::Maps::value_type (nm, result));
	return (result); // Finally it is returned...
}

// ---
void QGAMES::MapBuilder::readFile (const std::string& fDef)
{
	__WLOGPLUS (std::string ("Reading maps definition file: ") + fDef);

	TiXmlDocument doc (fDef.c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	// If the root element doesn't call "Maps" the code will work too!
	// This is done to speed up the loading...

	for (TiXmlElement* defElement = rootElement -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		// The line can be an include line...
		// That line can be at any place in the map definition file...
		if (strcmp (defElement -> Value (), __QGAMES_INCLUDENAME__) == 0)
		{
			assert (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)); 
			readFile (__RL (defElement -> Attribute (__QGAMES_INCLUDEATTRFILE__)));
		}
		else
		// Other type of line could be a simple map line...
		if (strcmp (defElement -> Value (), __QGAMES_MAPTAGNAME__) == 0)
		{
			QGAMES::MapBuilder::MapDefinition def = readMapDefinition (defElement);
			assert (def._id != -1); // Is it a valid entity?
			_definitions.insert (QGAMES::MapBuilder::Definitions::value_type (def._id, def));
		}
		// Or a et of them...
		else
		if (strcmp (defElement -> Value (), __QGAMES_SETOFMAPSTAGNAME__) == 0)
		{
			QGAMES::MapBuilder::MapDefinition def = readSetOfMapsDefinition (defElement);
			assert (def._id != -1 && def._number > 0); // Is it a valid entity?
			_setOfDefinitions.insert (QGAMES::MapBuilder::Definitions::value_type (def._id, def));
		}
	}
}

// ---
QGAMES::MapBuilder::MapDefinition QGAMES::MapBuilder::readMapDefinition (TiXmlElement* e)
{
	QGAMES::MapBuilder::MapDefinition result;
	e -> Attribute (__QGAMES_MAPSATTRID__, &result._id);
	e -> Attribute (__QGAMES_MAPSATTRTYPE__, &result._type);
	result._typeBlock = QGAMES::MapBuilder::MapDefinition::TypeBlock::_SIMPLE;
	result._definition  = _basePath + std::string (__PATH_SEPARATOR__) + 
		e -> Attribute (__QGAMES_MAPSATTRDEFFILE__);
	return (result);	
}

// ---
QGAMES::MapBuilder::MapDefinition QGAMES::MapBuilder::readSetOfMapsDefinition (TiXmlElement* e)
{
	QGAMES::MapBuilder::MapDefinition result;
	e -> Attribute (__QGAMES_MAPSATTRBASEID__, &result._id);
	e -> Attribute (__QGAMES_MAPSATTRNUMBER__, &result._number);
	e -> Attribute (__QGAMES_MAPSATTRTYPE__, &result._type);
	result._typeBlock = QGAMES::MapBuilder::MapDefinition::TypeBlock::_SET; 
	result._definition  = _basePath + std::string (__PATH_SEPARATOR__) + 
		e -> Attribute (__QGAMES_MAPSATTRDEFFILE__);
	return (result);	
}

// ---
QGAMES::Ellipse* QGAMES::TMXMapBuilderAddsOn::createEllipse (int id, 
	const QGAMES::Position& p, const QGAMES::Vector& o, QGAMES::bdata a1, QGAMES::bdata a2,
	const ObjectProperties& oPrp)
{
	return (new QGAMES::Ellipse (id, p, o, a1, a2, QGAMES::Color (0,0,0), oPrp));
}

// ---
QGAMES::GraphicalPolygon* QGAMES::TMXMapBuilderAddsOn::createPolygon (int id, const QGAMES::Positions& pts, 
		const QGAMES::Position& p, const ObjectProperties& oPrp)
{
	return (new QGAMES::GraphicalPolygon (id, p, pts, QGAMES::Color (0,0,0), oPrp));
}

// ---
QGAMES::GraphicalPolyline* QGAMES::TMXMapBuilderAddsOn::createPolyline (int id, const QGAMES::Positions& pts, 
		const QGAMES::Position& p, const ObjectProperties& oPrp)
{
	return (new QGAMES::GraphicalPolyline (id, p, pts, QGAMES::Color (0,0,0), oPrp));
}

// ---
QGAMES::Map* QGAMES::TMXMapBuilderAddsOn::createMap (const QGAMES::MapBuilder::MapDefinition& def,
	const std::string& bP)
{
	assert (def._type == __QGAMES_TMXMAP__); // It should be obvious...

	_basePath = bP;
	Tmx::Map *map = new Tmx::Map ();
	map -> ParseFile (__RL (def._definition));
	assert (!map -> HasError ()); // Is the map file well defined?...

	// The tile map supported are:
	// ORTHOGONAL, no limits at the size of the tiles
	// ISOMETRIC when tile width == tile height * 2
	assert (map -> GetOrientation () == Tmx::MapOrientation::TMX_MO_ORTHOGONAL ||
			(map -> GetOrientation () == Tmx::MapOrientation::TMX_MO_ISOMETRIC && 
				map -> GetTileWidth () == (map -> GetTileHeight () << 1)));

	QGAMES::Forms forms;
	for (int a = 0; a < map -> GetNumTilesets (); a++)
	{
		QGAMES::Form* form = createFormFrom (map -> GetTileset (a), map -> GetOrientation ());
		// It is not needed to erify whether the form is or not right, 
		// because the method to create it can't be overloaded by the user...
		forms.insert (QGAMES::Forms::value_type (form -> id (), form));
	}

	// The layers habe to be created in the same order they are defined...
	std::map <int, QGAMES::Layer*> layersMap;
	QGAMES::Layers layers;
	for (int b = 0; b < map -> GetNumLayers (); b++)
		layersMap.insert (std::map <int, QGAMES::Layer*>::value_type 
			(map -> GetLayer (b) -> GetZOrder (), createTileLayerFrom (map -> GetLayer (b), map, forms)));
	for (int c = 0; c < map -> GetNumObjectGroups (); c++)
		layersMap.insert (std::map <int, QGAMES::Layer*>::value_type 
			(map -> GetObjectGroup (c) -> GetZOrder (), createObjectLayerFrom (map -> GetObjectGroup (c), forms)));
	for (int d = 0; d < map -> GetNumImageLayers (); d++)
		layersMap.insert (std::map <int, QGAMES::Layer*>::value_type 
			(map -> GetImageLayer (d) -> GetZOrder (), createImageLayerFrom (map -> GetImageLayer (d), forms)));
	for (std::map <int, QGAMES::Layer*>::const_iterator i = layersMap.begin ();
			i != layersMap.end (); i++)
		layers.push_back ((*i).second);
	// After they have been arranged into a map, the are copied into a list
	// The list is the official array object used to draw the layers...

	QGAMES::Map* result = 
		createMapObject (def._id, layers, map -> GetWidth (), map -> GetHeight (), 0,
			map -> GetTileWidth (), map -> GetTileHeight (), 0, map -> GetProperties ().GetList ());
	assert (result);

	// The map is assigned later setMap methiod is virtual to introduce (if wanted) verifications in the middle
	for (QGAMES::Layers::const_iterator i = layers.begin (); i != layers.end (); i++)
		(*i) -> setMap (result);

	delete (map); // No longer used...

	return (result);
}

// ---
QGAMES::Form* QGAMES::TMXMapBuilderAddsOn::createFormFrom (const Tmx::Tileset* tS, Tmx::MapOrientation o)
{
	assert (tS);

	std::string fDef = _basePath + std::string (__PATH_SEPARATOR__) + tS -> GetImage () -> GetSource (); 

	// Verifies whether there is already a form created with the same name and with the same definition file
	// If it is the case, the form already created is returned.
	// When there is a lot of TMX maps defined ovr the same type of tiles, it saves a lot of memory
	// and loading the game is a little bit quicker!
	int fP = __MININT__;
	QGAMES::TMXMapBuilderAddsOn::CreatedForms::const_iterator i;
	if ((i = _createdForms.find (tS -> GetName ())) != _createdForms.end ())
	{
		std::map <std::string, int>::const_iterator j;
		if ((j = (*i).second.find (fDef)) != (*i).second.end ())
			fP = (*j).second;
	}

	if (fP != __MININT__)
		return (_formBuilder -> form (fP));

	__WLOGPLUS (std::string ("Defining form: ") + std::to_string (_lastFormIdCreated));
	
	int nFH = (tS -> GetImage () -> GetWidth () + 1 - (2 * tS -> GetMargin ())) / 
		(tS -> GetTileWidth () + tS -> GetSpacing ());
	int nFV = (tS -> GetImage () -> GetHeight () + 1 - (2 * tS -> GetMargin ())) / 
		(tS -> GetTileHeight () + tS -> GetSpacing ());

	QGAMES::Form* result = (_formBuilder) -> formDefined 
		(_lastFormIdCreated, tS -> GetName (), nFH * nFV,
		 fDef, 
		 tS -> GetImage () -> GetWidth (), tS -> GetImage () -> GetHeight (),
		 tS -> GetTileWidth (), tS -> GetTileHeight (), tS -> GetSpacing (), tS -> GetMargin (), 
		 255 /** Full color */, 
		 (o == Tmx::MapOrientation::TMX_MO_ORTHOGONAL) ? 0 /** Orthogonla */ : 1 /** Isometric. */,
		 1 /** squared in the x y axis when isometric or cavalier. No used in orthogonal. */, 
		 QGAMES::ListOfSprite2DFrameData (), tS -> GetProperties ().GetList ());

	_createdForms [tS -> GetName ()][fDef] = _lastFormIdCreated;

	_lastFormIdCreated++;

	return (result);
}

// ---
QGAMES::TileLayer* QGAMES::TMXMapBuilderAddsOn::createTileLayerFrom (const Tmx::Layer* l, 
	const Tmx::Map* m, const QGAMES::Forms& f)
{
	assert (l);
	assert (m);

	QGAMES::Tiles tiles;
	std::string lastTsName (__NULL_STRING__);
	QGAMES::Form* frm = NULL;
	for (int bY = 0; bY < l -> GetHeight (); bY++)
	{
		for (int bX = 0; bX < l -> GetWidth (); bX++)
		{
			QGAMES::Tile* tile = NULL;
			if (l -> GetTileTilesetIndex (bX, bY) == -1) // The tile is empty...
				tile = /** new QGAMES::NullTile (_lastTileIdCreated++); Instead creating a new one... */
					QGAMES::NullTile::_nullTile; // Just to reuse a default one an avoid memory consume!
			else
			{
				const Tmx::Tileset* tS = 
					m -> GetTileset (l -> GetTileTilesetIndex (bX, bY));
				int tId = l -> GetTileId (bX, bY);
				QGAMES::TileProperties p = (tS -> GetTile (tId) != NULL) 
						? (tS -> GetTile (tId) -> GetProperties ().GetList ()) 
						: QGAMES::TileProperties ();
				if (tS -> GetName () != lastTsName) 
				{
						lastTsName = tS -> GetName ();
						frm = findForm (tS -> GetName (), f);
						assert (frm);
				}

				tile = createTile (_lastTileIdCreated++, frm, tId, p);
				assert (tile);
			}

			tiles.push_back (tile);
		}
	}

	QGAMES::TileLayer::VisualOrientation o = QGAMES::TileLayer::VisualOrientation::_ISOMETRIC;
	if (m -> GetOrientation () == Tmx::TMX_MO_ORTHOGONAL) 
		o = QGAMES::TileLayer::VisualOrientation::_ORTHOGONAL;
	QGAMES::TileLayer* result = 
		createTileLayer (_lastTileLayerIdCreated++, l -> GetName (), tiles, NULL, // The map...
			o, l -> GetProperties ().GetList ());
	assert (result);
	// The offset in the y aspect is set into the z ccordinate...
	result -> setPosition (QGAMES::Position (
				__BD (l -> GetOffsetX ()), 
				(o == QGAMES::TileLayer::VisualOrientation::_ORTHOGONAL) ? __BD (l -> GetOffsetY ()) : __BD 0, 
				(o == QGAMES::TileLayer::VisualOrientation::_ISOMETRIC) ? __BD (l -> GetOffsetY ()) : __BD 0));
	result -> setFade ((int) (l -> GetOpacity () * 255.0));

	return (result);
}

// ---
QGAMES::ObjectLayer* QGAMES::TMXMapBuilderAddsOn::createObjectLayerFrom (const Tmx::ObjectGroup* oG, const QGAMES::Forms& f)
{
	assert (oG);

	QGAMES::Objects objs;
	for (int i = 0; i < oG -> GetNumObjects (); i++)
	{
		const Tmx::Object* obj = oG -> GetObject (i);
		if (obj -> GetEllipse () != NULL)
		{
			const Tmx::Ellipse* e = obj -> GetEllipse ();
			QGAMES::Ellipse* ell = createEllipse (_lastObjectCreated++,
				QGAMES::Position ((QGAMES::bdata) e -> GetCenterX (), 
					(QGAMES::bdata) e -> GetCenterY (), (QGAMES::bdata) 0), QGAMES::Vector::_zNormal,
						(QGAMES::bdata) e -> GetRadiusX (), (QGAMES::bdata) e -> GetRadiusY (), 
							obj -> GetProperties ().GetList ());
			assert (ell);
			objs.push_back (ell); 
			// In a TMX Map ellipses are perpedicular to normal plane...
		}
		else
		if (obj -> GetPolygon () != NULL)
		{
			// It is a polygon, made up of pints...
			const Tmx::Polygon* pl = obj -> GetPolygon ();
			QGAMES::Positions pts;
			for (int j = 0; j < pl -> GetNumPoints (); j++)
				pts.push_back (QGAMES::Position ((QGAMES::bdata) pl -> GetPoint (j).x,
					(QGAMES::bdata) pl -> GetPoint (j).y, (QGAMES::bdata) 0));
			QGAMES::GraphicalPolygon* pgon = createPolygon (_lastObjectCreated++, 
				pts, QGAMES::Position ((QGAMES::bdata) obj -> GetX (),
					(QGAMES::bdata) obj -> GetY (), (QGAMES::bdata) 0), obj -> GetProperties ().GetList ());
			assert (pgon);
			objs.push_back (pgon); // In a TMX map there is no z coordinate...
		}
		else
		if (obj -> GetPolyline () != NULL)
		{
			// It is a polygon, made up of pints...
			const Tmx::Polyline* pl = obj -> GetPolyline ();
			QGAMES::Positions pts;
			for (int j = 0; j < pl -> GetNumPoints (); j++)
				pts.push_back (QGAMES::Position ((QGAMES::bdata) pl -> GetPoint (j).x,
					(QGAMES::bdata) pl -> GetPoint (j).y, (QGAMES::bdata) 0));
			QGAMES::GraphicalPolyline* pline = createPolyline (_lastObjectCreated++, 
				pts, QGAMES::Position ((QGAMES::bdata) obj -> GetX (),
					(QGAMES::bdata) obj -> GetY (), (QGAMES::bdata) 0), obj -> GetProperties ().GetList ());
			assert (pline);
			objs.push_back (pline); // In a TMX map there is no z coordinate...
		}
		else
		{
			// It is a rectangle...
			QGAMES::Positions pts;
			QGAMES::Position pt;
			pt = QGAMES::Position ((QGAMES::bdata) obj -> GetX (), 
				(QGAMES::bdata) obj -> GetY (), (QGAMES::bdata) 0); pts.push_back (pt);
			pt = QGAMES::Position ((QGAMES::bdata) obj -> GetX () + obj -> GetWidth (), 
				(QGAMES::bdata) obj -> GetY (), (QGAMES::bdata) 0); pts.push_back (pt);
			pt = QGAMES::Position ((QGAMES::bdata) obj -> GetX () + obj -> GetWidth (), 
				(QGAMES::bdata) obj -> GetY () + obj -> GetHeight (), (QGAMES::bdata) 0); pts.push_back (pt);
			pt = QGAMES::Position ((QGAMES::bdata) obj -> GetX (), 
				(QGAMES::bdata) obj -> GetY () + obj -> GetHeight (), (QGAMES::bdata) 0); pts.push_back (pt);
			QGAMES::GraphicalPolygon* pgon = createPolygon (_lastObjectCreated++,
				pts, QGAMES::Position::_cero, obj -> GetProperties ().GetList ());
			assert (pgon);
			objs.push_back (pgon);
		}
	}

	QGAMES::ObjectLayer* result =
		createObjectLayer (_lastObjectLayerCreated++,
			oG -> GetName (), objs, NULL, oG -> GetProperties ().GetList ());
	assert (result);

	return (result);
}

// ---
QGAMES::ImageLayer* QGAMES::TMXMapBuilderAddsOn::createImageLayerFrom (const Tmx::ImageLayer* l, const QGAMES::Forms& f)
{
	assert (l);

	QGAMES::Image* img =
		new QGAMES::Image (_lastObjectCreated++, QGAMES::Position::_cero,
			l -> GetImage () -> GetSource (), QGAMES::ObjectProperties ());
	QGAMES::ImageLayer* result =
		createImageLayer (_lastImageLayerCreated++, l -> GetName (), 
			img, NULL, l -> GetProperties ().GetList ());
	assert (result);
	result -> setFade ((int) (l -> GetOpacity () * 255.0));

	return (result);
}

// ---
QGAMES::Form* QGAMES::TMXMapBuilderAddsOn::findForm (const std::string& n, const Forms& f)
{
	QGAMES::Form* result = NULL;
	for (Forms::const_iterator i = f.begin (); i != f.end () && !result; i++)
		if ((*i).second -> name () == n)
			result = (*i).second;
	return (result);
}
// ---
QGAMES::Map* QGAMES::ObjectMapBuilderAddsOn::createMap (const QGAMES::MapBuilder::MapDefinition& def,
	const std::string& bP)
{
	_basePath = bP; // Always it is overloaded...
	TiXmlDocument doc (__RL (def._definition).c_str ());
	int e = doc.LoadFile ();
	assert (e); // Is it a valid doc?

	TiXmlElement* rootElement = doc.RootElement ();
	assert (rootElement); // One element minimum...

	int id = -1;
	int width, height, depth; // In pixels...
	width = height = depth = 0;
	QGAMES::Objects objects;
	QGAMES::Layers layers;
	QGAMES::MapProperties properties;
	rootElement -> Attribute (__QGAMES_OBJECTMAPATTRNAME__, &id);
	rootElement -> Attribute (__QGAMES_OBJECTMAPATTRWIDTH__, &width);
	rootElement -> Attribute (__QGAMES_OBJECTMAPATTRHEIGHT__, &height);
	rootElement -> Attribute (__QGAMES_OBJECTMAPATTRDEPTH__, &depth); // If nothing is defined it will be 0!
	assert (id != -1);

	// Read the different parts of the definition file...
	for (TiXmlElement* groupElement = rootElement -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_OBJECTMAPLAYERSBLOCK__) == 0)
			layers = readLayersSection (groupElement);
		if (strcmp (groupElement -> Value (), __QGAMES_OBJECTMAPATTRSBLOCK__) == 0)
			properties = readPropertiesSection (groupElement);
	}

	// Creates the map, assign the map to the different layers, and return...
	QGAMES::Map* result = createMap (id, width, height, depth, layers, properties);
	assert (result);

	// The map is assigned later setMap methiod is virtual to introduce (if wanted) verifications in the middle
	for (QGAMES::Layers::const_iterator i = layers.begin (); i != layers.end (); i++)
		(*i) -> setMap (result);

	return (result);
}

// ---
QGAMES::Map* QGAMES::ObjectMapBuilderAddsOn::createMap (int id, int w, int h, int d,
	const QGAMES::Layers& l, const QGAMES::MapProperties& pties)
{
	return (new QGAMES::ObjectMap (id, l, w, h, d, pties));
}

// ---
QGAMES::Layer* QGAMES::ObjectMapBuilderAddsOn::createLayer (int id, const std::string& name, 
	QGAMES::Objects& objs, QGAMES::Map* m, const QGAMES::LayerProperties& lP)
{
	return (new QGAMES::ObjectLayer (id, name, objs, m, lP));
}

// ---
QGAMES::Layers QGAMES::ObjectMapBuilderAddsOn::readLayersSection (TiXmlElement* e)
{
	assert (e);

	QGAMES::Layers result;
	for (TiXmlElement* groupElement = e -> FirstChildElement ();
			groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
		result.push_back (readLayer (groupElement));
	return (result);
}

// ---
std::map <std::string, std::string> QGAMES::ObjectMapBuilderAddsOn::readPropertiesSection (TiXmlElement* e)
{
	assert (e); 

	std::map <std::string, std::string> result;
	for (TiXmlElement* defElement = e -> FirstChildElement ();
		defElement != NULL; defElement = defElement -> NextSiblingElement ())
	{
		// It the element doesn't call "Property" it doesn't matter, the system continues...

		std::string id (__NULL_STRING__);  std::string v (__NULL_STRING__);
		id = defElement -> Attribute (__QGAMES_OBJECTMAPATTRSATTRID__);
		v = defElement -> Attribute (__QGAMES_OBJECTMAPATTRSATTRVALUE__);
		assert (id != std::string (__NULL_STRING__) && v != std::string (__NULL_STRING__)); // Right?...
		result.insert (std::map <std::string, std::string>::value_type (id, v));
	}

	// Returns the list of properties created...
	return (result);
}

// ---
QGAMES::Layer* QGAMES::ObjectMapBuilderAddsOn::readLayer (TiXmlElement* e)
{
	assert (e);

	int id; std::string name;
	QGAMES::Objects objs;
	std::map <std::string, std::string> pties;
	e -> Attribute (__QGAMES_OBJECTMAPELMTSLAYERATTRID__, &id);
	name = e -> Attribute (__QGAMES_OBJECTMAPELMTSLAYERATTRNAME__);

	// Read the elements this layer is made up of
	for (TiXmlElement* groupElement = e -> FirstChildElement ();
		groupElement != NULL; groupElement = groupElement -> NextSiblingElement ())
	{
		if (strcmp (groupElement -> Value (), __QGAMES_OBJECTMAPELEMNTBLOCK__) == 0)
		{
			int idElmnt = -1;
			double xElmnt, yElmnt, zElmnt;
			xElmnt = yElmnt = zElmnt = 0.0;
			QGAMES::Object* obj = NULL;
			groupElement -> Attribute (__QGAMES_OBJECTMAPLAYERELMNTATTRID__, &idElmnt);
			groupElement -> Attribute (__QGAMES_OBJECTMAPLAYERELMNTATTRX__, &xElmnt);
			groupElement -> Attribute (__QGAMES_OBJECTMAPLAYERELMNTATTRY__, &yElmnt);
			groupElement -> Attribute (__QGAMES_OBJECTMAPLAYERELMNTATTRZ__, &zElmnt);
			assert (idElmnt != -1);
			obj = _objectBuilder -> object (idElmnt);
			assert (obj != NULL);
			objs.push_back (new QGAMES::ObjectProxy (_lastObjectId++, 
				QGAMES::Position (__BD xElmnt, __BD yElmnt, __BD zElmnt), obj)); // To do not own the object...
		}

		if (strcmp (groupElement -> Value (), __QGAMES_OBJECTMAPATTRSBLOCK__) == 0)
			pties = readPropertiesSection (groupElement);
	}

	// Creates the layer and returns...
	QGAMES::Layer* ly = createLayer (id, name, objs, NULL, pties);
	assert (ly);

	return (ly);
}
