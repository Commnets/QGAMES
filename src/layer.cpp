#include <Arcade/layer.hpp>
#include <Arcade/map.hpp>
#include <Arcade/artist.hpp>
#include <Common/screen.hpp>
#include <iostream>
#include <sstream>

// ---
QGAMES::Layer::ComplexVisualEffect::~ComplexVisualEffect ()
{
	for (QGAMES::Layer::VisualEffects::const_iterator i = _effects.begin ();
			i != _effects.end (); i++)
		delete ((*i));
	_effects.clear ();
}	

// ---
void QGAMES::Layer::ComplexVisualEffect::initialize ()
{
	for (QGAMES::Layer::VisualEffects::const_iterator i = _effects.begin ();
			i != _effects.end (); i++)
		(*i) -> initialize ();
}

// ---
void QGAMES::Layer::ComplexVisualEffect::updatePositions ()
{
	for (QGAMES::Layer::VisualEffects::const_iterator i = _effects.begin ();
			i != _effects.end (); i++)
		(*i) -> updatePositions ();
}

// ---
void QGAMES::Layer::ComplexVisualEffect::drawOn (Screen* s, const Position& pos)
{
	for (QGAMES::Layer::VisualEffects::const_iterator i = _effects.begin ();
			i != _effects.end (); i++)
		(*i) -> drawOn (s, pos);
}

// ---
void QGAMES::Layer::ComplexVisualEffect::finalize ()
{
	for (QGAMES::Layer::VisualEffects::const_iterator i = _effects.begin ();
			i != _effects.end (); i++)
		(*i) -> finalize ();
}

// ---
void QGAMES::Layer::MovementBehaviour::initialize ()
{ 
	_counterSpeed = 0;
	
	_vectorMoved = QGAMES::Vector::_cero; 
	_averageVectorMoved = QGAMES::Vector::_cero;

	_counterSteps = 0;
}

// ---
QGAMES::Position QGAMES::Layer::MovementBehaviour::newPosition (const QGAMES::Position& p)
{
	_counterSteps++;

	if (_originalPosition == QGAMES::Position::_noPoint)
		_originalPosition = p;

	if (hasFinished ())
	{
		_vectorMoved = QGAMES::Vector::_cero; // It doesn't move any more...
		_averageVectorMoved = QGAMES::Vector::_cero;

		return (p); // If the movement has finished, then there is nothing to move...
	}

	QGAMES::Position result = p;
	if (++_counterSpeed >= _speed)
	{
		_counterSpeed = 0;
		result = calculateNewPosition (p);
	}

	_vectorMoved = result - p;
	if (_counterSteps > 0)
		_averageVectorMoved = 
			(_averageVectorMoved * __BD (_counterSteps - 1) / __BD _counterSteps) + 
			(_vectorMoved / __BD _counterSteps);
	else
		_averageVectorMoved = _vectorMoved;

	return (result); 
}

// ---
QGAMES::Layer::SetOfMovementsBehaviour::~SetOfMovementsBehaviour ()
{
	for (int i = 0; i < (int) _movements.size (); 
		delete (_movements [i++]));
	_movements.clear ();
}

// ---
QGAMES::Layer::MovementBehaviour* QGAMES::Layer::SetOfMovementsBehaviour::clone () const
{
	QGAMES::Layer::MovementBehaviours movs;
	for (int i = 0; i < (int) _movements.size (); movs [i] = _movements [i] -> clone (), i++);
	return (new QGAMES::Layer::SetOfMovementsBehaviour (movs, _ciclic));
}

// ---
QGAMES::Layer::MovementBehaviour* QGAMES::Layer::SetOfMovementsBehaviour::opposite ()
{
	QGAMES::Layer::MovementBehaviours movs;
	for (int i = 0; i < (int) _movements.size (); movs [i] = _movements [i] -> opposite (), i++);
	return (new QGAMES::Layer::SetOfMovementsBehaviour (movs, _ciclic));
}

// ---
QGAMES::SetOfOpenValues QGAMES::Layer::SetOfMovementsBehaviour::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Layer::MovementBehaviour::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	// The current movement...
	result.addOpenValue (lE + 1, QGAMES::OpenValue (_currentMovement));

	// The status of the movements...
	// The movements are stored in a vector, so it is supossed they are sequential
	int i = 0;
	QGAMES::SetOfOpenValues mVV (__QGAMES_RUNTIMEVALUESMOVSTYPE__);
	for (QGAMES::Layer::MovementBehaviours::const_iterator j = _movements.begin ();
			j != _movements.end (); j++, i++)
		mVV.addSetOfOpenValues (i, (*j) -> runtimeValues ());
	result.addSetOfOpenValues (lNE + 1, mVV); // It could be empty...

	return (result);
}

// ---
void QGAMES::Layer::SetOfMovementsBehaviour::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();
	
	assert (cfg.existOpenValue (lE) &&
			cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_currentMovement = cCfg.openValue (lE).intValue ();
	cCfg.removeOpenValue (lE); // Just treated...

	int i = 0;
	QGAMES::SetOfOpenValues mVV = cCfg.setOfOpenValues (lNE);
	assert (mVV.name () == std::string (__QGAMES_RUNTIMEVALUESMOVSTYPE__)); // Just to guaranttee the structure...
	for (QGAMES::Layer::MovementBehaviours::const_iterator j = _movements.begin ();
			j != _movements.end (); j++, i++)
		if (mVV.existSetOfOpenValues (i)) // Only if it exists...
			(*j) -> initializeRuntimeValuesFrom (mVV.setOfOpenValues (i));
	cCfg.removeSetOfOpenValues (lNE); // Just treated...

	QGAMES::Layer::MovementBehaviour::initializeRuntimeValuesFrom (cCfg);
}

// ---
bool QGAMES::Layer::SetOfMovementsBehaviour::hasCurrentMovementFinished () const
{
	return ((int) _movements.size () == 0 || _currentMovement >= (int) _movements.size () || 
			_movements [_currentMovement] -> hasFinished ()); 
}

// ---
bool QGAMES::Layer::SetOfMovementsBehaviour::hasFinished () const
{
	return (_movements.size () == 0 || _currentMovement >= (int) _movements.size ()); 
}

// ---
void QGAMES::Layer::SetOfMovementsBehaviour::initialize ()
{
	// Makes no sense to invoke the parent constructor
	// This class has its own rules!
	QGAMES::Layer::MovementBehaviour::initialize (); // ..but any case...

	for (int i = 0; i < (int) _movements.size (); _movements [i++] -> initialize ());

	_currentMovement = 0;
}

// ---
void QGAMES::Layer::SetOfMovementsBehaviour::finalize ()
{
	QGAMES::Layer::MovementBehaviour::finalize (); 

	for (int i = 0; i < (int) _movements.size (); _movements [i++] -> finalize ());
}

// ---
QGAMES::Position QGAMES::Layer::SetOfMovementsBehaviour::newPosition (const QGAMES::Position& p)
{
	if (_originalPosition == QGAMES::Position::_noPoint)
		_originalPosition = p;

	if (_movements.size () == 0 || _currentMovement >= (int) _movements.size ())
		return (p); // There is nothing to move at all!

	int lMov = _currentMovement;

	bool cMov = false;
	if (_movements [_currentMovement] -> hasFinished ())
	{
		cMov = true;
		_currentMovement++; // ...initially moves the pointer to the next movement...
		if (_ciclic && _currentMovement == _movements.size ())
			_currentMovement = 0; // ...but starts back because is ciclic, and the limit has been reaches...
	}

	// Initialize the new movement (if any), if it has been changed
	if (_currentMovement < (int) _movements.size () && cMov)
	{
		_movements [lMov] ->  finalize ();
		_movements [_currentMovement] -> initialize ();
	}

	// Then calculates the new position and the vector moved!
	QGAMES::Position nP = p;
	_vectorMoved = QGAMES::Vector::_cero;
	if (_currentMovement < (int) _movements.size ())
	{
		nP = _movements [_currentMovement] -> newPosition (p);
		_vectorMoved = _movements [_currentMovement] -> vectorMoved ();
		_averageVectorMoved = _movements [_currentMovement] -> averageVectorMoved ();
		// The vector moved has to be actualized as it is used by other methods...
		// and it has to be the same than the last movement made!
	}

	// Finally returned the new position...
	return (nP);
}

// ---
QGAMES::Layer::MovementBehaviour* QGAMES::Layer::LinearMovementBehaviour::clone () const
{
	return (new QGAMES::Layer::LinearMovementBehaviour (_speed, _steps, _direction));
}

// ---
QGAMES::Layer::MovementBehaviour* QGAMES::Layer::LinearMovementBehaviour::opposite ()
{
	return (new QGAMES::Layer::LinearMovementBehaviour (_speed, _steps, -_direction));
}

// ---
QGAMES::SetOfOpenValues QGAMES::Layer::LinearMovementBehaviour::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Layer::MovementBehaviour::runtimeValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue ((int) _currentStep));

	return (result);
}

// ---
void QGAMES::Layer::LinearMovementBehaviour::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_currentStep = cCfg.openValue (lE).intValue ();
	cCfg.removeOpenValue (lE); // Just treated...

	QGAMES::Layer::MovementBehaviour::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::Layer::LinearMovementBehaviour::setDirection (const QGAMES::Vector& d)
{
	_direction = d;

	initialize ();
}

// ---
bool QGAMES::Layer::LinearMovementBehaviour::hasFinished () const
{
	return (_currentStep >= _steps);
}

// ---
void QGAMES::Layer::LinearMovementBehaviour::initialize ()
{
	QGAMES::Layer::MovementBehaviour::initialize ();

	_currentStep = 0;
}

// ---
QGAMES::Position QGAMES::Layer::LinearMovementBehaviour::calculateNewPosition (const QGAMES::Position& p)
{
	_currentStep++;
	return (p + _direction);
}

// ---
QGAMES::Layer::MovementBehaviour* QGAMES::Layer::CircularMovementBehaviour::clone () const
{
	return (new QGAMES::Layer::CircularMovementBehaviour 
		(_speed, _radix, _initialAngle, _maxAngle, _angle, _clockwise));
}

// ---
QGAMES::Layer::MovementBehaviour* QGAMES::Layer::CircularMovementBehaviour::opposite ()
{
	return (new QGAMES::Layer::CircularMovementBehaviour 
		(_speed, _radix, _maxAngle, _initialAngle, _angle, !_clockwise));
}

// ---
QGAMES::SetOfOpenValues QGAMES::Layer::CircularMovementBehaviour::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Layer::MovementBehaviour::runtimeValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_currentAngle));

	return (result);
}

// ---
void QGAMES::Layer::CircularMovementBehaviour::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_currentAngle = cCfg.openValue (lE).bdataValue ();
	cCfg.removeOpenValue (lE); // Just treated...

	QGAMES::Layer::MovementBehaviour::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::Layer::CircularMovementBehaviour::setRadix (unsigned int r)
{
	_radix = r;
	_rotationCenter = Position (__BD 0, __BD r, __BD 0);
}

// ---
bool QGAMES::Layer::CircularMovementBehaviour::hasFinished () const
{
	return ((_clockwise && _currentAngle >= _maxAngle) ||
			(!_clockwise && _currentAngle <= _maxAngle));
}

// ---
void QGAMES::Layer::CircularMovementBehaviour::initialize ()
{
	QGAMES::Layer::MovementBehaviour::initialize ();
	_currentAngle = _initialAngle;
}

// ---
QGAMES::Position QGAMES::Layer::CircularMovementBehaviour::calculateNewPosition 
	(const QGAMES::Position& p)
{
	// Before rotating...
	QGAMES::Position rB = QGAMES::Position::_cero.rotate 
		(_rotationCenter, _rotationCenter + QGAMES::Vector::_zNormal, _currentAngle);
	// ...then
	_currentAngle += _angle * (_clockwise ? __BD 1 : __BD -1);
	if ((_clockwise && _currentAngle >= _maxAngle) ||
		(!_clockwise && _currentAngle <= _maxAngle))
		_currentAngle = _maxAngle;
	QGAMES::Position rA = QGAMES::Position::_cero.rotate 
		(_rotationCenter, _rotationCenter + QGAMES::Vector::_zNormal, _currentAngle);
	// So what it moved was...
	return (p + (rA - rB));
}

// ---
QGAMES::Layer::BounceMovementBehaviour::BounceMovementBehaviour (QGAMES::Layer::MovementBehaviour* mov)
	: QGAMES::Layer::SetOfMovementsBehaviour (QGAMES::Layer::MovementBehaviours (), true)
{
	assert (mov);

	_movements.push_back (mov);
	_movements.push_back (mov -> opposite ());
}

// ---
QGAMES::Layer::BounceNTimesMovementBehaviour::BounceNTimesMovementBehaviour 
		(int n, QGAMES::Layer::MovementBehaviour* mov)
	: QGAMES::Layer::SetOfMovementsBehaviour (QGAMES::Layer::MovementBehaviours (), false)
{
	assert (mov);
	assert (n > 0);

	_movements.push_back (mov);
	_movements.push_back (mov -> opposite ());
	for (int i = 0; i < (n - 1); i++)
	{
		QGAMES::Layer::MovementBehaviour* nMov = mov -> clone ();
		_movements.push_back (nMov);
		_movements.push_back (nMov -> opposite ());
	}
}

// ---
QGAMES::Layer::Layer (int c, const std::string& n, QGAMES::Map* m, 
	const QGAMES::LayerProperties& p)
			: Element (c),
			  _name (n),
			  _map (m),
			  _movementBehaviour (NULL),
			  _moves (true),
			  _visualEffect (NULL),
			  _properties (p),
			  _position (QGAMES::Position::_noPoint),
			  _orientation (QGAMES::Vector::_zNormal)
{
	// Always visible by default...
	setVisible (true);

	// Map reference is not mandatory here,
	// See TMX map construction...
}

// ---
QGAMES::Layer::~Layer ()
{ 
	delete (_movementBehaviour); // If any...
	_movementBehaviour = NULL;
	delete (_visualEffect); // If any...
	_visualEffect = NULL;
}

// ---
QGAMES::SetOfOpenValues QGAMES::Layer::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Element::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	// The position of the layer...
	std::stringstream sS; sS << _position; 
	result.addOpenValue (lE + 1, QGAMES::OpenValue (sS.str ()));
	// Whether it moves or not
	result.addOpenValue (lE + 2, QGAMES::OpenValue (_moves));

	// ...and the movement behaviour...
	result.addSetOfOpenValues (lNE + 1, 
		_movementBehaviour ? _movementBehaviour -> runtimeValues () 
			: QGAMES::SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESMOVTYPE__)));
	
	return (result);
}

// ---
void QGAMES::Layer::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1) && 
			cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_moves = cCfg.openValue (lE).boolValue ();
	cCfg.removeOpenValue (lE); 
	_position = QGAMES::Position (cCfg.openValue (lE - 1).strValue ());
	cCfg.removeOpenValue (lE - 1); 

	// Movement might not exist yet when runtime values were saved first time
	// if the movement already exists at this point, is still necessary to double check 
	// That the run time values wre also saved initially...
	QGAMES::SetOfOpenValues mVV = cCfg.setOfOpenValues (lNE);
	assert (mVV.name () == std::string (__QGAMES_RUNTIMEVALUESMOVTYPE__));
	if (_movementBehaviour && mVV.numberValues () != 0)
		_movementBehaviour -> initializeRuntimeValuesFrom (mVV);
	cCfg.removeSetOfOpenValues (lNE); // Just treat it...

	QGAMES::Element::initializeRuntimeValuesFrom (cCfg);
}

// ---
bool QGAMES::Layer::existsProperty (const std::string& p) const
{
	return (_properties.find (p) != _properties.end ());
}

// ---
std::string QGAMES::Layer::layerProperty (const std::string& p) const
{
	QGAMES::LayerProperties::const_iterator i = _properties.find (p);
	assert (i != _properties.end ());

	return ((*i).second);
}

// ---
void QGAMES::Layer::setMovement (QGAMES::Layer::MovementBehaviour* mov)
{
	// The movement can be null...

	// If other previous movement exists, then it is finalized and deleted first!
	if (_movementBehaviour != NULL)
	{
		_movementBehaviour -> finalize ();
		delete (_movementBehaviour);
	}

	_movementBehaviour = mov;
	if (_movementBehaviour != NULL)
		_movementBehaviour -> initialize ();
}

// ---
void QGAMES::Layer::setVisualEffect (QGAMES::Layer::VisualEffect* eff)
{
	// The visual effect can be null...

	// If other previous visual effect exists, then it is finalized and deleted first!
	if (_visualEffect != NULL)
	{
		_visualEffect -> finalize ();
		delete (_visualEffect);
	}

	_visualEffect = eff;
	if (_visualEffect != NULL)
		_visualEffect -> initialize ();
}

// ---
void QGAMES::Layer::initialize ()
{
	if (_movementBehaviour != NULL)
		_movementBehaviour -> initialize ();

	// The position can't be set here as it is used in many drawing activities...

	QGAMES::Element::initialize ();
}

// ---
void QGAMES::Layer::updatePositions ()
{
	if (_movementBehaviour != NULL && _moves) // If it exists and moves...
		setPosition (_movementBehaviour -> newPosition (position ()));

	if (_visualEffect != NULL)
		_visualEffect -> updatePositions ();

	if (_externalEngine)
		_externalEngine -> whenUpdatePositions (this);
}

// ---
void QGAMES::Layer::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	// Notice that the parent method is not invoked here ever
	// It will, in the drawOnWithElements method.
	drawOnWithElements (s, p, QGAMES::Elements ());
}

// ---
void QGAMES::Layer::drawOnWithElements (QGAMES::Screen* s, const QGAMES::Position& p, QGAMES::Elements& elmts)
{
	// Notice that the parent method drawOn is invoked here.
	// This method is invoked only from Layer::drawOn
	QGAMES::Element::drawOn (s, p);

	if (_visualEffect)
		_visualEffect -> drawOn (s, 
			(p == QGAMES::Position::_noPoint) ? QGAMES::Position::_cero : p);
}

// ---
QGAMES::Vector QGAMES::TileLayer::WaveTileUpdater::updateDirection (const Tile& tl) const
{
	QGAMES::Vector result = QGAMES::Vector::_noPoint;
	QGAMES::TileLayer::WaveTileUpdater::TileStateMap::const_iterator i;
	if ((i = _tileState.find (tl.id ())) != _tileState.end ())
		result = QGAMES::Vector (__BD 0, __BD ((*i).second._down ? 1 : -1), __BD 0);
	return (result);
}

// ---
QGAMES::bdata QGAMES::TileLayer::WaveTileUpdater::pixelsUpdated (const Tile& tl) const
{
	QGAMES::bdata result = 0;
	QGAMES::TileLayer::WaveTileUpdater::TileStateMap::const_iterator i;
	if ((i = _tileState.find (tl.id ())) != _tileState.end ())
		result = __BD (abs ((*i).second._speed - (*i).second._lastSpeed));
	return (result);
}

// ---
void QGAMES::TileLayer::WaveTileUpdater::updateTiles (const QGAMES::Tiles& tiles)
{
	if (_counter++ < _properties._loopsToUpdate)
		return;

	_counter = 0;
	_cycles++;
	bool finish = false;
	for (QGAMES::Tiles::const_iterator i = tiles.begin ();
			i != tiles.end () && !finish; i++)
	{
		QGAMES::Tile* tile = (*i);
		if (tile == QGAMES::NullTile::_nullTile)
			continue; // Not useful...

		if (std::find (_properties._tilesToWave.begin (), _properties._tilesToWave.end (), 
				tile -> numberFrame ()) != _properties._tilesToWave.end ())
		{
			int id = tile -> id ();
			int oF = tile -> frame () -> numberFrame (); // The initial aspect to rebuilt...
			if (_tileState.empty ())
			{
				finish = true; // First time there is only one to move...
				_tileState [id] = QGAMES::TileLayer::WaveTileUpdater::TileState (tile, oF, true, 0.0);
			}
			else
			{
				if (_tileState.find (id - 1) != _tileState.end () &&
					_tileState.find (id) == _tileState.end ())
				{
					finish = true;
					_tileState [id] = 
						QGAMES::TileLayer::WaveTileUpdater::TileState (tile, oF, _tileState [id - 1]._down, 0.0);
					// If it has no previous state, 
					// it will inherit the same state than the previous one...
					// But with no speed...
				}
			}

			// It is time to move, unless there is nothing to draw...
			if (_tileState.find (id) != _tileState.end ())
			{
				QGAMES::TileLayer::WaveTileUpdater::TileState tS = _tileState [id];

				int nF = tile -> numberFrame ();
				tS._lastSpeed = tS._speed; // Keeps the last...
				tS._speed += (tS._down ? 1.0 : -1.0) * _properties._acceleration;
				if (abs (tS._speed) > _properties._maximumSpeed) tS._down = !tS._down;
				nF = nF + (int) tS._speed;
				if (nF < _properties._tilesToWave [0]) nF = _properties._tilesToWave [0];
				if (nF > (_properties._tilesToWave [_properties._tilesToWave.size () - 1]))
					nF = _properties._tilesToWave [_properties._tilesToWave.size () - 1];
				tile -> setForm (tile -> form (), nF);

				_tileState [id] = tS;
			}
		}
	}
}

// ---
void QGAMES::TileLayer::WaveTileUpdater::finalize ()
{
	for (QGAMES::TileLayer::WaveTileUpdater::TileStateMap::const_iterator i = _tileState.begin ();
			i != _tileState.end (); i++)
	{
		QGAMES::TileLayer::WaveTileUpdater::TileState tS = (*i).second;
		tS._tileRef -> setForm (tS._tileRef -> form (), tS._originalFrame);
	}
}

// ---
void QGAMES::TileLayer::FlowTileUpdater::updateTiles (const QGAMES::Tiles& tiles)
{
	if (_counter++ < _properties._loopsToUpdate)
		return;

	_counter = 0;
	_cycles++;
	for (QGAMES::Tiles::const_iterator i = tiles.begin (); i != tiles.end (); i++)
	{
		QGAMES::Tile* tile = (*i);
		if (_firstUpdate)
			_tileState [tile -> id ()] = 
				QGAMES::TileLayer::FlowTileUpdater::TileState (tile, tile -> frame () -> numberFrame ());

		if (std::find (_properties._tilesToFlow.begin (), _properties._tilesToFlow.end (), 
				tile -> numberFrame ()) != _properties._tilesToFlow.end ())
		{
			int nF = tile -> numberFrame () + 1;
			if (nF < 0) nF = 0;
			if (nF >= (_properties._tilesToFlow [_properties._tilesToFlow.size () - 1]))
				nF = _properties._tilesToFlow [0];
		}
	}

	_firstUpdate = false;
}

// ---
void QGAMES::TileLayer::FlowTileUpdater::finalize ()
{
	for (QGAMES::TileLayer::FlowTileUpdater::TileStateMap::const_iterator i = _tileState.begin ();
			i != _tileState.end (); i++)
	{
		QGAMES::TileLayer::FlowTileUpdater::TileState tS = (*i).second;
		tS._tileRef -> setForm (tS._tileRef -> form (), tS._originalFrame);
	}
}

// ---
QGAMES::TileLayer::TileLayer (int c, const std::string& n, const QGAMES::Tiles& t, 
	QGAMES::Map* m, QGAMES::TileLayer::VisualOrientation o, const QGAMES::LayerProperties& p, bool oT)
		: QGAMES::Layer (c, n, m, p),
		  _tiles (t),
		  _visualOrientation (o),
		  _tilesUpdater (NULL),
		  _updateTiles (true),
		  _calculus (),
		  _tilesToDraw ()
{
#ifndef NDEBUG
	QGAMES::Forms lFrms;
#endif
	for (auto i : _tiles)
	{
		if (dynamic_cast <QGAMES::NullTile*> (i) == NULL)
		{
#ifndef NDEBUG
			// Take note of all forms used in the layer (usually only one, but just in case)
			if (lFrms.find (i -> form () -> id ()) == lFrms.end ())
				lFrms [i -> form () -> id ()] = i -> form ();
#endif

			// If the tiles have to be observed...by default they aren't
			// Take care it costs too much work later in terms of notifications...
			if (oT) 
				observe (i);
		}
	}

#ifndef NDEBUG
	// Now it is time to see what is the visual orientation defined for all those tiles
	// All off them should have the same one, and also to be aligned with the visual orientation
	// defined for the layer when created...
	std::vector <QGAMES::TileLayer::VisualOrientation> vOs;
	for (auto i : lFrms)
	{
		QGAMES::TileLayer::VisualOrientation vO = QGAMES::TileLayer::VisualOrientation::_EXTERANLLYDEFINED;
		if (dynamic_cast <QGAMES::Sprite2D*> (i.second) != NULL)
		{
			QGAMES::Sprite2DFormData* fD = 
				dynamic_cast <QGAMES::Sprite2DFormData*> (((QGAMES::Sprite2D*) i.second) -> formData ());
			switch (fD -> projectionType ())
			{
				case 0: vO = QGAMES::TileLayer::VisualOrientation::_ORTHOGONAL; break;
				case 1: vO = QGAMES::TileLayer::VisualOrientation::_ISOMETRIC; break;
				case 2: vO = QGAMES::TileLayer::VisualOrientation::_CAVALIER; break;
			}
		}
		else
		if (dynamic_cast <QGAMES::Form3D*> (i.second) != NULL)
			vO = QGAMES::TileLayer::VisualOrientation::_CONIC;

		if (std::find (vOs.begin (), vOs.end (), vO) == vOs.end ())
			vOs.push_back (vO);
	}

	assert ((int) vOs.size () == 0 || /** No forms. */
			((int) vOs.size () == 1 && vOs [0] == _visualOrientation) /** Or many with just a single orientation. */);
#endif
}

// ---
QGAMES::TileLayer::~TileLayer ()
{
	for (QGAMES::Tiles::iterator i = _tiles.begin (); 
			i != _tiles.end (); i++)
	{
		if ((*i) != QGAMES::NullTile::_nullTile)
		{
			unObserve ((*i)); // Stop to observe them if any...
			delete ((*i));
		}
	}

	_tiles.clear ();

	delete (_tilesUpdater);
	_tilesUpdater = NULL;
}

// ---
void QGAMES::TileLayer::setMap (QGAMES::Map* m)
{
	// To ensue the owner map is a tiled map...
	// Otherwise most of the methods are not possible...
	// Just the map is received...
	assert (dynamic_cast <QGAMES::TiledMap*> (m) != NULL);
	QGAMES::Layer::setMap (m);
}

// ---
void QGAMES::TileLayer::setFade (int fd) 
{
	for (QGAMES::Tiles::iterator i = _tiles.begin (); 
			i != _tiles.end (); i++)
		if ((*i) != QGAMES::NullTile::_nullTile) (*i) -> setFade (fd);
}

// ---
QGAMES::Tile* QGAMES::TileLayer::tileAt (const QGAMES::Position& p)
{
	assert (_map);
	assert (_tiles.size ()); // size can't be 0...

	int tWidth = (__TM _map) -> visualTileWidth ();
	int tHeight = (__TM _map) -> visualTileHeight ();
	int tDepth = (__TM _map) -> visualTileDepth ();
	assert (tWidth != 0 && tHeight != 0); // The sizes can't be 0...
										  // but depth can (meaning plane = 2D pure)

	// Calculating the number of tiles per axis of the layer...
	int mWidth = _map -> width () / tWidth;
	int mHeight = _map -> height () / tHeight; 
	int mDepth = (tDepth == 0) ? 1 : _map -> depth () / tDepth;
	// Now to calculate the number of the tile per axis where the position is in...
	int posX = (int) ((p.posX () - _position.posX ()) / tWidth); 
	int posY = (int) ((p.posY () - _position.posY ()) / tHeight);
	int posZ = (tDepth == 0) ? 0 : (int) ((p.posZ () - _position.posZ ()) / tDepth);
	// Then to calculate the number of the tile in the list...
	int nE = (posZ * mWidth * mHeight) + (posY * mWidth) + posX; 
	// If that tile doesn't exist a null tile is returned, 
	// otherwise a reference to the right tile is passed back...
	return ((nE >= (int) _tiles.size () || nE < 0 || (posX < 0 || posY < 0 || posZ < 0)) 
		? QGAMES::NullTile::_nullTile : _tiles [nE]);
}

// ---
QGAMES::Tiles QGAMES::TileLayer::tilesAt (const QGAMES::Position& p, int n)
{
	assert (_map);
	assert (n > 0);
	assert (_tiles.size ());

	int tWidth = (__TM _map) -> visualTileWidth ();
	int tHeight = (__TM _map) -> visualTileHeight ();
	int tDepth = (__TM _map) -> visualTileDepth ();
	assert (tWidth != 0 && tHeight != 0); 

	int mWidth = _map -> width () / tWidth;
	int mHeight = _map -> height () / tHeight; 
	int mDepth = (tDepth == 0) ? 1 : _map -> depth () / tDepth;
	int posX = (int) ((p.posX () - _position.posX ()) / tWidth); 
	int posY = (int) ((p.posY () - _position.posY ()) / tHeight);
	int posZ = (tDepth == 0) ? 0 : (int) ((p.posZ () - _position.posZ ()) / tDepth);
	int nE = (posZ * mWidth * mHeight) + (posY * mWidth) + posX; 

	QGAMES::Tiles result;
	for (int i = 0; i < n; i++)
	{
		result.push_back 
			((nE >= (int) _tiles.size () || nE < 0 || (posX < 0 || posY < 0 || posZ < 0)) 
				? QGAMES::NullTile::_nullTile : _tiles [nE]);
		nE++; posX++;
	}

	return (result);
}

// ---
QGAMES::Tiles QGAMES::TileLayer::tilesAt (const std::vector <QGAMES::Position>& p)
{
	QGAMES::Tiles result;
	for (std::vector <QGAMES::Position>::const_iterator i = p.begin (); i != p.end (); 
		result.push_back (tileAt ((*i++))));
	return (result);
}

// ---
QGAMES::Position QGAMES::TileLayer::tilePosition (const QGAMES::Tile* t) const
{
	assert (t);

	int tWidth = (__TM _map) -> visualTileWidth ();
	int tHeight = (__TM _map) -> visualTileHeight ();
	int tDepth = (__TM _map) -> visualTileDepth ();
	assert (tWidth != 0 && tHeight != 0); // The sizes can't be 0...
										  // but the depth can be 0 (meaning plain)

	// Calculating the number of tiles per axis of the layer...
	int mWidth = _map -> width () / tWidth;
	int mHeight = _map -> height () / tHeight; 
	// When tere is no depth for the tile,
	// The number of plane of tiles where to look for the right one is just one
	int mDepth = (tDepth == 0) ? 1 : _map -> depth () / tDepth; 

	// Look for the right tile (comparing types) in a for loop...(sequentially)
	int pTile = -1;
	for (int i = 0; i < (int) _tiles.size () && pTile == -1; i++)
		if (_tiles [i] == t) pTile = i; // Found it!!
	// If the tile hasn't been foun then no point is returned, otherwise the position is calculated...
	return ((pTile != -1) 
		? QGAMES::Position (
			__BD ((((pTile % (mWidth * mHeight)) % mWidth) * tWidth) + _position.posX ()), 
			__BD ((((pTile % (mWidth * mHeight)) / mWidth) * tHeight) + _position.posY ()), 
			__BD ((mWidth * mHeight) * tDepth)) // Just taking into account the Z axis...
		: QGAMES::Position::_noPoint);
}

// ---
QGAMES::Tiles QGAMES::TileLayer::tilesAround (QGAMES::Entity* e, QGAMES::bdata m, const Vector& pv)
{
	assert (e);

	// If nothing has changed, then the tiles returned will be the same
	QGAMES::TileLayer::TilesAroundCalculus cal (this, e, m, pv);
	if (_calculus.find (e -> id ()) != _calculus.end () && 
			_calculus [e -> id ()] == cal)
		return (_calculus [e -> id ()].result ());

	// Otherwise the calculus has to be done...
	// What are the tiles occupied by the entity...
	int ePosX, ePosY, ePosZ, ntW, ntH, ntD;
	calculateTilesPositionesFor (e, ePosX, ePosY, ePosZ, ntW, ntH, ntD, m, pv);

	// Read the tiles...
	// ...and actualize the calculus for the next time!
	QGAMES::Tiles result = tilesIn (ePosX, ePosY, ePosZ, ntW, ntH, ntD); 
	cal.actualize (this, e -> id (), ePosX, ePosY, ePosZ, ntW, ntH, ntD, m, pv, result);
	_calculus [e -> id ()] = cal;

	// ...and returns it
	return (result);
}

// ---
QGAMES::Tiles QGAMES::TileLayer::tilesIn (int pX, int pY, int pZ, int w, int h, int d)
{
	assert (w >= 0 && h >= 0); // The depth could be 0 in a plain map ie.

	QGAMES::Tiles result;

	// Basic data related with the layer...
	int tWidth = (__TM _map) -> visualTileWidth (); // tile width in pixels
	int tHeight = (__TM _map) -> visualTileHeight (); // tile height in pixels
	int tDepth = (__TM _map) -> visualTileDepth ();
	assert (tWidth != 0 && tHeight != 0); // The sizes can't be 0...but z dimension can be 0
	int mWidth = _map -> width () / tWidth; // width map in terms of tiles
	int mHeight = _map -> height () / tHeight; // height map in terms of tiles 
	int mDepth = (tDepth == 0) ? 1 : _map -> depth () / tDepth;

	// Read the tiles...
	int nE = (pZ * mWidth * mHeight) + (pY * mWidth) + pX;
	result.resize (h * w * ((d == 0) ? 1 : d));
	for (int k = 0; k < ((d == 0) ? 1 : 0); k++)
	{
		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				int iPos = nE + (k * mWidth * mHeight) + (i * mWidth) + j;
				result [(k * w * h) + (i * w) + j] = 
					(iPos < 0 || iPos >= (int) _tiles.size ()) 
						? QGAMES::NullTile::_nullTile :_tiles [iPos]; 
			}
		}
	}

	return (result);
}

// ---
bool QGAMES::TileLayer::emptyPositionAt (const QGAMES::Position& p) 
{
	return (tileAt (p) -> isEmpty ());
}

// ---
bool QGAMES::TileLayer::emptyBoxAt (const QGAMES::Position& p, const QGAMES::Position& s) 
{
	bdata posAX = p.posX ();
	bdata posAY = p.posY ();
	bdata posAZ = p.posZ ();
	bdata posBX = s.posX ();
	bdata posBY = s.posY ();
	bdata posBZ = s.posZ ();
	if (posBZ < posAZ || ((posBZ >= posAZ) && (posBX < posAX || (posBX >= posAX && posBY < posAY))))
		return (false);

	int tWidth = _tiles [0] -> form () -> frameWidth ();
	int tHeight = _tiles [0] -> form () -> frameHeight ();
	int tDepth = _tiles [0] -> form () -> frameDepth ();
	bool result = true;
	for (bdata k = posAZ; k <= posBZ && result == true; k += tDepth)
		for (bdata i = posAY; i <= posBY && result == true; i += tHeight)
			for (bdata j = posAX; j <= posBX && result == true; j += tWidth)
				result = emptyPositionAt (QGAMES::Position (j, i, k));
	return (result);
}

// ---
QGAMES::Position QGAMES::TileLayer::positionFirstTile (int t, int f)
{
	QGAMES::Position result = QGAMES::Position::_noPoint;

	int tWidth = (__TM _map) -> visualTileWidth ();
	int tHeight = (__TM _map) -> visualTileHeight ();
	int tDepth = (__TM _map) -> visualTileDepth ();
	assert (tWidth != 0 && tHeight != 0); // The sizes can't be 0...but z sinze can be 0 (meaning plain layer)

	int mWidth = _map -> width () / tWidth;
	int mHeight = _map -> height () / tHeight; 
	int mDepth = (tDepth == 0) ? 1 : _map -> depth () / tDepth;

	bool found = false;
	int xT = 0; int yT = 0; int zT = 0;
	for (QGAMES::Tiles::const_iterator i = _tiles.begin (); 
		i != _tiles.end () && !found; i++)
	{
		if (((*i) -> type () == t && f == -1) ||
			((*i) -> type () == t && (f != -1 && (*i) -> numberFrame () == f)))
			found = true;
		else
		{
			if (++xT == mWidth)
			{
				if (++yT == mHeight)
				{
					++zT;
					yT = 0;
				}
				
				xT = 0;
			}
		}
	}

	if (found)
		result = QGAMES::Position (__BD (xT * tWidth), __BD (yT * tHeight), __BD (zT * tDepth));
	return (result);
}

// ---
void QGAMES::TileLayer::setTileUpdater (QGAMES::TileLayer::TileUpdater* tU)
{
	if (_tilesUpdater != NULL)
	{
		_tilesUpdater -> finalize ();
		delete (_tilesUpdater);
	}

	_tilesUpdater = tU;
	if (_tilesUpdater != NULL)
		_tilesUpdater -> initialize ();
}

// ---
void QGAMES::TileLayer::updatePositions ()
{
	QGAMES::Layer::updatePositions ();

	if (_tilesUpdater != NULL && _updateTiles)
		_tilesUpdater -> updateTiles (_tiles);
}

// ---
void QGAMES::TileLayer::drawOnWithElements (QGAMES::Screen* s, const QGAMES::Position& p, QGAMES::Elements& elmts)
{
	assert (_map);
	assert (s);

	if (_tilesToDraw.empty ())
		QGAMES::TileLayer::calculateTilesToDraw (); // Before the first time the layer is drawn...

	switch (_visualOrientation)
	{
		case QGAMES::TileLayer::VisualOrientation::_ISOMETRIC:
			drawOnIsometric (s, p, elmts);
			break;

		case QGAMES::TileLayer::VisualOrientation::_CAVALIER:
			drawOnCavalier (s, p, elmts);
			break;

		case QGAMES::TileLayer::VisualOrientation::_ORTHOGONAL:
			drawOnOrthogonal (s, p, elmts);
			break;

		case QGAMES::TileLayer::VisualOrientation::_CONIC:
		default: // Also when defined externally...
			drawOnConic (s, p, elmts);
	}

	QGAMES::Layer::drawOnWithElements (s, p, elmts);
}

// ---
void QGAMES::TileLayer::calculateTilesPositionesFor (QGAMES::Entity* e,
			int& pX, int& pY, int& pZ, int& w, int& h, int& d,
			QGAMES::bdata m, const QGAMES::Vector& pv)
{
	assert (dynamic_cast <QGAMES::ArtistInATiledMap*> (e));

	// The entity received has to be an artist moving over the tiles
	// The position to read should be the same than what are calculated for the entity!
	// This is the way to make the calculations coherents!
	((QGAMES::ArtistInATiledMap*) e) -> tilesOcuppied (pX, pY, pZ, w, h, d, _position, m, pv);
}

// ---
void QGAMES::TileLayer::calculateTilesToDraw ()
{
	int tWidth = (__TM _map) -> visualTileWidth ();
	int tHeight = (__TM _map) -> visualTileHeight ();
	int tDepth = (__TM _map) -> visualTileDepth ();
	assert (tWidth != 0 && tHeight != 0); // The sizes can't be 0...but z sizes can!

	int mWidth = _map -> width () / tWidth;
	int mHeight = _map -> height () / tHeight; 
	int mDepth = (tDepth == 0) ? 1 : _map -> depth () / tDepth;
	int xP = 0; int yP = 0; int zP = 0;

	_tilesToDraw = QGAMES::TileLayer::TilesToDraw ();
	for (int i = 0; i < (int) _tiles.size (); i++)
	{
		if (_tiles [i] != QGAMES::NullTile::_nullTile)
			_tilesToDraw.push_back (QGAMES::TileLayer::TileToDraw (_tiles [i], xP, yP, zP));
		if (++xP == mWidth)
		{
			if (++yP == mHeight)
			{
				++zP;
				yP = 0;
			}

			xP = 0;
		}
	}
}

// ---
void QGAMES::TileLayer::drawOnOrthogonal (QGAMES::Screen* s, const QGAMES::Position& p, QGAMES::Elements& elmts)
{
	// Projection has to be orthogonal...
	// Otherwise something won't match actually!
	assert (s -> currentCamera () -> projectionType () == QGAMES::Camera::ProjectionType::_ORTHOGONAL);

	int tWidth  = (__TM _map) -> tileWidth ();
	int tHeight = (__TM _map) -> tileHeight ();
	assert (tWidth != 0 && tHeight != 0); // The sizes can't be 0...

	// Draw the tiles...
	if (!_tilesToDraw.empty ())
	{
		QGAMES::Position coordM = (p == QGAMES::Position::_noPoint) ? _position : p;
		for (int i = 0; i < (int) _tilesToDraw.size (); i++)
		{
			const QGAMES::TileLayer::TileToDraw& tD = _tilesToDraw [i];

			QGAMES::Position rPos = 
				_tilesToDraw [0]._tile -> form () -> roundingCentralZone (_tilesToDraw [0]._tile -> numberFrame ()).pos1 ();
			tD._tile -> drawOn (s, QGAMES::Position (__BD (tD._x * tWidth), __BD (tD._y * tHeight), __BD 0) - rPos + coordM);
			// In an orthogonal map, rPos will take the values of the offset mainly!
		}
	}

	// Draw elements...
	for (QGAMES::Elements::const_iterator i = elmts.begin (); i != elmts.end (); i++)
		if ((*i) -> isVisible ()) (*i) -> drawOn (s, p);
	elmts = QGAMES::Elements (); // All of them have been used...
}

// ---
void QGAMES::TileLayer::drawOnIsometric (QGAMES::Screen* s, const QGAMES::Position& p, QGAMES::Elements& elmts)
{
	// Just in case...
	// Otherwise something won't match later sure!
	assert (s -> currentCamera () -> projectionType () == QGAMES::Camera::ProjectionType::_QUICKDIEDRIC);

	// In isometric the visual width of the tiles in the X axis is half of the width defined in the map per tile...
	int tWidth2 = (__TM _map) -> tileWidth ();
	int tWidth  = tWidth2 >> 1;
	// ...but in the Y axis is what is has been defined for the tiles in the map
	// That, by the way, is usually (in ISOMETRIC) HALF of the total width of the tile
	int tHeight  = (__TM _map) -> tileHeight ();
	int tHeightH = tHeight >> 1;
	assert (tWidth != 0 && tHeight != 0); // The sizes can't be 0...
	assert (tWidth == tHeight); // The sizes have to be the same

	// Draw the tiles...
	if (!_tilesToDraw.empty ())
	{
		QGAMES::Position coordM = (p == QGAMES::Position::_noPoint) ? _position : p;
		
		// In a tiled map, let's suposse initially all tiles to be drawn have the same size and are also plane (Vd == 0)
		// If it were the case, the left up corner of the base of the tile located in the left up corner of the grid
		// would be located at the position 0,0...
		// if the Vd weren't 0, then the tile should get up Vd over that initial position. Get it?

		// Then, to locate a tile in the right position of the grid
		// it is necessary to locate firt the left up corner of the box 
		// envolving the tile at the reference position of the square in the grid,
		// and then moving up Vd (visualHeight ()) that position.

		// This all is right when the tile to draw matches (in width) with the size of the grid (in width too)
		// But, when it doesn't, other different approach has to be applied.
		// The tile to draw could be considered as "decoration"
		// The reference point of the grid to considered should be the left down one insted the left top one.
		// and the position in the axis Z should be used!

		for (int i = 0; i < (int) _tilesToDraw.size (); i++)
		{
			const QGAMES::TileLayer::TileToDraw& tD = _tilesToDraw [i];

			if (tD._tile -> form () -> frameWidthForFrame (tD._tile -> numberFrame ()) == tWidth2)
			{
				QGAMES::Position rPos = 
					tD._tile -> form () -> coverZone (tD._tile -> numberFrame ()).pos1 ();
				int oZ = tD._tile -> form () -> visualHeight (tD._tile -> numberFrame ());
				tD._tile -> drawOn 
					(s, QGAMES::Position (__BD (tD._x * tWidth), __BD (tD._y * tHeight), __BD -oZ) - rPos + coordM);
			}
			else
			{
				int oZ = tD._tile -> form () -> frameHeightForFrame (tD._tile -> numberFrame ()) - tHeightH;
				tD._tile -> drawOn 
					(s, QGAMES::Position (__BD (tD._x * tWidth), __BD ((tD._y + 1) * tHeight), __BD -oZ) + coordM);
				// In TMX maps (designed with tiledmap, the reference point in this case is the left down corner
				// that is the same that the left up corner of the next tile in the Y axis!
				// This is the reason to add one to the y coordinate of the tile to draw
			}
		}
	}

	// Draw elements...
	for (QGAMES::Elements::const_iterator i = elmts.begin (); i != elmts.end (); i++)
		if ((*i) -> isVisible ()) (*i) -> drawOn (s, p);
	elmts = QGAMES::Elements (); // All of them have been used...
}

// ---
void QGAMES::TileLayer::drawOnCavalier (QGAMES::Screen* s, const QGAMES::Position& p, QGAMES::Elements& elmts)
{
	// Just in case...
	// Otherwise something won't match later sure!
	assert (s -> currentCamera () -> projectionType () == QGAMES::Camera::ProjectionType::_QUICKCAVALIER);

	int tWidth2 = (__TM _map) -> tileWidth ();
	int tWidth  = tWidth2 >> 1;
	int tHeight2  = (__TM _map) -> tileHeight ();
	int tHeight = tHeight2 >> 1;
	assert (tWidth != 0 && tHeight != 0); // The sizes can't be 0...

	// For further explanations see isometric definition. It is quite similar!

	// Draw the tiles...
	if (!_tilesToDraw.empty ())
	{
		QGAMES::Position coordM = (p == QGAMES::Position::_noPoint) ? _position : p;

		for (int i = 0; i < (int) _tilesToDraw.size (); i++)
		{
			const QGAMES::TileLayer::TileToDraw& tD = _tilesToDraw [i];

			if (tD._tile -> form () -> frameWidthForFrame (tD._tile -> numberFrame ()) == tWidth2)
			{
				QGAMES::Position rPos = 
					tD._tile -> form () -> coverZone (tD._tile -> numberFrame ()).pos1 ();
				int oZ = tD._tile -> form () -> visualHeight (tD._tile -> numberFrame ());
				tD._tile -> drawOn 
					(s, QGAMES::Position (__BD (tD._x * tWidth), __BD (tD._y * tHeight), __BD -oZ) - rPos + coordM);
			}
			else
			{
				int oZ = tD._tile -> form () -> frameHeightForFrame (tD._tile -> numberFrame ()) - tHeight;
				tD._tile -> drawOn 
					(s, QGAMES::Position (__BD (tD._x * tWidth), __BD ((tD._y + 1) * tHeight), __BD -oZ) + coordM);
			}
		}
	}

	// Draw elements...
	for (QGAMES::Elements::const_iterator i = elmts.begin (); i != elmts.end (); i++)
		if ((*i) -> isVisible ()) (*i) -> drawOn (s, p);
	elmts = QGAMES::Elements (); // All of them have been used...
}

// ---
void QGAMES::TileLayer::drawOnConic (QGAMES::Screen* s, const QGAMES::Position& p, QGAMES::Elements& elmts)
{
	// Here it is not neccessary to test the type of camera, 
	// The one defined for the screen should be applied
	// What is clear is that when reaching this point (in debug) modes it will have been tested already
	// that the tile projection matches this way of visualization...

	int tWidth  = (__TM _map) -> tileWidth ();
	int tHeight = (__TM _map) -> tileHeight ();
	int tDepth  = (__TM _map) -> tileDepth ();
	assert (tWidth != 0 && tHeight != 0 && tDepth != 0); // The sizes can't be 0...

	// It this case, to draw the tiles is a simple method...
	for (auto i : _tilesToDraw)
		i._tile -> drawOn (s, QGAMES::Position (__BD (i._x * tWidth), __BD (i._y * tHeight), __BD (i._z * tDepth)));

	// Draw elements...
	for (auto i : elmts)
		if ((i) -> isVisible ()) (i) -> drawOn (s, p);
	elmts = QGAMES::Elements (); // All of them have been used...
}

// ---
QGAMES::TileLayer::TilesAroundCalculus::TilesAroundCalculus (QGAMES::TileLayer* l, QGAMES::Entity* e,
		QGAMES::bdata m, const QGAMES::Vector& pv)
	: _position (QGAMES::Position::_noPoint),
	  _id (-1),
	  _pX (0), _pY (0), _pZ (0), _w (0), _h (0), _d (0),
	  _m (m), _pV (pv),
	  _tiles () // The very result of the calculation...
{
	assert (l);
	assert (e);

	_position = l -> position ();
	_id = e -> id ();

	l -> calculateTilesPositionesFor (e, _pX, _pY, _pZ, _w, _h, _d, m, pv);
}

// ---
void QGAMES::TileLayer::TilesAroundCalculus::actualize (QGAMES::TileLayer* l,
				int id, int pX, int pY, int pZ, int w, int h, int d, QGAMES::bdata m, const QGAMES::Vector& pv,
				const QGAMES::Tiles& t)
{
	assert (l);

	_position = l -> position ();

	_id = id; 
	_pX = pX; _pY = pY; _pZ = pZ; 
	_w = w; _h = h; _d = d;
	_m = m; _pV = pv;

	_tiles = t;
}

// ---
QGAMES::ObjectLayer::~ObjectLayer ()
{
	for (QGAMES::Objects::iterator i = _objects.begin (); 
			i != _objects.end (); i++)
		delete ((*i));
	_objects.clear ();
}

// ---
void QGAMES::ObjectLayer::setFade (int fd)
{
	for (QGAMES::Objects::iterator i = _objects.begin (); 
			i != _objects.end (); i++)
		(*i) -> setFade (fd);
}

// ---
void QGAMES::ObjectLayer::initialize ()
{
	for (QGAMES::Objects::iterator i = _objects.begin (); 
			i != _objects.end (); i++)
		(*i) -> initialize ();
	QGAMES::Layer::initialize ();
}

// ---
void QGAMES::ObjectLayer::inEveryLoop ()
{
	for (QGAMES::Objects::iterator i = _objects.begin (); 
			i != _objects.end (); i++)
		(*i) -> inEveryLoop ();
	QGAMES::Layer::inEveryLoop ();
}

// ---
void QGAMES::ObjectLayer::drawOnWithElements (QGAMES::Screen* s, const QGAMES::Position& p,
	QGAMES::Elements& elmts)
{
	assert (s);

	for (QGAMES::Objects::iterator i = _objects.begin (); 
			i != _objects.end (); i++)
		if ((*i) -> isVisible ())
			(*i) -> drawOn (s, (p == QGAMES::Position::_noPoint) 
				? _position : ((_position == QGAMES::Position::_noPoint) ?  p : p + _position));

	// Draw elements
	// In an orthogonal expresion, the elements are always drawn on top...
	for (QGAMES::Elements::const_iterator i = elmts.begin (); i != elmts.end (); i++)
		if ((*i) -> isVisible ()) (*i) -> drawOn (s, p);
	elmts = QGAMES::Elements (); // All of them has been used...

	QGAMES::Layer::drawOnWithElements (s, p, elmts);
}

// ---
void QGAMES::ObjectLayer::finalize ()
{
	for (QGAMES::Objects::iterator i = _objects.begin (); 
			i != _objects.end (); i++)
		(*i) -> finalize ();
	QGAMES::Layer::finalize ();
}

// ---
QGAMES::ImageLayer::~ImageLayer ()
{
	delete (_image);
	_image = NULL;
}

// ---
void QGAMES::ImageLayer::setFade (int fd)
{
	_image -> setFade (fd);
}

// ---
void QGAMES::ImageLayer::drawOnWithElements (QGAMES::Screen* s, const QGAMES::Position& p,
	QGAMES::Elements& elmts)
{
	assert (_image);

	_image -> drawOn (s);

	// Draw elements
	// In an orthogonal expresion, the elements are always drawn on top...
	for (QGAMES::Elements::const_iterator i = elmts.begin (); i != elmts.end (); i++)
		if ((*i) -> isVisible ()) (*i) -> drawOn (s, p);
	elmts = QGAMES::Elements (); // All of them has been used...

	QGAMES::Layer::drawOnWithElements (s, p, elmts);
}
