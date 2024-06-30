#include <Arcade/artist.hpp>
#include <Arcade/map.hpp>
#include <Common/event.hpp>

// ---
void QGAMES::Artist::updatePositions ()
{
	QGAMES::Character::updatePositions ();
	// Just to conserve the chain...
	// It is something requiered by the compiler
}

// ---
void QGAMES::ArtistInATiledMap::StandardTileCollisionHandler::actionFor 
	(QGAMES::ArtistInATiledMap* a, QGAMES::Tile* t)
{
	if (!used () || repeat ())
	{
		a -> notify (QGAMES::Event (__QGAMES_COLLISIONWITHTILE__, t));
		setUsed (true);
	}
}

// ---
bool QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::used (int nF) const
{
	QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::
		DetectionStates::const_iterator i = _detectionStates.find (nF);
	return ((i == _detectionStates.end ()) ? false : (*i).second -> _used);
}

// ---
void QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::actionFor (QGAMES::ArtistInATiledMap* a, Tile* t)
{
	if (!used (t -> numberFrame ()) || repeat ())
	{
		QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::NotificationInfo* nI = 
			notificationInfo (t -> numberFrame ());
		nI -> _tile = t;
		nI -> _owner = a;
		a -> notify (QGAMES::Event (__QGAMES_COLLISIONWITHTILE__, nI));

		// It is not declared as used until it is really used.
		// the _detectionState attribute of the variable nI included as part of the notification has to be set!
	}
}

// ---
void QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::initializeDetectionStates ()
{
	for (QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::
			DetectionStates::const_iterator i = _detectionStates.begin (); i != _detectionStates.end (); i++)
		delete ((*i).second);
	_detectionStates.clear ();
}

// ---
void QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::deleteNotificationInfo ()
{
	for (QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::
			NotificationInfos::const_iterator i = _notificationInfos.begin (); i != _notificationInfos.end (); i++)
		delete ((*i).second);
	_notificationInfos.clear ();
}

// ---
QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::DetectionState* 
	QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::detectionState (int nF)
{
	QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::DetectionState* result = NULL;

	QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::
		DetectionStates::const_iterator i = _detectionStates.find (nF);
	if (i == _detectionStates.end ())
	{
		result = new QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::DetectionState (nF, false);
		_detectionStates.insert 
			(QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::DetectionStates::value_type (nF, result));
	}
	else
		result = (*i).second;

	return (result);
}

// ---
QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::NotificationInfo* 
	QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::notificationInfo (int nF)
{
	QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::NotificationInfo* result = NULL;

	QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::
		NotificationInfos::const_iterator i = _notificationInfos.find (nF);
	if (i == _notificationInfos.end ())
	{
		result = new QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::NotificationInfo (detectionState (nF), NULL);
		_notificationInfos.insert 
			(QGAMES::ArtistInATiledMap::TileCollisionHandlerPerFrame::NotificationInfos::value_type (nF, result));
	}
	else
		result = (*i).second;

	return (result);
}

// ---
QGAMES::ArtistInATiledMap::~ArtistInATiledMap ()
{
	for (QGAMES::ArtistInATiledMap::TileCollisionHandlers::const_iterator i = _tileHitHandlers.begin ();
			i != _tileHitHandlers.end (); i++)
		delete ((*i).second);
	_tileHitHandlers.clear ();
}

// ---
QGAMES::Rectangle QGAMES::ArtistInATiledMap::tileRectangle () const
{
	assert (map ());

	QGAMES::Rectangle result = QGAMES::Rectangle::_noRectangle;
	switch (((QGAMES::TiledMap*) map ()) -> visualOrientation ())
	{
		case QGAMES::TileLayer::VisualOrientation::_ORTHOGONAL:
		case QGAMES::TileLayer::VisualOrientation::_EXTERANLLYDEFINED:
			result = roundingFormZone ();
			break;

		case QGAMES::TileLayer::VisualOrientation::_ISOMETRIC:
		case QGAMES::TileLayer::VisualOrientation::_CAVALIER:
			result = baseZone ();
			break;

		default:
			assert (0); // It shouldn't be here...
			break;
	}

	return (result);
}

// ---
void QGAMES::ArtistInATiledMap::tilesOcuppied (int& pX, int& pY, int& pZ, 
		int& w, int& h, int& d, 
		const QGAMES::Position& dt, 
		QGAMES::bdata m, const QGAMES::Vector& pv)
{
	// A map has to exist to determinate the numner of tile occuppied by the entity
	assert (_map);

	// If nothing has changed the calculuis is avoided
	QGAMES::ArtistInATiledMap::TilesOcuppiedCalculus cal (this, dt, m, pv);
	if (_calculus == cal)
	{
		_calculus.putResultInto (pX, pY, pZ, w, h, d);
		return; // ...simplifiying the routine, and avoiding floating point calulation!
	}

	// General data related with the map...
	int tWidth = (__TM _map) -> visualTileWidth (); // Tile width in pixels
	int tHeight = (__TM _map) -> visualTileHeight (); // Tile height in pixels
	int tDepth = (__TM _map) -> visualTileDepth (); // Tile depth in pixels
	assert (tWidth != 0 && tHeight != 0); // The sizes of the map where the entity is can't be 0...except z sizes (meaning 2d plain entity)
	int ptWidth = (int) (tWidth * m); // Adjust in the x axis
	int ptHeight = (int) (tHeight * m); // Adjust in the y axis
	int ptDepth = (int) (tDepth * m); // Adjust in the z axis

	// General data related with the entity...
	int eDepth = _currentForm -> frameDepthForFrame (_currentAspect); // Entity depth in pixels
	// In 2D and in 2.5D this value usually is 0 (not the visual equivalent)
	// Pointing that the the sprite is a 2D sprite...

	// The rectangle used to determinate the tiles ocuppied by the artist
	// will depend on the perspective of the map where the artist is moving
	// The rectangle has no z dimension ever. 
	// The value of that dimension will remain on the depth defined at frame level.
	QGAMES::Rectangle tR = tileRectangle ();

	QGAMES::bdata dtXP = (dt == QGAMES::Position::_noPoint) ? (bdata) 0.0 : dt.posX ();
	int ePosXP = ((int) (tR.pos1 ().posX () - dtXP) / tWidth) + (int) pv.posX ();
	if (((int) (tR.pos1 ().posX () - dtXP) % tWidth) >= ptWidth) ePosXP++;
	int ePosX2P = ((int) (tR.pos3 ().posX () - dtXP) / tWidth) + (int) pv.posX ();
	if (((int) (tR.pos3 ().posX () - dtXP) % tWidth) <= (tWidth - ptWidth)) ePosX2P--;
	int tWP = (ePosX2P - ePosXP) + 1; if (tWP <= 0) tWP = 1;

	QGAMES::bdata dtYP = (dt == QGAMES::Position::_noPoint) ? (bdata) 0.0 : dt.posY ();
	int ePosYP = ((int) (tR.pos1 ().posY () - dtYP) / tHeight) + (int) pv.posY ();
	if (((int) (tR.pos1 ().posY () - dtYP) % tHeight) >= ptHeight) ePosYP++;
	int ePosY2P = ((int) (tR.pos4 ().posY () - dtYP) / tHeight) + (int) pv.posY ();
	if (((int) (tR.pos4 ().posY () - dtYP) % tHeight) <= (tHeight - ptHeight)) ePosY2P--;
	int tHP = (ePosY2P - ePosYP) + 1; if (tHP <= 0) tHP = 1;

	QGAMES::bdata dtZP = (dt == QGAMES::Position::_noPoint) ? (bdata) 0.0 : dt.posZ ();
	int ePosZP = (int) (tR.pos1 ().posZ () - dtZP); int tDP = 0; 
	if (tDepth != 0)
	{
		ePosZP = ((int) (tR.pos1 ().posZ () - dtZP) / tDepth) + (int) pv.posZ ();
		if (((int) (tR.pos1 ().posZ () - dtZP) % tDepth) >= ptDepth) ePosZP++;
		int ePosZ2P = ((int) (tR.pos1 ().posX () + eDepth /** frame depth needed. */ - dtZP) / tDepth) + (int) pv.posZ ();
		if (((int) (tR.pos1 ().posX () + eDepth /** frame depth needed. */ - dtZP) % tDepth) <= (tDepth - ptDepth)) ePosZ2P--;
		tDP = (ePosZ2P - ePosZP) + 1; if (tDP <= 0) tDP = 1;
	}

	pX = ePosXP; pY = ePosYP; pZ = ePosZP; w = tWP, h = tHP; d = tDP;
	// ...but actalize first the data used...
	_calculus.actualize (this, dt, m, pv, pX, pY, pZ, w, h, d);
}

// ---
void QGAMES::ArtistInATiledMap::setMap (QGAMES::Map* m)
{
	assert (m == NULL || (m != NULL && dynamic_cast <QGAMES::TiledMap*> (m) != NULL)); // Has to be a tiled map just in case...

	QGAMES::Artist::setMap (m);
}

// ---
void QGAMES::ArtistInATiledMap::updatePositions ()
{
	// First of all, the artist is moved...
	QGAMES::Artist::updatePositions ();

	// If there is no map, there is no any collision to detect...
	// At that point the map can be null...
	if (!_map)
		return;

	bdata m = (bdata) 0.0;
	QGAMES::Vector pv = QGAMES::Vector::_cero;
	adjustRelevants (m, pv); // This is to detect the influence box of the tiles...
	QGAMES::Layers rLayers = (__TM _map) -> relevantLayers ();
	for (QGAMES::Layers::const_iterator i = rLayers.begin (); 
		i != rLayers.end (); i++)
	{
		// The collision with a tile, can only happen within a tiled layer...
		if ((*i) -> type () != __QGAMES_TILELAYER__) continue;

		// Apart of that, other things could be done taken into account the tiles around (all)
		int ePosX, ePosY, ePosZ, tW, tH, tD;
		tilesOcuppied (ePosX, ePosY, ePosZ /* no sense in 2D */, 
			tW, tH, tD /** no sense in 2D */, (*i) -> position (), m, pv); // Overloaded (ePosX, ... tW...fullfilled)
		QGAMES::Tiles tA = (__TM _map) -> tilesAround (this, (QGAMES::TileLayer*) (*i), m, pv);
		whenOver (tA, (QGAMES::TileLayer*) (*i), tW, tH, tD); 

		// Otherwise get the relevant tiles of the specific layer... 
		// and what to do is there...
		QGAMES::SetOfTiles relevant = 
			tilesRelevant (((QGAMES::TileLayer*) (*i)), m, pv); // Overloaded
		whenCollisionWithTiles (relevant, (QGAMES::TileLayer*) (*i));
		
		// Actualize the tiles attached to the artist if any
		if (_attachedToTile)
		{
			if (relevant [_tileAttached].size () != (size_t) 0)
			{
				// Deletes the previous tile attached!
				if (_refTileAttached != NULL)
					_refTileAttached -> deleteArtistLiked (this); 
				// ...before adding a new one
				(_refTileAttached = relevant [_tileAttached][relevant [_tileAttached].size () / 2]) -> 
					addArtistLinked (this); 
			}
		}
	}
}

// ---
QGAMES::SetOfTiles QGAMES::ArtistInATiledMap::tilesRelevant 
	(QGAMES::TileLayer* tL, QGAMES::bdata m, const QGAMES::Vector& pv)
{
	assert (tL);

	// Tiles relevant per position in the space...
	QGAMES::SetOfTiles result (6, Tiles ());
	
	if (_map)
	{
		int ePosX, ePosY, ePosZ, tW, tH, tD;
		bdata m = (bdata) 0.0;
		QGAMES::Vector pv = QGAMES::Vector::_cero;
		adjustRelevants (m, pv); // Overloaded 
		tilesOcuppied (ePosX, ePosY, ePosZ /* no sense in 2D */, 
			tW, tH, tD /** no sense in 2D */, tL -> position (), m, pv); // Overloaded (ePosX, ... tW...fullfilled)
		QGAMES::Tiles tA = (__TM _map) -> tilesAround (this, tL, m, pv);
		std::vector <int> up, down, right, left, front, back;
		relevantTilePositions (tW, tH, tD /* no sense in 2D */, up, down, right, left, 
			front /* no sense in 2D */, back /* no sense in 2D. */); // Overloaded (up, down, ... fullfilled)
		for (int i = 0; i < (int) up.size (); result [UP].push_back (tA [up [i++]]));
		for (int i = 0; i < (int) right.size (); result [RIGHT].push_back (tA [right [i++]]));
		for (int i = 0; i < (int) down.size (); result [DOWN].push_back (tA [down [i++]]));
		for (int i = 0; i < (int) left.size (); result [LEFT].push_back (tA [left [i++]]));
		for (int i = 0; i < (int) front.size (); result [FRONT].push_back (tA [front [i++]]));
		for (int i = 0; i < (int) back.size (); result [BEHIND].push_back (tA [back [i++]]));
	}

	return (result);
}

// ---
void QGAMES::ArtistInATiledMap::whenCollisionWith (const QGAMES::Tiles& t, 
	QGAMES::ArtistInATiledMap::TilePosition tP, QGAMES::TileLayer* l)
{
	assert (l);

	QGAMES::ArtistInATiledMap::TileCollisionHandlers::const_iterator j;
	if (_tileHitHandlers.size () == 0)
		return; // No sense if there is no handlers registered!

	for (int i = 0; i < (int) t.size (); i++)
	{
		j = _tileHitHandlers.find (t [i] -> type ());
		if (j != _tileHitHandlers.end ())
			(*j).second -> actionFor (this, t [i]);
	}
}

// ---
void QGAMES::ArtistInATiledMap::relevantTilePositions (int w, int h, int d, 
	std::vector <int>& up, std::vector <int>& down, 
	std::vector <int>& right, std::vector <int>& left, 
	std::vector <int>& front, std::vector <int>& back)
{
	up.clear (); down.clear (); 
	right.clear (); left.clear (); 
	front.clear (); back.clear ();

	int lL = (h - 1) * w; 
	for (int i = 0; i < w; up.push_back (i++));
	for (int i = 0; i < w; down.push_back (lL + i++));
	for (int i = 0; i < h; left.push_back (w * i++));
	for (int i = 0; i < h; right.push_back ((w * i++) + (w - 1)));
}

// ---
void QGAMES::ArtistInATiledMap::whenCollisionWithTiles (const QGAMES::SetOfTiles& ts, 
	QGAMES::TileLayer* tL)
{
	assert (tL);
	assert (ts.size () == 6);

	// It is time to check whether the artist is or not bumping something...
	// The character is moving to the right, left, down or up, in a three dimension world...
	// The door is open to test another possibilites...
	if (_data._orientation.posX () > 0) whenCollisionWith (ts [RIGHT], RIGHT, tL); // Going to the right?
	if (_data._orientation.posX () < 0) whenCollisionWith (ts [LEFT], LEFT, tL); // Going to the left?
	if (_data._orientation.posY () > 0) whenCollisionWith (ts [DOWN], DOWN, tL); // Going down?
	if (_data._orientation.posY () < 0) whenCollisionWith (ts [UP], UP, tL); // Going up?
	if (_data._orientation.posZ () > 0) whenCollisionWith (ts [BEHIND], BEHIND, tL); // Going to the back?
	if (_data._orientation.posZ () < 0) whenCollisionWith (ts [FRONT], FRONT, tL); // Going to the front?

	// In any case, if the character is not moving up, 
	// there would be something behind
	// ...and it is needed to check what hapen when the character is on it...
	if (_data._orientation.posY () >= 0) whenOnTile (ts [DOWN], tL); // Or nothing or moving down...
}

// ---
void QGAMES::ArtistInATiledMap::initializeTileHitHandlers ()
{
	for (QGAMES::ArtistInATiledMap::TileCollisionHandlers::const_iterator i = _tileHitHandlers.begin ();
		i != _tileHitHandlers.end (); i++)
			(*i).second -> initialize ();
}

// ---
QGAMES::ArtistInATiledMap::TilesOcuppiedCalculus::TilesOcuppiedCalculus (ArtistInATiledMap* a,
		const QGAMES::Position& dt, QGAMES::bdata m, const QGAMES::Vector& pv)
	: _form (0), _aspect (0), _position (QGAMES::Position::_noPoint),
	  _delta (dt), _m (m), _pV (pv),
	  _pX (0), _pY (0), _pZ (0), _w (0), _h (0), _d (0)
{ 
	assert (a);
	_form = a -> _currentForm -> id (); 
	_aspect = a -> _currentAspect; 
	_position = a -> position ();
}

// ---
void QGAMES::ArtistInATiledMap::TilesOcuppiedCalculus::actualize (QGAMES::ArtistInATiledMap* a,
	const QGAMES::Position& dt, QGAMES::bdata m, const QGAMES::Vector& pv, 
	int pX, int pY, int pZ, int w, int h, int d)
{ 
	assert (a);

	_form = a -> _currentForm -> id ();
	_aspect = a ->_currentAspect;
	_position = a -> position ();
	_delta = dt; _m = m; _pV = pv;
	_pX = pX; _pY = pY; _pZ = pZ; _w = w; _h = h; _d = d; 
}
