/** \ingroup BSII */
/*@{*/

/**	
 *	@file	
 *	File: SceneActionBlocks.hpp \n
 *	Game: Battleship II \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 21/11/2020 \n
 *	Description: Defines the specific action blocks in Battleship II \n
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIPII_SCENEACTIONBLOCKS__
#define __BATTLESHIPII_SCENEACTIONBLOCKS__

#include "Defs.hpp"
#include "Artists.hpp"
#include <BattleshipLike/bsinclude.hpp>
#include <BattleshipLike/bsactionblocks.hpp>

namespace BattleshipII
{
	/** An action block to control the Mothership UFOs. */
	class MothershipUFOSceneActionBlock : public BATTLESHIP::StdUFOSceneActionBlock
	{
		public:
		MothershipUFOSceneActionBlock (int id, BATTLESHIP::StdUFOSceneActionBlock::Properties* prps,
				BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF = 
					new BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory)
			: BATTLESHIP::StdUFOSceneActionBlock (id, prps, 1 /** Always. */, cF)
							{ }

		protected:
		/** @see parent. */
		virtual std::function <bool (BATTLESHIP::SpaceElement*)> isElementValidForActionBlock () const override final
							{ return ([](BATTLESHIP::SpaceElement* spc) -> bool 
								{ return (spc != NULL && 
										  dynamic_cast <MothershipUFO*> (spc) != NULL &&
										  !spc -> isVisible ()); }); } 
	};

	/** A simple modification to the action block for set of UFOs, especially for the scenes with a mothership. \n
		In these scenes the scenes mves following the lines described by DancingInLinesForBigUFOSCombatFormationFactory,
		but the first movement has to be done inmediatly because it consists on moving the UFO to the middle of the screen. */
	class MothershipUFOSetSceneActionBlock : public BATTLESHIP::StdSetUFOsSceneActionBlock
	{
		public:
		MothershipUFOSetSceneActionBlock (int id, Properties* prps = new Properties ());

		/** @see parent.
			To save the state of the _firstTime variable. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg);

		/** @see parent. */
		virtual void initialize ();
		virtual void updatePositions ();

		protected:
		// Implementation
		bool _firstTime;
	};

	/** A new type of combat block. 
		The ufos appear in the left side of the screen, and then move from the left to the right
		each of them with PI/2 radians of delay. 
		The closest pattern to follow is "all followinf the leader". */
	class SinusoideFromLeftBlockAndFlyCombatFormationFactory : 
		public BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackOneByOneCombatFormationFactory
	{
		public:
		/** 
			* To create a queue of UFO following the first:
			* @param cF:	The curve and position factory used to create individual action blocks.
			* @param t:	The time between the UFOS when starting to move. 
			*/
		SinusoideFromLeftBlockAndFlyCombatFormationFactory 
			(BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF, QGAMES::bdata sBE = 0.05,
			 const BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::SingleActionBlockFunction& eABlock = __SINGLESTDUFOSCTIONBLOCK);

		/** @see parent. */
		virtual bool forCombatType (int t) const override final
						{ return (t == __BATTLESHIPII_STAYANDMOVESINFROMLEFTTORIGHT__); }

		private:
		/** The separattion between elements. */
		QGAMES::bdata _separationBetweenElements;
	};

	/** Similar to the previous one but starting from the right. */
	class SinusoideFromRightBlockAndFlyCombatFormationFactory : 
		public BATTLESHIP::StdSetUFOsSceneActionBlock::StayAndAttackOneByOneCombatFormationFactory
	{
		public:
		/** 
			* To create a queue of UFO following the first:
			* @param cF:	The curve and position factory used to create individual action blocks.
			* @param t:	The time between the UFOS when starting to move. 
			*/
		SinusoideFromRightBlockAndFlyCombatFormationFactory 
			(BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF, QGAMES::bdata sBE = 0.05,
			 const BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::SingleActionBlockFunction& eABlock = __SINGLESTDUFOSCTIONBLOCK);

		/** @see parent. */
		virtual bool forCombatType (int t) const override final
						{ return (t == __BATTLESHIPII_STAYANDMOVESINFROMRIGHTTOLEFT__); }

		private:
		QGAMES::bdata _separationBetweenElements;
	};

	/** Dancing but for motherships. */
	class DancingInLinesForBigUFOSCombatFormationFactory : 
		public BATTLESHIP::StdSetUFOsSceneActionBlock::DancingInLinesCombatFormationFactory
	{
		public:
		DancingInLinesForBigUFOSCombatFormationFactory (
			BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF, int t = 10,
			const BATTLESHIP::StdSetUFOsSceneActionBlock::CombatFormationFactory::SingleActionBlockFunction& eABlock =
				[](int i, BATTLESHIP::StdUFOSceneActionBlock::CurveAndInitialPositionFactory* cF) -> BATTLESHIP::StdUFOSceneActionBlock* 
					{ return (new MothershipUFOSceneActionBlock (i, 
						new BATTLESHIP::StdUFOSceneActionBlock::Properties (), cF -> clone ())); });

		/** @see parent. */
		virtual bool forCombatType (int t) const override final
						{ return (t == __BATTLESHIPII_BIGUFODANCINGINLINES__); }

		protected:
		bool _firstMovement;
	};
}

#endif
  
// End of the file
/*@}*/
