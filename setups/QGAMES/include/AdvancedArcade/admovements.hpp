/**\ingroup AdArcadeGame */
/**\ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: admovements.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 26/03/2018 \n
 *	Description: Defines movements en a phisical environment. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ADMOVEMENTS__
#define __QGAMES_ADMOVEMENTS__

#include <AdvancedArcade/addefinitions.hpp>
#include <Arcade/arcadeinclude.hpp>

namespace QGAMES
{
	/** 
		A movement following a curve. \n
		The curve can be only una or a set of the. \n
		The movement can be cyclic or not.
	*/
	class FollowingACurveMovement : public IncrementalMovement
	{
		public:
		/** 
			The curve followed in the movement is defined as a template. \n
			The curve is divided in multiple points and anyone of them can be got with a method. 
			The number of point can be 0. \n
			The point returned should be always a relative one. \n
			The absolute value will be interpreted by the movement itself. */
		class CurveTemplate
		{
			public:
			virtual ~CurveTemplate ()
							{ }

			/** To clone it. */
			virtual CurveTemplate* clone () = 0;

			/** To know the type. */
			virtual int type () const = 0;

			/** To get (and set) the parameters as a set of open values. */
			virtual SetOfOpenValues asSetOfOpenValues () const = 0;
			virtual void fromSetOfOpenValues (const SetOfOpenValues& oV) = 0;

			/** To get the length of the curve. */
			virtual bdata length () const = 0;
			/** To rotate the curve. 
				The point to take as a reference will depend on the curve. */
			virtual void rotate (const Vector& v, bdata ang) = 0;

			// Managing the sample in the curve...
			/** To get / set the number of sampled points the template is made up of. */
			virtual int numberPoints () const = 0;
			virtual void setNumberPoints (int nP) = 0;
			/** To get a position of the curve that has been divided into nP points. */
			Position positionNumber (int nP);
			Position firstPosition ()
							{ return (positionNumber (0)); }
			Position lastPosition ()
							{ return (positionNumber (numberPoints ())); }

			protected:
			virtual Position positionNumberImplementation (int nP) = 0;
		};

		/** A set of templates. Used later */
		typedef std::vector <CurveTemplate*> CurveTemplates;

		/** Just a simple curve. */
		class SimpleCurveTemplate : public CurveTemplate
		{
			public:
			SimpleCurveTemplate (int nP)
				: _numberPoints (nP)
							{ assert (nP > 0); }

			/** @see parent. */
			virtual SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const SetOfOpenValues& oV);

			/** @see parent. */
			virtual int numberPoints () const
							{ return (_numberPoints); } // _numberPoints really represents the segments...
			virtual void setNumberPoints (int nP)
							{ assert (nP > 0); _numberPoints = nP; }

			protected:
			int _numberPoints;
		};

		/** No curve at all. So it is static.
			The point returned is always the same one and equal to cero. */
		class NoCurveTemplate : public SimpleCurveTemplate
		{
			public:
			NoCurveTemplate ()
				: SimpleCurveTemplate (1)
							{ }

			/** @see parent. */
			virtual CurveTemplate* clone ()
							{ return (new NoCurveTemplate ()); }

			/** @see parent. */
			virtual int type () const
							{ return (__QGAMES_NOMOVEMENTTRACE__); }

			/** @see parent. */
			virtual bdata length () const
							{ return ((bdata) 0); }
			virtual void rotate (const Vector& v, bdata ang)
							{ }

			/** @see parent. */
			virtual void setNumberPoints (int)
							{ /** Does nothing. */ }

			protected:
			/** @see parent. */
			virtual Position positionNumberImplementation (int nP)
							{ return (Position::_cero); }
		};

		/** A line. From cero to a point.
			It can be positive or negative. */
		class LineCurveTemplate : public SimpleCurveTemplate
		{
			public:
			LineCurveTemplate (int nP, const Position& t)
				: SimpleCurveTemplate (nP),
				  _to (t)
							{ setNumberPoints (_numberPoints); }

			/** @see parent. */
			virtual CurveTemplate* clone ()
							{ return (new LineCurveTemplate (_numberPoints, _to)); }

			/** @see parent. */
			virtual int type () const
							{ return (__QGAMES_LINEARCURVEMOVEMENTTRACE___); }

			/** @see parent. */
			virtual SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const SetOfOpenValues& oV);

			/** @see parent. */
			virtual bdata length () const
							{ return (_to.module ()); }
			virtual void rotate (const Vector& v, bdata ang);

			/** @see parent. */
			virtual void setNumberPoints (int nP);

			const Position& to () const
							{ return (_to); }

			protected:
			/** @see parent. */
			virtual Position positionNumberImplementation (int nP)
							{ return ((bdata) ((nP <= _numberPoints) ? nP : _numberPoints) * _vector); } 

			protected:
			Position _to;

			// Implementation
			/** The increment */
			Vector _vector;
		};

		/** A circle on a plane. 
			Can simulate varios types of circles clockwise or anti-clockwise using the initial and final angles. */
		class CircleCurveTemplate : public SimpleCurveTemplate
		{
			public:
			CircleCurveTemplate (int nP, const Vector& p, /** The plane. */ 
					bdata rdx, bdata aI = (bdata) 0, bdata aF = (bdata) 2 * __PI);

			/** @see parent. */
			virtual CurveTemplate* clone ()
							{ return (new CircleCurveTemplate (_numberPoints, 
								_plane, _radix, _initialAngle, _finalAngle)); }

			/** @see parent. */
			virtual int type () const
							{ return (__QGAMES_CIRCULARCURVEMOVEMENTTRACE___); }

			/** @see parent. */
			virtual SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const SetOfOpenValues& oV);

			/** @see parent. */
			virtual bdata length () const
							{ return (std::abs (_finalAngle - _initialAngle) * _radix); }
			virtual void rotate (const Vector& v, bdata ang);

			/** @see parent. */
			virtual void setNumberPoints (int nP);

			const Vector& plane () const
							{ return (_plane); }
			bdata radix () const
							{ return (_radix); }
			bdata initialAngle () const
							{ return (_initialAngle); }
			bdata finalAngle () const
							{ return (_finalAngle); }

			protected:
			/** @see parent. */
			virtual Position positionNumberImplementation (int nP);

			protected:
			Vector _plane;
			bdata _radix;
			bdata _initialAngle;
			bdata _finalAngle;

			// Implementation
			bdata _step;
			Vector _newX, _newY; /** To know well the plane where the circle is. */
			Position _position0;
		};

		/** A sinusoidal movement in a plane. */
		class SinosoidalCurveTemplate : public SimpleCurveTemplate
		{
			public:
			SinosoidalCurveTemplate (int nP, const Vector& p, /** The plane. */ 
					bdata rg, bdata pr, bdata aI = (bdata) 0, bdata aF = (bdata) 2 * __PI);

			/** @see parent. */
			virtual CurveTemplate* clone ()
							{ return (new SinosoidalCurveTemplate (_numberPoints, 
								_plane, _range, _period, _initialAngle, _finalAngle)); }

			/** @see parent. */
			virtual int type () const
							{ return (__QGAMES_SINUSOIDALCURVEMOVEMENTTRACE__); }

			/** @see parent. */
			virtual SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const SetOfOpenValues& oV);

			/** @see parent. */
			virtual bdata length () const // Right approximation when the number points is enough high and...
							{	return (_numberPoints * std::sqrt 
									(((_step * _period) * (_step * _period)) + 
									 ((_range * _range) * (std::sin (_step) * std::sin (_step))))); }
			virtual void rotate (const Vector& v, bdata ang);

			/** @see parent. */
			virtual void setNumberPoints (int nP);

			const Vector& plane () const
							{ return (_plane); }
			bdata range () const
							{ return (_range); }
			bdata period () const
							{ return (_period); }
			bdata initialAngle () const
							{ return (_initialAngle); }
			bdata finalAngle () const
							{ return (_finalAngle); }

			protected:
			/** @see parent. */
			virtual Position positionNumberImplementation (int nP);

			protected:
			Vector _plane;
			bdata _range;
			bdata _period;
			bdata _initialAngle;
			bdata _finalAngle;

			// Implementation
			bdata _step;
			Vector _newX, _newY; /** To know well where the circle is. */
			Position _position0;
		};

		/** A complex curve, made of many. */
		class ComplexCurveTemplate : public CurveTemplate
		{
			public:
			ComplexCurveTemplate (const CurveTemplates& crvs)
				: _curves (crvs),
				  _numberPoints (-1) // No calculated yet!
							{ }

			virtual ~ComplexCurveTemplate ()
							{ deleteCurves (); }

			/** @see parent. */
			virtual CurveTemplate* clone ();

			/** @see parent. */
			virtual int type () const
							{ return (__QGAMES_COMPLEXCURVEMOVEMENTTRACE__); }

			/** @see parent. */
			virtual SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const SetOfOpenValues& oV);

			/** @see parent. */
			virtual bdata length () const;
			virtual void rotate (const Vector& v, bdata ang);

			const CurveTemplates& curves () const
							{ return (_curves); }
			CurveTemplates& curves () 
							{ return (_curves); }

			/** @see parent. */
			virtual int numberPoints () const;
			virtual void setNumberPoints (int nP); 

			protected:
			/** @see parent. */
			virtual Position positionNumberImplementation (int nP);

			private:
			// Implementation
			void deleteCurves ();

			protected:
			CurveTemplates _curves;

			// Implementation
			/** First time, it is calculated. */
			mutable int _numberPoints;
		};

		FollowingACurveMovement (int id, std::map <int, double> v)
			: IncrementalMovement (id, v),
			  _curve (new NoCurveTemplate ()), // None by default...
			  _cyclic (false),
			  _currentPoint (0)
							{ }

		~FollowingACurveMovement ()
							{ delete (_curve); _curve = NULL; }

		/** @see parent. */
		virtual Movement* clone () const;

		/** @see parent. \n
			The information added is the current number of point. */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** To manage a curve. */
		const CurveTemplate* curve () const
							{ return (_curve); }
		CurveTemplate* curve ()
							{ return (_curve); }
		void setCurve (CurveTemplate* crv);

		/** To manage whether the movement is or not cyclic. 
			It is also determine by a configuration parameter. */
		bool cylic () const
							{ return (_cyclic); }
		void setCyclic (bool c)
							{ _cyclic = c; }

		/** @see parent. 
			There is a variable defining the time in seconds the entity should take 
			to move along the curve. */
		virtual void initialize ();
		virtual void initializeFrom (const Movement* m);

		protected:
		class PData;

		/** @see parent. */
		virtual void prepareMovement (Data* d, const Vector& dr, const Vector& a, Entity* e);
		virtual void calculateNextMovement (Data* d, const Vector& dr, const Vector& a, Entity* e);
		virtual void somethingElseToDoAfterMoving (const Vector& d, const Vector& a, Entity* e);

		/** @see parent. */
		PData* pData ()
							{ return (dynamic_cast <PData*> (_data)); }
		const PData* pData () const
							{ return (dynamic_cast <PData*> (_data)); }
		PData* pData (Data* d)
							{ return (dynamic_cast <PData*> (d)); }
		const PData* pData (Data* d) const
							{ return (dynamic_cast <PData*> (d)); }
		virtual Data* createData ()
							{ return (new PData ()); }

		protected:
		class PData : public IncrementalMovement::Data
		{
			public:
			PData ()
				: IncrementalMovement::Data (),
				  _currentPosition (Vector::_noPoint), // MEans not initialized 
				  _currentPositionInPixels (Vector::_noPoint)
							{ }

			/** @see parent. \n
				The information add is the one defined in this class (openValues). */
			virtual SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const SetOfOpenValues& cfg);

			/** @see parent. */
			virtual void initFrom (const Data* o);

			public:
			/** Which is the current position of the entity. 
				event considering the initial position. */
			Position _currentPosition;
			/** What is the las position in pixels. 
				should be the integer part of the previous one. */
			Position _currentPositionInPixels;
		};

		CurveTemplate* _curve;
		bool _cyclic;

		// Implementation
		int _currentPoint;
	};

	/** 
		A linear accelerated movement in the space. 
		This movement is so nice.

		The typical instructions to set it up are (in the entity): 
		10-	setMovement (IncrementalLinearMovementId);
		11-	(IncrementalLinearMovement*) currentMovement () -> setMovePath 
			(from, to, stepLength, accelerationModule, bool decceleration = false);
		12-	setMove (initialSpeed, QGAMES::Vector::_cero);
		As you apreciate the movement will start considering the initial speed (it could be cero).
		If there is an acceleration comming from the previous movement, it is not taken into account.
		The movement could have really four diffent phases:
		1.-Acceleration until reaching the max speed.
		2.-A mmovement with constant speed but trying to get aligned with the final destination.
		3.-One the direction has been aligned, then a real constant speed movement.
		4. To decelerate to get the final destination, only it that behaviour has been selected.
		When the movement is not possible, then a event is thown (different depending on the Axis: __QGAMES_MOVAXIS?NOTPOSSIBLE__).
		When the movement reaches the target an event is thrown (__QGAMES_MOVEMENTLIMITREACHED__)
	*/
	class IncrementalLinearMovement : public IncrementalMovement
	{
		public:
		IncrementalLinearMovement (int id, std::map <int, double> v)
			: IncrementalMovement (id, v),
			  _from (Vector::_cero), _to (Vector::_cero), 
			  _entrySpeed (Vector::_cero),
			  _maxSpeed ((bdata) 0),
			  _acceleration ((bdata) 0),
			  _decceleration (false),
			  _externalSpeed (Vector::_noPoint), _externalAcceleration (Vector::_noPoint),
			  _zone (0),
			  _k1 (Vector::_cero), _k2 (Vector::_cero), _k3 (Vector::_cero),
			  _increment (Vector::_cero) 
								{ }

		/** @see parent. */
		virtual Movement* clone () const;

		/** @see parent. \n
			The information added is the one related with this movement (openValues),
			even the _k1, _k2,... because the also change when moving. */ 
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** The parameters of the movement. */
		void setMovePath (const Position& from, const Position& to, const Vector& eS, 
			bdata mS, bdata a, bool dC = false);

		/** @see parent. */
		virtual void initialize ();
		virtual void initializeFrom (const Movement* m);

		protected:
		class PData;

		/** @see parent. */
		virtual void prepareMovement (Data* d, const Vector& dr, const Vector& a, Entity* e);
		virtual void calculateNextMovement (Data* d, const Vector& dr, const Vector& a, Entity* e);
		virtual void somethingElseToDoAfterMoving (const Vector& d, const Vector& a, Entity* e);

		/** see parent. */
		virtual Data* createData ()
							{ return (new PData ()); }
		PData* pData ()
							{ return (dynamic_cast <PData*> (_data)); }
		const PData* pData () const
							{ return (dynamic_cast <PData*> (_data)); }
		PData* pData (Data* d)
							{ return (dynamic_cast <PData*> (d)); }
		const PData* pData (Data* d) const
							{ return (dynamic_cast <PData*> (d)); }
		
		/** To adjust internal variales (if needed) when the parameters used 
			in the method setMove (at entity level) change.
			By default nothing is done. 
			The new vectors for speed and acceleration are received as parameter. 
			The old ones are kept in internal variables first (in the prepareMovement method. */
		void adjustInternalParametersWhenSetMoveChange (const Vector& dr, const Vector& ac)
							{ }
		/** To calculate the internal parameters used to determinate incerments in position and speed.
			The movement is a uniformly accelrated movement. 
			The data kept in pData structure should be used. */
		void calculateInternalParametersForMovement ();

		protected:
		class PData : public IncrementalMovement::Data
		{
			public:
			PData ()
				: IncrementalMovement::Data (),
				  _currentPosition (Vector::_noPoint), // MEans not initialized 
				  _currentPositionInPixels (Vector::_noPoint),
				  _currentSpeed (Vector::_noPoint),
				  _currentAcceleration (Vector::_noPoint)
							{ }

			/** @see parent. \n
				The information add is the one defined in this class (openValues). */
			virtual SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const SetOfOpenValues& cfg);

			/** @see parent. */
			virtual void initFrom (const Data* o);

			public:
			/** Which is the current position of the entity. 
				event considering the initial position. */
			Vector _currentPosition;
			/** What is the las position in pixels. 
				should be the integer part of the previous one. */
			Vector _currentPositionInPixels;
			/** The current speed. */
			Vector _currentSpeed;
			/** The current aceleration. This should be constant. 
				The same that the one received when preparing the movement. */
			Vector _currentAcceleration;
		};

		Vector _from;
		Vector _to;
		Vector _entrySpeed;
		bdata _maxSpeed;
		bdata _acceleration;
		bool _decceleration;

		// Implementation
		/** These two vectors are the ones sent by the entity using the method setMove. 
			When they change, a exit method to ajust (if any) internal variables could be invoked. */
		Vector _externalSpeed, _externalAcceleration;
		int _zone; // From 0 to 4 (entry + acceleration, getting right, stright ahead, decceleration, goal)
		Vector _k1, _k2, _k3, _k4;
		Vector _increment;
	};

	/** 
		A type of movement relaying pure in phisycs.
		The movement considers: a = F / m; where "a" is a constant vector.
		The speed and the position are calculated from this previous value:
		a (t) = a0;
		v (t) = a0 * t + v0
		p (t) = a0 / 2 * t + v0 * t + p0;
		All of them are vectorial measures
		The movement accelerates continiously in the direction of the speed set using setMove.

		The typical way of starting the movement is:
		10-	setMovement (PhysicsIncremetalMovementId);
		11-	setMove (initialSpeed, initialAcceleration);
		The second instruction receives v0 and a0, then the movement will calculate the rest of the data needed.
		If there is an acceleration comming from the previous movement, it is not taken into account.
		The speed can be 0, but not both (speed and acceleration) can, because the entity will not move.
		This type of movement can be defined inthe configuration files.

		The movement can be affected externally using two instructions:
		1.- definitivelly, using the method additionalAcceleration. 
			For instance, imagine the entity in a liquid, this acceleration could represent the bouyancy force.
			Or e.g. the entity is moving against the wind, represented by this force.
		2.- Some times, the movement is temporary affected. 
			It is what is known like an impulse. It is set using the method addImpulse.
			The method receives the force and the time it will affect the movement in terms of cycles.

		Additionally the movement can be affected by friction:
		Friction means a force that is always against the movement (the vector of the speed).
		The friction affects the entity only if it is moving...

	*/
	class PhysicsIncremetalMovement : public IncrementalMovement
	{
		public:
		PhysicsIncremetalMovement (int id, std::map <int, double> v)
			: IncrementalMovement (id, v),
			  _maxSpeed (__MAXBDATA__), _minSpeed (__MINBDATA__),
			  _impulses (), // Empty...
			  _frictionCoefficient ((bdata) 0), // Meand nothing...
			  _externalSpeed (Vector::_noPoint), _externalAcceleration (Vector::_noPoint),
			  _k1 (Vector::_noPoint), _k2 (Vector::_noPoint), _k3 (Vector::_noPoint), _k4 (Vector::_noPoint),
			  _directionX (1), _directionY (1), _directionZ (1),
			  _frictionForce (Vector::_cero)
							{ }

		/** @see parent. */
		virtual Movement* clone () const;

		/** @see parent. \n
			The information added is the one related with this movement (openValues). */ 
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);
		
		/** To set a maximum speed.
			When that speed is reached, the a notification is sent. 
			By default maxSpeed is MAXBDATA. */
		bdata maxSpeed () const
							{ return (_maxSpeed); }
		void setMaxSpeed (bdata mS)
							{ _maxSpeed = mS; }
		
		/** To set a minimum speed.
			When that speed is reached, the a notification is sent. 
			By default maxSpeed is MINBDATA. */
		bdata minSpeed () const
							{ return (_minSpeed); }
		void setMinSpeed (bdata mS)
							{ _minSpeed = mS; }

		/** To get and change the movement variables. 
			It can be used to modify the acceleration that the movement has when started. 
			Same with speed. */
		const Vector& currentSpeed () const
							{ return (pData () -> _currentSpeed); }
		void setSpeed (const Vector& s);
		const Vector& currentAcceleration () const
							{ return (pData () -> _currentAcceleration); }
		void setAcceleration (const Vector& a);

		/** To introduce an external force that affects the movemennt. 
			e.g. if swiming the external force could represent the bouyancy force. */
		void setAdditionalAcceleration (const Vector& aA);

		/** To add and impulse, The impulse is temporal by definition,
			so not only the value of the acceleration is needed but also the number of cycles
			that this force lasts. */
		void addImpulse (const Vector& i, int mC);
		/** To replace the impulses for this new one. */
		void replaceImpulses (const Vector& i, int mC);
		/** To eliminate all impulses. */
		void stopImpulses ()
							{ replaceImpulses (Vector::_cero, 0); }

		/** To manage the friction.
			The value passed is the friction when the entity is stopped.
			When it is moving that value will be half. 
			So managing friction means usually to manage also the minSpeed value. */
		bdata frictionCoefficient () const
							{ return (_frictionCoefficient); }
		/** Take care when changing the friction froce in the middle of the movement.
			It will affect clearly to the movement itself. */
		void setFrictionCoefficient (bdata fC);

		/** To stop the movement. */
		void stopMovement ();

		/** Just to change the direction of the movement.
			Nothing about the internal status of the movement changes. 
			Only the movement has to go to the other place. */
		void changeDirectionInAxis (int nA);
		/** To know the direction. */
		Vector directionInAxis ()
							{ return (Vector ((bdata) _directionX, (bdata) _directionY, (bdata) _directionZ)); }

		/** @see parent. */
		virtual void initialize ();
		virtual void initializeFrom (const QGAMES::Movement* m);

		protected:
		class PData;

		/** @see parent. */
		virtual void prepareMovement (Data* d, const Vector& dr, const Vector& a, Entity* e);
		virtual void calculateNextMovement (Data* d, const Vector& dr, const Vector& a, Entity* e);
		virtual void somethingElseToDoAfterMoving (const Vector& d, const Vector& a, Entity* e);

		/** see parent. */
		virtual Data* createData ()
							{ return (new PData ()); }
		PData* pData ()
							{ return (dynamic_cast <PData*> (_data)); }
		const PData* pData () const
							{ return (dynamic_cast <PData*> (_data)); }
		PData* pData (Data* d)
							{ return (dynamic_cast <PData*> (d)); }
		const PData* pData (Data* d) const
							{ return (dynamic_cast <PData*> (d)); }

		/** To adjust internal variales (if needed) when the parameters used 
			in the method setMove (at entity level) change.
			By default a new calculation of everything is done.  
			The new vectors for speed and acceleration are received as parameter. 
			The old ones are kept in internal variables first (in the prepareMovement method. */
		void adjustInternalParametersWhenSetMoveChange (const Vector& dr, const Vector& ac);
		/** To calculate the internal parameters used to determinate incerments in position and speed.
			The movement is a uniformly accelrated movement. 
			The data kept in pData structure should be used. */
		void calculateInternalParametersForMovement ();
		/** To adjust the next speed and next acceleration taking into account 
			the movement. and whether the movement has changed the direction. */
		void toAdjustNextSpeedAndAcceleration ();

		protected:
		/** Structure to keep information about the impulses.
			Two simple methods has been added to control how the impulse evolves. 
			Two methods are used in the calculateNextMovement method. */
		struct Impulse
		{
			Impulse ()
				: _impulse (Vector::_cero), _maxCycles (0), _currentCycle (0)
							{ }

			Impulse (const Vector& i, int mC);

			/** @see parent. \n
				The information add is the one defined in this structure (openValues). */
			virtual SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const SetOfOpenValues& cfg);

			/** Adds a new cycle and return true if the limit has been reached. */
			bool addCycle ()
							{ return ((++_currentCycle >= _maxCycles) ? true : false); }
			/** Just returns true when the limit has been reached. */
			bool isEnd ()
							{ return (_currentCycle >= _maxCycles ? true : false); }

			Vector _impulse;
			int _maxCycles;
			int _currentCycle;
		};

		typedef std::vector <Impulse> Impulses;

		/** Internal class with information related with the movement. */
		class PData : public IncrementalMovement::Data
		{
			public:
			PData ()
				: IncrementalMovement::Data (),
				  _currentPosition (Vector::_noPoint), // Means not initialized
				  _currentPositionInPixels (Vector::_noPoint),
				  _currentSpeed (Vector::_noPoint),
				  _currentAcceleration (Vector::_noPoint)
							{ }

			/** @see parent. \n
				The information add is the one defined in this class (openValues). */
			virtual SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const SetOfOpenValues& cfg);

			/** @see parent. */
			virtual void initFrom (const Data* o);

			public:
			// Implmentation
			/** Which is the current position of the entity. 
				event considering the initial position. */
			Vector _currentPosition;
			/** What is the las position in pixels. 
				should be the integer part of the previous one. */
			Vector _currentPositionInPixels;
			/** The current speed. */
			Vector _currentSpeed;
			/** The current aceleration. This should be constant. 
				The same that the one received when preparing the movement. */
			Vector _currentAcceleration;
		};

		/** When reached a notification is thrown. */
		bdata _maxSpeed;
		bdata _minSpeed;
		/** The impulses added to the movement. */
		Impulses _impulses;
		/** The friction coeficient, if any. 0 when nothing. */
		bdata _frictionCoefficient;

		// Implementation
		/** These two vectors are the ones sent by the entity using the method setMove. 
			When they change, a exit method to ajust (if any) internal variables could be invoked. */
		Vector _externalSpeed, _externalAcceleration;
		/** Factors used in the movement calculation. */
		Vector _k1, _k2, _k3, _k4;
		/** To be able to change the direction of the movement in a specific axis. */
		int _directionX, _directionY, _directionZ;
		/** The friction force in every moment. */
		Vector _frictionForce;
	};

	/** A movement to bounce until the speed is below the minimum. 
		When the movement is not possible in a specific direction, 
		the direction is changed and the speed reduced a little bit.
		If after not possible to move in a specific direction and change it, 
		the movement is not possible either, then the entity is blocjed and it has to be notified. */
	class BouncePhysicsIncrementalMovement : public PhysicsIncremetalMovement
	{
		public:
		BouncePhysicsIncrementalMovement (int id, std::map <int, double> v)
			: PhysicsIncremetalMovement (id, v),
			  _elasticCoefficient ((bdata) 0.5) // Half by default...
							{ }

		/** @see parent. */
		virtual Movement* clone () const;

		/** @see parent. \n
			The information added is the one related with this movement (openValues). */ 
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** To control the elastic coefficient.
			At the end this coeficient is the one affecting the change of speed.
			It could be greater than 1, meaning that the surface behaves like an elastic bed. 
			If affects when the entity stops its movement. */
		bdata elasticCoefficient () const
							{ return (_elasticCoefficient); }
		void setElasticCoefficient (bdata fC) 
							{ _elasticCoefficient = fC; }

		/** @see parent. */
		virtual void initialize ();

		protected:
		/** @see parent. */
		virtual void stopDuringMovement (const Vector& dr, const Vector& a, Entity* e);

		protected:
		bdata _elasticCoefficient;
	};
}

#endif
  
// End of the file
/*@}*/