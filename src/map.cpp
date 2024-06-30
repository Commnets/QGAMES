#include <Arcade/map.hpp>
#include <Arcade/scene.hpp>
#include <Common/form.hpp>
#include <algorithm>
#include <iostream>

// ---
QGAMES::Map::Map (int c, const Layers& l, const MapProperties& p)
		: Element (c),
		  _scene (NULL),
		  _layers (l),
		  _objects (),
		  _images (),
		  _activeLayer (NULL),
		  _properties (p),
		  _entities (),
		  _entitiesPerLayer ()
{
	for (QGAMES::Layers::iterator i = _layers.begin (); i != _layers.end (); i++)
	{
		observe ((*i));

		switch ((*i) -> type ())
		{
			case __QGAMES_OBJECTLAYER__:
				_objects.insert (QGAMES::ObjectLayers::value_type ((*i) -> id (), 
					(QGAMES::ObjectLayer*) ((*i))));
				break;

			case __QGAMES_IMAGELAYER__:
				_images.insert (QGAMES::ImageLayers::value_type ((*i) -> id (), 
					(QGAMES::ImageLayer*) ((*i))));
				break;

			default:
				break;
		}
	}
}

// ---
QGAMES::Map::~Map ()
{
	for (QGAMES::Layers::iterator i = _layers.begin (); i != _layers.end (); i++)
	{
		unObserve ((*i));
		delete ((*i));
	}

	_layers.clear ();
}

// ---
QGAMES::SetOfOpenValues QGAMES::Map::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Element::runtimeValues ();

	int lNE = result.lastNestedOpenValueId ();

	// The list of the layers...
	QGAMES::SetOfOpenValues lVV (__QGAMES_RUNTIMEVALUESLAYERSTYPE__);
	for (QGAMES::Layers::const_iterator i = _layers.begin (); i != _layers.end (); i++)
		lVV.addSetOfOpenValues ((*i) -> id (), (*i) -> runtimeValues ());
	result.addSetOfOpenValues (lNE + 1, lVV); // It could be empty if no layers has been defined (strange anyway)

	return (result);
}

// ---
void QGAMES::Map::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	QGAMES::SetOfOpenValues lVV = cCfg.setOfOpenValues (lNE);
	assert (lVV.name () == std::string (__QGAMES_RUNTIMEVALUESLAYERSTYPE__)); // Just to guarantte that the structure is going ok...
	for (QGAMES::Layers::const_iterator i = _layers.begin (); i != _layers.end (); i++)
		// Only the layers with info...
		if (lVV.existSetOfOpenValues ((*i) -> id ()))
			(*i) -> initializeRuntimeValuesFrom (lVV.setOfOpenValues ((*i) -> id ()));
	cCfg.removeSetOfOpenValues (lNE);

	QGAMES::Element::initializeRuntimeValuesFrom (cCfg);
}

// ---
QGAMES::World* QGAMES::Map::world ()
{
	assert (_scene);

	return (_scene -> world ());
}

// ---
bool QGAMES::Map::emptyPositionAt (const QGAMES::Position& p) const
{ 
	assert (_activeLayer);

	return (_activeLayer -> emptyPositionAt (p)); 
}

// ---
bool QGAMES::Map::emptyBoxAt (const QGAMES::Position& p, const QGAMES::Position& s) const
{
	assert (_activeLayer);

	return (_activeLayer -> emptyBoxAt (p, s)); 
}

// ---
void QGAMES::Map::setLayer (int nl)
{
	// __MININT__ means none

	if (nl == __MININT__)
	{
		if (_activeLayer != NULL)
		{
			unObserve (_activeLayer);
			_activeLayer -> setMap (NULL);
			_activeLayer = NULL;
		}
	}
	else
	{
		QGAMES::Layer* nLayer = NULL;
		for (QGAMES::Layers::const_iterator i = _layers.begin ();
				i != _layers.end (); i++)
			if ((*i) -> id () == nl)
				nLayer = (*i);
		assert (nLayer != NULL);

		// The active layer is no longer observed!!
		if (_activeLayer != NULL)
			unObserve (_activeLayer);

		_activeLayer = nLayer;
		_activeLayer -> setMap (this);
		observe (_activeLayer);
	}
}

// ---
bool QGAMES::Map::existLayer (int nl) const
{ 
	bool found = false;
	for (QGAMES::Layers::const_iterator i = _layers.begin ();
			i != _layers.end () && !found; i++)
		if ((*i) -> id () == nl)
			found = true;
	return (found);
}

// ---
bool QGAMES::Map::existLayer (const std::string& nl) const
{ 
	bool found = false;
	for (QGAMES::Layers::const_iterator i = _layers.begin ();
			i != _layers.end () && !found; i++)
		if ((*i) -> name () == nl)
			found = true;
	return (found);
}

// ---
QGAMES::Layer* QGAMES::Map::layer (int nl)
{
	QGAMES::Layer* result = NULL;
	for (QGAMES::Layers::const_iterator i = _layers.begin ();
			i != _layers.end () && !result; i++)
		if ((*i) -> id () == nl)
			result = (*i);
	assert (result != NULL);

	return (result);
}

// ---
QGAMES::Layer* QGAMES::Map::layer (const std::string& nl)
{
	QGAMES::Layer* result = NULL;
	for (QGAMES::Layers::const_iterator i = _layers.begin ();
			i != _layers.end () && !result; i++)
		if ((*i) -> name () == nl) // If there are more than one with the same name, returns just first!
			result = (*i);
	assert (result != NULL);

	return (result);
}

// ---
QGAMES::Layers QGAMES::Map::relevantLayers ()
{
	QGAMES::Layers result;
	if (_activeLayer != NULL)
		result.push_back (_activeLayer);
	return (result); // It can be empty (makes no sense but it can)
}

// ---
void QGAMES::Map::initialize ()
{
	for (QGAMES::Layers::const_iterator i = _layers.begin ();
			i != _layers.end (); i++)
		(*i) -> initialize ();

	QGAMES::Element::initialize ();
}

// ---
void QGAMES::Map::inEveryLoop ()
{
	QGAMES::Element::inEveryLoop ();
	for (QGAMES::Layers::const_iterator i = _layers.begin ();
			i != _layers.end (); i++)
		(*i) -> inEveryLoop ();

	QGAMES::Element::initialize ();
}

// ---
void QGAMES::Map::updatePositions ()
{
	for (QGAMES::Layers::const_iterator i = _layers.begin ();
			i != _layers.end (); i++)
		(*i) -> updatePositions ();

	if (_externalEngine)
		_externalEngine -> whenUpdatePositions (this);
}

// ---
void QGAMES::Map::considerEntitiesForDrawing (const QGAMES::Entities& et, const QGAMES::Entities& chrs, 
	const QGAMES::EntitiesPerLayer& epl, int eChgPos)
{ 
	// Just for drawing considerations...
	_entities = et;
	_entities.insert (chrs.begin (), chrs.end ()); // Add the characters...
	_entitiesPerLayer = epl;
} 

// ---
void QGAMES::Map::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s);

	// In this option the order among layers and entities is decided in a configuration file
	// The configuartion file of the map...
	if (!_entitiesPerLayer.empty ())
	{
		// This means, there are entities that has to be drawn over a specific layer.
		// So it starts to iterate over the layers, 
		// drawing them, and the entities (or characters) over each (if any)
		// and also keeping control which one are drawn in each layer.
		// Then, the rest of the enties are dawn ordered on top of any layer.
		QGAMES::Entities ePainted;
		for (QGAMES::Layers::const_iterator i = _layers.begin (); i != _layers.end (); i++)
		{
			if ((*i) -> isVisible ())
				(*i) -> drawOn (s, p);

			std::vector <int> ePL;
			QGAMES::EntitiesPerLayer::const_iterator j = _entitiesPerLayer.find ((*i) -> name ());
			if (j != _entitiesPerLayer.end ()) ePL = (*j).second;
			for (std::vector <int>::const_iterator k = ePL.begin (); k != ePL.end (); k++)
			{
				QGAMES::Entities::const_iterator l = _entities.find ((*k));
				if (l != _entities.end ()) // The entity in the layer has been defined as part of the map...
				{
					QGAMES::Entity* eToP = (*l).second;
					if (eToP -> isVisible ()) 
						eToP -> drawOn (s, p);
					ePainted.insert (QGAMES::Entities::value_type ((*k), eToP));
				}
			}
		}

		// Paint the rest of the elements...
		// ...all of them are going to be entities!
		QGAMES::Elements eToDraw;
		for (QGAMES::Entities::const_iterator j = _entities.begin (); j != _entities.end (); 
			eToDraw.push_back ((*j++).second));
		eToDraw = orderElementsToDrawOnScreen (eToDraw, s); // The entities (only) are ordered attending the type of projection...
		for (QGAMES::Elements::const_iterator i = eToDraw.begin (); i != eToDraw.end (); i++)
			if (ePainted.find (((Entity*)(*i)) -> id ()) == ePainted.end () && ((Entity*) (*i)) -> isVisible ())
				((Entity*) (*i)) -> drawOn (s, p); // Only the ones not painted before
	}
	// In this option the order among layers and entities depends totally in the internal algorithms
	else
	{
		QGAMES::Elements eToDraw;
		eToDraw.insert (eToDraw.end (), _layers.begin (), _layers.end ());
		for (QGAMES::Entities::const_iterator j = _entities.begin (); j != _entities.end (); 
			eToDraw.push_back ((*j++).second));
		eToDraw = orderElementsToDrawOnScreen (eToDraw, s); // All, entities are layers are ordered...		

		for (QGAMES::Elements::const_iterator i = eToDraw.begin (); i != eToDraw.end (); i++)
		{
			// The element to draw is a layer...
			if (dynamic_cast <QGAMES::Layer*> (*i))
			{
				QGAMES::Layer* ly = (QGAMES::Layer*) (*i);
				if (ly -> isVisible ())
				{
					QGAMES::Elements elmtsPerLayer;
					for (i++; i != eToDraw.end () && 
							  (!dynamic_cast <QGAMES::Layer*> (*i) || 
							   (dynamic_cast <QGAMES::Layer*> (*i) && !(*i) -> isVisible ())); i++) // up to the next layer...
						if (!dynamic_cast <QGAMES::Layer*> (*i) && (*i) -> isVisible ())
							elmtsPerLayer.push_back ((*i));
					i--; // When the next layer is reached, then it is time...

					// ...to draw the layer with the elements on top
					// Take into account that even at that point the elements are also ordered!
					ly -> drawOnWithElements (s, p, elmtsPerLayer);
					// The previous method remove from the elemtsPerLayer list any element finally drawn
					// By default all of them will, but just in case!

					// ...Draws the rest...
					for (QGAMES::Elements::const_iterator j = elmtsPerLayer.begin (); j != elmtsPerLayer.end (); j++)
						if ((*j) -> isVisible ()) (*j) -> drawOn (s, p);
				}
			}

			// The element to draw is not a layer, but a entity actually!
			else
			if (dynamic_cast <QGAMES::Entity*> (*i))
			{
				QGAMES::Entity* ety = (QGAMES::Entity*) (*i);
				if (ety-> isVisible ())
					ety -> drawOn (s, p);
			}
			else
				assert (0); // Shouldn't be here...
		}
	}

	QGAMES::Element::drawOn (s, p);
}

// ---
void QGAMES::Map::finalize ()
{
	for (QGAMES::Layers::const_iterator i = _layers.begin ();
			i != _layers.end (); i++)
		(*i) -> finalize ();

	QGAMES::Element::finalize ();
}

// ---
QGAMES::TiledMap::TiledMap (int c, const Layers& l, int w, int h, int d, int tW, int tH, int tD,
	const MapProperties& p)
		: Map (c, l, p),
		  _tiles (),
		  _width (w),
		  _height (h),
		  _depth (d),
		  _tileWidth (tW),
		  _tileHeight (tH),
		  _tileDepth (tD)
{
	bool fTL = true;
	QGAMES::TileLayer::VisualOrientation vO = QGAMES::TileLayer::VisualOrientation::_ORTHOGONAL; 
	// First definition, no further implications...
	for (QGAMES::Layers::iterator i = _layers.begin (); i != _layers.end (); i++)
	{
		switch ((*i) -> type ())
		{
			case __QGAMES_TILELAYER__:
				{
					observe ((*i)); 
					_tiles.insert (QGAMES::TileLayers::value_type ((*i) -> id (), 
						(QGAMES::TileLayer*) ((*i))));
					if (fTL) { fTL = false; vO = ((QGAMES::TileLayer*) (*i)) -> visualOrientation (); }
					else assert (vO == ((QGAMES::TileLayer*) (*i)) -> visualOrientation ()); 
					// All layers must have the same visual orientation, otherwise the system won't work properly
				}

				break;

			default:
				break;
		}
	}

	// There must be a tile layer minimum...
	assert (!_tiles.empty ());
}

// ---
QGAMES::TileLayer::VisualOrientation QGAMES::TiledMap::visualOrientation () const
{
	// The visual orientation of the first layer is taken into consideration only...
	return ((*_tiles.begin ()).second -> visualOrientation ());
}

// ---
int QGAMES::TiledMap::visualTileWidth () const
{
	int result = tileWidth ();
	switch (visualOrientation ())
	{
		case QGAMES::TileLayer::VisualOrientation::_ISOMETRIC:
			result = tileWidth () >> 1; // /2
			break;

		case QGAMES::TileLayer::VisualOrientation::_CAVALIER:
			result = (tileWidth () >> 2) * 3; // 3/4
			break;

		// Orthogonal, conic and external defined are left as they were defined...
		default:
			break;
	}

	return (result);
}

// ---
int QGAMES::TiledMap::visualTileHeight () const
{
	int result = tileHeight ();
	switch (visualOrientation ())
	{
		case QGAMES::TileLayer::VisualOrientation::_ISOMETRIC:
			result = tileWidth () >> 1; // /2
			break;

		case QGAMES::TileLayer::VisualOrientation::_CAVALIER:
			result = tileWidth () >> 2; // /4
			break;

		// Orthogonal, conic and external defined are left as they were defined...
		default:
			break;
	}

	return (result);
}

// ---
int QGAMES::TiledMap::visualTileDepth () const
{
	int result = tileDepth (); 
	switch (visualOrientation ())
	{
		case QGAMES::TileLayer::VisualOrientation::_ISOMETRIC:
			result = tileHeight () - ((visualTileWidth () + visualTileHeight ()) >> 1);
			break;

		case QGAMES::TileLayer::VisualOrientation::_CAVALIER:
			result = tileHeight () - visualTileHeight ();
			break;

		// Orthogonal, conic and external defined are left as they were defined...
		default:
			break;
	}

	return (result);
}

// ---
QGAMES::Tile* QGAMES::TiledMap::tileAt (const QGAMES::Position& p, QGAMES::Layer* l) 
{ 
	QGAMES::Layer* lT = l;
	if (lT == NULL) lT = _activeLayer;
	assert (lT);
	// One layer to check minimum...

	// Only when the active layer is a tile layer, 
	// the result could be a not null tile (obvious, isn't it?)
	return ((lT -> type () ==  __QGAMES_TILELAYER__) 
		? ((QGAMES::TileLayer*) lT) -> tileAt (p) : QGAMES::NullTile::_nullTile);
}

// ---
QGAMES::Tiles QGAMES::TiledMap::tilesAt (const QGAMES::Position& p, int n, QGAMES::Layer* l)
{
	QGAMES::Layer* lT = l;
	if (lT == NULL) lT = _activeLayer;
	assert (lT);

	return ((lT -> type () ==  __QGAMES_TILELAYER__) 
		? ((QGAMES::TileLayer*) lT) -> tilesAt (p, n) : QGAMES::Tiles ());
}

// ---
QGAMES::Tiles QGAMES::TiledMap::tilesAt (const std::vector <QGAMES::Position>& p, QGAMES::Layer* l)
{
	QGAMES::Layer* lT = l;
	if (lT == NULL) lT = _activeLayer;
	assert (lT);

	return ((lT -> type () ==  __QGAMES_TILELAYER__) 
		? ((QGAMES::TileLayer*) lT) -> tilesAt (p) : QGAMES::Tiles ());
}

// ---
QGAMES::Tiles QGAMES::TiledMap::tilesAround (QGAMES::Entity *e, QGAMES::TileLayer* tL,
	QGAMES::bdata m, const QGAMES::Vector& pv)
{
	assert (e);
	assert (tL);

	// If the layer to be used, is not a tiled layer, 
	// there could not be tiles around an entity, no?
	if (tL -> type () != __QGAMES_TILELAYER__)
		return (QGAMES::Tiles ());
	return (((QGAMES::TileLayer*) tL) -> tilesAround (e, m, pv));
}

QGAMES::Tiles QGAMES::TiledMap::tilesIn (QGAMES::TileLayer* tL, 
	int pX, int pY, int pZ, int w, int h, int d)
{
	// The tl == NULL means to use the active one...

	// If the layer to be used, is not a tiled layer, 
	// there could not be tiles around an entity, no?
	if (tL == NULL && _activeLayer -> type () != __QGAMES_TILELAYER__)
		return (QGAMES::Tiles ());
	QGAMES::TileLayer* tToUse = (tL == NULL) 
		? (QGAMES::TileLayer*) _activeLayer : tL;
	assert (tToUse);

	return (tToUse -> tilesIn (pX, pY, pZ, w, h, d));
}

// ---
QGAMES::Position QGAMES::TiledMap::tilePosition (const QGAMES::Tile* t, const QGAMES::TileLayer* tL) const
{
	// Only when the active layer is a tile layer, 
	// the result could be a not null tile (obvious, isn't it?)
	return (tL ? tL -> tilePosition (t) : ((_activeLayer -> type () ==  __QGAMES_TILELAYER__) 
		? ((QGAMES::TileLayer*) _activeLayer) -> tilePosition (t) : QGAMES::Position::_noPoint));
}

// ---
QGAMES::Elements QGAMES::TiledMap::orderElementsToDrawOnScreen (const QGAMES::Elements& e, QGAMES::Screen* scr)
{
	assert (scr != NULL);

	QGAMES::Elements result = e;

	// Makes no sense to order what it not going to be visible!!
	// In a tiled map, the speed is one of the most important things, specially with many elements...
	for (QGAMES::Elements::iterator i = result.begin (); i != result.end ();)
		if (!(*i) -> isVisible ()) i = result.erase (i); else i++;

	// Define an internal structure to compare what to project taking into account the proyection point.
	// the z position of the entities, and then the y...
	// The visual orientation is taken into account
	struct determineRelativePositionAmongElements
	{
		public:
		determineRelativePositionAmongElements (QGAMES::TileLayer::VisualOrientation vO, const QGAMES::Matrix3& eM)
			: _visualOrientation (vO), _eulerMatrix (eM)
							{ }

		// To know whiy the next formulas are used @see "Matemáticas básicas para juegos"
		bool operator () (QGAMES::Element* a, QGAMES::Element* b)
		{
			// This is declared as a very internal function because the same
			// procedure (algorithm) is used twice. So, this is to reuse code actually only!
			auto compareEntityVsTileLayer = [=](QGAMES::Entity* e, QGAMES::TileLayer* l) -> bool
			{
				QGAMES::Position pA = e -> position ().transformWith (_eulerMatrix); // The top corner of the box rounding it
				QGAMES::bdata pB = l -> position ().transformWith (_eulerMatrix).posZ (); // The basic level

				// The z axis is positive going into the screen (in orthoghonal projection or external one)
				// or going down in the isometric or cavalier ones
				// So, the greater the z vaue is, the first to be drawn
				return (pA.posZ () > pB);
			};

			bool result = false;

			// Either an entity...
			if (dynamic_cast <QGAMES::Entity*> (a) != NULL)
			{
				// ...vs another Entity too?
				if (dynamic_cast <QGAMES::Entity*> (b))
				{
					QGAMES::Entity* eA = (QGAMES::Entity*) a;
					QGAMES::Entity* eB = (QGAMES::Entity*) b;

					switch (_visualOrientation)
					{
						case QGAMES::TileLayer::VisualOrientation::_ISOMETRIC:
							{
								QGAMES::Position pCA = eA -> centerPosition ().transformWith (_eulerMatrix);
								QGAMES::bdata dA = pCA.posX () + pCA.posY () - 
									/** Because the z negative is the visible zone. */ pCA.posZ ();
								QGAMES::Position pCB = eB -> centerPosition ().transformWith (_eulerMatrix);
								QGAMES::bdata dB = pCB.posX () + pCB.posY () - 
									/** Because the z negative is the visible zone. */ pCB.posZ ();
								QGAMES::bdata i = dA - dB; 
								// The one closer to the center (0,0,0) will be the first to be drawn

								result = (i < __BD 0) ? true : ((i == __BD 0) ? eA -> id () < eB -> id () : false);
							}

							break;

						case QGAMES::TileLayer::VisualOrientation::_CAVALIER:
							{
								
								QGAMES::Position pA = eA -> centerPosition ().transformWith (_eulerMatrix);
								QGAMES::Position pB = eB -> centerPosition ().transformWith (_eulerMatrix);
								result = (pA.posY () == pB.posY ())
											? ((pA.posX () == pB.posX ()) 
												? (eA -> id () < eB -> id ()) : (pA.posX () < pB.posX ())) 
											: (pA.posY () < pB.posY ()); 
								// They are ordered following the y axis really...
								// Only when this value is the same, the position in the x axis is taken into account
							}

							break;

						case QGAMES::TileLayer::VisualOrientation::_ORTHOGONAL:
						case QGAMES::TileLayer::VisualOrientation::_CONIC:
						case QGAMES::TileLayer::VisualOrientation::_EXTERANLLYDEFINED:
						default:
							{
								QGAMES::Position pA = eA -> centerPosition ().transformWith (_eulerMatrix);
								QGAMES::Position pB = eB -> centerPosition ().transformWith (_eulerMatrix);
								result = (pA.posZ () == pB.posZ ())
											? ((pA.posY () == pB.posY ()) 
													? (eA -> id () < eB -> id ()) : (pA.posY () < pB.posY ())) 
											: (pA.posZ () > pB.posZ ()); 
								// The z axis is positive going into the screen
								// so, first the one with the greater z coordinate
							}
					}
				}

				// or vs a TileLayer instead?
				else 
				if (dynamic_cast <QGAMES::TileLayer*> (b) != NULL)
					result = compareEntityVsTileLayer ((QGAMES::Entity*) a, (QGAMES::TileLayer*) b);
				else
					assert (0); // It shouldn't be here...
			}

			// or, a TileLayer...
			else 
			if (dynamic_cast <QGAMES::TileLayer*> (a) != NULL)
			{
				// vs an Entity too? (same situation actually than 2 but the way around)
				if (dynamic_cast <QGAMES::Entity*> (b))
					result = !compareEntityVsTileLayer ((QGAMES::Entity*) b, (QGAMES::TileLayer*) a);

				// ..or vs another TileLayer instead?
				else
				if (dynamic_cast <QGAMES::TileLayer*> (b) != NULL)
				{
					QGAMES::TileLayer* l1 = (QGAMES::TileLayer*) a;
					QGAMES::TileLayer* l2 = (QGAMES::TileLayer*) b;
					return (l1 -> position ().transformWith (_eulerMatrix).posZ () > 
							l2 -> position ().transformWith (_eulerMatrix).posZ ());
					// Whatever the projection is the greater the z value is, the first to be drawn...
				}
			}

			return (result);
		}

		private:
		QGAMES::TileLayer::VisualOrientation _visualOrientation;
		QGAMES::Matrix3 _eulerMatrix;
	};

	std::sort (result.begin (), result.end (), 
		determineRelativePositionAmongElements (visualOrientation (), 
		scr -> currentCamera () -> currentStatus ()._eulerMatrix)); 
	// Pass the euler matrix to rotate only the reference points, 
	// To translate is not needed to determine whether a point is before other...

	return (result);
}
