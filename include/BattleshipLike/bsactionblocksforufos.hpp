/** \ingroup Battleship */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: bsactionblocksforufos.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 02/11/2019 \n
 *	Description: Define specific action blocks for ufos in Battleship like games.
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_ACTIONBLOCKSFORUFOS__
#define __BATTLESHIP_ACTIONBLOCKSFORUFOS__

#include <BattleshipLike/bsactionblocks.hpp>

namespace BATTLESHIP
{
	/** General definition of a standard action block to control UFOs. \n
		It is little more "expandable" than the previous ones. \n
		All UFOs will move using a QGAMES::FollowingACurveMovement (@see). \n
		The class owns a Curve Factory object (received at construction time). \n
		This object is used to create to set the initial position of the UFO in the screen
		and also to create the specific curve (template) of the movement.\n
		Both parameters will be assigned regarding the movementType parameter. */
	class StdUFOSceneActionBlock : public StdSpaceElementsSceneActionBlock
	{
		public:
		struct Properties : public StdSpaceElementsSceneActionBlock::Properties
		{
			public:
			Properties ()
				: StdSpaceElementsSceneActionBlock::Properties (),
				  _type (-1),		 // Means random between 0 and the max types allowed (defined in DataGame)
				  _movementType (-1), // Meaning random...
				  _movementTypeParameters (), // Meaning the default ones per type or curve
				  _typeShooting (0), // Means the basic one, -1 would mean random
				  _timeToShoot (1),  // Negative will meand random up to this time
				  _probabilityToShoot (50), // Between 0 and 100
				  _shootingForce (1), // The very basic force for the shooting
				  _speed (__BD 1)	 // It will multiply the basic speed 
									 // (and without taken into account the speed of the background ifself)
									 // A negative value would mean random up to that number (in positive)
									 // The difficulty and level of the game aren't taking into account here
							{ }

			Properties (QGAMES::bdata tA, int pA, bool tE, QGAMES::bdata tM, bool r, bool rll,
						const QGAMES::Position& pos, const QGAMES::Rectangle& mZ, QGAMES::bdata mTA,
						int eT, int mT, const std::map <int, double>& mTP, 
						int sT, QGAMES::bdata ttS, int pTT, int sF, QGAMES::bdata s);

			/**
			  *	<Attributes> \n
			  *		<Attribute id="TimeToAppear" value="-|+"/> \n
			  *		<Attribute id="ProbabilityToAppear" value="0..100"/> \n
			  *		<Attribute id="Emerge" value="YES|NO" \n
			  *		<Attribute id="TimeToMove" value="-|+"/> \n
			  *		<Attribute id="Regenerate" value="YES|NO"/> \n
			  *		<Attribute id="Roll" value"YES|NO"> \n
			  *		<Attribute id="InitialPosition" value="x,y,z"/> \n
			  *		<Attribute id="MovZone" value="x1,y1,z1,x2,y2,z2"/> \n
			  *		<Attribute id="MaxTimeAction" value="-1|0.."> \n
			  *
			  * ...adding these ones for the UFO control:
			  *
			  *		<Attribute id="Type" value="-1|"/> \n
			  *		<Attribute id="MovType" value="-1|0..."/> \n
			  *		<Attribute id="MovTypeParameters" value="XX,XX,..."/> /n
			  *		<Attribute id="TypeShooting" value="-1|0..."/> \n
			  *		<Attribute id="TimeToShoot" value="-|+"/> \n
			  *		<Attribute id="ProbabilityToShoot" value="0..100"/> \n
			  *		<Attribute id="ShootingForce" value="-,+"/> \n
			  *		<Attribute id="Speed" value="-,+"/> \n
			  *	</Attributes>
			  */
			Properties (const std::map <std::string, std::string>& prps);

			/** @see parent. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			int _type; 
			int _movementType; 
			std::map <int, double> _movementTypeParameters;
			int _typeShooting; 
			QGAMES::bdata _timeToShoot;
			int _probabilityToShoot;
			int _shootingForce;
			QGAMES::bdata _speed; 
		};

		/**
		  *	Important factory class to get: \n
		  *	1.- The initial point of an UFO and \n
		  *	2.- The curve folowed in the movement. \n
		  *	To get both data the parameters of the block are taken into account. \n
		  * One instance of this class has to be passed as parameter of the constructor.
		  *	The class can be inherited later for mnore definitions.
		  * The default implementation takes into account only the type of movement defined in the attribute "prps", 
		  *	in the following sense: \n
		  *	__BATTLESHIP_UFOATTRTYPEMOVLINEAR0__	: Line from the upper part of the window to the bottom part of it. \n
		  *	__BATTLESHIP_UFOATTRTYPEMOVLINEAR1__	: Line from the left part of the window to the right part of it . \n
		  *	__BATTLESHIP_UFOATTRTYPEMOVCIRCULAR0__	: Circular movement.\n
		  *	__BATTLESHIP_UFPATTRTYPEMOVEDANCING0__	: Moving horizontally a little bit. \n
		  *	__BATTLESHIP_UFPATTRTYPEMOVEDANCING1__	: Moving vertically a little bit. \n
		  * __BATTLESHIP_XYUFOATTRTYPEATTACKING0__	: Circle to the righ and then a line to the bottom. \n
		  * __BATTLESHIP_XYUFOATTRTYPEATTACKING1__	: Idem, but with the circle to the left. \n
		  * __BATTLESHIP_XYUFOATTRTYPEATTACKING2__	: Line, circle and line again. \n
		  * __BATTLESHIP_XYUFOATTRTYPEATTACKING3__	: Line, an arc and line again. \n
		  * __BATTLESHIP_XYUFOATTRTYPEATTACKING4__	: A sinusoide.
		  * The initial position will be either the one defined in the attribute "prms" or a random one (it that is not defined)
		  *	according to the type of movement. \n
		  * The movements followed by an UFO is one of the other most important configurable things within the gamne. \n
		  * The methods initialPositionFor and curveFor translate the movemento into a curve. \n
		  * The translation between one and other are defined also in the DataGame class and also are defined there
		  * the default parameters of the movement. @see DataGame.
		  */
		class CurveAndInitialPositionFactory
		{
			public:
			virtual ~CurveAndInitialPositionFactory ()
							{ }

			/** To clone the factory. */
			virtual CurveAndInitialPositionFactory* clone ()
							{ return (new CurveAndInitialPositionFactory); }

			/** 
			  * To get the initial position of an UFO, taking into account the properties of the action block. \n
			  *	@params i	: The number of UFO. 
			  *	@params ufo	: A reference to the UFO. 
			  * @returns	: The curve to follow. Be carefull. It can be null.
			  */
			virtual QGAMES::Position initialPositionFor (int i , UFO* ufo, Properties* prps);

			/** Same with the curve followed by the UFO. 
				The method receives (potentially) a set of paremeters than can be used to apply to the curve. \n
				That information can only be modified. \n
				The standard behaviour used those parameters if they have been provided, and set them if haven't. \n
				The method can also set the parameters used to create the curve. \n
				The same behaviour in any class inheriting this one is suggested. \n. 
				This method used the DataGame. */
			virtual QGAMES::FollowingACurveMovement::CurveTemplate* curveFor 
				(int i, UFO* ufo, Properties* prps, std::map <int, double>& crvprms);
		};

		StdUFOSceneActionBlock (int id, Properties* prps, int nU = 1, 
			CurveAndInitialPositionFactory* cF = new CurveAndInitialPositionFactory);

		virtual ~StdUFOSceneActionBlock ()
							{ delete (_curvePositionFactory); _curvePositionFactory = NULL; }

		/** To get or to change the curve and position factory. */
		CurveAndInitialPositionFactory* curvePositionFactory () 
							{ return (_curvePositionFactory); }
		const CurveAndInitialPositionFactory* curvePositionFactory () const
							{ return (_curvePositionFactory); }
		void setCurvePositionFactory (CurveAndInitialPositionFactory* cf)
							{ assert (cf); delete (_curvePositionFactory); _curvePositionFactory = cf; }

		/** To change externally the type of movement, but never the position where the UFO is in. 
			The parameter passed is the type of movement. \n
			This method can be used by the action block managing a set of UFO when someone of them is 
			desired to start an attack e.g. \n
			The second parameter referes to a the speed factor to apply to to the new movement respect the original one. 
			By default 1 meaning, appliying the same. \n
			The method can also receive a set of parameters to apply to the curve.
			Those parameters can be modified. 
			The default behaviour uses those parameters is they have been provided and set them if not. */
		virtual void toChangeMovement (int tM, QGAMES::bdata sF, std::map <int, double>& crvprms);

		/** @see parent. 
			Just to include the possibility to fire. */
		virtual void initialize ();
		virtual void updatePositions ();

		protected:
		/** @see parent. */
		virtual void getAndConfigureSpaceElements (const std::vector <int>& elmts = std::vector <int> ());
		virtual void startToMoveSpaceElements ();

		/** @see parent. */
		virtual std::function <bool (SpaceElement*)> isElementValidForActionBlock () const
							{ return ([](SpaceElement* spc) -> bool 
								{ return (spc != NULL && 
										  dynamic_cast <UFO*> (spc) != NULL &&
										  !spc -> isVisible ()); }); } 

		protected:
		/** The curve factory used to create the movements of the UFO. */
		CurveAndInitialPositionFactory* _curvePositionFactory;
		/** The number of UFOs controlled by the action block. */
		int _numberOfUFOS;

		static const int _COUNTERTOFIRE = 4;
	};

	/** An action block to control a set of UFOs (individual action blocks). \n
		All UFOs in the set should move in one combat formation. \n
		The combat formation determines the behaviour for each individual UFO and what to do with them. \n
		The parameters defining the set are quite similar to the individual ones. 
		The constructor receives a combat formation factory and own it. */
	class StdSetUFOsSceneActionBlock : public SceneActionBlock
	{
		public:
		/** The properties are like the ones defining a single UFO plus:
			the combat formation, the number of UFOs in the set and the time to take a decisión about what to do which each UFO. */
		struct Properties : public StdUFOSceneActionBlock::Properties
		{
			public:
			Properties ()
				: StdUFOSceneActionBlock::Properties (),
				  _combatFormation (0), 
				  _combatFormationAttrs (),
				  _combatFormationAttackAttrs (),
				  _numberUFOs (2), // The minimum...
				  _timerEvery (1), // Every second by default...
				  _timerForMany (-1), // For all by default..
				  _timerProbability (100) // Always by default...
							{ }

			Properties (QGAMES::bdata tA, int pA, bool tE, QGAMES::bdata tM, bool r, bool rll,
						const QGAMES::Position& pos, const QGAMES::Rectangle& mZ, QGAMES::bdata mTA,
						int eT, int mT, const std::map <int, double>& mTP,
						int sT, QGAMES::bdata ttS, int ppT, int sF, QGAMES::bdata s, 
						int cT, const std::vector <QGAMES::bdata>& cTAttrs, const std::map <int, double>& cTAAttrs,
						int nU, QGAMES::bdata tt, int ttM, int ttP);

			/**
			  *	<Attributes> \n
			  *		<Attribute id="TimeToAppear" value="-|+"/> \n
			  *		<Attribute id="ProbabilityToAppear" value="0..100"/> \n
			  *		<Attribute id="Emerge" value="YES|NO" \n
			  *		<Attribute id="TimeToMove" value="-|+"/> \n
			  *		<Attribute id="Regenerate" value="YES|NO"/> \n
			  *		<Attribute id="Roll" value"YES|NO"> \n
			  *		<Attribute id="InitialPosition" value="x,y,z"/> \n // Orientative
			  *		<Attribute id="MovZone" value="x1,y1,z1,x2,y2,z2"/> \n // Orientative also
			  *		<Attribute id="MaxTimeAction" value="-1|0.."> \n
			  *
			  * ...adding these ones for the set of UFOs control:
			  *
			  *		<Attribute id="Type" value="-1|"/> \n
			  *		<Attribute id="MovType" value="-1|0..."/> \n
			  *		<Attribute id="MovTypeParameters" value="XX,XX,..."/> /n
			  *		<Attribute id="TypeShooting" value="-1|0..."/> \n
			  *		<Attribute id="TimeToShoot" value="-|+"/> \n
			  *		<Attribute id="ProbabilityToShoot" value="0..100"/> \n
			  *		<Attribute id="ShootingForce" value="-,+"/> \n
			  *		<Attribute id="Speed" value="-,+"/> \n // The basic one...
			  *		<Attribute id="CombatFormation" value="0..."/> \n
			  *		<Attribute id="CombatFormationAttrs" value="XX,XX,..."/> \n // It can be empty or event not defined
			  *		<Attribute id="CombatFormtionAttackParameters" value="XX,XX,..."/> /n
			  *		<Attribute id="NumberUFOS" value="1..."/> \n
			  *		<Attribute id="TimerEvery" value="-|+"/> \n // Not cero
			  *		<Attribute id="TimerForHowMany" value="-1|1.."/> \n // Greater than 0 (-1 means all)
			  *		<Attribute id="TimerProbability" value="0..100"/> \n
			  *	</Attributes>
			  */
			Properties (const std::map <std::string, std::string>& prps);

			/** @see parent. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			int _combatFormation;
			std::vector <QGAMES::bdata> _combatFormationAttrs;
			std::map <int, double> _combatFormationAttackAttrs;
			int _numberUFOs;
			QGAMES::bdata _timerEvery;
			int _timerForMany;
			int _timerProbability;
		};

		/** Class used to create and manipulate a combat formation. \n
			The combat formation should determine how each UFO behaves creating an action block (type UFO) per each. \n
			The method individualActionBlock is invoked for this task from the StdSetUFOsSceneActionBlock class. \n
			Every x seconds (defined in the properties of that class) a decisión about what to do with each action block
			(where one single UFO is being manipulated) should be taken. \n
			Then method whenTimerIsSparked is invoked for this task, 
			again receiving the properties of StdSetUFOsSceneActionBlock as parameter. */
		class CombatFormationFactory
		{
			public:
			typedef std::function <QGAMES::Vector (int, Properties*)> PositionFunction;
			typedef std::function <QGAMES::bdata (int, Properties*)> TimeFunction;
			typedef const std::function <StdUFOSceneActionBlock* 
				(int, StdUFOSceneActionBlock::CurveAndInitialPositionFactory*)> SingleActionBlockFunction;

			// Default functions that can be used by any combact factory...
			static const PositionFunction _SAMEPOSITION; 
			static const TimeFunction _SAMETIME;
			static const SingleActionBlockFunction __SINGLESTDUFOSCTIONBLOCK;

			/** 
			  * To create a formation:
			  * @param cF:		To create the curve factory used when creating individual action blocks.
			  *					That factory has to be clones per each!
			  * @param pFunc:	Function to determine the relative position of a UFO.
			  *					The function receives the number of the UFO (or block controlling it) 
			  *					and returns the vector to add to the initial position. 
			  * @param tFunc:	Funtion to determine the relative time to start to move of a UFO.
			  *					The function receives the number of the UFO (or block controlling it) 
			  *					and returns the time to add to the initial time.
			  * @param eABlock:	Function to create the individual action block to control the UFOS,
			  *					The function receives the number of UFO, and the curve factory needed,
			  *					and returns the block to control.
			  */
			CombatFormationFactory 
					(StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF, 
					 const PositionFunction& pFunc = _SAMEPOSITION, // By default, the same position
					 const TimeFunction& tFunc = _SAMETIME, // By default the same time
					 const SingleActionBlockFunction& eABlock = __SINGLESTDUFOSCTIONBLOCK) // By default the stdUFOActionBlock
				: _owner (NULL),
				  _curvePositionFactory (cF),
				  _positionFunction (pFunc), 
				  _timeFunction (tFunc),
				  _createEmptyIndividualActionBlockFunction (eABlock)
							{ assert (_curvePositionFactory); }

			// Just in case...
			virtual ~CombatFormationFactory ()
							{ delete (_curvePositionFactory); _curvePositionFactory = NULL; }

			/** To stablichs the set this factory works for. */
			const StdSetUFOsSceneActionBlock* owner () const
							{ return (_owner); }
			StdSetUFOsSceneActionBlock* owner ()
							{ return (_owner); }
			void setOwner (StdSetUFOsSceneActionBlock* o)
							{ _owner = o; }

			/** To get and set the curve and position factory. */
			const StdUFOSceneActionBlock::CurveAndInitialPositionFactory* curvePositionFactory () const
							{ return (_curvePositionFactory); }
			StdUFOSceneActionBlock::CurveAndInitialPositionFactory* curvePositionFactory ()
							{ return (_curvePositionFactory); }
			void setCurvePositionFactory (StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF)
							{ assert (cF); delete (_curvePositionFactory); _curvePositionFactory = cF; }

			/** To save the current status of the formation, if needed. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const = 0;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV) = 0;

			/** To initilize & finalize the factory. */
			virtual void initialize () = 0;
			virtual void finalize () = 0;

			/** To know whether this is the factory or not for a right type. */
			virtual bool forCombatType (int t) const = 0;

			/** To create one individual action block configurated for the type of combat formation. 
				taking into account the parameters of the set of ufos. \n
				The method uses _createEmptyIndividualActionBlockFunction. */
			virtual StdUFOSceneActionBlock* individualActionBlock (int i, Properties* prps);
			
			/** What to do with one action block when the timer is reached. */
			virtual void whenTimerIsSparked (StdUFOSceneActionBlock* aB, Properties* prps) = 0;

			protected:
			/** The owner. */
			StdSetUFOsSceneActionBlock* _owner;

			/** The curve factory associated to any individual action block created. */
			StdUFOSceneActionBlock::CurveAndInitialPositionFactory* _curvePositionFactory;
			/** Function to determine the relative initial position of a UFO taking into account the 
				number of UFO and the properties of the StdSetUFOsSceneActionBlock. */
			PositionFunction _positionFunction;
			/** Funtion to determine the relative time to start to move of an UFO taking into account the 
				number of the UFO and the properties of the StdSetUFOsSceneActionBlock. */
			TimeFunction _timeFunction; 
			/** Function to create and empty action block to control the individual UFOSs. */
			SingleActionBlockFunction _createEmptyIndividualActionBlockFunction;
		};

		/** The UFOS stay moving as it has been defined, and then attack individually when possible. */
		class StayAndAttackOneByOneCombatFormationFactory : public CombatFormationFactory
		{
			public:
			typedef std::function <int (int, Properties*)> CurveFunction;

			// Default functions that can be used by any StayAndAttackOneByOneCombatFormationFactory instance
			static const CurveFunction _NOMOVEMENTCURVE;

			/** 
			  * To create a formation:
			  * @param			Same parameters than the parent.
			  * @param cFunc:	Function to determine the curve used to attack when it is the time.
			  *					The function receives the number of the UFO (or block controlling it) 
			  *					and returns the number of the curve.
			  */
			StayAndAttackOneByOneCombatFormationFactory 
					(StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF,
					 const PositionFunction& pFunc = _SAMEPOSITION, // By default, the same position
					 const TimeFunction& tFunc = _SAMETIME, // By default the same time
					 const SingleActionBlockFunction& eABlock = __SINGLESTDUFOSCTIONBLOCK, // By default the std ufo action block
					 const CurveFunction& cFunc = _NOMOVEMENTCURVE) // By default no attack
				: CombatFormationFactory (cF, pFunc, tFunc, eABlock),
				  _justSparked (),
				  _curveFunction (cFunc)
							{ }

			/** @see parent. 
				To save or recover the changed already made in the movement. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			/** @see parent. */
			virtual void initialize ()
							{ _justSparked.clear (); }
			virtual void finalize () // It is not needed really, just initialize is enough!
							{ }

			/** @see parent. */
			/** It used _curveFunction. */
			virtual void whenTimerIsSparked (StdUFOSceneActionBlock* aB, Properties* prps);

			protected:
			/** Funtion to determine the movement (curve) of the UFO when attacking taking into
				account the number of the UFO ans the properties of the StdSetUFOsSceneActionBlock. */
			CurveFunction _curveFunction;

			// Implementation
			std::map <int, bool> _justSparked;
		};

		/** The UFOs moves in a block made of several rows, and then they attack to the player. \n
			The number of elements per row, the separation between the UFOs, and the attacking movement
			are passed as parameter. \n */
		class BlockAndFlyCombatFormationFactory : public StayAndAttackOneByOneCombatFormationFactory
		{
			public:
			/** 
			  * To create a block of combat.
			  * @param	cF:		The curve factory used to create individual action blocks!
			  * @param	ePR:	The number of UFOS per row, 
			  *					if something hasn't been defined in the properties of the set of ufos
			  * @param	s:		The relative separation amon UFOS, 
			  *					if something different hasn't been defined in the properties of a set of UFOs.
			  *	@param  aM:		The attacking movement. -1 will mean random between flying in curve to the right or to the left.
			  * @param eABlock:	Function to create the individual action block to control the UFOS,
			  *					The function receives the number of UFO, and the curve factory needed,
			  *					and returns the block to control.
			  * The "CombatFormationAttrs" parameter, included (optional) in the worlds definition file, considers
			  * three parameters maximum: 0 = ePR, 1 = s, 2 = aM
			  * The "CombatFormationAttackAttrs" parameter, included (optional) in the worlds definition file, should
			  *	be aligned with the type of movement defined.
			  */
			BlockAndFlyCombatFormationFactory (StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF, 
				int ePR = 7, QGAMES::bdata sH = 0.08, QGAMES::bdata sV = 0.08, 
				int aM= -1, const SingleActionBlockFunction& eABlock = __SINGLESTDUFOSCTIONBLOCK);

			/** @see parent. */
			virtual bool forCombatType (int t) const
							{ return (t == __BATTLESHIP_BLOCKANDFLYCOMBATFORMATIONTYPE__); }

			protected:
			/** Number of elements per row in the block. */
			int _elementsPerRow;
			/** Separation among elements in the same row. */
			QGAMES::bdata _horizontalSeparationBetweenElements;
			/** Separation among elemeent in different rows. */
			QGAMES::bdata _verticalSeparationBetweenElements;
			/** The attacking movement. */
			int _attackingMovement;
		};

		/** The UFOs stay moving as it has been defined, an then they attack following the first one. */
		class StayAndAttackFollowingFirstCombatFormationFactory : public CombatFormationFactory
		{
			public:
			typedef std::function <int (int, Properties*)> CurveFunction;

			// Default functions that can be used by any StayAndAttackFollowingFirstCombatFormationFactory instance
			static const CurveFunction _NOMOVEMENTCURVE;

			/** 
			  * To create a formation:
			  * @param			Same parameters than the parent.
			  * @param cFunc:	Function to determine the curve used to attack when it is the time.
			  *					The function receives the number of the UFO (or block controlling it) 
			  *					and returns the number of the curve.
			  */
			StayAndAttackFollowingFirstCombatFormationFactory
					(StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF,
					 const PositionFunction& pFunc = _SAMEPOSITION, // By default, the same position
					 const TimeFunction& tFunc = _SAMETIME, // By default the same time
					 const SingleActionBlockFunction& eABlock = __SINGLESTDUFOSCTIONBLOCK, // By default the std ufo action block
					 const CurveFunction& cFunc = _NOMOVEMENTCURVE) // By default no attack
				: CombatFormationFactory (cF, pFunc, tFunc, eABlock),
				  _movement (),
				  _curveParameters (), // Empty
				  _curveFunction (cFunc)
							{ } 

			/** @see parent.
				To save or recover which elements are already moving, and also the curve parameters applied to all of them. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			/** @see parent. */
			virtual void initialize ()
							{ _movement.clear (); _curveParameters.clear (); }
			virtual void finalize () // It is not needed really, just initialize is enough!
							{ }

			/** @see parent. */
			virtual void whenTimerIsSparked (StdUFOSceneActionBlock* aB, Properties* prps);

			protected:
			/** Funtion to determine the movement (curve) of the UFO when attacking taking into
				account the number of the UFO ans the properties of the StdSetUFOsSceneActionBlock. */
			CurveFunction _curveFunction;

			// Implementation
			std::map <int, int> _movement;
			std::map <int, double> _curveParameters;
		};

		/** One after other (separated for a while) from the early beginning all drawing the same curve. */
		class QueueAndFlyCombatFormationFactory : public StayAndAttackFollowingFirstCombatFormationFactory
		{
			public:
			/** 
			  *	To create a queue of UFO following the first:
			  * @param cF:	The curve and position factory used to create individual action blocks.
			  *	@param t:	The time between the UFOS when starting to move,
			  *				if something different hasn't been defined in the properties of the set of UFOs.
			  * @param aM:	Attacking movement. -1 means line, loop (right or left random) and the line back.
			  * @param eABlock:	Function to create the individual action block to control the UFOS,
			  *					The function receives the number of UFO, and the curve factory needed,
			  *					and returns the block to control.
			  * The "CombatFormationAttrs" parameter, included (optional) in the worlds configuration file, considers
			  * two parameters maximum: 0 = t, 1 = aM. \n
			  * The "CombatFormationAttackAttrs" parameter, included (optional) in the worlds definition file, should
			  *	be aligned with the type of movement defined.
			  */
			QueueAndFlyCombatFormationFactory (StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF,
				QGAMES::bdata t = 0.3, int aM = -1, const SingleActionBlockFunction& eABlock = __SINGLESTDUFOSCTIONBLOCK);

			/** @see parent. */
			virtual bool forCombatType (int t) const
							{ return (t == __BATTLESHIP_QUEUEANDFLYCOMBATFORMATIONTYPE__); }

			protected:
			/** Time between the elements of the queue. */
			QGAMES::bdata _timeBetweenElements;
			/** The type of attack. */
			int _attackingMovement;
		};

		/** All them follow at the leader, but the movement can change from tiem to time. */
		class DancingCombatFormationFactory : public StayAndAttackFollowingFirstCombatFormationFactory
		{
			public:
			struct Movement
			{
				int _movementId;
				std::map <int, double> _parameters;
			};

			typedef std::vector <Movement> Movements;

			/** 
			  * To create a formation:
			  * @param cF:		The curve and position factory used when a ufo individual block is set.
			  * @param crvs:	the set of different movements which to choose among.
			  * @param tC:		Times sparked to decided to change the movement. Negative means random up to that number.
			  * @param cFunc:	Function to determine the relative position of the element at the beginning (all in the same place by default)
			  * @param tFunc:	Function to determine the time to start to move (all at the same time by default).
			  * @param eABlock:	Function to create the individual action block to control the UFOS,
			  *					The function receives the number of UFO, and the curve factory needed,
			  *					and returns the block to control.
			  */
			DancingCombatFormationFactory 
					(StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF,
					 const Movements& crvs, int tC,
					 const PositionFunction& pFunc = _SAMEPOSITION, // By default, all at the same position
					 const TimeFunction& tFunc = _SAMETIME, // By default all move at the same time
					 const SingleActionBlockFunction& eABlock = __SINGLESTDUFOSCTIONBLOCK); // By default the std ufo action block

			/** @see parent. 
				To save and get the times executed. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			/** @see parent. */
			virtual void initialize ();

			/** @see parent. */
			virtual void whenTimerIsSparked (StdUFOSceneActionBlock* aB, Properties* prps);

			protected:
			/** The different possibilities. */
			Movements _curves;
			/** The second to change the movement of the first. */
			int _timesToChange;

			private:
			int _timesExecuted;
		};

		/** 
		  *	All UFOS move in linear directions one after other. 
		  *	The move following diferent types of lines:
		  *	0|1|2
		  *	3|4|5
		  *	6|7|8
		  */
		class DancingInLinesCombatFormationFactory : public DancingCombatFormationFactory
		{
			public:
			/** 
			  *	To create a queue of UFO following the first:
			  *	@param t:	The number of times passing over here a decision to change de direction is taken,
			  *				if something different hasn't been defined in the parameters of the set of UFOs.
			  * The "CombatFormationAttrs" parameter, included (optional) in the worlds configuration file, considers
			  * one parameters maximum: 0 = t. \n
			  */
			DancingInLinesCombatFormationFactory (
				StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF, int t = 10,
				const SingleActionBlockFunction& eABlock = __SINGLESTDUFOSCTIONBLOCK);

			/** @see parent. */
			virtual bool forCombatType (int t) const
							{ return (t == __BATTLESHIP_DANCINGINLINESCOMBATFORMATIONTYPE__); }

			protected:
			/** Time between the elements of the queue. */
			QGAMES::bdata _timeBetweenElements;
		};

		/** The list of combat factories. \n
			The object ows the factories. \n
			With the list of standard combat factories. \n
			The class can be overloaded, but with a lot of care. */
		class CombatFormationFactories
		{
			public:
			/** To create a list of combat formation factories. 
				By default he curve and position factory used is the default one. */
			CombatFormationFactories (const std::vector <CombatFormationFactory*>& f = 
				std::vector <CombatFormationFactory*> 
					{ new BlockAndFlyCombatFormationFactory (new StdUFOSceneActionBlock::CurveAndInitialPositionFactory),
					  new QueueAndFlyCombatFormationFactory (new StdUFOSceneActionBlock::CurveAndInitialPositionFactory),
					  new DancingInLinesCombatFormationFactory (new StdUFOSceneActionBlock::CurveAndInitialPositionFactory) } );

			~CombatFormationFactories ();

			/** To save the status of the combat factories. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			/** To change the owner of each factory. 
				The ownership is transmited to every factiry. It could be null. */
			void setOwner (StdSetUFOsSceneActionBlock* o);

			/* *To get the right factory for a type.
				If there were more than one valid for the same type, the first one found would be returned. \n
				The behaviour can be changed. */
			virtual CombatFormationFactory* combatFormationFactoryForType (int t) const;

			/** To initialize and finalize. 
				Can be overloaded. By default the initialize and finalize all cambat factories defined inside. */
			virtual void initialize ();
			virtual void finalize ();

			protected:
			std::vector <CombatFormationFactory*> _factories;
		};

		/** A list of combat factories can be provided. 
			The right one is choosen taken into accoun the parameter CombatFormation. */
		StdSetUFOsSceneActionBlock (int id, Properties* prps = new Properties (), 
			CombatFormationFactories* cF = new CombatFormationFactories ());

		virtual ~StdSetUFOsSceneActionBlock (); // The combat factory

		/** To know the properties. */
		const Properties* properties () const
							{ return (_properties); }
		Properties* properties ()
							{ return (_properties); }
		virtual void setProperties (Properties* prps);

		/** To know the space elements controlled. */
		SpaceElements spaceElementsControlled () const;
		/** To know the number of elements playing. */
		int spaceElementsFlying () const;

		/** To know the action blocks. */
		const QGAMES::SceneActionBlocks actionBlocks () const
							{ return (_actionBlocks); }

		/** @see parent. 
			Just save / get also the properties. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** To know whether there are elements undeer control. */
		bool spaceElementsUnderControl () const;
		/** To know whether there is or not one element moving. */
		bool spaceElementMoving () const;

		/** @see parent. */
		virtual void initialize ();
		virtual void inEveryLoop ();
		virtual void updatePositions ();
		virtual void finalize ();

		/** @see parent. 
			Just to continue the notification chain up. */
		virtual void processEvent (const QGAMES::Event& evnt);

		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }

		protected:
		/** When the timer reaches the limit. 
			By default the same method for the combat factory is invoked per action block. */
		virtual void whenTimerIsSparked ();

		/** Very internal method to delete the action blocks. */
		void deleteActionBlocks ();

		protected:
		Properties* _properties;
		CombatFormationFactories* _combatFormationFactories;

		// Implementation
		QGAMES::SceneActionBlocks _actionBlocks;
		int _blockToSparkNextTime;

		static const int _COUNTERTIMER = 0;
		static const int _COUNTERMAXTIMEACTION = 1;
	};
}

#endif
  
// End of the file
/*@}*/