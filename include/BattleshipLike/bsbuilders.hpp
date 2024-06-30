/** \ingroup Battleship */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: bsbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/11/2019 \n
 *	Description: Defines specific builders for Battleship like projects. \n
 *	Versions: 1.0 Initial
 */

#ifndef __BATTLESHIP_BUILDERS__
#define __BATTLESHIP_BUILDERS__

#include <BattleshipLike/bsdefinitions.hpp>
#include <AdvancedArcade/adinclude.hpp>

namespace BATTLESHIP
{
	class Battleship;
	class Spaceship;
	class SpaceshipShooting;
	class Asteroid;
	class EnergyElement;
	class ThingToCatch;

	/** The standard GUI System builder for Battleship like games. \n
		The GUI System created is always the basic one, 
		except when it is required the one with the options shown when playing. */
	class GUISystemBuilder : public QGAMES::StandardGUISystemBuilder
	{
		public:
		GUISystemBuilder (const std::string& fDef, QGAMES::FormBuilder* fB)
			:  QGAMES::StandardGUISystemBuilder (fDef, fB)
							{ }

		protected:
		/** @see parent. */
		virtual QGAMES::GUISystem* createSystem (const QGAMES::GUISystemBuilder::GUISystemDefinition& def, 
			QGAMES::CompositeWidget* mW);
		virtual QGAMES::Widget* createWidget (const QGAMES::GUISystemBuilder::WidgetDefinition& def);
	};

	/** To create the entities of the Battleship like games. */
	class EntityBuilder : public QGAMES::AdvancedEntityBuilder
	{
		public:
		EntityBuilder (const std::string& eDef, QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
			: QGAMES::AdvancedEntityBuilder (eDef, fB, mB)
							{ }

		protected:
		/** @see parent. */
		virtual QGAMES::Entity* createEntity (const QGAMES::EntityBuilder::EntityDefinition& def);

		// This new methods have been defined to create the specific types needed in this type of games. 
		// A default implementation is given for each, but it can be overloaded...
		virtual Spaceship* createSpaceship (const QGAMES::EntityBuilder::EntityDefinition& def);
		virtual SpaceshipShooting* createSpaceshipShooting (const QGAMES::EntityBuilder::EntityDefinition& def);
		virtual Asteroid* createAsteroid (const QGAMES::EntityBuilder::EntityDefinition& def);
		/** By default the first out of 5 (the defined) elements will be pills of energy, 
			1 will be a energy element, 1 a heart of live, and the last one a shiled to cover ownerselvers. */
		virtual ThingToCatch* createThingToCatch (const QGAMES::EntityBuilder::EntityDefinition& def);
		virtual Battleship* createBattleship (const QGAMES::EntityBuilder::EntityDefinition& def);
	};

	/** To create the worlds and scenes of a Battleship like game. */
	class WorldBuilder : public QGAMES::WorldBuilder
	{
		public:
		WorldBuilder (const std::string& eDef, QGAMES::MapBuilder* mB)
			: QGAMES::WorldBuilder (eDef, mB)
							{ }

		protected:
		/** @see parent. 
			By default just only BATTLESHIP::Scene and World are created. */
		virtual QGAMES::World* createWorldObject (int no, const QGAMES::Scenes& s, const QGAMES::WorldProperties& p);
		virtual QGAMES::Scene* createSceneObject (int ns, const QGAMES::Maps& m, const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL);
		virtual QGAMES::SceneActionBlock* createSceneActionBlockObject (int nAB, 
			const QGAMES::SceneActionBlockProperties& prps);
	};

	/** To create the movements of the Battleship like game. 
		Those movement by the ways are quite simple. Just only linear movements and no movements are needed 
		at least in the default basic implementation. */
	class MovementBuilder : public QGAMES::AdvancedMovementBuilder
	{
		public:
		MovementBuilder (const std::string& fDef, 
				QGAMES::CurveTemplateBuilder* cB = new QGAMES::CurveTemplateBuilder)
			: QGAMES::AdvancedMovementBuilder (fDef, cB)
								{ }
	
		protected:
		/** @see parent. */
		virtual QGAMES::Movement* createMovement (const QGAMES::MovementBuilder::MovementDefinition& def);
	};

	/** To create the game state to initialize the game. */
	class ForInitializeBattleshipGameStateAddOn : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForInitializeBattleshipGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __BATTLESHIP_GAMESTATEINITIALIZEGAMETYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create a game state add on that shows the battlehip in the right side of the screen moving randomly. */
	class ForShowingBattleshipMovingGameStateAddOn : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingBattleshipMovingGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __BATTLESHIP_GAMESTATEMOVINGBATTLESHIPTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create a game state add on to show the name of the player. */
	class ForShowingPlayerNameGameStateAddOn : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingPlayerNameGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __BATTLESHIP_GAMESTATEPLAYERNAMETYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create a game state add on to show the name of the current world. */
	class ForShowingWorldNameGameStateAddOn : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingWorldNameGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __BATTLESHIP_GAMESTATEWORLDNAMETYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create a game state add on to show the name of the current scene. */
	class ForShowingSceneNameGameStateAddOn : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingSceneNameGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __BATTLESHIP_GAMESTATESCENENAMETYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create the special credit title game state.
		Two additional parameters are added to define an object of this type:
		The number of the credit title that is the title of the game and then a battleship has to be shown behind.
		The content of the credit that has to be shown in a bigger mode.
		<Attributes>
			<Attribute id="NUMBERTITLE" value="XX"/>
			<Attribute id="CONTENTTITLE" value="YYY"/>
		</Attributes>
	 */
	class ForCreditTitlesGameStateAddOn : public QGAMES::ForCreditTitlesGameStateAddOn
	{
		public:
		ForCreditTitlesGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::ForCreditTitlesGameStateAddOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __BATTLESHIP_GAMESTATECREDITTITLESTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create the special type of menu needed here.
		That is like the standard one defined in AdvancedArcade but adding a couple of options more 
		to first including the possibility of selecting the type of battleship, and second
		to consider additional control keys in the keyboard. */
	class ForMenuAddsOn : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForMenuAddsOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __BATTLESHIP_GAMESTATEMENUTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create the control playing game state of this type of games. 
		One additional parameter is added to define an object of this type:
		The id of the artist being moved (it has to be defined in the entities.xml definition file.
		<Attributes>
			<Attribute id="ARTISTID" value="X"/>
		</Attributes>
	*/
	class ForPlayingControlGameStateAddOn : public QGAMES::GameStateBuilder::ForControlAddsOn
	{
		public:
		ForPlayingControlGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::ForControlAddsOn (b)
							{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __BATTLESHIP_GAMESTATECONTROLPLAYINGTYPE__); }

		protected:
		/** @see parent. */
		virtual QGAMES::GameState* createControlGameState 
			(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
			 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl);
	};

	/** To create the game state to play. */
	class ForPlayingGameStateAddOn : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForPlayingGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
							{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __BATTLESHIP_GAMESTATEPLAYINGTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create the game state with the options when playing. */
	class ForOptionsWhenPlayingGameStateAddOn : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForOptionsWhenPlayingGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
							{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __BATTLESHIP_GAESTATEOPTIONSWHENPLAYTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create the game state talking about the hall of fame in this type of games. 
		The parameters are the same than the ones used for the basic hall of fame game state defined in AdvacedArcade
		but what changes is the ways the information is understood. */
	class ForShowingHallOfFameGameStateAddOn : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingHallOfFameGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
							{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __BATTLESHIP_GAMESTATEHALLOFFAMETYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create the special object to control the flow of the game. */
	class ForGameStateControlAddon : public QGAMES::ForAdvancedGameStateControlAddon
	{
		public:
		ForGameStateControlAddon (QGAMES::GameStateBuilder* b)
			: QGAMES::ForAdvancedGameStateControlAddon (b)
							{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (QGAMES::ForAdvancedGameStateControlAddon::canCreateGameState (t) ||
								  t == __BATTLESHIP_GAMESTATECTRLTYPE__); }

		protected:
		/** @see parent. */
		virtual QGAMES::GameState* createControlGameState 
			(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
			 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl);
	};
}

#endif
  
// End of the file
/*@}*/