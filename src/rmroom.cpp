#include <Room/rmroom.hpp>

// ---
QGAMES::RoomElement::RoomElement (int eId, int fId, int aId, 
		const QGAMES::Position& pos, const QGAMES::Forms& f, const QGAMES::Entity::Data& d)
	: QGAMES::Entity (eId, f, d)
{ 
	setCurrentForm (fId); 
	setCurrentAspect (aId); 
	setPosition (pos); 
}

QGAMES::RoomWall::WallElement::WallElement 
		(int id, QGAMES::RoomElement* rE, const QGAMES::RoomWall::Hole& h)
	: _id (id), _wallElement (rE), _where (h)
{
	assert (_wallElement != nullptr);
}

// ---
QGAMES::RoomWall::WallElement::WallElement (const QGAMES::RoomWall::WallElement& wE)
	: _id (wE._id), _wallElement (nullptr), _where (wE._where)
{
	_wallElement = dynamic_cast <QGAMES::RoomElement*> (wE._wallElement -> clone ());
	assert (_wallElement); // Just in case...
}

// ---
QGAMES::RoomWall::WallElement& QGAMES::RoomWall::WallElement::operator = 
	(const QGAMES::RoomWall::WallElement& wE)
{
	_id = wE._id;
	_wallElement = dynamic_cast <QGAMES::RoomElement*> (wE._wallElement -> clone ());
	assert (_wallElement);
	_where = wE._where;

	return (*this);
}

// ---
QGAMES::RoomWall::RoomWall (int id, int brickEId, 
	const QGAMES::Position& iPos,
	int nBL, int nBH, int nBD,
	const QGAMES::RoomWall::WallElements& d)
	: _id (id),
	  _brickEntityId (brickEId),
	  _initialPosition (iPos),
	  _numberBricksLength (nBL), 
	  _numberBricksHigh (nBH), 
	  _numberBricksDepth (nBD),
	  _wallElements (d),
	  _baseBrick (nullptr),
	  _lengthDirection (QGAMES::Vector::_cero), 
	  _heightDirection (QGAMES::Vector::_cero), 
	  _depthDirection (QGAMES::Vector::_cero),
	  _bricks (),
	  _initialized (false), // When created, and not initialized!
	  _scene (nullptr)
{
	assert (_numberBricksLength > 0 && _numberBricksHigh > 0 && _numberBricksDepth > 0);

	// It has to exist...
	// It has to set the state needed to calculate the dimensions to build up the wall...
	// The brick must have a size, in two dimensions at least!
	_baseBrick = dynamic_cast <QGAMES::RoomElement*> (QGAMES::Game::game () -> entity (_brickEntityId));
	assert (_baseBrick != nullptr);
	_baseBrick -> initialize (); 
	assert ((_baseBrick -> visualLength () != 0 && _baseBrick -> visualHeight () != 0) ||
			(_baseBrick -> visualLength () != 0 && _baseBrick -> visualDepth () != 0) ||
			(_baseBrick -> visualHeight () != 0 && _baseBrick -> visualDepth () != 0));

	// Create the bricks and locate them in the wall (The unity of measurement is the brick itself)
	// The direction of the different measurements will depend on the type of brick projection!
	// Sure they won't be the same in orthogonal than in isometric e.g.
	_lengthDirection = 
		(_baseBrick -> roundingBox ().bottomRectangle ().pos3 () - 
		 _baseBrick -> roundingBox ().bottomRectangle ().pos1 ()).normalize ();
	_depthDirection = 
		(_baseBrick -> roundingBox ().bottomRectangle ().pos4 () - 
		 _baseBrick -> roundingBox ().bottomRectangle ().pos1 ()).normalize ();
	_heightDirection = -
		(_baseBrick -> roundingBox ().betweenFrontAndBackRectangle ().pos4 () - 
		 _baseBrick -> roundingBox ().betweenFrontAndBackRectangle ().pos1 ()).normalize ();
	for (int j = 0; j < _numberBricksDepth; j++)
	{
		for (int k = 0; k < _numberBricksHigh; k++)
		{
			for (int l = 0; l < _numberBricksLength; l++)
			{
				QGAMES::RoomWall::Hole hole ((float) l, (float) k, (float) j);
				_bricks.insert (QGAMES::RoomWall::Bricks::value_type 
					(idBrickOnHole (hole), 
					 new QGAMES::RoomWall::WallElement 
						(idBrickOnHole (hole), dynamic_cast <QGAMES::RoomElement*> (_baseBrick -> clone ()), hole)));
			}
		}
	}
}

// ---
QGAMES::RoomWall* QGAMES::RoomWall::clone () const
{
	QGAMES::RoomWall::WallElements wEN;
	for (auto i : _wallElements)
		wEN.insert (QGAMES::RoomWall::WallElements::value_type
			(i.first, new QGAMES::RoomWall::WallElement (*(i.second)))); // Using the copy constructor...

	QGAMES::RoomWall* result = new QGAMES::RoomWall (_id, _brickEntityId, 
		_initialPosition, _numberBricksLength, _numberBricksHigh, _numberBricksDepth, wEN);
	auto i = result -> _bricks.begin ();
	while (i != result -> _bricks.end ())
	{
		if (_bricks.find ((*i).first) == _bricks.end ())
			i = result -> _bricks.erase (i);
		else
			i++;
	}

	return (result);
}

// ---
const QGAMES::RoomWall::WallElement* QGAMES::RoomWall::brick (int id) const
{
	QGAMES::RoomWall::Bricks::const_iterator i = _bricks.find (id);
	assert (i != _bricks.end ()); 

	return ((*i).second);
}

// ---
void QGAMES::RoomWall::setVisibleBricksFromHoles (const QGAMES::RoomWall::Holes& h, bool v)
{
	for (auto i : h)
	{
		QGAMES::RoomWall::Bricks::const_iterator j = _bricks.find (idBrickOnHole (i));
		if (j != _bricks.end ())
			(*j).second -> _wallElement -> setVisible (v);
	}
}

// ---
void QGAMES::RoomWall::addBricksInHoles (const QGAMES::RoomWall::Holes& h)
{
	for (auto i : h)
	{
		QGAMES::RoomWall::Bricks::const_iterator j = _bricks.find (idBrickOnHole (i));
		if (j == _bricks.end ())
		{
			int idBrick = idBrickOnHole (i);
			_bricks.insert (QGAMES::RoomWall::Bricks::value_type 
				(idBrick, new QGAMES::RoomWall::WallElement 
					(idBrick, dynamic_cast <QGAMES::RoomElement*> (_baseBrick -> clone ()), i)));

			// Whether the wall has alrady been initialized, 
			// the brick has to be initialized and also added to the scene...
			if (_initialized)
			{
				_bricks [idBrick] -> _wallElement -> initialize ();

				_bricks [idBrick] -> _wallElement -> 
					setPositionOnBase (_initialPosition +
						(__BD _bricks [idBrick] -> _where._positionDepth *  (__BD _baseBrick -> visualDepth ()  * _depthDirection)) +
						(__BD _bricks [idBrick] -> _where._positionHeight * (__BD _baseBrick -> visualHeight () * _heightDirection)) +
						(__BD _bricks [idBrick] -> _where._positionLength * (__BD _baseBrick -> visualLength () * _lengthDirection)));

				_scene -> addEntity (_bricks [idBrick] -> _wallElement);
			}
		}
	}
}

// ---
void QGAMES::RoomWall::removeBricksFromHoles (const QGAMES::RoomWall::Holes& h)
{
	std::vector <int> toRemove;

	for (auto i : h)
		if (existsBrickOnHole (i))
			toRemove.push_back (idBrickOnHole (i));

	for (auto i : toRemove)
	{
		if (_initialized)
		{
			const QGAMES::RoomWall::WallElement* brk = brick (i);
			
			brk -> _wallElement -> finalize ();
			
			_scene -> removeEntity (brk -> _wallElement);
		}

		_bricks.erase (i);
	}
}

// ---
const QGAMES::RoomWall::WallElement* QGAMES::RoomWall::wallElement (int id) const
{
	QGAMES::RoomWall::WallElements::const_iterator i = _wallElements.find (id);
	assert (i != _wallElements.end ());

	return ((*i).second);
}

// ---
void QGAMES::RoomWall::addWallElement (QGAMES::RoomWall::WallElement* wE)
{
	assert (wE != nullptr);

	QGAMES::RoomWall::WallElements::const_iterator i = _wallElements.find (wE -> _id);
	assert (i == _wallElements.end ());

	_wallElements.insert (QGAMES::RoomWall::WallElements::value_type (wE -> _id, wE));

	// If the wall has been initialized, the new element has to be also initialized!
	if (_initialized)
	{
		_wallElements [wE -> _id] -> _wallElement -> initialize ();

		_wallElements [wE -> _id] -> _wallElement -> setPositionOnBase 
			(_initialPosition + 
				(__BD _wallElements [wE -> _id] -> _where._positionDepth  * 
					(__BD _baseBrick -> visualDepth ()  * _depthDirection)) +
				(__BD _wallElements [wE -> _id] -> _where._positionHeight * 
					(__BD _baseBrick -> visualHeight () * _heightDirection)) +
				(__BD _wallElements [wE -> _id] -> _where._positionLength * 
					(__BD _baseBrick -> visualLength () * _lengthDirection)));

		_scene -> addEntity (_wallElements [wE -> _id] -> _wallElement);
	}
}

void QGAMES::RoomWall::removeWallElement (int id)
{
	QGAMES::RoomWall::WallElements::const_iterator i = _wallElements.find (id);
	assert (i != _wallElements.end ());

	if (_initialized)
	{
		(*i).second -> _wallElement -> finalize ();

		_scene -> removeEntity ((*i).second -> _wallElement);
	}

	delete ((*i).second);

	_wallElements.erase (id);
}

// ---
void QGAMES::RoomWall::move (const QGAMES::Vector& v)
{
	_initialPosition += v;

	for (auto i : _bricks)
		i.second -> _wallElement -> setPosition (i.second -> _wallElement -> position () + v);

	for (auto i : _wallElements)
		i.second -> _wallElement -> setPosition (i.second -> _wallElement -> position () + v);
}

// ---
void QGAMES::RoomWall::initializeOn (QGAMES::Scene* scn)
{ 
	if (_initialized)
		return; // Just once...

	assert (scn);

	_initialized = true;
	_scene = scn;

	for (auto i : _bricks)
	{
		i.second -> _wallElement -> initialize ();

		i.second -> _wallElement -> 
			setPositionOnBase (_initialPosition +
				(__BD i.second -> _where._positionDepth *  (__BD _baseBrick -> visualDepth ()  * _depthDirection)) +
				(__BD i.second -> _where._positionHeight * (__BD _baseBrick -> visualHeight () * _heightDirection)) +
				(__BD i.second -> _where._positionLength * (__BD _baseBrick -> visualLength () * _lengthDirection)));

		_scene -> addEntity (i.second -> _wallElement);
	}

	for (auto i : _wallElements)
	{
		i.second -> _wallElement -> initialize ();

		i.second -> _wallElement -> setPositionOnBase 
			(_initialPosition + 
				(__BD i.second -> _where._positionDepth  * (__BD _baseBrick -> visualDepth ()  * _depthDirection)) +
				(__BD i.second -> _where._positionHeight * (__BD _baseBrick -> visualHeight () * _heightDirection)) +
				(__BD i.second -> _where._positionLength * (__BD _baseBrick -> visualLength () * _lengthDirection)));

		_scene -> addEntity (i.second -> _wallElement);
	}
}

// ---
void QGAMES::RoomWall::finalize ()
{
	if (!_initialized)
		return;

	_initialized = false;

	for (auto i : _bricks)
	{
		i.second -> _wallElement -> finalize ();

		_scene -> removeEntity (i.second -> _wallElement);
	}

	for (auto i : _wallElements)
	{
		i.second -> _wallElement -> finalize ();

		_scene -> removeEntity (i.second -> _wallElement);
	}
}

// ---
void QGAMES::RoomWall::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	assert (s != nullptr);

#ifndef NDEBUG
	for (auto i : _bricks)
		s -> drawBox (i.second -> _wallElement -> roundingBox (), __QGAMES_YELLOWCOLOR__);

	for (auto i : _wallElements)
		s -> drawBox (i.second -> _wallElement -> roundingBox (), __QGAMES_REDCOLOR__);
#endif
}

// ---
bool QGAMES::RoomWall::hasCollisionWith (QGAMES::Entity* e) const
{
	assert (e != nullptr);

	bool result = false;

	for (QGAMES::RoomWall::Bricks::const_iterator i = _bricks.begin (); 
			i != _bricks.end () && !result; i++)
		result = e -> hasCollisionWith ((*i).second -> _wallElement);

	for (QGAMES::RoomWall::WallElements::const_iterator i = _wallElements.begin (); 
			i != _wallElements.end () && !result; i++)
		result = e -> hasCollisionWith ((*i).second -> _wallElement);

	return (result);
}

// ---
QGAMES::Entity* QGAMES::RoomWall::firstElementCollisioningWith (QGAMES::Entity* e)
{
	assert (e != nullptr);

	QGAMES::Entity* result = NULL;

	for (QGAMES::RoomWall::Bricks::const_iterator i = _bricks.begin (); 
			i != _bricks.end () && result == nullptr; i++)
		if (e -> hasCollisionWith ((*i).second -> _wallElement)) result = (*i).second -> _wallElement;
	for (QGAMES::RoomWall::WallElements::const_iterator i = _wallElements.begin (); 
			i != _wallElements.end () && result == nullptr; i++)
		if (e -> hasCollisionWith ((*i).second -> _wallElement)) result = (*i).second -> _wallElement;

	return (result);
}

// ---
QGAMES::Room::DecorativeElement::DecorativeElement 
		(int id, QGAMES::RoomElement* e, const QGAMES::Position& p)
	: _id (id), _element (e), _where (p)
{ 
	assert (_element != nullptr);
}

// ---
QGAMES::Room::DecorativeElement::DecorativeElement (const QGAMES::Room::DecorativeElement& dE)
	: _id (dE._id), _element (nullptr), _where (dE._where)
{
	_element = dynamic_cast <QGAMES::RoomElement*> (dE._element -> clone ());
	assert (_element);
}

// ---
QGAMES::Room::DecorativeElement& QGAMES::Room::DecorativeElement::operator = (const QGAMES::Room::DecorativeElement& dE)
{
	_id = dE._id;
	_element = dynamic_cast <QGAMES::RoomElement*> (dE._element -> clone ());
	assert (_element);
	_where = dE._where;

	return (*this);
}

// ---
QGAMES::Room* QGAMES::Room::clone () const
{
	QGAMES::RoomWalls wN;
	for (auto i : _walls)
		wN.insert (QGAMES::RoomWalls::value_type (i.first, i.second -> clone ())); 
	
	QGAMES::Room::Decoration dN;
	for (auto i : _decoration)
		dN.insert (QGAMES::Room::Decoration::value_type (i.first, 
			new QGAMES::Room::DecorativeElement (*(i.second)))); // Using the copy constructor...

	QGAMES::Room* result = new QGAMES::Room (_id, _type, wN, dN);

	return (result);
}

// ---
const QGAMES::RoomWall* QGAMES::Room::wall (int id) const
{
	QGAMES::RoomWalls::const_iterator i = _walls.find (id);
	assert (i != _walls.end ()); // Has to exist...

	return ((*i).second);
}

// ---
void QGAMES::Room::addWall (QGAMES::RoomWall* w)
{
	assert (w != nullptr);

	QGAMES::RoomWalls::const_iterator i = _walls.find (w -> id ());
	assert (i == _walls.end ()); // It hasn't to exist already

	_walls.insert (QGAMES::RoomWalls::value_type (w -> id (), w));
}

// ---
void QGAMES::Room::removeWall (int id)
{
	QGAMES::RoomWalls::iterator i = _walls.find (id);
	assert (i != _walls.end ()); // Has to exist...

	if (_initialized)
		(*i).second -> finalize ();

	delete ((*i).second);

	_walls.erase (i);
}

// ---
const QGAMES::Room::DecorativeElement* QGAMES::Room::decorativeElement (int id) const
{
	QGAMES::Room::Decoration::const_iterator i = _decoration.find (id);
	assert (i != _decoration.end ());

	return ((*i).second);
}

// ---
void QGAMES::Room::addDecorativeElement (QGAMES::Room::DecorativeElement* dE)
{
	assert (dE != nullptr);

	QGAMES::Room::Decoration::const_iterator i = _decoration.find (dE -> _id);
	assert (i == _decoration.end ());

	_decoration.insert (QGAMES::Room::Decoration::value_type (dE -> _id, dE));

	// If the room has been initialized, the new element has to be also initialized!
	if (_initialized)
	{
		_decoration [dE -> _id] -> _element -> initialize ();
		_decoration [dE -> _id] -> _element -> setPositionOnBase (_decoration [dE -> _id] -> _where);

		_scene -> addEntity (_decoration [dE -> _id] -> _element);
	}
}

// ---
void QGAMES::Room::removeDecorativeElement (int id)
{
	QGAMES::Room::Decoration::const_iterator i = _decoration.find (id);
	assert (i != _decoration.end ());

	if (_initialized)
	{
		(*i).second -> _element -> finalize ();

		_scene -> removeEntity ((*i).second -> _element);
	}

	delete ((*i).second);

	_decoration.erase (id);
}

// ---
void QGAMES::Room::addAllFromRoom (QGAMES::Room* r)
{
	assert (r != nullptr);

	QGAMES::RoomWalls wC;
	for (auto i : r -> _walls)
		wC.insert (QGAMES::RoomWalls::value_type (i.first, i.second -> clone ())); 
	addWalls (wC);

	QGAMES::Room::Decoration dC;
	for (auto i : r -> _decoration)
		dC.insert (QGAMES::Room::Decoration::value_type
			(i.first, new QGAMES::Room::DecorativeElement (*(i.second)))); // Using the copy constructor...
	addDecoration (dC);
}

// ---
void QGAMES::Room::initializeOn (QGAMES::Scene* scn)
{
	if (_initialized)
		return;

	assert (scn != nullptr);

	_initialized = true;
	_scene = scn;

	for (auto i : _walls) 
		i.second -> initializeOn (_scene);

	for (auto i : _decoration)
	{
		i.second -> _element -> initialize ();

		i.second -> _element -> setPositionOnBase (i.second -> _where);

		_scene -> addEntity (i.second -> _element);
	}
}

// ---
void QGAMES::Room::finalize ()
{
	if (!_initialized)
		return;

	_initialized = false;

	for (auto i : _walls)
		i.second -> finalize ();

	for (auto i : _decoration)
	{
		i.second -> _element -> finalize ();

		_scene -> removeEntity (i.second -> _element);
	}
}

// ---
void QGAMES::Room::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{ 
	assert (s != nullptr);

	for (auto i : _walls) 
		i.second -> drawOn (s, p); 

#ifndef NDEBUG
	for (auto i : _decoration)
		s -> drawBox (i.second -> _element -> roundingBox (), __QGAMES_REDCOLOR__);
#endif
}

// ---
QGAMES::BasicRoomScene::BasicRoomScene (int c, QGAMES::Room* r,
		const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn, 
		const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
	: QGAMES::SceneWithBackground (c, m, cn, p, ePL),
	  _room (r)
{
	assert (_room != nullptr);

	_room ->  setScene (this); 
}

// ---
QGAMES::PlatformRoomScene::PlatformRoomScene (int c, QGAMES::Room* r,
		const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn, 
		const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL)
	: QGAMES::PlatformScene (c, m, cn, p, ePL),
	  _room (r)
{
	assert (_room != nullptr);

	_room ->  setScene (this); 
}

// ---
QGAMES::WorldBasedOnRooms::WorldBasedOnRooms (int c, const QGAMES::Scenes& s, QGAMES::MazeModel* m,
			const QGAMES::WorldProperties& p)
	: QGAMES::World (c, s, p),
	  _model (m)
{
	for (auto i : scenes ())
		assert (dynamic_cast <QGAMES::BasicRoomScene*> (i.second) ||
				dynamic_cast <QGAMES::PlatformScene*> (i.second)); 

	assert (_model != nullptr);
}

// ---
QGAMES::SetOfOpenValues QGAMES::WorldBasedOnRooms::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::World::runtimeValues ();

	int lNE = result.lastNestedOpenValueId ();

	result.addSetOfOpenValues (lNE + 1, _model -> asSetOfOpenValues ());

	return (result);
}

// ---
void QGAMES::WorldBasedOnRooms::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_model -> fromSetOfOpenValues (cCfg.setOfOpenValues (lNE));

	cCfg.removeSetOfOpenValues (lNE);

	QGAMES::World::initializeRuntimeValuesFrom (cCfg);
}
