/** \ingroup Dragonwind */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: dwbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 09/03/2019 \n
 *	Description: Defines specific builders for Dragonwind like projects. \n
 *	Versions: 1.0 Initial
 */

#ifndef __DRAGONWIND_BUILDERS__
#define __DRAGONWIND_BUILDERS__

#include <DragonwindLike/dwdefinitions.hpp>
#include <DragonwindLike/dwartists.hpp>
#include <Platform/ptinclude.hpp>

namespace DRAGONWIND
{
	/** The standard GUI System builder for Dragonwind like game
		It is used by most of the implementations. */
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

	/** To create the entities of the Dragonwind like games. */
	class EntityBuilder : public QGAMES::PlatformEntityBuilder
	{
		public:
		EntityBuilder (const std::string& eDef, QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
			: QGAMES::PlatformEntityBuilder (eDef, fB, mB)
							{ }

		protected:
		/** @see parent. */
		virtual QGAMES::Entity* createEntity (const QGAMES::EntityBuilder::EntityDefinition& def);
		virtual QGAMES::CompositeEntity* createCompositeEntity (const QGAMES::EntityBuilder::EntityDefinition& def);

		/** @see parent. */
		virtual QGAMES::SomethingToEat* createSomethingToEat (const QGAMES::EntityBuilder::EntityDefinition& def);
		virtual QGAMES::SomethingToCatch* createSomethingToCatch (const QGAMES::EntityBuilder::EntityDefinition& def);
		virtual QGAMES::BadGuy* createBadGuy (const QGAMES::EntityBuilder::EntityDefinition& def);
	};

	/** To create the movements of the Dragonwind like game. */
	class MovementBuilder : public QGAMES::AdvancedMovementBuilder
	{
		public:
		MovementBuilder (const std::string& fDef)
			: QGAMES::AdvancedMovementBuilder (fDef)
								{ }
	
		protected:
		/** @see parent. */
		virtual QGAMES::Movement* createMovement (const QGAMES::MovementBuilder::MovementDefinition& def);
	};

	/** To create the worlds and scenes of a Dragonwind like game. */
	class WorldBuilder : public QGAMES::PlatformWorldBuilder
	{
		public:
		WorldBuilder (const std::string& eDef, QGAMES::MapBuilder* mB)
			: QGAMES::PlatformWorldBuilder (eDef, mB)
							{ }

		/** @see parent. 
			Probabily a specific implementation will define new Worlds and Scenes and, as a consequence,
			these methods will have to be reimplemented to instantiate them.
			But the very basic version of the builder will always build the very basic version of those. */
		virtual QGAMES::World* createWorldObject (int no, const QGAMES::Scenes& s, 
			const QGAMES::WorldProperties& p);
		virtual QGAMES::Scene* createSceneObject (int ns, const QGAMES::Maps& m, 
			const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL);
		virtual QGAMES::SceneActionBlock* createSceneActionBlockObject (int nAB, 
			const QGAMES::SceneActionBlockProperties& prps);

		protected:
		/** @see parent. */
		virtual void afterCreateElement (QGAMES::Element* elmnt);
	};

	/** A structure used to know things about the tiles.
		It is an extension of the default one defined in the Platform library to include
		the tiles used to identify things like intermediate goals, etc... 
		Additionaly the constructor intialize the default values. */
	struct ToKnowTileFrameInformation : public QGAMES::ToKnowTileFrameInformation
	{
		public:
		// Default values...
		/** Default tiles frames representing things to eat (@see Location.png) */
		static int _defaultTileFramesRepresentingSomethingToEat [];
		/** Default tile frames representing things to catch (see Location.png) */
		static int _defaultTileFramesRepresentingSomethingToCatch [];
		/** Default tile frames representing an initial position for bad guys (@see Location.png) */
		static int _defaultTileFramesRepresentingBadGuysPosition [];
		/** Default tile frames representing the position where doors stand (@see Notification.png) */
		static int _defaultTileFramesRepresentingDoorsPosition [];
		/** Default tile frames rerpesenting connection points between scenes. */
		static int _defaultTileFramesRepresentingConnectionGoals [];
		/** Default tile frames representing intermediate goals along the ninja way (@see Notification.png) */
		static int _defaultTileFramesRepresentingGoalsPosition [];
		/** Default tile frames representing initial positions (@see Location.png) */
		static int _defaultTileFramesRepresentingInitialPositions [];
		/** Default tile frames representing intrecations with action blocks (@see Notification.png). */
		static int _defaultTileFramesRepresentingActionBlocs [];

		ToKnowTileFrameInformation ();

		/** Methods to know whether a tile id is or not includedin one of the lists */
		bool isTileConnectionGoalLocation (int tF) const
							{ return (std::find (_connectionGoalsNotificationTileFrames.begin (), 
								_connectionGoalsNotificationTileFrames.end (), tF) != _connectionGoalsNotificationTileFrames.end ()); }
		bool isTileIntermediateGoalLocation (int tF) const
							{ return (std::find (_intermediateGoalsNotificationTileFrames.begin (), 
								_intermediateGoalsNotificationTileFrames.end (), tF) != _intermediateGoalsNotificationTileFrames.end ()); }
		bool isTileInitialLocation (int tF) const
							{ return (std::find (_initialPositionsTileFrames.begin (), 
								_initialPositionsTileFrames.end (), tF) != _initialPositionsTileFrames.end ()); }
		bool isTileActionBlockLocation (int tF) const
							{ return (std::find (_actionBlocksTileFrames.begin (), 
								_actionBlocksTileFrames.end (), tF) != _actionBlocksTileFrames.end ()); }

		// To translate information witin the tile frame number into application info
		/** To know which is the type of food associated to a specific food position tile frame. 
			It can be overloaded later, but the default implementation returns the tile frame in base 0
			regarding that the default list os food tile frames are sequential.
			The type of food are: 0 (energy 1, in the air), 1 (energy 1, falling),
								  2 (energy 2, in the air), 3 (energy 2, falling),
								  4 (energy 3, in the air), 5 (ebergy 3, falling)....and so for up to 9. 
			In the default implementation the number returned will be considered to get the initial energy level
			and also initial state of the food. */
		virtual int typeOfFoodAssociatedToFoodTileFrame (int tE) const
							{ return (tE - (*_thingsToEatLocationTileFrames.begin ())); }
		virtual int tileFrameAssociatedToTypeOfFood (int tE) const
							{ return (tE + (*_thingsToEatLocationTileFrames.begin ())); }

		/** To know which is the init point associated to a specific connection point tile frame. \n
			It can be overloaded later, but the default implementation returns the tile frame in base 0. */
		virtual int initPointAssociatedToConnectionGoalTileFrame (int cG) const
							{ return (cG - (*_connectionGoalsNotificationTileFrames.begin ())); }
		/** To know which is the init point associated to a specific intermediate init point tile frame.
			It can be overload later, but the default implementation returns the tile frame in base 0
			regarding that the default list intermediate init point tile frames are sequential. */
		virtual int initPointAssociatedToIntermediateGoalTileFrame (int iG) const
							{ return (iG - (*_intermediateGoalsNotificationTileFrames.begin ())); }
		/** To know which is the tile fram associated to a initial point. */
		virtual int tileFrameAssociatedToInitPoint (int iG) const
							{ return (iG + (*_initialPositionsTileFrames.begin ())); }

		/** To know which is the type of gun associated to a specific thing to catch position tile frame. 
			It can be overload later, but the default implementation returns the tile frame.
			The type of guns returned (default) will be: 10 (arrow), 11 (bomb), 16 (star). */
		virtual int typeOfGunAssociatedToThingToCatchTileFrame (int tC) const
							{ return (tC); }
		virtual int tileFrameAssociatedToTypeOfGun (int tC) const
							{ return (tC); }

		std::vector <int> _connectionGoalsNotificationTileFrames;
		std::vector <int> _intermediateGoalsNotificationTileFrames;
		std::vector <int> _initialPositionsTileFrames;
		std::vector <int> _actionBlocksTileFrames;
	};

	/** To create the maps (addon) of a DragonWind like game. */
	class TMXMapBuilder : public QGAMES::PlatformTMXMapBuilder
	{
		public:
		TMXMapBuilder (QGAMES::Sprite2DBuilder* sB)
			: QGAMES::PlatformTMXMapBuilder (sB)
							{ }

		/** @see parent. */
		virtual QGAMES::ToKnowTileFrameInformation* tileFrameInformation () 
							{ return (_tileFrameInformation 
								? _tileFrameInformation : (_tileFrameInformation = new ToKnowTileFrameInformation ())); }
		protected:
		/** @see parent. */
		virtual QGAMES::TileLayer* createTileLayer (int id, const std::string& n, 
			const QGAMES::Tiles& t, QGAMES::Map* m, QGAMES::TileLayer::VisualOrientation o, 
			const QGAMES::LayerProperties& p = QGAMES::LayerProperties ());
		virtual QGAMES::Map* createMapObject (int id, const QGAMES::Layers& l, int w, int h, int d,
			int tW, int tH, int tD, const QGAMES::MapProperties& p = QGAMES::MapProperties ());
		virtual ToKnowTileFrameInformation* createTileFrameInformationObject ()
							{ return (new ToKnowTileFrameInformation ()); }
	};

	/** To create the game state to initialize the game. */
	class ForInitializeDragonWindGameStateAddOn : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForInitializeDragonWindGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __DRAGONWIND_GAMESTATEINITIALIZEGAMETYPE__); }
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
						{ return (t == __DRAGONWIND_GAMESTATEPLAYERNAMETYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create a game state add on to show the name of the world. */
	class ForShowingWorldNameGameStateAddOn : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingWorldNameGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __DRAGONWIND_GAMESTATEWORLDNAMETYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create a game state add on to show the name of the scene. */
	class ForShowingSceneNameGameStateAddOn : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingSceneNameGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __DRAGONWIND_GAMESTATESCENENAMETYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create the special credit title game state.
		Two additional parameters are added to define an object of this type:
		The number of the credit title that is the title of the game and then a dragon has to be shown behind.
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
						{ return (t == __DRAGONWIND_GAMESTATECREDITTITLESTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create the special type of menu needed here.
		It is like the standard but adding a couple of options more to consider additional keys. */
	class ForMenuAddsOn : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForMenuAddsOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __DRAGONWIND_GAMESTATEMENUTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create the playing game state of this type of games. 
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
						{ return (t == __DRAGONWIND_GAMESTATECONTROLPLAYINGTYPE__); }

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
						{ return (t == __DRAGONWIND_GAMESTATEPLAYINGTYPE__); }
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
						{ return (t == __DRAGONWIND_GAESTATEOPTIONSWHENPLAYTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def);
	};

	/** To create the game state talking about the hall of fame in this type of games. 
		The parameters are the same than the ones used for the basic hall of fame game state. */
	class ForShowingHallOfFameGameStateAddOn : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingHallOfFameGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
							{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __DRAGONWIND_GAMESTATEHALLOFFAMETYPE__); }
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
								  t == __DRAGONWIND_GAMESTATECTRLTYPE__); }

		protected:
		/** @see parent. */
		virtual QGAMES::GameState* createControlGameState 
			(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
			 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl);
	};

	/** Just to control how to build the type of step specific of a dragonwin like game. */
	class CharacterControlStepsMonitorBuilder : public QGAMES::CharacterControlStepsMonitorBuilder
	{
		public:
		CharacterControlStepsMonitorBuilder (const std::string& fDef)
			: QGAMES::CharacterControlStepsMonitorBuilder (fDef)
							{ }
		
		/** 
		  *	@see parent.
		  * The structure of the definition in the monitor.xml file is as follows:
		  *	<Step type="50000">
		  *		<Variable id="Type" value="0|1|2|3|4|5"/> <!-- Meaning STAND, JUMP, FALL, MOVE, CLIMB and SLIDE -->
		  *		<Variable id="Direction" value="0|1"/> <!-- Meaning RIGHT or LEFT --> <!-- 0 by default -->
		  *		<Variable id="Impulse" value="1.."/> <!-- The impulse --> <!-- 1 by default -->
		  *		<Variable id="Seconds" value="0.00../> <!-- Meaning the seconds in the state --> <!-- 0 none, -1 infinite -->
		  *		In many states, the state is stable so the time is not important.
		  *		Others finishes automatically because it changes after reaching something.
		  *	</Step>
		  */
		virtual QGAMES::CharacterControlStep* createStep 
			(const QGAMES::CharacterControlStepsMonitorBuilder::StepDefinition& def);
	};
}

#endif
  
// End of the file
/*@}*/