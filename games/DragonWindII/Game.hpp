/** \ingroup DWTheRevege */
/*@{*/

/**	
 *	@file	
 *	File: TestDragonwind.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 04/01/2019 \n
 *	Description: Testing how the Dragonwind library works. \n
 *	Versions: 1.0 Initial
 */

#ifndef __GAMETEST_DRAGONWINDGAME__
#define __GAMETEST_DRAGONWINDGAME__

#include <DragonwindLike/dwinclude.hpp>
#include "Defs.hpp"
#include "DataGame.hpp"
#include "Builders.hpp"

namespace DragonTheRevenge
{
	class Game : public DRAGONWIND::Game
	{
		public:
		class Conf : public DRAGONWIND::Game::Conf
		{
			public:
			Conf (int nP, int mL = 3)
				: DRAGONWIND::Game::Conf (nP, __DRAGONWIND_EXAMPLEWORLDID__, __DRAGONWIND_EXAMPLESCENEID__),
				  _maxLives (mL)
							{ adjustToPlayers (nP); }

			/** @see parent. */
			virtual Configuration* clone () const
							{ Conf* result = new Conf (*this); 
							  result -> cloneImplementation (this); 
							  return (result); }

			protected:
			/** @see parent. */
			virtual void adjustToPlayers (int nP);

			private:
			/** The max number of lives defined in configuration. */
			int _maxLives;
		};

		Game ()
			: DRAGONWIND::Game (),
			  _showFPS (false) // Not to show by default...
							{ delete (_theDataGame); /** The new */ _theDataGame = new DataGame; }

		/** To switch the visualization of the fPS. */
		void setShowFPS (bool s);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt);

		private:
		/** see parent. */
		virtual QGAMES::InputHandler* createInputHandler ()
							{ return (implementation () -> createInputHandler (new DRAGONWIND::InputHandlerBehaviour ())); }
		virtual QGAMES::EntityBuilder* createEntityBuilder ()
							{ return (new EntityBuilder (parameter (__GAME_PROPERTYENTITIESFILE__), 
								formBuilder (), movementBuilder ())); }
		virtual QGAMES::MovementBuilder* createMovementBuilder () 
							{ return (new MovementBuilder (parameter (__GAME_PROPERTYMOVEMENTSFILE__))); }
		virtual QGAMES::WorldBuilder* createWorldBuilder ()
							{ return (new WorldBuilder (parameter (__GAME_PROPERTYWORLDSFILE__), 
								mapBuilder ())); }
		virtual QGAMES::MapBuilder* createMapBuilder ();
		virtual QGAMES::CharacterControlStepsMonitorBuilder* createCharacterMonitorBuilder ()
							{ return (new DRAGONWIND::CharacterControlStepsMonitorBuilder 
								(parameter (__GAME_PROPERTUCHARACTERMONITORFILE__))); }

		/** @see parent. */
		virtual Configuration* createConfiguration ();

		/** @see parent. */
		virtual std::string defaultParameter (const std::string& p) const;

		/** @see parent. */
		virtual void initialize ();
		virtual void finalize ();

		private:
		/** To show or not the FPS in the right bottom corner. */
		bool _showFPS;
	};
}

#endif
  
// End of the file
/*@}*/