/**\ingroup AdArcadeGame */
/**\ingroup Structure */
/**\ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: adbuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/04/2018 \n
 *	Description: Defines all builders used in the basic advanced game form. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_ADBUILDERS__
#define __QGAMES_ADBUILDERS__

#include <AdvancedArcade/admovements.hpp>
#include <AdvancedArcade/addefinitions.hpp>
#include <Arcade/arcadeinclude.hpp>

namespace QGAMES
{
	/** To create a text builder able to use the default fonts defined at letters file. */
	class AdvancedTextBuilder : public TextBuilder
	{
		public:
		AdvancedTextBuilder ();

		/** @see parent. */
		virtual int nextSizeTo (int fnt);
		virtual int previousSizeTo (int fnt);
		virtual int inColor (int fnt, const Color& cl);
		virtual ScoreObjectText* createText (int fnt, const std::string& txt);

		protected:
		struct FontData
		{
			FontData () // When not valid, id becomes -1
				: _id (-1), _type (-1), _size (0), _color (__QGAMES_WHITECOLOR__)
							{ }
			FontData (int fId, int fT, int s, const Color& c)
				: _id (fId), _type (fT), _size (s), _color (c)
							{ }

			int _id;
			int _type;
			int _size;
			Color _color;
		};

		typedef std::list <FontData> FontDataList;
		typedef std::map <int, FontDataList> FontDataPerSize;
		typedef std::map <int, FontDataPerSize> FontDataMap; // Per type...

		/** To get the "distance" between two colors. */
		bdata distanceBetweenColors (const Color& c1, const Color& c2);

		/** To find the information of a font.
			If it doesn't exist a not valid font is returned. */
		FontData fontFor (int id);

		/** The list of the fonts managed by this builder.
			They are defined when the object is instantiated. */
		FontDataMap _fontData;
	};

	/** To create the standard entities defined for any advanced arcade game. 
		The only one today is the entity drawn when the game is paused. */
	class AdvancedEntityBuilder : public EntityBuilder
	{
		public:
		AdvancedEntityBuilder (const std::string& fDef, FormBuilder* fB, MovementBuilder* mB)
			: EntityBuilder (fDef, fB, mB)
								{ }
	
		protected:
		/** @see parent. */
		Entity* createEntity (const EntityBuilder::EntityDefinition& def);
	};

	/** To create a builder for curve movements. 
		The builder doesn't keep a copy of the elements created. */
	class CurveTemplateBuilder
	{
		public:
		virtual ~CurveTemplateBuilder () 
							{ }

		/** This method can be used carefully. 
			It creates a very default curve of the type received as parameter. */
		virtual FollowingACurveMovement::CurveTemplate* createBasicDefaultCurve (int t);

		/** 
		  *	To create the curves. \n
		  *	Default implementation for a copuple of them is provided. \n
		  *	@param	id		: type of the curve. \n
		  * @param	prms	: The parameters potentially required to create the curves. \n
		  *					  They can come from the movement file parameters.
		  */
		virtual FollowingACurveMovement::CurveTemplate* createCurve (int t, const std::map <int, double>& prms);
	};

	/** To create the standard movements defined for any advanced arcade game. \n
		The only one today is the "no movement" movement. \n
		The advanced movement builder includes a curve movement builder as well. */
	class AdvancedMovementBuilder : public MovementBuilder
	{
		public:
		AdvancedMovementBuilder (const std::string& fDef, CurveTemplateBuilder* cB = new CurveTemplateBuilder);
		~AdvancedMovementBuilder ()
							{ delete (_curveBuilder); _curveBuilder = NULL; }

		CurveTemplateBuilder* curveBuilder ()
							{ return (_curveBuilder); }
		const CurveTemplateBuilder* curveBuilder () const
							{ return (_curveBuilder); }
	
		protected:
		/** @see parent. */
		virtual Movement* createMovement (const MovementBuilder::MovementDefinition& def);

		protected:
		CurveTemplateBuilder* _curveBuilder;
	};

	/** This adds on is to create the background maps type. 
		This add on has to be added to any final map builder instantiated by the game. 
		Any background type map hasy to be defined as type __QGAMES_BACKGROUNDOBJECTMAP__
		in the definition file. */
	class BackgroundObjectMapBuilder : public QGAMES::ObjectMapBuilderAddsOn
	{
		public:
		BackgroundObjectMapBuilder (QGAMES::ObjectBuilder* oB)
			: QGAMES::ObjectMapBuilderAddsOn (oB)
								{ }

		virtual bool canCreateMap (int t) const
							{ return (t == __QGAMES_BACKGROUNDOBJECTMAP__); }

		protected:
		/** @see parent. */
		virtual QGAMES::Map* createMap (int id, int w, int h, int d, const QGAMES::Layers& l, 
			const QGAMES::MapProperties& pties);
		virtual QGAMES::Layer* createLayer (int id, const std::string& name, 
			QGAMES::Objects& objs, QGAMES::Map* m, const QGAMES::LayerProperties& lP);
	};

	/** To create the special widgets needed in any standard advanced arcade game. */
	class AdvancedGUISystemBuilder : public StandardGUISystemBuilder
	{
		public:
		AdvancedGUISystemBuilder (const std::string& fDef, QGAMES::FormBuilder* fB)
			:  StandardGUISystemBuilder (fDef, fB)
							{ }

		protected:
		/** @see parent. */
		virtual Widget* createWidget (const QGAMES::GUISystemBuilder::WidgetDefinition& def);
	};

	/** Add on to create any of these advanced game states, except CreditTitles one.
		Only the ones that can be created alone!! */
	class ForAdvancedGameStatesAddon : public GameStateBuilder::AddsOn
	{
		public:
		ForAdvancedGameStatesAddon (GameStateBuilder* b)
			: GameStateBuilder::AddsOn (b)
							{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const;
		virtual GameState* createGameState (const GameStateBuilder::GameStateDefinition* def);
	};

	/** Add on to create Credit Title Game State. */
	class ForCreditTitlesGameStateAddOn : public GameStateBuilder::AddsOn
	{
		public:
		struct GameStateCreditTitlesDefinition : public GameStateBuilder::GameStateDefinition
		{
			GameStateCreditTitlesDefinition ()
				: GameStateBuilder::GameStateDefinition (),
				  _titles ()
							{ }

			/** To keep the titles. */
			std::vector <std::string> _titles;
		};

		public:
		ForCreditTitlesGameStateAddOn (GameStateBuilder* b)
			: GameStateBuilder::AddsOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __QGAMES_GAMESTATECREDITTITLESTYPE__); }
		virtual GameState* createGameState (const GameStateBuilder::GameStateDefinition* def);

		/** @see parent. */
		virtual GameStateBuilder::GameStateDefinition* createEmptyDefinition () 
						{ return (new GameStateCreditTitlesDefinition ()); }

		protected:
		/** @see parent. */
		virtual void readSpecificBlockOver (const std::string& blkName, 
			GameStateBuilder::GameStateDefinition* def, TiXmlElement* e);
	};

	/** Add on to create Showing Instructions Game State. */
	class ForShowingInstructionsGameStateAddOn : public GameStateBuilder::AddsOn
	{
		public:
		struct GameStateInstructionDefinition
		{
			GameStateInstructionDefinition ()
				: _line (std::string (__NULL_STRING__)),
				  _type (0),
				  _font (__QGAMES_RAVIE24ORANGELETTERS__), // Ravie 24 by default...
				  _position (0), // Between 0 and 1
				  _reference (0) // In the left... (or 1 or 2)
							{ }

			std::string _line;
			int _type;
			int _font;
			bdata _position;
			int _reference;
		};

		typedef std::vector <GameStateInstructionDefinition> GameStateInstructionDefinitions;

		struct GameStateInstructionsDefinition : public GameStateBuilder::GameStateDefinition
		{
			GameStateInstructionsDefinition ()
				: GameStateBuilder::GameStateDefinition (),
				  _instructions () // With no lines...
							{ }

			/** To keep the titles. */
			GameStateInstructionDefinitions _instructions;
		};

		public:
		ForShowingInstructionsGameStateAddOn (GameStateBuilder* b)
			: GameStateBuilder::AddsOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __QGAMES_GAMESTATESHOWINSTTYPE__); }
		virtual GameState* createGameState (const GameStateBuilder::GameStateDefinition* def);

		/** @see parent. */
		virtual GameStateBuilder::GameStateDefinition* createEmptyDefinition () 
						{ return (new GameStateInstructionsDefinition ()); }

		protected:
		/** @see parent. */
		virtual void readSpecificBlockOver (const std::string& blkName, 
			GameStateBuilder::GameStateDefinition* def, TiXmlElement* e);
	};

	/** Add on to create roll states game states. */
	class ForRollingStatesGameStateAddOn : public GameStateBuilder::AddsOn
	{
		public:
		struct GameStateRollingStatesDefinition : public GameStateBuilder::GameStateDefinition
		{
			GameStateRollingStatesDefinition ()
				: GameStateBuilder::GameStateDefinition (),
				  _separations ()
							{ }

			/** To keep the separations. */
			typedef std::map <std::string, std::string> SeparationAttrs;
			std::vector <SeparationAttrs> _separations;
		};

		public:
		ForRollingStatesGameStateAddOn (GameStateBuilder* b)
			: GameStateBuilder::AddsOn (b)
						{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (t == __QGAMES_GAMESTATEROLLSTATESTYPE__); }
		virtual GameState* createGameState (const GameStateBuilder::GameStateDefinition* def);

		/** @see parent. */
		virtual GameStateBuilder::GameStateDefinition* createEmptyDefinition () 
						{ return (new GameStateRollingStatesDefinition ()); }

		protected:
		/** @see parent. */
		virtual void readSpecificBlockOver (const std::string& blkName, 
			GameStateBuilder::GameStateDefinition* def, TiXmlElement* e);
	};

	/**
	 * Add on to create the advanced game state control.
	 * There is one mandatory additional parameter in the game definition file for this state.
	 * It is to point what is the pause game state used:
	 * ...
	 * <Attributes>
	 *		<Attribute id="PAUSEGAMESTATE" value="YYY"/>
	 *		... (More attributes definition)
	 * </Attributes>
	 * ...
	 */
	class ForAdvancedGameStateControlAddon : public GameStateBuilder::ForControlAddsOn
	{
		public:
		ForAdvancedGameStateControlAddon (GameStateBuilder* b)
			: GameStateBuilder::ForControlAddsOn (b)
							{ }

		/** @see parent. */
		virtual bool canCreateGameState (int t) const
						{ return (GameStateBuilder::ForControlAddsOn::canCreateGameState (t) ||
								  t == __QGAMES_GAMESTATEADVANCEDCTRL__); }

		protected:
		/** @see parent. */
		virtual GameState* createControlGameState (const GameStateBuilder::ControlGameStateDefinition* def,
			const GameStates& sts, const GameStateControl::FlowMap& fl);
	};
}

#endif
  
// End of the file
/*@}*/