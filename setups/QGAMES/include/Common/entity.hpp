/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: entity.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines a class to represent an entity. \n
 *				 An entity is something being part of the game able to be moved. \n
 *				 An score object is a fixed entity in the game drawn over all. \n
 *				 Usually they are used to represent elements in the score system of the game. \n
 *				 A character is usually an entity managed by the user, able to move by its own. \n
 *				 A composite object is something made up of several of them.
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ENTITY__
#define __QGAMES_ENTITY__

#include <Common/element.hpp>
#include <Common/charactersteps.hpp>
#include <Common/form.hpp>
#include <Common/movement.hpp>
#include <Common/definitions.hpp>
#include <Common/rectangle.hpp>
#include <map>

namespace QGAMES
{
	/** 
	  *	An entity is like an "sprite" shown in the game. So it has forms and aspects in its base. \n
	  *	The concept "form" represents a set of "aspects", and the "aspect" represents a specific drawing for the entity.
	  *	So during Entity's life-cycle form and aspect can change.
	  *	A new method called updatePositions is added to do so and managed by the game. \n
	  *	The entities can also move. They include a reference to a position and a orientation and methods to control its movement.
	  *	The updatePositions method should control also how those position and orientation are changed. \n
	  *	The Entity defines also a set of potential movements, and a reference to the current one that can be change. \n
	  *	And finally an Element is also defined by physical variables (weight, size,...) an properties (color,...). 
	  *	All those structural attributes commented above are grouped in an internal structure called "data". \n
	  *	To simplify the way the forms and aspects of an entity changes along the game, the Animation class is defined. 
	  *	An entity can have many Animations defined but just only one active (or none) at the same time. \n
	  * When an animation is grouped with a movement a State cames up. 
	  * It is again a way to simplify how the entities are managed. The user can manage both separately.
	  */
	class Entity : public Element
	{
		public:
		typedef std::map <int, bool> Capacities;
		typedef std::map <int, double> PhysicalVariables;
		typedef std::map <std::string, std::string> Parameters;

		/** Representing how the entity aspect changes along the game. \n
			It can be overloaded by the user, 
			but a default way of making Animations (based on periodical changes in the aspect of a form) is defined. */
		class Animation
		{
			public:
			typedef std::map <std::string, std::string> Parameters;

			friend Entity;

			Animation (int id, const Parameters& prms)
				: _id (id),
				  _entity (NULL),
				  _parameters (prms)
							{ }

			virtual ~Animation ()
							{ }

			/** To clone the animation. \n
				It has to be redefined in every class inhereting this one. */
			virtual Animation* clone () const = 0;

			/** Add all parameters (openValues).
				It can be overloaded later as new data classes are created inheriting from this one. 
				By default the don't do anything. */ 
			virtual SetOfOpenValues asSetOfOpenValues () const
							{ return (SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESANIMATIONTYPE__))); }
			virtual void fromSetOfOpenValues (const SetOfOpenValues& cfg)
							{ assert (cfg.name () == std::string (__QGAMES_RUNTIMEVALUESANIMATIONTYPE__)); }

			int id () const
							{ return (_id); }

			const Parameters& parameters () const
							{ return (_parameters); }
			void setParameters (const Parameters& prms)
							{ _parameters = prms; }
			const std::string parameter (const std::string& prmName) const;
			bool existParameter (const std::string& prmName) const;
			void setParameter (const std::string& prmName, const std::string& val);
			void setParameter (const std::string& prmName, int val)
							{ setParameter (prmName, std::to_string (val)); }
			void setParameter (const std::string& prmName, float val)
							{ setParameter (prmName, std::to_string (val)); }
			void setParameter (const std::string& prmName, bool val)
							{ setParameter (prmName, std::to_string ((int) val)); }
			int parameterAsInt (const std::string& prmName) const
							{ return (std::stoi (parameter (prmName))); }
			float parameterAsFloat (const std::string& prmName) const
							{ return (std::stof (parameter (prmName))); }
			bool parameterAsBool (const std::string& prmName) const
							{ return (std::stoi (parameter (prmName)) != 0); }

			/** To indicate whether the animation links automatically with other or not. 
				By default it returns __MININT__ wich means none. */
			virtual int link () const
							{ return (__MININT__); }

			/** To know whether the animation is at the beginning. 
				It must be redefined in classes inheriting this one. */
			virtual bool beginning () const = 0;
			/** To indicate whether is the end of the animation. 
				It has to be redefined in every class inheriting this one. */
			virtual bool end () const = 0;

			protected:
			/** To set the entity.
				This is done by the Entity when a new animation is added. */
			void setEntity (Entity* ety)
							{ _entity = ety; }

			// The life-cycle of the animation
			/** The initialization. \n
				It is invoked when the animation is set at Entity level (@see setAnimation method).
				By default it does nothing. */
			virtual void initialize ()
							{ }
			/** To inherit the animation state from another one previous, if any.
				By default nothing is done. */
			virtual void inheritStateFrom (Animation* a)
							{ }
			/** Update the animation per loop. \n
				Invoked from updatePositions at Entity level (if an animation is set).
				By default it does nothing. */
			virtual void updatePositions ()
							{ }
			/** The finalization. When the animation finishes. \n
				Invoked from finalize at Entity level (if an animation is set).
				By default it does nothing. */
			virtual void finalize ()
							{ }

			protected:
			int _id;
			Entity* _entity;
			Parameters _parameters;
		};

		typedef std::map <int, Animation*> Animations;

		/** Where there is no animation at all.
			It doesn't mean a state can not exist. */
		class NoAnimation : public Animation
		{
			public:
			NoAnimation (int id)
				: Animation (id, Parameters ())
							{ }

			/** @see parent. */
			virtual Animation* clone () const
							{ return (new NoAnimation (_id)); }

			/** @see parent. */
			// The animation never is at the beginning...
			virtual bool beginning () const
							{ return (false); }
			// The animation always has finished...
			virtual bool end () const
							{ return (true); }
		};

		/** An empty animation, just to use only the parameters. 
			Certain type of engines could use it. (@see OGREE). */
		class EmptyAnimation : public Animation
		{
			public:
			EmptyAnimation (int id, const Parameters& prms)
				: Animation (id, prms)
							{ }

			/** @see parent. */
			virtual Animation* clone () const
							{ return (new EmptyAnimation (_id, _parameters)); }

			/** @see parent. */
			// The animation always is at the beginning...
			virtual bool beginning () const
							{ return (true); }
			// The animation never finishes...
			virtual bool end () const
							{ return (false); }
		};

		/** The standard animation based on changes in the form and aspect. \n
			The form, the aspect, the speed of the change, etc... are defined in parameters. 
			The standard entity builder reads them the entity definition file (@see EntityBuilder). */
		class FormAnimation : public Animation
		{
			public:
			FormAnimation (int id, const Parameters& prms)
				: Animation (id, prms),
				  _counterAspect (NULL),
				  _counterToChange (NULL),
				  _beginning (true),
				  _end (false)
							{ }

			virtual ~FormAnimation ();

			/** @see parent. */
			virtual Animation* clone () const
							{ return (new FormAnimation (_id, _parameters)); }

			/** @see parents. \
				Add the information of the counters (setOfOpenValue). */ 
			virtual SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const SetOfOpenValues& cfg);

			// To change the internal attributes
			int formId () const
							{ return (parameterAsInt (__QGAMES_ENTITYANATTRFORM__)); }
			void setFormId (int f)
							{ setParameter (__QGAMES_ENTITYANATTRFORM__, f); }
			int initialAspectId () const
							{ return (parameterAsInt (__QGAMES_ENTITYANATTRINITIALASPECT__)); }
			void setInitialAspectId (int iA)
							{ setParameter (__QGAMES_ENTITYANATTRINITIALASPECT__, iA); }
			int finalAspectId () const
							{ return (parameterAsInt (__QGAMES_ENTITYANATTRFINALASPECT__)); }
			void setFinalAspectId (int fA)
							{ setParameter (__QGAMES_ENTITYANATTRFINALASPECT__, fA); }
			bool cyclic () const
							{ return (parameterAsBool (__QGAMES_ENTITYANATTRCYCLIC__)); }
			void setCyclic (bool c)
							{ setParameter (__QGAMES_ENTITYANATTRCYCLIC__, c); }
			int fractionToChange () const
							{ return (parameterAsInt (__QGAMES_ENTITYANATTRFRACTIONTOCHANGE__)); }
			void setFractionToChange (int fc)
							{ setParameter (__QGAMES_ENTITYANATTRFRACTIONTOCHANGE__, fc); }
			int link () const
							{ return (existParameter (__QGAMES_ENTITYANATTRLINK__) 
									? parameterAsInt (__QGAMES_ENTITYANATTRLINK__) : Animation::link ()); } // Optional
			void setLink (int lk)
							{ setParameter (__QGAMES_ENTITYANATTRLINK__, lk); }
			
			/** @see parent. */
			virtual bool beginning () const
							{ return (_beginning); }
			virtual bool end () const
							{ return (_end); }

			protected:
			/** @see parent. */
			virtual void initialize ();
			virtual void inheritStateFrom (Animation* a); // Take care when using this method...
			virtual void updatePositions ();

			/** Exit to do something when an specific aspect is reached. 
				By default, there is nothing to do. */
			virtual void onAspect (int)
							{ }

			protected:
			// Implementation
			Counter* _counterAspect;
			Counter* _counterToChange;
			bool _beginning;
			bool _end;
		};

		/** To represent different states of the entity along the game. \n
		  * The state is a combination of an animation and a movement by default,
		  *	but it can be extended to cover more things. \n
		  *	Again a state can have parameters. By default two parameters can be provided (@see definitions.hpp): \n
		  *	__QGAMES_ENTITYSTATEPRMDIRECTION__ with the direction of the movement. \n
		  *	__QGAMES_ENTITYSTATEPRMACCELERATION__ with the acceleration. \n
		  * These two parameters are read by the default EntityBuilder (@see EntityBuilder).
		  * An State only should set the animation and the movement, but it shouldn't actualize them per loop.
		  * This is done direclty by the entity in the method updatePositions. 
		  * So the method updatePositions is to update only the state itself if it is needed (can be overloaded).
		  */
		class State
		{
			public:
			typedef std::map <std::string, std::string> Parameters;

			friend Entity;

			State (int id, int aId, int mId, const Parameters& prms)
				: _id (id), _animationId (aId), _movementId (mId),
				  _entity (NULL),
				  _parameters (prms)
							{ }

			virtual ~State ()
							{ }

			/** To clone the state. 
				The default state is useful, so a clone method is defined and valid. */
			virtual State* clone () const
							{ return (new State (_id, _animationId, _movementId, _parameters)); }

			/** @see parents. \
				Adds the runtim information of the state. By default, nothing is done. */ 
			virtual SetOfOpenValues asSetOfOpenValues () const
							{ return (SetOfOpenValues (std::string (__QGAMES_RUNTIMEVALUESSTATETYPE__))); }
			virtual void fromSetOfOpenValues (const SetOfOpenValues& cfg)
							{ assert (cfg.name () == std::string (__QGAMES_RUNTIMEVALUESSTATETYPE__)); }

			int id () const
							{ return (_id); }
			int animationId () const
							{ return (_animationId); }
			int movementId () const
							{ return (_movementId); }
			const Parameters& parameters () const
							{ return (_parameters); }

			protected:
			/** To set the entity.
				This is done by the Entity when a new state is added. */
			void setEntity (Entity* ety)
							{ _entity = ety; }

			// The life-cycle
			/** The initialization. \n
				It is invoked when the state is set at Entity level (@see updatePositions method there). \n
				By default the animation and the movement are again set,
				unless the parameter passed is true (cA = Continue Animation?). \n
				When the animation and the state are set, they are initialized. */
			virtual void initialize (bool cA = false);
			/** The update per loop. \n
				Invoked in updatePositions at Entity level (@see updatePositions methid there). \n
				By default nothing is done as everything id done in the entity loop. */
			virtual void updatePositions ()
							{ }
			/** Finalize the state.
				Finalize both the animation and the movement. */
			virtual void finalize ();

			protected:
			int _id;
			int _animationId;
			int _movementId;
			Entity* _entity;
			Parameters _parameters;
		};

		typedef std::map <int, State*> States;

		struct Data
		{
			Data ()
				: _position (),
				  _orientation (),
				  _capacities (),
				  _physics (),
				  _parameters (),
				  _movements ()
							{ }

			Data (const Position& pos, const Vector& o, 
					const Capacities& c, const PhysicalVariables& pV, const Parameters& prms, const Movements& movs)
				: _position (pos),
				  _orientation (o),
				  _capacities (c),
				  _physics (pV),
				  _parameters (prms),
				  _movements (movs)
							{ }

			// Operator (structural data has to be equal)
			bool operator == (const Data& dt) const;
			bool operator != (const Data& dt) const
							{ return (!(*this == dt)); }

			/** Entity's position. */
			Position _position; 
			/** The entity is pointing to a place following that vector. */
			Vector _orientation; 
			/** Capacities can be variables to indicate whether the 
				entity can or not do something, such as flying i.e. */
			Capacities _capacities; 
			/** Physycal variables defining the entity, such as weight, size,... */
			PhysicalVariables _physics; 
			/** Any other parameter interesting in the defintion of the entity (and strings). */
			Parameters _parameters; 
			/** What movements the entity can do. */
			Movements _movements; 
		};

		Entity (int eId, const Forms& f = Forms (), const Data& d = Data ());
		~Entity (); // The entity owns its animations...

		/** In some situations could be interesting to replicate an entity.
			e.g: The entity represents a bullet that can be shooted. \n
			It could be interesting to replicate the bullet if many of them
			can be shooted at the same time, because their behaviour will be quite similar later. \n
			Take care with the id you assign. If you are going to register the entity in the game it has to be unique. \n
			Additionally you are accountable to delete the entity cloned as 
			it hasn't been created using the builder. Bearing this in mind. */
		virtual Entity* clone () const = 0;

		/** To clone the animations managed by the entity. \n
			As they are not part of the entity (like the original ones are), 
			the one invoking this method is accountable to destroy them. \n
			Usually this method is invoked from the method clone of the entity (@see above). */
		Animations cloneAnimations () const;
		/** To clone the states managed by the entity. \n
			As they are not part of the entity (like the original ones are), 
			the one invoking this method is accountable to destroy them. 
			Usually this method is invoked from the method clone of the entity (@see above). */
		States cloneStates () const;

		/** @see parent. \n
			The information added is: \n
			The state id (openValue), the position (openValue),
			and the information comming from the movement (setOfMovements) 
			if any or empty if there is no movement. */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);
		
		const Forms& forms () const
							{ return (_forms); }
		const Data& data () const
							{ return (_data); }
		const Position& position () const
							{ return (_data._position); }
		void setNotifyWhenChangePosition (bool n)
							{ _notifyWhenChangePosition = n; }
		/** When the position changed, a notification (__QGAMES_POSITIONCHANGED__) 
			is thrown if the switch to do so is on (@see above). \n
			This is the default behaviour but it can be changed (only for this attribute). */
		virtual void setPosition (const Position& p)
							{ _data._position = p; 
							  if (_notifyWhenChangePosition) notify (Event (__QGAMES_POSITIONCHANGED__, this)); }
		/** To set the position of the entity from the position of the base. 
			This is tipically done in platform games. */
		void setPositionOnBase (const Position& p) // The position of the base...
							{ setPosition (p - (basePosition () - position ())); }
		const Vector& orientation () const
							{ return (_data._orientation); }
		void setOrientation (const Vector& o)
							{ _data._orientation = o; }

		// To know different parts of the entity
		/** Methods to locate special zones of the entity based on the current form and aspect (@see Form). \n
			All of them are used in many other parts of the game. e.g. to detect collisions. \n
			They can be overloaded. e.g. in a particular game the basePosition could be a corner of the object, 
			instead the center of the bottom part. \n
			Taking a 3D form as example, it could be contained into a box, so: */
		/** The center of the box. */
		virtual Position centerPosition () const;
		/** The center of bottom rectangle part of the box rounding the object. */
		virtual Position basePosition () const;
		/** A rectangle rounding the central part of the object. */
		virtual Rectangle roundingFormZone () const;
		/** The top rectangle of the box rounding the object. */
		virtual Rectangle coverZone () const;
		/** The bottom rectangle of the box rounding the object. */
		virtual Rectangle baseZone () const;
		/** The nox rounding the object. */
		virtual Box roundingBox () const;
		/** How length (figure out the x axis) is the box rounding the object? */
		virtual int visualLength () const;
		/** How height (figure out the y axis) is the box rounding the object? */
		virtual int visualHeight () const;
		/** How depth (figure out the z axis) is the box rounding the object? */
		virtual int visualDepth () const;
		/** All the current dimensions in a single instruction. */
		void currentVisualDimensions (int& vL, int& vH, int& vD)
							{ vL = visualLength (); vH = visualHeight (); vD = visualDepth (); }
		/** Considering all different forms and aspects that the object can adopt, what are the max dimensions?
			This is useful to know in some contexts. */
		void maxVisualDimensions (int& vL, int& vH, int& vD) const;

		// Managing the movements
		void setMovements (const Movements& m)
							{ _data._movements = m; }
		const Movements& movements () const
							{ return (_data._movements); }
		Movement* currentMovement () 
							{ return (_currentMovement); }
		const Movement* currentMovement () const
							{ return (_currentMovement); }
		/** If the parameter is __MININT__, then
			there will not be movement in place. Otherwise the movement has to exist. */
		void setCurrentMovement (int nM);
		/** To know whether can or not move depending on their current situation. \n
			The direction and the acceleration received as parameters have to be considered as "potential".
			By default any movement is always possible. */
		virtual bool canMove (const Vector& d, const Vector& a)
							{ return (true); }

		// To manage the forms
		void setForms (const Forms& f)
							{ _forms = f; }
		Form* currentForm ()
							{ return (_currentForm); }
		const Form* currentForm () const
							{ return (_currentForm); }
		void setCurrentForm (int nf);
		int currentAspect () const
							{ return (_currentAspect); }
		void setCurrentAspect (int cA)
							{ _currentAspect = cA; }

		// To manage the animations
		void addAnimation (Animation* a);
		bool existAnimation (int id);
		Animation* animation (int id);
		const Animation* animation (int id) const;
		const Animations& animations () const
							{ return (_animations); }
		void setAnimations (const Animations& as)
							{ deleteAnimations (); _animations = as; }
		void setCurrentAnimation (int nA);
		Animation* currentAnimation ()
							{ return (_currentAnimation); }
		const Animation* currentAnimation () const
							{ return (_currentAnimation); }

		// To manage the states
		void addState (State* st);
		bool existState (int id);
		State* state (int id);
		const State* state (int id) const;
		const States& states () const
							{ return (_states); }
		void setStates (const States& sts)
							{ deleteStates (); _states = sts; }
		void setCurrentState (int st, bool cA = false); // Continue or nbot the animation? (false by default)
		void setCurrentStateDeferred (int st, bool cA = false); // Using the buoy...
		State* currentState ()
							{ return (_currentState); }
		const State* currentState () const
							{ return (_currentState); }

		/** To get the hot collision zone of the entity. \n
			By default it returns the full size of the entity, taking into account its current frame. */
		virtual Rectangle collisionZone () const;
		/** To determinate whether the entity has or not collisioned with other. \n
			By default it returns true when the two collision zones overlap. */
		virtual bool hasCollisionWith (Entity* e) const;
		/** To determinate whether the entity receives is or not near of another. \n
			Two parameters are needed: The entity and the distance considered as "near". 
			In the default implementation the centers are taken into account. */
		virtual bool isNearOf (Entity* e, bdata d) const;

		/** The entities acts across the game. \n
			Their positions / aspect should be actualized in its life-cycle.
			This is the right method to do so. \n
			By default the actualization is delegated into a external engine, if any. */
		virtual void updatePositions ();
		/** @see parent. 
			The current frame will be drawn by default, taking into account its alpha level. \n
			If it visible or not is NOT taken into account. */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		/** To get the alpha level. */
		int alphaLevel () const
							{ return (_alphaLevel); }
		/** To control the alpha level of visualization. */
		virtual void setAlphaLevel (int aL);

		/** The entity represents something "alive" in the game. \n
			Then, the entity can collision with other entities.
			Following method is created to manage that situation. 
			A typical behaviour should be to throw an event when the collision is detected.
			Who is observing the entity will react. \n
			By default no behaviour is defined. */
		virtual void whenCollisionWith (Entity* e)
							{ }

		private:
		void deleteAnimations ();
		void deleteStates ();

		protected:
		/** 
			A default buoy to change the state of the entity. \n
			In many circumstances, when the state is changed into the updatePosition method the system can crash.
			This is due to many reasons:
			e.g: A movement notifies something, the processEvent changes the state and also the movement associated.
			The system crashes because the old movement is unobserved and after the treatment of
			the event notified the loop dealing with the notifications becomes inconsistent and then crashes. \n
			In that circumstances the state has to be changed in the inEveryLoop method, so a Buoy should be used. \n
			This buoy is declared and set at construction time of the Entity.
			*/
		class ChangeStateBuoy : public Buoy
		{
			public:
			ChangeStateBuoy ()
				: Buoy (__QGAMES_CHANGESTATEBUOY__, (bdata) 0),
				  _targetState (0),
				  _continueAnimation (false)
							{ }

			void setTargetState (int tS)
							{ _targetState = tS; }
			void setContinueAnimation (bool cA)
							{ _continueAnimation = cA; }

			/** @see parent. */
			virtual void* treatFor (Element* e);

			private:
			int _targetState;
			bool _continueAnimation; // To continue the animation or not?
		};

		Forms _forms; /** The different forms the entity can adopt. */
		Data _data; /** The internal attributes defining the entity. */
		Animations _animations; /** The list of animations the entity has. */
		States _states; /** The list of potential states the entity has. */

		// Implementation
		Form* _currentForm;
		Movement* _currentMovement;
		Animation* _currentAnimation; /** Null if none. */
		State* _currentState; /** Null if none. */
		int _currentAspect;
		int _alphaLevel;

		bool _notifyWhenChangePosition;

		// The maximum dimensions of the entity taking into account all its different possible aspects
		// They are implementation variables. They are calculated first time and never more...
		// They are defined as mutable, so they can be changed in a const method
		mutable int _maxVisualLength, _maxVisualHeight, _maxVisualDepth;
	};

	// To simplify the definition of set of entities...
	typedef std::map <int, Entity*> Entities;

	/** A character is an Entity than can me moved externally. \n
		The "direction" of the movement is defined by a direction, and an acceleration sets in the setMove method. \n
		They have necessarily nothing to do with the orientation that defines where 
		the entity is pointing to. \n 
		The updatePositions method invokes automatically the current movement with the direction and movement set before. \n
		The character has also the possibility to add a control monitor (and owns it). */
	class Character : public Entity
	{
		public:
		Character (int cId, const Forms& f = Forms (), const Data& d = Data ())
			: Entity (cId, f, d),
			  _stepsControlMonitor (NULL),
			  _direction (Vector::_cero),
			  _acceleration (Vector::_cero)
							{ }
		~Character () // The characters owns the monitor if any
							{ delete (_stepsControlMonitor); }

		/** @see parent. \n
			The information added is: \n
			the direction (openValue) and the aceleration (openValue) of the movement (just in case),
			the monitor id (openValue) (it has to be possible to get it back fromthe the builder),
			and the information comming from that monitor (setOfOpenValues). 
			IMPORTANT: There must exist a steps monitor builder created at game level.
			Otherwise initialization will crash!! */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** To get the direction of the movement. */
		const Vector& direction () const
							{ return (_direction); }
		/** To get the acceleration of the movement. */
		const Vector& acceleration () const
							{ return (_acceleration); }
		/** Use this method to change the parameters of the current movement. */
		virtual void setMove (const Vector& d, 
			const Vector& a = Vector ((bdata) 0, (bdata) 0, (bdata) 0))
							{ _direction = d; _acceleration = a; }

		// To control the steps of the character
		/** To add a step's monitor. \n
			The same method can be used to remove it if the parameter is null. */
		void addControlStepsMonitor (CharacterControlStepsMonitor* s);
		CharacterControlStepsMonitor* stepsMonitor () // There can be only one at the same time (if any)
								{ return (_stepsControlMonitor); }

		/** Moves automatically the entity,
			using the direction, acceleration and Movement currently defined. \n
			If there is no Movement set nothing happens. 
			It also actualize the steps monitor. */
		virtual void updatePositions ();

		protected:
		CharacterControlStepsMonitor* _stepsControlMonitor;
		Vector _direction;
		Vector _acceleration;
	};

	// To simplify the definition of a set of characters...
	typedef std::map <int, Character*> Characters;

	/** The very basic character.
		It is more than enough for multiple purposes related with the animation. */
	class BasicCharacter : public Character
	{
		public:
		BasicCharacter (int cId, const Forms& f = Forms (), const Data& d = Data ())
			: Character (cId, f, d)
							{ }

		/** @see parent. */
		virtual Entity* clone () const
							{ return (new BasicCharacter (_id, _forms, _data)); }

		/** @see parent.
			Sets the first state in the list. */
		virtual void initialize ();
	};

	/** A composite entity is a set of entities joined. \n
		It has no entity by itself. \n
		Everything what is done over the composite entity has to be done
		in each of the different entities that this one is made up of. \n 
		Animation, movements and states of the composite entity makes no sense...at least initially. 
		The composite entity is the owner of the entities it manages. 
		So they are destroyed when finishes. */
	class CompositeEntity : public Entity
	{
		public:
		CompositeEntity (int cId, const Entities& e, const Data& dt = Data ())
			: Entity (cId, Forms (), dt),
			  _entities (e)
							{ }
		~CompositeEntity ()
							{ deleteEntities (); }

		/** @see parent. 
			Clone entity by entity being part of this other one. */
		virtual Entity* clone () const;

		/** @see parent. \n
			The information added is the information comming from each internal entity (setOfOpenValues). */
		virtual SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const SetOfOpenValues& cfg);

		/** To manage the entities inside. 
			Remember: The coposite entity is the owner of the entities inside. */
		void setEntities (const Entities& eties)
							{ deleteEntities (); _entities = eties; }
		const Entities& entities () const
							{ return (_entities); }

		/** @see parent. */
		/** The center of the centers. */
		virtual Position centerPosition () const
							{ return (Position::centerOf 
								(getPointsFromVisibleEntities (&Entity::centerPosition))); }
		/** The center of all bases. */
		virtual Position basePosition () const
							{ return (Position::centerOf 
								(getPointsFromVisibleEntities (&Entity::basePosition))); }
		/** A rectangle rounding all visible entities. \n
			If all entities are in the same plane the outcome will be a rectangle in the same plane that them as well. \n
			If all entities are in parallel planes, the outcome will be a rectangle in 
			the middle of the cube rounding all forms. \n
			In other circunstances, the result will be unpredictible. */
		virtual Rectangle roundingFormZone () const
							{ return (roundingBox ().betweenFrontAndBackRectangle ()); }
		virtual Rectangle coverZone () const // A little more complex than simply get the cover of the form...
							{ return (roundingBox ().topRectangle ()); }
		virtual Rectangle baseZone () const // Same...
							{ return (roundingBox ().bottomRectangle ()); }
		virtual Box roundingBox () const;
		/** Based on the previous. */
		virtual int visualLength () const
							{ return ((int) baseZone ().width ()); }
		virtual int visualHeight () const
							{ return ((int) roundingFormZone ().height ()); }
		virtual int visualDepth () const
							{ return ((int) baseZone ().height ()); }

		/** The collision in a composite entity is a rectangle grouping all the elements
			included in the entity. \n
			All entities should be in the same plane. othewise the outcome will be not valid actually. */
		virtual Rectangle collisionZone () const
							{ return (roundingBox ().betweenFrontAndBackRectangle ()); }

		/** @see parent.
			A collision happens when one entyty of the*/
		virtual bool hasCollisionWith (Entity* e) const;

		/** A composite entity is made up of several other entities. \n
			To initialize the composite entity all other entities must be initialized.
			So at this point, the list of entities must be set up. \n
			Similar behaviour with the rest of methods related with the life.cycle of the entity. */
		virtual void initialize ();
		virtual void inEveryLoop ();
		virtual void updatePositions ();
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);
		virtual void finalize ();

		/** To control the alpha level for all entities. */
		virtual void setAlphaLevel (int aL);

		/** A collision with all entities is checked. */
		virtual void whenCollisionWith (Entity* e);

		private:
		// Implementation
		// Just to reuse more code
		void deleteEntities ();
		Positions getPointsFromVisibleEntities (Position (Entity::*func)() const) const;

		protected:
		/** The list of the entities this other one is made up. */
		Entities _entities;
	};

	/** An score object is also something "alive", but as opposed to it it doesn't move (usually). 
		and typically doesn't integrate with others. \n
		The scoreObjects are also part of the game. \n
		They usually represent the situation of the game: lives remaining, points,... 
		The game main loop represents them fixed on the screen (@see Game class). */
	class ScoreObject : public Entity
	{
		public:
		ScoreObject (int cId, const Forms& f = Forms (), const Data& d = Data ())
			: Entity (cId, f, d)
							{ }

		/** score object is initially defined by its basic aspect and form... 
			The aspect defined is the 0. */
		virtual void initialize ();
	};

	/** An score digit is the minimal expression of an score object. \n
		e.g. let's figure out we have a score holding the points.
		The score digit is each of the digits of the score. */
	class ScoreObjectDigit : public ScoreObject
	{
		public:
		ScoreObjectDigit (int cId, const Forms& f = Forms (), const Data& d = Data ())
			: ScoreObject (cId, f, d)
								{ }

		virtual Entity* clone () const;
	};

	/** An score text is a score object made up of score digits 
		which aspects are get from the character code of every char in the text. \n
		The text needs a Form with all elements to be printed. Otherwise the system will crash. */
	class ScoreObjectText : public CompositeEntity
	{
		public:
		enum TextDirection
		{
			_HORIZONTALRTOL = 0,
			_VERTICALUTOD = 1,
			_HORIZONTALLTOR = 2,
			_VERTICALDTOU = 3
		};

		/** When the aspect for every digit is calculated from the char code of every letter in the text, 
			a frame base can be taken into account. \n
			e.g. Let's figure out the text is "AA", then the aspect of the 2 digits being part of the composition will be
			65 and 65 (the ASCII code for A letter in UNICODE). 
			A form with at least 65 different aspects defined would be needed. 
			But only the aspect 0 is need if the parameter fB is set to 65. \n
			Take this into consideration when defining score text objets. */
		ScoreObjectText (int id, const std::string& t = std::string (__NULL_STRING__),
			const Forms& f = Forms (), int fB = 0);

		/** @ see parent. */
		virtual int visualLength () const;
		virtual int visualHeight () const;

		/** To change the text printed out. */
		const std::string& text () const
							{ return (_text); }
		/** When the text is set the digits are created (different entities, score digits) taking into account 
			things like the orientation of the text, and the seperation among the digits defined. */
		void setText (const std::string& t);

		int space () const // Between digits...
							{ return (_space); }
		void setSpace (int p)
							{ _space = p; }
		TextDirection textDirection () const
							{ return (_textDirection); }
		void setTextDirection (TextDirection d)
							{ _textDirection = d; }
		int textLength () const // In characters, not inpixels...
							{ return (_text.length ()); }
		int maxTextLength () const
							{ return (_maxTextLength); }
		void setMaxTextLength (int mL);

		/** @see parent. */
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);

		protected:
		void fixInternalEntities ();
		/** To get the right form to draw something. 
			Probably the font will be different depending on the orientation of the text,
			or any other property impossible to determinate right now. */
		virtual int formToDrawLetters () = 0;

		protected:
		std::string _text;
		int _maxTextLength;
		int _space;
		TextDirection _textDirection;
		int _frameBase;
	};

	/** An score number is an score object made up of score digits. \n
		The behaviour is like ScoreObjectText, but from number intead from a text. */
	class ScoreObjectNumber : public CompositeEntity
	{
		public:
		enum NumberDirection
		{
			_HORIZONTALRTOL = 0,
			_VERTICALUTOD = 1,
			_HORIZONTALLTOR = 2,
			_VERTICALDTOU = 3
		};

		ScoreObjectNumber (int id, int n = 0, 
				const Forms& f = Forms (), int fB = 0);

		/** @ see parent. */
		virtual int visualLength () const;
		virtual int visualHeight () const;

		int number () const
							{ return (_number); }
		void setNumber (int n);
		char ceroAtLeft () const
							{ return (_ceroAtLeft); }
		void setCeroAtLeft (char nF)
							{ _ceroAtLeft = nF; }
		int space () const // Among digits...
							{ return (_space); }
		void setSpace (int p)
							{ _space = p; }
		NumberDirection textDirection () const
							{ return (_numberDirection); }
		void setTextDirection (NumberDirection d)
							{ _numberDirection = d; }
		int numberLength () const // in number of characters...
							{ return (_numberLength); }
		void setNumberLength (int nL);

		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint);

		protected:
		void fixInternalEntities ();
		/** To get the right form to draw something. 
			Probably the font will be different depending on the orientation of the number,
			or any other property impossible to determinate right now. */
		virtual int formToDrawDigits () = 0;

		protected:
		int _number;
		char _ceroAtLeft;
		int _space;
		int _numberLength;
		NumberDirection _numberDirection;
		int _frameBase;

		// Implementation
		std::string _numberAsAText;
	};
}

// To simplify the access to entities and characters...
#define __ET	(QGAMES::Entity*)
#define __CH	(QGAMES::Character*)

#endif
  
// End of the file
/*@}*/
