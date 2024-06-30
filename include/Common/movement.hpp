/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: movement.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a movement. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_MOVEMENT__
#define __QGAMES_MOVEMENT__

#include <Common/position.hpp>
#include <Common/notifierobserver.hpp>
#include <map>

namespace QGAMES
{
	class Movement;
	typedef std::map <int, Movement*> Movements;
	class Entity;

	/** 
	 *	A movement is always for an entity (or entities). \n
	 *	A movement moves an entity. \n
	 *	Before the entity is moved, it has a direction and an acceleration 
	 *	(usually known as velocity and accelrations vectors). \n
	 *	After the movement the entity could have others totally different. 
	 *	The movement class is accountable of changing those. \n
	 *	The movement is defined by a direction vector. This vector has nothing to do
	 *	with the previous ones. We could undertand it as its signature, and it
	 *	is very usefull to manage the entities. \n
	 *	The movement can be defined by several variables. These variables
	 *	can be changed at running time (setVariable method), but usually they are 
	 *	loaded from a definition file through the movement's builder (@see MovementBuilder). \n
	 *	The movement has also an id to identify it. \n
	 *	The movement can also be a notifier, when something special happens, the 
	 *	situation can be notified to someone. \n
	 *	The runtime situation of a movement can be get using the method runtimeValues. \
	 *	That this runtime situation can be set back using the method initializeRuntimeValuesFrom.
	 *	To get and set back the runtime situation of a movement is a very complex task.
	 *	Usually many variables are describing the internal status, so usually this
	 *	method will imply manhy instructions. Manage them with care.
	 */
	class Movement : public Notifier
	{
		public:
		Movement (int id, std::map <int, double> v)
			: _id (id), 
			  _variables (v)
							{ }
		virtual ~Movement ()
							{ }

		int id () const
							{ return (_id); }

		/** To clone a movement. */
		virtual Movement* clone () const = 0;

		/** To get the runtime status of the movement. Nothing is returned by default, but an empty structure */
		virtual SetOfOpenValues runtimeValues () const
							{ return (SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESENTITYMOVTYPE__))); }
		/** To set the runtime info of the movement from a set of values. 
			Nothing is done by default, but checking the structure received could be the right one. */
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg)
							{ assert (cfg.name () == std::string (__QGAMES_RUNTIMEVALUESENTITYMOVTYPE__)); }

		double variable (int id) const;
		const std::map <int, double>& variables () const
							{ return (_variables); }
		void setVariable (int id, double v);
		/** To say which should be the direction of the entity after executing the movement. \n
			But it could be only an intention. \n
			Remember the real position of the entiy should depend on the movement itself (@see orientation method in Entity) */
		virtual Vector direction () const = 0;
		/** The same with the accleration. */
		virtual Vector acceleration () const = 0;

		/** Before init a movement, this method could be used. \n 
			By default it does nothing. */
		virtual void initialize ()
							{ }
		/** To initialize the movement from the information of another movement. 
			By default it executes the basic initialization. */
		virtual void initializeFrom (const Movement* m)
							{ initialize (); }
		/** The very important method.
			How to move an entity? */
		virtual void move (const Vector& d, const Vector& a, Entity* e) = 0;
		virtual void finalize ()
							{ }

		protected:
		int _id; /** Which is the identificator for this movement. */
		std::map <int, double> _variables; /** Which are the variables (speed, ...) defining it? */
	};

	/** A possible movement is the NO movement. */
	class NoMovement : public Movement
	{
		public:
		NoMovement (int id, std::map <int, double> v)
			: Movement (id, v) 
							{ }

		virtual Movement* clone () const;
		virtual Vector direction () const
							{ return (Vector::_cero); }
		virtual Vector acceleration () const
							{ return (Vector::_cero); }

		virtual void move (const Vector& d, const Vector& a, Entity* e);
	};

	/** This describes a very basic linear movement. 
		The only external variable is to indicate a factor to increment the speed (that can be modified later).
		In this basic movement the accleration is not taken into account. */
	class SimpleLinearMovement : public Movement
	{
		public:
		SimpleLinearMovement (int id, std::map <int, double> v)
			: Movement (id, v),
			  _speed (0),
			  _defaultSpeed (0),
			  _lastDirection (Vector::_cero)
							{ }

		bdata speed () const
							{ return (_speed); }
		bdata defaultSpeed () const
							{ return (_defaultSpeed); }
		void setSpeed (bdata s)
							{ _speed = s; }

		virtual Movement* clone () const;

		/** @see parent. \n
			The information added is the speed (openValue) and the lastDirection (openValue). \n
			First parameter can be changed after being initilized and the second describe 
			where the movement was pointing to when it is saved. \n
			The baseSpeed is not saved because it can't be changed after it is read from the conf parameters. */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		virtual Vector direction () const
							{ return (_lastDirection); }
		virtual Vector acceleration () const
							{ return (Vector::_cero); }

		virtual void initialize ();
		virtual void move (const Vector& d, const Vector& a, Entity* e);

		protected:
		bdata _speed;
		bdata _defaultSpeed;
		Vector _lastDirection;
	};

	/** A very basic parabolic movement in the plane XY 
		It could be used to simulate a very simple jump in 2D games (no gravity effect). */
	class SimpleXYParabolicMovement : public Movement
	{
		public:
		SimpleXYParabolicMovement (int id, std::map <int, double> v);

		bool goingRight () const
							{ return (_lastDirection.posX () >= (bdata) 0); }
		bool isFalling () const
							{ return (_lastDirection.posY () >= (bdata) 0); }
		bool isClimbing () const
							{ return (_lastDirection.posY () < (bdata) 0); }

		void setJumpParameters (bdata l, bdata h, bdata tH, bdata s = __BD 1);
		bdata timeToHeight () const
							{ return (_timeToHeight); }
		bdata length () const
							{ return (_length); }
		bdata height () const
							{ return (_height); }
		bdata speed () const
							{ return (_speed); }
		void setGravity (bdata g); // It is converted into acceleration...

		virtual Movement* clone () const;

		/** @see parent. \n
			The information added is the one related with the movement: _length, _high, ... (openValue),
			and the data related with the implementation that describe the real status of the movement (openValue). \n
			What is not saved are the values of the internal constants (k1,...) because they depend on the others. */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		virtual Vector direction () const
							{ return (_lastDirection); }
		virtual Vector acceleration () const
							{ return (Vector::_cero); }

		/** 
		 *	Variable 0 = length (jumping)
		 *	Variable 1 = height (jumping)
		 *	Variable 2 = time in seconds (or fraction) to get the hightest point.
		 *				 The greater the time lesser the gravity.
		 *	Variable 3 = speed (1 by default)
		 */
		virtual void initialize ();
		/** In this case the new entity's position will be independant of the direction and
			acceleration the entity had. */
		virtual void move (const Vector& d, const Vector& a, Entity* e);

		protected:
		bdata _length;
		bdata _height;
		bdata _timeToHeight;
		bdata _speed;

		// Implementation, to improve performance...
		bool _firstMovement;
		bdata _x;
		bdata _y;
		Position _originalPosition;
		bdata _kx, _ky, _ka, _kn;
		Vector _lastDirection;
	};

	/** Movement one by one pixel.
		This class is one of the most important in terms of movement.
		The movement only moves one pixel after verifying first that this is possible with the entity (@see canMove method). \n
		The speed of the movement is the basic attribute. */
	class MovementOneByOne : public Movement
	{
		public:
		MovementOneByOne (int id, std::map <int, double> v);

		/** To know and set the speed. */
		int speed () const
							{ return (_speed); }
		void setSpeed (int s)
							{ _speed = s; }
		/** To know the base speed. */
		int baseSpeed () const
							{ return (_baseSpeed); }

		/** @see parent. \n
			The information added is the speed (openValue) and the 
			status of the internal counters for moving (openValue). \n
			The baseSpeed is not saved because it can't be changed after being read from the conf data. */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** 
		  *	Variable 0 = speed,
		  *	Variable 1 = base speed.
		  */
		virtual void initialize ();
		/** In this case the routine is just to move the entity one by one.
			The number of pixels to move in each loop depends on the speed (one of the variables)
			The speed is always relative to the base speed (the other variable).
			Moving one by one pixels is many times important, just to be sure, the position and the collisions
			against the things is well controlled. In that case, every individual movement has to be
			double checked. */
		virtual void move (const Vector& d, const Vector& a, Entity* e);

		protected:
		/** To move only one pixel. */
		virtual void moveOne (const Vector& d, const Vector& a, QGAMES::Entity* e) = 0;

		protected:
		int _speed;
		int _baseSpeed;

		// Implementation
		int _counterMSpeed [2];	
	};

	/** A movement that is made of little increments that are calculated in each iteration.
		Those little increments are also executed one by one (but not pixel by pixel take care) and also 
		verifying first with the entity that the movement is possible (@see canMove method in the class Entity). \n
		The internal information of the movement is kept into the internal structure Data
		The users can inherit from this class to add particular fields and values to it. \n
		The speed is also taking into account. */
	class IncrementalMovement : public Movement
	{
		public:
		IncrementalMovement (int id, ::std::map <int, double> v);

		virtual ~IncrementalMovement ();

		/* To know wether the entity is or not in the first movement. */
		bool isFirstMovement () const
							{ return (_firstMovement); }

		/** To know and set the speed. */
		int speed () const
							{ return (_speed); }
		void setSpeed (int s)
							{ _speed = s; }

		/** @see parent. \n
			The information added is the whether it is or not the first movement (openValue),
			the speed (openValue too), and the internal Data structure (it can be overloaded, a setOfOpenValues). */ 
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** @see parent. */
		virtual Vector direction () const
							{ return (_data -> _nextDirection); }
		virtual Vector acceleration () const
							{ return (_data -> _nextAcceleration); }

		/** @see parent. */
		virtual void initialize ();
		/** Call prepareMovement first time, the calculateNextMovement after very little increment, then
			somethingElseToDoAfterMoving when the movement has finished and if the movement has not been possible
			in any direction the method stopDuringMovement is invoked. */
		virtual void move (const Vector& d, const Vector& a, Entity* e);

		protected:
		class Data;

		/** To prepare the movement. This method is called once, during the first movement. \n
			It must be overloaded by the classes inheriting this one. */
		virtual void prepareMovement (Data* d, const Vector& dr, const Vector& a, Entity* e) = 0;
		/** Any time a movement has been done, the next one is calculated using this method. */
		virtual void calculateNextMovement (Data* d, const Vector& dr, const Vector& a, Entity* e) = 0;
		/** After any movement, this a "exit" routine to do something else related with the movement. \n
			By default there is nothing else to do. */
		virtual void somethingElseToDoAfterMoving (const Vector& d, const Vector& a, Entity* e)
							{ /** Nothing to do by default. */ }
		/** Impossible to increment the position during the calculated previous movement. */
		virtual void stopDuringMovement (const Vector& dr, const Vector& a, Entity* e)
							{ /** Nthing to do by default. */ }

		Data* data () 
							{ return (_data); }
		const Data* data () const
							{ return (_data); }
		/** To set the data internal element. 
			A potential other data element can be passed as a reference, 
			to copy all internal attributes. */
		void setData (const Data* o = NULL);
		/** To reset the internal data. */
		void resetData () 
							{ delete (_data); _data = NULL; }
		/** To create the basic element. 
			By default the basic one is created. */
		virtual Data* createData ()
							{ return (new Data ()); }

		protected:
		/** Very internal structure to keep the information of the movement.
			Every movement con need different information, so this class can 
			be inherited. */
		class Data 
		{
			public:
			Data ()
				: _initialPosition (Position::_cero), 
				  _nextDirection (Vector::_cero), _nextAcceleration (Vector::_cero) ,
				  _incX (INT_MIN), _incY (INT_MIN), _incZ (__MININT__) // INT_MIN means not initialized!
							{ }
			virtual ~Data () // It is virtual becaus it can be overloaded....
							{ }

			/** Add all parameters (openValues).
				It can be overloaded later as new data classes are created inheriting from this one. */ 
			virtual SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const SetOfOpenValues& cfg);

			/** To init the data from another data. */
			virtual void initFrom (const Data* o);

			public:
			Position _initialPosition;
			Vector _nextDirection, _nextAcceleration; // Direction after finishing the movement...
			Vector _nextOrientation; // Orientation after finishing the movement...
			int _incX, _incY, _incZ; // Pixels to advance next time...
		};

		bool _firstMovement;
		int _speed;
		Data* _data;
	};
}

#endif
  
// End of the file
/*@}*/
