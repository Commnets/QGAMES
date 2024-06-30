#include <Common/game.hpp>
#include <Common/widgetsystem.hpp>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

QGAMES::Game* QGAMES::Game::_theGame = NULL;

// ---
int QGAMES::Game::Configuration::save (const std::string& fN)
{
	char* data = NULL;
	int nBDT = confToData (data); // Returns the number of bites to write...
	if (nBDT == 0)
		return (-1); // If there is no configuration, then nothing to be saved!

	int result = 0;
	try
	{
		std::ofstream f (fN, std::fstream::out | std::fstream::binary | std::fstream::trunc);
		f.write (data, nBDT); // Write the data
		delete [] data; // Delete what was generated
		f.close (); // ...and close the file
	}
	catch (...) // Maybe it is the only try and catch block of the system!!
	{
		result = -1;
	}

	return (result);
}

// ---
int QGAMES::Game::Configuration::load (const std::string& fN)
{
	std::ifstream f (fN, std::fstream::in | std::fstream::binary);
	if (!f) return (-1); // The file hasn't been opened, so nothing can't be done!
	
	int result = 0;
	try
	{
		struct stat fSt;
		stat (fN.c_str (), &fSt);
		char* data = NULL;
		int nBDT = fSt.st_size;
		if (nBDT != 0)
		{
			data = new char [nBDT]; // Creates the right place to read that info
			f.seekg (0); // Back to the begining
			f.read (data, nBDT); // Reads the data...
			dataToConf (data, nBDT); // ...and converts it into the configuration data file
			delete [] data; // Deletes the info used
		}
		else
			result = -1; // There is no conf to load...

		f.close (); // finally closes the conf file!
	}
	catch (...)
	{
		result = -1;
	}

	return (result);
}

// ---
void QGAMES::Game::StdConfiguration::refreshConfiguration ()
{
	_confValues = QGAMES::SetsOfOpenValues ();
	for (QGAMES::Elements::const_iterator i = _elementsToKeep.begin (); i != _elementsToKeep.end (); i++)
		_confValues [(*i) -> id ()] = (*i) -> runtimeValues ();
}

// ---
void QGAMES::Game::StdConfiguration::removeConfValueForElement (int id)
{
	QGAMES::SetsOfOpenValues::const_iterator i = _confValues.find (id);
	assert (i != _confValues.end ());
	_confValues.erase (i);
}

// ---
void QGAMES::Game::StdConfiguration::setConfValueForElement (int id, const QGAMES::SetOfOpenValues& oV)
{
	assert (_confValues.find (id) != _confValues.end ()); // it has to exist...

	_confValues [id] = oV;
}

// ---
void QGAMES::Game::StdConfiguration::cfgToStream (std::ostringstream& oS)
{
	refreshConfiguration ();

	oS << (int) _confValues.size () << std::endl;
	for (QGAMES::SetsOfOpenValues::const_iterator i = _confValues.begin (); i != _confValues.end (); i++)
		oS << (*i).first << std::endl << (*i) .second << std::endl;
}

// ---
void QGAMES::Game::StdConfiguration::cfgFromStream (std::istringstream& iS)
{
	_confValues = QGAMES::SetsOfOpenValues ();

	int nE; iS >> nE;
	for (int i = 0; i < nE; i++)
	{
		int id; 
		QGAMES::SetOfOpenValues cfgVal;
		iS >> id; iS >> cfgVal;
		_confValues [id] = cfgVal;
	}

	// Sets the configuration into the elements if they exist...
	QGAMES::SetsOfOpenValues::const_iterator j;
	for (QGAMES::Elements::const_iterator i = _elementsToKeep.begin (); 
			i != _elementsToKeep.end (); i++)
		if ((j = _confValues.find ((*i) -> id ())) != _confValues.end ()) 
			(*i) -> initializeRuntimeValuesFrom ((*j).second);
}

// ---
int QGAMES::Game::StdConfiguration::confToData (char*& str)
{
	std::ostringstream dt;

	cfgToStream (dt);

	// Copy the result to the parameter...
	int result = dt.str ().size ();
	str = new char [result];
	memcpy (str, dt.str ().c_str (), result);
	for (int i = 0; i < result; i++)
	{
		if (str [i] == '\n') str [i] = 0x00; // Removes end of line or end of file character...
		str [i] += ' '; // Just to avoid the manipulation of the file...
	}

	return (result);
}

// ---
void QGAMES::Game::StdConfiguration::dataToConf (char* str, int nB)
{
	char* istr = new char [nB + 1];
	memcpy (istr, str, nB);
	for (int i = 0; i < nB; i++)
	{
		istr [i] -= ' ';
		if (istr [i] == 0x00) istr [i] = '\n'; // Restablishes end of line or end file character...
	}

	istr [nB] = 0;

	std::istringstream dt (istr);
	delete [] istr;

	cfgFromStream (dt);
}

// ---
QGAMES::Game::Game (QGAMES::GameImplementation* imp, int fps)
		: _implementation (imp),
		  _framesPerSecond (fps), 
		  // It can't never been greater that the refresh rate of the display, because the card will limit it!
		  _externalEngine (NULL),
		  _entityBuilder (NULL),
		  _guiSystemBuilder (NULL),
		  _gameStateBuilder (NULL),
		  _formBuilder (NULL),
		  _textBuilder (NULL),
		  _maskBuilder (NULL),
		  _objectBuilder (NULL),
		  _movementBuilder (NULL),
		  _inputHandler (NULL),
		  _soundBuilder (NULL),
		  _resourceGroupLoader (NULL),
		  _characterMonitorBuilder (NULL),
		  _communicationsBuilder (NULL),
		  _uniqueIdGenerator (NULL),
		  _configuration (NULL),
		  _cleanConfiguration (NULL),
		  _screens (),
		  _scoreObjects (),
		  _gameStates (),
		  _gameAddsOn (),
		  _logActive (false), // The log is not active by default...
		  _log (NULL), // So no log is created...
		  _currentGameState (NULL),
		  _linkDrawToFrameRate (true),
		  _preLoad (false),
		  _properties (QGAMES::ConfigurationFile (std::string (__GAME_CONFIGURATIONFILE__))),
		  _running (false),
		  _paused (false),
		  _initialized (false)
{ 
	assert (_implementation);
	assert (!_theGame); // Only one...
	assert (_framesPerSecond > 0); // Positive and bigger than 0 always...

	_theGame = this;
} 

// ---
QGAMES::Game::~Game ()
{
	QGAMES::LoopCounters::const_iterator k;
	for (k = _loopCounters.begin (); k != _loopCounters.end (); k++)
		delete ((*k));
	_loopCounters.clear ();

	QGAMES::GameStates::const_iterator j;
	for (j = _gameStates.begin (); j != _gameStates.end (); j++)
		delete ((*j).second);
	_gameStates.clear ();

	QGAMES::GameTypeAddsOnMap::const_iterator l;
	for (l = _gameAddsOn.begin (); l != _gameAddsOn.end (); l++)
		delete ((*l).second);
	_gameAddsOn.clear ();

	delete (_log);

	delete (_configuration);
	delete (_cleanConfiguration);

	delete (_entityBuilder); 
	delete (_movementBuilder);
	delete (_objectBuilder);
	delete (_gameStateBuilder);
	delete (_textBuilder);
	delete (_formBuilder); 
	delete (_maskBuilder);
	delete (_soundBuilder);
	delete (_resourceGroupLoader);
	delete (_characterMonitorBuilder);
	delete (_communicationsBuilder);
	delete (_uniqueIdGenerator);

	delete (_externalEngine);

	QGAMES::Screens::const_iterator i;
	for (i = _screens.begin (); i != _screens.end (); i++)
		delete ((*i).second);
	_screens.clear ();

	// The input handler has to be deleted after the screens
	// Because in some systems it is attached to the screen
	// and when the screen is deleted, then some part of the input method is called back!
	delete (_inputHandler);

	delete (_implementation);

	_theGame = NULL;
}

// ---
QGAMES::Game* QGAMES::Game::game ()
{
	assert (_theGame);

	return (_theGame);
}

// ---
void QGAMES::Game::setConfiguration (QGAMES::Game::Configuration* cfg)
{
	delete (_configuration);
	_configuration = cfg;
}

// ---
QGAMES::Entity* QGAMES::Game::entity (int ne)
{
	QGAMES::Entity* result = entityBuilder () -> entity (ne);
	observe (result);
	return (result);
}

// ---
QGAMES::Entities QGAMES::Game::entities (int from, int to)
{
	QGAMES::Entities result;
	for (int i = from; i <= to; i++)
		result [i] = entity (i);
	return (result);
}

// ---
QGAMES::Entities QGAMES::Game::entities (const std::vector <int> et)
{
	QGAMES::Entities result;
	for (int i = 0; i < (int) et.size (); i++)
		result [et [i]] = entity (et [i]);
	return (result);
}

// ---
QGAMES::Character* QGAMES::Game::character (int nc)
{
	QGAMES::Character* result = dynamic_cast <QGAMES::Character*> (entityBuilder () -> entity (nc));
	assert (result); // It should be...

	observe (result);
	return (result);
}

// ---
QGAMES::Characters QGAMES::Game::characters (int from, int to)
{
	QGAMES::Characters result;
	for (int i = from; i < to; i++)
		result [i] = character (i);
	return (result);
}

// ---
QGAMES::Characters QGAMES::Game::characters (const std::vector <int> chrs)
{
	QGAMES::Characters result;
	for (int i = 0; i < (int) chrs.size (); i++)
		result [chrs [i]] = character (chrs [i]);
	return (result);
}

// ---
QGAMES::InputHandler* QGAMES::Game::inputHandler ()
{ 
	if (_inputHandler == NULL)
	{ 
		_inputHandler = createInputHandler (); 
		_inputHandler -> initialize (); 
	} 
							  
	return (_inputHandler); 
}

// ---
bool QGAMES::Game::existScreen (int id)
{
	if (_screens.empty ())
		_screens = createScreens ();

	return (_screens.find (id) != _screens.end ()); 
}

// ---
QGAMES::Screen* QGAMES::Game::screen (int id)
{
	if (_screens.empty ())
		_screens = createScreens ();
	
	QGAMES::Screens::const_iterator i = _screens.find (id);
	assert (i != _screens.end ()); 

	return ((*i).second);
}

// ---
void QGAMES::Game::addState (QGAMES::GameState* gS, const std::string& nS)
{
	assert (gS);
	assert (nS != std::string (__NULL_STRING__)); // Can't be null...

	QGAMES::GameStates::const_iterator i = _gameStates.find (nS);
	assert (i == _gameStates.end ()); // No two equal...

	_gameStates.insert (QGAMES::GameStates::value_type (nS, gS));
}

// ---
void QGAMES::Game::addMainStatesFromBuilder ()
{
	if (gameStateBuilder () == NULL)
		return; // Nothing to do if no builder is defined...

	gameStateBuilder () -> preLoad (); // Just in case, loaded everything...
	QGAMES::GameStateBuilder::IdNameGameStates& gS = gameStateBuilder () -> mainGameStatesIdName ();
	for (QGAMES::GameStateBuilder::IdNameGameStates::const_iterator i = gS.begin (); i != gS.end (); i++)
		addState ((*i).second._gameState, (*i).second._name);
}

// ---
const QGAMES::GameState* QGAMES::Game::state (const std::string& s) const
{
	QGAMES::GameStates::const_iterator i = _gameStates.find (s);
	assert (i != _gameStates.end ());

	return ((*i).second);
}

// ---
void QGAMES::Game::setState (const std::string& s)
{
	QGAMES::GameStates::iterator i = _gameStates.find (s);
	assert (i != _gameStates.end ());

	if (_currentGameState != NULL)
		_currentGameState -> onExit ();

	// Save the previous situation, just in case...
	QGAMES::GameState* oldState = _currentGameState;
	_currentGameState = (*i).second;
	_currentGameState -> setPreviousState (oldState); 
	_currentGameState -> onEnter ();
}

// ---
void QGAMES::Game::setState (int tp)
{
	std::string n (__NULL_STRING__);
	for (QGAMES::GameStates::iterator i = _gameStates.begin ();
			i != _gameStates.end () && n == std::string (__NULL_STRING__); i++)
		if ((*i).second -> type () == tp)
			n = (*i).first;
	setState (n); // Uses the normal system to set the state!
}

// ---
void QGAMES::Game::setState (QGAMES::GameState* gS)
{
	std::string n (__NULL_STRING__);
	for (QGAMES::GameStates::iterator i = _gameStates.begin ();
			i != _gameStates.end () && n == std::string (__NULL_STRING__); i++)
		if ((*i).second == gS)
			n = (*i).first;
	setState (n);
}

// ---
void QGAMES::Game::addAddOn (const std::string& n, QGAMES::GameTypeAddOn* a)
{
	assert (a); // Can't be null...

	QGAMES::GameTypeAddsOnMap::const_iterator i = _gameAddsOn.find (n);
	assert (i == _gameAddsOn.end ()); // It hasn't to exist previously...

	_gameAddsOn.insert (QGAMES::GameTypeAddsOnMap::value_type (n, a));
}

// ---
const QGAMES::GameTypeAddOn* QGAMES::Game::addOn (const std::string& n) const
{
	QGAMES::GameTypeAddsOnMap::const_iterator i = _gameAddsOn.find (n);
	assert (i != _gameAddsOn.end ()); // It has to exist...

	return ((*i).second);
}

// ---
void QGAMES::Game::exec ()
{
	// The log is activated when the execution starts...
	// Just if the mode is not debugging...
#ifdef NDEBUG
	setLog (false);
#else
	setLog (true);
#endif

	__WLOG ("|red|Community Networks Library");
	__WLOG (std::string ("|blue|Version: ") + __QGAMES_LIBRARYVERSION__);
	__WLOG ("|cyan|Ignacio Cea Fornies");
	__WLOG ("--------------------------------------");
	
	__WLOGPLUS ("|green|Starting execution...");

	// Initialize for random numbers...
	srand ((int) time (NULL));

	int loopCounter = 0;

	__WLOGPLUS ("|yellow|Initilizing...");
	initialize ();
	if (!isInitialized ())
		return;
	__WLOGPLUS ("|yellow|Initilized...");

	_running = true;
	while (_running)
	{
		handleEvents ();
		inEveryLoop ();
		if (QGAMES::GUISystem::system () != NULL) // If it exists...
			QGAMES::GUISystem::system () -> inEveryLoop ();


		// Is the main loop linked to the frame rate or not?
		// if it is, then maybe this loop is not the right one to do anything...
		bool mustDoLoop = true;
		if (_linkDrawToFrameRate)
		{
			if (++loopCounter >= (timer () -> loopsPerSecond () / _framesPerSecond))
				loopCounter = 0;
			else
				mustDoLoop = false;
		}

		// If finally the loop has to be done...do it!
		if (mustDoLoop)
		{
			// Actualize the positions...
			if (!_paused)
			{
				if (_currentGameState != NULL)
					_currentGameState -> updatePositions ();
				if (mustIUpdate ()) // Only if needed! 
					updatePositions ();
			}

			// Actualize the image in the screens...
			for (QGAMES::Screens::const_iterator i = _screens.begin ();
				i != _screens.end (); i++)
			{
				QGAMES::Screen* scr = (*i).second;
				if (mustIUpdate ())
					scr -> updatePositions (); // If there is something to move...

				scr -> openTransaction ();
				
				if (drawGameBeforeState ())
					drawOn (scr); // The screen itself (world, entities of the world,...)
				if (_currentGameState != NULL)
					_currentGameState -> drawOn (scr); // The state itself...
				if (!drawGameBeforeState ())
					drawOn (scr); // The screen itself...

				if (QGAMES::GUISystem::system () != NULL) // And if it exists the gui system...
					QGAMES::GUISystem::system () -> drawOn (scr);

				scr -> drawOnYourOwn (); // Sometimes there is something to draw on its own!...
				
				scr -> closeTransaction ();
			}

			// Actualize the counters, and do additional tasks...
			if (!_paused)
			{
				for (QGAMES::LoopCounters::const_iterator j = _loopCounters.begin ();
					j != _loopCounters.end (); j++)
					(*j) -> count ();

				additionalTasks ();
			}
		}
	}
	
	_running = false;
	
	finalize ();

	__WLOGPLUS ("|green|...Finishing execution");
}

// ---
void QGAMES::Game::addScoreObject (QGAMES::Entity* sObj)
{
	assert (sObj); // Can't be null...

	if (_scoreObjects.find (sObj -> id ()) == _scoreObjects.end ())
		_scoreObjects.insert (QGAMES::Entities::value_type (sObj -> id (), sObj));
}

// ---
QGAMES::LoopCounter* QGAMES::Game::createLoopCounter (int id, int nb, bool c, QGAMES::LoopCounter::Action* a)
{
	QGAMES::LoopCounter* result = new QGAMES::LoopCounter (id, nb, c, a);
	result -> registerObserver (this);
	_loopCounters.push_back (result);
	return (result);
}

// ---
bool QGAMES::Game::existsLoopCounter (int id) const
{
	bool result = false;
	for (QGAMES::LoopCounters::const_iterator i = _loopCounters.begin (); 
		i != _loopCounters.end () && !result; i++)
		if ((*i) -> id () == id) result = true;
	return (result);
}

// ---
QGAMES::LoopCounter* QGAMES::Game::loopCounter (int id)
{
	QGAMES::LoopCounter* result = NULL;
	for (QGAMES::LoopCounters::const_iterator i = _loopCounters.begin (); 
		i != _loopCounters.end () && !result; i++)
		if ((*i) -> id () == id) result = (*i);
	
	assert (result);
	return (result);
}

// ---
void QGAMES::Game::removeLoopCounter (int id)
{
	QGAMES::LoopCounters::const_iterator result = _loopCounters.end ();
	for (QGAMES::LoopCounters::const_iterator i = _loopCounters.begin (); 
			i != _loopCounters.end () && result == _loopCounters.end (); i++)
		if ((*i) -> id () == id)
			result = i;

	assert (result != _loopCounters.end ());

	delete ((*result));
	_loopCounters.erase (result);
}

// ---
void QGAMES::Game::setLog (bool s)
{
	if ((s && _logActive) || (!s && !_logActive))
		return; // Nothing changes...

	_logActive = s;

	if (_logActive)
	{
		QGAMES::CommunicationChannelBuilder* bld = communicationsBuilder ();
		_log = (bld != NULL) 
			? _log = bld -> createCommChannel (parameter (std::string (__GAME_LOGTYPEPROPERTYNAME__)))
			: _log = new CommunicationChannel (new ConsoleLogChannelImplementation);
		// If no builder for communications has been defined (it is not mandatory) a default log based on the console is created...
	}
	else
	{
		delete (_log);

		_log = NULL;
	}
}

// ---
void QGAMES::Game::writeIntoLog (const std::string str, const std::string& f, int l)
{ 
	if (!_log)
		return; // It there is no log, nothing to write...

	std::string pI (__NULL_STRING__);
	if (f != std::string (__NULL_STRING__) && l != -1) // Something to add?
	{
		std::stringstream s;
		s << f << "," << l;
		pI = std::string ("[") + s.str () + std::string ("]");
	}

	_log -> send (QGAMES::CommunicationChannelTarget (std::string ("LOG")), pI + str); 
}

// ---
std::string QGAMES::Game::defaultParameter (const std::string& p) const
{
	std::string result (__NULL_STRING__);

	// Gets the default value for the basic attributes that the game needs to created builders
	// from definition files...
	if (p == std::string (__GAME_DATADIRPROPERTYNAME__))
		result = std::string (__GAME_DEFAULTDATADIR__);
	else if (p == std::string (__GAME_XSCALE__))
		result = std::string (__GAME_DEFAULTXSCALE__);
	else if (p == std::string (__GAME_YSCALE__))
		result = std::string (__GAME_DEFAULTYSCALE__);
	else if (p == std::string (__GAME_PROPERTYGAMESTATESFILE__))
		result = std::string (__GAME_GAMESTATESDEFINITIONSTDFILE__);
	else if (p == std::string (__GAME_PROPERTYFORMSFILE__)) 
		result = std::string (__GAME_FORMSDEFINITIONSTDFILE__);
	else if (p == std::string (__GAME_PROPERTYMASKFILE__))
		result = std::string (__GAME_MASKDEFINITIONSSTDFILE__);
	else if (p == std::string (__GAME_PROPERTYOBJECTSFILE__))
		result = std::string (__GAME_OBJECTSDEFINITIONSTDFILE__);
	else if (p == std::string (__GAME_PROPERTYENTITIESFILE__))
		result = std::string (__GAME_ENTITIESDEFINITIONSSTDFILE__);
	else if (p == std::string (__GAME_PROPERTYMOVEMENTSFILE__))
		result = std::string (__GAME_MOVEMENTSDEFINITIONSSTDFILE__);
	else if (p == std::string (__GAME_PROPERTYSOUNDSFILE__)) 
		result = std::string (__GAME_SOUNDSDEFINITIONSTDFILE__);
	else if (p == std::string (__GAME_PROPERTYGUISYSTEMFILE__)) 
		result = std::string (__GAME_GUISYSTEMDEFINITIONSDFILE__);
	else if (p == std::string (__GAME_PROPERTUCHARACTERMONITORFILE__))
		result = std::string (__GAME_CHARACTERMONITORSTDFILE__);
	else if (p == std::string (__GAME_LOGTYPEPROPERTYNAME__))
		result = std::string (__GAME_DEFAULTLOGTYPE__);

	return (result);
}

// ---
void QGAMES::Game::initialize ()
{
	if (isInitialized ())
		return; // If it's been initialized already then there is nothing else to do...

	// Initialize the implementation...
	_implementation -> setGame (this);
	if (!_implementation -> initialize ())
		return; // Not initialized!
	
	// Timer...
	// Just calling the method, the timer is created...
	timer (); 

	// Screen...
	// Just trying to get the main screen, the screens are created!
	mainScreen (); // At this point all screen are created first time!
	_implementation -> initializeScreens (); // Just in case...usually it is not needed!

	(_preLoad) ? soundBuilder () -> preLoad () : soundBuilder ();
	(_preLoad) ? formBuilder () -> preLoad () : formBuilder ();
	(_preLoad) ? objectBuilder () -> preLoad () : objectBuilder ();
	(_preLoad) ? movementBuilder () -> preLoad () : movementBuilder ();
	(_preLoad) ? entityBuilder () -> preLoad () : entityBuilder ();
	guiSystemBuilder (); // Creates the builder for GUI systems...really useful in complex games...
	// Builders for resources and for monitor are not created by default...

	// Just trying to get the input handler,
	// The system is initialized!
	inputHandler ();

	_configuration = createConfiguration ();
	_cleanConfiguration = _configuration -> clone (); // Clean, and it can't never be changed!

	// The game states can't be loaded here
	// Because they could depend on things being loaded in further methods...

	// All adds on have to be added before initialized the game
	// Otherwise they won't be initialized and the their initialization should have to be done manually...
	for (QGAMES::GameTypeAddsOnMap::const_iterator i = _gameAddsOn.begin ();
		i != _gameAddsOn.end (); (*i++).second -> initialize ());

	_initialized = true; 
}

// ---
void QGAMES::Game::handleEvents ()
{
	_implementation -> handleEvents ();
}

// ---
void QGAMES::Game::inEveryLoop () 
{ 
	for (QGAMES::GameTypeAddsOnMap::const_iterator i = _gameAddsOn.begin ();
		i != _gameAddsOn.end (); (*i++).second -> inEveryLoop ());

	timer () -> inEveryLoop (); 
	implementation () -> inEveryLoop ();

	for (QGAMES::Entities::const_iterator i = _scoreObjects.begin ();
			i != _scoreObjects.end (); i++)
		(*i).second -> inEveryLoop ();
}

// ---
void QGAMES::Game::updatePositions ()
{
	for (QGAMES::GameTypeAddsOnMap::const_iterator i = _gameAddsOn.begin ();
		i != _gameAddsOn.end (); (*i++).second -> inEveryLoop ());

	for (QGAMES::Entities::const_iterator i = _scoreObjects.begin ();
			i != _scoreObjects.end (); i++)
		(*i).second -> updatePositions ();
}

// ---
void QGAMES::Game::drawOn (QGAMES::Screen* s)
{
	for (QGAMES::GameTypeAddsOnMap::const_iterator i = _gameAddsOn.begin ();
		i != _gameAddsOn.end (); (*i++).second -> drawOn (s));

	// Changes the camera and put the very basic one...
	QGAMES::Camera* cm = s -> currentCamera ();
	s -> setCurrentCamera (std::string (__QGAMES_DEFAULTCAMERAID__));

	// Draws the objects...
	for (QGAMES::Entities::const_iterator i = _scoreObjects.begin ();
			i != _scoreObjects.end (); i++)
	{
		QGAMES::Entity* e = (*i).second;
		QGAMES::Position p = s -> position ();
		e -> drawOn (s, e -> position () + p);
	}

	// Sets back to the previous camera...
	s -> setCurrentCamera (cm);
}

// ---
void QGAMES::Game::finalize () 
{ 
	for (QGAMES::GameTypeAddsOnMap::const_iterator i = _gameAddsOn.begin ();
		i != _gameAddsOn.end (); (*i++).second -> finalize ());

	_inputHandler -> finalize ();
	_implementation -> finalizeScreens ();
	_implementation -> finalize ();
	_initialized = false; 
}
