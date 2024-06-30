/** \ingroup Battleship */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: bsactionblocks.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 02/11/2019 \n
 *	Description: Define specific action blocks for Battleship like games.
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_ACTIONBLOCKS__
#define __BATTLESHIP_ACTIONBLOCKS__

#include <BattleshipLike/bsdefinitions.hpp>
#include <BattleshipLike/bsartists.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace BATTLESHIP
{
	/** A base class for all action blocks used in Battleship like games. */
	class SceneActionBlock : public QGAMES::SceneActionBlock
	{
		public:
		SceneActionBlock (int id, 
			const QGAMES::SceneActionBlockProperties& prps = QGAMES::SceneActionBlockProperties ())
			: QGAMES::SceneActionBlock (id, prps)
							{ }
	};

	/** An action block to wait a time. 
		The time to wait comes into one of the properties. */
	class WaitTimeActionBlock : public SceneActionBlock
	{
		public:
		WaitTimeActionBlock (int id,
			const QGAMES::SceneActionBlockProperties& prps = QGAMES::SceneActionBlockProperties ());

		/** @see parent. */
		virtual void initialize () override;
		virtual void updatePositions () override;

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }

		protected:
		static const int _COUNTERTIME = 0;

		// Implementation
		QGAMES::bdata _timeToWait;
	};

	/** A standard action block to move (theoretically) any type of space element o set of them. */
	class StdSpaceElementsSceneActionBlock : public SceneActionBlock
	{
		public:
		struct Properties
		{
			public:
			Properties ()
				: _timeToAppear (__BD 2), // Negative means random up to that number!
				  _probabilityToAppear (100),
				  _toEmerge (false),
				  _timeToMove (__BD 0),
				  // If regenerated means it is destroyed or exit the screen a new instance starts again
				  _regenerate (true), 
				  // If rolled means it is put in the other side of the screen when it exists the screen 
				  _roll (false), 
				  _initialPosition (QGAMES::Position::_noPoint), // Meaning not needed...
				  _movementZone (QGAMES::Rectangle::_noRectangle), // Meaning to take the elements has as reference
				  _maxTimeAction (-1) // Means no limit (other will decide when it finishes
							{ }

			Properties (QGAMES::bdata tA, int p, bool tE, QGAMES::bdata tM, bool r, bool rll,
						const QGAMES::Position& pos, const QGAMES::Rectangle& mZ, QGAMES::bdata mTA);

			virtual ~Properties ()
							{ }

			/**
			  * The attributes definied in the xml file should look like as follows: \n
			  * This method translates a map of those into internal variables of the structure. \n
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
			  *	</Attributes>
			  */
			Properties (const std::map <std::string, std::string>& prps);

			/** To get the properties as a set of openvalues. \n
				The properties are saved as part of the runtime info of the action block
				because they could change along the game, and even after being initialized. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			/** To get the initial position ob a space element (usually the ones controlled by the action block),
				within the movement zone. It that is null, one should be taken as a base. 
				Remember that the initial position specified in the properties is always relative to that movementZone. */
			virtual QGAMES::Position absInitialPositionFor (SpaceElement* spc);

			QGAMES::bdata _timeToAppear; 
			int _probabilityToAppear; 
			bool _toEmerge;
			QGAMES::bdata _timeToMove;
			bool _regenerate;
			bool _roll;
			QGAMES::Position _initialPosition; // Relative to the size of the map the space element is in
											   // For some types of elements this couldn't have any sense
			QGAMES::Rectangle _movementZone;   // No rectangle means to take the one the element has as reference 
			QGAMES::bdata _maxTimeAction; 
		};

		/** 
		  *	The constructor receives:
		  *	@param id	The id of the scene block.
		  *	@param prps	A reference to the properties used by the action block. \n
		  *				The class owns the properties element.
		  */
		StdSpaceElementsSceneActionBlock (int id, Properties* prps);

		virtual ~StdSpaceElementsSceneActionBlock ()
							{ delete (_properties); _properties = NULL; }

		/** To know the properties. 
			If they are changed after being initilized there could be no effect. */
		const Properties* properties () const
							{ return (_properties); }
		Properties* properties ()
							{ return (_properties); }
		virtual void setProperties (Properties* prps);

		/** To know the list of space elements controlled. */
		const SpaceElements& spaceElementsControlled () const
							{ return (_spaceElements); }
		/** To know the number of elements playing. */
		int spaceElementsFlying () const
							{ return ((int) _spaceElements.size ()); }

		/** @see parent. 
			Just save / get also the properties. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** To know whether there are or not still elements under the control of the block. 
			The control is lost when, e.g, all space elements are killed and they haven't to be regenerated anymore. */
		bool spaceElementsUnderControl () const
							{ return (onOffSwitch (_SWITCHELEMENTSUNDERCONTROL) -> isOn ()); }
		/** To know whether there is or not one element moving. */
		bool spaceElementsMoving () const
							{ return (spaceElementsUnderControl () && onOffSwitch (_SWITCHELEMENTSMOVING) -> isOn ()); }

		/** @see parent. */
		virtual void initialize ();
		/** 
		  *	The default implementation of this method, works as follows:
		  *	1.- If there is no spaceElements in the screen, an it is time for them to appear
		  *		and a random number between 0 and 100 is below the probability defined in the properties object, 
		  * 	a new speceElement is created.
		  *	2.- Those spaceElements are given by the method getAndConfigureSpaceElements (usually just one).
		  *	3.- Their initial movements (including its speed if needed) are given by the method: startToMoveSpaceElements.
		  * 4.- If the property MaxTimeAction has been defined and the time of the action block  is over and 
		  *		no elements are moving a notification is sent up. This notification should be usually gathered 
		  *		and treated by the scene.
		  *	5.- The space elements can be regenerated if it has defined so in the properties object and there are no more
		  *		space elements of the group managed by the action block still in the scene.
		  *	@see all of them for more details. 
		  */
		virtual void updatePositions ();
		virtual void finalize ();

		/** @see parent. 
			When the element is out of the screen or it has been destroyed, 
			a signal from its updatePositions method is received and processed here. 
			The space elements will then be destroyed and no observed anymore. 
			For these actions a buoy (_FINALIZESPACEELEMENTBUOY) is used. @see below. */
		virtual void processEvent (const QGAMES::Event& evnt);

		protected:
		// Algorithm design pattern...
		// Both methods are invoked from updatePositions method.
		/** To get the spaceElements just ready. 
			It is with the aspect, form, etc... and put it in the right position. \n 
			What the method shoud do is to select and configure the elements (_spaceElements list)
			to be moved when no parameter is received or it is empty, or just configure them when the parameter is not empty. */
		virtual void getAndConfigureSpaceElements (const std::vector <int>& elmts = std::vector <int> ()) = 0;
		/** To start to move every of the elements within the _spaceElements list. \n 
			It has to be redefined later in classes inheriting this one.
			When the code reaches this position, the space elements should be already created. */
		virtual void startToMoveSpaceElements () = 0;

		// Implementation
		/** This two method are to select nE elements from the list of entities passed as parameter
			and given a function to validate whether each one is valid. \n
			If elements are got from eties list. \h
			The element got are either the ones described in the lst if it is not empty ot those. 
			This method used the next. \n
			Both are desacoplate becuase other objects can be extended from Asteroids and others 
			and it wouldn't be necessary to overload the next function but only changing the validator. */
		SpaceElements getListValidSpaceElementsTypeFrom (const std::vector <int>& lst, int nE, const QGAMES::Entities& eties); 
		virtual std::function <bool (SpaceElement*)> isElementValidForActionBlock () const
							{ return ([](SpaceElement* spc) -> bool { return (spc != NULL); }); } 
		
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters ()
							{ return (new Counters ()); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches);
		virtual QGAMES::OnOffSwitches* createOnOffSwitches ()
							{ return (new OnOffSwitches ()); }

		protected:
		SpaceElements _spaceElements;
		Properties* _properties;

		/** To finalize a Space Element. */
		class ToFinalizeSpaceElement : public QGAMES::Buoy
		{
			public:
			ToFinalizeSpaceElement ()
				: QGAMES::Buoy (_FINALIZESPACEELEMENTBUOY, __BD 0),
				  _spaceElement (NULL)
							{ }

			// To set the spece element to treat...
			void setSpaceElement (SpaceElement* sE)
							{ _spaceElement = sE; }
			SpaceElement* spaceElement ()
							{ return (_spaceElement); }
			const SpaceElement* spaceElement () const
							{ return (_spaceElement); }

			/** @see parent. */
			virtual void* treatFor (QGAMES::Element* e);

			private:
			SpaceElement* _spaceElement;
		};

		static const int _COUNTERTOAPPEAR = 0;
		static const int _COUNTEREMERGINGGRADE = 1;
		static const int _COUNTERTOMOVE = 2;
		static const int _COUNTERMAXTIMEACTION = 3;
		static const int _SWITCHELEMENTSINSCENE = 0;
		static const int _SWITCHELEMENTSEMERGING = 1;
		static const int _SWITCHELEMENTSTOMOVE = 2;
		static const int _SWITCHELEMENTSMOVING = 3;
		static const int _SWITCHELEMENTSUNDERCONTROL = 4;

		static const int _FINALIZESPACEELEMENTBUOY = 0;

		// Implementation
		static const int _FADEGRADES [18];
	};

	/** General definition of a standard action block to control asteroids. */
	class StdAsteroidSceneActionBlock : public StdSpaceElementsSceneActionBlock
	{
		public:
		struct Properties : public StdSpaceElementsSceneActionBlock::Properties
		{
			public:
			Properties ()
				: StdSpaceElementsSceneActionBlock::Properties (),
				  _type (-1),		 // -1 Means random between 0 and the max types allowed (defined in DataGame)
				  _movementType (0), // 0 Means the basic one... but -1 should mean random (depending on the implementation)
				  _points (10),		 // Points given when destroyed. Negative means random up to that value
				  _speed (__BD 1)	 // It will multiply the basic speed 
									 // (and without taken into account the speed of the background itself)
									 // A negative value means random up to that number (in positive)
							{ }

			Properties (QGAMES::bdata tA, int pA, bool tE, QGAMES::bdata tM, bool r, bool rll, 
						const QGAMES::Position& pos, const QGAMES::Rectangle& mZ, QGAMES::bdata mTA,
						int t, int mT, int p, QGAMES::bdata s);

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
			  * ...adding these ones for the asteroid control:
			  *
			  *		<Attribute id="Type" value="-1|0..."/> \n
			  *		<Attribute id="MovType" value="-1|..."/> \n
			  *		<Attribute id="PointsGiven" value="0.."/> \n
			  *		<Attribute id="Speed" value="-|+"/> \n
			  *	</Attributes>
			  */
			Properties (const std::map <std::string, std::string>& prps);

			/** @see parent. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			int _type; 
			int _movementType;
			int _points; 
			QGAMES::bdata _speed; 
		};

		/** The action block can be valid for many asteroids. \n
			The number of asteroids is received as parameter. */
		StdAsteroidSceneActionBlock (int id, Properties* prps, int nA = 1)
			: StdSpaceElementsSceneActionBlock (id, prps),
			  _numberOfAsteroids (nA)
							{ assert (_numberOfAsteroids > 0); }

		protected:
		/** @see parent. */
		virtual void getAndConfigureSpaceElements (const std::vector <int>& elmts = std::vector <int> ());
		virtual void startToMoveSpaceElements ();

		/** @see parent. */
		virtual std::function <bool (SpaceElement*)> isElementValidForActionBlock () const
							{ return ([](SpaceElement* spc) -> bool 
								{ return (spc != NULL && 
										  dynamic_cast <Asteroid*> (spc) != NULL &&
										  !spc -> isVisible ()); }); } 

		protected:
		/** The number of asteroids this action block is for. */
		int _numberOfAsteroids;
	};

	/** General definition of a standard action block to control things to catch */
	class StdThingToCathActionBlock : public StdSpaceElementsSceneActionBlock
	{
		public:
		struct Properties : public StdSpaceElementsSceneActionBlock::Properties
		{
			public:
			Properties ()
				: StdSpaceElementsSceneActionBlock::Properties (),
				  _type (-1),		 // Means random between 0 and the max types allowed (defined in DataGame)
				  _movementType (0), // Means the basic one...but -1 should mean random (but it will depend on the implementation)
				  _speed (__BD 1)	 // It will multiply the basic speed 
									 // (and without taken into account the speed of the background ifself)
									 // A negative value would mean random up to that number (in positive)
							{ }

			Properties (QGAMES::bdata tA, int pA, bool tE, QGAMES::bdata tM, bool r, bool rll, 
						const QGAMES::Position& pos, const QGAMES::Rectangle& mZ, QGAMES::bdata mTA,
						int eT, int mT, QGAMES::bdata s);

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
			  * ...adding these ones for the pill of shootings control:
			  *
			  *		<Attribute id="Type" value="-1|0..."/> \n
			  *		<Attribute id="MovType" value="-1|0..."/> \n
			  *		<Attribute id="Speed" value="-,+"/> \n
			  *	</Attributes>
			  */
			Properties (const std::map <std::string, std::string>& prps);

			/** @see parent. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			int _type; 
			int _movementType; 
			QGAMES::bdata _speed; 
		};

		StdThingToCathActionBlock (int id, Properties* prps, int nP = 1 /** 1 by default. */)
			: StdSpaceElementsSceneActionBlock (id, prps),
			  _numberOfThingsToCatch (nP)
							{ assert (_numberOfThingsToCatch > 0); }

		protected:
		/** @see parent. */
		virtual void getAndConfigureSpaceElements (const std::vector <int>& elmts = std::vector <int> ());
		virtual void startToMoveSpaceElements ();

		/** @see parent. */
		virtual std::function <bool (SpaceElement*)> isElementValidForActionBlock () const
							{ return ([](SpaceElement* spc) -> bool 
								{ return (spc != NULL && 
										  dynamic_cast <PillPowerToCatch*> (spc) != NULL &&
										  !spc -> isVisible ()); }); } 

		protected:
		/** The number of things to catch this action block is for. */
		int _numberOfThingsToCatch;
	};
}

#include <BattleshipLike/bsactionblocksforufos.hpp>
#include <BattleshipLike/bsactionblocksforthingstocatch.hpp>

#endif
  
// End of the file
/*@}*/