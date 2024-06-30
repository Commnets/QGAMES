/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: characterstep.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 30/03/2018 \n
 *	Description: Defines a class to represent the step taken (usually automatically) by a character. \n
 *				 This can be a way to control how a character moves in a scene automatically e.g.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_CHARACTERSTEPS__
#define __QGAMES_CHARACTERSTEPS__

#include <Common/notifierobserver.hpp>
#include <Common/definitions.hpp>
#include <Common/position.hpp>
#include <Common/rectangle.hpp>
#include <vector>
#include <map>

namespace QGAMES
{
	class Character;

	/** Structure defining a planned movement.
		The planned movements only applies over a character. 
		The reason to do this only over a character (and not over an entity e.g.) 
		is because it is the only one that can move! */
	class CharacterControlStep 
	{
		public:
		CharacterControlStep (int tp)
			: _type (tp)
							{ }
		virtual ~CharacterControlStep ()
							{ }

		/** To clone the step. 
			It has to be defined in each class inheriting this one. 
			In most of ocassions the default copy constructor should be enough. */
		virtual CharacterControlStep* clone () const = 0;

		/** To get a list of open values describing the step.
			By default, an empty list is returned. */
		virtual SetOfOpenValues runtimeValues () const
						{ return (SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESCHRSTEPTYPE__))); }
		/** To initialize the step from a set of configuration values. 
			By default, nothing is done. */
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg)
						{ assert (cfg.name () == std::string (__QGAMES_RUNTIMEVALUESCHRSTEPTYPE__)); }

		/** To initialize the step. */
		virtual void initializeOn (Character* a) = 0;
		/** To know whether it is or not the end of the step. */
		virtual bool isEndOn (Character* a) = 0;
		/** To update the step. 
			Depending on the step the actualizacion will be different. */
		virtual void updateOn (Character* a) = 0;
		/** To execute the step. */
		virtual void executeOn (Character* a) = 0;

		protected:
		/** The type of step. */
		int _type;
	};

	/** To simplify the definition of a list of steps */
	typedef std::vector <CharacterControlStep*> CharacterControlSteps;

	/** To group a set of steps. 
		When _currentStep == -1 no steps further. */
	class ComplexCharacterControlStep : public CharacterControlStep
	{
		public:
		ComplexCharacterControlStep (int t, const CharacterControlSteps& stps)
			: CharacterControlStep (t),
			  _steps (stps),
			  _currentStep (0)
							{ }
		~ComplexCharacterControlStep ();

		/** @see parent. 
			The inner steps state is saved, and also the currentStep. */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** @see parent. */
		virtual void initializeOn (Character* a);
		virtual bool isEndOn (Character* a)
							{ return (_currentStep == -1); }
		virtual void updateOn (Character* a);
		virtual void executeOn (Character* a);

		protected:
		/** To select the number of the first / next step to progress with. 
			It has to be implemented in other classes inheriting from this one. */
		virtual int firstStep () = 0;
		virtual int nextStep () = 0;
		virtual int afterLastStep () 
							{ return (-1); }

		// Implementation
		/** To clone the steps. 
			It should be invoked from any "clone" method of classes inheriting from this one. */
		CharacterControlSteps cloneSteps () const;

		protected:
		CharacterControlSteps _steps;

		// Implementation
		int _currentStep; // -1 would mean no step...
	};

	/** A step to do nothing. */
	class CharacterControlStepNone : public CharacterControlStep
	{
		public:
		CharacterControlStepNone ()
			: CharacterControlStep (__QGAMES_CHRSTEPNONE__)
							{ }

		/** @see parent. */
		virtual CharacterControlStep* clone () const
							{ return (new CharacterControlStepNone (*this)); }

		/** @see parent. */
		virtual void initializeOn (Character* a) { }
		virtual bool isEndOn (Character* a) { return (true); }
		virtual void updateOn (Character* a) { }
		virtual void executeOn (Character* a) { }
	};

	/** A step to spend time. */
	class CharacterControlTimeStep : public CharacterControlStep
	{
		public:
		CharacterControlTimeStep (bdata nS)
			: CharacterControlStep (__QGAMES_CHRSTEPTIME__),
			  _seconds (nS),
			  _currentLoop (0)
							{ assert (_seconds >= 0); }

		/** @see parent. */
		virtual CharacterControlStep* clone () const
							{ return (new CharacterControlTimeStep (*this)); }

		/** @see parent. \n
			The information added it the currentLoop (openValue). */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** @see parent. */
		virtual void initializeOn (Character* a) { _currentLoop = 0; }
		virtual bool isEndOn (Character* a);
		virtual void updateOn (Character* a) { _currentLoop++; }
		virtual void executeOn (Character* a) { }

		protected:
		bdata _seconds;
		int _currentLoop;
	};

	/** A step to wait a random number of seconds (in a range from 0 up to of a concrete number). */
	class CharacterControlRandomTimeStep : public CharacterControlTimeStep
	{
		public:
		CharacterControlRandomTimeStep (int nS)
			: CharacterControlTimeStep ((bdata) nS),
			  _maxSeconds (nS)
							{ _type = __QGAMES_CHRSTEPRANDOMTIME__; } // To change the type...

		/** @see parent. */
		virtual CharacterControlStep* clone () const
							{ return (new CharacterControlRandomTimeStep (*this)); }

		/** @see parent. */
		virtual void initializeOn (Character* a) 
							{ CharacterControlTimeStep::initializeOn (a); 
							  _seconds = (bdata) (rand () % _maxSeconds); }

		protected:
		int _maxSeconds;
	};

	/** A step to wait until the character is an specific state. */
	class CharacterControlWaitForStateStep : public CharacterControlStep
	{
		public:
		CharacterControlWaitForStateStep (int st)
			: CharacterControlStep (__QGAMES_CHRSTEPWAITFORSTATE__),
			  _state (st)
							{ }

		/** @see parent. */
		virtual CharacterControlStep* clone () const
							{ return (new CharacterControlWaitForStateStep (*this)); }

		/** @see parent. */
		virtual void initializeOn (Character* a) { }
		virtual bool isEndOn (Character* a);
		virtual void updateOn (Character* a) { }
		virtual void executeOn (Character* a) { }

		protected:
		int _state;
	};

	/** A step to to change the state of the entity. */
	class CharacterControlChangeStateStep : public CharacterControlStep
	{
		public:
		CharacterControlChangeStateStep (int st)
			: CharacterControlStep (__QGAMES_CHRSTEPCHANGESTATE__),
			  _state (st)
							{ }

		/** @see parent. */
		virtual CharacterControlStep* clone () const
							{ return (new CharacterControlChangeStateStep (*this)); }

		/** @see parent. */
		virtual void initializeOn (Character* a);
		virtual bool isEndOn (Character* a);
		virtual void updateOn (Character* a) { }
		virtual void executeOn (Character* a) { }

		protected:
		int _state;
	};

	/** A step just to start to move the entity and doesn't think in anything else. */
	class CharacterControlJustMoveStep : public CharacterControlStep
	{
		public:
		CharacterControlJustMoveStep (const Vector& dir, const Vector& accl)
			: CharacterControlStep (__QGAMES_CHRSTEPJUSTMOVESTATE__),
			  _direction (dir),
			  _acceleration (accl)
							{ }

		/** @see parent. */
		virtual CharacterControlStep* clone () const
							{ return (new CharacterControlJustMoveStep (*this)); }

		/** @see parent. */
		virtual void initializeOn (Character* a);
		/** When does this movement finish?: It has to be determinated with other step...maybe. */
		virtual bool isEndOn (Character* a) { return (true); }
		virtual void updateOn (Character* a) { }
		virtual void executeOn (Character* a) { }

		protected:
		Vector _direction;
		Vector _acceleration;
	};

	/** A step to move the character until it is not longer possible. */
	class CharacterControlMoveStep : public CharacterControlStep
	{
		public:
		CharacterControlMoveStep (const Vector& dir, const Vector& accl)
			: CharacterControlStep (__QGAMES_CHRSTEPMOVESTATE__),
			  _direction (dir),
			  _acceleration (accl)
							{ }

		/** @see parent. */
		virtual CharacterControlStep* clone () const
							{ return (new CharacterControlMoveStep (*this)); }

		/** @see parent. */
		virtual void initializeOn (Character* a);
		/** This movement finishes when the movement is not longer possible. */
		virtual bool isEndOn (Character* a);
		virtual void updateOn (Character* a) { }
		virtual void executeOn (Character* a) { }

		protected:
		Vector _direction;
		Vector _acceleration;
	};

	/** A step to control the distance advanced by a character. */
	class CharacterControlDistanceStep : public CharacterControlStep
	{
		public:
		CharacterControlDistanceStep (bdata dT)
			: CharacterControlStep (__QGAMES_CHRSTEPDISTANCE__),
			  _distance (dT), // Based on differences in the positions...
			  _originalPosition (Position::_cero)
							{ }

		/** @see parent. */
		virtual CharacterControlStep* clone () const
							{ return (new CharacterControlDistanceStep (*this)); }

		/** @see parent. \n
			The information added is the original position (openValue). */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** @see parent. */
		virtual void initializeOn (Character* a);
		virtual bool isEndOn (Character* a);
		virtual void updateOn (Character* a) { }
		virtual void executeOn (Character* a) { }

		protected:
		bdata _distance;

		// Implementation
		Position _originalPosition;
	};

	/** A step to advance until the distance to a point was less than a concrete distance
		from the original position. \n
		Some, but not all, of the coordinates of the final position can be __MINBDATA__
		meaning that this coordinate specifically hasn't to be considered into account, being replaced by
		the original one. */
	class CharacterControlMoveNearStep : public CharacterControlStep
	{
		public:
		CharacterControlMoveNearStep (const Position& fPos, bdata dT = (bdata) 0);

		/** @see parent. */
		virtual CharacterControlStep* clone () const
							{ return (new CharacterControlMoveNearStep (*this)); }

		/** @see parent. \n
			The information added is the original position (openValue). */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** @see parent. */
		virtual void initializeOn (Character* a);
		virtual bool isEndOn (Character* a);
		virtual void updateOn (Character* a) { }
		virtual void executeOn (Character* a) { }

		protected:
		Position _originalPosition;
		Position _finalPosition;
		bdata _distance;
	};

	/** A step to advance a set of steps but always in the limits marked by a rectangle. \n
		The rectangle can be located also in a 3D space. */
	class CharacterControlMoveInLimitsStep : public CharacterControlStep
	{
		public:
		CharacterControlMoveInLimitsStep (const Rectangle& rect, bdata d)
			: CharacterControlStep (__QGAMES_CHRSTEPLIMITS__),
			  _limits (rect),
			  _distance (d),
			  _originalPosition (Position::_cero)
							{ }

		/** @see parent. */
		virtual CharacterControlStep* clone () const
							{ return (new CharacterControlMoveInLimitsStep (*this)); }

		/** @see parent. */
		virtual void initializeOn (Character* a);
		virtual bool isEndOn (Character* a);
		virtual void updateOn (Character* a) { }
		virtual void executeOn (Character* a) { }

		protected:
		Rectangle _limits;
		bdata _distance;

		// Implementation
		Position _originalPosition;
	};

	/** A complex step where the next step is the previous one plus one. */
	class SequentialComplexCharacterControlStep : public ComplexCharacterControlStep
	{
		public:
		SequentialComplexCharacterControlStep (const CharacterControlSteps& stps)
			: ComplexCharacterControlStep (__QGAMES_CHRCOMPLEXSTEPSEQUENTIAL__, stps)
							{ }

		/** @see parent. */
		virtual CharacterControlStep* clone () const
							{ return (new SequentialComplexCharacterControlStep (cloneSteps ())); }

		protected:
		/** @see parent. 
			If next step is out the size, then it would finishes later. */
		virtual int firstStep ()
							{ return (0); }
		virtual int nextStep ()
							{ return (_currentStep + 1); }
	};

	/** A complex step where the next step is random. 
		It can't finishes ever, because the next step is always in the limits. */
	class RandomComplexCharacterControlStep : public ComplexCharacterControlStep
	{
		public:
		RandomComplexCharacterControlStep (const CharacterControlSteps& stps)
			: ComplexCharacterControlStep (__QGAMES_CHRCOMPLEXSTEPRANDOM__, stps)
							{ }

		/** @see parent. */
		virtual CharacterControlStep* clone () const
							{ return (new RandomComplexCharacterControlStep (cloneSteps ())); }

		protected:
		/** @see parent. 
			If next step is out the size, then it would finishes later. */
		virtual int firstStep ()
							{ return (rand () % (int) _steps.size ()); }
		virtual int nextStep ()
							{ return (rand () % (int) _steps.size ()); }
	};

	/** A complex step cyclic */ 
	class CyclicComplexCharacterControlStep : public ComplexCharacterControlStep
	{
		public:
		CyclicComplexCharacterControlStep (const CharacterControlSteps& stps)
			: ComplexCharacterControlStep (__QGAMES_CHRCOMPLEXSTEPCYCLIC__, stps)
							{ }

		/** @see parent. */
		virtual CharacterControlStep* clone () const
							{ return (new CyclicComplexCharacterControlStep (cloneSteps ())); }

		protected:
		/** @see parent. */
		virtual int firstStep ()
							{ return (0); }
		virtual int nextStep ()
							{ return (_currentStep + 1); }
		virtual int afterLastStep () 
							{ return (firstStep ()); }
	};

	/** To monitor and execute a set of steps for an entity. 
		This class is usually used to control a set of steps for a character. */
	class CharacterControlStepsMonitor : public Notifier
	{
		public:
		/** Constructor. 
			The class receives the steps to be executed and a reference to the character
			the steps belong to and whether the execution is or not cyclic.
			It is that when the last step finishes it starts again. 
			The monitor owns the steps. */
		CharacterControlStepsMonitor (int id, const CharacterControlSteps& s, Character* a, bool c = true);
		~CharacterControlStepsMonitor (); // To delete the steps...

		int id () const
							{ return (_id); }
		const CharacterControlSteps& steps () const
							{ return (_steps); }

		/** @see parent. \n
			The information added is the current step (openValue),
			and information comming from each step (setOfOpenValues). */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		// Managing the steps
		/** To return the current step. */
		int currentStep () const
							{ return (_currentStep); }
		/** Returns a list with the the steps remaining. */
		CharacterControlSteps stepsLeft () const;
		/** To add the steps from another steps monitor (at the end) */
		void addStepsFrom (CharacterControlStepsMonitor* m);
		/** To add the steps left from another steps monitor. */
		void addStepsLeftFrom (CharacterControlStepsMonitor* m);
		/** To know whether the first step is being executed. */
		bool executingFirstStep () const
							{ return (_currentStep == 0); }
		/** To know whether the monitor is executing the last step. */
		bool executingLastStep () const
							{ return (_currentStep == (int) (_steps.size () - 1)); }
		/** To know whether the monitor started to run. */
		bool startedToRun () const
							{ return (_currentStep != -1); }
		/** To know whether the monitor has finished. */
		bool hasFinished () const
							{ return (_currentStep == -2); }
		/** To know whether is now running. */
		bool isRunning () const
							{ return (_currentStep != -1 && _currentStep != -2); }

		Character* character ()
							{ return (_artist); }
		const Character* character () const
							{ return (_artist); }

		/** To know whether the execution is or not cyclic. */
		bool ciclic () const
							{ return (_ciclic); }
		void setCiclic (bool c)
							{ _ciclic = c;  }

		/** To initialize the monitor. */
		virtual void initialize ();

		/** Execute the steps. \n
			This method is invoked from the method updatePositions of Character (@see Character) 
			if a steps Monitor has been defined. \n
			the method throws three different events: \n
			__QGAMES_CHRSTEPSMONITORFIRSTSTEP__ when the first step is executed. \n
			__QGAMES_CHRSTEPSMONITORLASTSTEP__ when the last step is executed. \n
			and __QGAMES_CHRSTEPSMONITORSTARTSBACK__ when it starts back (because it is ciclic). */
		void run ();

		protected:
		/** The id. */
		int _id;
		/** The list of steps the monitor has to execute. */
		CharacterControlSteps _steps;
		/** Is the movement ciclic?.
			That's it: Does the entity have to start back when finishes the planned movements? */
		bool _ciclic;

		// Implementation
		/** Which is the current step. */
		int _currentStep;
		/** Which is the artis being supervised? */
		Character* _artist;
	};

	/** To simplify the management of a set of monitors. */
	typedef std::map <int, CharacterControlStepsMonitor*> CharacterControlStepsMonitors;
}

#endif
  
// End of the file
/*@}*/
