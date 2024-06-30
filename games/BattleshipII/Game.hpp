/** \ingroup BSII */
/*@{*/

/**	
 *	@file	
 *	File: Game.hpp \n
 *	Game: Battleship II \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 23/03/2020 \n
 *	Description: To manipulate the very basics of the Battleship II game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIPII_GAME__
#define __BATTLESHIPII_GAME__

#include "Defs.hpp"
#include "Builders.hpp"
#include <BattleshipLike/bsinclude.hpp>

namespace BattleshipII
{
	/**
	  * Just to change a couple of default parameters:
	  * _numberOfUFOTypes.
	  */
	struct DataGame : public BATTLESHIP::DataGame
	{
		public:
		DataGame ();

		/** @see parent. */
		virtual std::vector <int> defaultStatesForUFOType (int uT) const override final;
		virtual inline const std::vector <int>& ufoTypesList () const override final;
	
		/** @see parent. */
		virtual QGAMES::Position defaultInitialPointForMovementType (int nC, 
			const QGAMES::Rectangle& vZ, const QGAMES::Rectangle& mZ) override final;
		virtual std::map <int, double> defaultParametersForMovementType (int nC, 
			QGAMES::bdata sF, const QGAMES::Rectangle& vZ, const QGAMES::Rectangle& mZ, const QGAMES::Position& iP, 
			QGAMES::bdata& tR) override final;
		virtual inline const std::map <int, int>& curveforMovementTypeMap () const override final;
	};

	class Game : public BATTLESHIP::Game
	{
		public:
		class Conf : public BATTLESHIP::Game::Conf
		{
			public:
			Conf (int nP, int mL = 3)
				: BATTLESHIP::Game::Conf (nP, __BATTLESHIP_EXAMPLEWORLDID__, __BATTLESHIP_TIMELIMITEXAMPLESCENEID__),
				  _maxLives (mL)
							{ adjustToPlayers (nP); }

			/** @see parent. */
			virtual Configuration* clone () const override final
							{ Conf* result = new Conf (*this); 
							  result -> cloneImplementation (this); 
							  return (result); }

			protected:
			/** @see parent. */
			virtual void adjustToPlayers (int nP) override final;

			private:
			/** The max number of lives defined in configuration. */
			int _maxLives;
		};

		Game ();

		/** To switch the visualization of the fPS. */
		void setShowFPS (bool s);

		/** @see parent. */
		virtual void processEvent (const QGAMES::Event& evnt) override final;

		private:
		/** see parent. */
		virtual QGAMES::InputHandler* createInputHandler () override final
							{ return (implementation () -> createInputHandler (new BATTLESHIP::InputHandlerBehaviour ())); }
		virtual QGAMES::EntityBuilder* createEntityBuilder () override final
							{ return (new EntityBuilder (parameter (__GAME_PROPERTYENTITIESFILE__), 
									formBuilder (), movementBuilder ())); }
		virtual QGAMES::MovementBuilder* createMovementBuilder () override final
							{ return (new MovementBuilder (parameter (__GAME_PROPERTYMOVEMENTSFILE__))); }
		virtual QGAMES::WorldBuilder* createWorldBuilder () override final
							{ return (new WorldBuilder (parameter (__GAME_PROPERTYWORLDSFILE__), 
								mapBuilder ())); }
		virtual QGAMES::CharacterControlStepsMonitorBuilder* createCharacterMonitorBuilder () override final
							{ return (new QGAMES::CharacterControlStepsMonitorBuilder 
								(parameter (__GAME_PROPERTUCHARACTERMONITORFILE__))); }

		/** @see parent. */
		virtual Configuration* createConfiguration () override final;

		/** @see parent. */
		virtual std::string defaultParameter (const std::string& p) const override final;

		/** @see parent. */
		virtual void initialize () override final;
		virtual void finalize () override final;

		private:
		/** To show or not the FPS in the right bottom corner. */
		bool _showFPS;
	};
}

#endif
  
// End of the file
/*@}*/