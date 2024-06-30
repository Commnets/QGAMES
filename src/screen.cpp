#include <Common/screen.hpp>
#include <Common/form.hpp>

const int QGAMES::Screen::_FONTBOLD = 0x01;
const int QGAMES::Screen::_FONTITALIC = 0x02;
const int QGAMES::Screen::_FONTUNDERLINE = 0x04;
const int QGAMES::Screen::_FONTSTRIKETHROUGH = 0x08;
const int QGAMES::Screen::_FONTNORMAL = 0x10;

// ---
void QGAMES::ZoommerOnScreen::toInitialPosition ()
{
	setValue (__BD 1);
	setEffect (QGAMES::ZoommerOnScreen::Effect::_TOUPLEFT);
}

// ---
void QGAMES::ZoommerOnScreen::setStatus (const QGAMES::ZoommerOnScreen::Status& st)
{
	setValue (st._value);

	setEffect (st._effect);

	adjustLimits (st._maximumWidth, st._maximumHeight);
}

// ---
QGAMES::bdata QGAMES::ZoommerOnScreen::effectOver (QGAMES::bdata a, bool x) const
{
	QGAMES::bdata result = a;

	switch (_status._effect)
	{
		case QGAMES::ZoommerOnScreen::_CENTER:
			result = (_useSoftware) 
				? (a + (((x) ? _status._scaleVector.posX () : _status._scaleVector.posY ()) / _status._value))
				: (a * _status._value + ((x) ? _status._scaleVector.posX () : _status._scaleVector.posY ()));
			break;

		case QGAMES::ZoommerOnScreen::_TOUPLEFT:
			result = (_useSoftware) ? a : (a * _status._value);
			break;

		case QGAMES::ZoommerOnScreen::_TODOWNRIGHT:
			result = (_useSoftware) 
				? (a + (((x) ? (QGAMES::bdata) 2 * _status._scaleVector.posX () : (QGAMES::bdata) 2 * _status._scaleVector.posY ()) / _status._value))
				: (a * _status._value + ((x) ? (QGAMES::bdata) 2 * _status._scaleVector.posX () : (QGAMES::bdata) 2 * _status._scaleVector.posY ()));
			break;
	}

	return (result);
}

// ---
void QGAMES::ZoommerOnScreen::calculateVectorScale ()
{
	// Let's suppose we are representing a figure in the screen...
	// Imagine we want to maintain the figure always in the middle of the screen...
	// The figure will be: (_WIDTH * _SCALE) pixels long.
	// It would have an empty right (and left) space of: ((_SCREENWIDTH - (WIDTH * _SCALE)) / 2) pixels long.
	// Let's figure out the length of the figure is _WIDTH and _SCALE the scale being applied.
	// The same way of thinking can be applied to the up and down spaces.
	// The scale vector is the vector we should add to any scaled point to maintain 
	// the screen's main reference point at the middle of the screen, and it is equivalent
	// To the length of empty space an scaled figure left...
	_status._scaleVector = QGAMES::Vector ((QGAMES::bdata) ((1 - _status._value) * (_status._maximumWidth / 2)),
		(QGAMES::bdata) ((1 - _status._value) * (_status._maximumHeight / 2)), (QGAMES::bdata) 0);
}

// ---
void QGAMES::Camera::adjustLimitsTo (int mW, int mH)
{ 
	assert (mH > 0 && mW > 0);

	_status._maximumWidth = mW; _status._maximumHeight = mH;
	_status._screenRatio = __BD _status._maximumWidth / __BD _status._maximumHeight;
	_status._conicLimitZ = (-__BD _status._maximumWidth / __BD 2) * __BD 0.9; // 90% of the maximum (to prevent limits)

	if (_zoommer) 
		_zoommer -> adjustLimits (_status._maximumWidth, _status._maximumHeight); 
}

// ---
void QGAMES::Camera::adjustLimitsTo (QGAMES::Screen* scr)
{
	assert (scr);

	adjustLimitsTo (scr -> visualWidth (), scr -> visualHeight ());
}

// ---
QGAMES::Position QGAMES::Camera::center () const
{
	return (_status._position + 
		QGAMES::Vector (
			QGAMES::Vector (_status._horizontalVector).normalize ().posX () * __BD _status._maximumWidth / 2,
			_status._orientation.crossProduct (_status._horizontalVector).normalize ().posY () *  __BD _status._maximumHeight / 2, 
			__BD 0));
}

// ---
void QGAMES::Camera::rotateAround (const QGAMES::Vector& n, const QGAMES::Position& p, QGAMES::bdata a)
{ 
	setOrientation (_status._orientation.rotate (p, p + n, a),
					_status._horizontalVector.rotate (p, p + n, a));

	// Pitch (like moving the head up and down. Rotating over the axis OX. What moves is OZ and OY)
	// The negative sign is for moving things to the other way around...
	QGAMES::bdata pmM [3][3] =  { { __BD 1, __BD 0,								 __BD 0 }, 
								  { __BD 0, __BD std::cos (_status._pitchAngle), __BD -std::sin (_status._pitchAngle) },
								  { __BD 0, __BD std::sin (_status._pitchAngle), __BD  std::cos (_status._pitchAngle) } };
	QGAMES::Matrix3 pM (pmM);

	// Yaw (like moving the head to the right and to the left. Rotating over the axis OY. What moves is OX and OZ)
	// The negative sign is for moving things to the other way around...
	QGAMES::bdata ymM [3][3] =  { { __BD  std::cos (_status._yawAngle), __BD 0, __BD std::sin (_status._yawAngle) }, 
								  { __BD  0,							__BD 1, __BD 0 },
								  { __BD -std::sin (_status._yawAngle), __BD 0, __BD std::cos (_status._yawAngle) } };
	QGAMES::Matrix3 yM (ymM);

	// Roll (like rotating the head to the right or to the left. Rotating over the axis OZ. What moves is OX and OY)
	// The negative sign is for moving things to the other way around...
	QGAMES::bdata rmM [3][3] =  { { __BD std::cos (_status._rollAngle), __BD -std::sin (_status._rollAngle), __BD 0 }, 
								  { __BD std::sin (_status._rollAngle), __BD  std::cos (_status._rollAngle), __BD 0 },
								  { __BD 0,								__BD 0,								 __BD 1 } };
	QGAMES::Matrix3 rM (rmM);

	_status._eulerMatrix = pM * yM * rM;
}

// ---
void QGAMES::Camera::toInitialPosition ()
{
	setPosition (QGAMES::Position::_cero);

	setOrientation (QGAMES::Vector::_zNormal, QGAMES::Vector::_xNormal);

	_status._yawAngle = _status._pitchAngle; _status._rollAngle = __BD 0;
	_status._screenRatio = __BD 1;
	_status._conicLimitZ = __BD -100;
	_status._eulerMatrix = QGAMES::Matrix3 (__BD 1); // Identity...

	_zoommer -> toInitialPosition ();
}

// ---
void QGAMES::Camera::setStatus (const QGAMES::Camera::Status& st)
{
	setPosition (st._position);

	setOrientation (st._orientation, st._horizontalVector);

	_status._yawAngle = st._yawAngle;
	_status._pitchAngle = st._pitchAngle;
	_status._rollAngle = st._rollAngle;

	_status._screenRatio = st._screenRatio;
	_status._conicLimitZ = st._conicLimitZ;
	_status._eulerMatrix = st._eulerMatrix;
}

// ---
QGAMES::Positions QGAMES::Camera::justProject (const QGAMES::Positions& ps)
{
	// Lambda function to project a single point...
	auto projectPointFunction = [this](QGAMES::Position& p)
		{
			switch (_projectionType)
			{
				case QGAMES::Camera::ProjectionType::_ORTHOGONAL:
					p = QGAMES::Position (p.posX (), p.posY (), __BD 0);
					break;

				case QGAMES::Camera::ProjectionType::_DIEDRIC:
					p = QGAMES::Position (
						((p.posX () - p.posY ()) * __BD 0.8660254037844386), // SQRT(3)/2
						((p.posX () + p.posY ()) / __BD 2) + p.posZ (),
						__BD 0);
					break;

				case QGAMES::Camera::ProjectionType::_QUICKDIEDRIC:
					p = QGAMES::Position (
						(p.posX () - p.posY ()),
						((p.posX () + p.posY ()) / __BD 2) + p.posZ (),
						__BD 0);
					break;

				case QGAMES::Camera::ProjectionType::_CAVALIER:
					p = QGAMES::Position (
						(p.posX () - (p.posY () * __BD 0.7071067811865475)), // SQRT(2)/2
						(p.posY () * __BD 0.70710678118654752) + p.posZ (),
						__BD 0);
					break;

				case QGAMES::Camera::ProjectionType::_QUICKCAVALIER:
					p = QGAMES::Position (
						(p.posX () - (p.posY ()) / __BD 2),
						(p.posY () / __BD 2) + p.posZ (),
						__BD 0);
					break;

				case QGAMES::Camera::ProjectionType::_CONIC:
					p = (p.posZ () != _status._conicLimitZ) // Is this just in the plane of the point of view... 
						? (__BD _status._maximumWidth / (_status._maximumWidth + (__BD 2 * p.posZ ()))) * 
							QGAMES::Position (
								(p.posX () + p.posZ ()),
								(p.posY () + (_status._screenRatio * p.posZ ())),
								__BD 0)
						: QGAMES::Position::_noPoint; // ...the point is not possible...
					break;

				default:
					assert (0); // It shouldn't be here...
			}
		};

	// The funciton to limit the points of a line to the maximum projectable ones...
	// The lambda function returns whether any point has or not been limited!
	// This lambda function is only invoke for conic projections
	auto limitLinePoints = [this](QGAMES::Position& p1, QGAMES::Position& p2) -> bool
		{
			bool result = true;

			bool p1V = p1.posZ () <= _status._conicLimitZ;
			bool p2V = p2.posZ () <= _status._conicLimitZ;

			// Both are out of the limts...
			if (p1V && p2V)
				p1 = p2 = QGAMES::Position::_noPoint; // Not visible...
			else
			// One them only is out of the limits
			if (p1V || p2V)
			{
				QGAMES::Position& pOut = p1V ? p1 : p2;
				QGAMES::Position& pIn  = p1V ? p2 : p1; // The other...

				QGAMES::bdata mW2 = __BD _status._maximumWidth / __BD 2;
				// 1 left, 2 right, 3 vertical...
				unsigned left = pOut.posX () < mW2
					? 1
					: ((pOut.posX () > mW2)
						? 2
						: (pIn.posX () > mW2) ? 1 : ((pIn.posX () < mW2) ? 2 : 3));

				QGAMES::bdata mH2 = __BD _status._maximumHeight / __BD 2;
				// 1 up, 2 down, 3 horizontal...
				unsigned up = pOut.posY () < mH2
					? 1
					: ((pOut.posY () > mH2)
						? 2
						: (pIn.posY () > mH2) ? 1 : ((pIn.posY () < mH2) ? 2 : 3));

				// If the projetion is vertical and horizontal at the same time
				// then the line is perpendicular to the projection plane
				// and going through the centrer where the vision point is projected too...
				if (left == 3 && up == 3)
					pOut = QGAMES::Position (mW2, mH2, _status._conicLimitZ); // So the last point visible should be the limit...
				else
				{
					QGAMES::bdata tx = __BD 1;
					QGAMES::bdata ty = __BD 1;

					if (left != 3)
						tx = (left == 1) // left?
							? (-__BD _status._maximumWidth * (pOut.posX () + pOut.posZ ())) / 
								((__BD 2 * (pIn.posX () - pOut.posX ())) + (_status._maximumWidth * (pIn.posZ () - pOut.posZ ()))) 
							: ((__BD _status._maximumWidth - pOut.posX ()) + pOut.posZ ()) / 
								((pIn.posX () - pOut.posX ()) - (pIn.posZ () - pOut.posZ ()));
					if (up != 3)
						ty = (up == 1) // up?
							? (-__BD _status._maximumWidth * (pOut.posY () + (pOut.posZ () / _status._screenRatio))) / 
								((__BD 2 * (pIn.posY () - pOut.posY ())) + ((__BD _status._maximumWidth / _status._screenRatio) * (pIn.posZ () - pOut.posZ ())))
							: (((__BD _status._maximumWidth / _status._screenRatio) - pOut.posY ()) + (pOut.posZ () / _status._screenRatio)) / 
								((pIn.posY () - pOut.posY ()) - ((__BD 1 / _status._screenRatio) * (pIn.posZ () - pOut.posZ ()))); 

					assert (tx != __BD 0 && ty != __BD 0);
					QGAMES::bdata pztx = pOut.posZ () + ((pIn.posZ () - pOut.posZ ()) * tx);
					if (pztx < _status._conicLimitZ) pztx = std::numeric_limits <QGAMES::bdata> ().max ();
					QGAMES::bdata pzty = pOut.posZ () + ((pIn.posZ () - pOut.posZ ()) * ty);
					if (pzty < _status._conicLimitZ) pzty = std::numeric_limits <QGAMES::bdata> ().max ();
					assert (pztx != std::numeric_limits <QGAMES::bdata> ().max () || pzty != std::numeric_limits <QGAMES::bdata> ().max ());
					pOut = (pztx < pzty)
						? QGAMES::Position (__BD pOut.posX () + ((pIn.posX () - pOut.posX ()) * tx), 
											__BD pOut.posY () + ((pIn.posY () - pOut.posY ()) * tx), 
											pztx)
						: QGAMES::Position (__BD pOut.posX () + ((pIn.posX () - pOut.posX ()) * ty), 
											__BD pOut.posY () + ((pIn.posY () - pOut.posY ()) * ty), 
											pzty);
				}
			}
			else
				result = false;

			return (result);
		};

	// Project the points over the projection plane...
	// The CONIC type of projection has specific things...
	int numPts = (int) ps.size ();
	QGAMES::Positions result; 
	if (_projectionType != QGAMES::Camera::ProjectionType::_CONIC ||
		(_projectionType == QGAMES::Camera::ProjectionType::_CONIC && numPts == 1))
	{
		result.resize (numPts);
		for (int i = 0; i < numPts; i++)
			projectPointFunction (result [i] = (ps [i] - _status._position).transformWith (_status._eulerMatrix));
	}
	// This is for the conic projection with two or more points (lines, triangles,...polygons in general)
	else
	{
		// Is this a line?
		if (numPts == 2)
		{
			result.resize (numPts);

			// Cut the poins is needed...
			limitLinePoints (
				result [0] = (ps [0] - _status._position).transformWith (_status._eulerMatrix), 
				result [1] = (ps [1] - _status._position).transformWith (_status._eulerMatrix));

			if (result [0] != QGAMES::Position::_noPoint && 
				result [1] != QGAMES::Position::_noPoint) 
				// When one of them is noPoint the other one is as well! (but just in case)
			{
				projectPointFunction (result [0]); 
				projectPointFunction (result [1]);
			}
			else
				result = QGAMES::Positions (); // None is visible...
		}

		// It is a poligon: triangle, rectangle,...
		else
		{
			// First, change the reference system
			// Bringing all points to the ubication of the camera
			QGAMES::Positions pResult (numPts);
			for (int i = 0; i < numPts; i++)
				pResult [i] = (ps [i] - _status._position).transformWith (_status._eulerMatrix);

			// Then, creates different lines per pair of points limiting them if needed
			QGAMES::Positions ppResult (numPts << 1 );
			for (int i = 0; i < (numPts - 1); i++)
			{ 
				ppResult [i << 1] = pResult [i]; ppResult [(i << 1) + 1] = pResult [i + 1]; 
				limitLinePoints (ppResult [i << 1], ppResult [(i << 1) + 1]);
			}

			// The last line to create should link the last point with the first one!
			ppResult [(numPts - 1) << 1] = pResult [numPts - 1];
			ppResult [((numPts - 1) << 1) + 1] = pResult [0]; // the first
			limitLinePoints (ppResult [(numPts - 1) << 1], ppResult [((numPts - 1) << 1) + 1]);

			// Then, remove all no visible points! (if a line is totally out of view, both limited points will be _noPoints)
			ppResult.erase (std::remove (ppResult.begin (), ppResult.end (), QGAMES::Position::_noPoint), ppResult.end ());
			// Here the size of ppResult is not necessary numPts * 2... (the same or less)
			// It could even be 0 when all points were out of the visualization!

			// Now rebuild back the list of points deleting the one that are repited
			if (!ppResult.empty ())
			{
				result.push_back (ppResult [0]);
				for (int i = 1; i < (int) ppResult.size (); i++)
				{
					if ((i == (int) (ppResult.size () - 1) && ppResult [i] != ppResult [0]) ||
						(i != (int) (ppResult.size () - 1) && ppResult [i] != ppResult [i - 1]))
							result.push_back (ppResult [i]);
				}
			}

			// and finally, project them (if any)
			for (int i = 0; i < (int) result.size (); projectPointFunction (result [i++]));
		}
	}

	return (result);
}

// ---
QGAMES::Positions QGAMES::Camera::justScale (const QGAMES::Positions& ps)
{
	if (!_zoommer)
		return (ps);

	/** No effect over z coordinate. */
	QGAMES::Positions result;
	for (int i = 0; i < (int) ps.size (); i++)
		result.push_back (QGAMES::Position 
			(_zoommer -> effectOverX (ps [i].posX ()), _zoommer -> effectOverY (ps [i].posY ()), ps [i].posZ ()));

	return (result);
}

// ---
QGAMES::VisualCloudPoints QGAMES::Camera::convertPositionsIntoVisualCloudPoints (const QGAMES::Positions& pos)
{
	// One line is visible if the distance between the center of the screen and the line itself is less than the
	// maximum distance visible in the screen (it will also depend whether the screen is vertical or horizontal)
	bool oneVisible = false;
	QGAMES::Position centre = QGAMES::Position 
		(__BD _status._maximumWidth, __BD _status._maximumHeight, __BD 0 /** They all are already screen points */) / 2;
	QGAMES::bdata mD = __BD (_status._maximumWidth * _status._maximumWidth) + 
						__BD (_status._maximumHeight * _status._maximumHeight);
	for (int i = 0; i < ((int) pos.size () - 1) && !oneVisible; i++)
	{
		QGAMES::Vector vR = (pos [i + 1] - pos [i]).normalize ();
		if ((pos [i + 1] - (((pos [i + 1] - centre).dotProduct (vR)) * vR) - centre).module2 () <= mD)
			oneVisible = true;
	}

	// It doesn't matter whethere the poinst are or not into the limits of the screen.
	// All of them are converted actually, but the attribute _areVisible will be set to false in that situation!
	QGAMES::VisualCloudPoints result;
	result._areVisible = oneVisible;
	for (int i = 0; i < (int) pos.size (); i++)
		result._points.push_back (QGAMES::ScrCoordinates ((int) pos [i].posX (), (int) pos [i].posY ()));

	return (result); 
}

// ---
void QGAMES::CameraControlledByInputHandler::setInputHandler (QGAMES::InputHandler* iH)
{
	// Input handler can be null...

	if (_inputHandler != NULL)
		unObserve (_inputHandler);
	_inputHandler = iH;
	if (_inputHandler != NULL)
		observe (_inputHandler);
}

// ---
void QGAMES::CameraControlledByInputHandler::processEvent (const QGAMES::Event& e)
{
	// Depending on the type of event oe method or other is executed!
	switch (e.code ())
	{
		case __QGAMES_MOVEJOYSTICKEVENT__:
			onJoystickAxisMoveEvent ((QGAMES::JoystickMovementEventData*) e.data ());
			break;

		case __QGAMES_BUTTONJOYSTICKEVENT__:
			onJoystickButtonEvent ((QGAMES::JoystickButtonEventData*) e.data ());
			break;

		case __QGAMES_KEYBOARDEVENT__:
			onKeyboardEvent ((QGAMES::KeyBoardEventData*) e.data ());
			break;

		case __QGAMES_MOUSEMOVEMENTEVENT__:
			onMouseMovementEvent ((QGAMES::MouseMovementEventData*) e.data ());
			break;

		case __QGAMES_MOUSEBUTTONEVENT__:
			onMouseButtonEvent ((QGAMES::MouseButtonEventData*) e.data ());
			break;

		default:
			break;
	}
}

// ---
void QGAMES::FreeMovementCamera::manualStop ()
{
	_goingForward = false;
	_goingBack = false;
	_goingLeft = false;
	_goingRight = false;
	_goingUp = false;
	_goingDown = false;
	_velocity = Vector::_cero;
}

// ---
void QGAMES::FreeMovementCamera::updatePositions ()
{
	// Build our acceleration vector based on keyboard input composite
	QGAMES::Vector accel = QGAMES::Vector::_cero;
	if (_goingForward) accel += frontDirection ();
	if (_goingBack) accel += backDirection ();
	if (_goingRight) accel += rightDirection ();
	if (_goingLeft) accel += leftDirection ();
	if (_goingUp) accel += upDirection ();
	if (_goingDown) accel += downDirection ();

	// If accelerating, try to reach top speed in a certain time...
	QGAMES::bdata tSpd = _fastMove ? (_topSpeed * __BD 20) : _topSpeed;
	if (accel.module2 () != __BD 0)
		_velocity += accel * tSpd * _timeBetweenTwoFrames * 10;
	// ...If not accelerating, try to stop in a certain time
	else _velocity -= _velocity * _timeBetweenTwoFrames * 10;
	
	// Keep camera velocity below top speed and above the minimum...
	if (_velocity.module () > tSpd)
		{ _velocity.normalize (); _velocity *= tSpd; }
	else 
	if (_velocity.module () < QGAMES::__QGAMES_ERROR)
		_velocity = QGAMES::Vector::_cero;

	// ...and finally, if the velocity is not cero, then moves the camera...
	if (_velocity != QGAMES::Vector::_cero)
		setPosition (position () + (_velocity * _timeBetweenTwoFrames));
}

// ---
void QGAMES::FreeMovementCamera::onKeyboardEvent (QGAMES::KeyBoardEventData* dt)
{
	assert (dt);

	if (dt -> _internalCode == _keys._forward) _goingForward = dt -> _on;
	else if (dt -> _internalCode == _keys._back) _goingBack = dt -> _on;
	else if (dt -> _internalCode == _keys._left) _goingLeft = dt -> _on;
	else if (dt -> _internalCode == _keys._right) _goingRight = dt -> _on;
	else if (dt -> _internalCode == _keys._up) _goingUp = dt -> _on;
	else if (dt -> _internalCode == _keys._down) _goingDown = dt -> _on;
	else if (dt -> _internalCode == _keys._fast) _fastMove = dt -> _on;
}

// ---
void QGAMES::FreeMovementCamera::onMouseMovementEvent (QGAMES::MouseMovementEventData* dt)
{
	assert (dt);

	// Get the statuus of the mouse because 
	// Not only the x position but the y is going to be treated!
	QGAMES::MouseStateData st = inputHandler () -> mouseState ();

	// If something has changed, then take it into account...
	if (_lastMX != -1 && _lastMY != -1)
	{
		if (_lastMX != st._X._abs || _lastMY != st._Y._abs)
		{
			yaw (__BD (_lastMX - st._X._abs) * _angleRotated * __PI / __BD 180);
			pitch (-__BD (_lastMY - st._Y._abs) * _angleRotated * __PI / __BD 180);
		}
	}

	_lastMX = st._X._abs; _lastMY = st._Y._abs;
}

// ---
void QGAMES::Screen::Transaction::open ()
{
	(_numberOfTx++ == 0) ? openFirst () : openOne (); 
}

// ---
void QGAMES::Screen::Transaction::close ()
{
	(--_numberOfTx == 0) ? closeLast () : closeOne ();
}

// ---
QGAMES::Screen::ToFixScreen::ToFixScreen (QGAMES::Screen* s)
	: _screen (s)
{
	assert (s);
}

const int QGAMES::Screen::Buffer::_PUREBLENDMODE = 1;
const int QGAMES::Screen::Buffer::_ADDBLENDMODE = 2;
const int QGAMES::Screen::Buffer::_MODBLENDMODE = 3;
const int QGAMES::Screen::Buffer::_NOBLENDMODE = 0;

// ---
QGAMES::Screen::Buffer::Buffer (QGAMES::Screen* s, int w, int h)
	: _screen (s),
	  _width (w),
	  _height (h)
{ 
	assert (s);

	if (_width == -1)
		_width = _screen -> visualWidth ();
	if (_height == -1)
		_height = _screen -> visualHeight ();

	assert (_width > 0 && _height > 0);
}

// ---
QGAMES::Screen::Screen (const std::string& t, int w, int h, const QGAMES::MapOfCameras& cms)
	  : _title (t),
		_visualWidth (w),
		_visualHeight (h),
		_fix (NULL),
		_cameras (cms), // This list has to be fulfilled first time a drawn is tried...
		_currentCamera (NULL),
		_transaction (NULL)
{
	// None of both can be null, otherwise it will sure crash at some point later!
	assert ((_visualWidth > 0) && (_visualHeight > 0)); 

	// Adds a default camera for simple purposes.
	// This camera is used e.g to draw the score objects.
	addCamera (new StaticOrthogonalDefaultCamera (w, h));

	// If the list of cameras received is not empty
	// Selects the first as current camera and adjust their limits to the screen...
	if (!_cameras.empty ())
	{
		_currentCamera = (*_cameras.begin ()).second;
		_currentCamera -> adjustLimitsTo (this);
	}
}

// ---
QGAMES::Screen::~Screen ()
{ 
	delete (_fix);
	_fix = NULL;

	removeAllCameras ();
}

// ---
bool QGAMES::Screen::existsCamera (const std::string& id) const
{
	QGAMES::MapOfCameras::const_iterator i = _cameras.find (id);
	return (i != _cameras.end ());
}

// ---
QGAMES::Camera* QGAMES::Screen::camera (const std::string& id)
{
	QGAMES::MapOfCameras::const_iterator i = _cameras.find (id);
	return ((i != _cameras.end ()) ? (*i).second : NULL);
}

// ---
void QGAMES::Screen::addCamera (QGAMES::Camera* c)
{
	assert (c);

	QGAMES::MapOfCameras::const_iterator i = _cameras.find (c -> name ());
	if (i != _cameras.end ()) return;
	_cameras.insert (QGAMES::MapOfCameras::value_type (c -> name (), c));
}

// ---
void QGAMES::Screen::removeCamera (const std::string& id)
{
	QGAMES::MapOfCameras::const_iterator i = _cameras.find (id);
	assert (i != _cameras.end ());

	_cameras.erase (i); // Deletes the node...
}

// ---
void QGAMES::Screen::removeAllCameras ()
{
	for (QGAMES::MapOfCameras::const_iterator i = _cameras.begin (); i != _cameras.end (); i++)
		delete ((*i).second);
	_cameras.clear ();
	_currentCamera = NULL; // None active...
}

// ---
void QGAMES::Screen::setCurrentCamera (QGAMES::Camera* c)
{
	if (c != NULL && _cameras.find (c -> name ()) == _cameras.end ())
		return; 
	_currentCamera = c; // Can't be null...
}

// ---
const QGAMES::Position& QGAMES::Screen::position ()
{
	QGAMES::Camera* cTD = _currentCamera ? _currentCamera : defaultCamera ();
	return (cTD ? cTD -> position () : Position::_noPoint); 
}

// ---
void QGAMES::Screen::setPosition (const Position& p)
{ 
	QGAMES::Camera* cTD = _currentCamera ? _currentCamera : defaultCamera ();
	if (cTD) 
		cTD -> setPosition (p); 
}

// ---
void QGAMES::Screen::move (const QGAMES::Position& p)
{ 
	QGAMES::Camera* cTD = _currentCamera ? _currentCamera : defaultCamera ();
	if (cTD) 
		cTD -> setPosition (_currentCamera -> position ().plus (p)); 
}

// ---
const QGAMES::Vector& QGAMES::Screen::orientation ()
{ 
	QGAMES::Camera* cTD = _currentCamera ? _currentCamera : defaultCamera ();
	return (cTD ? cTD -> orientation () : Vector::_noPoint); 
}

// ---
void QGAMES::Screen::rotateAround (const QGAMES::Vector& n, const QGAMES::Position& p, QGAMES::bdata a)
{
	QGAMES::Camera* cTD = _currentCamera ? _currentCamera : defaultCamera ();
	if (cTD) 
		cTD -> rotateAround (n, p, a); 
}

// ---
void QGAMES::Screen::yaw (QGAMES::bdata a)
{
	QGAMES::Camera* cTD = _currentCamera ? _currentCamera : defaultCamera ();
	if (cTD) 
		cTD -> yaw (a); 
}

// ---
void QGAMES::Screen::pitch (QGAMES::bdata a)
{
	QGAMES::Camera* cTD = _currentCamera ? _currentCamera : defaultCamera ();
	if (cTD) 
		cTD -> pitch (a); 
}

// ---
void QGAMES::Screen::rotate (QGAMES::bdata a)
{
	QGAMES::Camera* cTD = _currentCamera ? _currentCamera : defaultCamera ();
	if (cTD) 
		cTD -> rotate (a); 
}

// ---
void QGAMES::Screen::yawPithRotate (QGAMES::bdata y, QGAMES::bdata p, QGAMES::bdata r)
{
	QGAMES::Camera* cTD = _currentCamera ? _currentCamera : defaultCamera ();
	if (cTD) 
		{ cTD -> yaw (y); cTD -> pitch (p); cTD -> rotate (r); }
}

// ---
bool QGAMES::Screen::isVisible (const QGAMES::Position& p)
{
	return (!_currentCamera ? false : _currentCamera -> calculateVisualPositions (p)._areVisible);
}

// ---
bool QGAMES::Screen::isVisible (const QGAMES::Rectangle& r)
{
	return (isVisible (r.pos1 ()) || isVisible (r.pos2 ()) ||
			isVisible (r.pos3 ()) || isVisible (r.pos4 ()));
}

// ---
void QGAMES::Screen::updatePositions ()
{
	if (_fix != NULL)
		_fix -> moveScreen (); // Move the screen when something has been defined...
	for (QGAMES::MapOfCameras::const_iterator i = _cameras.begin (); i != _cameras.end (); i++)
		(*i).second -> updatePositions (); // The cameras can be updated...
}

// ---
void QGAMES::Screen::openTransaction ()
{
	if (_transaction == NULL)
		_transaction = createTransaction ();
	assert (_transaction); // The create transaction is an user's method...

	_transaction -> open ();
}

// ---
QGAMES::Screen::Buffer* QGAMES::Screen::openBuffer (int tp, int w, int h)
{
	QGAMES::Screen::Buffer* result = createBuffer (tp, w, h);
	result -> open (); // It is initialized...
	return (result);
}

// ---
void QGAMES::Screen::drawForm (const QGAMES::Position& p, QGAMES::Form* f, 
	int px, int py, int w, int h, int a)
{
	assert (f);

	if (!_currentCamera)
		return;

	int wD = w; int hD = h;
	QGAMES::Positions pts = _currentCamera -> justProject (p);
	if ((int) pts [0].posX () > _visualWidth || (int) pts [0].posY () > _visualHeight ||
		((int) pts [0].posX () + wD) < 0 || ((int) pts [0].posY () + hD) < 0)
		return; // If the image is not in thelimits, then it isn't drawn...

	QGAMES::VisualCloudPoints vPts = 
		_currentCamera -> convertPositionsIntoVisualCloudPoints (_currentCamera -> justScale (pts));
	QGAMES::ZoommerOnScreen* zm = _currentCamera -> zoommer ();
	if (zm != NULL && !zm -> useSoftware ()) // If there is zoommer declared, and by internal then adjust the scale!
		{ wD = (int) (wD * zm -> value ()); hD = (int) (hD * zm -> value ()); }

	_currentCamera -> beforeDrawing ();
	drawForm (vPts._points [0]._x, vPts._points [0]._y, f, px, py, wD, hD, a);
	_currentCamera -> afterDrawing ();
}

// ---
void QGAMES::Screen::drawForm (const QGAMES::Position& p, int oX, int oY, 
	QGAMES::Form* f, int px, int py, int w, int h, int a)
{
	assert (f);

	if (!_currentCamera)
		return;

	int wD = w; int hD = h;

	QGAMES::Positions pts = _currentCamera -> justProject (p);
	if ((int) pts [0].posX () > _visualWidth || (int) pts [0].posY () > _visualHeight ||
		((int) pts [0].posX () + wD + oX) < 0 || ((int) pts [0].posY () + hD + oY) < 0)
		return; // If the image is not in thelimits, then it isn't drawn...

	QGAMES::VisualCloudPoints vPts = 
		_currentCamera -> convertPositionsIntoVisualCloudPoints (_currentCamera -> justScale (pts));
	QGAMES::ZoommerOnScreen* zm = _currentCamera -> zoommer ();
	if (zm != NULL && !zm -> useSoftware ()) // If there is zoommer declared, and by internal then adjust the scale!
		{ wD = (int) (wD * zm -> value ()); hD = (int) (hD * zm -> value ()); }

	_currentCamera -> beforeDrawing ();
	drawForm (vPts._points [0]._x + oX, vPts._points [0]._y + oY, f, px, py, wD, hD, a);
	_currentCamera -> afterDrawing ();
}

// ---
void QGAMES::Screen::drawPoint (const QGAMES::Position& p, const QGAMES::Color& c)
{
	if (!_currentCamera)
		return;

	QGAMES::VisualCloudPoints pts = _currentCamera -> calculateVisualPositions (p);
	if (!pts._areVisible)
		return; // If the figure delimited by the points is not going to be visible, 
				// then nothing to do!

	_currentCamera -> beforeDrawing ();

	drawPoint (pts._points [0]._x, pts._points [0]._y, c);

	_currentCamera -> afterDrawing ();
}

// ---
void QGAMES::Screen::drawLine (const QGAMES::Position& p1, const QGAMES::Position& p2, 
		const QGAMES::Color& c, int t)
{
	if (!_currentCamera)
		return;

	QGAMES::VisualCloudPoints pts = _currentCamera -> calculateVisualPositions (p1, p2);
	if (!pts._areVisible)
		return; // If the figure delimited by the points is not going to be visible, 
				// then nothing to do!

	_currentCamera -> beforeDrawing ();

	drawLine (pts._points [0]._x, pts._points [0]._y, pts._points [1]._x, pts._points [1]._y, c, t);

	_currentCamera -> afterDrawing ();
}

// ---
void QGAMES::Screen::drawRectangle (const QGAMES::Rectangle& r, const QGAMES::Color& c, bool f)
{
	if (!_currentCamera)
		return;

	QGAMES::VisualCloudPoints pts = 
		_currentCamera -> calculateVisualPositions (r.pos1 (), r.pos3 (), r.pos2 (), r.pos4 ()); 
	// Notice that the positions are passed in order (important) following clockwise orientation
	// ...and not in the way the are stored!
	if (!pts._areVisible)
		return; // If the figure delimited by the points is not going to be visible, 
				// then nothing to do!

	_currentCamera -> beforeDrawing ();

	// A rectangle can become a polygon in any type of projection type...
	// and almost under any type of circunstance (except if it has been rotated in a orthogonal projection)!
	// Even the number of the points of the rectangle can be greater than 4 (conic projection when the rectangle is limited)
	drawPolygonFromScreenCoordinates (pts._points, c, f);
	
	_currentCamera -> afterDrawing ();
}

// ---
void QGAMES::Screen::drawRoundRectangle (const QGAMES::Rectangle& r, int rdx, const QGAMES::Color& c, bool f)
{
	if (!_currentCamera)
		return;

	QGAMES::VisualCloudPoints pts = 
		_currentCamera -> calculateVisualPositions (r.pos1 (), r.pos3 (), r.pos2 (), r.pos4 ()); 
	if (!pts._areVisible)
		return; // If the figure delimited by the points is not going to be visible, 
				// then nothing to do!

	// Can only be rounded when the proyection is orthogonal 
	// and the projection hasn't been deformed at all!
	// Otherwise la proyection is like the previous one (at least we find another solution!)
	_currentCamera -> beforeDrawing ();

	if (pts._points.size () == 4 && _currentCamera -> projectionType () == QGAMES::Camera::ProjectionType::_ORTHOGONAL &&
		pts._points [0]._x == pts._points [3]._x && pts._points [1]._x == pts._points [2]._x &&
		pts._points [0]._y == pts._points [1]._y && pts._points [2]._y == pts._points [3]._y)
		drawRoundRectangle (pts._points [0]._x, pts._points [0]._y,
							pts._points [2]._x, pts._points [2]._y, rdx, c, f); 
	else
		drawPolygonFromScreenCoordinates (pts._points, c, f);

	_currentCamera -> afterDrawing ();
}

// ---
void QGAMES::Screen::drawBox (const QGAMES::Box& b, const QGAMES::Color& c, bool f)
{
	if (!_currentCamera)
		return; // To avoid further calculus if needed...

	// Draw made up of the different rectangles...
	drawRectangle (b.topRectangle (), c, f);
	drawRectangle (b.bottomRectangle (), c, f);
	drawRectangle (b.leftRectangle (), c, f);
	drawRectangle (b.rightRectangle (), c, f);
	drawRectangle (b.frontRectangle (), c, f);
	drawRectangle (b.backRectangle (), c, f);
}

// ---
void QGAMES::Screen::drawPolygon (const QGAMES::Polygon& r, const QGAMES::Color& c, bool f)
{
	if (!_currentCamera)
		return;

	QGAMES::VisualCloudPoints pts = _currentCamera -> calculateVisualPositions (r.points ());
	if (!pts._areVisible)
		return; // If the figure delimited by the points is not going to be visible, 
				// then nothing to do!

	_currentCamera -> beforeDrawing ();

	drawPolygonFromScreenCoordinates (pts._points, c, f);

	_currentCamera -> afterDrawing ();
}

// ---
void QGAMES::Screen::drawTriangle (const QGAMES::Position& p1, const QGAMES::Position& p2, 
		const QGAMES::Position& p3, const QGAMES::Color& c, bool f)
{
	if (!_currentCamera)
		return;

	QGAMES::VisualCloudPoints pts = _currentCamera -> calculateVisualPositions (p1, p2, p3);
	// The order of the points in the case of a triangle is not neccessary so important
	// Any pair the points always form a line...
	if (!pts._areVisible)
		return; // If the figure delimited by the points is not going to be visible, 
				// then nothing to do!

	_currentCamera -> beforeDrawing ();

	// Projecting a triangle in the screen is mainly another triangle except when it is limited
	// That the number of point can be greater than 3 (even 5)!
	drawPolygonFromScreenCoordinates (pts._points, c, f);

	_currentCamera -> afterDrawing ();
}

// ---
void QGAMES::Screen::drawImage (const QGAMES::Position& p, const std::string& img, int a)
{
	if (!_currentCamera)
		return;

	QGAMES::Positions pts = _currentCamera -> justProject (p);
	if ((int) pts [0].posX () > _visualWidth || (int) pts [0].posY () > _visualHeight)
		return; // If the image is not in the window, it is not drawn...
	QGAMES::VisualCloudPoints vPts = 
		_currentCamera -> convertPositionsIntoVisualCloudPoints (_currentCamera -> justScale (pts));

	_currentCamera -> beforeDrawing ();

	drawImage (vPts._points [0]._x, vPts._points [0]._y, img, a);

	_currentCamera -> afterDrawing ();
}

// ---
void QGAMES::Screen::drawCircle (const QGAMES::Position& p, const QGAMES::Vector& o, 
	QGAMES::bdata a1, QGAMES::bdata a2, const QGAMES::Color& c, bool f)
{
	if (!_currentCamera)
		return;

	QGAMES::bdata a1D = a1; QGAMES::bdata a2D = a2;
	QGAMES::VisualCloudPoints pts = _currentCamera -> calculateVisualPositions (p); 
	QGAMES::ZoommerOnScreen* zm = _currentCamera -> zoommer ();
	if (zm != NULL && !zm -> useSoftware ()) // If there is zoommer declared, and by algorithm, then adjust the scale!
		{ a1D *= zm -> value (); a2D *= zm -> value (); }

	_currentCamera -> beforeDrawing ();

	drawCircle (pts._points [0]._x, pts._points [0]._y, (int) a1D, (int) a2D, c, f);

	_currentCamera -> afterDrawing ();
}

// ---
void QGAMES::Screen::drawText (const QGAMES::Position& p, const std::string& s,
	 const std::string& f, int w, int h, const QGAMES::Color& c, int lt, int ot)
{
	if (!_currentCamera)
		return;

	int wD = w; int hD = h;
	QGAMES::VisualCloudPoints pts = _currentCamera -> calculateVisualPositions (p); 
	QGAMES::ZoommerOnScreen* zm = _currentCamera -> zoommer ();
	if (zm != NULL && !zm -> useSoftware ()) // If there is zoommer declared, and by algorithm, then adjust the scale!
		{ wD = (int) (wD * zm -> value ()); hD = (int) (hD * zm -> value ()); }

	_currentCamera -> beforeDrawing ();

	drawText (pts._points [0]._x, pts._points [0]._y, s, f,  wD, hD, c, lt, ot);

	_currentCamera -> afterDrawing ();
}

// ---
void QGAMES::Screen::closeTransaction ()
{
	if (_transaction == NULL)
		return;

	_transaction -> close ();
	if (_transaction -> numberOfTx () == 0)
	{
		delete (_transaction);
		_transaction = NULL;
	}
}

// ---
void QGAMES::Screen::closeBuffer (QGAMES::Screen::Buffer* b)
{
	assert (b);

	b -> close ();
}

// ---
void QGAMES::Screen::drawPolygonFromScreenCoordinates (const QGAMES::ListOfScrCoordinates& ps, const QGAMES::Color& c, bool f)
{
	int nPts = (int) ps.size ();
	int* x = new int [nPts]; 
	int* y = new int [nPts];
	for (int i = 0; i < nPts; i++)
		{ x [i] = ps [i]._x; y [i] = ps [i]._y; }

	drawPolygon (x, y, nPts, c, f);

	delete [] x; delete [] y;
}
