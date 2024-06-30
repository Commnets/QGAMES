#include <PacManLike/pcgamestates.hpp>
#include <PacManLike/pcgame.hpp>

const int PACMAN::StopPacmanElemntsMovingGameState::_FADE [14] = 
	{ 255, 240, 220, 200, 180, 160, 140, 120, 100, 80, 60, 40, 20, 0 };

// ---
void PACMAN::InitializePACMANGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	PACMAN::Game* aG = dynamic_cast <PACMAN::Game*> (game ());
	assert (aG != nullptr); // Just in case...

	aG -> initializeEntitiesAndWorlds ();
}

// ---
void PACMAN::InitializePACMANGameState::updatePositions ()
{
	notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
}

// ---
PACMAN::MonsterMovingGameState::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _fade (255),
	  _position (QGAMES::Position (__BD 0.5, __BD 0.5, __BD 0)), // Center position in a 2D screen...
	  _iterations (-1),
	  _bigPacman (false)
{
	if (prps.find (__PACMAN_GAMESTATEMOVMONSTERATTRFADE__) != prps.end ()) 
		_fade = std::atoi ((*prps.find (__PACMAN_GAMESTATEMOVMONSTERATTRFADE__)).second.c_str ());
	if (prps.find (__PACMAN_GAMESTATEMOVMONSTERATTRPOSITION__) != prps.end ()) 
		_position = QGAMES::Position ((*prps.find (__PACMAN_GAMESTATEMOVMONSTERATTRPOSITION__)).second);
	if (prps.find (__PACMAN_GAMESTATEMOVMONSTERATTRITERATIONS__) != prps.end ()) 
		_iterations = std::atoi ((*prps.find (__PACMAN_GAMESTATEMOVMONSTERATTRITERATIONS__)).second.c_str ());
	if (prps.find (__PACMAN_GAMESTATEMOVMONSTERATTRBIGPACMAN__) != prps.end ()) 
		_bigPacman = (*prps.find (__PACMAN_GAMESTATEMOVMONSTERATTRBIGPACMAN__)).second == std::string (__YES_STRING__) ? true : false;

	PACMAN::MonsterMovingGameState::Properties::Properties (_fade, _position, _iterations, _bigPacman);
}

// ---
PACMAN::MonsterMovingGameState::~MonsterMovingGameState ()
{
	for (int i = 0; i < (int) _artists.size (); i++)
	{
		_artists [i] -> addControlStepsMonitor (nullptr);
		_artists [i] = nullptr;
	}
}

// ---
void PACMAN::MonsterMovingGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	// Get the monsters and pacman...
	_artists [0] = dynamic_cast <PACMAN::Artist*> (game () -> entity (__PACMAN_PACMANBASEENTITYID__));
	_artists [1] = dynamic_cast <PACMAN::Artist*> (game () -> entity (__PACMAN_INKYBASEENTITYID__));
	_artists [2] = dynamic_cast <PACMAN::Artist*> (game () -> entity (__PACMAN_PINKYBASEENTITYID__));
	_artists [3] = dynamic_cast <PACMAN::Artist*> (game () -> entity (__PACMAN_BLINKYBASEENTITYID__));
	_artists [4] = dynamic_cast <PACMAN::Artist*> (game () -> entity (__PACMAN_CLYDEBASEENTITYID__));
#ifndef _NDEBUG
	for (int i = 0; i < (int) _artists.size (); i++)
		assert (_artists [i] != nullptr);
#endif

	// All of them are set in the state running to the right...
	for (auto i : _artists)
		{ i -> setOrientation (QGAMES::Vector (__BD 0, __BD -1, __BD 0) /** The only one accepted for monsters. */); i -> toStand (); }
	_artists [0] -> setCurrentState (__PACMAN_PACMANSTATEEATINGLOOKINGRIGHT__);
	_artists [1] -> setCurrentState (__PACMAN_MONSTERSTATEMOVINGANDLOOKINGRIGHT__);
	_artists [2] -> setCurrentState (__PACMAN_MONSTERSTATEMOVINGANDLOOKINGRIGHT__);
	_artists [3] -> setCurrentState (__PACMAN_MONSTERSTATEMOVINGANDLOOKINGRIGHT__);
	_artists [4] -> setCurrentState (__PACMAN_MONSTERSTATEMOVINGANDLOOKINGRIGHT__);

	reStartAllCounters ();
	counter (_COUNTERITERATIONS) -> initialize 
		((_properties._iterations == -1) ? __MAXINT__ : _properties._iterations, 0, true, false);

	reStartAllOnOffSwitches ();

	QGAMES::Screen* scr = game () -> mainScreen  ();
	setInitialPositionFrom (
		QGAMES::Position (
			__BD scr -> visualWidth () * _properties._position.posX () * __BD -0.4,
			__BD scr -> visualHeight () * _properties._position.posY (),
			__BD 0));

	for (auto i : _artists)
		i -> setVisible (true);
}

// ---
void PACMAN::MonsterMovingGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	QGAMES::Screen* scr = game () -> mainScreen ();

	for (auto i : _artists)
	{
		i -> inEveryLoop ();
		i -> updatePositions ();
	}

	if (counter (_COUNTERMOVE) -> isEnd ())
	{
		QGAMES::bdata r = __BD (onOffSwitch (_SWITCHRIGHT) -> isOn () ? 1 : -1); 
		for (auto i : _artists)
		{
			QGAMES::bdata inc = __BD (((i != _artists [0] && r == __BD 1) || (i == _artists [0] && r != __BD 1)) ? 1.05 : 1);
			i -> setPosition (i -> position () + (__BD 2.5 * r * QGAMES::Vector (__BD inc, __BD 0, __BD 0)));
		}

		bool cd = false;
		if (_artists [0] -> centerPosition ().posX () > (__BD scr -> visualWidth () * __BD 1.4))
		{
			_artists [0] -> setCurrentState (_properties._bigPacman 
				? __PACMAN_BIGPACMANSTATEEATINGLOOKINGLEFT__ : __PACMAN_PACMANSTATEEATINGLOOKINGLEFT__);
			_artists [1] -> setCurrentState (__PACMAN_MONSTERSTATERUNNINGAWAY__);
			_artists [2] -> setCurrentState (__PACMAN_MONSTERSTATERUNNINGAWAY__);
			_artists [3] -> setCurrentState (__PACMAN_MONSTERSTATERUNNINGAWAY__);
			_artists [4] -> setCurrentState (__PACMAN_MONSTERSTATERUNNINGAWAY__);

			onOffSwitch (_SWITCHRIGHT) -> set (false); 

			setInitialPositionFrom (
				QGAMES::Position (
					__BD scr -> visualWidth () * __BD 1.4,
					__BD scr -> visualHeight () * _properties._position.posY (),
					__BD 0));
		}
		else
		if (_artists [0] -> centerPosition ().posX () < (__BD scr -> visualWidth () * __BD -0.4))
		{
			// After the defined number of iterations (if any) the nested game state finishes...
			if (counter (_COUNTERITERATIONS) -> isEnd ())
				notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
			// ...otherwise starts back again...
			else
			{
				_artists [0] -> setCurrentState (__PACMAN_PACMANSTATEEATINGLOOKINGRIGHT__);
				_artists [1] -> setCurrentState (__PACMAN_MONSTERSTATEMOVINGANDLOOKINGRIGHT__);
				_artists [2] -> setCurrentState (__PACMAN_MONSTERSTATEMOVINGANDLOOKINGRIGHT__);
				_artists [3] -> setCurrentState (__PACMAN_MONSTERSTATEMOVINGANDLOOKINGRIGHT__);
				_artists [4] -> setCurrentState (__PACMAN_MONSTERSTATEMOVINGANDLOOKINGRIGHT__);

				onOffSwitch (_SWITCHRIGHT) -> set (true); 

				setInitialPositionFrom (
					QGAMES::Position (
						__BD scr -> visualWidth () * __BD -0.4,
						__BD scr -> visualHeight () * _properties._position.posY (),
						__BD 0));
			}
		}
	}
}

// ---
void PACMAN::MonsterMovingGameState::drawOn (QGAMES::Screen* scr)
{
	QGAMES::GameState::drawOn (scr);

	for (auto i : _artists)
		i -> drawOn (scr);
}

// ---
void PACMAN::MonsterMovingGameState::onExit ()
{
	QGAMES::GameState::onExit ();

	for (int i = 0; i < 5; i++)
	{
		_artists [i] -> setVisible (true);
		_artists [i] = nullptr;
	}
}

// ---
__IMPLEMENTCOUNTERS__ (PACMAN::MonsterMovingGameState::Counters)
{
	addCounter (new QGAMES::Counter (PACMAN::MonsterMovingGameState::_COUNTERMOVE, 1, 0, true, true));
	// This last will be redefined later...
	addCounter (new QGAMES::Counter (PACMAN::MonsterMovingGameState::_COUNTERITERATIONS, 0, 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (PACMAN::MonsterMovingGameState::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (PACMAN::MonsterMovingGameState::_SWITCHRIGHT, true));
}

// ---		
void PACMAN::MonsterMovingGameState::setInitialPositionFrom (const QGAMES::Position& iPos)
{
	QGAMES::Position p = iPos;

	_artists [0] -> setPosition (p - _artists [0] -> currentForm () -> centerPosition (_artists [0] -> currentAspect ()));
	p -= ((!_properties._bigPacman || 
		   (_properties._bigPacman && onOffSwitch (_SWITCHRIGHT) -> isOn ())) ? __BD 2.4 : __BD 1.0) * 
		QGAMES::Vector (__BD _artists [0] -> visualLength (), __BD 0, __BD 0);

	for (std::vector <PACMAN::Artist*>::const_reverse_iterator i = _artists.rbegin (); i != _artists.rend (); i++)
	{
		if ((*i) != _artists [0])
		{
			(*i) -> setPosition (p - (*i) -> currentForm () -> centerPosition ((*i) -> currentAspect ()));
			p -= (__BD 8 / __BD 7) * QGAMES::Vector (__BD (*i) -> visualLength (), __BD 0, __BD 0);
		}
	}
}

// ---
void PACMAN::ShowingCurrentPlayerNameGameState::onEnter ()
{
	QGAMES::ShowingStandardTextGameState::onEnter ();

	// If there is no player name already introduced, then 
	// a GUI system is started to ask for it...
	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr); // Just in case...

	if (g -> playerName () == std::string (__NULL_STRING__))
	{
		_guiSystem = game () -> guiSystemBuilder () -> system (__PACMAN_PLAYERNAMEGUISYSTEM__);
		QGAMES::CompositeWidget* cW = dynamic_cast <QGAMES::CompositeWidget*> (_guiSystem -> mainWidget ());
		assert (cW != nullptr); // Just in case...
		QGAMES::IntroWidget* iW = 
			dynamic_cast <QGAMES::IntroWidget*> (cW -> widget (__PACMAN_PLAYERNAMEOPTIONID__));
		assert (iW != nullptr); // Just in case...

		// The position of the main widget is defined with the same
		// attributes used when the name has already been introduced!
		int tW = iW -> width (); int tH = iW -> height ();
		QGAMES::Vector adj = QGAMES::Vector::_cero;
		if (_properties._reference == QGAMES::ShowingTextGameState::Properties::PosReference::_CENTER)
			adj = QGAMES::Vector (__BD (tW >> 1), __BD (tH >> 1), __BD 0);
		else if (_properties._reference == QGAMES::ShowingTextGameState::Properties::PosReference::_BOTTONRIGHT)
			adj = QGAMES::Vector (__BD tW, __BD tH, __BD 0);
		_guiSystem -> mainWidget () -> setPosition (QGAMES::Position (
				__BD game () -> mainScreen () -> visualWidth () * _properties._position.posX (),
				__BD game () -> mainScreen () -> visualHeight () * _properties._position.posY (),
				__BD 0) - adj); // The position declared under the properties structure is always a percentage...

		observe (_guiSystem -> mainWidget ());

		// Just to know when the intro is pressed to finish the state
		// and then continue with the game...
		observe (game () -> inputHandler ());
	}
}

// ---
void PACMAN::ShowingCurrentPlayerNameGameState::onExit ()
{
	QGAMES::ShowingStandardTextGameState::onExit ();

	if (_guiSystem)
	{
		unObserve (game () -> inputHandler ());

		PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
		assert (g != nullptr); // Just in case...
		QGAMES::CompositeWidget* cW = dynamic_cast <QGAMES::CompositeWidget*> (_guiSystem -> mainWidget ());
		assert (cW != nullptr);

		// Get the name introduced...
		std::string txt = ((QGAMES::IntroWidget*) cW -> widget (__PACMAN_PLAYERNAMEOPTIONID__)) -> text ();
		QGAMES::trim (txt); // Erase the spaces...if any
		g -> setPlayerName ((txt == std::string (__NULL_STRING__)) ? std::string ("AAA") : txt); 
		// Sets the name introduced or a default one if none...

		// ...and finally the GUI system is deleted...
		deleteGUISystem ();
	}
}

// ---
void PACMAN::ShowingCurrentPlayerNameGameState::processEvent (const QGAMES::Event& evnt)
{
	QGAMES::ShowingStandardTextGameState::processEvent (evnt);

	// When the GUI system is active, and the INTRO key is pressed, the state finishes only
	// when the name of the player is not null...
	if (_guiSystem)
	{
		if (evnt.code () == __QGAMES_KEYBOARDEVENT__ &&
			((QGAMES::KeyBoardEventData*) evnt.data ()) -> _internalCode == QGAMES::KeyCode::QGAMES_RETURN &&
			!((QGAMES::KeyBoardEventData*) evnt.data ()) -> _on && // The key has been released!!
			((QGAMES::IntroWidget*) ((QGAMES::CompositeWidget*) _guiSystem -> mainWidget ()) -> 
				widget (__PACMAN_PLAYERNAMEOPTIONID__)) -> text () != std::string (__NULL_STRING__))
			notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
	}
}

// ---
void PACMAN::ShowingCurrentPlayerNameGameState::actualizeText ()
{
	QGAMES::ShowingStandardTextGameState::actualizeText ();

	if (!_guiSystem)
	{
		assert (dynamic_cast <PACMAN::Game*> (game ()) != nullptr);

		std::string str = _textString + std::string (" ") + // An space after...
			((PACMAN::Game*) game ()) -> playerName ();
		_text -> setMaxTextLength (str.length ()); _text -> setText (str);
	}
	else
	{
		_text -> setMaxTextLength (1); // Can't be 0...
		_text -> setText (std::string (__NULL_STRING__));
	}
}

// ---
void PACMAN::ShowingCurrentPlayerNameGameState::deleteGUISystem ()
{
	if (_guiSystem)
		unObserve (_guiSystem -> mainWidget ());

	delete (_guiSystem);
	_guiSystem = nullptr;
}


// ---
void PACMAN::ShowingCurrentLevelGameState::actualizeText ()
{
	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr);

	QGAMES::ShowingTextGameState::actualizeText ();
	std::string str = _textString + std::string (" " /** a separation. */) + std::to_string (g -> level ());
	_text -> setMaxTextLength (str.length ()); _text -> setText (str);
}

// ---
std::multimap <std::string, QGAMES::SetOfOpenValues> PACMAN::ShowingHallOfFameGameState::orderHallOfFame 
	(const std::multimap <std::string, QGAMES::SetOfOpenValues>& hF)
{
	// The result is almost what is being received...
	// But the index of the map has to be the score instead the number of the player...
	std::multimap <int, QGAMES::SetOfOpenValues, std::greater <int>> pR;
	for (std::multimap <std::string, QGAMES::SetOfOpenValues>::const_iterator i = hF.begin ();
			i != hF.end (); i++)
		pR.insert (std::multimap <int, QGAMES::SetOfOpenValues, std::greater <int>>::value_type 
			((*i).second.openValue (0).intValue (), (*i).second)); // The score in the openValu number 0..
	// Because the mulmap is ordered using the score...

	// The result is reasy to translate into...
	int counter = 1;
	std::multimap <std::string, QGAMES::SetOfOpenValues> result;
	for (std::multimap <int, QGAMES::SetOfOpenValues, std::greater <int>>::const_iterator i = pR.begin (); i != pR.end (); i++)
		result.insert (std::multimap <std::string, QGAMES::SetOfOpenValues>::value_type (std::to_string (counter++), (*i).second));

	return (result);
}

// ---
QGAMES::ScoreObjectText* PACMAN::ShowingHallOfFameGameState::createTextFor 
	(const std::string& eK, const QGAMES::SetOfOpenValues& sV)
{
	// The first element is not taken into count ever...
	std::string scr = std::to_string (sV.openValue (0).intValue ());
	if (scr.length () < 5) 
		scr = std::string ("00000").substr (0, 5 - scr.length ()) + scr;
	return (game () -> textBuilder () -> createText 
		(properties ()._font, (scr + std::string ("  ") + sV.openValue (1).strValue ())));
}

// ---
PACMAN::PlayingGameState* PACMAN::PlayingControlGameState::playingState ()
{
	PACMAN::PlayingGameState* result = nullptr;
	for (QGAMES::GameStates::const_iterator i = states ().begin (); i != states ().end () && !result; i++)
		if (dynamic_cast <PACMAN::PlayingGameState*> ((*i).second))
			result = (PACMAN::PlayingGameState*) ((*i).second);
	return (result);
}

// ---
void PACMAN::PlayingControlGameState::onEnter ()
{
	QGAMES::GameStateControl::onEnter ();

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr); // Just in case...
	_pacman = g -> pacman ();
	assert (_pacman != nullptr);
	_pacman -> setOrientation (QGAMES::Vector (__BD 1, __BD 0, __BD 0)); // Looking by default to the right...
	_pacman -> toStand ();

	g -> addScoreObjects ();

	// This instruction sets the world and the scene accoinding to the level defined in DataGame
	g -> setLevel ((_nextLevel == -1) ? g -> level () : _nextLevel);
	_nextLevel = -1; // One used it is set back to the original value...

	// ...and here we increment number of times the player tries this level...
	g -> setTriesOnLevel (g -> level (), g -> triesOnLevel (g -> level ()) + 1);

	g -> setSeconds (0);

	g -> mainScreen () -> setPosition (QGAMES::Position (__BD 32, __BD 32, __BD 0));
}

// ---
void PACMAN::PlayingControlGameState::onExit ()
{
	QGAMES::GameStateControl::onExit ();

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr); // Just in case...

	// Just to maintain the compatibility with the old conf...
	g -> setSceneVisited (g -> activeWorld () -> activeScene () -> id ()); 

	g -> activeWorld () -> finalize ();
	// When the world is fanalized, the scene will be too, and the list of the eaten balls will save also into conf.

	g -> setWorld (__MININT__);

	g -> removeScoreObjects ();

	g -> mainScreen () -> setPosition (QGAMES::Position::_cero);

	_pacman = nullptr;
}

// ---
int PACMAN::PlayingControlGameState::circunstanceWhen (QGAMES::GameState* st, const QGAMES::Event& evnt)
{
	int result = 0;

	if (evnt.code () == __QGAMES_GAMENESTEDSTATEFINISHED__)
	{
		switch (st -> type ())
		{
			case __PACMAN_SHOWINGTHELEVELGAMESTATE__:
				{
					PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
					assert (g != nullptr);

					result = (g -> triesOnLevel (g -> level ()) == 1) ? 0 : 1;
				}

				break;

			case __PACMAN_PLAYINGGAMESTATE__:
				{
					PACMAN::PlayingGameState* gS = dynamic_cast <PACMAN::PlayingGameState*> (st);
					assert (gS != nullptr);

					switch (gS -> lastMainCharacterState ())
					{
						case PACMAN::PlayingGameState::MainCharacterState::_DIED:
							result = 0; // What a pitty!!
							break;

						case PACMAN::PlayingGameState::MainCharacterState::_REACHGOAL:
							result = 1; // All energy balls have been eaten!
							break;

						case PACMAN::PlayingGameState::MainCharacterState::_OPTIONS:
							result = 2; // Asking for options to continue...
							break;
						
						default:
							// No other situation of the character is tken into account so far!
							assert (false);
							break;
					}
				}

				break;

			case __PACMAN_OPTIONSWHENPLAYINGGAMESTATE__:
				{
					PACMAN::OptionsWhenPlayingGameState* gS =
						dynamic_cast <PACMAN::OptionsWhenPlayingGameState*> (st);
					assert (gS != nullptr);

					switch (gS -> lastOptionSelected ())
					{
						case __PACMAN_GUISYSTEMEXITGAMEWDT__:
							result = 1; // To exit has been selected...
							break;

						default:
							result = 0; // Other option has been selected, but the code will necer reach this position!
							break; 
					}
				}

				break;

			default:
				result = 0;
				break; 
		}
	}

	return (result);
}

// ---
void PACMAN::PlayingGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr); // Just in case...
	_pacman = g -> pacman ();
	assert (_pacman != nullptr); // Just in case...

	observe (_pacman);

	_lastMainCharacterState = PACMAN::PlayingGameState::MainCharacterState::_MOVING;

	observe (g -> inputHandler ());
	
	// The _COUNTERSECONDS loop (created at construction time of any AdvacedArcadeGame)
	// will also be used to update the number of seconds the main entity stands in the scene (@see processEvent method in this class)
	// That number of seconds is kept in the configuration object of the game, 
	// and it is set to 0 when that object is instatiated first time or cleaned (@see @adjustPlayers in PACMAN::Game)
	observe (g -> loopCounter (PACMAN::Game::_COUNTERSECONDS));

	// Starts to play the siren...
	PACMAN::World* w = dynamic_cast <PACMAN::World*> (g -> activeWorld ());
	assert (w != nullptr);
	w -> setClapperBoard (true);
	w -> playSiren (true /** forced */);

	// The world is also observed, among other things to detect when it has been empty up!
	observe (w);

	// Collisions are allowed...
	g -> setDetectCollisions (true);
}

// ---
void PACMAN::PlayingGameState::onExit ()
{
	QGAMES::GameState::onExit ();

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr); // Just in case...

	unObserve (g -> loopCounter (PACMAN::Game::_COUNTERSECONDS));

	unObserve (g -> inputHandler ());
	
	unObserve (_pacman);

	_pacman = nullptr;

	// No more siren...
	PACMAN::World* w = dynamic_cast <PACMAN::World*> (g -> activeWorld ());
	assert (w != nullptr);
	w -> setClapperBoard (false);
	w -> stopBackgroundSounds ();

	unObserve (w);

	g -> setDetectCollisions (false);
}

// ---
void PACMAN::PlayingGameState::processEvent (const QGAMES::Event& evnt)
{
	switch (evnt.code ())
	{
		// When the main character says that he is dead!!
		case __PACMAN_PACMANDESTROYED__:
			{
				if (evnt.data () == (void*) _pacman)
				{
					_lastMainCharacterState = PACMAN::PlayingGameState::MainCharacterState::_DIED;
					notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this)); // To finish the state...
				}
			}

			break;

		// When the main character has reached his goal!!
		case __PACMAN_PACMANREACHEDGOAL__:
			{
				if (evnt.data () == (void*) _pacman)
				{
					_lastMainCharacterState = PACMAN::PlayingGameState::MainCharacterState::_REACHGOAL;
					notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this)); // To finish the state...
				}
			}

			break;

		// When a key has been pressed in the input handler...
		// For something to happen, that key has to be either ESC (then the options pannel appears but nothings stops)
		// or the pause / continue key (then the game stops or continue)
		case __QGAMES_KEYBOARDEVENT__:
			{
				QGAMES::AdvancedArcadeGame::Conf* gCfg = 
					dynamic_cast <QGAMES::AdvancedArcadeGame::Conf*> (game () -> configuration ());
				assert (gCfg != nullptr); // Just in case...(to look for the right key later)

				QGAMES::KeyBoardEventData* dt = (QGAMES::KeyBoardEventData*) evnt.data ();
				if (dt -> _internalCode == QGAMES::KeyCode::QGAMES_ESCAPE && !dt -> _on) // ESC for options
				{
					if (_pacman -> isAlive ()) // Only if moving is valid...
					{
						_lastMainCharacterState = PACMAN::PlayingGameState::MainCharacterState::_OPTIONS;

						notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this)); // This state has to finish...
					}
				}
				else
				if (dt -> _internalCode == gCfg -> controlKeys ()[__QGAMES_KEYPAUSE__] && !dt -> _on) // PAUSE to pause
				{
					// Either stops or continue the game...
					if (game () -> isGamePaused ()) 
						game () -> continueGame ();
					else 
						game () -> pauseGame ();
				}
				else
				if (dt -> _internalCode == gCfg -> controlKeys ()[__QGAMES_KEYSAVE__] && !dt -> _on) // Save to save...
				{
					PACMAN::Game::Conf* conf = 
						dynamic_cast <PACMAN::Game::Conf*> (game () -> configuration ());
					assert (conf != nullptr); // It must be a BATTLESHIP
					conf -> save (game () -> parameter (__GAME_DATADIRPROPERTYNAME__) + 
						std::string (__PATH_SEPARATOR__) + QGAMES::StandardInitialSelectionOptionsGameState::_CONFFILENAME);
				}
			}

			break;

		// ---
		// Now a set of events comming from the specialized input event handler...
		// Now the group of events related with how to control the main artist...
		case __PACMAN_MOVINGUPEVENT__:
			_pacman -> changeDirectionWhenPossibleTo (QGAMES::Vector (__BD 0, __BD -1, __BD 0));
			break;

		case __PACMAN_MOVINGRIGHTEVENT__:
			_pacman -> changeDirectionWhenPossibleTo (QGAMES::Vector (__BD 1, __BD 0, __BD 0));
			break;

		case __PACMAN_MOVINGDOWNEVENT__:
			_pacman -> changeDirectionWhenPossibleTo (QGAMES::Vector (__BD 0, __BD 1, __BD 0));
			break;

		case __PACMAN_MOVINGLEFTEVENT__:
			_pacman -> changeDirectionWhenPossibleTo (QGAMES::Vector (__BD -1, __BD 0, __BD 0));
			break;

		// This movements are not allowed in pacman...
		case __PACMAN_MOVINGUPRIGHTEVENT__:
			break;

		case __PACMAN_MOVINGDOWNRIGHTEVENT__:
			break;

		case __PACMAN_MOVINGUPLEFTEVENT__:
			break;

		case __PACMAN_MOVINGDOWNLEFTEVENT__:
			break;

		// Doing nothing with the joystick, o keyboard doesn't mean to stop!
		// Same when pushing fire...
		case __PACMAN_NOMOVINGEVENT__:
			break;

		case __PACMAN_FIREEVENT__:
			break;
		// ---

		// Observing the loop just to add seconds to the time that the player has been playing
		// When this happen, then a notification happens there an the score could be actualized!
		case __QGAMES_LOOPCOUNTEREVENT__:
			{
				PACMAN::Game* dG = dynamic_cast <PACMAN::Game*> (game ());
				assert (dG != nullptr); // Just in case...
				dG -> addToPlayerSeconds (1); // To the current player...
			}

			break;

		default:
			QGAMES::GameState::processEvent (evnt);
			break;
	}
}

// ---
PACMAN::StopPacmanElemntsMovingGameState::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _stopAll (true),
	  _hideMonsters (false),
	  _vanishPacman (false)
{
	auto isYes = [](const std::string& prp) -> bool
		{ std::string txt = prp; QGAMES::toUpper (txt); return (txt == std::string (__YES_STRING__)); };

	if (prps.find (__PACMAN_GAMESTATESTOPPINGELEMENTSATTRSTOPALL__) != prps.end ())
		_stopAll = isYes ((*prps.find (__PACMAN_GAMESTATESTOPPINGELEMENTSATTRSTOPALL__)).second);
	if (prps.find (__PACMAN_GAMESTATESTOPPINGELEMENTSATTRHIDEMONSTERS__) != prps.end ()) 
		_hideMonsters = isYes ((*prps.find (__PACMAN_GAMESTATESTOPPINGELEMENTSATTRHIDEMONSTERS__)).second);
	if (prps.find (__PACMAN_GAMESTATESTOPPINGELEMENTSATTRHIDETHINGS__) != prps.end ()) 
		_hideThings = isYes ((*prps.find (__PACMAN_GAMESTATESTOPPINGELEMENTSATTRHIDETHINGS__)).second);
	if (prps.find (__PACMAN_GAMESTATESTOPPINGELEMENTSATTRVPACMAN__) != prps.end ()) 
		_vanishPacman = isYes ((*prps.find (__PACMAN_GAMESTATESTOPPINGELEMENTSATTRVPACMAN__)).second);

	PACMAN::StopPacmanElemntsMovingGameState::Properties (_stopAll, _hideMonsters, _hideThings, _vanishPacman);
}

// ---
void PACMAN::StopPacmanElemntsMovingGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr); // Just in case...
	_mazeWorld = dynamic_cast <PACMAN::World*> (g -> activeWorld ());
	assert (_mazeWorld != nullptr);
	_pacman = g -> pacman ();
	assert (_pacman != nullptr);

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	_mazeWorld -> setClapperBoard (false);

	if (_properties._stopAll)
		_mazeWorld -> stopAllElements ();
	if (_properties._hideMonsters)
		_mazeWorld -> hideAllMonsters ();
	if (_properties._hideThings)
		_mazeWorld -> hideAllThings ();
}

// ---
void PACMAN::StopPacmanElemntsMovingGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	if (_properties._vanishPacman)
	{
		if (!onOffSwitch (_SWITCHVANISHCOMPLETED) -> isOn () && 
			counter (_COUNTERTOCHANGESTATUS) -> isEnd ())
		{
			_pacman -> setAlphaLevel (counter (_COUNTERVANISHSTATUS) -> value ());

			onOffSwitch (_SWITCHVANISHCOMPLETED) -> set (counter (_COUNTERVANISHSTATUS) -> isEnd ());
		}
	}
}

// ---
void PACMAN::StopPacmanElemntsMovingGameState::onExit ()
{
	QGAMES::GameState::onExit ();

	_mazeWorld = nullptr;

	_pacman -> setAlphaLevel (255);
	_pacman = nullptr;
}

// ---
__IMPLEMENTCOUNTERS__ (PACMAN::StopPacmanElemntsMovingGameState::Counters)
{
	addCounter (new QGAMES::Counter 
		(PACMAN::StopPacmanElemntsMovingGameState::_COUNTERTOCHANGESTATUS, 3, 0, true, true));
	addCounter (new QGAMES::Counter 
		(PACMAN::StopPacmanElemntsMovingGameState::_COUNTERVANISHSTATUS, 
			sizeof (PACMAN::StopPacmanElemntsMovingGameState::_FADE) / sizeof (int), 0, true, false));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (PACMAN::StopPacmanElemntsMovingGameState::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (PACMAN::StopPacmanElemntsMovingGameState::_SWITCHVANISHCOMPLETED, false));
}

// ---
PACMAN::BlinkMazePlayingGameState::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _blinkingTime (__BD 0.25),
	  _numberBlinks (5)
{
	if (prps.find (__PACMAN_GAMESTATEBLINKMAZEATTRBLINKTIME__) != prps.end ()) 
		_blinkingTime = __BD (std::atof ((*prps.find (__PACMAN_GAMESTATEBLINKMAZEATTRBLINKTIME__)).second.c_str ()));
	if (prps.find (__PACMAN_GAMESTATEBLINKMAZEATTRNUMBERBLINKS__) != prps.end ()) 
		_numberBlinks = std::atoi ((*prps.find (__PACMAN_GAMESTATEBLINKMAZEATTRNUMBERBLINKS__)).second.c_str ());

	PACMAN::BlinkMazePlayingGameState::Properties (_blinkingTime, _numberBlinks);
}

// ---
void PACMAN::BlinkMazePlayingGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
	assert (g != nullptr); // Just in case...
	if ((_mazeWorld = dynamic_cast <PACMAN::World*> (g -> activeWorld ())) != nullptr)
		_mazeWorld -> startBlinking  (_properties._blinkingTime, _properties._numberBlinks); // Start blinking...
	else
		notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this)); // The state has to finish if there is no a tiles map...
}

// ---
void PACMAN::BlinkMazePlayingGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	if (!_mazeWorld -> isBlinking ())
		notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
}

// ---
void PACMAN::BlinkMazePlayingGameState::onExit ()
{
	QGAMES::GameState::onExit ();

	if (_mazeWorld != nullptr)
		_mazeWorld ->  stopBlinking ();
	_mazeWorld = nullptr;
}

// ---
void PACMAN::OptionsWhenPlayingGameState::onEnter ()
{
	QGAMES::SelectionOptionsGameState::onEnter ();

	observe (game () -> inputHandler ());
}

// ---
void PACMAN::OptionsWhenPlayingGameState::onExit ()
{
	unObserve (game () -> inputHandler ());

	QGAMES::SelectionOptionsGameState::onExit ();
}

// ---
void PACMAN::OptionsWhenPlayingGameState::processEvent (const QGAMES::Event& evnt)
{
	QGAMES::SelectionOptionsGameState::processEvent (evnt);

	// If the ESC key is press, the the state finishes...
	if (evnt.code () == __QGAMES_KEYBOARDEVENT__)
	{
		QGAMES::KeyBoardEventData* dt = (QGAMES::KeyBoardEventData*) evnt.data ();
		if (dt -> _internalCode == QGAMES::KeyCode::QGAMES_ESCAPE && !dt -> _on)
			onOffSwitch (_SWITCHFINISH) -> set (true);
	}
}

// ---
QGAMES::SelectionOptionsGameState::Menus PACMAN::OptionsWhenPlayingGameState::createMenus ()
{
	QGAMES::SelectionOptionsGameState::Menus result;
	result.insert (QGAMES::SelectionOptionsGameState::Menus::value_type 
		(__PACMAN_OPTIONWHENPLAYINGGUISYSTEM__, QGAMES::SelectionOptionsGameState::Menu 
			(__PACMAN_OPTIONWHENPLAYINGGUISYSTEM__, QGAMES::Position (__BD 10, __BD 10, __BD 0))));
	return (result);
}

// ---
int PACMAN::OptionsWhenPlayingGameState::SelectionHandler::whenWidgetSelected (QGAMES::Widget* wdt)
{
	PACMAN::Game::Conf* conf = 
		dynamic_cast <PACMAN::Game::Conf*> (owner () -> game () -> configuration ());
	assert (conf != nullptr); // It must be a BATTLESHIP

	int result = 0; // To continue...

	switch (wdt -> id ())
	{
		case __PACMAN_GUISYSTEMEXITGAMEWDT__:
		case __PACMAN_GUISYSTEMCONTINUEGAMEWDT__:
			result = wdt -> id (); // Time to exit the menu...with the option selected.
			break;

		case __PACMAN_GUISYSTEMSAVEGAMEWDT__:
			conf -> save (owner () -> game () -> parameter (__GAME_DATADIRPROPERTYNAME__) + 
				std::string (__PATH_SEPARATOR__) + QGAMES::StandardInitialSelectionOptionsGameState::_CONFFILENAME);
			// ...but it continues in the menu...
			owner () -> game () -> sound (__QGAMES_SAVEWAVSOUND__) -> play (__QGAMES_GAMESTATESOUNDCHANNEL__);
			break;

		default:
			assert (false); // It shouldn't be here...It means there is options not controlled!!
			break;
	}

	return (result);
}

// ---
int PACMAN::GameStateControl::circunstanceWhen (QGAMES::GameState* st, const QGAMES::Event& evnt)
{
	int result = 0;

	if (evnt.code () == __QGAMES_GAMENESTEDSTATEFINISHED__)
	{
		PACMAN::Game* g = dynamic_cast <PACMAN::Game*> (game ());
		assert (g != nullptr);

		switch (st -> type ())
		{
			// When the state has finishes because an options has been selected...
			case __PACMAN_MENUGAMESTATE__:
				{
					QGAMES::StandardInitialSelectionOptionsGameState* gSS =
						dynamic_cast <QGAMES::StandardInitialSelectionOptionsGameState*> (st);
					assert (gSS != nullptr); // Just in case there is mistake at some point of the code not totally controlled...
					switch (gSS -> lastOptionSelected ())
					{
						case __QGAMES_STDGUISYSTEMSTARTGAMEWDT__: 
							result = 0; // Start the game...
							break;

						case __QGAMES_STDGUISYSTEMEXITWDT__: 
							result = 1; // Exit the game...
							break;

						case __QGAMES_STDGUISYSTEMHALLOFFAMEWDT__: 
							result = 2; // Hall of fame...
							break;

						default:
							assert (false); // It shouldn't be here...It means there is an option not well managed!!
							break;
					}
				}

				break;

			// When the playing control finishes...
			case __PACMAN_CONTROLPLAYINGGAMESTATE__:
				{
					// The playing has finished, but depending on its final state
					// What to do will be different...
					PACMAN::PlayingControlGameState* cPS = 
						dynamic_cast <PACMAN::PlayingControlGameState*> (st);
					assert (cPS != nullptr); // Just in case...it shouldn't happen but who knows!
					PACMAN::PlayingGameState* pS = cPS -> playingState ();
					assert (pS != nullptr); // Same!...

					switch (pS -> lastMainCharacterState ())
					{
						case PACMAN::PlayingGameState::MainCharacterState::_DIED:
							result = 0; // Situation when the pacman dies...
							break;

						case PACMAN::PlayingGameState::MainCharacterState::_REACHGOAL:
							{
								g -> setScore (g -> pacman () -> score ());
								g -> setFruitsEaten (g -> pacman () -> fruitsEaten ());

								// When the pacman reaches the goal, either the game finishes when the level is the last possible (4)
								// or stops (a pacman is threaten by a monster) for a while if the level requieres it, or
								// just moves to the next level possible...
								int nInterScr = -1;
								result = ((nInterScr = g -> intermissionScreenAfterCurrentLevel ()) != -1) 
									? (g -> isCurrentLevelLast () 
										? 4 // Meaning to finish!!
										: intermissionCircunstanceFor (nInterScr)) // By default always 3...
									: 1; // Meaning to continue with the nextlevel without stopping...

								// Keeps that information in the playing control entity
								// to be used once it is initialized!
								cPS -> setNextLevel (g -> level () + 1);
							}
							break;

						case PACMAN::PlayingGameState::MainCharacterState::_OPTIONS:
							result = 2; // The main character is asking for options to continue...
							break;

						default:
							assert (false); // It shouldn't be here. It means there is an state not weel managed!!
							break;
					}
				}

				break;

			// When the state showing that player has died finishes, it needed to take a decision...
			case __PACMAN_PLAYERDIEDGAMESTATE__:
				{
					// If the player is the last and there is no more player to play...
					// Then the game finishes...
					int cPlayer = g -> currentPlayer ();
					g -> setLives (g -> lives () - 1);
					do g -> setNextPlayer (); 
					while (g -> lives () == 0 && cPlayer != g ->  currentPlayer ());

					result = (g -> lives () == 0 && cPlayer == g -> currentPlayer ()) ? 1 : 0;
					// 1 finishes the game, 0 starts back again with a new player...
				}

				break;

			// When the state of "game over" finished...
			// It is time to actualize the hall of fame!
			// The playing state will have finished and also the ninja and then the score will have been actualized!
			case __PACMAN_PLAYERGAMEOVERGAMESTATE__:
				{
					g -> actualizeHallOfFame ();

					result = 0;
				}

				break;

			default:
				// Standard result is enough...
				break;
		}
	}

	return (result);
}
