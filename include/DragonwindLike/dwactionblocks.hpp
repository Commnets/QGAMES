/** \ingroup Dragonwind */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: dwactionblocks.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 12/05/2019 \n
 *	Description: Define specific action blocks for dragonwind like games.
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_ACTIONBLOCKS__
#define __DRAGONWIND_ACTIONBLOCKS__

#include <DragonwindLike/dwdefinitions.hpp>
#include <DragonwindLike/dwartists.hpp>
#include <Platform/ptinclude.hpp>

namespace DRAGONWIND
{
	/** 
	  * Class to define common functionality to any scene action block ina Dragonwind like game. \n
	  *	The Dragonwind Scene Action Blocks can be declared in a file (like any other). \n
	  * The initial active status can be changed along the game, so it has to be saved. 
	  * @see also DRAGONWIND::WorldBuilder for more details. The method afterCreateElement has been redefined.
	  */
	class SceneActionBlock : public QGAMES::SceneActionBlock
	{
		public:
		SceneActionBlock (int id, 
			const QGAMES::SceneActionBlockProperties& prps = QGAMES::SceneActionBlockProperties ())
			: QGAMES::SceneActionBlock (id, prps),
			  _initialActive (_active)
							{ }

		/** To know ehether the block was or not active when built. */
		bool initialActive () const
							{ return (_initialActive); }
		void setInitialActive (bool iA)
							{ _initialActive = iA; }

		/** @see parent. */
		virtual void initialize ();

		/** To adapt the the scene block to the difficulty level. 
			This method is invoked from "initialize" (PlayingScene) 
			This method has to adapt everything: Scene block itself, entities in it, etc. 
			By default it does nothing. */
		virtual void adaptSceneToDifficultyLevel ()
							{ }

		protected:
		bool _initialActive;
	};

	/** A block to move flying mosters along the screen. */ 
	class FlyingMonstersActionBlock : public SceneActionBlock
	{
		public:
		struct Properties
		{
			Properties ()
				: _typeOfMonster (__DRAGONWIND_ACTIONBLOCKFMONSTERTYPEBEE__), 
				  _timeToAppear (__BD 2)
							{ }

			Properties (int tM, QGAMES::bdata tA)
				: _typeOfMonster (tM),
				  _timeToAppear (tA)
							{ assert (tM >= 0 && tA > 0); }

			/**
			  * In base to the attributes defined in the worlds.xml file. \n
			  *	<Attributes> \n
			  *		<Attribute id="TypeOfMonster" value="0|1"/> \n
			  *		<Attribute id="TimeToAppear" value="0.."/> <!-- Seconds --> \n
			  *	</Attributes>
			  */
			Properties (const std::map <std::string, std::string>& dt);

			/** To get the properties as a set of openvalues. \n
				The configuration is saved as part of the runtime info of the action block
				because it can be change along the game, and after being initialized. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			int _typeOfMonster;
			QGAMES::bdata _timeToAppear;
		};

		// The properties is a pointer to allow overload this class later considering
		// considering other elements in the set of porperties...
		FlyingMonstersActionBlock (int id, Properties* prps = new Properties ());
		~FlyingMonstersActionBlock ()
							{ delete (_properties); _properties = NULL; }

		/** To get the properties. */
		Properties* properties ()
							{ return (_properties); }
		const Properties* properties () const
							{ return (_properties); }

		/** @see parent. \n
			To the information kept by the parent, 
			it is needed to add the monitor id (openValue number 1). */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** @see parent. */
		virtual void setActive (bool a);

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		/** To get an instance of the flock of monsters to be be used, 
			attending (by default) the type of block and also the difficulty of the game. \n
			@see file dwentities.xml
			to know about how flocks are defined depending on the difficulty of the game. \n
			The behaviour can be changed. */
		virtual FlockOfMonsters* flock ();
		const FlockOfMonsters* flock () const
							{ return ((const FlockOfMonsters*) flock ()); }

		/** To initialize the flock. */
		void initializeFlock ();
		/** To finalize the flock. */
		void finalizeFlock ();

		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }

		protected:
		Properties* _properties;

		// Implementation
		FlockOfMonsters* _monsters; // They don't belong to the action block...

		/** To finalize the flock. */
		class ToFinalizeFlock : public QGAMES::Buoy
		{
			public:
			ToFinalizeFlock ()
				: QGAMES::Buoy (_FINALIZEFLOCKBUOY, __BD 0)
							{ }

			/** @see parent. */
			virtual void* treatFor (QGAMES::Element* e);
		};

		static const int _COUNTERTOAPPEAR = 0;
		static const int _MONSTERSONSCREEN = 0;
		static const int _FINALIZEFLOCKBUOY = 0;
	};

	/** A block to move a bad guy over the screen. \n
		The block consideres the possibility to fire. \n 
		The movement never starts until the entity is in a stable position on a base. 
		The effects when it is activated or disactivated are not controlled. */
	class BadGuysActionBlock : public SceneActionBlock
	{
		public:
		struct Properties
		{
			Properties ()
				: _monitorId (-1), 
				  _toFire (false),
				  _fireType (), 
				  _fireImpulse (1), // Negative values means random up to that number...
				  _sToFire (__BD 2), _sBetweenFires (__BD 2), _sFiring (__BD 2)
				  // Negative values would mean random up to that value per block of actions...
							{ _fireType.push_back (-1); } // Means what is defined in the bad guy as default!

			Properties (int mId, int bId, int mT, bool f, std::vector <int>& fT, int fI, 
					QGAMES::bdata sTF, QGAMES::bdata sBF, QGAMES::bdata sF)
				: _monitorId (mId),
				  _toFire (f),
				  _fireType (fT),
				  _fireImpulse (fI),
				  _sToFire (sTF), _sBetweenFires (sBF), _sFiring (sF)
							{ }

			/**
			  * In base to the attributes defined in the worlds.xml file. \n
			  *	<Attributes> \n
			  *		<Attribute id="MonitorId" value="0.."/> \n
			  *		<Attribute id="Fire" value="YES|NO"/> \n
			  *		<Attribute id="TypeOfFire" value="0..,0..,.."/> \n // There could be more than one, then random!
			  *		<Attribute id="ImpulseOfFire" value="..0.."/> \n
			  *		<Attribute id="SecondsToFire" value="..0.."/> <!-- Seconds --> \n
			  *		<Attribute id="SecondsBetweenFires" value="..0.."/> <!-- Seconds --> \n
			  *		<Attribute id="SecondsFiring" value="..0.."/> <!-- Seconds -->
			  *	</Attributes>
			  */
			Properties (const std::map <std::string, std::string>& dt);

			/** To get the properties as a set of openvalues. \n
				The configuration is saved as part of the runtime info of the action block
				because it can be change along the game, and after being initialized. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			int _monitorId; // Which is the monitor id controlling the movements?
			bool _toFire; // Fire?
			std::vector <int> _fireType; // Type of fire when it does...
			int _fireImpulse; // The impulse when firing. -1 means random.
			QGAMES::bdata _sToFire; // if does, seconds to start (random when negative)
			QGAMES::bdata _sBetweenFires; // If does, seconds between fires (random when negative)
			QGAMES::bdata _sFiring; // If does, seconds firing before stopping and start back (random when negative)
		};

		/** The properties is a pointer to allow overload this class later and consider
			more attributes in the list of those... */
		BadGuysActionBlock (int id, Properties* prps = new Properties ());
		~BadGuysActionBlock ()
							{ delete (_properties); _properties = NULL; }

		/** To get the properties. */
		Properties* properties ()
							{ return (_properties); }
		const Properties* properties () const
							{ return (_properties); }

		/** @see parent. \n
			the information added is the one properties element (setOfOpenValues) and
			the one comming from the active bad buy being managed in the block (setOfOpenValues). */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** To fix / change the bad guy assigned to this block. \n
			This instruction usually makes only sense before starting to move. \n 
			No character is assigned by default. It has to be set before executing the method updatePositions. */
		DRAGONWIND::Character* badGuy ()
							{ return (_character); }
		const DRAGONWIND::Character* badGuy () const
							{ return (_character); }
		virtual void setBadGuy (DRAGONWIND::BadGuy* bG);

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }

		// Implementation 
		void initCounterToFire ();
		void initCounterBetweenFires ();
		void initCounterFiring ();

		protected:
		Properties* _properties;

		// Implementation
		DRAGONWIND::Character* _character;

		static const int _COUNTERTOFIRE = 0;
		static const int _CONTERBETWEENFIRES = 1;
		static const int _COUNTERFIRING = 2;
		static const int _SWITCHTOMOVE = 0;
		static const int _SWITCHFIRING = 1;
	};

	/** A block to create and maintain for a while an environmental condition. */
	class EnvironmentalConditionActionBlock : public SceneActionBlock
	{
		public:
		struct Properties
		{
			Properties ()
				: _typeCondition (-1), // Means random...
				  _sToStart (__BD 0), // Negative would means random up to that value (in positive)
				  _sCondition (__BD -1), // -1 Means eternal. 
										 // Other negative values would mean random up to that value (in positive)
				  _otherProperties ()
							{ }

			Properties (int t, QGAMES::bdata sTS, QGAMES::bdata sC, 
							const std::map <std::string, std::string>& oC = std::map <std::string, std::string> ())
				: _typeCondition (t),
				  _sToStart (sTS),
				  _sCondition (sC),
				  _otherProperties (oC)
							{ }

			/**
			  * In base to the attributes defined in the worlds.xml file. \n
			  *	<Attributes> \n
			  *		<Attribute id="TypeCondition" value="0.."/> \n
			  *		<Attribute id="SecondsToStart" value="0.."/> <!-- Seconds --> \n
			  *		<Attribute id="SecondsCondition" value="0.."/> <!-- Seconds --> \n
			  *		... \n
			  *		Here the specific properties for the condition (@see adenvconditions.hpp) \n
			  *	</Attributes>
			  */
			Properties (const std::map <std::string, std::string>& dt);

			/** To get the properties as a set of openvalues. \n
				The configuration is saved as part of the runtime info of the action block
				because it can be change along the game, and after being initialized. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			int _typeCondition; // The type of environmental condition
			QGAMES::bdata _sToStart; // if does, seconds to start
			QGAMES::bdata _sCondition; // If does, seconds with the condition maintained
			std::map <std::string, std::string> _otherProperties;
		};

		/** The properties is a pointer to allow overload this class later and consider
			more attributes in the list of those... */
		EnvironmentalConditionActionBlock (int id, Properties* prps = new Properties ());
		~EnvironmentalConditionActionBlock ();

		/** To get the properties. */
		Properties* properties ()
							{ return (_properties); }
		const Properties* properties () const
							{ return (_properties); }

		/** @see parent. \n
			the information added is the one properties element (setOfOpenValues) and
			the one comming from the active environmental condition managed in the block (setOfOpenValues). */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** @see parent. */
		virtual void setActive (bool a);

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }

		/** To create the environmental conditionn from the type id. */
		virtual QGAMES::EnvironmentalCondition* conditionForType (int t);

		// Implementation
		void initCounterToStart ();
		void initCounterCondition ();

		protected:
		Properties* _properties;

		// The enviromental condition managed...
		QGAMES::EnvironmentalCondition* _environmentalCondition;

		static const int _COUNTERTOSTART = 0;
		static const int _COUNTERCONDITION = 1;
		static const int _SWITCHETERNAL = 0;
	};

	/** An action block to switch the visibility between different sets of layers. \n
		When one set is on the rest will be off. \n
		If there was only one set the effect would the equivalent to activate or desactivate the set itself. \n
		This block can be used to simukate e,g, that a wall is destroyed when a bomb explodes near of it. \n
		It there were sounds involved in the activation or desactivation, they would have to be managed from outside.  */
	class SwitchVisibilityBetweenASetOfLayersActionBlock : public SceneActionBlock
	{
		public:
		struct Properties
		{
			Properties ()
				: _defaultSetOfLayersActived (-1), _setsOfLayers ()
							{ }

			Properties (int dS, const std::map <int, std::vector <std::string>>& s)
				: _defaultSetOfLayersActived (dS), _setsOfLayers (s)
							{ }

			Properties (const std::map <std::string, std::string>& prps);

			/** To get the properties as a set of openvalues. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			int _defaultSetOfLayersActived;
			std::map <int, std::vector <std::string>> _setsOfLayers;

		};

		/** The properties is a pointer to allow overload this class later and consider
			more attributes in the list of those... */
		SwitchVisibilityBetweenASetOfLayersActionBlock (int id, Properties* prps = new Properties ());
		~SwitchVisibilityBetweenASetOfLayersActionBlock ()
							{ delete (_properties); _properties = NULL; }

		/** To get the properties. */
		Properties* properties ()
							{ return (_properties); }
		const Properties* properties () const
							{ return (_properties); }

		/** @see parent. \n
			the information added is the one properties element (setOfOpenValues) and
			the one indicating which is the set of layers active. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** Active one set and switch off the rest. */
		int setOfLayersActive ()
							{ return (_setOfLayersActived); }
		virtual void activeSetOfLayers (int nS);

		/** @see parent. 
			When it is not active, no layer is active. */
		virtual void setActive (bool a)
							{ activeSetOfLayers (a 
								? _setOfLayersActived : _properties -> _defaultSetOfLayersActived); }

		/** @see parent. */
		virtual void initialize ();
		virtual void finalize ();

		protected:
		Properties* _properties;
		std::map <int, QGAMES::AdvancedTileLayers> _setsOfLayers;

		// Implementation
		int _setOfLayersActived; // -1 Means none...
	};

	/** An action block to execute a movement in a direction (or its opposite) for a set of layers. \n
		This action block can be used to simulated that a door is getting opened or closed e.g. \n
		There could be a sound associated to the movement. */
	class MoveLinearASetOfLayersActionBlock : public SceneActionBlock
	{
		public:
		struct Properties
		{
			Properties ()
				: _setOfLayers (), 
				  _direction (QGAMES::Vector::_cero),
				  _steps (0),
				  _speed (0),
				  _sound (-1),
				  _soundChannel (__QGAMES_GAMESOUNDCHANNEL__)
							{ }

			Properties (const std::vector <std::string>& s1, 
					const QGAMES::Vector& d, int l, int s, int so, int sChn)
				: _setOfLayers (s1),
				  _direction (d),
				  _steps (l),
				  _speed (s),
				  _sound (so),
				  _soundChannel (sChn)
							{ }

			Properties (const std::map <std::string, std::string>& prps);

			/** To get the properties as a set of openvalues. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			std::vector <std::string> _setOfLayers;
			QGAMES::Vector _direction;
			int _steps;
			int _speed;
			int _sound;
			int _soundChannel;
		};

		/** The properties is a pointer to allow overload this class later and consider
			more attributes in the list of those... */
		MoveLinearASetOfLayersActionBlock (int id, Properties* prps = new Properties ());
		~MoveLinearASetOfLayersActionBlock ()
							{ delete (_properties); _properties = NULL; }

		/** To get the properties. */
		Properties* properties ()
							{ return (_properties); }
		const Properties* properties () const
							{ return (_properties); }

		/** @see parent. \n
			the information added is the properties element (setOfOpenValues) and
			the one indicating which is direction of the movement. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** To move the layers. */
		virtual void toMoveForward ();
		bool isMovingForwards ();
		bool hasMovedForwards ();
		virtual void toMoveBackward ();
		bool isMovingBackwards ();
		bool hasMovedBackwards ();

		/** @see parent. 
			When the layer is not actived, the layers move to the original position. */
		virtual void setActive (bool a);

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();
		virtual void finalize ();

		protected:
		Properties* _properties;

		// Implementation
		QGAMES::AdvancedTileLayers _setOfLayers;
		bool _moving;
	};
}

#endif
  
// End of the file
/*@}*/