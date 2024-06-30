#include "ScObjects.hpp"
#include "Game.hpp"

// ---
SabreWulfScoreFrame::SabreWulfScoreFrame ()
	: QGAMES::ScoreObject (__SABREWULFFRAMESCOREOBJ__),
	  _color (0)
{
	// This is not really needed...
	// Just to avoid an error during the standard initialization
	// It is double checked that there is a form at least...
	_forms.insert (QGAMES::Forms::value_type (__SABREWULFCOMMTY__, 
		QGAMES::Game::game () -> form (__SABREWULFCOMMTY__)));
	setCurrentForm (__SABREWULFCOMMTY__);
	setCurrentAspect (0);
}

// ---
void SabreWulfScoreFrame::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	// QGAMES::ScoreObject::drawOn (s, p);
}

// ---
SabreWulfEnergy::SabreWulfEnergy ()
		: QGAMES::ScoreObject (__SABREWULFENERGYSCOREOBJ__),
		  _percentage (100)
{
	// This is not really needed...
	// Just to avoid an error during the standard initialization
	// It is double checked that there is a form at least...
	_forms.insert (QGAMES::Forms::value_type (__SABREWULFENERGYFORM__, 
		QGAMES::Game::game () -> form (__SABREWULFENERGYFORM__)));
}

// ---
void SabreWulfEnergy::setPercentage (int p)
{
	_percentage = p;
	if (_percentage > 100) _percentage = 100;
	if (_percentage < 0) _percentage = 0;
}

// ---
void SabreWulfEnergy::initialize ()
{
	QGAMES::ScoreObject::initialize ();
	_percentage = 100;
}

// ---
void SabreWulfEnergy::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Form* f = QGAMES::Game::game () -> form (__SABREWULFENERGYFORM__);
	int posF = (int) (f -> frameWidth () * (_percentage / 100.0));
	int pixF = f -> frameWidth () - posF;
	s -> drawForm (position (), f, 0, 0, posF, f -> frameHeight (), 200);
	s -> drawForm (position () + QGAMES::Vector (__BD posF, __BD 0, __BD 0), 
		f, posF, f -> frameHeight (), pixF, f -> frameHeight (), 200);
}

// ---
SabreWulfLives::SabreWulfLives ()
		: QGAMES::ScoreObject (__SABREWULFLIVESSCOREOBJ__),
		  _lives (0)
{
	// This is not really needed...
	// Just to avoid an error during the standard initialization
	// It is double checked that there is a form at least...
	_forms.insert (QGAMES::Forms::value_type (__SABREWULFICO__, 
		QGAMES::Game::game () -> form (__SABREWULFICO__)));
}

// ---
void SabreWulfLives::setLives (int l)
{
	_lives = l;
	if (_lives > __MAXNUMBEROFLIVES__ ) _lives = __MAXNUMBEROFLIVES__;
	if (_lives < 0) _lives = 0;
}

// ---
void SabreWulfLives::initialize ()
{
	QGAMES::ScoreObject::initialize ();
	_lives = 0;
}

// ---
void SabreWulfLives::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Form* f = QGAMES::Game::game () -> form (__SABREWULFICO__);
	// The position is recalculated in base the number of lives...
	QGAMES::Position pos = position () - 
		QGAMES::Vector (__BD ((f -> frameWidth () * _lives) >> 1), __BD 0, __BD 0);
	for (int i = 0; i < _lives; i++)
		s -> drawForm (pos + 
			QGAMES::Vector (__BD (i * f -> frameWidth ()), __BD 0, __BD 0), f,
				0, 0, f -> frameWidth (), f -> frameHeight ());
}

// ---
SabreWulfTimeCounter::SabreWulfTimeCounter ()
	: QGAMES::ScoreObjectText (__SABREWULFTIMESCOREOBJ__, "", QGAMES::Forms (), 0 /* the space */)
{
	_forms.insert (QGAMES::Forms::value_type (__SABREWULFLETTERS__, 
		QGAMES::Game::game () -> form (__SABREWULFLETTERS__)));
}

// ---
void SabreWulfTimeCounter::setSeconds (int nS)
{
	int nM = (nS / 60);
	int nSS = (nS % 60);

	std::string cero (2, '0');
	std::stringstream nMT; nMT << nM; std::string nMTs = nMT.str ();
	std::stringstream nST; nST << nSS; std::string nSTs = nST.str ();

	std::string tx = cero.substr (0, 2 - nMTs.length ()) + nMTs + ":" +
		cero.substr (0, 2 - nSTs.length ()) + nSTs;
	setText (tx);
}

// ---
SabreWulfScoreCounter::SabreWulfScoreCounter ()
	: QGAMES::ScoreObjectNumber (__SABREWULFPOINTSSCOREOBJ__, 0, QGAMES::Forms (), 16 /* the 0 */)
{
	_forms.insert (QGAMES::Forms::value_type (__SABREWULFLETTERS__, 
		QGAMES::Game::game () -> form (__SABREWULFLETTERS__)));
}
