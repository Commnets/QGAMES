/** \ingroup PacManII */
/*@{*/

/**	
 *	@file	
 *	File: SceneActionBlocks.hpp \n
 *	Game: PacManII \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 29/01/2021 \n
 *	Description: Define the main action blocks of any PacManII game: \n
 *				 To control how a monster appears random in the maze and starts to move, 
 *				 and a shield to appear in a specific position once it has been decided so.
 *	Versions: 1.0 Initial
 */

#ifndef __PACMANII_SCENEACTIONBLOCKS__
#define __PACMANII_SCENEACTIONBLOCKS__

#include <PacManLike/pcinclude.hpp>

namespace PacManII
{
	/** To move Wormy. */
	class WormyMonsterSceneActionBlock : public PACMAN::MonsterSceneActionBlock
	{
		public:
		struct Properties
		{
			Properties ()
				: _monsterProperties (), _trailLength (12)
							{ }

			Properties (PACMAN::MonsterSceneActionBlock::Properties mprps, int tL)
				: _monsterProperties (mprps),
				  _trailLength (tL)
							{ assert (_trailLength >= 2); }

			Properties (const QGAMES::SceneActionBlockProperties& prps);

			Properties (const Properties&) = default;

			Properties& operator = (const Properties&) = default;

			PACMAN::MonsterSceneActionBlock::Properties _monsterProperties;
			int _trailLength;
		};

		WormyMonsterSceneActionBlock (int id, const Properties& prps)
			: PACMAN::MonsterSceneActionBlock (id, prps._monsterProperties),
			  _properties (prps)
							{ }

		// Never change properties after being initialized...
		void setProperties (const Properties& prps)
							{ _properties = prps; 
							  PACMAN::MonsterSceneActionBlock::_properties = prps._monsterProperties; } // To be compatible...
		const Properties& properties () const
							{ return (_properties); }

		virtual void initialize () override;

		protected:
		Properties _properties;
	};

	/** An element to appear in a random place of the maze and then anothr action block takes the control. */
	class ElementToAppearSceneActionBlock : public PACMAN::SceneActionBlock
	{
		public:
		struct Properties  
		{
			Properties ()
				: _entityId (0),
				  _position (QGAMES::MazeModel::_noPosition),
				  _ballsEatenToAppear (40),
				  _secondsMaxToAppear (__BD 2.0),
				  _blockToControlElement (-1) // Meaning not defined
							{ }

			Properties (int eId, const QGAMES::MazeModel::PositionInMaze& p, int bA, QGAMES::bdata sA, int bAc)
				: _entityId (eId),
				  _position (p),
				  _ballsEatenToAppear (bA), 
				  _secondsMaxToAppear (sA),
				  _blockToControlElement (bAc)
							{ }

			Properties (const QGAMES::SceneActionBlockProperties& prps);

			Properties (const Properties&) = default;

			Properties& operator = (const Properties&) = default;

			int _entityId;
			QGAMES::MazeModel::PositionInMaze _position;
			int _ballsEatenToAppear;
			QGAMES::bdata _secondsMaxToAppear;
			int _blockToControlElement;
		};

		ElementToAppearSceneActionBlock (int id, const Properties& prps)
			: PACMAN::SceneActionBlock (id),
			  _properties (prps),
			  _element (nullptr)
							{ }

		virtual QGAMES::SetOfOpenValues runtimeValues () const override;
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg) override;

		// Never change properties after being initialized...
		void setProperties (const Properties& prps)
							{ _properties = prps; }
		const Properties& properties () const
							{ return (_properties); }

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		protected:
		__DECLARECOUNTERS__ (Counters)
		virtual QGAMES::Counters* createCounters () override
							{ return (new Counters); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches)
		virtual QGAMES::OnOffSwitches* createOnOffSwitches () override
							{ return (new OnOffSwitches); }

		protected:
		/** The properties of the action block. */
		Properties _properties;

		/** The counters & switches. */
		static const int _COUNTERTOAPPEAR = 0;
		static const int _COUNTERTOFADE = 1;
		static const int _COUNTERFADE = 2;
		static const int _SWITCHAPPEARING = 0;
		static const int _SWITCHAPPEARED = 1;
		static const int _FADE [14];

		// Implementation
		PACMAN::PacmanElement* _element;
	};
}

#endif
  
// End of the file
/*@}*/