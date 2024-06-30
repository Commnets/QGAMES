/** \ingroup PACMAN */
/*@{*/

/**	
 *	@file	
 *	File: pcbuilders.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 22/08/2021 \n
 *	Description: The constructors that any PACMAN like game needs. \n
 *	Versions: 1.0 Initial
 */

#ifndef __PACMAN_BUILDERS__
#define __PACMAN_BUILDERS__

#include <PacManLike/pcdefs.hpp>

#include <advancedArcade/adinclude.hpp>

namespace PACMAN
{
	/** To create the letters of this game. */
	class TextBuilder final : public QGAMES::AdvancedTextBuilder
	{
		public:
		TextBuilder ();

		virtual QGAMES::ScoreObjectText* createText (int fnt, const std::string& txt) override;
	};

	/** To build the GUI systems and widgets of this game. */
	class GUISystemBuilder final : public QGAMES::StandardGUISystemBuilder
	{
		public:
		GUISystemBuilder (const std::string& fDef, QGAMES::FormBuilder* fB)
			:  QGAMES::StandardGUISystemBuilder (fDef, fB)
							{ }

		private:
		virtual QGAMES::GUISystem* createSystem (const QGAMES::GUISystemBuilder::GUISystemDefinition& def, 
			QGAMES::CompositeWidget* mW) override;
		virtual QGAMES::Widget* createWidget (const QGAMES::GUISystemBuilder::WidgetDefinition& def) override;
	};

	/** To create the game state to initialize the game. */
	class ForInitializePACMANGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForInitializePACMANGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMAN_GAMESTATEINITIALIZEGAMETYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create a game state adds on that shows a set of monsters moving in the right side of the screen moving randomly. */
	class ForShowingMonsterMovingGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingMonsterMovingGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMAN_GAMESTATEMOVINGMONSTERSTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create a game state adds on to show the name of the player. */
	class ForShowingPlayerNameGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingPlayerNameGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMAN_GAMESTATEPLAYERNAMETYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create a game state adds on to show the name of the current lvel. */
	class ForShowingLevelGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingLevelGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMAN_GAMESTATELEVELTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create the control playing game state of this type of games. */
	class ForPlayingControlGameStateAddOn final : public QGAMES::GameStateBuilder::ForControlAddsOn
	{
		public:
		ForPlayingControlGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::ForControlAddsOn (b)
							{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMAN_GAMESTATECONTROLPLAYINGTYPE__); }

		private:
		virtual QGAMES::GameState* createControlGameState 
			(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
			 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl) override;
	};

	/** To create the game state to play. */
	class ForPlayingGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForPlayingGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
							{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMAN_GAMESTATEPLAYINGTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create the game stat to stop monsters moving. */
	class ForStopPacmanElemntsMovingGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForStopPacmanElemntsMovingGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
							{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMAN_GAMESTATESTOPPINGELEMENTSTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create the game state for the map to blink. */
	class ForBlinkMazePlayingGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForBlinkMazePlayingGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
							{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMAN_GAMESTATEBLINKMAZEMAPTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create the game state with the options when playing. */
	class ForOptionsWhenPlayingGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForOptionsWhenPlayingGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
							{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMAN_GAESTATEOPTIONSWHENPLAYTYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create the game state talking about the hall of fame in this type of games. 
		The parameters are the same than the ones used for the basic hall of fame game state defined in AdvacedArcade
		but what changes is the ways the information is understood. */
	class ForShowingHallOfFameGameStateAddOn final : public QGAMES::GameStateBuilder::AddsOn
	{
		public:
		ForShowingHallOfFameGameStateAddOn (QGAMES::GameStateBuilder* b)
			: QGAMES::GameStateBuilder::AddsOn (b)
							{ }

		virtual bool canCreateGameState (int t) const override
						{ return (t == __PACMAN_GAMESTATEHALLOFFAMETYPE__); }
		virtual QGAMES::GameState* createGameState (const QGAMES::GameStateBuilder::GameStateDefinition* def) override;
	};

	/** To create the special object to control the flow of the game. */
	class ForGameStateControlAddon final : public QGAMES::ForAdvancedGameStateControlAddon
	{
		public:
		ForGameStateControlAddon (QGAMES::GameStateBuilder* b)
			: QGAMES::ForAdvancedGameStateControlAddon (b)
							{ }

		virtual bool canCreateGameState (int t) const override
						{ return (QGAMES::ForAdvancedGameStateControlAddon::canCreateGameState (t) ||
								  t == __PACMAN_GAMESTATECTRLTYPE__); }

		private:
		virtual QGAMES::GameState* createControlGameState 
			(const QGAMES::GameStateBuilder::ControlGameStateDefinition* def,
			 const QGAMES::GameStates& sts, const QGAMES::GameStateControl::FlowMap& fl) override;
	};

	/** To create the movements of the game. \n
		It can be extended to include new ones or to define movements for the new entities. */
	class MovementBuilder : public QGAMES::AdvancedMovementBuilder
	{
		public:
		MovementBuilder () = delete;

		MovementBuilder (const std::string& fDef)
			: QGAMES::AdvancedMovementBuilder (fDef) 
								{ }

		MovementBuilder (const MovementBuilder&) = delete;

		MovementBuilder& operator = (const MovementBuilder&) = delete;

		protected:
		virtual QGAMES::Movement* createMovement 
			(const QGAMES::MovementBuilder::MovementDefinition& def) override;
	};

	/** To create specific entities of the game. \n
		It can be extendd to include our own!. */
	class EntityBuilder : public QGAMES::AdvancedEntityBuilder
	{
		public:
		EntityBuilder () = delete;

		EntityBuilder (const std::string& eDef, QGAMES::FormBuilder* fB, QGAMES::MovementBuilder* mB)
			: QGAMES::AdvancedEntityBuilder (eDef, fB, mB)
								{ }

		EntityBuilder (const EntityBuilder&) = delete;

		EntityBuilder& operator = (const EntityBuilder&) = delete;

		protected:
		virtual QGAMES::Entity* createEntity (const QGAMES::EntityBuilder::EntityDefinition& def) override;
	};

	/** The extension to create maps. */
	class TMXMapBuilder final : public QGAMES::TMXMapBuilderAddsOn
	{
		public:
		TMXMapBuilder (QGAMES::Sprite2DBuilder* sB);

		// Managing the tiles of the path
		/** To know the basic number of frame used to represent things in the path. */
		int frameForEmptyPath () const
							{ return (_TILESPATH [0]); }
		/** To calculate the bright frame equivalent to another that is not. */
		int brightFrameFor (int nF) const;
		/** The opposite of the precioud method. */
		int darkFrameFor (int nF) const;

		// Managing the tiles related to location
		/** To know the tile frames to locate the pacman. */
		const std::vector <int>& pacmanHomeFrames () const
							{ return (_TILESHOMEPACMAN); }
		/** To know the tile frames to locate monsters in the map. */
		const std::vector <int>& monsterHomeFrames () const
							{ return (_TILESHOMEMONSTERS); }
		/** To know the tile frames to locate the running away monster's positions. */
		const std::vector <int>& monsterRunAwayFrames () const
							{ return (_TILESRUNAWAYMONSTERS); }
		/** To know the tile frame to locate the fruit. */
		const std::vector <int>& thingPositionFrames () const
							{ return (_TILETHINGS); }

		/** To know the tile frame defining the exit of monster's home. */
		int monsterExitingHomeFrame () const
							{ return (_TILEEXITMONSTERSHOME); }
		/** To know the tile frame defining whre a monster has to come back after dieing. */
		int monsterToComeBackFrame () const
							{ return (_TILERETURNMONSTERAFTERDIE); }
		/** To know the file describing connections points, usually the tunnels. */
		const std::vector <int>& connectionFrames () const
							{ return (_TILESCONNECTION); }

		/** To know the frames definning zones. 
			Every tile frame will indicate a number of zone. */
		const std::vector <int>& tilesZones () const
							{ return (_TILESZONES); }

		// Managing the tiles of maze description (directions). */ 
		/** To know the tiles describing a direction a the direction itself. 
			4 bools: LEFT, RIGHT, UP, DOWN. */
		const std::map <int, std::vector <bool>>& directionFrames () const
							{ return (_TILESDIRECTIONSMAZE); }

		protected:
		virtual QGAMES::Tile* createTile (int id, QGAMES::Form* form, int nf, 
			const QGAMES::TileProperties& p = QGAMES::TileProperties ()) override;
		virtual QGAMES::TileLayer* createTileLayer (int id, const std::string& n, 
			const QGAMES::Tiles& t, QGAMES::Map* m, QGAMES::TileLayer::VisualOrientation o, 
			const QGAMES::LayerProperties& p = QGAMES::LayerProperties ()) override;
		virtual QGAMES::ObjectLayer* createObjectLayer (int id, const std::string& n, const QGAMES::Objects&, 
			QGAMES::Map* m, const QGAMES::LayerProperties& oP = QGAMES::LayerProperties ()) override;
		virtual QGAMES::ImageLayer* createImageLayer (int id, const std::string& n, QGAMES::Image* i, 
			QGAMES::Map* m, const QGAMES::LayerProperties& oP = QGAMES::LayerProperties ()) override;
		virtual QGAMES::Map* createMapObject (int id, const QGAMES::Layers& l, int w, int h, int d, int tW, int tH, int tD,
			const QGAMES::MapProperties& p = QGAMES::MapProperties ()) override;

		protected:
		bool isInType (int nF, const std::vector <int>& t) const
							{ return (std::find (t.begin (), t.end (), nF) != t.end ()); };
		int positionInType (int nF, const std::vector <int>& t) const;

		protected:
		// Implementation
		// Tiles located in the "ArtistsLocation" layer
		/** The tiles used to describe the path. */
		const std::vector <int> _TILESLIMITDARK, _TILESLIMITBRIGHT, 
			_TILESPOWERBALL, _TILESNORMALBALL, _TILESPATH, _TILESPATHLIMITRIGHT, _TILESPATHLIMITLEFT;
		/** The tiles used to define the location of artists. */
		const std::vector <int> _TILESHOMEPACMAN, _TILESHOMEMONSTERS, _TILESRUNAWAYMONSTERS;
		/** The tile to indicate the position where the fruit appears. */
		const std::vector <int> _TILETHINGS;

		// Tiles located in the "MazeLocations" layer
		/** The tile used to define the home exiting position for monsters */
		const int _TILEEXITMONSTERSHOME;
		/** The tile usd to define the place where monsters have to return after dieing. */
		const int _TILERETURNMONSTERAFTERDIE;
		/** Tiles indicating connection points. */
		const std::vector <int> _TILESCONNECTION;

		// Tiles located in the "MazeZones" layer
		/** Tile frame to indicate different zones. */
		const std::vector <int> _TILESZONES; 

		// Tiles located in the "Directions" layer
		/** The equivalent between tiles to mark direection and the direction itself.
			Th order of the second veector is LEFT/RIGHT/UP/DOWN. */
		const std::map <int, std::vector <bool>> _TILESDIRECTIONSMAZE;
	};

	/** The extension to create worlds. 
		It is not a final class, because new worlds can be added in future extension of the game. */
	class WorldBuilder : public QGAMES::WorldBuilder
	{
		public: 
		WorldBuilder () = delete;
			
		WorldBuilder (const std::string& eDef, QGAMES::MapBuilder* mB)
			: QGAMES::WorldBuilder (eDef, mB)
							{ }

		WorldBuilder (const WorldBuilder&) = delete;

		WorldBuilder& operator = (const WorldBuilder&) = delete;

		protected:
		virtual QGAMES::World* createWorldObject (int no, const QGAMES::Scenes& s, 
			const QGAMES::WorldProperties& p) override;
		virtual QGAMES::Scene* createSceneObject (int ns, const QGAMES::Maps& m, 
			const QGAMES::Scene::Connections& cn, 
			const QGAMES::SceneProperties& p, const QGAMES::EntitiesPerLayer& ePL) override;
		virtual QGAMES::SceneActionBlock* createSceneActionBlockObject (int nAB, 
			const QGAMES::SceneActionBlockProperties& prps) override;
	};
}

#endif
  
// End of the file
/*@}*/