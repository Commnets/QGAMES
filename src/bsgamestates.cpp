#include <BattleshipLike/bsgamestates.hpp>
#include <BattleshipLike/bsgame.hpp>
#include <BattleshipLike/bsworlds.hpp>
#include <BattleshipLike/bsscenes.hpp>
#include <BattleshipLike/bsinputhandler.hpp>

// ---
void BATTLESHIP::InitializeBattleshipGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	BATTLESHIP::Game* aG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (aG); // Just in case...

	aG -> initializeEntitiesAndWorlds ();
}

// ---
void BATTLESHIP::InitializeBattleshipGameState::updatePositions ()
{
	notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
}

// ---
BATTLESHIP::BattleshipMovingGameState::~BattleshipMovingGameState ()
{
	((BATTLESHIP::Battleship*) game () -> entity (__BATTLESHIP_BATTLESHIPTYPE1ID__)) -> 
		addControlStepsMonitor (NULL); // Just in case...
}

// ---
void BATTLESHIP::BattleshipMovingGameState::onEnter ()
{
	QGAMES::ShowingEntityGameState::onEnter ();

	BATTLESHIP::Battleship* bS = 
		(BATTLESHIP::Battleship*) game () -> entity (__BATTLESHIP_BATTLESHIPTYPE1ID__);
	bS -> setPosition (QGAMES::Position (
		__BD (game () -> mainScreen () -> visualWidth () * 0.65),
		__BD (game () -> mainScreen () -> visualHeight () * 0.5), 
		__BD 0));
	bS -> setVisible (true);

	_currentMonitor = NULL;
	_loadNextMonitor = true;
}

// ---
void BATTLESHIP::BattleshipMovingGameState::updatePositions ()
{
	QGAMES::ShowingEntityGameState::updatePositions ();

	if (_loadNextMonitor)
	{
		_loadNextMonitor = false; // Done

		if (_currentMonitor)
			unObserve (_currentMonitor);

		BATTLESHIP::Battleship* bS = 
			(BATTLESHIP::Battleship*) game () -> entity (__BATTLESHIP_BATTLESHIPTYPE1ID__);
		if ((_currentMonitor = game () -> characterMonitorBuilder () -> monitorFor 
			(__BATTLESHIP_PRESENTATIONMONITORBASEID__ + (rand () % __BATTLESHIP_PRESENTATIONMONITORNUMBERMOVS__), bS)) != NULL)
		{
			bS -> addControlStepsMonitor (_currentMonitor); // Removes the previous control...
			observe (_currentMonitor);
		}
	}
}

// ---
void BATTLESHIP::BattleshipMovingGameState::onExit ()
{
	QGAMES::ShowingEntityGameState::onExit ();

	if (_currentMonitor)
		unObserve (_currentMonitor);

	BATTLESHIP::Battleship* bS = 
		(BATTLESHIP::Battleship*) game () -> entity (__BATTLESHIP_BATTLESHIPTYPE1ID__);
	bS -> addControlStepsMonitor (NULL); // Removes the control...
	bS -> setVisible (false);

	_currentMonitor = NULL;
}

// ---
void BATTLESHIP::BattleshipMovingGameState::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __QGAMES_CHRSTEPSMONITORLASTSTEP__)
		_loadNextMonitor = true;
	else
		QGAMES::GameState::processEvent (evnt);
}

// ---
void BATTLESHIP::ShowingCurrentPlayerNameGameState::onEnter ()
{
	QGAMES::ShowingStandardTextGameState::onEnter ();

	// If there is no player name already introduced, then 
	// a GUI system is started to ask for it...
	BATTLESHIP::Game* g = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (g); // Just in case...
	if (g -> playerName () == std::string (__NULL_STRING__))
	{
		_guiSystem = game () -> guiSystemBuilder () -> system (__BATTLESHIP_PLAYERNAMEGUISYSTEM__);
		QGAMES::CompositeWidget* cW = dynamic_cast <QGAMES::CompositeWidget*> (_guiSystem -> mainWidget ());
		assert (cW); // Just in case...
		QGAMES::IntroWidget* iW = 
			dynamic_cast <QGAMES::IntroWidget*> (cW -> widget (__BATTLESHIP_PLAYERNAMEOPTIONID__));
		assert (iW); // Just in case...
		QGAMES::IteratingLabelWidget* aW = 
			dynamic_cast <QGAMES::IteratingLabelWidget*> (cW -> widget (__BATTLESHIP_GUISYSTEMTYPEARTISTWDT__));
		assert (aW); // Just in case...

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

		// Sets the default type of artist to play...
		aW -> setCurrentValue (g -> artistType ());

		observe (_guiSystem -> mainWidget ());

		// Just to know when the intro is pressed to finish the state
		// and then continue with the game...
		observe (game () -> inputHandler ());
	}
}

// ---
void BATTLESHIP::ShowingCurrentPlayerNameGameState::onExit ()
{
	QGAMES::ShowingStandardTextGameState::onExit ();

	if (_guiSystem)
	{
		unObserve (game () -> inputHandler ());

		BATTLESHIP::Game* g = dynamic_cast <BATTLESHIP::Game*> (game ());
		assert (g); // Just in case...
		QGAMES::CompositeWidget* cW = dynamic_cast <QGAMES::CompositeWidget*> (_guiSystem -> mainWidget ());
		assert (cW);

		// Get the name introduced...
		std::string txt = ((QGAMES::IntroWidget*) cW -> widget (__BATTLESHIP_PLAYERNAMEOPTIONID__)) -> text ();
		QGAMES::trim (txt); // Erase the spaces...if any
		g -> setPlayerName ((txt == std::string (__NULL_STRING__)) ? std::string ("AAA") : txt); 
		// Sets the name introduced or a default one if none...

		// Get the type of artist selected...
		g -> setArtistType (((QGAMES::IteratingLabelWidget*) 
			cW -> widget (__BATTLESHIP_GUISYSTEMTYPEARTISTWDT__)) -> currentValue ());

		// ...and finally the GUI system is deleted...
		deleteGUISystem ();
	}
}

// ---
void BATTLESHIP::ShowingCurrentPlayerNameGameState::processEvent (const QGAMES::Event& evnt)
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
				widget (__BATTLESHIP_PLAYERNAMEOPTIONID__)) -> text () != std::string (__NULL_STRING__))
			notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
	}
}

// ---
void BATTLESHIP::ShowingCurrentPlayerNameGameState::actualizeText ()
{
	QGAMES::ShowingStandardTextGameState::actualizeText ();

	if (!_guiSystem)
	{
		assert (dynamic_cast <BATTLESHIP::Game*> (game ()));
		// It has to be an DRAGONWIND game to access to the number of the player...

		std::string str = _textString + std::string (" ") + // An space after...
			((BATTLESHIP::Game*) game ()) -> playerName ();
		_text -> setMaxTextLength (str.length ()); _text -> setText (str);
	}
	else
	{
		_text -> setMaxTextLength (1); // Can't be 0...
		_text -> setText (std::string (__NULL_STRING__));
	}
}

// ---
void BATTLESHIP::ShowingCurrentPlayerNameGameState::deleteGUISystem ()
{
	if (_guiSystem)
		unObserve (_guiSystem -> mainWidget ());

	delete (_guiSystem);
	_guiSystem = NULL;
}

// ---
void BATTLESHIP::ShowingCurrentWorldNameGameState::actualizeText ()
{
	assert (dynamic_cast <BATTLESHIP::World*> ((__AGM game ()) -> activeWorld ()));
	// Just in case...It has to be a BATTLESHIP::World

	QGAMES::ShowingTextGameState::actualizeText ();
	std::string str = _textString + std::string (" ") + // A space after...
		((BATTLESHIP::World*) (__AGM game ()) -> activeWorld ()) -> name ();
	_text -> setMaxTextLength (str.length ()); _text -> setText (str);
}

// ---
void BATTLESHIP::ShowingCurrentSceneNameGameState::actualizeText ()
{
	assert (dynamic_cast <BATTLESHIP::Scene*> ((__AGM game ()) -> activeWorld () -> activeScene ()));
	// Jus in case...It has to be a BATTLESHIP::Scene

	QGAMES::ShowingTextGameState::actualizeText ();
	std::string str = _textString + std::string (" ") + // A space after...
		((BATTLESHIP::Scene*) (__AGM game ()) -> activeWorld () -> activeScene ()) -> name ();
	_text -> setMaxTextLength (str.length ()); _text -> setText (str);
}

// ---
BATTLESHIP::CreditTitlesGameState::CreditTitlesGameState 
		(int id, int nT, const std::string& cT, 
		 const QGAMES::CreditTitlesGameState::Properties& prps, QGAMES::GameState* nS)
	: QGAMES::CreditTitlesGameState (id, prps, nS),
		_numberTitle (nT),
		_contentTitle (cT),
		_logoGameState (NULL)
{
	// To be able to configure the logo behind...
	_logoGameState = dynamic_cast <QGAMES::ShowingFixFormGameState*> (nestedState ());
	// It could be null, meaning that the state hasn't been well defined at all...
}

// ---
void BATTLESHIP::CreditTitlesGameState::drawOn (QGAMES::Screen* s)
{
	if (_logoGameState != NULL)
	{
		QGAMES::ShowingFixFormGameState::Properties prps = _logoGameState -> properties ();
		prps._fade = (_titleToPaint == _numberTitle) ? _titleStatus [_titleToPaint] -> _fadeGrade : 0;
		_logoGameState -> setProperties (prps); // Change the fade according with the title shown...
	}

	QGAMES::CreditTitlesGameState::drawOn (s);
}

// ---
QGAMES::ScoreObjectText* BATTLESHIP::CreditTitlesGameState::createText (const std::string& txt)
{
	QGAMES::ScoreObjectText* result = NULL;

	bool last = (txt == properties ()._titles [properties ()._titles.size () - 1]);
	if (txt.find (_contentTitle) != txt.npos) // It the text to show is the one defined as the main title... 
		result = game () -> textBuilder () -> createText
			(game () -> textBuilder () -> nextSizeTo ( // Even bigger...
				(game () -> textBuilder () -> nextSizeTo // Bigger and green...
					(game () -> textBuilder () -> inColor (properties ()._font, __QGAMES_GREENCOLOR__)))), txt);
	else 
	if (last) // The last text is show in a green always...
		result = game () -> textBuilder () -> createText 
			(game () -> textBuilder () -> inColor (properties ()._font, __QGAMES_GREENCOLOR__), txt);
	else 
		result = QGAMES::CreditTitlesGameState::createText (txt);

	return (result);
}

// ---
std::multimap <std::string, QGAMES::SetOfOpenValues> BATTLESHIP::ShowingHallOfFameGameState::orderHallOfFame 
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
	std::multimap <std::string, QGAMES::SetOfOpenValues> result;
	for (std::multimap <int, QGAMES::SetOfOpenValues, std::greater <int>>::const_iterator i = pR.begin (); i != pR.end (); i++)
		result.insert (std::multimap <std::string, QGAMES::SetOfOpenValues>::value_type (std::to_string ((*i).first), (*i).second));

	return (result);
}

// ---
QGAMES::ScoreObjectText* BATTLESHIP::ShowingHallOfFameGameState::createTextFor 
	(const std::string& eK, const QGAMES::SetOfOpenValues& sV)
{
	std::string scr = std::to_string (sV.openValue (0).intValue ());
	if (scr.length () < 5) 
		scr = std::string ("00000").substr (0, 5 - scr.length ()) + scr;
	return (game () -> textBuilder () -> createText 
		(properties ()._font, (scr + std::string ("  ") + sV.openValue (1).strValue ())));
}

// ---
void BATTLESHIP::SelectionGameState::initializeGUISystem ()
{
	QGAMES::StandardInitialSelectionOptionsGameState::initializeGUISystem ();

	QGAMES::CompositeWidget* mWdt = 
		dynamic_cast <QGAMES::CompositeWidget*> (currentGUISystem () -> mainWidget ());
	// It is not needed to assert it as it has been already done at parent level
	if (mWdt -> id () == __QGAMES_STDGUISYSTEMMAINWIDGET__)
	{
		BATTLESHIP::Game::Conf* cfg = dynamic_cast <BATTLESHIP::Game::Conf*> (_configuration);
		assert (cfg); // Just in case...
		((QGAMES::IteratingLabelWidget*) // The same type of artist for all of them...
			(mWdt -> widget (__BATTLESHIP_GUISYSTEMTYPEARTISTWDT__))) -> setCurrentValue (cfg -> artistType (1));
	}
	else
	if (mWdt -> id () == __QGAMES_STDGUISYSTEMMAINCONFWIDGET__)
	{
		std::vector <int> controlKeys = _configuration -> controlKeys ();
		// It is not needed to assert the size at it has already been done at parent level...
		((QGAMES::IteratingLabelWidget*) 
			(mWdt -> widget (__BATTLESHIP_GUISYSTEMCFGKEYITERATEWDT__))) -> 
				setCurrentValue (_keyCodesConverterInv [(QGAMES::KeyCode) controlKeys [__BATTLESHIP_KEYITERATE__]]);
		((QGAMES::IteratingLabelWidget*) 
			(mWdt -> widget (__BATTLESHIP_GUISYSTEMCFGKEYCATCHLEAVEWDT__))) -> 
				setCurrentValue (_keyCodesConverterInv [(QGAMES::KeyCode) controlKeys [__BATTLESHIP_KEYCATCHLEAVE__]]);
		((QGAMES::IteratingLabelWidget*) 
			(mWdt -> widget (__BATTLESHIP_GUISYSTEMCFGKEYJUMPWDT__))) -> 
				setCurrentValue (_keyCodesConverterInv [(QGAMES::KeyCode) controlKeys [__BATTLESHIP_KEYJUMP__]]);
	}
}

// ---
void BATTLESHIP::SelectionGameState::finalizeGUISystem ()
{
	QGAMES::StandardInitialSelectionOptionsGameState::finalizeGUISystem ();

	QGAMES::CompositeWidget* mWdt = 
		dynamic_cast <QGAMES::CompositeWidget*> (currentGUISystem () -> mainWidget ());
	// It is not needed to assert it as it has been already done at parent level
	if (mWdt -> id () == __QGAMES_STDGUISYSTEMMAINWIDGET__)
	{
		BATTLESHIP::Game::Conf* cfg = dynamic_cast <BATTLESHIP::Game::Conf*> (_configuration);
		assert (cfg); // Just in case...
		for (int i = 1; i <= cfg -> numberPlayers (); i++)
			cfg -> setArtistType (i,
				((QGAMES::IteratingLabelWidget*) // All of them are set as male or female...
					(mWdt -> widget (__BATTLESHIP_GUISYSTEMTYPEARTISTWDT__))) -> currentValue ());
	}
	else
	if (mWdt -> id () == __QGAMES_STDGUISYSTEMMAINCONFWIDGET__)
	{
		std::vector <int> controlKeys (9);
		controlKeys = _configuration -> controlKeys ();
		controlKeys [__BATTLESHIP_KEYITERATE__] = 
			_keycodesConverter [((QGAMES::IteratingLabelWidget*) 
				(mWdt -> widget (__BATTLESHIP_GUISYSTEMCFGKEYITERATEWDT__))) -> currentValue ()];
		controlKeys [__BATTLESHIP_KEYCATCHLEAVE__] = 
			_keycodesConverter [((QGAMES::IteratingLabelWidget*) 
				(mWdt -> widget (__BATTLESHIP_GUISYSTEMCFGKEYCATCHLEAVEWDT__))) -> currentValue ()];
		controlKeys [__BATTLESHIP_KEYJUMP__] = 
			_keycodesConverter [((QGAMES::IteratingLabelWidget*) 
				(mWdt -> widget (__BATTLESHIP_GUISYSTEMCFGKEYJUMPWDT__))) -> currentValue ()];
		_configuration -> setControlKeys (controlKeys);
	}
}

// ---
int BATTLESHIP::SelectionGameState::SelectionHandler::whenWidgetSelected (QGAMES::Widget* wdt)
{
	int result = QGAMES::StandardInitialSelectionOptionsGameState::SelectionHandler::whenWidgetSelected (wdt);
	if (result != -1) // Something has happened already?
		return (result);

	switch (wdt -> id ())
	{
		case __BATTLESHIP_GUISYSTEMINSTRUCTIONSWDT__:
			result = wdt -> id (); // Time to exit the menu, looking for the instructions...
			break;

		// In these cases lets analize whether there is any key duplicated
		// If it is the case, then an information message should be shown!
		case __BATTLESHIP_GUISYSTEMCFGKEYITERATEWDT__:
		case __BATTLESHIP_GUISYSTEMCFGKEYCATCHLEAVEWDT__:
		case __BATTLESHIP_GUISYSTEMCFGKEYJUMPWDT__:
			{
				if (owner () -> informationText () != std::string (__NULL_STRING__))
				{
					QGAMES::CompositeWidget* mWdt = 
						dynamic_cast <QGAMES::CompositeWidget*> (owner () -> currentGUISystem () -> mainWidget ());
					assert (mWdt); // It has to be...
					std::vector <int> nowKeys (3);
					nowKeys [__BATTLESHIP_KEYITERATE__ - 7] = ((QGAMES::IteratingLabelWidget*) 
							(mWdt -> widget (__BATTLESHIP_GUISYSTEMCFGKEYITERATEWDT__))) -> currentValue ();
					nowKeys [__BATTLESHIP_KEYCATCHLEAVE__ - 7] = ((QGAMES::IteratingLabelWidget*) 
							(mWdt -> widget (__BATTLESHIP_GUISYSTEMCFGKEYCATCHLEAVEWDT__))) -> currentValue ();
					nowKeys [__BATTLESHIP_KEYJUMP__ - 7] = ((QGAMES::IteratingLabelWidget*) 
							(mWdt -> widget (__BATTLESHIP_GUISYSTEMCFGKEYJUMPWDT__))) -> currentValue ();

					int nE = nowKeys.size ();
					auto end = nowKeys.end ();
					for (auto it = nowKeys.begin (); it != end; ++it)
						end = std::remove (it + 1, end, *it);
					nowKeys.erase(end, nowKeys.end ());
					owner () -> showInformationText 
						(std::string ((nowKeys.size () != nE) ? "Take care! Keys repeated" : " "));
				}
			}

			break;

		default:
			break; // When the rest of the widgets are selected what the standard behaviour does is more than enough
	}

	return (result);
}

// ---
BATTLESHIP::PlayingGameState* BATTLESHIP::PlayingControlGameState::playingState ()
{
	BATTLESHIP::PlayingGameState* result = NULL;
	for (QGAMES::GameStates::const_iterator i = states ().begin (); i != states ().end () && !result; i++)
		if (dynamic_cast <BATTLESHIP::PlayingGameState*> ((*i).second))
			result = (BATTLESHIP::PlayingGameState*) ((*i).second);
	return (result);
}

// ---
void BATTLESHIP::PlayingControlGameState::onEnter ()
{
	QGAMES::GameStateControl::onEnter ();

	BATTLESHIP::Game* dG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (dG); // Just in case...
	BATTLESHIP::Game::Conf* dGConf = dynamic_cast <BATTLESHIP::Game::Conf*> (dG -> configuration ());
	assert (dGConf); // Just in case...

	dG -> addScoreObjects ();

	_battleship = dG -> battleship (); 
	assert (_battleship); // Just in case...

	int nCS = dG -> currentScene ();
	dG -> setWorld (dG -> currentWorld ());
	dG -> activeWorld () -> setScene (nCS);
	BATTLESHIP::Scene* dScn = dynamic_cast <BATTLESHIP::Scene*> (dG -> activeWorld () -> activeScene ());
	assert (dScn); // Just in case, only BATTLESHIP::Scenes are allowed...
	dG -> activeWorld () -> initialize ();

	// The position of the main artist will depend on whether is finishes last time...
	QGAMES::Screen* scr = dG -> mainScreen ();
	QGAMES::Position lPP = dG -> lastPlayerPosition ();
	if (lPP == QGAMES::Position::_cero) // No last position was set...
		_battleship -> setPosition (QGAMES::Position 
			(__BD ((scr -> visualWidth () >> 1) - (_battleship -> visualLength () >> 1)), // In the middle (x axis)
			 __BD (scr -> visualHeight () - (__BD 1.5 * __BD _battleship -> visualHeight ())), // Close to the bottom (y axis) 
			 __BD 0)); 
	else // Last position was set...
		_battleship -> setPosition (lPP);

	// Any case, the battleship was born with some seconds sealed!
	_battleship -> toSeal (__BD 3); // 2 seconds to be more specific...

	// Fix the visualization over the main character
	QGAMES::ToFixScreenOnArtistInSceneWithBackground* tFS =
		new QGAMES::ToFixScreenOnArtistInSceneWithBackground (scr, _battleship, __BD 0.2, __BD 0.2, __BD 0);
	scr -> setFix (tFS);

	// It will depend on the attributes of the current screen...
	QGAMES::Vector dScnM = dScn -> movement ();
	tFS -> setConsiderXAxis (dScnM.posX () != __BD 0 ? true : false); 
	tFS -> setConsiderYAxis (dScnM.posY () != __BD 0 ? true : false);
	tFS -> setConsiderZAxis (dScnM.posZ () != __BD 0 ? true : false);
}

// ---
void BATTLESHIP::PlayingControlGameState::onExit ()
{
	QGAMES::GameStateControl::onExit ();

	BATTLESHIP::Game* dG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (dG); // Just in case...

	dG -> setSceneVisited (dG -> activeWorld () -> activeScene () -> id ()); // The scene has already been visited...

	dG -> activeWorld () -> finalize ();
	dG -> setWorld (__MININT__); // No world any more...

	dG -> mainScreen () -> setFix (NULL); // To follow none anymore...
	dG -> mainScreen () -> setPosition (QGAMES::Position::_cero); // At the beginning...

	dG -> removeScoreObjects ();

	_battleship = NULL;
}

// ---
int BATTLESHIP::PlayingControlGameState::circunstanceWhen (QGAMES::GameState* st, const QGAMES::Event& evnt)
{
	int result = 0;

	if (evnt.code () == __QGAMES_GAMENESTEDSTATEFINISHED__)
	{
		switch (st -> type ())
		{
			case __BATTLESHIP_BEFOREPLAYINGGAMESTATE__:
				{
					BATTLESHIP::Game* dG = dynamic_cast <BATTLESHIP::Game*> (game ());
					assert (dG); // Just in case...it should be
					BATTLESHIP::Scene* cS = 
						dynamic_cast <BATTLESHIP::Scene*> (dG -> activeWorld () -> activeScene ());
					assert (cS); // Just in case...it should be but depends on the implementation.

					// Visted already? ...give control directly
					result = dG -> sceneVisited (cS -> id ()) ? 1 : 0; 
				}
				
				break;

			case __BATTLESHIP_PLAYINGGAMESTATE__:
				{
					BATTLESHIP::PlayingGameState* gS = dynamic_cast <BATTLESHIP::PlayingGameState*> (st);
					assert (gS);

					switch (gS -> lastMainCharacterState ())
					{
						case BATTLESHIP::PlayingGameState::MainCharacterState::_REACHGOAL:
							{	
								BATTLESHIP::Game* dG = dynamic_cast <BATTLESHIP::Game*> (game ());
								assert (dG); // Just in case...it should be
								BATTLESHIP::Scene* cS = 
									dynamic_cast <BATTLESHIP::Scene*> (dG -> activeWorld () -> activeScene ());
								assert (cS); // Just in case...

								// Not completed before?...then celebration, otherwise moves to the next asap!
								result = dG -> sceneCompleted (cS -> id ()) ? 3 : 1;
							}

							break;

						case BATTLESHIP::PlayingGameState::MainCharacterState::_OPTIONS:
							result = 2; // Asking for options to continue...
							break;

						default:
							break; // In the rest of the cases, the default option is enough...
					}
				}

				break;

			case __BATTLESHIP_OPTIONSWHENPLAYINGGAMESTATE__:
				{
					BATTLESHIP::OptionsWhenPlayingGameState* gS =
						dynamic_cast <BATTLESHIP::OptionsWhenPlayingGameState*> (st);
					assert (gS);

					switch (gS -> lastOptionSelected ())
					{
						case __BATTLESHIP_GUISYSTEMEXITGAMEWDT__:
							result = 1; // To exit...
							break;

						default:
							break; // In the rest of the cases, the default option is enough...
					}
				}

				break;

			default:
				break; // Standard result is enough...
		}
	}

	return (result);
}

// ---
void BATTLESHIP::PlayingGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	BATTLESHIP::Game* dG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (dG); // Just in case...
	_battleship = dG -> battleship ();
	assert (_battleship); // Just in case...

	observe (_battleship);
	_battleship -> setLosingEnergy (true); // it has to...unless other thing is said!!

	_lastMainCharacterState = BATTLESHIP::PlayingGameState::MainCharacterState::_MOVING;

	QGAMES::InputHandler* iH = game () -> inputHandler ();
	if (!iH)
		return;
	observe (iH);

	// Now the collisions can be detected...
	dG -> setDetectCollisions (true);
	
	// The _COUNTERSECONDS loop (created at construction time of any AdvacedArcadeGame)
	// will also be used to update the number of seconds the main entity stands in the scene (@see processEvent method in this class)
	// That number of seconds is kept in the configuration object of the game, 
	// and it is set to 0 when that object is instatiated first time or cleaned (@see @adjustPlayers in BATTLESHIP::Game)
	observe (dG -> loopCounter (BATTLESHIP::Game::_COUNTERSECONDS));
}

// ---
void BATTLESHIP::PlayingGameState::onExit ()
{
	QGAMES::GameState::onExit ();

	BATTLESHIP::Game* dG = dynamic_cast <BATTLESHIP::Game*> (game ());
	assert (dG); // Just in case...
	dG -> setDetectCollisions (false);

	unObserve (dG -> loopCounter (BATTLESHIP::Game::_COUNTERSECONDS));

	QGAMES::InputHandler* iH = game () -> inputHandler ();
	if (!iH)
		return;
	unObserve (iH);

	_battleship -> setLosingEnergy (false); // Just in case...
	unObserve (_battleship);

	_battleship = NULL;
}

// ---
void BATTLESHIP::PlayingGameState::processEvent (const QGAMES::Event& evnt)
{
	switch (evnt.code ())
	{
		// When the main character says that he is dead!!
		case __BATTLESHIP_SPACEELMNTHASDESTROYED__:
			{
				if (evnt.data () == (void*) _battleship)
				{
					_lastMainCharacterState = BATTLESHIP::PlayingGameState::MainCharacterState::_DIED;
					notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this)); // To finish the state...
				}
			}

			break;

		// When the main character has reached his goal!!
		case __BATTLESHIP_SPACESHIPREACHEDGOAL__:
			{
				if (evnt.data () == (void*) _battleship)
				{
					_lastMainCharacterState = BATTLESHIP::PlayingGameState::MainCharacterState::_REACHGOAL;
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
				assert (gCfg); // Just in case...(to look for the right key later)

				QGAMES::KeyBoardEventData* dt = (QGAMES::KeyBoardEventData*) evnt.data ();
				if (dt -> _internalCode == QGAMES::KeyCode::QGAMES_ESCAPE && !dt -> _on) // ESC for options
				{
					if (_battleship -> isAlive ()) // Only if moving is valid...
					{
						_lastMainCharacterState = BATTLESHIP::PlayingGameState::MainCharacterState::_OPTIONS;
						notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this)); // This state has to finish...
					}
				}
				else
				if (dt -> _internalCode == gCfg -> controlKeys ()[__QGAMES_KEYPAUSE__] && !dt -> _on) // PAUSE to pause
				{
					// Stops or continue the game...
					if (game () -> isGamePaused ()) game () -> continueGame ();
					else game () -> pauseGame ();
					// ...and the main artist stops or continue losing energy in consequence...
					_battleship -> setLosingEnergy (!_battleship -> losingEnergy ());
				}
				else
				if (dt -> _internalCode == gCfg -> controlKeys ()[__QGAMES_KEYSAVE__] && !dt -> _on) // Save to save!
				{
					BATTLESHIP::Game::Conf* conf = 
						dynamic_cast <BATTLESHIP::Game::Conf*> (game () -> configuration ());
					assert (conf); // It must be a BATTLESHIP
					conf -> save (game () -> parameter (__GAME_DATADIRPROPERTYNAME__) + 
						std::string (__PATH_SEPARATOR__) + QGAMES::StandardInitialSelectionOptionsGameState::_CONFFILENAME);
				}
			}

			break;

		// ---
		// Now a set of events comming from the specialized input event handler...
		// Now the group of events related with how to control the main artist...
		case __BATTLESHIP_MOVINGUPEVENT__:
			_battleship -> toMove (0, QGAMES::Vector (__BD 0, __BD -1, __BD 0));
			break;

		case __BATTLESHIP_MOVINGRIGHTEVENT__:
			_battleship -> toMove (0, QGAMES::Vector (__BD 1, __BD 0, __BD 0));
			break;

		case __BATTLESHIP_MOVINGUPRIGHTEVENT__:
			_battleship -> toMove (0, QGAMES::Vector (__BD 1, __BD -1, __BD 0));
			break;

		case __BATTLESHIP_MOVINGDOWNEVENT__:
			_battleship -> toMove (0, QGAMES::Vector (__BD 0, __BD 1, __BD 0));
			break;

		case __BATTLESHIP_MOVINGDOWNRIGHTEVENT__:
			_battleship -> toMove (0, QGAMES::Vector (__BD 1, __BD 1, __BD 0));
			break;

		case __BATTLESHIP_MOVINGLEFTEVENT__:
			_battleship -> toMove (0, QGAMES::Vector (__BD -1, __BD 0, __BD 0));
			break;

		case __BATTLESHIP_MOVINGUPLEFTEVENT__:
			_battleship -> toMove (0, QGAMES::Vector (__BD -1, __BD -1, __BD 0));
			break;

		case __BATTLESHIP_MOVINGDOWNLEFTEVENT__:
			_battleship -> toMove (0, QGAMES::Vector (__BD -1, __BD 1, __BD 0));
			break;

		case __BATTLESHIP_NOMOVINGEVENT__:
			_battleship -> toMove (0, QGAMES::Vector::_cero);
			break;

		case __BATTLESHIP_FIREEVENT__:
			{
				BATTLESHIP::InputHandlerBehaviour::NotificationData* dt = 
					(BATTLESHIP::InputHandlerBehaviour::NotificationData*) evnt.data ();
				_battleship -> toShoot (1 + (int) (__BD dt ->  _impulse / __BD 10), QGAMES::Vector (__BD 0, __BD -1, __BD 0), 0);
			}

			break;

		case __BATTLESHIP_JUMPEVENT__:
			{
				BATTLESHIP::InputHandlerBehaviour::NotificationData* dt = 
					(BATTLESHIP::InputHandlerBehaviour::NotificationData*) evnt.data ();
				_battleship -> toJump (dt ->  _impulse,
					QGAMES::Vector (__BD (dt -> _rightOn ? 1 : ((dt -> _leftOn) ? -1 : 0)),
									__BD (dt -> _downOn ? 1 : ((dt -> _upOn) ? -1 : 0)), __BD 0));
			}

			break;

		case __BATTLESHIP_CATCHLEAVESOMETHINGEVENT__:
			_battleship -> toCatchOrLeaveThing ();
			break;

		case __BATTLESHIP_ITERATEELEMENTSEVENT__:
			_battleship -> toIteratePocket ();
			break;
		// ---

		// Observing the loop just to add seconds to the time that the player has been playing
		// When this happen, then a notification happens there an the score could be actualized!
		case __QGAMES_LOOPCOUNTEREVENT__:
			{
				BATTLESHIP::Game* dG = dynamic_cast <BATTLESHIP::Game*> (game ());
				assert (dG); // Just in case...
				dG -> addToPlayerSeconds (1); // To the current player...
			}

			break;

		default:
			QGAMES::GameState::processEvent (evnt);
			break;
	}
}

// ---
void BATTLESHIP::OptionsWhenPlayingGameState::onEnter ()
{
	QGAMES::SelectionOptionsGameState::onEnter ();

	observe (game () -> inputHandler ());
}

// ---
void BATTLESHIP::OptionsWhenPlayingGameState::onExit ()
{
	unObserve (game () -> inputHandler ());

	QGAMES::SelectionOptionsGameState::onExit ();
}

// ---
void BATTLESHIP::OptionsWhenPlayingGameState::processEvent (const QGAMES::Event& evnt)
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
QGAMES::SelectionOptionsGameState::Menus BATTLESHIP::OptionsWhenPlayingGameState::createMenus ()
{
	QGAMES::SelectionOptionsGameState::Menus result;
	result.insert (QGAMES::SelectionOptionsGameState::Menus::value_type 
		(__BATTLESHIP_OPTIONWHENPLAYINGGUISYSTEM__, QGAMES::SelectionOptionsGameState::Menu 
			(__BATTLESHIP_OPTIONWHENPLAYINGGUISYSTEM__, QGAMES::Position (__BD 10, __BD 10, __BD 0))));
	return (result);
}

// ---
int BATTLESHIP::OptionsWhenPlayingGameState::SelectionHandler::whenWidgetSelected (QGAMES::Widget* wdt)
{
	BATTLESHIP::Game::Conf* conf = 
		dynamic_cast <BATTLESHIP::Game::Conf*> (owner () -> game () -> configuration ());
	assert (conf); // It must be a BATTLESHIP

	int result = 0; // To continue...

	switch (wdt -> id ())
	{
		case __BATTLESHIP_GUISYSTEMEXITGAMEWDT__:
		case __BATTLESHIP_GUISYSTEMCONTINUEGAMEWDT__:
			result = wdt -> id (); // Time to exit the menu...with the option selected.
			break;

		case __BATTLESHIP_GUISYSTEMSAVEGAMEWDT__:
			conf -> save (owner () -> game () -> parameter (__GAME_DATADIRPROPERTYNAME__) + 
				std::string (__PATH_SEPARATOR__) + QGAMES::StandardInitialSelectionOptionsGameState::_CONFFILENAME);
			// ...but it continues in the menu...
			owner () -> game () -> sound (__QGAMES_SAVEWAVSOUND__) -> play (__QGAMES_GAMESTATESOUNDCHANNEL__);
			break;

		default:
			assert (0); // It shouldn't be here...It means there is options not controlled!!
			break;
	}

	return (result);
}

// ---
int BATTLESHIP::GameStateControl::circunstanceWhen (QGAMES::GameState* st, const QGAMES::Event& evnt)
{
	int result = 0;

	// Internal funcrion to change the world used twice at least...
	auto setNewWorldAndScene = 
		[=](int& nW, int& nS, int& cP) -> void
			{
				BATTLESHIP::Game* dG = dynamic_cast <BATTLESHIP::Game*> (game ());
				assert (dG); 

				nW = 1; nS = -1; cP = 0;
				dG -> nextWorldAndScene (dG -> currentWorld (), dG -> currentScene (), 
					0 /** Always the main conection point. */, nW, nS, cP);
				if (nW != -1 && nS != -1)
				{
					// The scenes are set thinking for the next state...
					dG -> setCurrentWorld (nW);
					dG -> setCurrentScene (nS);
					dG -> setCurrentInitPoint (nS, cP);
				}
			};

	if (evnt.code () == __QGAMES_GAMENESTEDSTATEFINISHED__)
	{
		BATTLESHIP::Game* bG = dynamic_cast <BATTLESHIP::Game*> (game ());
		assert (bG);

		switch (st -> type ())
		{
			// When the state has finishes because an options has been selected...
			case __BATTLESHIP_MENUGAMESTATE__:
				{
					QGAMES::StandardInitialSelectionOptionsGameState* gSS =
						dynamic_cast <QGAMES::StandardInitialSelectionOptionsGameState*> (st);
					assert (gSS); // Just in case there is mistake at some point of the code not totally controlled...
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

						case __BATTLESHIP_GUISYSTEMINSTRUCTIONSWDT__: 
							result = 3; // Instructions
							break;

						default:
							assert (0); // It shouldn't be here...It means there is an option not well managed!!
							break;
					}
				}

				break;

			// When the playing control finishes...
			case __BATTLESHIP_CONTROLPLAYINGGAMESTATE__:
				{
					// The playing has finished, but depending on its final state
					// What to do will be different...
					BATTLESHIP::PlayingControlGameState* cPS = 
						dynamic_cast <BATTLESHIP::PlayingControlGameState*> (st);
					assert (cPS); // Just in case...it shouldn't happen but who knows!
					BATTLESHIP::PlayingGameState* pS = cPS -> playingState ();
					assert (pS); // Same!...

					switch (pS -> lastMainCharacterState ())
					{
						case BATTLESHIP::PlayingGameState::MainCharacterState::_DIED:
							{
								BATTLESHIP::PlayingScene* scn = 
									dynamic_cast <BATTLESHIP::PlayingScene*> (bG -> activeWorld () -> activeScene ());
								assert (scn);

								// When the current scene is a challenge scene, 
								// to die has something to do with reaching the goal
								// But the live is lost always!
								if (dynamic_cast <BATTLESHIP::ChallengeScene*> (scn))
								{
									int nW, nS, cP;
									setNewWorldAndScene (nW, nS, cP);
									if (nW == -1 && nS == -1) result = 3; 
									// Just in case no more scenes have been defined after this one!
								}

								// Remember that result = 0 (the default) means to wait...

							}

							break;

						case BATTLESHIP::PlayingGameState::MainCharacterState::_REACHGOAL:
							{
								int nW, nS, cP;
								setNewWorldAndScene (nW, nS, cP);
								result = 
									((nW == -1 && nS == -1) 
										? 3 // No more scene, the player wins!!
											// The hall of fame has to be actualized there (when the win message is shown)
											// At this point neither the playing state has been finalized yet not the ninja
											// so the score has been actualized and the actualizedHallOfFame would be uncomplete!
										: 1 // Go to the next
									);

								// Always...
								assert (bG -> battleship ()); // Just in case
								bG -> setScore (bG -> battleship () -> score ());
							}

							break;

						case BATTLESHIP::PlayingGameState::MainCharacterState::_OPTIONS:
							result = 2; // The main character is asking for options to continue...
							break;

						default:
							assert (0); // It shouldn't be here. It means there is an state not weel managed!!
							break;
					}
				}

				break;

			// When the state showing that player has died finishes, it needed to take a decision...
			case __BATTLESHIP_PLAYERDIEDGAMESTATE__:
				{
					// If the player is the last and there is no more player to play...
					// Then the game finishes...
					int cPlayer = bG -> currentPlayer ();
					bG -> setLives (bG -> lives () - 1);
					do bG -> setNextPlayer (); 
					while (bG -> lives () == 0 && cPlayer != bG ->  currentPlayer ());
					result = (bG -> lives () == 0 && cPlayer == bG -> currentPlayer ()) ? 1 : 0;
					// 1 finishes the game, 0 starts back again with a new player...
				}

				break;

			// When the state of "game over" finished...
			// It is time to actualize the hall of fame!
			// The playing state will have finished and also the ninja and then the score will have been actualized!
			case __BATTLESHIP_PLAYERGAMEOVERGAMESTATE__:
				{
					bG -> actualizeHallOfFame ();

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
