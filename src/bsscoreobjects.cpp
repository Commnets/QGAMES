#include <BattleshipLike/bsscoreobjects.hpp>

const int BATTLESHIP::BattleshipThingsBeingCarried::_FADELEVEL [25] =
	{ 0, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250 };

// ---
BATTLESHIP::EntityEnergy::EntityEnergy (int pId, int eF)
		: QGAMES::ScoreObject (__BATTLESHIP_SPACEELMNTENERGYSCOBJID__ + pId),
		  _energyForm (eF),
		  _percentage (0)
{
	_forms.insert (QGAMES::Forms::value_type (_energyForm, QGAMES::Game::game () -> form (_energyForm)));
	setCurrentForm (_energyForm); 
}

// ---
void BATTLESHIP::EntityEnergy::setPercentage (int p)
{
	_percentage = p;
	if (_percentage < 0) _percentage = 0;
	if (_percentage > 100) _percentage = 100;
}

// ---
void BATTLESHIP::EntityEnergy::initialize ()
{
	QGAMES::ScoreObject::initialize ();

	_percentage = 0;
}

// ---
void BATTLESHIP::EntityEnergy::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	QGAMES::Form* f = currentForm ();
	float pct = (1.0f - ((100.0f - (float) _percentage) / 100.0f));
	int pixF = (int) ((float) f -> frameWidth () * pct);
	s -> drawForm (position () + s -> position (), f, 0, 0, f -> frameWidth (), f -> frameHeight ());
	s -> drawForm (position () + s -> position (),	f, 0, f -> frameHeight (), pixF, f -> frameHeight ());
}

// ---
void BATTLESHIP::EntityEnergy::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __BATTLESHIP_SPACEELMNTENERGYACTUALIZED__)
		setPercentage ((int) evnt.values ().openValue (__BATTLESHIP_SPACEELMNTENERGYVALUEPARAMETER__).bdataValue ());
	else
		QGAMES::ScoreObject::processEvent (evnt); // To the parent...
}

// ---
BATTLESHIP::BattleshipLives::BattleshipLives (int tA, int lF)
		: QGAMES::ScoreObject (__BATTLESHIP_BATTLESHIPLIVESSCOBJID__),
		  _livesForm (lF),
		  _typeArtist (tA),
		  _lives (0)
{
	_forms.insert (QGAMES::Forms::value_type (_livesForm, QGAMES::Game::game () -> form (_livesForm)));
	setCurrentForm (_livesForm); 
}

// ---
void BATTLESHIP::BattleshipLives::setLives (int l)
{
	_lives = l;
	if (_lives < 0) _lives = 0;
	if (_lives > __BATTLESHIP_NUMBEROFLIVES__) _lives = __BATTLESHIP_NUMBEROFLIVES__;
}

// ---
void BATTLESHIP::BattleshipLives::initialize ()
{
	QGAMES::ScoreObject::initialize ();

	_lives = 0;
}

// ---
void BATTLESHIP::BattleshipLives::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Position iPos = QGAMES::Position::_cero;
	QGAMES::Frame* frame = currentForm () -> frame (_typeArtist);
	QGAMES::bdata w = __BD (frame -> originalForm () -> frameWidth () + 5);
	for (int i = 0; i < _lives; i++, iPos += QGAMES::Vector (w, __BD 0, __BD 0))
		frame -> drawOn (s, p + iPos);
}

// ---
void BATTLESHIP::BattleshipLives::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __BATTLESHIP_BATTLESHIPLIVESACTUALIZED__)
		setLives (evnt.values ().openValue (__BATTLESHIP_BATTLESHIPLIVESVALUEPARAMETER__).intValue ());
	else
		QGAMES::ScoreObject::processEvent (evnt); // To the parent...
}

// ---
BATTLESHIP::BattleshipThingBeingCarried::BattleshipThingBeingCarried (int pId, int tF)
		: QGAMES::ScoreObject (__BATTLESHIP_BATTLESHIPTHINGBEINGCARRIEDID__ + pId),
		  _thingsForm (tF),
		  _thing (-1) // Means not visible...
{
	// The form used...
	_forms.insert (QGAMES::Forms::value_type (_thingsForm, QGAMES::Game::game () -> form (_thingsForm)));
	setCurrentForm (_thingsForm); 
	setCurrentAspect (0); // The first one by default...
}

// ---
void BATTLESHIP::BattleshipThingBeingCarried::setThing (int f)
{
	assert (f == -1 || (f >= 0 && f < currentForm () -> numberFrames ()));

	_thing = f;

	setCurrentAspect (_thing == -1 ? 0 : _thing);
}

// ---
void BATTLESHIP::BattleshipThingBeingCarried::initialize ()
{
	QGAMES::ScoreObject::initialize ();

	_thing = -1;
}

// ---
void BATTLESHIP::BattleshipThingBeingCarried::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (_thing == -1 || !isVisible ())
		return; // If no form or not visible, it doesn't exist...

	QGAMES::ScoreObject::drawOn (s, p);
}

// ---
BATTLESHIP::BattleshipThingsBeingCarried::BattleshipThingsBeingCarried (int nT)
		: QGAMES::ScoreObject (__BATTLESHIP_BATTLESHIPTHINGSBEGINGCARRIEDID__),
		  _things (std::vector <BATTLESHIP::BattleshipThingBeingCarried*> (__BATTLESHIP_NUMBERTHINGSBEINGCARRIED__, NULL))
{
	assert (nT >= 0 && nT <= __BATTLESHIP_NUMBERTHINGSBEINGCARRIED__);

	for (int i = 0; i < nT; i++) // This entity is the owner of the internal elements...
		_things [i]= createThingBeingCarried (i);
}

// ---
BATTLESHIP::BattleshipThingsBeingCarried::~BattleshipThingsBeingCarried ()
{
	for (int i = 0; i < __BATTLESHIP_NUMBERTHINGSBEINGCARRIED__; delete (_things [i++]));
	_things.clear ();
}

// ---
QGAMES::Entity* BATTLESHIP::BattleshipThingsBeingCarried::clone () const
{
	BATTLESHIP::BattleshipThingsBeingCarried* result = new BATTLESHIP::BattleshipThingsBeingCarried (*this);
	for (int i = 0; i < __BATTLESHIP_NUMBERTHINGSBEINGCARRIED__; i++)
		if (_things [i] != NULL) 
			result -> _things [i] = ((BATTLESHIP::BattleshipThingBeingCarried*) _things [i] -> clone ());
	return (result);
}

// ---
void BATTLESHIP::BattleshipThingsBeingCarried::setPosition (const QGAMES::Position& p)
{
	QGAMES::ScoreObject::setPosition (p); 

	for (int i = 0; i < __BATTLESHIP_NUMBERTHINGSBEINGCARRIED__; i++)
		if (_things [i] != NULL) 
			_things [i] -> setPosition (position () + 
				QGAMES::Vector (__BD (i * __BATTLESHIP_BATTLESHIPTHINGTOCARRYPIXELSWIDE__), __BD 0, __BD 0));
	// It is located in horizontal...
}

// ---
std::vector <int> BATTLESHIP::BattleshipThingsBeingCarried::things () const
{
	std::vector <int> result;
	for (int i = 0; i < __BATTLESHIP_NUMBERTHINGSBEINGCARRIED__; i++)
		if (_things [i] != NULL) 
			result.push_back (_things [i] -> thing ());
	return (result);
}

// ---
void BATTLESHIP::BattleshipThingsBeingCarried::setThings (const std::vector <int>& t)
{
	assert ((int) t.size () <= __BATTLESHIP_NUMBERTHINGSBEINGCARRIED__);

	for (int i = 0; i < (int) t.size (); i++)
	{
		assert (_things [i]); // There can not be more things to "set" than things being currently carried...
		_things [i] -> setThing (t [i]);
	}
}

// ---
void BATTLESHIP::BattleshipThingsBeingCarried::initialize ()
{
	QGAMES::ScoreObject::initialize ();

	for (int i = 0; i < __BATTLESHIP_NUMBERTHINGSBEINGCARRIED__; i++)
		if (_things [i] != NULL) 
			_things [i] -> initialize ();

	reStartAllCounters ();
	reStartAllOnOffSwitches ();
}

// ---
void BATTLESHIP::BattleshipThingsBeingCarried::updatePositions ()
{
	if (counter (_COUNTERTOFADE) -> isEnd ()) // Time to change the fade?
	{
		if (counter (_COUNTERFADELEVEL) -> isEnd ())
		{
			onOffSwitch (_SWITCHFADERLIGHTER) -> change ();
			if (onOffSwitch (_SWITCHFADERLIGHTER) -> isOn ())
				counter (_COUNTERFADELEVEL) -> initialize (24, 10, true, false);
			else 
				counter (_COUNTERFADELEVEL) -> initialize (10, 24, false, false);
		}
	}

	QGAMES::ScoreObject::updatePositions ();
}

// ---
void BATTLESHIP::BattleshipThingsBeingCarried::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	// There is no other internal form to draw...so the parent is not needed

	QGAMES::Position iPos = (p == QGAMES::Position::_noPoint) ? _data._position : p;
	for (int i = 0; i < __BATTLESHIP_NUMBERTHINGSBEINGCARRIED__; i++)
	{
		if (_things [i] != NULL) 
		{
			if (i == (__BATTLESHIP_NUMBERTHINGSBEINGCARRIED__ - 1)) // Last one (if any) blinks...
				_things [i] -> setAlphaLevel (_FADELEVEL [counter (_COUNTERFADELEVEL) -> value ()]); 
			_things [i] -> drawOn (s, iPos);
			iPos += QGAMES::Vector (
					__BD (10 + _things [i] -> currentForm () -> frameWidthForFrame (_things [i] -> currentAspect ())), 
					__BD 0, // A little bit separated one each other...
					__BD 0);
		}
	}
}

// ---
void BATTLESHIP::BattleshipThingsBeingCarried::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __BATTLESHIP_BATTLESHIPTHINGSCARRIEDACTUALIZED__)
		setThings (QGAMES::getElementsFromAsInt (
			evnt.values ().openValue (__BATTLESHIP_BATTLESHIPTHINGSCARRIEDVALUEPARAM__).strValue (), ','));
	else
		QGAMES::ScoreObject::processEvent (evnt);
}

// ---
__IMPLEMENTCOUNTERS__ (BATTLESHIP::BattleshipThingsBeingCarried::Counters)
{
	addCounter (new QGAMES::Counter 
		(BATTLESHIP::BattleshipThingsBeingCarried::_COUNTERTOFADE, 2, 0, true, true));
	addCounter (new QGAMES::Counter 
		(BATTLESHIP::BattleshipThingsBeingCarried::_COUNTERFADELEVEL, 24, 0, true, false));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (BATTLESHIP::BattleshipThingsBeingCarried::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (BATTLESHIP::BattleshipThingsBeingCarried::_SWITCHFADERLIGHTER, true));
}

// ---
BATTLESHIP::ShootsLeftCounter::ShootsLeftCounter (int pId, int f)
	: QGAMES::ScoreObjectNumber (__BATTLESHIP_SPACEELMNTSHOOTINGSSTILLLEFTID__ + pId, 0, QGAMES::Forms (), 
		16 /** The difference between the space position and cero position. */),
	  _font (f),
	  _shootingsLeft (0) // None by default...
{
	_forms.insert (QGAMES::Forms::value_type (_font, QGAMES::Game::game () -> form (_font)));
	setCurrentForm (_font); 
	setNumberLength (3); // No more than 999...
	setSpace (0); // All digits as closer as possible...
}

// ---
void BATTLESHIP::ShootsLeftCounter::initialize ()
{
	setNumber (-1); // Means unlimited, no nothing has to be drawn...

	QGAMES::ScoreObjectNumber::initialize ();
}

// ---
void BATTLESHIP::ShootsLeftCounter::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (number () == -1)
		return; // Nothing is drawn when the number is -1 (it means infinite)

	QGAMES::ScoreObjectNumber::drawOn (s, p);
}

// ---
void BATTLESHIP::ShootsLeftCounter::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __BATTLESHIP_SPACESHIPSHOOTINGSACTUALIZED__)
		setNumber (evnt.values ().openValue (__BATTLESHIP_SPACESHIPSHOOTINGSVALUEPARAM__).intValue ());
	else
		QGAMES::ScoreObjectNumber::processEvent (evnt);
}

// ---
BATTLESHIP::TimeCounter::TimeCounter (int f)
	: QGAMES::ScoreObjectText (__BATTLESHIP_GAMETIMECOUNTERID__),
	  _font (f)
{
	_forms.insert (QGAMES::Forms::value_type (_font, QGAMES::Game::game () -> form (_font)));
	setCurrentForm (_font); 
	setMaxTextLength (5); // Minutes plus seconds....
	setSpace (0); // All together..
}

// ---
void BATTLESHIP::TimeCounter::setSeconds (int nS)
{
	std::string cero (2, '0');
	std::string nMTs = std::to_string (nS / 60);
	std::string nSTs = std::to_string (nS % 60);
	std::string tx = cero.substr (0, 2 - nMTs.length ()) + nMTs + ":" +
		cero.substr (0, 2 - nSTs.length ()) + nSTs;
	setText (tx);
}

// ---
void BATTLESHIP::TimeCounter::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __BATTLESHIP_GAMETIMEACTUALIZED__)
		setSeconds (evnt.values ().openValue (__BATTLESHIP_GAMETIMEVALUEPARAMETER__).intValue ());
	else
		QGAMES::ScoreObjectText::processEvent (evnt);
}

// ---
BATTLESHIP::ScoreCounter::ScoreCounter (int f)
	: QGAMES::ScoreObjectNumber (__BATTLESHIP_GAMESCOREID__, 0, QGAMES::Forms (), 
		16 /** The difference between the space position and cero position. */),
	  _font (f)
{
	_forms.insert (QGAMES::Forms::value_type (_font, QGAMES::Game::game () -> form (_font)));
	setCurrentForm (_font); 
	setNumberLength (5);
	setSpace (0); // All together...
}

// ---
void BATTLESHIP::ScoreCounter::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __BATTLESHIP_SPACESHIPSCOREACTUALIZED__)
		setNumber (evnt.values ().openValue (__BATTLESHIP_SPACESHIPSCOREVALUEPARAMETER__).intValue ());
	else
		QGAMES::ScoreObjectNumber::processEvent (evnt);
}

// ---
BATTLESHIP::FixText::FixText (int id, const std::string& fT, int f)
	: QGAMES::ScoreObjectText (__BATTLESHIP_GAMESCOREFIXTEXTBASEID__ + id, fT),
	  _font (f)
{
	_forms.insert (QGAMES::Forms::value_type (_font, QGAMES::Game::game () -> form (_font)));
	setCurrentForm (_font); 
	setSpace (0); // All together...

	setText (fT);
}
