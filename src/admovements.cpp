#include <AdvancedArcade/admovements.hpp>
#include <AdvancedArcade/adbuilders.hpp>
#include <Common/game.hpp>
#include <sstream>

// ---
QGAMES::SetOfOpenValues QGAMES::FollowingACurveMovement::SimpleCurveTemplate::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result;

	result.addOpenValue (0, QGAMES::OpenValue (_numberPoints));

	return (result);
}

// ---
void QGAMES::FollowingACurveMovement::SimpleCurveTemplate::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	_numberPoints = oV.openValue (0).intValue ();
}

// ---
QGAMES::Position QGAMES::FollowingACurveMovement::CurveTemplate::positionNumber (int nP)
{
	if (nP < 0)
		return (firstPosition ());
	if (nP > numberPoints ())
		return (lastPosition ());
	return (positionNumberImplementation (nP));
}

// ---
QGAMES::SetOfOpenValues QGAMES::FollowingACurveMovement::LineCurveTemplate::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::FollowingACurveMovement::SimpleCurveTemplate::asSetOfOpenValues ();

	int lE = result.lastOpenValueId ();

	std::stringstream sS; sS << _to;
	result.addOpenValue (lE + 1, QGAMES::OpenValue (sS.str ()));
	sS.str (std::string (__NULL_STRING__)); sS << _vector;
	result.addOpenValue (lE + 2, QGAMES::OpenValue (sS.str ()));

	return (result);
}

// ---
void QGAMES::FollowingACurveMovement::LineCurveTemplate::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	int lE = oV.lastOpenValueId ();

	assert (oV.existOpenValue (lE) && oV.existOpenValue (lE - 1));

	QGAMES::SetOfOpenValues cCfg = oV;

	_vector = QGAMES::Vector (cCfg.openValue (lE).strValue ());
	cCfg.removeOpenValue (lE);
	_to = QGAMES::Vector (cCfg.openValue (lE - 1).strValue ());
	cCfg.removeOpenValue (lE - 1);

	QGAMES::FollowingACurveMovement::SimpleCurveTemplate::fromSetOfOpenValues (cCfg);
}

// ---
void QGAMES::FollowingACurveMovement::LineCurveTemplate::rotate (const QGAMES::Vector& v, QGAMES::bdata ang)
{ 
	_to		= _to.rotate (QGAMES::Position::_cero, v, ang); 
	_vector	= _vector.rotate (QGAMES::Position::_cero, v, ang); 
}

// ---
void QGAMES::FollowingACurveMovement::LineCurveTemplate::setNumberPoints (int nP)
{
	QGAMES::FollowingACurveMovement::SimpleCurveTemplate::setNumberPoints (nP);

	_vector = _to / __BD _numberPoints;
}

// ---
QGAMES::FollowingACurveMovement::CircleCurveTemplate::CircleCurveTemplate (int nP, const Vector& p,  
		bdata rdx, QGAMES::bdata aI, QGAMES::bdata aF)
	: QGAMES::FollowingACurveMovement::SimpleCurveTemplate (nP),
	  _plane (p), _radix (rdx), _initialAngle (aI), _finalAngle (aF),
	  _newX (QGAMES::Vector::_cero), _newY (QGAMES::Vector::_cero),
	  _position0 (QGAMES::Position::_cero)
{ 
	assert (_plane != Vector::_cero);

	setNumberPoints (nP);
}

// ---
QGAMES::SetOfOpenValues QGAMES::FollowingACurveMovement::CircleCurveTemplate::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::FollowingACurveMovement::SimpleCurveTemplate::asSetOfOpenValues ();

	int lE = result.lastOpenValueId ();

	std::stringstream sS; sS << _plane;
	result.addOpenValue (lE + 1, QGAMES::OpenValue (sS.str ()));
	result.addOpenValue (lE + 2, QGAMES::OpenValue (_radix));
	result.addOpenValue (lE + 3, QGAMES::OpenValue (_initialAngle));
	result.addOpenValue (lE + 4, QGAMES::OpenValue (_finalAngle));
	result.addOpenValue (lE + 5, QGAMES::OpenValue (_step));
	sS.str (std::string (__NULL_STRING__)); sS << _newX;
	result.addOpenValue (lE + 6, QGAMES::OpenValue (sS.str ()));
	sS.str (std::string (__NULL_STRING__)); sS << _newY;
	result.addOpenValue (lE + 7, QGAMES::OpenValue (sS.str ()));
	sS.str (std::string (__NULL_STRING__)); sS << _position0;
	result.addOpenValue (lE + 8, QGAMES::OpenValue (sS.str ()));

	return (result);
}

// ---
void QGAMES::FollowingACurveMovement::CircleCurveTemplate::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	int lE = oV.lastOpenValueId ();

	assert (oV.existOpenValue (lE) && oV.existOpenValue (lE - 1) && oV.existOpenValue (lE - 2) &&
			oV.existOpenValue (lE - 3) && oV.existOpenValue (lE - 4) && oV.existOpenValue (lE - 5) &&
			oV.existOpenValue (lE - 6) && oV.existOpenValue (lE - 7));

	QGAMES::SetOfOpenValues cCfg = oV;

	_position0 = QGAMES::Vector (cCfg.openValue (lE).strValue ());
	cCfg.removeOpenValue (lE);
	_newY = QGAMES::Vector (cCfg.openValue (lE - 1).strValue ());
	cCfg.removeOpenValue (lE - 1);
	_newX = QGAMES::Vector (cCfg.openValue (lE - 2).strValue ());
	cCfg.removeOpenValue (lE - 2);
	_step = cCfg.openValue (lE - 3).bdataValue ();
	cCfg.removeOpenValue (lE - 3);
	_finalAngle = cCfg.openValue (lE - 4).bdataValue ();
	cCfg.removeOpenValue (lE - 4);
	_initialAngle = cCfg.openValue (lE - 5).bdataValue ();
	cCfg.removeOpenValue (lE - 5);
	_radix = cCfg.openValue (lE - 6).bdataValue ();
	cCfg.removeOpenValue (lE - 6);
	_plane = QGAMES::Vector (cCfg.openValue (lE - 7).strValue ());
	cCfg.removeOpenValue (lE - 7);

	QGAMES::FollowingACurveMovement::SimpleCurveTemplate::fromSetOfOpenValues (cCfg);
}

// ---
void QGAMES::FollowingACurveMovement::CircleCurveTemplate::rotate (const QGAMES::Vector& v, QGAMES::bdata ang)
{ 
	_plane		= _plane.rotate (QGAMES::Position::_cero, v, ang);
	_newX		= _newX.rotate (QGAMES::Position::_cero, v, ang);
	_newY		= _newY.rotate (QGAMES::Position::_cero, v, ang);
	_position0	= _position0.rotate	(QGAMES::Position::_cero, v,ang); 
}

// ---
void QGAMES::FollowingACurveMovement::CircleCurveTemplate::setNumberPoints (int nP)
{
	QGAMES::FollowingACurveMovement::SimpleCurveTemplate::setNumberPoints (nP);
	
	_step = (_finalAngle - _initialAngle) / __BD _numberPoints; 

	QGAMES::bdata rotX = 
		(_plane.posY () == __BD 0 && _plane.posZ () == __BD 0) 
			? __BD 0 : QGAMES::Vector (__BD 0, _plane.posY (), _plane.posZ ()).angleWith (QGAMES::Vector::_zNormal);
	QGAMES::bdata rotZ = 
		(_plane.posX () == __BD 0 && _plane.posY () == __BD 0)
			? __BD 0 : QGAMES::Vector (_plane.posX (), _plane.posY (), __BD 0).angleWith (QGAMES::Vector::_yNormal);
	_newY  = _radix * QGAMES::Vector::_yNormal.
		rotate (QGAMES::Position::_cero, QGAMES::Position::_xNormal, rotX).
		rotate (QGAMES::Position::_cero, QGAMES::Position::_zNormal, rotZ).normalize (); // It is not really needed.
	_newX  = _radix * _newY.crossProduct (_plane).normalize (); // It is not really needed.

	_position0 = positionNumberImplementation (0);
}

// ---
QGAMES::Position QGAMES::FollowingACurveMovement::CircleCurveTemplate::positionNumberImplementation (int nP)
{   
	int rNP = (nP <= _numberPoints) ? nP : _numberPoints;
								
	return (-_position0 +
		(__BD  cos (_initialAngle + (_step * (double) rNP)) * _newX) + 
		(__BD -sin (_initialAngle + (_step * (double) rNP)) * _newY)); 
}

// ---
QGAMES::FollowingACurveMovement::SinosoidalCurveTemplate::SinosoidalCurveTemplate 
		(int nP, const QGAMES::Vector& p, 
		QGAMES::bdata rg, QGAMES::bdata pr, QGAMES::bdata aI, QGAMES::bdata aF)
	: QGAMES::FollowingACurveMovement::SimpleCurveTemplate (nP),
	  _plane (p), _range (rg), _period (pr), _initialAngle (aI), _finalAngle (aF),
	  _newX (QGAMES::Vector::_cero), _newY (QGAMES::Vector::_cero),
	  _position0 (QGAMES::Position::_cero)
{
	assert (_plane != Vector::_cero);

	setNumberPoints (nP);
}

// ---
QGAMES::SetOfOpenValues QGAMES::FollowingACurveMovement::SinosoidalCurveTemplate::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::FollowingACurveMovement::SimpleCurveTemplate::asSetOfOpenValues ();

	int lE = result.lastOpenValueId ();

	std::stringstream sS; sS << _plane;
	result.addOpenValue (lE + 1, QGAMES::OpenValue (sS.str ()));
	result.addOpenValue (lE + 2, QGAMES::OpenValue (_range));
	result.addOpenValue (lE + 3, QGAMES::OpenValue (_period));
	result.addOpenValue (lE + 4, QGAMES::OpenValue (_initialAngle));
	result.addOpenValue (lE + 5, QGAMES::OpenValue (_finalAngle));
	result.addOpenValue (lE + 6, QGAMES::OpenValue (_step));
	sS.str (std::string (__NULL_STRING__)); sS << _newX;
	result.addOpenValue (lE + 7, QGAMES::OpenValue (sS.str ()));
	sS.str (std::string (__NULL_STRING__)); sS << _newY;
	result.addOpenValue (lE + 8, QGAMES::OpenValue (sS.str ()));
	sS.str (std::string (__NULL_STRING__)); sS << _position0;
	result.addOpenValue (lE + 9, QGAMES::OpenValue (sS.str ()));

	return (result);
}

// ---
void QGAMES::FollowingACurveMovement::SinosoidalCurveTemplate::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	int lE = oV.lastOpenValueId ();

	assert (oV.existOpenValue (lE) && oV.existOpenValue (lE - 1) && oV.existOpenValue (lE - 2) &&
			oV.existOpenValue (lE - 3) && oV.existOpenValue (lE - 4) && oV.existOpenValue (lE - 5) &&
			oV.existOpenValue (lE - 6) && oV.existOpenValue (lE - 7) && oV.existOpenValue (lE - 8));

	QGAMES::SetOfOpenValues cCfg = oV;

	_position0 = QGAMES::Vector (cCfg.openValue (lE).strValue ());
	cCfg.removeOpenValue (lE);
	_newY = QGAMES::Vector (cCfg.openValue (lE - 1).strValue ());
	cCfg.removeOpenValue (lE - 1);
	_newX = QGAMES::Vector (cCfg.openValue (lE - 2).strValue ());
	cCfg.removeOpenValue (lE - 2);
	_step = cCfg.openValue (lE - 3).bdataValue ();
	cCfg.removeOpenValue (lE - 3);
	_finalAngle = cCfg.openValue (lE - 4).bdataValue ();
	cCfg.removeOpenValue (lE - 4);
	_initialAngle = cCfg.openValue (lE - 5).bdataValue ();
	cCfg.removeOpenValue (lE - 5);
	_period = cCfg.openValue (lE - 6).bdataValue ();
	cCfg.removeOpenValue (lE - 6);
	_range = cCfg.openValue (lE - 7).bdataValue ();
	cCfg.removeOpenValue (lE - 7);
	_plane = QGAMES::Vector (cCfg.openValue (lE - 8).strValue ());
	cCfg.removeOpenValue (lE - 8);

	QGAMES::FollowingACurveMovement::SimpleCurveTemplate::fromSetOfOpenValues (cCfg);
}

// ---
void QGAMES::FollowingACurveMovement::SinosoidalCurveTemplate::rotate (const QGAMES::Vector& v, QGAMES::bdata ang)
{
	_plane		= _plane.rotate (QGAMES::Position::_cero, v, ang);
	_newX		= _newX.rotate (QGAMES::Position::_cero, v, ang);
	_newY		= _newY.rotate (QGAMES::Position::_cero, v, ang);
	_position0	= _position0.rotate (QGAMES::Position::_cero, v, ang);
}

// ---
void QGAMES::FollowingACurveMovement::SinosoidalCurveTemplate::setNumberPoints (int nP)
{
	QGAMES::FollowingACurveMovement::SimpleCurveTemplate::setNumberPoints (nP);

	_step = (_finalAngle - _initialAngle) / __BD _numberPoints; 

	QGAMES::bdata rotX = 
		(_plane.posY () == __BD 0 && _plane.posZ () == __BD 0) 
			? __BD 0 : QGAMES::Vector (__BD 0, _plane.posY (), _plane.posZ ()).angleWith (QGAMES::Vector::_zNormal);
	QGAMES::bdata rotZ = 
		(_plane.posX () == __BD 0 && _plane.posY () == __BD 0)
			? __BD 0 : QGAMES::Vector (_plane.posX (), _plane.posY (), __BD 0).angleWith (QGAMES::Vector::_yNormal);
	_newY  = QGAMES::Vector::_yNormal.
		rotate (QGAMES::Position::_cero, QGAMES::Position::_xNormal, rotX).
		rotate (QGAMES::Position::_cero, QGAMES::Position::_zNormal, rotZ).normalize (); 
	_newX  = _newY.crossProduct (_plane).normalize (); 

	_position0 = positionNumberImplementation (0);
}

// ---
QGAMES::Position QGAMES::FollowingACurveMovement::SinosoidalCurveTemplate::positionNumberImplementation (int nP)
{
	int rNP = (nP <= _numberPoints) ? nP : _numberPoints;

	return (-_position0 + 
				(__BD nP * _step * _period * _newX) + 
				(_range * __BD -std::sin (_initialAngle + (double) (__BD nP * _step))) * _newY);
}

// ---
QGAMES::FollowingACurveMovement::CurveTemplate* QGAMES::FollowingACurveMovement::ComplexCurveTemplate::clone ()
{
	QGAMES::FollowingACurveMovement::CurveTemplates nCrvs;
	for (int i = 0; i < (int) _curves.size (); nCrvs.push_back (_curves [i++] -> clone ()));
	return (new QGAMES::FollowingACurveMovement::ComplexCurveTemplate (nCrvs));
}

// ---
QGAMES::SetOfOpenValues QGAMES::FollowingACurveMovement::ComplexCurveTemplate::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result;

	result.addOpenValue (0, QGAMES::OpenValue (_numberPoints));

	result.addOpenValue (1, QGAMES::OpenValue ((int) _curves.size ()));
	QGAMES::SetOfOpenValues crvsData;
	if (!_curves.empty ())
	{
		for (int i = 0; i < (int) _curves.size (); i++)
		{
			// The parameters of every curve...
			QGAMES::SetOfOpenValues crvData; // in singular...
			crvData.addOpenValue (0, QGAMES::OpenValue (_curves [i] -> type ()));
			crvData.addSetOfOpenValues (0, _curves [i] -> asSetOfOpenValues ());

			// The information is added to the list...
			crvsData.addSetOfOpenValues (i, crvData); 
		}
	}

	result.addSetOfOpenValues (0, crvsData);

	return (result);
}

// ---
void QGAMES::FollowingACurveMovement::ComplexCurveTemplate::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV)
{
	assert (oV.existOpenValue (0) && oV.existOpenValue (1) && oV.existSetOfOpenValues (0));

	_numberPoints = oV.openValue (0).intValue ();

	QGAMES::AdvancedMovementBuilder* mBld = 
		dynamic_cast <QGAMES::AdvancedMovementBuilder*> (QGAMES::Game::game () -> movementBuilder ());
	assert (mBld); // Just in case...

	int nCrvs = oV.openValue (1).intValue ();
	QGAMES::SetOfOpenValues crvsData = oV.setOfOpenValues (0);
	deleteCurves (); //Nothing previous if any...
	if (nCrvs > 0)
	{
		for (int i = 0; i < nCrvs; i++)
		{
			QGAMES::SetOfOpenValues crvData = crvsData.setOfOpenValues (i);
			QGAMES::FollowingACurveMovement::CurveTemplate* crv = // The important method...
				mBld -> curveBuilder () -> createBasicDefaultCurve (crvData.openValue (0).intValue ());
			assert (crv); // Just in case;
			_curves.push_back (crv); // The curve is added...
			crv -> fromSetOfOpenValues (crvData.setOfOpenValues (0)); // ...and then configured as well!
		}
	}

	_numberPoints = oV.openValue (0).intValue ();
}

// ---
QGAMES::bdata QGAMES::FollowingACurveMovement::ComplexCurveTemplate::length () const
{ 
	QGAMES::bdata result = __BD 0;
	for (int i = 0; i < (int) _curves.size (); result += _curves [i++] -> length ());
	return (result);
}

// ---
void QGAMES::FollowingACurveMovement::ComplexCurveTemplate::rotate (const Vector& v, bdata ang)
{
	for (int i = 0; i < (int) _curves.size (); _curves [i++] -> rotate (v, ang));
}

// ---
int QGAMES::FollowingACurveMovement::ComplexCurveTemplate::numberPoints () const
{
	if (_numberPoints != -1)
		return (_numberPoints);

	_numberPoints = 0;
	for (int i = 0; i < (int) _curves.size (); _numberPoints += _curves [i++] -> numberPoints ());
	return (_numberPoints);
}

// ---
void QGAMES::FollowingACurveMovement::ComplexCurveTemplate::setNumberPoints (int nP) 
{ 
	// The number of points are distributed propotional to the length of the specific curve...
	QGAMES::bdata l = length ();
	for (int i = 0; i < (int) _curves.size (); i++)
		_curves [i] -> setNumberPoints ((int) (__BD nP * (_curves [i] -> length () / l)));
}

// ---
QGAMES::Position QGAMES::FollowingACurveMovement::ComplexCurveTemplate::positionNumberImplementation (int nP)
{
	QGAMES::Position rP = QGAMES::Position::_cero;

	int i = 0;
	int nC = nP;
	while (i < (int) _curves.size () && nC > _curves [i] -> numberPoints ())
	{
		int lP  = _curves [i] -> numberPoints ();
		rP     += _curves [i] -> positionNumber (lP);
		nC     -= lP;

		i++;
	}

	// Can't happen...
	assert (nC >= 0); 

	// But this can, in the case a wrong position is received...
	if (i >= (int) _curves.size ()) 
	{
		i = (int) _curves.size () - 1;
		nC = _curves [i] -> numberPoints (); // NC has to be the last one...
	}

	return (rP + _curves [i] -> positionNumber (nC));
}

// ---
void  QGAMES::FollowingACurveMovement::ComplexCurveTemplate::deleteCurves ()
{
	for (int i = 0; i < (int) _curves.size (); i++)
	{
		delete (_curves [i]);
		_curves [i] = NULL;
	}

	_curves = QGAMES::FollowingACurveMovement::CurveTemplates ();
}

// ---
QGAMES::Movement* QGAMES::FollowingACurveMovement::clone () const
{
	QGAMES::FollowingACurveMovement* result = 
		new QGAMES::FollowingACurveMovement (*this);

	result -> _curve = this -> _curve -> clone (); // Copy the curve...
	result -> setData (data ());
	
	return (result);
}

// ---
QGAMES::SetOfOpenValues QGAMES::FollowingACurveMovement::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::IncrementalMovement::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	result.addOpenValue (lE + 1, _currentPoint);
	result.addOpenValue (lE + 2, _cyclic);
	result.addOpenValue (lE + 3, QGAMES::OpenValue (_curve ? _curve -> type () : __MININT__));
	result.addSetOfOpenValues (lNE + 1, _curve ? _curve -> asSetOfOpenValues () : QGAMES::SetOfOpenValues ());
	
	return (result);
}

// ---
void QGAMES::FollowingACurveMovement::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1) && cfg.existOpenValue (lE - 2) &&
			cfg.existSetOfOpenValues (lNE));

	QGAMES::AdvancedMovementBuilder* mBuilder = 
		dynamic_cast <QGAMES::AdvancedMovementBuilder*> (QGAMES::Game::game () -> movementBuilder ());
	assert (mBuilder); // Just in case...

	QGAMES::SetOfOpenValues cCfg = cfg;

	int tC = cCfg.openValue (lE).intValue ();
	cCfg.removeOpenValue (lE);
	QGAMES::SetOfOpenValues cData = cCfg.setOfOpenValues (lNE);
	cCfg.removeSetOfOpenValues (lNE);
	if (tC != __MININT__)
	{
		setCurve (mBuilder -> curveBuilder () -> createBasicDefaultCurve (tC));
		_curve -> fromSetOfOpenValues (cData); // Asjust the data after being created...
	}

	_cyclic = cCfg.openValue (lE - 1).boolValue ();
	cCfg.removeOpenValue (lE - 1);

	_currentPoint = cCfg.openValue (lE - 2).intValue ();
	cCfg.removeOpenValue (lE - 2);

	QGAMES::IncrementalMovement::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::FollowingACurveMovement::setCurve (QGAMES::FollowingACurveMovement::CurveTemplate* crv)
{ 
	delete (_curve); 
	
	_curve = crv; 
	
	// Changing the curve, means to come back to the initial point...
	_currentPoint = 0; 
}

// ---
void QGAMES::FollowingACurveMovement::initialize ()
{
	assert (_curve);

	QGAMES::IncrementalMovement::initialize ();

	_currentPoint = 0;

	// The variable 0 determines the speed of the movement...
	_curve -> setNumberPoints ((int) variable (1)); // very important variable, very related with the speed...
	_cyclic = (variable (2) == (double) 1); 
}

// ---
void QGAMES::FollowingACurveMovement::initializeFrom (const QGAMES::Movement* m)
{
	QGAMES::IncrementalMovement::initializeFrom (m);

	// What is important to know at this point is that the curve hasn't been copied at all...
	// It could be then different, but the speed if the same...

	const QGAMES::FollowingACurveMovement* mov = 
		dynamic_cast <const QGAMES::FollowingACurveMovement*> (m);
	if (!m || !mov) return;

	if (pData ()) 
		resetData ();
	setData (mov -> pData ());
}

// ---
void QGAMES::FollowingACurveMovement::prepareMovement 
	(QGAMES::IncrementalMovement::Data* d, const QGAMES::Vector& dr, const QGAMES::Vector& a, QGAMES::Entity* e)
{
	assert (e);

	QGAMES::FollowingACurveMovement::PData* pD = pData (d);
	assert (pD);

	pD -> _currentPosition = pD -> _initialPosition = e -> position ();
	pD -> _currentPositionInPixels = e -> position ().round ();
	pD -> _incX = pD -> _incY = pD -> _incZ = 0;
	pD -> _nextDirection = pD -> _nextOrientation = pD -> _nextAcceleration = QGAMES::Vector::_cero;

	_currentPoint = 0;
}

// ---
void QGAMES::FollowingACurveMovement::calculateNextMovement 
	(QGAMES::IncrementalMovement::Data* d, const QGAMES::Vector& dr, const QGAMES::Vector& a, QGAMES::Entity* e)
{
	assert (_curve);

	if (_currentPoint > _curve -> numberPoints ())
		return; // Nothing else to do when finishes...

	assert (e);

	QGAMES::FollowingACurveMovement::PData* pD = pData (d);
	assert (pD);

	QGAMES::Position lPP = pD -> _currentPosition;
	QGAMES::Position lP = pD -> _currentPositionInPixels;

	pD -> _currentPosition += (_currentPoint == 0) 
		? QGAMES::Vector::_cero 
		: (_curve ->  positionNumber (_currentPoint) - _curve -> positionNumber (_currentPoint - 1));

	QGAMES::Vector dV = pD -> _currentPosition - lPP;
	pD -> _currentPositionInPixels = pD -> _currentPosition.round ();
	QGAMES::Vector iP = pD -> _currentPositionInPixels - lP;
	d -> _incX = (int) iP.posX (); d -> _incY = (int) iP.posY (); d -> _incZ = (int) iP.posZ ();
	d -> _nextDirection = d ->_nextOrientation = 
		QGAMES::Vector (__BD ((dV.posX () < __BD 0) ? -1 : ((dV.posX () > __BD 0) ? 1 : 0)), 
						__BD ((dV.posY () < __BD 0) ? -1 : ((dV.posY () > __BD 0) ? 1 : 0)), 
						__BD ((dV.posZ () < __BD 0)	? -1 : ((dV.posZ () > __BD 0) ? 1 : 0)));

	if (++_currentPoint > _curve -> numberPoints () && _cyclic)
		_currentPoint = 0; // To starts back...
}

// ---
void QGAMES::FollowingACurveMovement::somethingElseToDoAfterMoving 
	(const QGAMES::Vector& d, const QGAMES::Vector& a, QGAMES::Entity* e)
{
	assert (_curve);

	if (_currentPoint == __QGAMES_MOVEMENTSTARTSBACK__ && _cyclic)
		notify (QGAMES::Event (0, this)); // Back...
	if (_currentPoint >= _curve -> numberPoints ())
		notify (QGAMES::Event (__QGAMES_MOVEMENTLIMITREACHED__, this)); // In the last point...
}

// ---
QGAMES::SetOfOpenValues QGAMES::FollowingACurveMovement::PData::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::IncrementalMovement::Data::asSetOfOpenValues ();

	int lE = result.lastOpenValueId ();

	std::stringstream sS; sS << _currentPosition;
	result.addOpenValue (lE + 1, sS.str ());
	sS.str (std::string (__NULL_STRING__)); sS << _currentPositionInPixels;
	result.addOpenValue (lE + 2, sS.str ());

	return (result);
}

// ---
void QGAMES::FollowingACurveMovement::PData::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_currentPositionInPixels = QGAMES::Vector (cCfg.openValue (lE).strValue ());
	cCfg.removeOpenValue (lE);
	_currentPosition = QGAMES::Vector (cCfg.openValue (lE - 1).strValue ());
	cCfg.removeOpenValue (lE - 1);

	QGAMES::IncrementalMovement::Data::fromSetOfOpenValues (cCfg);
}

// ---
void QGAMES::FollowingACurveMovement::PData::initFrom (const Data* o)
{
	assert (o);
	const QGAMES::FollowingACurveMovement::PData* oP = 
		dynamic_cast <const QGAMES::FollowingACurveMovement::PData*> (o);
	assert (oP);

	QGAMES::IncrementalMovement::Data::initFrom (o);
	_currentPosition = oP -> _currentPosition;
	_currentPositionInPixels = oP -> _currentPositionInPixels;
}

// ---
QGAMES::Movement* QGAMES::IncrementalLinearMovement::clone () const
{
	QGAMES::IncrementalLinearMovement* result = 
		new QGAMES::IncrementalLinearMovement (*this);
	result -> setData (data ());
	return (result);
}

// ---
QGAMES::SetOfOpenValues QGAMES::IncrementalLinearMovement::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::IncrementalMovement::runtimeValues ();

	int lE = result.lastOpenValueId ();

	std::stringstream sS; sS << _from;
	result.addOpenValue (lE + 1, QGAMES::OpenValue (sS.str ()));
	sS.str (std::string (__NULL_STRING__)); sS << _to;
	result.addOpenValue (lE + 2, QGAMES::OpenValue (sS.str ()));
	result.addOpenValue (lE + 3, QGAMES::OpenValue (_maxSpeed));
	result.addOpenValue (lE + 4, QGAMES::OpenValue (_acceleration));
	result.addOpenValue (lE + 5, QGAMES::OpenValue (_decceleration));
	sS.str (std::string (__NULL_STRING__)); sS << _externalSpeed;
	result.addOpenValue (lE + 6, QGAMES::OpenValue (sS.str ()));
	sS.str (std::string (__NULL_STRING__)); sS << _externalAcceleration;
	result.addOpenValue (lE + 7, QGAMES::OpenValue (sS.str ()));
	result.addOpenValue (lE + 8, QGAMES::OpenValue (_zone));
	sS.str (std::string (__NULL_STRING__)); sS << _increment;
	result.addOpenValue (lE + 9, QGAMES::OpenValue (sS.str ()));

	sS.str (std::string (__NULL_STRING__)); sS << _k1;
	result.addOpenValue (lE + 10, QGAMES::OpenValue (sS.str ()));
	sS.str (std::string (__NULL_STRING__)); sS << _k2;
	result.addOpenValue (lE + 11, QGAMES::OpenValue (sS.str ()));
	sS.str (std::string (__NULL_STRING__)); sS << _k3;
	result.addOpenValue (lE + 12, QGAMES::OpenValue (sS.str ()));
	sS.str (std::string (__NULL_STRING__)); sS << _k4;
	result.addOpenValue (lE + 13, QGAMES::OpenValue (sS.str ()));
	
	return (result);
}

// ---
void QGAMES::IncrementalLinearMovement::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1) &&
			cfg.existOpenValue (lE - 2) && cfg.existOpenValue (lE - 3) &&
			cfg.existOpenValue (lE - 4) && cfg.existOpenValue (lE - 5) &&
			cfg.existOpenValue (lE - 6) && cfg.existOpenValue (lE - 7) &&
			cfg.existOpenValue (lE - 8) && cfg.existOpenValue (lE - 9) &&
			cfg.existOpenValue (lE - 10) && cfg.existOpenValue (lE - 11) &&
			cfg.existOpenValue (lE - 12));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_k4 = QGAMES::Vector (cCfg.openValue (lE).strValue ());
	cCfg.removeOpenValue (lE);
	_k3 = QGAMES::Vector (cCfg.openValue (lE - 1).strValue ());
	cCfg.removeOpenValue (lE - 1);
	_k2 = QGAMES::Vector (cCfg.openValue (lE - 2).strValue ());
	cCfg.removeOpenValue (lE - 2);
	_k1 = QGAMES::Vector (cCfg.openValue (lE - 3).strValue ());
	cCfg.removeOpenValue (lE - 3);

	_increment = QGAMES::Vector (cCfg.openValue (lE - 4).strValue ());
	cCfg.removeOpenValue (lE - 4);
	_zone = cCfg.openValue (lE - 5).intValue ();
	cCfg.removeOpenValue (lE - 5);
	_externalAcceleration = QGAMES::Vector (cCfg.openValue (lE - 6).strValue ());
	cCfg.removeOpenValue (lE - 6);
	_externalSpeed = QGAMES::Vector (cCfg.openValue (lE - 7).strValue ());
	cCfg.removeOpenValue (lE - 7);
	_decceleration = cCfg.openValue (lE - 8).boolValue ();
	cCfg.removeOpenValue (lE - 8);
	_acceleration = cCfg.openValue (lE - 9).bdataValue ();
	cCfg.removeOpenValue (lE - 9);
	_maxSpeed = cCfg.openValue (lE - 10).bdataValue ();
	cCfg.removeOpenValue (lE - 10);
	_to = QGAMES::Vector (cCfg.openValue (lE - 11).strValue ());
	cCfg.removeOpenValue (lE - 11);
	_from = QGAMES::Vector (cCfg.openValue (lE - 12).strValue ());
	cCfg.removeOpenValue (lE - 12);

	QGAMES::IncrementalMovement::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::IncrementalLinearMovement::setMovePath (const QGAMES::Position& from, 
	const QGAMES::Position& to, const QGAMES::Vector& eS, QGAMES::bdata mS, QGAMES::bdata a, bool dC)
{
	assert (a > 0); // Always positive value...

	_from = from; 
	_to = to; 
	_entrySpeed = eS;
	_maxSpeed = mS;
	_acceleration = std::abs (a);
	_decceleration= dC;

	_increment = (_to - _from).normalize ();

	if (_acceleration == __BD 0 && 
		std::abs (_entrySpeed.normalize ().dotProduct (_increment)) > 0.01)
		_entrySpeed = _entrySpeed.module () * _increment; 
	// The entry speed in the same direction when tehre is no acceleration to change it...

	// The paremeters are initially set to 0...
	_zone = 0;
	_k1 = _k2 = _k3 = _k4 = QGAMES::Vector::_cero;

	// Changing the move path, everything starts back...
	_firstMovement = true;
}

// ---
void QGAMES::IncrementalLinearMovement::initialize ()
{
	QGAMES::IncrementalMovement::initialize ();

	_from = _to = _entrySpeed = Vector::_cero;
	_maxSpeed = _acceleration = (bdata) 0;
	_decceleration = false;
	_zone = 0;
	_k1 = _k2 = _k3 = _k4 = Vector::_cero;
	_increment = Vector::_cero; 
}

// ---
void QGAMES::IncrementalLinearMovement::initializeFrom (const QGAMES::Movement* m)
{
	QGAMES::IncrementalMovement::initializeFrom (m);

	const QGAMES::IncrementalLinearMovement* mov = 
		dynamic_cast <const QGAMES::IncrementalLinearMovement*> (m);
	if (!m || !mov) return;

	if (pData ()) 
		resetData ();
	setData (mov -> pData ());
}

// ---
void QGAMES::IncrementalLinearMovement::prepareMovement (QGAMES::IncrementalMovement::Data* d, 
	const QGAMES::Vector& dr, const QGAMES::Vector& a, QGAMES::Entity* e)
{
	assert (e);

	QGAMES::IncrementalLinearMovement::PData* pD = pData (d);
	assert (pD);

	// The data initially used to invoke the movement....
	_externalSpeed = dr; 
	_externalAcceleration = a;

	pD -> _currentPosition = pD -> _initialPosition = e -> position ();
	pD -> _currentPositionInPixels = e -> position ().round ();
	// The speed can come from the previous movement, but is combined always what it has been fixed in the params...
	// But the speed received here is not considered
	pD -> _currentSpeed = _entrySpeed + ((pD -> _currentSpeed == QGAMES::Vector::_noPoint) 
		? QGAMES::Vector::_cero : pD -> _currentSpeed);
	// Neither the previous acceleration not the data received here is taken into account...
	pD -> _currentAcceleration =  _acceleration * _increment; 
	pD -> _incX = pD -> _incY = pD -> _incZ = 0;
	d -> _nextDirection = d ->_nextOrientation = 
		QGAMES::Vector (__BD ((pD -> _currentSpeed.posX () < __BD 0) 
								? -1 : ((pData () ->  _currentSpeed.posX () > __BD 0) ? 1 : 0)), 
						__BD ((pD -> _currentSpeed.posY () < __BD 0) 
								? -1 : ((pData () ->  _currentSpeed.posY () > __BD 0) ? 1 : 0)), 
						__BD ((pD -> _currentSpeed.posZ () < __BD 0) 
								? -1 : ((pData () ->  _currentSpeed.posZ () > __BD 0) ? 1 : 0)));
	pD -> _nextAcceleration = pD -> _currentAcceleration; // It never changes...

	// The parameters to control the movement...
	_zone = 0; // Starting from the beginning...
	calculateInternalParametersForMovement ();
	if (_acceleration == __BD 0)
		_zone = 2; // Stratight to a constant movement...
}

// ---
void QGAMES::IncrementalLinearMovement::calculateNextMovement (QGAMES::IncrementalMovement::Data* d, 
	const QGAMES::Vector& dr, const QGAMES::Vector& a, QGAMES::Entity* e)
{
	assert (e);

	QGAMES::IncrementalLinearMovement::PData* pD = pData (d);
	assert (pD);

	// Has something changed in the setMove method?
	if (dr != _externalSpeed || a != _externalAcceleration)
	{
		adjustInternalParametersWhenSetMoveChange (dr, a);

		_externalSpeed = dr; _externalAcceleration = a;
	}

	QGAMES::Position lPP = pD -> _currentPosition;
	QGAMES::Position lP = pD -> _currentPositionInPixels;

	switch (_zone)
	{
		case 0: // Entry + accelerating...
			{
				_k4 += 2 * _k3;
				pD -> _currentSpeed += _k1;
				pD -> _currentPosition += _k4;

				if (pD -> _currentSpeed.module () >= _maxSpeed)
				{
					pD -> _currentSpeed = _maxSpeed * pD -> _currentSpeed.normalize (); // at the maximum speed...
					pD -> _currentAcceleration = QGAMES::Vector::_cero; // Mantain the speed...

					_zone = 1;
				}
			}

			break;

		case 1: // Trying to correct the movement...
			{
				QGAMES::Vector k1n = _k1; k1n.normalize (); 
				QGAMES::Vector k4n = _k4; k4n.normalize ();
				QGAMES::Vector mD = (k1n + (_to - pD -> _currentPosition).normalize ()).normalize ();
				_k4 = _k4.module () * mD;

				pD -> _currentPosition += _k4;
				pD -> _currentSpeed = pD -> _currentSpeed.module () * mD;

				if (std::abs (std::abs (mD.dotProduct (k4n) - __BD 1.0)) < __BD 0.01)
				{
					_k4 = _k4.module () * mD; // To the target...
					pD -> _currentSpeed = pD -> _currentSpeed.module () * mD; // The same with the speed...
					// The acceleration is still 0 (set in the previous zone)

					_zone = 2; 
				}
			}

			break;

		case 2: // Linear movement...
			{
				pD -> _currentPosition += _k4;

				if (_decceleration &&
					(_to - pD -> _currentPosition).module () < 
						(_maxSpeed * _maxSpeed) / (2 * _acceleration))
				{
					int nfs = QGAMES::Game::game () -> framesPerSecond ();
					QGAMES::Vector nD = (_to - pD -> _currentPosition).normalize ();

					_k1 = -(_acceleration / __BD nfs) * nD; 
					_k2 = (_maxSpeed / __BD nfs) * nD;
					_k3 = (- _acceleration / __BD (2 * nfs * nfs)) * nD;
					_k4 = _k2 + _k3;

					// The speed mantains the same direction...
					pD -> _currentAcceleration = -_acceleration * nD;
					// Starts to deccelerate...

					_zone = 3;
				}
			}

			break;

		case 3: // Decceleration...
			{
				_k4 += 2 * _k3;
				pD -> _currentSpeed += _k1;
				pD -> _currentPosition += _k4;
			}

			break;

		case 4:
			break; // Nothing, because the goal has been reached...

		default: // Stopped...
			assert (0); // It shouldn't be here. No more than 4 types of zones are allowed...
			break;
	}

	pD -> _currentPositionInPixels = pD -> _currentPosition.round ();
	QGAMES::Vector iP = pD -> _currentPositionInPixels - lP;
	d -> _incX = (int) iP.posX (); d -> _incY = (int) iP.posY (); d -> _incZ = (int) iP.posZ ();
	d -> _nextDirection = d ->_nextOrientation = 
		QGAMES::Vector (__BD ((pD -> _currentSpeed.posX () < __BD 0) 
								? -1 : ((pData () ->  _currentSpeed.posX () > __BD 0) ? 1 : 0)), 
						__BD ((pD -> _currentSpeed.posY () < __BD 0) 
								? -1 : ((pData () ->  _currentSpeed.posY () > __BD 0) ? 1 : 0)), 
						__BD ((pD -> _currentSpeed.posZ () < __BD 0) 
								? -1 : ((pData () ->  _currentSpeed.posZ () > __BD 0) ? 1 : 0)));

	if ((_to - lPP).module2 () < (_to - pD -> _currentPosition).module2 ())
		_zone = 4; // The limit is reached when the last position has been reached (or near)...
}

// ---
void QGAMES::IncrementalLinearMovement::somethingElseToDoAfterMoving (const QGAMES::Vector& d, 
	const QGAMES::Vector& a, QGAMES::Entity* e)
{
	if (_zone == 4)
		notify (QGAMES::Event (__QGAMES_MOVEMENTLIMITREACHED__, this));
}

// ---
void QGAMES::IncrementalLinearMovement::calculateInternalParametersForMovement ()
{
	QGAMES::IncrementalLinearMovement::PData* pD = pData ();
	assert (pD);

	int nfs = QGAMES::Game::game () -> framesPerSecond ();
	_k1 = pD -> _currentAcceleration / __BD nfs;
	_k2 = pD -> _currentSpeed / __BD nfs;
	_k3 = pD -> _currentAcceleration / __BD (2 * nfs * nfs);
	_k4 = _k2 + _k3;
}

// ---
QGAMES::SetOfOpenValues QGAMES::IncrementalLinearMovement::PData::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::IncrementalMovement::Data::asSetOfOpenValues ();

	int lE = result.lastOpenValueId ();

	std::stringstream sS; sS << _currentPosition;
	result.addOpenValue (lE + 1, sS.str ());
	sS.str (std::string (__NULL_STRING__)); sS << _currentPositionInPixels;
	result.addOpenValue (lE + 2, sS.str ());
	sS.str (std::string (__NULL_STRING__)); sS << _currentSpeed;
	result.addOpenValue (lE + 3, sS.str ());
	sS.str (std::string (__NULL_STRING__)); sS << _currentAcceleration;
	result.addOpenValue (lE + 4, sS.str ());

	return (result);
}

// ---
void QGAMES::IncrementalLinearMovement::PData::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1) &&
			cfg.existOpenValue (lE - 2) && cfg.existOpenValue (lE - 3));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_currentAcceleration = QGAMES::Vector (cCfg.openValue (lE).strValue ());
	cCfg.removeOpenValue (lE);
	_currentSpeed = QGAMES::Vector (cCfg.openValue (lE - 1).strValue ());
	cCfg.removeOpenValue (lE - 1);
	_currentPositionInPixels = QGAMES::Vector (cCfg.openValue (lE - 2).strValue ());
	cCfg.removeOpenValue (lE - 2);
	_currentPosition = QGAMES::Vector (cCfg.openValue (lE - 3).strValue ());
	cCfg.removeOpenValue (lE - 3);

	QGAMES::IncrementalMovement::Data::fromSetOfOpenValues (cCfg);
}

// ---
void QGAMES::IncrementalLinearMovement::PData::initFrom (const Data* o)
{
	assert (o);
	const QGAMES::IncrementalLinearMovement::PData* oP = 
		dynamic_cast <const QGAMES::IncrementalLinearMovement::PData*> (o);
	assert (oP);

	QGAMES::IncrementalMovement::Data::initFrom (o);
	_currentPosition = oP -> _currentPosition;
	_currentPositionInPixels = oP -> _currentPositionInPixels;
	_currentSpeed = oP -> _currentSpeed;
	_currentAcceleration = oP -> _currentAcceleration;
}

// ---
QGAMES::Movement* QGAMES::PhysicsIncremetalMovement::clone () const
{
	QGAMES::PhysicsIncremetalMovement* result = 
		new QGAMES::PhysicsIncremetalMovement (*this);
	result -> setData (data ());
	return (result);
}

// ---
QGAMES::SetOfOpenValues QGAMES::PhysicsIncremetalMovement::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::IncrementalMovement::runtimeValues ();

	int lE = result.lastOpenValueId ();
	int lNE = result.lastNestedOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_maxSpeed));
	result.addOpenValue (lE + 2, QGAMES::OpenValue (_minSpeed));
	
	int j = 0;
	QGAMES::SetOfOpenValues iVV (std::string (__QGAMES_RUNTIMEVALUESIMPULSESTYPE__));
	for (QGAMES::PhysicsIncremetalMovement::Impulses::const_iterator i = _impulses.begin ();
			i != _impulses.end (); i++, j++)
		iVV.addSetOfOpenValues (j, (*i).asSetOfOpenValues ());
	result.addSetOfOpenValues (lNE + 1, iVV);

	result.addOpenValue (lE + 3, QGAMES::OpenValue (_frictionCoefficient));

	std::stringstream sS; sS << _externalSpeed;
	result.addOpenValue (lE + 4, QGAMES::OpenValue (sS.str ()));
	sS.str (std::string (__NULL_STRING__)); sS << _externalAcceleration;
	result.addOpenValue (lE + 5, QGAMES::OpenValue (sS.str ()));

	result.addOpenValue (lE + 6, QGAMES::OpenValue (_directionX));
	result.addOpenValue (lE + 7, QGAMES::OpenValue (_directionY));
	result.addOpenValue (lE + 8, QGAMES::OpenValue (_directionX));

	sS.str (std::string (__NULL_STRING__)); sS << _k1;
	result.addOpenValue (lE + 9, QGAMES::OpenValue (sS.str ()));
	sS.str (std::string (__NULL_STRING__)); sS << _k2;
	result.addOpenValue (lE + 10, QGAMES::OpenValue (sS.str ()));
	sS.str (std::string (__NULL_STRING__)); sS << _k3;
	result.addOpenValue (lE + 11, QGAMES::OpenValue (sS.str ()));
	sS.str (std::string (__NULL_STRING__)); sS << _k4;
	result.addOpenValue (lE + 12, QGAMES::OpenValue (sS.str ()));

	sS.str (std::string (__NULL_STRING__)); sS << _frictionForce;
	result.addOpenValue (lE + 13, QGAMES::OpenValue (sS.str ()));

	return (result);
}

// ---
void QGAMES::PhysicsIncremetalMovement::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();
	int lNE = cfg.lastNestedOpenValueId ();

	assert (cfg.existOpenValue (lE)	&& cfg.existOpenValue (lE - 1) &&
			cfg.existOpenValue (lE - 2) && cfg.existOpenValue (lE - 3) &&
			cfg.existOpenValue (lE - 4) && cfg.existOpenValue (lE - 5) &&
			cfg.existOpenValue (lE - 6) && cfg.existOpenValue (lE - 7) &&
			cfg.existOpenValue (lE - 8)	&& cfg.existOpenValue (lE - 9) &&
			cfg.existOpenValue (lE - 10) && cfg.existOpenValue (lE - 11) &&
			cfg.existOpenValue (lE - 12) &&
			cfg.existSetOfOpenValues (lNE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_frictionForce = QGAMES::Vector (cCfg.openValue (lE).strValue ());
	cCfg.removeOpenValue (lE);

	_k4 = QGAMES::Vector (cCfg.openValue (lE - 1).strValue ());
	cCfg.removeOpenValue (lE - 1);
	_k3 = QGAMES::Vector (cCfg.openValue (lE - 2).strValue ());
	cCfg.removeOpenValue (lE - 2);
	_k2 = QGAMES::Vector (cCfg.openValue (lE - 3).strValue ());
	cCfg.removeOpenValue (lE - 3);
	_k1 = QGAMES::Vector (cCfg.openValue (lE - 4).strValue ());
	cCfg.removeOpenValue (lE - 4);

	_directionZ = cCfg.openValue (lE - 5).intValue ();
	cCfg.removeOpenValue (lE - 5);
	_directionY = cCfg.openValue (lE - 6).intValue ();
	cCfg.removeOpenValue (lE - 6);
	_directionX = cCfg.openValue (lE - 7).intValue ();
	cCfg.removeOpenValue (lE - 7);

	_externalAcceleration = QGAMES::Vector (cCfg.openValue (lE - 8).strValue ());
	cCfg.removeOpenValue (lE - 8);
	_externalSpeed = QGAMES::Vector (cCfg.openValue (lE - 9).strValue ());
	cCfg.removeOpenValue (lE - 9);

	_frictionCoefficient = cCfg.openValue (lE - 10).bdataValue ();
	cCfg.removeOpenValue (lE - 10);
	_minSpeed = cCfg.openValue (lE - 11).bdataValue ();
	cCfg.removeOpenValue (lE - 11);
	_maxSpeed = cCfg.openValue (lE - 12).bdataValue ();
	cCfg.removeOpenValue (lE - 12);

	QGAMES::SetOfOpenValues iVV = cCfg.setOfOpenValues (lNE);
	assert (iVV.name () == std::string (__QGAMES_RUNTIMEVALUESIMPULSESTYPE__));
	_impulses = QGAMES::PhysicsIncremetalMovement::Impulses ();
	QGAMES::PhysicsIncremetalMovement::Impulse nI;
	for (QGAMES::SetsOfOpenValues::const_iterator i = iVV.nestedOpenValues ().begin (); 
			i != iVV.nestedOpenValues ().end (); i++)
		{ nI.fromSetOfOpenValues ((*i).second);  _impulses.push_back (nI); }
	cCfg.removeSetOfOpenValues (lNE);

	QGAMES::IncrementalMovement::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::PhysicsIncremetalMovement::setSpeed (const QGAMES::Vector& s)
{
	assert (pData ());

	pData () -> _currentSpeed = s; 

	calculateInternalParametersForMovement ();

	pData () -> _nextDirection = pData () ->_nextOrientation = 
		QGAMES::Vector (__BD ((pData () -> _currentSpeed.posX () < __BD 0) 
								? -1 : ((pData () ->  _currentSpeed.posX () > __BD 0) ? 1 : 0)), 
						__BD ((pData () -> _currentSpeed.posY () < __BD 0) 
								? -1 : ((pData () ->  _currentSpeed.posY () > __BD 0) ? 1 : 0)), 
						__BD ((pData () -> _currentSpeed.posZ () < __BD 0) 
								? -1 : ((pData () ->  _currentSpeed.posY () > __BD 0) ? 1 : 0)));
}

// ---
void QGAMES::PhysicsIncremetalMovement::setAcceleration (const QGAMES::Vector& a)
{ 
	assert (pData ());

	pData () -> _currentAcceleration = a;

	calculateInternalParametersForMovement ();

	toAdjustNextSpeedAndAcceleration ();
}

// ---
void QGAMES::PhysicsIncremetalMovement::setAdditionalAcceleration (const QGAMES::Vector& aA)
{
	assert (pData ());

	pData () -> _currentAcceleration += aA;

	calculateInternalParametersForMovement ();

	toAdjustNextSpeedAndAcceleration ();
}

// ---
void QGAMES::PhysicsIncremetalMovement::addImpulse (const QGAMES::Vector& i, int mC)
{
	if (i == QGAMES::Vector::_cero)
		return; // Makes no sense to add no impulse...

	// Keep track of it...
	_impulses.push_back (QGAMES::PhysicsIncremetalMovement::Impulse (i, mC));
	
	// Adds the impulse to the current acceleration...
	setAdditionalAcceleration (i); 
}

// ---
void QGAMES::PhysicsIncremetalMovement::replaceImpulses (const QGAMES::Vector& i, int mC)
{
	// Calculates how is the current acceleration effect...
	QGAMES::Vector aD = QGAMES::Vector::_cero;
	std::for_each (_impulses.begin (), _impulses.end (),
		[&aD] (QGAMES::PhysicsIncremetalMovement::Impulse j) { aD += j._impulse; });
	_impulses = QGAMES::PhysicsIncremetalMovement::Impulses (); // Clear the list...

	// Removes the acculated one and adds the new one...
	addImpulse (i - aD, mC);
}

// ---
void QGAMES::PhysicsIncremetalMovement::setFrictionCoefficient (QGAMES::bdata fC)
{
	_frictionCoefficient = fC;

	_frictionForce = QGAMES::Vector::_cero;
}

// ---
void QGAMES::PhysicsIncremetalMovement::stopMovement ()
{
	assert (pData ());

	pData () -> _currentSpeed = QGAMES::Vector::_cero;
	pData () -> _currentAcceleration = QGAMES::Vector::_cero;

	calculateInternalParametersForMovement ();

	toAdjustNextSpeedAndAcceleration ();
}

// ---
void QGAMES::PhysicsIncremetalMovement::changeDirectionInAxis (int nA)
{
	assert (nA >= 0 && nA < 3); // 0 = x, 1 = y, 2 = z

	if (nA == 0) _directionX *= -1;
	else if (nA == 1) _directionY *= -1;
	else if (nA == 2) _directionZ *= -1;
}

// ---
void QGAMES::PhysicsIncremetalMovement::initialize ()
{
	QGAMES::IncrementalMovement::initialize ();

	_externalSpeed = _externalAcceleration = QGAMES::Vector::_cero;

	_maxSpeed = __MAXBDATA__; _minSpeed = __MINBDATA__;

	_impulses = QGAMES::PhysicsIncremetalMovement::Impulses ();
	
	_frictionCoefficient = __BD 0;

	_frictionForce = QGAMES::Vector::_cero;

	_k1 = _k2 = _k3 = _k4 = Vector::_cero;

	_directionX = _directionY = _directionZ = 1; // Normal movement (as it is defined)
}

// ---
void QGAMES::PhysicsIncremetalMovement::initializeFrom (const QGAMES::Movement* m)
{
	QGAMES::IncrementalMovement::initializeFrom (m);

	const QGAMES::PhysicsIncremetalMovement* mov = 
		dynamic_cast <const QGAMES::PhysicsIncremetalMovement*> (m);
	if (!m || !mov) return;

	if (pData ()) 
		resetData ();
	setData (mov -> pData ());

	// If the movement comes from another physics, 
	// the previous modificators to direction must be taken into account!
	_directionX = mov -> _directionX;
	_directionY = mov -> _directionY;
	_directionZ = mov -> _directionZ;

	// Because maybe the entity is changing the environment where it moves
	// The friction and the friction forces are not copied, leaving them to the initial values ​​...

	// But not the impulses...
	_impulses = mov -> _impulses;

	// IMPORTANT:
	// The max and minimum speed are not copied either...
	// So there it initialized.
	// They have to be set by type of movement!
}

// ---
void QGAMES::PhysicsIncremetalMovement::prepareMovement 
	(QGAMES::IncrementalMovement::Data* d, const QGAMES::Vector& dr, const QGAMES::Vector& a, QGAMES::Entity* e)
{
	assert (e);

	QGAMES::PhysicsIncremetalMovement::PData* pD = pData (d);
	assert (pData (d));

	// Keep the external data sent from setMove...
	_externalSpeed = dr;
	_externalAcceleration = a;

	pD -> _currentPosition = pD -> _initialPosition = e -> position ();
	pD -> _currentPositionInPixels = e -> position ().round ();
	// No previous movement or not speed defined there, then the parameter is taken...
	pD -> _currentSpeed = _externalSpeed + ((pD -> _currentSpeed == QGAMES::Vector::_noPoint) 
		? QGAMES::Vector::_cero : pD -> _currentSpeed);
	// No previous movement or not acceleration defined there, then the parameter is taken...
	pD -> _currentAcceleration = _externalAcceleration + ((pD -> _currentAcceleration == QGAMES::Vector::_noPoint) 
		? QGAMES::Vector::_cero : pD -> _currentAcceleration);
	pD -> _incX = pD -> _incY = pD -> _incZ = 0;

	calculateInternalParametersForMovement ();

	toAdjustNextSpeedAndAcceleration ();
}

// ---
void QGAMES::PhysicsIncremetalMovement::calculateNextMovement 
	(QGAMES::IncrementalMovement::Data* d, const QGAMES::Vector& dr, const QGAMES::Vector& a, QGAMES::Entity* e)
{
	assert (e);

	QGAMES::PhysicsIncremetalMovement::PData* pD = pData (d);
	assert (pD);

	// Manages the impulses, if there is any...
	for (int i = 0; i < (int) _impulses.size (); i++)
	{
		if (_impulses [i].addCycle ())
		{
			setAdditionalAcceleration (-_impulses [i]._impulse); // Removes the effect...
			for (int j = i + 1; j < (int) _impulses.size (); j++)
				_impulses [j - 1] = _impulses [j];
			_impulses.pop_back (); // Erase the last...
		}
	}

	// If either the external speed or acceleration has been changed, then
	// The new values are adopted, and included into the movement...
	if (dr != _externalSpeed || a != _externalAcceleration)
		adjustInternalParametersWhenSetMoveChange (_externalSpeed = dr, _externalAcceleration = a);

	// The friction cuold affect like a force opposed to the movement (determine by the speed)
	// The friction starts when there is movement, if not there is not friction...
	// The friction is constant when the speed is greater than the minimum and related whit the acceleration module
	if (_frictionCoefficient != __BD 0)
	{
		if (pD -> _currentSpeed.module () > _minSpeed)
		{	
			if (_frictionForce == QGAMES::Vector::_cero || // First time..,
				_frictionForce.crossProduct (pD -> _currentSpeed).module2 () > __QGAMES_ERROR) // ...or speed has changed it diection...
			{
				QGAMES::Vector sVV = pD ->  _currentSpeed; sVV.normalize ();
				if (_frictionForce != QGAMES::Vector::_cero)
					setAdditionalAcceleration (-_frictionForce); // Removes if something not null before...
				_frictionForce = -_frictionCoefficient * pD -> _currentAcceleration.module () * sVV;
				setAdditionalAcceleration (_frictionForce); 
			}
		}
		else
		{
			if (_frictionForce != QGAMES::Vector::_cero)
			{
				setSpeed (QGAMES::Vector::_cero);
				setAdditionalAcceleration (-_frictionForce); // Remove it...
				_frictionForce = QGAMES::Vector::_cero;
			}
		}
	}

	// Once the external "forces" have been managed, the movements takes place...
	QGAMES::Position lPP = pD -> _currentPosition;
	QGAMES::Position lP = pD -> _currentPositionInPixels;

	_k4 += 2 * _k3;
	pD -> _currentSpeed += _k1;
	pD -> _currentPosition += _k4;

	pD -> _currentPositionInPixels = pD -> _currentPosition.round ();
	QGAMES::Vector iP = pD -> _currentPositionInPixels - lP;
	d -> _incX = ((int) iP.posX ()) * _directionX; 
	d -> _incY = ((int) iP.posY ()) * _directionY;
	d -> _incZ = ((int) iP.posZ ()) * _directionZ;

	toAdjustNextSpeedAndAcceleration ();
}

// ---
void QGAMES::PhysicsIncremetalMovement::somethingElseToDoAfterMoving (const QGAMES::Vector& d, 
	const QGAMES::Vector& a, QGAMES::Entity* e)
{
	if (pData () -> _currentSpeed.module () > _maxSpeed)
		notify (QGAMES::Event (__QGAMES_MOVEMENTSPEEDLIMITREACHED__, this));
	if (pData () -> _currentSpeed.module () < _minSpeed)
		notify (QGAMES::Event (__QGAMES_MOVEMENTSPEEDLIMITREACHED__, this));
}

// ---
void QGAMES::PhysicsIncremetalMovement::adjustInternalParametersWhenSetMoveChange 
	(const QGAMES::Vector& dr, const QGAMES::Vector& ac)
{
	pData () -> _currentSpeed = dr;
	pData () -> _currentAcceleration = ac;

	calculateInternalParametersForMovement ();

	toAdjustNextSpeedAndAcceleration ();
}

// ---
void QGAMES::PhysicsIncremetalMovement::calculateInternalParametersForMovement ()
{
	QGAMES::PhysicsIncremetalMovement::PData* pD = pData ();
	assert (pD);

	int nfs = QGAMES::Game::game () -> framesPerSecond ();
	_k1 = pD -> _currentAcceleration / __BD nfs;
	_k2 = pD -> _currentSpeed / __BD nfs;
	_k3 = pD -> _currentAcceleration / __BD (2 * nfs * nfs);
	_k4 = _k2 + _k3;
}

// ---
void QGAMES::PhysicsIncremetalMovement::toAdjustNextSpeedAndAcceleration ()
{
	pData () -> _nextDirection = pData () -> _nextOrientation = 
		QGAMES::Vector (__BD (((pData () -> _currentSpeed.posX () < __BD 0) 
								? -1 : ((pData () ->  _currentSpeed.posX () > __BD 0) ? 1 : 0)) * _directionX), 
						__BD (((pData () -> _currentSpeed.posY () < __BD 0) 
								? -1 : ((pData () ->  _currentSpeed.posY () > __BD 0) ? 1 : 0)) * _directionY), 
						__BD (((pData () -> _currentSpeed.posZ () < __BD 0) 
								? -1 : ((pData () ->  _currentSpeed.posZ () > __BD 0) ? 1 : 0)) * _directionZ));

	pData () -> _nextAcceleration = pData () -> _currentAcceleration;
}

QGAMES::PhysicsIncremetalMovement::Impulse::Impulse (const QGAMES::Vector& i, int mC)
	: _impulse (i), _maxCycles (mC),  _currentCycle (0)
{ 
	assert (_impulse != QGAMES::Vector::_cero && _maxCycles >= 0); 
}

// ---
QGAMES::SetOfOpenValues QGAMES::PhysicsIncremetalMovement::Impulse::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result;

	std::stringstream sS; sS << _impulse;
	result.addOpenValue (0, QGAMES::OpenValue (sS.str ()));
	result.addOpenValue (1, QGAMES::OpenValue (_maxCycles));
	result.addOpenValue (2, QGAMES::OpenValue (_currentCycle));

	return (result);
}

// ---
void QGAMES::PhysicsIncremetalMovement::Impulse::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& cfg)
{
	assert (cfg.existOpenValue (0) && 
			cfg.existOpenValue (1) && cfg.existOpenValue (2));

	_impulse = cfg.openValue (0).strValue ();
	_maxCycles = cfg.openValue (1).intValue ();
	_currentCycle = cfg.openValue (2).intValue ();
}

// ---
QGAMES::SetOfOpenValues QGAMES::PhysicsIncremetalMovement::PData::asSetOfOpenValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::IncrementalMovement::Data::asSetOfOpenValues ();

	int lE = result.lastOpenValueId ();

	std::stringstream sS; sS << _currentPosition;
	result.addOpenValue (lE + 1, sS.str ());
	sS.str (std::string (__NULL_STRING__)); sS << _currentPositionInPixels;
	result.addOpenValue (lE + 2, sS.str ());
	sS.str (std::string (__NULL_STRING__)); sS << _currentSpeed;
	result.addOpenValue (lE + 3, sS.str ());
	sS.str (std::string (__NULL_STRING__)); sS << _currentAcceleration;
	result.addOpenValue (lE + 4, sS.str ());

	return (result);
}

// ---
void QGAMES::PhysicsIncremetalMovement::PData::fromSetOfOpenValues (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE) && cfg.existOpenValue (lE - 1) &&
			cfg.existOpenValue (lE - 2) && cfg.existOpenValue (lE - 3));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_currentAcceleration = QGAMES::Vector (cCfg.openValue (lE).strValue ());
	cCfg.removeOpenValue (lE);
	_currentSpeed = QGAMES::Vector (cCfg.openValue (lE - 1).strValue ());
	cCfg.removeOpenValue (lE - 1);
	_currentPositionInPixels = QGAMES::Vector (cCfg.openValue (lE - 2).strValue ());
	cCfg.removeOpenValue (lE - 2);
	_currentPosition = QGAMES::Vector (cCfg.openValue (lE - 3).strValue ());
	cCfg.removeOpenValue (lE - 3);

	QGAMES::IncrementalMovement::Data::fromSetOfOpenValues (cCfg);
}

// ---
void QGAMES::PhysicsIncremetalMovement::PData::initFrom (const Data* o)
{
	assert (o);
	const QGAMES::PhysicsIncremetalMovement::PData* oP = 
		dynamic_cast <const QGAMES::PhysicsIncremetalMovement::PData*> (o);
	assert (oP);

	QGAMES::IncrementalMovement::Data::initFrom (o);
	_currentPosition = oP -> _currentPosition;
	_currentPositionInPixels = oP -> _currentPositionInPixels;
	_currentSpeed = oP -> _currentSpeed;
	_currentAcceleration = oP -> _currentAcceleration;
}

// ---
QGAMES::Movement* QGAMES::BouncePhysicsIncrementalMovement::clone () const
{
	QGAMES::BouncePhysicsIncrementalMovement* result = 
		new QGAMES::BouncePhysicsIncrementalMovement (*this);
	result -> setData (data ());
	return (result);
}

// ---
QGAMES::SetOfOpenValues QGAMES::BouncePhysicsIncrementalMovement::runtimeValues () const
{
	QGAMES::SetOfOpenValues result = QGAMES::PhysicsIncremetalMovement::runtimeValues ();

	int lE = result.lastOpenValueId ();

	result.addOpenValue (lE + 1, QGAMES::OpenValue (_elasticCoefficient));

	return (result);
}

// ---
void QGAMES::BouncePhysicsIncrementalMovement::initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg)
{
	int lE = cfg.lastOpenValueId ();

	assert (cfg.existOpenValue (lE));

	QGAMES::SetOfOpenValues cCfg = cfg;

	_elasticCoefficient = cCfg.openValue (lE).bdataValue ();
	cCfg.removeOpenValue (lE);

	QGAMES::PhysicsIncremetalMovement::initializeRuntimeValuesFrom (cCfg);
}

// ---
void QGAMES::BouncePhysicsIncrementalMovement::initialize ()
{
	QGAMES::PhysicsIncremetalMovement::initialize ();

	_elasticCoefficient = __BD 0.5; // The default one...
}

// ---
void QGAMES::BouncePhysicsIncrementalMovement::stopDuringMovement
	(const QGAMES::Vector& dr, const QGAMES::Vector& a, QGAMES::Entity* e)
{
	int aA = 0x00;
	if (dr.posX () != 0) aA |= 0x01;
	if (dr.posY () != 0) aA |= 0x02;
	if (dr.posZ () != 0) aA |= 0x04;
	// Not possible that various at the same time, but just in case...

	// Change the speed...
	pData () -> _currentSpeed = QGAMES::Vector (
		((aA & 0x01) ? __BD -1 : __BD 1) * pData () -> _currentSpeed.posX (),
		((aA & 0x02) ? __BD -1 : __BD 1) * pData () -> _currentSpeed.posY (),
		((aA & 0x04) ? __BD -1 : __BD 1) * pData () -> _currentSpeed.posZ ());
	QGAMES::Vector nSV = pData () -> _currentSpeed; nSV.normalize (); // The normalize vector of the speed...
	// But not the acceleration...

	// Then its module is reduced...
	pData () -> _currentSpeed = _elasticCoefficient * pData () -> _currentSpeed.module () * nSV;

	// Not to bounce any more if the limit speed has been reached...
	// Notice that this event is different than the one issued after the movement is executed
	notify (QGAMES::Event (pData () -> _currentSpeed.module () < _minSpeed 
		? __QGAMES_NOTBOUNCINGMOVEMENTANYMORE__ : __QGAMES_BOUNCINGMOVEMENT__, this));

	// ...and the internal paraneters are calculated back...
	calculateInternalParametersForMovement ();

	// Notoce that this movement only notifies it has finished when the speed is below a minimum o above a maximum
	// Take care and define them at the beginning of the movement.
}
