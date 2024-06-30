#include <Platform/ptartists.hpp>
#include <Platform/ptlayers.hpp>

// ---
QGAMES::PlatformArtist::PlatformArtist (int cId, const Forms& f, const Data& d)
	: QGAMES::ArtistInATiledMap (cId, f, d),
	  _definition (NULL), // No definition by default...
	  _platformState (-1), // Not initialized...
	  _onTheEdge (0),
	  _lastLayerBase (NULL),
	  _layerBase (NULL),
	  _layerBasePosition (QGAMES::Position::_cero),
	  _layerBaseTiles (),
	  _layersToTest (),
	  _tileKillingEntity (false),
	  _tileSolid (false),
	  _collisionAtDir (7, false),
	  _canClimbUp (false), _canClimbDown (false),
	  _layerLadder (NULL),
	  _canSwim (false),
	  _layerWater (NULL),
	  _canExitWaterUp (false), _canExitWaterDown (false), _canExitWaterLeft (false), _canExitWaterRight (false),
	  _drawLimits (false),
	  _fixBasePosition (QGAMES::Position::_noPoint)
{ 
#ifndef NDEBUG
	_drawLimits = true;
#endif

	// Register a handler to notify from the artist when it hits a notification tile...
	// One of the default collision handers is enough.
	// The event launched will be __QGAMES_COLLISIONWITHTILE__
	// It will be received whoever is observing the entity
	// If the event is treated, it should be marked...
	registerTileHitHandler (__QGAMES_PTNOTIFICATIONTILETYPE__, 
		new QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame (__QGAMES_PTNOTIFICATIONTILETYPE__, false));

	// Register an additional buoy to the one inserted previously at the entity constructor...
	assert (!buoys ().empty ()); // Just in case...
	buoys ().insert (QGAMES::Buoys::value_type
		(__QGAMES_PTCHANGESTATEDEFEREDBUOYID__, new QGAMES::PlatformArtist::ToChangeStateBuoy));
}

// ---
QGAMES::SetOfOpenValues QGAMES::PlatformArtist::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::ArtistInATiledMap::runtimeValues ();

	int lE = result.lastOpenValueId ();

	std::stringstream sS; sS << _fixBasePosition;
	result.addOpenValue (lE + 1, QGAMES::OpenValue (sS.str ()));

	return (result);
}

// ---
void QGAMES::PlatformArtist::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_fixBasePosition = QGAMES::Vector (cCfg.openValue (lE).strValue ());
	cCfg.removeOpenValue (lE);

	QGAMES::ArtistInATiledMap::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::PlatformArtist::setFromDefinition (PlatformGame::ElementDefinition* eD)
{ 
	assert (eD);
	// For no definition, use setFromNoDefinition

	_definition = eD;
	
	setPlatformState (eD -> _state);

	// The position...
	if (eD -> _positions.size () != 0) 
		setPosition (eD -> _positions [0]);
	// The base... (take into account that the base could have been calculated in another state)
	setFixBasePosition ((eD -> _positions.size () > 1) ? eD -> _positions [1] : QGAMES::Position::_noPoint);

	// The orientation
	setOrientation (eD -> _orientation);
	
	// By default, the other values are not taken into account... 
}

// ---
void QGAMES::PlatformArtist::setFromNoDefinition ()
{ 
	// The artist is not the owner, so it is not necessary to delete the old definition...
	_definition = NULL;

	setPlatformState (-1); // Has to be treated...

	setPosition (QGAMES::Position::_cero);
}

// ---
bool QGAMES::PlatformArtist::canMove (const QGAMES::Vector& d, const QGAMES::Vector& a)
{
	// Take into account that the system doesn't consider 
	// either the direction of the movement or the acceleration to determinate whether the movement is possible.
	// Just check whether the reference tile "RIGHT NOW" is or not valid for the movement.
	// If it is, the entity will move to the next position, otherwise the movement will not be possible...
	// All types of movements in platform games should be defined as pixel-by-pixel movements

	if (!map ()) return (true); // Make no sense but...
	if (!isPossiblePosInMap (position () + d)) return (false);

	bool result = true;
	QGAMES::bdata m = (QGAMES::bdata) 0.0;
	QGAMES::Vector pv = QGAMES::Vector::_cero;
	adjustRelevants (m, pv); // To adjust the way the tile is considered...
	QGAMES::Layers rLayers = (__TM map ()) -> relevantLayers ();
	for (QGAMES::Layers::const_iterator i = rLayers.begin (); 
		i != rLayers.end () && result; i++)
	{
		if ((*i) -> type () != __QGAMES_TILELAYER__ || 
			((*i) -> type () == __QGAMES_TILELAYER__ && ((QGAMES::PlatformTileLayer*) (*i)) -> transparent ())) 
			continue;

		QGAMES::Tiles rlTile = 
			referenceTilesNow (tilesRelevant (((QGAMES::TileLayer*) (*i)), m, pv), d, a);
		for (int j = 0; j < (int) rlTile.size () && result; result = !isTileAnObstacle (rlTile [j++]));
	}

	return (result);
}

// ---
bool QGAMES::PlatformArtist::isPossiblePosInMap (const QGAMES::Position& v)
{
	if (map () == NULL)	return (true); // If there is no map, the movement is possible...(no sense but...)

	int mVL, mVH, mVD;
	maxVisualDimensions (mVL, mVH, mVD);
	return (v.posX () > __BD 0 && v.posX () < __BD (map () -> width () - mVL) &&
			v.posY () > __BD 0 && v.posY () < __BD (map () -> height () - mVH));
}

// ---
bool QGAMES::PlatformArtist::isOnABase ()
{
	bool result = false;
	QGAMES::Layers rLayers = _map -> relevantLayers ();
	QGAMES::Position bP = basePosition ();
	for (QGAMES::Layers::const_iterator i = rLayers.begin (); i != rLayers.end () && !result; i++)
	{
		if (dynamic_cast <QGAMES::PlatformTileLayer*> (*i) && (*i) -> isVisible ())
		{
			QGAMES::Tile* tl = ((QGAMES::PlatformTileLayer*) (*i)) -> tileAt (bP);
			if (tl != QGAMES::NullTile::_nullTile && tl -> type () == __QGAMES_PTTBASETILETYPE__)
				result = true;
		}
	}

	return (result);
}

// ---
void QGAMES::PlatformArtist::setPlatformState (int id)
{ 
	if (!isPlatformStatePossible (id) && id != -1) // -1 means not defined...
		return; // Nothing to do when it is not possible...

	_platformState = id; 

	if (_definition)
		_definition -> _state = _platformState; 
}

// ---
void QGAMES::PlatformArtist::initialize ()
{
	// The layers under my feet are set to null...
	_lastLayerBase = NULL;
	_layerBase = NULL;
	_layerBasePosition = QGAMES::Position::_cero;
	_layerBaseTiles = QGAMES::Tiles ();
	_fixBasePosition = QGAMES::Position::_noPoint;
	_layersToTest = QGAMES::Layers ();
	_layerWater = NULL;
	_layerLadder = NULL;
	_tileKillingEntity = false;
	_tileSolid = false;

	// No climb, no swin so far...
	_canClimbUp = _canClimbDown = false;
	_canSwim = false;
	_canExitWaterUp = _canExitWaterDown = _canExitWaterLeft = _canExitWaterRight = false;

	// No colision at front...
	for (int i = 0; i < 7; _collisionAtDir [i++] = false);

	QGAMES::ArtistInATiledMap::initialize ();
}

// ---
void QGAMES::PlatformArtist::updatePositions ()
{
	// At this point the implementation (_layerBase and so) won't be "cero" yet...

	// At this point if there is something beneath
	// and it has moved, then the position of the entity is actualized...
	// ...to maintain the entity fixed to that platform...
	if (_layerBase != NULL) // All layers gotta be AdvancedTileLayer (verified at map construction time)...
	{
		const QGAMES::Vector& cM = ((QGAMES::AdvancedTileLayer*) _layerBase) -> vectorMoved 
			(*(((QGAMES::AdvancedTileLayer*) _layerBase) 
				? ((QGAMES::AdvancedTileLayer*) _layerBase) -> tileAt (centerPosition ())
				: QGAMES::NullTile::_nullTile)); // Taking into account the tile at the center...
		if (cM != QGAMES::Vector::_cero && _layerBase -> moves ()) // Only if there is movement...
		{
			setPosition (position () + cM);
			// The base has also to be moved if any...
			if (fixBasePosition () != QGAMES::Position::_noPoint) 
				setFixBasePosition (fixBasePosition () + cM);
		}
	}

	// At this point, nobody knows whether other potential activities will be possible
	// The method "whenOver" is the first invoked in ArtistInATiledMap::update
	// and these variables will be then set up!
	// Take into account that all thease variable can't be used in "canMove" method becase
	// They all will be false (or null) at that point...
	// "canMove" method is executed before these variables are set basck to their right values
	// They can be used in the "whenOnTile" method instead, that is executed later...
	_canClimbUp = _canClimbDown = false;
	_layerLadder = NULL;
	_canSwim = false;
	_layerWater = NULL;
	_canExitWaterUp = _canExitWaterDown = _canExitWaterLeft = _canExitWaterRight = false;

	// Then the method whenCollisionWith is invoked...
	for (int i = 0; i < 7; _collisionAtDir [i++] = false);

	// ...and then when on tile...
	_lastLayerBase = _layerBase;
	_layerBase = NULL;
	_layerBasePosition = QGAMES::Position::_cero;
	_layerBaseTiles = QGAMES::Tiles ();
	_onTheEdge = 0;
	_tileKillingEntity = false;
	_tileSolid = false;
	if (map ()) _layersToTest = (__TM map ()) -> relevantLayers ();

	QGAMES::Position cPos = position ();
	int Cst = platformState ();
	bool cFPos = fixBasePosition () != QGAMES::Position::_noPoint;

	QGAMES::ArtistInATiledMap::updatePositions ();

	// Update the fix position if it makes sense...
	if (Cst == platformState () && // The status is the same... 
		cFPos && fixBasePosition () != QGAMES::Position::_noPoint && // There is a fix position to maintain...
		_layerBase == _lastLayerBase) // ...and in the same base (otherwise it could be adjusted in other part of the program)
	{
		QGAMES::Position dPos = position () - cPos;
		setFixBasePosition (QGAMES::Position (
			fixBasePosition ().posX () == __MINBDATA__ ? __MINBDATA__ : fixBasePosition ().posX () + dPos.posX (),
			fixBasePosition ().posY () == __MINBDATA__ ? __MINBDATA__ : fixBasePosition ().posY () + dPos.posY (),
			fixBasePosition ().posZ () == __MINBDATA__ ? __MINBDATA__ : fixBasePosition ().posZ () + dPos.posZ ()));
	}

	// Update the positions and the orientation in the definition object..if any...
	if (_definition)
	{
		QGAMES::Positions pos; pos.push_back (position ());
		if (fixBasePosition () != QGAMES::Position::_noPoint)
			pos.push_back (basePosition ()); // The base if any...
		_definition -> _positions = pos; 
		_definition -> _orientation = orientation ();
		_definition -> _state = platformState ();
	}
}

// ---
void QGAMES::PlatformArtist::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	// Draws the rest...normally the entity itself!
	QGAMES::ArtistInATiledMap::drawOn (s, p);

	if (_drawLimits)
		drawOnLimits (s, p);
}

// ---
void QGAMES::PlatformArtist::whenCollisionWith (const QGAMES::Tiles& t, 
	QGAMES::ArtistInATiledMap::TilePosition tP, QGAMES::TileLayer* l)
{
	if (l -> type () == __QGAMES_TILELAYER__ && !((QGAMES::PlatformTileLayer*) l) -> transparent ()) 
	{
		for (QGAMES::Tiles::const_iterator i = t.begin (); 
				i != t.end () && !_collisionAtDir [(int) tP]; i++)
			if ((*i) -> type () == __QGAMES_PTSOLIDTILETYPE__)
				_collisionAtDir [(int) tP] = true;
	}

	QGAMES::ArtistInATiledMap::whenCollisionWith (t, tP, l);
}

// ---
void QGAMES::PlatformArtist::whenOnTile (const QGAMES::Tiles& t, QGAMES::TileLayer* l)
{
	// One layer less to check...
	_layersToTest.remove (l);

	// The reference tile taken into account for this method, is the reference one, unless the tile was null.
	// In that case the routine will verify the artist is stable enough! (not in the edge)
	int onEdge = 0;
	QGAMES::Tiles rTB = referenceBaseTiles (l);
	assert (rTB.size () == 3); // Base, at the right and at the left...
	QGAMES::Tile* rT = rTB [1];

	// Nothing below the central position?
	if (rTB [1] == QGAMES::NullTile::_nullTile) 
	{
		// Looking at the right?
		if (orientation ().posX () > __BD 0) 
		{ 
			// Something in the right?
			if (rTB [2] != QGAMES::NullTile::_nullTile)
			{
				onEdge = 1; // Meaning: Not stable position just below, but one potential at the right
				rT = rTB [2];   //...and additionally the referece tile is changed!
							// That tile at the right is got, 
							// so after this instruction the holes are left at the left...
			}
		}
		// Looking at the left?
		else
		if (orientation ().posX () < __BD 0)
		{ 
			// Something in the left?
			if (rTB [0] != QGAMES::NullTile::_nullTile)
			{
				onEdge = -1; // Meaning: Not stable position just below, but one potential at the left
				rT = rTB [0];	 // ...and additionally the reference tile is changed!
							 // That tile at the left is got,
							 // so after this instruction, the holes are left at the right...
			}
		}
		// Not clear...
		else
		{
			if (rTB [2] == QGAMES::NullTile::_nullTile)
			{
				if (rTB [0] != QGAMES::NullTile::_nullTile)
				{
					onEdge = -1;
					rT = rTB [0];
				}
			}
			else
			{
				onEdge = 1;
				rT = rTB [2];
			}
		}
	}

	// There is something below the central position
	else
	{
		// Looking to the right?
		if (orientation ().posX () > __BD 0)
		{
			// Nothing in the right?
			if (rTB [2] == QGAMES::NullTile::_nullTile)
				onEdge = 2; // Meaning: It is stable, but not enought maybe, because there is a hole at the right!
							// But the reference tile is not changed anyway
		}
		else
		// Looking to the left?
		if (orientation ().posX () < __BD 0)
		{
			// Nothing in the left?
			if (rTB [0] == QGAMES::NullTile::_nullTile)
				onEdge = -2; // Meaning: It is stable, but not enough maybe, because there is a hole at the left!
		}
		// Not clear...
		else
		{
			if (rTB [2] != QGAMES::NullTile::_nullTile)
			{
				if (rTB [0] == QGAMES::NullTile::_nullTile)
					onEdge = -2;
			}
			else
				onEdge = 2;
		}
	}

	// If the reference is not null but it is solid...
	// The movement will be impossible, unless there is other base near!
	// So it is needed to look for another potential base!
	if (rT -> type () == __QGAMES_PTSOLIDTILETYPE__ && l -> isVisible ())
	{
		for (int i = 0; i < (int) t.size (); i++)
			if (t [i] != QGAMES::NullTile::_nullTile && t [i] -> type () == __QGAMES_PTTBASETILETYPE__)
				rT = t [i];
	}

	// Check whether there is a base behind (in a solid layer)...
	if (rT -> type () == __QGAMES_PTTBASETILETYPE__ &&
			dynamic_cast <QGAMES::PlatformTileLayer*> (l) && ((QGAMES::PlatformTileLayer*) l) -> solid ())
	{
		_layerBase = l; // Detected...
		_layerBasePosition = ((QGAMES::TileLayer*) _layerBase) -> tilePosition (rT);
		_layerBaseTiles = t; // Keep the tiles referenced in the base...
		_onTheEdge = onEdge; //But, is it on the edge? This will be taken into account later...
		_tileKillingEntity = false; // If there is solid tile beneath whatever the layer is, 
									// nothing can kill entity...
		_tileSolid = false;
	}

	// Check whether there is something behind killing you...
	if (rT -> type () == __QGAMES_PTKILLINGTILETYPE__ && 
			((QGAMES::PlatformTileLayer*) l) -> solid () && !_layerBase)
		_tileKillingEntity = true;

	// Check whether there is something solid behing...
	if (rT -> type () == __QGAMES_PTSOLIDTILETYPE__ && 
			((QGAMES::PlatformTileLayer*) l) -> solid () && !_layerBase)
		_tileSolid = true;

	// If there are still more layers to check just return...
	// Otherwise it is time to determinate what to do...
	// An what to do will depend on the artist (it has to be implemented later)
	if (_layersToTest.empty ())
		afterAllLayersTestedInWhenOnTile ();

	QGAMES::ArtistInATiledMap::whenOnTile (t, l);
}

// ---
void QGAMES::PlatformArtist::whenOver (const QGAMES::Tiles& t, QGAMES::TileLayer* tL, int w, int h, int d)
{
	// Notice that _canClimb and similars are set to false at the beginning of the method "updatePositions"
	// This method is invoked several times per loop (one per relevant layer)
	// The ladder can be detected at any of them...
	
	// First, the artist could climb up if top positions have a tile with a ladder...
	// _canClimbUp & _canClimbDown are set to false at the beginning of every "updatePositions" execution.
	if (!_canClimbUp && 
		t [(w >> 1)] -> type () == __QGAMES_PTLADDERTILETYPE__)
	{
		_canClimbUp = true; 
		_layerLadder = tL;
	}

	// Then, the artist could climb down, when the last line has a ladder!
	if (!_canClimbDown &&
		t [(((h > 0) ? (h - 1) : 0) * w) + (w >> 1)] -> type () == __QGAMES_PTLADDERTILETYPE__)
	{
		_canClimbDown = true; 
		_layerLadder = tL;
	}

	// The detection of the water is also important...
	// because some entities are able to swim in the appropiate conditions.
	// As _canClimb attribute, _canSwim is set to false at the beginning of the updatePositions method (once per loop)
	if (!_canSwim && 
		t [((h >> 1) * w) + (w >> 1)] -> type () == __QGAMES_PTLIQUIDTILETYPE__)
	{
		_canSwim = true;
		_layerWater = tL;
	}

	// But to exit the water is possible when being inside the water the top tiles are empty...
	if (_canSwim && tL == _layerWater)
	{
		_canExitWaterUp = true;
		for (int i = 0; i < w && _canExitWaterUp; i++)
			if (t [i] -> type () != __QGAMES_TILEEMPTY__)
				_canExitWaterUp = false;
		
		int r = (h - 1) * w;
		_canExitWaterDown = true;
		for (int i = 0; i < w && _canExitWaterDown; i++)
			if (t [r + i] -> type () != __QGAMES_TILEEMPTY__)
				_canExitWaterDown = false;

		_canExitWaterLeft = true;
		for (int i = 0; i < h && _canExitWaterLeft; i++)
			if (t [i * w] -> type () != __QGAMES_TILEEMPTY__)
				_canExitWaterLeft = false;

		_canExitWaterRight = true;
		for (int i = 1; i <= h && _canExitWaterRight; i++)
			if (t [(i * w) - 1] -> type () != __QGAMES_TILEEMPTY__)
				_canExitWaterRight = false;
	}
}

// ---
void QGAMES::PlatformArtist::relevantTilePositions (int w, int h, int d, 
	std::vector <int>& up, std::vector <int>& down, 
	std::vector <int>& right, std::vector <int>& left, 
	std::vector <int>& front, std::vector <int>& back)
{
	up.clear (); down.clear (); 
	right.clear (); left.clear (); 
	front.clear (); back.clear ();

	// Looking to the left?
	// It includes all tiles in the left part! (excepting last line)
	// Unless there was only one line or two
	if (orientation ().posX () <= 0)
	{
		if (h == 1 || h == 2) left.push_back (0);
		else for (int i = 0; i < (h - 1); left.push_back (w * i++));
	}

	// Looking to the right?
	// It includes all tiles in the right side! (excepting last line)
	// Unless there was only one line or two
	if (orientation ().posX () >= 0)
	{
		if (h == 1 || h == 2) right.push_back (w - 1);
		else for (int i = 0; i < (h - 1); right.push_back ((w * i++) + (w - 1))); 
	}

	// Moving up?
	if (orientation ().posY () <= 0)
	{
		// ...and looking to the left?
		// It includes all tiles in the up left half part
		if (w == 1) up.push_back (0);
		if (orientation ().posX () <= 0)
			for (int i = 0; i < (w >> 1); up.push_back (i++));
		// ..or looking to the right?
		// It includes all tiles in the up right half part
		if (orientation ().posX () >= 0)
			for (int i = (w >> 1); i < w; up.push_back (i++));
		// Notice that in case not looking to any specific position
		// all top tiles would be included...
	}

	// Moving down?
	// Same rational than previous but in the down!
	if (orientation ().posY () >= 0)
	{
		// ...and looking to the left?
		// It includes all tiles in the down left half part
		int lL = (h - 1) * w; 
		if (w == 1) down.push_back (lL);
		if (orientation ().posX () <= 0)
			for (int i = 0; i < (w >> 1); down.push_back (lL + i++));
		// ..or looking to the right?
		// It includes all tiles in the down right half part
		if (orientation ().posX () >= 0)
			for (int i = (w >> 1); i < w; down.push_back (lL + i++));
		// Notice that in case not looking to any specific position
		// all botton tiles would be included...
	}
}

// ---
void QGAMES::PlatformArtist::drawOnLimits (QGAMES::Screen* s, const QGAMES::Position& p)
{
	// Draws a box rounding the entity
	s -> drawBox (roundingBox (), __QGAMES_YELLOWCOLOR__);

	// Draws a little rectangle to represent the collision zone...
	// In the same, this routine should be valid whatever the type of projection is!
	s -> drawRectangle (collisionZone (), __QGAMES_REDCOLOR__);

	// Draws a little cross to represent the center
	// Following the same, this part of the routine should be valid, whatever the projection is...
	QGAMES::Position cP = centerPosition ();
	s -> drawLine (cP - QGAMES::Vector (__BD 5, __BD 0, __BD 0), 
				   cP + QGAMES::Vector (__BD 5, __BD 0, __BD 0), __QGAMES_BLACKCOLOR__);
	s -> drawLine (cP - QGAMES::Vector (__BD 0, __BD 0, __BD 5), 
				   cP + QGAMES::Vector (__BD 0, __BD 0, __BD 5), __QGAMES_BLACKCOLOR__);
}

// ---
void* QGAMES::PlatformArtist::ToChangeStateBuoy::treatFor (QGAMES::Element* e)
{
	((QGAMES::PlatformArtist*) e) -> setPlatformState (_targetState);
	if (_adjustAfterSetState) 
		((QGAMES::PlatformArtist*) e) -> adjustPositionToBase ();
	return (this);
}

// ---
void QGAMES::PlatformArtist::setPlatformStateDeferred (int s, bool a)
{
	QGAMES::PlatformArtist::ToChangeStateBuoy* b = 
		dynamic_cast <QGAMES::PlatformArtist::ToChangeStateBuoy*> (buoy (__QGAMES_PTCHANGESTATEDEFEREDBUOYID__));
	assert (b); // Just in case...

	b -> setTargetState (s);
	b -> setAdjustBaseAfterTargetSet (a);
	b -> active (true); // Activate to change the state in the next loop...
}

// ---
QGAMES::Tiles QGAMES::PlatformArtist::referenceBaseTiles (QGAMES::Layer* l)
{
	QGAMES::Position bP = basePosition ();

	QGAMES::Tiles result;
	if (bP == QGAMES::Position::_noPoint)
		for (int i = 0; i < 3; i++)
			result.push_back (QGAMES::NullTile::_nullTile);
	else
	{
		QGAMES::Vector v (__BD ((__TM map ()) -> tileWidth ()), __BD 0, __BD 0);
		result = (__TM map ()) -> tilesAt (bP - v, 3, l);
	}

	return (result);
}

// ---
bool QGAMES::PlatformArtist::isTileAnObstacle (QGAMES::Tile* t)
{
	return (t && t != QGAMES::NullTile::_nullTile && 
			(t -> type () == __QGAMES_PTSOLIDTILETYPE__ ||
			 t -> type () == __QGAMES_PTTBASETILETYPE__));
}

// ---
bool QGAMES::PlatformArtist::collisioningWithTile (int tT, const QGAMES::Vector& d, const QGAMES::Vector& a)
{
	bool result = false;

	QGAMES::bdata m = (QGAMES::bdata) 0.0;
	QGAMES::Vector pv = QGAMES::Vector::_cero;
	adjustRelevants (m, pv); // To adjust the way the tile is considered...
	QGAMES::Layers rLayers = (__TM map ()) -> relevantLayers ();
	for (QGAMES::Layers::const_iterator i = rLayers.begin (); 
		i != rLayers.end () && !result; i++)
	{
		if ((*i) -> type () != __QGAMES_TILELAYER__ || 
			((*i) -> type () == __QGAMES_TILELAYER__ && ((QGAMES::PlatformTileLayer*) (*i)) -> transparent ())) 
			continue;

		QGAMES::Tiles rlTile = 
			referenceTilesNow (tilesRelevant (((QGAMES::TileLayer*) (*i)), m, pv), d, a);
		for (int j = 0; j < (int) rlTile.size () && !result; j++)
			result = (rlTile [j] != NULL && rlTile [j] -> type () == tT);
	}

	return (result);
}

// ---
bool QGAMES::PlatformArtist::collisioningWithTile (const std::vector <int> tT, 
	const QGAMES::Vector& d, const QGAMES::Vector& a)
{
	bool result = false;

	QGAMES::bdata m = (QGAMES::bdata) 0.0;
	QGAMES::Vector pv = QGAMES::Vector::_cero;
	adjustRelevants (m, pv); // To adjust the way the tile is considered...
	QGAMES::Layers rLayers = (__TM map ()) -> relevantLayers ();
	for (QGAMES::Layers::const_iterator i = rLayers.begin (); 
		i != rLayers.end () && !result; i++)
	{
		if ((*i) -> type () != __QGAMES_TILELAYER__ || 
			((*i) -> type () == __QGAMES_TILELAYER__ && ((QGAMES::PlatformTileLayer*) (*i)) -> transparent ())) 
			continue;

		QGAMES::Tiles rlTile = 
			referenceTilesNow (tilesRelevant (((QGAMES::TileLayer*) (*i)), m, pv), d, a);
		for (int j = 0; j < (int) rlTile.size () && !result; j++)
			result = (rlTile [j] != NULL && (std::find (tT.begin (), tT.end (), rlTile [j] -> type ()) != tT.end ()));
	}

	return (result);
}

// ---
bool QGAMES::PlatformArtist::overTile (int tT)
{
	bool result = false;

	QGAMES::bdata m = (QGAMES::bdata) 0.0;
	QGAMES::Vector pv = QGAMES::Vector::_cero;
	adjustRelevants (m, pv); // To adjust the way the tile is considered...
	QGAMES::Layers rLayers = (__TM map ()) -> relevantLayers ();
	for (QGAMES::Layers::const_iterator i = rLayers.begin (); 
		i != rLayers.end () && !result; i++)
	{
		if ((*i) -> type () != __QGAMES_TILELAYER__ || 
			((*i) -> type () == __QGAMES_TILELAYER__ && ((QGAMES::PlatformTileLayer*) (*i)) -> transparent ())) 
			continue;

		QGAMES::Tiles rlTile = (__TM _map) -> tilesAround (this, (QGAMES::TileLayer*) (*i), m, pv);
		for (int j = 0; j < (int) rlTile.size () && !result; j++)
			result = (rlTile [j] != NULL && rlTile [j] -> type () == tT);
	}

	return (result);
}

// ---
bool QGAMES::PlatformArtist::overTile (const std::vector <int> tT)
{
	bool result = false;

	QGAMES::bdata m = (QGAMES::bdata) 0.0;
	QGAMES::Vector pv = QGAMES::Vector::_cero;
	adjustRelevants (m, pv); // To adjust the way the tile is considered...
	QGAMES::Layers rLayers = (__TM map ()) -> relevantLayers ();
	for (QGAMES::Layers::const_iterator i = rLayers.begin (); 
		i != rLayers.end () && !result; i++)
	{
		if ((*i) -> type () != __QGAMES_TILELAYER__ || 
			((*i) -> type () == __QGAMES_TILELAYER__ && ((QGAMES::PlatformTileLayer*) (*i)) -> transparent ())) 
			continue;

		QGAMES::Tiles rlTile = (__TM _map) -> tilesAround (this, (QGAMES::TileLayer*) (*i), m, pv);
		for (int j = 0; j < (int) rlTile.size () && !result; j++)
			result = (rlTile [j] != NULL && (std::find (tT.begin (), tT.end (), rlTile [j] -> type ()) != tT.end ()));
	}

	return (result);
}

// ---
QGAMES::Tile* QGAMES::PlatformArtist::tileNotNullAtTheRightOf (QGAMES::Tile* rT, const QGAMES::Tiles& t)
{
	QGAMES::Tile* tResult = QGAMES::NullTile::_nullTile;

	int i = (int) t.size () - 1; // The definition 
	bool result = (i >= 0) ? ((t [i] != QGAMES::NullTile::_nullTile) ? true : false) : false;
	for (i--; i >= 0 && result && t [i] != rT;
			result = (t [i--] != QGAMES::NullTile::_nullTile) ? true : false);

	if (result) 
		tResult = t [i + 1]; 

	return (tResult);
}

// ---
QGAMES::Tile* QGAMES::PlatformArtist::tileNotNullAtTheLeftOf (QGAMES::Tile* rT, const QGAMES::Tiles& t)
{
	QGAMES::Tile* tResult = QGAMES::NullTile::_nullTile;

	int i = 0;
	bool result = (i < (int) t.size ()) ? ((t [i] != QGAMES::NullTile::_nullTile) ? true : false) : false;
	for (i++; i < (int) t.size () && result && t [i] != rT;
			result = (t [i++] != QGAMES::NullTile::_nullTile) ? true : false);

	if (result) 
		tResult = t [i - 1]; 

	return (tResult);
}

// ---
void QGAMES::PlatformArtist::adjustPositionToBase (bool oY)
{
	if (basePosition () == QGAMES::Position::_noPoint)
		return; // Makes no sense...when there i sno previous base position defined.

	QGAMES::Position oBase = basePosition (); // Keeps the old position...
	setFixBasePosition (); // Now, it is none...
	if (!oY) setPosition (position () + (oBase - basePosition ()));
	else setPosition (position () + 
		QGAMES::Vector (__BD 0, oBase.posY () - basePosition ().posY (), __BD 0));
	setFixBasePosition (oBase);
}

// ---
void QGAMES::PlatformArtist::adjustPositionToCenter (const QGAMES::Position& pos)
{
	if (pos == QGAMES::Position::_noPoint)
		return; // Makes no sense to continue...

	setPosition (position () + pos - centerPosition ());
}

// ---
QGAMES::Position QGAMES::PlatformArtist::roundedLayerPositionCloserTo 
	(const QGAMES::Position& pos, QGAMES::TileLayer* l, int nP, bool x, bool y, bool z)
{
	assert (l);

	QGAMES::Position lP = l -> position ();
	int tW = ((QGAMES::TiledMap*) l -> map ()) -> tileWidth ();
	int tH = ((QGAMES::TiledMap*) l -> map ()) -> tileHeight ();
	int tD = ((QGAMES::TiledMap*) l -> map ()) -> tileDepth ();

	int ePX = (((int) (pos.posX () - lP.posX ()) / tW) * tW) + (int) lP.posX ();
	int ePY = (((int) (pos.posY () - lP.posY ()) / tH) * tH) + (int) lP.posY ();
	int ePZ = (((int) (pos.posZ () - lP.posZ ()) / (tD == 0 ? 1 : 0)) * tD) + (int) lP.posZ ();

	return (QGAMES::Position (x ? __BD (ePX + nP) : pos.posX (),
							  y ? __BD (ePY + nP) : pos.posY (),
							  z ? __BD (ePZ + nP) : pos.posZ ()));
}
