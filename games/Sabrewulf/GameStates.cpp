#include "GameStates.hpp"
#include "Game.hpp"
#include "Worlds.hpp"
#include "Scenes.hpp"
#include "InputHandler.hpp"
#include "Sabreman.hpp"

// ---
SabreWulfPresentationText::SabreWulfPresentationText (const std::string& t)
		: QGAMES::ScoreObjectText (0, t, QGAMES::Forms (), 0)
{
	_forms.insert (QGAMES::Forms::value_type (__SABREWULFLETTERS__, 
		QGAMES::Game::game () -> form (__SABREWULFLETTERS__)));
	setText (t);
}

// ---
SabreWulfLittlefPresentationText::SabreWulfLittlefPresentationText (const std::string& t)
		: QGAMES::ScoreObjectText (0, t, QGAMES::Forms (), 0)
{
	_forms.insert (QGAMES::Forms::value_type (__SABREWULFLITTLELETTERS__, 
		QGAMES::Game::game () -> form (__SABREWULFLITTLELETTERS__)));
	setText (t);
}

// ---
void SabreWulfIntroState::onEnter ()
{
	// The counter to control how long the 
	// screen is a loading mode...
	_counter = 0;
	// The background
	_background = _game -> form (__SABREWULFBACKGROUND__);
	// The sabrewulf logo used during this state
	_sabrewulfLogo = _game -> form (__SABREWULFLOGO__);
	// The logo used during this state...
	_logo = _game -> form (__SABREWULFCOMMTY__);
	// The text used...
	_text = new SabreWulfPresentationText (std::string ("COPYRIGHT 2016 Ignacio Cea"));
	_text -> setSpace (-5); // Letters are tight!!
	// Not fadding 
	_fadeOut = false;
	// The counter for alpha channel to the begging...
	_counterAlpha = 255;
	// It doesn't move the logo on enter
	_moveLogo = false;
	// The position of the sabrewulf logo has no increment
	_posLogo = 0;

	// There is no active world...so far
	(__AGM _game) -> setWorld (__MININT__);
	// There is no score objects...
	(__AGM _game) -> removeScoreObjects ();

	// The music sounds...
	_game -> sound (__SABREWULFSTARTMUSIC__) -> play (__SABREWULF_BACKGROUNDCHANNEL__);
}

// ---
void SabreWulfIntroState::updatePositions ()
{
	// Two seconds in this state...
	// Before fading the title out...
	if (_counter++ > (5 * _game -> framesPerSecond ()))
		_fadeOut = true;

	// If fadding out...
	if (_fadeOut)
	{
		// ...and start to disappear also...
		_counterAlpha = _counterAlpha - 2; 
		if (_counterAlpha < 0) _counterAlpha = 0;
		if (_counterAlpha < 80) _moveLogo = true;
	}

	// If the logo has to move...
	if (_moveLogo)
	{
		// ...only 50 pixels...
		if (_posLogo < 50) _posLogo++;
	}

	// No more than 10 seconds...
	if (_counter++ > (12 * _game -> framesPerSecond ())) 
		_game -> setState (std::string (__SABREWULFSELECTSTATENAME__));
}

// ---
void SabreWulfIntroState::drawOn (QGAMES::Screen* s)
{
	// Displays the background...
	_background -> drawOn (s, 0, QGAMES::Position::_cero, 100);

	// Display the sabrewulf logo...a very scared one!
	QGAMES::Position bPos = QGAMES::Position 
		(__BD ((__SCREENWIDTH__ - _sabrewulfLogo -> frameWidth ()) >> 1),
		 __BD (((__SCREENHEIGHT__ - _sabrewulfLogo -> frameHeight ()) >> 1) - 35 + _posLogo), __BD 0); 
	_sabrewulfLogo -> drawOn (s, 0, bPos, 255);

	// During this state the logo of Ceasoftware is displayed at the bottom
	QGAMES::Position lPos = QGAMES::Position 
		(__BD 10, __BD (__SCREENHEIGHT__ - _logo -> frameHeight () - 10), __BD 0); 
	_logo -> drawOn (s, 0, lPos, _counterAlpha);
	// Write down the text, next to the logo...
	_text -> setAlphaLevel (_counterAlpha);
	_text -> drawOn (s, lPos + QGAMES::Position (__BD (_logo -> frameWidth () + 20), 
		__BD ((_logo -> frameHeight () - _text -> visualHeight ()) >> 1), __BD 0));
}

// ---
void SabreWulfIntroState::onExit ()
{
	delete (_text);
	// The music stops...if any...
	_game -> sound (__SABREWULFSTARTMUSIC__) -> stop ();
}

// ---
SabreWulfSelectionState::SabreWulfSelectionState (QGAMES::Game* g)
	: QGAMES::GameState (__SABREWULFSELECTSTATE__, g),
	  _logo (NULL),
	  _options (),
	  _optionLevels (),
	  _optionGraphics (),
	  _optionForms (),
	  _currentOption (0),
	  _currentOptionLevel (0),
	  _counterMovement (0),
	  _positionOption (0),
	  _movingToOption (0),
	  _figure (0),
	  _typeOfControl (0),
	  _blinkAttr (50),
	  _blinkDirection (2),
	  _counterNoActivity (0),
	  _startToExit (false),
	  _counterExiting (0),
	  _wantToExit (false)
{
	// Nothing else to do...
}

// ---
void SabreWulfSelectionState::nextOption ()
{
	if (_startToExit)
		return; // Nothing is possible when exiting...

	_counterNoActivity = 0; // Something has been pressed...
	if (_movingToOption != 0)
		return;
	if (_currentOption < (__SABREWULFNUMBEROPTIONS__ - 1))
		_movingToOption = 1;
}

// ---
void SabreWulfSelectionState::previousOption ()
{
	if (_startToExit)
		return; // Nothing is possible when exiting...

	_counterNoActivity = 0; // Something has been pressed...
	if (_movingToOption != 0)
		return;
	if (_currentOption > 0)
		_movingToOption = -1;
}

// ---
void SabreWulfSelectionState::optionSelected ()
{
	if (_startToExit)
		return; // Nothing is possible when exiting...

	_counterNoActivity = 0; // Something has been pressed...
	if (_movingToOption != 0)
		return;
	if (_currentOption == 0)
		_typeOfControl = 0;
	if (_currentOption == 1)
		_typeOfControl = 1;
	if (_currentOption == 3)
	{
		_startToExit = true;
		_game -> sound (__SABREMANSTARTGAMEMUSIC__) -> play (__SABREWULF_BACKGROUNDCHANNEL__);
	}

	if (_currentOption == 2)
	{
		if (++_currentOptionLevel >= __SABREWULFNUMBEROFLEVELS__)
			_currentOptionLevel = 0;
		_options [2] = _optionLevels [_currentOptionLevel];
		delete (_optionGraphics [2]);
		_optionGraphics [2] = new SabreWulfPresentationText (_options [2]);
		_optionGraphics [2] -> setSpace (0);
	}
}

// ---
void SabreWulfSelectionState::optionAt (const QGAMES::Position& pos)
{
	if (_startToExit)
		return; // Nothing is possible when exiting...

	_counterNoActivity = 0;
	QGAMES::Position oPos (__BD (__SCREENWIDTH__ - 490), __BD 130, __BD 0);
	for (int i = 0; i < __SABREWULFNUMBEROPTIONS__; i++)
	{
		QGAMES::Position p1C = oPos + 
			(__BD (i * 60) * QGAMES::Vector (__BD 0, __BD 1, __BD 0));
		QGAMES::Position p2C = p1C + QGAMES::Vector (__BD 350, __BD 54, __BD 0);
		QGAMES::Rectangle rOpt (p1C, p2C);
		if (rOpt.hasIn (pos))
			_currentOption = i;
	}
}

// ---
void SabreWulfSelectionState::onEnter ()
{
	// The background used during this state
	_background = _game -> form (__SABREWULFBACKGROUND__);
	// ...and the logo used during this state...
	_logo = _game -> form (__SABREWULFCOMMTY__);

	// Define the options related with the level...
	_optionLevels.resize (__SABREWULFNUMBEROFLEVELS__);
	_optionLevels [0] = std::string ("Easy");
	_optionLevels [1] = std::string ("Normal");
	_optionLevels [2] = std::string ("Difficult");

	// Define the options...
	_options.resize (__SABREWULFNUMBEROPTIONS__);
	_options [0] = std::string ("Keyboard");
	_options [1] = std::string ("Joystick");
	_options [2] = _optionLevels [_currentOptionLevel];
	// When the game restarts, the level is the same than last...
	_options [3] = std::string ("Start game");

	// ...and the equivalent graphics...
	_optionGraphics.resize (__SABREWULFNUMBEROPTIONS__);
	for (int i = 0; i < __SABREWULFNUMBEROPTIONS__; i++)
	{
		_optionGraphics [i] = new SabreWulfPresentationText (_options [i]);
		_optionGraphics [i] -> setSpace (0);
	}

	// ..and also the graphics in the left...
	_optionForms.clear ();
	_optionForms.resize (__SABREWULFNUMBEROPTIONS__);
	_optionForms [0] = _game -> form (__SABREWULFKEYBOARDFORM__);
	_optionForms [1] = _game -> form (__SABREWULFJOYSTICKFORM__);
	_optionForms [2] = _game -> form (__SABREWULFLEVELFORM__);
	_optionForms [3] = _game -> form (__SABREWULFGOFORM__);

	// It is not time to exit, so far...
	_startToExit = false;
	// ...and the counter to count how much left to exit is set to 0
	_counterExiting = 0;
	// It doesn't want to exit...so far
	_wantToExit = false;
	// The counter movement is set to 0
	_counterMovement = 0;
	// No movement to none option...
	_movingToOption = 0;
	// No counter in place to execute the movement
	_positionOption = 0;
	// Type of control selected: Keyboard or Joystick?
	// First time it will be keyboard, but after first game who knows?
	_typeOfControl = ((InputHandler*) _game -> inputHandler () -> behaviour ()) -> isJoystickActive () ? 1 : 0; 
	// Blinking attribute
	_blinkAttr = 50;
	// The dircetion is up...
	_blinkDirection = 2;
	// The counter of no activity starts...
	_counterNoActivity = 0;
	// The figure is 0
	_figure = 0;

	// Sets up the score objects...
	(__AGM _game) -> addScoreObjects ();
	// Sets up the world...
	(__AGM _game) -> setWorld (__SABREWULFWORLD__);
	// Reset everything...
	((SabreWulfGame*) _game) -> resetGame ();

	// But this state has nothing to show...
	(__AGM _game) -> setWorld (__MININT__);
	// ...even no score objects...
	(__AGM _game) -> removeScoreObjects ();
}

// ---
void SabreWulfSelectionState::updatePositions ()
{
	// The things move...
	if (++_counterMovement > 6)
	{
		_counterMovement = 0;
		if (++_figure > 7)
			_figure = 0;
	}

	// Moves the rectangle around the option...
	if (_movingToOption != 0)
	{
		_positionOption += 4;
		if (_positionOption > 60)
		{
			_positionOption = 0;
			if (_movingToOption == 1)
				_currentOption++;
			else
				_currentOption--;
			_movingToOption = 0;
		}
	}

	// Blinks...
	_blinkAttr += _blinkDirection;
	if (_blinkAttr > 150) _blinkAttr = 150;
	if (_blinkAttr < 50) _blinkAttr = 50;
	if (_blinkAttr == 150 || _blinkAttr == 50)
		_blinkDirection *= -1;

	// if the right key has been selected,
	// It is time to start to move to the next state...
	if (_startToExit)
		if (_counterExiting++ > (2 * _game -> framesPerSecond ()))
			_wantToExit = true;

	// If it has to exit, the status is changed...
	if (_wantToExit)
		_game -> setState (std::string (__SABREWULFSTARTSTATENAME__));

	// Counts the no activity...
	if (++_counterNoActivity > (10 * _game -> framesPerSecond ()))
		_game -> setState (std::string (__SABREWULFDEMOSTATENAME__));
}

// ---
void SabreWulfSelectionState::drawOn (QGAMES::Screen* s)
{
	// Draw the background and the logo...
	_background -> drawOn (s, 0, QGAMES::Position::_cero, 100); // A little bit fade - out!

	// Draws the border for the presentation...
	// ...the top
	QGAMES::Form* top1 = _game -> form (__SPRITETOPBORDER1__);
	QGAMES::Form* top2 = _game -> form (__SPRITETOPBORDER2__);
	for (int i = 0; i < 3; i++)
	{
		int len = 2 * top1 -> frameWidth () + top2 -> frameWidth ();
		top1 -> drawOn (s, 0, QGAMES::Position (__BD ((i * len) + 8), __BD 12, __BD 0));
		top2 -> drawOn (s, 0, QGAMES::Position (__BD (top1 -> frameWidth () + (i * len) + 8), __BD 12, __BD 0));
		top1 -> drawOn (s, 0, QGAMES::Position 
			(__BD (top1 -> frameWidth () + top2 -> frameWidth () + (i * len) + 8), __BD 12, __BD 0));
	}

	// ..the bottom...
	QGAMES::Form* bottom1 = _game -> form (__SPRITEBOTTOMBORDER1__);
	QGAMES::Form* bottom2 = _game -> form (__SPRITEBOTTOMBORDER2__);
	for (int i = 0; i < 3; i++)
	{
		int len = 2 * bottom1 -> frameWidth () + bottom2 -> frameWidth ();
		bottom1 -> drawOn (s, 0, QGAMES::Position (__BD ((i * len) + 8), 
			__BD (__SCREENHEIGHT__ - bottom1 -> frameHeight () - 12), __BD 0));
		bottom2 -> drawOn (s, 0, QGAMES::Position (__BD (top1 -> frameWidth () + (i * len) + 8), 
			__BD (__SCREENHEIGHT__ - bottom1 -> frameHeight () - 12), __BD 0));
		bottom1 -> drawOn (s, 0, QGAMES::Position 
			(__BD (top1 -> frameWidth () + top2 -> frameWidth () + (i * len) + 8), 
				__BD (__SCREENHEIGHT__ - bottom1 -> frameHeight () - 12), __BD 0));
	}

	// The laterals...
	QGAMES::Form* side1 = _game -> form (__SPRITESIDEBORDER__);
	for (int i = 0; i < 2; i++)
	{
		side1 -> drawOn (s, 0, QGAMES::Position (__BD 8, 
			__BD (64 + (i * side1 -> frameHeight ())), __BD 0));
		side1 -> drawOn (s, 0, QGAMES::Position (__BD (__SCREENWIDTH__ - side1 -> frameWidth () - 8), 
			__BD (64 + (i * side1 -> frameHeight ())), __BD 0));
	}

	// Draws the logo in the bottom left corner
	_logo -> drawOn (s, 0, QGAMES::Position 
		(__BD 10, __BD (__SCREENHEIGHT__ - _logo -> frameHeight () - 10), __BD 0));

	// The original position of the options..
	QGAMES::Position oPos (__BD (__SCREENWIDTH__ - 490), __BD 130, __BD 0);

	// Draws a full rectangle around the current options selected
	// The color of the rectangle is gray...
	QGAMES::Position p1A = oPos + 
		(__BD ((_typeOfControl == 0) ? 0 : 1) * 60) * QGAMES::Vector (__BD 0, __BD 1, __BD 0);
	QGAMES::Position p2A = p1A + QGAMES::Vector (__BD 350, __BD 54, __BD 0);
	s -> drawRectangle (QGAMES::Rectangle (p1A, p2A), QGAMES::Color (_blinkAttr, _blinkAttr, _blinkAttr), true);
	s -> drawRectangle (QGAMES::Rectangle (p1A, p2A), QGAMES::Color (255, 255, 255));

	// Draws the options...
	for (int i = 0; i < __SABREWULFNUMBEROPTIONS__; i++)
	{
		QGAMES::Vector aPos (__BD 0, __BD (i * 60), __BD 0);
		_optionForms [i] -> drawOn (s, 0, oPos + aPos);
		_optionGraphics [i] -> 
			drawOn (s, oPos + aPos + QGAMES::Vector (__BD 60, __BD 10, __BD 0));
	}
	
	// Draws a while rectangle around the selected option...
	QGAMES::Position p1C = oPos + 
		(__BD (_currentOption * 60) * QGAMES::Vector (__BD 0, __BD 1, __BD 0)) + 
			(__BD _movingToOption * QGAMES::Vector (__BD 0, __BD _positionOption, __BD 0));
	QGAMES::Position p2C = p1C + QGAMES::Vector (__BD 350, __BD 54, __BD 0);
	s -> drawRectangle (QGAMES::Rectangle (p1C, p2C), QGAMES::Color (255,255,255), false);
}

// ---
void SabreWulfSelectionState::onExit ()
{
	// Deletes all the reserved things...
	for (int i = 0; i < __SABREWULFNUMBEROPTIONS__; i++)
		delete (_optionGraphics [i]);
	_options.clear ();
	_optionGraphics.clear ();
	_optionForms.clear ();

	// It is time to select the level...
	((SabreWulfGame*) _game) -> setLevel (_currentOptionLevel);
	// Select the type of control: joystick or keyboard?
	((SabreWulfGame*) _game) -> setJoystick ((_typeOfControl == 0) ? false : true);

	// Just in case the exiting sound is still playing, it is stoped!
	_game -> sound (__SABREMANSTARTGAMEMUSIC__) -> stop ();
}

// ---
void SabreWulfDemoState::onEnter ()
{
	// It doesn't want to exit...so far
	_wantToExit = false;
	// The counter to 0
	_counter = 0;

	// The background has to appear...
	(__AGM _game) -> setWorld (__SABREWULFWORLD__);
	// ...and the score objects to...
	(__AGM _game) -> addScoreObjects ();
	// The energy is now back...
	((SabreWulfGame*) _game) -> setEnergyAvailable (100); 
	// Switch to invisible the main character...
	((Sabreman*) (__AGM _game) -> artist (__SABREWULFSABREMAN__)) -> setVisible (false);
	// Sets the initial room...
	((SabreWulfGame*) _game) -> setMazePlaceNumber (__FIRSTMAZEPLACE__);

	// The sound whilst the different parts of maze appears in the screen
	_soundPlaying = __SABREWULFTITLEMUSIC__ + (rand () % __SABREWULFMAXNUMTITLEMUSICS__);
	_game -> sound (_soundPlaying) -> play (__SABREWULF_BACKGROUNDCHANNEL__);
}

// ---
void SabreWulfDemoState::updatePositions ()
{
	// See if it's time to change the maze place visited...
	if (++_counter > (5 * _game -> framesPerSecond ())) // Every two seconds...
	{
		_counter = 0;
		int nR = rand () % __NUMBEROFMAZEPLACES__;
		((SabreWulfGame*) _game) -> setMazePlaceNumber (nR);
	}

	// if the right key has been selected,
	// It is time to move back to the selection state...
	if (_wantToExit)
		_game -> setState (std::string (__SABREWULFSELECTSTATENAME__));
}

// ---
void SabreWulfDemoState::onExit ()
{
	// The background has to disappear...
	(__AGM _game) -> setWorld (__MININT__);
	// ...and the score objects too...
	(__AGM _game) -> removeScoreObjects ();

	// Stop the sound...
	_game -> sound (_soundPlaying) -> stop ();
}

// ---
void SabreWulfStartState::onEnter ()
{
	// This is a simple counter to count 
	// how long the game is in this state...
	_counter = 0;

	// The background has to appear...
	(__AGM _game) -> setWorld (__SABREWULFWORLD__);
	// ...and the score objects to...
	(__AGM _game) -> addScoreObjects ();
	// The energy is now back...
	((SabreWulfGame*) _game) -> setEnergyAvailable (100); 
	// Switch to invisible the main character...
	((Sabreman*) (__AGM _game) -> artist (__SABREWULFSABREMAN__)) 
		-> setVisible (false); 

	// A simple sound to indicate that the player is appearing...
	_game -> sound (__SABREMANAPPEARSSOUND__) -> play (__SABREWULF_BACKGROUNDCHANNEL__);
}

// ---
void SabreWulfStartState::updatePositions ()
{
	// ...time limit?
	// Two seconds is the time the player takes to graw...
	if (_counter++ > (1.5 * _game -> framesPerSecond ()))
		_game -> setState (std::string (__SABREWULFPLAYINGSTATENAME__));
}

// ---
void SabreWulfStartState::onExit ()
{
	// Just in case...
	// It is suppossed the sound has finished at this point...
	_game -> sound (__SABREMANAPPEARSSOUND__) -> stop ();
}

// ---
void SabreWulfPlayingState::onEnter ()
{
	// Sets the world to play...
	// Probably this call is not needed as the previous state did invoke it.
	// But it is used just to confirm...
	(__AGM _game) -> setWorld (__SABREWULFWORLD__);
	// There is finally score objects...
	// The same, probably this invocation is not needed...
	(__AGM _game) -> addScoreObjects ();
	// Re-set the number of the room...
	((SabreWulfWorld*) (__AGM _game) -> activeWorld ()) 
		-> setMazePlaceNumber (((SabreWulfGame*) _game) -> mazePlaceNumber ());

	// The player appear in the same place it was before...
	// It is the fist time it appear, it will be at the middle of the screen...
	// Otherwise, i.e it has died, it will appear at the place he died!
	// So it is not needed here to fix his position!
	// The player is now visible...
	// Switch to invisible the main character...
	((Sabreman*) (__AGM _game) -> artist (__SABREWULFSABREMAN__)) 
		-> setVisible (true);
}

// ---
void SabreWulfPlayingState::drawOn (QGAMES::Screen* s)
{
	if (_game -> isGamePaused ())
	{
		QGAMES::Form* f = _game -> form (__SABREWULFPAUSEGAME__);
		f -> drawOn (s, 0, QGAMES::Position (__BD (__SCREENWIDTH__ >> 1), 
			__BD (__SCREENHEIGHT__ >> 1), __BD 0) - QGAMES::Vector (__BD (f -> frameWidth () >> 1), 
				__BD (f -> frameHeight () >> 1), __BD 0), 255);
	}
}

// ---
void SabreWulfAmuletState::onEnter ()
{
	_counter = 0;

	// The texts...
	_textGot = new SabreWulfLittlefPresentationText; 
	// Text is defining when printing...
	// Take care: maxlength ot the text now is 0
	_textGot -> setSpace (0);
	_textPending = new SabreWulfLittlefPresentationText; 
	// Text content is defining when printing...
	// Take care: maxlength ot the text now is 0
	_textPending -> setSpace (0);

	// The sound starts...
	QGAMES::SoundSystem::system () -> pause ();
	_game -> sound (__SABREMANFINDSAMULET__) -> play (__SABREWULF_BACKGROUNDCHANNEL__);
}

// ---
void SabreWulfAmuletState::updatePositions ()
{
	if (_counter++ == (5 * _game -> framesPerSecond ()))
		_game -> setState (__SABREWULFPLAYINGSTATENAME__); // Start to play again...
}

// ---
void SabreWulfAmuletState::drawOn (QGAMES::Screen* s)
{
	QGAMES::Form* aF = _game -> form (__SABREWULFAMULET__);
	QGAMES::Form* aE = _game -> form (__SABREWULFAEMPTYAMULET__);
	Sabreman* p = (Sabreman*) (__AGM _game) -> artist (__SABREWULFSABREMAN__);
	assert (p); // It has to exist...

	// Draws the pieces...
	int nPieces = 0;
	for (int i = 0; i < __NUMBEROFAMULETPIECES__; i++)
	{
		QGAMES::Position pos = 
			QGAMES::Position (__BD ((i % 2) * aF -> frameWidth ()),  
							  __BD ((i >> 1) * aF -> frameHeight ()), 
							  __BD 0);
		pos = pos + QGAMES::Vector (__BD (__SCREENWIDTH__ >> 1), __BD (__SCREENHEIGHT__ >> 1), __BD 0) -
			QGAMES::Vector (__BD aF -> frameWidth (), __BD aF -> frameHeight (), __BD 0);
		pos = pos - QGAMES::Vector (__BD 0, __BD 20, __BD 0); // Moves it a little bit up...
		if (p -> isCarryingPiece (i)) { aF -> frame (i) -> drawOn (s, pos); nPieces++; }
		else aE -> frame (i) -> drawOn (s, pos);
	}

	// Draws the text...
	// The texts depends on the number of pieces the player carries!
	std::string tGot ("One piece is all that you have found");
	std::string tPending ("Collect three more and outward bound");
	if (nPieces == 2)
	{
		tGot		= std::string ("Two pieces found are yours to keep");
		tPending	= std::string ("This score again is what you seek");
	}
	else if (nPieces == 3)
	{
		tGot		= std::string ("One piece more must be amass");
		tPending	= std::string ("And find the keeper you may pass");
	} 
	else if (nPieces == 4)
	{
		tGot		= std::string ("Four pieces found have now made whole");
		tPending	= std::string ("To find the cave must be your goal");
	}

	_textGot -> setMaxTextLength (tGot.length ());
	_textGot -> setText (tGot);
	_textGot -> drawOn (s, QGAMES::Position 
		(__BD ((__SCREENWIDTH__ - _textGot -> visualLength ()) >> 1), __BD 360, __BD 0));
	_textPending -> setMaxTextLength (tPending.length ());
	_textPending -> setText (tPending);
	_textPending -> drawOn (s, QGAMES::Position 
		(__BD ((__SCREENWIDTH__ - _textPending -> visualLength ()) >> 1), __BD 380, __BD 0));
}

// ---
void SabreWulfAmuletState::onExit ()
{
	// Just in case, the sound stops...
	_game -> sound (__SABREMANFINDSAMULET__) -> stop ();
	QGAMES::SoundSystem::system () -> resume ();
}

// ---
void SabreWulfFlyingDie::onEnter ()
{
	// Get sabreman
	Sabreman* sb = (Sabreman*) ((__AGM _game) -> artist (__SABREWULFSABREMAN__));

	// To know the direction where to move the flying sabreman to...
	_dieOrientation = sb -> lastMonsterOrientation ();
	// The to know where the player is dieying...
	_diePosition = sb -> position ();
	// To know the maximum position to fly to...
	_dieLimitPosition = ((SabreWulfScene*) (__AGM _game) -> activeWorld () -> activeScene ()) -> 
		limitPosition (((SabreWulfGame*) _game) -> mazePlaceNumber (), _diePosition, _dieOrientation);

	// Depeding on the flying it is neccsaary to adjust the final position
	// Taking into account how wide sabreman is...
	_dieLimitPosition -= 5 * _dieOrientation;
	if (_dieOrientation.posX () == __BD 1) // Flying to the right
	{
		_dieLimitPosition -= 
			QGAMES::Vector (__BD (sb -> currentForm () -> frameWidth ()),__BD 0, __BD 0);
		if (_dieLimitPosition.posX () < _diePosition.posX ())
			_dieLimitPosition = _diePosition; // Avoid a neverending fly!
	}
	
	// Adjust the fly to avoid a minum fly!
	int dt = (int) (_dieLimitPosition.posX () - _diePosition.posX ());
	if (dt < 0) dt = -dt; 
	if (dt <= 5) // If the distance to fly is less than `5 pixels..no fly!
		_dieLimitPosition = _diePosition;

	// Kill all monsters...
	((SabreWulfWorld*) ((__AGM _game) -> activeWorld ())) -> killNasties ();
	// ...and makes sabreman invisible...
	sb -> setVisible (false);
	// ...and he doesn't move any more
	sb -> setMove (QGAMES::Vector::_cero);

	// The counter count the time the sabreman is died...
	_counter = 0;
	// Itinitially he's not sat down...
	_satDown = false;

	// Once the status starts, the sound starts...
	_game -> sound (__SABREWULFFLYDISOUND__) -> play (__SABREWULF_BACKGROUNDCHANNEL__);
}

// ---
void SabreWulfFlyingDie::updatePositions ()
{
	// When sabreman gets the limit position...
	// the system waits for 2 seconds showing sabreman sat down...
	if (_diePosition == _dieLimitPosition)
	{
		_satDown = true;
		if (_game -> sound (__SABREWULFFLYDISOUND__) -> isPlaying ())
			_game -> sound (__SABREWULFFLYDISOUND__) -> stop (); // Stops the sound if it is playing...
		if (_counter++ >= 2 * _game -> framesPerSecond ())
		{
			if (((SabreWulfGame*) _game) -> numberOfLives () == 1) // It was the last live...
				((SabreWulfGame*) _game) -> // ...so the game finishes!
					setState (std::string (__SABREWULFDIEFOREVERSTATENAME__));
			else
			{
				// Only in real mode, the lives go down!
				#ifdef NDEBUG
				((SabreWulfGame*) _game) -> 
					setNumberOfLives (((SabreWulfGame*) _game) -> numberOfLives () - 1);
				#endif
				((SabreWulfGame*) _game) -> // The game starts back again!
					setState (std::string (__SABREWULFSTARTSTATENAME__));
			}
		}
	}
	else
	{
		// Moves the die position...pixels by pixel
		// until sabreman reaches the maximum position possible...
		for (int i = 0; i < 4; i++)
			if (_diePosition != _dieLimitPosition)
				_diePosition += _dieOrientation;
	}
}

// ---
void SabreWulfFlyingDie::drawOn (QGAMES::Screen* s)
{
	// Depending on the orientation...
	// ...and the status the form choosen is one or other!
	QGAMES::Form* _sForm =
		_game -> form ((_dieOrientation.posX () >= 0) 
			? (_satDown ? __SABREMANSITRIGHTFORM__ : __SABREMANFLYRIGHTFORM__)
			: (_satDown ? __SABREMANSITLEFTFORM__ : __SABREMANFLYLEFTFORM__));

	// The draws the form in the right position...
	_sForm -> drawOn (s, 0, _diePosition);
}

// ---
void SabreWulfFlyingDie::onExit ()
{
	Sabreman* s = (Sabreman*) ((__AGM _game) -> artist (__SABREWULFSABREMAN__));
	// Put the new position of sabreman...
	s -> setPosition (_diePosition);
	// ...and makes sabreman visible back...
	s -> setVisible (true);
	// ...and sabreman is now in the normal state...
	s -> setInternalState (Sabreman::InternalState::_NORMAL);
	// ...and additionally he's not fighting!
	s -> setFighting (false);
}

// ---
void SabreWulfDieState::onEnter ()
{
	// A counter to count the number of seconds in this state...
	_counter = 0;

	// Plays the sound dieing...
	_game -> sound (__SABREMANDIEINGSOUND__) -> play (__SABREWULF_BACKGROUNDCHANNEL__);

	// The world still exists...
	// ..and the place is the same the player is in...
	// but it has to be re-set
	((SabreWulfWorld*) (__AGM _game) -> activeWorld ()) 
		-> setMazePlaceNumber (((SabreWulfGame*) _game) -> mazePlaceNumber ());
	// all the nasties are reset...
	((SabreWulfWorld*) (__AGM _game) -> activeWorld ()) -> killNasties ();
	// Switch to invisible the main character...
	Sabreman* sb = (Sabreman*) ((__AGM _game) -> artist (__SABREWULFSABREMAN__));
	sb -> setVisible (false);
	// ...and he doesn't move any more
	sb -> setMove (QGAMES::Vector::_cero);
}

// ---
void SabreWulfDieState::updatePositions ()
{
	// After 3 seconds, it is time to decide whether the game 
	// finishes or continues...
	if (_counter++ > (2 * _game -> framesPerSecond ()))
	{
		if (((SabreWulfGame*) _game) -> numberOfLives () == 1) // It was the last live...
			((SabreWulfGame*) _game) -> // ...so the game finishes!
				setState (std::string (__SABREWULFDIEFOREVERSTATENAME__));
		else
		{
			// Only in real mode, the lives go down!
			#ifdef NDEBUG
			((SabreWulfGame*) _game) -> 
				setNumberOfLives (((SabreWulfGame*) _game) -> numberOfLives () - 1);
			#endif
			((SabreWulfGame*) _game) -> // The game starts back again!
				setState (std::string (__SABREWULFSTARTSTATENAME__));
		}
	}
}

// ---
void SabreWulfDieState::drawOn (QGAMES::Screen* s)
{
	// There is nothing to draw so far...
}

// ---
void SabreWulfDieState::onExit ()
{
	// Definetively at this point, the sound has finished...
	// So this instruction is just in case
	_game -> sound (__SABREMANDIEINGSOUND__) -> stop ();
	// ...and sabreman is now in the normal state...
	Sabreman* s = (Sabreman*) ((__AGM _game) -> artist (__SABREWULFSABREMAN__));
	s -> setInternalState (Sabreman::InternalState::_NORMAL);
	// ...and not fighting!
	s -> setFighting (false);
}

// ---
void SabreWulfDieForEverState::onEnter ()
{
	// Get sabreman
	Sabreman* sb = (Sabreman*) ((__AGM _game) -> artist (__SABREWULFSABREMAN__));

	// To know the direction sabreman was moving towards...
	_dieOrientation = sb -> lastOrientation ();
	// The to know where the player is dieying...
	_diePosition = sb -> position ();

	// Kill all monsters...
	((SabreWulfWorld*) ((__AGM _game) -> activeWorld ())) -> killNasties ();
	// ...and makes sabreman invisible...
	((Sabreman*) ((__AGM _game) -> artist (__SABREWULFSABREMAN__))) -> setVisible (false);
	// The forever die happens always after several flyings dies or nornal dies
	// so it is supossed the player channel has ben stopped previously
	// so it is not needed to stop that channel again
	// as killing nasties also stops the sound of nasties growing, it
	// is not either needed to stop nasties channel...

	// The counter count the time the sabreman is died...
	_counter = 0;
}

void SabreWulfDieForEverState::updatePositions ()
{
	if (_counter++ >= 2 * _game -> framesPerSecond ())
		_game -> setState (__SABREWULFENDSTATENAME__);
}

void SabreWulfDieForEverState::drawOn (QGAMES::Screen* s)
{
	// Depending on the orientation...
	QGAMES::Form* _sForm =
		_game -> form ((_dieOrientation.posX () >= 0) 
			? __SABREMANDIERIGHTFORM__ : __SABREMANDIELEFTFORM__);
	// The draws the form in the right position...
	_sForm -> drawOn (s, 0, _diePosition);
}

// ---
void SabreWulfEndState::onEnter ()
{
	_counter = 0;

	// The texts...
	_textEndGame = new SabreWulfLittlefPresentationText ("Game Over Player"); 
	_textEndGame -> setSpace (0);
	std::stringstream n; n << ((SabreWulfGame*) _game) -> percentagePlayed ();
	_textPercentageGame = new SabreWulfLittlefPresentationText ("Adventure Completed " + n.str () + " percent"); 
	_textPercentageGame -> setSpace (0);

	// It doesn't want to exit...
	_wantToExit = false;

	// The sound starts...
	_game -> sound (__SABREWULFFAILMUSIC__) -> play (__SABREWULF_BACKGROUNDCHANNEL__);
}

// ---
void SabreWulfEndState::updatePositions ()
{
	if (_counter++ == (20 * _game -> framesPerSecond ()) ||
		_wantToExit) // Ten seconds in the same state, or the right button has been selected...
	{
		((SabreWulfGame*) _game) -> resetGame (); // Reset the game...
		_game -> setState (__SABREWULFSELECTSTATENAME__); // Start to select again...
	}
}

// ---
void SabreWulfEndState::drawOn (QGAMES::Screen* s)
{
	// Draws the text...
	_textEndGame -> drawOn (s, QGAMES::Position 
		(__BD ((__SCREENWIDTH__ - _textEndGame -> visualLength ()) >> 1), __BD 240, __BD 0));
	_textPercentageGame -> drawOn (s, QGAMES::Position 
		(__BD ((__SCREENWIDTH__ - _textPercentageGame -> visualLength ()) >> 1), __BD 270, __BD 0));
}

// ---
void SabreWulfEndState::onExit ()
{
	// Just in case, the sound stops...
	_game -> sound (__SABREWULFFAILMUSIC__) -> stop ();
}

// ---
void SabreWulfWinState::onEnter ()
{
	_counter = 0;

	// The texts...
	_textNextGame = new SabreWulfLittlefPresentationText 
		("The next adventures of Sabreman will be continued in"); 
	_textNextGame -> setSpace (-2);
	_textCopyright = new SabreWulfLittlefPresentationText
		("The Underwurlde Copyright 1984 by ACG LTD"); 
	_textCopyright -> setSpace (-2);

	// It doesn't want to exit so far...
	_wantToExit = false;

	// The sound starts...
	QGAMES::SoundSystem::system () -> pause ();
	_game -> sound (__SABREWULFWINMUSIC__) -> play (__SABREWULF_BACKGROUNDCHANNEL__);
}

// ---
void SabreWulfWinState::updatePositions ()
{
	if (_counter++ == (45 * _game -> framesPerSecond ()) ||
		_wantToExit) // Ten seconds in the same state...or the right key has been selected!
	{
		((SabreWulfGame*) _game) -> resetGame (); // Reset the game...
		_game -> setState (__SABREWULFSELECTSTATENAME__); // Start to select again...
	}
}

// ---
void SabreWulfWinState::drawOn (QGAMES::Screen* s)
{
	QGAMES::Form* uW = _game -> form (__SABREWULFUNDERWURLDE__);

	// Draws the logo...
	QGAMES::Position pos = QGAMES::Position (__BD (__SCREENWIDTH__ >> 1), __BD (__SCREENHEIGHT__ >> 1), __BD 0) -
			QGAMES::Vector (__BD (uW -> frameWidth () >> 1), __BD (uW -> frameHeight () >> 1), __BD 0);
	uW -> drawOn (s, 0, pos);
	// Draws the text...
	_textNextGame -> drawOn (s, QGAMES::Position 
		(__BD ((__SCREENWIDTH__ - _textNextGame -> visualLength ()) >> 1), __BD 150, __BD 0));
	_textCopyright -> drawOn (s, QGAMES::Position 
		(__BD ((__SCREENWIDTH__ - _textCopyright -> visualLength ()) >> 1), __BD 320, __BD 0));
}

// ---
void SabreWulfWinState::onExit ()
{
	// Just in case, the sound stops...
	_game -> sound (__SABREWULFWINMUSIC__) -> stop ();
}
