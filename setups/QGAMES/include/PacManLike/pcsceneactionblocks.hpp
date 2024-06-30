/** \ingroup PACMAN */
/*@{*/

/**	
 *	@file	
 *	File: pcsceneactionblocks.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: Define the main action blocks of any PACMAN game: \n
 *				 On to control the monsters and other to show the fruit. \n
 *				 The spcific behaviour of the original pacman is replicated in each.
 *	Versions: 1.0 Initial
 */

#ifndef __PACMAN_SCENEACTIONBLOCKS__
#define __PACMAN_SCENEACTIONBLOCKS__

#include <PacManLike/pcdefs.hpp>
#include <PacManLike/pcartists.hpp>

#include <advancedArcade/adinclude.hpp>

namespace PACMAN
{
	class Scene;
	class Map;

	/** A general behavior of the action blocks. */
	class SceneActionBlock : public QGAMES::SceneActionBlock
	{
		public:
		SceneActionBlock () = delete;

		SceneActionBlock (int id)
			: QGAMES::SceneActionBlock (id)
							{ }

		SceneActionBlock (const SceneActionBlock&) = delete;

		SceneActionBlock& operator = (const SceneActionBlock&) = delete; 

		/** Just to avoid dealing with information of switches and counters that it is not needed to keep in pacman. */
		virtual QGAMES::SetOfOpenValues runtimeValues () const override 
							{ return (QGAMES::SetOfOpenValues ()); }
		virtual void initializeRuntimeValuesFrom (const QGAMES::SetOfOpenValues& cfg) override { }

		protected:
		/** Returns the PacMan scene used. */
		inline const Scene* pScene () const; // Not defined here to avoid recursive #include
		/** Same than above. */
		inline Scene* pScene ();
		/** Returns the PacMan map of the scene used. */
		inline const Map* pMap () const;
		/** Same than above. */
		inline Map* pMap ();

	};

	/** The behaviour of the Things in the screen. */
	class ThingSceneActionBlock : public SceneActionBlock
	{
		public:
		struct Properties
		{
			Properties ()
				: _entityId (0),
				  _types ({ 0 }),
				  _points ({ 200 }),
				  _ballsEatenToAppear ({ 40 }), 
				  _secondsToDisappear ({ __BD 10 }),
				  _speeds ({ __BD 0 }),
				  _positionsId ({-1 }) // Where the thing will be shown! (type) Meaning -1 random
							{ }

			Properties (int eId, const std::vector <int>& tp, const std::vector <int>& p, 
					const std::vector <int>&bE, const std::vector <QGAMES::bdata>& sD, 
					const std::vector <QGAMES::bdata>& spd, const std::vector <int>& pId)
				: _entityId (eId),
				  _types (tp), 
				  _points (p),
				  _ballsEatenToAppear (bE), 
				  _secondsToDisappear (sD),
				  _speeds (spd),
				  _positionsId (pId)
							{ int sz = (int) _types.size ();
							  assert ((int) _points.size () == sz &&
									  (int) _ballsEatenToAppear.size () == sz && 
									  (int) _secondsToDisappear.size () == sz &&
									  (int) _speeds.size () == sz &&
									  (int) _positionsId.size () == sz &&
									  !_types.empty ()); }

			Properties (const QGAMES::SceneActionBlockProperties& prps);

			Properties (const Properties&) = default;

			Properties& operator = (const Properties&) = default;

			int _entityId;
			std::vector <int> _types;
			std::vector <int> _points;
			std::vector <int> _ballsEatenToAppear;
			std::vector <QGAMES::bdata> _secondsToDisappear;
			std::vector <QGAMES::bdata> _speeds;
			std::vector <int> _positionsId;
		};

		ThingSceneActionBlock (int id, const Properties& prps)
			: SceneActionBlock (id),
			  _properties (prps),
			  _thing (nullptr)
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

		protected:
		__DECLARECOUNTERS__ (Counters);
		virtual QGAMES::Counters* createCounters () override
							{ return (new Counters); }
		__DECLAREONOFFSWITCHES__ (OnOffSwitches)
		virtual QGAMES::OnOffSwitches* createOnOffSwitches () override
							{ return (new OnOffSwitches); }

		/** To overload later, to define al alternative initial position of the thing. 
			If the method return noPosition a default one will be selected. */
		virtual QGAMES::MazeModel::PositionInMaze initialMazePosition ()
							{ return (QGAMES::MazeModel::_noPosition); }

		protected:
		/** The properties. */
		Properties _properties;

		/** The counters and the switches. */
		static const int _COUNTERNUMBERTHING = 0;
		static const int _COUNTERTODISAPPEARTHING = 1;
		static const int _SWITCHBLOCKACTIVE = 0;
		static const int _SWITCHTHINGONSCENE = 1;
		static const int _SWITCHTHINGSHOWN = 2;

		// Implementation
		Thing* _thing;
	};

}

#include <PacManLike/pcstandardsceneactionblocks.hpp>

#endif
  
// End of the file
/*@}*/