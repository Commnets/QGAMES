/** \ingroup Game */
/** \ingroup Structure */
/** \ingroup Builders */
/*@{*/

/**	
 *	@file	
 *	File: gamestatebuilder.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 30/01/2019 \n
 *	Description: Defines a class to create game states from a file. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_GAMESTATEBUILDER__
#define __QGAMES_GAMESTATEBUILDER__

#include <Common/gamestate.hpp>
#include <tinyxml.h>
#include <string>
#include <vector>
#include <map>

namespace QGAMES
{
	/** 
	  *	This is the base class for a game state builder (Can be extended). \n
	  *	It is mainly made up of adds-on, which are in charge of setting up instances of specific game state types. \n
	  *	The structure defining a game state in the definition file is something variable. \n
	  *	So to read the file can't be done at construction time like happens in any other type of builder. \n
	  *	However, this builder requires to read the definition file after a game state is read. \n
	  * The genetal structure of the definition file is: \n
	  *	<?xml version="1.0"?> \n
	  *	<GameStates> \n
	  *		<Include file="CCC"/> \n
	  *		<GameState id="X" type="Y" name="ZZ" main="NO" (optional)> \n
	  *			<Attributes> \n
	  *				<Attribute id="XXX" value="YYY"/> \n
	  *				... (More attributes definition) \n
	  *			</Attributes> \n
	  *			<Nested> \n
	  *				<GameState id="X" type="Y" name="ZZ"> \n
	  *				... (Same blocks than any other game state definition) \n
	  *				</GameState> \n
	  *			</Nested> \n
	  *		</GameState> \n
	  *		... \n
	  *	<GameStates> \n
	  * \n
	  *	IMPORTANT: Game states set up don't belong to the builder. \n
	  *	The class keeps pointers to any game state instantiated 
	  * to avoid creation process if the same state is required twice or more. \n
	  *	Be careful, because if the pointer is destroyed the references will still be kept here. 
	  */
	class GameStateBuilder
	{
		public:
		/** A pair to have together id, the name and the pointer to the game state. */
		struct IdNameGameState
		{
			IdNameGameState ()
				: _id (-1), _name (std::string (__NULL_STRING__)), _main (true), _gameState (NULL)
							{ }
			IdNameGameState (int id, const std::string& n, GameState* gS, bool m = true)
				: _id (id), _name (n), _gameState (gS), _main (m)
							{ }

			int _id; 
			std::string _name;
			bool _main;
			GameState* _gameState;
		};

		// To simplify the access to id's of game states...
		typedef std::map <int, IdNameGameState> IdNameGameStates;

		/** The general definition of a game state in the file. 
			This class can be overloaded to support specific attributes for specific game states. */
		struct GameStateDefinition
		{
			GameStateDefinition ()
				: _id (-1), // Means bad definition 
				  _main (true), // Means that the game is under the control of the 
				  _name (std::string (__NULL_STRING__)), // NULL_STRING is not well defined or only nested.
				  _type (-1), // Means bad definition
				  _attributes (),
				  _nested (NULL)
							{ }

			virtual ~GameStateDefinition ()
							{ delete (_nested); }

			/** To get the GameStateDefinition valid for the id. 
				Look through the nested definitions even. If not null is returned. */
			GameStateDefinition* definitionFor (int id)
							{ return ((_id == id) 
								? this : ((_nested != NULL) ? _nested -> definitionFor (id) : NULL)); }
			/** To get the GameStateDefinition valid for the name. /
				Look through the nested definitions even. If not null is returned. */
			GameStateDefinition* definitionFor (const std::string& n)
							{ return ((_name == n) 
								? this : ((_nested != NULL) ? _nested -> definitionFor (n) : NULL)); }

			int _id;
			bool _main; 
			std::string _name; // Name of the game state...
			int _type;
			std::map <std::string, std::string> _attributes; // In general form...
			GameStateDefinition* _nested; // NULL when nothing nested...
		};

		/** An add-on is an interface used by the GameStateBuilder for one thing only: \n
		  *	To read the blocks and attributes of a specific game state (defined by its type). \n
		  *	The game state builder owns their add-on. It means that
		  *	when it is destroyed the adds-on will be also destroyed with it. 
		  */
		class AddsOn
		{
			public:
			AddsOn (GameStateBuilder* b)
				: _builder (b)
							{ assert (_builder); } // Can't be null...
			virtual ~AddsOn () // Just in case. By default nothing is needed...
							{ }

			/** To get the builder. */
			GameStateBuilder* builder ()
							{ return (_builder); }
			const GameStateBuilder* builder () const
							{ return (_builder); }

			// Methods related with the creation of a game state...
			/** Returns true when the add-on is able to create a concrete type of game state. */
			virtual bool canCreateGameState (int t) const = 0;
			/** Creates a game state of a type from the definition structure. \n
				If the adds-on is not able to create a type of game state (@see method above), it should crash. */
			virtual GameState* createGameState (const GameStateDefinition* def) = 0;

			/** Invoked before and after creating the game state. By default nothing is done. */
			virtual void beforeCreatingGameState (const GameStateDefinition* def)
							{ }
			virtual void afterCreatingGameState (const GameStateDefinition* def)
							{ }

			/** To create the "internal definition structure" valid for this add-on. \n
				It should be defined for each, but by default creates the default one. */
			virtual GameStateDefinition* createEmptyDefinition () 
							{ return (new GameStateDefinition ()); }

			// Implementation
			// Accessible only from GameStateBuilder instances
			/** To read and add the specific attributes. 
				By default there is no more attributes to read. */
			virtual void readSpecificAttributesOver (GameStateDefinition* def, TiXmlElement* e)
							{ }
			/** To read an specific block. 
				Bu default there is no other blocks to read than the default ones. */
			virtual void readSpecificBlockOver (const std::string& blkName, 
				GameStateDefinition* def, TiXmlElement* e)
							{ }

			protected:
			// Implementation
			/** A reference to the builder. */
			GameStateBuilder* _builder; // Can't be null...
		};

		// A typedef to simplify the access to the addsOn elements. */
		typedef std::list <AddsOn*> AddsOnList;

		/** The add-on used to create the GameState used to finalize the game. */
		class ForExitAddsOn : public AddsOn
		{
			public:
			ForExitAddsOn (GameStateBuilder* b)
				: AddsOn (b)
							{ }

			/** @see parent. */
			virtual bool canCreateGameState (int t) const
							{ return (t == __QGAMES_GAMESTATETYPEEXITTYPE__); }
			virtual GameState* createGameState (const GameStateDefinition* def)
							{ return (new QGAMES::ExitGameGameState ()); }
		};

		/** 
		  *	Add-on to create the Game State definition used to control game states. \n
		  *	The control game state manages a set of other game states. \n
		  *	The structure of this game state definition should be as follow: \n
		  *	<?xml version="1.0"?> \n
		  *	<GameStates> \n
		  *		<Include file="CCC"/> \n
		  *		<GameState id="X" type="Y" name="ZZ"> \n
		  *			<FlowConditions> \n
		  *				<FlowCondition on="XX" when="YYY" to="ZZ"/> \n
		  *				... (More flow conditions) \n
		  *			</FlowConditions> \n
		  *			<Attributes> \n
		  *				<Attribute id="XXX" value="YYY"/> \n
		  *				... (More attributes definition) \n
		  *			</Attributes> \n
		  *			<Nested> \n
		  *				<GameState id="X" type="Y" name="ZZ"> \n
		  *				... (Same blocks than any other game state definition) \n
		  *				</GameState> \n
		  *			</Nested> \n
		  *			... \n
		  *		</GameState> \n
		  *		... \n
		  *	<GameStates>
		  */
		struct ControlGameStateDefinition : public GameStateDefinition
		{
			struct FlowCondition
			{
				FlowCondition (int cdt, int st)
					: _conditionId (cdt), _state (st)
							{ }

				int _conditionId; // if...
				int _state; // ...then this is the state to move into...
			};

			typedef std::map <int, FlowCondition> FlowConditionList; // if condition...to state...
			typedef std::map <int, FlowConditionList> FlowList; // on state...conditions...

			ControlGameStateDefinition ()
				: GameStateDefinition (),
				  _flow ()
							{ }

			// To add a new flow condition...only one of the type cdt over the same "on" state...
			void addFlowCondition (int on, int cdt, int to);	

			FlowList _flow;
		};

		/** An add-on to create Game State Control instances. \n 
			Take care. When a control game state is created, the ones under control 
			have to be have been created previously. */
		class ForControlAddsOn : public AddsOn
		{
			public:
			ForControlAddsOn (GameStateBuilder* b)
				: AddsOn (b)
							{ }

			/** @see parent. */
			virtual bool canCreateGameState (int t) const
							{ return (t == __QGAMES_GAMESTATETYPECONTROLTYPE__); }
			virtual GameState* createGameState (const GameStateDefinition* def);

			/** To create the "definition structure" for this add-on (@see above). 
				It should be defined for each, but by default creates the default one. */
			virtual GameStateDefinition* createEmptyDefinition () 
							{ return (new ControlGameStateDefinition ()); }

			protected:
			/** @see parent. */
			virtual void readSpecificBlockOver (const std::string& blkName, 
				GameStateDefinition* def, TiXmlElement* e);

			/** createGameState method invokes this one .*/
			virtual GameState* createControlGameState (const ControlGameStateDefinition* def,
				const GameStates& sts, const GameStateControl::FlowMap& fl);
		};

		/** An instance of the two adds-on defined above are added to the builder. */
		GameStateBuilder (const std::string& gDef);
		virtual ~GameStateBuilder (); // To delete the adds-on and definitions

		/** IMPORTANT: The builder is not accountable for any game state loaded through it. 
			When game states are loaded, then they must be registered in the game 
			who will delete them when finish. \n
			So initially no destructor for this action is needed. */

		/** To get the number of definitions included in the definition file. */
		int numberGameStates () const
							{ return ((int) _definitions.size ()); }

		/** To get the id of every resource to be loaded. */
		std::vector <int> gameStatesId () const;

		// To manage addsOn
		/** To get the adds-on list. */
		const AddsOnList& addsOn () const
							{ return (_addsOn); }
		/** To add a addn-on */
		void addAddsOn (AddsOn *aO);

		/** First time a game state is requested, the definition file will be read also. \n
			So, at this point, all adds-on should already be created, otherwise the system could crash. \n
			If a game state is not in memory when it is requested, 
			it will be created using the definition structure read. \n */
		GameState* gameState (int nGS);
		/** Similar function that before, but looking for the name. */
		GameState* gameState (const std::string& nGS);

		/** To preload all game states defined as "main" (@see class header). */
		void preLoad ();

		/** To get a list of all game states created by the builder. */
		const IdNameGameStates& gameStatesCreated () const
							{ return (_gameStates); }
		/** Returning a structure including the id of the game state
			its name, and a reference to the object created. */
		const IdNameGameState& gameStateIdName (int nGS) // Can't be const because the game state can be created on the fly...
							{ gameState (nGS); return (_gameStates [nGS]); }
		/** To get the main game states created only. */
		IdNameGameStates mainGameStatesIdName (); // Not const because the game states can be created on the fly...

		private:
		void readFile (const std::string& fDef);
		GameStateDefinition* readGameStateDefinition (TiXmlElement* e);
		std::map <std::string, std::string> readAttributesGroup (TiXmlElement* e);

		protected:
		// A typedef to simplify the access to a list of definitions...(notice that keep pointers)
		typedef std::map <int, GameStateDefinition*> GameStateDefinitions;

		/** A list of the definitions read from the definition file. */
		GameStateDefinitions _definitions;
		/** Has the file already been read. */
		bool _fileRead;
		/** The game states already instantiated. Remember they don't belong to the builder. */
		IdNameGameStates _gameStates;
		/** A list of the adds on belonging to the builder.
			They will also be destroyed as the builder is deleted. */
		AddsOnList _addsOn;

		std::string _definitionFile;
		std::string _basePath;
	};
}

#endif
  
// End of the file
/*@}*/
