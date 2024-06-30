/** \addtogroup SabreWulf */
/*@{*/

#ifndef __MTENTITIES_HPP__
#define __MTENTITIES_HPP__

#include "Sabreman.hpp"
#include "Scenes.hpp"
#include "Defs.hpp"
#include <Arcade/arcadeinclude.hpp>

/** This class defines a monster. */
class SabreWulfMonster : public QGAMES::Artist
{
	public:
	/** The different types of monster sabrewulf can have. */
	enum Aspect {
		// The nasties...
		_APE = 0, _BIRD1, _BIRD2, _BIRD3, _BUG, _LOBSTER, 
		_MOTH, _MOUSE, _SCORPION, _SKUNK, _SNAKE1, _SNAKE2, 
		_SNAKE3, _SPIDER1, _SPIDER2, _SPIDER3, _THING,
		_FROG1, _FROG2, _LIZARD, _OWL1, _OWL2, _FIRE, // The fire is a special nastie...because it is inmortal...
		// The inmortals...
		_RHINOLEFT, _RHINORIGHT, _SPEARMANBLUE, 
		_SPEARMANPINK, _SPEARMANRED, 
		_WOLF, _HIPPO };

	/** The different state a monster can have.
		When waiting, the monster is not in screen yet. After a couple of
		seconds it will start to grow!.
		When growing a couple of bars appear in some random part of the maze.
		When moving just pursuit sabreman from time to time.
		When destroying, a cloud appears. 
		When the monster is not alive means it is free
		for the code to assign a new aspect and start to wait...*/
	enum State { _WAITING = 0, _GROWING, _MOVING, _DIEING, __NOTALIVE};

	/** Every monster has a behaviour.
		The behaviour of all nastie monsters are the same,
		but depending on the inmortal the bahviour could be different. */
	class Behaviour
	{
		public:
		Behaviour (SabreWulfMonster* m, Sabreman* s)
			: _sabreman (s),
			  _monster (m)
							{ }

		/** To determinate is which circunstances the monster can die...if any. */
		virtual bool canDie () const = 0;
		/** To determinate whether the monster can or not move,
			receiving a direction and a acceleration. 
			By default, monsters always can move. */
		virtual bool canMove (const QGAMES::Vector& o, const QGAMES::Vector& a) 
							{ return (true); }
		/** To determinate a new movement, if any. */
		virtual QGAMES::Vector changeMovementDirection () = 0;
		/** To indicate a new factor to multiply the speed if needed...by default 1.5 */
		virtual QGAMES::bdata changeMovementSpeedFactor () 
							{ return (__BD 1.5); }
		/** Once the movement has been changed to indicate the next. */
		virtual QGAMES::Vector movementDirection () = 0;
		/** Number of steps before decinding or not to change the movement. */
		virtual int numberStepsBeforeAsking () const = 0;
		/** To determinate whether the monster could or not be damaged by sabreman. 
			By default any monster can be damage by sabreman. */
		virtual bool couldBeDamagedBySabreman () const
							{ return (true); }
		/** When sabreman hits the monster, something could happen to the
			monster. i.e Nasties die. By default it does nothing. */
		virtual void whatToDoWhenSabremanHitsIt ()
							{ }
		virtual SabreWulfMonster::State newState ();
		/** To draw some special related with the behaviour.
			By default, there is nothing special to draw. 
			In some cases a line with the path the monster is going to foloow i.e can be drawn. */
		virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
							{ }

		protected:
		SabreWulfMonster* _monster;
		Sabreman* _sabreman;
	};

	// Constructors & Destructores
	SabreWulfMonster (int id, const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

	/** To determinate whether the monster can or not move.
		If the monster is in one of the moving zones and the potential next 
		movements also, the movemenet is possible. */
	virtual bool canMove (const QGAMES::Vector& d, const QGAMES::Vector& a);

	/** @see parent. */
	virtual QGAMES::Rectangle collisionZone () const;

	/** To get the position of the character to take into a count when moving
		or collisioning with some one. */
	QGAMES::Position referenceZone (const QGAMES::Vector& d) const;

	/** To know the number of the maze place where the monster is. */
	int mazePlaceNumber () const
							{ return (_mazePlaceNumber); }
	/** To set where the monster is. */
	void setMazePlaceNumber (int mP);

	// To control the aspect of the monster.
	// The invocations cames always from outside
	/** To know the aspect the mosnter has now. */
	Aspect aspect () const
							{ return (_aspect); }
	/** To know whether the	monster is or not a nastie. */
	virtual bool isNastie () const = 0;
	/** To know whether the monster is or not an inmortal. */
	bool isInmortal () const
							{ return (!isNastie ()); }
	/** To know the status of the monster. */
	State state () const
							{ return (_state); }
	/** To set the state of the monster... */
	void setState (State s);
	/** To set the state and the aspect of the monster. */
	void setAspectAndState (Aspect a, State s);
	/** To know whether it can be damaged by a sabreman. 
		It will depend on th behaviour. */
	bool couldBeDamagedBySabreman ();

	virtual void initialize ();
	virtual void initializeAs (Aspect a, int mP, const QGAMES::Position& pos, Sabreman* s);
	virtual void inEveryLoop ();
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

	/** @see parent. */
	virtual void whenCollisionWith (QGAMES::Entity* e);

	protected:
	// Implementation
	/** To decide what to do when the monster is waiting. 
		The method returns a portential change in the state of the monster. */
	virtual State whenWaiting () = 0;
	/** To decide what to do when the monster is growing. 
		The method returns a potential change in the state of the monster. */
	virtual State whenGrowing () = 0;
	/** To decide what to do when the monster is moving. 
		The method returns a potential change in the state of the monster. */
	virtual State whenMoving (); // The default implementation is useful for both nasties and inmortals! 
	/** To decide what to do when the monster is dieing. 
		The method returns a potential change in the state of the monster. */
	virtual State whenDieing () = 0;
	/** To decide what to do when the monster is not living. 
		The method returns a potential change in the state of the monster. */
	virtual State whenNotLiving (); // The default implementation does not do anything!!
	/** Internal method to set the aspect and the form of the monster
		depending in its current on the type of monster and the state it is in. */
	virtual void setAspectAndForm () = 0;

	protected:
	/** The number of the maze place where the monster is. */
	int _mazePlaceNumber;
	/** The current aspect the monster has. */
	Aspect _aspect;
	/** The current status. */
	State _state;
	/** The behaviour. */
	Behaviour* _behaviour;
	/** A reference to Sabreman.
		This reference is used to pursuit him i.e. */
	Sabreman* _sabreman;

	// Implementation
	/** To indicate 
	/** The initial and final frame.
		It is something that depens on the aspect of the monster. */
	int _initialFrameRight, _finalFrameRight;
	int _initialFrameLeft, _finalFrameLeft;
	int _cInitialFrame, _cFinalFrame;
	/** Counter to count steps to change the aspect, or the state itself,
		depending in which one the monster is. 
		A counter for genrical purpouses indeed. */
	int _counter;
	/** When to decide to move to the target. */
	mutable bool _decideTarget;
	/** Number of steps the monster is walking in a direction
		before deciding a new one. */
	int _numberSteps;
};

/** This class represents a Nastie. 
	A nastie is one of the mosnters that appear in the maze place
	after a couple of miliseconds sabreman is in the place. */
class SabreWulfNastie : public SabreWulfMonster
{
	public:
	// Constructors & Destructores
	SabreWulfNastie (int id, const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

	virtual Entity* clone () const
							{ return (new SabreWulfNastie (_id, _forms, _data)); }

	virtual bool isNastie () const
							{ return (true); }
	/** To know whether the monster is or not free for the code
		to assign it a new aspect and start to grow. */
	bool isFree () const
							{ return (_state == __NOTALIVE); }

	/** To kill the nastie. */
	void kill ();

	virtual void initialize ();
	virtual void initializeAs (Aspect a, int mP, const QGAMES::Position& pos, Sabreman* s);
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

	private:
	// Implementation
	/** To make the sound when appearing. */
	void soundAppearing (bool s);
	virtual State whenWaiting ();
	virtual State whenGrowing ();
	virtual State whenDieing ();
	virtual void setAspectAndForm ();

	private:
	// Implementation
	/** A counter to count the number of times the appearance process is repeted
		in the case the monster is a nastie one. */
	int _counterAppearances;
	/** Counter being not visible. */
	int _counterNotVisible;
	/** Number of sounds appearing. */
	static int _numberSoundsAppearing;
	/** Time to appear, in case this is a nastie. */
	int _timeToAppear;
	/** Number of times the apparition cycle is taken. */
	int _numberOfAppears;
};

/** The behaviour of a nastie monster. 
	The behaviour of the nastie is quite simple.
	It has some kind of random movement, but suddlenly they can 
	decide to pursuit sabreman. */
class NastieBehaviour : public SabreWulfMonster::Behaviour
{
	public:
	NastieBehaviour (SabreWulfMonster* m, Sabreman* s)
		: SabreWulfMonster::Behaviour (m, s),
		  _lastMovement (QGAMES::Vector::_cero)
							{ }

	/** They can die, when sabreman is fighting! */
	virtual bool canDie () const
							{ return (true); }
	/** The movement direction is decided maybe randomically! */
	virtual QGAMES::Vector changeMovementDirection ();
	/** Once the direction has been changed it is maintained. */
	virtual QGAMES::Vector movementDirection ()
							{ return (_lastMovement); }
	/** They can walk 40 steps from decision to decision...take care */
	virtual int numberStepsBeforeAsking () const
							{ return (40); }
	/** Nasties die when sabreman hits them. */
	virtual void whatToDoWhenSabremanHitsIt ();

	protected:
	QGAMES::Vector _lastMovement;
};

/** The behaviour of the fire. 
	The fire appears when sabreman stays a while in the same place,
	and dissapears once sabreman leaves the maze place he is in. 
	The behaviour is quite close to nastie's but always going after 
	sabreman. */
class FireBehaviour : public NastieBehaviour
{
	public:
	FireBehaviour (SabreWulfMonster* m, Sabreman* s)
		: NastieBehaviour (m, s)
							{ }

	/** Fire can't die! */
	virtual bool canDie () const
							{ return (false); }
	/** The movement direction is going always after sabreman! */
	virtual QGAMES::Vector changeMovementDirection ();
	/** Can't be damage by sabreman. */
	virtual bool couldBeDamagedBySabreman () const
						{ return (false); }
	/** Nothing happens when sabreman hits it. */
	virtual void whatToDoWhenSabremanHitsIt ()
						{ }
};

/** This class represents all entities that are inmortal in the game. 
	An inmortal is a "monster" that pursuit sabreman.
	There are several types of inmortals.
	Usually they are creature of the jungle, such as hippo, shino, or a wulf.
	They can't die, but if sabreman hits them, the start to run away in the apposite
	direction, at least for a while!. */
class SabreWulfInmortal : public SabreWulfMonster
{
	public:
	// Constructors & Destructores
	SabreWulfInmortal (int id, const QGAMES::Forms& f = QGAMES::Forms (), 
		const QGAMES::Entity::Data& d = QGAMES::Entity::Data ());

	virtual Entity* clone () const
							{ return (new SabreWulfInmortal (_id, _forms, _data)); }

	virtual bool isNastie () const
							{ return (false); }

	virtual void initialize ();
	virtual void initializeAs (Aspect a, int mP, const QGAMES::Position& pos, Sabreman* s);
	virtual void initializeAs (int a, int mP, const QGAMES::Position& pos, Sabreman* s);
	virtual void updatePositions ();
	virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

	private:
	// Implementation
	virtual State whenWaiting ();
	virtual State whenGrowing ();
	virtual State whenMoving (); 
	virtual State whenDieing ();
	virtual void setAspectAndForm ();
};

/** The behaviour of the inmortal monster. 
	This class is generic for all inmortals.
	It defines that inmortals can't die, and the number of steps
	the wals without no asking where the user is are just 20. */
class InmortalBehaviour : public SabreWulfMonster::Behaviour
{
	public:
	InmortalBehaviour (SabreWulfMonster* m, Sabreman* s)
		: SabreWulfMonster::Behaviour (m, s)
							{ }

	/** Inmortals never die. */
	virtual bool canDie () const
							{ return (false); }
	/** By default at the same speed than sabreman! */
	virtual QGAMES::bdata changeMovementSpeedFactor ()
							{ return (4); }
	/** They can walk 20 steps from decision to decision...take care */
	virtual int numberStepsBeforeAsking () const
							{ return (20); }
};

/** The behaviour of the rhino monster. */
class RhinoBehaviour : public InmortalBehaviour
{
	public:
	RhinoBehaviour (SabreWulfMonster* m, Sabreman* s, const QGAMES::Vector& lO)
		: InmortalBehaviour (m, s),
		  _lastOrientation (lO),
		  _toRunAwayFromSabreman (false)
							{ }

	/** The rhino, moves always horizontally without taking into 
		account where sabreman is. They take into account
		Just the limits of the maze, and the limits of the 
		hall they are running through! */
	virtual QGAMES::Vector changeMovementDirection ();
	virtual QGAMES::Vector movementDirection ()
							{ return (_lastOrientation); }
	/** When sabreman hits rhino, then it changes ts direction. */
	virtual void whatToDoWhenSabremanHitsIt ();

	protected:
	// Implementation...
	QGAMES::Vector _lastOrientation;
	bool _toRunAwayFromSabreman;
};

/** The behaviour of the hippo monster. 
	It is similar to rhino when hippo moves, but it has
	the possibility of taking a relax, after a couple of steps! */
class HippoBehaviour : public RhinoBehaviour
{
	public:
	HippoBehaviour (SabreWulfMonster* m, Sabreman* s)
		: RhinoBehaviour (m, s, QGAMES::Vector::_cero),
		  _toStartToRun (false)
							{ }
	/** The hipp moves like rhino, except when it is waiting. 
		The initial movement is random then. */
	virtual QGAMES::Vector changeMovementDirection ();
	virtual QGAMES::Vector movementDirection ()
							{ return (_lastOrientation); }
	/** When sabreman hits rhino, then it changes ts direction. */
	virtual void whatToDoWhenSabremanHitsIt ();
	/** From time to time, hiip takes a rest...
		onviously if it is already running, and the opoosite is also true.
		From time to time, hippo need to run!. */
	virtual SabreWulfMonster::State newState ();

	private:
	bool _toStartToRun;
};

/** The behaviour of the wolf monster. */
class WolfBehaviour : public InmortalBehaviour
{
	public:
	WolfBehaviour (SabreWulfMonster* m, Sabreman* s, const QGAMES::Vector& lO)
		: InmortalBehaviour (m, s),
		  _lastOrientation (lO)
							{ }

	/** Wolf moves like rhino whilst sabreman is not in the same line of movement.
		But when he is, the speed of the movement increments a little bit
		to pursuit him. */
	virtual QGAMES::Vector changeMovementDirection ();
	virtual QGAMES::bdata changeMovementSpeedFactor ();
	virtual QGAMES::Vector movementDirection ()
							{ return (_lastOrientation); }
	virtual bool couldBeDamagedBySabreman () const
							{ return (false); }

	private:
	// Implementation...
	QGAMES::Vector _lastOrientation;
};

/** The behaviour of the spearman monster. */
class SpearmanBehaviour : public InmortalBehaviour
{
	public:
	SpearmanBehaviour (SabreWulfMonster* m, Sabreman* s);

	virtual bool canMove (const QGAMES::Vector& or, const QGAMES::Vector& a);
	/** Spearman pursuit in some way sabreman.
		When sabreman is near an spearman, it starts to pursuit sabreman.
		Unless it's been hit. In that case, it starts to
		run away from sabreman for a while. the behaviour starts back. */
	virtual QGAMES::Vector changeMovementDirection ();
	virtual int numberStepsBeforeAsking () const
							{ return (10); } // New direction is calculated every step!
	/** When an spearman is running away from sabreman, the speed is a little bit
		higher that it used to be. The normal speed is the normal rate of sabreman. */
	virtual QGAMES::bdata changeMovementSpeedFactor ();
	virtual QGAMES::Vector movementDirection ();
	/** When sabreman hits spearman, it starts to runaway. */
	virtual void whatToDoWhenSabremanHitsIt ();
	/** Draw the path. */
	virtual void drawOn (QGAMES::Screen* s, const QGAMES::Position& p);

	private:
	QGAMES::Vector randomMovement ();
	/** Pursuing sabreman or not?
		Moving using the las old path but in the opposite way. */
	QGAMES::Vector randomDirection (bool pS = false, bool rA = false);
	/** To know whether sabreman is or not near of the spearman. */
	bool isSabremanNear ();
	/** To confirm a direction already calculated. 
		The method receives the orientation calculated, and the position of the next point. */
	QGAMES::Vector confirmDirection (const QGAMES::Vector& o,
		const QGAMES::Position& nP = QGAMES::Position::_cero);

	private:
	/** Represents the different state sabreman can be. */
	enum State { _RANDOM = 0, _RUNNINGAWAY, _PURSUIT };
	State _state;
	State decideState ();

	// Implementation
	bool _hit;
	int _counterRunningAway;
	std::vector <QGAMES::Vector> _movDirs;
	bool _firstTimeInState;
	std::map <int, SabreWulfScene::Path> _path, _oldpath;
	int _lastMazePlaceVisited;
	int _positionToVisit;
	int _lastPositionVisited;
	QGAMES::Vector _lastDirection;
	int _ax, _ay;
	int _countAx, _countAy;
	int _incX, _incY;
	bool _biggerX;
	int _counterRandom;
	bool _randomMov;
};

#endif
  
// End of the file
/*@}*/