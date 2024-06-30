#include <Common/background.hpp>
#include <Common/game.hpp>

// ---
QGAMES::Background::Background (int id, const Position& pos, Sprite2D* f,
	int n, const QGAMES::ObjectProperties& oP)
	: Object (id, pos, oP),
	  _background (f), _numberOfTimes (n),
	  _fadeGrade (255),
	  _automaticMov (false),
	  _movDirection (QGAMES::Vector::_cero),
	  _speed (0),
	  _pixels (0),
	  _linkedBackground (NULL),
	  _linkedSpeed (0)
{
	assert (_background);
	assert (_numberOfTimes >= 1); // 1 minumum!
}

// ---
void QGAMES::Background::setFade (int fG)
{
	assert (fG >= 0 && fG <= 255);

	_fadeGrade = fG;
}

// ---
void QGAMES::Background::setMovementData (int s, int p)
{
	assert (s >= 0 && p >= 0); // They must both be positive!!
	_speed = s; _pixels = p;
	counter (_COUNTERSPEED) -> initialize (_speed, 0);
	reStartAllCounters ();
}

// ---
void QGAMES::Background::setMove (bool s, const QGAMES::Vector& d, bool e)
{
	// The linked background can be null!, 
	// It will mean no relation between tese two backgrounds!
	_automaticMov = s; _movDirection = d;
	if (e && _linkedBackground != NULL)
		_linkedBackground -> setMove (s, 
			(_linkedBackground -> movementDirection () == QGAMES::Vector::_cero) ? QGAMES::Vector::_cero : d, e);
	reStartAllCounters ();
}

// ---
void QGAMES::Background::move ()
{
	if (_roll)
	{
		QGAMES::Position posI = position ();
		QGAMES::Position posF = posI + 
			QGAMES::Vector (__BD _background -> width (), __BD _background -> height (), __BD 0);
		QGAMES::Position nPos = posI;

		// If the background is out of the screen...
		// Then a reset is needed...
		if (posI.posX () < __BD 0 && posF.posX () < __BD 0)
			nPos += QGAMES::Vector (__BD _background -> width (), __BD 0, __BD 0);
		if (posI.posX () > __BD _background -> width () && posF.posX () > _background -> width ())
			nPos -= QGAMES::Vector (__BD _background -> width (), __BD 0, __BD 0);
		if (posI.posY () < __BD 0 && posF.posY () < __BD 0)
			nPos += QGAMES::Vector (__BD 0, __BD _background -> height (), __BD 0);
		if (posI.posY () > __BD _background -> height () && posF.posY () > _background -> height ())
			nPos -= QGAMES::Vector (__BD 0, __BD _background -> height (), __BD 0);
		setPosition (nPos);
	}

	setPosition (position () - (__BD _pixels * _movDirection));
	if (_linkedBackground != NULL) // There is something linked...
		if (counter (_COUNTERLINKEDSPEED) -> isEnd ())
			_linkedBackground -> move (); // ...moves taking into account how it is attached!
}

// ---
void QGAMES::Background::solidMove (const QGAMES::Vector& d)
{
	if (!_fixPosition)
		setPosition (position () + d);
	if (_linkedBackground != NULL)
		_linkedBackground -> solidMove (d);
}

// ---
void QGAMES::Background::setLinkedBackground (int s, QGAMES::Background* b)
{
	assert (s >= 0); // Has to be positive...but the background can be null to unlink the previous!

	_linkedBackground = b; _linkedSpeed = s;
	counter (_COUNTERLINKEDSPEED) -> initialize (_linkedSpeed, 0);
	reStartAllCounters ();
}

// ---
void QGAMES::Background::setRoll (bool r)
{
	_roll = r;
	if (_linkedBackground != NULL)
		_linkedBackground -> setRoll (r);
	reStartAllCounters ();
}

// ---
void QGAMES::Background::initialize ()
{
	// The initial values...
	int fadeGrade = 255;
	bool autMove = false;
	QGAMES::Vector movDirection (__BD 1, __BD 0, __BD 0);
	int speed = 0, pixels = 0;
	QGAMES::Background* linkedBackground = NULL;
	int linkedSpeed = 0;
	bool roll = false;
	bool fix = false;

	// Has the fadegrade property been defined?
	if (existObjProperty (__QGAMES_BKFADEGRADEATTR__))
		fadeGrade = std::stoi (objectProperty (__QGAMES_BKFADEGRADEATTR__));

	// Has the automatic movement property been defined?
	if (existObjProperty (__QGAMES_BKAUTOMATICMOVEATTR__))
		autMove = (QGAMES::toUpper (std::string (objectProperty (__QGAMES_BKAUTOMATICMOVEATTR__))) == 
			std::string (__YES_STRING__)) ? true : false ;

	// Has the movement direction property been defined?
	if (existObjProperty (__QGAMES_BKMOVDIRECTIONATTR__))
	{
		std::vector <QGAMES::bdata> cV;
		cV = QGAMES::getElementsFromAsBDATA (objectProperty (__QGAMES_BKMOVDIRECTIONATTR__), ',', 3);
		movDirection = QGAMES::Vector (__BD cV [0], cV [1], __BD cV [2]);
	}

	// Has the speed property been defined?
	if (existObjProperty (__QGAMES_BKSPEEDPROPERTYATTR__))
		speed = std::stoi (objectProperty (__QGAMES_BKSPEEDPROPERTYATTR__));
	// Has the pixels property been defined?
	if (existObjProperty (__QGAMES_BKPIXELSATTR__))
		pixels = std::stoi (objectProperty (__QGAMES_BKPIXELSATTR__));
	
	// Has the background linked been defined?
	if (existObjProperty (__QGAMES_BKLINKEDBKATTR__))
	{
		assert (game ()); // The game has to have been defined...
		linkedBackground = (QGAMES::Background*) game () -> objectBuilder () -> 
			object (std::stoi (objectProperty (__QGAMES_BKLINKEDBKATTR__)));
		linkedBackground -> initialize ();
	}
	
	// Has the speed of the background linked been defined?
	if (existObjProperty (__QGAMES_BKSPEEDLINKEDBKATTR__))
		linkedSpeed = std::stoi (objectProperty (__QGAMES_BKSPEEDLINKEDBKATTR__));

	// Has to roll?
	if (existObjProperty (__QGAMES_BKROLLATTR__))
		roll = (QGAMES::toUpper (std::string (objectProperty (__QGAMES_BKROLLATTR__))) == 
			std::string (__YES_STRING__)) ? true : false ;

	// The position is fixed?
	if (existObjProperty (__QGAME_BKFIXPOSITIONATTR__))
		fix = (QGAMES::toUpper (std::string (objectProperty (__QGAME_BKFIXPOSITIONATTR__))) == 
			std::string (__YES_STRING__)) ? true : false ;

	// The background is a mandatory attribute...
	// Sets the fadegrade
	setFade (fadeGrade);
	// Sets the movement
	setMove (autMove, movDirection);
	// Sets the movement data
	setMovementData (speed, pixels);
	// Sets the linked background if any...
	setLinkedBackground (linkedSpeed, linkedBackground);
	// Sets whether it rolls or not!
	setRoll (roll);
	// Sets whether the position is or not fix...
	setFixPosition (fix);

	// Restats the counters...
	reStartAllCounters ();

	// Initially the position of the background is to 0...
	setPosition (QGAMES::Position::_cero);
}

// ---
void QGAMES::Background::inEveryLoop ()
{
	if (_automaticMov)
		if (counter (_COUNTERSPEED) -> isEnd ())
			move ();
}
		
// ---
void QGAMES::Background::drawOn (QGAMES::Screen* s, const QGAMES::Position& p)
{
	if (_linkedBackground != NULL)
		_linkedBackground -> drawOn (s, p); // From back to front!
	
	QGAMES::Position pos = _position;
	if (_fixPosition)
		pos += s -> position (); // Restore the original position of the background...
	
	int sgnX = (_movDirection.posX () > 0) ? 1 : ((_movDirection.posX () < 0) ? -1 : 0);
	int sgnY = (_movDirection.posY () > 0) ? 1 : ((_movDirection.posY () < 0) ? -1 : 0);
	// Stupid to count additional times unnecessarily
	int cntX = (sgnX != 0) ? _numberOfTimes : 1; 
	int cntY = (sgnY != 0) ? _numberOfTimes : 1;
	
	QGAMES::Position iPos = pos;
	QGAMES::Position posIRow = iPos;

	// If the background rolls, 
	// The image should be completed around
	if (_roll && !_fixPosition)
	{
		iPos = pos - QGAMES::Vector 
			(__BD (_background -> width () * sgnX * cntX), 
			 __BD (_background -> height () * sgnY * cntY), __BD 0);
		posIRow = iPos;

		for (int i = 0; i < (cntY << 1); i++)
		{
			for (int j = 0; j < (cntX << 1); j++, 
					iPos += QGAMES::Vector (__BD _background -> width () * sgnX, __BD 0, __BD 0))
				_background -> drawOn (s, 0, iPos, _fadeGrade);

			posIRow += QGAMES::Vector (__BD 0, __BD _background -> height () * sgnY, __BD 0);
			iPos = posIRow;
		}
	}
	else
	{
		for (int i = 0; i < cntY; i++)
		{
			for (int j = 0; j < cntX; j++, 
					iPos += QGAMES::Vector (__BD _background -> width () * sgnX, __BD 0, __BD 0))
				_background -> drawOn (s, 0, iPos, _fadeGrade);

			posIRow += QGAMES::Vector (__BD 0, __BD _background -> height () * sgnY, __BD 0);
			iPos = posIRow;
		}
	}
}

// ---
void QGAMES::Background::finalize ()
{
	if (_linkedBackground != NULL)
		_linkedBackground -> finalize ();
	// If there is no linked background, by default there is nothing else to do!
}

// ---
__IMPLEMENTCOUNTERS__ (QGAMES::Background::Counters)
{
	addCounter (new QGAMES::Counter 
		(QGAMES::Background::_COUNTERSPEED, 1 /** This value is changed at initialization */, 
			0, true, true));
	addCounter (new QGAMES::Counter 
		(QGAMES::Background::_COUNTERLINKEDSPEED, 1 /** This value is changed at initializacion. */,
			0, true, true));
}
