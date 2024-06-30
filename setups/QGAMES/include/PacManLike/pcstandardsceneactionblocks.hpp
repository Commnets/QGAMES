/** \ingroup PACMAN */
/*@{*/

/**	
 *	@file	
 *	File: pcstandradsceneactionblocks.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 29/01/2022 \n
 *	Description: Define the specific action block to manage monsters and fruits.
 *	Versions: 1.0 Initial
 */

#ifndef __PACMAN_STANDARDSCENEACTIONBLOCKS__
#define __PACMAN_STANDARDSCENEACTIONBLOCKS__

#include <PacManLike/pcsceneactionblocks.hpp>

namespace PACMAN
{
	/** The behaviour of a monster. */
	class MonsterSceneActionBlock : public SceneActionBlock
	{
		public:
		struct Properties
		{
			Properties ()
				: _initializeElement (true), _entityId (0), _numberInMap (0), _points (0)
							{ }

			Properties (bool lF, int eId, int nM, int p)
				: _initializeElement (lF), _entityId (eId), _numberInMap (nM), _points (p)
							{ }

			Properties (const QGAMES::SceneActionBlockProperties& prps);

			Properties (const Properties&) = default;

			Properties& operator = (const Properties&) = default;

			bool _initializeElement;
			int _entityId;
			int _numberInMap;
			int _points;
		};

		MonsterSceneActionBlock (int id, const Properties& prps)
			: SceneActionBlock (id),
			  _properties (prps),
			  _monster (nullptr)
							{ }

		// Never change properties after being initialized...
		void setProperties (const Properties& prps)
							{ _properties = prps; }
		const Properties& properties () const
							{ return (_properties); }

		// To manage the action block from the scene...
		/** To know the number of monster being managed here. */
		int monsterNumber () const
							{ return ((_monster == nullptr) ? -1 : _monster -> monsterNumber ()); }
		/** To know whether the monster is already moving. */
		bool isMonsterMoving () const
							{ return (onOffSwitch (_SWITCHMOVING) -> isOn ()); }
		/** When te scene determines a monster has to departure, 
			its number is communicated to all action blocks. 
			Returns true when the instruction was accepted. */
		bool timeToStart (int nM);

		virtual void initialize () override;
		virtual void updatePositions () override;
		virtual void finalize () override;

		virtual void processEvent (const QGAMES::Event& evnt) override;

		protected:
		/** To determine other potential conditions to start moving. 
			It could be overloaded later. By default takes into account the number of balls eaten. */
		virtual bool readyToStart ();
		/** To execute the elron condition if any. */
		virtual bool elronCondition ();

		__DECLAREONOFFSWITCHES__ (OnOffSwitches)
		virtual QGAMES::OnOffSwitches* createOnOffSwitches () override
							{ return (new OnOffSwitches); }

		protected:
		/** The properties of the action block. */
		Properties _properties;

		/** The switches. */
		static const int _SWITCHTOSTART = 0;
		static const int _SWITCHMOVING = 1;

		// Implementation
		Monster* _monster;
	};
}

#endif
  
// End of the file
/*@}*/