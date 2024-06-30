/** \ingroup DoomCastle */
/*@{*/

/**	
 *	@file	
 *	File: Game.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 02/04/2021 \n
 *	Description: Central class for DoomCastle. \n
 *	Versions: 1.0 Initial
 */

#ifndef __DOOMCASTLE_GAME__
#define __DOOMCASTLE_GAME__

#include <Platform/ptinclude.hpp>

#include "Defs.hpp"
#include "DataGame.hpp"
#include "Maze.hpp"
#include "InputHandler.hpp"
#include "Builders.hpp"

namespace DOOMCASTLE
{
	class Game final : public QGAMES::PlatformGame
	{
		public:
		/** The configuration class. */
		class Conf final : public QGAMES::PlatformGame::Conf
		{
			public:
			Conf () = delete;

			Conf (int nP, int mL = 3, int iW = __DOOMCASTLE_EXAMPLEWORLDID__, int iS = __DOOMCASTLE_EXAMPLESCENEID__);

			Conf (const Conf& cfg) = default; 

			Conf& operator = (const Conf& cfg) = default; 

			/** @see parent. */
			virtual Configuration* clone () const override;

			/** To get the maze. */
			const MazeModel& mazeModel () const
							{ return (_theMazeModel); }

			protected:
			/** @see parent. */
			virtual void adjustToPlayers (int nP) override;

			/** @see parent. */
			virtual void cfgToStream (std::ostringstream& oS) override;
			virtual void cfgFromStream (std::istringstream& iS) override;

			private:
			/** The max number of lives defined in configuration. */
			int _maxLives;
			/** The initial world and scene. */
			int _initialWorld, _initialScene;
			/** A reference to the maze. */
			MazeModel _theMazeModel;
		};

		Game ()
			: QGAMES::PlatformGame (),
			  _showFPS (false), // Not to show by default...
			  _theDataGame ()
							{ }

		Game (const Game&) = delete;

		Game& operator = (const Game&) = delete;

		// To get the data game class (parametrization values)
		const DataGame& dataGame () const
							{ return (_theDataGame); }

		// To get the maze model
		const MazeModel& mazeModel () const
							{ return (dynamic_cast <const Conf*> (configuration ()) -> mazeModel ()); }

		/** To switch the visualization of the fPS. */
		void setShowFPS (bool s);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt) override;

		private:
		/** see parent. */
		virtual QGAMES::InputHandler* createInputHandler () override
							{ return (implementation () -> createInputHandler (new InputHandlerBehaviour ())); }
		virtual QGAMES::EntityBuilder* createEntityBuilder () override
							{ return (new EntityBuilder (parameter (__GAME_PROPERTYENTITIESFILE__), 
								formBuilder (), movementBuilder ())); }
		virtual QGAMES::MovementBuilder* createMovementBuilder () override
							{ return (new MovementBuilder (parameter (__GAME_PROPERTYMOVEMENTSFILE__))); }
		virtual QGAMES::WorldBuilder* createWorldBuilder () override
							{ return (new WorldBuilder (parameter (__GAME_PROPERTYWORLDSFILE__), 
								mapBuilder ())); }
		virtual QGAMES::MapBuilder* createMapBuilder () override;
		virtual QGAMES::CharacterControlStepsMonitorBuilder* createCharacterMonitorBuilder () override
							{ return (new QGAMES::CharacterControlStepsMonitorBuilder 
								(parameter (__GAME_PROPERTUCHARACTERMONITORFILE__))); }

		/** @see parent. */
		virtual Configuration* createConfiguration () override;

		/** @see parent. */
		virtual std::string defaultParameter (const std::string& p) const override;

		/** @see parent. */
		virtual void initialize () override;
		virtual void finalize () override;

		private:
		/** To show or not the FPS in the right bottom corner. */
		bool _showFPS;
		/** The data game. */
		DataGame _theDataGame;
	};
}

#endif
  
// End of the file
/*@}*/