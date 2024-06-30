#include <Common/movement.hpp>
#include <Common/entity.hpp>
#include <Common/game.hpp>
#include <stdlib.h>
#include <sstream>

// ---
double QGAMES::Movement::variable (int id) const
{
	std::map <int, double>::const_iterator i = _variables.find (id);
	return ((i == _variables.end ()) ? 0.0 : (*i).second);
}

// ---
void QGAMES::Movement::setVariable (int id, double v)
{
	std::map <int, double>::iterator i = _variables.find (id);
	if (i != _variables.end ())
		_variables.erase (i);
	_variables.insert (std::map <int, double>::value_type (id, v));
}

// ---
QGAMES::Movement* QGAMES::NoMovement::clone () const
{
	QGAMES::Movement* result = new QGAMES::NoMovement (_id, _variables); 
	result -> initialize (); 
	return (result);
}

// ---
void QGAMES::NoMovement::move (const QGAMES::Vector& d, const QGAMES::Vector& a, QGAMES::Entity* e)
{
	assert (e);

	e -> setOrientation (d); // In the direction of the movement...
}

// ---
QGAMES::Movement* QGAMES::SimpleLinearMovement::clone () const
{
	QGAMES::Movement* result = new QGAMES::SimpleLinearMovement (_id, _variables); 
	result -> initialize ();
	((QGAMES::SimpleLinearMovement*) result) -> _speed = _speed;
	((QGAMES::SimpleLinearMovement*) result) -> _defaultSpeed = _defaultSpeed;
	((QGAMES::SimpleLinearMovement*) result) -> _lastDirection = _lastDirection;

	return (result);
}

// ---
QGAMES::SetOfOpenValues QGAMES::SimpleLinearMovement::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Movement::runtimeValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_speed));
	std::stringstream sS; sS << _lastDirection;
	result.addOpenValue (lE + 2, QGAMES::OpenValue (sS.str ()));

	return (result);
}

// ---
void QGAMES::SimpleLinearMovement::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_lastDirection = QGAMES::Vector (cCfg.openValue (lE).strValue ());
	cCfg.removeOpenValue (lE);
	_speed = cCfg.openValue (lE - 1).bdataValue ();
	cCfg.removeOpenValue (lE - 1);

	QGAMES::Movement::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::SimpleLinearMovement::initialize ()
{
	QGAMES::Movement::initialize ();
	_defaultSpeed = _speed = (QGAMES::bdata) variable (0);
	_lastDirection = QGAMES::Vector::_cero;
}

// ---
void QGAMES::SimpleLinearMovement::move (const QGAMES::Vector& d, const QGAMES::Vector& a, 
	QGAMES::Entity* e)
{
	assert (e);

	_lastDirection = d; // The direftion is the same even if it is ot possible to move...

	if (e -> canMove (d * _speed, a))
		e -> setPosition (e -> position () + _speed * _lastDirection);
	else
		notify (QGAMES::Event (__QGAMES_MOVEMENTNOTPOSSIBLE__, this));

	e -> setOrientation (direction ());
}

// ---
QGAMES::SimpleXYParabolicMovement::SimpleXYParabolicMovement (int id, std::map <int, double> v)
	: QGAMES::Movement (id, v),
	  _length (__BD 0), _height (__BD 0), _timeToHeight (__BD 0), _speed (__BD 1),
	  _firstMovement (true), 
	  _x (0), _y (0), _originalPosition (QGAMES::Position::_cero),
	  _kx (0), _ky (0), _ka (0), _kn (0),
	  _lastDirection (QGAMES::Vector::_cero)
{
}

// ---
void QGAMES::SimpleXYParabolicMovement::setJumpParameters (QGAMES::bdata l, QGAMES::bdata h, QGAMES::bdata tH, QGAMES::bdata s)
{
	_length = l;
	_height = h;
	_timeToHeight = tH;
	_speed = s;
	assert (_timeToHeight != __BD 0); // Can't be 0...

	int fps = QGAMES::Game::game () -> framesPerSecond ();
	_kx = _speed * (_length / (2 * _timeToHeight * fps));
	_ky = _speed * ((2 * _height) / (_timeToHeight * fps));
	_ka = _speed * (_height / (_timeToHeight * _timeToHeight * fps * fps));
	_kn = 0; // Incremental...

	// These parameters can be set on the fly, son the positions are not set...
}

// ---
void QGAMES::SimpleXYParabolicMovement::setGravity (QGAMES::bdata g)
{
	assert (g != __BD 0); // It can't be 0...

	setJumpParameters (length (), height (), __BD (sqrt (2 * height () / g)), speed ());
}

// ---
QGAMES::Movement* QGAMES::SimpleXYParabolicMovement::clone () const
{
	QGAMES::Movement* result = new QGAMES::SimpleXYParabolicMovement (_id, _variables); 
	result -> initialize (); 
	return (result);
}

// ---
QGAMES::SetOfOpenValues QGAMES::SimpleXYParabolicMovement::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Movement::runtimeValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_length));
	result.addOpenValue (lE + 2, QGAMES::OpenValue (_height));
	result.addOpenValue (lE + 3, QGAMES::OpenValue (_timeToHeight));
	result.addOpenValue (lE + 4, QGAMES::OpenValue (_speed));

	result.addOpenValue (lE + 5, QGAMES::OpenValue (_firstMovement));
	result.addOpenValue (lE + 6, QGAMES::OpenValue (_x));
	result.addOpenValue (lE + 7, QGAMES::OpenValue (_y));
	std::stringstream sS; sS << _originalPosition;
	result.addOpenValue (lE + 8, QGAMES::OpenValue (sS.str ()));
	sS.str (std::string (__NULL_STRING__)); sS << _lastDirection;
	result.addOpenValue (lE + 9, QGAMES::OpenValue (sS.str ()));

	return (result);
}

// ---
void QGAMES::SimpleXYParabolicMovement::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1) &&
			cfg.existOpenValue (lE - 2) && cfg.existOpenValue (lE - 3) &&
			cfg.existOpenValue (lE - 4) && cfg.existOpenValue (lE - 5) &&
			cfg.existOpenValue (lE - 6) && cfg.existOpenValue (lE - 7) &&
			cfg.existOpenValue (lE - 8));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_lastDirection = cCfg.openValue (lE).strValue ();
	cCfg.removeOpenValue (lE);
	_originalPosition = cCfg.openValue (lE - 1).strValue ();
	cCfg.removeOpenValue (lE - 1);
	_y = cCfg.openValue (lE - 2).bdataValue ();
	cCfg.removeOpenValue (lE - 2);
	_x = cCfg.openValue (lE - 3).bdataValue ();
	cCfg.removeOpenValue (lE - 3);
	_firstMovement = cCfg.openValue (lE - 4).boolValue ();
	cCfg.removeOpenValue (lE - 4);

	_speed = cCfg.openValue (lE - 5).bdataValue ();
	cCfg.removeOpenValue (lE - 5);
	_timeToHeight = cCfg.openValue (lE - 6).bdataValue ();
	cCfg.removeOpenValue (lE - 6);
	_height = cCfg.openValue (lE - 7).bdataValue ();
	cCfg.removeOpenValue (lE - 7);
	_length = cCfg.openValue (lE - 8).bdataValue ();
	cCfg.removeOpenValue (lE - 8);

	setJumpParameters (_length, _height, _timeToHeight, _speed);
	// JUst to calculate the internal constants...

	QGAMES::Movement::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::SimpleXYParabolicMovement::initialize ()
{
	QGAMES::Movement::initialize ();

	setJumpParameters (__BD variable (0), __BD variable (1), __BD variable (2), __BD variable (3));
	_firstMovement = true;
	_x = _y = __BD 0;
	_lastDirection = QGAMES::Vector::_cero;
}

// ---
void QGAMES::SimpleXYParabolicMovement::move (const QGAMES::Vector& d, const QGAMES::Vector& a, 
	QGAMES::Entity* e)
{
	assert (e);

	// d is neveer taken into account...
	// Because jumping parameters are set using other variables...
	if (_firstMovement)
	{  
		_originalPosition = e -> position ();
		_firstMovement = false;
		setJumpParameters (length (), height (), timeToHeight (), speed ());
		_x = _originalPosition.posX ();
		_y = _originalPosition.posY ();
		if (d.posX () < __BD 0) _kx = -_kx;
		else if (d.posX () == 0) _kx = 0;
		_lastDirection = 
			QGAMES::Vector (__BD ((_kx >= 0) ? 1 : -1), __BD (((_ky - _ka - _kn) >= __BD 0) ? -1 : 1), __BD 0);
	}
	else
	{
		QGAMES::bdata okn, ox, oy;
		okn = _kn; ox = _x; oy = _y;
		_kn = _kn + __BD 2 * _ka;
		_x = _x + _kx;
		_y = _y - _ky + _ka + _kn;
		if (e -> canMove (QGAMES::Vector (__BD (_x - ox), __BD (_y - oy), __BD 0), acceleration ()))
		{
			_lastDirection = 
				QGAMES::Vector (__BD ((_kx >= 0) ? 1 : -1), __BD (((_ky - _ka - _kn) >= __BD 0) ? -1 : 1), __BD 0);
			e -> setPosition (QGAMES::Position (__BD _x,__BD _y, __BD 0));
		}
		else
			notify (QGAMES::Event (__QGAMES_MOVEMENTNOTPOSSIBLE__, this));
	}

	e -> setOrientation (direction ());
}

// ---
QGAMES::MovementOneByOne::MovementOneByOne (int id, std::map <int, double> v)
	: QGAMES::Movement (id, v),
	  _speed (0),
	  _baseSpeed (0)
{
	_counterMSpeed [0] = _counterMSpeed [1] = 0;	
}

// ---
QGAMES::SetOfOpenValues QGAMES::MovementOneByOne::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Movement::runtimeValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_speed));
	result.addOpenValue (lE + 2, QGAMES::OpenValue (_counterMSpeed [0]));
	result.addOpenValue (lE + 3, QGAMES::OpenValue (_counterMSpeed [1]));

	return (result);
}

// ---
void QGAMES::MovementOneByOne::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1) && cfg.existOpenValue (lE - 2));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_counterMSpeed [1] = cCfg.openValue (lE).intValue ();
	cCfg.removeOpenValue (lE);
	_counterMSpeed [0] = cCfg.openValue (lE - 1).intValue ();
	cCfg.removeOpenValue (lE - 1);

	QGAMES::Movement::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::MovementOneByOne::initialize ()
{
	QGAMES::Movement::initialize ();

	_speed = (int) variable (0);
	_baseSpeed = (int) variable (1);
	if (_baseSpeed == 0)
		_baseSpeed = 1;
	if (_speed == 0)
		_speed = _baseSpeed;
	_counterMSpeed[0] = _counterMSpeed[1] = 0;
}

// ---
void QGAMES::MovementOneByOne::move (const QGAMES::Vector& d, const QGAMES::Vector& a, 
		QGAMES::Entity* e)
{
	if (!e -> canMove (d, a))
	{
		moveOne (QGAMES::Vector::_cero, 
			QGAMES::Vector::_cero, e); // Stops...
		notify (QGAMES::Event (__QGAMES_MOVEMENTNOTPOSSIBLE__, this));
		return;
	}

	// The ways the pacman entity moves, depends 
	// whether the speed is greater or less than the base speed...
	// If greater...
	if (_speed >= _baseSpeed)
	{
		// Moves x times points the speed...
		// As it is possible to move more than one pixel in this loop
		// It is necessary to control the entity can move more than one
		// or it must stop because it reaches
		bool contMove = true;
		for (int counterTSpeed = (int) (_speed / _baseSpeed); 
			counterTSpeed > 0 && contMove; counterTSpeed--)
		{
			moveOne (d, a, e); // Any time, it moves only one...
			if (!e -> canMove (d, a))
				contMove = false;
		}

		// Has it to continue moving...
		if (!contMove)
			return; // no...

		// If something rests...
		// Takes into account two decimals...
		// When the entity passes throught this point
		// it can only move one pixel more maximum...
		if ((_speed % _baseSpeed) != 0)
		{
			if (_counterMSpeed[0]++ == (int) ((_baseSpeed / (_speed % _baseSpeed))))
			{
				_counterMSpeed[0] = 0;
				if ((_baseSpeed % (_speed % _baseSpeed)) != 0)
				{
					if (_counterMSpeed[1]++ == 
						((_speed % _baseSpeed) / (_baseSpeed % (_speed % _baseSpeed))))
						_counterMSpeed[1] = 0;
					else
						moveOne (d, a, e); // Any time, it moves only one...
				}
			}
			else
				moveOne (d, a, e); // Any time, it moves only one...
		}
	}

	// If less...
	// In all the situations the entity moves here
	// it moves only one pixel...
	else
	{
		if (_counterMSpeed[0]++ == (int) ((_baseSpeed / (_speed % _baseSpeed))))
		{
			_counterMSpeed[0] = 0;
			moveOne (d, a, e); // Any time it moves only one...
			if ((_baseSpeed % (_speed % _baseSpeed)) != 0)
			{
				if (_counterMSpeed[1]++ < 
					((_speed % _baseSpeed) / (_baseSpeed % (_speed % _baseSpeed))))
					moveOne (d, a, e); // Any time, it moves only one...
				else
					_counterMSpeed[1] = 0;
			}
		}
	}
}

// ---
QGAMES::IncrementalMovement::IncrementalMovement (int id, std::map <int, double> v)
	: QGAMES::Movement (id, v),
	  _firstMovement (true),
	  _speed (0),
	  _data (NULL)
{
	// Nothing by default...
}

// ---
QGAMES::IncrementalMovement::~IncrementalMovement ()
{
	delete (_data);
}

// ---
QGAMES::SetOfOpenValues QGAMES::IncrementalMovement::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::Movement::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	result.addOpenValue (lE + 1, _firstMovement);
	result.addOpenValue (lE + 2, _speed);
	result.addSetOfOpenValues (lNE + 1, 
		_data ? _data -> asSetOfOpenValues () : QGAMES::SetOfOpenValues (std::string (__NULL_STRING__)));

	return (result);
}

// ---
void QGAMES::IncrementalMovement::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1) &&
			cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	QGAMES::SetOfOpenValues mVV = cCfg.setOfOpenValues (lNE);
	// It would be very strange to have a null data structure when the confg was saved...but it could happen!
	// In that situation the mVV will be empty...
	if (mVV.numberValues () != 0)
		(_data = createData ()) /** Creates the right estructure */ -> fromSetOfOpenValues (mVV);
	cCfg.removeSetOfOpenValues (lNE);

	_speed = cCfg.openValue (lE).intValue ();
	cCfg.removeOpenValue (lE);
	_firstMovement = cCfg.openValue (lE - 1).boolValue ();
	cCfg.removeOpenValue (lE - 1);

	QGAMES::Movement::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::IncrementalMovement::initialize ()
{
	_firstMovement = true;
	_speed = (int) variable (0); if (_speed <= 0) _speed = 0; // It can't be negative
	delete (_data); _data = NULL;
}

// ---
void QGAMES::IncrementalMovement::move (const QGAMES::Vector& d, const QGAMES::Vector& a, 
	QGAMES::Entity* e)
{
	assert (e);

	// The number of times to repaet this part of the code
	// will depend on the speed!
	for (int s = 0; s < _speed; s++)
	{
		// If it the first time passing through here...
		// it is only needed just to prepare the movement!
		if (_firstMovement)
		{
			if (_data == NULL) setData (); 
			// Internal Data to track the movement is set, if there is nothing before
			// Remember that the internal data varible can be set, i.e initializing the movement
			_data -> _initialPosition = e -> position ();
			_data -> _nextDirection = _data -> _nextAcceleration =
				QGAMES::Vector::_cero; // Starts from the beggining...

			prepareMovement (_data, d, a, e); // Additional data preparation...

			// It is not longer the first movement...
			_firstMovement = false;
		}
		// Otherwise the entity is moved... using Bresemham's algorithm
		else
		{
			// What is the increment to execute?
			int l = abs (_data -> _incX); int dx2 = l << 1;
			int m = abs (_data -> _incY); int dy2 = m << 1;
			int n = abs (_data -> _incZ); int dz2 = n << 1;
			// Vectors with the little steps (one by one) per direction
			QGAMES::Vector vX (__BD ((_data -> _incX < 0) ? -1 : 1), __BD 0, __BD 0);
			QGAMES::Vector vY (__BD 0, __BD ((_data -> _incY < 0) ? -1 : 1), __BD 0);
			QGAMES::Vector vZ (__BD 0, __BD 0, __BD ((_data -> _incZ < 0) ? -1 : 1));
	
			int err_1, err_2;

			// The X axis is the predominat!
			if ((l >= m) && (l >= n)) 
			{
				err_1 = dy2 - l; err_2 = dz2 - l;
				bool mX = true; bool mY = true; bool mZ = true;
				bool nMX = false; bool nMY = false; bool nMZ = false;
				for (int i = 0; i < l && mX; i++) 
				{
					if (err_1 > 0)
					{
						err_1 -= dx2;
						if (mY && (mY = e -> canMove (vY, _data -> _nextAcceleration)))
							e -> setPosition (e -> position () + vY);
						else if (!nMY)
						{
							nMY = true; // Just one notification...
							notify (QGAMES::Event (__QGAMES_MOVEMENTNOTPOSSIBLE__, this));
							stopDuringMovement (vY, _data -> _nextAcceleration, e);
						}
					}

					if (err_2 > 0)
					{
						err_2 -= dx2;
						if (mZ && (mZ = e -> canMove (vZ, _data -> _nextAcceleration)))
							e -> setPosition (e -> position () + vZ);
						else if (!nMZ)
						{
							nMZ = true;
							notify (QGAMES::Event (__QGAMES_MOVEMENTNOTPOSSIBLE__, this));
							stopDuringMovement (vZ, _data -> _nextAcceleration, e);
						}
					}

					err_1 += dy2; err_2 += dz2;
					if (mX && (mX = e -> canMove (vX, _data -> _nextAcceleration)))
						e -> setPosition (e -> position () + vX);
					else if (!nMX)
					{
						nMX = true;
						notify (QGAMES::Event (__QGAMES_MOVEMENTNOTPOSSIBLE__, this));
						stopDuringMovement (vX, _data -> _nextAcceleration, e);
					}
				}
			} 
			// The Y Axis is the predominat...
			else if ((m >= l) && (m >= n)) 
			{
				err_1 = dx2 - m; err_2 = dz2 - m;
				bool mX = true; bool mY = true; bool mZ = true;
				bool nMX = false; bool nMY = false; bool nMZ = false;
				for (int i = 0; i < m && mY; i++) 
				{
					if (err_1 > 0)
					{
						err_1 -= dy2;
						if (mX && (mX = e -> canMove (vX, _data -> _nextAcceleration)))
							e -> setPosition (e -> position () + vX);
						else if (!nMX)
						{
							nMX = true; // Just one notification...
							notify (QGAMES::Event (__QGAMES_MOVEMENTNOTPOSSIBLE__, this));
							stopDuringMovement (vX, _data -> _nextAcceleration, e);
						}
					}

					if (err_2 > 0)
					{
						err_2 -= dy2;
						if (mZ && (mZ = e -> canMove (vZ, _data -> _nextAcceleration)))
							e -> setPosition (e -> position () + vZ);
						else if (!nMZ)
						{
							nMZ = true;
							notify (QGAMES::Event (__QGAMES_MOVEMENTNOTPOSSIBLE__, this));
							stopDuringMovement (vZ, _data -> _nextAcceleration, e);
						}
					}

					err_1 += dx2; err_2 += dz2;
					if (mY && (mY = e -> canMove (vY, _data -> _nextAcceleration)))
						e -> setPosition (e -> position () + vY);
					else if (!nMY)
					{
						nMY = true;
						notify (QGAMES::Event (__QGAMES_MOVEMENTNOTPOSSIBLE__, this));
						stopDuringMovement (vY, _data -> _nextAcceleration, e);
					}
				}
			} 
			// The Z Axis is th predominat...
			else 
			{
				err_1 = dy2 - n; err_2 = dx2 - n;
				bool mX = true; bool mY = true; bool mZ = true;
				bool nMX = false; bool nMY = false; bool nMZ = false;
				for (int i = 0; i < n && mZ; i++) 
				{
					if (err_1 > 0)
					{
						err_1 -= dz2;
						if (mY && (mY = e -> canMove (vY, _data -> _nextAcceleration)))
							e -> setPosition (e -> position () + vY);
						else if (!nMY)
						{
							nMY = true; // Just one notification...
							notify (QGAMES::Event (__QGAMES_MOVEMENTNOTPOSSIBLE__, this));
							stopDuringMovement (vY, _data -> _nextAcceleration, e);
						}
					}

					if (err_2 > 0)
					{
						err_2 -= dz2;
						if (mX && (mX = e -> canMove (vX, _data -> _nextAcceleration)))
							e -> setPosition (e -> position () + vX);
						else if (!nMX)
						{
							nMX = true;
							notify (QGAMES::Event (__QGAMES_MOVEMENTNOTPOSSIBLE__, this));
							stopDuringMovement (vX, _data -> _nextAcceleration, e);
						}
					}

					err_1 += dy2; err_2 += dx2;
					if (mZ && (mZ = e -> canMove (vZ, _data -> _nextAcceleration)))
						e -> setPosition (e -> position () + vZ);
					else if (!nMZ)
					{
						nMZ = true;
						notify (QGAMES::Event (__QGAMES_MOVEMENTNOTPOSSIBLE__, this));
						stopDuringMovement (vZ, _data -> _nextAcceleration, e);
					}
				}
			}

			calculateNextMovement (_data, d, a, e); // ...calculate next movement
			somethingElseToDoAfterMoving (d, a, e); // ...and exit to add something!
		}

		// Nothing else to do except prepare the entity point to the next movement...
		e -> setOrientation (_data -> _nextOrientation);
	}
}

// ---
void QGAMES::IncrementalMovement::setData (const QGAMES::IncrementalMovement::Data* o)
{
	_data = createData ();
	if (o != NULL) _data -> initFrom (o);
}

// ---
QGAMES::SetOfOpenValues QGAMES::IncrementalMovement::Data::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result;

	std::stringstream sS; sS << _initialPosition;
	result.addOpenValue (0, sS.str ());
	sS.str (std::string (__NULL_STRING__)); sS << _nextDirection;
	result.addOpenValue (1, sS.str ());
	sS.str (std::string (__NULL_STRING__)); sS << _nextAcceleration;
	result.addOpenValue (2, sS.str ());
	sS.str (std::string (__NULL_STRING__)); sS << _nextOrientation;
	result.addOpenValue (3, sS.str ());
	result.addOpenValue (4, _incX);
	result.addOpenValue (5, _incY);
	result.addOpenValue (6, _incZ);

	return (result);
}

// ---
void QGAMES::IncrementalMovement::Data::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& cfg)
{
	assert (cfg.existOpenValue (0) && cfg.existOpenValue (1) &&
			cfg.existOpenValue (2) && cfg.existOpenValue (3) &&
			cfg.existOpenValue (4) && cfg.existOpenValue (5) &&
			cfg.existOpenValue (6));

	_incZ = cfg.openValue (6).intValue ();
	_incY = cfg.openValue (5).intValue ();
	_incX = cfg.openValue (4).intValue ();
	_nextOrientation = cfg.openValue (3).strValue ();
	_nextAcceleration = cfg.openValue (2).strValue ();
	_nextDirection = cfg.openValue (1).strValue ();
	_initialPosition = cfg.openValue (0).strValue ();
}

// ---
void QGAMES::IncrementalMovement::Data::initFrom (const QGAMES::IncrementalMovement::Data* o)
{
	assert (o);
	_initialPosition = o -> _initialPosition;
	_nextDirection = o -> _nextDirection;
	_nextAcceleration = o -> _nextAcceleration;
	_nextOrientation = o -> _nextOrientation;
	_incX = o -> _incX;
	_incY = o -> _incY;
	_incZ = o -> _incZ;
}
