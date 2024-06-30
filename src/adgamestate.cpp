#include <AdvancedArcade/adgamestate.hpp>
#include <AdvancedArcade/adletters.hpp>

const int QGAMES::SelectionOptionsGameState::_FADEBLINKING [20] = 
	{ 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 240, 230, 220, 210, 200, 190, 180, 170, 160 };

// ---
void QGAMES::SpendingTimeGameState::reStart ()
{
	counter (_COUNTERTIME) -> initialize ((int) (_seconds * __BD game () -> framesPerSecond ()), 0, true, false);
}

// ---
void QGAMES::SpendingTimeGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	reStartAllCounters ();
	reStart ();
}

// ---
void QGAMES::SpendingTimeGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	if (counter (_COUNTERTIME) -> isEnd ())
		notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::SpendingTimeGameState::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::SpendingTimeGameState::_COUNTERTIME, 0 /* to change when initialized. */, 0, true, false));
}

// ---
QGAMES::ForEverUntilInputHandlerEventGameState::ForEverUntilInputHandlerEventGameState 
	(int id, QGAMES::ForEverUntilInputHandlerEventGameState::Handler* hdl, GameState* nS)
		: QGAMES::GameState (id, Game::game (), nS),
		  _handler (hdl)
{ 
	assert (_handler); 

	_handler -> _owner = this;
}

// ---
QGAMES::ForEverUntilInputHandlerEventGameState::~ForEverUntilInputHandlerEventGameState ()
{ 
	delete (_handler); 
	_handler = NULL;
}

// ----
void QGAMES::ForEverUntilInputHandlerEventGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	reStartAllOnOffSwitches ();

	QGAMES::InputHandler* iH = game () -> inputHandler ();
	if (!iH)
		return; // Nothing to take care...
	observe (iH);
}

// ---
void QGAMES::ForEverUntilInputHandlerEventGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	if (onOffSwitch (_SWITCHFINISH) -> isOn ())
		notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
}

// ----
void QGAMES::ForEverUntilInputHandlerEventGameState::onExit ()
{
	QGAMES::GameState::onExit ();

	QGAMES::InputHandler* iH = game () -> inputHandler ();
	if (!iH)
		return; 
	unObserve (iH);
}

// ---
void QGAMES::ForEverUntilInputHandlerEventGameState::processEvent (const QGAMES::Event& evnt)
{
	QGAMES::GameState::processEvent (evnt);
	if (_handler -> stopWhen (evnt))
		onOffSwitch (_SWITCHFINISH) -> set (true);
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::ForEverUntilInputHandlerEventGameState::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::ForEverUntilInputHandlerEventGameState::_SWITCHFINISH, false));
}

// ---
bool QGAMES::ForEverUntilIntroKeyOrLeftButtonMousePressedGameState::InputHandler::stopWhen 
	(const QGAMES::Event& evnt)
{
	bool result = false;
	if (evnt.code () == __QGAMES_KEYBOARDEVENT__)
	{
		QGAMES::KeyBoardEventData* dt = (QGAMES::KeyBoardEventData*) evnt.data ();
		if (!dt -> _on && dt ->_internalCode == QGAMES::KeyCode::QGAMES_RETURN)
			result = true;
	}
	else
	if (evnt.code () == __QGAMES_MOUSEBUTTONEVENT__)
	{
		QGAMES::MouseButtonEventData* dt = (QGAMES::MouseButtonEventData*) evnt.data ();
		if (!dt -> _on && dt ->_internalCode == QGAMES::MouseButtonCode::QGAMES_BUTTONLEFT)
			result = true;
	}
	else
	if (evnt.code () == __QGAMES_BUTTONJOYSTICKEVENT__)
	{
		QGAMES::JoystickButtonEventData* dt = (QGAMES::JoystickButtonEventData*) evnt.data ();
		if (!dt -> _on && dt -> _numberButton == 0) // Whatever the joystick is...
			result = true;
	}

	return (result);
}

// ---
bool QGAMES::SpedingTimeUnlessKeyIsPressedGameState::InputHandler::stopWhen (const QGAMES::Event& evnt)
{ 
	return ((evnt.code () == __QGAMES_KEYBOARDEVENT__ && !((QGAMES::KeyBoardEventData*) evnt.data ()) -> _on) ||
			(evnt.code () == __QGAMES_MOUSEBUTTONEVENT__ && !((QGAMES::MouseButtonEventData*) evnt.data ()) -> _on) ||
			(evnt.code () == __QGAMES_BUTTONJOYSTICKEVENT__ && !((QGAMES::JoystickButtonEventData*) evnt.data ()) -> _on));
}

// ---
void QGAMES::SpendingTimeUnlessInputHandlerEventGameState::processEvent (const QGAMES::Event& evnt)
{
	// If the nested game state finishes, then the state itself finishes...
	QGAMES::GameState::processEvent (evnt);
	// If the key indicates it is needed to stop, then the counter is put back to zero...
	if (_handler -> stopWhen (evnt)) // See that the switch to finishes is not put it back into TRUE...
		((QGAMES::SpendingTimeGameState*) nestedState ()) -> reStart ();
}

// ---
bool QGAMES::SpendingTimeOrWaitingUntilAnyKeyPressedGameState::InputHandler::stopWhen (const QGAMES::Event& evnt)
{ 
	return ((evnt.code () == __QGAMES_KEYBOARDEVENT__ && !((QGAMES::KeyBoardEventData*) evnt.data ()) -> _on) ||
			(evnt.code () == __QGAMES_MOUSEBUTTONEVENT__ && !((QGAMES::MouseButtonEventData*) evnt.data ()) -> _on) ||
			(evnt.code () == __QGAMES_BUTTONJOYSTICKEVENT__ && !((QGAMES::JoystickButtonEventData*) evnt.data ()) -> _on));
}

// ---
QGAMES::ShowingTextGameState::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _fade (255),
	  _reference (QGAMES::ShowingTextGameState::Properties::PosReference::_TOPLEFT),
	  _position (Position (__BD 0.5, __BD 0.5, __BD 0)), 
	  _orientation (Vector (__BD 1, __BD 0, __BD 0)) 
{
	if (prps.find (__QGAMES_GAMESTATEPROPATTRFADE__) != prps.end ()) 
		_fade = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRFADE__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__) != prps.end ())
		_position = QGAMES::Position ((*prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__)).second);
	if (prps.find (__QGAMES_GAMESTATEPROPATTRPOSREF__) != prps.end ()) 
		_reference = QGAMES::ShowingTextGameState::Properties::PosReference
			(std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRPOSREF__)).second.c_str ())); // like enum...
	if (prps.find (__QGAMES_GAMESTATEPROPATTRORIENTATION__) != prps.end ())
		_orientation = QGAMES::Vector ((*prps.find (__QGAMES_GAMESTATEPROPATTRORIENTATION__)).second);

	QGAMES::ShowingTextGameState::Properties::Properties (_fade, _position, _reference, _orientation);
}

// ---
QGAMES::ShowingTextGameState::ShowingTextGameState 
		(int id, const QGAMES::ShowingTextGameState::Properties& prps, QGAMES::GameState* nS)
	: QGAMES::GameState (id, QGAMES::Game::game (), nS),
	  _text (NULL),
	  _properties (prps)
{
	// Nothing else to do...
}

// ---
void QGAMES::ShowingTextGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	_text = createText ();
	assert (_text); // Just to take care, it couldn't be null ever...
	_text -> setOrientation (_properties._orientation);
}

// ---
void QGAMES::ShowingTextGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	actualizeText ();
}

// ---
void QGAMES::ShowingTextGameState::drawOn (QGAMES::Screen* s)
{
	QGAMES::GameState::drawOn (s);

	int tW = _text -> visualLength (); int tH = _text -> visualHeight ();
	// Text is 2D...
	QGAMES::Vector adj = QGAMES::Vector::_cero;
	if (_properties._reference == QGAMES::ShowingTextGameState::Properties::PosReference::_CENTER)
		adj = QGAMES::Vector (__BD (tW >> 1), __BD (tH >> 1), __BD 0);
	else if (_properties._reference == QGAMES::ShowingTextGameState::Properties::PosReference::_BOTTONRIGHT)
		adj = QGAMES::Vector (__BD tW, __BD tH, __BD 0);

	QGAMES::Position pos (
		__BD s -> visualWidth () * _properties._position.posX (),
		__BD s -> visualHeight () * _properties._position.posY (),
		__BD 0); // The position declared under the properties structure is always a percentage...
	_text -> setAlphaLevel (_properties._fade);
	_text -> drawOn (s, pos - adj + s -> position ());
}

// ---
QGAMES::ScoreObjectText* QGAMES::ShowingStandardTextGameState::createText ()
{
	return (game () -> textBuilder () -> createText (_fontType, _textString));
}

// ---
QGAMES::ShowingBlinkingStandardTextGameState::Properties::Properties (const std::map <std::string, std::string>& prps)
	: QGAMES::ShowingStandardTextGameState::Properties (prps),
	  _elapsedCycles (2),
	  _numberFadeChanges (5),
	  _fadeStep (5)
{
	if (prps.find (__QGAMES_GAMESTATEPROPATTRELAPSEDCYCLES__) != prps.end ()) 
		_elapsedCycles = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRELAPSEDCYCLES__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRNFADECHANGES__) != prps.end ()) 
		_numberFadeChanges = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRNFADECHANGES__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRFADESTEP__) != prps.end ()) 
		_fadeStep = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRFADESTEP__)).second.c_str ());

	QGAMES::ShowingBlinkingStandardTextGameState::Properties::Properties (_fade, _position, _reference, _orientation, 
		_elapsedCycles, _numberFadeChanges, _fadeStep);
}

// ---
void QGAMES::ShowingBlinkingStandardTextGameState::onEnter ()
{
	QGAMES::ShowingStandardTextGameState::onEnter ();

	reStartAllCounters ();
	counter (_ELAPSEDCYCLEDCOUNTER) -> initialize (_properties._elapsedCycles, 0, true, true);
	counter (_NUMBERFADECHANGESCOUNTER) -> initialize (_properties._numberFadeChanges, 0, true, true);
}

// ---
void QGAMES::ShowingBlinkingStandardTextGameState::actualizeText ()
{
	if(counter (_ELAPSEDCYCLEDCOUNTER) -> isEnd ())
	{
		if (counter (_NUMBERFADECHANGESCOUNTER) -> isEnd ())
			onOffSwitch (_COUNTINGUPSWITCH) -> set (!onOffSwitch (_COUNTINGUPSWITCH) -> isOn ());

		_properties._fade += _properties._fadeStep * (onOffSwitch (_COUNTINGUPSWITCH) -> isOn () ? 1 : -1);
		QGAMES::ShowingStandardTextGameState::_properties._fade = _properties._fade; // The parent is used in draw...
		if (_properties._fade > 255) _properties._fade = 255; // Never greater
		if (_properties._fade < 0) _properties._fade = 0; // Never below
	}
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::ShowingBlinkingStandardTextGameState::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::ShowingBlinkingStandardTextGameState::_ELAPSEDCYCLEDCOUNTER, 0, 0, true, true)); 
	addCounter (new QGAMES::Counter 
		(QGAMES::ShowingBlinkingStandardTextGameState::_NUMBERFADECHANGESCOUNTER, 0, 0, true, true));

	// All of them will be actualized with real values in onEnter method...
}

__IMPLEMENTONOFFSWITCHES__ (QGAMES::ShowingBlinkingStandardTextGameState::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (QGAMES::ShowingBlinkingStandardTextGameState::_COUNTINGUPSWITCH, true));
}

// ---
void QGAMES::ShowingCurrentPlayerNumberGameState::actualizeText ()
{
	assert (dynamic_cast <QGAMES::AdvancedArcadeGame*> (game ()));
	// It has to be an Advanced Arcade Game to access to the number of the player...

	QGAMES::ShowingTextGameState::actualizeText ();
	std::string str = _textString + std::string (" ") + // A space after...
		std::to_string (((QGAMES::AdvancedArcadeGame*) game ()) -> currentPlayer ());
	_text -> setMaxTextLength (str.length ()); _text -> setText (str);
}

// ---
void QGAMES::ShowingFrameRateGameState::actualizeText ()
{
	QGAMES::ShowingTextGameState::actualizeText ();
	std::stringstream ss; ss << game() -> timer () -> loopsPerSecond ();
	std::string str = ss.str () + std::string (" ") + /** A space after... */ _textString;
	_text -> setMaxTextLength (str.length ()); _text -> setText (str);
}

// ---
QGAMES::PlayingASoundGameState::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _soundId (0), _channelId (0)
{
	if (prps.find (__QGAMES_GAMESTATEPROPATTRSOUND__) != prps.end ()) 
		_soundId = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRSOUND__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRCHANNEL__) != prps.end ()) 
		_channelId = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRCHANNEL__)).second.c_str ());

	QGAMES::PlayingASoundGameState::Properties::Properties (_soundId, _channelId);
}

// ---
void QGAMES::PlayingASoundGameState::onEnter ()
{ 
	QGAMES::GameState::onEnter ();

	_soundPlaying = true; 
	game () -> soundBuilder () -> sound (_properties._soundId) -> play (_properties._channelId); 
}

// ---
void QGAMES::PlayingASoundGameState::onExit ()
{ 
	QGAMES::GameState::onExit ();

	_soundPlaying = false; // Always...
	game () -> soundBuilder () -> sound (_properties._soundId) -> stop (); 
}

// ---
QGAMES::ShowingFixFormGameState::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _formId (__QGAMES_COMMTYNETSBIGLOGO__),
	  _fade (255),
	  _position (Position (__BD 0.5, __BD 0.5, __BD 0)), // Center in a 2D screen...
	  _reference (QGAMES::ShowingFixFormGameState::Properties::PosReference::_CENTER)
{
	if (prps.find (__QGAMES_GAMESTATEPROPATTRFORM__) != prps.end ()) 
		_formId = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRFORM__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRFADE__) != prps.end ()) 
		_fade = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRFADE__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__) != prps.end ()) 
		_position = QGAMES::Position ((*prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__)).second);
	if (prps.find (__QGAMES_GAMESTATEPROPATTRPOSREF__) != prps.end ()) 
		_reference = QGAMES::ShowingFixFormGameState::Properties::PosReference
			(std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRPOSREF__)).second.c_str ())); // like enum...
	
	QGAMES::ShowingFixFormGameState::Properties::Properties (_formId, _fade, _position, _reference);
}

// ---
void QGAMES::ShowingFixFormGameState::drawOn (QGAMES::Screen* s)
{
	QGAMES::GameState::drawOn (s);

	QGAMES::Form* frm = game () -> form (_properties._formId);

	QGAMES::Vector tAdd (
		__BD frm -> width (),
		__BD frm -> height (),
		__BD ((frm -> depth () == -1) ? 0 : frm -> depth ()));
	QGAMES::Position pos (
		__BD s -> visualWidth () * _properties._position.posX (), 
		__BD s -> visualHeight () * _properties._position.posY (), 
		__BD 0);
	if (_properties._reference == 
		QGAMES::ShowingFixFormGameState::Properties::PosReference::_CENTER) pos -= tAdd / 2;
	else if (_properties._reference == 
		QGAMES::ShowingFixFormGameState::Properties::PosReference::_BOTTONRIGHT) pos -= tAdd;

	frm -> drawOn (s, 0, pos, _properties._fade);
}

// ---
QGAMES::ShowingAppearingFormGameState::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _formId (__QGAMES_COMMTYNETSBIGLOGO__),
	  _initialFade (255), _finalFade (255), _fadeIncrement (10),
	  _loopsPerFadeChange (6),
	  _position (Position (__BD 0.5, __BD 0.5, __BD 0)), 
	  _reference (QGAMES::ShowingAppearingFormGameState::Properties::PosReference::_CENTER)
{
	if (prps.find (__QGAMES_GAMESTATEPROPATTRFORM__) != prps.end ()) 
		_formId = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRFORM__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRINITIALFADE__) != prps.end ()) 
		_initialFade = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRINITIALFADE__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRFINALFADE__) != prps.end ()) 
		_finalFade = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRFINALFADE__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRFADEINC__) != prps.end ()) 
		_fadeIncrement = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRFADEINC__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRLOOPSFADE__) != prps.end ())
		_loopsPerFadeChange = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRLOOPSFADE__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__) != prps.end ()) 
		_position = QGAMES::Position ((*prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__)).second);
	if (prps.find (__QGAMES_GAMESTATEPROPATTRPOSREF__) != prps.end ())
		_reference = QGAMES::ShowingAppearingFormGameState::Properties::PosReference
			(std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRPOSREF__)).second.c_str ())); // like enum...
	
	QGAMES::ShowingAppearingFormGameState::Properties::Properties 
		(_formId, _initialFade, _finalFade, _fadeIncrement, _loopsPerFadeChange, _position, _reference);
}

// ---
QGAMES::ShowingAppearingFormGameState::ShowingAppearingFormGameState (int id, 
		const QGAMES::ShowingAppearingFormGameState::Properties& prps, QGAMES::GameState* nS)
	: QGAMES::GameState (id, QGAMES::Game::game (), nS),
	  _properties (prps)
{
	// Nothing else to do...
}

// ---
void QGAMES::ShowingAppearingFormGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	reStartAllCounters ();
	counter (_COUNTERWAITFADE) -> initialize (_properties._loopsPerFadeChange, 0, true, true);

	_currentFade = _properties._initialFade;
}

// ---
void QGAMES::ShowingAppearingFormGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();
	
	if (_currentFade != _properties._finalFade)
	{
		if (counter (_COUNTERWAITFADE) -> isEnd ())
			_currentFade++;
	}
	else
		notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
}

// ---
void QGAMES::ShowingAppearingFormGameState::drawOn (QGAMES::Screen* s)
{
	QGAMES::GameState::drawOn (s);

	QGAMES::Form* frm = game () -> form (_properties._formId);

	int fW = frm -> width (); int fH = frm -> height (); 
	int fD = frm -> depth (); if (fD == -1) fD = 0;
	QGAMES::Vector adj = QGAMES::Vector::_cero;
	if (_properties._reference == QGAMES::ShowingAppearingFormGameState::Properties::PosReference::_CENTER)
		adj = QGAMES::Vector (__BD (fW >> 1), __BD (fH >> 1), __BD (fD >> 1));
	else if (_properties._reference == QGAMES::ShowingAppearingFormGameState::Properties::PosReference::_BOTTONRIGHT)
		adj = QGAMES::Vector (__BD fW, __BD fH, __BD fD);
	
	QGAMES::Position pos (
		__BD s -> visualWidth () * _properties._position.posX (),
		__BD s -> visualHeight () * _properties._position.posY (),
		__BD 0); // The position declared under the properties structure is always a percentage...
	frm -> drawOn (s, 0, pos + s -> position () - adj, _currentFade);
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::ShowingAppearingFormGameState::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::ShowingAppearingFormGameState::_COUNTERWAITFADE, 0 /* to change when initialized. */, 0, true, true));
}

// ---
QGAMES::ShowingEntityGameState::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _entityId (__QGAMES_COMMTYNETSPAUSEENTITYID__),
	  _fade (255),
	  _controlPosition (true),
	  _position (Position (__BD 0.5, __BD 0.5, __BD 0)), // Center position in a 2D screen...
	  _reference (QGAMES::ShowingEntityGameState::Properties::PosReference::_CENTER)
{
	if (prps.find (__QGAMES_GAMESTATEPROPATTRENTITY__) != prps.end ()) 
		_entityId = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRENTITY__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRFADE__) != prps.end ()) 
		_fade = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRFADE__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRCTRLPOS__) != prps.end ()) 
		_controlPosition = QGAMES::toUpper (QGAMES::trim (std::string ((*prps.find (__QGAMES_GAMESTATEPROPATTRCTRLPOS__)).second))) == std::string (__YES_STRING__);
	if (prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__) != prps.end ()) 
		_position = QGAMES::Position ((*prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__)).second);
	if (prps.find (__QGAMES_GAMESTATEPROPATTRPOSREF__) != prps.end ()) 
		_reference = QGAMES::ShowingEntityGameState::Properties::PosReference
			(std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRPOSREF__)).second.c_str ())); // like enum...

	QGAMES::ShowingEntityGameState::Properties::Properties 
		(_entityId, _fade, _controlPosition, _position, _reference);
}

// ---
QGAMES::ShowingEntityGameState::ShowingEntityGameState (int id, 
		const QGAMES::ShowingEntityGameState::Properties& prps, QGAMES::GameState* nS)
	: QGAMES::GameState (id, QGAMES::Game::game (), nS),
	  _properties (prps)
{
	// Nothing else to do
}

// ---
void QGAMES::ShowingEntityGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	game () -> entity (_properties._entityId) -> initialize ();
}

// ---
void QGAMES::ShowingEntityGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	QGAMES::Entity* ety = game () -> entity (_properties._entityId);
	// This way is not formal, but states have not an "inEveryLoop" method, 
	// so the only way to execute it for the entity is within this other one!
	// That method is import e.g to actualize the buoys!
	ety -> inEveryLoop ();
	ety -> updatePositions ();
}

// ---
void QGAMES::ShowingEntityGameState::drawOn (QGAMES::Screen* s)
{
	QGAMES::GameState::drawOn (s);

	QGAMES::Entity* ety = game () -> entity (_properties._entityId);
	if (_properties._controlPosition)
	{
		int fW = ety -> currentForm () -> frameWidthForFrame (ety -> currentAspect ()); 
		int fH = ety -> currentForm () -> frameHeightForFrame (ety -> currentAspect ()); 
		int fD = ety -> currentForm () -> frameDepthForFrame (ety -> currentAspect ()); if (fD == -1) fD = 0;
		QGAMES::Vector adj = QGAMES::Vector::_cero;
		if (_properties._reference == QGAMES::ShowingAppearingFormGameState::Properties::PosReference::_CENTER)
			adj = QGAMES::Vector (__BD (fW >> 1), __BD (fH >> 1), __BD (fD >> 1));
		else if (_properties._reference == QGAMES::ShowingAppearingFormGameState::Properties::PosReference::_BOTTONRIGHT)
			adj = QGAMES::Vector (__BD fW, __BD fH, __BD fD);

		ety -> setPosition (QGAMES::Position (
				__BD s -> visualWidth () * _properties._position.posX (),
				__BD s -> visualHeight () * _properties._position.posY (),
				__BD 0) + s -> position () - adj); // The position has to be relative to the screen...
	}

	ety -> setAlphaLevel (_properties._fade);
	ety -> drawOn (s);
}

// ---
void QGAMES::ShowingEntityGameState::onExit ()
{
	QGAMES::GameState::onExit ();

	game () -> entity (_properties._entityId) -> finalize ();
}

// ---
QGAMES::ShowingFormBeingFulfilledGameState::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _formId (__QGAMES_COMMTYNETSBIGLOGO__),
	  _fade (255),
	  _position (Position (__BD 0.5, __BD 0.5, __BD 0)), // Center position in a 2D screen...
	  _reference (QGAMES::ShowingFormBeingFulfilledGameState::Properties::PosReference::_CENTER),
	  _reverse (false), // Means from the left (or top) to the end...
	  _horizontal (false) // Means vertical...
{
	if (prps.find (__QGAMES_GAMESTATEPROPATTRFORM__) != prps.end ()) 
		_formId = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRFORM__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRFADE__) != prps.end ()) 
		_fade = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRFADE__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__) != prps.end ()) 
		_position = QGAMES::Position ((*prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__)).second);
	if (prps.find (__QGAMES_GAMESTATEPROPATTRPOSREF__) != prps.end ()) 
		_reference = QGAMES::ShowingFormBeingFulfilledGameState::Properties::PosReference
			(std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRPOSREF__)).second.c_str ())); // like enum...
	if (prps.find (__QGAMES_GAMESTATEPROPATTRREVERSE__) != prps.end ()) 
		_reverse = QGAMES::toUpper (QGAMES::trim (std::string ((*prps.find (__QGAMES_GAMESTATEPROPATTRREVERSE__)).second))) == std::string (__YES_STRING__);
	if (prps.find (__QGAMES_GAMESTATEPROPATTRHORIZONTAL__) != prps.end ()) 
		_horizontal = QGAMES::toUpper (QGAMES::trim (std::string ((*prps.find (__QGAMES_GAMESTATEPROPATTRHORIZONTAL__)).second))) == std::string (__YES_STRING__);

	QGAMES::ShowingFormBeingFulfilledGameState::Properties::Properties
		(_formId, _fade, _position, _reference, _reverse, _horizontal);
}

// ---
QGAMES::ShowingFormBeingFulfilledGameState::ShowingFormBeingFulfilledGameState (int id, 
		const QGAMES::ShowingFormBeingFulfilledGameState::Properties& prps, QGAMES::GameState* nS)
	: QGAMES::GameState (id, QGAMES::Game::game (), nS),
	  _properties (prps),
	  _currentPercentage (0)
{
	// Nothing else to do...
}

// ---
void QGAMES::ShowingFormBeingFulfilledGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	_currentPercentage = 0;
}

// ---
void QGAMES::ShowingFormBeingFulfilledGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	_currentPercentage = doAction ();
	if (_currentPercentage > 100) _currentPercentage = 100;
	if (_currentPercentage < 0) _currentPercentage = 0;

	if (_currentPercentage == 100)
		notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
}

// ---
void QGAMES::ShowingFormBeingFulfilledGameState::drawOn (QGAMES::Screen* s)
{
	QGAMES::GameState::drawOn (s);

	QGAMES::Form* frm = game () -> form (_properties._formId);

	QGAMES::bdata cP = __BD _currentPercentage / __BD 100;

	int fW = frm -> width (); int fH = frm -> height (); int fD = frm -> depth ();
	if (fD == -1) fD = 0;
	QGAMES::Vector adj = QGAMES::Vector::_cero;
	if (_properties._reference == QGAMES::ShowingFormBeingFulfilledGameState::Properties::PosReference::_CENTER)
		adj = QGAMES::Vector (__BD (fW >> 1), __BD (fH >> 1), __BD (fD >> 1));
	else if (_properties._reference == QGAMES::ShowingFormBeingFulfilledGameState::Properties::PosReference::_BOTTONRIGHT)
		adj = QGAMES::Vector (__BD fW, __BD fH, __BD fD);

	QGAMES::Vector dDir (__BD 0, __BD 0, __BD 0);
	int pX, pY, w, h;
	if (_properties._horizontal)
	{
		w = (int) (__BD fW * cP);
		h = fH;
		pX = _properties._reverse ? (fW - w) : 0;
		pY = 0;
		dDir = QGAMES::Vector (__BD (_properties._reverse ? (fW - w) : 0), __BD 0, __BD 0);
	}
	else
	{
		w = fW;
		h = (int) (__BD fH * cP);
		pX = 0;
		pY = _properties._reverse ? (fH - h) : 0;
		dDir = QGAMES::Vector (__BD 0, __BD (_properties._reverse ? (fH - h) : 0), __BD 0);
	}

	QGAMES::Position pos (
		__BD s -> visualWidth () * _properties._position.posX (),
		__BD s -> visualHeight () * _properties._position.posY (),
		__BD 0); // The position declared under the properties structure is always a percentage...
	s -> drawForm (pos + dDir - adj, frm, pX, pY, w, h, _properties._fade);
}

// ---
QGAMES::LoadingGameState::LoadingGameState (int id, 
		const QGAMES::ShowingFormBeingFulfilledGameState::Properties& prps, QGAMES::GameState* nS)
	: QGAMES::ShowingFormBeingFulfilledGameState (id, prps, nS),
	  _toLoadPerCycle (3), // Three by default is more than enought, I guess...
	  _maxFormsToLoad (0), 
	  _maxSoundsToLoad (0),
	  _maxMovementsToLoad (0), 
	  _maxEntitiesToLoad (0),
	  _maxMapsToLoad (0),
	  _maxWorldsToLoad (0),
	  _currentFormsLoaded (0),
	  _currentSoundsLoaded(0),
	  _currentMovementsLoaded (0),
	  _currentEntitiesLoaded (0),
	  _currentMapsLoaded (0),
	  _currentWorldsLoaded (0),
	  _refForms (),
	  _refSounds (),
	  _refMovements (),
	  _refEntities (),
	  _refMaps (),
	  _refWorlds ()
{
	// Nothing else to do...
}

//---
void QGAMES::LoadingGameState::onEnter ()
{
	QGAMES::ShowingFormBeingFulfilledGameState::onEnter ();

	_maxFormsToLoad 		= game () -> formBuilder () -> numberResources ();
	_maxSoundsToLoad 		= game () -> soundBuilder () -> numberResources ();
	// Objects are not considered part of the party as they are created as long as the resource file is read.
	_maxMovementsToLoad 	= game () -> movementBuilder () -> numberResources ();
	_maxEntitiesToLoad 		= game () -> entityBuilder () -> numberResources ();
	_maxMapsToLoad 			= (__AGM game ()) -> mapBuilder () -> numberResources ();
	_maxWorldsToLoad 		= (__AGM game ()) -> worldBuilder () -> numberResources ();

	_refForms 				= game () -> formBuilder () -> resourcesId ();
	_refSounds 				= game () -> soundBuilder () -> resourcesId ();
	_refMovements 			= game () -> movementBuilder () -> resourcesId ();
	_refEntities 			= game () -> entityBuilder () -> resourcesId ();
	_refMaps 				= (__AGM game ()) -> mapBuilder () -> resourcesId ();
	_refWorlds 				= (__AGM game ()) -> worldBuilder () -> resourcesId ();

	_currentFormsLoaded 	= 
	_currentSoundsLoaded 	= 
	_currentMovementsLoaded =
	 _currentEntitiesLoaded =
	_currentMapsLoaded 		= 
	_currentWorldsLoaded 	= 0;
}

// ---
int QGAMES::LoadingGameState::doAction ()
{
	for (int i = 0; i < _toLoadPerCycle; i++)
	{
		if (_currentFormsLoaded < _maxFormsToLoad)
			game () -> formBuilder () -> form (_refForms [_currentFormsLoaded++]);
		else
		if (_currentSoundsLoaded < _maxSoundsToLoad)
			game () -> soundBuilder () -> sound (_refSounds [_currentSoundsLoaded++]);
		else
		if (_currentMovementsLoaded < _maxMovementsToLoad)
			game () -> movementBuilder () -> movement (_refMovements [_currentMovementsLoaded++]);
		else
		if (_currentEntitiesLoaded < _maxEntitiesToLoad)
			game () -> entityBuilder () -> entity (_refEntities [_currentEntitiesLoaded++]);
		else
		if (_currentMapsLoaded < _maxMapsToLoad)
			(__AGM game ()) -> mapBuilder () -> map (_refMaps [_currentMapsLoaded++]);
		else
		if (_currentWorldsLoaded < _maxWorldsToLoad)
			(__AGM game ()) -> worldBuilder () -> world (_refWorlds [_currentWorldsLoaded++]);
	}

	int current = 
		_currentFormsLoaded + 
		_currentSoundsLoaded +
		_currentMovementsLoaded +
		_currentEntitiesLoaded +
		_currentMapsLoaded +
		_currentWorldsLoaded;
	int total =
		_maxFormsToLoad +
		_maxSoundsToLoad +
		_maxMovementsToLoad +
		_maxEntitiesToLoad + 
		_maxMapsToLoad +
		_maxWorldsToLoad;

	return ((total != 0) ? (int) ((__BD current / __BD total) * 100) : 100);	
}

// ---
QGAMES::ShowingInstructionsGameState::Properties::Properties 
		(const QGAMES::ShowingInstructionsGameState::Properties::TextLineList& txts, 
		 const std::map <std::string, std::string>& prps)
	: _texts (txts),
	  _textSeparation (0),
	  _visibleZone (QGAMES::Rectangle (QGAMES::Position (__BD 0, __BD 0, __BD 0),
									   QGAMES::Position (__BD 1, __BD 1, __BD 1))),
	  _fade (255),
	  _movement (-1), // ...from bottom to top of the visible zone...
	  _toMove (0)
{
	if (prps.find (__QGAMES_GAMESTATEPROPATTRSEPARATION__) != prps.end ()) 
		_textSeparation = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRSEPARATION__)).second.c_str ());

	if (prps.find (__QGAMES_GAMESTATEPROPATTRVISIBLEZONE__) != prps.end ())
	{
		std::vector <QGAMES::bdata> cds = QGAMES::getElementsFromAsBDATA ((*prps.find (__QGAMES_GAMESTATEPROPATTRVISIBLEZONE__)).second, ',');
		if ((int) cds.size () == 6) // There must be 6 elements only...
			_visibleZone = QGAMES::Rectangle (QGAMES::Position (cds [0], cds [1], cds [2]),
											  QGAMES::Position (cds [3], cds [4], cds [5]));
	}

	if (prps.find (__QGAMES_GAMESTATEPROPATTRFADE__) != prps.end ()) 
		_fade = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRFADE__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRMOVEMENT__) != prps.end ()) 
		_movement = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRMOVEMENT__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRTOMOVE__) != prps.end ()) 
		_toMove = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRTOMOVE__)).second.c_str ());

	QGAMES::ShowingInstructionsGameState::Properties::Properties 
		(_texts, _textSeparation, _visibleZone, _fade, _movement, _toMove);
}

// ---
void QGAMES::ShowingInstructionsGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	if (!_texts.empty ())
		deleteTexts ();

	for (int i = 0; i < (int) _properties._texts.size (); i++)
		_texts.push_back (createTextForLine (_properties._texts [i]));

	reStartAllCounters ();
	reStartAllOnOffSwitches ();
	counter (_COUNTERTOMOVE) -> initialize (_properties._toMove, 0, true, true);

	_currentPosition = 0; // At the bottom...
	_lastPositionDrawn = false; // Until other thing was said...
}

// ---
void QGAMES::ShowingInstructionsGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	if (!onOffSwitch (_SWITCHTOMOVE) -> isOn ())
		return; // Not to move...

	if (counter (_COUNTERTOMOVE) -> isEnd ())
		_currentPosition += _properties._movement;

	// If the last line is totally out of the visible zone...
	if (_lastPositionDrawn)
	{
		onOffSwitch (_SWITCHTOMOVE) -> set (true); // Do not move anymore...
		notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this)); // and send a notification...
	}

	// Take into account that not to move can be set in other circunstances...
}

// ---
void QGAMES::ShowingInstructionsGameState::drawOn (QGAMES::Screen* s)
{
	QGAMES::GameState::drawOn (s);

	int sW = s -> visualWidth (); int sH = s -> visualHeight ();
	int lS = (int) (_properties._visibleZone.pos1 ().posY () * sH);
	int lI = (int) (_properties._visibleZone.pos2 ().posY () * sH); 
	int iP = lI  - _currentPosition;
	for (int i = 0; i < (int) _texts.size (); i++)
	{
		// Out of the visible zone?
		// Then blur little by little the intensity of the line...
		// or unblur depending the part of the screen the text is!
		int tZ = _texts [i] -> visualHeight ();
		if (iP < lS)
		{
			int vZ =  tZ + iP - lS; if (vZ < 0) vZ = 0;
			if (tZ == 0) tZ = (vZ == 0) ? 1 : vZ;
			_texts [i] -> setAlphaLevel (vZ * _properties._fade / tZ);
		}
		else
		if ((iP + tZ) > lI)
		{
			int vZ = tZ + iP - lI; if (vZ < 0) vZ = 0;
			if (tZ == 0) tZ = (vZ == 0) ? 1 : vZ;
			_texts [i] -> setAlphaLevel (255 - (vZ * _properties._fade / tZ));
		}
		else
			_texts [i] -> setAlphaLevel (_properties._fade);

		int hP = (int) ((_properties._visibleZone.pos1 ().posX () + 
			(_properties._visibleZone.width () * _properties._texts [i]._position)) * sW);
		if (_properties._texts [i]._reference == QGAMES::ShowingInstructionsGameState::Properties::TextLine::_CENTER)
			hP -= (_texts [i] -> visualLength () >> 1);
		else 
		if (_properties._texts [i]._reference == QGAMES::ShowingInstructionsGameState::Properties::TextLine::_RIGHT)
			hP -= _texts [i] -> visualLength ();

		_texts [i] -> setPosition (QGAMES::Position (__BD hP, __BD iP, __BD 0));
		if (iP > (lS -_texts [i] -> visualHeight ())) // Only draws what is visible...
			_texts [i] -> drawOn (s); 

		iP += _texts [i] -> visualHeight () + _properties._textSeparation;
	}

	_lastPositionDrawn = (iP < -lS);
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::ShowingInstructionsGameState::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::ShowingInstructionsGameState::_COUNTERTOMOVE, 0 /* to change when initialized. */, 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::ShowingInstructionsGameState::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::ShowingInstructionsGameState::_SWITCHTOMOVE, true));
}

// ---
QGAMES::ScoreObjectText* QGAMES::ShowingInstructionsGameState::createTextForLine 
	(const QGAMES::ShowingInstructionsGameState::Properties::TextLine& tL)
{
	// In the default implementation the type of the line is not taken into account...
	return (game () -> textBuilder () -> createText (tL._font, tL._text));
}

// ---
void QGAMES::ShowingInstructionsGameState::deleteTexts ()
{
	for (int i = 0; i < (int) _texts.size (); i++)
		delete (_texts [i]);
	_texts.clear ();
}

// ---
QGAMES::CountingDownGameState::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _formId (__QGAMES_GAMESTATECOUNTDOWNSTDBSCFORM__),
	  _beepSoundId (__QGAMES_BEEPWAVSOUND__),
	  _framesPerNumber (__QGAMES_GAMESTATECOUNTDOWNSTDFRAMES__),
	  _framesOverlapped (2),
	  _numbers (__QGAMES_GAMESTATECOUNTDOWNSTDNUMBERS__), // A number can be any thing remember!!
	  _position (Position (__BD 0.5, __BD 0.5, __BD 0)) // The center of the counting...
{
	if (prps.find (__QGAMES_GAMESTATEPROPATTRFORM__) != prps.end ()) 
		_formId = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRFORM__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRBEEPSOUND__) != prps.end ()) 
		_beepSoundId = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRBEEPSOUND__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRFRAMESPN__) != prps.end ()) 
		_framesPerNumber = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRFRAMESPN__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRFRAMESOVP__) != prps.end ()) 
		_framesOverlapped = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRFRAMESOVP__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRNUMBERS__) != prps.end ()) 
		_numbers = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRNUMBERS__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__) != prps.end ()) 
		_position = QGAMES::Position ((*prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__)).second);

	QGAMES::CountingDownGameState::Properties::Properties
		(_formId, _beepSoundId, _framesPerNumber, _framesOverlapped, _numbers, _position);
}

// ---
QGAMES::CountingDownGameState::CountingDownGameState (int id, 
		const QGAMES::CountingDownGameState::Properties& prps, QGAMES::GameState* nS)
	: QGAMES::GameState (id, QGAMES::Game::game (), nS),
	  _properties (),
	  _states ()
{
	setProperties (prps);
}

// ---
void QGAMES::CountingDownGameState::setProperties (const QGAMES::CountingDownGameState::Properties& prps)
{
	_properties = prps;

	_states.clear (); 
	_states.push_back (State (0, 0, 255, false)); // First number to the screen, but no sound at all...
}

// ---
void QGAMES::CountingDownGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	reStartAllCounters ();
	counter (_COUNTERONSCREEN) -> 
		initialize (QGAMES::Game::game () -> framesPerSecond () / 
			(_properties._framesPerNumber + _properties._framesOverlapped /** Because the next begin x frames in advance with the previous. */), 
				0, true, true);
	reStartAllOnOffSwitches ();

	_states.clear (); 
	_states.push_back (State (0, 0, 255, false)); 
	// First "number" in its minimal expression is on the screen already...

	game () -> sound (_properties._beepSoundId) -> play (__QGAMES_GAMESTATESOUNDCHANNEL__);
}

// ---
void QGAMES::CountingDownGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	// Nothing else to do in case it's finished...
	if (hasFinished ())
	{ 
		notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
		return;
	}

	if (counter (_COUNTERONSCREEN) -> isEnd ())
	{
		bool nToM = false;
		int s = (int) _states.size (); // The size of _states can change during the loop...
		for (int i = 0; i < s; i++) 
		{
			if (_states [i]._used == false) // Is the state still valid?
			{
				nToM = true;
				_states [i]._size++;
				// The fade is proportional to the potential total size...
				// and reverse proportional to the number of frames per number...
				_states [i]._fade = 255 - (240 * _states [i]._size / _properties._framesPerNumber);
				if (_states [i]._size == (_properties._framesPerNumber - _properties._framesOverlapped) && 
					_states [i]._number < _properties._numbers)
				{ 
					// When the previous state is about to disappear, 
					// and the number being shown is almost the last (1?)...
					_states.push_back (State (++_states [i]._number, 0, 255, false)); // ...creates a new state...
					game () -> sound (_properties._beepSoundId) -> play (__QGAMES_GAMESTATESOUNDCHANNEL__);
				}

				// When the maximal size has been reached...
				if (_states [i]._size == _properties._framesPerNumber) 
					_states [i]._used = true; // ...the element is not longer used!!
			}
		}

		if (!nToM) // If there is nothing to move...
			finish ();
	}
}

// ---
void QGAMES::CountingDownGameState::drawOn (QGAMES::Screen* s)
{
	QGAMES::GameState::drawOn (s);

	for (int i = ((int) _states.size () >= 0) ? (int) _states.size () - 1: 0; 
			i >= 0; i--) // The numbers are drawn from back to front...
	{
		if (!_states [i]._used)
		{
			QGAMES::Form* f = game () -> form (_properties._formId + _states [i]._size);
			QGAMES::Vector frS (__BD (f -> frameWidthForFrame (_states [i]._number)),
				__BD (f -> frameHeightForFrame (_states [i]._number)), __BD 0);
			QGAMES::Vector off (__BD (f -> frameXOffsetForFrame (_states [i]._number)),
				__BD (f -> frameYOffsetForFrame (_states [i]._number)), __BD 0);
			QGAMES::Position pos = QGAMES::Position (
				__BD s -> visualWidth () * _properties._position.posX (),
				__BD s -> visualHeight () * _properties._position.posY (),
				__BD 0) - (frS / 2) - off; // The position defined under the properties structure is relative...
			f -> drawOn (s, _states [i]._number, pos + s -> position (), _states [i]._fade);
		}
	}
}

// ---
void QGAMES::CountingDownGameState::onExit ()
{
	QGAMES::GameState::onExit ();

	game () -> sound (__QGAMES_BEEPWAVSOUND__) -> stop ();
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::CountingDownGameState::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::CountingDownGameState::_COUNTERONSCREEN, 0 /* to change when initialized. */, 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::CountingDownGameState::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch
		(QGAMES::CountingDownGameState::_SWITCHSTATEHASFINISHED, false));
}

// ---
QGAMES::ShowingHallOfFameGameState::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _fade (255),
	  _position (Position (__BD 0, __BD 0, __BD 0)), // Center position in a 2D screen...
	  _separation (5), // Separation in pixels among each entry
	  _font (__QGAMES_RAVIE24ORANGELETTERS__)
{
	if (prps.find (__QGAMES_GAMESTATEPROPATTRFADE__) != prps.end ()) 
		_fade = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRFADE__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRSEPARATION__) != prps.end ()) 
		_separation = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRSEPARATION__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__) != prps.end ()) 
		_position = QGAMES::Position ((*prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__)).second);
	if (prps.find (__QGAMES_GAMESTATEPROPATTRTFONT__) != prps.end ())
		_font = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRTFONT__)).second.c_str ());

	QGAMES::ShowingHallOfFameGameState::Properties::Properties (_fade, _position, _separation, _font);
}

// ---
QGAMES::ShowingHallOfFameGameState::ShowingHallOfFameGameState (int id, 
		const QGAMES::ShowingHallOfFameGameState::Properties& prps, QGAMES::GameState* nS)
	: QGAMES::GameState (id, QGAMES::Game::game (), nS),
	  _properties (prps),
	  _texts ()
{
	// Nothing else to do...
}

// ---
void QGAMES::ShowingHallOfFameGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	if (!_texts.empty ())
		deleteTexts ();
	
	std::multimap <std::string, QGAMES::SetOfOpenValues> hF;
	// The hall of fame is located under the configuration of any advanced arcade game
	// So it is necessary to unsure the game belongs to that type before going on!
	// Other case, the process can't continue...and nothing will be shown at all!
	QGAMES::AdvancedArcadeGame* g = dynamic_cast <QGAMES::AdvancedArcadeGame*> (game ());
	if (g) hF = orderHallOfFame (((QGAMES::AdvancedArcadeGame::Conf*) g -> configuration ()) -> hallOfFame ());
	
	for (std::multimap <std::string, QGAMES::SetOfOpenValues>::const_iterator i = hF.begin (); i != hF.end (); i++)
	{
		QGAMES::ScoreObjectText* tt = createTextFor ((*i).first, (*i).second);
		tt -> setOrientation (QGAMES::Vector (__BD 1, __BD 0, __BD 0)); // Not other orientation is allowed...
		_texts.push_back (tt);
	}
}

// ---
void QGAMES::ShowingHallOfFameGameState::drawOn (Screen* s)
{
	QGAMES::GameState::drawOn (s);

	// Text of hall of fame is also in 2D
	QGAMES::Position pos (
		__BD s -> visualWidth () * _properties._position.posX (),
		__BD s -> visualHeight () * _properties._position.posY (),
		__BD 0);
	for (int i = 0; i < (int) _texts.size (); i++)
	{
		_texts [i] -> setAlphaLevel (_properties._fade);
		_texts [i] -> drawOn (s, pos + s -> position ());
		pos += QGAMES::Vector (__BD 0, __BD (_properties._separation + _texts [i] -> visualHeight ()), __BD 0);
	}
}

// ---
QGAMES::ScoreObjectText* QGAMES::ShowingHallOfFameGameState::createTextFor (const std::string& eK, const QGAMES::SetOfOpenValues& sV)
{
	std::string txt = eK + // Includes the score if it's been defined only
		(sV.existOpenValue (__QGAMES_HALLOFFAMEATTRSCORE__) 
			? std::to_string (sV.openValue (__QGAMES_HALLOFFAMEATTRSCORE__).intValue ()) 
			: std::string (__NULL_STRING__));
	return (game () -> textBuilder () -> createText (_properties._font, txt));
}

// ---
void QGAMES::ShowingHallOfFameGameState::deleteTexts ()
{
	for (int i = 0; i < (int) _texts.size (); i++)
		delete (_texts [i]);
	_texts.clear ();
}

// ---
QGAMES::RollingFormsGameState::Properties::Properties (const std::map <std::string, std::string>& prps)
	: _forms (),
	  _delay (6),
	  _secondsOnScreen ((bdata)1),
	  _direction (Vector (__BD 1, __BD 0, __BD 0)),
	  _instances (1), // Just 1 instance so far by default...
	  _sequential (false)
{
	if (prps.find (__QGAMES_GAMESTATEPROPATTRFORMSLST__) != prps.end ()) 
		_forms = QGAMES::getElementsFromAsInt ((*prps.find (__QGAMES_GAMESTATEPROPATTRFORMSLST__)).second, ',');
	if (prps.find (__QGAMES_GAMESTATEPROPATTRDELAY__) != prps.end ()) 
		_delay = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRDELAY__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRSECONSCR__) != prps.end ()) 
		_secondsOnScreen = __BD (std::atof ((*prps.find (__QGAMES_GAMESTATEPROPATTRSECONSCR__)).second.c_str ()));
	if (prps.find (__QGAMES_GAMESTATEPROPATTRDIRECTION__) != prps.end ()) 
		_direction = QGAMES::Vector ((*prps.find (__QGAMES_GAMESTATEPROPATTRDIRECTION__)).second);
	if (prps.find (__QGAMES_GAMESTATEPROPATTRINSTANCES__) != prps.end ()) 
		_instances = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRINSTANCES__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRSEQUENTIAL__) != prps.end ()) 
		_sequential = QGAMES::toUpper (QGAMES::trim (std::string ((*prps.find (__QGAMES_GAMESTATEPROPATTRSEQUENTIAL__)).second))) == std::string (__YES_STRING__);

	QGAMES::RollingFormsGameState::Properties::Properties 
		(_forms, _delay, _secondsOnScreen, _direction, _instances, _sequential);
}

// ---
QGAMES::RollingFormsGameState::RollingFormsGameState (int id, 
		const QGAMES::RollingFormsGameState::Properties& prps, QGAMES::GameState* nS)
	: QGAMES::GameState (id, QGAMES::Game::game (), nS),
		_properties (),
		_presentationStatus (),
		_frmToPaint1 (-1), _frmToPaint2 (-1)
{
	setProperties (prps); // Just to create the forms also...
}

// ---
void QGAMES::RollingFormsGameState::setProperties (const QGAMES::RollingFormsGameState::Properties& prps)
{
	_properties = prps;
	
	createForms ();
}

// ---
void QGAMES::RollingFormsGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	createForms ();
	// The form status are recreated...so the presentation is ready to start again!

	reStartAllCounters ();
	reStartAllOnOffSwitches ();
	counter (_COUNTERTOMOVE) -> initialize (_properties._delay, 0, true, true);
	counter (_COUNTERTOSTARTNEXT) -> 
		initialize ((int) (_properties._secondsOnScreen * __BD game () -> framesPerSecond ()), 0, true, true);

	_frmToPaint1 = 0; _frmToPaint2 = -1;
	// There is always maximum two frame to draw.
	// At the very beginning of the presentation, the only form to draw is the first
	// When this one starts to disappear, the next one will start to appear...
}

// ---
void QGAMES::RollingFormsGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	if (counter (_COUNTERTOMOVE) -> isEnd ())
	{
		for (int i = 0; i < (int) _properties._forms.size (); i++)
		{
			// The maximum, minimum,... position depends on the form
			// The forms being rolled can have different sizes...
			QGAMES::bdata maxX, maxY, maxZ; maxX = maxY = maxZ = __BD 0;
			QGAMES::bdata minX, minY, minZ; minX = minY = minZ = __BD 0;
			int w  = _presentationStatus [i]._form -> width ();
			int h  = _presentationStatus [i]._form -> height ();
			int d  = _presentationStatus [i]._form -> depth ();
			QGAMES::Vector dim (__BD w, __BD h, __BD ((d < 0) ? 0 : d));
			for (int j = 0; j < (int) _presentationStatus [i]._positions.size (); j++)
			{
				QGAMES::Position posMax = _presentationStatus [i]._positions [j] + dim;
				QGAMES::Position posMin = _presentationStatus [i]._positions [j] - dim;
				if (posMax.posX () > maxX) maxX = posMax.posX ();
				if (posMax.posY () > maxY) maxY = posMax.posY ();
				if (posMax.posZ () > maxZ) maxZ = posMax.posZ ();
				if (posMin.posX () < minX) minX = posMin.posX ();
				if (posMin.posY () < minY) minY = posMin.posY ();
				if (posMin.posZ () < minZ) minZ = posMin.posZ ();
			}

			// Move the instances...
			for (int j = 0; j < _properties._instances; j++)
			{
				_presentationStatus [i]._positions [j] -= _properties._direction;

				QGAMES::bdata pX = _presentationStatus [i]._positions [j].posX ();
				QGAMES::bdata pY = _presentationStatus [i]._positions [j].posY ();
				QGAMES::bdata pZ = _presentationStatus [i]._positions [j].posZ ();
				QGAMES::bdata nPX = pX; QGAMES::bdata nPY = pY; QGAMES::bdata nPZ = pZ;

				if (pX < -w && _properties._direction.posX () > 0)
					nPX = maxX + __BD (w + pX); // pX is negative...
				if (pX > w && _properties._direction.posX () < 0)
					nPX = minX + __BD (pX - w);
				if (pY < -h && _properties._direction.posY () > 0)
					nPY = maxY + __BD (h + pY); // pY is negative...
				if (pY > h && _properties._direction.posY () < 0)
					nPY = minY + __BD (pY - h);
				if (pZ < -d && _properties._direction.posZ () > 0) 
					nPZ = maxZ + __BD (d + pZ); // pZ is negative...
				if (pZ > d && _properties._direction.posZ () < 0)  
					nPZ = minZ + __BD (pZ - d); 
				if (pZ < 0)
					nPZ =  __BD 0; // 2D

				_presentationStatus [i]._positions [j] = QGAMES::Position (nPX, nPY, nPZ);
			}
		}
	}

	if (!onOffSwitch (_SWITCHDOINGFRMFADE) -> isOn ())
	{
		if (counter (_COUNTERTOSTARTNEXT) -> isEnd ())
			onOffSwitch (_SWITCHDOINGFRMFADE) -> set (true);
	}
	else
	{
		for (int i = 0; i < (int) _properties._forms.size (); i++)
		{
			if (_presentationStatus [i]._status == 
					QGAMES::RollingFormsGameState::PresentationStatus::_VISIBLE)
			{
				_frmToPaint1 = i; _frmToPaint2 = (i == ((int) _properties._forms.size () - 1)) ? 0 : (i + 1); 
				_presentationStatus [_frmToPaint1]._status = 
					QGAMES::RollingFormsGameState::PresentationStatus::_FADEOUT;
				_presentationStatus [_frmToPaint2]._status = 
					QGAMES::RollingFormsGameState::PresentationStatus::_FADEIN;
			}
			else
			if (_presentationStatus [i]._status == 
					QGAMES::RollingFormsGameState::PresentationStatus::_FADEOUT)
			{
				if (--_presentationStatus [i]._fadeGrade == 0)
					_presentationStatus [i]._status = 
						QGAMES::RollingFormsGameState::PresentationStatus::_INVISIBLE;
			}
			else
			if (_presentationStatus [i]._status == 
					QGAMES::RollingFormsGameState::PresentationStatus::_FADEIN)
			{
				if (++_presentationStatus [i]._fadeGrade == 255)
				{
					_frmToPaint1 = i; _frmToPaint2 = -1;
					_presentationStatus [i]._status = 
						QGAMES::RollingFormsGameState::PresentationStatus::_VISIBLE;
					onOffSwitch (_SWITCHDOINGFRMFADE) -> set (false);
				}
			}
		}
	}
}

// ---
void QGAMES::RollingFormsGameState::drawOn (QGAMES::Screen* s)
{
	QGAMES::GameState::drawOn (s);

	if (_frmToPaint2 != -1)
	{
		for (int j = 0; j < _properties._instances; j++)
			_presentationStatus [_frmToPaint2]._form -> 
				drawOn (s, 0, // First frame...
							_presentationStatus [_frmToPaint2]._positions [j] + s -> position (), // Fix in the screen...
							_presentationStatus [_frmToPaint2]._fadeGrade);
	}

	if (_frmToPaint1 != -1)
	{
		for (int j = 0; j < _properties._instances; j++)
			_presentationStatus [_frmToPaint1]._form -> 
				drawOn (s, 0, // First frame...
							_presentationStatus [_frmToPaint1]._positions [j] + s -> position (), // Fix in the screen...
							_presentationStatus [_frmToPaint1]._fadeGrade);
	}
}

// ---
void QGAMES::RollingFormsGameState::addoptStatusFrom (QGAMES::RollingFormsGameState* st)
{
	assert (st);

	if (_properties.similarTo (st -> _properties))
		_presentationStatus = st -> _presentationStatus;
}

// ---
void QGAMES::RollingFormsGameState::createForms ()
{
	_presentationStatus = 
		std::vector <QGAMES::RollingFormsGameState::PresentationStatus> 
			((int) _properties._forms.size (), PresentationStatus ());

	QGAMES::Position pPos = QGAMES::Position::_cero;
	for (int i = 0; i < (int) _properties._forms.size (); i++)
	{
		if (!_properties._sequential)
			pPos = QGAMES::Position::_cero;
		QGAMES::Form* frm = game () -> form (_properties._forms [i]);
		_presentationStatus [i]._form = frm;
		_presentationStatus [i]._fadeGrade = (i == 0) ? 255 : 0;
		_presentationStatus  [i]._status = (i == 0) 
			? QGAMES::RollingFormsGameState::PresentationStatus::_VISIBLE
			: QGAMES::RollingFormsGameState::PresentationStatus::_INVISIBLE;
		_presentationStatus [i]._positions = 
			std::vector <QGAMES::Vector> (_properties._instances, QGAMES::Vector::_cero); 
		for (int j = 0; j < _properties._instances; j++)
		{
			_presentationStatus [i]._positions [j] = pPos;
			pPos += QGAMES::Vector (__BD frm -> frameWidth ()  * _properties._direction.posX (), 
									__BD frm -> frameHeight () * _properties._direction.posY (), 
									__BD frm -> frameDepth ()  * _properties._direction.posZ ());
		}
	}
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::RollingFormsGameState::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::RollingFormsGameState::_COUNTERTOMOVE, 0 /** Set at initialization time. */, 0, true, true));
	addCounter (new QGAMES::RandomCounter 
		(QGAMES::RollingFormsGameState::_COUNTERTOSTARTNEXT, 1 /** Set at initialization time. */, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::RollingFormsGameState::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::RollingFormsGameState::_SWITCHDOINGFRMFADE, false));
}

// ---
QGAMES::CreditTitlesGameState::Properties::Properties 
	(const std::vector <std::string>& tts, const std::map <std::string, std::string>& prps)
	: _titles (),
	  _maxLettersPerTitle (-1),
	  _position (Vector::_cero),
	  _reference (_CENTER),
	  _secondsOnScreen (__BD 1),
	  _font (__QGAMES_RAVIE24ORANGELETTERS__)
{
	_titles = tts; // Assign the titles...

	if (prps.find (__QGAMES_GAMESTATEPROPATTRLPERTITLE__) != prps.end ())
		_maxLettersPerTitle = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRLPERTITLE__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__) != prps.end ()) 
		_position = QGAMES::Position ((*prps.find (__QGAMES_GAMESTATEPROPATTRPOSITION__)).second);
	if (prps.find (__QGAMES_GAMESTATEPROPATTRPOSREF__) != prps.end ()) 
		_reference = QGAMES::CreditTitlesGameState::Properties::PosReference
			(std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRPOSREF__)).second.c_str ())); // like enum...
	if (prps.find (__QGAMES_GAMESTATEPROPATTRSECONSCR__) != prps.end ()) 
		_secondsOnScreen = __BD (std::atof ((*prps.find (__QGAMES_GAMESTATEPROPATTRSECONSCR__)).second.c_str ()));
	if (prps.find (__QGAMES_GAMESTATEPROPATTRTFONT__) != prps.end ())
		_font = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRTFONT__)).second.c_str ());

	QGAMES::CreditTitlesGameState::Properties::Properties 
		(_titles, _maxLettersPerTitle, _position, _reference, _secondsOnScreen, _font);
}

// ---
QGAMES::CreditTitlesGameState::CreditTitlesGameState (int id, 
		const QGAMES::CreditTitlesGameState::Properties& prps, QGAMES::GameState* nS)
	: QGAMES::GameState (id, QGAMES::Game::game (), nS),
		_properties (),
		_titleStatus (),
		_titleToPaint (-1)
{
	setProperties (prps);
}

// ---
void QGAMES::CreditTitlesGameState::setProperties (const QGAMES::CreditTitlesGameState::Properties& prps)
{
	_properties = prps;

	createTitles ();
}

// ---
void QGAMES::CreditTitlesGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	createTitles ();
	_titleStatus [0] -> _fadeGrade = 255;
	_titleStatus [0] -> _status = QGAMES::CreditTitlesGameState::TitleStatus::_VISIBLE;
	// The status are recreated...
	// The presentation starts back!

	reStartAllCounters ();
	reStartAllOnOffSwitches ();
	counter (_COUNTERTEXTATSCREEN) -> 
		initialize ((int) (_properties._secondsOnScreen * __BD game () -> framesPerSecond ()), 0, true, true);

	_titleToPaint = 0; 
}

// ---
void QGAMES::CreditTitlesGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	if (!onOffSwitch (_SWITCHDOINGTEXTFADE) -> isOn ())
	{
		if (counter (_COUNTERTEXTATSCREEN) -> isEnd ())
			onOffSwitch (_SWITCHDOINGTEXTFADE) -> set (true);
	}
	else
	{
		for (int i = 0; i < (int) _titleStatus.size (); i++)
		{
			if (_titleStatus [i] -> _status == 
					QGAMES::CreditTitlesGameState::TitleStatus::_VISIBLE)
			{
				_titleToPaint = i; 
				if (i != ((int) _titleStatus.size () - 1)) 
					_titleStatus [_titleToPaint] -> _status = 
						QGAMES::CreditTitlesGameState::TitleStatus::_FADEOUT;
			}
			else
			if (_titleStatus [i] -> _status == 
					QGAMES::CreditTitlesGameState::TitleStatus::_FADEOUT)
			{
				if (--_titleStatus [i] -> _fadeGrade == 0) // Has disappear...
				{
					_titleStatus [i] -> _status = 
						QGAMES::CreditTitlesGameState::TitleStatus::_INVISIBLE;
					_titleToPaint = i + 1;
					_titleStatus [_titleToPaint] -> _status = 
						QGAMES::CreditTitlesGameState::TitleStatus::_VISIBLE;
					_titleStatus [_titleToPaint] -> _fadeGrade = 255; // Visible back...
					onOffSwitch (_SWITCHDOINGTEXTFADE) -> set (false); // Starts back with the next title...
				}
			}
		}
	}
}

// ---
void QGAMES::CreditTitlesGameState::drawOn (QGAMES::Screen* s)
{
	QGAMES::GameState::drawOn (s);

	if (_titleToPaint == -1)
		return;

	for (int i = 0; i < (int) _titleStatus [_titleToPaint] -> _texts.size (); i++)
	{
		QGAMES::Position pos
			((__BD s -> visualWidth () * _properties._position.posX ()) - _titleStatus [_titleToPaint] -> _positions [i].posX (),
			 (__BD s -> visualHeight () * _properties._position.posY ()) - _titleStatus [_titleToPaint] -> _positions [i].posY (),
			 __BD 0); // The position defined is a relative position...
		_titleStatus [_titleToPaint] -> _texts [i] -> setAlphaLevel 
			(_titleStatus [_titleToPaint] -> _fadeGrade);
		_titleStatus [_titleToPaint] -> _texts [i] -> drawOn (s, pos);
	}
}

// ---
QGAMES::ScoreObjectText* QGAMES::CreditTitlesGameState::createText (const std::string& txt)
{
	return (game () -> textBuilder () -> createText (_properties._font, txt));
}

// ---
void QGAMES::CreditTitlesGameState::createTitles ()
{
	if (!_titleStatus.empty ())
		destroyTitles ();

	for (int i = 0; i < (int) _properties._titles.size (); i++)
	{
		QGAMES::ScoreObjectText* tt = createText (_properties._titles [i]); 
		tt -> setOrientation (QGAMES::Vector (__BD 1, __BD 0, __BD 0)); // No other orientation is allowed
		_titleStatus.push_back (
			new QGAMES::CreditTitlesGameState::TitleStatus (
				tt,
				_properties._maxLettersPerTitle, 
				_properties._reference,
				(i == 0) ? 255 : 0,
				((i == 0) ? QGAMES::CreditTitlesGameState::TitleStatus::Status::_VISIBLE
							: QGAMES::CreditTitlesGameState::TitleStatus::Status::_INVISIBLE)));
	}
}

// ---
void QGAMES::CreditTitlesGameState::destroyTitles ()
{
	for (int i = 0; i < (int) _titleStatus.size (); i++)
		delete (_titleStatus [i]); // It will delete the score objects...
	_titleStatus.clear ();
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::CreditTitlesGameState::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::CreditTitlesGameState::_COUNTERTEXTATSCREEN, 0 /** Set at initialization time. */, 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::CreditTitlesGameState::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::CreditTitlesGameState::_SWITCHDOINGTEXTFADE, false));
}

// ---
QGAMES::CreditTitlesGameState::TitleStatus::TitleStatus (QGAMES::ScoreObjectText* txt, 
		int mLL, QGAMES::CreditTitlesGameState::Properties::PosReference adj,
		int fG, QGAMES::CreditTitlesGameState::TitleStatus::Status st)
	: _texts (), // Adjusted later...
	  _positions (), // Adjusted later...
	  _fadeGrade (fG),
	  _status (st)
{
	assert (txt);

	// When the code reaches this point there must be a score object already created and valid
	// But maybe with not the right length and obviously not in the right position
	// Both things should be arranged if needed!

	// Split the original text into many
	// because its length is bigger than the maximum per line allowed
	QGAMES::ScoreObjectText* nTxt = NULL;
	std::string str = txt -> text (); std::string nStr (__NULL_STRING__);
	do
	{
		if ((int) str.length () > mLL) 
			{ nStr = str.substr (mLL); str  = str.substr (0, mLL); }
		else nStr = std::string (__NULL_STRING__);

		QGAMES::trim (str); // The different should not include include spaces, but... 
		if (str == std::string (__NULL_STRING__)) str = std::string (" "); // ..it can be totally empty...
		txt -> setMaxTextLength (str.length ());
		txt -> setText (str);
		_texts.push_back (txt);

		nTxt = NULL;
		if (nStr != std::string (__NULL_STRING__))
		{
			nTxt = dynamic_cast <QGAMES::ScoreObjectText*> (txt -> clone ());
			assert (nTxt); // Just in case...
			nTxt -> setMaxTextLength (nStr.length ());
			nTxt -> setText (nStr);
		}

		str = nStr;	txt = nTxt;
	} while (txt);

	// Now it is time to fix the different positions
	// according with the parameters received
	// Credit titles are in 2D...
	int k = ((int) _texts.size ()) >> 1;
	for (int i = 0; i < (int) _texts.size (); i++, k--)
	{
		int w = _texts [i] -> visualLength ();
		int h = _texts [i] -> visualHeight ();

		QGAMES::Position pos = QGAMES::Position::_cero;
		QGAMES::Vector tAdj = __BD k * QGAMES::Vector (__BD 0, __BD (h + 10), __BD 0);
		switch (adj)
		{
			case QGAMES::CreditTitlesGameState::Properties::PosReference::_CENTER:
				pos = QGAMES::Position (__BD (w >> 1), __BD (h >> 1), __BD 0);
				break;

			case QGAMES::CreditTitlesGameState::Properties::PosReference::_BOTTONRIGHT:
				pos = QGAMES::Position (__BD w, __BD h, __BD 0);
				break;

			default:
				break;
		}

		pos += tAdj;
		_positions.push_back (pos);
	}
}

// ---
QGAMES::CreditTitlesGameState::TitleStatus::~TitleStatus ()
{
	for (int i = 0; i < (int) _texts.size (); i++)
		delete (_texts [i]);
	_texts.clear ();
}

// ---
QGAMES::SelectionOptionsGameState::SelectionOptionsGameState (int id, 
			const QGAMES::SelectionOptionsGameState::Menus& ms, QGAMES::SelectionOptionsGameState::Handler* hdr, 
			QGAMES::GameState* ns)
	: QGAMES::GameState (id, Game::game (), ns),
	  _menus (ms),
	  _handler (hdr),
	  _lastOptionSelected (-1),
	  _informationText (NULL), // Nothing by default...
	  _currentMenu (QGAMES::SelectionOptionsGameState::Menu (-1)),
	  _currentGUISystem (NULL),
	  _connectionSelected (Menu::Connection (-1)) // None so far...
{
	assert (_handler);
	assert (!_menus.empty ());

	_handler -> _owner = this;
}

// ---
QGAMES::SelectionOptionsGameState::~SelectionOptionsGameState ()
{ 
	delete (_handler);
	_handler = NULL;

	deleteCurrentGUISystem ();

	delete (_informationText);
	_informationText = NULL;
}

// ---
void QGAMES::SelectionOptionsGameState::setMenus (const QGAMES::SelectionOptionsGameState::Menus& ms)
{
	assert (!ms.empty ());

	if (_currentGUISystem != NULL)
		unObserve (_currentGUISystem -> mainWidget ());
	
	_menus = ms;

	_lastOptionSelected = -1;

	_currentMenu = ((*_menus.begin ()).second);
	_currentGUISystem = game () -> guiSystemBuilder () -> system (_currentMenu._guiSystemId);
	assert (dynamic_cast <QGAMES::SelectionOptionsGameState::MenuControlWidget*> 
		(_currentGUISystem -> mainWidget ()));

	initializeGUISystem ();

	observe (_currentGUISystem -> mainWidget ());
}

// ---
void QGAMES::SelectionOptionsGameState::showInformationText (const std::string& eTxt)
{
	if(_informationText == NULL)
		_informationText = createEmptyInformationText ();

	_informationText -> setMaxTextLength (eTxt.length ());
	_informationText -> setPosition 
		(QGAMES::Position (__BD 15, __BD game () -> mainScreen () -> visualHeight () - 30, __BD 0));
	_informationText -> setTextDirection (QGAMES::ScoreObjectText::TextDirection::_HORIZONTALLTOR);
	_informationText -> setText (eTxt);
}

// ---
void QGAMES::SelectionOptionsGameState::setHandler (QGAMES::SelectionOptionsGameState::Handler* hd)
{
	delete (_handler);
	_handler = hd;
	_handler -> _owner = this;
}

// ---
void QGAMES::SelectionOptionsGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	reStartAllCounters ();
	reStartAllOnOffSwitches ();

	setMenus (_menus); // Reset the menus...
}

// ---
void QGAMES::SelectionOptionsGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	// Time to exit the state?
	if (onOffSwitch (_SWITCHFINISH) -> isOn ())
	{
		onOffSwitch (_SWITCHFINISH) -> set (false);
		notify (QGAMES::Event (__QGAMES_GAMENESTEDSTATEFINISHED__, this));
	}

	// Time to rebuilt the current GUI system...
	if (onOffSwitch (_SWITCHREBUILTMENU) -> isOn ())
	{
		onOffSwitch (_SWITCHREBUILTMENU) -> set (false);
		initializeGUISystem (); // Regenerate the values, just in case...
	}

	// Blinking information text
	if (counter (_COUNTERTOBLINKTEXT) -> isEnd ()) 
		counter (_COUNTERBLINKINGTEXT) -> isEnd ();

	// If a connection was selected previously...
	if (_connectionSelected._menuId != -1)
	{
		deleteCurrentGUISystem ();

		QGAMES::SelectionOptionsGameState::Menus::const_iterator j = _menus.find (_connectionSelected._menuId);
		if (j != _menus.end ())
		{
			_currentMenu = (*j).second;
			_currentGUISystem = game () -> guiSystemBuilder () -> system (_currentMenu._guiSystemId);
			assert (dynamic_cast <QGAMES::SelectionOptionsGameState::MenuControlWidget*> 
				(_currentGUISystem -> mainWidget ())); // Just in case...
			initializeGUISystem ();

			observe (_currentGUISystem -> mainWidget ()); 
			if (_connectionSelected._firstFocusedWidgetId != -1)
				((QGAMES::CompositeWidget*) _currentGUISystem -> mainWidget ()) -> 
						widget (_connectionSelected._firstFocusedWidgetId) -> setFocus (true); 
		}

		_connectionSelected = QGAMES::SelectionOptionsGameState::Menu::Connection (-1); // None again...
	}
}

// ---
void QGAMES::SelectionOptionsGameState::drawOn (QGAMES::Screen* scr)
{
	QGAMES::GameState::drawOn (scr);

	if (_informationText != NULL)
	{
		_informationText -> setAlphaLevel (_FADEBLINKING [counter (_COUNTERBLINKINGTEXT) -> value ()]);
		_informationText -> drawOn (scr);
	}
}

// ---
void QGAMES::SelectionOptionsGameState::onExit ()
{
	QGAMES::GameState::onExit ();

	deleteCurrentGUISystem ();
}

// ---
void QGAMES::SelectionOptionsGameState::processEvent (const Event& evnt)
{
	QGAMES::GameState::processEvent (evnt);

	if (evnt.code () == __QGAMES_WIDGETSELECTEDEVENT__)
	{
		QGAMES::Widget* wdt = (QGAMES::Widget*) evnt.data ();
		
		_lastOptionSelected = wdt -> id ();
		// Something special to be done?
		switch (_handler -> whenWidgetSelected (wdt))
		{
			case -1: // By default nothing has to be done...
				break;
			case -2: // The menu has to be rebuilt...
				onOffSwitch (_SWITCHREBUILTMENU) -> set (true);
				break;
			default: // Any other number will mean, to exit the menu...
				onOffSwitch (_SWITCHFINISH) -> set (true);
				break;
		}

		QGAMES::SelectionOptionsGameState::Menu::Connections::const_iterator i = 
			_currentMenu._connections.find (wdt -> id ()); // Any menu connected?...
		if (i != _currentMenu._connections.end ())
			_connectionSelected = (*i).second; 
		// The change of menu can't be done here, as other events can be being processed
	}
}

// ---
void QGAMES::SelectionOptionsGameState::deleteCurrentGUISystem ()
{
	if (_currentGUISystem)
	{
		finalizeGUISystem ();
		unObserve (_currentGUISystem -> mainWidget ());
	}

	delete (_currentGUISystem);
	_currentGUISystem = NULL;
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::SelectionOptionsGameState::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::SelectionOptionsGameState::_COUNTERTOBLINKTEXT, 2, 0, true, true));
	addCounter (new QGAMES::Counter 
		(QGAMES::SelectionOptionsGameState::_COUNTERBLINKINGTEXT, 20, 0, true, true));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::SelectionOptionsGameState::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::SelectionOptionsGameState::_SWITCHFINISH, false));
	addOnOffSwitch (new QGAMES::OnOffSwitch
		(QGAMES::SelectionOptionsGameState::_SWITCHREBUILTMENU, false));
}

std::string QGAMES::StandardInitialSelectionOptionsGameState::_CONFFILENAME = "Game.cfg";

// ---
QGAMES::StandardInitialSelectionOptionsGameState::StandardInitialSelectionOptionsGameState 
		(int id, QGAMES::GameState* ns, const Menus& m, QGAMES::SelectionOptionsGameState::Handler* h)
	: QGAMES::SelectionOptionsGameState (id, m, h, ns),
	  _configuration (NULL),
	  _keycodesConverter (), _keyCodesConverterInv ()
{
	// The key codes...
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_Q] =  0; _keycodesConverter [ 0] = QGAMES::KeyCode::QGAMES_Q; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_W] =  1; _keycodesConverter [ 1] = QGAMES::KeyCode::QGAMES_W; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_E] =  2; _keycodesConverter [ 2] = QGAMES::KeyCode::QGAMES_E; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_R] =  3; _keycodesConverter [ 3] = QGAMES::KeyCode::QGAMES_R;
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_I] =  4; _keycodesConverter [ 4] = QGAMES::KeyCode::QGAMES_I; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_Y] =  5; _keycodesConverter [ 5] = QGAMES::KeyCode::QGAMES_Y; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_U] =  6; _keycodesConverter [ 6] = QGAMES::KeyCode::QGAMES_U; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_I] =  7; _keycodesConverter [ 7] = QGAMES::KeyCode::QGAMES_I; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_O] =  8; _keycodesConverter [ 8] = QGAMES::KeyCode::QGAMES_O; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_P] =  9; _keycodesConverter [ 9] = QGAMES::KeyCode::QGAMES_P; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_A] = 10; _keycodesConverter [10] = QGAMES::KeyCode::QGAMES_A;
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_S] = 11; _keycodesConverter [11] = QGAMES::KeyCode::QGAMES_S;
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_D] = 12; _keycodesConverter [12] = QGAMES::KeyCode::QGAMES_D; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_F] = 13; _keycodesConverter [13] = QGAMES::KeyCode::QGAMES_F; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_G] = 14; _keycodesConverter [14] = QGAMES::KeyCode::QGAMES_G; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_H] = 15; _keycodesConverter [15] = QGAMES::KeyCode::QGAMES_H; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_J] = 16; _keycodesConverter [16] = QGAMES::KeyCode::QGAMES_J; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_K] = 17; _keycodesConverter [17] = QGAMES::KeyCode::QGAMES_K; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_L] = 18; _keycodesConverter [18] = QGAMES::KeyCode::QGAMES_L; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_Z] = 19; _keycodesConverter [19] = QGAMES::KeyCode::QGAMES_Z; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_X] = 20; _keycodesConverter [20] = QGAMES::KeyCode::QGAMES_X; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_C] = 21; _keycodesConverter [21] = QGAMES::KeyCode::QGAMES_C; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_V] = 22; _keycodesConverter [22] = QGAMES::KeyCode::QGAMES_V; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_B] = 23; _keycodesConverter [23] = QGAMES::KeyCode::QGAMES_B; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_N] = 24; _keycodesConverter [24] = QGAMES::KeyCode::QGAMES_N; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_M] = 25; _keycodesConverter [25] = QGAMES::KeyCode::QGAMES_M; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_1] = 26; _keycodesConverter [26] = QGAMES::KeyCode::QGAMES_1; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_2] = 27; _keycodesConverter [27] = QGAMES::KeyCode::QGAMES_2; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_3] = 28; _keycodesConverter [28] = QGAMES::KeyCode::QGAMES_3; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_4] = 29; _keycodesConverter [29] = QGAMES::KeyCode::QGAMES_4; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_5] = 30; _keycodesConverter [30] = QGAMES::KeyCode::QGAMES_5; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_6] = 31; _keycodesConverter [31] = QGAMES::KeyCode::QGAMES_6; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_7] = 32; _keycodesConverter [32] = QGAMES::KeyCode::QGAMES_7; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_8] = 33; _keycodesConverter [33] = QGAMES::KeyCode::QGAMES_8; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_9] = 34; _keycodesConverter [34] = QGAMES::KeyCode::QGAMES_9; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_0] = 35; _keycodesConverter [35] = QGAMES::KeyCode::QGAMES_0; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_SPACE] = 36; _keycodesConverter [36] = QGAMES::KeyCode::QGAMES_SPACE; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_UP] = 37; _keycodesConverter [37] = QGAMES::KeyCode::QGAMES_UP; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_DOWN] = 38; _keycodesConverter [38] = QGAMES::KeyCode::QGAMES_DOWN; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_LEFT] = 39; _keycodesConverter [39] = QGAMES::KeyCode::QGAMES_LEFT; 
	_keyCodesConverterInv [QGAMES::KeyCode::QGAMES_RIGHT] = 0; _keycodesConverter [40] = QGAMES::KeyCode::QGAMES_RIGHT; 
}

// ---
void QGAMES::StandardInitialSelectionOptionsGameState::onEnter ()
{
	QGAMES::AdvancedArcadeGame* g = dynamic_cast <QGAMES::AdvancedArcadeGame*> (game ());
	// Nothing if possible if the game doesn't belong to this type...
	// It should but as the game is created by the user a bad creation could be done!
	if (!g) 
		return; 

	// Creates a copy of the configuration to operate over it
	_configuration = dynamic_cast <QGAMES::AdvancedArcadeGame::Conf*> (g -> configuration () -> clone ());
	if (!_configuration)
		return; // Has to be valid...
	// Clean it from temporal data...
	_configuration -> cleanTemporalData ();
	g -> setCurrentPlayer (1); // Always starts from the beginning...
	g -> setSeconds (0); // No time happened...

	QGAMES::SelectionOptionsGameState::onEnter ();
}

// ---
void QGAMES::StandardInitialSelectionOptionsGameState::onExit ()
{
	// Bad initial creation...
	// Maybe the type of the game is not what it must be
	if (!_configuration)
		return;

	// Then the GUI system is deleted...
	QGAMES::SelectionOptionsGameState::onExit ();

	// Saves the game state...
	game () -> setConfiguration (_configuration);

	// It is not necessary to delete the configuration
	// Now it is part of the game...
}

// ---
void QGAMES::StandardInitialSelectionOptionsGameState::initializeGUISystem ()
{
	assert (currentGUISystem ());

	// When exit the widget visualized if the main one (or it should be)
	QGAMES::CompositeWidget* mWdt = 
		dynamic_cast <QGAMES::CompositeWidget*> (currentGUISystem () -> mainWidget ());
	assert (mWdt); // It has to be...

	if (mWdt -> id () == __QGAMES_STDGUISYSTEMMAINWIDGET__)
	{
		((QGAMES::IntroWidget*) mWdt -> widget (__QGAMES_STDGUISYSTEMNUMBERPLYWDT__)) ->
			setText (std::to_string (_configuration -> numberPlayers ()));
		((QGAMES::IteratingLabelWidget*) mWdt -> widget (__QGAMES_STDGUISYSTEMTYPECTRLWDT__)) ->
			setCurrentValue (_configuration -> useJoystick () ? 0 : 1);
		((QGAMES::IteratingLabelWidget*) mWdt -> widget (__QGAMES_STDGUISYSTEMLEVELDIFFWDT__)) ->
			setCurrentValue (_configuration -> difficulty ());
		((QGAMES::IteratingLabelWidget*) mWdt -> widget (__QGAMES_STDGUISYSTEMMUSICONWDT__)) ->
			setCurrentValue (_configuration -> musicOn () ? 0 : 1);
	}
	else 
	if (mWdt -> id () == __QGAMES_STDGUISYSTEMMAINCONFWIDGET__)
	{
		std::vector <int> controlKeys = _configuration -> controlKeys ();
		assert (controlKeys.size () >= 7);
		((QGAMES::IteratingLabelWidget*) 
			(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYLEFTWDT__))) -> 
				setCurrentValue (_keyCodesConverterInv [(QGAMES::KeyCode) controlKeys [__QGAMES_KEYLEFT__]]);
		((QGAMES::IteratingLabelWidget*) 
			(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYRIGHTWDT__))) -> 
				setCurrentValue (_keyCodesConverterInv [(QGAMES::KeyCode) controlKeys [__QGAMES_KEYRIGHT__]]);
		((QGAMES::IteratingLabelWidget*) 
			(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYUPWDT__))) -> 
				setCurrentValue (_keyCodesConverterInv [(QGAMES::KeyCode) controlKeys [__QGAMES_KEYUP__]]);
		((QGAMES::IteratingLabelWidget*) 
			(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYDOWNWDT__))) -> 
				setCurrentValue (_keyCodesConverterInv [(QGAMES::KeyCode) controlKeys [__QGAMES_KEYDOWN__]]);
		((QGAMES::IteratingLabelWidget*) 
			(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYFIGHTWDT__))) -> 
				setCurrentValue (_keyCodesConverterInv [(QGAMES::KeyCode) controlKeys [__QGAMES_KEYFIRE__]]);
		((QGAMES::IteratingLabelWidget*) 
			(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYPAUSEWDT__))) -> 
				setCurrentValue (_keyCodesConverterInv [(QGAMES::KeyCode) controlKeys [__QGAMES_KEYPAUSE__]]);
		((QGAMES::IteratingLabelWidget*) 
			(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYSAVEWDT__))) -> 
				setCurrentValue (_keyCodesConverterInv [(QGAMES::KeyCode) controlKeys [__QGAMES_KEYSAVE__]]);
	}
}

// ---
void QGAMES::StandardInitialSelectionOptionsGameState::finalizeGUISystem ()
{
	assert (currentGUISystem ());

	// Assign the variable elements to the elements in the configuration
	// When exit the widget visualized if the main one (or it should be)
	QGAMES::CompositeWidget* mWdt = 
		dynamic_cast <QGAMES::CompositeWidget*> (currentGUISystem () -> mainWidget ());
	assert (mWdt); // It has to be...

	if (mWdt -> id () == __QGAMES_STDGUISYSTEMMAINWIDGET__)
	{
		_configuration -> setNumberPlayers (std::atoi (((QGAMES::IntroWidget*) 
			mWdt -> widget (__QGAMES_STDGUISYSTEMNUMBERPLYWDT__)) -> text ().c_str ()));
		_configuration -> setJoystick (((QGAMES::IteratingLabelWidget*) 
			mWdt -> widget (__QGAMES_STDGUISYSTEMTYPECTRLWDT__)) -> currentValue () == 0);
		_configuration -> setDifficulty (((QGAMES::IteratingLabelWidget*) 
			mWdt -> widget (__QGAMES_STDGUISYSTEMLEVELDIFFWDT__)) -> currentValue ());
		_configuration -> setMusic (((QGAMES::IteratingLabelWidget*) 
			mWdt -> widget (__QGAMES_STDGUISYSTEMMUSICONWDT__)) -> currentValue () == 0);
	}
	else 
	if (mWdt -> id () == __QGAMES_STDGUISYSTEMMAINCONFWIDGET__)
	{
		std::vector <int> controlKeys = _configuration -> controlKeys (); // With the same number of elements...
		controlKeys [__QGAMES_KEYLEFT__] = 
			_keycodesConverter [((QGAMES::IteratingLabelWidget*) 
				(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYLEFTWDT__))) -> currentValue ()];
		controlKeys [__QGAMES_KEYRIGHT__] = 
			_keycodesConverter [((QGAMES::IteratingLabelWidget*) 
				(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYRIGHTWDT__))) -> currentValue ()];
		controlKeys [__QGAMES_KEYUP__] = 
			_keycodesConverter [((QGAMES::IteratingLabelWidget*) 
				(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYUPWDT__))) -> currentValue ()];
		controlKeys [__QGAMES_KEYDOWN__] = 
			_keycodesConverter [((QGAMES::IteratingLabelWidget*) 
				(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYDOWNWDT__))) -> currentValue ()];
		controlKeys [__QGAMES_KEYFIRE__] = 
			_keycodesConverter [((QGAMES::IteratingLabelWidget*) 
				(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYFIGHTWDT__))) -> currentValue ()];
		controlKeys [__QGAMES_KEYPAUSE__] = 
			_keycodesConverter [((QGAMES::IteratingLabelWidget*) 
				(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYPAUSEWDT__))) -> currentValue ()];
		controlKeys [__QGAMES_KEYSAVE__] = 
			_keycodesConverter [((QGAMES::IteratingLabelWidget*) 
				(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYSAVEWDT__))) -> currentValue ()];
		_configuration -> setControlKeys (controlKeys);
	}
}

// ---
QGAMES::SelectionOptionsGameState::Menus QGAMES::StandardInitialSelectionOptionsGameState::createStandardMenus ()
{
	QGAMES::SelectionOptionsGameState::Menus result;

	// Menu 1: Main Options
	QGAMES::SelectionOptionsGameState::Menu::Connections conns1;
	conns1.insert (QGAMES::SelectionOptionsGameState::Menu::Connections::value_type 
		(__QGAMES_STDGUISYSTEMCONFKEYSWDT__, __QGAMES_STDGUISYSTEMCONFKEYSID__));
	QGAMES::SelectionOptionsGameState::Menu menu1 (__QGAMES_STDGUISYSTEMID__, 
			QGAMES::Position (__BD 10, __BD 10, __BD 0), conns1);
	result.insert (QGAMES::SelectionOptionsGameState::Menus::value_type (__QGAMES_STDGUISYSTEMID__, menu1));

	// Menu 2: Configuration keys
	QGAMES::SelectionOptionsGameState::Menu::Connections conns2;
	conns2.insert (QGAMES::SelectionOptionsGameState::Menu::Connections::value_type 
		(__QGAMES_STDGUISYSTEMCONFKEYBACKWDT__, __QGAMES_STDGUISYSTEMID__));
	QGAMES::SelectionOptionsGameState::Menu menu2 (__QGAMES_STDGUISYSTEMCONFKEYSID__, 
			QGAMES::Position (__BD 10, __BD 10, __BD 0), conns2);
	result.insert (QGAMES::SelectionOptionsGameState::Menus::value_type (__QGAMES_STDGUISYSTEMCONFKEYSID__, menu2));

	return (result);
}

// ---
int QGAMES::StandardInitialSelectionOptionsGameState::SelectionHandler::whenWidgetSelected (QGAMES::Widget* wdt)
{
	int result = -1; // To do nothing...

	switch (wdt -> id ())
	{
		case __QGAMES_STDGUISYSTEMSTARTGAMEWDT__:
		case __QGAMES_STDGUISYSTEMHALLOFFAMEWDT__:
		case __QGAMES_STDGUISYSTEMEXITWDT__:
			result = wdt -> id (); // Time to exit the menu...
			break;

		case __QGAMES_STDGUISYSTEMLOADCONFWDT__:
			{
				QGAMES::StandardInitialSelectionOptionsGameState* gSt = 
					dynamic_cast <QGAMES::StandardInitialSelectionOptionsGameState*> (owner ());
				assert (gSt); // Just in case...
				gSt -> _configuration -> load (gSt -> game () -> parameter (__GAME_DATADIRPROPERTYNAME__) + 
					std::string (__PATH_SEPARATOR__) + QGAMES::StandardInitialSelectionOptionsGameState::_CONFFILENAME);
				// What it was saved, has to be reloaded back into the configuration 
				// data object being managed by the state...
				owner () -> game () -> sound (__QGAMES_SAVEWAVSOUND__) -> play (__QGAMES_GAMESTATESOUNDCHANNEL__);
				result = -2; // To rebuilt the menu...
			}

			break;

		// In these cases lets analize whether there is any key duplicated
		// If it is the case, then an information message should be shown!
		case __QGAMES_STDGUISYSTEMCONFKEYLEFTWDT__:
		case __QGAMES_STDGUISYSTEMCONFKEYRIGHTWDT__:
		case __QGAMES_STDGUISYSTEMCONFKEYUPWDT__:
		case __QGAMES_STDGUISYSTEMCONFKEYDOWNWDT__:
		case __QGAMES_STDGUISYSTEMCONFKEYFIGHTWDT__:
		case __QGAMES_STDGUISYSTEMCONFKEYPAUSEWDT__:
		case __QGAMES_STDGUISYSTEMCONFKEYSAVEWDT__:
			{
				QGAMES::CompositeWidget* mWdt = 
					dynamic_cast <QGAMES::CompositeWidget*> (owner () -> currentGUISystem () -> mainWidget ());
				assert (mWdt); // It has to be...
				std::vector <int> nowKeys (7);
				nowKeys [__QGAMES_KEYLEFT__] = ((QGAMES::IteratingLabelWidget*) 
						(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYLEFTWDT__))) -> currentValue ();
				nowKeys [__QGAMES_KEYRIGHT__] = ((QGAMES::IteratingLabelWidget*) 
						(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYRIGHTWDT__))) -> currentValue ();
				nowKeys [__QGAMES_KEYUP__] = ((QGAMES::IteratingLabelWidget*) 
						(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYUPWDT__))) -> currentValue ();
				nowKeys [__QGAMES_KEYDOWN__] = ((QGAMES::IteratingLabelWidget*) 
						(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYDOWNWDT__))) -> currentValue ();
				nowKeys [__QGAMES_KEYFIRE__] = ((QGAMES::IteratingLabelWidget*) 
						(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYFIGHTWDT__))) -> currentValue ();
				nowKeys [__QGAMES_KEYPAUSE__] = ((QGAMES::IteratingLabelWidget*) 
						(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYPAUSEWDT__))) -> currentValue ();
				nowKeys [__QGAMES_KEYSAVE__] = ((QGAMES::IteratingLabelWidget*) 
						(mWdt -> widget (__QGAMES_STDGUISYSTEMCONFKEYSAVEWDT__))) -> currentValue ();

				int nE = nowKeys.size ();
				auto end = nowKeys.end ();
				for (auto it = nowKeys.begin (); it != end; ++it)
					end = std::remove (it + 1, end, *it);
				nowKeys.erase(end, nowKeys.end ());
				owner () -> showInformationText 
					(std::string ((nowKeys.size () != nE) ? "Take care! Keys repeated" : " "));
			}

			break;

		// When the rest of the widgets are selected what the standard behaviour does is more than enough
		// That's it do not anything...

		default:
			break;
	}

	return (result);
}

// ---
QGAMES::ShowingTwoStatesGameState::Separation::Separation 
		(QGAMES::Mask* msk, QGAMES::GameState* stA, QGAMES::GameState* stB,
		 const QGAMES::ShowingTwoStatesGameState::Separation::Properties& prps)
	: QGAMES::Notifier (),
	  _mask (msk),
	  _stateA (stA), _stateB (stB),
	  _properties (prps),
	  _moving (true),
	  _percentage (0),
	  _initializeStateA (true), _initializeStateB (true),
	  _finalizeStateA (true), _finalizeStateB (true),
	  _oppositeMask (NULL),
	  _counter (0)
{
	assert (_mask != NULL && _stateA != NULL);
	// The second state can be null...
	// It means that the state in the screen disappear, but it is not replaced by any other one else...

	_oppositeMask = _mask -> opposite ();
}

// ---
QGAMES::ShowingTwoStatesGameState::Separation::~Separation () 
{ 
	delete (_mask);
	_mask = NULL;
	delete (_oppositeMask);
	_oppositeMask = NULL;
}

// ---
void QGAMES::ShowingTwoStatesGameState::Separation::setPercentage (int p)
{
	_percentage = p;
	if (_percentage < 0) _percentage = 0;
	if (_percentage > 100) _percentage = 100;
	
	if (_percentage == 0)
		notify (QGAMES::Event (__QGAMES_SEPARATION0PERCENTAGE__, this));
	if (_percentage == 100)
		notify (QGAMES::Event (__QGAMES_SEPARATION100PERCENTAGE__, this));

	actualizeMaskWith (percentage ());
	delete (_oppositeMask);
	_oppositeMask = _mask -> opposite (); // Actualize the apposite...
}

// ---
void QGAMES::ShowingTwoStatesGameState::Separation::initialize ()
{
	if (_initializeStateA) _stateA -> onEnter ();
	if (_initializeStateB && _stateB != NULL) _stateB -> onEnter ();

	_percentage = 0;
	_counter = 0;
}

// ---
void QGAMES::ShowingTwoStatesGameState::Separation::update ()
{
	_stateA -> updatePositions ();
	if (_stateB != NULL) _stateB -> updatePositions ();

	if (_moving)
	{
		if (_counter++ > _properties._ticks)
		{
			_counter = 0;
			setPercentage (percentage () + _properties._increment);
		}
	}
}

// ---
void QGAMES::ShowingTwoStatesGameState::Separation::finalize ()
{
	if (_finalizeStateA) _stateA -> onExit ();
	if (_finalizeStateB && _stateB != NULL) _stateB -> onExit ();
}

// ---
void QGAMES::ShowingTwoStatesGameState::Separation::drawOn (Screen* scr)
{
	QGAMES::Screen::Buffer* bA = _mask -> bufferOver (scr);			// Gets the mask for the state 1 (just drawn)
	QGAMES::Screen::Buffer* gSBA = 
		scr -> openBuffer (QGAMES::Screen::Buffer::_ADDBLENDMODE);	// ADD Mask. Gets the buffer where the mask will act over the game state 1 (ADD)
	_stateA -> drawOn (scr);										// The active buffer ere is gSBA
	bA -> moveToBuffer (gSBA);										// The buffer with the mask is written down over the game state
	scr -> closeBuffer (gSBA);										// The render goes back to the main screen
	// It is not needed to delete the buffer because it is owned by the mask and it will be deleted with it
	// When the properties of the mask changes, the buffer will be rebuilt.

	QGAMES::Screen::Buffer* bB = _oppositeMask -> bufferOver (scr);
	QGAMES::Screen::Buffer* gSBB = 
		scr -> openBuffer (QGAMES::Screen::Buffer::_PUREBLENDMODE);	// Normal type of mask instead...
	if (_stateB != NULL) _stateB -> drawOn (scr);					// Only if there is something to draw...
	bB -> moveToBuffer (gSBB);
	scr -> closeBuffer (gSBB);

	gSBB -> moveToScreen ();										// Write down the image of the state 2 affected by its mask...
	gSBA -> moveToScreen ();										// ...and ADD the image of the state 1

	delete (gSBB);													// Finally they both are not longer needed
	delete (gSBA);
}

// ---
void QGAMES::ShowingTwoStatesGameState::CircleGrowingFromCenterSeparation::actualizeMaskWith (int p)
{
	int d = (int) ((float) std::max (QGAMES::Game::game () -> mainScreen () -> visualWidth (), 
					  QGAMES::Game::game () -> mainScreen () -> visualHeight ()) * 1.1f);
	((QGAMES::OUTMiddleCircleMask*) _mask) -> 
		setProperties (QGAMES::OUTMiddleCircleMask::Properties ((int) (d * p / 100), __QGAMES_WHITECOLOR__));
}

// ---
void QGAMES::ShowingTwoStatesGameState::CircleReducingToCenterSeparation::actualizeMaskWith (int p)
{
	int d = (int) ((float) std::max (QGAMES::Game::game () -> mainScreen () -> visualWidth (), 
					  QGAMES::Game::game () -> mainScreen () -> visualHeight ()) * 1.1f);
	((QGAMES::INMiddleCircleMask*) _mask) -> 
		setProperties (QGAMES::INMiddleCircleMask::Properties ((int) (d * (100 - p) / 100), __QGAMES_WHITECOLOR__));
}

// ---
void QGAMES::ShowingTwoStatesGameState::VerticalLineMovingFromLeftSeparation::actualizeMaskWith (int p)
{
	((QGAMES::RightSideMask*) _mask) -> 
		setProperties (QGAMES::RightSideMask::Properties (
			(int) (QGAMES::Game::game () -> mainScreen () -> visualWidth () * p / 100), __QGAMES_WHITECOLOR__));
}

// ---
void QGAMES::ShowingTwoStatesGameState::VerticalLineMovingFromRightSeparation::actualizeMaskWith (int p)
{
	((QGAMES::LeftSideMask*) _mask) -> 
		setProperties (QGAMES::LeftSideMask::Properties (
			(int) (QGAMES::Game::game () -> mainScreen () -> visualWidth () * (100 - p) / 100), __QGAMES_WHITECOLOR__));
}

// ---
void QGAMES::ShowingTwoStatesGameState::HorizontalLineMovingFromTopSeparation::actualizeMaskWith (int p)
{
	((QGAMES::DownSideMask*) _mask) -> 
		setProperties (QGAMES::DownSideMask::Properties (
			(int) (QGAMES::Game::game () -> mainScreen () -> visualWidth () * p / 100), __QGAMES_WHITECOLOR__));
}

// ---
void QGAMES::ShowingTwoStatesGameState::HorizontalLineMovingFromBottomSeparation::actualizeMaskWith (int p)
{
	((QGAMES::UpSideMask*) _mask) -> 
		setProperties (QGAMES::UpSideMask::Properties (
			(int) (QGAMES::Game::game () -> mainScreen () -> visualWidth () * (100 - p) / 100), __QGAMES_WHITECOLOR__));
}

// ---
void QGAMES::ShowingTwoStatesGameState::setSeparation (QGAMES::ShowingTwoStatesGameState::Separation* s)
{
	assert (s);

	if (_separation) 
	{
		_separation -> finalize ();
		unObserve (_separation);
	}

	delete (_separation);
	_separation = s;

	if (_separation)
	{
		observe (_separation);
		_separation -> initialize ();
	}
}

// ---
void QGAMES::ShowingTwoStatesGameState::onEnter ()
{
	QGAMES::GameState::onEnter ();

	if (_separation)
	{
		observe (_separation);
		_separation -> initialize ();
	}
}

// ---
void QGAMES::ShowingTwoStatesGameState::updatePositions ()
{
	QGAMES::GameState::updatePositions ();

	if (_separation)
		_separation -> update ();
}

// ---
void QGAMES::ShowingTwoStatesGameState::drawOn (QGAMES::Screen* scr)
{
	QGAMES::GameState::drawOn (scr);

	if (_separation)
		_separation -> drawOn (scr);
}

// ---
void QGAMES::ShowingTwoStatesGameState::onExit ()
{
	QGAMES::GameState::onExit ();

	if (_separation)
	{
		unObserve (_separation);
		_separation -> finalize ();
	}
}

// ---
QGAMES::RollingStatesGameState::SeparationState::SeparationState 
		(const std::map <std::string, std::string>& prps)
	: _typeRolling (0), 
	  _stateName (__NULL_STRING__),
	  _secondsOnScreen (__BD 0),
	  _ticks (0),
	  _increment (0)
{
	if (prps.find (__QGAMES_GAMESTATEPROPATTRTYPEROLL__) != prps.end ())
		_typeRolling = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRTYPEROLL__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRTSTATENAME__) != prps.end ()) 
		_stateName = (*prps.find (__QGAMES_GAMESTATEPROPATTRTSTATENAME__)).second;
	if (prps.find (__QGAMES_GAMESTATEPROPATTRSECONSCR__) != prps.end ())
		_secondsOnScreen = __BD (std::atof ((*prps.find (__QGAMES_GAMESTATEPROPATTRSECONSCR__)).second.c_str ()));
	if (prps.find (__QGAMES_GAMESTATEPROPATTRTTICKS__) != prps.end ())
		_ticks = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRTTICKS__)).second.c_str ());
	if (prps.find (__QGAMES_GAMESTATEPROPATTRINCREMENT__) != prps.end ())
		_increment = std::atoi ((*prps.find (__QGAMES_GAMESTATEPROPATTRINCREMENT__)).second.c_str ());

	QGAMES::RollingStatesGameState::SeparationState::SeparationState 
		(_typeRolling, _stateName, _secondsOnScreen, _ticks, _increment);
}

// ---
QGAMES::RollingStatesGameState::RollingStatesGameState (int id, 
		const QGAMES::RollingStatesGameState::SeparationStates& sS, QGAMES::GameState* nS)
	: QGAMES::ShowingTwoStatesGameState (id, NULL, nS), // The separation is initially null... (it'll be set onEnter)
	  _states (sS),
	  _currentState (0),
	  _toChangeState (false)
{
	// The list of potential states can't be empty...
	assert (!_states.empty ());
}

// ---
void QGAMES::RollingStatesGameState::onEnter ()
{
	activeSeparation (_currentState = 0);
	setStopSeparation ();

	_toChangeState = false;

	reStartAllCounters ();
	reStartAllOnOffSwitches ();
	counter (_SEPARATIONONSCREENCOUNTER) -> initialize (
		(int) (game () -> framesPerSecond () * _states [_currentState]._secondsOnScreen), 0, true, false);

	QGAMES::ShowingTwoStatesGameState::onEnter ();
}

// ---
void QGAMES::RollingStatesGameState::updatePositions ()
{
	if (onOffSwitch (_SWITCHROLLING) -> isOn ())
	{
		if (counter (_SEPARATIONONSCREENCOUNTER) -> isEnd () && !movingSeparation ())
			setMoveSeparation ();

		if (_toChangeState)
		{
			_toChangeState = false;

			activeSeparation (++_currentState);
			setStopSeparation ();

			counter (_SEPARATIONONSCREENCOUNTER) -> initialize (
				(int) (game () -> framesPerSecond () * _states [_currentState]._secondsOnScreen), 0, true, false);
		}
	}

	QGAMES::ShowingTwoStatesGameState::updatePositions ();
}

// ---
void QGAMES::RollingStatesGameState::processEvent (const Event& evnt)
{
	if (evnt.code () == __QGAMES_SEPARATION100PERCENTAGE__)
	{
		if (_currentState >= ((int) _states.size () - 2)) // The last has been reached...
		{
			setStopSeparation ();
			onOffSwitch (_SWITCHROLLING) -> set (false);
			notify (QGAMES::Event (__QGAMES_ROLLINGGAMESTATESFINISHED__, this));
		}
		else
			_toChangeState = true; // The state can't be change now, 
								   // because the process is coming in the middle of the previous state...
	}
	else
		QGAMES::ShowingTwoStatesGameState::processEvent (evnt);
}

// ---
void QGAMES::RollingStatesGameState::activeSeparation (int nS)
{
	assert (nS >= 0 && nS < (int) _states.size ());

	QGAMES::RollingStatesGameState::SeparationState sSA = _states [nS];
	QGAMES::RollingStatesGameState::SeparationState sSB; // Nothing by default...
	if (nS < ((int) _states.size () - 1)) sSB = _states [nS + 1];

	QGAMES::ShowingTwoStatesGameState::Separation* s = createSeparation (
		sSA._typeRolling,
		game () -> state (sSA._stateName), 
		(sSB._stateName == std::string (__NULL_STRING__)) ? NULL : game () -> state (sSB._stateName), 
		QGAMES::ShowingTwoStatesGameState::Separation::Properties (sSA._ticks, sSA._increment));

	s -> setPercentage (0); // Starting from the early beginning...

	if (nS == 0) s -> setInitializeStates (true, true); // If it is the first, both states must be initialized...
	else s -> setInitializeStates (false, true); // If it's not, just only the second, 
												 // because it is supposed the other was initialized in the previous iteration...
	s -> setFinalizeStates (true, false); // When finalizing is just the opposite: Only the first must be finalized...
	if (nS == ((int) _states.size () - 1)) s -> setFinalizeStates (true, true); // unless the requested state was the last one...
	
	setSeparation (s);
}

// ---
QGAMES::ShowingTwoStatesGameState::Separation* QGAMES::RollingStatesGameState::createSeparation 
	(int t, QGAMES::GameState* sA, QGAMES::GameState* sB, const QGAMES::ShowingTwoStatesGameState::Separation::Properties& prps)
{
	QGAMES::ShowingTwoStatesGameState::Separation* result = NULL;

	switch (t)
	{
		case _CIRCLEGROWINGFROMCENTER:
			result = new QGAMES::ShowingTwoStatesGameState::CircleGrowingFromCenterSeparation (sA, sB, prps); 
			break;
		case _CIRCLEDECRESINGTOCENTER:
			result = new QGAMES::ShowingTwoStatesGameState::CircleReducingToCenterSeparation (sA, sB, prps); 
			break;
		case _VERTICALLINEFROMLEFTTORIGHT:
			result = new QGAMES::ShowingTwoStatesGameState::VerticalLineMovingFromLeftSeparation (sA, sB, prps); 
			break;
		case _VERTICALLINEFROMRIGHTTOLEFT:
			result = new QGAMES::ShowingTwoStatesGameState::VerticalLineMovingFromRightSeparation (sA, sB, prps); 
			break;
		case _HORIZONTALLINEFROMTOPTODOWN:
			result = new QGAMES::ShowingTwoStatesGameState::HorizontalLineMovingFromTopSeparation (sA, sB, prps); 
			break;
		case _HORIZONTALLINEFROMDOWNTOTOP:
			result = new QGAMES::ShowingTwoStatesGameState::HorizontalLineMovingFromBottomSeparation (sA, sB, prps); 
			break;

		default:
			assert (0); // it shouldn't be here...
	}

	return (result);
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::RollingStatesGameState::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::RollingStatesGameState::_SEPARATIONONSCREENCOUNTER, 0 /* to change when initialized. */, 0, true, false));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (QGAMES::RollingStatesGameState::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch 
		(QGAMES::RollingStatesGameState::_SWITCHROLLING, true));
}

// ---
void QGAMES::AdvancedGameStateControl::stop ()
{
	if (_stopped)
		return;

	_stopped = true; 
	_pauseState -> onEnter ();
}

// ---
void QGAMES::AdvancedGameStateControl::resume ()
{
	if (!_stopped)
		return;

	_stopped = false; 
	_pauseState -> onExit ();
}

// ---
void QGAMES::AdvancedGameStateControl::setOverlay (QGAMES::GameState* oState)
{
	if (_overlayState != NULL)
		_overlayState -> onExit ();
	// Not deleted because the control is not the owner of the overlay...

	_overlayState = oState;
	if (_overlayState != NULL)
		_overlayState -> onEnter ();
}

// ---
void QGAMES::AdvancedGameStateControl::updatePositions ()
{
	QGAMES::GameStateControl::updatePositions ();

	// The overlay state (like a laker) is updated just only when it exists...
	if (_overlayState)
		_overlayState -> updatePositions ();
}

// ---
void QGAMES::AdvancedGameStateControl::drawOn (QGAMES::Screen* scr)
{
	QGAMES::GameStateControl::drawOn (scr);

	if (_stopped)
	{
		// The pause state is only actualized when the game has been stpped...
		_pauseState -> updatePositions ();

		// Always orthogonal...
		QGAMES::Camera* cC = QGAMES::Game::game () -> mainScreen () -> currentCamera ();
		QGAMES::Camera::ProjectionType tP = cC -> projectionType ();
		cC -> setProjectType (QGAMES::Camera::ProjectionType::_ORTHOGONAL);
		_pauseState -> drawOn (scr);
		cC -> setProjectType (tP);
	}

	if (_overlayState)
		_overlayState -> drawOn (scr);
}
