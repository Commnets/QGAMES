/** \ingroup Battleship */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: bsactionblocksforthingstocatch.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 27/12/2020 \n
 *	Description: Define specific action blocks for different things to catch in Battleship like games.
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_ACTIONBLOCKSFORTHINGSTOCATCH__
#define __BATTLESHIP_ACTIONBLOCKSFORTHINGSTOCATCH__

#include <BattleshipLike/bsactionblocks.hpp>

namespace BATTLESHIP
{
	/** An action block to control the pill of energy.
		It is a detail of the things to catch. */
	class StdPillSceneActionBlock : public StdThingToCathActionBlock
	{
		public:
		public:
		struct Properties : public StdThingToCathActionBlock::Properties
		{
			public:
			Properties ()
				: StdThingToCathActionBlock::Properties (),
				  _powerLevel (-1)  // Means random, -2 would mean the next of the current one...
							{ }

			Properties (QGAMES::bdata tA, int pA, bool tE, QGAMES::bdata tM, bool r, bool rll, 
						const QGAMES::Position& pos, const QGAMES::Rectangle& mZ, QGAMES::bdata mTA,
						int eT, int mT, int pL, QGAMES::bdata s);

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
			  *		<Attribute id="TypeShooting" value="-3|-2|-1|0..."/> \n -3 meaning the previous, -2 the next, -1 random...
			  *		<Attribute id="Speed" value="-,+"/> \n
			  *	</Attributes>
			  */
			Properties (const std::map <std::string, std::string>& prps);

			/** @see parent. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			int _powerLevel; 
		};

		StdPillSceneActionBlock (int id, Properties* prps, int nT = 1)
			: StdThingToCathActionBlock (id, prps, nT)
							{ }

		protected:
		/** @see parent. 
			To stablish thepower of the pill. */
		virtual void getAndConfigureSpaceElements (const std::vector <int>& elmts = std::vector <int> ()); 

		/** @see parent. */
		virtual std::function <bool (BATTLESHIP::SpaceElement*)> isElementValidForActionBlock () const override final
							{ return ([](BATTLESHIP::SpaceElement* spc) -> bool 
								{ return (spc != NULL && 
										  dynamic_cast <PillPowerToCatch*> (spc) != NULL &&
										  !spc -> isVisible ()); }); } 
	};

	/** General definition of a standard action block to control energy elements. 
		It is a redefinition of the standard block to control things to catch. */
	class StdEnergyElementSceneActionBlock : public StdThingToCathActionBlock
	{
		public:
		struct Properties : public StdThingToCathActionBlock::Properties
		{
			Properties ()
				: StdThingToCathActionBlock::Properties (),
				  _energy (10)		 // Always between -100 and 100... 
									 // -1000 random between -100 and 0, 1000 same but between 0 and 100
							{ }

			Properties (QGAMES::bdata tA, int pA, bool tE, QGAMES::bdata tM, bool r, bool rll, 
						const QGAMES::Position& pos, const QGAMES::Rectangle& mZ, QGAMES::bdata mTA,
						int eT, QGAMES::bdata eL, int mT, QGAMES::bdata s);

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
			  * ...adding these ones for the energy element control:
			  *
			  *		<Attribute id="Type" value="-1|0..."/> \n
			  *		<Attribute id="MovType" value="-1|0.."/> \n
			  *		<Attribute id="EnergyAdded" value="-1000|1000,-100..100"/> \n
			  *		<Attribute id="Speed" value="-,+"/> \n
			  *	</Attributes>
			  */
			Properties (const std::map <std::string, std::string>& prps);

			/** @see parent. */
			virtual QGAMES::SetOfOpenValues asSetOfOpenValues () const;
			virtual void fromSetOfOpenValues (const QGAMES::SetOfOpenValues& oV);

			QGAMES::bdata _energy; 
		};

		StdEnergyElementSceneActionBlock (int id, Properties* prps, int nE = 1 /** 1 by default. */)
			: StdThingToCathActionBlock (id, prps, nE)
							{ }

		protected:
		/** @see parent. 
			To stablish the energy to add. */
		virtual void getAndConfigureSpaceElements (const std::vector <int>& elmts = std::vector <int> ());

		/** @see parent. */
		virtual std::function <bool (SpaceElement*)> isElementValidForActionBlock () const
							{ return ([](SpaceElement* spc) -> bool 
								{ return (spc != NULL && 
										  dynamic_cast <EnergyElement*> (spc) != NULL &&
										  !spc -> isVisible ()); }); } 
	};

	/** An action block to control the live.
		Usually this type of action block is not defined in the configuration files. */
	class StdLiveSceneActionBlock : public StdThingToCathActionBlock
	{
		public:
		StdLiveSceneActionBlock (int id, Properties* prps, int nT = 1)
			: StdThingToCathActionBlock (id, prps, nT)
							{ }

		protected:
		/** @see parent. */
		virtual std::function <bool (BATTLESHIP::SpaceElement*)> isElementValidForActionBlock () const override final
							{ return ([](BATTLESHIP::SpaceElement* spc) -> bool 
								{ return (spc != NULL && 
										  dynamic_cast <LiveToCatch*> (spc) != NULL &&
										  !spc -> isVisible ()); }); } 
	};

	/** An action block to control the shield.
		Usually this type of action block is not defined in the configuration files. */
	class StdShieldSceneActionBlock : public StdThingToCathActionBlock
	{
		public:
		StdShieldSceneActionBlock (int id, Properties* prps, int nT = 1)
			: StdThingToCathActionBlock (id, prps, nT)
							{ }

		protected:
		/** @see parent. */
		virtual std::function <bool (BATTLESHIP::SpaceElement*)> isElementValidForActionBlock () const override final
							{ return ([](BATTLESHIP::SpaceElement* spc) -> bool 
								{ return (spc != NULL && 
										  dynamic_cast <ShieldToCatch*> (spc) != NULL &&
										  !spc -> isVisible ()); }); } 
	};
}

#endif
  
// End of the file
/*@}*/
