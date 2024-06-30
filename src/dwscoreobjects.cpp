#include <DragonwindLike/dwscoreobjects.hpp>

const int DRAGONWIND::NinjaThingsBeingCarried::_FADELEVEL [25] =
	{ 0, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250 };

// ---
DRAGONWIND::EntityEnergy::EntityEnergy (int pId, int eF)
		: QGAMES::ScoreObject (__DRAGONWIND_ENTITYENERGYSCOBJID__ + pId),
		  _energyForm (eF),
		  _percentage (0)
{
	_forms.insert (QGAMES::Forms::value_type (_energyForm, QGAMES::Game::game () -> form (_energyForm)));
	setCurrentForm (_energyForm); 
}

// ---
void DRAGONWIND::EntityEnergy::setPercentage (int p)
{
	_percentage = p;
	if (_percentage < 0) _percentage = 0;
	if (_percentage > 100) _percentage = 100;
}

// ---
void DRAGONWIND::EntityEnergy::initialize ()
{
	QGAMES::ScoreObject::initialize ();

	_percentage = 0;
}

// ---
void DRAGONWIND::EntityEnergy::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
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
void DRAGONWIND::EntityEnergy::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __DRAGONWIND_ENTITYENERGYACTUALIZED__)
		setPercentage ((int) evnt.values ().openValue (__DRAGONWIND_ENTITYENERGYVALUEPARAMETER__).bdataValue ());
	else
		QGAMES::ScoreObject::processEvent (evnt); // To the parent...
}

// ---
DRAGONWIND::NinjaLives::NinjaLives (int tA, int lF)
		: QGAMES::ScoreObject (__DRAGONWIND_NINJALIVESSCOBJID__),
		  _livesForm (lF),
		  _typeArtist (tA),
		  _lives (0)
{
	_forms.insert (QGAMES::Forms::value_type (_livesForm, QGAMES::Game::game () -> form (_livesForm)));
	setCurrentForm (_livesForm); 
}

// ---
void DRAGONWIND::NinjaLives::setLives (int l)
{
	_lives = l;
	if (_lives < 0) _lives = 0;
	if (_lives > __DRAGONWIND_MAXNUMBEROFLIVES__) _lives = __DRAGONWIND_MAXNUMBEROFLIVES__;
}

// ---
void DRAGONWIND::NinjaLives::initialize ()
{
	QGAMES::ScoreObject::initialize ();

	_lives = 0;
}

// ---
void DRAGONWIND::NinjaLives::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	QGAMES::Position iPos = QGAMES::Position::_cero;
	QGAMES::Frame* frame = currentForm () -> frame (_typeArtist);
	QGAMES::bdata w = __BD (frame -> originalForm () -> frameWidth () + 5);
	for (int i = 0; i < _lives; i++, iPos += QGAMES::Vector (w, __BD 0, __BD 0))
		frame -> drawOn (s, p + iPos);
}

// ---
void DRAGONWIND::NinjaLives::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __DRAGONWIND_NINJALIVESACTUALIZED__)
		setLives (evnt.values ().openValue (__DRAGONWIND_NINJALIVESVALUEPARAMETER__).intValue ());
	else
		QGAMES::ScoreObject::processEvent (evnt); // To the parent...
}

// ---
DRAGONWIND::NinjaThingBeingCarried::NinjaThingBeingCarried (int pId, int tF)
		: QGAMES::ScoreObject (__DRAGONWIND_NINJATHINGBEINGCARRIEDID__ + pId),
		  _thingsForm (tF),
		  _thing (-1) // Means not visible...
{
	// The form used...
	_forms.insert (QGAMES::Forms::value_type (_thingsForm, QGAMES::Game::game () -> form (_thingsForm)));
	setCurrentForm (_thingsForm); 
	setCurrentAspect (0); // The first one by default...
}

// ---
void DRAGONWIND::NinjaThingBeingCarried::setThing (int f)
{
	assert (f == -1 || (f >= 0 && f < currentForm () -> numberFrames ()));

	_thing = f;

	setCurrentAspect (_thing == -1 ? 0 : _thing);
}

// ---
void DRAGONWIND::NinjaThingBeingCarried::initialize ()
{
	QGAMES::ScoreObject::initialize ();

	_thing = -1;
}

// ---
void DRAGONWIND::NinjaThingBeingCarried::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (_thing == -1 || !isVisible ())
		return; // If no form or not visible, it doesn't exist...

	QGAMES::ScoreObject::drawOn (s, p);
}

// ---
DRAGONWIND::NinjaThingsBeingCarried::NinjaThingsBeingCarried (int nT)
		: QGAMES::ScoreObject (__DRAGONWIND_NINJATHINGSBEGINGCARRIEDID__),
		  _things (std::vector <DRAGONWIND::NinjaThingBeingCarried*> (__DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__, NULL))
{
	assert (nT >= 0 && nT <= __DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__);

	for (int i = 0; i < nT; i++) // This entity is the owner of the internal elements...
		_things [i]= createThingBeingCarried (i);
}

// ---
DRAGONWIND::NinjaThingsBeingCarried::~NinjaThingsBeingCarried ()
{
	for (int i = 0; i < __DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__; delete (_things [i++]));
	_things.clear ();
}

// ---
QGAMES::Entity* DRAGONWIND::NinjaThingsBeingCarried::clone () const
{
	DRAGONWIND::NinjaThingsBeingCarried* result = new DRAGONWIND::NinjaThingsBeingCarried (*this);
	for (int i = 0; i < __DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__; i++)
		if (_things [i] != NULL) 
			result -> _things [i] = ((DRAGONWIND::NinjaThingBeingCarried*) _things [i] -> clone ());
	return (result);
}

// ---
void DRAGONWIND::NinjaThingsBeingCarried::setPosition (const QGAMES::Position& p)
{
	QGAMES::ScoreObject::setPosition (p); 

	for (int i = 0; i < __DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__; i++)
		if (_things [i] != NULL) 
			_things [i] -> setPosition (position () + 
				QGAMES::Vector (__BD (i * __DRAGONWIND_NINJATHINGTOCARRYPIXELSWIDE__), __BD 0, __BD 0));
	// It is located in horizontal...
}

// ---
std::vector <int> DRAGONWIND::NinjaThingsBeingCarried::things () const
{
	std::vector <int> result;
	for (int i = 0; i < __DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__; i++)
		if (_things [i] != NULL) 
			result.push_back (_things [i] -> thing ());
	return (result);
}

// ---
void DRAGONWIND::NinjaThingsBeingCarried::setThings (const std::vector <int>& t)
{
	assert ((int) t.size () <= __DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__);

	for (int i = 0; i < (int) t.size (); i++)
	{
		assert (_things [i]); // There can not be more things to "set" than things being currently carried...
		_things [i] -> setThing (t [i]);
	}
}

// ---
void DRAGONWIND::NinjaThingsBeingCarried::initialize ()
{
	QGAMES::ScoreObject::initialize ();

	for (int i = 0; i < __DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__; i++)
		if (_things [i] != NULL) 
			_things [i] -> initialize ();

	reStartAllCounters ();
	reStartAllOnOffSwitches ();
}

// ---
void DRAGONWIND::NinjaThingsBeingCarried::updatePositions ()
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
void DRAGONWIND::NinjaThingsBeingCarried::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	// There is no other internal form to draw...so the parent is not needed

	QGAMES::Position iPos = (p == QGAMES::Position::_noPoint) ? _data._position : p;
	for (int i = 0; i < __DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__; i++)
	{
		if (_things [i] != NULL) 
		{
			if (i == (__DRAGONWIND_MAXNUMBERTHINGSBEINGCARRIED__ - 1)) // Last one (if any) blinks...
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
void DRAGONWIND::NinjaThingsBeingCarried::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __DRAGONWIND_NINJATHINGSCARRIEDACTUALIZED__)
		setThings (QGAMES::getElementsFromAsInt (
			evnt.values ().openValue (__DRAGONWIND_NINJATHINGSCARRIEDVALUEPARAM__).strValue (), ','));
	else
		QGAMES::ScoreObject::processEvent (evnt);
}

// ---
__IMPLEMENTCOUNTERS__ (DRAGONWIND::NinjaThingsBeingCarried::Counters)
{
	addCounter (new QGAMES::Counter 
		(DRAGONWIND::NinjaThingsBeingCarried::_COUNTERTOFADE, 2, 0, true, true));
	addCounter (new QGAMES::Counter 
		(DRAGONWIND::NinjaThingsBeingCarried::_COUNTERFADELEVEL, 24, 0, true, false));
}

// ---
__IMPLEMENTONOFFSWITCHES__ (DRAGONWIND::NinjaThingsBeingCarried::OnOffSwitches)
{
	addOnOffSwitch (new QGAMES::OnOffSwitch (DRAGONWIND::NinjaThingsBeingCarried::_SWITCHFADERLIGHTER, true));
}

// ---
DRAGONWIND::ShootsLeftCounter::ShootsLeftCounter (int pId, int f)
	: QGAMES::ScoreObjectNumber (__DRAGONWIND_ENTITYSHOOTINGSSTILLLEFTID__ + pId, 0, QGAMES::Forms (), 
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
void DRAGONWIND::ShootsLeftCounter::initialize ()
{
	setNumber (-1); // Means unlimited, no nothing has to be drawn...

	QGAMES::ScoreObjectNumber::initialize ();
}

// ---
void DRAGONWIND::ShootsLeftCounter::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	if (number () == -1)
		return; // Nothing is drawn when the number is -1 (it means infinite)

	QGAMES::ScoreObjectNumber::drawOn (s, p);
}

// ---
void DRAGONWIND::ShootsLeftCounter::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __DRAGONWIND_ENTITYSHOOTINGSACTUALIZED__)
		setNumber (evnt.values ().openValue (__DRAGONWIND_ENTITYSHOOTINGSVALIEPARAM__).intValue ());
	else
		QGAMES::ScoreObjectNumber::processEvent (evnt);
}

// ---
DRAGONWIND::TimeCounter::TimeCounter (int f)
	: QGAMES::ScoreObjectText (__DRAGONWIND_GAMETIMECOUNTERID__),
	  _font (f)
{
	_forms.insert (QGAMES::Forms::value_type (_font, QGAMES::Game::game () -> form (_font)));
	setCurrentForm (_font); 
	setMaxTextLength (5); // Minutes plus seconds....
	setSpace (0); // All together..
}

// ---
void DRAGONWIND::TimeCounter::setSeconds (int nS)
{
	std::string cero (2, '0');
	std::string nMTs = std::to_string (nS / 60);
	std::string nSTs = std::to_string (nS % 60);
	std::string tx = cero.substr (0, 2 - nMTs.length ()) + nMTs + ":" +
		cero.substr (0, 2 - nSTs.length ()) + nSTs;
	setText (tx);
}

// ---
void DRAGONWIND::TimeCounter::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	QGAMES::ScoreObjectText::drawOn (s, p);
}

// ---
void DRAGONWIND::TimeCounter::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __DRAGONWIND_GAMETIMEACTUALIZED__)
		setSeconds (evnt.values ().openValue (__DRAGONWIND_GAMETIMEVALUEPARAMETER__).intValue ());
	else
		QGAMES::ScoreObjectText::processEvent (evnt);
}

// ---
DRAGONWIND::ScoreCounter::ScoreCounter (int f)
	: QGAMES::ScoreObjectNumber (__DRAGONWIND_GAMESCOREID__, 0, QGAMES::Forms (), 
		16 /** The difference between the space position and cero position. */),
	  _font (f)
{
	_forms.insert (QGAMES::Forms::value_type (_font, QGAMES::Game::game () -> form (_font)));
	setCurrentForm (_font); 
	setNumberLength (5);
	setSpace (0); // All together...
}

// ---
void DRAGONWIND::ScoreCounter::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	QGAMES::ScoreObjectNumber::drawOn (s, p);
}

// ---
void DRAGONWIND::ScoreCounter::processEvent (const QGAMES::Event& evnt)
{
	if (evnt.code () == __DRAGONWIND_GAMESCOREACTUALIZED__)
		setNumber (evnt.values ().openValue (__DRAGONWIND_GAMESCOREVALUEPARAMETER__).intValue ());
	else
		QGAMES::ScoreObjectNumber::processEvent (evnt);
}

// ---
DRAGONWIND::FixText::FixText (int id, const std::string& fT, int f)
	: QGAMES::ScoreObjectText (__DRAGONWIND_GAMESCOREFIXTEXTBASEID__ + id, fT),
	  _font (f)
{
	_forms.insert (QGAMES::Forms::value_type (_font, QGAMES::Game::game () -> form (_font)));
	setCurrentForm (_font); 
	setSpace (0); // All together...

	setText (fT);
}

// ---
void DRAGONWIND::FixText::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (!isVisible ())
		return;

	QGAMES::ScoreObjectText::drawOn (s, p);
}
