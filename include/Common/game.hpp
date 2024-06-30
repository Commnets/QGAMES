/** \ingroup Game */
/** \ingroup Structure */
/*@{*/

/**	
 *	@file	
 *	File: game.hpp \n
 *	Framework: Commty Game Library (CGL) \n
 *	Author: Ignacio Cea Fornies (Community Networks) \n
 *	Creation Date: 01/12/2014 \n
 *	Description: Defines the main class for any game. \n
 *	Versions: 1.0 Initial
 */

#ifndef __QGAMES_GAME__
#define __QGAMES_GAME__

#include <Common/notifierobserver.hpp>
#include <Common/gameimplementation.hpp>
#include <Common/soundbuilder.hpp>
#include <Common/entitybuilder.hpp>
#include <Common/formbuilder.hpp>
#include <Common/maskbuilder.hpp>
#include <Common/objectbuilder.hpp>
#include <Common/movementbuilder.hpp>
#include <Common/resourcesgrouploader.hpp>
#include <Common/characterstepsbuilder.hpp>
#include <Common/gamestate.hpp>
#include <Common/gamestatebuilder.hpp>
#include <Common/textbuilder.hpp>
#include <Common/timer.hpp>
#include <Common/screen.hpp>
#include <Common/inputhandler.hpp>
#include <Common/entity.hpp>
#include <Common/widgetbuilder.hpp>
#include <Common/loopcounter.hpp>
#include <Common/comms.hpp>
#include <Common/commsbuilder.hpp>
#include <Common/uniqueidgenerator.hpp>
#include <Common/conffile.hpp>
#include <map>
#include <string>

namespace QGAMES
{
	class Form;
	class Object;
	class Entity;
	class Movement;
	class Sound;
	class Timer;

	/** This class is to define potentail add ons to a game. \n
		The adds on could be useful to add a type of functionlity over an standard type. \n
		e.g. Imagine there is an arcade game that shows and manage a board, then it could be defined
		as Arcade but with an add on to manage the board. */
	class GameTypeAddOn
	{
		public:
		GameTypeAddOn (const std::string& n, Game* g)
			: _name (n),
			  _game (g)
							{ assert (g); } // Can't be null...
		virtual ~GameTypeAddOn ()
							{ } 

		const std::string& name () const
							{ return (_name); }
		const Game* game () const
							{ return (_game); }
		Game* game ()
							{ return (_game); }

		/** These methods has to be defined per add on. By default they don't do anything. */
		virtual void initialize () = 0;
		virtual void inEveryLoop () = 0;
		virtual void updatePositions () = 0;
		virtual void drawOn (Screen* s, const Position& p = Position::_noPoint) = 0;
		virtual void finalize () = 0;

		protected:
		std::string _name;
		Game* _game; // The owner...
	};

	// To simplify how the list of adds on are managed...
	typedef std::map <std::string, GameTypeAddOn*> GameTypeAddsOnMap;

	/** 
	 *	Game class is the fundamental structure of any game. \n
	 *	This a facade object to access all important things in a game. \n
	 *	A game has all the necessary builders to create instances of sounds, entities, forms,
	 *	and any other element being part of a game. \n
	 *  \n
	 *	Any game follows a life-cycle when executed (using exec method). The game is first 
	 *	of all initialized, then pass through the methods every loop, handleEvents, updatePositions 
	 *	inside a no ending loop. \n
	 *	When the loops is broken the method finalize and the game finishes. \n
	 *  \n
	 *	A game can be in different states: Typically a game starts with a menu to select
	 *	or change its basic options (keys used to play,...), then it moves to the history itself,
	 *	and finalizes with an screen showing the score. \n
	 *	It there is an score set up, then the same methods are executed per state. \n
	 *  \n
	 *	A game uses always an implementation usually determined by the initialization of
	 *	the graphic card,...  A game can managed several "screens". \n
	 *	At the end of any loop the draw method is invoked per each screen defined.
	 *	This activity can be done "only" a couple of times per cycle. At construction time
	 *	the game receives the number of frames per second, what means the number of
	 *	times the draw method will be executed. updatePositions method is executed the same number
	 *	of times per second than the draw method. This is not the case of the inEveryLoop method
	 *	that is executed one per loop always. \n
	 *  \n
	 *	The main loop checks the variable _running per loop. \n
	 *	When this variable is false, the loop finishes (@see exitGame method). \n
	 *	\n
	 *	The Game can observe any element in the system. So it can receives information
	 *	from them, and response as needed. \n
	 *  \n
	 *	The game can manage also a GuiSystem which is a set of widgets used usually to configure
	 *	special behaviour of the game. The game instance is not the owner of the GUI system. \n
	 *	So, the one creating it has also to destroy it! Normally it will be done in a game state. \n
	 *  \n
	 *  The game has also a configuration data. \n
	 *	First time the configurationData element is created a copy is saved (called cleanConfiguration). \n
	 *	It can be used along the game to know whatthe initial configuration values were. \n
	 *  \n
	 *  The main loop also manage LoopCounters. There is a method to create them and to obtain a reference to it. \n
	 *	These loop counters are kept into the game class (and also deteled in its destructor) and actulize per loop.
	 *	@see LoopCounter class. \n
	 *  \n
	 *	The game also has the possibility to create communication objects. \n
	 *	That creation is done using a communication builder that is got using the right method. \n
	 *	\n
	 *	The game also defines a log method, that can be obtained from any part of the code. \n
	 *	To insert things into the log two macros have been defined: __WLOG & __WLOGPLUS.\n
	 *	The first one just keeps the parameter passed to the macro into the log, while the other one
	 *	keeps the text but also the function and the line where the information was generated. \n
	 *	The method to active and desactivate the log is setLog.
	 *	The type of log created is defined in the configuration parameter "LOG". 
	 *	If no type was defined there then a console log is created by default ("CONSOLE"). \n
	 *	To create other types re/define the parameter "LOG" in the configuration data file (Conf.xml).
	 *	Also @see commsbuilder.hpp.
	 *	\n
	 *	When the game is created a default configuration file is read (if exists). \n
	 *	The variables defined there can be read using the method parameter (@see parameter method). \n
	 *	It is key to understand what variables can be defined there (@see defaultParameter for more information).
	 */
	class Game : public Observer, public Notifier
	{
		public:
		friend GameImplementation;

		/** A class to represent the configuration of the game. \n
			The configuration can be saved into and got from a file. */
		class Configuration
		{
			public:
			virtual ~Configuration ()
							{ }

			/** To clone. */
			virtual Configuration* clone () const = 0;
			/** To clean it from no pure configuration data. 
				By default it does nothing. */
			virtual void cleanTemporalData ()
							{ }

			/** To save the configuration into a file.
				If there is any error then -1 is returned. 0 in any other circunstance. 
				This method uses confToData method defined behind. */
			int save (const std::string& fN);
			/** To load configuration data from a file.
				If there is any error then -1 is returned. 0 in any other circumstance. 
				This method uses dataToConf method defined behind. */
			int load (const std::string& fN);

			protected:
			/** How to convert the configuration data from and to a string to be saved or retrieved from a file
				it is something that will depend on the structure of the file or on the configuration data itself. \n
				So it is something that has to be redefined later per game. */
			virtual int confToData (char*& str) = 0;
			virtual void dataToConf (char* str, int nB) = 0;

			/** This methid is to support the clone method defined above. \n
				Most of the cases, when clone method is implemented, an invocation to the copy constructor is enought.
				But sometimes it isn't . When there is the need to replicate memory objects as part of copy,
				copy constructor is not enough because it will copy only the references to that objects and not the objects itself.
				This method can be implemented to do so . \n
				The default implementation is empty. */
			virtual void cloneImplementation (const Configuration* dt)
							{ }
		};

		/** A no configuration object. 
			When configuration is not needed. */
		class NoConfiguration : public Configuration
		{
			protected:
			/** @see parent. */
			virtual Configuration* clone () const
							{ return (new NoConfiguration); }

			/** @see parent. */
			virtual int confToData (char*& str)
							{ return (0); }
			virtual void dataToConf (char*, int nB)
							{ }
		};

		/** 
		  *	A standard configuration. \n
		  *	This object keeps a set of configuration data. \n
		  *	Those configuration data elements are got from a list of elements passed at construction time. \n
		  *	The list can be actualized later with the method refreshElements. \n
		  *	The configuration data is then kept in a file. \n
		  *	There are method to get back the configuration data for an specific element 
		  *	and to verify whether it exists or not. \n
		  */
		class StdConfiguration : public Configuration
		{
			public:
			StdConfiguration (const Elements& elmts = Elements ())
				: Configuration (), _elementsToKeep (elmts), _confValues ()
							{ refreshConfiguration (); }

			/** @see parent. */
			virtual Configuration* clone () const
							{ return (new StdConfiguration (*this)); }

			void setElementsToKeep (const Elements& elmts)
							{ _elementsToKeep = elmts; refreshConfiguration (); }
			void refreshConfiguration ();
			bool existConfValueForElement (int id) const
							{ return (_confValues.find (id) != _confValues.end ()); }
			void removeConfValueForElement (int id);
			const SetOfOpenValues& confValueForElement (int id) const
							{ return ((*(_confValues.find (id))).second); }
			/** This method is to actualize the conf data for a specific element if it exist.
				Take into account that when save the conf or refresh it, that value will be replaced. 
				The element must be defined in the list. */
			void setConfValueForElement (int id, const SetOfOpenValues& oV);
			const SetsOfOpenValues& confValuesForElements () const
							{ return (_confValues); }

			protected:
			/** These are the important methods.
				They can be both overloaded later, but their default implementation
				iterates over the persistent elements kept into this class, saving the configurations on it. */
			virtual void cfgToStream (std::ostringstream& oS);
			virtual void cfgFromStream (std::istringstream& iS);

			private:
			/** @see parent. */
			virtual int confToData (char*& str);
			virtual void dataToConf (char* str, int nB);

			protected:
			Elements _elementsToKeep;
			SetsOfOpenValues _confValues;
		};

		/** 
		  *	A game can have a default configuration file with attributes that can be used 
		  *	along the game. The file has to be called: "conf.xml", and to be located 
		  *	in the same path than the EXE file. \n
		  *	If it exists, its structure should be as follows: \n
		  *	<?xml version="1.0"?> \n
		  *	<Properties> \n
		  *		<Property id="background" value=".\forms\background1.png"/> \n
		  *		... \n
		  *	</Properties> 
		  */
		Game (GameImplementation* imp, int fps = 60);
		virtual ~Game (); // To delete builders,... (singleton elements mainly)

		/** To get a reference to the unique class of this type. */
		static Game* game ();

		GameImplementation* implementation ()
							{ return (_implementation); }

		/** To get a reference to the configuration element. */
		Configuration* configuration ()
							{ return (_configuration); }
		const Configuration* configuration () const
							{ return (_configuration); }
		const Configuration* cleanConfiguration () const // Can't be modified...ever!
							{ return (_cleanConfiguration); }
		/** To set a new configuration object. \n
			By default it deletes the previous elements and sets the new one. \n
			It is virtual, because it could be extended, to do any other thing or extend the default behaviour. */
		virtual void setConfiguration (Configuration* cfg);

		/** To get one of the parameters loaded from
			The configuration file. If it doesn't exist
			a default value should be return (@see defaultParameter method), or a null string minimum. */
		std::string parameter (const std::string& p) const
							{ return (_properties.existsProperty (p) 
								? _properties.property (p) : defaultParameter (p)); }
		/** To get all parameters loaded at initialization. */
		const ConfigurationFile::Properties& properties () const
							{ return (_properties.properties ()); }

		/** To check whether the application is or not running. */
		bool isRunning () const
							{ return (_running); }
		/** To check whether the application has or not been initialized already. */
		bool isInitialized () const
							{ return (_initialized); }

		/** To know which is the number of frames (loops) per second
			This is a very important variable to keep sync the application. \n
			It can be used later in any Element of the game to calculate seconds e.g. */
		int framesPerSecond () const
							{ return (_framesPerSecond); }
		/** The currentloops per second. */
		int currentLoopsPerSecond () const
							{ return (QGAMES::Timer::timer () -> loopsPerSecond ()); }
		/** To change the frames per second. Tke care using this method. */
		void setFramesPerSecond (int fps)
							{ assert (fps > 0); _framesPerSecond = fps; }

		/** An application linked to the frame rate, draws every
			framerate times per second. */
		void setLinkDrawToFrameRate (bool s)
							{ _linkDrawToFrameRate = s; }

		/** An application pre loading the contents, loads frames, entities, etc...
			before it runs. */
		void setPreLoad (bool p)
							{ _preLoad = p; }

		// To get things from the builders...
		/** To get a form. */
		Form* form (int nf)
							{ return (_formBuilder -> form (nf)); }

		bool existsEntity (int ne)
							{ return (entityBuilder () -> existsEntity (ne)); }
		/** To get an entity, and observe it...
			call this method twice doesn't means observe the entity twice... */
		Entity* entity (int ne);
		/** To get a set of entities. */
		Entities entities (int from, int to);
		Entities entities (const std::vector <int> et);

		bool existsCharacter (int nC)
							{ return (existsEntity (nC) && dynamic_cast <QGAMES::Character*> (entity (nC))); }
		/** The same but with a character. \n
			If the requested entity is not a character then the method crashes in debug mode. */
		Character* character (int nc);
		Characters characters (int from, int to); // All of them have to be characters
		Characters characters (const std::vector <int> chrs);

		/** To get a movement. */
		Movement* movement (int nm)
							{ return (_movementBuilder -> movement (nm)); }

		/** To get a sound. */
		Sound* sound (int ns)
							{ return (_soundBuilder -> sound (ns)); }

		/** To get a reference to the external engine used by the implementation...if any.
			If the external engine has not be defined yet, the method tries to create first. */
		_ExEngine* externalEngine ()
							{ return ((_externalEngine == NULL) 
								? (_externalEngine = createExternalEngine ()) : _externalEngine); }

		/** To get the reference to the builders of the game. \n
			When invoked first time, the system tries to create them using methods defined below. \n
			As the creation process relies on a user's method they fnal outcome can be NULL, so take care!". */
		FormBuilder* formBuilder () 
							{ return ((_formBuilder == NULL) 
								? (_formBuilder = createFormBuilder ()) : _formBuilder); }
		TextBuilder* textBuilder () 
							{ return ((_textBuilder == NULL)
								? (_textBuilder = createTextBuilder ()) : _textBuilder); }
		MaskBuilder* maskBuilder ()
							{ return ((_maskBuilder == NULL)
								? (_maskBuilder = createMaskBuilder ()) : _maskBuilder); }
		ObjectBuilder* objectBuilder () 
							{ if (_objectBuilder == NULL)
								(_objectBuilder = createObjectBuilder ()) -> setExternalEngine (externalEngine ());
							  return (_objectBuilder); }
		EntityBuilder* entityBuilder () 
							{ if (_entityBuilder == NULL)
								(_entityBuilder = createEntityBuilder ()) -> setExternalEngine (externalEngine ());
							  return (_entityBuilder); }
		GUISystemBuilder* guiSystemBuilder ()
							{ if (_guiSystemBuilder == NULL)
								(_guiSystemBuilder = createGUISystemBuilder ()) -> setExternalEngine (externalEngine ());
							  return (_guiSystemBuilder); }
		MovementBuilder* movementBuilder () 
							{ return ((_movementBuilder == NULL) 
								? (_movementBuilder = createMovementBuilder ()) : _movementBuilder); } 
		SoundBuilder* soundBuilder () 
							{ return ((_soundBuilder == NULL) 
								? (_soundBuilder = createSoundBuilder ()) : _soundBuilder); }
		ResourcesGroupLoader* resourcesGroupLoader ()
							{ return ((_resourceGroupLoader == NULL) 
								? (_resourceGroupLoader = createResourceGroupLoader ()) : _resourceGroupLoader); }
		CharacterControlStepsMonitorBuilder* characterMonitorBuilder ()
							{ return ((_characterMonitorBuilder == NULL)
								? (_characterMonitorBuilder = createCharacterMonitorBuilder ()) : _characterMonitorBuilder); }
		CommunicationChannelBuilder* communicationsBuilder ()
							{ return ((_communicationsBuilder == NULL)
								? (_communicationsBuilder = createCommunicationsBuilder ()) : _communicationsBuilder); }

		InputHandler* inputHandler ();

		/** The timer is who controls the time frame. \n
			This is really really critical for the game to progress soomthly. */
		Timer* timer ()
							{ return ((Timer::timer () == NULL) ? (createTimer ()) : Timer::timer ()); }

		/** To get the main screen of the game. \n
			Usually games only uses one screen which is the main one. */
		Screen* mainScreen ()
							{ return screen (__QGAMES_MAINSCREEN__); }
		/** To get other screens. */
		bool existScreen (int id);
		Screen* screen (int id);

		// To manage the states of the game
		void addState (GameState* gS, const std::string& nS);
		void addMainStatesFromBuilder ();
		const GameState* activeState () const
							{ return (_currentGameState); }
		GameState* activeState ()
							{ return (_currentGameState); }
		bool existState (const std::string& s) const
							{	return (_gameStates.find (s) != _gameStates.end ()); }
		const GameState* state (const std::string& s) const;
		GameState* state (const std::string& s)
							{ return ((GameState*) (((const Game*) this) -> state (s))); }
		GameStateBuilder* gameStateBuilder () // Including the way to get the gameState builder
							{ return ((_gameStateBuilder == NULL) 
								? (_gameStateBuilder = createGameStateBuilder ()) : _gameStateBuilder); } 
		void setState (const std::string& s);
		void setState (int id); // Using the id...
		void setState (GameState* gS); // Using the game state...it has to exist and not to be null...

		// To manage the adds on of the game
		void addAddOn (const std::string& n, GameTypeAddOn* a);
		const GameTypeAddOn* addOn (const std::string& n) const;
		GameTypeAddOn* addOn (const std::string& n)
							{ return ((GameTypeAddOn*) ((const Game*) this -> addOn (n))); }
		bool existsAddOn (const std::string& n) const
							{ return (_gameAddsOn.find (n) != _gameAddsOn.end ()); }
		
		// To manage the game...
		/** The main loop of the game. \n
			This is the most important method of the class. \n
			It starts an infinite loop doing always the same: InEveryLoop, UpdatePositions, and... */
		void exec ();
		/** The only way to break the game externally is to invoke this method. */
		virtual void exitGame ()
							{ _running = false; }
		/** To pause the game.
			The updatePositions method of all elements will not be executed. \n
			It can be overloaded, but never forget to invoke parent's one. */
		virtual void pauseGame ()
							{ _paused = true; }
		/** To continue the game. */
		virtual void continueGame ()
							{ _paused = false; }
		/** To know whether the game is or not paused. */
		bool isGamePaused () const
							{ return (_paused); }

		/** 
		  *	This two functions are to add or to remove score objects. \n
		  *	Those objects are usually shown at a specific position of the screen and they are
		  *	to show/control the situation of the game. \n
		  *	By default adding does not do anything, and remove cleans up the collection. \n
		  *	It is supposed the elements inside belongs to the game, and they will come
		  *	from the builder. Otherwise, you are accountable of deleting those in the removeScoreObjects method.
		  */
		virtual void addScoreObjects ()
							{ }
		virtual void removeScoreObjects ()
							{ _scoreObjects.clear (); }
		bool areThereScoreObjects ()
							{ return (!_scoreObjects.empty ()); }
		const Entities& scoreObjects ()
							{ return (_scoreObjects); }
		void addScoreObject (Entity* sObj);

		/** To manage loop counters within the main loop. 
			Using this method, the game class is accountable of deleting the counter when finishes. */
		LoopCounter* createLoopCounter (int id, int nb, bool c = true, LoopCounter::Action* a = NULL);
		bool existsLoopCounter (int id) const;
		const LoopCounter* loopCounter (int id) const
							{ return ((const LoopCounter*) (((Game*) (this)) -> loopCounter (id))); }
		LoopCounter* loopCounter (int id);
		void removeLoopCounter (int id);

		/** To process events sent from the things observed (usually Entities and Characters) \n
			By default it does nothing. */
		virtual void processEvent (const Event& e)
							{ }

		/** The unique Id generator can be useful to generate simple ids.
			By default a standard one is created. */
		const UniqueIdGenerator* uniqueIdGenerator () const
							{ return ((_uniqueIdGenerator == NULL)
									? (_uniqueIdGenerator = createUniqueIdGenerator ()) : _uniqueIdGenerator); }
		/** To get a unique id. \n
			It can be redefined. \n
			By default, uses the as a reference the time the game has been created as a base. */
		virtual int uniqueId () const
							{ return (uniqueIdGenerator () -> nextId ()); }

		// Managing the log
		/** To active or desactive the log. */
		void setLog (bool s);
		/** To get a reference to the log. */
		CommunicationChannel* log ()
							{ return (_log); }
		/** To write something int the log */
		void writeIntoLog (const std::string str, 
			const std::string& f = std::string (__NULL_STRING__), int l = -1);

		protected:
		/** 
		  *	The methods used to create the different builders... \n
		  *	Some of them depends on the way the framework is implemented. It is 
		  *	the libraries behind, but that behaviour can be changed. \n
		  *	Which objects, entities, or movements can be created depends on the game. 
		  */
		virtual GameStateBuilder* createGameStateBuilder ()
							{ return (new GameStateBuilder (parameter (__GAME_PROPERTYGAMESTATESFILE__))); }
		virtual FormBuilder* createFormBuilder ()
							{ return (_implementation -> createFormBuilder ()); }
		virtual TextBuilder* createTextBuilder () // It is not mandatory in the basic implementation...
							{ return (NULL); }
		virtual MaskBuilder* createMaskBuilder ()
							{ return (new MaskBuilder (parameter (__GAME_PROPERTYMASKFILE__))); }
		virtual ObjectBuilder* createObjectBuilder ()
							{ return (new ObjectBuilder (parameter (__GAME_PROPERTYOBJECTSFILE__), formBuilder ())); }
		virtual EntityBuilder* createEntityBuilder () = 0; // It something to define per implementation
		virtual GUISystemBuilder* createGUISystemBuilder ()
							{ return (new StandardGUISystemBuilder (parameter (__GAME_PROPERTYGUISYSTEMFILE__), formBuilder ())); }
		virtual MovementBuilder* createMovementBuilder () = 0; // Something also mandatory per implementation
		virtual SoundBuilder* createSoundBuilder ()
							{ return (_implementation -> createSoundBuilder ()); }
		virtual ResourcesGroupLoader* createResourceGroupLoader ()
							{ return (NULL); } // By default it is not needed...
		virtual CharacterControlStepsMonitorBuilder* createCharacterMonitorBuilder ()
							{ return (NULL); } // By default it is not needed...
		virtual CommunicationChannelBuilder* createCommunicationsBuilder ()
							{ return (NULL); } // Ny default it is not needed...

		/** To create the external engine. */
		virtual _ExEngine* createExternalEngine ()
							{ return (_implementation -> createExternalEngine ()); }

		/** A method to create the configuration class. \n
			The things to keep as part of the configuration class can be different depending on the game.
			So a virtual method is needed to create the right configuration object. */
		virtual Configuration* createConfiguration ()
							{ return (new StdConfiguration ()); } // By default...the standard configuration over nothing
		
		/** The timer is the way the time per loop is controlled.
			So it is so really important.  \n
			Once is created by default but it can be changed. */
		virtual Timer* createTimer ()
							{ return (new UsualTimer ()); }
		
		/** The creation of the input handler used by the game. */
		virtual InputHandler* createInputHandler () = 0; // Mandatory per implementation
		/** What are the screens of the game. It is something that depends on the game behind.
			So the specific implementation has to be set per game. */
		virtual Screens createScreens () = 0; // Obviosuly one mandatory per implementation (usually relies on the implementation)

		/** The unique id generator can be a useful way to guaranttee 
			an id is unique in the instalation. 
			In the standard implementation the base is set in a number 
			great enough to avoid conflict witj usual entities' id. */
		virtual UniqueIdGenerator* createUniqueIdGenerator () const
							{ return (new StandardUniqueIdGenerator ()); }

		/**	
		  *	This method can be overloaded, and it is to get the default value of
		  *	a parameter in case it has not been loaded at construction time
		  *	from the configuration file... \n
		  *	By default it returns a default value for the definition files of the builders and (@see definitions.hpp)
		  * 1 .- __GAME_XSCALE__						: The scale of the font (x) in the system (__GAME_DEFAULTXSCALE__)
		  * 2 .- __GAME_YSCALE__						: The scale of the font (y) in the system __GAME_DEFAULTYSCALE__
		  * 3 .- __GAME_PROPERTYGAMESTATESFILE__		: The name of definition file for states (__GAME_GAMESTATESDEFINITIONSTDFILE__)
		  * 4 .- __GAME_PROPERTYFORMSFILE__				: Same for forms (__GAME_FORMSDEFINITIONSTDFILE__)
		  * 5 .- __GAME_PROPERTYMASKFILE__				: Same for maks if any (__GAME_MASKDEFINITIONSSTDFILE__)
		  * 6 .- __GAME_PROPERTYOBJECTSFILE__			: Same for objects (__GAME_OBJECTSDEFINITIONSTDFILE__)
		  * 7 .- __GAME_PROPERTYENTITIESFILE__			: Same for entities (__GAME_ENTITIESDEFINITIONSSTDFILE__)
		  * 8 .- __GAME_PROPERTYMOVEMENTSFILE__			: Same for movements (__GAME_MOVEMENTSDEFINITIONSSTDFILE__)
		  * 9 .- __GAME_PROPERTYSOUNDSFILE__			: Same for sounds (__GAME_SOUNDSDEFINITIONSTDFILE__)
		  * 10.- __GAME_PROPERTYGUISYSTEMFILE__			: Same for gui system (__GAME_GUISYSTEMDEFINITIONSDFILE__)
		  * 11.- __GAME_PROPERTUCHARACTERMONITORFILE__	: Same for character's monitor (__GAME_CHARACTERMONITORSTDFILE__)
		  */
		virtual std::string defaultParameter (const std::string& p) const;

		// The methods executed in the exec main loop.
		/** Once. By default initialize the implementation, the timer, 
			creates the screens, and preload (if any) the builder's content. 
			to speed up the game performance. */
		virtual void initialize ();  
		/** Read the events and manage them. */
		virtual void handleEvents (); 
		/** Every loop, by default actualize the timer and calls the equivalent in the implementation. \n
			This is critical. If you forget to do so, the system can behave strange. \n
			The GUI system if it exists is also managed here. */
		virtual void inEveryLoop (); 
		/** As much as the number of frames per second. \n
			By default the score objects are updated. \n
			Remember that score objects' default behaviour is not to update their positions, but who knows? */
		virtual void updatePositions (); 
		/** This routine is to draw the game... \n
			It is declared public because it could be invoke from any game state e.g. \n
			Any case it is invoked from the main loop, as described at the beginning. \n
			The GUI system is managed here if it exists. */
		virtual void drawOn (Screen* s);
		/** As much as the number of frames per second. \n
			This method is really and exit place for the real game to do something special 
			in every loop. By default nothing is done. */
		virtual void additionalTasks () 
							{ }
		/** Once. The default implementation is to finalize the timer. */
		virtual void finalize (); 

		/** This method is checked before game class updates the positions. \n
			So it could be used to avoid updating under specific circumstances.
			By default it returns true. \n
			In some games, when the game in under an specific state, the update 
			method shouldn't be executed to avoid i.e. the monsters move or similar. */
		virtual bool mustIUpdate () const
							{ return (true); }

		/** This method determines whether the game is drawn before than the state or not. \n
			By default it is. So the game state is always drawn on top of game's information but it couldn't!. */
		virtual bool drawGameBeforeState () const
							{ return (true); }

		protected:
		/** There can only be one instance of this class per EXE. */
		static Game* _theGame;

		GameImplementation* _implementation; /** The implementation of the game. */
		int _framesPerSecond; /** Number of frames the game draws per second (at construction time). */
		_ExEngine* _externalEngine; /** A reference to the external engine. */
		MaskBuilder* _maskBuilder; /** Who must create the masks?. */
		GameStateBuilder* _gameStateBuilder; /** To build game states. */
		FormBuilder* _formBuilder; /** Who must create the forms? */
		TextBuilder* _textBuilder; /** Who must create the texts, if any. */
		ObjectBuilder* _objectBuilder; /** Who must create the objects? */
		EntityBuilder* _entityBuilder; /** Who must create the entities? */
		GUISystemBuilder* _guiSystemBuilder; /** Who must create the interfaces to interact and define usually parameters. */
		MovementBuilder* _movementBuilder; /** Who must create the movements? */
		InputHandler* _inputHandler; /** Who must create the input handler to manage the events? */
		SoundBuilder* _soundBuilder; /** Who must create the sounds? */
		ResourcesGroupLoader* _resourceGroupLoader; /** Lo load group of resources. */
		CharacterControlStepsMonitorBuilder* _characterMonitorBuilder; /** To create monitor for characters. */
		CommunicationChannelBuilder* _communicationsBuilder; /** To create communications. It is not mandatory. */
		mutable UniqueIdGenerator* _uniqueIdGenerator; /** To crate unique ids in the instalation. */
		Configuration* _configuration;
		Configuration* _cleanConfiguration; /** A copy of the configuration, created one the previous is created too. */
		Screens _screens; /** The screens of the game. A game can manage several screens if needed. */
		/** The score objects are fixed objects in the screen.
			The usually represents elements in an score such as the remaining lives and so. */
		Entities _scoreObjects;

		GameStates _gameStates; /** The different states a game can be on. */
		GameState* _currentGameState; /** ...and, which is the current one? (if any) */

		GameTypeAddsOnMap _gameAddsOn; /** The potential adds on managed by the game. */

		bool _logActive; /** To indicate whether the log is or not active. By default it is. */
		CommunicationChannel* _log; /** A log to be used for the application if any. */

		// Basic attributes
		bool _linkDrawToFrameRate; /** To guarantee that it draws only x number of times per second. */
		bool _preLoad; /** Does it have to load the builders' content at initialization time. */
		ConfigurationFile _properties; /** The configiuration data file. */
		LoopCounters _loopCounters; /** The loop counters managed in the exec main loop. */	

		// Implementation
		bool _running; /** Is the game running? */
		bool _paused; /** Is the game paused. */
		bool _initialized; /** Has the game been initialized already? */
	};
}

// To Simplify the access to the game object...
#define __GM	(QGAMES::Game*)
#define __WLOG(T)  QGAMES::Game::game () -> writeIntoLog (T)
#define __WLOGPLUS(T) QGAMES::Game::game () -> writeIntoLog (T, __FILE__, __LINE__)

#endif
  
// End of the file
/*@}*/
