/** \ingroup Game */
/** \ingroup Screen */
/*@{*/

/**	
 *	@file	
 *	File: screen.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines the screen where everything will be drawn. \n
 *	Versions: 1.0 Initial
 *			  2.0 Major change. Screen position and orientation is now controlled by cameras,
 *			  to align the design to what other rendering engines offer. \n
 *			  The cameras are now in charge of controlling even the type of projection: ISOMETRIC,...
 */

#ifndef __QGAMES_SCREEN__
#define __QGAMES_SCREEN__

#include <Common/position.hpp>
#include <Common/rectangle.hpp>
#include <Common/box.hpp>
#include <Common/color.hpp>
#include <Common/inputhandler.hpp>
#include <Common/notifierobserver.hpp>
#include <map>

namespace QGAMES
{
	class Form;
	class Screen;

	/** This class represents a pair of coordinates in a visual screen. \n
		The coordinates in a visual screen have always x and y value only. \n
		When a point in the space is represented on a screen there are only x and y values and
		as they are pixels int type is used. */
	struct ScrCoordinates
	{
		/** A default constructor is declared to be sure the coordinates are set to 0. */
		ScrCoordinates ()
			: _x (0), _y (0) { }
		/** And a constructor for basic data. */
		ScrCoordinates (int x, int y)
			: _x (x), _y (y) { }

		int _x, _y;
	};

	/** To simplify how to manage a list of screen coordinates. */
	typedef std::vector <ScrCoordinates> ListOfScrCoordinates;

	/** A structure to represent a list of visual screen coordinates. \n
		Apart of the list itself, this class defines an internal attribute
		to indicate whether the list is or not visible in the limits
		of the screen using them. This last information can be used by other methods
		to determinate whether the points are drawn eventually. */
	struct VisualCloudPoints
	{
		/** A default constructor is defined to set the internal variables 
			to the right default value. */
		VisualCloudPoints ()
			: _areVisible (false), _points () { }
		/** And a normal constructor too. */
		VisualCloudPoints (bool v, const ListOfScrCoordinates& lC)
			: _areVisible (v), _points (lC) { }

		bool _areVisible;
		ListOfScrCoordinates _points;
	};

	/** A class to represent zoom effects over a coordinate in the screen. \n
		When a value is zoomed three options can be taken into account: \n
		1.- To maintain the point in the same "square" of the camera. \n
		2.- To maintain it at the left up corner of it or 
		2.- To do same in the down right corner. */
	class ZoommerOnScreen
	{
		public:
		/** The possible effects. */
		enum Effect
		{
			_CENTER = 0,
			_TOUPLEFT = 1,
			_TODOWNRIGHT = 2
		};

		/** To store the internal status of the zoommer. */
		struct Status
		{
			Status (bdata v, int mW, int mH, Effect e, const Vector& sV)
				: _value (v),
				  _maximumWidth (mW), _maximumHeight (mH),
				  _effect (e),
				  _scaleVector (sV)
							{ }

			/** The value of the zoom itself. */
			bdata _value;
			/** Effect of the zoom. Teh refecete point actually. */
			Effect _effect;
			/** The size the rectangle where to apply the zoom in. */
			int _maximumWidth, _maximumHeight;
			/** The scale vector calculated. */
			Vector _scaleVector;
		};

		/** Constructor:
			The value of the zoom, that can't be less than 0,005 (to avoid divisions by 0),
			The parameters receives are:
			The maximum width and height of the screen, the type of effect when zoomming,
			Whether the zoom is going to be done by software (the framework behind) or
			using an algorithm implemented by QGAMES' makers. */
		ZoommerOnScreen (bdata v, int mW, int mH, Effect e = _TOUPLEFT, bool sw = false)
			: _status (v, mW, mH, e, Vector::_cero), _useSoftware (sw)
							{ calculateVectorScale (); adjustValue (); }

		/** To change the value of the zoom. */
		bdata value () const
							{ return (_status._value); }
		void setValue (bdata v)
							{ _status._value = v; calculateVectorScale (); adjustValue (); }
		/** To change the type of zoom. */
		Effect effect () const
							{ return (_status._effect); }
		void setEffect (Effect e)
							{ _status._effect = e; }

		/** To know whether software is used to make the zoomming. */
		bool useSoftware () const
							{ return (_useSoftware); }

		/** To change the limits where the zoom happens. */
		void adjustLimits (int mW, int mH)
							{ _status._maximumWidth = mW; _status._maximumHeight = mH; calculateVectorScale (); }

		/** To move the zoomer to the original position. 
			This instruction doesn't modify the dimensions the zommer is working on. */
		void toInitialPosition ();

		/** Affecting the different coordinates.
			This are the very important methods to be used. */
		bdata effectOverX (bdata v) const
							{ return ((_status._value == (bdata) 1) ? v : effectOver (v, true)); } 
		bdata effectOverY (bdata v) const
							{ return ((_status._value == (bdata) 1) ? v : effectOver (v, false)); }

		/** To get and set the internal status. */
		const Status& currentStatus () const
							{ return (_status); }
		void setStatus (const Status& st);

		/** These methods are used before and after drawing to 
			prepare the context. By default they don't do anything. 
			But usually, when the framework is used to zoom instead the internal algorithm,
			this methods are useful to activate and desactivate the framework's 
			internal zooming methods. */
		virtual void beforeDrawing () { }
		virtual void afterDrawing () { }

		protected:
		/** When zoom is done by the underlined drawing framework used,
			This method could have to to be rewritten maybe. */
		virtual bdata effectOver (bdata v, bool x = true) const;

		private:
		// Implementation
		void calculateVectorScale ();
		void adjustValue () // To avoid division by cero...
							{ if (_status._value <= (bdata) 0) _status._value = (bdata) 0.005; }

		public:
		Status _status;
		bool _useSoftware;
	};

	/** A class to represent a camera. \n
		The camera is the element used to project things on a screen. \n
		So, the camera has the intelligence to scale, zoom and move the point of view in any scene. \n
		A camera has always to be defined before drawing anything on the screen. */
	class Camera : public Observer
	{
		public:
		/** To know the very internal status of the camera.
			This structure could be used both for reading and setting back a status. */
		struct Status
		{
			Status ()
				: _position (Position::_cero),
				  _maximumWidth (-1), _maximumHeight (-1),
				  _orientation (Vector::_zNormal), _horizontalVector (Vector::_xNormal),
				  _yawAngle ((bdata) 0), _pitchAngle ((bdata) 0), _rollAngle ((bdata) 0),
				  _screenRatio ((bdata) 1), _eulerMatrix ((bdata) 1),
				  _conicLimitZ ((bdata) -100) // The very default...
							{ }

			Status (int mW, int mH, 
					const Position& p, const Vector& v, const Vector& h, 
					bdata yA, bdata pA, bdata rA,
				    bdata sR, const Matrix3& eM,
					bdata cL)
				: _position (p),
				  _maximumWidth (mW), _maximumHeight (mH),
				  _orientation (v), _horizontalVector (h),
				  _yawAngle (yA), _pitchAngle (pA), _rollAngle (rA),
				  _screenRatio (sR), _eulerMatrix (eM),
				  _conicLimitZ (cL)
							{ assert ((_maximumWidth == -1 || _maximumWidth > 0) &&
									  (_maximumHeight == -1 || _maximumHeight > 0)); }

			/** The maximum width the camera is watching. */
			int _maximumWidth;
			/** The maximum height the camera is watching. */
			int _maximumHeight;
			/** The position of the camera in the space. */
			Position _position;
			/** The direction the camera is looking to. 
				Its module should be always 1. */
			Vector _orientation;
			/** If the camara is like a rectangle. It can be horizontal respect the 
				ground or rotated. This is vector to indicate the orintation of that limit. 
				It is usefuill to calculate many other things. */
			Vector _horizontalVector;
			/** The angles the reference system is turned. */
			bdata _yawAngle, _pitchAngle, _rollAngle;
			/** The ratio of the screen. */
			bdata _screenRatio;
			/** To implement the movements in the screen. */
			Matrix3 _eulerMatrix;

			// Specific for conic projection
			bdata _conicLimitZ;
		};

		/** Sometimes the camera has an external representation.
			e.g in OGRE to manipulate this camera will imply to do things in a OGRE's camera object. \n
			This class is to define specific additional external methods when 
			the camera's position or orientation are moved. \n
			The method has to be redefined with an specific implementation if needed. */
		class ExternalLink
		{
			public:
			virtual void setPosition (const Position& pos) = 0;
			virtual void setOrientation (const Vector& o, const Vector& hv) = 0;
		};

		/** The camera can project things on the screen in 4 ways ony. */
		enum ProjectionType
		{
			// The basic one, when the distance to the focus of the camera doesn't matter.
			_ORTHOGONAL = 0,
			_DIEDRIC = 1,
			_QUICKDIEDRIC = 2,
			_CAVALIER = 3,
			_QUICKCAVALIER = 4,
			// The way 3D engines usually work.
			_CONIC = 5
		};

		/** 
		  *	To create a camera. \n
		  *	The camera is who calculates the final screen point. \n
		  *	It has to be done using a type of projection (all types possible are supported),
		  *	an  a zoommer to scale the final point. It can be null meaning no zoom is used. \n 
		  *	The camera controls also the maximum zone visualized, 
		  *	and its position and orientation in the space. \n
		  *	The position is the position of the left up corner, and the orientation is the normal vector to the rectangle
		  *	representing the camera. By default the orientation will be looking right to the front, but it can be changed
		  *	using the right method to do so. \n
		  *	To take into account in the orientation of the camera at the beginning: \n
		  *	(Z) x---------> (X) \n
		  *	    | \n
		  *	    | \n
		  * 	| \n
		  *	    | \n
		  * 	v \n
		  *	    y
		  */
		Camera (const std::string& n, ProjectionType pt, ZoommerOnScreen* z, int mW, int mH,
				const Position& pos = Position::_cero)
			  : _name (n),
				_projectionType (pt), 
				_zoommer (NULL),
				_status (mW, mH, pos, Vector::_zNormal, Vector::_xNormal, 
						 (bdata) 0, (bdata) 0, (bdata) 0,
						 (bdata) mW / (bdata) mH, Matrix3 ((bdata) 1),
						-(bdata) mW / (bdata) 2), 
				_link (NULL)
							{ setZoomer (z); }
			
		virtual ~Camera () // The camera own the zoommer and the external link. Only one zoommer per camera. 
							{ delete (_zoommer); delete (_link); }

		// The attributes...
		const std::string& name () const // Can't be changed...
							{ return (_name); }
		ZoommerOnScreen* zoommer () const
							{ return (_zoommer); }
		ProjectionType projectionType () const 
							{ return (_projectionType); }
		void setProjectType (ProjectionType pt)
							{ _projectionType = pt; }
		void setZoomer (ZoommerOnScreen* z)
							{ if ((_zoommer = z) != NULL) // The zommer has to be adjusted to the screen limits...
								_zoommer -> adjustLimits (_status._maximumWidth, _status._maximumHeight); }
		int width () const // The visual width...
							{ return (_status._maximumWidth); }
		int height () const // The visual height...
							{ return (_status._maximumHeight); }
		void adjustLimitsTo (int mW, int mH);
		void adjustLimitsTo (Screen* scr);
		Position center () const;

		/** To change the external link if needed. */
		ExternalLink* externalLink ()
							{ return (_link); }
		void setExternalLink (ExternalLink* link)
							{ _link = link; }

		// Managing the basic attributes related with the position of the camera
		const Position& position () const
							{ return (_status._position); }
		/** Sometimes the  game engines manages its own internal cameras and
			this method has to be overloaded to change engine's camera also. */
		virtual void setPosition (const Position& pos)
							{ _status._position = pos; 
							  if (_link) _link -> setPosition (pos); }
		void moveOnDirection (const Vector& d, bdata p)
							{ setPosition (position () + (Vector (d).normalize () * p)); }

		// Mamaging the attributes related with the orientation
		const Vector& orientation () const
							{ return (_status._orientation); }
		/** To get the front, back, right, left, up and down direction (normal vector) of the camera, 
			taking into account its direction. */
		Vector frontDirection () const
							{ return (Vector (_status._orientation).normalize ()); }
		Vector backDirection () const
							{ return (-frontDirection ()); }
		Vector rightDirection () const
							{ return (Vector (_status._horizontalVector).normalize ()); }
		Vector leftDirection () const
							{ return (-rightDirection ()); }
		Vector upDirection () const
							{ return (_status._horizontalVector.crossProduct (_status._orientation).normalize ()); }
		Vector downDirection () const
							{ return (-upDirection ()); }
		/** Just rotate the orientation around a point and taking into account a orientation. 
			The eurler matrix is calculated back. */
		virtual void rotateAround (const Vector& n, const Position& p, bdata a);

		/** To yaw.
			Like moving the head left and right.
			Rotating over the axis OY. */ 
		void yaw (bdata a)
							{ _status._yawAngle += a; 
							  rotateAround (_status._orientation.crossProduct (_status._horizontalVector), Position::_cero, a); }
		/** To pitch. 
			Like moving the head up and down. 
			Rotating over the axis OX. */
		void pitch (bdata a)
							{ _status._pitchAngle += a; 
							  rotateAround (_status._horizontalVector, Position::_cero, a); }
		/** To rotate around the orientation. 
			Like rotating the head left and right. 
			Rotating over the axis OZ. */
		void rotate (bdata a)
							{ _status._rollAngle += a; 
							  rotateAround (_status._orientation, Position::_cero, a); }
		/** All together. */
		void yawPithRotate (bdata y, bdata p, bdata r)
							{ yaw (y), pitch (p); rotate (r); }

		/** To move the camera to the very initial position. */
		void toInitialPosition ();

		/** To manage the internal status of the camera.
			Take care using this very internal method. */
		const Status currentStatus () const
							{ return (_status); }
		void setStatus (const Status& s);

		/** 
		  *	These all are the very important methods in the class. \n
		  *	The methods receives a set of points in the space to be fit in the zone 
		  *	visualized by the camera, and returns the position of those in the screen, 
		  *	taking into account the position and orientation of the camera and the zoom being 
		  *	used by it, and also the type of projection. \n
		  *	IMPORTANT: It is supossed the list of positions passed as parameter represents always a polygon. \n
		  *	Then the result includes always an attribute to indicate whether is possible the
		  *	polygon is in the limits or not.
		  */
		VisualCloudPoints calculateVisualPositions (const Positions& ps)
							{ return (convertPositionsIntoVisualCloudPoints (justScale (justProject (ps)))); }
		VisualCloudPoints calculateVisualPositions (const Position& p1)
							{ Positions ps (1); ps [0] = p1; 
						  return (calculateVisualPositions (ps)); }
		VisualCloudPoints calculateVisualPositions (const Position& p1, const Position& p2)
							{ Positions ps (2); ps [0] = p1; ps [1] = p2;
							  return (calculateVisualPositions (ps)); }
		VisualCloudPoints calculateVisualPositions (const Position& p1, 
			const Position& p2, const Position& p3)
							{ Positions ps (3); ps [0] = p1; ps [1] = p2; ps [2] = p3;
							  return (calculateVisualPositions (ps)); }
		VisualCloudPoints calculateVisualPositions (const Position& p1, 
			const Position& p2, const Position& p3, const Position& p4)
							{ Positions ps (4); ps [0] = p1; ps [1] = p2; ps [2] = p3; ps [3] = p4;
							  return (calculateVisualPositions (ps)); }

		/** Just to project a set of positions. 
			No scale effect is done at all after the projection. */
		Positions justProject (const Positions& ps);
		Positions justProject (const Position& p1)
							{ Positions ps (1); ps [0] = p1; 
							  return (justProject (ps)); }
		Positions justProject (const Position& p1, const Position& p2)
							{ Positions ps (2); ps [0] = p1; ps [1] = p2;
							  return (justProject (ps)); }
		Positions justProject (const Position& p1, const Position& p2, const Position& p3)
							{ Positions ps (3); ps [0] = p1; ps [1] = p2; ps [2] = p3;
							  return (justProject (ps)); }
		Positions justProject (const Position& p1, const Position& p2,
			const Position& p3, const Position& p4)
							{ Positions ps (4); ps [0] = p1; ps [1] = p2; ps [2] = p3; ps [3] = p4;
							  return (justProject (ps)); }

		/** Just to Scale a set of points. The position of the point is not change.
			The scalation is made using a screen zoomer, so it take place only in the x and y coordinates!!
			After the scalation the Z position sremains the same. */
		Positions justScale (const Positions& ps);
		Positions justScale (const Position& p1)
							{ Positions ps (1); ps [0] = p1; 
							  return (justScale (ps)); }
		Positions justScale (const Position& p1, const Position& p2)
							{ Positions ps (2); ps [0] = p1; ps [1] = p2;
							  return (justScale (ps)); }
		Positions justScale (const Position& p1, const Position& p2, const Position& p3)
							{ Positions ps (3); ps [0] = p1; ps [1] = p2; ps [2] = p3;
							  return (justScale (ps)); }
		Positions justScale (const Position& p1, const Position& p2, 
			const Position& p3, const Position& p4)
							{ Positions ps (4); ps [0] = p1; ps [1] = p2; ps [2] = p3; ps [3] = p4;
							  return (justScale (ps)); }

		/** Convert a list of positions into a list of visible cloud points. 
			IMPORTANT: To determine whethr the position is or not into the creen, the list of points
			are taken as lines of a polygon!. */
		VisualCloudPoints convertPositionsIntoVisualCloudPoints (const Positions& pos);

		/** To move the camera in every iteration of the screen...if any. 
			The method can be overloaded in any class inheriting from this one. \n
			called from the method updatePositions in the Screen. 
			By default it has nothing to do. */
		virtual void updatePositions ()
							{ }

		/** These methods are used before and after drawing to 
			prepare the context.  \n
			By default they don't do anything except calling 
			the similar method in the zoommer. */
		virtual void beforeDrawing () 
							{ if (_zoommer) _zoommer -> beforeDrawing (); }
		virtual void afterDrawing () 
							{ if (_zoommer) _zoommer -> afterDrawing (); }

		/** @see parent.
			Not needed by default o observe events. */
		virtual void processEvent (const Event& e)
							{ }

		protected:
		/** Sometimes the game engine manages its own internal camera and
			this method has to be overloaded to change that camera also.
			This method is only invoked internally. */
		virtual void setOrientation (const Vector& o, const Vector& hv)
							{ _status._orientation = o; _status._horizontalVector = hv; 
							  if (_link) _link -> setOrientation (o, hv); }

		protected:
		/** The name of the camera. */
		std::string _name;
		/** The type of projection the camera will generate. */
		ProjectionType _projectionType;
		/** The zoom effect that the camera will apply. 
			It could be NULL meaning no zoom will be apply. */
		ZoommerOnScreen* _zoommer;
		/** The internal status. */
		Status _status;
		/** A potential external reference. 
			It is null by default. */
		ExternalLink* _link;
	};

	// To simplify how to manage a map of cameras
	typedef std::map <std::string, Camera*> MapOfCameras;

	/** A default camera with no zoom effects at all.
		It is used e.g to project the score objects. Becasuse no effects are nneded at all.
		No links with anyone. No movements allowed. */
	class StaticOrthogonalDefaultCamera : public Camera
	{
		public:
		StaticOrthogonalDefaultCamera (int mW, int mH)
			: Camera (std::string (__QGAMES_DEFAULTCAMERAID__), Camera::ProjectionType::_ORTHOGONAL,
				new ZoommerOnScreen ((bdata) 1, mW, mH, ZoommerOnScreen::Effect::_TOUPLEFT, true),
				mW, mH, Position::_cero)
							{ }

		/** To avoid any kind of movement. 
			The methods are declared in the way to avoid them to be extended. */
		virtual void setPosition (const Position& pos) final override { }
		virtual void rotateAround (const Vector& n, const Position& p, bdata a) final override { }
		virtual void updatePositions () final override { }

		protected:
		virtual void setOrientation (const Vector& o, const Vector& hv) final override { }
	};

	/** 
	  *	A camera linked to the input handler. \n
	  *	The camera observes the input handler an any event that happened there
	  *	is notified to this class that reacts depending on the event. \n
	  *	This camera can be use to make FSP games e.g. \n
	  *	By default nothing is done when the event is received from the input handler. 
	  */
	class CameraControlledByInputHandler : public Camera
	{
		public:
		CameraControlledByInputHandler (const std::string& name, ProjectionType pt, ZoommerOnScreen* z,
			int mW, int mH,
			const Position& pos = Position::_cero, 
			InputHandler* iH = NULL)
				: Camera (name, pt, z, mW, mH, pos),
				  _inputHandler (NULL) // At this point it could be null...
							{ setInputHandler (iH); }

		/** To get the input handler attached...if any. */
		InputHandler* inputHandler ()
							{ return (_inputHandler); }
		void setInputHandler (InputHandler* iH); // From this point onwards it will be observed...

		/** @see parent. */
		virtual void processEvent (const Event& e);

		protected:
		// To act as the events happen...by default they don't do anything at all.
		/** When the joystick is moved. By default nothing is done. */
		virtual void onJoystickAxisMoveEvent (JoystickMovementEventData* dt) { }
		/** When the joystick button is pressed. By default nothing is done. */
		virtual void onJoystickButtonEvent (JoystickButtonEventData* dt) { }
		/** When the keyboard is used. Bu default nothing is done. */
		virtual void onKeyboardEvent (KeyBoardEventData* dt) { }
		/** When the mouse is moved. Bu default nothing is done. */
		virtual void onMouseMovementEvent (MouseMovementEventData* dt) { }
		/** When the mouse button is pressed. By default nothing is done. */
		virtual void onMouseButtonEvent (MouseButtonEventData* dt) { }

		protected:
		/** A reference to the input handler observed. */
		InputHandler* _inputHandler;
	};

	/** One of the most classic cameras in a full dimension game is the one which moves
		using the different keys and joystick defined to managed it. */
	class FreeMovementCamera : public CameraControlledByInputHandler
	{
		public:
		/** A class to configure the keys used to move the camera.
			Up, down, right and left, forward and backward, and also to speed up! */
		struct Keys
		{
			// Default constructor
			Keys ()
				: _forward (KeyCode::QGAMES_UP), _back (KeyCode::QGAMES_DOWN),
				  _left (KeyCode::QGAMES_LEFT), _right (KeyCode::QGAMES_RIGHT),
				  _up (KeyCode::QGAMES_PGUP), _down (KeyCode::QGAMES_PGDOWN),
				  _fast (KeyCode::QGAMES_LSHIFT)
								{ }

			// A constructor to change the keys...
			Keys (KeyCode f, KeyCode b, 
				  KeyCode l, KeyCode r,
				  KeyCode u, KeyCode d,
				  KeyCode ft)
				: _forward (f), _back (b), _left (l), _right (r), _up (u), _down (d), _fast (ft)
								{ }

			/** The keys used to move the camera. */
			KeyCode _forward, _back, _left, _right, _up, _down, _fast;
		};

		// The constructor...
		FreeMovementCamera (const std::string& n, const Keys& k = Keys (), 
				bdata tS = (bdata) 150, bdata aR = (bdata) 0.15f)
			: CameraControlledByInputHandler 
				(n, Camera::ProjectionType::_ORTHOGONAL, NULL, -1, -1),
			  _zooming (false),
			  _topSpeed (tS),
			  _angleRotated (aR),
			  _velocity (Vector::_cero),
			  _keys (k),
			  _timeBetweenTwoFrames ((bdata) 0),
			  _goingForward (false),
			  _goingBack (false),
			  _goingLeft (false),
			  _goingRight (false),
			  _goingUp (false),
			  _goingDown (false),
			  _fastMove (false),
			  _lastMX (-1), _lastMY (-1)
							{ }

		// Managing the speed in very momevemt...
		/** Sets the camera's top speed.
			The method is declared virtual for the possibility to add actions in specific cameras later. */
		bdata topSpeed ()
								{ return (_topSpeed); }
		virtual void setTopSpeed (bdata topSpeed)
								{ _topSpeed = topSpeed; }
		/** Manually stops the camera when in free-look mode. 
			It is also virtual to add specific actions later. */
		virtual void manualStop ();

		// Managing the angle rotated in every iteration... (in degrees)
		bdata angleRotated ()
								{ return (_angleRotated); }
		virtual void setAngleRotated (bdata aR)
								{ _angleRotated = aR; }

		/** @see parent. */
		/** Actualize the position of the camera. */
		virtual void updatePositions ();

		/** @see parent. 
			Only these two events have been parametrized. 
			For the rest of those, the default behaviour is enought. That means not doing anything. */
		virtual void onKeyboardEvent (KeyBoardEventData* dt);
		virtual void onMouseMovementEvent (MouseMovementEventData* dt);

		protected:
		/** Does the camera have to approach the target?
			Only in orbit style. */
		bool _zooming;
		/** Which is the maximum speed of the movement? */
		bdata _topSpeed;
		/** The angle rotated when used. */
		bdata _angleRotated;
		/** Is which direction? */
		Vector _velocity;
		/** The keys used to move. */
		Keys _keys;
		/** The time between two invokes to the update method. */
		bdata _timeBetweenTwoFrames;

		// Implementation
		bool _goingForward;
		bool _goingBack;
		bool _goingLeft;
		bool _goingRight;
		bool _goingUp;
		bool _goingDown;
		bool _fastMove;
		int _lastMX, _lastMY; 
	};

	/**
	 *	The screen is the place where all elements, objects, entities,...
	 *	being part of a game are drawn eventually. \n
	 *	So it defines basic instructions to draw basic elements being part of any other 
	 *	element involved in the game: forms, circles, lines,... \n
	 *	The implementation of those primitives depends on the graphical library finally used behind. \n
	 *  \n
	 *	The best way to understand how the screen class works is imaging you wear a pair of glasses. \n
	 *	The game elements passing throught the limits of the glasses are projected 
	 *	in the display. \n
	 *  \n
	 *	But what it is shown in your glasses is filmed through a camera always. \n
	 *	All things in the screen have to be drawn using a camera. \n
	 *	There have to be a default camera minimum created first time a drawing is tried. \n
	 *  \n
	 *	To draw something in the screen a graphical transaction is required. \n
	 *	Opening and closing a graphical transaction depends on the graphical library behind.
	 *  \n
	 *	A default camera is added. This camera is used to draw the score objects by default \n
	 *	but it can be also used for further purposes.
	 */
	class Screen
	{
		public:
		// Basic standard types of font supported by QGAMES library (for the method drawText)
		static const int _FONTBOLD;
		static const int _FONTITALIC;
		static const int _FONTUNDERLINE;
		static const int _FONTSTRIKETHROUGH;
		static const int _FONTNORMAL;

		/** 
		  *	This class is to fix the screen over something (the position of that "something" is kept) \n
		  *	Depending on that the screen is moved/oriented in every loop automatically. \n
		  *	So, this is not a camera, it is to follow something and move the screen automatically. \n
		  *	For instance it can be used to turn around an object or to maintain the main
		  *	character at the center of the screen. \n
		  *	At the end of the day this class uses the current camera associated to the screen. 
		  */
		class ToFixScreen
		{
			public:
			ToFixScreen (Screen* s);

			virtual ~ToFixScreen ()
							{ }

			virtual void moveScreen () = 0;

			protected:
			/** A reference to the Screen this automatic camara controller is working over.
				It can't be null. Something that is verified at construction time in debug mode. */
			Screen* _screen;
		};

		/** 
		  *	A buffer could be in some graphical systems a temporal region of the memory,
		  *	where drawing actions happen. \n
		  *	Many graphical systems work using this as part of its standard. \n 
		  *	When the transaction finishes it content can be knocking over the real visible part
		  *	of the graphical card. \n
		  *	@see Mask class.
		  */
		class Buffer
		{
			public:
			// The different types of blending supported by QGAMES library
			static const int _PUREBLENDMODE;
			static const int _ADDBLENDMODE;
			static const int _MODBLENDMODE;
			static const int _NOBLENDMODE;

			/** -1 means to get the size of the screen. 
				The buffer is always created over an screen. */
			Buffer (Screen* s, int w = -1, int h = -1);
			virtual ~Buffer () // Just in case...
							{ }

			int width () const
							{ return (_width); }
			int height () const
							{ return (_height); }

			/** In some graphical systems, the buffer has to be open and close. 
				It's something to be explicity defined under the graphical system itself. */
			virtual void open () = 0;
			virtual void close () = 0;
			/** To move the content to the screen. */
			virtual void moveToScreen (int x = 0, int y = 0) = 0;
			/** To move the content of the buffer to other buffer. 
				This function allows to mix buffer before knocking tem over the real screen. */
			virtual void moveToBuffer (Buffer* b,int x = 0, int y = 0) = 0;

			protected:
			Screen* _screen;
			int _width, _height;
		};

		Screen (const std::string& t, int w, int h, 
				const MapOfCameras& cms = MapOfCameras ());
		virtual ~Screen (); // By default it destroyes the cameras...and the ToFixScreen method.

		// Managing the main attributes of the screen...
		const std::string& title () const
							{ return (_title); }
		virtual void setIcon (Form* f)
							{ /** Nothing by default */ }
		int visualWidth () const
							{ return (_visualWidth); }
		int visualHeight () const
							{ return (_visualHeight); }
		Position visualCenter () const
							{ return (Position (__BD (_visualWidth >> 1), 
								__BD (_visualHeight >> 1), __BD 0)); } 
		Rectangle visualZone () const
							{ return (Rectangle (Position::_cero, 
										Position (__BD _visualWidth, __BD _visualHeight, __BD 0))); }
		virtual void windowAtPosition (int x, int y)
							{ /** Nothing by default. */ }
		virtual void windowAtCenter ()
							{ /** Nothing by default. */ }
		void setVisualAspect (int w, int h)
							{ _visualWidth = w; _visualHeight = h; }
		ToFixScreen* fix () 
							{ return (_fix); }
		void setFix (ToFixScreen* f)
							{ delete (_fix); _fix = f; }

		// Mamaning the cameras...
		MapOfCameras cameras ()
							{ return (_cameras); }
		bool existsCamera (const std::string& id) const;
		Camera* camera (const std::string& id); // NULL when not find...
		/** No two cameras with the same id... */
		void addCamera (Camera* c); 
		void removeCamera (const std::string& id);
		void removeAllCameras ();
		Camera* currentCamera () // It can be NULL, take care!
							{ return (_currentCamera); }
		void setCurrentCamera (const std::string& id) // The result can be NULL...
							{ _currentCamera = camera (id); }
		void setCurrentCamera (Camera* c); // The camera has to be within the list of those...
		/** Can be overloaded. By default it takes the first in the list if any! */
		virtual Camera* defaultCamera () 
							{ return ((_cameras.size () > (int) 0) 
								? (*_cameras.begin ()).second : NULL); }

		// These methods have been thought to manage the camera from the screen
		// In many ocasions, there is only one camera operating in the game...
		// Most of the methods don't work when the currentCamera has not been set!
		const Position& position ();
		void setPosition (const Position& p);
		void move (const Position& p);
		const Vector& orientation ();
		void rotateAround (const Vector& n, const Position& p, bdata a);
		void yaw (bdata a);
		void pitch (bdata a);
		void rotate (bdata a);
		void yawPithRotate (bdata y, bdata p, bdata r);
		bool isVisible (const Position& p);
		bool isVisible (const Rectangle& r);

		/** The positions of the screens can be updated every game loop. \n
			By default the screen doesn't move unless a fix element has been defined, 
			and update also the cameras associated. */
		virtual void updatePositions ();

		/** 
		  *	Most of the graphical libraries (SDL, OpenGL i.e) draws first everything on
		  *	a buffer that becomes visible with a single instruction. \n 
		  *	The buffer has to be cleaned up before drawing a new content. \n
		  *	The instructions to do both (clear the buffer, and become the buffer visible
		  *	depends on the graphical library being used. \n
		  *	Two methods have been declared in this library: openTransaction, and closeTransaction. \n
		  */
		void openTransaction ();
		/** When a buffer is created all drawing instructions must go to the buffer until it is destroyed. */
		Buffer* openBuffer (int tp, int w = -1, int h = -1);
		void drawForm (const Position& p, Form* f, int px, int py, int w, int h, int a = 255); // All need a camera selected!
		void drawForm (const Position& p, int oX, int oY, Form* f, int px, int py, int w, int h, int a = 255); 
		// Takes into account that an offset is possible in the form and it has oto be affected by camara conversions...
		void drawPoint (const Position& p, const Color& c);
		void drawLine (const Position& p1, const Position& p2, const Color& c, int t = 1); // t = thickness...must be positive
		void drawRectangle (const Rectangle& r, const Color& c, bool f = false); // f = true, when a filled rectangle is requested
		void drawRoundRectangle (const Rectangle& r, int rdx, const Color& c, bool f = false); // f = true when a filled rounded rectangled is requested
		void drawBox (const Box& b, const Color& c, bool f = false); // To draw a box made up of rectangles...
		void drawPolygon (const Polygon& r, const Color& c, bool f = false); // f = true, when a filled rectangle is requested
		void drawTriangle (const Position& p1, const Position& p2, const Position& p3, 
			const Color& c, bool f = false); // f = true, when a filled triangle is requested
		void drawImage (const Position& p, const std::string& img, int a = 255);
		void drawCircle (const Position& p, const Vector& o, bdata a1, bdata a2, 
			const Color& c, bool f = false); // F = true, when a filled ellipse is requested
		void drawText (const Position& p, const std::string& s,
			const std::string& f, int w, int h /** Just if the system supports w and h different. */, 
			const Color& c, int lt = _FONTNORMAL, int ot = 0);
		/** Some engines need the screen to be specificly update...But it not the normal situation! */
		virtual void drawOnYourOwn () 
							{ }
		void closeTransaction ();
		void closeBuffer (Buffer* b);

		protected:
		/** 
		  *	Internal class to control the number of graphical transactions opened. \n
		  *	Only when first is created, or when the last one is closed eventually
		  *	the internal screen / buffer methods to open and close the transaction are invoked. \n	
		  *	In some graphical libraries any time a transaction is open, even if another
		  *	one is already in use, a special instruction has to be invoked. \n
		  *	The transactions can be opened against the screen or a buffer of that. 
		  */
		class Transaction
		{
			public:
			Transaction (Screen* s)
				: _screen (s),
				  _numberOfTx (0)
							{ assert (s); }
			virtual ~Transaction ()
							{ }

			Screen* screen ()
							{ return (_screen); }
			const Screen* screen () const
							{ return (_screen); }

			void open (); 
			void close ();
			int numberOfTx () const
							{ return (_numberOfTx); }

			protected:
			virtual void openFirst () = 0;
			virtual void closeLast () = 0;
			virtual void openOne () = 0;
			virtual void closeOne () = 0;

			protected:
			Screen* _screen;
			int _numberOfTx;
		};

		protected:
		Transaction* currentTransaction ()
							{ return (_transaction); }
		const Transaction* currentTransaction () const
							{ return (_transaction); }

		// The very internal methods to define depending on the graphical libray behind.
		virtual void drawForm (int x, int y, Form* f, int px, int py, int w, int h, int a) = 0;
		virtual void drawPoint (int x, int y, const Color& c) = 0;
		virtual void drawLine (int x1, int y1, int x2, int y2, const Color& c, int t) = 0;
		virtual void drawRectangle (int x1, int y1, int x2, int y2, const Color& c, bool f) = 0;
		virtual void drawRoundRectangle (int x1, int y1, int x2, int y2, int rdx, const Color& c, bool f)
							{ drawRectangle (x1, y1, x2, y2, c, f); } // By default it does the same that the previous one (not all graphical engines support it)
		virtual void drawPolygon (int* x, int* y, int np, const Color& c, bool f) = 0;
		virtual void drawTriangle (int x1, int y1, int x2, int y2, int x3, int y3, const Color& c, bool f) = 0;
		virtual void drawImage (int x, int y, const std::string& i, int a) = 0;
		virtual void drawCircle (int x, int y, int a1, int a2, const Color& c, bool f) = 0;
		virtual void drawText (int x, int y, const std::string& s, 
			const std::string& f, int w, int h, const Color& c, int lt, int ot) = 0;
		virtual void activeScale () { } // It is not necessary by default... 
		virtual void desactiveScale () { } // It is not necessary by default...
		virtual Transaction* createTransaction () = 0;
		virtual Buffer* createBuffer (int tp, int w, int h) = 0;

		private:
		// A very internal method very used when drawing a polygon from a list of positions
		// It is supossed the positions are in order!
		// This method can't be overloaded (not virtual) and it invokes the other one drawPolygon (int* x, ...)
		void drawPolygonFromScreenCoordinates (const ListOfScrCoordinates& cL, const Color& c, bool f);

		protected:
		std::string _title;
		int _visualWidth, _visualHeight;
		ToFixScreen* _fix;
		MapOfCameras _cameras;
		Camera* _currentCamera;

		// Implementation
		Transaction* _transaction;
	};

	// To simplify the manipulation of a set of screens...
	typedef std::map <int, Screen*> Screens;
}

#endif

// End of the file
/*@}*/
