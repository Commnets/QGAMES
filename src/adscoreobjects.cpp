#include <AdvancedArcade/adscoreobjects.hpp>

// ---
QGAMES::LivesScoreObject::LivesScoreObject (int id, int nL, 
		const QGAMES::LivesScoreObject::Properties& prps, const QGAMES::Position& p)
	: QGAMES::ScoreObject (id), // With no forms by default and not data aditionally...
	  _maxNumberOfLives (nL),
	  _properties (prps),
	  _livesStillInUse (nL)
{ 
	assert (_maxNumberOfLives >= 1);

	// Forms has to exist...
	_forms.insert (QGAMES::Forms::value_type (_properties._formUp, 
		QGAMES::Game::game () -> form (_properties._formUp)));
	_forms.insert (QGAMES::Forms::value_type (_properties._formDown, 
		QGAMES::Game::game () -> form (_properties._formDown)));
	setCurrentForm (_properties._formUp); 
}

// ---
void QGAMES::LivesScoreObject::setLivesStillInUse (int nL)
{
	_livesStillInUse = nL;

	if (_livesStillInUse > _maxNumberOfLives) _livesStillInUse = _maxNumberOfLives;
	if (_livesStillInUse < 0) _livesStillInUse = 0;
}

// ---
void QGAMES::LivesScoreObject::initialize ()
{
	QGAMES::ScoreObject::initialize ();

	_livesStillInUse = _maxNumberOfLives;
}

// ---
void QGAMES::LivesScoreObject::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Position iPos = QGAMES::Position::_cero;

	QGAMES::Vector dO = QGAMES::Vector (__BD 1, __BD 0, __BD 0);
	if (_properties._orientation == QGAMES::LivesScoreObject::Properties::Orientation::_UPDOWN)
		dO = QGAMES::Vector (__BD 0, __BD 1, __BD 0);

	QGAMES::Form* frmUp = game () -> form (_properties._formUp); 
	for (int i = 0; i < _livesStillInUse; i++, iPos += dO * __BD (frmUp -> frameWidth () + _properties._pixelsSeparation))
		frmUp -> frame (0) -> drawOn (s, p + iPos);

	if (_properties._formDown != -1)
	{
		QGAMES::Form* frmDown = game () -> form (_properties._formDown); 
		for (int i = 0; i < (_maxNumberOfLives - _livesStillInUse); i++, 
				iPos += dO * __BD (frmDown -> frameWidth () + _properties._pixelsSeparation))
			frmDown -> frame (0) -> drawOn (s, p + iPos);
	}
}

// ---
QGAMES::EnergyBarScoreObject::EnergyBarScoreObject (int id, int fU, const QGAMES::Position& p)
		: QGAMES::ScoreObject (id),
		  _formUsed (fU),
		  _percentage (0)
{
	_forms.insert (QGAMES::Forms::value_type (_formUsed, QGAMES::Game::game () -> form (_formUsed)));
	setCurrentForm (_formUsed); // If it doesn't exist, the instruction will fail...

	setPosition (p);
}

// ---
void QGAMES::EnergyBarScoreObject::setPercentage (int p)
{
	_percentage = p;

	if (_percentage < 0) _percentage = 0;
	if (_percentage > 100) _percentage = 100;
}

// ---
void QGAMES::EnergyBarScoreObject::initialize ()
{
	QGAMES::ScoreObject::initialize ();

	_percentage = 100;
}

// ---
void QGAMES::EnergyBarScoreObject::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	QGAMES::Form* f = QGAMES::Game::game () -> form (_formUsed);
	float pct = (1.0f - ((100.0f - (float) _percentage) / 100.0f));
	int pixF = (int) ((float) f -> frameWidth () * pct);
	s -> drawForm (p, f, 0, 0, f -> frameWidth (), f -> frameHeight ());
	s -> drawForm (p, f, 0, f -> frameHeight (), pixF, f -> frameHeight ());
}
